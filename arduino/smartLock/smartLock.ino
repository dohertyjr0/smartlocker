#include <Keypad.h>
#include <ESP32Servo.h>
#include <Wire.h>
#include <Adafruit_PN532.h>
#include "rgb_lcd.h"
#include <WiFi.h>
#include <FirebaseESP32.h>

const char* ssid = "iPhone 12 Pro";
const char* password = "01234567";
#define FIREBASE_HOST "https://esp32-database-5fc3b-default-rtdb.europe-west1.firebasedatabase.app/"
#define FIREBASE_AUTH ""

FirebaseData firebaseData;

#define PN532_SDA 21
#define PN532_SCL 22
#define RED_LED_PIN 13
#define WHITE_LED_PIN 12
#define YELLOW_LED_PIN 14

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  { '1', '4', '7', 'F' },
  { '2', '5', '6', 'E' },
  { '3', '6', '9', 'D' },
  { 'A', '0', 'B', 'C' }
};
byte rowPins[ROWS] = { 15, 2, 4, 23 };
byte colPins[COLS] = { 17, 25, 26, 33 };

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
Adafruit_PN532 nfc(PN532_SDA, PN532_SCL);
Servo myServo;
rgb_lcd lcd;
const int LCD_COLS = 16;
const int LCD_ROWS = 2;

bool locked = true;
char enteredCode[5];
int codeIndex = 0;
String currentPassword = "1234";
const String allowedUID = "3463953";

unsigned long lastUnlockTime = 0;
const unsigned long lockDelay = 5000;
unsigned long lastRFIDCheck = 0;
const unsigned long rfidCheckInterval = 2000;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to Wi-Fi...");
  }
  Serial.println("Connected to Wi-Fi");
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  if (Firebase.setString(firebaseData, "/test", "Hello Firebase")) {
    Serial.println("Data written successfully!");
  } else {
    Serial.println("Error writing to Firebase: " + firebaseData.errorReason());
  }
  
  Wire.begin();
  myServo.attach(27);

  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(WHITE_LED_PIN, OUTPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);

  lockDoor();

  lcd.begin(LCD_COLS, LCD_ROWS);
  lcd.setRGB(255, 255, 255);
  lcd.clear();
  lcd.print("System Ready");

  if (!nfc.begin()) {
    Serial.println("RFID reader not detected!");
    lcd.clear();
    lcd.print("RFID Error!");
    while (1);  //Using while(1) to ensure loop is infinite until RFID reader is detected
  }
  nfc.SAMConfig();  //Secure Access Module, ensures communciation between RFID & ESP32, ONLY when RFID is detected
  Serial.println("RFID reader initialized!");
  lcd.setCursor(0, 1);
  lcd.print("RFID Ready!");
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(WHITE_LED_PIN, LOW);
  digitalWrite(YELLOW_LED_PIN, LOW);
}

void loop() {

  char key = keypad.getKey();
  if (key) {
    Serial.print("Key Pressed: ");
    Serial.println(key);
    handleKeypadInput(key);
  }

  if (millis() - lastRFIDCheck >= rfidCheckInterval) {
    lastRFIDCheck = millis();
    Serial.println("Checking RFID...");
    checkRFID();
  }

  if (!locked && (millis() - lastUnlockTime > lockDelay)) {
    lockDoor();
    displayMessage("Locker Locked", "");
  }
}

void displayMessage(const char* line1, const char* line2) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(line1);
  lcd.setCursor(0, 1);
  lcd.print(line2);
}

void unlockDoor() {
  locked = false;
  myServo.write(270);
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(WHITE_LED_PIN, HIGH);
  digitalWrite(YELLOW_LED_PIN, LOW);
}

void lockDoor() {
  locked = true;
  myServo.write(90);
  digitalWrite(RED_LED_PIN, HIGH);
  digitalWrite(WHITE_LED_PIN, LOW);
  digitalWrite(YELLOW_LED_PIN, LOW);
}
