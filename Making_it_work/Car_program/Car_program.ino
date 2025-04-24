////////////////////////////////CAR PROGRAM/////////////////////////////
#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_VL53L0X.h>
//#include <iostream>
using namespace std;
////////////////////////////////Wireless prep///////////////////////////
uint8_t destMACaddress[] = {0x20,0x43,0xa8,0x62,0xb8,0x54};   //MAC address of PEER ESP32
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
///////////////////////////Rotary Encoder Prep///////////////////////////
TaskHandle_t RotEncRead;
int pinArr[2][4] = {16, 4, 26, 27,      //FRApin, FLApin, BRApin, BLApin
                    17, 2, 25, 14};     //FRBpin, FLBpin, BRBpin, BLBpin
int posArr[1][4] = {0, 0, 0, 0};        //Array to hold enc position *before* it's converted to deg
int aState[1][4];                       //Encoders A channel current state
int aLastState[1][4];                   //Encoders A channel previous state
int bufferPosArr[1][4] = {0, 0, 0, 0};  //Buffer array for reading info to send wirelessly
uint8_t RotEncReady = 0;                //Value for when to start enc read loop

////////////////////////////T-o-F Sensor prep////////////////////////////
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
//////////////////////Rotary Encoder Reading Loop////////////////////////
void RotEncReadCode() {
  //Serial.print("RotEncReadLoop running on core ");
  //Serial.println(xPortGetCoreID());
    //for(;;) {
      for(int i=0; i<4; i++) {            //Loop that iterates through pin assignment array
      aState[0][i] = digitalRead(pinArr[0][i]); // and changes angle based on change
      if(aState[0][i] != aLastState[0][i]) {  //between A & B channels of encoders and
        if(digitalRead(pinArr[1][i]) != aState[0][i]) { // stores it in an array
          posArr[0][i]++;
        } else {
          posArr[0][i]--;
        }
      }aLastState[0][i] = aState[0][i];
    }
    memcpy(&bufferPosArr, posArr, sizeof(bufferPosArr));
    //}
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
  //Time of Flight sensor startup sequence
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
  //Rotary encoder start up sequence
  for(int a=0; a<2; a++) {                    //Loop that iterates through pin assignment array
    for(int b=0; b<4; b++) {                  //, sets internal pulldown to each pin and sets 
      pinMode(pinArr[a][b], INPUT_PULLDOWN);  //initial "Last State" of each encoder
      aLastState[0][b] = digitalRead(pinArr[0][b]);
    }
  }
  /*
  xTaskCreatePinnedToCore(RotEncReadCode,             //Function for task
                          "Rotary Encoder Read Loop", //Name of task
                          10000,                      //Stack size of task
                          NULL,                       //Task parameter
                          1,                          //Task priority
                          &RotEncRead,                //Handle to keep track of task
                          0);                         //Core to pin task to
  */
  //RotEncReady = 1;
}

void loop() {
  TCA9548A(2);//Back Right
  VL53L0X_RangingMeasurementData_t measure2;
  lox.rangingTest(&measure2, false);  // pass in 'true' to get debug data printout!
  if (measure2.RangeStatus != 4) {    // phase failures have incorrect data
    outBRdis = measure2.RangeMilliMeter; //Update distance measurement
  } else {
    outBRdis = 9001;                     //IT'S OVER 9000 (error number)
  } RotEncReadCode();

  TCA9548A(3);//Front Right
  VL53L0X_RangingMeasurementData_t measure3;
  lox.rangingTest(&measure3, false);
  if(measure3.RangeStatus != 4) {
    outFRdis = measure3.RangeMilliMeter;
  } else {
    outFRdis = 9001;
  } RotEncReadCode();

  TCA9548A(4);//Front Middle
  VL53L0X_RangingMeasurementData_t measure4;
  lox.rangingTest(&measure4, false);
  if(measure4.RangeStatus != 4) {
    outFMdis = measure4.RangeMilliMeter;
  } else {
    outFMdis = 9001;
  } RotEncReadCode();

  TCA9548A(5);//Front Left
  VL53L0X_RangingMeasurementData_t measure5;
  lox.rangingTest(&measure5, false);
  if(measure5.RangeStatus != 4) {
    outFLdis = measure5.RangeMilliMeter;
  } else {
    outFLdis = 9001;
  } RotEncReadCode();
  
  TCA9548A(6);//Back Middle
  VL53L0X_RangingMeasurementData_t measure6;
  lox.rangingTest(&measure6, false);
  if(measure6.RangeStatus != 4) {
    outBMdis = measure6.RangeMilliMeter;
  } else {
    outBMdis = 9001;
  } RotEncReadCode();

  TCA9548A(7);//Back Left
  VL53L0X_RangingMeasurementData_t measure7;
  lox.rangingTest(&measure7, false);
  if(measure7.RangeStatus != 4) {
    outBLdis = measure7.RangeMilliMeter;
  } else {
    outBLdis = 9001;
  } RotEncReadCode();
  //Copy sensor readings to struct message to be sent
  outgoingReadings.FLdis = outFLdis; outgoingReadings.FMdis = outFMdis; outgoingReadings.FRdis = outFRdis;
  outgoingReadings.BLdis = outBLdis; outgoingReadings.BMdis = outBMdis; outgoingReadings.BRdis = outBRdis;

  outgoingReadings.FLang = outFLang; outgoingReadings.FRang = outFRang;
  outgoingReadings.BLang = outBLang; outgoingReadings.BRang = outBRang;
  //Copy values of rotary encoder position to struct message to be sent
  outgoingReadings.FRang = bufferPosArr[0][0]; outgoingReadings.FLang = bufferPosArr[0][1];
  outgoingReadings.BRang = bufferPosArr[0][2]; outgoingReadings.BLang = bufferPosArr[0][3];
  //Send struct message
  esp_err_t result = esp_now_send(destMACaddress, (uint8_t*) &outgoingReadings, sizeof(outgoingReadings));
  /*
  if(result == ESP_OK) {
    Serial.println("Sent Successfully");
  } else {
    Serial.println("Error Sending");
  }
  */
  delay(inSampleRate);
}
