int pin_DT = 7;
int pin_CLK = 8;
int pos_encoder = 0;
int Last;
int DT;
boolean left;

void setup() {
  pinMode (pin_DT,INPUT);
  pinMode (pin_CLK,INPUT);
  Last = digitalRead (pin_DT);
  Serial.begin (9600);
}

void loop() {
  DT = digitalRead (pin_DT);
  if (DT != Last) {
    if (digitalRead (pin_CLK) != DT) {
      pos_encoder++;
      left = false; 
    } else {
      left = true;
      pos_encoder--;
    }
    Serial.print ("Napravlenie vrasheniya: ");
    if (!left) {
      Serial.println("NAPRAVO ->");
    } else {
      Serial.println("NALEVO <-");
    }
    Serial.print("Pologenie: ");
    Serial.println(pos_encoder);
    }
    Last = DT;
}
