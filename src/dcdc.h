#ifndef _BOX_DCDC_
#define _BOX_DCDC_

#include <Arduino.h>

struct dcdc;

struct dcdc *new_dcdc(HardwareSerial *uart);

void enable_output();
void disable_output();
void enable_lock();
void disable_lock();
void enable_display();
void disable_display();

void set_voltage(float v);
void set_current(float a);

float get_set_voltage();
float get_set_current();
float get_voltage();
float get_current();
bool get_output();

#endif