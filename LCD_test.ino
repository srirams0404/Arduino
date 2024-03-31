#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.print("Blinking Display");
}

void loop() {
  lcd.display();
  delay(1000);
  lcd.noDisplay();
  delay(1000);
}