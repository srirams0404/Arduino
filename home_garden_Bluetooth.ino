#include <SoftwareSerial.h>
#include <DHT.h>

#define DHTPIN 13         // Pin for DHT sensor data
#define SOIL_MOISTURE_PIN A1  // Pin for soil moisture sensor

DHT dht(DHTPIN, DHT11);
SoftwareSerial bluetooth(0, 1);  // RX, TX (connect Bluetooth module to these pins)

void setup() {
  Serial.begin(9600);
  bluetooth.begin(9600);  // Initialize the software serial port for Bluetooth
  dht.begin();
}

void loop() {
  // Read humidity and temperature from DHT22
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Read soil moisture
  int soilMoisture = analogRead(SOIL_MOISTURE_PIN);

  // Print the sensor data to the serial monitor
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("%  Temperature: ");
  Serial.print(temperature);
  Serial.print("°C  Soil Moisture: ");
  Serial.println(soilMoisture);

  // Send the data over Bluetooth
  bluetooth.print("H:");
  bluetooth.print(humidity);
  bluetooth.print(" T:");
  bluetooth.print(temperature);
  bluetooth.print(" S:");
  bluetooth.println(soilMoisture);

  delay(1000);  // Adjust the delay as needed
}
