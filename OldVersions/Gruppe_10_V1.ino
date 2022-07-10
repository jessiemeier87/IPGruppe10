#include "LedControl.h"
#include <binary.h>

LedControl led = LedControl(13, 12, 11, 4);

int fillRow = 0;

void setup() {
  for (int i = 0; i < 4; i++) {
  led.shutdown(i, false);
  led.setIntensity(i, 8);
  led.clearDisplay(i);
  }
}



void fill(){

while (fillRow <= 7) {
  for (int ledNo = 4; ledNo >= 0; ledNo--) {
    for (int col = 0; col <= 7; col++) {
      led.setLed(ledNo, fillRow, col, true);
      delay(7); 
      }
    }
    fillRow = fillRow + 1;
  }
  fillRow = 0;
}

void erase() {
  for (int ledNo = 4; ledNo >= 0; ledNo--) {
    for (int col = 0; col <= 7; col++) {
      for (int row = 0; row <= 7; row++) {
        led.setLed(ledNo, row, col, false);
        delay(7);
      }  
    }
  }              
}

void loop() {

fill();
erase();

}
