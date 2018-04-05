// This code is used by Arduino Mega that is connected to Arduino Ethernet Shield V1,
// Dallas temperature sensor and LCD screen. The data is send to ThingsBoard through
// MQTT connection. 

// Note: Due to security reasons, username (i.e authentication token) is replaced with
// XXXXXXXXXXXXXXXXXX.

#define DEBUG // Enable debug mode, comment it out to disable it.

// Specifying debug utilities
#ifdef DEBUG // If DEBUG is defined all the DEBUG_PRINT commands will be executed in the following way
  #define DEBUG_PRINT(...) Serial.print(__VA_ARGS__)     // ... -> tells that one or more arguments must be passed - __VA_ARGS__ -> variadic macros 
  #define DEBUG_PRINTLN(...) Serial.println(__VA_ARGS__) // To put it simply, it allows you to pass any argument to serial.print, which is what we want.
#else // If DEBUG is not defined, make the following macros do nothing. Now we don't need to waste computing power for Serial.print commands
  #define DEBUG_PRINT(...)
  #define DEBUG_PRINTLN(...)
#endif

// Libraries
#include <LiquidCrystal.h>      // LCD
#include <OneWire.h>            // OneWire
#include <DallasTemperature.h>  // Dallas temperature sensor
#include <ArduinoJson.h>        // For making JSON documents
#include <Ethernet.h>           // For establishing Internet connection
#include <PubSubClient.h>       // For establishing MQTT connection

EthernetClient ethernetClient;
PubSubClient mqttClient(ethernetClient);

// Update these with values suitable for your network.
byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED }; // MAC address to connect as
//IPAddress ip(192, 168, 1, 38); // IP adress to connect as. Note: IP is not required, as you can get it from DHCP server.
                                 // However, if you want to use static IP, feel free to do so.

// ****************************** TAMK VM Log in details ******************************
const char* server = "demo.thingsboard.io";
float port = 1883;
 
const char* client_id = "TAMK Sensor Board";          // Can be anything
const char* username = "XXXXXXXXXXXXXXXXXX";  // Authentication token here

const char* topicToPublish_DATA = "v1/devices/me/telemetry"; // Topic address to publish to for sending data. 
const char* topicToPublish_ATTRIBUTES = "v1/devices/me/attributes"; // Topic address to publish to for sending attributes. 
// *************************************************************************************

#define ONE_WIRE_BUS_1 40          // Data wire is plugged into port 2 on the Arduino
OneWire oneWire_1(ONE_WIRE_BUS_1); // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)

DallasTemperature sensors_1(&oneWire_1); // Pass our oneWire reference to Dallas Temperature. 

//                RS  E   D4  D5  D6  D7
LiquidCrystal lcd(37, 36, 35, 34, 33, 32);

// Data variables
float voltageV1,     // Used to store voltage V1
      voltageV2,     // Used to store voltage V2
      temperature1;  // Used to store temperature
char JSON_Data[100]; // Used to store the generated data in JSON

// Variables for counting time.
unsigned long previousMillis = 0; // Last time updated
long interval = 60000;            // Interval at which to publish data (60 sec)

void setup()
{
  #ifdef DEBUG
    Serial.begin(9600);  // Open serial communications.
    while (!Serial) {
      ; // Wait for serial port to connect.
    }
  #endif
  sensors_1.begin();   // Start Dallas temperature sensor
  lcd.begin(20, 4);    // Set up the LCD's number of columns: 20 and rows: 4
  Ethernet.begin(mac); // Start the Internet connection with only mac and thus get IP from DHCP. Note: With IP -> Ethernet.begin(mac, ip);
  delay(1500);         // Allow the hardware to sort itself out
  mqttClient.setServer(server, port); // Configure the server address and port.
}

void loop()
{
  unsigned long currentMillis = millis(); // Store the time since the board started
  
  // If not connected to MQTT, we will reconnect.
  if (!mqttClient.connected()) { 
    reconnect();
  }

  // When the time set to interval has passed, it will execute a command (publishing data to ThingsBoard)
  if(currentMillis - previousMillis > interval) {
    previousMillis = currentMillis; // reset the previous millis, so that it will continue to publish data.
    workflow(); // Workflow to use data
  }
  
  mqttClient.loop(); // Called to maintain connection to server
}

// Connect and reconnect MQTT
void reconnect() {
  // Loop until we're reconnected
  while (!mqttClient.connected()) {
    DEBUG_PRINT("Attempting MQTT connection...");
    // Attempt to connect
    if (mqttClient.connect(client_id, username, NULL)) { // password null, because authentication token is put into username
      DEBUG_PRINTLN("Connected!");
      workflow(); // publish data, once connected
    } else {
      DEBUG_PRINT("Failed, result code=");// If you get a result code, refer to their docs at https://pubsubclient.knolleary.net/api.html#state
      DEBUG_PRINT(mqttClient.state());    // Prints the current result code for easier troubleshooting
      DEBUG_PRINTLN(" try again in 5 seconds");
      delay(5000);  // Wait 5 seconds before retrying
    }
  }
}

// Prints the information on LCD
void print_measurement(void)
{
  lcd.setCursor(0,0);
  lcd.print("SeAMK IoTTi");
  
  lcd.setCursor(0,1);
  lcd.print("Voltage V1 =");
  lcd.setCursor(13,1); lcd.print(voltageV1,2);lcd.print(" V");
  
  lcd.setCursor(0,2);
  lcd.print("Voltage V2 =");
  
  lcd.setCursor(13,2); lcd.print(voltageV2,2);lcd.print(" V");
  
  lcd.setCursor(0,3);
  lcd.print("Temperature =");
  lcd.setCursor(13,3); lcd.print(temperature1,2);lcd.print(" C");
}

// Measures all the values available to us (at least those that I know of)
void measure(void)
{
  sensors_1.requestTemperatures(); // Get temperature from Dallas sensor
  
  voltageV1 = analogRead(A0);       // Get the voltage on analog pin
  voltageV1 = voltageV1*10.0/1024;
  
  voltageV2 = analogRead(A1);      // Get the voltage on analog pin
  voltageV2 = voltageV2*10.0/1024; 
    
  temperature1=sensors_1.getTempCByIndex(0); // Save the temperature value on global variable
}

// Send data with MQTT to IBM Cloud IoT platform
void send_data(void)
{
  create_JSON_Data(); // Set up the data to be published
  mqttClient.publish(topicToPublish_DATA, JSON_Data); // Publish JSON data to ThingsBoard
}

// A human and Arduino friendly way to create JSON documents in Arduino
void create_JSON_Data(void)
{
  StaticJsonBuffer<200> JSON_Buffer; // Allocate JSON buffer with 200-byte pool
  JsonObject& JSON_Object = JSON_Buffer.createObject(); // Create JSON object (i.e. document)
  
  // Now populate the JSON document with data
  JSON_Object["Temperature"] = temperature1; // Create JSON object named "Temperature", assigned with our temperature data
  JSON_Object["Voltage1"] = voltageV1;       // Create JSON object named "Voltage1", assigned with our voltage data
  JSON_Object["Voltage2"] = voltageV2;       // Create JSON object named "Voltage2", assigned with our voltage data
  
  JSON_Object.printTo(JSON_Data); // Store the data on global variable

  DEBUG_PRINTLN("\nSending data");
  DEBUG_PRINTLN("This is the data I am sending:");
  #ifdef DEBUG
    JSON_Object.prettyPrintTo(Serial); // Prints the data to serial in a human-friendly format
  #endif
}

// The order of methods to handle data
void workflow(void)
{
  measure();           // First measure and save the data
  print_measurement(); // Print them on the LCD screen
  send_data();         // Publish data to IBM IoT
}
