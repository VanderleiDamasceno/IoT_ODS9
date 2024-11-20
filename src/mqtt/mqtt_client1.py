import asyncio
from gmqtt import Client as MQTTClient

# Callback for handling messages
async def on_message(client, topic, payload, qos, properties):
    print(f"Received message: {payload.decode()} on topic: {topic}")

# Configure the client
async def main():
    client = MQTTClient("broker")

    # Set the message callback
    client.on_message = on_message

    # Connect to the Mosquitto MQTT broker on port 1883
    await client.connect("127.0.0.1", port=1883)

    # Subscribe to a test topic
    client.subscribe("cli/topic")

    # Publish a message to test
    client.publish("test/topic", "Hello from gmqtt 1!")

    # Run forever
    await asyncio.Future()  # Keeps the script running

if __name__ == "__main__":
    asyncio.run(main())
