#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_event.h"
#include "esp_netif.h"
#include "esp_system.h"
#include "esp_wifi.h"

#include "lwip/api.h"
#include "lwip/arch.h"
#include "lwip/err.h"
#include "lwip/ip_addr.h"
#include "lwip/netbuf.h"
#include "lwip/opt.h"
#include "lwip/sys.h"

static const char *TAG = "application";

#define PORT 8361

static void registersTask(void *pvParameters) {

  struct netbuf *buff;

  struct netconn *nc = netconn_new(NETCONN_UDP);

  netconn_bind(nc, IP4_ADDR_ANY, PORT);

  for (;;) {
    netconn_recv(nc, &buff);
    ESP_LOGI(TAG, "RCV %d %d", buff->p->len, buff->p->tot_len);
    netconn_send(nc, buff);
    netbuf_delete(buff);
  }

  vTaskDelete(NULL);
}

static void handleWifiStaStart(void *arg, esp_event_base_t event_base,
                               int32_t event_id, void *event_data) {
  ESP_LOGI(TAG, "WiFi STA started, connecting...");
  esp_wifi_connect();
}

static void handleWifiStaDisconnected(void *arg, esp_event_base_t event_base,
                                      int32_t event_id, void *event_data) {
  ESP_LOGI(TAG, "WiFi disconnected, reconnecting...");
  esp_wifi_connect();
}

static void handleIpEventStaGotIp(void *arg, esp_event_base_t event_base,
                                  int32_t event_id, void *event_data) {
  ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
  ESP_LOGI(TAG, "Got IP:" IPSTR, IP2STR(&event->ip_info.ip));
}

void app_main(void) {

  ESP_ERROR_CHECK(esp_netif_init());
  ESP_ERROR_CHECK(esp_event_loop_create_default());
  esp_netif_create_default_wifi_sta();

  wifi_init_config_t wifi_cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&wifi_cfg));

  esp_event_handler_instance_t event_instance_wifi_sta_start;
  ESP_ERROR_CHECK(esp_event_handler_instance_register(
      WIFI_EVENT, WIFI_EVENT_STA_START, &handleWifiStaStart, NULL,
      &event_instance_wifi_sta_start));

  esp_event_handler_instance_t event_instance_wifi_sta_disconnected;
  ESP_ERROR_CHECK(esp_event_handler_instance_register(
      WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &handleWifiStaDisconnected, NULL,
      &event_instance_wifi_sta_disconnected));

  esp_event_handler_instance_t event_instance_ip_sta_got_ip;
  ESP_ERROR_CHECK(esp_event_handler_instance_register(
      IP_EVENT, IP_EVENT_STA_GOT_IP, &handleIpEventStaGotIp, NULL,
      &event_instance_ip_sta_got_ip));

  wifi_config_t wifi_config = {
      .sta =
          {
              .ssid = "416T",
              .password = "ferdamravenec",
              .threshold.authmode = WIFI_AUTH_OPEN,
              .sae_pwe_h2e = WPA3_SAE_PWE_BOTH,
          },
  };
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
  ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
  ESP_ERROR_CHECK(esp_wifi_start());

  ESP_LOGI(TAG, "Application started");

  xTaskCreate(registersTask, "registers", 4096, NULL, 5, NULL);

  vTaskDelete(NULL);
}
