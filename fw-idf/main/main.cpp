#include "drake-iot.hpp"

using namespace drake::iot;

Application app;

extern "C" {
void app_main(void) {
  app.start();
}
}
