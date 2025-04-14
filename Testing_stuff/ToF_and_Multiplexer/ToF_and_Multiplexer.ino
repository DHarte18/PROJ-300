//2 T-o-Fs connected to multiplexer on 0 and 1 channels
//Multiplexer SDA to pin 21, SCL to pin 22, reset to pin 19, A0 to GND, A1 to GND, A2 to GND hence address of 0x70
//All being powered at 5V

#include "Adafruit_VL53L0X.h"
#include "iostream"
using namespace std;

Adafruit_VL53L0X lox = Adafruit_VL53L0X();
int FLdis = 0; int FMdis = 0; int FRdis = 0;
int BLdis = 0; int BMdis = 0; int BRdis = 0;

int delays = 25;

void setup() 
{
  Serial.begin(115200);
  Wire.begin();
  while(! Serial) { delay(1); }                 //Delay funcion until serial is ready
  Serial.println("Multiplexer and T-o-F test");

  TCA9548A(2);
  if(!lox.begin()) {
    Serial.println(F("Failed to boot channel BR T-o-F sensor"));
    while(1);
  } else {;}
  
  TCA9548A(3);
  if(!lox.begin()) {
    Serial.println(F("Failed to boot channel FR T-o-F sensor"));
    while(1);
  } else {;}
  
  TCA9548A(4);
  if(!lox.begin()) {
    Serial.println(F("Failed to boot FM"));
    while(1);
  } else {;}

  TCA9548A(5);
  if(!lox.begin()) {
    Serial.println(F("Failed to boot FL"));
    while(1);
  } else {;}
 
  TCA9548A(6);
  if(!lox.begin()) {
    Serial.println(F("Failed to boot BM"));
    while(1);
  } else {;}

  TCA9548A(7);
  if(!lox.begin()) {
    Serial.println(F("Failed to boot BL"));
    while(1);
  } else {;}
}

void loop() 
{
  
  TCA9548A(2);//Back Right
  VL53L0X_RangingMeasurementData_t measure2;
  lox.rangingTest(&measure2, false);  // pass in 'true' to get debug data printout!
  if (measure2.RangeStatus != 4) {    // phase failures have incorrect data
    BRdis = measure2.RangeMilliMeter; //Update distance measurement
  } else {
    BRdis = 9001;                     //IT'S OVER 9000 (error number)
  } delay(delays);

  TCA9548A(3);//Front Right
  VL53L0X_RangingMeasurementData_t measure3;
  lox.rangingTest(&measure3, false);
  if(measure3.RangeStatus != 4) {
    FRdis = measure3.RangeMilliMeter;
  } else {
    FRdis = 9001;
  } delay(delays);

  TCA9548A(4);//Front Middle
  VL53L0X_RangingMeasurementData_t measure4;
  lox.rangingTest(&measure4, false);
  if(measure4.RangeStatus != 4) {
    FMdis = measure4.RangeMilliMeter;
  } else {
    FMdis = 9001;
  } delay(delays);

  TCA9548A(5);//Front Left
  VL53L0X_RangingMeasurementData_t measure5;
  lox.rangingTest(&measure5, false);
  if(measure5.RangeStatus != 4) {
    FLdis = measure5.RangeMilliMeter;
  } else {
    FLdis = 9001;
  } delay(delays);
  
  TCA9548A(6);//Back Middle
  VL53L0X_RangingMeasurementData_t measure6;
  lox.rangingTest(&measure6, false);
  if(measure6.RangeStatus != 4) {
    BMdis = measure6.RangeMilliMeter;
  } else {
    BMdis = 9001;
  } delay(delays);

  TCA9548A(7);//Back Left
  VL53L0X_RangingMeasurementData_t measure7;
  lox.rangingTest(&measure7, false);
  if(measure7.RangeStatus != 4) {
    BLdis = measure7.RangeMilliMeter;
  } else {
    BLdis = 9001;
  } delay(25);

  int ToFarr[1][6] = {FLdis,FMdis,FRdis,BLdis,BMdis,BRdis};
  int i,j;
  cout<<"=====Distance Array=====";
  for(i=0;i<1;i++)
  {
    for(j=0;j<6;j++)
    {
      cout<<"\t"<<ToFarr[i][j];
    }
    cout<<endl;
  }
}

void TCA9548A (uint8_t bus)
{
  Wire.beginTransmission(0x70);
  Wire.write(1<<bus);
  Wire.endTransmission();
}
