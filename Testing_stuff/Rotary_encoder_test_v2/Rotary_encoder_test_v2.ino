#define outputA 36
#define outputB 39

int counter = 0; 
int aState;
int aLastState;  

void setup()
{
  pinMode(outputA, INPUT_PULLUP);
  pinMode(outputB, INPUT_PULLUP);

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
