import asyncio
import streamlit as st
import pandas as pd
import matplotlib.pyplot as plt
from datetime import datetime
from gmqtt import Client as MQTTClient

# Data storage for the graph, now including "is_watering" status
data = {
    "time": [],
    "humidity": [],
    "is_watering": []
}

# Streamlit configuration
st.set_page_config(page_title="Real-Time Humidity Graph", layout="wide")
st.title("Real-Time Humidity Graph")

# Initialize the plot with Streamlit
graph_placeholder = st.empty()
data_df = pd.DataFrame(data)

# Callback for handling messages
async def on_message(client, topic, payload, qos, properties):
    msg = payload.decode()
    print(f"Received message: {msg} on topic: {topic}")
    if topic == "graph/data":
        handle_new_data(msg)
        update_graph()

# MQTT client setup and main loop
async def mqtt_main():
    client = MQTTClient("subscriber")
    client.on_message = on_message
    await client.connect("127.0.0.1", port=1883)
    client.subscribe("graph/data")
    await asyncio.Future()  # Keeps the script running

# Function to handle incoming data
def handle_new_data(msg):
    time_str, humidity_str, is_watering = msg.split(",")
    
    # Convert full datetime to time only (H:M:S)
    try:
        time_obj = datetime.strptime(time_str, "%Y-%m-%d %H:%M:%S")  # Adjust format if needed
        formatted_time = time_obj.strftime("%H:%M:%S")
    except ValueError:
        formatted_time = time_str  # Use as-is if parsing fails

    humidity = int(humidity_str)
    data["time"].append(formatted_time)
    data["humidity"].append(humidity)
    data["is_watering"].append(is_watering == "1")  # Store as boolean

    # Keep data frame size manageable
    if len(data["time"]) > 150:  # Limit to last 150 data points
        data["time"].pop(0)
        data["humidity"].pop(0)
        data["is_watering"].pop(0)

# Function to update the Streamlit graph
def update_graph():
    data_df = pd.DataFrame(data)
    fig, ax = plt.subplots(figsize=(12, 3)) 

    # Plot each line segment between points with the color based on the watering status
    for i in range(1, len(data_df)):
        # Determine the color of the line segment based on consecutive points
        color = "blue" if data["is_watering"][i-1] and data["is_watering"][i] else "red"
        ax.plot(data_df["time"][i-1:i+1], data_df["humidity"][i-1:i+1], color=color, linewidth=1)

    # Set colors for the points based on is_watering status
    colors = ["blue" if is_watering else "red" for is_watering in data["is_watering"]]

    # Plot individual points with different colors on top of the lines
    ax.scatter(data_df["time"], data_df["humidity"], c=colors, marker="o", zorder=5)
    
    # Set labels and title
    ax.set_xlabel("Time")
    ax.set_ylabel("Humidity")
    ax.set_title("Real-Time Humidity Data")
    
    # Limit the number of x-axis labels
    num_labels = 6  # Adjust as needed
    ax.set_xticks(data_df["time"][::max(1, len(data_df) // num_labels)])

    # Rotate x-axis labels
    ax.tick_params(axis='x', rotation=45)
    
    # Display the graph in Streamlit
    graph_placeholder.pyplot(fig)
    
    # Close the figure to free up memory
    plt.close(fig)

# Run Streamlit app with asyncio event loop for MQTT
async def main():
    asyncio.create_task(mqtt_main())  # Start MQTT in the background
    while True:
        await asyncio.sleep(1)  # Keep the Streamlit app running

if __name__ == "__main__":
    asyncio.run(main())
