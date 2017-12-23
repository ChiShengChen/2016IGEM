const int analogInPin = A1;
int sensorValue = 0;
float sensorVoltage = 0.0;
float windSpeed = 0.0;

void setup()
{
Serial.begin(9600);
}

void loop()
{
sensorValue = analogRead(analogInPin);

if (sensorValue == 0 || sensorValue == 1023) return;
 sensorVoltage = sensorValue * .004882814;
//int nMapSensorVal = map(sensorValue, 0, 1023, 0.4, 2);

Serial.print("Sensor Value: ");
Serial.print(sensorValue);
Serial.print("\t");
Serial.print("Sensor Voltage: ");
Serial.print(sensorVoltage);

if (sensorVoltage <= 0.4)
windSpeed = 0;
else
{
windSpeed = (sensorVoltage-0.4) * 10 * 2;
}

Serial.print("\t");
Serial.print("Wind Speed: ");
Serial.println(windSpeed);
delay(100);
}
