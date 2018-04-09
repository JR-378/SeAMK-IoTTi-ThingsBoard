# Arduino

libraries.zip contains the required libraries for the code. Unzip it with 7-Zip or Winrar and Copy and paste the folders inside into the Arduino libraries folder, usually located in C:\Users\YOUR-USERNAME\Documents\Arduino\libraries

For information on installing libraries, see: http://www.arduino.cc/en/Guide/Libraries

If you have a Chinese copy of Arduino Mega 2560, Driver.zip contains the USB drivers for the PC.

## Thingsboard-MQTT.ino

Is used to connect to a ThingsBoard Live Demo MQTT Broker, using TAMK's sensor board. Only the following part of code needs to be changed.

```C
const char* username = "XXXXXXXXXXXXXXXXXX";   // Authentication token here
```

## ThingsBoard-Socket.ino

Is used to connect to a socket, using TAMK's sensor board. Only the following part of code needs to be changed.

```C
// ******************************* Socket login details ********************************
const char* server = "XXX.XX.XXX.XX"; // The socket IP of your computer
float port = 30010; // Port that you use for the socket
// *************************************************************************************
```

## I don't have Dallas temperature sensor, nor an LCD screen

Remove methods:

```C
void print_measurement(void)
void measure(void)
```

Remember to remove the methods from `void workflow(void)` as well, and create a JSON document with hard-coded data.