
#define outputFRA 22
#define outputFRB 23
#define outputFLA 19
#define outputFLB 21

int counterFR = 0; 
int aStateFR;
int aLastStateFR;
int counterFL = 0; 
int aStateFL;
int aLastStateFL;

void setup()
{
  pinMode(outputFRA, INPUT_PULLDOWN);
  pinMode(outputFRB, INPUT_PULLDOWN);

  pinMode(outputFLA, INPUT_PULLDOWN);
  pinMode(outputFLB, INPUT_PULLDOWN); 

  Serial.begin(115200);
  aLastStateFR = digitalRead(outputFRB);
  aLastStateFL = digitalRead(outputFLB);
}

void loop()
{
  aStateFR = digitalRead(outputFRA);
  if(aStateFR != aLastStateFR)
  {
    if(digitalRead(outputFRB) != aStateFR)
    {
      counterFR++;
    } else
    {
      counterFR--;
    }
    Serial.print("Position FR ");
    Serial.println(counterFR);
    aLastStateFR = aStateFR;
  } else
  {
    ;
  }

  aStateFL = digitalRead(outputFLA);
  if(aStateFL != aLastStateFL)
  {
    if(digitalRead(outputFLB) != aStateFL)
    {
      counterFL++;
    } else
    {
      counterFL--;
    }
    Serial.print("Position FL ");
    Serial.println(counterFL);
    aLastStateFL = aStateFL;
  } else 
  {
    ;
  }
  
}
