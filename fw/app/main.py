import esp_wifi
import config

print('EASUN SMP Telemetry')
print('Inverter index:', config.inverter_index)

esp_wifi.init(ssid = config.wifi_ssid, password=config.wifi_password, ledPin = 4, ledLogic = True)

