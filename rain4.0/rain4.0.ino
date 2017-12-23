int rainpin = 7;
int rainCounter = 0;
int RainMeter;
int rainState;
int lastrainState;
void setup() {
rainState = digitalRead(rainpin);
Serial.begin(9600);
}
void loop() {
 lastrainState = digitalRead(rainpin);
if(rainState != lastrainState){
if(rainState == HIGH){
rainCounter++;
}
delay(20);
lastrainState = rainState;
RainMeter = rainCounter*0.2;
Serial.print("Rain volume:");
Serial.print(RainMeter);
Serial.println("mm");
}
}
