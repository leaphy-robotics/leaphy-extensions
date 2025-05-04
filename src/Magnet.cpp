#include "Magnet.h"

Magnet::Magnet(const int d0, const int a0) {
    this->d0 = d0;
    this->a0 = a0;
    pinMode(d0, INPUT);
    pinMode(a0, INPUT);
}

bool Magnet::readD() { return digitalRead(d0); }

int Magnet::readA() { return analogRead(a0); }