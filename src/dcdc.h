#ifndef _BOX_DCDC_
#define _BOX_DCDC_

#include <Arduino.h>

struct dcdc;

struct dcdc *new_dcdc(HardwareSerial *serial);

void dcdc_enable_output(struct dcdc *dcdc);
void dcdc_disable_output(struct dcdc *dcdc);
void dcdc_enable_lock(struct dcdc *dcdc);
void dcdc_disable_lock(struct dcdc *dcdc);
void dcdc_enable_display(struct dcdc *dcdc);
void dcdc_disable_display(struct dcdc *dcdc);

void dcdc_set_voltage(struct dcdc *dcdc, float v);
void dcdc_set_current(struct dcdc *dcdc, float a);

float dcdc_get_set_voltage(struct dcdc *dcdc);
float dcdc_get_set_current(struct dcdc *dcdc);
float dcdc_get_voltage(struct dcdc *dcdc);
float dcdc_get_current(struct dcdc *dcdc);
bool dcdc_get_output(struct dcdc *dcdc);
bool dcdc_get_lock(struct dcdc *dcdc);

#endif