import esp_wifi
import wifi_config

print('EASUN SMP Telemetry')

esp_wifi.init(ssid = wifi_config.ssid, password=wifi_config.password, ledPin = 4, ledLogic = True)

