#include "drake-iot.hpp"

using namespace drake::iot;

extern "C" {
void app_main(void) {
  
  Application app = {
    .name = "EASUN SMP telemetry",
    .wifi = WiFi {
    }
  };

  app.start();

}
}
