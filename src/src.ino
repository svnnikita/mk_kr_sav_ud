//#include <Wire.h> 
#include <LCD_1602_RUS.h>
#include "func.h"

constexpr int zummer {8}; 	// Сигнальный вывод зуммера 
constexpr float freq1 {440.0}; 	// Первые 3 сигнала 
constexpr float freq2 {493.88}; // Сигнал старта или сигнал паузы
constexpr int oneSec {1000}; 	// Задержка

unsigned long timing = 0;

/* 	Устанавливаем флажки для управления отображением информацией на дисплее
	и выполениния частей кода:

		flag1. Отвечает за отображение приветственных надписей. Значения:
	0 -- "ТАЙМЕР ДЛЯ ТРЕНИРОВОК",
	1 -- "СОСТОЯНИЕ: ГОТОВ К РАБОТЕ",
	2 -- "ВВЕДИ ДАННЫЕ",
	3 -- "КОЛИЧЕСТВО ПОДХОДОВ: q И ЧИСЛО",
	4 -- "ВРЕМЯ ОДНОГО ПОДХОДА: w И ЧИСЛО",
	5 -- "ВРЕМЯ ПАУЗЫ: e И ЧИСЛО".

		flag2. Отвечает за ожидание программы ввода данных. Значения:
	0 -- друг за другом на дисплей выводятся приветственные надписи и параллельно
	с этим программа ожидает ввода значений. В зависимости от значения на дисплей
	выводится надпись и число. Динамик при нуле молчит.
	1 -- флаг принимает значение единицы, когда все данные введены и прозвучали 4 
	сигнала о начале работы таймера.

		flag3. Отвечает за работу таймера и за промежуточные сигналы динамика. Значения:
	0 -- при значении нуля выполняется остальная часть программы. 
	1 -- таймер заканчивает свою работу. Значения остальных флагов сбрасываются в ноль.
*/
int flag1 = 0;
int flag2 = 0;
int flag3 = 0;
int flag4 = 0;

// Объявляем массив из 3-х значений: 
// 1) количество подходов
// 2) время одного подхода
// 3) время паузы между подходами
unsigned int arrayOfParam[3];

void setup() {	
	Serial.begin(9600);
	tone(zummer, freq2, oneSec);
	lcd.begin();
	lcd.blink();
}

void loop() {
	if (millis() - timing > oneSec * 2 && flag1 == 0 && flag2 == 0 && flag3 == 0) {
		timing = millis();
		onDisplay("ТАЙМЕР ДЛЯ", "ТРЕНИРОВОК");
		flag1 = 1;
	}

	if (millis() - timing > oneSec * 2 && flag1 == 1  && flag2 == 0) {
		timing = millis();
		onDisplay("СОСТОЯНИЕ: ГОТОВ", "К РАБОТЕ");
		flag1 = 2;
	}

	if (millis() - timing > oneSec * 2 && flag1 == 2  && flag2 == 0) {
		timing = millis();
		onDisplay("ВВЕДИ", "ДАННЫЕ");
		flag1 = 3;
	}

	if (millis() - timing > oneSec * 2 && flag1 == 3  && flag2 == 0) {
		timing = millis();
		onDisplay("КОЛИЧЕСТВО ПОД", "ХОДОВ: q И ЧИСЛО");
		flag1 = 4;
	}

	if (millis() - timing > oneSec * 2 && flag1 == 4  && flag2 == 0) {
		timing = millis();
		onDisplay("ВРЕМЯ ОДНОГО ПОД", "ХОДА: w И ЧИСЛО");
		flag1 = 5;
	}

	if (millis() - timing > oneSec * 2 && flag1 == 5  && flag2 == 0) {
		timing = millis();
		onDisplay("ВРЕМЯ ПАУЗЫ:", "e И ЧИСЛО");
		flag1 = 0;
	}

	// Проверяем, ввел ли пользователь какие-либо параметры (есть ли в буфере данные)
	if (Serial.available() > 1) {
		char key = Serial.read();
		switch (key) {
		// q + число (сразу после буквы) -- количество подходов, первое значение в массиве arrayOfParam[]
		case 'q': 
			onDisplay("КОЛИЧЕСТВО", "ПОДХОДОВ:");
			arrayOfParam[0] = Serial.parseInt();
			lcd.print(arrayOfParam[0]);
			delay(oneSec * 2);
			break;
		// w + число -- время одного подхода, второе значение в массиве arrayOfParam[]
		case 'w':
			onDisplay("ВРЕМЯ ОДНОГО", "ПОДХОДА:");
			arrayOfParam[1] = Serial.parseInt();
			lcd.print(arrayOfParam[1]);
			delay(oneSec * 2); 
			break;
		// e + число -- количество подходов, третье значение в массиве arrayOfParam[]
		case 'e':
			onDisplay("ВРЕМЯ", "ПАУЗЫ:");
			arrayOfParam[2] = Serial.parseInt();
			lcd.print(arrayOfParam[2]);
			delay(oneSec * 2); 
			break;
		// Если пользователь введел некорректные данные
		default:
			onDisplay("НЕИЗВЕСТНЫЙ", "СИМВОЛ");
			delay(oneSec * 2); 
			break;
		}
	}

	// Вывод значений массива arrayOfParam[] в последовательный порт
	//for (int i = 0; i < 3; i++) {
		//Serial.println(arrayOfParam[i]);
	//}

	// Если каждый элемент массива не равен нулю (пользователь ввел 
	// все необходимые значения), таймер начинает свою работу 
	if (arrayOfParam[0] > 0 && arrayOfParam[1] > 0 && arrayOfParam[2] > 0 && flag2 == 0) {
		lcd.noBlink();
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("НАЧИНАЕМ!!!");
		delay(oneSec * 2);

		int j = 0;
		for ( ; j < 3; j++) {
			tone(zummer, freq1, oneSec / 2);
			delay(oneSec);
		}

		if (j = 3) {	
			tone(zummer, freq2, oneSec / 2);
		}
		flag2 = 1;
	}

	// Создаем указатели на каждый элемент массива для управления таймером
	unsigned int* pNumOfAppr = &arrayOfParam[0]; 	// количество подходов
	unsigned int* pTimeOfAppr = &arrayOfParam[1];	// время одного подхода
	unsigned int* pTimeOfPause = &arrayOfParam[2];	// время паузы

	// Начало работы таймера. Первый подход
	if (millis() - timing > oneSec && flag2 == 1 && flag3 == 0) {
		timing = millis();
		*pTimeOfAppr = *pTimeOfAppr - 1;

		/* При условии, что заданное время одного подхода кончилось,
		флаг 3 устанавливается  единицу, и выполняется следующее условие - 
		звучит сигнал о начале паузы */
		if (*pTimeOfAppr == 0) {
			flag3 = 1;
		}

		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print(*pTimeOfAppr);
		}
	
	// Сигнал паузы. Флаг 3 устанавливается в положение 2, и начинается отсчет паузы
	if (millis() - timing > oneSec && flag3 == 1) {
		tone(zummer, freq2, oneSec / 2);
		flag3 = 2;
	}

	// Начало паузы.
	if (millis() - timing > oneSec && *pTimeOfAppr == 0 && flag3 == 2) {
		timing = millis();

		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("ОТДЫХАЕМ");

		*pTimeOfPause = *pTimeOfPause - 1;

		// По окончании паузы, если еще остались подходы, работа программы
		// продолжается. Флаг 3 устанавливается в значение 3
		if (*pTimeOfPause == 0 && *pNumOfAppr != 0) {
			flag3 = 3;
		}

		lcd.setCursor(0, 1);
		lcd.print(*pTimeOfPause);
		}	
		
	if (millis() - timing > oneSec && flag3 == 3 && flag4 == 0) {
		timing = millis();
		flag2 == 1;
		flag3 == 0;
		//if () {

		//}
		//flag3 = 2;
	}

}

