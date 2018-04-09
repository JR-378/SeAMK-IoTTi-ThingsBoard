# SeAMK-IoTTi-ThingsBoard

## Introduction
This project uses TAMK's Sensor Board, which consists of Arduino Mega 2560, Arduino Ethernet Shield V1, Dallas temperature sensor and an LCD screen. The data is sent through MQTT connection to ThingsBoard's live demo server.

There is also a socket version, that can be used to send live data to ThingsBoard service, that is running on a virtual installation of Ubuntu server.

A more thorough documentation will be in the Docs folder, once it is ready.

## Quick instructions - Live Demo

1. Add your live demo device authentication token to username variable under ThingsBoard login details in Thingsboard-MQTT.ino
```C
const char* username = "XXXXXXXXXXXXXXXXXX";   // Authentication token here
```
2. Upload the Thingsboard-MQTT.ino to your board
3. Check that the data is sent to the live demo, at your device's LATEST TELEMETRY tab at http://demo.thingsboard.io

## Quick instructions - Sockets

If you are not satisfied with only knowing that the ThingsBoard service is up and running in your virtual server, this can be used to test it out.

### SocketServer_ThingsBoard-Host.py

1. Edit the SocketServer_ThingsBoard-Host socket details to match your virtual machine running the ThingsBoard service.
```Python
SOCKET_HOST = "192.168.56.102" # Use the IP provided by Oracle VM VirtualBox Host-only Adapter (enp0s8)
                               # Setup: In the virtual PC's network settings, make sure the Adapter 1 is 
                               # attached to NAT, and Adapter 2 is attached to Host-only Adapter. Reboot 
                               # if you changed this while it was running. Check the ip with ifconfig in 
                               # terminal of the server.
SOCKET_PORT = 30020            # Use this port or choose another one that is free.
```
2. Then change the username to match your access token. If you are not running on localhost (it runs there on default), you need to  edit the ThingsBoard host as well.
```Python
# MQTT - ThingsBoard
thingsboardHost = "127.0.0.1"   # Address where your ThingsBoard is hosted (127.0.0.1 => localhost)
clientID = "TAMK Sensor Board"  # Client ID, can be anything.
username = "XXXXXXXXXXXXXXXXX"  # Device's authentication token.
```
3. Upload the file to your Linux server with FTP or SSL (or if you are using GUI, you could use cloud storage (e.g. Dropbox, Google Drive, OneDrive, MEGA) to download the file)
4. Run the program on your Linux Server:
```
python3 SocketServer_ThingsBoard-Host.py
```
### SocketServer_VirtualPC-Host.py

1. Edit the SocketServer_VirtualPC-Host socket details to match the network details of your computer that is running the virtual machine.
```Python
SOCKET_HOST = "XXX.XX.XXX.XX" # Use your computer's IP.
SOCKET_PORT = 30010           # Use this port or choose another one that is free.
```
2. Edit the Linux socket details in send_data method
```Python
SOCKET_LINUX_HOST = "192.168.56.102" # Linux Server socket - use the ip in your code that runs on Linux!
SOCKET_LINUX_PORT = 30020            # Socket port - use the port in your code that runs on Linux!
```
3. Run the program on your computer using VS Code or in command prompt:
```
python3 SocketServer_VirtualPC-Host.py
```

### ThingsBoard-Socket.ino
1. Edit socket login details
```C
// ******************************* Socket login details ********************************
const char* server = "XXX.XX.XXX.XX"; // The socket IP of your computer
float port = 30010; // Port that you use for the socket
// *************************************************************************************
```
2. Upload the code to your board using Arduino IDE
