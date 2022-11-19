#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_event.h"
#include "esp_netif.h"
#include "esp_system.h"
#include "esp_wifi.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>

static const char *TAG = "application";

#define PORT 8361

static void registersTask(void *pvParameters) {
  char rx_buffer[128];
  char addr_str[128];
  int ip_protocol = 0;
  
  while (1) {

    struct sockaddr_in dest_addr_ip;
    dest_addr_ip.sin_addr.s_addr = htonl(INADDR_ANY);
    dest_addr_ip.sin_family = AF_INET;
    dest_addr_ip.sin_port = htons(PORT);
    ip_protocol = IPPROTO_IP;

    int sock = socket(AF_INET, SOCK_DGRAM, ip_protocol);
    if (sock < 0) {
      ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
      break;
    }
    ESP_LOGI(TAG, "Socket created");

    // Set timeout
    struct timeval timeout;
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof timeout);

    int err = bind(sock, (struct sockaddr *)&dest_addr_ip, sizeof(dest_addr_ip));
    if (err < 0) {
      ESP_LOGE(TAG, "Socket unable to bind: errno %d", errno);
    }
    ESP_LOGI(TAG, "Socket bound, port %d", PORT);

    struct sockaddr_storage source_addr; // Large enough for both IPv4 or IPv6
    socklen_t socklen = sizeof(source_addr);

    while (1) {
      ESP_LOGI(TAG, "Waiting for data");
      int len = recvfrom(sock, rx_buffer, sizeof(rx_buffer) - 1, 0,
                         (struct sockaddr *)&source_addr, &socklen);
      // Error occurred during receiving
      if (len < 0) {
        ESP_LOGE(TAG, "recvfrom failed: errno %d", errno);
        break;
      }
      // Data received
      else {
        // Get the sender's ip address as string
        if (source_addr.ss_family == PF_INET) {
          inet_ntoa_r(((struct sockaddr_in *)&source_addr)->sin_addr, addr_str,
                      sizeof(addr_str) - 1);
        } 

        rx_buffer[len] =
            0; // Null-terminate whatever we received and treat like a string...
        ESP_LOGI(TAG, "Received %d bytes from %s:", len, addr_str);
        ESP_LOGI(TAG, "%s", rx_buffer);

        int err = sendto(sock, rx_buffer, len, 0,
                         (struct sockaddr *)&source_addr, sizeof(source_addr));
        if (err < 0) {
          ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
          break;
        }
      }
    }

    if (sock != -1) {
      ESP_LOGE(TAG, "Shutting down socket and restarting...");
      shutdown(sock, 0);
      close(sock);
    }
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

//#include "sdkconfig.h"
// #include "driver/gpio.h"
// const gpio_num_t LED_GPIO = gpio_num_t(CONFIG_LED_GPIO);
//     gpio_reset_pin(LED_GPIO);
//     gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);
//     gpio_set_level(LED_GPIO, 1);
