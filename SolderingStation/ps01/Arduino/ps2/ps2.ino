#include <GyverTimers.h>    // библиотека таймера

#define pin_zerro   2   // пин детектора нуля
#define int_num     0   // соответствующий ему номер прерывания
#define pin_bottom  13  // управление нижним нагревом
#define pin_top     11  // управление верхним нагревом
#define thermoDO    5   // * пины термопар
#define thermoCLK   4   // *
#define thermoCS_b  6   // *
#define thermoCS_t  7   // *
const byte delay_trm = 200; //задержка между опросами термопар(миллисекунды)
const byte period_int = 37;  // 37 мкс - период прерываний для 255 шагов и 50 Гц
// для 60 Гц - 31
const unsigned int period_com = 3000; // период после которого отключатся
//нагреватели без связи с ПК
//#define symbolT 'T'
//#define symbolT 'E'
//#define symbolT 'S'

byte counter_pwm = 0;   // счётчик тактов для реле
byte counter_trm = 0;   // счётчик тактов для термопар

int b1, b2, b3;
byte p_top, p_bottom;  //текущее значение мощности
int t_top, t_bottom;  //текущие показания температуры
byte prev_p_top, prev_p_bottom; //предыдущая уставка мощности
int prev_t_top, prev_t_bottom; //предыдущие показания температуры
unsigned long int timer; //предыдущее время подключения к ПК
unsigned long int timer2;

char buf[32];

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
  t_top, t_bottom = 0;
  p_top, p_bottom = 0;
  prev_p_top, prev_p_bottom = 0;
  prev_t_top, prev_t_bottom = 0;
  timer, timer2 = 0;
  b1, b2, b3 = 0;

  pinMode(pin_zerro, INPUT_PULLUP);
  attachInterrupt(int_num, isr, RISING); // для самодельной схемы - RISING, иначе - FALLING
  Timer2.enableISR();
  Timer2.setPeriod(period_int);
}

void loop()
{
  while (1) {
    if (Serial.available() > 0) {
      timer = millis();
      b3 = b2; b2 = b1;
      b1 = Serial.read();
      if ((b1 == 'T') && (b2 == 'E') && (b3 == 'S')) {
        p_top = Serial.parseInt();
        if (p_top < 0) p_top = 0;
        if (p_top > 100) p_top = 100;
        p_bottom = Serial.parseInt();
        if (p_bottom < 0) p_bottom = 0;
        if (p_bottom > 100) p_bottom = 100;
        //        timer2 = millis();
        if (counter_trm > delay_trm) {
          counter_trm = 0;
          t_bottom = max6675_read_temp (thermoCLK, thermoCS_b, thermoDO);
          t_top = max6675_read_temp (thermoCLK, thermoCS_t, thermoDO);
        }
        if (prev_p_bottom != p_bottom && prev_p_top != p_top
            && prev_t_bottom != t_bottom && prev_t_top != t_top) {
          sprintf (buf, "OK%03d%03d%03d%03d\r\n", p_top, p_bottom, t_top, t_bottom);
          Serial.print(buf);
          prev_p_bottom = p_bottom;
          prev_p_top = p_top;
          prev_t_bottom = t_bottom;
          prev_t_top = t_top;
        }
      }
    }
  }
}
// прерывание детектора нуля
void isr() {
  counter_pwm = 255;
  counter_trm++;
  Timer2.restart();
}
// прерывание таймера
ISR(TIMER2_A) {
  if ( millis() - timer < period_com ) {
    // на текущем тике включаем
    if (counter_pwm == p_bottom) digitalWrite(pin_bottom, 1);
    // на следующем выключаем
    else {
      if (counter_pwm == p_bottom - 1) digitalWrite(pin_bottom, 0);
    }
    if (counter_pwm == p_top) digitalWrite(pin_top, 1);
    else {
      if (counter_pwm == p_top - 1) digitalWrite(pin_top, 0);
    }
  }
  else digitalWrite(pin_bottom, 0);
  counter_pwm--;
}

double max6675_read_temp (int ck, int cs, int so) {
  char i;
  int tmp = 0;
  digitalWrite(cs, LOW);//cs = 0;   // Stop a conversion in progress
  asm volatile (" nop"        "\n\t");
  for (i = 15 ; i >= 0 ; i--) {
    digitalWrite(ck, HIGH);
    asm volatile (" nop"        "\n\t");
    if (digitalRead(so)) tmp |= (1 << i);
    digitalWrite(ck, LOW);
    asm volatile (" nop"        "\n\t");
  }
  digitalWrite(cs, HIGH);
  if (tmp & 0x4) {
    return NAN;
  } else
    return ((tmp >> 3)) * 0.25;
}
