//#include "max6675.h"

//int thermoDO = 5;  //он же SO
//int thermoCS1 =6;
//int thermoCS2 =7;
//int thermoCLK = 4;  //он же SCK
#define thermoDO    5   // * пины термопар
#define thermoCLK   4   // *
#define thermoCS_b  6   // *
#define thermoCS_t  7   // *
#define delay_trm   200 //задержка между опросами термопар(миллисекунды)
#define resetError  3

int t_top, t_bottom;  //текущие показания температуры
unsigned long int timer; //предыдущее время опроса термодатчиков
unsigned long int timer2;
int prev_t_top, prev_t_bottom; //предыдущие показания температуры
int t_max_bottom, t_max_top; 
//int vccPin = 5;  //пин для питания
//int gndPin = 6;  //пин для земли

void setup() {
  Serial.begin(9600);
  pinMode(thermoCS_b, OUTPUT);
  pinMode(thermoCS_t, OUTPUT);
  pinMode(thermoCLK, OUTPUT);
  pinMode(thermoDO, INPUT);
   pinMode(resetError, INPUT);
//  thermoCLK, thermoCS_b,thermoCS_t, thermoDO  = 0;
  t_top, t_bottom = 0;
  timer, timer2 = 0;
    prev_t_top, prev_t_bottom = 0;
    
  //активируем питание и землю
  //pinMode(vccPin, OUTPUT); digitalWrite(vccPin, HIGH);
 // pinMode(gndPin, OUTPUT); digitalWrite(gndPin, LOW);
  Serial.println("MAX6675 test");
  //ждем стабилизации чипа MAX
  delay(500);
}

void loop() {
  //Выводим показания в монитор порта
  timer2 = millis();
        if (timer2 - timer >= delay_trm) {
          timer = millis();
          t_bottom = max6675_read (thermoCLK, thermoCS_b, thermoDO);
          t_top = max6675_read (thermoCLK, thermoCS_t, thermoDO);
          
          //Serial.println(t_top);
          }
         // if( prev_t_bottom != t_bottom || prev_t_top != t_top) {
         // sprintf (buf, "OK%03d%03d%03d%03d\r\n", p_top, p_bottom, t_top, t_bottom);
         // Serial.print("ok");Serial.println(t_bottom);
//          prev_p_bottom = p_bottom;
//          prev_p_top = p_top;
          if (t_max_bottom < t_bottom && t_bottom > prev_t_bottom ) t_max_bottom = t_bottom;
          prev_t_bottom = t_bottom;
          if (t_max_top < t_top && t_top > prev_t_top) t_max_top = t_top;
          prev_t_top = t_top;
          Serial.print(t_bottom);
          Serial.print(" ");
          Serial.println(t_top);
          //}
        
 //delay(300);
 //Serial.println(timer2);
  
}
double max6675_read (int ck, int cs, int so) { 
  char i;
  int tmp = 0;
  digitalWrite(cs, LOW);//cs = 0;   // Stop a conversion in progress
  asm volatile (" nop"        "\n\t");
  for (i=15 ; i>=0 ;i--) {
    digitalWrite(ck, HIGH);
    asm volatile (" nop"        "\n\t");
    if (digitalRead(so)) tmp|=(1<<i);
    digitalWrite(ck, LOW);
    asm volatile (" nop"        "\n\t");
  }
  digitalWrite(cs, HIGH);
  if (tmp & 0x4) {
    return NAN;
  } else
    return ((tmp>>3))*0.25;
}
