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
uint8_t sampleRate = 0;                                       //Variable for sample rate to be sent in milliseconds

typedef struct struct_message {                             //Struct to hold incoming readings
  uint8_t ToFArr[1][6];
  uint8_t RotEncArr[1][4];
} struct_message;
struct_message incomingReadings;

String success;                                               //Variable to store if sending data was successful

esp_now_peer_info_t peerInfo;                                 //Variable to store info about the PEER

/////////////////////Callback function for when data is sent///////////////////
void OnDataSent (const uint8_t* mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status: \t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivey Success" : "Delivery Fail");
  if (status == 0) {
    success = "Delivery Success :)";
  } else {
    success = "Delivery Failure :(";
  }
}

//////////////////Callback function for when data is recieved//////////////////
void OnDataRecv (const uint8_t* mac, const uint8_t* incomingData, int len) {
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  //incomingToFArr = incomingReadings.ToFArr;
  //incomingRotEncArr = incomingReadings.RotEncArr;
  int i, j;
  for (i=0; i<1; i++) {
    for (j=0; j<6; j++) {
      cout<<"\t"<<incomingReadings.ToFArr[i][j];
    } cout<<endl;
  }
  int g, h;
  for (g=0; g<1; g++) {
    for (h=0; h<1; h++) {
      cout<<"\t"<<incomingReadings.RotEncArr[g][h];
    } cout<<endl;
  }
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
  //Register callback function to be called when data recieved
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
}

void loop() {
  sampleRate = 100;
  //Send message w/ ESP_NOW
  esp_err_t result = esp_now_send(destMACaddress, (uint8_t*) &sampleRate, sizeof(sampleRate));
  if (result == ESP_OK) {
    Serial.println("Sent successfully");
  } else {
    Serial.println("Error Sending");
  } delay(10000);
}
