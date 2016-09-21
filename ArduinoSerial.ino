#include <Servo.h>
#include <math.h>

Servo servo, servo2, servo3, servo4;

const byte numLEDs = 3;
//byte ledPin[numLEDs] = {11, 12, 13};

const byte buffSize = 40;
char inputBuffer[buffSize];
const char startMarker = '<';
const char endMarker = '>';
byte bytesRecvd = 0;
boolean readInProgress = false;
boolean newDataFromPC = false;

char messageFromPC[buffSize] = {0};

double servoSpeed = 0.0;
double servoSpeed2 = 0.0;
double servoSpeed3 = 0.0;
double servoSpeed4 = 0.0;

int joystick = -1;

unsigned long curMillis;

unsigned long prevReplyToPCmillis = 0;
unsigned long replyToPCinterval = 1000;

//=============

void setup() {
  Serial.begin(9600);
  
  // initialize the servos
  servo.attach(10);
  //servo2.attach(11);
  servo3.attach(11);
  //servo4.attach(13);
  
    // tell the PC we are ready
  Serial.println("<Arduino is ready>");
}

//=============

void loop() {
  curMillis = millis();
  getDataFromPC();
  updateServos();
  replyToPC();
}

//=============

void getDataFromPC() {

    // receive data from PC and save it into inputBuffer
    
  if(Serial.available() > 0) {

    char x = Serial.read();

      // the order of these IF clauses is significant
      
    if (x == endMarker) {
      readInProgress = false;
      newDataFromPC = true;
      inputBuffer[bytesRecvd] = 0;
      parseData();
    }
    
    if(readInProgress) {
      inputBuffer[bytesRecvd] = x;
      bytesRecvd ++;
      if (bytesRecvd == buffSize) {
        bytesRecvd = buffSize - 1;
      }
    }

    if (x == startMarker) { 
      bytesRecvd = 0; 
      readInProgress = true;
    }
  }
}

//=============
 
void parseData() {

    // split the data into its parts
    
  char * strtokIndx; // this is used by strtok() as an index

  /*strtokIndx = strtok(inputBuffer, ",");
  joystick = atoi(strtokIndx); //gets which joystick it is */
  
  strtokIndx = strtok(inputBuffer,",");      // get the first part - the string   CHANGED FROM: strtok(inputBuffer, ",")
  servoSpeed = atof(strtokIndx);
  servoSpeed *= 100.0;
  if(servoSpeed >= 3)
    servoSpeed = fscale(-2, 102, 90, 20, servoSpeed, -2);
  else if(servoSpeed <= -3)
    servoSpeed = fscale(-2, 102, 90, 160, abs(servoSpeed), -2);
  else
    servoSpeed = 90;
  
  strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
  servoSpeed2 = atof(strtokIndx);     // convert this part to an integer
  servoSpeed2 *= 100.0;
  if(servoSpeed2 >= 3)
    servoSpeed2 = fscale(-2, 102, 90, 20, servoSpeed2, -2);
    //servoSpeed2 = map(servoSpeed2, 0, 100, 90, 70);
  else if(servoSpeed2 <= -3)
    servoSpeed2 = fscale(-2, 102, 90, 160, abs(servoSpeed2), -2);
    //servoSpeed2 = map(servoSpeed2, 0, -100, 90, 110);
  else
    servoSpeed2 = 90;
  
  strtokIndx = strtok(NULL, ","); 
  servoSpeed3 = atof(strtokIndx);
  servoSpeed3 *= 100.0;
  if(servoSpeed3 >= 3)
    servoSpeed3 = fscale(-2, 102, 90, 20, servoSpeed3, -2);
  else if(servoSpeed3 <= -3)
    servoSpeed3 = fscale(-2, 102, 90, 160, abs(servoSpeed3), -2);
  else
    servoSpeed3 = 90;
    
  strtokIndx = strtok(NULL, ","); 
  servoSpeed4 = atof(strtokIndx);
  servoSpeed4 *= 100.0;
  if(servoSpeed4 >= 3)
    servoSpeed4 =  fscale(-2, 102, 90, 20, servoSpeed4, -2);
  else if(servoSpeed4 <= -3)
    servoSpeed4 = fscale(-2, 102, 90, 160, abs(servoSpeed4), -2);
  else
    servoSpeed4 = 90;
}

//=============

void replyToPC() {

  if (newDataFromPC) {
    newDataFromPC = false;
    Serial.print("<Msg ");
    Serial.print(messageFromPC);
    Serial.print(" Servo1 ");
    Serial.print(servoSpeed);
    Serial.print(" Servo2 ");
    Serial.print(servoSpeed2);
    Serial.print(" Servo3 ");
    Serial.print(servoSpeed3);
    Serial.print(" Time ");
    Serial.print(curMillis >> 9); // divide by 512 is approx = half-seconds
    Serial.println(">");
  }
}

//============

void updateServos() {
  updateServo();
  updateServo2();
  updateServo3();
  updateServo4();
}

//=============

void updateServo() {
 // if(joystick == 0) //change joystick num later
    servo.write(servoSpeed);
}

//=============

void updateServo2() {
 // if(joystick == 1) //change joystick num later
    servo2.write(servoSpeed2);
}

//=============

void updateServo3() {
//  if(joystick == 1) //change joystick num later
    servo3.write(servoSpeed3);
}

//=============

void updateServo4() {
//  if(joystick == 1) //change joystick num later
    servo4.write(servoSpeed4);
}

//=============

float fscale( float originalMin, float originalMax, float newBegin, float newEnd, float inputValue, float curve){

  float OriginalRange = 0;
  float NewRange = 0;
  float zeroRefCurVal = 0;
  float normalizedCurVal = 0;
  float rangedValue = 0;
  boolean invFlag = 0;


  // condition curve parameter
  // limit range

  if (curve > 10) curve = 10;
  if (curve < -10) curve = -10;

  curve = (curve * -.1) ; // - invert and scale - this seems more intuitive - postive numbers give more weight to high end on output 
  curve = pow(10, curve); // convert linear scale into lograthimic exponent for other pow function

  /*
   Serial.println(curve * 100, DEC);   // multply by 100 to preserve resolution  
   Serial.println(); 
   */

  // Check for out of range inputValues
  if (inputValue < originalMin) {
    inputValue = originalMin;
  }
  if (inputValue > originalMax) {
    inputValue = originalMax;
  }

  // Zero Refference the values
  OriginalRange = originalMax - originalMin;

  if (newEnd > newBegin){ 
    NewRange = newEnd - newBegin;
  }
  else
  {
    NewRange = newBegin - newEnd; 
    invFlag = 1;
  }
   zeroRefCurVal = inputValue - originalMin;
  normalizedCurVal  =  zeroRefCurVal / OriginalRange;   // normalize to 0 - 1 float

  /*
  Serial.print(OriginalRange, DEC);  
   Serial.print("   ");  
   Serial.print(NewRange, DEC);  
   Serial.print("   ");  
   Serial.println(zeroRefCurVal, DEC);  
   Serial.println();  
   */

  // Check for originalMin > originalMax  - the math for all other cases i.e. negative numbers seems to work out fine 
  if (originalMin > originalMax ) {
    return 0;
  }

  if (invFlag == 0){
    rangedValue =  (pow(normalizedCurVal, curve) * NewRange) + newBegin;

  }
  else     // invert the ranges
  {   
    rangedValue =  newBegin - (pow(normalizedCurVal, curve) * NewRange); 
  }

  return rangedValue;
}


