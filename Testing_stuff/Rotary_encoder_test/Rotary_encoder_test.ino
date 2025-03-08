//List of what potentiometer is at what component and at what pin
int MPLCAFrontRight = 36;     //MPLCA => MacPherson Lower Control Arm

           //Front left potentiometer value
int FRpotVal = 0;

void setup()
{
  Serial.begin(115200);
  delay(1000);
}

void loop()
{
  delay(1000);
  FRpotVal = analogRead(MPLCAFrontRight);
  Serial.println(FRpotVal);
}
