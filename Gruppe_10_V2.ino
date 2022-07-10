#include <stdio.h>
#include "LedControl.h"
#include <binary.h>

  LedControl led = LedControl(13, 12, 11, 4);

  #define DISPLAY_WIDTH 32
  #define CHAR_WIDTH 4

  unsigned long text[8] = {0b00000000000000000000000000000000,
                           0b00000000000000000000000000000000,
                           0b00000000000000000000000000000000,
                           0b00000000000000000000000000000000,
                           0b00000000000000000000000000000000,
                           0b00000000000000000000000000000000,
                           0b00000000000000000000000000000000,
                           0b00000000000000000000000000000000
                           };

  byte empty[8] = {0b0000, 0b0000, 0b0000, 0b0000, 0b0000, 0b0000, 0b0000, 0b0000};

  byte doubleDot[8] = {0b0000, 0b0000, 0b0001, 0b0000, 0b0000, 0b0001, 0b0000, 0b0000};
  
  byte* blinkingDots[2] = {doubleDot, empty};
  
  //define digits
  byte zero[8] = {0b0000, 0b0111, 0b0101, 0b0101, 0b0101, 0b0101, 0b0111, 0b0000};
  byte one[8] = {0b0000, 0b0010, 0b0110, 0b0010, 0b0010, 0b0010, 0b0111, 0b0000};
  byte two[8] = {0b0000, 0b0110, 0b0001, 0b0010, 0b0010, 0b0100, 0b0111, 0b0000};
  byte three[8] = {0b0000, 0b0111, 0b0001, 0b0001, 0b0111, 0b0001, 0b0111, 0b0000};
  byte four[8] = {0b0000, 0b0101, 0b0101, 0b0101, 0b0111, 0b0001, 0b0001, 0b0000};
  byte five[8] = {0b0000, 0b0111, 0b0100, 0b0100, 0b0111, 0b0001, 0b0111, 0b0000};
  byte six[8] = {0b0000, 0b0110, 0b0100, 0b0100, 0b0111, 0b0101, 0b0111, 0b0000};
  byte seven[8] = {0b0000, 0b0111, 0b0001, 0b0010, 0b0010, 0b0100, 0b0100, 0b0000};
  byte eight[8] = {0b0000, 0b0111, 0b0101, 0b0101, 0b0111, 0b0101, 0b0111, 0b0000};
  byte nine[8] = {0b0000, 0b0111, 0b0101, 0b0111, 0b0001, 0b0001, 0b0001, 0b0000};

  byte* digits[10] = {zero, one, two, three, four, five, six, seven, eight, nine};

  unsigned long currentMillis;
  unsigned long startMillis;
  
  unsigned long startMillis2;
  unsigned long currentMillis2;
  const unsigned long dotBlink = 2000;

  unsigned int secondShift = 0;

  int blinking = 0;

  int sek1 = 0;
  int sek10 = 0;
  int min1 = 0;
  int min10 = 0;
  int stu1 = 0;
  int stu10 = 0;

void displayText(unsigned long *t) {
    byte currentMatrix;
    for (int ledNo = 0; ledNo < 4; ledNo++) {
      for (int row = 0; row < 8; row++) {
        currentMatrix = (((t[row]) >> (ledNo * 8)) & 0b11111111);
        led.setRow(ledNo, row, currentMatrix);
      }
    }
 }

void ledClock() {
    unsigned long h1, h2, m1, m2, s1, s2, d1, d2;
    for (int i = 0; i < 8; i++) {
      h1 = ((unsigned long)((digits[stu10])[i])) << DISPLAY_WIDTH - CHAR_WIDTH;
      h2 = ((unsigned long)((digits[stu1])[i])) << DISPLAY_WIDTH - (CHAR_WIDTH * 2);
      d1 = ((unsigned long)((blinkingDots[blinking])[i])) << DISPLAY_WIDTH - (CHAR_WIDTH * 3);
      m1 = ((unsigned long)((digits[min10])[i])) << DISPLAY_WIDTH - (CHAR_WIDTH * 4);
      m2 = ((unsigned long)((digits[min1])[i])) << DISPLAY_WIDTH - (CHAR_WIDTH * 5);
      d2 = ((unsigned long)((blinkingDots[blinking])[i])) << DISPLAY_WIDTH - (CHAR_WIDTH * 6);
      s1 = ((unsigned long)((digits[sek10])[i])) << DISPLAY_WIDTH - (CHAR_WIDTH * 7);
      s2 = ((unsigned long)((digits[sek1])[(i+secondShift)%8])) << DISPLAY_WIDTH - (CHAR_WIDTH * 8);
      text[i] = h1 + h2 + d1 + m1 + m2 + d2 + s1 + s2;
    }
    displayText(text);

    secondShift+=1;
    if(secondShift >= 8) {
      secondShift = 0;
    }
}

void calcTime() {

    int curSec = currentMillis/1000;
    int hours = curSec/3600;
    int hoursRest = curSec % 3600;
    int minutes = hoursRest/60;
    int seconds = hoursRest % 60;
  
    stu10 = hours/10;
    stu1 = hours%10;
    min10 = minutes/10;
    min1 = minutes%10;
    sek10 = seconds/10;
    sek1 = seconds%10;
      
  currentMillis2 = millis();
  if (currentMillis2 - startMillis2 >= dotBlink) {
    startMillis2 = currentMillis2;
    blinking = (blinking + 1) % 2;
  }
}

void setup() {
  
  for (int i = 0; i < 4; i++) {
    led.shutdown(i, false);
    led.setIntensity(i, 0);
    led.clearDisplay(i);
  }
  
   Serial.begin(115200);
   startMillis = millis();
   
}

void loop() {
  currentMillis = millis();
  if (currentMillis - startMillis >= 125) {
    calcTime();
    ledClock();
    startMillis=currentMillis;
  }
}
