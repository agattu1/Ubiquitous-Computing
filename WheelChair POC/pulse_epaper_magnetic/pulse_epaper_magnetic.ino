/*
   Sketch to handle each sample read from a PulseSensor.
   Typically used when you don't want to use interrupts
   to read PulseSensor voltages.

   Here is a link to the tutorial that discusses this code
   https://pulsesensor.com/pages/getting-advanced

   Copyright World Famous Electronics LLC - see LICENSE
   Contributors:
     Joel Murphy, https://pulsesensor.com
     Yury Gitman, https://pulsesensor.com
     Bradford Needham, @bneedhamia, https://bluepapertech.com

   Licensed under the MIT License, a copy of which
   should have been included with this software.

   This software is not intended for medical use.
*/

/*
   Every Sketch that uses the PulseSensor Playground must
   define USE_ARDUINO_INTERRUPTS before including PulseSensorPlayground.h.
   Here, #define USE_ARDUINO_INTERRUPTS false tells the library to
   not use interrupts to read data from the PulseSensor.

   If you want to use interrupts, simply change the line below
   to read:
     #define USE_ARDUINO_INTERRUPTS true

   Set US_PS_INTERRUPTS to false if either
   1) Your Arduino platform's interrupts aren't yet supported
   by PulseSensor Playground, or
   2) You don't wish to use interrupts because of the side effects.

   NOTE: if US_PS_INTERRUPTS is false, your Sketch must
   call pulse.sawNewSample() at least once every 2 milliseconds
   to accurately read the PulseSensor signal.
*/
#define USE_ARDUINO_INTERRUPTS false
#include <PulseSensorPlayground.h>
#include <epd.h>
#include <String.h>

/*
   The format of our output.

   Set this to PROCESSING_VISUALIZER if you're going to run
    the Processing Visualizer Sketch.
    See https://github.com/WorldFamousElectronics/PulseSensor_Amped_Processing_Visualizer

   Set this to SERIAL_PLOTTER if you're going to run
    the Arduino IDE's Serial Plotter.
*/
const int OUTPUT_TYPE = SERIAL_PLOTTER;

/*
   Pinout:
     PULSE_INPUT = Analog Input. Connected to the pulse sensor
      purple (signal) wire.
     PULSE_BLINK = digital Output. Connected to an LED (and 1K series resistor)
      that will flash on each detected pulse.
     PULSE_FADE = digital Output. PWM pin onnected to an LED (and 1K series resistor)
      that will smoothly fade with each pulse.
      NOTE: PULSE_FADE must be a pin that supports PWM. Do not use
      pin 9 or 10, because those pins' PWM interferes with the sample timer.
     THRESHOLD should be set higher than the PulseSensor signal idles
      at when there is nothing touching it. The expected idle value
      should be 512, which is 1/2 of the ADC range. To check the idle value
      open a serial monitor and make note of the PulseSensor signal values
      with nothing touching the sensor. THRESHOLD should be a value higher
      than the range of idle noise by 25 to 50 or so. When the library
      is finding heartbeats, the value is adjusted based on the pulse signal
      waveform. THRESHOLD sets the default when there is no pulse present.
      Adjust as neccesary.
*/
const int PULSE_INPUT = 5;
const int PULSE_BLINK = LED_BUILTIN;
const int switchPin = 3;
const int wake_up = 47;
const int reset = 21;
const int debug_button = 37;
//const int PULSE_FADE = 5;
const int THRESHOLD = 550;   // Adjust this number to avoid noise when idle

/*
- name : Robert “Trey” Quinn
- yoB : 1999
Condition: Cerebrela palsy diplegia
- blood type : O+
- The pulse information
*/


/*
   samplesUntilReport = the number of samples remaining to read
   until we want to report a sample over the serial connection.

   We want to report a sample value over the serial port
   only once every 20 milliseconds (10 samples) to avoid
   doing Serial output faster than the Arduino can send.
*/
byte samplesUntilReport;
const byte SAMPLES_PER_SERIAL_SAMPLE = 10;

/*
   All the PulseSensor Playground functions.
*/
PulseSensorPlayground pulseSensor;

bool crash = 0;
int bpm = 65;
void print_vitals(){
  epd_set_color(BLACK, WHITE);
  epd_clear();

  epd_set_en_font(ASCII48);
  epd_disp_string("Detection Level 2: Magnetic contact", 10, 20);
  epd_disp_string("Name: Robert \"Trey\" Quinn", 10, 120);
  epd_disp_string("Year of Birth: 2000 ", 10, 220); // + 80
  epd_disp_string("Condition: Cerebral palsy diplegia", 10, 320); // + 80
  epd_disp_string("Blood Type: O-", 10, 420); // + 80

  //HERE************************************************************
  //CHANGE THIS************************************************************
  //IF YOU WANT************************************************************
  //PULSE TO BE************************************************************
  //SOMETHING ELSE************************************************************
  bpm = bpm + random(-3,3);
  String pulseString = "BPM: " + String(bpm);

  if (pulseSensor.sawNewSample()) {
    Serial.println("sawnewsample");
    if (--samplesUntilReport == (byte) 0) {
      samplesUntilReport = SAMPLES_PER_SERIAL_SAMPLE;

      if (pulseSensor.sawStartOfBeat()) {
        // Serial.println(" and you");
        Serial.println("sawbeat");

        pulseSensor.outputBeat();
        int pulse = pulseSensor.getBeatsPerMinute();
        pulseString = "BPM: " + String(pulse);
        // crash = false;
        
        
      }
    }
  epd_disp_string(pulseString.c_str(), 10, 520); // + 80
  epd_udpate();

  delay(5000);
  }

}
void setup() {
  /*
     Use 115200 baud because that's what the Processing Sketch expects to read,
     and because that speed provides about 11 bytes per millisecond.

     If we used a slower baud rate, we'd likely write bytes faster than
     they can be transmitted, which would mess up the timing
     of readSensor() calls, which would make the pulse measurement
     not work properly.
  */
  Serial.begin(115200);

  // Configure the PulseSensor manager.
  pulseSensor.analogInput(PULSE_INPUT);
  pulseSensor.blinkOnPulse(PULSE_BLINK);
  //pulseSensor.fadeOnPulse(PULSE_FADE);

  pulseSensor.setSerial(Serial);
  pulseSensor.setOutputType(OUTPUT_TYPE);
  pulseSensor.setThreshold(THRESHOLD);

  // Skip the first SAMPLES_PER_SERIAL_SAMPLE in the loop().
  samplesUntilReport = SAMPLES_PER_SERIAL_SAMPLE;

  // Now that everything is ready, start reading the PulseSensor signal.
  if (!pulseSensor.begin()) {
    /*
       PulseSensor initialization failed,
       likely because our Arduino platform interrupts
       aren't supported yet.

       If your Sketch hangs here, try changing USE_PS_INTERRUPT to false.
    */
    for(;;) {
      // Flash the led to show things didn't work.
      digitalWrite(PULSE_BLINK, LOW);
      delay(50); Serial.println('!');
      digitalWrite(PULSE_BLINK, HIGH);
      delay(50);
    }
    
  }
  crash = false;

  epd_init(wake_up,reset);
  epd_wakeup(wake_up);
  epd_set_memory(MEM_NAND);
  epd_clear();

  pinMode(debug_button, INPUT);
  pinMode(switchPin, INPUT_PULLUP);

}
// int start = 0;
// int end = 0;
// bool trigger = 0;

void loop() {

  int switchState = digitalRead(switchPin);
  int buttonState = digitalRead(debug_button);
  // Serial.println(buttonState);
  if (buttonState == HIGH) {
    
    // crash = true;
    Serial.println("Dec2: Wheelchair Crashed");
  } 
  else {
    // Serial.println("Dec2: Wheelchair Intact");

  }
  

  // Check the state and print the corresponding message
  // if (switchState == HIGH) {
  //   // Serial.println("Dec2: Wheelchair Intact");
  //   crash = true;
  //   Serial.println("Dec2: Wheelchair Crashed");
  // } else {
  //   crash = false;
  //   // Serial.println("Dec2: Wheelchair Crashed");
  // }
  if (switchState == LOW){
    epd_set_color(BLACK, WHITE);
    epd_clear();

    epd_set_en_font(ASCII48);
    epd_disp_string("Detection Level 2: No Fall",10,20);
   // epd_disp_string("Magnetic Contact Jurisdiction",10,120);

    epd_udpate();
    delay(5000);
  }
  else {
    print_vitals();

  }
  if(crash==true){
    if (switchState == LOW) {
      Serial.println("Clear");
      crash = false;
      // epd_clear();
      // epd_enter_stopmode();
    }
  }
  else if(crash == false){
    if(switchState==HIGH){
      Serial.println("Dec2: Wheelchair Crashed");
      crash = true;
    }
  }
  Serial.println(crash);
  // end = millis();
  // if(end - start==500){
  //   Serial.println(trigger);
  //   trigger = ~trigger;
  //   start = end;
  // }
  
    /*******
      Here is a good place to add code that could take up
      to a millisecond or so to run.
    *******/
  
  delay(50);

}

