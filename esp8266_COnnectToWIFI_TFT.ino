#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <ESP8266WiFi.h>

// WiFi network
const char* ssid     = "Sakthi";
const char* password = "Olympics2011";

// For the ESP8266, the pins are defined as follows:
#define TFT_CS     14
#define TFT_RST    2  // You can also connect this to the ESP8266's reset
#define TFT_DC     0

// Using software SPI is also possible if needed.
//Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

// Hardware SPI
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

void setup() {
  Serial.begin(9600);
  // Initialize display
  tft.initR(INITR_BLACKTAB);      // Init ST7735S chip, black tab
  tft.fillScreen(ST7735_BLACK);   // Fill screen with black color
  tft.setRotation(1);             // Set orientation. Use 0-3 to rotate as needed
  tft.color565(uint8_t r, uint8_t g, uint8_t b)
  // Text settings
  tft.setTextSize(1);             // Normal 1:1 pixel scale
  tft.setTextColor(ST7735_WHITE); // White text color
  tft.setCursor(0, 0);            // Start at top-left corner

  // Print "Hello, World!" to the TFT display
  tft.println("Connecting to ");
  tft.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    tft.print(".");
  }

  tft.println("");
  tft.println("WiFi connected");  
  tft.println("IP address: ");
  tft.println(WiFi.localIP());

}

void loop() {}