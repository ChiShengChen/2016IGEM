#include "arduino.h"
#ifndef __IR_RESPONE_COUNT__
#define __IR_RESPONE_COUNT__

class CIRResponeCount
{
private:
 unsigned long m_ulCount;
 boolean m_bIsLToH;
 int m_nPinPrevStatus;
 int m_nPinNum;


public:
   CIRResponeCount();
 CIRResponeCount(int nPinNum);
 ~CIRResponeCount();

 void resetCount();

 long getCount();

 void IRRecvCount(int nCycleTime);

};

#endif
