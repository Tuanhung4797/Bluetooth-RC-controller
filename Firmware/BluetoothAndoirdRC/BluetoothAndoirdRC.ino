/*
 *  Project: Bluetooth RC Controller
 *  Hardware: Arduino, L298 Module, HC06 bluetooth
 *  Bluetooth app: 
 *  Bluetooth app is built on MIT App Inventor platform
 *  Author: Tuan Hung
 *  Github: https://github.com/Tuanhung4797
 */

#include <SoftwareSerial.h>

#define DEBUG 1

#define READ_BLUETOOTH 0
#define PARSING 1
#define MOVING 2

int State = 0;

#define BT_Tx_Pin 7 // The pins of HC06
#define BT_Rx_Pin 8

#define IN1 2 // The pins of L298 module
#define IN2 3
#define IN3 4
#define IN4 5
#define ENA 9
#define ENB 10

SoftwareSerial BT(BT_Rx_Pin,BT_Tx_Pin);
char data; //Data read from bluetooth
char Direction; //direction moving
int  Speed = 255;
String slider; //To store some string

void setup() {
  pinMode(IN1, OUTPUT); 
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  
  BT.begin(9600);         // HC06 Bluetooth baurate Speed is 9600 bps
  Serial.begin(9600);     //Serial Monitor baurate Speed is 9600;
  Serial.println(" Control robots with bluetooth app ");
  delay(2000);

}

void loop()
{
  switch (State){
    case READ_BLUETOOTH:{
      readData();
      break;
    }
    case PARSING:{
      parseData();
      break;
    }
    case MOVING:{
      Moving(Direction, Speed);
      break;
    }
  }
}

void readData()
{
  while(BT.available()){
    if(BT.available() > 0){
      data = BT.read();
      if((data != 'S') && (data != 'F') && (data != 'B') && (data != 'R') && (data != 'L')){ // If data is not the directions //Data is speed text
        slider += data; //Store the data by creating a phrase
      }
      else{
        Direction = data;
        Serial.println(String("Data: ") + Direction);
      }
    }
  }
  State = PARSING;
}
void parseData()
{
  if(slider.length() > 0){ //If there is data
    Speed = slider.toInt(); //Convert to numerical value
    Speed = map(Speed,0,100,0,255); 
    Serial.println(String("Speed: ") + Speed);
  }
  slider = ""; //Clear buffer
  State = MOVING;
}
void Moving(char MoveID, int Speeds)
{
  switch(MoveID){
    case 'S':
      stop();
      break;
    case 'F':
      forward(Speeds,Speeds);
      break;
    case 'B':
      backward(Speeds,Speeds);
      break;
    case 'L':
      turnLeft(Speeds);
      break;
    case 'R':
      turnRight(Speeds);
      break;
  }
  State = READ_BLUETOOTH;
}
void stop()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA,0);
  analogWrite(ENB,0);
}
void forward(int speedL, int speedR)
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA,speedL);
  analogWrite(ENB,speedR);
}
void backward(int speedL, int speedR)
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA,speedL);
  analogWrite(ENB,speedR);
}
void turnLeft(int speeds)
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA,0);
  analogWrite(ENB,speeds);
}
void turnRight(int speeds)
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA,speeds);
  analogWrite(ENB,0);
}
