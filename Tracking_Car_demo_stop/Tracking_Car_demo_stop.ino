//前輪驅動
// 宣告 MotorA I3、I4為右輪
// 宣告 MotorB I1、I2為左輪

//目前車體傾向左轉--->左輪不夠力喵~

#define MotorA_I1     6
#define MotorA_I2     9
#define MotorB_I3    10
#define MotorB_I4    11
//#define MotorA_PWMA    5
//#define MotorB_PWMB    6

// 循線模組
#define SensorLeft    8
#define SensorMiddle  12
#define SensorRight   13
int off_track = 0;     

int carSH = 70;
int carSL = 0;
int carSM = 100;

//修正項(傾向左偏--->右修正)
int carSHS = 30;
int carSLS = 0;

void setup()
{
  Serial.begin(9600);

  pinMode(MotorA_I1,  OUTPUT);
  pinMode(MotorA_I2,  OUTPUT);
  pinMode(MotorB_I3,  OUTPUT);
  pinMode(MotorB_I4,  OUTPUT);
  //pinMode(MotorA_PWMA, OUTPUT);
  //pinMode(MotorB_PWMB, OUTPUT);

  pinMode(SensorLeft,   INPUT);
  pinMode(SensorMiddle, INPUT);
  pinMode(SensorRight,  INPUT);
}

/*void advance(int a)   // 前進
{
  analogWrite(MotorA_I1,0);
  analogWrite(MotorA_I2,60);
  analogWrite(MotorB_I3,0);
  analogWrite(MotorB_I4,60);
    delay(a * 100);
}

void turnR(int d)    //右轉
{
    analogWrite(MotorA_I1,0);    //馬達（右）逆時針轉動
    analogWrite(MotorA_I2,0);
    analogWrite(MotorB_I3,0);   //馬達（左）逆時針轉動
    analogWrite(MotorB_I4,60);
    delay(d * 100);
}

void turnL(int e)    //左轉
{
    analogWrite(MotorA_I1,0);   //馬達（右）順時針轉動
    analogWrite(MotorA_I2,60);
    analogWrite(MotorB_I3,0);    //馬達（左）順時針轉動
    analogWrite(MotorB_I4,0);
    delay(e * 100);
}


void stopRL(int f)  //停止
{
  analogWrite(MotorA_I1,0);
  analogWrite(MotorA_I2,0);
  analogWrite(MotorB_I3,0);
  analogWrite(MotorB_I4,0);
    delay(f * 100);
}

void back(int g)    //後退
{
    analogWrite(MotorA_I1,60);    //馬達（右）逆時針轉動
    analogWrite(MotorA_I2,0);
    analogWrite(MotorB_I3,60);    //馬達（左）順時針轉動
    analogWrite(MotorB_I4,0);
    delay(g * 100);
}*/


void loop()
{
  // 讀取感測器狀態
  int SL = digitalRead(SensorLeft);
  int SM = digitalRead(SensorMiddle);
  int SR = digitalRead(SensorRight);

  if(SM == HIGH)
  {

   if((SL == HIGH) && (SR == LOW))// 車車右邊碰線//往左修正
   {
    analogWrite(MotorA_I1,carSL);
    analogWrite(MotorA_I2,0);
    analogWrite(MotorB_I3,carSH);
    analogWrite(MotorB_I4,0);
    
   }

   else if((SL == LOW) && (SR == HIGH))// 車車左邊碰線//往右修正
  {
   analogWrite(MotorA_I1,carSH);
   analogWrite(MotorA_I2,0);
   analogWrite(MotorB_I3,carSL);
   analogWrite(MotorB_I4,0);
  }

  else//((SL == HIGH) && (SR == HIGH)) // 車車完全在白線//前進
 {
  analogWrite(MotorA_I1,carSH);
  analogWrite(MotorA_I2,0);
  analogWrite(MotorB_I3,carSH);
  analogWrite(MotorB_I4,0);
  }


}

else //SM碰黑線
{
  if((SL == HIGH) && (SR == LOW))// 車車右邊碰線//往左修正
  {
   analogWrite(MotorA_I1,carSL);
   analogWrite(MotorA_I2,0);
   analogWrite(MotorB_I3,carSH);
   analogWrite(MotorB_I4,0);
  }

  else if((SL == LOW) && (SR == HIGH))// 車車左邊碰線//往右修正
 {
  analogWrite(MotorA_I1,carSH);
  analogWrite(MotorA_I2,0);
  analogWrite(MotorB_I3,carSL);
  analogWrite(MotorB_I4,0);
 }

 else//((SL == LOW) && (SR == LOW)) // 車車完全在黑線//後退
{
 analogWrite(MotorA_I1,0);
 analogWrite(MotorA_I2,60);
 analogWrite(MotorB_I3,0);
 analogWrite(MotorB_I4,60);
 delay(500);
/* analogWrite(MotorA_I1,carSLS);
 analogWrite(MotorA_I2,0);
 analogWrite(MotorB_I3,carSHS);
 analogWrite(MotorB_I4,0);*/
 }

}
Serial.print("SL");
Serial.println(SL);
Serial.print("SM");
Serial.println(SM);
Serial.print("SR");
Serial.println(SR);
Serial.println("---");
}
