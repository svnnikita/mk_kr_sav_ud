#include <Wire.h> 
#include <LCD_1602_RUS.h>
//#include "func.h"

constexpr int output_pin {8}; 	// сигнальный вывод зуммера 
constexpr float freq1 {440.0}; 	// первые 3 сигнала 
constexpr float freq2 {493.88}; // сигнал старта или сигнал паузы
constexpr int delay_sec {1000}; // задержка

int numOfAppr; 		// количество подходов
int timeOfAppr; 	// время одного подхода
int timeOfPause; 	// время паузы между подходами

// инициализируем дисплей, имеющий 2 строки по 16 символов (16 столбцов) в каждой и с адресом I2C – 0x27
LCD_1602_RUS lcd(0x27,16,2);

void setup() {	
	Serial.begin(115200);
	tone(output_pin, freq2, delay_sec);

	lcd.begin();
	lcd.home();
	lcd.print("ТАЙМЕР ДЛЯ");
	lcd.setCursor(0, 1);
	lcd.print("ТРЕНИРОВОК");
	delay(2000);

	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("СОСТОЯНИЕ: ГОТОВ");
	lcd.setCursor(0, 1);
	lcd.print("К РАБОТЕ");
	delay(2000);

	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("НАЖМИТЕ 0 ДЛЯ");
	lcd.setCursor(0, 1);
	lcd.print("ВЫБОРА ПАРАМЕТРА");
	delay(2000);
}

void loop() {
	while (Serial.available() == 0) {}

	int Count = Serial.parseInt();

	switch (Count) {
		case 1:
			lcd.clear();
			lcd.setCursor(0, 0);
			lcd.print("ВВЕДИТЕ КОЛ-ВО");
			lcd.setCursor(0, 1);
			lcd.print("ПОДХОДОВ:");
			lcd.blink();
			numOfAppr = Serial.parseInt();
			lcd.print(numOfAppr);
			delay(1000);
			break;
		case 2:
			lcd.clear();
			lcd.setCursor(0, 0);
			lcd.print("ВВЕДИТЕ ВРЕМЯ ОД");
			lcd.setCursor(0, 1);
			lcd.print("НОГО ПОДХОДА:");
			lcd.blink();
			timeOfAppr = Serial.parseInt();
			lcd.print(timeOfAppr);
			delay(1000);
			break;
		case 3:
			lcd.clear();
			lcd.setCursor(0, 0);
			lcd.print("ВВЕДИТЕ ВРЕМЯ");
			lcd.setCursor(0, 1);
			lcd.print("ПАУЗЫ:");
			lcd.blink();
			timeOfPause = Serial.parseInt();
			lcd.print(timeOfPause);
			delay(1000);
			break;
		case 0:
			lcd.noBlink();
			lcd.clear();
			lcd.setCursor(0, 0);
			lcd.print("ВЫБРАТЬ ДАННЫЕ");
			lcd.setCursor(0, 1);
			delay(2000);

			lcd.clear();
			lcd.setCursor(0, 0);
			lcd.print("КОЛИЧЕСТВО");
			lcd.setCursor(0, 1);
			lcd.print("ПОДХОДОВ: 1");
			delay(2000);

			lcd.clear();
			lcd.setCursor(0, 0);
			lcd.print("ВРЕМЯ ОДНОГО");
			lcd.setCursor(0, 1);
			lcd.print("ПОДХОДА: 2");
			delay(2000);

			lcd.clear();
			lcd.setCursor(0, 0);
			lcd.print("ВРЕМЯ ");
			lcd.setCursor(0, 1);
			lcd.print("ПАУЗЫ: 3");
			delay(2000);
			break;
		default:
			break;
		}
		
}
