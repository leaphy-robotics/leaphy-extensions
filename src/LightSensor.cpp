#include "LightSensor.h"

#include <Arduino.h>

LightSensor::LightSensor(const int pin) { this->pin = pin; }

bool LightSensor::begin() {
    pinMode(pin, INPUT);
    return true;
}

int LightSensor::read() { return analogRead(pin); }