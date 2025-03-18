void checkRFID() {
  uint8_t uid[7];
  uint8_t uidLength;
// used readMifareClassic example from RFID library
  if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 100)) { //50 in this case indicates how long (in ms) how long function waits for tag to be detected
    scannedUID = "";
    for (uint8_t i = 0; i < uidLength; i++) {
      scannedUID += String(uid[i], HEX); //appending string to hex
    }
    Serial.print("Scanned RFID UID: ");
    Serial.println(scannedUID);
    if (scannedUID == allowedUID) {
      unlockDoor();
      displayLCD("Access Granted", "Locker Unlocked");
      lastUnlockTime = millis(); //closes lock after 100000 milliseconds
    } else if(scannedUID != allowedUID){
      displayLCD("Card unauthorised", "Incorrect Lock");
      delay(2000);
      displayLCD("Welcome Back", "Door Locked");
    }
    else{
      displayLCD("Access Denied", "Invalid Tag");
      delay(2000);
      displayLCD("Welcome Back", "Door Locked");
    
    //adding test comment for GitHub
    }
  }
}
