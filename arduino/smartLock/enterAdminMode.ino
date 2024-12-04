void enterAdminMode() {
  char adminKey = '\0'; 

  while (true) {
    displayMessage("Admin Mode", "1:Change PIN");
    digitalWrite(RED_LED_PIN, LOW);   
    digitalWrite(WHITE_LED_PIN, LOW);  
    digitalWrite(YELLOW_LED_PIN, HIGH); 
    adminKey = keypad.getKey();

    if (adminKey) {
      Serial.print("Admin Key Pressed: ");
      Serial.println(adminKey);

      if (adminKey == '1') {
        Serial.println("Changing PIN...");
        changePassword();
        break; 
      } else {
        Serial.println("Invalid Admin Option");
        displayMessage("Invalid Option", "");
        delay(2000);
      }
    }
  }

  displayMessage("Welcome Back", "Door Locked");
}