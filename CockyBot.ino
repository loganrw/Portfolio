#include<LiquidCrystal.h>
#include<sensorbar.h>

//Control Variables////////////////
int dirPin1 = 8;                 //
int pwmPin1 = 9;                 //
int dirPin2 = 7;                //
int pwmPin2 = 11;                //
//Motor Control Pins             //
int rightMotor, leftMotor,       //
motorReverse1,motorReverse2;     //
int motorSpeed1;                 //
int motorSpeed2;                 //
int leftMotorPin = 5;            //
int rightMotorPin = 6;           //
//Receiver Vals                  //
int rightValHigh = 1905;         //
int rightValLow = 1100;          //
int rightValRest = 1490;         //
int rightValRestHigh = 1510;     // 
int leftValHigh = 2000;          //
int leftValRest = 1480;          //
int leftValRestHigh = 1495;      //
int leftValLow = 980;            //
//Autonomous Mode Variables      //
int autoModePin = 10;     
int autoVal;
int sensorVal;
const uint8_t SX1509_ADDRESS = 0x3E;//
SensorBar mySensorBar(SX1509_ADDRESS); //
LiquidCrystal lcd(22,23,24,25,26,27); //           
void autoMode();  
bool autoTime = false; 
bool lineRead = false;  
int startTime = 20;
void startUp();
//////////////////////////////////

void setup() {
  pinMode(dirPin1, OUTPUT);
  pinMode(pwmPin1, OUTPUT);
  pinMode(dirPin2, OUTPUT);
  pinMode(pwmPin2, OUTPUT);
  pinMode(rightMotorPin, INPUT);
  pinMode(leftMotorPin, INPUT);
  pinMode(autoModePin, INPUT);
  lcd.begin(16,2);
  digitalWrite(12, LOW);
  mySensorBar.clearBarStrobe();
  mySensorBar.clearInvertBits();
  mySensorBar.begin();

  Serial.begin(9600);
  
  
  

}

void loop() {
  startUp();

  while(!autoTime && startTime == 0){
    delay(100);
    lcd.begin(16,2);
    lcd.setCursor(0,0);
    lcd.print("CockyBot Status:");
    lcd.setCursor(0,1);
    lcd.print("MANUAL MODE");
    digitalWrite(12, LOW);
    rightMotor = pulseIn(rightMotorPin, HIGH, 25000); // Read the pulse width of each channel
    leftMotor = pulseIn(leftMotorPin, HIGH, 25000);
    autoVal = pulseIn(autoModePin, HIGH, 25000);
    sensorVal = mySensorBar.getPosition();
  
    
    //Serial.print("Left Motor:"); 
    //Serial.println(leftMotor);        
  
  
    //Serial.print("Right Motor:");
    //Serial.println(rightMotor);
  
    //Serial.print("Auto Mode:");
    //Serial.println(autoVal);
  
    //Serial.print("Auto Bool: ");
    //Serial.println(autoTime);
    
    motorSpeed1 = map(rightMotor,rightValRest,rightValHigh,0,255);
    motorSpeed2 = map(leftMotor,leftValRest,leftValHigh,0,255 );
    motorReverse1 = map(rightMotor, rightValRest,rightValLow,0,255);
    motorReverse2 = map(leftMotor,leftValRest,leftValLow,0,255);
  
    /////// MOTOR CONTROL ///////
  
    if(rightMotor > rightValRest){
      analogWrite(pwmPin1,motorSpeed1);
      digitalWrite(dirPin1, HIGH);
    }
  
    if(rightMotor < rightValRestHigh && rightMotor > rightValRest ){
      analogWrite(pwmPin1, 0);
    }
  
    if(rightMotor < rightValRest){
      analogWrite(pwmPin1,motorReverse1);
      digitalWrite(dirPin1, LOW);
    }
  
  
      if(leftMotor > leftValRest){
      analogWrite(pwmPin2,motorSpeed2);
      digitalWrite(dirPin2, LOW);
    }
  
    if(rightMotor < leftValRestHigh && rightMotor > leftValRest ){
      analogWrite(pwmPin1, 0);
    }
  
    if(leftMotor < leftValRest){
      analogWrite(pwmPin2,motorReverse2);
      digitalWrite(dirPin2, HIGH);
    }
  
  
     while(autoVal < 1850){
      autoTime = true;
      while(autoTime){
        digitalWrite(12,HIGH);
        autoMode();
      }
     }
     
    delay(100);
  }
}

void startUp(){
  while(startTime > 0){
    lcd.begin(16,2);
    lcd.setCursor(0,0);
    lcd.print("CockyBot Status:");
    lcd.setCursor(0,1);
    lcd.print("Starting.");
    delay(200);
    lcd.print(".");
    delay(200);
    lcd.print(".");
    delay(200);
    lcd.print(".");
    delay(100);
    startTime--;
    Serial.println(startTime);
    analogWrite(pwmPin1, 0);
    analogWrite(pwmPin2, 0);
  }
  
}


void autoMode(){
  if(autoVal > 1500){
    autoTime = false;
  }else{
    autoVal = pulseIn(autoModePin, HIGH, 25000);
    //Serial.print("Auto Mode:");
    //Serial.println(autoVal);
    sensorVal = mySensorBar.getPosition();
    //Serial.println(lineRead);
    //Serial.println(mySensorBar.getDensity());
    lcd.begin(16,2);
    lcd.setCursor(0,0);
    lcd.print("CockyBot Status:");
    lcd.setCursor(0,1);
    lcd.print("AUTO MODE");
    if(lineRead){
      lcd.begin(16,2);
      lcd.setCursor(0,0);
      lcd.print("CockyBot Status:");
      lcd.setCursor(0,1);
      lcd.print("READ LINE");
      delay(100);
      if(sensorVal < 0){
        analogWrite(pwmPin2, 255);
        digitalWrite(dirPin2, LOW);
      }else{analogWrite(pwmPin2,0);}
      if(sensorVal > 0){
        analogWrite(pwmPin1, 255);
        digitalWrite(dirPin1, HIGH);
      }else{analogWrite(pwmPin1, 0);}
      if(sensorVal == 0){
        analogWrite(pwmPin1, 255);
        analogWrite(pwmPin2, 255);
        digitalWrite(dirPin1, HIGH);
        digitalWrite(dirPin2, LOW);
       }
    }else{
      if(!lineRead){
        analogWrite(pwmPin1, 250);
        analogWrite(pwmPin2, 250);
        digitalWrite(dirPin1, HIGH);
        digitalWrite(dirPin2, LOW);
        if(mySensorBar.getDensity() < 8){
          lineRead = true;
        }
      }
    }
  }
}
