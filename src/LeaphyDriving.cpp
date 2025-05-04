// Leaphy_Driving.cpp
// suitable for Fundumoto Shield
// Copyright Science4Kids& Stichting Leaphy 2018
// version 1.0.0    25 JULI 2018
#include "Leaphy_Driving.h"

float getDistanceSonar(const int trigPin, const int echoPin) {
    pinMode(trigPin, OUTPUT);
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    const auto duration = static_cast<float>(pulseIn(echoPin, HIGH, 30000));
    double distance = 0.034 * duration / 2;
    if (distance == 0) {
        distance = distance + 1313;
    }

    return static_cast<float>(distance);
}

/* Allow motor pins to be overridden for different robot types */
void setMotorPins(const int m1_pwm, const int m1_dir, const int m2_pwm, const int m2_dir) {
    motor_pins.m1_pwm = m1_pwm;
    motor_pins.m1_dir = m1_dir;
    motor_pins.m2_pwm = m2_pwm;
    motor_pins.m2_dir = m2_dir;
}

void setMotor(const int fpMotor, const int fpSpeed) {
    /* Select right pins for the given motor */
    const uint8_t dirPin = fpMotor == MOTOR_LEFT    ? motor_pins.m1_dir
                           : fpMotor == MOTOR_RIGHT ? motor_pins.m2_dir
                                                    : -1;
    const uint8_t pwnPin = fpMotor == MOTOR_LEFT    ? motor_pins.m1_pwm
                           : fpMotor == MOTOR_RIGHT ? motor_pins.m2_pwm
                                                    : -1;
    /* Set DIR and PWM pins */
    pinMode(dirPin, OUTPUT);
    pinMode(pwnPin, OUTPUT);
    const int speed = abs(fpSpeed);
    digitalWrite(dirPin, fpSpeed > 0 ? 1 : 0);
    analogWrite(pwnPin, speed);
}

void moveMotors(const int fpDirection, const int fpSpeed) {
    switch (fpDirection) {
        case FORWARD:
            setMotor(MOTOR_LEFT, fpSpeed);
            setMotor(MOTOR_RIGHT, fpSpeed);
            break;
        case BACKWARD:
            setMotor(MOTOR_LEFT, -fpSpeed);
            setMotor(MOTOR_RIGHT, -fpSpeed);
            break;
        case LEFT:
            setMotor(MOTOR_LEFT, -fpSpeed);
            setMotor(MOTOR_RIGHT, fpSpeed);
            break;
        case RIGHT:
            setMotor(MOTOR_LEFT, fpSpeed);
            setMotor(MOTOR_RIGHT, -fpSpeed);
            break;
    }
}
