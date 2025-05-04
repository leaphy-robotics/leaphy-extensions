#include "AirQuality.h"

uint8_t AirQuality::CRC8(const uint16_t data) {
    uint8_t val[2];
    val[0] = data >> 8;
    val[1] = data & 0xFF;

    uint8_t crc = 0xFF;
    for (const uint8_t i : val) {
        crc ^= i;
        for (uint8_t b = 8; b > 0; b--) {
            if (crc & 0x80)
                crc = crc << 1 ^ 0x31;
            else
                crc <<= 1;
        }
    }
    return crc;
};

AirQuality::AirQuality(TwoWire *wire) : wire(wire) {
    address = 0x58;
    TVOC = 0;
    CO2 = 0;
    H2 = 0;
    ethanol = 0;

    lastTime = 0;
    error = AirQuality_OK;
}

bool AirQuality::begin() {
    wire->begin();
    if (!isConnected()) return false;
    init();
    return true;
}

bool AirQuality::isConnected() const {
    wire->beginTransmission(address);
    return (wire->endTransmission() == 0);
}

bool AirQuality::measure(const bool all) {
    if (millis() - lastTime < 1000) return false;
    lastTime = millis();

    request();
    delay(12);
    read();

    if (not all) return true;

    requestRaw();
    delay(25);
    readRaw();
    return true;
}

void AirQuality::request() {
    lastRequest = millis();
    command(0x2008);
}

bool AirQuality::read() {
    if (lastRequest == 0) return false;
    if (millis() - lastRequest < 13) return false;
    lastRequest = 0;

    if (wire->requestFrom(address, static_cast<uint8_t>(6)) != 6) {
        error = AirQuality_ERROR_I2C;
        return false;
    }
    CO2 = wire->read() << 8;
    CO2 += wire->read();
    uint8_t crc = wire->read();
    if (CRC8(CO2) != crc) {
        error = AirQuality_ERROR_CRC;
        return false;
    }
    TVOC = wire->read() << 8;
    TVOC += wire->read();
    crc = wire->read();
    if (CRC8(TVOC) != crc) {
        error = AirQuality_ERROR_CRC;
        return false;
    }
    error = AirQuality_OK;
    return true;
}

void AirQuality::requestRaw() {
    lastRequest = millis();
    command(0x2050);
}

bool AirQuality::readRaw() {
    if (lastRequest == 0) return false;
    if (millis() - lastRequest < 26) return false;
    lastRequest = 0;

    if (wire->requestFrom(address, static_cast<uint8_t>(6)) != 6) {
        error = AirQuality_ERROR_I2C;
        return false;
    }
    H2 = wire->read() << 8;
    H2 += wire->read();
    uint8_t crc = wire->read();
    if (CRC8(H2) != crc) {
        error = AirQuality_ERROR_CRC;
        return false;
    }
    ethanol = wire->read() << 8;
    ethanol += wire->read();
    crc = wire->read();
    if (CRC8(ethanol) != crc) {
        error = AirQuality_ERROR_CRC;
        return false;
    }
    error = AirQuality_OK;
    return true;
}

float AirQuality::getH2() const {
    constexpr float cref = 0.5;  //  ppm
    return cref * static_cast<float>(exp((srefH2 - H2) * 1.953125e-3));
}

float AirQuality::getEthanol() const {
    constexpr float cref = 0.4;  //  ppm
    return cref * static_cast<float>(exp((srefEth - ethanol) * 1.953125e-3));
}

int AirQuality::command(const uint16_t cmd) {
    wire->beginTransmission(address);
    wire->write(cmd >> 8);
    wire->write(cmd & 0xFF);
    error = wire->endTransmission();
    return error;
}

int AirQuality::command(const uint16_t cmd, const uint16_t v1) {
    wire->beginTransmission(address);
    wire->write(cmd >> 8);
    wire->write(cmd & 0xFF);
    wire->write(v1 >> 8);
    wire->write(v1 & 0xFF);
    wire->write(CRC8(v1));
    error = wire->endTransmission();
    return error;
}

int AirQuality::command(const uint16_t cmd, const uint16_t v1, const uint16_t v2) {
    wire->beginTransmission(address);
    wire->write(cmd >> 8);
    wire->write(cmd & 0xFF);
    wire->write(v1 >> 8);
    wire->write(v1 & 0xFF);
    wire->write(CRC8(v1));
    wire->write(v2 >> 8);
    wire->write(v2 & 0xFF);
    wire->write(CRC8(v2));
    error = wire->endTransmission();
    return error;
}
void AirQuality::init() { command(0x2003); };
