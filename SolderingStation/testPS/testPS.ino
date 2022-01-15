#include <GyverTimers.h>    // библиотека таймера

#define pin_zerro   2       // пин детектора нуля
#define int_num     0       // соответствующий ему номер прерывания
#define pin_bottom  13      // управление нижним нагревом
#define pin_top     11      // управление верхним нагревом
#define thermoDO    5       // * пины термопар
#define thermoCLK   4       // *
#define thermoCS_b  6       // *
#define thermoCS_t  7       // *
#define resetError  3       // сброс ошибки отвала термопар
#define buzzer      8       // зуммер аварийной ситуации
const byte delay_trm = 200; //задержка между опросами термопар(миллисекунды)
const byte period_int = 37; // 37 мкс - период прерываний для 255 шагов и 50 Гц
// для 60 Гц - 31
const unsigned int period_com = 3000; // период после которого отключатся
//нагреватели без связи с ПК
const byte t_max = 250;  // максимальная температура 

byte counter_pwm = 0;       // счётчик тактов для реле
byte counter_trm = 0;       // счётчик тактов для термопар

int b1, b2, b3;
byte p_top, p_bottom;       //текущее значение мощности
int t_top, t_bottom;        //текущие показания температуры
byte prev_p_top, prev_p_bottom; //предыдущая уставка мощности
int prev_t_top, prev_t_bottom; //предыдущие показания температуры
int t_max_bottom, t_max_top;
unsigned long int timer;    //предыдущее время подключения к ПК
byte ps_error;
byte com_enable;

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
  pinMode(resetError, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);

  pinMode(pin_zerro, INPUT_PULLUP);
  attachInterrupt(int_num, isr, RISING); // для самодельной схемы - RISING, иначе - FALLING
  Timer2.enableISR();
  Timer2.setPeriod(period_int);
  t_top, t_bottom = 0;
  p_top, p_bottom = 0;
  prev_p_top, prev_p_bottom = 0;
  prev_t_top, prev_t_bottom = 0;
  timer = 0;
  b1, b2, b3 = 0;
  t_max_bottom, t_max_top = 10;
  ps_error = 0;
  com_enable = 1;
}

void loop()
{
  if (Serial.available() > 0) {
    timer = millis();
    com_enable = 1;
    b3 = b2; b2 = b1;
    b1 = Serial.read();
    if ((b1 == 'T') && (b2 == 'E') && (b3 == 'S')) {
      p_top = Serial.parseInt();
      if (p_top < 0) p_top = 0;
      if (p_top > 100) p_top = 100;
      p_bottom = Serial.parseInt();
      if (p_bottom < 0) p_bottom = 0;
      if (p_bottom > 100) p_bottom = 100;
      //        timer2 = millis();*/
      if (counter_trm > delay_trm) {
        counter_trm = 0;
        t_bottom = max6675_read (thermoCLK, thermoCS_b, thermoDO);
        t_top = max6675_read (thermoCLK, thermoCS_t, thermoDO);
      }
      //if (prev_p_bottom != p_bottom || prev_p_top != p_top
        //  || prev_t_bottom != t_bottom || prev_t_top != t_top) {
        sprintf (buf, "OK%03d%03d%03d%03d\r\n", p_top, p_bottom, t_top, t_bottom);
        Serial.print(buf);
        prev_p_bottom = p_bottom;
        prev_p_top = p_top;

       // if (t_max_bottom != t_bottom && t_bottom > prev_t_bottom ) t_max_bottom = t_bottom;
      //  prev_t_bottom = t_bottom;
      //  if (t_max_top != t_top && t_top > prev_t_top) t_max_top = t_top;
      //  prev_t_top = t_top;
      //}
    }
  }
/* if (digitalRead(resetError) && ps_error == 1) {
    t_max_bottom = t_bottom;
    t_max_top = t_top;
    com_enable = 1;
    ps_error = 0;
    digitalWrite(buzzer, 0);
  }*/
}

// прерывание детектора нуля
void isr() {
  counter_pwm = 255;
  counter_trm++;
  Timer2.restart();
}

// прерывание таймера
ISR(TIMER2_A) {
 // if (ps_error == 0 && t_bottom < t_max && t_top < t_max
 //                  && t_bottom != 0 && t_top != 0                            // защита от обрыва термопар
 //                  && t_bottom - t_top < 10 && t_top - t_bottom < 10         // защита от отвала термопар
 //                  && t_max_bottom - t_bottom < 10 && t_max_top - t_max_top < 10
 //                  && millis() - timer < period_com) {                       // защита от потери связи с ПК
    if (p_bottom != 0) {
      if (counter_pwm == p_bottom) digitalWrite(pin_bottom, 1);
      else {
        if (counter_pwm == p_bottom - 1) digitalWrite(pin_bottom, 0);
      }
    }
    if (p_top != 0) {
      if (counter_pwm == p_top) digitalWrite(pin_top, 1);
      else {
        if (counter_pwm == p_top - 1) digitalWrite(pin_top, 0);
      }
    }
    digitalWrite(buzzer, 0);
  //}
 // else {
 //   digitalWrite(pin_bottom, 0);
 //   digitalWrite(pin_top, 0);
 //   ps_error = 1;
 //   digitalWrite(buzzer, 1);
 // }
  counter_pwm--;
}

double max6675_read (int ck, int cs, int so) {
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
