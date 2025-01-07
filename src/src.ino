//#include <Wire.h> 
#include <LCD_1602_RUS.h>
#include "func.h"

constexpr int zummer {8}; 	// сигнальный вывод зуммера 
constexpr float freq1 {440.0}; 	// первые 3 сигнала 
constexpr float freq2 {493.88}; // сигнал старта или сигнал паузы
constexpr int delaySec {1000}; 	// задержка

unsigned long timing = 0;
int flag1 = 0;
int flag2 = 0;
int flag3 = 0;

// объявляем массив из 3-х значений: 
// 1) количество подходов
// 2) время одного подхода
// 3) время паузы между подходами
int arrayOfParam[3];

void setup() {	
	Serial.begin(9600);
	tone(zummer, freq2, delaySec);
	lcd.begin();
	lcd.blink();
}

void loop() {
	if (millis() - timing > delaySec * 2 && flag1 == 0 && flag2 == 0) {
		timing = millis();
		onDisplay("ТАЙМЕР ДЛЯ", "ТРЕНИРОВОК");
		flag1 = 1;
	}

	if (millis() - timing > delaySec * 2 && flag1 == 1  && flag2 == 0) {
		timing = millis();
		onDisplay("СОСТОЯНИЕ: ГОТОВ", "К РАБОТЕ");
		flag1 = 2;
	}

	if (millis() - timing > delaySec * 2 && flag1 == 2  && flag2 == 0) {
		timing = millis();
		onDisplay("ВВЕДИ", "ДАННЫЕ");
		flag1 = 3;
	}

	if (millis() - timing > delaySec * 2 && flag1 == 3  && flag2 == 0) {
		timing = millis();
		onDisplay("КОЛИЧЕСТВО ПОД", "ХОДОВ: q И ЧИСЛО");
		flag1 = 4;
	}

	if (millis() - timing > delaySec * 2 && flag1 == 4  && flag2 == 0) {
		timing = millis();
		onDisplay("ВРЕМЯ ОДНОГО ПОД", "ХОДА: w И ЧИСЛО");
		flag1 = 5;
	}

	if (millis() - timing > delaySec * 2 && flag1 == 5  && flag2 == 0) {
		timing = millis();
		onDisplay("ВРЕМЯ ПАУЗЫ:", "e И ЧИСЛО");
		flag1 = 0;
	}

	// проверяем, ввел ли пользователь какие-либо параметры (есть ли в буфере данные)
	if (Serial.available() > 1) {
		char key = Serial.read();
		switch (key) {
		// q + число (сразу после буквы) -- количество подходов, первое значение в массиве arrayOfParam[]
		case 'q': 
			onDisplay("КОЛИЧЕСТВО", "ПОДХОДОВ:");
			arrayOfParam[0] = Serial.parseInt();
			lcd.print((int)arrayOfParam[0]);
			delay(delaySec * 2);
			break;
		// w + число -- время одного подхода, второе значение в массиве arrayOfParam[]
		case 'w':
			onDisplay("ВРЕМЯ ОДНОГО", "ПОДХОДА:");
			arrayOfParam[1] = Serial.parseInt();
			lcd.print((int)arrayOfParam[1]);
			delay(delaySec * 2); 
			break;
		// e + число -- количество подходов, первое значение в массиве arrayOfParam[]
		case 'e':
			onDisplay("ВРЕМЯ", "ПАУЗЫ:");
			arrayOfParam[2] = Serial.parseInt();
			lcd.print((int)arrayOfParam[2]);
			delay(delaySec * 2); 
			break;
		// на случай, если пользователь введет некорректные данные
		default:
			onDisplay("НЕИЗВЕСТНЫЙ", "СИМВОЛ");
			delay(delaySec * 2); 
			break;
		}
	}

	// вывод значений массива arrayOfParam[] в последовательный порт
	//for (int i = 0; i < 3; i++) {
		//Serial.println(arrayOfParam[i]);
	//}

	// если каждый элемент массива не равен нулю (пользователь ввел 
	// все необходимые значения), таймер начинает свою работу 

	if (arrayOfParam[0] > 0 && arrayOfParam[1] > 0 && arrayOfParam[2] > 0 && flag2 == 0) {
		lcd.noBlink();
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("НАЧИНАЕМ!!!");
		delay(delaySec * 2);

		int j = 0;
		for ( ; j < 3; j++) {
			tone(zummer, freq1, delaySec / 2);
			delay(delaySec);
		}

		if (j = 3) {	
			tone(zummer, freq2, delaySec / 2);
		}
		flag2 = 1;
	}

	if (millis() - timing > delaySec && flag2 == 1) {
		timing = millis();

		int SEC = arrayOfParam[1];
		SEC = SEC + 1;
		if (SEC = 0) {
			flag3 = 1;
			lcd.clear();
			lcd.setCursor(0, 0);
			lcd.print("ОТДЫХАЕМ");	
		} 
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print(arrayOfParam[1]);
		}
}

