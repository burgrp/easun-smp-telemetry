#ifndef DRAKE_IOT_HPP
#define DRAKE_IOT_HPP 1

#include "esp_check.h"
#include "esp_log.h"
#include "esp_system.h"

#include <stdexcept>
#include <string>
#include <vector>
#include <array>
#include <map>

using namespace std;

namespace drake::iot {

#define TAG "drake-iot"

class Application;

#include "etc/esp-error.hpp"
#include "etc/wifi.hpp"
#include "etc/nvs.hpp"
#include "etc/console.hpp"
#include "etc/cmd-wifi.hpp"
#include "etc/application.hpp"

#endif

} // namespace drake::iot
