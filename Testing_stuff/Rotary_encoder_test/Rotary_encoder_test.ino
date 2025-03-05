//List of what potentiometer is at what component and at what pin
int MPLCAFrontRight = 36;     //MPLCA => MacPherson Lower Control Arm
int MPLCAFrontLeft = 39;

int FLpotVal = 0;           //Front left potentiometer value
int FRpotVal = 0;

void setup()
{
  Serial.begin(115200);
  delay(1000);
}

void loop()
{
  FLpotVal = analogRead(MPLCAFrontLeft);
  FRpotVal = analogRead(MPLCAFrontRight);
  Serial.print("Front Left Lower Control Arm = ");
  Serial.println(FLpotVal);
  Serial.print("Front Right Lower Control Arm = ");
  Serial.println(FRpotVal);
}
