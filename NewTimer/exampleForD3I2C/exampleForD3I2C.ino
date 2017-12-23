#include <MsTimer2.h>
#include "SHT31.h"
#include "Seeed_BMP280.h"
SHT31 sht31;
BMP280 bmp280;

#define DEF_D7      3
void GetDataFormI2C()
{
    /*1. disable D3 Interrput
      2.
      for(int i = 0; i < 3; i++)
      { 
        //ret =  get data from I2C
        if(ret == OK)
          break;  
       }
      3. enable D3 Interrput
       */
       Serial.println("\n");
       Serial.println("-------------------Start Timer Interrput GetDataFormI2C---------------");
       detachInterrupt (digitalPinToInterrupt(DEF_D7));

        float fSHT3Temp = 0.0;
        float fSHT3Hum = 0.0;
        float fBMP280Temp = 0.0;
        float fBMP280Press = 0.0;
        getSHT3TempAndHum(fSHT3Temp, fSHT3Hum);
        getBMP280TempAndPressure(fBMP280Temp,fBMP280Press);
        attachInterrupt(digitalPinToInterrupt(DEF_D7), D3InterruptFun, FALLING);
        Serial.println("-------------------end of Timer Interrput GetDataFormI2C---------------");
        Serial.println("\n");
}
void getSHT3TempAndHum(float &fTemp, float &fHum)
{

      for(int i = 0; i < 3; i++)
      {
            fTemp = sht31.getTemperature();
     
            fHum = sht31.getHumidity();

            if((NAN == fTemp) || (NAN == fHum))
            {
              Serial.println("\n");
              Serial.print("Get get Temp or Hum Fail from SHT3, Retry : ");
              Serial.println(i);
              Serial.println("\n");
              continue;
            }
            else
            {
              Serial.println("\n");
              Serial.print("SHT32 Temp = ");
              Serial.print(fTemp);
              Serial.println(" C");
              Serial.println("\n");
              
              Serial.print("SHT32 Hum = ");
              Serial.print(fHum);
              Serial.println("%");
              Serial.println();
              
              break;
            }
      }
}

void getBMP280TempAndPressure(float &fTemp, float &fPressure)
{
     fTemp = bmp280.getTemperature();
     fPressure = bmp280.getPressure();


     Serial.println("\n");
     Serial.print("bmp280 Temp = ");
     Serial.print(fTemp);
     Serial.println(" C");
     Serial.println("\n");
              
     Serial.print("bmp280 Pressure = ");
     Serial.print(fPressure);
     Serial.println(" Pa");
     Serial.println("\n");
              
}


void D3InterruptFun()
{
   Serial.println("\n");
   Serial.println("----------------D3InterruptFun-------------------------");
   Serial.println("\n");
}

void initialize()
{

     MsTimer2::set(1 * 1000 * 60,  GetDataFormI2C); //中断设置函数, 每5分钟进入一次中断
     MsTimer2::start();    //开始计时
      if(!bmp280.init()){
         Serial.println("Device error!");
     }
     sht31.begin();
     pinMode(DEF_D7, INPUT_PULLUP);
     attachInterrupt(digitalPinToInterrupt(DEF_D7), D3InterruptFun, FALLING);
 }
void setup() {
    Serial.begin(9600);
    initialize();

}
void loop() {
 
}
