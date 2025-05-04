#include "OLED_Display.h"
#include "Ledstrip.h"
#include "RedMP3.h"

LEDStrip ledstrip(1, 60);

MP3 mp3(7, 8);
uint16_t RawColor_Red, RawColor_Green, RawColor_Blue, RawColor_Clear;
byte Color_Red, Color_Green, Color_Blue, Color_Clear;

OLEDDisplay display;
void leaphyProgram() {
  _speed = 1;
  ledstrip.runFunction(4, 255, 255, 255);
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("Hallo");
  display.display();
  delay(500);
  mp3.playWithVolume(0x01, 0x1a);
}

void setup() {
  Serial.begin(115200);
  if(!display.begin())
  {
    Serial.println(F("Contact with the display failed: Check the connections"));
  }

  leaphyProgram();
}

void loop() {

}
