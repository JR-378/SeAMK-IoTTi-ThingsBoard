# SeAMK-IoTTi-ThingsBoard

## Introduction
This project uses TAMK's Sensor Board, which consists of Arduino Mega 2560, Arduino Ethernet Shield V1, Dallas temperature sensor and an LCD screen. The data is sent through MQTT connection to ThingsBoard's live demo server.

At a later date, I will upload my other set of code, where I will be using sockets to send the data to an Ubuntu virtual machine, where a Thingsboard service is running on localhost. It is programmed with Python. However, due to its complexity, its documentation is going to take even longer!

## Quick instructions - Live Demo

1. Add your live demo device authentication token to username variable under ThingsBoard login details in Arduino_Mega-Thingsboard-MQTT.ino
2. Upload the Arduino_Mega-Thingsboard-MQTT.ino to your board
3. Check that the data is sent to the live demo, at your device's LATEST TELEMETRY tab

A more thorough documentation will be in the Docs folder, once it is ready.
