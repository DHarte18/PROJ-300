#define outputA 19
#define outputB 21

int counter = 0; 
int aState;
int aLastState;  

void setup()
{
  pinMode(outputA, INPUT_PULLDOWN);
  pinMode(outputB, INPUT_PULLDOWN);

  Serial.begin(115200);
  aLastState = digitalRead(outputB);
}

void loop()
{
  aState = digitalRead(outputA);
  if(aState != aLastState)
  {
    if(digitalRead(outputB) != aState)
    {
      counter++;
    } else
    {
      counter--;
    }
    Serial.print("Position");
    Serial.println(counter);
  }
  aLastState = aState;
}
