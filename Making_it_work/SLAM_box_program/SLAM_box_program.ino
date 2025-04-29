//////////////////////////////////SLAM BOX PROGRAM/////////////////////////////////
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
#include <iostream>
#include <WiFi.h>
#include <Wire.h>
#include <esp_now.h>
#include <Arduino.h>
using namespace std;

//////////////////////////////////Position sensors//////////////////////////////////
Adafruit_MPU6050 mpu;           //Object to handle mpus
TinyGPSPlus gps;                //Object to handle TinyGPS
SoftwareSerial ss(16, 17);      //Create instance of SoftwareSerial class for Serial 2 on RX pin 16 & TX 17
static const uint32_t GPSBaud = 9600; //Baud rate of GPS

uint8_t sampleDelay = 100;        //Delay between loops of coordinate update
uint8_t delays = 10;              //Delay between MPU reading for calculating distance change
int MPU1A = 0;  int MPU1B = 0;    //Channel 0
int MPU2A = 0;  int MPU2B = 0;    //Channel 1
int MPU3A = 0;  int MPU3B = 0;    //Channel 2
double coordx = 0; double coordy = 0;         //Initialise position coordinates x and y
unsigned long startTime;                //First time, in ms, a reading is taken
unsigned long timeHour;                     //Tracker for time
unsigned long timeMin;
unsigned long timeSec;
unsigned long timeCentSec;
int startLat;                     //Initial longitude of lap
int startLong;                    //Initial latitude of lap

//////////////////////////////////////Wireless//////////////////////////////////////
uint8_t destMACaddress[] = {0x20, 0x43, 0xa8, 0x64, 0x16, 0x20};
int FLdis = 0; int FMdis = 0; int FRdis = 0;
int BLdis = 0; int BMdis = 0; int BRdis = 0;
int FLang = 0; int FRang = 0;
int BLang = 0; int BRang = 0;

typedef struct struct_message1 {
  int FLdis; int FMdis; int FRdis;
  int BLdis; int BMdis; int BRdis;

  int FLang; int FRang;
  int BLang; int BRang;
} struct_message1;
struct_message1 incomingReadings;

typedef struct struct_message2 {
  int FLdis; int FMdis; int FRdis;
  int BLdis; int BMdis; int BRdis;

  int FLang; int FRang;
  int BLang; int BRang;
  double coordx; double coordy;
  int timeHour; int timeMin; int timeSec; int timeCentSec;
} struct_message2;
struct_message2 outgoingReadings;

String success;

esp_now_peer_info_t peerInfo;
/////////////////////////////////////Main Loops/////////////////////////////////////
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
  ss.begin(GPSBaud);
  WiFi.mode(WIFI_STA);
  if(esp_now_init() != ESP_OK) {
    Serial.println("Error initialising ESP_NOW");
    return;
  }
  //Register peer
  memcpy(peerInfo.peer_addr, destMACaddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  //Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
}

void loop() {     
  while (ss.available() > 0) {
    gps.encode(ss.read());
  }
  outgoingReadings.coordx = gps.location.lat();
  outgoingReadings.coordy = gps.location.lng();
  outgoingReadings.timeHour = gps.time.hour();
  outgoingReadings.timeMin = gps.time.minute();
  outgoingReadings.timeSec = gps.time.second();
  outgoingReadings.timeCentSec = gps.time.centisecond();

  esp_err_t result = esp_now_send(destMACaddress, (uint8_t*) &outgoingReadings, sizeof(outgoingReadings));    //Send data to reciever
  cout<<"\n\r"<<outgoingReadings.coordx<<","<<outgoingReadings.coordy;
   
  delay(sampleDelay);
}
//////////////////Callback function for when data is recieved//////////////////
void OnDataRecv (const uint8_t* mac, const uint8_t* incomingData, int len) {
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  outgoingReadings.FLdis = incomingReadings.FLdis; outgoingReadings.FMdis = incomingReadings.FMdis; outgoingReadings.FRdis = incomingReadings.FRdis;
  outgoingReadings.BLdis = incomingReadings.BLdis; outgoingReadings.BMdis = incomingReadings.BMdis; outgoingReadings.BRdis = incomingReadings.BRdis;

  outgoingReadings.FLang = incomingReadings.FLang; outgoingReadings.FRang = incomingReadings.FRang;
  outgoingReadings.BLang = incomingReadings.BLang; outgoingReadings.BRang = incomingReadings.BRang;
}
