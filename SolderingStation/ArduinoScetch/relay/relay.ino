#include <GyverTimer.h>

#define ZERO_PIN 2    // пин детектора нуля
#define INT_NUM 0     // соответствующий ему номер прерывания
#define PWM_PIN 13  // управляющий пин симистора


void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(ZERO_PIN, INPUT_PULLUP);
  pinMode(PWM_PIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(ZERO_PIN)) {
    int val = map(analogRead(0),0,1023,50,9500);
    delayMicroseconds(val);
    digitalWrite(PWM_PIN, 1);
    delayMicroseconds(20);
    digitalWrite(PWM_PIN, 0);
    Serial.println(val);
  }
}
