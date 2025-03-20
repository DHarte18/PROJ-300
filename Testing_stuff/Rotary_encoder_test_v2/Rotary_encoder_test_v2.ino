////Pin definitions////
//Front Right
#define outputFRA 22
#define outputFRB 23
//Front Left
#define outputFLA 19
#define outputFLB 21
//Rear Left
#define outputBLLA 34
#define outputBLLB 35
#define outputBLWA 32
#define outputBLWB 33
//Rear Right
#define outputBRLA 25
#define outputBRLB 26
#define outputBRWA 27
#define outputBRWB 14
///////////////////////
//Variables for tracking angle//
//Front Right
int counterFR = 0; 
int aStateFR;
int aLastStateFR;
//Front Left
int counterFL = 0; 
int aStateFL;
int aLastStateFL;
//Back Left
int BLLpos = 0;   //BLL = Back Left Lower (control arm)
int BLLcurState;
int BLLlasState;
int BLWpos = 0;   //BLW = Back Left Wall
int BLWcurState;
int BLWlasState;
//Back right
int BRLpos = 0;   //BRL = Back Right Lower (control arm)
int BRLcurState;
int BRLlasState;
int BRWpos = 0;   //BRW = Back Right Wall
int BRWcurState;
int BRWlasState;
////////////////////////////////

void setup()
{
  Serial.begin(115200);
  pinMode(outputFRA, INPUT_PULLDOWN);
  pinMode(outputFRB, INPUT_PULLDOWN);

  pinMode(outputFLA, INPUT_PULLDOWN);
  pinMode(outputFLB, INPUT_PULLDOWN);

  pinMode(outputBLLA, INPUT_PULLDOWN);
  pinMode(outputBLLB, INPUT_PULLDOWN);
  pinMode(outputBLWA, INPUT_PULLDOWN);
  pinMode(outputBLWB, INPUT_PULLDOWN);

  pinMode(outputBRLA, INPUT_PULLDOWN);
  pinMode(outputBRLB, INPUT_PULLDOWN);
  pinMode(outputBRWA, INPUT_PULLDOWN);
  pinMode(outputBRWB, INPUT_PULLDOWN);

  aLastStateFR = digitalRead(outputFRB);
  aLastStateFL = digitalRead(outputFLB);
  BLLlasState = digitalRead(outputBLLB);
  BLWlasState = digitalRead(outputBLWB);
  BRLlasState = digitalRead(outputBRLB);
  BRWlasState = digitalRead(outputBRWB);
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
  
  BLLcurState = digitalRead(outputBLLA);
  if (BLLcurState != BLLlasState)
  {
    if (digitalRead(outputBLLB) != BLLcurState)
    {
      BLLpos++;
    } else 
    {
      BLLpos--;
    }
    Serial.print("Back Left Lower at ");
    Serial.println(BLLpos);
    BLLlasState = BLLcurState;
  } else
  {
    ;
  }

  BLWcurState = digitalRead(outputBLWA);
  if (BLWcurState != BLWlasState)
  {
    if (digitalRead(outputBLWB) != BLWcurState)
    {
      BLWpos++;
    } else
    {
      BLWpos--;
    }
    Serial.print("Back Left Wall at ");
    Serial.println(BLWpos);
    BLWlasState = BLWcurState;
  } else 
  {
    ;
  }

  BRLcurState = digitalRead(outputBRLA);
  if (BRLcurState != BRLlasState)
  {
    if (digitalRead(outputBRLB) != BRLcurState)
    {
      BRLpos++;
    } else 
    {
      BRLpos--;
    }
    Serial.print("Back Right Lower at ");
    Serial.println(BRLpos);
    BRLlasState = BRLcurState;
  } else
  {
    ;
  }

  BRWcurState = digitalRead(outputBRWA);
  if (BRWcurState != BRWlasState)
  {
    if (digitalRead(outputBRWB) != BRWcurState)
    {
      BRWpos++;
    } else
    {
      BRWpos--;
    }
    Serial.print("Back Right Wall at ");
    Serial.println(BRWpos);
    BRWlasState = BRWcurState;
  } else 
  {
    ;
  }
}
