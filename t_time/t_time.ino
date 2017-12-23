#include<MsTimer2.h>



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
