int nD3Pin = 7;
volatile int nInterCount = 0;
bool bHToL = true;
int nPinVal = LOW;
void setup() {
   Serial.begin(9600);
   pinMode(nD3Pin, INPUT_PULLUP);
   attachInterrupt(digitalPinToInterrupt(nD3Pin), blink, RISING); 
}
void blink()
{
  Serial.println("------------------------------------------");
  nInterCount++;
  Serial.print("interrput count =");
  Serial.println(nInterCount);
  Serial.println("------------------------------------------");
  delay(2000);
}
void loop() {
    nPinVal = digitalRead(nD3Pin);
 
   if(nPinVal == HIGH)
      Serial.println("D7 GPIO Status = HIGH");
    else
      Serial.println("D7 GPIO Status = LOW"); 
   delay(1000);
}
