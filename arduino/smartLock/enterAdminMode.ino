void enterAdminMode() {
  char adminKey = '\0'; // initially i had set this to 0 not realising since this is a char ' ' not realising whitespace has an ASCII value, ensures adminKey is set to null

  while (true) {
    displayLCD("Admin Mode", "1:Change PIN");
    digitalWrite(RED_LED, LOW);    
    digitalWrite(YELLOW_LED, HIGH); 
    adminKey = keypad.getKey(); // stores keypresses in adminKey

    if (adminKey) {
      Serial.print("Admin Key Pressed: ");
      Serial.println(adminKey); 

      if (adminKey == '1') {
        Serial.println("Changing PIN...");
        changePassword();
        break; 
      } else {
        Serial.println("Invalid Admin Option");
        displayLCD("Invalid Option", "");
        delay(2000);
      }
    }
  }

  displayLCD("Welcome Back", "Door Locked");
}
