#ifndef _LEAPHY_BLE_H
#define _LEAPHY_BLE_H

#include <ArduinoBLE.h>

class LeaphyBLE
{
public:
  bool initialize(const char *deviceName);
  BLEBoolCharacteristic addBinaryCharacteristic(const char *name, bool initialValue);
  BLEStringCharacteristic addStringCharacteristic(const char *name, const char *initialValue);
  BLECharacteristic* getCharacteristicByName(const char *name);

private:
  String characteristicNames[10];
  BLECharacteristic* characteristics[10];
  int characteristicCount = 0;
};

#endif
