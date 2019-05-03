 /*
 *  Project: Bluetooth RC Controller
 *  Hardware: Arduino, L298 Module, HC06 bluetooth
 *  Bluetooth app: https://github.com/Tuanhung4797/Bluetooth-RC-controller/blob/master/Android%20app/Bluetooth_RC_Controller.apk
 *  Bluetooth app is built on MIT App Inventor platform
 *  Author: Tuan Hung
 *  Github: https://github.com/Tuanhung4797
 */

#include <SoftwareSerial.h>

//#define DEBUG 1
#define DEBUG_DATA 1

#define BT_Tx_Pin 6 // The pins of HC06
#define BT_Rx_Pin 8

#define IN1 2 // The pins of L298 module
#define IN2 3
#define IN3 5
#define IN4 4
#define ENA 9
#define ENB 10

SoftwareSerial BT(BT_Rx_Pin,BT_Tx_Pin);
char data; //Data read from bluetooth
char Direction; //direction moving
char char_Speed; 
int  Speed = 100;

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
  if(BT.available() > 0){
      data = BT.read();
      if((data != 'S') && (data != 'F') && (data != 'B') && (data != 'R') && (data != 'L') && (data != 'G') && (data != 'I') && (data != 'H') && (data != 'J')){ 
      // If data is not the directions => Data is speed
        char_Speed = data;
        #ifdef DEBUG_DATA
          Serial.println(String("Speed: ") + char_Speed);
        #endif
      }
      else{
        Direction = data;
        #ifdef DEBUG_DATA
          Serial.println(String("Data: ") + Direction);
        #endif
      }
      //////// convert speed ////////
      switch (char_Speed){
        case '0':
          Speed = 10;
          break;
        case '1':
          Speed = 20;
          break;  
        case '2':
          Speed = 30;
          break; 
        case '3':
          Speed = 40;
          break;
        case '4':
          Speed = 50;
          break;  
        case '5':
          Speed = 60;
          break;
        case '6':
          Speed = 70;
          break;
        case '7':
          Speed = 80;
          break;
        case '8':
          Speed = 90;
          break;
        case '9':
          Speed = 100;
          break;
      }
   }
   Moving(Direction,Speed);
}

void Moving(char MoveID, int V)
{
  int Speeds = map(V,0,100,0,255);
  switch(MoveID){
    case 'S':
      stop();
      //Serial.println("STOP");
      break;
    case 'F':
      forward(Speeds,Speeds);
      #ifdef DEBUG
        Serial.println(String("Forward - Speed: ") + Speeds);
      #endif
      break;
    case 'B':
      backward(Speeds,Speeds);
      #ifdef DEBUG
        Serial.println(String("Backward - Speed: ") + Speeds);
      #endif
      break;
    case 'L':
      turnLeft(Speeds);
      #ifdef DEBUG
        Serial.println(String("Turn Left - Speed: ") + Speeds);
      #endif
      break;
    case 'R':
      turnRight(Speeds);
      #ifdef DEBUG
        Serial.println(String("Turn Right - Speed: ") + Speeds);
      #endif
      break;
    case 'G':
      forward(Speeds,Speeds/2);
      #ifdef DEBUG
        Serial.println(String("Forward Left - Speed: ") + Speeds);
      #endif
      break;
    case 'I':
      forward(Speeds/2,Speeds);
      #ifdef DEBUG
        Serial.println(String("Forward Right - Speed: ") + Speeds);
      #endif
      break;
    case 'H':
      backward(Speeds,Speeds/2);
      #ifdef DEBUG
        Serial.println(String("Backward Left - Speed: ") + Speeds);
      #endif
      break;
    case 'J':
      backward(Speeds/2,Speeds);
      #ifdef DEBUG
        Serial.println(String("Backward Right - Speed: ") + Speeds);
      #endif
      break;
  }
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
void forward(int speedR, int speedL)
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA,speedR);
  analogWrite(ENB,speedL);
}
void backward(int speedR, int speedL)
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA,speedR);
  analogWrite(ENB,speedL);
}
void turnLeft(int speeds)
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA,speeds);
  analogWrite(ENB,0);
}
void turnRight(int speeds)
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA,0);
  analogWrite(ENB,speeds);
}
