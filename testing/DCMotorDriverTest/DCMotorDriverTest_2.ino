//motor A connected between A01 and A02
//motor B connected between B01 and B02

int STBY = 0; //standby

//Motor A
int PWMA = 3; //Speed control 
int AIN1 = 1; //Direction
int AIN2 = 2; //Direction

//Motor B
int PWMB = 6; //Speed control
int BIN1 = 4; //Direction
int BIN2 = 5; //Direction

void setup(){
  Serial.begin(9600);
  
  pinMode(STBY, OUTPUT);

  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);

  pinMode(PWMB, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
}

void loop(){
  move(1, 150, 1); //motor 1, full speed, left
  move(0, 150, 1); //motor 2, full speed, left
  //Serial.println("first movement");

  delay(2000); //go for 1 second
  stop(); //stop
  delay(1000); //hold for 250ms until move again

  move(1, 150, 0); //motor 1, half speed, right
  move(0, 150, 0); //motor 2, half speed, right
 // Serial.println("second movement");

  delay(2000);
  stop();
  
  delay(1000);
}

// problems with motor 1

void move(int motor, int speed, int direction){
//Move specific motor at speed and direction
//motor: 0 for B 1 for A
//speed: 0 is off, and 255 is full speed
//direction: 0 clockwise, 1 counter-clockwise

  digitalWrite(STBY, HIGH); //disable standby

  boolean inPin1 = LOW;
  boolean inPin2 = HIGH;

  if(direction == 1){
    inPin1 = HIGH;
    inPin2 = LOW;
  }

  if(motor == 1){
    digitalWrite(AIN1, inPin1);
    digitalWrite(AIN2, inPin2);
    analogWrite(PWMA, speed);
  }else{
    digitalWrite(BIN1, inPin1);
    digitalWrite(BIN2, inPin2);
    analogWrite(PWMB, speed);
  }
}

void stop(){
//enable standby  
  //digitalWrite(STBY, LOW); 
  move(0, 0, 1); //stops motor 0
  move(1, 0, 1); //stops motor 1
}
