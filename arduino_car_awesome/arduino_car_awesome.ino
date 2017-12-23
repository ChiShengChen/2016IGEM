#define MOTORLF 3 // MOTOR LEFT FORWARD WHEN HIGH
#define MOTORLB 5 // MOTOR LEFT BACKWARD WHEN HIGH
#define MOTORRF 6 // MOTOR RIGHT FORWARD WHEN HIGH
#define MOTORRB 9 // MOTOR RIGHT BACKWARD WHEN HIGH

#define IRR 13
#define IRML 2
#define IRMR A5
#define IRL A4

#define BUTTON 7

#define RIGHT_FIRST 1
#define LEFT_FIRST 0
#define DETECTED 1
#define NOT_DETECTED 0
#define DIR_LEFT 0
#define DIR_RIGHT 1

#define TURN_FACTOR 0.45
#define TURN_BOUND 800
int SPEED_LEFT = 65;
int SPEED_RIGHT = 65;

void leftMotor(int);
void rightMotor(int);

long long detectTime = 0;
bool whichSideFirst = LEFT_FIRST;
bool isDetected = NOT_DETECTED;

void setup() {
  pinMode(MOTORLF, OUTPUT);
  pinMode(MOTORLB, OUTPUT);
  pinMode(MOTORRF, OUTPUT);
  pinMode(MOTORRB, OUTPUT);
  pinMode(IRR, INPUT);
  pinMode(IRMR, INPUT);
  pinMode(IRML, INPUT);
  pinMode(IRL, INPUT);
  pinMode(BUTTON, INPUT_PULLUP);

  //Serial.begin(9600);
  //Serial.println("BEGIN");
  //leftMotor(50);
  //rightMotor(50);
  while(digitalRead(BUTTON)){
    // loop until button is pressed down
  }
  delay(2000);
  // GO!!!!
  //Serial.println("GOOOO");
}

void loop() {
//  bool irrState = !digitalRead(IRR);
  bool irmrState = digitalRead(IRMR);
  bool irmlState = !digitalRead(IRML);
//  bool irlState = !digitalRead(IRL);

/*
  if(!digitalRead(IRMR)){ //右邊先碰線
    Serial.println("RIGHT");
    detectTime = millis();
    while(digitalRead(IRML)){}
    // 左轉
    Serial.println("LEFT TURN");
    leftMotor(-TURN_SPEED);
    rightMotor(TURN_SPEED);
    delay(TURN_FACTOR*(millis()-detectTime));
  } else if (!digitalRead(IRML)){ //左邊先碰線
    Serial.println("LEFT");
    detectTime = millis();
    while(digitalRead(IRMR)){}
    // 右轉
    Serial.println("RIGHT TURN");
    leftMotor(TURN_SPEED);
    rightMotor(-TURN_SPEED);
    delay(TURN_FACTOR*(millis()-detectTime));
  } else {
    Serial.println("FORWARD");
    leftMotor(GO_SPEED_LEFT);
    rightMotor(GO_SPEED_RIGHT);
  }
*/

//Serial.println(irmlState);


  if(irmrState && !isDetected){
    //Serial.println("RIGHT DETECT");
    isDetected = DETECTED;
    whichSideFirst = RIGHT_FIRST;
    detectTime = millis();
    //increaseSpeed(DIR_RIGHT);
  } else if (irmlState && !isDetected){
    //Serial.println("LEFT DETECT");
    isDetected = DETECTED;
    whichSideFirst = LEFT_FIRST;
    detectTime = millis();
    //increaseSpeed(DIR_LEFT);
  } else if (irmrState && isDetected && (whichSideFirst == LEFT_FIRST)){
    // 右轉
    //Serial.println("RIGHT TURN");
    leftMotor(SPEED_LEFT);
    rightMotor(-SPEED_RIGHT);
    delay(TURN_FACTOR*constrain(TURN_BOUND-(millis()-detectTime), 0, TURN_BOUND));
    isDetected = NOT_DETECTED;
    backCheck();
  } else if (irmlState && isDetected && (whichSideFirst == RIGHT_FIRST)){
    // 左轉
    //Serial.println("LEFT TURN");
    leftMotor(-SPEED_LEFT);
    rightMotor(SPEED_RIGHT);
    delay(TURN_FACTOR*constrain(TURN_BOUND-(millis()-detectTime), 0, TURN_BOUND));
    isDetected = NOT_DETECTED;
    backCheck();
  } else {
    leftMotor(SPEED_LEFT);
    rightMotor(SPEED_RIGHT);
  }


//    if(irmState){
//      leftMotor(-30);
//      rightMotor(-60);
//      delay(100);
//    }else if(irlState){
//      leftMotor(50);
//      rightMotor(20);
//    }else{
//      leftMotor(20);
//      rightMotor(60);
//    }


    
//  if(irmState){
//    if(irrState){
//      leftMotor(20);
//      rightMotor(80);
//    }else if(irlState){
//      leftMotor(80);
//      rightMotor(20);
//    } else {
//      leftMotor(-40);
//      rightMotor(-60);
//    }
//  } else {
//    if(irrState){
//      leftMotor(30);
//      rightMotor(70);
//    }else if(irlState){
//      leftMotor(70);
//      rightMotor(30);
//    } else {
//      leftMotor(50);
//      rightMotor(50);
//    }
//  }
}

void increaseSpeed(bool dir){
  if(dir == DIR_RIGHT && SPEED_RIGHT < 55){
    ++SPEED_RIGHT;
    --SPEED_LEFT;
  } else if(dir == DIR_LEFT && SPEED_LEFT < 55){
    ++SPEED_LEFT;
    --SPEED_RIGHT;
  }
}

void backCheck(){
  bool irmrState = digitalRead(IRMR);
  bool irmlState = !digitalRead(IRML);
  if(irmrState && irmlState){
    // 後退一下
    leftMotor(-SPEED_LEFT);
    rightMotor(-SPEED_RIGHT);
    delay(300);
    backCheck();
  } else if(whichSideFirst == RIGHT_FIRST){
    leftMotor(SPEED_LEFT);
    rightMotor(-SPEED_RIGHT);
    delay(130);
  } else {
    leftMotor(-SPEED_LEFT);
    rightMotor(SPEED_RIGHT);
    delay(130);
  }
}


void leftMotor(int s){
  if(s > 0){
    analogWrite(MOTORLF, s);
    digitalWrite(MOTORLB, LOW);
  } else if (s == 0){
    digitalWrite(MOTORLF, LOW);
    digitalWrite(MOTORLB, LOW);
  } else {
    digitalWrite(MOTORLF, LOW);
    analogWrite(MOTORLB, -s);
  }
}

void rightMotor(int s){
  if(s > 0){
    analogWrite(MOTORRF, s);
    digitalWrite(MOTORRB, LOW);
  } else if (s == 0){
    digitalWrite(MOTORRF, LOW);
    digitalWrite(MOTORRB, LOW);
  } else {
    digitalWrite(MOTORRF, LOW);
    analogWrite(MOTORRB, -s);
  }
}

