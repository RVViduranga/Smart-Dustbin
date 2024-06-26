#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>
 
#define echoPin2 4
#define trigPin2 5
#define buzzer 6
#define echoPin 7
#define trigPin 8
#define LEDPin 13
 
LiquidCrystal_PCF8574 lcd(0x38); 
 
Servo myservo;
 
int maximumRange = 30;
int minimumRange = 00;
unsigned long mulai, selesai, over, duration, distance, duration2, distance2;
float detik;
boolean a=0;
 
void setup() {
    Serial.begin (9600);
    lcd.begin(16,2);
    lcd.setBacklight(255);
  
    lcd.setCursor(0,0);
    lcd.print(" Smart Dustbin");
    lcd.setCursor(0,1);
    lcd.print("================");
    delay(2000);
    lcd.clear();
    
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(LEDPin, OUTPUT);
    pinMode(buzzer, OUTPUT);
   
    pinMode(trigPin2, OUTPUT);
    pinMode(echoPin2, INPUT);
    digitalWrite(LEDPin, LOW);
    
    myservo.attach(9);
    mulai=0;
}
 
void tim(){
      over = (mulai-selesai) % 3600000;
      detik = int(over / 1000);
}
 
void cek_jarak(){
    digitalWrite(trigPin, LOW); delayMicroseconds(2);
    digitalWrite(trigPin, HIGH); delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
     

    distance = duration/58.2;
 
    digitalWrite(trigPin2, LOW); delayMicroseconds(2);
    digitalWrite(trigPin2, HIGH); delayMicroseconds(10);
    digitalWrite(trigPin2, LOW);
    duration2 = pulseIn(echoPin2, HIGH);
     

    distance2 = duration2/58.2;
}
 
void cek_kondisi(){
      lcd.clear();
        while(1){  
         cek_jarak(); 
      digitalWrite(buzzer, HIGH);
      lcd.setCursor(0,0);
      lcd.print(" Smart Dustbin ");
      lcd.setCursor(00,1);
      lcd.print("   - Full -     ");
    if(distance2 > 10){break; lcd.clear();
      }
}}
 
void loop() {
    cek_jarak();
     
    if (distance < 30 && a==0) {
      lcd.clear(); a=1;
       while(1){
        cek_jarak();
        lcd.setCursor(00,00);
        lcd.print("Value:");
        lcd.print(distance);
          myservo.write(180); //90 
          mulai = millis();
          tim();
          lcd.setCursor(00,1);
          lcd.print("Timer:");
          if (detik <= 9) {lcd.print("0");}
          lcd.print(detik,0);
          if(detik==5) {digitalWrite(LEDPin, HIGH); }
          if ((detik>5) && distance>30) { break;  lcd.clear();}
          if ((detik>=3) && distance>30) { break;  lcd.clear();}
          if ((detik>=8) && distance<30 && a==1) { break;  lcd.clear();}
        }
    }
 
    if (distance < 30 && a==1) {
      lcd.clear();
    while(1){
     cek_jarak();
      
        myservo.write(90); //0 
    
        lcd.setCursor(00,00);
        lcd.print("1:");
        lcd.print(distance);
        lcd.setCursor(9,00);
        lcd.print("2:");
        lcd.print(distance2);
        lcd.setCursor(0,1);
        lcd.print("- Sensor Value -");
    
        cek_kondisi();
        if (distance>30) { break;  lcd.clear();}
        } 
    }
        
    if (distance > 30) {
      selesai= millis();
      detik = 0;
    digitalWrite(LEDPin, LOW);
    myservo.write(90); //0 
    a=0;
 
     lcd.setCursor(00,00);
     lcd.print("1:");
     lcd.print(distance);
     lcd.print("   ");
     lcd.setCursor(9,00);
     lcd.print("2:");
     lcd.print(distance2);
     lcd.print("   ");
     lcd.setCursor(0,1);
     lcd.print("- Sensor Value -");
        }
 
      if(distance2 <= 10){  cek_kondisi();}
    
    delay(50);
}
