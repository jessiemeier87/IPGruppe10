  #include <stdio.h>
  #include <stdlib.h>
  #include "LedControl.h"
  #include <binary.h>
  #include <string.h>

  LedControl led = LedControl(13, 12, 11, 4); //13 is orange, 12 is green, 11 is yellow

  #define TRIGPIN 4
  #define ECHOPIN 3
  #define MICRO_DELAY 10

  #define RED_OUTPUT 10
  #define YELLOW_OUTPUT 9
  #define GREEN_OUTPUT 8

  #define LSENSOR 5
  #define BUZZER 7
  #define BUTTON 2

  #define DISPLAY_WIDTH 32
  #define CHAR_WIDTH 4

  // used in displayZeit() and ledClock()
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

  //for dots
  unsigned long startMillis2;
  const unsigned long dotBlink = 2000;

  // secondShift increases every 125 miliseconds (1000/8), so that we can move the bits
  //for the right-most second counter on clock from the bottom up
  unsigned int secondShift = 0;

  //define time letters

  byte Z[8] = {0b0000, 0b0000, 0b1110, 0b0010, 0b0100, 0b1000, 0b1110, 0b0000};
  byte e[8] = {0b0000, 0b0000, 0b0111, 0b0101, 0b0111, 0b0100, 0b0011, 0b0000};
  byte i[8] = {0b0000, 0b0000, 0b0010, 0b0000, 0b0010, 0b0010, 0b0010, 0b0000};
  byte t[8] = {0b0000, 0b0000, 0b0010, 0b0111, 0b0010, 0b0010, 0b0011, 0b0000};
  byte lessThan[8] = {0b0000, 0b0000, 0b0010, 0b0100, 0b1000, 0b0100, 0b0010, 0b0000};
  byte greaterThan[8] = {0b0000, 0b0000, 0b0100, 0b0010, 0b0001, 0b0010, 0b0100, 0b0000};
  
  byte* zeitLetters[8] = {lessThan, lessThan, Z, e, i, t, greaterThan, greaterThan};

  /*
   regular bool + toggling wasn't working; tried volatile after reading
   this thread: https://forum.arduino.cc/t/changing-a-variable-flag-with-an-interrupt/690074
   used in displayZeit() and interrupt_function()
  */
  volatile bool buttonToggle = true;

  //timers in void loop()
  unsigned long currentMillis, startMillis;

  unsigned long buttonTime = 0, lastButtonTime = 0;

  int lValue; // used in lightMeasure() and serialCommands()

  //timers for turning off LEDs and buzzer after testing
  //used in readSerInput() and handleLightsNBuzzer()
  unsigned long lastExRed, lastExGreen, lastExYellow, lastExBuzzer;

  // variables for ledClock() and calcTime()
  int blinking = 0;
  int sek1 = 0;
  int sek10 = 0;
  int min1 = 0;
  int min10 = 0;
  int stu1 = 0;
  int stu10 = 0;

  //variables for reading serial input
  String userInput;
  char incomingByte;

  /* Gets executed when the button is pressed */
  void interrupt_function() {
    buttonTime = millis(); // sets 'buttonTime' to millis()
    /*
    check if buttonTime - lastButtonTime is > than 50 ms; if it is, then
    we know that the current state has changed
    */
    if ((buttonTime - lastButtonTime) > 50) { 
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

    unsigned long currentMillis2 = millis();
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
      lValue = map(brightness,0,1,15,4);
  
      lastMeasurement = lightMillis;
      led.setIntensity(0, lValue);
      led.setIntensity(1, lValue);
      led.setIntensity(2, lValue);
      led.setIntensity(3, lValue);
    }
  }
  
  void readSerInput() {
    if (Serial.available()) {
      incomingByte = Serial.read();
      if (incomingByte != '\n') {
        userInput += incomingByte;     
      } else {
        Serial.println("You typed: ");
        Serial.println(userInput);
        Serial.println(" ");
        serialCommands(userInput);
        userInput = "";
      }
    }
  }

  void serialCommands(String eingabe) {
    if (eingabe == "Test LED red") {
      digitalWrite(RED_OUTPUT, HIGH);
      lastExRed = millis();
    }
    else if (eingabe == "Test LED green") {
      digitalWrite(GREEN_OUTPUT, HIGH);
      lastExGreen = millis();      
    }
    else if (eingabe == "Test LED yellow") {
      digitalWrite(YELLOW_OUTPUT, HIGH);
      lastExYellow = millis();
    }
    else if (eingabe == "Test buzzer") {
      tone(BUZZER, 1000);
      lastExBuzzer = millis();
    }
    else if (eingabe == "Print distance") {
      Serial.println("Distance: ");
      Serial.print("20 ");
      Serial.print("cm\n");         
    }
    else if (eingabe == "Print humidity") {
      Serial.println("Humidity level: ");
      Serial.print("20\n");         
    }
    else if (eingabe == "Print C02 level") {
      Serial.println("C02 level: ");
      Serial.print("20\n");         
    }
    else if (eingabe == "Print temperature") {
      Serial.println("Temperature: ");
      Serial.print("20 ");
      Serial.print("Celsius\n");         
    }
    else if (eingabe == "Print LED brightness") {
      Serial.println("LEDs at level ");
      Serial.print(lValue);
      Serial.print(" brightness (out of 0 - 15)\n");
    } else {
      Serial.println("Invalid input. Try again.");
    }
  }

  void handleLightsNBuzzer(){
    if (millis() >= lastExRed+5000) { // checks to see if at least 5 seconds has past
      if (digitalRead(RED_OUTPUT)) {
        digitalWrite(RED_OUTPUT, LOW);
      }
    }
    if (millis() >= lastExGreen+5000) { // checks to see if at least 5 seconds has past
      if (digitalRead(GREEN_OUTPUT)) {
        digitalWrite(GREEN_OUTPUT, LOW);
      }
    }
    if (millis() >= lastExYellow+5000) { // checks to see if at least 5 seconds has past
      if (digitalRead(YELLOW_OUTPUT)) {
        digitalWrite(YELLOW_OUTPUT, LOW);
      }
    }
    if (millis() >= lastExBuzzer+5000) { // checks to see if at least 5 seconds has past
       if (BUZZER){
        noTone(BUZZER);
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
    readSerInput();
    handleLightsNBuzzer();
  }
