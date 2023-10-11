import mqtt_call
from machine import UART
from crc_half import crc_half

import sys
sys.path.append('/')
import site_config
import device_config

print('EASUN SMP Telemetry')
print('Inverter index:', device_config.inverter_index)

CR = 0x0D

class Handler:

    uart = None

    def __init__(self):
        self.uart = UART(1, baudrate=2400, tx=21, rx=20, timeout=5000, timeout_char=5000)


    def export_message(self, request):

        while self.uart.any() > 1:
            self.uart.read(1)

        requestData = bytearray(request, "utf-8")
        crcHigh, crcLow = crc_half(requestData)
        requestData.append(crcHigh)
        requestData.append(crcLow)
        requestData.append(CR)

        self.uart.write(requestData)

        line = bytearray()

        while True:
            char = self.uart.read(1);

            if char == None:
                break

            char = char[0]
            if char == CR:
                break

            line.append(char)

        return line[:-2].decode()

    def export_get_status(self):
        return {
            "rssi": server.mqtt_client._sta_if.status('rssi'),
            "ip": server.mqtt_client._sta_if.ifconfig()[0]
        }


server = mqtt_call.Server(
    handler=Handler(),
    name="solar-inverter-{}".format(device_config.inverter_index),
    wifi_ssid=site_config.wifi_ssid,
    wifi_password=site_config.wifi_password,
    mqtt_broker=site_config.mqtt_broker,
    ledPin=4,
    debug=device_config.debug
)

server.dump()
server.start()
