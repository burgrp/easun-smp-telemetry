#ifndef DRAKE_IOT_HPP
#define DRAKE_IOT_HPP 1

#include <string>
#include <stdexcept>

using namespace std;

namespace drake::iot {

#define TAG "drake-iot"

class WiFi {
public:
    void start();
};

class Application {
public:

    string name;

    #ifdef CONFIG_DRAKE_IOT_HANDLE_WIFI
    WiFi wifi;
    #endif

    void start();
};


}

#endif