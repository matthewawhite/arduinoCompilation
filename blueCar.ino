#include <Servo.h>
#include <NewPing.h>
#include <SoftwareSerial.h>

Servo leftServo;
Servo rightServo;
Servo topServo; //mini servo for hc-04

#define TRIGGER_PIN 6
#define ECHO_PIN 7
#define MAX_DISTANCE 100 //max distance hc-04 will display

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

unsigned int time; 
int triggerDistance = 20; //20 cm then it will turn if detected
int distance; //distance from hc-04
int pos; //position of hc-04

char command; //command from serial data to control movement

void setup() {
  leftServo.attach(10); //next few lines instantiating servos
  leftServo.write(92); //92 makes this servo stay still, not convential 90
  rightServo.attach(9);
  rightServo.write(91); //91 makes still servo stay still
  topServo.attach(8);
  topServo.write(90);

  Serial.begin(9600); 
  Serial.println("Type Commands");
}

void loop() {  
  command = '0'; //make sure command is instantiated
  if (Serial.available() > 0) 
  {   
     command = Serial.read();
     
     switch(command) //apply serial data to commands for robot
     {
        case 'w': 
           moveForward();
           delay(100);
           Serial.println("Moving forward");
           Serial.println("--------------");
           break;
        case 'a': 
           turnLeft();
           delay(100);
           Serial.println("Turning left");
           Serial.println("--------------");
           break;
        case 's':
           moveBackward();
           delay(100);
           Serial.println("Moving backward");
           Serial.println("--------------");
           break;
        case 'd':
           turnRight();
           delay(100);
           Serial.println("Turn right");
           Serial.println("--------------");
           break;
        case '1':
           while(true)
           {
              scan();
              autoRun();
           }
           Serial.println("In auto mode");
           Serial.println("--------------");
           break;
        case '0':
           stopMoving();
           delay(100);
        default:
           moveForward();
           delay(100);
           break; 
     }   
  }
}

void autoRun() //run autonomously with hc-04
{
  moveForward();
  for (pos = 50; pos < 140; pos += 5) 
    automatic(pos);
  for (pos = 140; pos > 50; pos -= 5)
    automatic(pos);
  delay(1);
}

void scan() //hc-04 scan area to make sure distance is not less than trigger distance
{
  time = sonar.ping();
  distance = time / US_ROUNDTRIP_CM;
  if (distance == 0) {
    distance = 100;
  }
}

void automatic(int pos) { //part of autoRun() to move hc-04 back and forth to detect obstructions
  topServo.write(pos);
  delay(40);
  scan();
  if (distance < triggerDistance) {
    moveBackward();
    delay(1000);
    if (pos > 90){
      turnRight();
      delay (1000);
      moveForward();
    } else if (pos < 90) {
      turnLeft();
      delay(1000);
      moveForward();
    } else if (pos == 90) {
      turnRight();
      delay(1000);
      moveForward();
    } else {
      moveForward();
    }
  }
}

void moveForward()
{
  leftServo.write(180);
  rightServo.write(0);
}

void moveBackward() 
{
  leftServo.write(0);
  rightServo.write(180);
}

void turnRight() 
{
  leftServo.write(180);
  rightServo.write(180);
}

void turnLeft() 
{
  leftServo.write(0);
  rightServo.write(0);
}

void stopMoving() 
{
  leftServo.write(92);
  rightServo.write(91);
}

