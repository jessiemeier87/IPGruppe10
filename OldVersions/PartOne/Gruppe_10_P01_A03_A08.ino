
#define RED_OUTPUT 10
#define YELLOW_OUTPUT 9
#define GREEN_OUTPUT 8
#define BUZZER 7
#define BUTTON 2
#define DEBOUNCE_DELAY 50

// ul for increment
unsigned long lastExecution = 0;

// ul for buzzer
unsigned long lastBuzz = -1; // start at -1 so it triggers for wert == 0

// uls for LEDs
// individual counters for the LEDs
unsigned long lastExRed = 0;
unsigned long lastExYellow = 0;
unsigned long lastExGreen = 0;

// ul for buttonChecker function
unsigned long lastDebounceTime = 0;

// ul for interrupt_function
unsigned long buttonTime = 0;
unsigned long lastButtonTime = 0;

// variables for buttonChecker
int buttonValue = 1;
int lastButtonState = 1;

int wert = 0; // Is incremented every 10 seconds

/* Gets executed when the button is pressed */
void interrupt_function() {
  buttonTime = millis(); // sets 'buttonTime' to millis()
  /*
  check if buttonTime - lastButtonTime is > than 50 ms; if it is, then
  we know that the current state has changed
  */
  if ((buttonTime - lastButtonTime) > 50) { 
    Serial.println("Taster Interrupt");
    lastButtonTime = buttonTime; // set lastButtonTime, so we can detect next change
  }
}

/* Periodically increments wert */
void increment(){
  if (millis() >= lastExecution+10000) { // checks to see if at least 10 seconds has past
    Serial.println(wert);
    lastExecution=millis(); // resets lastExecution so we can count up to 10 again
    wert+= 1;
  }
}

/* Runs the buzzer every 30 seconds */
void buzzy(){
  if (wert %3 == 0 && lastBuzz != wert) { // lastBuzz != wert, so it only buzzes once over 10 sec period
      tone(BUZZER, 1000);
      long current = millis();
      while(millis() < current+500);
      noTone(BUZZER);
      lastBuzz=wert;
  } 
}

/* Handles the flashing of the red light */
void handleRedLight(){
  // checks millis() and, if it is >= to the lastExRed counter, toggles red LEDs
  if (millis() >= lastExRed+10000) { // checks to see if at least 10 seconds has past
    if (digitalRead(RED_OUTPUT)) {
      digitalWrite(RED_OUTPUT, LOW);
    }
    else {
      digitalWrite(RED_OUTPUT, HIGH);
    }
    lastExRed=millis(); // resets lastExRed counter so we can count up to 10 again
  }
}

/* Handles the flashing of the yellow light */
void handleYellowLight(){
    // checks millis() and, if it is >= to the lastExYellow counter, toggles yellow LEDs
  if (millis() >= lastExYellow+1000) { // checks to see if at least 1 second has past
    if (digitalRead(YELLOW_OUTPUT)) {
      digitalWrite(YELLOW_OUTPUT, LOW);
    }
    else {
      digitalWrite(YELLOW_OUTPUT, HIGH);
    }
    lastExYellow=millis(); // resets lastExYellow counter so we can count up to 1 again
  }
}

/* Handles the flashing of the green light */
void handleGreenLight(){
  // checks millis() and, if it is >= to the lastExGreen counter, toggles green LEDs
  if (millis() >= lastExGreen+5000) { // checks to see if at least 5 seconds has past
    if (digitalRead(GREEN_OUTPUT)) {
      digitalWrite(GREEN_OUTPUT, LOW);
    }
    else {
      digitalWrite(GREEN_OUTPUT, HIGH);
    }
  lastExGreen=millis(); // resets lastExGreen counter so we can count up to 5 again
  }
}

/* function from P01.6; left in, but commented out,
 because it was made irrelevant by interrupt
 */
//void buttonChecker() {
//  int sensorValue = digitalRead(BUTTON); // gets current sensor value
//  /*
//  check if current and last known state are different
//  so we only get a print-out if they're different (otherwise it's just a constant printout)
//  */
//  if (sensorValue != lastButtonState) {
//    lastDebounceTime = millis(); // reset debounce timer
//  }
//  /*
//  check if current millis() - lastDebounceTime is > than DEBOUNCE_DELAY; if it is, then
//  we know that the current state has changed
//  */
//  if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
//    if(sensorValue != buttonValue) { // check if there's a difference
//      buttonValue = sensorValue; // make two values the same
//      if (sensorValue == LOW) { 
//        Serial.println("pressed"); // if low, print pressed
//      }
//      else {
//        Serial.println("Not pressed"); // else high, print not pressed
//      }
//    }
//  }
//  lastButtonState = sensorValue; // set lastButtonState to sensorValue so we can detect next change
//}

void setup() {
  Serial.begin(9600);
  pinMode(RED_OUTPUT, OUTPUT);
  digitalWrite(RED_OUTPUT, HIGH);
  
  pinMode(YELLOW_OUTPUT, OUTPUT);
  digitalWrite(YELLOW_OUTPUT, HIGH);
  
  pinMode(GREEN_OUTPUT, OUTPUT);
  digitalWrite(GREEN_OUTPUT, HIGH);

  pinMode(BUZZER, OUTPUT);

  pinMode(BUTTON, INPUT_PULLUP); // for pull-up resistor

  attachInterrupt(digitalPinToInterrupt(BUTTON), interrupt_function, CHANGE);

}

void loop() {
  //Serial.println("Hello world"); // old code from P01.3
  //delay(5000); // old code from P01.3

  increment();
  buzzy();
  handleRedLight();
  handleYellowLight();
  handleGreenLight();
}
