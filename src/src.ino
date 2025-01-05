#include <Wire.h> 
#include <LCD_1602_RUS.h>
#include "func.h"

constexpr int output_pin {8}; 	// сигнальный вывод зуммера 
constexpr float freq1 {440.0}; 	// первые 3 сигнала 
constexpr float freq2 {493.88}; // сигнал старта или сигнал паузы
constexpr int delaySec {1000}; 	// задержка

// объявляем массив из 3-х значений: 
// 1) количество подходов
// 2) время одного подхода
// 3) время паузы между подходами
int arrayOfParam[3];

// инициализируем дисплей, имеющий 2 строки по 16 символов (16 столбцов) в каждой и с адресом I2C – 0x27
//LCD_1602_RUS lcd(0x27,16,2);

void setup() {	
	Serial.begin(9600);
	tone(output_pin, freq2, delaySec);

	lcd.begin();
	onDisplay("ТАЙМЕР ДЛЯ", "ТРЕНИРОВОК");
	delay(delaySec * 2);

	onDisplay("СОСТОЯНИЕ: ГОТОВ", "К РАБОТЕ");
	delay(delaySec * 2);
}

void loop() {
	onDisplay("ВВЕДИ", "ДАННЫЕ");
	delay(delaySec * 2);

	onDisplay("КОЛИЧЕСТВО ПОД", "ХОДОВ: q И ЧИСЛО");
	delay(delaySec * 2);

	onDisplay("ВРЕМЯ ОДНОГО ПОД", "ХОДА: w И ЧИСЛО");
	delay(delaySec * 2);

	onDisplay("ВРЕМЯ ПАУЗЫ:", "e И ЧИСЛО");
	delay(delaySec * 2);

	lcd.blink();
	delay(delaySec * 2);

	if (Serial.available() > 1) {
		char key = Serial.read();
		switch (key) {
		case 'q': 
			onDisplay("КОЛИЧЕСТВО", "ПОДХОДОВ:");
			arrayOfParam[0] = Serial.parseInt();
			lcd.print(arrayOfParam[0]);
			delay(delaySec * 2);
			break;
		case 'w':
			onDisplay("ВРЕМЯ ОДНОГО", "ПОДХОДА:");
			arrayOfParam[1] = Serial.parseInt();
			lcd.print(arrayOfParam[1]);
			delay(delaySec * 2); 
			break;
		case 'e':
			onDisplay("ВРЕМЯ", "ПАУЗЫ:");
			arrayOfParam[2] = Serial.parseInt();
			lcd.print(arrayOfParam[2]);
			delay(delaySec * 2); 
			break;
		default:
			onDisplay("НЕИЗВЕСТНЫЙ", "СИМВОЛ");
			delay(delaySec * 2); 
			break;
		}
	}

	for (int i = 0; i < 3; i++) {
		Serial.println(arrayOfParam[i]);
	}
}