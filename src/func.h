#include <Wire.h> 
#include <LCD_1602_RUS.h>

LCD_1602_RUS lcd(0x27,16,2);

void beginning() {
    lcd.begin();
    lcd.clear();
	lcd.setCursor(0, 0);
    lcd.print("ТАЙМЕР ДЛЯ");
  	lcd.setCursor(0, 1);
	lcd.print("ТРЕНИРОВОК");
	delay(2000);

	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("СОСТОЯНИЕ:");
	lcd.setCursor(0, 1);
	lcd.print("ГОТОВ К РАБОТЕ");
	delay(2000);
}