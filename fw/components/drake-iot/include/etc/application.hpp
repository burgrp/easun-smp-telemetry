class Application {
public:
  NVS nvs;
  Console console;
  WiFi wifi;
  WiFiConnectCommand wifiCommand;

  Application() {}

  void start() {

    try {
      ESP_LOGI(TAG, "Starting application...");

      nvs.init();
      console.start();
      wifi.start();

      wifiCommand.attach(console);

      ESP_LOGI(TAG, "Application started.");

    } catch (exception *e) {
      const char *msg = e->what();
      ESP_LOGE(TAG, "Unhandled exception on application start: %s", msg);
      esp_system_abort(msg);
    }
  }
};
