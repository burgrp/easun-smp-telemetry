import sys
sys.path.append('dep')

import site_config
print('EASUN SMP Telemetry')
print('Inverter index:', site_config.inverter_index)

from mqtt_as import MQTTClient, config
import uasyncio as asyncio

# Local configuration
config['ssid'] = site_config.wifi_ssid  # Optional on ESP8266
config['wifi_pw'] = site_config.wifi_password
config['server'] = site_config.mqtt_broker  # Change to suit e.g. 'iot.eclipse.org'

async def messages(client):  # Respond to incoming messages
    async for topic, msg, retained in client.queue:
        print((topic, msg, retained))

async def up(client):  # Respond to connectivity being (re)established
    while True:
        await client.up.wait()  # Wait on an Event
        client.up.clear()
        await client.subscribe('foo_topic', 1)  # renew subscriptions

async def main(client):
    await client.connect()
    for coroutine in (up, messages):
        asyncio.create_task(coroutine(client))
    n = 0
    while True:
        await asyncio.sleep(5)
        print('publish', n)
        # If WiFi is down the following will pause for the duration.
        await client.publish('result', '{}'.format(n), qos = 1)
        n += 1

config["queue_len"] = 1  # Use event interface with default queue size
MQTTClient.DEBUG = True  # Optional: print diagnostic messages
client = MQTTClient(config)
try:
    asyncio.run(main(client))
finally:
    client.close()  # Prevent LmacRxBlk:1 errors

