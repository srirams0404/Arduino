#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

// For the ESP8266, the pins are defined as follows:
#define TFT_CS     27
#define TFT_RST    12  // You can also connect this to the ESP8266's reset
#define TFT_DC     21

// Using software SPI is also possible if needed.
//Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

// Hardware SPI
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

void setup() {
  Serial.begin(115200);
  tft.initR(INITR_BLACKTAB);   // Initialize the display
  tft.fillScreen(ST7735_BLACK); // Clear the screen to black
}

void loop() {
  // Your display code here
}
