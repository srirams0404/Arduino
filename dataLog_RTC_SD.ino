/*
SD:
vcc - 5V
miso - 12
mosi - 11
sck - 13
cs - 4

DHT11 - 5
sda -A4
scl -A5
*/
#include <Wire.h>
#include <RTClib.h>
#include <SD.h>
#include <DHT.h>

#define DHTPIN 5           // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11      // DHT sensor type
#define SD_CHIP_SELECT 4  // Pin connected to the SD card module chip select

RTC_DS3231 rtc;            // Create an instance of the RTC_DS3231 class
DHT dht(DHTPIN, DHTTYPE);  // Create an instance of the DHT class

File dataFile;             // File object to interact with the SD card

void setup() {
  Serial.begin(9600);
  Wire.begin();
  rtc.begin();
  dht.begin();

  // If RTC lost power, set the time to the compile time
  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    Serial.println("RTC lost power, setting time to compile time.");
  }

  // Initialize SD card
  if (!SD.begin(SD_CHIP_SELECT)) {
    Serial.println("SD card initialization failed!");
    return;
  }

  Serial.println("SD card initialized.");

  // Create and open a file named "data.csv" for writing
  dataFile = SD.open("data.csv", FILE_WRITE);

}

void loop() {
  // Read current date and time
  DateTime now = rtc.now();

  // Read temperature and humidity from DHT sensor
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Open the data file in append mode
  dataFile = SD.open("data.csv", FILE_WRITE);

  if (dataFile) {
    // Print date, time, temperature, and humidity values to the serial monitor
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
    Serial.print(now.second(), DEC);
    Serial.print(", Temperature: ");
    Serial.print(temperature);
    Serial.print("°C, Humidity: ");
    Serial.print(humidity);
    Serial.println("%");

    // Print date, time, temperature, and humidity values to the CSV file
    dataFile.print(now.year(), DEC);
    dataFile.print('/');
    dataFile.print(now.month(), DEC);
    dataFile.print('/');
    dataFile.print(now.day(), DEC);
    dataFile.print(",");
    dataFile.print(now.hour(), DEC);
    dataFile.print(':');
    dataFile.print(now.minute(), DEC);
    dataFile.print(':');
    dataFile.print(now.second(), DEC);
    dataFile.print(",");
    dataFile.print(temperature);
    dataFile.print(",");
    dataFile.println(humidity);

    dataFile.close();
  } else {
    Serial.println("Error opening file for writing!");
  }

  delay(5000); // Delay for 5 seconds before the next reading
}
