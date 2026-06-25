#include "RedMP3.h"
#include <SegmentDisplay.h>

MP3 mp3(7, 8);
TM1637Display segment_display(10, 11);

void leaphyProgram() {
  delay(500);
  mp3.playWithVolume(0x01, 0x1a);
  segment_display.showNumberDec(8);
}

void setup() {
  Serial.begin(115200);
  segment_display.setBrightness(255);

  leaphyProgram();
}

void loop() {

}
