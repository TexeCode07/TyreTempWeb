
#include <Arduino.h>
#include <ArduinoJson.h>

#include <FS.h>
#include <LittleFS.h> // For ESP8266 use <LittleFS.h>, for ESP32 use <SPIFFS.h>

#include <Preferences.h>
Preferences preferences;
float temp_target_gas, temp_target_tr, value_tg1, value_tg2, value_tg3, value_tg4, value_p1, value_p2;
float irt1, irt2, irt3, temp;

#ifdef ESP32
  #include <WiFi.h>
  #include <AsyncTCP.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
AsyncWebServer server(80);
AsyncEventSource events("/events");     // Create an Event Source on /events

// REPLACE WITH YOUR NETWORK CREDENTIALS
const char* ssid      = "onebroadband";
const char* password  = "12345678";
// IPAddress ip(192, 168, 98, 18);      // Set your desired fixed IP address
// IPAddress gateway(192, 168, 98, 1);  // Set your gateway IP address
// IPAddress subnet(255, 255, 255, 0);  // Set your subnet mask

// Send Sensor Values
unsigned long previousMillis  = 0;      // Stores last time the sensor data was sent
const long interval           = 20000;  // Interval at which to send sensor data (milliseconds = 20 seconds)

// Timer for retry connection
unsigned long previousMillis2 = 0;
unsigned long interval2       = 5000;

#include "NTP.h"
WiFiUDP wifiUdp;
NTP ntp(wifiUdp);


#include "index.h"
String htmlContent;

#include "teststring.h"

#include <Wire.h>               // I2C library, required for MLX90614
#include <SparkFunMLX90614.h>   // Click here to get the library: http://librarymanager/All#Qwiic_IR_Thermometer by SparkFun
IRTherm therm1,therm2,therm3;   // Create an IRTherm object to interact with throughout

#include "DHT.h"
int SENSOR_DHT = 4;             
# define SensorTyp DHT22        // DHT22
DHT dht(SENSOR_DHT, SensorTyp); // Sensor einen Namen zuweisen



void setupRead(){
    // Initialize Preferences
    preferences.begin("appConfig", false);                           // "appConfig" is the namespace. false indicates read/write mode.
    // Attempt to read stored values
    temp_target_gas = preferences.getFloat("temp_target_gas", 20.0); 
    temp_target_tr  = preferences.getFloat("temp_target_tr" , 20.0); 
    value_tg1       = preferences.getFloat("value_tg1" , 1.50); 
    value_tg2       = preferences.getFloat("value_tg2" , 1.50); 
    value_tg3       = preferences.getFloat("value_tg3" , 1.50); 
    value_tg4       = preferences.getFloat("value_tg4" , 1.50); 
    value_p1        = preferences.getFloat("value_p1"  , 1.80); 
    value_p2        = preferences.getFloat("value_p2"  , 1.75); 

    // Check and print values - here we are just printing, assuming the values could be defaults or saved ones.
    Serial.println("--------------------------------------------------------------------------");
    Serial.println("Setup Configuration:");
    Serial.print("Temp Target Gas: ");    Serial.print(temp_target_gas);
    Serial.print("\tTemp Target TR: ");   Serial.println(temp_target_tr);
    Serial.print("Value TG1: ");          Serial.print(value_tg1);
    Serial.print("\t\tValue TG2: ");        Serial.print(value_tg2);
    Serial.print("\t\tValue TG3: ");        Serial.print(value_tg3);
    Serial.print("\t\tValue TG4: ");        Serial.println(value_tg4);
    Serial.print("Value P1: ");           Serial.print(value_p1);
    Serial.print("\t\tValue P2: ");       Serial.println(value_p2);
    Serial.println("--------------------------------------------------------------------------");
}

void calcRead(){
    if(!LittleFS.exists("/CalcData.txt")) {             
        Serial.println("/CalcData.txt file not found");
    }else{
        Serial.println("--------------------------------------------------------------------------");
        Serial.println("Read data on /CalcData.txt file.");
        File file = LittleFS.open("/CalcData.txt", "r");    
        if (!file) {
            Serial.println("Failed to open /CalcData.txt file for reading");
            return;
        }

        String fileContent;
        while(file.available()){
            fileContent += file.readStringUntil('\n') + '\n';
        }
        file.close();
        Serial.println("/CalcData.txt");
        Serial.println(fileContent);
        Serial.println("--------------------------------------------------------------------------");
    }
}

void dataRead(){
    if(!LittleFS.exists("/sensor_data.txt")) {              // For ESP32, replace LittleFS with SPIFFS
        Serial.println("/sensor_data.txt file not found");
    }else{
        Serial.println("--------------------------------------------------------------------------");
        Serial.println("Read data on /sensor_data.txt file.");
        File file = LittleFS.open("/sensor_data.txt", "r");
        if (!file) {
            Serial.println("Failed to open /sensor_data.txt file for reading");
            return;
        }
        String dataContent="[";
        while(file.available()){
            dataContent += file.readStringUntil('\n') + '\n';
        }
        if (dataContent.length() > 0) {
            dataContent.remove(dataContent.length() - 1);
        }
        dataContent+="]";
        // Serial.println("size:");
        file.close();
        Serial.println("/Sensor_data.txt");
        Serial.println(dataContent);
        Serial.println("--------------------------------------------------------------------------");
    }
}


void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

void handleUpdate(AsyncWebServerRequest *request) {
  if (request->method() == HTTP_POST) {

      preferences.begin("appConfig", false);

      // Temp Target Gas
      if (request->hasParam("temp_target_gas", true)) {
          temp_target_gas = request->getParam("temp_target_gas", true)->value().toFloat();
          preferences.putFloat("temp_target_gas", temp_target_gas);
      }
      // Temp Target TR
      if (request->hasParam("temp_target_tr", true)) {
          temp_target_tr = request->getParam("temp_target_tr", true)->value().toFloat();
          preferences.putFloat("temp_target_tr", temp_target_tr);
      }
      // Value TG1
      if (request->hasParam("value_tg1", true)) {
          value_tg1 = request->getParam("value_tg1", true)->value().toFloat();
          preferences.putFloat("value_tg1", value_tg1);
      }
      // Value TG2
      if (request->hasParam("value_tg2", true)) {
          value_tg2 = request->getParam("value_tg2", true)->value().toFloat();
          preferences.putFloat("value_tg2", value_tg2);
      }
      // Value TG3
      if (request->hasParam("value_tg3", true)) {
          value_tg3 = request->getParam("value_tg3", true)->value().toFloat();
          preferences.putFloat("value_tg3", value_tg3);
      }
      // Value TG4
      if (request->hasParam("value_tg4", true)) {
          value_tg4 = request->getParam("value_tg4", true)->value().toFloat();
          preferences.putFloat("value_tg4", value_tg4);
      }
      // Value P1
      if (request->hasParam("value_p1", true)) {
          value_p1 = request->getParam("value_p1", true)->value().toFloat();
          preferences.putFloat("value_p1", value_p1);
      }
      // Value P2
      if (request->hasParam("value_p2", true)) {
          value_p2 = request->getParam("value_p2", true)->value().toFloat();
          preferences.putFloat("value_p2", value_p2);
      }
      // Close the Preferences
      preferences.end();

      StaticJsonDocument<200> docset; // Adjust size based on your data needs
      docset["ttg"] = temp_target_gas;
      docset["ttr"] = temp_target_tr;
      docset["vt1"] = value_tg1;
      docset["vt2"] = value_tg2;
      docset["vt3"] = value_tg3;
      docset["vt4"] = value_tg4;
      docset["vp1"] = value_p1;
      docset["vp2"] = value_p2;

      String jsonSetup;
      serializeJson(docset, jsonSetup);
      events.send(jsonSetup.c_str(), "setupData", millis());

      setupRead();

      // Send a response back to the client
      request->send(200, "text/plain", "Setup values updated successfully");
  } else {
      request->send(405, "text/plain", "Method Not Allowed");
  }
}


void initSensor(){
    Wire.begin(); //Joing I2C bus1
    
    // 5B // 5A // 70
    if (therm1.begin(0x5B) == false){ // Initialize thermal IR sensor
      Serial.println("Qwiic IR thermometer - 1 did not acknowledge! Freezing!");
      while(1);
    }
    Serial.println("Qwiic IR Thermometer - 1 did acknowledge.");
  
    if (therm2.begin(0x5A) == false){ 
      Serial.println("Qwiic IR thermometer - 2 did not acknowledge! Freezing!");
      while(1);
    }
    Serial.println("Qwiic IR Thermometer - 2 did acknowledge.");

    if (therm3.begin(0x70) == false){ 
      Serial.println("Qwiic IR thermometer - 3 did not acknowledge! Freezing!");
      while(1);
    }
    Serial.println("Qwiic IR Thermometer - 3 did acknowledge.");

    therm1.setUnit(TEMP_C); therm2.setUnit(TEMP_C); therm3.setUnit(TEMP_C); 
    // Alternatively, TEMP_F can be replaced with TEMP_C for Celsius or TEMP_K for Kelvin.
    
    dht.begin();
}

void setup() {
  Serial.begin(115200); Serial.println("Esp Ready!! : )");
  if (!LittleFS.begin(1)) {                         
    Serial.println("An error has occurred while mounting LittleFS");
    return;
  }
  Serial.println("LittleFS mounted successfully");

  setupRead();
  
  calcRead();
  dataRead();

  initSensor();

  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  //  WiFi.config(ip, gateway, subnet); 
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println();
  Serial.print("IP Address: "); Serial.println(WiFi.localIP());

  ntp.ruleDST("CEST", Last, Sun, Mar, 2, 120);
  ntp.ruleSTD("CET", Last, Sun, Oct, 3, 60);
  ntp.begin();    // ntp starten
  ntp.update();   // Zeit holen

  // Zeit mit formatedTime() anzeigen:
  // %d = Tag, %m = Monat, %Y = Jahr, %T = Zeit in Stunden, Minuten, Sekunden
  Serial.println(ntp.formattedTime("%d.%m.%Y Uhrzeit: %T"));

  // -------------------------Clear Calc-----------------------
  server.on("/clearCalc", HTTP_GET, [](AsyncWebServerRequest *request) {
      if (LittleFS.remove("/CalcData.txt")) { 
          request->send(200, "text/plain", "/CalcData.txt cleared successfully");
          events.send("Table cleared", "clearTable", millis());
      } else {
          request->send(500, "text/plain", "Failed to clear data");
      }
  });

  // -------------------------Calc Save-----------------------
  server.on("/saveCalcData", HTTP_POST, [](AsyncWebServerRequest *request){}, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
      // Here we do First save data to file and read and send to server for show..
      String body = "";
      for(size_t i = 0; i < len; i++) {
          body += (char)data[i];
      }
      Serial.println(body); // This should print the raw JSON string

      DynamicJsonDocument doc(1024);
      DeserializationError error = deserializeJson(doc, body);
      if (error) {
          Serial.print(F("deserializeJson() failed: "));
          Serial.println(error.f_str());
          request->send(400, "text/plain", "Invalid JSON");
          return;
      }

      // Append data to a file
      File file;
      file = LittleFS.open("/CalcData.txt", "a"); 
      if (!file) {
          Serial.println("Failed to open /CalcData.txt file for appending");
          request->send(500, "text/plain", "Failed to open file");
          return;
      }
      file.println(body);                         
      file.close();

      file = LittleFS.open("/CalcData.txt", "r");    
      if (!file) {
          Serial.println("Failed to open /CalcData.txt file for reading");
          return;
      }

      String fileContent;
      while(file.available()){
          fileContent += file.readStringUntil('\n') + '\n';
      }
      file.close();
      Serial.println(fileContent.c_str());
      events.send(fileContent.c_str(), "calculateData", millis());

      request->send(200, "text/plain", "Calc Data saved!");
  });



  // -------------------------Clear Data-----------------------
  server.on("/clearData", HTTP_GET, [](AsyncWebServerRequest *request) {
      if (LittleFS.remove("/sensor_data.txt")) { 
          request->send(200, "text/plain", "/sensor_data.txt cleared successfully");
          events.send("Data cleared", "clearDataTable", millis());
      } else {
          request->send(500, "text/plain", "Failed to Delete sensor_data.txt");
      }
  });


  // Send web page 
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send_P(200, "text/html", index_html);
  });

  // Handle POST requests on Setup Page
  server.on("/", HTTP_POST, handleUpdate);

  // When Web Server Get Connect
  events.onConnect([](AsyncEventSourceClient *client){
      if(client->lastId()){
          Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
      }
      client->send("ESP Ready!!", NULL, millis(), 10000);
      
      Serial.println("web is online");
      events.send(ssid,"wifiname",millis());
      events.send(WiFi.localIP().toString().c_str(), "ipaddr", millis());
      events.send(ntp.formattedTime("%d.%m.%Y"),"date",millis());
      
      StaticJsonDocument<200> docset; 
      docset["ttg"] = temp_target_gas;
      docset["ttr"] = temp_target_tr;
      docset["vt1"] = value_tg1;
      docset["vt2"] = value_tg2;
      docset["vt3"] = value_tg3;
      docset["vt4"] = value_tg4;
      docset["vp1"] = value_p1;
      docset["vp2"] = value_p2;

      String jsonSetup;
      serializeJson(docset, jsonSetup);
      events.send(jsonSetup.c_str(), "setupData",millis());

      File file;
      file = LittleFS.open("/sensor_data.txt", "r"); 
      if (!file) {
        Serial.println("Failed to open file for reading");
        return;
      }
      String dataContent2 ="[";
      while(file.available()){
          dataContent2 += file.readStringUntil('\n');
      }
      
      if (dataContent2.length() > 0) {
          dataContent2.remove(dataContent2.length() - 1);
      }
      dataContent2+="]";
      Serial.print("/sensor_data.txt size: ");  Serial.println(dataContent2.length());
      file.close();
  
      // Serial.println(dataContent2.c_str());
      events.send(dataContent2.c_str(), "DataData", millis());
      // Serial.println(test);
      // events.send(test, "DataData", millis());
      
      file = LittleFS.open("/CalcData.txt", "r"); 
      if (!file) {
          Serial.println("Failed to open /CalcData.txt file for reading");
          return;
      }

      String fileContent;
      while(file.available()){
          fileContent += file.readStringUntil('\n') + '\n';
      }
      file.close();

      events.send(fileContent.c_str(), "calculateData", millis());
  });

  server.addHandler(&events);
  server.onNotFound(notFound);
  server.begin();

  previousMillis = millis();
}

void loop() {
  ntp.update();   // Zeit holen
  unsigned long currentMillis = millis();

  // Check if it's time to send sensor data again
  if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;  // Save the last time sensor data was sent

      sendSensorData();
  }
    
  if ((WiFi.status() != WL_CONNECTED) && (currentMillis - previousMillis2 >=interval2)) {
      Serial.print(millis());
      Serial.println("Reconnecting to WiFi...");
      WiFi.disconnect();
      WiFi.reconnect();
      previousMillis2 = currentMillis;
  }  
}

void sendSensorData() {
  StaticJsonDocument<200> doc; // Adjust size based on your data needs
  
  // Call therm.read() to read object and ambient temperatures from the sensor.
  if (therm1.read()) { 
    // Serial.print("Object: " + String(therm1.object(), 2));   Serial.println("C");
    // Serial.print("Ambient: " + String(therm1.ambient(), 2)); Serial.println("C");
    // Serial.println();
    doc["irt1"] = therm1.object(); 
  }
  
  if (therm2.read()) { 
    // Serial.print("Object: " + String(therm2.object(), 2));   Serial.println("C");
    // Serial.print("Ambient: " + String(therm2.ambient(), 2)); Serial.println("C");
    // Serial.println();
     doc["irt2"] = therm2.object(); 
  }

  if (therm3.read()) { 
    // Serial.print("Object: " + String(therm3.object(), 2));   Serial.println("C");
    // Serial.print("Ambient: " + String(therm3.ambient(), 2)); Serial.println("C");
    // Serial.println();
     doc["irt3"] = therm3.object(); 
  }

    // Read temperature from DHT sensor
  float tempDHT = dht.readTemperature();

  if (isnan(tempDHT)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }else{
    doc["temp"] = tempDHT; 
  }

  doc["timestamp"] = ntp.formattedTime("%Y-%m-%d %H:%M:%S");

  String jsonString;
  serializeJson(doc, jsonString);
  
  // Append the data to a file
  File file = LittleFS.open("/sensor_data.txt", "a"); // For ESP32, replace LittleFS with SPIFFS
  if (!file) {
    Serial.println("Failed to open /sensor_data.txt file for appending");
    return;
  }
  file.print(jsonString); file.print(",");
  file.close();
  events.send(jsonString.c_str(), "sensorData", millis());
}
