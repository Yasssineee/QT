#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define SS_PIN 10  // RFID SS Pin
#define RST_PIN 9  // RFID Reset Pin
MFRC522 rfid(SS_PIN, RST_PIN);  // Create MFRC522 instance

Servo servo1, servo2, servo3;

LiquidCrystal_I2C lcd(0x27, 16, 2);  // LCD setup (I2C address)

#define BUZZER_PIN 2 // Buzzer Pin

int greenLED[1] = {7};  // Pins for green LEDs
int redLED[1] = {8};    // Pins for red LEDs

void setup() {
  Serial.begin(9600);
  SPI.begin();            // Initialize SPI
  rfid.PCD_Init();        // Initialize RFID

  // Setup Servo Motors
  servo1.attach(3);
  servo2.attach(5);
  servo3.attach(6);

  // Setup LEDs
  for (int i = 0; i < 1; i++) {
    pinMode(greenLED[i], OUTPUT);
    pinMode(redLED[i], OUTPUT);
  }

  pinMode(BUZZER_PIN, OUTPUT);  // Set the buzzer pin as output

  // Initialize LCD
  lcd.init();
  lcd.backlight();  // Turn on the LCD backlight
  lcd.setCursor(0, 0);
  lcd.print("System Ready!");
  delay(2000);
}

void loop() {
  if (rfid.PICC_IsNewCardPresent()) {
    if (rfid.PICC_ReadCardSerial()) {
      String clientId = "";
      for (byte i = 0; i < rfid.uid.size; i++) {
        clientId += String(rfid.uid.uidByte[i], HEX);
      }
      Serial.println("ID:" + clientId);  // Send ID to Qt for database check

      delay(1000);  // Wait for 1 second before scanning again
    }
  }

  if (Serial.available() > 0) {
    String command = Serial.readString();  // Read command from Qt
    Serial.println("Received Command: " + command);

    if (command == "GRANT_ACCESS") {
      grantAccess();  // Open door and show green LED
    } else if (command == "DENY_ACCESS") {
      denyAccess();  // Deny access and show red LED
    }
  }
}

void grantAccess() {
  // Open the doors (rotate servos) and show green LED
  digitalWrite(greenLED[0], HIGH);
  digitalWrite(redLED[0], LOW);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Access Granted");
  lcd.setCursor(0, 1);
  lcd.print("Welcome!");

  Serial.println("Green LED ON - Access Granted");

  tone(BUZZER_PIN, 1000, 500);  // Play success tone (1000 Hz for 500 ms)

  // Move servo to open doors
  servo1.write(90);
  delay(1000);
  servo2.write(90);
  delay(1000);
  servo3.write(90);
  delay(5000);

  // Close doors (reset servos)
  servo1.write(0);
  delay(1000);
  servo2.write(0);
  delay(1000);
  servo3.write(0);

  Serial.println("Doors closed");
  digitalWrite(greenLED[0], LOW);
  digitalWrite(redLED[0], HIGH);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Doors Closed");
  lcd.setCursor(0, 1);
  lcd.print("Default State");
}

void denyAccess() {
  digitalWrite(redLED[0], HIGH);

  for (int i = 0; i < 1; i++) {
    digitalWrite(greenLED[i], LOW);
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Access Denied");
  lcd.setCursor(0, 1);
  lcd.print("Try Again!");

  // Play error tone
  tone(BUZZER_PIN, 500, 1000);  // 500 Hz for 1 second

  Serial.println("Red LED ON - Access Denied");
}
