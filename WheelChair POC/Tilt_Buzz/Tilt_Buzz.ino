#include <LiquidCrystal.h>

LiquidCrystal lcd(19, 23, 18, 17, 16, 15);
unsigned long display_time;
const int buzzPin = 27;
const int tiltPin = 13;		// tilt sensor pin is connected to pin 2
const int ledPin = 2;		


void setup() {
  Serial.begin(115200);
	pinMode(tiltPin, INPUT);		// set sensor pin as an INPUT pin
	digitalWrite(tiltPin, HIGH);	// turn on the built in pull-up resistor
	pinMode(ledPin, OUTPUT);		// set LED pin as an OUTPUT pin
  pinMode(buzzPin, OUTPUT);		// set LED pin as an OUTPUT pin
  // Initialize LEDC
  ledcSetup(0, 3000, 8);  // Use LEDC channel 0, set the frequency to 3000 Hz, and use 8-bit resolution

  // Attach the buzzer to the LEDC channel
  ledcAttachPin(buzzPin, 0);

  //the lCD inital
  lcd.begin(16, 2);  //sets beggining and end of text on lcd screen
  lcd.print("Detetcion lvl 3:");
  
}
int trigger_time = 0;
const int debounce_delay = 200;
const int frequency = 2500;
const int duration = 10;
bool lastTiltState = 0;
bool tiltState = false;

void loop() {
  lcd.begin(16, 2);  //sets beggining and end of text on lcd screen
  lcd.print("Detetcion lvl 3:");

  bool tiltState = digitalRead(tiltPin);
  if(tiltState)
    Serial.println("Tilt");
  else
    Serial.println("No Tilt");


	if (tiltState != lastTiltState) { // check if the pin is high
   
    trigger_time = millis();
 
	}
  
  if(!tiltState && ( (millis() -trigger_time) > debounce_delay) ){
    Serial.println("Buzz");
    digitalWrite(ledPin, HIGH);	// turn on the LED
    // analogWrite(buzzPin, 255);	// turn on the LED
    tone(buzzPin, frequency, duration);
    lcd.setCursor(0, 1);
    lcd.print("  Audio  Output ! ");
  }
  else { // if it isn't
		digitalWrite(ledPin, LOW);	// do the opposite
    noTone(buzzPin);
	}
  lastTiltState = tiltState;
  delay(100);
}