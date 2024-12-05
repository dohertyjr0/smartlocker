void handleKeypadInput(char key) {
  Serial.print("Key Pressed: ");
  Serial.println(key);

  if (key == 'A' && codeIndex > 0) { 
    enteredCode[codeIndex] = '\0'; 
    Serial.print("Entered PIN: ");
    Serial.println(enteredCode);

    if (String(enteredCode) == currentPassword) {
      Serial.println("Admin Mode Access Granted");
      enterAdminMode();
      unlockDoor();
    } else {
      Serial.println("Incorrect PIN!");
      displayLCD("Incorrect Pin!", "Try Again");
      delay(2000);
      displayLCD("Welcome Back", "Door Locked");
    }

    memset(enteredCode, 0, sizeof(enteredCode));
    codeIndex = 0;
  } else if (key == 'C' && codeIndex > 0) { 
    codeIndex--;
    lcd.setCursor(codeIndex, 1);
    lcd.print(' ');
    Serial.println("Last digit cleared.");
  } else if (key != 'A' && key != 'C' && codeIndex < 4) { 
    enteredCode[codeIndex] = key;
    lcd.setCursor(codeIndex, 1);
    lcd.print('*'); 
    codeIndex++;
    
    Serial.print("Current PIN Entry: ");
    Serial.println(enteredCode);
  }
}
