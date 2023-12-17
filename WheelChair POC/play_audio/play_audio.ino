

// https://github.com/pcbreflux/espressif/blob/master/esp32/arduino/sketchbook/ESP32_DFPlayer_full/ESP32_DFPlayer_full.ino
/***************************************************
 DFPlayer - A Mini MP3 Player For Arduino
 <https://www.dfrobot.com/index.php?route=product/product&product_id=1121>
 
 ***************************************************
 This example shows the all the function of library for DFPlayer.
 
 Created 2016-12-07
 By [Angelo qiao](Angelo.qiao@dfrobot.com)
 
 GNU Lesser General Public License.
 See <http://www.gnu.org/licenses/> for details.
 All above must be included in any redistribution
 ****************************************************/

#include <Arduino.h>
#include <DFRobotDFPlayerMini.h>

HardwareSerial mySoftwareSerial(1);
DFRobotDFPlayerMini myDFPlayer;

const int button1Pin = 9;  // the number of the pushbutton pin
const int button2Pin = 10;  // the number of the pushbutton pin
const int button3Pin = 11;  // the number of the pushbutton pin

int button1 = 0;  // variable for reading the pushbutton status
int button2 = 0;  // variable for reading the pushbutton status
int button3 = 0;  // variable for reading the pushbutton status

void printDetail(uint8_t type, int value);

// esp32 
// #define TX_PIN 17
// #define RX_PIN 16 

// esp32 s3
#define TX_PIN 17
#define RX_PIN 18

#define BloodType 1
#define Emergency 2
#define Stuck 3
const int reset_time = 10000;
int start_timer = 0;
int end_timer = 0;
void setup() {
  // button setup 
  pinMode(button1Pin, INPUT);
  pinMode(button2Pin, INPUT);
  pinMode(button3Pin, INPUT);

  Serial.begin(115200);
  mySoftwareSerial.begin(9600, SERIAL_8N1, TX_PIN ,  RX_PIN);  // speed, type, TX, RD
  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    
    Serial.println(myDFPlayer.readType(),HEX);
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  Serial.println(F("DFPlayer Mini online."));
  
  myDFPlayer.setTimeOut(500); //Set serial communictaion time out 500ms
  
  //----Set volume----
  myDFPlayer.volume(15);  //Set initial volume value (0~30).
  myDFPlayer.volumeUp(); //Volume Up
  myDFPlayer.volumeDown(); //Volume Down
  
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
  
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
  
  int delayms=100;

  //----Read imformation----
  Serial.println(F("readState--------------------"));
  Serial.println(myDFPlayer.readState()); //read mp3 state
  Serial.println(F("readVolume--------------------"));
  Serial.println(myDFPlayer.readVolume()); //read current volume
  //Serial.println(F("readEQ--------------------"));
  //Serial.println(myDFPlayer.readEQ()); //read EQ setting
  Serial.println(F("readFileCounts--------------------"));
  Serial.println(myDFPlayer.readFileCounts()); //read all file counts in SD card
  Serial.println(F("readCurrentFileNumber--------------------"));
  Serial.println(myDFPlayer.readCurrentFileNumber()); //read current play file number
  Serial.println(F("readFileCountsInFolder--------------------"));
  Serial.println(myDFPlayer.readFileCountsInFolder(3)); //read fill counts in folder SD:/03
  Serial.println(F("--------------------"));

  // Serial.println(F("myDFPlayer.play(1)"));
  // myDFPlayer.play(1);  //Play the first mp3 
  start_timer = 0;
}
int wrong_count = 0;
void loop() {
  // use Serial to send messages, and debug
  /*if (Serial.available()) {
      String inData = "";
      inData = Serial.readStringUntil('\n');
      if (inData.startsWith("p")) {
        Serial.println(F("previous--------------------"));
        myDFPlayer.previous();
        Serial.println(myDFPlayer.readCurrentFileNumber()); //read current play file number
      } else if (inData.startsWith("+")) {
        Serial.println(F("up--------------------"));
        myDFPlayer.volumeUp();
        Serial.println(myDFPlayer.readVolume()); //read current volume
      } else if (inData.startsWith("-")) {
        Serial.println(F("down--------------------"));
        myDFPlayer.volumeDown();
        Serial.println(myDFPlayer.readVolume()); //read current volume
      }
  }*/

  button1 = digitalRead(button1Pin);
  button2 = digitalRead(button2Pin);
  button3 = digitalRead(button3Pin);

  int delayms = 100;
  // Serial.print(button1);
  // Serial.print(button2);
  // Serial.println(button3);

  if(button1==HIGH){  
    Serial.println("Play Blood Type");
    myDFPlayer.play(1);  //Play the 1 mp3
    // myDFPlayer.next();
    delay(delayms);
  }
  else if(button2==HIGH){
    Serial.println("Play Emergency");
    myDFPlayer.play(2);  //Play the 2 mp3
    // myDFPlayer.pause();
    delay(delayms);
  }
  else if(button3==HIGH){
    Serial.println("Play Stuck");
    myDFPlayer.play(3);  //Play the 3 mp3
    // myDFPlayer.start();
    delay(delayms);
  }

    
  // if (myDFPlayer.available()) {
  //   Serial.println("Available");
  //   // this means that before the previous is done playing it can't play another one 
    // if (myDFPlayer.readType()==DFPlayerPlayFinished) {
    // // myDFPlayer.pause();  //pause the mp3 first
    //   Serial.println(myDFPlayer.read());
    //   if(button1==HIGH){  
    //     Serial.println("Play Blood Type");
    //     myDFPlayer.play(BloodType);  //Play the 1 mp3
    //     delay(delayms);
    //   }
    //   else if(button2==HIGH){
    //     Serial.println("Play Emergency");
    //     myDFPlayer.play(Emergency);  //Play the 2 mp3
    //     delay(delayms);
    //   }
    //   else if(button3==HIGH){
    //     Serial.println("Play Stuck");
    //     myDFPlayer.play(Stuck);  //Play the 3 mp3
    //     delay(delayms);
    //   }
    // Serial.println(myDFPlayer.readCurrentFileNumber()); //read current play file number
    // delay(500);
    // }
  delay(delayms);
  // }
  if (myDFPlayer.available()) {
    reset_if_wrong(myDFPlayer.readType());
    printDetail(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
  }
}

void reset_if_wrong(uint8_t type){
  switch (type) {
    case TimeOut:
    case WrongStack:
      wrong_count++;
      Serial.print(wrong_count);
      if(wrong_count > 5){
        wrong_count = 0;
        mySoftwareSerial.write(0x0C);
        Serial.println("Something wrong");
      }
      break;
  }
}

void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}