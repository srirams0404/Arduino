#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10  // Define the SS (Slave Select) pin
#define RST_PIN 9  // Define the RST (Reset) pin
#define BUZZER_PIN 8  // Define the digital pin for the buzzer

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

// Predefined correct UID (replace this with your actual UID)
String correctUID = "b862e133";

void setup() {
  Serial.begin(9600);  // Start serial communication
  SPI.begin();  // Initiate SPI communication
  mfrc522.PCD_Init();  // Initiate MFRC522

  pinMode(BUZZER_PIN, OUTPUT);  // Set the buzzer pin as an output
}

void loop() {
  // Look for new cards
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    // Store UID (Unique Identifier) in a variable
    String uid = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      uid += String(mfrc522.uid.uidByte[i], HEX);
    }

    // Display UID on serial monitor
    Serial.print("RFID UID: ");
    Serial.println(uid);

    // Check if the UID matches the correct UID
    if (uid.equals(correctUID)) {
      // Correct UID, make a single buzzer sound
      activateBuzzer(1);
    } else {
      // Incorrect UID, buzz twice
      activateBuzzer(8);
    }

    // Halt the reader to avoid collisions
    mfrc522.PICC_HaltA();
    // Stop encryption on PCD
    mfrc522.PCD_StopCrypto1();
  }
}

void activateBuzzer(int buzzCount) {
  for (int i = 0; i < buzzCount; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(500);  // Adjust this delay as needed
    digitalWrite(BUZZER_PIN, LOW);
    delay(500);  // Adjust this delay as needed
  }
}