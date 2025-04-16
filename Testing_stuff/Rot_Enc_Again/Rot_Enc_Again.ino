using namespace std;
//Initialise arrays to store pins, angle/15, the current state of the encoders, the last state
// of the encoders and the buffer for angle read
int pinArr[2][4] = {19, 5, 16, 2,     //FRApin, FLApin, BRApin, BLApin,     Output A
                    18, 17, 4, 15};   //FRBpin, FLBpin, BRBpin, BLBpin      Output B
int angArr[1][4] = {0, 0, 0, 0};
int aState[1][4];
int aLastState[1][4];
int bufferAngArr[1][4];

void setup() {
  Serial.begin(115200);
  for(int a=0; a<2; a++) {            //Loop that iterates through pin assignment array
    for(int b=0; b<4; b++) {          //, sets internal pulldown to each pin and sets 
      pinMode(pinArr[a][b], INPUT_PULLDOWN);  //initial "Last State" of each encoder
      aLastState[1][b] = digitalRead(pinArr[2][b]);
    }
  }
}

void loop() {
  for(int i=0; i<4; i++) {            //Loop that iterates through pin assignment array
    aState[1][i] = digitalRead(pinArr[1][i]); // and changes angle based on change
    if(aState[1][i] != aLastState[1][i]) {  //between A & B channels of encoders and
      if(digitalRead(pinArr[2][i]) != aState[1][i]) { // stores it in an array
        angArr[1][i]++;
      } else {
        angArr[1][i]--;
      }
    } else {
      ;
    }
  }
  memcpy(&bufferAngArr, angArr, sizeof(bufferAngArr));  //copy contents of angle array to
}                                                       // a buffer for thread safety
