#define encoderPinA 2
#define encoderPinB 3

volatile int encoderPos = 0;
float angle = 0.0;

void setup() {
    pinMode(encoderPinA, INPUT);
    pinMode(encoderPinB, INPUT);

    void doEncoderA();
    void doEncoderB();

    attachInterrupt(digitalPinToInterrupt(encoderPinA), doEncoderA, CHANGE); // encoder pin with interrupt on pin 2
    attachInterrupt(digitalPinToInterrupt(encoderPinB), doEncoderB, CHANGE); // encoder pin with interrupt on pin 3

    Serial.begin(9600);
}

void loop() {
    // For positive and negative angles:
    angle = encoderPos * (360.0/2400.0);

    // For only positive angles
//    angle = ((encoderPos + 2400) % 2400) * (360.0/2400.0); // 360 degrees for 2400 pulses
    Serial.print("Angle: ");
    Serial.println(angle);
    delay(500);
 }

void doEncoderA() {
    if (rotation(encoderPinA) > 0) {
        if (abs(encoderPos) <= 2400)
            encoderPos++;
        else 
            encoderPos = 0;
    } else {
        if (abs(encoderPos) <= 2400)
            encoderPos--;
        else
            encoderPos = 0;
    }
}

void doEncoderB() {
    if (rotation(encoderPinB) > 0) {
        if (abs(encoderPos) <= 2400)
            encoderPos++;
        else 
            encoderPos = 0;
    } else {
        if (abs(encoderPos) <= 2400)
            encoderPos--;
        else
            encoderPos = 0;
    }
}

// Positive rotation defined as CCW, negative as CW
int rotation(int changedEncoderPin) {
    if (changedEncoderPin == encoderPinA) {
        if (digitalRead(encoderPinA) == HIGH) {
            if (digitalRead(encoderPinB) == LOW) {
                return -1;
            } else {
                return 1;
            }
        } else {
            if (digitalRead(encoderPinB) == HIGH) {
                return -1;
            } else {
                return 1;
            }
        } 
    } else {
        if (digitalRead(encoderPinB) == HIGH) {
            if (digitalRead(encoderPinA) == HIGH) {
                return -1;
            } else {
                return 1;
            }
        } else {
            if (digitalRead(encoderPinA) == LOW) {
                return -1;
            } else {
                return 1;
            }
        }
    }
}
