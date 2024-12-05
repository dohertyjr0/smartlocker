void handleKeypadInput(char key) {
  Serial.print("Key Pressed: ");
  Serial.println(key);

  if (key == 'A' && codeIndex > 0) { //if 'A' key is pressed AND the current key position is greater than 0 ie. 1 key
    enteredCode[codeIndex] = '\0'; // adds terminate string
    Serial.print("Entered PIN: ");
    Serial.println(enteredCode); //prints the code entered to serial monitor(was placed for debugging purposes)

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
    
    memset(enteredCode, 0, sizeof(enteredCode)); //memset is a built in c function which is primarily used for modifying arrays, here i used it to reset the array 
    codeIndex = 0; //seperately reset my way of tracking key positions
  } else if (key == 'C' && codeIndex > 0) { 
    codeIndex--; //the char 'C' is intended as a backspace, hence --
    lcd.setCursor(codeIndex, 1); //this techincally prints to the lcd the current key position {3, 2, 1, 0}
    lcd.print(' '); //provides empty whitespace as placeholder for previous characters entered
    Serial.println("Last digit cleared."); //Was used as debugging purpose, due to issues with whitespace
  } else if (key != 'A' && key != 'C' && codeIndex < 4) { 
    enteredCode[codeIndex] = key;
    lcd.setCursor(codeIndex, 1);
    lcd.print('*'); 
    codeIndex++;
    
    Serial.print("Current PIN Entry: ");
    Serial.println(enteredCode);
  }
}
