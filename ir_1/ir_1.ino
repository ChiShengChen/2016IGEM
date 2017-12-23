#include "IRResponeCount.h"
int nPinNum;
int m_ulCount = 0;
boolean m_bIsLToH = false;
int m_nPinNum = nPinNum;
int m_nPinPrevStatus;
void setup() {
 
  m_nPinPrevStatus = HIGH;
  pinMode(nPinNum, INPUT);
  
}

void getCount()
{
  return m_ulCount;
}


void loop(int nCycleTime) 
{
    delay(nCycleTime);
  int nPinCurStatus = digitalRead(m_nPinNum);

  if(m_nPinPrevStatus != nPinCurStatus)
  {
    m_nPinPrevStatus = nPinCurStatus;
        m_nPinPrevStatus == LOW ? m_bIsLToH = false : m_bIsLToH = true;
  }
  else
  {
    return;
  }

  if(m_bIsLToH)
    {
    m_ulCount++;
    }
}



