#include "esp_console.h"

class StdOut {
public:
  virtual void println(string &line) {}
};

class Command {
public:
  virtual void run(vector<string> &args) {}
};

class Console {
public:
  void start() {
    ESP_LOGI(TAG, "Starting console...");

    esp_console_repl_t *repl = NULL;
    esp_console_repl_config_t repl_config = ESP_CONSOLE_REPL_CONFIG_DEFAULT();
    repl_config.prompt = ">";

    esp_console_register_help_command();

    esp_console_dev_usb_serial_jtag_config_t hw_config =
        ESP_CONSOLE_DEV_USB_SERIAL_JTAG_CONFIG_DEFAULT();

    ESP_THROW(
        esp_console_new_repl_usb_serial_jtag(&hw_config, &repl_config, &repl));

    ESP_THROW(esp_console_start_repl(repl));
  }
};
