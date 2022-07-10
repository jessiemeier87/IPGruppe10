#define TRIGPIN 5
#define ECHOPIN 4
#define MICRO_DELAY 10

#define RED_OUTPUT 10
#define YELLOW_OUTPUT 9
#define GREEN_OUTPUT 8

#define BUZZER 7
#define BUTTON 2
#define DEBOUNCE_DELAY 50

// uls for sensor
unsigned long pulseWidth, distance, currentMicros;
unsigned long lastMeasurement=0;

// ul for buttonChecker function
unsigned long lastDebounceTime = 0;

int buttonValue = 1;
int lastButtonState = 1;

void setup() {
  Serial.begin(115200);

  pinMode(TRIGPIN, OUTPUT);
  pinMode(ECHOPIN, INPUT);
  digitalWrite(TRIGPIN, LOW);

  pinMode(RED_OUTPUT, OUTPUT);
  pinMode(YELLOW_OUTPUT, OUTPUT);
  pinMode(GREEN_OUTPUT, OUTPUT);

  pinMode(BUZZER, OUTPUT);

  pinMode(BUTTON, INPUT_PULLUP); // for pull-up resistor

}

/* Evaluates if the button was pressed */
void buttonChecker() {
  int sensorValue = digitalRead(BUTTON); // gets current sensor value
  /*
   * check if current and last known state are different
   * so we only get a print-out if they're different (otherwise it's just a constant printout)
   */
  if (sensorValue != lastButtonState) {
    lastDebounceTime = millis(); // reset debounce timer
  }
  /*
   * check if current millis() - lastDebounceTime is > than DEBOUNCE_DELAY; if it is, then
   * we know that the current state has changed
   */
  if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
    if(sensorValue != buttonValue) { // check if there's a difference
      buttonValue = sensorValue; // make two values the same
      if (sensorValue == LOW) { 
        Serial.print("Abstand: ");
        Serial.print(distance);
        Serial.println(" cm");
      }
    }
  }
  lastButtonState = sensorValue; // set lastButtonState to sensorValue so we can detect next change
}



void sensor() {
  currentMicros = micros();

  /* Only measure once a second */
  if ((currentMicros - lastMeasurement) >= 1000000) {
    digitalWrite(TRIGPIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGPIN, LOW);
    pulseWidth = pulseIn(ECHOPIN, HIGH);
    distance = (pulseWidth * 0.0343)/2;  // distance calc. from https://www.makerguides.com/hc-sr04-arduino-tutorial/
    Serial.println(distance);
    
    int val = map(distance, 2, 20, 1, 3);
  
    if (val == 1) {
      digitalWrite(RED_OUTPUT, HIGH);
      digitalWrite(YELLOW_OUTPUT, LOW);
      digitalWrite(GREEN_OUTPUT, LOW);
      tone(BUZZER, 100);
    }
    else if (val == 2) {
      digitalWrite(RED_OUTPUT, LOW);
      digitalWrite(YELLOW_OUTPUT, HIGH);
      digitalWrite(GREEN_OUTPUT, LOW);
      tone(BUZZER, 1000);
    }
    else if (val == 3) {
      digitalWrite(RED_OUTPUT, LOW);
      digitalWrite(YELLOW_OUTPUT, LOW);
      digitalWrite(GREEN_OUTPUT, HIGH);
      tone(BUZZER, 5000);
    }
    else if (val > 3) {
      digitalWrite(RED_OUTPUT, LOW);
      digitalWrite(YELLOW_OUTPUT, LOW);
      digitalWrite(GREEN_OUTPUT, LOW);
      noTone(BUZZER);
    }

    lastMeasurement = currentMicros;
  }
}

void loop() {
  sensor();
  buttonChecker();
}
