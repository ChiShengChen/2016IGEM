#include "Common.h"

void Delay(unsigned long ulTime)
{
    unsigned long startTime = millis();
    int i = 0;
    __printf("-------------Waiting 30s------------------------\n");
    while((millis() - startTime) < (ulTime))
    {
        __printf("%d.", i++);
        
        delay(1000);
    }
   __printf("\n-------------Start Running, Go,Go------------------------\n");
}

void ExFloatToString(float fSrc, char *pDestBuf, int &nBufLen)
{
    if(pDestBuf == NULL)
        return  ;

    sprintf(pDestBuf, "%.2f", fSrc);
    nBufLen = strlen(pDestBuf);


}

void __printf(const char *format, ...)  
{  
    va_list arg;  
    va_start(arg, format);  
    while (*format)  
    {  
        char ret = *format;  
        if (ret == '%')  
        {  
            switch (*++format)  
            {  
            case 'c':
            case 'C':  
                {  
                    char ch = (char)va_arg(arg, int);  
                    Serial.print(ch);  
                    break;  
                }  
            case 's':  
			case 'S':
                {  
                    char *pc = va_arg(arg, char *); 
                    Serial.print(pc); 
					/*
                    while (*pc)  
                    {  
                        pc++;  
                    }  */
                    break;  
                } 
            case 'd':
            case 'D':
                {
                    int nVal = va_arg(arg, int);  
                    Serial.print(nVal, DEC); 
                }
                break;
                case 'x':
                {
                      int nVal = va_arg(arg, int);  
                      Serial.print(nVal, HEX); 
                }
                break;
				case 'f':
				case 'F':
				{
					float fVal = (float)va_arg(arg, double);  
					Serial.print(fVal);
				}
				break; 
            default:  
                break;  
            }  
        }  
        else  
        {  
            Serial.print(*format);
        }  
        format++;  
    }  
    va_end(arg);  
}  

