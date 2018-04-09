// This code is used by Arduino Mega that is connected to Arduino Ethernet Shield V1,
// Dallas temperature sensor and LCD screen. The data is send to a socket.

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

EthernetClient ethernetClient;

// Update these with values suitable for your network.
byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED }; // MAC address to connect as
//IPAddress ip(192, 168, 1, 38); // IP adress to connect as. Note: IP is not required, as you can get it from DHCP server.
                                 // However, if you want to use static IP, feel free to do so.

// ******************************* Socket login details ********************************
const char* server = "XXX.XX.XXX.XX"; // The socket IP of your computer
float port = 30010; // Port that you use for the socket
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
  Ethernet.begin(mac); // Start the internet connection with only mac and thus get IP from DHCP. Note: With IP -> Ethernet.begin(mac, ip);
  delay(1500);         // Allow the hardware to sort itself out
  ethernetClient.connect(server, port); // Configure the server adress and port.
}

void loop()
{
  // Connect and send data to socket.
  if (!ethernetClient.connected()) { 
    connect_to_socket();
  }
  
  ethernetClient.stop(); // Stops the connection
  delay(5000);           // Wait five seconds
}

// Connect and send data
void connect_to_socket() {
  // Loop until we're reconnected
  while (!ethernetClient.connected()) {
    DEBUG_PRINT("Attempting connection...");
    // Attempt to connect
    int i = ethernetClient.connect(server, 30010);
    if (i == 1) {
      DEBUG_PRINTLN("Connected!");
      workflow(); // Workflow to measure, print and send data.
    } else {
      DEBUG_PRINT("Connection failed, code: ");
      DEBUG_PRINT(i);
      /* Code meanings:
       * SUCCESS 1
       * FAILED 0
       * TIMED_OUT -1
       * INVALID_SERVER -2
       * TRUNCATED -3
       * INVALID_RESPONSE -4
       */
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

// Send data to socket
void send_data(void)
{
  createJSON(); // Set up the data to be published
  ethernetClient.print(JSON_Data); // Publish JSON data to IBM IoT
}

// A human and Arduino friendly way to create JSON documents in Arduino
void createJSON(void)
{
  StaticJsonBuffer<200> JSON_Buffer; // Allocate JSON buffer with 200-byte pool
  JsonObject& JSON_Object = JSON_Buffer.createObject(); // Create JSON object (i.e. document)
  
  // Now populate the JSON document with data. Note: Only send data in this format. ThingsBoard doesn't like arrays or nested objects
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
  measure();           // First measure and save the data,
  print_measurement(); // print them on the LCD screen.
  send_data();         // Send data to a socket.
}
