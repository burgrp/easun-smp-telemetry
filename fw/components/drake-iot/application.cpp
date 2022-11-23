#include "drake-iot.hpp"

namespace drake::iot {

void Application::start() {

  try {
    ESP_LOGI(TAG, "Starting %s", name.c_str());

#ifdef CONFIG_DRAKE_IOT_HANDLE_WIFI
    wifi.start();
#endif

  } catch (runtime_error *e) {
    panic(e);
  }
}

}