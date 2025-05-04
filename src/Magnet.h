#ifndef MAGNET_H
#define MAGNET_H

#include "Arduino.h"

class Magnet {
    int d0;
    int a0;

   public:
    explicit Magnet(int d0 = -1, int a0 = -1);
    bool readD();
    int readA();
};

#endif