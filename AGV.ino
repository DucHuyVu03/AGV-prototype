#include <Wire.h>   
#include <Timer.h> 
const int LINE1_PIN = 12; 
const int LINE2_PIN = 11; 
const int LINE3_PIN = 10; 
const int LINE4_PIN = 9; 
const int LINE5_PIN = 8; 

const int TRIG_PIN = 2;
const int ECHO_PIN = 3;

const int IN1_PIN = 7;
const int IN2_PIN = 6;
const int IN3_PIN = 5;
const int IN4_PIN = 4;

const int RESET_PIN = 13;

const int MAX_SPEED = 255;
const int MIN_SPEED = 0;

int CUR_Distance;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //Line tracking setup
  pinMode(LINE1_PIN,INPUT);
  pinMode(LINE2_PIN,INPUT);
  pinMode(LINE3_PIN,INPUT);
  pinMode(LINE4_PIN,INPUT);
  pinMode(LINE5_PIN,INPUT);
  //Ultrasonic setup
  pinMode(TRIG_PIN,OUTPUT);
  pinMode(ECHO_PIN,INPUT);
  //Motors L298 setup
  pinMode(IN1_PIN,OUTPUT); 
  pinMode(IN2_PIN,OUTPUT); 
  pinMode(IN3_PIN,OUTPUT); 
  pinMode(IN4_PIN,OUTPUT); 

  pinMode(RESET_PIN,INPUT); 

  digitalWrite(IN1_PIN,LOW);
  digitalWrite(IN2_PIN,LOW); 
  digitalWrite(IN3_PIN,LOW);
  digitalWrite(IN4_PIN,LOW);
}

void loop() 
{
  // put your main code here, to run repeatedly:
  CUR_Distance = Distance_reader();
  if(CUR_Distance <=2)
  {
    Stop();
  }
  else
  {
    switch(Line_reader())
    {
      case 0:
      Forward(MAX_SPEED);
      case 1:
      LeftTurn(MAX_SPEED);
      case 2:
      RightTurn(MAX_SPEED);
      case 3:
      Stop();
      for(int i=0; i<20000;i++)
      {
        if(digitalRead(RESET_PIN)==1) break;
        else delayMicroseconds(1);
      }

      case 4:
      Stop();
    }
  }
}
int Line_reader()
{
  int Line1 = digitalRead(LINE1_PIN);
  int Line2 = digitalRead(LINE2_PIN);
  int Line3 = digitalRead(LINE3_PIN);
  int Line4 = digitalRead(LINE4_PIN);
  int Line5 = digitalRead(LINE5_PIN);
  if (Line1 == 0 && Line2 ==0 && Line3 !=0 && Line4 ==0 && Line5 ==0) return 0; //Forward
  else if (Line1 != 0 && Line2 !=0 && Line3 !=0 && Line4 ==0 && Line5 ==0) return 1; //Left
  else if (Line1 == 0 && Line2 ==0 && Line3 !=0 && Line4 !=0 && Line5 !=0) return 2; //Right
  else if (Line1 != 0 && Line2 != 0 && Line3 != 0 && Line4 !=0 && Line5 !=0) return 3; //Station Stop
  else 4;
  ; //ERROR
}
int Distance_reader()
{
  unsigned long duration; // biến đo thời gian
  int distance;           // biến lưu khoảng cách
    
  /* Phát xung từ chân trig */
  digitalWrite(TRIG_PIN,0);   // tắt chân trig
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN,1);   // phát xung từ chân trig
  delayMicroseconds(5);   // xung có độ dài 5 microSeconds
  digitalWrite(TRIG_PIN,0);   // tắt chân trig
    
  /* Tính toán thời gian */
  // Đo độ rộng xung HIGH ở chân echo. 
  duration = pulseIn(ECHO_PIN,HIGH);  
  // Tính khoảng cách đến vật.
  return distance = int(duration/2/29.412);
}
void Stop()
{
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, LOW);
  digitalWrite(IN3_PIN, LOW);
  digitalWrite(IN4_PIN, LOW);
}
void Forward(int speed)
{
  speed = constrain(speed, MIN_SPEED, MAX_SPEED);
  analogWrite(IN1_PIN, 255-speed);
  digitalWrite(IN2_PIN, HIGH);
  digitalWrite(IN3_PIN, HIGH);
  analogWrite(IN4_PIN, 255-speed);
}
void LeftTurn(int speed)
{
  speed = constrain(speed, MIN_SPEED, MAX_SPEED);
  analogWrite(IN1_PIN, speed);
  analogWrite(IN3_PIN, speed); 

  digitalWrite(IN2_PIN, LOW);
  digitalWrite(IN4_PIN, LOW);
}
void RightTurn(int speed)
{
  speed = constrain(speed, MIN_SPEED, MAX_SPEED);
  analogWrite(IN2_PIN, speed);
  analogWrite(IN4_PIN, speed); 

  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN3_PIN, LOW);
}
