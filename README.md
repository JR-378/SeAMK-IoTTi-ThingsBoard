# SeAMK-IoTTi-ThingsBoard

## Introduction
This project uses TAMK's Sensor Board, which consists of Arduino Mega 2560, Arduino Ethernet Shield V1, Dallas temperature sensor and an LCD screen. The data is sent through MQTT connection to ThingsBoard's live demo server.

There is also a socket version, that can be used to send live data to ThingsBoard service, that is running on a virtual installation of Ubuntu server.

A more thorough documentation will be in the Docs folder, once it is ready.

## Quick instructions - Live Demo

1. Add your live demo device authentication token to username variable under ThingsBoard login details in ThingsBoard-MQTT.ino
```C
const char* username = "XXXXXXXXXXXXXXXXXX";   // Authentication token here
```
2. Upload the Thingsboard-MQTT.ino to your board
3. Check that the data is sent to the live demo, at your device's LATEST TELEMETRY tab at http://demo.thingsboard.io

Note that with the same setup, this code can be universally used to upload to any public or accessible ThingsBoard service. For that, both the server address and username has to be changed

## Quick instructions - Socket

If you are not satisfied with only knowing that the ThingsBoard service is up and running in your virtual server, this can be used to test it out. Another alternative is to use a Bridged connection.

### SocketServer_ReceiveSend.py

1. Edit the SocketServer_ReceiveSend socket details to match the network details of your computer that is running the virtual machine.
```Python
SOCKET_HOST = "XXX.XX.XXX.XX" # Use your computer's IP (one hosting the Virtual Machine)
SOCKET_PORT = 30010           # Use this port or choose another one that is free.
```
2. Then change ThingsBoard host name to match the IP where it is hosted. Also change the username to match your access token.
```Python
# MQTT - ThingsBoard
thingsboardHost = "XXX.XXX.XX.XXX"  # Address where your ThingsBoard is hosted (127.0.0.1 => default(i.e. localhost))
                                    # To change host IP: sudo nano /etc/thingsboard/conf/thingsboard.yml
                                    # Change IP to your computer's IP. Remember to change the IP of MQTT Broker and use 
                                    # sudo reboot to save everything once you are finished.
clientID = "TAMK Sensor Board"      # Client ID, can be anything.
username = "XXXXXXXXXXXXXXXXXXXX"   # Device's authentication token.
```
3. Run the program on your computer using VS Code or in command prompt:
```
python3 SocketServer_ReceiveSend.py
```

### ThingsBoard-Socket.ino
1. Edit socket login details to match your computer.
```C
// ******************************* Socket login details ********************************
const char* server = "XXX.XX.XXX.XX"; // The socket IP of your computer
float port = 30010; // Port that you use for the socket
// *************************************************************************************
```
2. Upload the code to your board using Arduino IDE


### Check the data

1. Use your favorite browser and navigate to Things.Board.IP.address:ThingsBoardPort (e.g. 192.55.55.55:8080)
2. Login to ThingsBoard
3. Check the device's LATEST TELEMETRY tab.
