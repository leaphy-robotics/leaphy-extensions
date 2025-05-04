#include "Ledstrip.h"

LEDStrip::LEDStrip(const uint8_t p, const uint16_t tot) {
    strip.begin();
    strip.setPin(p);
    strip.updateLength(tot);
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
}

int _speed = 50;
int _delayTime = 20;

void LEDStrip::runFunction(const uint8_t f, const uint8_t r, const uint8_t g, const uint8_t b) {
    switch (f) {
        case 0:
            singleColor(r, g, b);
            break;
        case 1:
            breathe(r, g, b);
            break;
        case 2:
            marquee(r, g, b);
            break;
        case 3:
            rainbow(r, g, b);
            break;
        case 4:
            rainbowMarquee();
            break;
        default:
            marquee(r, g, b);
            break;
    }
}

void LEDStrip::singleColor(const uint8_t r, const uint8_t g, const uint8_t b) {
    strip.setBrightness(255);
    for (uint16_t i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, Adafruit_NeoPixel::Color(r, g, b));
    }
    strip.show();
}

void LEDStrip::breathe(const uint8_t r, const uint8_t g, const uint8_t b) {
    _delayTime = (101 - _speed);
    // set to dark
    strip.setBrightness(1);
    strip.show();
    for (int i = 0; i < 255; i++) {
        strip.setBrightness(i);
        for (uint16_t pixel = 0; pixel < strip.numPixels(); pixel++) {
            strip.setPixelColor(pixel, Adafruit_NeoPixel::Color(r, g, b));
        }
        strip.show();
        delay(_delayTime / 10);
    }

    delay(40);

    for (int i = 255; i >= 0; i--) {
        strip.setBrightness(i);
        for (uint16_t pixel = 0; pixel < strip.numPixels(); pixel++) {
            strip.setPixelColor(pixel, Adafruit_NeoPixel::Color(r, g, b));
        }
        strip.show();
        delay(_delayTime / 10);
    }

    delay(1);
}

void LEDStrip::marquee(const uint8_t r, const uint8_t g, const uint8_t b) {
    uint16_t scale;
    strip.setBrightness(255);
    _delayTime = (101 - _speed);
    for (int i = 0; i < static_cast<int>(strip.numPixels()) + 4; i++) {
        // light up 5 leds at a time with dim color
        for (int j = 0; j < 5; j++) {
            if (i - j >= 0 && i - j < static_cast<int>(strip.numPixels())) {
                scale = 255 - 80 * j >= 0 ? 255 - 80 * j : 0;
                const uint8_t r1 = r * scale >> 8;
                const uint8_t g1 = g * scale >> 8;
                const uint8_t b1 = b * scale >> 8;
                strip.setPixelColor(i - j, Adafruit_NeoPixel::Color(r1, g1, b1));
            }
        }
        strip.show();
        delay(_delayTime);
    }
    for (int pixel = static_cast<int>(strip.numPixels()) - 1; pixel >= -4; pixel--) {
        // light up 5 leds at a time with dim color
        for (int offset = 0; offset < 5; offset++) {
            if (pixel + offset >= 0 && pixel + offset < static_cast<int>(strip.numPixels())) {
                scale = 255 - 80 * offset >= 0 ? 255 - 80 * offset : 0;
                const uint8_t r1 = r * scale >> 8;
                const uint8_t g1 = g * scale >> 8;
                const uint8_t b1 = b * scale >> 8;
                strip.setPixelColor(pixel + offset, Adafruit_NeoPixel::Color(r1, g1, b1));
            }
        }
        strip.show();
        delay(_delayTime);
    }
}

void LEDStrip::rainbow(uint8_t r, uint8_t g, uint8_t b) {
    _delayTime = (101 - _speed);
    strip.setBrightness(255);
    for (uint16_t color = 0; color < 256; color++) {
        for (uint16_t pixel = 0; pixel < strip.numPixels(); pixel++) {
            strip.setPixelColor(pixel, Wheel((pixel + color) & 255));
        }
        strip.show();
        delay(_delayTime);
    }
}

void LEDStrip::rainbowMarquee() {
    _delayTime = (101 - _speed);
    strip.setBrightness(255);
    for (int color = 0; color < 256; color++) {  // cycle all 256 colors in the wheel
        for (int q = 0; q < 3; q++) {
            for (uint16_t pixel = 0; pixel < strip.numPixels(); pixel = pixel + 3) {
                strip.setPixelColor(pixel + q,
                                    Wheel((pixel + color) % 255));  // turn every third pixel on
            }
            strip.show();
            delay(_delayTime * 20);

            for (uint16_t pixel = 0; pixel < strip.numPixels(); pixel = pixel + 3) {
                strip.setPixelColor(pixel + q, 0);  // turn every third pixel off
            }
        }
    }
}
uint32_t LEDStrip::Wheel(byte WheelPos) {
    if (WheelPos < 85) {
        return Adafruit_NeoPixel::Color(WheelPos * 3, 255 - WheelPos * 3, 0);
    }
    if (WheelPos < 170) {
        WheelPos -= 85;
        return Adafruit_NeoPixel::Color(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    WheelPos -= 170;
    return Adafruit_NeoPixel::Color(0, WheelPos * 3, 255 - WheelPos * 3);
}

void LEDStrip::basis(const uint8_t led, const uint8_t r, const uint8_t g, const uint8_t b) {
    strip.setPixelColor(led, r, g, b);
    strip.show();
}
