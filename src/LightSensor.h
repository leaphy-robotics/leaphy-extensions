#ifndef LIGHTSENSOR_H
#define LIGHTSENSOR_H

class LightSensor {
    int pin;

   public:
    explicit LightSensor(int pin);
    bool begin();
    int read();
};

#endif