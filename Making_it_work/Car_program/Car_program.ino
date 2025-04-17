////////////////////////////////CAR PROGRAM/////////////////////////////
#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_VL53L0X.h>
//#include <iostream>
using namespace std;
////////////////////////////////Wireless prep///////////////////////////
uint8_t destMACaddress[] = {0x20,0x43,0xa8,0x64,0x16,0x20};   //MAC address of PEER ESP32
uint8_t inSampleRate = 0;                                           //Variable to store desired sample rate from receiver
uint8_t inReady = 0;
uint8_t delays = 25;                                          //Variable to store time delay between T-o-F readings

typedef struct struct_message1 {                              //Struct to hold outgoing message
  int FLdis; int FMdis; int FRdis;
  int BLdis; int BMdis; int BRdis;

  int FLang; int FRang;
  int BLang; int BRang;
} struct_message1;
struct_message1 outgoingReadings;

typedef struct struct_message2 {                              //Struct to hold incoming ifo
  int sampleRate;
  int ready;
} struct_message2;
struct_message2 incomingData;

String success;                                               //Variable to store if data was sent successfully

esp_now_peer_info_t peerInfo;                                 //Variabel to store info about PEER
///////////////////////////////T-o-F Sensor prep/////////////////////////
Adafruit_VL53L0X lox = Adafruit_VL53L0X();
int outFLdis = 0; int outFMdis = 0; int outFRdis = 0;
int outBLdis = 0; int outBMdis = 0; int outBRdis = 0;

///////////////////////////Rotary Encoder Prep///////////////////////////
int outFLang = 0; int outFRang = 0;
int outBLang = 0; int outBRang = 0;

//////////////////Callback function for when data is sent////////////////
void OnDataSent (const uint8_t* mac_addr, esp_now_send_status_t status) {
  //Serial.print("\r\nLast Packet Send Status: \t");
  //Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if(status == 0) {
    success = "Delivery Success :)";
  } else {
    success = "Delivery Failure :(";
  }
}

/////////////Callback function for when data is recieved/////////////////
void OnDataRecv (const uint8_t* mac, const uint8_t* incomingReadings, int len) {
  memcpy(&incomingData, incomingReadings, sizeof(incomingData));
  inSampleRate = incomingData.sampleRate;
  inReady = incomingData.ready;
  //Serial.print(inSampleRate);
  //Serial.println(inReady);
}

/////////////Function for channel assignment on multiplexer//////////////
void TCA9548A (uint8_t bus) {
  Wire.beginTransmission(0x70);                 //Multiplexer address, A0 = GND, A1 = GND, A2 = GND
  Wire.write(1<<bus);
  Wire.endTransmission();
}

////////////////////////////Standard Loops///////////////////////////////  
void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  if(esp_now_init() != ESP_OK) {
    //Serial.println("Error initialising ESP_NOW");
    return;
  }
  memcpy(peerInfo.peer_addr, destMACaddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  if(esp_now_add_peer(&peerInfo) != ESP_OK) {
    //Serial.println("Failed to add peer");
    return;
  }
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));

  Wire.begin();
  while(! Serial) {delay(1);}
  int i = 2;
  while(i<8) {
    TCA9548A(i);
    if(!lox.begin()) {
      //Serial.println(F("Failed to boot channel ")); Serial.println(i);
      while(1);
    } else {;}
    i++;
  }
}

void loop() {
  TCA9548A(2);//Back Right
  VL53L0X_RangingMeasurementData_t measure2;
  lox.rangingTest(&measure2, false);  // pass in 'true' to get debug data printout!
  if (measure2.RangeStatus != 4) {    // phase failures have incorrect data
    outBRdis = measure2.RangeMilliMeter; //Update distance measurement
  } else {
    outBRdis = 9001;                     //IT'S OVER 9000 (error number)
  } delay(delays);

  TCA9548A(3);//Front Right
  VL53L0X_RangingMeasurementData_t measure3;
  lox.rangingTest(&measure3, false);
  if(measure3.RangeStatus != 4) {
    outFRdis = measure3.RangeMilliMeter;
  } else {
    outFRdis = 9001;
  } delay(delays);

  TCA9548A(4);//Front Middle
  VL53L0X_RangingMeasurementData_t measure4;
  lox.rangingTest(&measure4, false);
  if(measure4.RangeStatus != 4) {
    outFMdis = measure4.RangeMilliMeter;
  } else {
    outFMdis = 9001;
  } delay(delays);

  TCA9548A(5);//Front Left
  VL53L0X_RangingMeasurementData_t measure5;
  lox.rangingTest(&measure5, false);
  if(measure5.RangeStatus != 4) {
    outFLdis = measure5.RangeMilliMeter;
  } else {
    outFLdis = 9001;
  } delay(delays);
  
  TCA9548A(6);//Back Middle
  VL53L0X_RangingMeasurementData_t measure6;
  lox.rangingTest(&measure6, false);
  if(measure6.RangeStatus != 4) {
    outBMdis = measure6.RangeMilliMeter;
  } else {
    outBMdis = 9001;
  } delay(delays);

  TCA9548A(7);//Back Left
  VL53L0X_RangingMeasurementData_t measure7;
  lox.rangingTest(&measure7, false);
  if(measure7.RangeStatus != 4) {
    outBLdis = measure7.RangeMilliMeter;
  } else {
    outBLdis = 9001;
  } delay(delays);
  //Copy sensor readings to struct message to be sent
  outgoingReadings.FLdis = outFLdis; outgoingReadings.FMdis = outFMdis; outgoingReadings.FRdis = outFRdis;
  outgoingReadings.BLdis = outBLdis; outgoingReadings.BMdis = outBMdis; outgoingReadings.BRdis = outBRdis;

  outgoingReadings.FLang = outFLang; outgoingReadings.FRang = outFRang;
  outgoingReadings.BLang = outBLang; outgoingReadings.BRang = outBRang;
  //Send struct message
  esp_err_t result = esp_now_send(destMACaddress, (uint8_t*) &outgoingReadings, sizeof(outgoingReadings));
  if(result == ESP_OK) {
    //Serial.println("Sent Successfully");
  } else {
    //Serial.println("Error Sending");
  }
  delay(inSampleRate);
}
