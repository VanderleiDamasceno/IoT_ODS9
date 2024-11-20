import asyncio
from gmqtt import Client as MQTTClient

# Configure the client
async def main():
    client = MQTTClient("publisher")

    # Connect to the Mosquitto MQTT broker on port 1883
    await client.connect("192.168.15.7", port=1883)

    # Publish a message to test
    client.publish("test/topic", "Hello from the Publisher!")

    # Disconnect after publishing
    await asyncio.sleep(1)  # Give time for the message to send
    await client.disconnect()

if __name__ == "__main__":
    asyncio.run(main())
