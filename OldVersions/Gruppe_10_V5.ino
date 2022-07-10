  #include <stdio.h>
  #include "LedControl.h"
  #include <binary.h>

  LedControl led = LedControl(13, 12, 11, 4);

  #define TRIGPIN 4
  #define ECHOPIN 3
  #define MICRO_DELAY 10

  #define RED_OUTPUT 10
  #define YELLOW_OUTPUT 9
  #define GREEN_OUTPUT 8

  #define LSENSOR 6
  #define BUZZER 7
  #define BUTTON 2
  #define DEBOUNCE_DELAY 50

  #define DISPLAY_WIDTH 32
  #define CHAR_WIDTH 4

  // regular bool + toggling wasn't working; tried volatile after reading
  // this thread: https://forum.arduino.cc/t/changing-a-variable-flag-with-an-interrupt/690074
  volatile bool buttonToggle = true;

  unsigned long currentMillis, startMillis, lightMillis, lastMeasurement;

  //ul for blinking dots
  unsigned long startMillis2;
  unsigned long currentMillis2;
  const unsigned long dotBlink = 2000;

  //ul for interrupt_function
  unsigned long buttonTime = 0;
  unsigned long lastButtonTime = 0;

  // secondShift increases every 125 miliseconds (1000/8), so that we can move the bits
  //for the right-most second counter from the bottom up
  unsigned int secondShift = 0;

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

  //define time letters

  byte Z[8] = {0b0000, 0b0000, 0b1110, 0b0010, 0b0100, 0b1000, 0b1110, 0b0000};
  byte e[8] = {0b0000, 0b0000, 0b0111, 0b0101, 0b0111, 0b0100, 0b0011, 0b0000};
  byte i[8] = {0b0000, 0b0000, 0b0010, 0b0000, 0b0010, 0b0010, 0b0010, 0b0000};
  byte t[8] = {0b0000, 0b0000, 0b0010, 0b0111, 0b0010, 0b0010, 0b0011, 0b0000};
  byte lessThan[8] = {0b0000, 0b0000, 0b0010, 0b0100, 0b1000, 0b0100, 0b0010, 0b0000};
  byte greaterThan[8] = {0b0000, 0b0000, 0b0100, 0b0010, 0b0001, 0b0010, 0b0100, 0b0000};

  byte* zeitLetters[8] = {lessThan, lessThan, Z, e, i, t, greaterThan, greaterThan};

  int blinking = 0;

  int sek1 = 0;
  int sek10 = 0;
  int min1 = 0;
  int min10 = 0;
  int stu1 = 0;
  int stu10 = 0;

  /* Gets executed when the button is pressed */
  void interrupt_function() {
    buttonTime = millis(); // sets 'buttonTime' to millis()
    /*
    check if buttonTime - lastButtonTime is > than 300 ms; if it is, then
    we know that the current state has changed
    */
    if ((buttonTime - lastButtonTime) > 300) { 
      if (buttonToggle == true) {
        buttonToggle = false;
      } else {
        buttonToggle = true;
      }
      lastButtonTime = buttonTime; // set lastButtonTime, so we can detect next change
    }
  }

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

  void displayZeit() {
    unsigned long char1, char2, char3, char4, char5, char6, char7, char8;
    for (int j = -8; j < 8; j++) {
      for (int i = 0; i < 8; i++) {
        char1 = ((unsigned long)((zeitLetters[0])[i])) << DISPLAY_WIDTH - CHAR_WIDTH;
        char2 = ((unsigned long)((zeitLetters[1])[i])) << DISPLAY_WIDTH - (CHAR_WIDTH * 2);
        char3 = ((unsigned long)((zeitLetters[2])[i])) << DISPLAY_WIDTH - (CHAR_WIDTH * 3);
        char4 = ((unsigned long)((zeitLetters[3])[i])) << DISPLAY_WIDTH - (CHAR_WIDTH * 4);
        char5 = ((unsigned long)((zeitLetters[4])[i])) << DISPLAY_WIDTH - (CHAR_WIDTH * 5);
        char6 = ((unsigned long)((zeitLetters[5])[i])) << DISPLAY_WIDTH - (CHAR_WIDTH * 6);
        char7 = ((unsigned long)((zeitLetters[6])[i])) << DISPLAY_WIDTH - (CHAR_WIDTH * 7);
        char8 = ((unsigned long)((zeitLetters[7])[i])) << DISPLAY_WIDTH - (CHAR_WIDTH * 8);
        text[i] = char1 + char2 + char3 + char4 + char5 + char6 + char7 + char8;
        if (j < 0) {
          text[i] = text[i] >> CHAR_WIDTH * (j*(-1));
        } else {
          text[i] = text[i] << CHAR_WIDTH * j;
        }
        if (buttonToggle == false) {
          return;
        }
        displayText(text);
      }
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

    /*
    We were unable to get the sensor to work correctly with analogRead().
    Reading the values of the sensor, the value never reached 1024;
    in fact, it never went above 250. That is why we're using digitalRead()
    below.
    
    I (Jessica) did email Herr Amanuel about the issue and he tried to help
    me in class, but we could not get it to work properly. I tried his sensor
    and he also checked our code.
    He said that we should use digitalRead() instead and leave a note
    here about why we're using it instead of analogRead().
    
    We left in the analogRead() code, commented out, to show that we did
    know how to do this with analogRead(). We would have used pin A2.
    */

//  void lightMeasure() {
//    
//    lightMillis = millis();
//    if ((lightMillis - lastMeasurement) >= 2000) { // checks for change every 2 seconds
//      byte brightness = analogRead(LSENSOR);
//      
//      byte lValue = map(brightness,0,1024,0,15);
//  
//      lastMeasurement = lightMillis;
//      led.setIntensity(0, lValue);
//      led.setIntensity(1, lValue);
//      led.setIntensity(2, lValue);
//      led.setIntensity(3, lValue);
//      Serial.println(brightness);
//      lastMeasurement = lightMillis;
//      Serial.println(brightness); // for checking values
//    }
//  }

  void lightMeasure() {

    unsigned long lightMillis, lastMeasurement;
    
    lightMillis = millis();
    if ((lightMillis - lastMeasurement) >= 2000) { // checks for change every 2 seconds
      int brightness = digitalRead(LSENSOR);

      // 6 because, with only 2 values, 0 is too dim and we wanted to show a
      // strong difference between being in a well-lit and poorly-lit area
      int lValue = map(brightness,0,1,15,0);
  
      lastMeasurement = lightMillis;
      led.setIntensity(0, lValue);
      led.setIntensity(1, lValue);
      led.setIntensity(2, lValue);
      led.setIntensity(3, lValue);
    }
  }

  void setup() {
    
    Serial.begin(9600);
    
    for (int i = 0; i < 4; i++) {
      led.shutdown(i, false);
      led.setIntensity(i, 8);
      led.clearDisplay(i);
    }
  
    pinMode(TRIGPIN, OUTPUT);
    pinMode(ECHOPIN, INPUT);
    digitalWrite(TRIGPIN, LOW);
  
    pinMode(RED_OUTPUT, OUTPUT);
    pinMode(YELLOW_OUTPUT, OUTPUT);
    pinMode(GREEN_OUTPUT, OUTPUT);
    
     startMillis = millis();
  
    pinMode(BUTTON, INPUT_PULLUP); // for pull-up resistor
  
    attachInterrupt(digitalPinToInterrupt(BUTTON), interrupt_function, LOW);
     
  }

  void loop() {
    currentMillis = millis();
    if (currentMillis - startMillis >= 125) {
      calcTime();
      ledClock();
      startMillis=currentMillis;
    }
    if (buttonToggle == true) {
      displayZeit();
      buttonToggle = false;
    }
    lightMeasure();
    
  }
