// BEERBOT 3.0 //
// TODO: detection of the distance in order to avoid the robot crashes something
// TODO: movement when it doesn't detect anything (now it just rotate on the spot)
// TODO: add another sonar to detect the ground and integrate with the exisisting code
// TODO: implement animantions (mustache and eyebrows) in the function sayHi()
// TODO: integrate miniDF player into sayHi() taking into account animations


// importing libraries
#include <NewPing.h>

// DEFINITIONS
#define TRIGGER_PIN_BORDER 9       // Arduino pin 9 tied to trigger pin on the border ultrasonic sensor
#define ECHO_PIN_BORDER 10          // Arduino pin 10 tied to echo pin on the border ultrasonic sensor
#define TRIGGER_PIN_DETECTION 12    // Arduino pin 12 tied to trigger pin of the detection sonar
#define ECHO_PIN_DETECTION 11       //Arduino pin 11 tied to echo pin of the detection sonar
#define MAX_DISTANCE_BORDER 200    // maximum distance we want to ping for (in centimeters)
#define MAX_DISTANCE_DETECTION 150 // maximum distance we want to ping for (in centimeters)
#define INTERACTION_TIME 5000    // maximum time of interaction with the user 
#define ROTATION_TIME 1000   // maximum time of rotation 

// VARIABLES

// border (Marco)
bool ground_detected;               // Tells if there is ground in front of the robot                 
bool look_for_ground;                // Tells if the robot has to look for ground
bool rotate_left;                   // Tells if the robot has to rotate left or right
bool end_interaction = 0;            // Manage the end of the interaction
int border_distance;                 // Distance acquired by the border sonar
int ground_limit = 10;              // Maximum distance for which the sonar output is considered ground
int is_ground_counter = 0;           // Counter needed to have an effective detection of ground

// detection (Nicola)
int detection_ping_counter = 0;
int awake_increment = 3;
int detection_distance;
boolean moved = 0;  //TODO maybe in the movement file
int sp = 140; // default speed

// variables to manage time and acquisitions
long starting_time_border_sonar;   // when the sonar starts to ping
long start_stop_look_for_ground;    // the robot has detected the ground? (TODO)
long starting_time_rotation;   // when the robot starts to rotate to detect people
long starting_time_turning;   // when the robot starts to rotate to detect ground
long starting_time_movement;   // when the robot starts to move
long starting_time_detection_sonar;     // when the sonar starts to ping
long starting_time_interaction;   // when interactions with users starts

NewPing border_sonar(TRIGGER_PIN_BORDER, ECHO_PIN_BORDER, MAX_DISTANCE_BORDER);
NewPing detection_sonar(TRIGGER_PIN_DETECTION, ECHO_PIN_DETECTION, MAX_DISTANCE_DETECTION);

void setup() {

  // initializing the serial port
  Serial.begin(9600);

  randomSeed(1234); // sets a seed for the random number generator
  
  //border_sonar = setupSonar(1);  // constructor of the border sonar
  //detection_sonar = setupSonar(0);  // constructor of the detection sonar
  setupMovement();    // initialize pins and whatever regarding the movement

  // millis initializations
  starting_time_border_sonar = millis();
  starting_time_detection_sonar = millis();

  ground_detected = false;
  look_for_ground = true;
}

//TODO
void loop() {

  borderDetect(10);
  processBorderDetection();

  moveRobot();
  
  // Print infos
  Serial.print("ground counter: ");
  Serial.println(is_ground_counter);
  Serial.print("detection counter: ");
  Serial.print(detection_ping_counter);
  Serial.print(" and distance: ");
  Serial.println(detection_distance);
}

// Border sonar detection
void borderDetect(int ping_time){
  if (millis() - starting_time_border_sonar > ping_time) {
    border_distance = border_sonar.ping_cm();
    Serial.println(border_distance);
    if (border_distance < ground_limit && border_distance != 0) {
      is_ground_counter++;
    } else {
      is_ground_counter--;
    }
  }
}

//Manage the detection of the border
void processBorderDetection(){
  if (is_ground_counter >= 2) {
    is_ground_counter = 2;
    if (!ground_detected) {
      Serial.print("Ground detected, with value: ");
      Serial.println(border_distance);
      ground_detected = true;
      setLookForGround(false);
    }
  } else if (is_ground_counter <= 0) {
      is_ground_counter = 0;
      stopRobot();
      if (ground_detected) {
        Serial.print("No more ground, with value: ");
        Serial.println(border_distance);
        ground_detected = false;
        if (!look_for_ground){
          setLookForGround(true);
        }
      }
  }
  starting_time_border_sonar = millis();
  delay(10);
}


// Manage the robot looking for ground
void setLookForGround(bool b) {
  if (b) {
    Serial.println("I'm looking for ground");
    look_for_ground = b;
    moveBackward(sp);
    // Get the sense of rotation;
    rotate_left = random(2);
    //moveRobot();
  } else {
    Serial.println("I'm going to stop looking for ground");
    //start_stop_look_for_ground = millis();
    look_for_ground = false;
    moved = 0;
    //moveRobot();
  }
}

// Manage the movement of the robot
void moveRobot(){
  // look for ground if the robot has to
  if(look_for_ground){
    rotate(sp, rotate_left);  // turn left or right with respect to the value of "rotate_left"
    starting_time_turning = millis();
    while(millis() - starting_time_turning < 1000 && look_for_ground){
      Serial.println("I'm rotating looking for ground");
      borderDetect(50);
      if(border_distance < ground_limit && border_distance != 0){
        setLookForGround(false);
      }
      starting_time_border_sonar = millis();
    }   
  }
  else if(!moved){
    Serial.println("Free movement");
    processPeopleDetection();
  }
}

// Manage pings of the sonar
void peopleDetect(){

  //delay(100);
  detection_distance = detection_sonar.ping_cm();
  Serial.print("detection_distance: ");
  Serial.println(detection_distance);
  if (detection_distance > 5) {
    // Read distance from sensor and send to serial
    // Decrese the idle counter
    detection_ping_counter = detection_ping_counter - awake_increment;
    if (detection_ping_counter < 0){
      detection_ping_counter = 0;
    }
  } else {
    detection_ping_counter++;
    if(detection_ping_counter >= 5){
      moved = 0;
    }
  }
}

// This method has to detect people
// TODO: detection of the wall must be implemented
void processPeopleDetection(){
  
  boolean flag = 0;   //detection of people while rotating

  if(!end_interaction){  // at the end of the interaction we don't need to update the counter because sayHi() has already done it
    peopleDetect();
  }
  
  if(detection_ping_counter >= 10){    // the robot starts to look around in order to detect someone
    
    //if(end_interaction){
      //end_interaction = 0;
    //}
    Serial.println("I have to rotate!");
    rotate(sp, random(2));  // turn right or left depending on the random number generator
    
    starting_time_rotation = millis();
    while(millis() - starting_time_rotation < ROTATION_TIME && !flag){
      peopleDetect();
      Serial.print("distance while rotating: ");
      Serial.println(detection_distance);
      if (detection_distance > 5){
        if(!end_interaction){
          // if it detects someone it stops rotating and tries to approach the person
          detection_ping_counter = 0;
          stopRobot();
          moveForward(sp);
          moved = 1;
          // it moves forward for a fixed amount of time
          starting_time_movement = millis();
          while(millis() - starting_time_movement < 1000){
            Serial.println("I'm moving forward");
            delay(50);
            borderDetect(50);
            processBorderDetection();
            if(look_for_ground){
              break;
            }
            else{
              moveRobot();
            }
          }
  
          stopRobot();
          sayHi();  //TODO
          flag = 1;  // breaks the loop
      }
    }
    else{
      end_interaction = 0;
    }
  }
    
    // after a certain time interval it stops anyway
    if(!flag){
      stopRobot();
      end_interaction = 0;
    }

    detection_ping_counter = 0;
        
  }
  else if(detection_ping_counter <= 0 ){
    // it has detected someone in front of him
    detection_ping_counter = 0; 
    if(!moved){
      Serial.println("Approaching the person");
      moveForward(sp); // approaching the person
      moved = 1;
      starting_time_movement = millis();
      while(millis() - starting_time_movement < 1000){
        Serial.println("I'm moving forward");
        delay(50);
        borderDetect(50);
        processBorderDetection();
        if(look_for_ground){
          break;
        }
        else{
          moveRobot();
        }
      }
      stopRobot();
      sayHi(); //TODO
    }
  }
  //TODO: moveAround();
}


// TODO: we have to model and implement the interaction with the user
// TODO: we have to stop rotating somewhere
void sayHi(){
  starting_time_interaction = millis();
  Serial.println("Hey man!");
  while(millis() - starting_time_interaction < INTERACTION_TIME && detection_ping_counter <= 4){
    Serial.println("I'm making friends");
    delay(200);
    peopleDetect();
  }
  if(detection_ping_counter <= 4){
    //rotate(sp, random(2));
    end_interaction = 1;
    detection_ping_counter = 10;
    processPeopleDetection();
  }
  moved = 0;
}


