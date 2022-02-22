#define HC_TRIG 3
#define HC_ECHO 2
#define Out1   13
#define Out2   12
#define xDelay   30 //Задержка между считывниями расстания для фильтра
#define x2Delay  100 //Задержка между считывниями расстояния
#define Accuracy 5 // Погрешность в %
uint16_t dist, dist1, dist2, dist3, distance; // Фильтр значений для расстояния
uint16_t distNormal, distAccuracyMin, distAccuracyMax;
unsigned long int timer1, timer2, timer3, timer4;
unsigned long int timerOut1On, timerOut2On, timerOut1Off, timerOut2Off;
boolean lightOn, objectIn;

void setup() {
  Serial.begin(9600);       // для связи
  pinMode(HC_TRIG, OUTPUT); // trig выход
  pinMode(HC_ECHO, INPUT);  // echo вход
  pinMode(Out1, OUTPUT);
  pinMode(Out2, OUTPUT);
  dist1, dist2, dist3 = 0;
  timer1, timer2, timer3, timer4 = 0;
  timerOut1On = 0;
  timerOut1Off = 5000;
  timerOut2On = 0;
  timerOut2Off = 500;
  distNormal = getDist();
  distAccuracyMin = distNormal - (distNormal / 100 * Accuracy );
  distAccuracyMax = distNormal + (distNormal / 100 * Accuracy );
  digitalWrite(Out1, LOW);
  lightOn = 0;
}
void loop() {
  timer3 = millis();
  if ( timer3 >= timer4 + x2Delay) {
    distance = getDist();   // получаем расстояние
    timer4 = timer3;
    //Serial.print(distNormal);
    //Serial.print(" ");
    //Serial.print(distAccuracyMin);
    //Serial.print(" ");
    //Serial.print(distAccuracyMax);
    //Serial.print(" ");
    Serial.println(distance); //Выводим
  }
  if ( distance < distAccuracyMin) {
    objectIn = 1;
    timerOut1On = millis();
  }
  if ( distance == distNormal || (distance > distAccuracyMin && distance < distAccuracyMax)) {
    objectIn = 0;
  }

  if ( lightOn == 0 && distance != 0) {
    if ( objectIn == 1) {
      digitalWrite(Out1, HIGH);
      lightOn = 1;
      //Serial.print(distNormal);
      //Serial.print(" ");
      //Serial.print(distAccuracyMin);
      //Serial.print(" ");
      //Serial.print(distAccuracyMax);
      //Serial.print(" ");
      //Serial.println(distance); //Выводим
      //Serial.println("***");
    }
  }
  if ( lightOn == 1 && distance != 0) {
    if ( objectIn == 0) {
      if ( timerOut1Off <= millis() - timerOut1On) {
        digitalWrite(Out1, LOW);
        lightOn = 0;
       // Serial.println("!*");
      }
    }
  }
  //if (distance > distAccucyMax) {
  //  digitalWrite(Out1, HIGH);
  // }
}
// сделаем функцию для удобства
int getDist() {
  // импульс 10 мкс
  for (int i = 0; i <= 3;) {
    timer1 = millis();
    if ( timer1 >= timer2 + xDelay) {
      digitalWrite(HC_TRIG, HIGH);
      delayMicroseconds(10);
      digitalWrite(HC_TRIG, LOW);
      // измеряем время ответного импульса
      dist = pulseIn(HC_ECHO, HIGH);
      // считаем расстояние и возвращаем
      dist3 = dist2; dist2 = dist1; dist1 = dist;
      timer2 = timer1;
      //timer1 = millis();
      i++;
    }
  }
  dist = ((dist3 + dist2 + dist1) / 3);
  return (dist);
}
