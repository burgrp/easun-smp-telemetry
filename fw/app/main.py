import sys
sys.path.append('dep')

import mqtt_call
import site_config

print('EASUN SMP Telemetry')
print('Inverter index:', site_config.inverter_index)

class Handler:
    def add(self, a, b):
        return a+b

    async def div(self, a, b):
        return a/b


server = mqtt_call.Server(
    handler=Handler(),
    name="solar-inverter-{}".format(site_config.inverter_index),
    wifi_ssid=site_config.wifi_ssid,
    wifi_password=site_config.wifi_password,
    mqtt_broker=site_config.mqtt_broker,
    debug=False
)

server.dump()
server.start()
