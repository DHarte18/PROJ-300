#ifndef ScaleCarHeader.h
#define ScaleCarHeader

int counter = 0; 
int aState;
int aLastState;
int angle;

aLastState = digitalRead(outputBpin);

int rotaryEncoderTracker (int outputApin, int outputBpin, aLastState)
{
  aState = digitalRead(outputApin);
  if(aState != aLastState)
  {
    if(digitalRead(outputBpin) != aState)
    {
      counter++;
    } else
    {
      counter--;
    }
  }
  aLastState = aState;
  angle = counter * 15;
  return angle;
  return aLastState;
}

#endif ScaleCarHeader.h