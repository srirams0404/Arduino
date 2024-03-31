// Libraries
#include <ESP8266WiFi.h>

// WiFi network
const char* ssid     = "Sakthi";
const char* password = "Olympics2011";

void setup() {
  
  // Start serial
  Serial.begin(9600);
  delay(10);

  // Connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  
}