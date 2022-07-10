#include "LedControl.h"
#include <binary.h>

LedControl led = LedControl(13, 12, 11, 4);

//int fillRow = 0;

byte H[8] = {
  0b01000010,
  0b01000010,
  0b01000010, 
  0b01111110, 
  0b01000010,
  0b01000010,
  0b01000010, 
  0b01000010
};

byte blank[8] = {
  0b00000000,
  0b00000000,
  0b00000000, 
  0b00000000, 
  0b00000000,
  0b00000000,
  0b00000000, 
  0b00000000
};


byte e[8] = {
  0b00000000,
  0b00000000,
  0b00111000,
  0b00100100,
  0b00100100,
  0b00111000,
  0b00100000,
  0b00111100};

byte l[8] = {
  0b00000000,
  0b00000000,
  0b00010000,
  0b00010000,
  0b00010000,
  0b00010000,
  0b00010000,
  0b00011000};

byte o[8] = {
  0b00000000,
  0b00000000,
  0b00000000,
  0b00111000,
  0b01000100,
  0b01000100,
  0b01000100,
  0b00111000};

byte W[8] = {
  0b10010010,
  0b10010010,
  0b10010010,
  0b10010010,
  0b10010010,
  0b10010010,
  0b10010010,
  0b11111110};

byte r[8] = {
  0b00000000,
  0b00000000,
  0b01000000,
  0b01111000,
  0b01000100,
  0b01000000,
  0b01000000,
  0b01000000};

byte d[8] = {
  0b00000000,
  0b00001000,
  0b00001000,
  0b00001000,
  0b00001000,
  0b00111000,
  0b00101000,
  0b00111000};

byte ex[8] = {
  0b00010000,
  0b00010000,
  0b00010000,
  0b00010000,
  0b00010000,
  0b00010000,
  0b00000000,
  0b00010000};

byte* text[18] = {
    blank,blank,blank,H,e,l,l,o,blank,W,o,r,l,d,ex,blank,blank,blank
 };

void setup() {
  for (int i = 0; i < 4; i++) {
  led.shutdown(i, false);
  led.setIntensity(i, 8);
  led.clearDisplay(i);
  }
}

//char text[12] = "Hello world!";

//void fill(){
//
//while (fillRow <= 7) {
//  for (int ledNo = 4; ledNo >= 0; ledNo--) {
//    for (int col = 0; col <= 7; col++) {
//      led.setLed(ledNo, fillRow, col, true);
//      delay(7); 
//      }
//    }
//    fillRow = fillRow + 1;
//  }
//  fillRow = 0;
//}
//
//void erase() {
//  for (int ledNo = 4; ledNo >= 0; ledNo--) {
//    for (int col = 0; col <= 7; col++) {
//      for (int row = 0; row <= 7; row++) {
//        led.setLed(ledNo, row, col, false);
//        delay(7);
//      }  
//    }
//  }              
//}
//
//void displayText(inLetter) {
//  return inLetter[8];
//}
//
//int ledNo = 0;
//

// Inputs are pointer to the character to display and
// number of the display to print on
void printCharacter(byte *c, int d) {
    for (int row = 0; row <= 7; row++) {
      led.setRow(d, row, c[row]);
    }
}

void printTextAtIndex(int i) {
  byte *c0 = text[i];
  byte *c1 = text[i+1];
  byte *c2 = text[i+2];
  byte *c3 = text[i+3];

  // Here, we reverse the display order because the
  // displays are counted from right to left
  printCharacter(c0, 3);
  printCharacter(c1, 2);
  printCharacter(c2, 1);
  printCharacter(c3, 0);
}

void helloWorld() {
  for(int i=0;i<15;i++) {
    printTextAtIndex(i);
    delay(1000);
  }
}

void loop() {

//fill();
//erase();
helloWorld();
}
