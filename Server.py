from paho.mqtt import client as mqtt_client
import time

# MQTT broker details
broker = 'broker.hivemq.com'
port = 1883
topic_sub = "Wio-5620"
topic_pub = "FIMA"

# Generate client ID with pub prefix randomly
client_id = 'mqttx_a20215620'
username = 'your username'
password = 'your password'
deviceId = "your deviceId"

# Variable to store received message
received_message = ""


def connect_mqtt():
    def on_connect(client, userdata, flags, rc):
        if rc == 0:
            print("Successfully connected to MQTT broker")
        else:
            print(f"Failed to connect, return code {rc}")

    client = mqtt_client.Client(client_id)
    client.username_pw_set(username, password)
    client.on_connect = on_connect
    client.connect(broker, port)
    return client



def subscribe(client: mqtt_client):
    def on_message(client, userdata, msg):
        global received_message
        received_message = msg.payload.decode()


    client.subscribe(topic_sub)
    client.on_message = on_message


def publish(client: mqtt_client, message):
    result = client.publish(topic_pub, message)
    if result.rc == mqtt_client.MQTT_ERR_SUCCESS:
        print(f"{topic_pub}: {message}")
    else:
        print(f"{topic_pub}: Failed to publish message to topic ")


def main():
    client = connect_mqtt()


    while True:
        subscribe(client)
        # Publish the received message (including Temperature and Humidity value and related warning)
        publish(client, received_message+"\n\n[Inventory Low: Apple][Expire Recent: Egg, Fish]")
        # Publish the inventory warning and expired warning
        #publish(client, "[Inventory Low: Apple][Expire Recent: Egg, Fish]")
        client.loop()

        # Delay
        time.sleep(1)


if __name__ == '__main__':
    main()
