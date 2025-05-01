void changePassword() {
  int i;
  displayLCD("Enter New PIN", "");
  String newPassword = ""; //since "String" is techincally an object, was able to use .length() method 

  while (newPassword.length() < 4) {
    char key = keypad.getKey();
    if (key) {
      newPassword += key;
      lcd.setCursor(newPassword.length() - 1, 1); //added -1 as a simple way to make sure its being type in the correct key position *on lcd*
      lcd.print('*');
    }
  }

  currentPassword = newPassword; //stores our new password in a new string
  for(i=0;i<6; i++){
    digitalWrite(YELLOW_LED, HIGH); 
    delay(100);
    digitalWrite(YELLOW_LED, LOW);
    delay(100); 
  }
  displayLCD("PIN Changed", ""); 
  lockDoor(); 
}
