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


void CIRResponeCount::IRRecvCount(int nCycleTime)
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
