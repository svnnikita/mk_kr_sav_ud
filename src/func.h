#include <Wire.h> 
#include <LCD_1602_RUS.h>

// инициализируем дисплей, имеющий 2 строки по 16 символов (16 столбцов) в каждой и с адресом I2C – 0x27
LCD_1602_RUS lcd(0x27,16,2);

template <typename S, typename N>
void onDisplay(S str1, N str2) {
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print(str1);
	lcd.setCursor(0, 1);
	lcd.print(str2);
}