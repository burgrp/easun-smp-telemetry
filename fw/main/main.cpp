#include "drake-iot.hpp"

using namespace drake::iot;

extern "C" {
void app_main(void) {

  Application app;

  app.start();
}
}
