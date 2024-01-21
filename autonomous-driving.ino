
const int MAF = 3;//define pin 2 for Motor A forward
const int MAR = 5;//define pin 3 for Motor B Backwards

const int MBF = 6;//define pin 10 for Motor B forward
const int MBR = 11;//define pin 11 for Motor B Back

float lrFraction = 0.83f;

// IR variables
bool triedAdjusting = false;

int frontIRPin = A0;
int backIRPin = A1;

int frontIRVal;
int backIRVal;

bool frontIR;
bool backIR;


int thresholdIR = 300;

void setup() {
  // put your setup code here, to run once:
  pinMode(MAF,OUTPUT);// define pin as output
  pinMode(MAR,OUTPUT);
  
  pinMode(MBF,OUTPUT);
  pinMode(MBR,OUTPUT);

  Serial.begin(9600);
  Serial.print("Front");
  Serial.print(",");
  Serial.println("Back");
  
  delay(3000);

  move_forward(150);
}

void loop() {
  // put your main code here, to run repeatedly:

  frontIRVal = analogRead(frontIRPin);
  backIRVal = analogRead(backIRPin);

  if (frontIRVal > thresholdIR) { frontIR = true; } else { frontIR = false; }
  if (backIRVal > thresholdIR) { backIR = true; } else { backIR = false; }

  Serial.print(frontIR);
  Serial.print(",");
  Serial.println(backIR);

  if (frontIR) { 
    Serial.println("STOP");
    move_stop(); 
    move_back(150);

    // turn 90
  }
  if (backIR) {
    Serial.println("STOP");
    move_stop();
    move_forward(150);
  }

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
