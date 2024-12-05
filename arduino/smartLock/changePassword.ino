void changePassword() {
  displayLCD("Enter New PIN", "");
  String newPassword = "";

  while (newPassword.length() < 4) {
    char key = keypad.getKey();
    if (key) {
      newPassword += key;
      lcd.setCursor(newPassword.length() - 1, 1);
      lcd.print('*');
    }
  }

  currentPassword = newPassword;
  displayLCD("PIN Changed", ""); 
  digitalWrite(YELLOW_LED_PIN, HIGH); 
  delay(1000);
  digitalWrite(YELLOW_LED_PIN, LOW);
  delay(1000); 
  digitalWrite(YELLOW_LED_PIN, HIGH); 
  delay(1000);
  digitalWrite(YELLOW_LED_PIN, LOW);
  lockDoor(); 
}
