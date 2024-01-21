
#include <math.h>
#include <ezButton.h>
#include <Servo.h>

const int MAF = 3;//define pin 2 for Motor A forward
const int MAR = 5;//define pin 3 for Motor B Backwards

const int MBF = 6;//define pin 10 for Motor B forward
const int MBR = 11;//define pin 11 for Motor B Back

const int J1x = A0;
const int J1y = A1;
const int J1b = 2;

const int J2x = A2;
const int J2y = A3;
const int J2b = 4;

int J1xValue;
int J1yValue;

int J2xValue;
int J2yValue;

bool J1State = false;
bool prevJ1State = true;
bool prevJ2State = true;

ezButton J1button(J1b);
ezButton J2button(J2b);

bool stickUp = true;

int stickServoPin = 9;
Servo stickServo;

float lrFraction = 0.83f;

// IR variables
bool triedAdjusting = false;

int leftIRPin = A4;
int rightIRPin = A5;

int leftIRVal;
int rightIRVal;

bool leftIR;
bool rightIR;

int leftIRLed = 12;
int rightIRLed = 8;

int thresholdIR = 800;


void setup() {
  pinMode(MAF,OUTPUT);// define pin as output
  pinMode(MAR,OUTPUT);
  
  pinMode(MBF,OUTPUT);
  pinMode(MBR,OUTPUT);

  pinMode(leftIRLed, OUTPUT);
  pinMode(rightIRLed, OUTPUT);

  stickServo.attach(stickServoPin);

  Serial.begin(9600);
  Serial.print("Left");
  Serial.print(",");
  Serial.println("Right");
  
  delay(3000);
}

void loop() {
  J1button.loop();
  J2button.loop();
  //Serial.println(J1button.getState());

  joystick1();
  joystick2();
  showIRLeds();
}

void showIRLeds() {
  leftIRVal = analogRead(leftIRPin);
  rightIRVal = analogRead(rightIRPin);

  //Serial.print(leftIRVal);
  //Serial.print(",");
  //Serial.println(rightIRVal);

  if (leftIRVal > thresholdIR) { leftIR = true; } else { leftIR = false; }
  if (rightIRVal > thresholdIR) { rightIR = true; } else { rightIR = false; }

  // Set the LEDs
  if (!leftIR) { digitalWrite(leftIRLed, HIGH); } else { digitalWrite(leftIRLed, LOW); }
  if (!rightIR) { digitalWrite(rightIRLed, HIGH); } else { digitalWrite(rightIRLed, LOW); }

  //Serial.println(leftIR);
  //Serial.println(rightIR);
}


void joystick1() {
  // Joystick 1
  int state = J1button.getState();
  //Serial.println(state);
  if (state == 0 and state != prevJ1State) {
    Serial.println("Pressed");

    checkRotate();
  }
  prevJ1State = state;

  J1xValue = analogRead(J1y) - 522;
  J1yValue = analogRead(J1x) - 513;

  int magnitude = (int) sqrt(pow(J1xValue, 2) + pow(J1yValue, 2));

  char dir = 'S';

  // Check if in S
  if (magnitude < 20) {
    dir = 'S';
  }
  else {

    // Check if in B
    if (J1yValue > abs(J1xValue)) {
    dir = 'B';
      magnitude = map(J1yValue + 513, 513, 1023, 0, 200);
    }

    // Check if in F
    if (-1*J1yValue > abs(J1xValue)) {
    dir = 'F';
      magnitude = map(J1yValue + 513, 513, 0, 0, 200);
    }

    // check if in R
    if (J1xValue > abs(J1yValue)) {
    dir = 'L';
      magnitude = map(J1xValue + 522, 522, 1023, 0, 200);
    }

    // Check if in L
    if (-1*J1xValue > abs(J1yValue)) {
    dir = 'R';
      magnitude = map(J1xValue + 522, 522, 0, 0, 200);
    }
  }

  //Serial.println(dir);
  //Serial.println(magnitude);
  //Serial.println();

  move(dir, magnitude);
}

void joystick2() {
  // Joystick 2
  int state = J2button.getState();
  //Serial.println(state);

  J2xValue = analogRead(J2y) - 522;
  J2yValue = analogRead(J2x) - 513;

  int magnitude2 = (int) sqrt(pow(J2xValue, 2) + pow(J2yValue, 2));

  char ramForce = 'M';

  // Check if in M
  if (magnitude2 < 60) {
    ramForce = 'M';
  }
  else {

    // Check if in H
    if (J2yValue < 300) {
      ramForce = 'H';
      //magnitude = map(J1yValue + 513, 513, 1023, 0, 200);
    }

    // Check if in L
    if (J2yValue > -300) {
      ramForce = 'L';
      //magnitude = map(J1yValue + 513, 513, 0, 0, 200);
    }

  }

  //Serial.println(ramForce);
  //Serial.println();

  if (state == 0 and state != prevJ2State) {
    Serial.println(ramForce);
    ram(ramForce);
  }
  prevJ2State = state;
}

void move_forward(int speed){
  analogWrite(MAF, (int) speed * lrFraction);
  analogWrite(MAR, 0);
  analogWrite(MBF, speed);
  analogWrite(MBR, 0);
}

void move_back(int speed){
  analogWrite(MAF, 0);
  analogWrite(MAR, (int) speed * lrFraction);
  analogWrite(MBF, 0);
  analogWrite(MBR, speed);
}

void move_left(int speed){
  analogWrite(MAF, 0);
  analogWrite(MAR, (int) speed * lrFraction);
  analogWrite(MBF, speed);
  analogWrite(MBR, 0);
}

void move_right(int speed){
  analogWrite(MAF, (int) speed * lrFraction);
  analogWrite(MAR, 0);
  analogWrite(MBF, 0);
  analogWrite(MBR, speed);
}

void move_stop(){
  analogWrite(MAF, 0);
  analogWrite(MAR, 0);
  analogWrite(MBF, 0);
  analogWrite(MBR, 0);
}

void move(char c, int speed){
  if(c == 'F'){
    move_forward(speed);
  }else if(c == 'B'){
    move_back(speed);
  }else if(c == 'R'){
    move_right(speed);
  }else if(c == 'L'){
    move_left(speed);
  }else{
    move_stop();
  }
}


void checkRotate() {
  if (!stickUp) {
    Serial.println("up");
    for (int i = 0; i < 60; i++) {

      stickServo.write(i);
      delay(15);
    }
  }
  else {
    Serial.println("dwon");
    for (int i = 60; i > 0; i--) {

      stickServo.write(i);
      delay(15);
    }
  }
  stickUp = !stickUp;
  //delay(5000);
}


void ram(char c){
  Serial.println("Starting ram");
  // try aligning the ir sensors
  if (!triedAdjusting) {
    Serial.println("adjusting");
    for (int i = 0; i < 3; i++) {
      leftIRVal = analogRead(leftIRPin);
      rightIRVal = analogRead(rightIRPin);

      if (leftIRVal > thresholdIR) { leftIR = true; } else { leftIR = false; }
      if (rightIRVal > thresholdIR) { rightIR = true; } else { rightIR = false; }

      Serial.println(leftIR);
      Serial.println(rightIR);

      // align the ir
      if (leftIR && !rightIR) { 
        Serial.println("moving left");
        move_left(150);
        delay(150); 
        move_stop();
      }
      if (!leftIR && rightIR) { 
        Serial.println("moving right");
        move_right(150);
        delay(150); 
        move_stop();
      }
    }
  }

  Serial.println("Finished adjusting");
  
  if (!triedAdjusting) {
    triedAdjusting = true;
  }
  else {
    Serial.println("Ramming");
    if(c == 'H'){
      move_forward(255);
      delay(270);
      move_stop();
      delay(100);
      move_back(100);
      delay(100);
      move_stop();
    }else if(c == 'M'){
      move_forward(200);
      delay(370);
      move_stop();
      delay(100);
      move_back(100);
      delay(100);
      move_stop();
    }else{
      move_forward(150);
      delay(480);
      move_stop();
      delay(100);
      move_back(100);
      delay(100);
      move_stop();
    }

    triedAdjusting = false;
  }

}
