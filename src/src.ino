int signal_output = 10;
int sec;

float short_signal = 329.63;  // Ми
float long_signal = 440.0;  // Ля

int delayy_short = 500;
int delayy_long = 600;

unsigned long last_time = millis();

void setup() {
    pinMode(signal_output, OUTPUT);
    Serial.begin(9600);

}

void loop() {
    if (Serial.available()) {
        if (sec = Serial.parseInt()) {
            for (byte i = 0; i < 3; i++) {

                tone(signal_output, short_signal);
                delay(delayy_short);

                noTone(signal_output);
                delay(delayy_short);

            }

            tone(signal_output, long_signal);
            delay(delayy_long);
            noTone(signal_output);
        }

        delay(sec);
        tone(signal_output, long_signal);

    }
    else {
        if (millis() - last_time > 1000) {
            last_time = millis();
            Serial.println("ожидание_времени");
        }
    };

}