#include "IRResponeCount.h"
#include"arduino.h"
#define DEF_IR_RECV_PIN_2   4
#define DEF_IR_RECV_PIN_5   5

//CIRResponeCount  g_IrRecv2(DEF_IR_RECV_PIN_2);
CIRResponeCount  g_IrRecv5(DEF_IR_RECV_PIN_5);


void setup() {
  Serial.begin(9600);
}

void loop() {
    /*g_IrRecv2.IRRecvCount(2);
    g_IrRecv2.getCount();*/

    g_IrRecv5.IRRecvCount(2);
    g_IrRecv5.getCount();
}
