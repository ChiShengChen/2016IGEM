int m_nPinNum = 4;
int nPinCurStatus = digitalRead(m_nPinNum); 

  void setup(){}

void loop() {
  


  if(nPinCurStatus == LOW)
  {
      Serial.print("nPinCurStatus --->");
      Serial.println(nPinCurStatus);
  }

}
