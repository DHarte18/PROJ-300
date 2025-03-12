#include "ScaleCarHeader.ino"

int FLangle;
int aLastState;

void setup()
{
  pinMode(19, INPUT_PULLDOWN);
  pinMode(21, INPUT_PULLDOWN);
  Serial.begin(115200);
  aLastState = digitalRead(21);
}

void loop()
{
  FLangle = rotaryEncoderTracker(19,21);
  Serial.print("Angle at ");
  Serial.println(FLangle);
}