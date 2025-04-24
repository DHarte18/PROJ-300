///////////////////////////////RECIEVER PROGRAM////////////////////////////
#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h>
#include <Arduino.h>
#include <FS.h>
#include <LittleFS.h>
#include <iostream>
using namespace std;

uint8_t destMACaddress[] = {0xf0,0x24,0xf9,0x7a,0x39,0x14};   //MAC address of peer ESP32 NOT the ESP32 this code is running on
uint8_t incomingToFArr[1][6];                                 //Array to store recieved T-o-F data
uint8_t incomingRotEncArr[1][4];                              //Array to store recieved rotary encoder data
uint8_t outSampleRate = 1000;                                           //Variable for sample rate to be sent in milliseconds
uint8_t outReady = 0;
unsigned long tStamp = 0;
unsigned long lapStart = 0;
int lapCounter = 2;

int FLdis = 0; int FMdis = 0; int FRdis = 0;
int BLdis = 0; int BMdis = 0; int BRdis = 0;
int FLang = 0; int FRang = 0;
int BLang = 0; int BRang = 0;
double coordx = 0; double coordy;
unsigned long startTime;                //First time, in ms, a reading is taken
unsigned long timeHour;                     //Tracker for time
unsigned long timeMin;
unsigned long timeSec;
unsigned long timeCentSec;

typedef struct struct_message1 {                               //Struct to hold incoming readings
  int FLdis; int FMdis; int FRdis;
  int BLdis; int BMdis; int BRdis;

  int FLang; int FRang;
  int BLang; int BRang;

  double coordx; double coordy;
  int timeHour; int timeMin; int timeSec; int timeCentSec;
} struct_message1;
struct_message1 incomingReadings;

typedef struct struct_message2 {
  int sampleRate;
  int ready;
} struct_message2;
struct_message2 outgoingData;

String success;                                               //Variable to store if sending data was successful

esp_now_peer_info_t peerInfo;                                 //Variable to store info about the PEER

/////////////////////Callback function for when data is sent///////////////////
void OnDataSent (const uint8_t* mac_addr, esp_now_send_status_t status) {
  /*
  Serial.print("\r\nLast Packet Send Status: \t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivey Success" : "Delivery Fail");
  if (status == 0) {
    success = "Delivery Success :)";
  } else {
    success = "Delivery Failure :(";
  }
  */
}

//////////////////Callback function for when data is recieved//////////////////
void OnDataRecv (const uint8_t* mac, const uint8_t* incomingData, int len) {
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  FLdis = incomingReadings.FLdis; FMdis = incomingReadings.FMdis; FRdis = incomingReadings.FRdis;
  BLdis = incomingReadings.BLdis; BMdis = incomingReadings.BMdis; BRdis = incomingReadings.BRdis;

  FLang = incomingReadings.FLang; FRang = incomingReadings.FRang;
  BLang = incomingReadings.BLang; BRang = incomingReadings.BRang;

  coordx = incomingReadings.coordx; coordy = incomingReadings.coordy;
  timeHour = incomingReadings.timeHour; timeMin = incomingReadings.timeMin; timeSec = incomingReadings.timeSec; timeCentSec = incomingReadings.timeCentSec;

  tStamp = millis() - lapStart;

  cout<<lapCounter<<","<<tStamp<<","<<FLdis<<","<<FMdis<<","<<FRdis<<","<<BLdis<<","<<BMdis<<","<<BRdis<<","<<FLang<<","<<FRang<<","<<BLang<<","<<BRang<<","<<coordx<<","<<coordy<<","<<timeHour<<":"<<timeMin<<":"<<timeSec<<"."<<timeCentSec<<"\n";
  /*
  cout<<"\n\r"<<FLdis<<" "<<FMdis<<" "<<FRdis;
  cout<<"\n\r"<<BLdis<<" "<<BMdis<<" "<<BRdis;
  cout<<"\n\r"<<" ";
  cout<<"\n\r"<<FLang<<" "<<FRang;
  cout<<"\n\r"<<BLang<<" "<<BRang;
  cout<<endl;
  */
}

/////////////////////////////////Normal Loops//////////////////////////////////
void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  //Init ESP_NOW
  if(esp_now_init() != ESP_OK) {
    Serial.println("Error initialising ESP_NOW");
    return;
  }
  //After ESP_NOW start, send CB to get transmitted packet status
  esp_now_register_send_cb(OnDataSent);
  //Register peer
  memcpy(peerInfo.peer_addr, destMACaddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  //Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
  lapStart = millis();
  //Register callback function to be called when data recieved
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
}

void loop() {
  outgoingData.sampleRate = outSampleRate;
  outgoingData.ready = outReady;
  //Send message w/ ESP_NOW
  esp_err_t result = esp_now_send(destMACaddress, (uint8_t*) &outgoingData, sizeof(outgoingData));
  
  if (result == ESP_OK) {
    //Serial.println("Sent successfully");
  } else {
    Serial.println("Error Sending");
  }
  
  delay(outSampleRate);
}
