//#include <Wire.h> 
#include <LCD_1602_RUS.h>

constexpr int zummer {8}; 	// Сигнальный вывод зуммера 
constexpr float freq1 {440.0}; 	// Первые 3 сигнала 
constexpr float freq2 {493.88}; // Сигнал старта или сигнал паузы
constexpr int oneSec {1000}; 	// Задержка

unsigned long timing = 0;

// Устанавливаем флажки для управления отображением информацией на дисплее
// и выполнения частей кода:
int flag1 = 0; 		// Отвечает за отображение приветственных надписей
byte flag2 = 0; 	// Отвечает за ожидание программы ввода данных
byte flag3 = 0;		// Отвечает за режим таймера "ПОДХОД"
byte flag4 = 0; 	// Отвечает за начало паузы между подходами
byte flag5 = 0;		// Отвечает за проверку условий продолжения таймера
byte flag6 = 0;		// Аналогично flag5

// Объявляем массив значений, где: 
// элемент 1 -- количество подходов
// элемент 2 -- время одного подхода
// элемент 3 -- время паузы между подходами
unsigned int arrayOfParam[3];

// Создаем указатели на каждый элемент массива для управления таймером
unsigned int* pNumOfAppr = &arrayOfParam[0]; 	// количество подходов
unsigned int* pTimeOfAppr = &arrayOfParam[1];	// время одного подхода
unsigned int* pTimeOfPause = &arrayOfParam[2];	// время паузы

// Создаем массив из 2-х элементов и копируем значения элементов 2 и 3  
// из предыдущего массива для дальнейшего взаимодействия
unsigned int arrayOfParamCopy[2];

// Создаем указатели на элементы массива-копии
unsigned int* pTimeOfApprCopy = &arrayOfParamCopy[0];	// время одного подхода
unsigned int* pTimeOfPauseCopy = &arrayOfParamCopy[1];	// время паузы

// инициализируем дисплей, имеющий 2 строки по 16 символов (16 столбцов) 
// в каждой и с адресом I2C – 0x27
LCD_1602_RUS lcd(0x27,16,2);

// Шаблон функции для вывода информации на дисплей
template <typename S, typename N>
void onDisplay(S str1, N str2) {
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print(str1);
	lcd.setCursor(0, 1);
	lcd.print(str2);
}

void setup() {	
	Serial.begin(9600);
	tone(zummer, freq2, oneSec);
	lcd.begin();
	lcd.blink();
}

void loop() {
	// Выводим на дисплей приветственные надписи
	if (millis() - timing > oneSec * 2 && flag1 == 0 && flag2 == 0) {
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
		char KEY = Serial.read();
		switch (KEY) {
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
			arrayOfParam[1] = arrayOfParamCopy[0] = Serial.parseInt();
			lcd.print(arrayOfParam[1]);
			delay(oneSec * 2); 
			break;
		// e + число -- количество подходов, третье значение в массиве arrayOfParam[]
		case 'e':
			onDisplay("ВРЕМЯ", "ПАУЗЫ:");
			arrayOfParam[2] = arrayOfParamCopy[1] = Serial.parseInt();
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

	// Начало работы таймера. Первый подход
	if (millis() - timing > oneSec && flag2 == 1 && flag3 == 0) {
		timing = millis();
		onDisplay("РАБОТАЕМ", *pTimeOfAppr);
		*pTimeOfAppr = *pTimeOfAppr - 1;
		
		// Если время подхода истекло, таймер издает сигнал о начале паузы
		if (*pTimeOfAppr == 0) {
			tone(zummer, freq2, oneSec / 2);

			flag3 = 1;
			flag4 = 1;
		}		
	}

	// Начало паузы. 
	if (millis() - timing > oneSec && flag4 == 1 && flag5 == 0) {
		timing = millis();
		onDisplay("ОТДЫХАЕМ", *pTimeOfPause);
		
		*pTimeOfPause = *pTimeOfPause - 1;

		// Условие окончания паузы
		if (*pTimeOfPause == 0) {
			tone(zummer, freq1, oneSec / 2);
			
			// Т.к. к этому времени истеки время подхода и паузы, обновляем
			// их значения из массива-копии 
			*pTimeOfAppr = *pTimeOfAppr + *pTimeOfApprCopy;
			*pTimeOfPause = *pTimeOfPause + *pTimeOfPauseCopy;

			*pNumOfAppr = *pNumOfAppr - 1;

			flag5 = 1;
		}
	}

	// Проверяем условия продолжения работы таймера
	if (millis() - timing > oneSec && flag5 == 1 && flag6 == 0) {
		if (*pNumOfAppr > 0) {
			flag2 = 1;
			flag3 = 0;
			flag5 = 0;
		}

		if (*pNumOfAppr == 0) {
			flag1 = 0;
			flag2 = 0;
			flag3 = 0;			
			flag4 = 0;
			flag5 = 0;
		}
	}
}

