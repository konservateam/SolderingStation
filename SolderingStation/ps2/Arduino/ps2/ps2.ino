#define pin_zerro 2    // пин детектора нуля
#define INT_NUM 0     // соответствующий ему номер прерывания
#define pin_bottom 13  // количество диммеров
#define pin_top 11
//const byte dimPins[] = {11, 13}; // их пины
#include <GyverTimers.h>    // библиотека таймера
//int dimmer[DIM_AMOUNT];     // переменная диммера
volatile int counter = 0;   // счётчик цикла

int b1=0;
int b2=0;
int b3=0;
int p_top, p_bottom;
int t_top, t_bottom;
int state_top, state_bottom;

char buf[32];
unsigned long prev_top, prev_bottom;
//int tick = 200;

unsigned long prev_t;

int thermoDO = 5;
int thermoCLK = 4;
int thermoCS_b = 6;
int thermoCS_t = 7;

void setup()
{
  Serial.begin(9600);
  pinMode(thermoCS_b, OUTPUT);
  pinMode(thermoCS_t, OUTPUT);
  pinMode(thermoCLK, OUTPUT);
  pinMode(thermoDO, INPUT);
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

  pinMode(pin_zerro, INPUT_PULLUP);
//  for (byte i = 0; i < DIM_AMOUNT; i++) pinMode(dimPins[i], OUTPUT);
  attachInterrupt(INT_NUM, isr, RISING); // для самодельной схемы ставь RISING
  Timer2.enableISR();
  // 37 мкс - период прерываний для 255 шагов и 50 Гц
  // для 60 Гц ставь число 31
  Timer2.setPeriod(37);
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
      t_bottom = max6675_read_temp (thermoCLK, thermoCS_b, thermoDO);
      t_top = max6675_read_temp (thermoCLK, thermoCS_t, thermoDO);
      sprintf (buf, "OK%03d%03d%03d%03d\r\n", p_top, p_bottom, t_top, t_bottom);
      Serial.print(buf);
    } 
  }

/*  if ((state_top == LOW) && ((millis()-prev_top) >= tick * (100-p_top) / 100)) {
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
  digitalWrite(pin_bottom, state_bottom);*/

}
// прерывание детектора нуля
void isr() {
  counter = 255;
  Timer2.restart();
}
// прерывание таймера
ISR(TIMER2_A) {
//  for (byte i = 0; i < DIM_AMOUNT; i++) {
//    if (counter == dimmer[i]) digitalWrite(dimPins[i], 1);  // на текущем тике включаем
      if (counter == p_bottom) digitalWrite(pin_bottom, 1);
//    else if (counter == dimmer[i] - 1) digitalWrite(dimPins[i], 0);  // на следующем выключаем
      else if (counter == p_bottom - 1) digitalWrite(pin_bottom, 0);
      if (counter == p_top) digitalWrite(pin_top, 1);
      else if (counter == p_top - 1) digitalWrite(pin_top, 0);
//  }
  counter--;
}

double max6675_read_temp (int ck, int cs, int so)            
{ char i;
    int tmp = 0;
   digitalWrite(cs, LOW);//cs = 0;   // Stop a conversion in progress
   asm volatile
    (
     " nop"        "\n\t"
    );
   for (i=15;i>=0;i--)
    {  digitalWrite(ck, HIGH);
      asm volatile
    (
     " nop"        "\n\t"
    );
      if( digitalRead(so))
      tmp|=(1<<i);
   
 
      digitalWrite(ck, LOW);
    asm volatile
    (
     " nop"        "\n\t"
    );
    }
   digitalWrite(cs, HIGH);
   if (tmp & 0x4) {
     return NAN;
   } else
   return ((tmp>>3))*0.25;
}
