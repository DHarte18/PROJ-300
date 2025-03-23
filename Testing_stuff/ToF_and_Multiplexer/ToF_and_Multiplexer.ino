//2 T-o-Fs connected to multiplexer on 0 and 1 channels
//Multiplexer SDA to pin 21, SCL to pin 22, reset to pin 19, A0 to GND, A1 to GND, A2 to GND hence address of 0x70
//All being powered at 5V

#include "Adafruit_VL53L0X.h"

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

void setup() 
{
  Serial.begin(115200);
  while(! Serial)
  {
    delay(1);
  }
  Serial.println("Multiplexer and T-o-F test");
  TCA9548A(0);
  if(!lox.begin())
  {
    Serial.println(F("Failed to boot channel 0 T-o-F sensor"));
    while(1);
  } else {;}
  TCA9548A(1);
  if(!lox.begin())
  {
    Serial.println(F("Failed to boot channel 1 T-o-F sensor"));
    while(1);
  } else {;}
}

void loop() 
{
  TCA9548A(0);
  VL53L0X_RangingMeasurementData_t measure0;
  //Serial.print("Reading a measurement... ");
  lox.rangingTest(&measure0, false); // pass in 'true' to get debug data printout!

  if (measure0.RangeStatus != 4) {  // phase failures have incorrect data
    Serial.print("Ch. 0 Distance (mm): "); Serial.println(measure0.RangeMilliMeter);
  } else {
    Serial.println("Ch. 0 out of range");
  }
    
  delay(100);

  TCA9548A(1);
  VL53L0X_RangingMeasurementData_t measure1;
  lox.rangingTest(&measure1, false);

  if(measure1.RangeStatus != 4) {
    Serial.print("Ch.1 Distance (mm): "); Serial.println(measure1.RangeMilliMeter);
  } else {
    Serial.println("Ch.1 out of range");
  }
}

void TCA9548A (uint8_t bus)
{
  Wire.beginTransmission(0x70);
  Wire.write(1<<bus);
  Wire.endTransmission();
}