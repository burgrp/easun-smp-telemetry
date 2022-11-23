#include "drake-iot.hpp"

namespace drake::iot {

void panic(exception *e) {
  ESP_LOGE(TAG, "Panic: %s", e->what());
  abort();
}

}