#include <Arduino.h>

HardwareSerial Serial2(USART2); // PA3  (RX)   PA2  (TX)
HardwareSerial Serial3(USART3); // PB11 (RX)   PB10 (TX)

void setup() {
	Serial.begin(115200);
	Serial2.begin(9600);
	Serial3.begin(9600);
}

void loop() {
}
