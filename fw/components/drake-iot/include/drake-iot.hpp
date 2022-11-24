#ifndef DRAKE_IOT_HPP
#define DRAKE_IOT_HPP 1

#include "esp_check.h"
#include "esp_log.h"

#include <stdexcept>
#include <string>

using namespace std;

namespace drake::iot {

#define TAG "drake-iot"

class Application;

class WiFi {
public:
  Application *application;
  WiFi(Application *application) : application(application) {}
  void start();
  void setSsidPassword(const string &ssid, const string &password);
};

class NVS {
public:
  Application *application;
  NVS(Application *application) : application(application) {}
  void init();
};

class Console {
public:
  Application *application;
  Console(Application *application) : application(application) {}
  void start();
};

class Application {
public:
  NVS nvs;
  Console console;
#ifdef CONFIG_DRAKE_IOT_HANDLE_WIFI
  WiFi wifi;

#endif

  Application(): nvs(this), console(this), wifi(this) {}
  void start();
};

class EspError : public exception {
public:
  esp_err_t code;
  EspError(esp_err_t code) : code(code) {}

  virtual const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW {
    return esp_err_to_name(code);
  }
};

#define ESP_THROW(x)                                                           \
  do {                                                                         \
    esp_err_t err_rc_ = (x);                                                   \
    if (unlikely(err_rc_ != ESP_OK)) {                                         \
      throw new EspError(err_rc_);                                             \
    }                                                                          \
  } while (0)

} // namespace drake::iot

#endif