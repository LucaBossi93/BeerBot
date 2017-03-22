// MOVEMENT TEST
//version 1.1

// importing libraries
#include <NewPing.h>

// definitions
#define TRIGGER  12
#define ECHO     11  
#define MAX_DISTANCE 150 // maximum distance we want to ping for (in centimeters)
#define RIGHT 0
#define LEFT 1
#define DIR_FORWARD 1  //clockwise
#define DIR_BACKWARD 0  //counter-clockwise
#define ROTATION_TIME 1000
#define MOVEMENT_TIME 1300
#define INTERACTION_TIME 500

// stannby pin
int STBY = 0; //standby

// MOTOR A
// connected between A01 and A02
int PWMA = 3; //Speed control 
int AIN1 = 1; //Direction
int AIN2 = 2; //Direction

// MOTOR B
// connected between B01 and B02
int PWMB = 6; //Speed control
int BIN1 = 4; //Direction
int BIN2 = 5; //Direction

// Variables
int sp = 150; //default speed
long starting_time_rotation;
long starting_time_movement;
long starting_time_sonar;
long starting_time_interaction;
int ping_counter = 0;
int awake_increment = 3;
int distance;
boolean movement = 0;


// Constructors
NewPing sonar(TRIGGER, ECHO, MAX_DISTANCE);

void setup() {
  // setting I/O 
  pinMode(STBY, OUTPUT);

  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);

  pinMode(PWMB, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);

  // initializing the serial port
  Serial.begin(9600);

  randomSeed(1234);

  //starting_time_rotation = millis();
  //starting_time_movement = millis();
  starting_time_sonar = millis();

}

void loop() {
  sonarDetection();
  
  // Print infos
  Serial.print("counter: ");
  Serial.print(ping_counter);
  Serial.print(" and distance: ");
  Serial.println(distance);
}

void sonarDetection(){
  
  boolean flag = 0;   //detection of people while rotating
  
  detect();

  if(ping_counter >= 10){    // the robot starts to look around in order to detect someone
    Serial.println("I have to rotate!");

    // random number generator to randomize the direction
    if(random(2)){
      rotate(sp, RIGHT); // turn left 
    }
    else{
      rotate(sp, RIGHT); //turn right (TODO: change the direction when the working shield is mounted)
    }

    
    starting_time_rotation = millis();
    while(millis() - starting_time_rotation < ROTATION_TIME && !flag){
      detect();
      if (distance > 5){
        // if it detects someone it stops rotating and tries to approach the person
        Serial.print("distance while rotating: ");
        Serial.println(distance);
        stopRobot();
        moveForward(sp);
        sayHi();  //TODO
        flag = 1;  // breaks the loop
      }
    }
    
    // after a certain time interval it stops anyway
    if(!flag){
      stopRobot();
    }

    ping_counter = 0;
        
  }
  else if(ping_counter <= 0 ){
    // it has detected someone in front of him
    ping_counter = 0; 
    if(!movement){
      moveForward(sp); // approaching the person
      sayHi(); //TODO
      starting_time_interaction = millis();
      while(millis - starting_time_interaction < INTERACTION_TIME){
        detect();
      }
    }
  }
}

void detect(){

  delay(100);
  distance = sonar.ping_cm();

  if (distance > 5) {
    // Read distance from sensor and send to serial
    // Decrese the idle counter
    ping_counter = ping_counter - awake_increment;
  } else {
    ping_counter++;
    movement = 0;
  }
}

// rotation in order to detect someone
void rotate(int vel, boolean dir){
  if(dir == LEFT){

    Serial.println("I'm rotating left");
    moveMotor(1, vel, DIR_BACKWARD);
    moveMotor(0, vel, DIR_FORWARD);
  }
  else{

    Serial.println("I'm rotating right");
    moveMotor(0, vel, DIR_BACKWARD);
    moveMotor(1, vel, DIR_FORWARD);
  }
}

void sayHi(){
  Serial.println("Hey man!");
}

void stopRobot(){
  Serial.println("STOP!");
  //digitalWrite(STBY, LOW);
  moveMotor(1,0,0);
  moveMotor(0,0,0);
  delay(2000);
}

void moveForward(int vel){
  moveMotor(0, vel, DIR_FORWARD);
  moveMotor(1, vel, DIR_FORWARD);
  starting_time_movement = millis();
  while(millis() - starting_time_movement < MOVEMENT_TIME){
    Serial.println("I'm moving forward");
  }
  stopRobot();
  movement = 1;
}

void moveMotor(int motor, int sp, boolean dir){
//Move specific motor at speed and direction
//motor: 0 for B 1 for A
//speed: 0 is off, and 255 is full speed
//direction: 0 clockwise, 1 counter-clockwise

  digitalWrite(STBY, HIGH); //disable standby

  boolean inPin1 = LOW;
  boolean inPin2 = HIGH;

  if(dir == 1){
    inPin1 = HIGH;
    inPin2 = LOW;
  }

  if(motor == 1){
    digitalWrite(AIN1, inPin1);
    digitalWrite(AIN2, inPin2);
    analogWrite(PWMA, sp);
  }else{
    digitalWrite(BIN1, inPin1);
    digitalWrite(BIN2, inPin2);
    analogWrite(PWMB, sp);
  }
}


