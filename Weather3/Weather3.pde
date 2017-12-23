#include <SdFat.h>
#include <Wire.h>
#include "RTClib.h"

// A Weather station data logger for the Arduino
// Creator: Michael Suzuki
// Aref is connected to 3.3Vdc

#define ECHO_TO_SERIAL   1 // echo data to serial port
#define WAIT_TO_START    0 // Wait for serial input in setup()

// define the Real Time Clock object
RTC_DS1307 RTC; 
DateTime now;

// The objects to talk to the SD card
Sd2Card card;
SdVolume volume;
SdFile root;
SdFile file;

int LogInterval;
int SampInterval=10; 	//Number of seconds in the LogInterval
const byte nsamp=10;

// Digital pins
#define greenLEDpin 3
#define WindSpeedPin 4
#define RainPin 5

// Wind variables
bool WindHigh=false;
int CntRPM=0;
float RPM=0.0;
float MPH=0.0;
float Twc=0.0;	//Wind chill Temperature

//Rain variable
bool RainHigh=false;
const float LowAmt=5.0;	//when rain is low, takes this ml to trip
const float HiAmt=5.0;	//when rain is high, takes this ml to trip
float RainAccum=0.0;	//Rain accumulator since start of sample

// Analog pins
//int humidityPin=1;
int ThermPin=2;		//MPC9701 thermal sensor
int VanePin=3;		// Weather vane input

unsigned int ThermValue = 0;  	// value coming from thermal sensor
unsigned int VaneValue=0;		// value coming frome weather vane
int ThermValueAvg;				// Averaged value over samples
int VaneValueAvg;				// Averaged value over samples

float mVTout;		// Temp voltage in mV	
float mVVout;		// Weather Vane voltage in mV
float TempC=0.0;
float TempF=0.0;

// Wind Direction strings
char Str1[ ] = "South";
char Str2[ ] = "SSW";
char Str3[ ] = "SW";
char Str4[ ] = "WSW";
char Str5[ ] = "West";
char Str6[ ] = "WNW";
char Str7[ ] = "NW";
char Str8[ ] = "NNW";
char Str9[ ] = "North";
char Str10[ ] = "NNE";
char Str11[ ] = "NE";
char Str12[ ] = "ENE";
char Str13[ ] = "East";
char Str14[ ] = "ESE";
char Str15[ ] = "SE";
char Str16[ ] = "SSE";
// Timing counters
unsigned long StartTime;	//Log Interval counter

void error(char *str) {
  Serial.print("error: ");
  Serial.println(str);
  while(1);
}


 
void setup(void) {
  analogReference(EXTERNAL); 		//3.3V connected to AREF thru 3.3K AREF=3VDC
  int dummy=analogRead(ThermPin); 	//discard first analogRead
  pinMode(WindSpeedPin,INPUT);
  pinMode(RainPin,INPUT);
  
   // Rain get start state
   // Look for High
   if (digitalRead(RainPin)==HIGH)
    {
		RainHigh=true;
	}
   else 
    {
		RainHigh=false;
    }
	
  LogInterval=SampInterval*1000;	 //  sec between entries
  Serial.begin(9600);
  Serial.println();
  
  #if WAIT_TO_START
	Serial.println("Type any character to start");
	while (!Serial.available());
  #endif //WAIT_TO_START
   
  // initialize the SD card
  if (!card.init()) error("card.init");
  
  // initialize a FAT volume
  if (!volume.init(card)) error("volume.init");
  
  // open root directory
  if (!root.openRoot(volume)) error("openRoot");
  
  // create a new file
  char name[] = "LOGGER00.CSV";
  for (uint8_t i = 0; i < 100; i++) {
	name[6] = i/10 + '0';
	name[7] = i%10 + '0';
	if (file.open(root, name, O_CREAT | O_EXCL | O_WRITE)) break;
  }
  if (!file.isOpen()) error ("file.create");
  Serial.print("Logging to: ");
  Serial.println(name);

  // write header
  file.writeError = 0;

  Wire.begin();  
  if (!RTC.begin()) {
    file.println("RTC failed");
	#if ECHO_TO_SERIAL
		Serial.println("RTC failed");
	#endif  //ECHO_TO_SERIAL
  }
  
  //DirInt starts North=0, increments clockwise, 15 max
  file.println("Date, Time, TempF, MPH, Twc, Direction, DirInt, RainAccum");   
  #if ECHO_TO_SERIAL
	Serial.println("$, TempF, MPH, Twc, Direction, DirInt, RainAccum");
  #endif //ECHO_TO_SERIAL

  // attempt to write out the header to the file
  if (file.writeError || !file.sync()) {
    error("write header");
  }

  StartTime=millis(); //Initialize StartTime
}

void loop(void)
{
	// clear print error
	file.writeError = 0;

 // Wind calculator, looks for RPM continuously
   // Look for High
   if (digitalRead(WindSpeedPin)==HIGH)
    {
		WindHigh=true;
		digitalWrite(greenLEDpin, LOW);
	}
   //Look for Low thus a High to Low transistion
   else if (WindHigh==true)
   {
		//Increment counter
		CntRPM++;
		WindHigh=false;
		digitalWrite(greenLEDpin,HIGH);
   }
   
 // Rain calculator, looks for Rain continuously
   // Look for low to high
   if ((RainHigh==false)&&(digitalRead(RainPin)==HIGH))
    {
		RainHigh=true;
		RainAccum+=LowAmt;
	}
   if ((RainHigh==true)&&(digitalRead(RainPin)==LOW)) 
    {
		RainHigh=false;
		RainAccum+=HiAmt;
    }
   
if ((millis()-StartTime)>long(LogInterval))  // Log Interval has passed
	{
		// fetch the time
		now = RTC.now();
		// log time
		//file.print('"');
		file.print(now.month(), DEC);
		file.print("/");
		file.print(now.day(), DEC);
		file.print("/");
		file.print(now.year(), DEC);
		//file.print('"');
		file.print(", ");
		//file.print('"');
		file.print(now.hour(), DEC);
		file.print(":");
		file.print(now.minute(), DEC);
		file.print(":");
		file.print(now.second(), DEC);
		
	// Do Wind calculations for LogInterval
		// RPM is calculated, Period is averaged
		RPM=CntRPM*(60.0/SampInterval);
		MPH=RPM*.054; //Calibration value
		// Get temperature readings Average nsamps
		for (byte j=0;j<nsamp;j++)
		{    
			ThermValue += analogRead(ThermPin); 
			VaneValue += analogRead(VanePin);
		}    
		ThermValueAvg=ThermValue/nsamp;
		VaneValueAvg=VaneValue/nsamp;

		//Temp calculation
		mVTout=(float) ThermValueAvg*3000.0/1023.0; //3.0V = 3000mV
		//TempC=(mVTout-400.0)/19.5; //Ta = (Vout-400mV)/19.5mV //Original
		//TempC=(mVTout-470.0)/19.5; //Ta = (Vout-400mV)/19.5mV  //Modified
		//TempF=TempC*(9.0/5.0)+32; 
		TempF=0.072811*mVTout-0.89104;	//Calculated with Vaisala
		
		//Wind Chill formula Twc=35.74+0.6215*Tf-35.75MPH^0.16 +0.4275*Tf*MPH^0.16
		//double pow (double base, double exponent )
		if ((TempF <50.0) && (MPH > 3.0))
		{
			Twc=35.74+0.6215*TempF-35.75*pow(MPH,0.16)+0.4275*TempF*pow(MPH,0.16);
		}
		else
		{
			Twc=TempF;
		}
		
		//Weather Vane calculation
		mVVout=(float) VaneValueAvg*3000.0/1023.0;	//3.0V = 3000mV
		
		//Clear Buffers
		ThermValue=0; 
		VaneValue=0;  

		file.print(", ");    
		file.print(TempF,1);
		file.print(", ");    
		file.print(MPH,1);
		file.print(", "); 
		file.print(Twc,1);
		file.print(", "); 
		if (mVVout <=187){
			file.print(Str1);
			file.print(", 8");
		}			
		if ((mVVout >187) && mVVout < 375){
			file.print(Str2);
			file.print(", 9"); 
		}
		if ((mVVout >375) && mVVout < 562){
			file.print(Str3);
			file.print(", 10"); 
		}
		if ((mVVout >562) && mVVout < 750){
			file.print(Str4);
			file.print(", 11"); 
		}
		if ((mVVout >750) && mVVout < 937){
			file.print(Str5);
			file.print(", 12"); 
		}
		if ((mVVout >937) && mVVout < 1125){
			file.print(Str6);
			file.print(", 13"); 
		}
		if ((mVVout >1125) && mVVout < 1312){
			file.print(Str7);
			file.print(", 14"); 
		}
		if ((mVVout >1312) && mVVout < 1500){
			file.print(Str8);
			file.print(", 15"); 
		}
		if ((mVVout >1500) && mVVout < 1687){
			file.print(Str9);
			file.print(", 0"); 
		}
		if ((mVVout >1687) && mVVout < 1875){
			file.print(Str10);
			file.print(", 1"); 
		}
		if ((mVVout >1875) && mVVout < 2062){
			file.print(Str11);
			file.print(", 2"); 
		}
		if ((mVVout >2062) && mVVout < 2250){
			file.print(Str12);
			file.print(", 3"); 
		}
		if ((mVVout >2250) && mVVout < 2437){
			file.print(Str13);
			file.print(", 4"); 
		}
		if ((mVVout >2437) && mVVout < 2625){
			file.print(Str14);
			file.print(", 5"); 
		}
		if ((mVVout >2625) && mVVout < 2812){
			file.print(Str15);
			file.print(", 6"); 
		}
		if (mVVout >2812){
			file.print(Str16);
			file.print(", 7");
		}
		file.print(", "); 
		file.print(RainAccum,1);
		
	#if ECHO_TO_SERIAL
		Serial.print("$, ");
		Serial.print(TempF,1);
		Serial.print(", ");
		Serial.print(MPH,1);
		Serial.print(", ");    
		Serial.print(Twc,1);
		Serial.print(", ");  
		if (mVVout <=187){
			Serial.print(Str1);
			Serial.print(", 8");
		}
		if ((mVVout >187) && mVVout < 375){
			Serial.print(Str2);
			Serial.print(", 9");
		}
		if ((mVVout >375) && mVVout < 562){
			Serial.print(Str3);
			Serial.print(", 10");
		}
		if ((mVVout >562) && mVVout < 750){
			Serial.print(Str4);
			Serial.print(", 11");
		}
		if ((mVVout >750) && mVVout < 937){
			Serial.print(Str5);
			Serial.print(", 12");
		}
		if ((mVVout >937) && mVVout < 1125){
			Serial.print(Str6);
			Serial.print(", 13");
		}
		if ((mVVout >1125) && mVVout < 1312){
			Serial.print(Str7);
			Serial.print(", 14");
		}
		if ((mVVout >1312) && mVVout < 1500){
			Serial.print(Str8);
			Serial.print(", 15");
		}
		if ((mVVout >1500) && mVVout < 1687){
			Serial.print(Str9);
			Serial.print(", 0");
		}
		if ((mVVout >1687) && mVVout < 1875){
			Serial.print(Str10);
			Serial.print(", 1");
		}
		if ((mVVout >1875) && mVVout < 2062){
			Serial.print(Str11);
			Serial.print(", 2");
		}
		if ((mVVout >2062) && mVVout < 2250){
			Serial.print(Str12);
			Serial.print(", 3");
		}
		if ((mVVout >2250) && mVVout < 2437){
			Serial.print(Str13);
			Serial.print(", 4");
		}
		if ((mVVout >2437) && mVVout < 2625){
			Serial.print(Str14);
			Serial.print(", 5");
		}
		if ((mVVout >2625) && mVVout < 2812){
			Serial.print(Str15);
			Serial.print(", 6");
		}
		if (mVVout >2812){
			Serial.print(Str16);
			Serial.print(", 7");
		}
		Serial.print(", ");    
		Serial.print(RainAccum,1);

	#endif //ECHO_TO_SERIAL
	
	file.println();
	#if ECHO_TO_SERIAL
		Serial.println();
	#endif // ECHO_TO_SERIAL

	if (file.writeError) error("write data");
	  
	StartTime = millis();	//Get StartTime for next LogInterval
	// Clear variables for next LogInterval
	digitalWrite(greenLEDpin, LOW);
	WindHigh=false;
	CntRPM=0;
	RainAccum=0.0;
	
	if (!file.sync()) error("sync");
	}
}