#include "MsTimer2.h"
#include <Wire.h>
#include "Adafruit_SHT31.h"

Adafruit_SHT31 sht31 = Adafruit_SHT31();


int nTime = 0;

#define DEF_D3    3
void OnTimer()
{
  Serial.println("/n");
  Serial.println("------------OnTimer()----------------------");
  detachInterrupt (digitalPinToInterrupt(DEF_D3));
  Serial.println("------------disable interrupt----------------------");

    Serial.println("/n");
      Serial.println("------------Start Get temp & Hum-----------------");
   float t = sht31.readTemperature();
   

      if (!isnan(t)) {  // check if 'is not a number'
        Serial.print("Temp *C = "); 
        Serial.println(t);
      } else { 
        Serial.println("Failed to read temperature");
      }
     float h = sht31.readHumidity();
        if (! isnan(h)) {  // check if 'is not a number'
          Serial.print("Hum. % = "); Serial.println(h);
        } else { 
          Serial.println("Failed to read humidity");
        }
 
  Serial.println("------------end of  Get temp & Hum-----------------");
              
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
     Serial.begin(115200);

     Serial.println("SHT31 test");
      if (! sht31.begin(0x44)) {   // Set to 0x45 for alternate i2c addr
        Serial.println("Couldn't find SHT31");
       while (1) delay(1);
    }
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
