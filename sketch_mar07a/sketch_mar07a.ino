int nD7Pin = 7;
volatile int nInterCount = 0;
bool bHToL = true;
int nPinVal = LOW;
void setup() {
   Serial.begin(9600);
   pinMode(nD7Pin, INPUT);
   attachInterrupt(0, blink, RISING); 
}
void blink()
{
  Serial.println("------------------------------------------");
  nInterCount++;
  Serial.print("interrput count =");
  Serial.println(nInterCount);
  Serial.print("D7 GPIO Status =");
  Serial.println(nPinVal);
  Serial.println("------------------------------------------");
}
void loop() {
   nPinVal = digitalRead(nD7Pin);
}
