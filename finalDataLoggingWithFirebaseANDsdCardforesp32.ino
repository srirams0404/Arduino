#include <Arduino.h>
#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>
// Provide the token generation process info.
#include "addons/TokenHelper.h"
// Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "Sakthi"
#define WIFI_PASSWORD "Olympics2011"

// Insert Firebase project API Key
#define API_KEY "AIzaSyDgBVyxaGNXlI7-50RWv_2OL2ETFRQfR1Q"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://scientificdatalogging001-default-rtdb.firebaseio.com/"

bool signupOK = false;

// Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

//DHT Sensor lib call
#include <DHT.h>
#define DHTPIN 5           // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11      // DHT sensor type
DHT dht(DHTPIN, DHTTYPE);  // Create an instance of the DHT class

#include <OneWire.h>
#include <DallasTemperature.h>
// GPIO where the DS18B20 is connected to
const int oneWireBus = 4;
// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);
// Pass our oneWire reference to Dallas Temperature sensor
DallasTemperature sensors(&oneWire);

//Call RTC Library
#include <Wire.h>
#include <RTClib.h>
RTC_DS1307 rtc;

//PinSetup for pH snesor and Calibaration Variables
#define SensorPin 36      //pH meter Analog output to Arduino Analog Input 0
float calibration_value = 21.34 + 3.2;
unsigned long int avgValue;  //Store the average value of the sensor feedback
int buf[10], temp;

#include <SPI.h>
#include <FS.h>
#include <FSImpl.h>
#include <vfs_api.h>
#include <SD.h>
#include <sd_defines.h>
#include <sd_diskio.h>
File myFile;
const int CS = 15;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(500);
  while (!Serial) { ; }  // wait for serial port to connect. Needed for native USB port only
  Serial.println("Initializing SD card...");
  if (!SD.begin(CS)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  Wire.begin();
  rtc.begin();
  rtc.adjust(DateTime(F(__DATE__),F(__TIME__)));
  //rtc.adjust(DateTime(2019, 1, 21, 5, 0, 0));
  dht.begin();
  // init pheripherals
  Serial.println("Init RTC...");
  Serial.println("Init DHT...");
  // Create and open a file named "data.csv" for writing
  myFile = SD.open("/data01.txt", FILE_WRITE);
  //initialize Wifi and FireBase
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
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
  if (Firebase.signUp(&config, &auth, "", ""))
  {
    Serial.println("ok");
    signupOK = true;
  }
  else
  {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

}

void loop() {
  // Read current date and time
  DateTime now = rtc.now();
  // Read temperature and humidity from DHT sensor
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  // DS18B20 Temperature Sensor Value
  int sensorValue = analogRead(39);
  float voltage = sensorValue * (5.0 / 1024.0);
  sensors.requestTemperatures();
  float temperatureC = sensors.getTempCByIndex(0);
  float temperatureF = sensors.getTempFByIndex(0);
  //pH Sensor reading
  for(int i=0;i<10;i++){ //Get 10 sample value from the sensor for smooth the value
  buf[i]=analogRead(SensorPin);
  delay(10);
  }
  for(int i=0;i<9;i++){ //sort the analog from small to large
    for(int j=i+1;j<10;j++){
      if(buf[i]>buf[j]){
        temp=buf[i];
        buf[i]=buf[j];
        buf[j]=temp;
      }
    }
  }
  avgValue=0;
  for(int i=2;i<8;i++)avgValue+=buf[i];  //take the average value of 6 center sample
  float phValue=(float)avgValue*5.0/4096/6; //convert the analog into millivolt
  phValue = -5.70 * phValue + calibration_value; //convert the millivolt into pH value

  // FireBase RTDB Data Transmission
  Firebase.RTDB.setFloat(&fbdo, "HUMIDITY/humd", humidity);
  Firebase.RTDB.setFloat(&fbdo, "TEMPERATURE/temp", temperature);
  Firebase.RTDB.setFloat(&fbdo, "TURBIDITY/turb", voltage);
  Firebase.RTDB.setFloat(&fbdo, "CONTACT_TEMPc/cTemp", temperatureC);
  Firebase.RTDB.setFloat(&fbdo, "CONTACT_TEMPf/fTemp", temperatureF);
  Firebase.RTDB.setFloat(&fbdo, "pH_value/pHVal", phValue);

  // Open the data file in append mode
  myFile = SD.open("/data01.txt", FILE_APPEND);
  //Write to SD CARD 
  if (myFile) {
    Serial.print("Date: ");
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" Time: ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.println(now.second(), DEC);
    Serial.print("Humididty:");
    Serial.println(humidity);
    Serial.print("Temperature:");
    Serial.println(temperature);
    Serial.print("Turbidity:");
    Serial.println(voltage);
    Serial.print("ContactTemp(c):");
    Serial.println(temperatureC);
    Serial.print("ContactTemp(f):");
    Serial.println(temperatureF);
    Serial.print("pH Value:");
    Serial.println(phValue);
    Serial.println("----------------");
     // Write RTC data to file
    myFile.print(now.year(), DEC);
    myFile.print('/');
    myFile.print(now.month(), DEC);
    myFile.print('/');
    myFile.print(now.day(), DEC);
    myFile.print(',');
    myFile.print(now.hour(), DEC);
    myFile.print(':');
    myFile.print(now.minute(), DEC);
    myFile.print(':');
    myFile.print(now.second(), DEC);
    myFile.print(',');
    //DHT sensor write
    myFile.print(humidity);
    myFile.print(',');
    myFile.print(temperature);
    //Turbidity Sensor Value
    myFile.print(voltage);
    myFile.print(',');
    //DS18C20 Temperature sesnor
    myFile.print(temperatureC);
    myFile.print(',');
    myFile.print(temperatureF);
    myFile.print(',');
    myFile.println(phValue);

    myFile.close();
  } else {
    Serial.println("Error opening file");
  }
  delay(2000);
}

