#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3f,16,2);  // Устанавливаем дисплей

const int buttonPin = 9; // D9
const int relay = 10;    // D10
int sensorPin = A0;      // A0 select the input pin for the Battery
int sensorValue = 0;     // variable to store the value coming from the sensor
bool buttonState = 0;
float voltage = 4.67;    // Voltage Arduino pin +5v
float resistor = 4.6;    // Balance resistor Omh
const int k = 1025;      // Потаму что так точнее напругу кажет ) 
float mah = 0.0;
long timestart  ;
float minVolt = 3.4;     // Минимальное напряжение разрядки

void ClearScreen (){
   lcd.setCursor(0, 1);
   lcd.print("                         ");
}

void setup()
{
  Serial.begin(9600);
  lcd.init();                     
  lcd.backlight();// Turn on the display backlight
  lcd.print("konservateam.ru");
  delay ( 1000 ) ;
    // set up the LCD's number of columns and rows:
  lcd.begin(16, 2) ;
  // Print a message to the LCD.
  lcd.print("Battery life tester");
  delay ( 1500 ) ;
  pinMode(relay, OUTPUT);
  pinMode(buttonPin, INPUT);
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  digitalWrite(relay, 0); //set the LED-relay off
 
  while (buttonState == LOW ) {
  sensorValue = analogRead(sensorPin); // read the battery
  //  the voltage
  buttonState = digitalRead(buttonPin);
//  if (buttonState == HIGH) {
//    if (bb > 0) { bb=0; } else { bb=1; }
//  }
  lcd.setCursor(0, 1);
  lcd.print((sensorValue*voltage)/k);
  lcd.print(" Volts");
  delay(500);
  } //end get ready loop 
  digitalWrite(relay, 1 ); //set the LED-relay on
  lcd.setCursor(0, 0);
  lcd.print("Testing              ");
  ClearScreen();
  mah = 0.0;
  timestart = millis();
  while ( ((sensorValue*voltage)/k) > minVolt ) {
        lcd.setCursor(0, 1);    
        lcd.print((sensorValue*voltage)/k);
        lcd.print(" V ");
        sensorValue = analogRead(sensorPin); // read the battery
        mah = mah + (((sensorValue*voltage)/k)/resistor)/7.2;
        lcd.print(mah);
        lcd.print(" mAh ");
        delay ( 500 ) ;
        lcd.setCursor(8,0);
        lcd.print((millis() - timestart)/1000); // time since test started*/
     delay(100);
     
  } // end battery drain
    digitalWrite(relay, 0 ); //set the LED-relay off

  // final results
 
   lcd.setCursor(0, 0);
   lcd.print(mah);
   lcd.print(" mAh ");
   lcd.print((millis() - timestart)/1000);
   lcd.print(" S ");
   ClearScreen();  
   buttonState = digitalRead(buttonPin);
  
}
