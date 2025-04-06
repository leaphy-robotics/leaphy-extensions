#include "LeaphyBLE.h"
#include <ArduinoBLE.h>
#include <utility/BLEUuid.h>

#define LEAPHY_BLE_SERVICE_UUID "a3a671c0-a063-444d-86b9-34fd0255d897" // This Bluetooth service should be the same for all Leaphy devices. It is used to separate them from other Bluetooth devices.

BLEService leaphyService(LEAPHY_BLE_SERVICE_UUID);

bool LeaphyBLE::initialize(const char *deviceName)
{
  if(!BLE.begin()) {
    return false;
  }

  BLE.setLocalName(deviceName);
  BLE.addService(leaphyService);
  BLE.setAdvertisedService(leaphyService);
  BLE.advertise();
  return true;
}

BLEBoolCharacteristic LeaphyBLE::addBinaryCharacteristic(const char *name, bool initialValue)
{
    BLEUuid uuid(name); // Turn name into UUID
    BLEBoolCharacteristic* characteristic = new BLEBoolCharacteristic(uuid.str(), BLERead | BLEWrite);
    characteristic->writeValue(initialValue);
    leaphyService.addCharacteristic(*characteristic); // Hook into service

    characteristicNames[characteristicCount] = name;
    characteristics[characteristicCount] = characteristic;
    characteristicCount++;

    return *characteristic;
}

BLEStringCharacteristic LeaphyBLE::addStringCharacteristic(const char *name, const char *initialValue)
{
    BLEUuid uuid(name); // Turn name into UUID
    BLEStringCharacteristic* characteristic = new BLEStringCharacteristic(uuid.str(), BLERead | BLEWrite, 20);
    characteristic->writeValue(initialValue);
    leaphyService.addCharacteristic(*characteristic); // Add to main service

    characteristicNames[characteristicCount] = name;
    characteristics[characteristicCount] = characteristic;
    characteristicCount++;

    return *characteristic;
}

BLECharacteristic* LeaphyBLE::getCharacteristicByName(const char *name)
{
    for (int i = 0; i < characteristicCount; i++) {
        if (characteristicNames[i] == name) {
            return characteristics[i];
        }
    }

    return nullptr; // characteristic not found
}

void LeaphyBLE::scanForLeaphyDevices()
{
    // Start filtered scan for BLE with Leaphy UUID
    BLE.scanForUuid(LEAPHY_BLE_SERVICE_UUID);
}

void LeaphyBLE::stopScanning()
{
    BLE.stopScan();
}

// Check if the device has the Leaphy service UUID
bool LeaphyBLE::isALeaphy(BLEDevice device)
{
    if (device.hasService(LEAPHY_BLE_SERVICE_UUID)) {
        return true;
    }
    return false;
}
