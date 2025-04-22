#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <TinyGPSPlus.h>
#include <iostream>
using namespace std;

#define RXD2 16
#define TXD2 17
#define GPS_BAUD 9600

Adafruit_MPU6050 mpu;           //Object to handle mpus
TinyGPSPlus gps;                //Object to handle TinyGPS
HardwareSerial gpsSerial(2);    //Create instance of HardwareSerial class for Serial 2

uint8_t sampleDelay = 100;        //Delay between loops of coordinate update
uint8_t delays = 10;              //Delay between MPU reading for calculating distance change
int MPU1A = 0;  int MPU1B = 0;    //Channel 0
int MPU2A = 0;  int MPU2B = 0;    //Channel 1
int MPU3A = 0;  int MPU3B = 0;    //Channel 2
int coordx; = int coordy;         //Initialise position coordinates x and y
unsigned long startTime;                //First time, in ms, a reading is taken
unsigned long Time;                     //Tracker for time in ms
int startLat;                     //Initial longitude of lap
int startLong;                    //Initial latitude of lap

void TCA9548A (uint8_t bus) {     //Function to select channel on multiplexer
  Wire.beginTransmission(0x70);
  Wire.write(1<<bus);
  Wire.endTransmission();
}

void setup() {
  Serial.begin(115200);
  while (!Serial) {delay(1);}    //Wait for serial to begin before continuing setup
  //Loop to check if all MPUs connected to multiplexer functional
  for (int i=0; i<3; i++) {
    TCA9548A(i);
    if (!mpu.begin()) {
      cout<<"\n\r"<<"Failed initialise MPU on channel"<<i;
    } else {;}
  }
  gpsSerial.begin(GPS_BAUD, SERIAL_8N1, RXD2, TXD2);
}

void loop() {
  unsigned long progBTime = millis();     //Program start time
     
  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
  }
  for (int j=0; j<1; j++) {
    if (gps.location.isUpdated()) {
      startLat = gps.location.lat();
      startLong = gps.location.lng();
      coordx = 0;
      coordy = 0;
      Time = 0;
      startTime = millis();
      /////////CODE TO SEND POINT TO RECEIVER//////////
    }
  } else {
    if (gps.location.isUpdated()) {
      //coordx = (gps.location.lng() - startLat) converted to metres
      //coordy = (gps.location.lat() - startLong) converted to metres
      //Time = millis() - startTime;
      //////////CODE TO SEND POINT AND TIME TO RECEIVER//////////
    } else {
      //read MPU 1, 2 & 3 accel store in "A"
      //delay(delays);
      //read MPUs accel again and store in "B"
      //Distance = ((A+B)/2)*(delays)^2
      //Add distance to coordx and coordy (distance is calc'd w/ regards to x and y to begin w/)
      ////////////CODE TO SEND POINT AND TIME TO RECEIVER////////
    }
  }
  delay(sampleDelay);
}
