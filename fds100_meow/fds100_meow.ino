int mudhumpin = A0;
void setup() {
  // put your setup code here, to run once:
pinMode(mudhumpin,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
float mudV = analogRead( mudhumpin );
float mudhum = (mudV/3.3*100);
if( mudhum>0)
{
  Serial.print("Sensor Output:");
  Serial.print(mudhum); 
 // Serial.println(" %");
}
else
  {
    Serial.println("Sensor Error");   
  }
    delay(200);   
}
