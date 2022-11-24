#ifndef DRAKE_IOT_HPP
#define DRAKE_IOT_HPP 1

#include "esp_check.h"
#include "esp_log.h"
#include "esp_system.h"

#include <stdexcept>
#include <string>

using namespace std;

namespace drake::iot {

#define TAG "drake-iot"

class Application;

#include "drake-iot-esp-error.hpp"
#include "drake-iot-wifi.hpp"
#include "drake-iot-nvs.hpp"
#include "drake-iot-console.hpp"
#include "drake-iot-application.hpp"

#endif

} // namespace drake::iot
