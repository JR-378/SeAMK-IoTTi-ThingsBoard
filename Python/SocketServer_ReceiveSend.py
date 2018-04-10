""" Creates a socket and waits a client to connnect to it and send data. It fowards the data to ThingsBoard
service that is running on a IP address, using MQTT. """
import socket
import paho.mqtt.client as mqtt

SOCKET_HOST = "192.84.177.52" # Use your computer's IP (one hosting the Virtual Machine)
SOCKET_PORT = 30010           # Use this port or choose another one that is free.

# MQTT - ThingsBoard
thingsboardHost = "XXX.XXX.XX.XXX"  # Address where your ThingsBoard is hosted (127.0.0.1 => default(i.e. localhost))
                                    # To change host IP: sudo nano /etc/thingsboard/conf/thingsboard.yml
                                    # Change IP to your computer's IP. Remember to change the IP of MQTT Broker and use 
                                    # sudo reboot to save everything once you are finished.
clientID = "TAMK Sensor Board"      # Client ID, can be anything.
username = "XXXXXXXXXXXXXXXXXXXX"   # Device's authentication token.

topicToPublish_DATA = "v1/devices/me/telemetry" # Topic to publish data to
#topicToPublish_ATTRIBUTES = "v1/devices/me/attributes" # Topic to publish attribute data to

def receive_Data(socket_connection):
    """ To receive data from client. """
    socket_data = socket_connection.recv(2048) # receive up to buffersize bytes from the socket
    socket_data = socket_data.decode('utf-8') # decode the bytes into string
    return socket_data

def waitForConnection(socket_server):
    """ Used to wait for a client to connect. """
    socket_connection, socket_address = socket_server.accept() # wait for an incoming connection
    print("Connected with " + socket_address[0] + ":" + str(socket_address[1]))
    return socket_connection

def main():
    """ Creates a socket that waits for a client to connect. The client sends data and the socket
    forwards it to an MQTT Broker. """

    socket_server = socket.socket(socket.AF_INET, socket.SOCK_STREAM) # Create a socket server.
    socket_server.bind((SOCKET_HOST, SOCKET_PORT)) # Bind the socket to a local address.
    socket_server.listen(5) # Enable the server to accept connections.
    print(" * Running on http://" + SOCKET_HOST + ":" + str(SOCKET_PORT))



    MQTT_client = mqtt.Client(clientID)   # Create MQTT client with the client ID
    MQTT_client.username_pw_set(username) # Assign username to MQTT client
    while True:
        try:
            MQTT_client.connect(thingsboardHost) # Connect to MQTT Broker
            print(" * Succesfully connected to MQTT Broker at: " + thingsboardHost)
        except:
            print("Unable to connect to MQTT Broker")
            break
        socket_connection = waitForConnection(socket_server)
        socket_data = receive_Data(socket_connection)
        if len(socket_data) > 0:
            print("-->", socket_data)
            try:
                MQTT_client.publish(topicToPublish_DATA, socket_data) # Publish data to MQTT Broker
                print("* Succesfully sent data to topic: " + topicToPublish_DATA)
            except:
                print("Failed to send data to MQTT Broker")
        MQTT_client.loop() # Maintain connection MQTT Broker

if __name__ == "__main__":
    main()
