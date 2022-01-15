void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(0, OUTPUT);
  digitalWrite(0, HIGH);
  pinMode(1, OUTPUT);
  digitalWrite(1, HIGH);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(2, INPUT);
}

void loop() {
  int value0 = analogRead(1);
  if ( value0 >= 4 ){
    digitalWrite(0, LOW);
    int value1 = digitalRead(3);
    if ( value1 == HIGH ){
      digitalWrite(0, HIGH);
      digitalWrite(1, LOW);
    }
    
  }else{
      digitalWrite(0, HIGH);
      digitalWrite(1, HIGH);
  }

  delay(1000);             // wait for a second
}
