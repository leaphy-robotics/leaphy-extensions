#pragma once
//
//    FILE: SGP30.h
//  AUTHOR: Rob Tillaart
// VERSION: 0.2.0
//    DATE: 2021-06-24
// PURPOSE: Arduino library for SGP30 environment sensor.
//     URL: https://github.com/RobTillaart/SGP30
//          https://www.adafruit.com/product/3709

#include "Arduino.h"
#include "Wire.h"

#define AirQuality_OK 0x00
#define AirQuality_ERROR_CRC 0xFF
#define AirQuality_ERROR_I2C 0xFE

class AirQuality {
    uint8_t address;
    int error;
    uint32_t lastTime = 0;
    uint32_t lastRequest = 0;
    int command(uint16_t cmd);
    int command(uint16_t cmd, uint16_t v1);
    int command(uint16_t cmd, uint16_t v1, uint16_t v2);
    static uint8_t CRC8(uint16_t data);
    void init();

    uint16_t TVOC;
    uint16_t CO2;
    uint16_t H2;
    uint16_t ethanol;
    uint16_t srefH2 = 13119;
    uint16_t srefEth = 18472;

    TwoWire *wire;

   public:
    explicit AirQuality(TwoWire *wire = &Wire);
    bool begin();
    bool isConnected() const;
    void GenericReset();
    bool getID();
    uint16_t getFeatureSet();
    bool measureTest();
    uint32_t lastMeasurement() const { return lastTime; };
    bool measure(bool all = false);
    void request();
    bool read();
    void requestRaw();
    bool readRaw();

    uint16_t getTVOC() const { return TVOC; };           //   PPB
    uint16_t getCO2() const { return CO2; };             //   PPM
    uint16_t getH2Raw() const { return H2; };            //   UNKNOWN
    uint16_t getEthanolRaw() const { return ethanol; };  //   UNKNOWN
    float getH2() const;                                 //  experimental       //   PPM
    float getEthanol() const;                            //  experimental       //   PPM

    void setSrefH2(const uint16_t s = 13119) { srefH2 = s; };
    uint16_t getSrefH2() const { return srefH2; };
    void setSrefEthanol(const uint16_t s = 18472) { srefEth = s; };
    uint16_t getSrefEthanol() const { return srefEth; };

    int lastError();
    uint8_t id[6];
};

//  -- END OF FILE --
