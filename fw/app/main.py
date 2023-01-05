import mqtt_call
from machine import UART

import sys
sys.path.append('/')
import site_config

print('EASUN SMP Telemetry')
print('Inverter index:', site_config.inverter_index)

END_OF_LINE = '\n'

class Handler:

    uart = None

    def __init__(self):
        self.uart = UART(1, baudrate=2400, tx=21, rx=20, timeout=5000, timeout_char=5000)


    def export_message(self, text):
        while self.uart.any() > 1:
            self.uart.read(1)

        self.uart.write(text + END_OF_LINE)

        line = self.uart.readline();
        if line != None:
            line = line.decode()

        return (line or "").strip()

    def export_get_status(self):
        return {
            "rssi": server.mqtt_client._sta_if.status('rssi'),
            "ip": server.mqtt_client._sta_if.ifconfig()[0]
        }


server = mqtt_call.Server(
    handler=Handler(),
    name="solar-inverter-{}".format(site_config.inverter_index),
    wifi_ssid=site_config.wifi_ssid,
    wifi_password=site_config.wifi_password,
    mqtt_broker=site_config.mqtt_broker,
    ledPin=4,
    debug=site_config.debug
)

server.dump()
server.start()
