#include "wiring_watchdog.h"
int timeout = 7000;
int R_led = 9;
int G_led = 10;
int B_led = 11;

void setup() {
  Serial.print("set watchdog timeout:");
  Serial.print(timeout);
  Serial.println("ms");
  pinMode(R_led,OUTPUT);
  pinMode(G_led,OUTPUT);
  pinMode(B_led,OUTPUT);
  wdt_enable(timeout);//開始計時
}

void loop() {
  digitalWrite(R_led,HIGH);
  delay(2000);
  digitalWrite(G_led,HIGH);
  delay(2000);
  digitalWrite(B_led,HIGH);
  delay(1000);
  wdt_reset();//reset"timer"

}
