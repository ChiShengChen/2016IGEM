//Car_testing

#define MotorA_I1     6
#define MotorA_I2     9
#define MotorB_I3    10
#define MotorB_I4    11
void setup() {
  // put your setup code here, to run once:
 pinMode(MotorA_I1,  OUTPUT);
  pinMode(MotorA_I2,  OUTPUT);
  pinMode(MotorB_I3,  OUTPUT);
  pinMode(MotorB_I4,  OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
 analogWrite(MotorA_I1,0);
 analogWrite(MotorA_I2,60);
 analogWrite(MotorB_I3,0);
 analogWrite(MotorB_I4,60);
}
