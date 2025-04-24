#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <iostream>
#include <iomanip>    //Literally just for printing w/ more than 1 decimal place
using namespace std;

Adafruit_MPU6050 mpu;
uint8_t delays = 500;
sensors_event_t a, g, temp;
int errorArr [6][3];
int adjReadArr [6][3];
int avgArr [6][1];
int xDisp;
int yDisp;
int zDisp;

void setup() {
  Serial.begin(115200);
  while (!Serial) {delay(1);}    //Wait for serial to begin before continuing setup
  //Loop to check if all MPUs connected to multiplexer functional
  for (int i=0; i<3; i++) {
    TCA9548A(i);
    if (!mpu.begin()) {
      cout<<"\n\r"<<"Failed initialise MPU on channel"<<i;
      delay(10);
    } else {
      mpu.setAccelerometerRange(MPU6050_RANGE_8_G);       //Range accelerations measurable before disregarded value; Options: 2, 4, 6, 8G
      mpu.setGyroRange(MPU6050_RANGE_500_DEG);            //Range of values measurable before overload occurs; Options: 250, 500, 1000, 2000deg/s
      mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);         //Range of measured frequencies, set to 21Hz for initial testing; Options: 5, 10, 21, 44, 94, 184, 260Hz    
      delay(100);                                         //Scale analogy - A penny weighed on a scale of range 0 - 10g will see a greater % change
      cout<<"\n\r"<<"MPU on channel "<<i<<" initialised"; // in the weight measured vs a scale of range 0 - 10kg
      cout<<endl;
      //Get error in MPU readings and store it
      mpu.getEvent(&a, &g, &temp);
      errorArr[0][i] = a.acceleration.x;
      errorArr[1][i] = a.acceleration.y;
      errorArr[2][i] = a.acceleration.z;
      errorArr[3][i] = g.gyro.x;
      errorArr[4][i] = g.gyro.y;
      errorArr[5][i] = g.gyro.z;
      delay(50);
    }
  }
  /*      //For displaying error on MPUs
  cout<<setprecision(6);
  for (int k=0; k<6; k++) {
    for (int l=0; l<3; l++) {
      cout<<errorArr[k][l];
      delay(10);
    }
    cout<<"\n\r";
  }
  */
}
void loop() {
  for (int j=0; j<3; j++) {
    TCA9548A(j);
    //cout<<"Channel "<<j;
    mpu.getEvent(&a, &g, &temp);
    adjReadArr[0][j] = a.acceleration.x - errorArr[0][j];
    adjReadArr[1][j] = a.acceleration.y - errorArr[1][j];
    adjReadArr[2][j] = a.acceleration.z - errorArr[2][j];
    adjReadArr[3][j] = g.gyro.x - errorArr[3][j];
    adjReadArr[4][j] = g.gyro.y - errorArr[4][j];
    adjReadArr[5][j] = g.gyro.z - errorArr[5][j];
/*  For displaying adjusted accel and rot
    cout<<"\n\r"<<"Acceleration X: "<<adjReadArr[0][j]<<"ms^-2 Y: "<<adjReadArr[1][j]<<"ms^-2 Z: "<<adjReadArr[2][j]<<"ms^-2";
    cout<<"\n\r"<<"Rotation X: "<<adjReadArr[3][j]<<"rads^-1 Y: "<<adjReadArr[3][j]<<"rads^-1 Z: "<<adjReadArr[5][j]<<"rads^-1";
    cout<<"\n\r"<<"Temperature: "<<temp.temperature<<" deg C";
    cout<<endl;
*/
    delay(delays);
  }
  for (int m=0; m<6; m++) {
    avgArr[m][0] = (adjReadArr[m][0] + adjReadArr[m][1]) / 2;
  }
}
/////Get error adj'd & aver'd accel and rot/////
void accelNGyroGet() {
  for (int j=0; j<3; j++) {
    TCA9548A(j);
    mpu.getEvent(&a, &g, &temp);
    adjReadArr[0][j] = a.acceleration.x - errorArr[0][j];
    adjReadArr[1][j] = a.acceleration.y - errorArr[1][j];
    adjReadArr[2][j] = a.acceleration.z - errorArr[2][j];
    adjReadArr[3][j] = g.gyro.x - errorArr[3][j];
    adjReadArr[4][j] = g.gyro.y - errorArr[4][j];
    adjReadArr[5][j] = g.gyro.z - errorArr[5][j];
  }
  for (int m=0; m<6; m++) {
    avgArr[m][0] = (adjReadArr[m][0] + adjReadArr[m][1]) / 2;
  }
}
///////////////Multiplexer Setup////////////////
void TCA9548A (uint8_t bus) {
  Wire.beginTransmission(0x70);
  Wire.write(1<<bus);
  Wire.endTransmission();
}