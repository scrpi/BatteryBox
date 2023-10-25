#include "dcdc.h"

static char buf[32];

struct dcdc {
	HardwareSerial *serial;
};

struct dcdc *new_dcdc(HardwareSerial *serial) {
	struct dcdc *dcdc = (struct dcdc*)calloc(1, sizeof(*dcdc));
	dcdc->serial = serial;
	return dcdc;
}

static void dcdc_send(struct dcdc *dcdc, const char *cmd) {
	dcdc->serial->print(cmd);
	dcdc->serial->print("\n");
	delay(50);
	dcdc->serial->readBytesUntil('\n', buf, sizeof(buf));
	delay(50);
}

static void dcdc_set(struct dcdc *dcdc, const char *cmd, float val) {
	uint16_t val2;
	strcpy(buf, cmd);
	val2 = (int)(val *= 100.f);
	snprintf(&buf[strlen(cmd)], sizeof(buf) - strlen(cmd), "%d", val2);
	dcdc_send(dcdc, buf);
}

static const char *dcdc_read(struct dcdc *dcdc, const char *msg) {
	dcdc->serial->print(msg);
	dcdc->serial->print("\n");
	delay(50);
	dcdc->serial->readBytesUntil('\n', buf, sizeof(buf));
	buf[14] = 0;
	delay(50);
	return buf;
}

void dcdc_enable_output(struct dcdc *dcdc) {
	dcdc_send(dcdc, "awo1");
}

void dcdc_disable_output(struct dcdc *dcdc) {
	dcdc_send(dcdc, "awo0");
}

void dcdc_enable_lock(struct dcdc *dcdc) {
	dcdc_send(dcdc, "awl1");
}

void dcdc_disable_lock(struct dcdc *dcdc) {
	dcdc_send(dcdc, "awl0");
}

void dcdc_enable_display(struct dcdc *dcdc) {
	dcdc_send(dcdc, "awd1");
}

void dcdc_disable_display(struct dcdc *dcdc) {
	dcdc_send(dcdc, "awd0");
}

void dcdc_set_voltage(struct dcdc *dcdc, float v) {
	dcdc_set(dcdc, "awu", v);
}

void dcdc_set_current(struct dcdc *dcdc, float i) {
	dcdc_set(dcdc, "awi", i);
}

float dcdc_get_set_voltage(struct dcdc *dcdc) {
	const char *msg = dcdc_read(dcdc, "arv");
	float v = (float)atoi(&msg[3]) / 100.f;
	return v;
}

float dcdc_get_set_current(struct dcdc *dcdc) {
	const char *msg = dcdc_read(dcdc, "ara");
	float i = (float)atoi(&msg[3]) / 100.f;
	return i;
}

float dcdc_get_voltage(struct dcdc *dcdc) {
	const char *msg = dcdc_read(dcdc, "aru");
	float v = (float)atoi(&msg[3]) / 100.f;
	return v;
}

float dcdc_get_current(struct dcdc *dcdc) {
	const char *msg = dcdc_read(dcdc, "ari");
	float i = (float)atoi(&msg[3]) / 100.f;
	return i;

}

bool dcdc_get_output(struct dcdc *dcdc) {
	const char *msg = dcdc_read(dcdc, "aro");
	return msg[13] == '1';
}

bool dcdc_get_lock(struct dcdc *dcdc) {
	const char *msg = dcdc_read(dcdc, "arl");
	return msg[13] == '1';
}