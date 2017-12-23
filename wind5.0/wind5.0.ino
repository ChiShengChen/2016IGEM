const int analogInPin = A1;
int sensorValue = 0;
float sensorVoltage = 0.0;
float windSpeed = 0.0;

void setup()
{
Serial.begin(9600);
}

void dataMap(int nValue, float flnMin, float finMax, float fOutMin, float fOutMax){
  float fParam = (fOutMax-fOutMin)/(flnMax-flnMin);
  return fParam*nValue+(fOutMin-fParam*flnMin);
}

void loop()
{
sensorValue = analogRead(analogInPin);

if (sensorValue == 0 || sensorValue == 1023) return;
//sensorVoltage = sensorValue * 0.012890624;
//int sensorVoltage = map(sensorValue, 0, 1023, 0.4, 2);


float sensorVoltage = dataMap(sensorValue, 0, 1023, 0.4,2);

Serial.print("Sensor Value: ");
Serial.print(sensorValue);
Serial.print("\t");
Serial.print("Sensor Voltage: ");
Serial.print(sensorVoltage);

if (sensorVoltage <= 0.4){
windSpeed = 0;}
else
{
windSpeed = ((sensorVoltage-0.4) * 10 * 2);
}

Serial.print("\t");
Serial.print("Wind Speed: ");
Serial.println(windSpeed);
delay(100);
}
