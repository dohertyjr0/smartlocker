void checkRFID() {
  uint8_t uid[7];
  uint8_t uidLength;

  if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 50)) {
    String scannedUID = "";
    for (uint8_t i = 0; i < uidLength; i++) {
      scannedUID += String(uid[i], HEX);
    }
    Serial.print("Scanned RFID UID: ");
    Serial.println(scannedUID);
    if (scannedUID == allowedUID) {
      unlockDoor();
      displayLCD("Access Granted", "Locker Unlocked");
      lastUnlockTime = millis();
    } else if(scannedUID != allowedUID){
      displayLCD("Card unauthorised", "Incorrect Lock");
      delay(2000);
      displayLCD("Welcome Back", "Door Locked");
    }
    else{
      displayLCD("Access Denied", "Invalid Tag");
      delay(2000);
      displayLCD("Welcome Back", "Door Locked");
    
    }
  }
}
