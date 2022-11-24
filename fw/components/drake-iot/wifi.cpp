#include "drake-iot.hpp"
#include "esp_wifi.h"

using namespace drake::iot;

static void wifiEventHandler(void *arg, esp_event_base_t event_base,
                             int32_t event_id, void *event_data) {
  try {
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
      ESP_LOGI(TAG, "Connecting WiFi...");
      ESP_THROW(esp_wifi_connect());
    } else if (event_base == WIFI_EVENT &&
               event_id == WIFI_EVENT_STA_DISCONNECTED) {
      ESP_LOGI(TAG, "WiFi disconnected. Trying to reconnect...");
      ESP_THROW(esp_wifi_connect());
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
      ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
      ESP_LOGI(TAG, "Got IP address:" IPSTR, IP2STR(&event->ip_info.ip));
    }
  } catch (exception *e) {
    ESP_LOGE(TAG, "Exception in wifiEventHandler: %s", e->what());
  }
}

void WiFi::start() {
  ESP_LOGI(TAG, "Starting WiFi...");

  ESP_THROW(esp_netif_init());

  ESP_THROW(esp_event_loop_create_default());
  esp_netif_create_default_wifi_sta();

  wifi_init_config_t initConfig = WIFI_INIT_CONFIG_DEFAULT();
  ESP_THROW(esp_wifi_init(&initConfig));

  ESP_THROW(esp_wifi_set_mode(WIFI_MODE_STA));

  esp_event_handler_instance_t instanceAnyId;
  ESP_THROW(esp_event_handler_instance_register(
      WIFI_EVENT, ESP_EVENT_ANY_ID, &wifiEventHandler, NULL, &instanceAnyId));

  esp_event_handler_instance_t instanceGotIp;
  ESP_THROW(esp_event_handler_instance_register(
      IP_EVENT, ESP_EVENT_ANY_ID, &wifiEventHandler, NULL, &instanceGotIp));
  
  ESP_THROW(esp_wifi_start());

  ESP_LOGI(TAG, "WiFi started.");
}

void WiFi::setSsidPassword(const string &ssid, const string &password) {
  wifi_config_t staConfig;
  staConfig.sta
      .ssid[ssid.copy((char *)staConfig.sta.ssid, sizeof(staConfig.sta.ssid))] =
      0;
  staConfig.sta.password[password.copy((char *)staConfig.sta.password,
                                       sizeof(staConfig.sta.password))] = 0;
  ESP_LOGI(TAG, "Wifi SSID/password configured as: '%s', '%s'",
           staConfig.sta.ssid, staConfig.sta.password);
  ESP_THROW(esp_wifi_set_config(WIFI_IF_STA, &staConfig));
}