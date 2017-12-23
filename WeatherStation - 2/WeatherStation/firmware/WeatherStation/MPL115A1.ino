// MPL115A1 SPI Digital Barometer
// ATmega328 / 5V
// 
//  MPL115A1                Arduino
//  SDN ------------------- D9
//  CSN ------------------- D10
//  SDO ------------------- D12
//  SDI ------------------- D11
//  SCK ------------------- D13
//  GND ------------------- GND
//  VDD ------------------- VCC

// Includes
#include <SPI.h>

// Pin definitions
#define MPL115A1_ENABLE_PIN 6
#define MPL115A1_SELECT_PIN 5

const float ERROR_CORECTION = 25.00; // inside 27 works better ?!?!?

/// MPL115A1 register address map

// conversion registers
#define PRESH 0x80
#define PRESL 0x82
#define TEMPH 0x84
#define TEMPL 0x86

// compensation registers
#define A0MSB  0x88
#define A0LSB  0x8A
#define B1MSB  0x8C
#define B1LSB  0x8E
#define B2MSB  0x90
#define B2LSB  0x92
#define C12MSB 0x94
#define C12LSB 0x96
#define C11MSB 0x98
#define C11LSB 0x9A
#define C22MSB 0x9C
#define C22LSB 0x9E
/// MPL115A1 register address map

void initBarometer() {
  pinMode(MPL115A1_SELECT_PIN, OUTPUT);
  pinMode(MPL115A1_ENABLE_PIN, OUTPUT);
  
  digitalWrite(MPL115A1_ENABLE_PIN, LOW);  // sleep the MPL115A1
  digitalWrite(MPL115A1_SELECT_PIN, HIGH); // set the chip select inactive, select signal is CS LOW
}

void getTemperaturePressure(weather &values) {
  signed int sia0, sib1, sib2, sic12, sic11, sic22;
  signed long lt1, lt2, lt3, si_c11x1, si_a11, si_c12x2;
  signed long si_a1, si_c22x2, si_a2, si_a1x1, si_y1, si_a2x2;
  unsigned int uiPadc, uiTadc;
  
  digitalWrite(MPL115A1_ENABLE_PIN, HIGH); // wake the MPL115A1
    delay(10);  // give the chip a few ms to wake up
    
    writeRegister(0x24, 0x00);      // Start Both Conversions
    delay(1);                       // Max wait time is 1ms, typ 0.8ms
    
    // read pressure
    uiPadc  = (unsigned int) readRegister(PRESH) << 8;
    uiPadc += (unsigned int) readRegister(PRESL) & 0x00FF;
    
    // read temperature
    uiTadc  = (unsigned int) readRegister(TEMPH) << 8;
    uiTadc += (unsigned int) readRegister(TEMPL) & 0x00FF;
    
    // Placing Coefficients into 16-bit Variables
    // a0
    sia0  = (signed int) readRegister(A0MSB) << 8;
    sia0 += (signed int) readRegister(A0LSB) & 0x00FF;
    
    // b1
    sib1  = (signed int) readRegister(B1MSB) << 8;
    sib1 += (signed int) readRegister(B1LSB) & 0x00FF;
    
    // b2
    sib2  = (signed int) readRegister(B2MSB) << 8;
    sib2 += (signed int) readRegister(B2LSB) & 0x00FF;
    
    // c12
    sic12  = (signed int) readRegister(C12MSB) << 8;
    sic12 += (signed int) readRegister(C12LSB) & 0x00FF;
    
    // c11
    sic11  = (signed int) readRegister(C11MSB) << 8;
    sic11 += (signed int) readRegister(C11LSB) & 0x00FF;
    
    // c22
    sic22  = (signed int) readRegister(C22MSB) << 8;
    sic22 += (signed int) readRegister(C22LSB) & 0x00FF;
  
  digitalWrite(MPL115A1_ENABLE_PIN, LOW); // put the MPL115A1 to sleep
  
  // Coefficient 9 equation compensation
  uiPadc = uiPadc >> 6;
  uiTadc = uiTadc >> 6;
  
  // Step 1 c11x1 = c11 * Padc
  lt1 = (signed long) sic11;
  lt2 = (signed long) uiPadc;
  lt3 = lt1*lt2;
  si_c11x1 = (signed long) lt3;
  
  // Step 2 a11 = b1 + c11x1
  lt1 = ((signed long)sib1)<<14;
  lt2 = (signed long) si_c11x1;
  lt3 = lt1 + lt2;
  si_a11 = (signed long)(lt3>>14);
  
  // Step 3 c12x2 = c12 * Tadc
  lt1 = (signed long) sic12;
  lt2 = (signed long) uiTadc;
  lt3 = lt1*lt2;
  si_c12x2 = (signed long)lt3;
  
  // Step 4 a1 = a11 + c12x2
  lt1 = ((signed long)si_a11<<11);
  lt2 = (signed long)si_c12x2;
  lt3 = lt1 + lt2;
  si_a1 = (signed long) lt3>>11;
  
  // Step 5 c22x2 = c22*Tadc
  lt1 = (signed long)sic22;
  lt2 = (signed long)uiTadc;
  lt3 = lt1 * lt2;
  si_c22x2 = (signed long)(lt3);
  
  // Step 6 a2 = b2 + c22x2
  lt1 = ((signed long)sib2<<15);
  lt2 = ((signed long)si_c22x2>1);
  lt3 = lt1+lt2;
  si_a2 = ((signed long)lt3>>16);
  
  // Step 7 a1x1 = a1 * Padc
  lt1 = (signed long)si_a1;
  lt2 = (signed long)uiPadc;
  lt3 = lt1*lt2;
  si_a1x1 = (signed long)(lt3);
  
  // Step 8 y1 = a0 + a1x1
  lt1 = ((signed long)sia0<<10);
  lt2 = (signed long)si_a1x1;
  lt3 = lt1+lt2;
  si_y1 = ((signed long)lt3>>10);
  
  // Step 9 a2x2 = a2 * Tadc
  lt1 = (signed long)si_a2;
  lt2 = (signed long)uiTadc;
  lt3 = lt1*lt2;
  si_a2x2 = (signed long)(lt3);
  
  // Step 10 pComp = y1 + a2x2
  lt1 = ((signed long)si_y1<<10);
  lt2 = (signed long)si_a2x2;
  lt3 = lt1+lt2;
  
  // lt3>>13 - Fixed point result with rounding
  // ADC counts are 0 to 1023, pressure is 50 to 115kPa respectively
  // temperature = (ERROR_CORECTION+(uiTadc-472)/-5.35); // -5.35 counts per °C, 472 counts is 25°C
  // pressure =  ((((65.0/1023.0)*(lt3>>13))+50)*7.50061683); // also convert kPa in mmHg (*7.50061683)
  
  values.temperature = ERROR_CORECTION+((float) uiTadc-472.00)/-5.35;
  values.pressure    = (((65.00/1023.00)*((float) (lt3>>13)))+50.00)*7.50061683;
}

unsigned int readRegister(byte thisRegister) {
  byte result = 0;
  
  digitalWrite(MPL115A1_SELECT_PIN, LOW); // select the MPL115A1
  
  // send the request
  SPI.transfer(thisRegister);
  result = SPI.transfer(0x00);
  
  digitalWrite(MPL115A1_SELECT_PIN, HIGH); // deselect the MPL115A1
  
  return result;
}

void writeRegister(byte thisRegister, byte thisValue) {
  digitalWrite(MPL115A1_SELECT_PIN, LOW); // select the MPL115A1
  
  // send the request
  SPI.transfer(thisRegister);
  SPI.transfer(thisValue);
  
  digitalWrite(MPL115A1_SELECT_PIN, HIGH); // deselect the MPL115A1
}
