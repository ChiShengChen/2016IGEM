#include "MsTimer2.h"

void OnTimer()
{
  Serial.println("/n");
  Serial.println("------------OnTimer()----------------------");
  Serial.println("/n");
  
}
void setup() {
     Serial.begin(9600);
  // put your setup code here, to run once:
     MsTimer2::set(30 * 1000,  OnTimer); //中断设置函数, 每5分钟进入一次中断
     MsTimer2::start();
}

int nTime = 0;
void loop() {
    Serial.println("/n");
    Serial.print("Time = ");
    Serial.println(nTime++);
    Serial.println("/n");
    delay(2 * 1000);
   

}
