#define rainpin 3
int RR = 0;
float rainmeter = 0;

void setup() {

  pinMode( rainpin , INPUT_PULLUP);
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(rainpin), blink, RISING);
  
}
void blink()
{
  rainmeter += 0.2;
}

void loop() {
  RR=digitalRead(rainpin);
  delay(500);
   Serial.print("RR:");
   Serial.println(RR);
  Serial.print(rainmeter);
  Serial.println("mm");
}
