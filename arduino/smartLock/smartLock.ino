#undef CLOSED
#undef HIGH
#include <Keypad.h>
#include <ESP32Servo.h>
#include <Wire.h>
#include <Adafruit_PN532.h>
#include "rgb_lcd.h"
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>

AsyncWebServer server(80);

const char* SSID = "HUAWEI-2.4G-ZUSW";
const char* PASSWORD = "pjMs5P5h";
const char* ADMIN = "password";

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
Adafruit_PN532 nfc(PN532_SDA, PN532_SCL);  // passing arguments through Adafruit class
Servo myServo;                             //called Servo class and created object, same for keypad and lcd

rgb_lcd lcd;
const int LCD_COLS = 16;
const int LCD_ROWS = 2;

bool lockedServo = true;
char enteredCode[5];
int codeIndex = 0;
String currentPassword = "1234";      //current password for keypad, can be changed in adminmode
const String allowedUID = "3463953";  //hardcoded allowed NFC tag
String scannedUID = "None";

unsigned long lastUnlockTime = 0;
unsigned long lastRFIDCheck = 0;
//unsigned long was used, as "int" is too small to store the length of this programs runtime, i believe 32-33000(33 seconds). Which when calculating in ms ->
//(runtime of program which can vary...) would result in integer overflow and a crashed program
void setup() {

  Serial.begin(115200);
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to Wi-Fi...");
  }
  Serial.println("Connected to Wi-Fi");
  delay(500);
  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request -> send(200, "text/html", webApp());
  });

  server.on("/scannedUID", HTTP_GET, [](AsyncWebServerRequest *request) {
    request -> send(200, "text/plain", scannedUID);
  });

  server.on("/admin-login", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (request -> hasParam("password") && request -> getParam("password")->value().equalsIgnoreCase(ADMIN)) {
      request -> send(200, "text/plain", "Access Granted");
    } else {
      request -> send(403, "text/plain", "Incorrect Password");
    }
  });

  server.on("/unlock", HTTP_GET, [](AsyncWebServerRequest *request) {
    lockedServo = false;
    unlockDoor();
    request -> send(200, "text/plain", "Unlocked");
  });

  server.on("/lock", HTTP_GET, [](AsyncWebServerRequest *request) {
    lockedServo = true;
    lockDoor();
    request -> send(200, "text/plain", "Locked");
  });

  server.begin();
  Serial.println("Server has started: HTTP");
//ok
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
    while (1)
      ;  //Using while(1) to ensure loop is infinite until RFID reader is detected
  }
  nfc.SAMConfig();  //Secure Access Module config outside if(), ensures communciation between RFID & ESP32, ONLY when RFID is detected ie. while(1) loop breaks
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
    yield();
  }
  //using built in c "millis()" function to track how long program has been run for
  if (millis() - lastRFIDCheck >= 2500) {
    lastRFIDCheck = millis();  // by letting LastRFIDcheck = millis() function, it will constantlyy check every 2.5 seconds (2500milliseconds)
    checkRFID();
    yield();
  }

  if (!lockedServo && (millis() - lastUnlockTime > 100000)) {
    lockDoor();
    displayLCD("Locked", "");
  }
}

void displayLCD(const char* line1, const char* line2) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(line1);
  lcd.setCursor(0, 1);
  lcd.print(line2);
}

void unlockDoor() {
  lockedServo = false;
  myServo.write(0);
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(WHITE_LED_PIN, HIGH);
  digitalWrite(YELLOW_LED_PIN, LOW);
}

void lockDoor() {
  lockedServo = true;
  myServo.write(90);
  digitalWrite(RED_LED_PIN, HIGH);
  digitalWrite(WHITE_LED_PIN, LOW);
  digitalWrite(YELLOW_LED_PIN, LOW);
}
