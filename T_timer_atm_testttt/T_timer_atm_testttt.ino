#include "MsTimer2.h"
#include "Seeed_BMP280.h"

BMP280 bmp280;
int nTime = 0;

#define DEF_D3    3
void OnTimer()
{
  Serial.println("/n");
  Serial.println("------------OnTimer()----------------------");
  detachInterrupt (digitalPinToInterrupt(DEF_D3));
  Serial.println("------------disable interrupt----------------------");

  float fPressure = bmp280.getPressure();
  Serial.print("bmp280 Pressure = ");
  Serial.print(fPressure);
  Serial.println(" Pa");
  Serial.println("\n");
              
  delay(200);
  attachInterrupt(digitalPinToInterrupt(DEF_D3), D3InterruptFun, RISING);
  Serial.println("------------enable interrupt----------------------");
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

     if(!bmp280.init()){
         Serial.println("Device error!");
     }
     else
        Serial.println("bmp280 init OK");
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
