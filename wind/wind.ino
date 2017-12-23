int windpin = A1;
void setup() {
  // put your setup code here, to run once:
pinMode( windpin , INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
float windV = analogRead( windpin );
float wind = ((windV-0.4)/1.6*32.4);
if( wind>0)
{
  Serial.print("Sensor Output:");
  Serial.println(wind); 

}
else
  {
    Serial.println("Sensor Error");   
  }
    delay(200);   
}
