#include "nvs.h"
#include "nvs_flash.h"

class NVS {
public:
  void init() {
    ESP_LOGI(TAG, "Initializing NVS...");

    esp_err_t errorCode = nvs_flash_init();
    if (errorCode == ESP_ERR_NVS_NO_FREE_PAGES ||
        errorCode == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_LOGI(TAG, "Erasing NVS flash.");
      ESP_THROW(nvs_flash_erase());
      errorCode = nvs_flash_init();
    }
    ESP_THROW(errorCode);
  }
};
