#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "Sriram"
#define WIFI_PASSWORD "sriram123"

// Insert Firebase project API Key
#define API_KEY "AIzaSyDX6bS8NGPuwJMYgJudMIcxni3jhGZNgl0"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://esp32smartsense-default-rtdb.firebaseio.com/" 

//Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
bool signupOK = false;

//Call Lib for TFT Display
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
// Pin definitions for the ESP32 and the TFT display
#define TFT_CS   5   // Chip select pin
#define TFT_RST  17  // Reset pin
#define TFT_DC   16  // Data/command pin
// Initialize Adafruit ST7735 TFT library
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

//Call Lib for Temperature and Humidity
#include <DHT.h>           //Library for DHT11
#define DHTPIN 4           //Define Pin Number
#define DHTTYPE DHT11      //Define DHT11 Component
DHT dht(DHTPIN, DHTTYPE);

//Setup for Light Intensity
const int LDRPin = 34;

//Setup for Gas Sensor
const int MQSense = A0;

//User Func for Temperature
float temp(){
  float T = dht.readTemperature();
  return T;
}

//User Func for Humidity
float humd(){
  float H = dht.readHumidity();
  return H;
}

//User Func for LightIntensity
float readLightIntensity(int pin) {
  int sensorValue = analogRead(pin);
  float voltage = sensorValue * (3.3 / 4095.0); // Convert to voltage
  float Rldr = ((3.3 / voltage) - 1.0) * 10000; // Calculate LDR resistance assuming a 10K resistor
  float lightIntensity = calculateIntensityFromResistance(Rldr);
  return lightIntensity;
}

float calculateIntensityFromResistance(float resistance) {
  float intensity = 10000 / resistance; 
  return intensity;
}

//User Func for GasSensor
int gas(){
  analogReadResolution(12);
  int gasensor = analogRead(MQSense);
  int CO2ppm= map(gasensor, 0, 4095, 500, 900);
  return CO2ppm;
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  dht.begin();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  /* Assign the api key (required) */
  config.api_key = API_KEY;
  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  // put your main code here, to run repeatedly:
  float cH = humd();
  float cT = temp();
  float Lint = readLightIntensity(LDRPin);
  int GAS = gas();

  Firebase.RTDB.setFloat(&fbdo , "TEMPERATURE/temp", cT);
  Firebase.RTDB.setFloat(&fbdo , "HUMIDITY/humd", cH);
  Firebase.RTDB.setFloat(&fbdo , "LightIntensity/Lint", Lint);
  Firebase.RTDB.setInt(&fbdo , "CO-2Level/gAs", GAS);
  /*Serial.print("Humidity:");
  Serial.println(cH);
  Serial.print("Temperature:");
  Serial.println(cT);
  Serial.print("Light Intensity:");
  Serial.println(Lint);
  Serial.print("CO2_PPM:");
  Serial.println(GAS);
  Serial.println("------------------------------");
  delay(800);*/
}

//GAS sensor ----> UN and 5V
//temp-humd -----> D4 and 3.3V
//LDR ------> D34 and 5V

