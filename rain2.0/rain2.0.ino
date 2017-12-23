
const int REED = 7;              //The reed switch outputs to digital pin 9

int val = 0;                    //Current value of reed switch

int old_val = 0;                //Old value of reed switch

int REEDCOUNT = 0;              //This is the variable that hold the count of switching



void setup(){

 Serial.begin(9600);

 pinMode (REED, INPUT_PULLUP); //This activates the internal pull up resistor

}



void loop(){

 val = digitalRead(REED);      //Read the status of the Reed swtich



 if ((val == LOW) && (old_val == HIGH)){    //Check to see if the status has changed

   delay(10);                   // Delay put in to deal with any "bouncing" in the switch.

   REEDCOUNT = REEDCOUNT + 1;   //Add 1 to the count of bucket tips

   old_val = val;              //Make the old value equal to the current value

   Serial.print("Count = ");

   Serial.println(REEDCOUNT);  //Output the count to the serial monitor

 }



 else {

   old_val = val;              //If the status hasn't changed then do nothing

 }

}
