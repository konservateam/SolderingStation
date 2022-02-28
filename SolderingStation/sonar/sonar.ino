#define HC_TRIG 3
#define HC_ECHO 2
#define Out1   13
#define Out2   12
#define xDelay   30 //Задержка между считывниями расстания для фильтра
#define x2Delay  50 //Задержка между считывниями расстояния
#define Accuracy 5 // Погрешность в %
uint16_t dist, dist1, dist2, dist3, distance, distance2; // Фильтр значений для расстояния
uint16_t distNormal, distAccuracyMin, distAccuracyMax;
unsigned long int timer1, timer2, timer3, timer4;
unsigned long int timerOut1On, timerOut2On, timerOut1Off, timerOut2Off;
boolean out1On, out2On, objectIn;

void setup() {
  Serial.begin(9600);       // для связи
  pinMode(HC_TRIG, OUTPUT); // trig выход
  pinMode(HC_ECHO, INPUT);  // echo вход
  pinMode(Out1, OUTPUT);
  pinMode(Out2, OUTPUT);
  dist1, dist2, dist3, distance2 = 0;
  timer1, timer2, timer3, timer4 = 0;
  timerOut1On = 0; // Буфер с временем включения первого канала
  timerOut1Off = 5000;// Время работы первого канала (мс)
  timerOut2On = 7000; // Смещение времени перед включением второго канала (мс)
  timerOut2Off = 5000; // Время работы второго канала (мс)
  distNormal = getDist();
  distAccuracyMin = distNormal - (distNormal / 100 * Accuracy );
  distAccuracyMax = distNormal + (distNormal / 100 * Accuracy );
  digitalWrite(Out1, LOW);
  digitalWrite(Out2, LOW);
  out1On = 0;
  out2On = 0;
}
void loop() {
  distance = getDist();   // получаем расстояние
  //Serial.print(distNormal);
  //Serial.print(" ");
  //Serial.print(distAccuracyMin);
  //Serial.print(" ");
  //Serial.print(distAccuracyMax);
  //S/erial.print(" ");
  //Serial.println(distance); //Выводим
  //---------------------------
  if ( distance < distAccuracyMin) {
    distance2 = getDist();
    //Serial.print(distance);
    //Serial.print(" ");
    //Serial.println(distance2);
    if ( distance < distAccuracyMin && distance2 < distAccuracyMin) {
      timerOut1On = millis();
      //timerOut1Buff = timerOut1On;
      if ( out1On == 0) {
        objectIn = 1;
        digitalWrite(Out1, HIGH);
        out1On = 1;
        Serial.println("1*");
      }
    }
  }

  if ( distance == distNormal || (distance > distAccuracyMin && distance < distAccuracyMax)) {
    objectIn = 0;
  }

  if ( out1On == 1 && distance != 0) {
    if ( objectIn == 0) {
      if ( timerOut1Off <= millis() - timerOut1On) {
        digitalWrite(Out1, LOW);
        out1On = 0;
        Serial.println("1*-");
      }
    }
  }
  if ( out2On == 0) {
    if ( timerOut1On != 0){
      if ( timerOut2On <= millis() - timerOut1On ) {
        digitalWrite(Out2, HIGH);
        out2On = 1;
        Serial.println("2*");
      }
  }
  }
  if ( out2On == 1) {
      if ( timerOut2Off +  timerOut2On <= millis() - timerOut1On ) {
        digitalWrite(Out2, LOW);
        out2On = 0;
        Serial.println("2*-");
        timerOut1On = 0;
      }
    
  }
}
// сделаем функцию для удобства
int getDist() {
  // импульс 10 мкс
  for (int i = 0; i <= 3;) {
    timer3 = millis();
    if ( timer3 >= timer4 + x2Delay) {
      timer4 = timer3;
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
        i++;
      }
    }
    dist = ((dist3 + dist2 + dist1) / 3);
  }
  return (dist);
}
