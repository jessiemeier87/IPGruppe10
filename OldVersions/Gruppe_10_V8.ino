#include <stdio.h>
#include <stdlib.h>
#include "LedControl.h"
#include <binary.h>
#include <string.h>
#include <DHT.h>
#include <Adafruit_CCS811.h>

LedControl led = LedControl(13, 12, 11, 4); //13 ist orange, 12 ist grün, 11 ist gelb
Adafruit_CCS811 ccs;

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
DHT dht(DHTPIN, DHTTYPE);

// verwendet in displayZeit() und ledClock()
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

//Text für Funktion textToScroll()
byte* digits[10] = {zero, one, two, three, four, five, six, seven, eight, nine};
byte* blinkingDots[2] = {doubleDot, empty};
byte* dotForMes[1] = {dot};  
byte* mesSymbols[10] = {C, degree, perc1, perc2, mesp, m1, m2, c, D, dotForD};
byte* timeLetters[8] = {lessThan, t, i, m1, m2, e, greaterThan, empty};
byte* tempLetters[8] = {t, tempe, m1, m2, p, abrevDot, empty, empty};
byte* humLetters[8] = {h, abrevDot, l, e, v1, v2, e, l};
byte* CO2Letters[8] = {C, O, two, empty, p, p, m1, m2};

/*  secondShift wird in ledClock() verwendet. Sie erhöht sich alle
 *  125 Millisekunden (1000/8), so dass wir die Bits für den 
 *  rechten Sekundenzähler auf der Uhr von unten nach oben verschieben können
*/
unsigned int secondShift = 0;

/*
 normales bool + toggling funktionierte nicht; wir haben es erfolgreich mit 
 volatile versucht, nachdem wir diesen Thread gelesen hatten:
 https://forum.arduino.cc/t/changing-a-variable-flag-with-an-interrupt/690074
 verwendet in displayZeit() und buttonInterrupt()
*/
volatile bool buttonToggle = true;

//timers within void loop()
unsigned long currentMillis, startMillis;

//uls for interrupt function
unsigned long buttonTime = 0, lastButtonTime = 0;

//Timer zum Ausschalten der LEDs und des Summers nach testen
//verwendet in readSerInput() und handleLightsNBuzzer()
unsigned long lastExRed, lastExGreen, lastExYellow, lastExBuzzer;

// Variablen für ledClock() and calcTime()
uint8_t blinking = 0, sek1 = 0, sek10 = 0, min1 = 0, min10 = 0,
stu1 = 0, stu10 = 0;

// Variable Umschaltung auf ein anderes Display per Tastendruck
// (time, temp, etc.)
uint8_t displayCounter = 1;

//Variablen zum Lesen der seriellen Benutzereingabe
char userInput[32];
int inputIndex = 0;
bool testBuzzer = false, testRedLED = false, testGreenLED = false, testYellowLED = false;

//Variablen für Alarm
uint8_t alarmHour = 100, alarmMinute = 60;
bool alarmOn = false;

//Zeitvariablen für Lichtsensor
unsigned long lightMillis, lastMeasurement;
int brightness;

//Zeitvariablen für andere Sensoren
unsigned long tempMillis, lastTempMillis, humMillis, lastHumMillis, lastDistMillis, distMillis;

//Variablen für CO2 Sensor
int eCO2;
bool CO2InRange = true;

//Variablen für Feuchtigkeits- (humidity) und Temperatursensoren
float humidity, temp;
bool humInRange = true, tempInRange = true;
int tempForArray, humForArray, CO2ForArray, distForArray;
uint8_t tempArray[4], humArray[4], CO2Array[4], distArray[4];

//Variablen für Distanzsensor
unsigned long pulseWidth, distance, lastDisMes, distanceMillis;
uint8_t distValue;
bool withinDist = false;


//FUNKTIONEN AB HIER

//Wird ausgeführt, wenn die Taste gedrückt wird
void buttonInterrupt() {
  buttonTime = millis(); // setzt 'buttonTime' auf millis()
  /*
  * Prüft, ob buttonTime - lastButtonTime > als 300 ms ist. Wenn ja, 
  * dann wissen wir, dass sich der aktuelle Zustand geändert hat. 
  * Setzt per Toggle in den entgegengesetzten Zustand, damit der 
  * nächste Druck erkannt werden kann
  */
  if ((buttonTime - lastButtonTime) > 300) { 
    if (buttonToggle == true) {
      buttonToggle = false;
    } else {
      buttonToggle = true;
    }
    lastButtonTime = buttonTime; // lastButtonTime setzen, damit wir die nächste Änderung erkennen können
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

/* 
 * steuert die Anzeige der Uhr auf der LED-Matrix
 * alle 1/8 Sekunden in der Hauptschleife aufgerufen.
 */
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
    /* 
     *  secondShift verschiebt die Bits der rechten Zahl alle 1/8
     *  Sekunde um 1, so dass sie von unten nach oben wandern kann.
     *  Modulo 8 wird verwendet, damit die Zahl von unten nach oben
     *  zurückkehrt, bevor sich die Zahl ändert.
    */
    s2 = ((unsigned long)((digits[sek1])[(i+secondShift)%8])) << DISPLAY_WIDTH - (CHAR_WIDTH * 8);
    text[i] = h1 + h2 + d1 + m1 + m2 + d2 + s1 + s2;
    }
  displayText(text);

  secondShift+=1;
  if(secondShift >= 8) {
      secondShift = 0;
  }
}

/*
 * Erledigt das Scrollen von Text über den Bildschirm.
 * Zum Beispiel: scrollText(tempLetters, 8)
 * zeigt < Zeit > auf dem Bildschirm an
*/

void scrollText(byte *textToScroll[], int n) {

  unsigned long char1, char2, char3, char4, char5, char6, char7, char8;
  
  for (int j = -n; j < n; j++) {
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

/*
 * Zur Anzeige von Entfernungswerten von Objekten auf dem Bildschirm.
 * Verwendet Array aus distanceSensorMes(), um Zahlen zu erhalten.
 */
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

/*
 * Zur Anzeige von Feuchtigkeit auf dem Bildschirm.
 * Verwendet Array aus getHum(), um Zahlen zu erhalten.
 */
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

/*
 * Zur Anzeige von Temperatur auf dem Bildschirm.
 * Verwendet Array aus getTemp(), um Zahlen zu erhalten.
 */
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

/*
 * Zur Anzeige von CO2 ppm auf dem Bildschirm.
 * Verwendet Array aus getCO2(), um Zahlen zu erhalten.
 */
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

/*
 * misst die Entfernung zum Objekt und legt sie in ein Array für die Anzeige
 */
void distanceSensorMes() {
  distanceMillis = millis();
  distForArray = distance;
  int i = 0;
  /* Misst einmal alle 1/2 Sekunde */
  if ((distanceMillis - lastDisMes) >= 1000) {
    digitalWrite(TRIGPIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGPIN, LOW);
    pulseWidth = pulseIn(ECHOPIN, HIGH);
    distance = (pulseWidth * 0.0343)/2;  // Entfernung calc. from https://www.makerguides.com/hc-sr04-arduino-tutorial/
    //2,21,1,4 weil Grün sonst selten auslöst
    distValue = map(distance, 2, 21, 1, 4);
    for (int j = 0; j < 4; j++) {
      distArray[j] = 0;     
    }
    while (distForArray != 0 && distForArray < 30) {
      int digit = distForArray%10;
      distForArray = distForArray/10;
      distArray[i] += digit;
      i+=1;
    }
    lastDisMes = distanceMillis;
  }
}

/*
 * Ermittelt den Wert der Luftfeuchtigkeit, verarbeitet ihn in
 * einem Array für die Anzeige und bestimmt auch den zulässigen Bereich.
 * Ob der Bereich akzeptabel ist oder nicht, wird beim Aufruf des
 * Befehls auf der seriellen Schnittstelle ausgegeben.
*/
void getHum() {
  humMillis = millis();
  int i = 0;
  if (humMillis - lastHumMillis >= 5000) { //checks for change every 5 seconds
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

/*
 * Ermittelt den Wert der Temperatur, verarbeitet ihn in
 * einem Array für die Anzeige und bestimmt auch den zulässigen Bereich.
 * Ob der Bereich akzeptabel ist oder nicht, wird beim Aufruf des
 * Befehls auf der seriellen Schnittstelle ausgegeben.
*/
void getTemp() {
  tempMillis = millis();
  int i = 0;
  if (tempMillis - lastTempMillis >= 5000) { //checks for change every 5 seconds
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

/*
 * Ermittelt den Wert der CO2, verarbeitet ihn in
 * einem Array für die Anzeige und bestimmt auch den zulässigen Bereich.
 * Ob der Bereich akzeptabel ist oder nicht, wird beim Aufruf des
 * Befehls auf der seriellen Schnittstelle ausgegeben.
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

  /*
   * Wir konnten den Sensor nicht dazu bringen, mit analogRead() korrekt zu arbeiten.
   * Beim Lesen der Werte des Sensors erreichte der Wert nie 1024; Tatsächlich stieg
   * er nie über 250. Aus diesem Grund verwenden wir digitalRead() unten.
   * 
   * Ich (Jessica) habe Herrn Amanuel eine E-Mail zu diesem Problem geschickt und er
   * hat versucht, mir in der Klasse zu helfen. mir in der Klasse zu helfen, aber wir
   * konnten es nicht richtig zum Laufen bringen. Ich habe seinen Sensor ausprobiert
   * und er hat auch unseren Code überprüft. Er sagte, wir sollten stattdessen
   * digitalRead() verwenden und eine Notiz hinterlassen warum wir diese Funktion
   * anstelle von analogRead() verwenden.
   * 
   * Wir haben den analogRead()-Code auskommentiert, um zu zeigen, dass wir wissen,
   * wie man das mit analogRead() macht. Wir hätten Pin A2 verwendet. Hinweis: Wir
   * glauben, dass es nicht funktioniert, weil der Sensor digital ist:
   * https://www.berrybase.de/Pixelpdfdata/Articlepdf/id/1/onumber/LIGHT-SENSOR
   * Und die analogen Pins könnten eher Rauschen aufnehmen, als dass sie Werte vom
   * Lichtsensor. Deshalb habe ich (Jessica) aus Versehen versehentlich den Lichtsensor
   * von meiner Platine abgezogen habe, um den von Herrn Amanuel zu testen, ohne ohne
   * die Platine vorher abzustecken, druckte er immer noch Werte in die serielle
   * Schnittstelle. Es war ein Floating Pin.
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

//misst das Licht und stellt die LED-Helligkeit ein
void lightMeasure() {
  
  lightMillis = millis();
    if ((lightMillis - lastMeasurement) >= 2000) { // prüft alle 2 Sekunden auf Veränderungen
      brightness = digitalRead(LSENSOR);
  
    if (brightness == 0) { //set brighter for bright room
      led.setIntensity(0, 15);
      led.setIntensity(1, 15);
      led.setIntensity(2, 15);
      led.setIntensity(3, 15);
    } else { //back to default if sensor at 1
      led.setIntensity(0, 8);
      led.setIntensity(1, 8);
      led.setIntensity(2, 8);
      led.setIntensity(3, 8);
    }
    lastMeasurement = lightMillis;
  }
}


// berechnet die Zeit und löst auch den Alarm aus
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

  blinking = seconds%2;
  
  /* 
   *  setzt den Alarm auf die Startnummern zurück, nachdem er ausgelöst
   *  wurde, sonst stoppt er nicht, auch wenn die Taste gedrückt wird
   */
  if (alarmHour <= hours && alarmMinute <= minutes) {
    alarm(true);
    alarmHour = 100;
    alarmMinute = 60;
  }
}

//Alarm Funktion
void alarm(bool on) {
  if (on) {
    alarmOn = true;
    tone(BUZZER, 1000);
  } else {
    noTone(BUZZER);
  }
}

//reads input from Serial
void readSerInput() {
  // Attempt to read if there are bytes available
  if (Serial.available()) {
    int availableBytes = Serial.available();
    for (int i=0; i<availableBytes; i++) {
      /*
       * Index, um Fehler zu vermeiden; ohne diesen Index
       * kam es ab und zu vor, dass der letzte Buchstabe
       * in eine neue Zeile verschoben wurde
       */
      userInput[inputIndex] = Serial.read();
      if(userInput[inputIndex]=='\n') {
        /* Ersetzt den Zeilenumbruch durch ein Null-Zeichen, 
         * damit der String-Vergleich in serialCommands() funktioniert
         */
        userInput[inputIndex] = '\0';
        Serial.println(F(" "));
        Serial.println(F("You typed: "));
        Serial.println(userInput);
        Serial.println(F(" "));
        serialCommands(userInput);
        inputIndex = 0;
        // das Lesen nach der Verarbeitung eines einzelnen Befehls beenden
        break;
      }
      inputIndex+=1;
    }
  }
}

//übernimmt 'Eingabe' von readSerInput()
//und verwendet sie zur Ausführung von Befehlen
void serialCommands(char* eingabe) {
  if(strcmp(eingabe, "Test LED red")==0) {
    testRedLED = true;
    digitalWrite(RED_OUTPUT, HIGH);
    lastExRed = millis();
  } else if(strcmp(eingabe, "Test LED green")==0) {
      testGreenLED = true;
      digitalWrite(GREEN_OUTPUT, HIGH);
      lastExGreen = millis();      
  } else if(strcmp(eingabe, "Test LED yellow")==0) {
      testYellowLED = true;
      digitalWrite(YELLOW_OUTPUT, HIGH);
      lastExYellow = millis();
  } else if(strcmp(eingabe, "Test buzzer")==0) {
      testBuzzer = true;
      tone(BUZZER, 1000);
      lastExBuzzer = millis();
  } else if(strcmp(eingabe, "Print distance")==0) {
      //(F()) uses flash memory instead of RAM
      Serial.print(F("Distance: "));
      Serial.print(distance);
      Serial.print(F(" cm\n"));         
  } else if(strcmp(eingabe, "Print humidity")==0) {
      Serial.print(F("Humidity level: "));
      Serial.print(humidity);
      Serial.print(F("%"));
      if (humInRange) {
        Serial.print(F(" (in range)"));   
    } else {
      Serial.print(F(" (out of range)"));
    }
  } else if(strcmp(eingabe, "Print CO2")==0) {
      Serial.print(F("CO2 level: "));
      Serial.print(eCO2);
      Serial.print(F(" ppm"));
      if (CO2InRange) {
        Serial.print(F(" (in range)"));    
    } else {
        Serial.print(F(" (out of range)"));
    }
  } else if(strcmp(eingabe, "Print temperature")==0) {
      Serial.print(F("Temperature: "));
      Serial.print(temp);
      Serial.print(F(" Celsius "));
      if (tempInRange) {
        Serial.print(F(" (in range)"));    
    } else {
        Serial.print(F(" (out of range)"));
    }      
  } else if(strcmp(eingabe, "Print LED level")==0) {
      Serial.print(F("LEDs at level "));
      if (brightness == 0) {
        Serial.print(F("15"));
      } else {
        Serial.print(F("8"));
      }
      Serial.print(F(" brightness (out of 0 - 15)\n"));
    //strncmp zur Überprüfung der ersten 9 Buchstaben
  } else if(strncmp(eingabe, "Set alarm", 9)==0) {
      int tempHour = atoi(eingabe+10);
      int tempMinute = atoi(eingabe+13);
    //Begrenzung der zulässigen Zeiten; die Zeitanzeige kann nicht höher werden
      if (tempHour <= 99 && tempMinute <= 59) {
        alarmHour = tempHour;
        alarmMinute = tempMinute;
    } else {
        Serial.println(F("Invalid time input. Try again.\n"));
      }
  } else {
      Serial.print(F("Invalid input. Try again.\n"));
  }
}

//zum Ausschalten von Testfunktionen
void serialTestTimer(){
  if (millis() >= lastExRed+5000) {
    if (digitalRead(RED_OUTPUT)) {
      digitalWrite(RED_OUTPUT, LOW);
      testRedLED = false;
    }
  }
  if (millis() >= lastExGreen+9000) {
    if (digitalRead(GREEN_OUTPUT)) {
      digitalWrite(GREEN_OUTPUT, LOW);
      testGreenLED = false;
    }
  }
  if (millis() >= lastExYellow+7000) {
    if (digitalRead(YELLOW_OUTPUT)) {
      digitalWrite(YELLOW_OUTPUT, LOW);
      testYellowLED = false;
    }
  }
  if (millis() >= lastExBuzzer+3000 && !alarmOn) {
     if (BUZZER){
      noTone(BUZZER);
      testBuzzer = false;
     }
  }
}

/*
 * Legt fest, was mit dem Summer und den Lichtern passiert,
 * wenn sie sich im Entfernungsbereich befinden.
 */
void distanceSensorOutput() {
  if (distance <= 20) {
    withinDist = true;
    if (distValue == 1 && !testBuzzer) {
      digitalWrite(RED_OUTPUT, HIGH);
      digitalWrite(YELLOW_OUTPUT, LOW);
      digitalWrite(GREEN_OUTPUT, LOW);
      tone(BUZZER, 100);
    } else if (distValue == 2 && !testBuzzer) {
        digitalWrite(RED_OUTPUT, LOW);
        digitalWrite(YELLOW_OUTPUT, HIGH);
        digitalWrite(GREEN_OUTPUT, LOW);
        tone(BUZZER, 1000);
    } else if (distValue == 3 && !testBuzzer) {
        digitalWrite(RED_OUTPUT, LOW);
        digitalWrite(YELLOW_OUTPUT, LOW);
        digitalWrite(GREEN_OUTPUT, HIGH);
        tone(BUZZER, 5000);
    }
  } else if (distance > 20 && !alarmOn && !testBuzzer &&
    !testRedLED && !testGreenLED && !testYellowLED) {
      withinDist = false;
      digitalWrite(RED_OUTPUT, LOW);
      digitalWrite(YELLOW_OUTPUT, LOW);
      digitalWrite(GREEN_OUTPUT, LOW);
      noTone(BUZZER);
    }
}

//legt fest, welche Meldung über die LED-Matrix laufen soll
void scroll() {
  /*
   * displayCounter = 1 und buttonToggle = 1 am Anfang,
   * um sicherzustellen, dass < time > das erste ist, was wir sehen
   */
  if (displayCounter == 1 && buttonToggle == true && !alarmOn) {
    scrollText(timeLetters, 8);
    buttonToggle = false;
    displayCounter += 1;
  } else if (displayCounter == 2 && buttonToggle == true && !alarmOn) {
      scrollText(tempLetters, 8);
      buttonToggle = false;
      displayCounter += 1;
  } else if (displayCounter == 3 && buttonToggle == true && !alarmOn) {
      scrollText(humLetters, 8);
      buttonToggle = false;
      displayCounter += 1;      
  } else if (displayCounter == 4 && buttonToggle == true && !alarmOn) {
      scrollText(CO2Letters, 8);
      buttonToggle = false;
      displayCounter = 1;
  }
}

//bestimmt, welche Zahlen angezeigt werden sollen
//(time, humidity, etc.)
void displayNums() {
  /*
   * Verzögerung bei der Entfernungserkennung, so dass < Zeit >
   * immer durchläuft, auch wenn sich ein Objekt innerhalb von
   * 20 cm befindet
   */
  if (millis() > 5000 && withinDist) {
    displayDist();
  } else {
    /*
     * beginnend mit '2', da die erste if-Anweisung in scroll()
     * 1 zu displayCounter hinzufügt
     */
    if (displayCounter == 2) {
      ledClock();
    } else if (displayCounter == 3) {
        displayTemp();
    } else if (displayCounter == 4) {
        displayHum();
    } else if (displayCounter == 1) {
        displayCO2();
    }
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

  pinMode(BUTTON, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(BUTTON), buttonInterrupt, CHANGE);
  
}

void loop() {
  distanceSensorOutput();
  currentMillis = millis();
  // 8 Zeilen in der Anzeige; 1000/8 = 125
  if (currentMillis - startMillis >= 125) {
    calcTime();
    scroll();
    displayNums();
    startMillis=currentMillis;
  }
  //schaltet den Alarm aus, wenn er eingeschaltet ist
  //und die Taste gedrückt wird
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
