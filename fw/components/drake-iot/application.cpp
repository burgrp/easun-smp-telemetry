#include "drake-iot.hpp"
#include "esp_log.h"

void drake::iot::Application::start() {

  try {
    ESP_LOGI(TAG, "Starting %s", name.c_str());

#ifdef CONFIG_DRAKE_IOT_HANDLE_WIFI
    wifi.start();
#endif

  } catch (runtime_error *e) {
    ESP_LOGE(TAG, "Unhandled exception: %s", e->what());
    abort();
  }
}