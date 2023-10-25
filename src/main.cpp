#include <Arduino.h>
#include "dcdc.h"

#define CHG_BAT_LOW 13.2f

#define CHG_BULK_V  14.6f
#define CHG_BULK_I  8.0f
#define CHG_BULK_V_END  14.4f

#define CHG_ABS_V   14.40f
#define CHG_ABS_I   6.0f
#define CHG_ABS_I_END 2.0f

#define CHG_END_DELAY (4 * 60) // 4 mins

HardwareSerial Serial2(USART2); // PA3  (RX)   PA2  (TX)

struct dcdc *dcdc;

void setup() {
	Serial.begin(115200);
	Serial2.begin(9600);

	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, HIGH);

	dcdc = new_dcdc(&Serial2);
	dcdc_disable_output(dcdc);
	dcdc_enable_display(dcdc);
	dcdc_enable_lock(dcdc);
	dcdc_set_voltage(dcdc, 12.0f);
	dcdc_set_current(dcdc, 0.5f);
	delay(3000);
}

enum {
	CHG_INIT,
	CHG_SELF_TEST,
	CHG_IDLE,
	CHG_BULK,
	CHG_ABSORB,
	CHG_END,
};

int chg_state = CHG_INIT;

void loop() {
	float val;
	int i;

	switch (chg_state) {
	case CHG_INIT:
		delay(10 * 1000);
		chg_state = CHG_BULK;
		break;
	case CHG_SELF_TEST:
		break;
	case CHG_IDLE:
		dcdc_disable_output(dcdc);
		digitalWrite(LED_BUILTIN, HIGH);
		Serial.println("IDLE");
		if (dcdc_get_voltage(dcdc) <= CHG_BAT_LOW) {
			chg_state = CHG_BULK;
		}
		break;
	case CHG_BULK:
		dcdc_set_voltage(dcdc, CHG_BULK_V);
		dcdc_set_current(dcdc, CHG_BULK_I);
		dcdc_enable_output(dcdc);
		digitalWrite(LED_BUILTIN, LOW);
		val = dcdc_get_voltage(dcdc);
		Serial.print("BULK: ");
		Serial.println(val);
		if (val >= CHG_BULK_V_END) {
			chg_state = CHG_ABSORB;
		}
		break;
	case CHG_ABSORB:
		dcdc_set_voltage(dcdc, CHG_ABS_V);
		dcdc_set_current(dcdc, CHG_ABS_I);
		dcdc_enable_output(dcdc);
		digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
		val = dcdc_get_current(dcdc);
		Serial.print("ABS: ");
		Serial.println(val);
		if (val < CHG_ABS_I_END) {
			chg_state = CHG_END;
		}
		break;
	case CHG_END:
		dcdc_disable_output(dcdc);
		digitalWrite(LED_BUILTIN, HIGH);
		Serial.println("END");
		for (i = 0; i < CHG_END_DELAY * 4; ++i) {
			digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
			delay(250);
		}
		chg_state = CHG_IDLE;
		break;
	}
	delay(1000);
}
