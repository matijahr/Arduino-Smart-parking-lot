
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <Wire.h>  
#include <LiquidCrystal_I2C.h> 
 
#define SS_PIN 10
#define RST_PIN 9
#define servoPin1 3
#define servoPin2 5

MFRC522 mfrc522(SS_PIN, RST_PIN);   
Servo Entrance_servo;
Servo Exit_servo;
LiquidCrystal_I2C lcd(0x27, 16, 2);


int ExitButton = 7; 
int ParkingSpots = 0;
int CurrentSpots;
int pos = 0;
int pos2 = 180;
int GreenLED = 8;
int RedLED = 9;
void setup() 
{
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();          // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  
  
  //LED and button initialization
  pinMode(ExitButton, INPUT_PULLUP);
  pinMode(GreenLED, OUTPUT);
  pinMode(RedLED, OUTPUT);

  //Servo initialization
  Entrance_servo.attach(servoPin1);
  Entrance_servo.write(pos);
  Exit_servo.attach(servoPin2);
  Exit_servo.write(pos2);

  //LCD initialization
  lcd.init();
  lcd.backlight();
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.print("Available spots: ");

}


//Entrance Ramp----------------------------------------------------------------------------------
void EntranceRampUp(){
  for (pos = 0; pos <= 90; pos += 5) { // goes from 0 degrees to 90 degrees in steps of 5 degree
    Entrance_servo.write(pos);         // tell servo to go to position in variable 'pos'
    delay(15);                         // waits 15ms for the servo to reach the position
  }

  int x;
  unsigned long starttime = millis();   //time since the function started
  unsigned long endtime = starttime;
  while((endtime-starttime) <=15000){
    if(digitalRead(ExitButton)==LOW){
    ExitRampUp();
    }
    x = analogRead(A0);
    digitalWrite(GreenLED,HIGH);
    digitalWrite(RedLED,LOW);
    if(x < 100){
        ParkingSpots++;
        if(ParkingSpots>=4){
          ParkingSpots=4; 
        }
        delay(100);
        break;
    }
    endtime = millis();                 //time since loop started
   }
 EntranceRampDown();
 }

void EntranceRampDown(){
  for (pos = 90; pos >= 0; pos -= 5) { // goes from 90 degrees to 0 degrees
    Entrance_servo.write(pos);         // tell servo to go to position in variable 'pos'
    delay(15);                         // waits 15ms for the servo to reach the position
  }
  digitalWrite(GreenLED,LOW);
}


//Exit Ramp--------------------------------------------------------------------------------------
void ExitRampUp(){
  for (pos2 = 180; pos2 >= 90; pos2 -= 5) { // goes from 180 degrees to 90 degrees in steps of 5 degree
    Exit_servo.write(pos2);                 // tell servo to go to position in variable 'pos'
    delay(15);                              // waits 15ms for the servo to reach the position
  }
  
  int x;
  unsigned long starttime = millis();
  unsigned long endtime = starttime;
  while((endtime-starttime) <=15000){           //while loop will run for 15s, after that ramp is down
    if (mfrc522.PICC_IsNewCardPresent()) {
     RFIDReader();
     }
     
    x = analogRead(A1);
    digitalWrite(GreenLED,HIGH);
    digitalWrite(RedLED,LOW);
    if(x < 120){
      
        ParkingSpots--;
        if(ParkingSpots<0){
           ParkingSpots=0;
        }    
        delay(100);      
        break;
    }
    endtime = millis();
   }
   ExitRampDown(); 
 }

void ExitRampDown(){
  for (pos2 = 90; pos2 <= 180; pos2 += 5) {  // goes from 90 degrees to 180 degrees
    Exit_servo.write(pos2);                  // tell servo to go to position in variable 'pos'
    delay(15);                               // waits 15ms for the servo to reach the position
  }
  digitalWrite(GreenLED,LOW);
}


//Closing / Opening park-------------------------------------------------------------------------
void ClosePark(){
  while(0==CurrentSpots){
     Entrance_servo.write(0);
     lcd.setCursor(0,0);
     lcd.print("Park is full");
     
     if(digitalRead(ExitButton)==LOW){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Available spots: ");
      ExitRampUp();
      break;
      }
   }
 
}

void EmptyPark(){
  while(4==CurrentSpots){
     Exit_servo.write(180);
     lcd.setCursor(0,0);
     lcd.print("Park is empty");

     if (  mfrc522.PICC_IsNewCardPresent()) {
     RFIDReader();
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.print("Available spots: "); 
     break;
     }
  
   }
}


//RFID reader function-------------------------------------------------------------------------  
void RFIDReader(){

 /*if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }*/
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  
  content.toUpperCase();
  if (content.substring(1) == "70 9D 0E A3")
  {
    delay(500);
    EntranceRampUp();
    
  }
  }

  
//---------------------------------------------------------------------------------------------



void loop() 
{
  digitalWrite(RedLED,HIGH);
  
  CurrentSpots=4-ParkingSpots;
  switch(CurrentSpots){

    case 4: 
    lcd.clear();
    EmptyPark();
    break;
    
    case 3: 
    lcd.setCursor(0,1);
    lcd.print(String(CurrentSpots));
    break;
    
    case 2: 
    lcd.setCursor(0,1);
    lcd.print(String(CurrentSpots));
    break;
    
    case 1: 
    lcd.setCursor(0,1);
    lcd.print(String(CurrentSpots));
    break;

    case 0: 
    lcd.clear();
    ClosePark();
    break;
    
    default: 
    
    break;
  }
//------------------------------------------------------

  if(digitalRead(ExitButton)==LOW){
  ExitRampUp();
}

//------------------------------------------------------
  // Look for new cards
if (  mfrc522.PICC_IsNewCardPresent()) {
     RFIDReader();
     } 
} 
