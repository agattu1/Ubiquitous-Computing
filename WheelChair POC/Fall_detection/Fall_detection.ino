// Automations for ABLE Alliance
// A Georgia Tech Crash Service & Detetction system for Wheelchair Users
// Ubiquitous Computing

/////////////////////////////////////////////////////////// inlcuded libraries
#include <esp_crt_bundle.h>
#include <ssl_client.h>
#include <WiFiClient.h>

#include <HTTPClient.h>
#include <LiquidCrystal.h>

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>

const char* ssid = "A-XII Pro";     //replace with name of network your computer and esp is connected to
const char* password = "anirudh1";  //network password
WiFiClient wifiClient;

const String REQUEST_URL = "http://172.20.10.7:7999/wheelchair/OPEN";  //Replace the 'xxx' with the local IP on your local server
int requestCounter = 0;


#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include "Capactive.h"
//////////////////////////////////////////////////////////n

Capactive capactive(2);
//int touch_pin = 2;

const int greenLED = 1;
const int redLED = 4;

Adafruit_MPU6050 mpu;

void setup(void) {
  Serial.begin(115200);

  pinMode(greenLED,OUTPUT);
  pinMode(redLED,OUTPUT);

  ////////////////////////////////////////////////////n
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to Network...");
  } 
  Serial.println(WiFi.localIP());
  /////////////////////////////////////////////////////////n

  while (!Serial)
    delay(10);  // will pause Zero

  Serial.println("Adafruit MPU6050!");

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("No MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set: ");
  switch (mpu.getAccelerometerRange()) {
    case MPU6050_RANGE_2_G:
      Serial.println("+-2G");
      break;
    case MPU6050_RANGE_4_G:
      Serial.println("+-4G");
      break;
    case MPU6050_RANGE_8_G:
      Serial.println("+-8G");
      break;
    case MPU6050_RANGE_16_G:
      Serial.println("+-16G");
      break;
  }


  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.print("Filter bandwidth set: ");
  switch (mpu.getFilterBandwidth()) {
    case MPU6050_BAND_260_HZ:
      Serial.println("260 Hz");
      break;
    case MPU6050_BAND_184_HZ:
      Serial.println("184 Hz");
      break;
    case MPU6050_BAND_94_HZ:
      Serial.println("94 Hz");
      break;
    case MPU6050_BAND_44_HZ:
      Serial.println("44 Hz");
      break;
    case MPU6050_BAND_21_HZ:
      Serial.println("21 Hz");
      break;
    case MPU6050_BAND_10_HZ:
      Serial.println("10 Hz");
      break;
    case MPU6050_BAND_5_HZ:
      Serial.println("5 Hz");
      break;
  }

  Serial.println("");
  delay(100);
}








void makeRequest() {  //local server
  HTTPClient http;

  http.begin(wifiClient, REQUEST_URL.c_str());
  int httpResponseCode = http.GET();
  Serial.println(httpResponseCode);

  http.end();
}























void loop() {

  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  bool denied = false;
  bool detected = false;
  bool moving = false;
  /* Print out the values */
  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");
  denied = capactive.isSecure();
  //int num = touchRead(touch_pin);
 
  if (a.acceleration.y < 2.0) {
    detected = true;
  }
  if ((a.acceleration.x + a.acceleration.y + a.acceleration.z) > 11.0) {
    moving = true;
  }
  if (! denied) {
    digitalWrite(greenLED,HIGH);
    digitalWrite(redLED,LOW);
    Serial.println("DENIED");
    delay(5000);
  } else if (moving) {
    digitalWrite(greenLED,HIGH);
    digitalWrite(redLED,LOW);
    Serial.println("MOVING");
  } else if (detected) {
    digitalWrite(greenLED,LOW);
    digitalWrite(redLED,HIGH);
    Serial.println("FALL DETECTED");
    /////////////////////////////////////////////////n
    requestCounter++;

    if (requestCounter % 2) {
      Serial.print("Request Counter is at: ");
      Serial.println(requestCounter);

      makeRequest();
      delay(10000);  //This delay is added so it doesnt get triggered from the door opening
                     //and closing. May need to be increased for your usage
    }
    ////////////////////////////////////////////////////
  } else {
    digitalWrite(greenLED,HIGH);
    digitalWrite(redLED,LOW);
    Serial.println("NO FALL");
  }
  Serial.println("");
  delay(500);
}
