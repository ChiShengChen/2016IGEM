#include "IRResponeCount.h"

CIRResponeCount::CIRResponeCount(int nPinNum)
{
	m_ulCount = 0;
	m_bIsLToH = false;
	m_nPinNum = nPinNum;
	m_nPinPrevStatus = HIGH;
	pinMode(nPinNum, INPUT);
}

CIRResponeCount::~CIRResponeCount()
{

}


void CIRResponeCount::resetCount()
{
	m_ulCount = 0;
}

long CIRResponeCount::getCount()
{
	return m_ulCount;
}


void
CIRResponeCount::IRRecvCount(int nCycleTime)
{
    delay(nCycleTime);
  Serial.println("------------------------------------Start-------------------------"); 
  int nPinCurStatus = digitalRead(m_nPinNum); 
  Serial.print("nPinCurStatus-->");
  Serial.println(nPinCurStatus);
  Serial.print("m_nPinPrevStatus-->");
  Serial.println(m_nPinPrevStatus);
  if(m_nPinPrevStatus != nPinCurStatus)
  {
      Serial.println("m_nPinPrevStatus != nPinCurStatus");  
    m_nPinPrevStatus = nPinCurStatus;
        m_nPinPrevStatus == LOW ? m_bIsLToH = false : m_bIsLToH = true;
    
    Serial.print("m_bIsLToH-->");
    Serial.println(m_bIsLToH);
  }
  else
  {
    Serial.println("m_nPinPrevStatus == nPinCurStatus");  
    return;
  }
  Serial.println("------------------------------------end-------------------------");   
  if(!m_bIsLToH)
    {
    m_ulCount++;
    Serial.print("m_ulCount-->");
    Serial.println(m_ulCount);
    }
}   

