#define NUM_OF_STATES 3
#define pinR  4
#define pinG  8
#define pinB  9
#define button  12
 
uint8_t state = 0;
uint8_t colorR[NUM_OF_STATES] = {1, 0, 1};
uint8_t colorG[NUM_OF_STATES] = {0, 1, 1};
uint8_t colorB[NUM_OF_STATES] = {1, 0, 1};
 
void button_handler(uint32_t id, uint32_t event) {
  state += 1;
  if (state == NUM_OF_STATES)
    state = 0;
 
  digitalWrite(pinR, !colorR[state]);
  digitalWrite(pinG, !colorG[state]);
  digitalWrite(pinB, !colorB[state]);
 
}
 
void setup() {
  pinMode(pinR, OUTPUT);
  pinMode(pinG, OUTPUT);
  pinMode(pinB, OUTPUT);
 
  pinMode(button, INPUT_IRQ_FALL);
  digitalSetIrqHandler(button, button_handler);
  Serial.begin(115200);
}
 
void loop() {
  Serial.print("state: ");
  Serial.println(state);
  Serial.println(!colorR[state]);
  Serial.println(!colorG[state]);
  Serial.println(!colorB[state]);
}
