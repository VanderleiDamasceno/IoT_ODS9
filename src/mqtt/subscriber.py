import threading
import queue
import streamlit as st
import pandas as pd
import matplotlib.pyplot as plt
from datetime import datetime
import paho.mqtt.client as mqtt
import warnings
import logging

# Setup logging
logging.basicConfig(level=logging.INFO)

# Suppress specific Matplotlib warnings if they persist
warnings.filterwarnings("ignore", message="Unable to import Axes3D.*")

# Streamlit configuration
st.set_page_config(page_title="Real-Time Humidity Graph", layout="wide")
st.title("Real-Time Humidity Graph")

# Initialize session state
if "data" not in st.session_state:
    st.session_state.data = {
        "time": [],
        "humidity": [],
        "is_watering": []
    }

# Initialize a Queue for thread-safe communication
data_queue = queue.Queue()

# Function to handle incoming MQTT messages
def on_message(client, userdata, msg):
    payload = msg.payload.decode()
    try:
        time_str, humidity_str, is_watering = payload.split(",")

        # Convert full datetime to time only (H:M:S)
        try:
            time_obj = datetime.strptime(time_str, "%Y-%m-%d %H:%M:%S")
            formatted_time = time_obj.strftime("%H:%M:%S")
        except ValueError:
            formatted_time = time_str  # Use as-is if parsing fails

        humidity = int(humidity_str)
        is_watering_bool = is_watering == "1"

        # Put the parsed data into the queue
        data_queue.put({
            "time": formatted_time,
            "humidity": humidity,
            "is_watering": is_watering_bool
        })
    except Exception as e:
        # Log the error instead of using st.error
        logging.error(f"Error parsing message: {e}")

# Function to initialize and run the MQTT client
def mqtt_thread():
    client = mqtt.Client()

    # Assign the updated on_message callback
    client.on_message = on_message

    try:
        client.connect("127.0.0.1", 1883, 60)
        logging.info("Connected to MQTT broker.")
    except Exception as e:
        logging.error(f"Failed to connect to MQTT broker: {e}")
        return

    client.subscribe("graph/data")
    client.loop_forever()

# Start MQTT client in a background thread
if "mqtt_client_initialized" not in st.session_state:
    thread = threading.Thread(target=mqtt_thread, daemon=True)
    thread.start()
    st.session_state.mqtt_client_initialized = True
    logging.info("MQTT thread started.")

# Publisher MQTT client for sending control messages
def get_publisher_client():
    if "publisher_client" not in st.session_state:
        publisher = mqtt.Client()
        try:
            publisher.connect("127.0.0.1", 1883, 60)
            publisher.loop_start()
            st.session_state.publisher_client = publisher
            logging.info("Publisher client connected.")
        except Exception as e:
            logging.error(f"Failed to connect publisher client: {e}")
            st.session_state.publisher_client = None
    return st.session_state.publisher_client

publisher_client = get_publisher_client()

# Buttons for control
if publisher_client:
    if st.button("Start"):
        publisher_client.publish("contrl/watering", "start")
        logging.info("Published 'start' to contrl/watering.")

    if st.button("Stop"):
        publisher_client.publish("contrl/watering", "stop")
        logging.info("Published 'stop' to contrl/watering.")
else:
    st.error("Publisher client not connected.")

# Function to update the graph
def update_graph():
    if not st.session_state.data["time"]:
        st.write("No data to display yet.")
        return

    data_df = pd.DataFrame(st.session_state.data)
    fig, ax = plt.subplots(figsize=(12, 3)) 

    # Draw line segments based on the watering status
    for i in range(1, len(data_df)):
        color = "blue" if data_df["is_watering"].iloc[i-1] and data_df["is_watering"].iloc[i] else "red"
        ax.plot(data_df["time"].iloc[i-1:i+1], data_df["humidity"].iloc[i-1:i+1], color=color, linewidth=1)

    # Draw individual points with colors based on is_watering status
    colors = ["blue" if is_watering else "red" for is_watering in data_df["is_watering"]]
    ax.scatter(data_df["time"], data_df["humidity"], c=colors, marker="o", zorder=5)
    
    # Set labels and title
    ax.set_xlabel("Time")
    ax.set_ylabel("Humidity")
    ax.set_title("Real-Time Humidity Data")
    
    # Limit x-axis labels
    num_labels = 6
    if len(data_df) > num_labels:
        step = max(1, len(data_df) // num_labels)
        ax.set_xticks(data_df["time"][::step])
    else:
        ax.set_xticks(data_df["time"])
    ax.tick_params(axis='x', rotation=45)
    
    # Adjust layout to prevent clipping of tick-labels
    plt.tight_layout()

    # Display the updated graph
    st.pyplot(fig)
    plt.close(fig)

# Process incoming data from the queue
while not data_queue.empty():
    new_data = data_queue.get()
    st.session_state.data["time"].append(new_data["time"])
    st.session_state.data["humidity"].append(new_data["humidity"])
    st.session_state.data["is_watering"].append(new_data["is_watering"])
    
    # Limit data size
    if len(st.session_state.data["time"]) > 150:
        st.session_state.data["time"].pop(0)
        st.session_state.data["humidity"].pop(0)
        st.session_state.data["is_watering"].pop(0)

# Initialize a placeholder for the graph
graph_placeholder = st.empty()

# Update the graph
update_graph()
