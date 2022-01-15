#include <max6675.h>

int b1=0;
int b2=0;
int b3=0;
int p_top, p_bottom;
int t_top, t_bottom;
int state_top, state_bottom;

char buf[32];
unsigned long prev_top, prev_bottom;
int pin_bottom = 13;
int pin_top = 11;
int tick = 200;

unsigned long prev_t;

int thermoDO = 5;
int thermoCLK = 4;
int thermoCS_b = 6;
int thermoCS_t = 7;
MAX6675 thermocouple_b(thermoCLK, thermoCS_b, thermoDO);
MAX6675 thermocouple_t(thermoCLK, thermoCS_t, thermoDO);

void setup()
{
  Serial.begin(9600);
  pinMode(pin_top, OUTPUT);
  digitalWrite(pin_top, 0);
  pinMode(pin_bottom, OUTPUT);
  digitalWrite(pin_bottom, 0);
  t_top = 10;
  t_bottom = 10;
  p_top = 0;
  p_bottom = 0;

  state_top = LOW;
  state_bottom = LOW;
  prev_top = millis();
  prev_bottom = millis();
}

void loop()
{
  if (Serial.available() > 0) {
    b3 = b2; b2 = b1;
    b1 = Serial.read();
    if ((b1 == 'T') && (b2 == 'E') && (b3 == 'S')) {
      p_top = Serial.parseInt(); 
      if (p_top < 0) p_top = 0;
      if (p_top > 100) p_top = 100;
      p_bottom = Serial.parseInt(); 
      if (p_bottom < 0) p_bottom = 0;
      if (p_bottom > 100) p_bottom = 100;
      t_bottom = thermocouple_b.readCelsius();
      t_top = thermocouple_t.readCelsius();
      sprintf (buf, "OK%03d%03d%03d%03d\r\n", p_top, p_bottom, t_top, t_bottom);
      Serial.print(buf);
    } 
  }

  if ((state_top == LOW) && ((millis()-prev_top) >= tick * (100-p_top) / 100)) {
    state_top = HIGH;
    prev_top = millis();  
  }
  if ((state_top == HIGH) && ((millis()-prev_top) >= tick * p_top / 100)) {
    state_top = LOW;
    prev_top = millis();
  }
  digitalWrite(pin_top, state_top);

  if ((state_bottom == LOW) && ((millis()-prev_bottom) >= tick * (100-p_bottom) / 100)) {
    state_bottom = HIGH;
    prev_bottom = millis();  
  }
  if ((state_bottom == HIGH) && ((millis()-prev_bottom) >= tick * p_bottom / 100)) {
    state_bottom = LOW;
    prev_bottom = millis();
  }
  digitalWrite(pin_bottom, state_bottom);
}
