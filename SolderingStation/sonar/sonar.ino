#define HC_TRIG 3
#define HC_ECHO 2
#define x_delay   100 //Задержка между считывниями расстания для фильтра
#define x2_delay  500 //Задержка между считывниями расстания

uint32_t dist, dist1, dist2, dist3, distance; // Фильтр значений для расстояния
unsigned long int timer1, timer2, timer3, timer4;

void setup() {
  Serial.begin(9600);       // для связи
  pinMode(HC_TRIG, OUTPUT); // trig выход
  pinMode(HC_ECHO, INPUT);  // echo вход
  dist1, dist2, dist3 = 0;
  timer1, timer2, timer3, timer4 = 0;
}
void loop() {
  timer3 = millis();
  if ( timer3 > timer4 + x2_delay) {
        distance = getDist();   // получаем расстояние
        timer4 = timer3;
        Serial.println(dist); //Выводим
      }
}
// сделаем функцию для удобства
int getDist() {
  // импульс 10 мкс
  timer1 = millis();
  for ( int i = 0; i < 4; i++) {
    if ( timer1 > timer2 + x_delay) {
      digitalWrite(HC_TRIG, HIGH);
      delayMicroseconds(10);
      digitalWrite(HC_TRIG, LOW);
      // измеряем время ответного импульса
      dist = pulseIn(HC_ECHO, HIGH);
      // считаем расстояние и возвращаем
      dist3 = dist2; dist2 = dist1; dist1 = dist;
      timer2 = timer1;
    }
  }
  dist = ((dist3 + dist2 + dist1) / 3) / 58.2;
  return (dist);
}
