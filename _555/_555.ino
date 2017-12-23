#include "IRResponeCount.h"

#define DEF_IR_RECV_PIN 3

CIRResponeCount  g_IrRecv(DEF_IR_RECV_PIN);

void setup(){}

void loop(){
  g_IrRecv.IRRecvCount(2);
}
