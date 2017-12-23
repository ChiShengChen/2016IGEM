void setup() {
  Serial.begin(9600);
}
void loop() {
  int sensorValue = analogRead(A1);
 // Serial.println(sensorValue);
  float value = sensorValue * (5.0 / 1024.0);
//  Serial.println(value);
  float fanspeed = value - 0.4 ;
 // Serial.print( fanspeed );
  fanspeed = fanspeed * 1.6*32.4 ;
  Serial.print("fanspeed is ");
  Serial.print( fanspeed );
  Serial.println( "m/s" );
 delay(500);

}
