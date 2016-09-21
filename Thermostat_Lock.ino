//A thermostat lock to keep people from changing the temperature in my room

#include <Keypad.h>
#include <Servo.h>
#include <SoftwareSerial.h>

const byte rows = 4; //byte stores number of rows
const byte cols = 4; //byte stores number of columns

Servo servo;

char code;

char keys[rows][cols] = {     // the list of buttons
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[rows] = {9, 8, 7, 6};   //the four pins for rows
byte colPins[cols] = {5, 4, 3, 2};  //the four pins for columns
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);

void setup() {
  servo.attach(10);
  servo.write(0);
  keypad.addEventListener(keypadEvent);
  keypad.setHoldTime(5000);
  Serial.begin(9600);
  Serial.println("Type Commands");
}

void loop() {
   char key = keypad.getKey();
   
   if (Serial.available()) {
     code = Serial.read();
   }
   if (code == '9') {
     access();
     Serial.println("The thermostat is now accessible");
   } if (code == '1') {
     lock();
     Serial.println("The thermostat is now locked");
   }
   delay(10);
}

void keypadEvent(KeypadEvent key) {
  switch(keypad.getState()) {
    case HOLD:
      if (key == '9') {
        access();
       }
    case PRESSED:
      if (key == '#') {  //code to close shutter
        lock();
      }
  }
}

void lock() {
  servo.write(0); //for locking the shutter
}

void access() {
  servo.write(90); //to open the shutter
}
