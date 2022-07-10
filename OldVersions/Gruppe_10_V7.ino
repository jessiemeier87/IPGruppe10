#include <stdio.h>
#include <stdlib.h>
#include "LedControl.h"
#include <binary.h>
#include <string.h>
#include <DHT.h>
#include <Adafruit_CCS811.h>

LedControl led = LedControl(13, 12, 11, 4); //13 is orange, 12 is green, 11 is yellow

#define TRIGPIN 5
#define ECHOPIN 4
#define MICRO_DELAY 10

#define RED_OUTPUT 10
#define YELLOW_OUTPUT 9
#define GREEN_OUTPUT 8

#define LSENSOR 6
#define BUZZER 7
#define BUTTON 2
#define DHTPIN 3

#define DISPLAY_WIDTH 32
#define CHAR_WIDTH 4

#define DHTTYPE DHT22

Adafruit_CCS811 ccs;
DHT dht(DHTPIN, DHTTYPE);

// used in displayZeit() and ledClock()
unsigned long text[8] = {0,0,0,0,0,0,0,0};
  
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

//define letters
byte C[8] = {0b0000, 0b0011, 0b0100, 0b1000, 0b1000, 0b0100, 0b0011, 0b0000};
byte D[8] = {0b0000, 0b1110, 0b1001, 0b1001, 0b1001, 0b1001, 0b1110, 0b0000};
byte O[8] = {0b0000, 0b0111, 0b0101, 0b0101, 0b0101, 0b0101, 0b0111, 0b0000};
byte t[8] = {0b0000, 0b0000, 0b0010, 0b0111, 0b0010, 0b0010, 0b0011, 0b0000};
byte i[8] = {0b0000, 0b0000, 0b0010, 0b0000, 0b0010, 0b0010, 0b0010, 0b0000};
byte m1[8] = {0b0000, 0b0000, 0b0000, 0b0100, 0b0110, 0b0101, 0b0100, 0b0000};
byte m2[8] = {0b0000, 0b0000, 0b0000, 0b0100, 0b1100, 0b0100, 0b0100, 0b0000};
byte e[8] = {0b0000, 0b0000, 0b0000, 0b1110, 0b1110, 0b1000, 0b1110, 0b0000};
byte c[8] = {0b0000, 0b0000, 0b0000, 0b0111, 0b0100, 0b0100, 0b0111, 0b0000};
byte tempe[8] = {0b0000, 0b0000, 0b0000, 0b0111, 0b0111, 0b0100, 0b0111, 0b0000};
byte h[8] = {0b0000, 0b0000, 0b0100, 0b0100, 0b0111, 0b0101, 0b0101, 0b0000};
byte l[8] = {0b0000, 0b0000, 0b0100, 0b0100, 0b0100, 0b0100, 0b0110, 0b0000};
byte v1[8] = {0b0000, 0b0000, 0b0000, 0b0100, 0b0100, 0b0010, 0b0001, 0b0000}; 
byte v2[8] = {0b0000, 0b0000, 0b0000, 0b0100, 0b0100, 0b1000, 0b0000, 0b0000};
byte p[8] = {0b0000, 0b0000, 0b0000, 0b1110, 0b1010, 0b1110, 0b1000, 0b1000};
byte mesp[8] = {0b0000, 0b0000, 0b0000, 0b0110, 0b0101, 0b0110, 0b0100, 0b0100};

//define symbols
byte empty[8] = {0b0000, 0b0000, 0b0000, 0b0000, 0b0000, 0b0000, 0b0000, 0b0000};
byte doubleDot[8] = {0b0000, 0b0000, 0b0001, 0b0000, 0b0000, 0b0001, 0b0000, 0b0000};
byte dotForD[8] = {0b0000, 0b0000, 0b0100, 0b0000, 0b0000, 0b0100, 0b0000, 0b0000};
byte dot[8] = {0b0000, 0b0000, 0b0000, 0b0000, 0b0000, 0b0000, 0b0010, 0b0000};
byte abrevDot[8] = {0b0000, 0b0000, 0b0000, 0b0000, 0b0000, 0b0000, 0b1000, 0b0000};
byte degree[8] = {0b0010, 0b0101, 0b0010, 0b0000, 0b0000, 0b0000, 0b0000, 0b0000};
byte perc1[8] = {0b0000, 0b0010, 0b0100, 0b1000, 0b0000, 0b0100, 0b1010, 0b0100};
byte perc2[8] = {0b0010, 0b0101, 0b0010, 0b0000, 0b0001, 0b0010, 0b0100, 0b0000};
byte lessThan[8] = {0b0000, 0b0000, 0b0010, 0b0100, 0b1000, 0b0100, 0b0010, 0b0000};
byte greaterThan[8] = {0b0000, 0b0000, 0b0100, 0b0010, 0b0001, 0b0010, 0b0100, 0b0000};

//text for textToScroll() function
byte* digits[10] = {zero, one, two, three, four, five, six, seven, eight, nine};
byte* blinkingDots[2] = {doubleDot, empty};
byte* dotForMes[1] = {dot};  
byte* mesSymbols[10] = {C, degree, perc1, perc2, mesp, m1, m2, c, D, dotForD};
byte* timeLetters[8] = {lessThan, t, i, m1, m2, e, greaterThan, empty};
byte* tempLetters[8] = {t, tempe, m1, m2, p, abrevDot, empty, empty};
byte* humLetters[8] = {h, abrevDot, l, e, v1, v2, e, l};
byte* CO2Letters[8] = {C, O, two, empty, p, p, m1, m2};

//for dots on clock to blink every 2 sec.
unsigned long startMillis2;
const unsigned long dotBlink = 2000;

// secondShift increases every 125 miliseconds (1000/8), so that we can move the bits
//for the right-most second counter on clock from the bottom up
unsigned int secondShift = 0;

/*
 regular bool + toggling wasn't working; tried volatile after reading
 this thread: https://forum.arduino.cc/t/changing-a-variable-flag-with-an-interrupt/690074
 used in displayZeit() and buttonInterrupt()
*/
volatile bool buttonToggle = true;

//timers in void loop()
unsigned long currentMillis, startMillis;

//uls for interrupt function
unsigned long buttonTime = 0, lastButtonTime = 0;

uint8_t lValue; // used in lightMeasure() and serialCommands()

//timers for turning off LEDs and buzzer after testing
//used in readSerInput() and handleLightsNBuzzer()
unsigned long lastExRed, lastExGreen, lastExYellow, lastExBuzzer;

// variables for ledClock() and calcTime()
uint8_t blinking = 0;
uint8_t sek1 = 0;
uint8_t sek10 = 0;
uint8_t min1 = 0;
uint8_t min10 = 0;
uint8_t stu1 = 0;
uint8_t stu10 = 0;

//variable to scroll through different displays
//(time, temp, etc.)
uint8_t displayCounter = 1;

//variables for reading serial input
char userInput[32];
int inputIndex = 0;
bool testBuzzer = false;
bool testLEDs = false;

//variables for alarm
uint8_t alarmHour = 100;
uint8_t alarmMinute = 60;
bool alarmOn = false;

//timing variables for light sensor
unsigned long lightMillis, lastMeasurement;

//timing variables for sensor mes.
unsigned long tempMillis, lastTempMillis, humMillis, lastHumMillis, lastDistMillis, distMillis;

//variables for CO2 sensor
int eCO2;
bool CO2InRange = true;

//variables for hum. and temp. sens.
float humidity;
float temp;
bool humInRange = true;
bool tempInRange = true;
int tempForArray, humForArray, CO2ForArray, distForArray;
uint8_t tempArray[4], humArray[4], CO2Array[4], distArray[4];

//variables for distance sensor
unsigned long pulseWidth, distance, lastDisMes, distanceMicros;
uint8_t distValue;
bool withinDist = false;


//FUNCTIONS START HERE

/* Gets executed when the button is pressed */
void buttonInterrupt() {
  buttonTime = millis(); // sets 'buttonTime' to millis()
  /*
  check if buttonTime - lastButtonTime is > than 50 ms; if it is, then
  we know that the current state has changed
  */
  if ((buttonTime - lastButtonTime) > 200) { 
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

// handles the display of the clock 
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

/*handles scrolling text across screen
for example, scrollText(tempLetters, 8)
displays <time> across screen
*/ 
void scrollText(byte *textToScroll[], int n) {

  unsigned long char1, char2, char3, char4, char5, char6, char7, char8;
  
  for (int j = -8; j < 8; j++) {
    for (int i = 0; i < 8; i++) {
      char1 = ((unsigned long)((textToScroll[0])[i])) << DISPLAY_WIDTH - CHAR_WIDTH;
      char2 = ((unsigned long)((textToScroll[1])[i])) << DISPLAY_WIDTH - (CHAR_WIDTH * 2);
      char3 = ((unsigned long)((textToScroll[2])[i])) << DISPLAY_WIDTH - (CHAR_WIDTH * 3);
      char4 = ((unsigned long)((textToScroll[3])[i])) << DISPLAY_WIDTH - (CHAR_WIDTH * 4);
      char5 = ((unsigned long)((textToScroll[4])[i])) << DISPLAY_WIDTH - (CHAR_WIDTH * 5);
      char6 = ((unsigned long)((textToScroll[5])[i])) << DISPLAY_WIDTH - (CHAR_WIDTH * 6);
      char7 = ((unsigned long)((textToScroll[6])[i])) << DISPLAY_WIDTH - (CHAR_WIDTH * 7);
      char8 = ((unsigned long)((textToScroll[7])[i])) << DISPLAY_WIDTH - (CHAR_WIDTH * 8);
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

//for displaying distance #s on screen
void displayDist() {

  unsigned long char1, char2, char3, char4, char5, char6, char7, char8;

  for (int i = 0; i < 8; i++) {
    char1 = ((unsigned long)((mesSymbols[8])[i])) << DISPLAY_WIDTH - CHAR_WIDTH;
    char2 = ((unsigned long)((mesSymbols[9])[i])) << DISPLAY_WIDTH - (CHAR_WIDTH * 2);
    char3 = ((unsigned long)((blinkingDots[1])[i])) << DISPLAY_WIDTH - (CHAR_WIDTH * 3);
    char4 = ((unsigned long)((digits[distArray[1]])[i])) << DISPLAY_WIDTH - (CHAR_WIDTH * 4);
    char5 = ((unsigned long)((digits[distArray[0]])[i])) << DISPLAY_WIDTH - (CHAR_WIDTH * 5);
    char6 = ((unsigned long)((mesSymbols[7])[i])) << DISPLAY_WIDTH - (CHAR_WIDTH * 6);
    char7 = ((unsigned long)((mesSymbols[5])[i])) << DISPLAY_WIDTH - (CHAR_WIDTH * 7);
    char8 = ((unsigned long)((mesSymbols[6])[i])) << DISPLAY_WIDTH - (CHAR_WIDTH * 8);
    text[i] = char1 + char2 + char3 + char4 + char5 + char6 + char7 + char8;
    }
  displayText(text);
}

//for displaying humidity #s on screen
void displayHum() {

  unsigned long char1, char2, char3, char4, char5, char6, char7, char8;

  for (int i = 0; i < 8; i++) {
    char1 = ((unsigned long)((blinkingDots[1])[i])) << DISPLAY_WIDTH - CHAR_WIDTH;
    char2 = ((unsigned long)((digits[humArray[3]])[i])) << DISPLAY_WIDTH - (CHAR_WIDTH * 2);
    char3 = ((unsigned long)((digits[humArray[2]])[i])) << DISPLAY_WIDTH - (CHAR_WIDTH * 3);
    char4 = ((unsigned long)((dotForMes[0])[i])) << DISPLAY_WIDTH - (CHAR_WIDTH * 4);
    char5 = ((unsigned long)((digits[humArray[1]])[i])) << DISPLAY_WIDTH - (CHAR_WIDTH * 5);
    char6 = ((unsigned long)((digits[humArray[0]])[i])) << DISPLAY_WIDTH - (CHAR_WIDTH * 6);
    char7 = ((unsigned long)((mesSymbols[3])[i])) << DISPLAY_WIDTH - (CHAR_WIDTH * 7);
    char8 = ((unsigned long)((mesSymbols[2])[i])) << DISPLAY_WIDTH - (CHAR_WIDTH * 8);
    text[i] = char1 + char2 + char3 + char4 + char5 + char6 + char7 + char8;
    }
  displayText(text);
}

//for displaying temp #s on screen
void displayTemp() {

  unsigned long char1, char2, char3, char4, char5, char6, char7, char8;

  for (int i = 0; i < 8; i++) {
    char1 = ((unsigned long)((blinkingDots[1])[i])) << DISPLAY_WIDTH - CHAR_WIDTH;
    char2 = ((unsigned long)((digits[tempArray[3]])[i])) << DISPLAY_WIDTH - (CHAR_WIDTH * 2);
    char3 = ((unsigned long)((digits[tempArray[2]])[i])) << DISPLAY_WIDTH - (CHAR_WIDTH * 3);
    char4 = ((unsigned long)((dotForMes[0])[i])) << DISPLAY_WIDTH - (CHAR_WIDTH * 4);
    char5 = ((unsigned long)((digits[tempArray[1]])[i])) << DISPLAY_WIDTH - (CHAR_WIDTH * 5);
    char6 = ((unsigned long)((digits[tempArray[0]])[i])) << DISPLAY_WIDTH - (CHAR_WIDTH * 6);
    char7 = ((unsigned long)((mesSymbols[1])[i])) << DISPLAY_WIDTH - (CHAR_WIDTH * 7);
    char8 = ((unsigned long)((mesSymbols[0])[i])) << DISPLAY_WIDTH - (CHAR_WIDTH * 8);
    text[i] = char1 + char2 + char3 + char4 + char5 + char6 + char7 + char8;
    }
  displayText(text);
}

//for displaying CO2 #s on screen
void displayCO2() {

  unsigned long char1, char2, char3, char4, char5, char6, char7, char8;

  for (int i = 0; i < 8; i++) {
    char1 = ((unsigned long)((digits[CO2Array[3]])[i])) << DISPLAY_WIDTH - CHAR_WIDTH;
    char2 = ((unsigned long)((digits[CO2Array[2]])[i])) << DISPLAY_WIDTH - (CHAR_WIDTH * 2);
    char3 = ((unsigned long)((digits[CO2Array[1]])[i])) << DISPLAY_WIDTH - (CHAR_WIDTH * 3);
    char4 = ((unsigned long)((digits[CO2Array[0]])[i])) << DISPLAY_WIDTH - (CHAR_WIDTH * 4);
    char5 = ((unsigned long)((mesSymbols[4])[i])) << DISPLAY_WIDTH - (CHAR_WIDTH * 5);
    char6 = ((unsigned long)((mesSymbols[4])[i])) << DISPLAY_WIDTH - (CHAR_WIDTH * 6);
    char7 = ((unsigned long)((mesSymbols[5])[i])) << DISPLAY_WIDTH - (CHAR_WIDTH * 7);
    char8 = ((unsigned long)((mesSymbols[6])[i])) << DISPLAY_WIDTH - (CHAR_WIDTH * 8);
    text[i] = char1 + char2 + char3 + char4 + char5 + char6 + char7 + char8;
    }
  displayText(text);
}

// calculates time and also triggers alarm
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

  //note: need to rework with modulo
  unsigned long currentMillis2 = millis();
  if (currentMillis2 - startMillis2 >= dotBlink) {
    startMillis2 = currentMillis2;
    blinking = (blinking + 1) % 2;
  }

  if (alarmHour <= hours && alarmMinute <= minutes) {
    alarm(true);
    alarmHour = 100;
    alarmMinute = 60;
  }
}

//alarm function
void alarm(bool on) {
  if (on) {
    alarmOn = true;
    tone(BUZZER, 1000);
  } else {
    noTone(BUZZER);
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

//measures light
void lightMeasure() {
  
  lightMillis = millis();
  if ((lightMillis - lastMeasurement) >= 2000) { // checks for change every 2 seconds
    int brightness = digitalRead(LSENSOR);

    // 6 because, with only 2 values, 0 is too dim and we wanted to show a
    // strong difference between being in a well-lit and poorly-lit area
    lValue = map(brightness,0,1,15,6);

    lastMeasurement = lightMillis;
    led.setIntensity(0, lValue);
    led.setIntensity(1, lValue);
    led.setIntensity(2, lValue);
    led.setIntensity(3, lValue);
  }
}

//reads input from Serial
void readSerInput() {
  // Attempt to read if there are bytes available
  if (Serial.available()) {
    int availableBytes = Serial.available();
    for(int i=0; i<availableBytes; i++) {
      userInput[inputIndex] = Serial.read();
      if(userInput[inputIndex]=='\n') {
        // Replace the newline with null character so string compare works
        userInput[inputIndex] = '\0';
        Serial.println(F("\nYou typed: "));
        Serial.println(userInput);
        Serial.println(" ");
        serialCommands(userInput);
        inputIndex = 0;
        // stop reading after processing a single command
        break;
      }
      inputIndex+=1;
    }
  }
}

/*gets CO2 numbers, processes into array for display,
 and also determines acceptable range. Whether or
 not the range is acceptable gets printed in Serial
 when command called.
*/
void getCO2() {
  bool dataAvailable = ccs.available();
  int i = 0;
  if (dataAvailable) {
    bool error = ccs.readData();
    if (!error) {
      eCO2 = ccs.geteCO2();
      CO2ForArray = eCO2;
    }
    for (int j = 0; j < 4; j++) {
      CO2Array[j] = 0;
    }
    while (CO2ForArray != 0) {
      int digit = CO2ForArray%10;
      CO2ForArray = CO2ForArray/10;
      CO2Array[i] += digit;
      i += 1;
    }
  }
  // https://www.umweltbundesamt.de/sites/default/files/medien/pdfs/kohlendioxid_2008.pdf , S. 1367
  // no lower limit because there's no min. safe CO2
  if (eCO2 >= 1000) {
    CO2InRange = false;
  } else {
      CO2InRange = true;
    }
}

/*gets temperature numbers, processes into array for display,
 and also determines acceptable range. Whether or
 not the range is acceptable gets printed in Serial
 when command called.
*/
void getTemp() {
  tempMillis = millis();
  int i = 0;
  if (tempMillis - lastTempMillis >= 10000) {
    temp = dht.readTemperature();
    tempForArray = temp * 100;
    for (int j = 0; j < 4; j++) {
      tempArray[j] = 0;     
    }
    while (tempForArray != 0) {
      int digit = tempForArray%10;
      tempForArray = tempForArray/10;
      tempArray[i] += digit;
      i+=1;
    }
    //https://www.baua.de/DE/Angebote/Rechtstexte-und-Technische-Regeln/Regelwerk/ASR/pdf/ASR-A3-5.pdf
    if (temp < 20 || temp > 26) {
      tempInRange = false;
    } else {
      tempInRange = true;
    }
    lastTempMillis = tempMillis;
  }
}

//gets distance numbers, processes into array
void getDist() {
  distMillis = millis();
  int i = 0;
  if (tempMillis - lastDistMillis >= 10000) {
    distForArray = distance;
    Serial.println(distForArray);
    for (int j = 0; j < 4; j++) {
      distArray[j] = 0;     
    }
    while (tempForArray != 0) {
      int digit = distForArray%10;
      distForArray = distForArray/10;
      distArray[i] += digit;
      i+=1;
    }
    lastDistMillis = distMillis;
  }
}

/*gets humidity numbers, processes into array for display,
 and also determines acceptable range. Whether or
 not the range is acceptable gets printed in Serial
 when command called.
*/
void getHum() {
  humMillis = millis();
  int i = 0;
  if (humMillis - lastHumMillis >= 10000) {
    humidity = dht.readHumidity();
    humForArray = humidity * 100;
    for (int j = 0; j < 4; j++) {
      humArray[j] = 0;     
    }
    while (humForArray != 0) {
      int digit = humForArray%10;
      humForArray = humForArray/10;
      humArray[i] += digit;
      i+=1;
    }
    //https://www.stuttgarter-nachrichten.de/inhalt.optimale-luftfeuchtigkeit-in-raeumen-mhsd.7bb6078b-3f9f-4643-8dd6-5d7020168605.html
    if (humidity < 40 || humidity > 60) {
      humInRange = false;
    } else {
      humInRange = true;
    }
    lastHumMillis = humMillis;
  }
}

//takes input as 'eingabe' from readSerInput() and
//uses it to execute commands
void serialCommands(char* eingabe) {
  if(strcmp(eingabe, "Test LED red")==0) {
    testLEDs = true;
    digitalWrite(RED_OUTPUT, HIGH);
    lastExRed = millis();
  }
  else if(strcmp(eingabe, "Test LED green")==0) {
    testLEDs = true;
    digitalWrite(GREEN_OUTPUT, HIGH);
    lastExGreen = millis();      
  }
  else if(strcmp(eingabe, "Test LED yellow")==0) {
    testLEDs = true;
    digitalWrite(YELLOW_OUTPUT, HIGH);
    lastExYellow = millis();
  }
  else if(strcmp(eingabe, "Test buzzer")==0) {
    testBuzzer = true;
    tone(BUZZER, 1000);
    lastExBuzzer = millis();
  }
  else if(strcmp(eingabe, "Print distance")==0) {
    Serial.println(F("Distance: "));
    Serial.print(distance);
    Serial.print(F(" cm\n"));         
  }
  // 
  else if(strcmp(eingabe, "Print humidity")==0) {
    Serial.println(F("Humidity level: "));
    Serial.print(humidity);
    Serial.print(F("%"));
    if (humInRange) {
      Serial.print(F(" (in range)"));   
    } else {
      Serial.print(F(" (out of range)"));
    }
  }
  else if(strcmp(eingabe, "Print CO2")==0) {
    Serial.println(F("CO2 level: "));
    Serial.print(eCO2);
    Serial.print(F(" ppm"));
    if (CO2InRange) {
      Serial.print(F(" (in range)"));    
    } else {
      Serial.print(F(" (out of range)"));
    }
  }
  else if(strcmp(eingabe, "Print temperature")==0) {
    Serial.println(F("Temperature: "));
    Serial.print(temp);
    Serial.print(F(" Celsius\n"));
    if (tempInRange) {
      Serial.print(F(" (in range)"));    
    } else {
      Serial.print(F(" (out of range)"));
    }      
  }
  else if(strcmp(eingabe, "Print LED level")==0) {
    Serial.println(F("LEDs at level "));
    Serial.print(lValue);
    Serial.print(F(" brightness (out of 0 - 15)\n"));
  }
  //strncmp to check first 9 letters
  else if(strncmp(eingabe, "Set alarm", 9)==0) {
    int tempHour = atoi(eingabe+10);
    int tempMinute = atoi(eingabe+13);
    //limit acceptable times; time display can't go higher
    if (tempHour <= 99 && tempMinute <= 59) {
      alarmHour = tempHour;
      alarmMinute = tempMinute;
    } else {
      Serial.println(F("Invalid time input. Try again."));
      }
  } else {
      Serial.print(F("Invalid input. Try again."));
  }
}

//turns off test functions 5 seconds after they were
//triggered; LEDs need to be reworked a little
void serialTestTimer(){
  if (millis() >= lastExRed+5000) {
    if (digitalRead(RED_OUTPUT)) {
      digitalWrite(RED_OUTPUT, LOW);
      testLEDs = false;
    }
  }
  if (millis() >= lastExGreen+5000) {
    if (digitalRead(GREEN_OUTPUT)) {
      digitalWrite(GREEN_OUTPUT, LOW);
      testLEDs = false;
    }
  }
  if (millis() >= lastExYellow+5000) {
    if (digitalRead(YELLOW_OUTPUT)) {
      digitalWrite(YELLOW_OUTPUT, LOW);
      testLEDs = false;
    }
  }
  if (millis() >= lastExBuzzer+5000 && !alarmOn) {
     if (BUZZER){
      noTone(BUZZER);
      testBuzzer = false;
     }
  }
}

//determines what scrolling message to display
void scroll() {
  if (displayCounter == 1 && buttonToggle == true && !alarmOn) {
    scrollText(timeLetters, 8);
    buttonToggle = false;
    displayCounter += 1;
  }
  else if (displayCounter == 2 && buttonToggle == true && !alarmOn) {
    scrollText(tempLetters, 8);
    buttonToggle = false;
    displayCounter += 1;
  }
  else if (displayCounter == 3 && buttonToggle == true && !alarmOn) {
    scrollText(humLetters, 8);
    buttonToggle = false;
    displayCounter += 1;      
  }
  else if (displayCounter == 4 && buttonToggle == true && !alarmOn) {
    scrollText(CO2Letters, 8);
    buttonToggle = false;
    displayCounter = 1;
  }
}

//determines what numbers to display
void displayNums() {
  if (millis() > 5000 && withinDist) {
    displayDist();
  } else {
    if (displayCounter == 2) {
      ledClock();
    }
    else if (displayCounter == 3) {
      displayTemp();
    }
    else if (displayCounter == 4) {
      displayHum();
    }
    else if (displayCounter == 1) {
      displayCO2();
    }
  }
}

//measures distance to object and
//also puts into array for display
void distanceSensorMes() {
  distanceMicros = micros();
  distForArray = distance;
  int i = 0;
  /* Only measure once per second */
  if ((distanceMicros - lastDisMes) >= 1000000) {
    digitalWrite(TRIGPIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGPIN, LOW);
    pulseWidth = pulseIn(ECHOPIN, HIGH);
    distance = (pulseWidth * 0.0343)/2;  // distance calc. from https://www.makerguides.com/hc-sr04-arduino-tutorial/

    distValue = map(distance, 2, 20, 1, 4);
    for (int j = 0; j < 4; j++) {
      distArray[j] = 0;     
    }
    while (distForArray != 0 && distForArray < 30) {
      int digit = distForArray%10;
      distForArray = distForArray/10;
      distArray[i] += digit;
      i+=1;
    }
    lastDisMes = distanceMicros;
  }
}

//determines what happens with buzzer and
//lights when within distance range
void distanceSensorOutput() {
  if (distance < 20) {
    withinDist = true;
    if (distValue == 1 && !testBuzzer) {
      digitalWrite(RED_OUTPUT, HIGH);
      digitalWrite(YELLOW_OUTPUT, LOW);
      digitalWrite(GREEN_OUTPUT, LOW);
      tone(BUZZER, 100);
    }
    else if (distValue == 2 && !testBuzzer) {
      digitalWrite(RED_OUTPUT, LOW);
      digitalWrite(YELLOW_OUTPUT, HIGH);
      digitalWrite(GREEN_OUTPUT, LOW);
      tone(BUZZER, 1000);
    }
    else if (distValue == 3 && !testBuzzer) {
      digitalWrite(RED_OUTPUT, LOW);
      digitalWrite(YELLOW_OUTPUT, LOW);
      digitalWrite(GREEN_OUTPUT, HIGH);
      tone(BUZZER, 5000);
    }
  }
  else if (distance > 20 && !alarmOn && !testBuzzer && !testLEDs) {
    withinDist = false;
    digitalWrite(RED_OUTPUT, LOW);
    digitalWrite(YELLOW_OUTPUT, LOW);
    digitalWrite(GREEN_OUTPUT, LOW);
    noTone(BUZZER);
    }
}

void setup() {
  
  Serial.begin(115200);
  
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

  pinMode(LSENSOR, INPUT);

  if(!ccs.begin()){
      Serial.println(F("Failed to start sensor! Please check your wiring."));
      while(1);
  }

  dht.begin();
  
  startMillis = millis();

  pinMode(BUTTON, INPUT_PULLUP); // for pull-up resistor

  attachInterrupt(digitalPinToInterrupt(BUTTON), buttonInterrupt, CHANGE);
  
}

void loop() {
  distanceSensorOutput();
  currentMillis = millis();
  if (currentMillis - startMillis >= 125) {
    calcTime();
    scroll();
    displayNums();
    startMillis=currentMillis;
  }
  if (alarmOn && buttonToggle) {
    alarmOn = false;
    alarm(false);
    buttonToggle = false;
  }
  getCO2();
  getHum();
  getTemp();
  distanceSensorMes();

  lightMeasure();
  readSerInput();
  serialTestTimer();
}
