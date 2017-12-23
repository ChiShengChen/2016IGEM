// 宣告 MotorA 為右邊
// 宣告 MotorB 為左邊

#define MotorA_I1     8  //定義 I1 接腳
#define MotorA_I2     9  //定義 I2 接腳
#define MotorB_I3    10  //定義 I3 接腳
#define MotorB_I4    11  //定義 I4 接腳
#define MotorA_PWMA    5  //定義 ENA (PWM調速) 接腳
#define MotorB_PWMB    6  //定義 ENB (PWM調速) 接腳

// 循線模組
#define SensorLeft    A0  //定義 左感測器 輸入腳
#define SensorMiddle  A1  //定義 中感測器 輸入腳
#define SensorRight   A2  //定義 右感測器 輸入腳
int off_track = 0;        //宣告變數


void setup()
{
  Serial.begin(9600);
  
  pinMode(MotorA_I1,  OUTPUT);
  pinMode(MotorA_I2,  OUTPUT);
  pinMode(MotorB_I3,  OUTPUT);
  pinMode(MotorB_I4,  OUTPUT);
  pinMode(MotorA_PWMA, OUTPUT);
  pinMode(MotorB_PWMB, OUTPUT);
  
  pinMode(SensorLeft,   INPUT); 
  pinMode(SensorMiddle, INPUT);
  pinMode(SensorRight,  INPUT);
}

void advance(int a)    // 前進
{
    digitalWrite(MotorA_I1,HIGH);   //馬達（右）順時針轉動
    digitalWrite(MotorA_I2,LOW);
    digitalWrite(MotorB_I3,HIGH);   //馬達（左）逆時針轉動
    digitalWrite(MotorB_I4,LOW);
    delay(a * 100);
}

void turnR(int d)    //右轉
{
    digitalWrite(MotorA_I1,LOW);    //馬達（右）逆時針轉動
    digitalWrite(MotorA_I2,HIGH);
    digitalWrite(MotorB_I3,HIGH);   //馬達（左）逆時針轉動
    digitalWrite(MotorB_I4,LOW);
    delay(d * 100);
}

void turnL(int e)    //左轉
{
    digitalWrite(MotorA_I1,HIGH);   //馬達（右）順時針轉動
    digitalWrite(MotorA_I2,LOW);
    digitalWrite(MotorB_I3,LOW);    //馬達（左）順時針轉動
    digitalWrite(MotorB_I4,HIGH);
    delay(e * 100);
}    

void stopRL(int f)  //停止
{
    digitalWrite(MotorA_I1,HIGH);   //馬達（右）停止轉動
    digitalWrite(MotorA_I2,HIGH);
    digitalWrite(MotorB_I3,HIGH);   //馬達（左）停止轉動
    digitalWrite(MotorB_I4,HIGH);
    delay(f * 100);
}

void back(int g)    //後退
{
    digitalWrite(MotorA_I1,LOW);    //馬達（右）逆時針轉動
    digitalWrite(MotorA_I2,HIGH);
    digitalWrite(MotorB_I3,LOW);    //馬達（左）順時針轉動
    digitalWrite(MotorB_I4,HIGH);
    delay(g * 100);     
}

void loop() 
{
  // 讀取感測器狀態
  int SL = digitalRead(SensorLeft);
  int SM = digitalRead(SensorMiddle);
  int SR = digitalRead(SensorRight);
  
  if((SM == LOW) && (SL == LOW) && (SR == LOW))  // 小車脫離黑線
  {
    analogWrite(MotorA_PWMA,200);    //設定馬達 (右) 轉速
    analogWrite(MotorB_PWMB,200);    //設定馬達 (左) 轉速
    
    
    if(off_track < 3)
    {
      switch(off_track)
      {
        case 0:
          back(1);
          break;
        
        case 1:
          turnR(1);
          break;
          
        case 2:
          turnL(2);
          break;
      }
      
      off_track++;
    }
    else
    {
      stopRL(0);
    }
  }
  else
  {
    off_track = 0;
                   
    if(SM == HIGH)  //中感測器在黑色區域
    {
      if((SL == LOW) && (SR == HIGH))  // 左白右黑, 車體偏右校正
      {
        analogWrite(MotorA_PWMA,200);    //設定馬達 (右) 轉速
        analogWrite(MotorB_PWMB, 80);    //設定馬達 (左) 轉速
        advance(0);
      } 
      else if((SL == HIGH) && (SR == LOW))  // 左黑右白, 車體偏左校正
      {
        analogWrite(MotorA_PWMA, 80);    //設定馬達 (右) 轉速
        analogWrite(MotorB_PWMB,200);    //設定馬達 (左) 轉速
        advance(0);
      }
      else  // 其他, 直走
      {
        analogWrite(MotorA_PWMA,200);    //設定馬達 (右) 轉速
        analogWrite(MotorB_PWMB,200);    //設定馬達 (左) 轉速
        advance(0);
      }
    } 
    else // 中感測器在白色區域, 車體已大幅偏離黑線
    {
      if((SL == LOW) && (SR == HIGH))  // 左白右黑, 車體快速右轉
      {
        analogWrite(MotorA_PWMA,200);    //設定馬達 (右) 轉速
        analogWrite(MotorB_PWMB,200);    //設定馬達 (左) 轉速
        turnR(0);
      }
      else if((SL == HIGH) && (SR == LOW))  // 左黑右白, 車體快速左轉
      {
        analogWrite(MotorA_PWMA,200);    //設定馬達 (右) 轉速
        analogWrite(MotorB_PWMB,200);    //設定馬達 (左) 轉速
        turnL(0);
      }
    }
  }
}
