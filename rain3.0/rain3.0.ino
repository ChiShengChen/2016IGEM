int rainpin = 7;
int rainCounter = 0;
int rainState = digitalRead(rainpin);
int RainMeter = 0;
void setup() {
  
    Serial.begin(9600);
}

void loop() {
int lastrainState = digitalRead(rainpin);
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
