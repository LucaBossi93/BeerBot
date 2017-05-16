//motor A connected between A01 and A02
//motor B connected between B01 and B02

#define MOTOR_PIN_PWMA 6                // Arduino digital pin 38 tied to motor A speed control
#define MOTOR_PIN_AIN2 40               // Arduino digital pin 40 tied to motor A direction
#define MOTOR_PIN_AIN1 42               // Arduino digital pin 42 tied to motor A direction
#define MOTOR_PIN_STBY 44               // Arduino digital pin 44 tied to the motors standby
#define MOTOR_PIN_BIN1 46               // Arduino digital pin 46 tied to motor B direction
#define MOTOR_PIN_BIN2 48               // Arduino digital pin 48 tied to motor B direction
#define MOTOR_PIN_PWMB 7                // Arduino digital pin 50 tied to motor B speed control

void setup() {
  Serial.begin(9600);

  // Set I/O
  pinMode(MOTOR_PIN_STBY, OUTPUT);
  // Set motor A pins
  pinMode(MOTOR_PIN_PWMA, OUTPUT);
  pinMode(MOTOR_PIN_AIN1, OUTPUT);
  pinMode(MOTOR_PIN_AIN2, OUTPUT);
  // Set motor B pins
  pinMode(MOTOR_PIN_PWMB, OUTPUT);
  pinMode(MOTOR_PIN_BIN1, OUTPUT);
  pinMode(MOTOR_PIN_BIN2, OUTPUT);
}

void loop() {
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

void move(int motor, int speed, int direction) {
  //Move specific motor at speed and direction
  //motor: 0 for B 1 for A
  //speed: 0 is off, and 255 is full speed
  //direction: 0 clockwise, 1 counter-clockwise

  digitalWrite(MOTOR_PIN_STBY, HIGH); //disable standby

  boolean inPin1 = LOW;
  boolean inPin2 = HIGH;

  if (direction == 1) {
    inPin1 = HIGH;
    inPin2 = LOW;
  }

  if (motor == 1) {
    digitalWrite(MOTOR_PIN_AIN1, inPin1);
    digitalWrite(MOTOR_PIN_AIN2, inPin2);
    analogWrite(MOTOR_PIN_PWMA, speed);
  } else {
    digitalWrite(MOTOR_PIN_BIN1, inPin1);
    digitalWrite(MOTOR_PIN_BIN2, inPin2);
    analogWrite(MOTOR_PIN_PWMB, speed);
  }
}

void stop() {
  //enable standby
  //digitalWrite(STBY, LOW);
  move(0, 0, 1); //stops motor 0
  move(1, 0, 1); //stops motor 1
}
