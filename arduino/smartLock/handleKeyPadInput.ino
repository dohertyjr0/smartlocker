void handleKeypadInput(char key) {
  Serial.print("Key Pressed: ");
  Serial.println(key);

  switch (key) {

    case 'A': 
      lockDoor();
      displayLCD("Locked", "Manual Override");
      memset(enteredCode, 0, sizeof(enteredCode));
      codeIndex = 0;
      return;

    case 'C': 
      if (codeIndex > 0) {
        codeIndex--;
        lcd.setCursor(codeIndex, 1);
        lcd.print(' ');
        lcd.setCursor(codeIndex, 1);
      }
      return;

    default: 
      if (codeIndex < 4 && isDigit(key)) {
        enteredCode[codeIndex++] = key;
        lcd.setCursor(codeIndex - 1, 1);
        lcd.print('*');
      }

      if (codeIndex == 4) {
        enteredCode[4] = '\0'; 
        String entered = String(enteredCode);

        if (entered == currentPassword) {
          unlockDoor();
          displayLCD("Unlocked", "Welcome");
          delay(3000);
        }
        else if(entered == adminPassword){
          enterAdminMode();
        }
         else {
          displayLCD("Wrong PIN", "Try Again");
          delay(2000);
        }

        memset(enteredCode, 0, sizeof(enteredCode));
        codeIndex = 0;
        displayLCD("Enter PIN", "");
      }
  }
}

