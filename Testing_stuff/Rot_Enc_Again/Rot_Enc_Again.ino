#include <iostream>
using namespace std;
//Initialise arrays to store pins, angle/15, the current state of the encoders, the last state
// of the encoders and the buffer for angle read
int pinArr[2][4] = {16, 4, 26, 27,     //FRApin, FLApin, BRApin, BLApin,     Output A
                    17, 2, 25, 14};    //FRBpin, FLBpin, BRBpin, BLBpin      Output B
int angArr[1][4] = {0, 0, 0, 0};
int aState[1][4];
int aLastState[1][4];
int bufferAngArr[1][4];

void setup() {
  Serial.begin(115200);
  for(int a=0; a<2; a++) {            //Loop that iterates through pin assignment array
    for(int b=0; b<4; b++) {          //, sets internal pulldown to each pin and sets 
      pinMode(pinArr[a][b], INPUT_PULLDOWN);  //initial "Last State" of each encoder
      aLastState[0][b] = digitalRead(pinArr[0][b]);
    }
  }
}

void loop() {
  for(int i=0; i<4; i++) {            //Loop that iterates through pin assignment array
    aState[0][i] = digitalRead(pinArr[0][i]); // and changes angle based on change
    if(aState[0][i] != aLastState[0][i]) {  //between A & B channels of encoders and
      if(digitalRead(pinArr[1][i]) != aState[0][i]) { // stores it in an array
        angArr[0][i]++;
      } else {
        angArr[0][i]--;
      }
    }aLastState[0][i] = aState[0][i];
  }
  memcpy(&bufferAngArr, angArr, sizeof(bufferAngArr));
  for(int j=0; j<4; j++) {
    cout<<angArr[0][j]<<", ";
  } cout<<endl;
}                                                       
