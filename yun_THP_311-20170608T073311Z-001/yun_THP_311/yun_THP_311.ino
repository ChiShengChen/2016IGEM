
/*
  HTU21D Humidity Sensor Example Code
  By: Nathan Seidle
  SparkFun Electronics
  Date: September 15th, 2013
  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

  Uses the HTU21D library to display the current humidity and temperature

  Open serial monitor at 9600 baud to see readings. Errors 998 if not sensor is detected. Error 999 if CRC is bad.

  Hardware Connections (Breakoutboard to Arduino):
  -VCC = 3.3V
  -GND = GND
  -SDA = A4 (use inline 330 ohm resistor if your board is 5V)
  -SCL = A5 (use inline 330 ohm resistor if your board is 5V)

*/
#include <Bridge.h>
#include <dht.h>
#include "SparkFunHTU21D.h"
#include "Adafruit_BMP085.h"
#include "T6603.h"
#include "SoftwareSerial.h"
#include <Console.h>
#include <SPI.h>
#include <math.h>
#include <Wire.h>
#define DS3231_I2C_ADDRESS 0x68
#define dht_dpin 12

//char TEST1[100]={'\0'};
//char TEST2[100]={'\0'};
//char TEST3[100]={'\0'};
//char TEST4[100]={'\0'};

//long randNumber;

//-------------------o2-------
float WarningValue= 19.5; //The minimum sate concentration of O2 in air
float VoutArray[] =  { 0.30769 ,20.00000, 40.00000 ,60.00000 ,120.61538 ,186.76923};
float  O2ConArray[] =  { 0.00018, 2.66129, 5.32258, 8.05300, 16.19851, 25.15367};

//-------------------o2-------

boolean DEBUG = true;
boolean LED_state = 1;
unsigned long previousMillis = 0;
unsigned long previousLEDMillis = 0;
int count = 0;
double humd = 0;
double temp = 0;
double co2 = 0;
double moisture  = 0; //water_level
double o2 = 0;

char MoisVal[100] = {'\0'};
char humdVal[100] = {'\0'};
char tempVal[100] = {'\0'};
char co2Val[100] = {'\0'};
char o2Val[100]={'\0'};
char timeVal[100]={'\0'};
String timev="";

float pressure = 0;
int a=1;
int b=1;
int c=1;
int d=0;
int d2=0;
int sp3v=1;
float e=0;
float f=0;

//// LED ODF////////////////////////////////////
char Switch[5];
int Switch_int = 999;

char Switch_r[5];
int Switch_r_int = 999;

char Switch_w[5];
int Switch_w_int = 999;

char Switch_sp1[5];
int Switch_sp1_int = 999;

char Switch_sp2[5];
int Switch_sp2_int = 999;

char Switch_b[5];
int Switch_b_int = 999;

char Switch_sp3[5];
int Switch_sp3_int = 999;

char incomming_Switch[2] = {'\0'};
int previous_incomming_Switch = 999;
//////////////////////////////////////////////////


//Create an instance of the object
HTU21D myHumidity;
Adafruit_BMP085 bmp;
T6603 sensor;
dht DHT;
//IPAddress server(140, 113, 89, 58);
//YunClient client;

// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val)
{
  return( (val/10*16) + (val%10) );
}
// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
  return( (val/16*10) + (val%16) );
}

void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte
dayOfMonth, byte month, byte year)
{
  // sets time and date data to DS3231
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set next input to start at the seconds register
  Wire.write(decToBcd(second)); // set seconds
  Wire.write(decToBcd(minute)); // set minutes
  Wire.write(decToBcd(hour)); // set hours
  Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
  Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
  Wire.write(decToBcd(month)); // set month
  Wire.write(decToBcd(year)); // set year (0 to 99)
  Wire.endTransmission();
}
void readDS3231time(byte *second,
byte *minute,
byte *hour,
byte *dayOfWeek,
byte *dayOfMonth,
byte *month,
byte *year)
{
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  // request seven bytes of data from DS3231 starting from register 00h
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}
void displayTime()
{
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
  &year);

  // send it to the serial monitor
  timev=timev+hour;
  // convert the byte variable to a decimal number when displayed
  //timev=timev+":";
  if (minute<10)
  {
    timev=timev+"0";
  }
  timev=timev+minute;
  //timev=timev+":";
  if (second<10)
  {
    timev=timev+"0";
  }
  timev=timev+second;
}

void setup()
{
  Bridge.begin();
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);

  if (DEBUG) Serial.begin(9600);
  //system("/etc/init.d/networking restart");
  delay(2500);
  if (DEBUG) Serial.println("HTU21D Example!");
  myHumidity.begin();
  sensor.begin(8,7);
  if (!bmp.begin()) {
    if (DEBUG) Serial.println("Could not find a valid BMP085 sensor, check wiring!");
  }

  Wire.begin();
  setDS3231time(38,32,15,2,24,10,16);
}

void loop()
{
      //humd = myHumidity.readHumidity();
      //temp = bmp.readTemperature();
      DHT.read11(dht_dpin);
      humd = DHT.humidity;
      temp = DHT.temperature;
      //pressure = bmp.readPressure();
      moisture = analogRead(0);
      co2 = sensor.get_co2();
      //co2 = 800;
      //file:///home/lkk/%E6%A1%8C%E9%9D%A2/dtostrf.ino
      
      ///// idf test /////////////// 
      dtostrf(moisture, 6, 6, MoisVal);
      dtostrf(humd, 6, 6, humdVal);
      dtostrf(temp, 6, 6, tempVal);
      dtostrf(co2, 6, 6, co2Val);

      Bridge.put("Water_Level", MoisVal);
      Bridge.put("Humidity", humdVal);
      Bridge.put("Temperature", tempVal);
      Bridge.put("CO2",co2Val);
      
      timeVal[100]={'\0'};
      displayTime();
      timev.toCharArray(timeVal,100);
      Bridge.put("Timer", timeVal);
      timev="";
    
    //Bridge.get("LED_R",  Switch, 5);
    //Switch[4] = '\0';
    //Switch_int = atoi(Switch);

    Bridge.get("LED_B",  Switch_b, 5);
    Switch[4] = '\0';
    Switch_b_int = atoi(Switch_b);

    Bridge.get("LED_R",  Switch_r, 5);
    Switch[4] = '\0';
    Switch_r_int = atoi(Switch_r);

    Bridge.get("LED_W",  Switch_w, 5);
    Switch[4] = '\0';
    Switch_w_int = atoi(Switch_w);

    Bridge.get("WATER_SPRAY",  Switch_sp1, 5);
    Switch[4] = '\0';
    Switch_sp1_int = atoi(Switch_sp1);

    Bridge.get("WATER_PUMP",  Switch_sp2, 5);
    Switch[4] = '\0';
    Switch_sp2_int = atoi(Switch_sp2);

    Bridge.get("NUTRITION",  Switch_sp3, 5);
    Switch[4] = '\0';
    Switch_sp3_int = atoi(Switch_sp3);
    

    digitalWrite(11, Switch_w_int);  
    digitalWrite(9, Switch_r_int);  
    digitalWrite(10, Switch_b_int);  
    //digitalWrite(12, Switch_int);
    digitalWrite(4, Switch_sp2_int); 
    //digitalWrite(5, Switch_sp3_int); 

    
    if(Switch_sp1_int==1&&c==1){
      digitalWrite(13, Switch_sp1_int);
      delay(10000);
      digitalWrite(13, 0);
      c=0;
    }
    else if(Switch_sp1_int==0){
      c=1;
    }

    if(Switch_sp3_int==1&&sp3v==1){
      digitalWrite(5,Switch_sp3_int);
      delay(5000);
      digitalWrite(5,0);
      sp3v=0;
    }
    else if(Switch_sp3_int==0){
      sp3v=1;
    }
    
    //-----o2-----
    double sensorValue;
    double sensorVoltage;
    sensorValue = analogRead(A1);
    sensorVoltage =(sensorValue/1000)*5.0;
    //sensorVoltage = sensorVoltage/201*10000;
    //o2 = sensorVoltage/7.43;
    o2 = sensorVoltage*10;
    if(d<6){
      e=e+o2;
      d++;
    }
    else{
      o2=e/6;
      dtostrf(o2, 6, 6, o2Val);
      Bridge.put("O2",o2Val);
      d=0;
      e=0;
    }
}


