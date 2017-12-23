int rainpin = 3;
int rainstate = 0;

void setup() {

  //pinMode( rainpin , INPUT);
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(rainpin), blink, LOW);
  
}
void blink()
{
  rainstate++;
  Serial.print("rainstate");
  Serial.println(rainstate);
}

void loop() {
  int RR=digitalRead(rainpin);
  if (rainstate != 0)
  { delay(500);
    rainstate = 0;
  }
  else
  {
    delay(500);
  }

  float  rainmeter = rainstate * 0.2;
   Serial.print(RR);
  Serial.print(rainmeter);
  Serial.println("mm");
}
