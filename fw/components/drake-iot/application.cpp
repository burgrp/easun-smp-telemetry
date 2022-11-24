#include "drake-iot.hpp"
#include "esp_system.h"

namespace drake::iot {

void Application::start() {

  try {
    ESP_LOGI(TAG, "Starting application...");

    nvs.init();

#ifdef CONFIG_DRAKE_IOT_HANDLE_WIFI
    wifi.start();
#endif

    ESP_LOGI(TAG, "Application started.");

  } catch (exception *e) {
    const char *msg = e->what();
    ESP_LOGE(TAG, "Unhandled exception on application start: %s", msg);
    esp_system_abort(msg);
  }
}

} // namespace drake::iot