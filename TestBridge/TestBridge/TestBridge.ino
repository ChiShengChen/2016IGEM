#include "MsTimer2.h"
#include "Command.h"
#include "Common.h"
volatile bool g_Status = false;
void OnTimer()
{
  __printf("OnTimer Set g_Status = true\n");
  g_Status = true;
}
void setup() {

    Serial.begin(9600);
    Delay(30 * 1000);
    MsTimer2::set(10 * 1000,  OnTimer);
    MsTimer2::start();

}

void loop() {
    pollingCmd();

  if(g_Status)
  {
	__printf("Start Send Data To Bridge\n");
    SendDataToBridge(DEF_A0_SOIL, 10.23); 
    SendDataToBridge(DEF_A1_WIND, 0.22);
    SendDataToBridge(DEF_A2_UV,   11);
    g_Status = false;
  }

}
