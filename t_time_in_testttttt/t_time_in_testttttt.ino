#include "MsTimer2.h"
int nTime = 0;

#define DEF_D3    3
void OnTimer()
{
  Serial.println("/n");
  Serial.println("------------OnTimer()----------------------");
  Serial.println("/n");
  nTime = 0;
  
}

void D3InterruptFun()
{
   Serial.println("\n");
   Serial.println("----------------D3InterruptFun-------------------------");
   Serial.println("\n");
}

void setup() {
     Serial.begin(9600);
  // put your setup code here, to run once:
     MsTimer2::set(30 * 1000,  OnTimer); 
     MsTimer2::start();

     pinMode(DEF_D3, INPUT_PULLUP);
     attachInterrupt(digitalPinToInterrupt(DEF_D3), D3InterruptFun, RISING);
}


void loop() {
    Serial.println("/n");
    Serial.print("Time = ");
    Serial.println(nTime++);
    Serial.println("/n");
    delay(2 * 1000);
}
