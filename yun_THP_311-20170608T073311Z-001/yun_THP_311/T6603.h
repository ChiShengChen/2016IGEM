/*
Copyright (c) 2014 Brian Manley

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "Arduino.h"
#include <SoftwareSerial.h>

#define MAX_ATTEMPTS 10

class T6603 {

 private:
    static const byte FLAG          = 0xFF;
    static const byte BRDCST        = 0xFE;
    static const byte CMD_READ      = 0x02;
    static const byte CMD_UPDATE    = 0x03;
    static const byte CMD_STATUS    = 0xB6;
    static const byte CMD_IDLE      = 0xB9;

    static const byte CO2_PPM       = 0x03;
    static const byte SERIL         = 0x01;
    static const byte ELEVATION     = 0x0F;


    
    int _lastReading;    
    
 public:
 	SoftwareSerial* _serial;
    T6603();  
    ~T6603();  
    void begin(uint8_t, uint8_t);
    int get_co2(void);
    byte get_status(void);
    void set_idle(bool);
};


