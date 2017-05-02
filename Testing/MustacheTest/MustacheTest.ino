// MUSTACHE TEMP SUPPORT FUNCTIONS //

// INCLUSIONS //

#include <Servo.h>

// DEFINITIONS //

#define SERVO_PIN_MUSTACHE 3     // First servo pin of the rigth mustache

// VARIABLES //

// Miscellaneous
int i, j = 0;                                 // Counters to menage the loops
int mustacheCurrentAction = 0;                // Current action inside an animation
int mustacheCurrentAnim = 2;                  // Current animation

// Servo mototors
Servo motor;

// Sensors

// Positions
int motorPosition = 90;                       // Default position of the servo (+25)
int motorPositionHigh = 130;                  // High position of the servo (+25)
int motorPositionLow = 85;                    // Low position of the servo (+25)
int positionDelta = 0;                        // Delta to be applied on the default position

// Millise
long mustacheMillis;                          // Tells how often update position
long mustacheChangeAnim;

// CODE //

void setup() {
  // put your setup code here, to run once:
  setupMustache();
}

void loop() {
  // put your main code here, to run repeatedly:
  menageMustache();

  if (millis() - mustacheChangeAnim > 5000) {
    setMustacheAnimin(2);
    mustacheChangeAnim = millis();
  }
}

// SUPPORT FUNCTIONS //

// Setup the movement (call this in the setup)
void setupMustache()
{
  motor.attach(SERVO_PIN_MUSTACHE);

  // Set initial position
  motor.write(motorPosition);

  // Initialize the millis
  mustacheMillis = millis();
}

// Sets the animation and resets the position
void setMustacheAnimin(int i) {
  if (i != mustacheCurrentAnim ) {
    mustacheCurrentAnim = i;
    mustacheCurrentAction = 1;
    motor.write(motorPosition);
  }
}

// Menage the movement (call this in the loop)
void menageMustache() {
  switch (mustacheCurrentAnim) {
    case 0:
      // Still, just do nothing at all
      break;
    case 1:
      // Talk
      mustacheTalk();
      break;
    case 2:
      // Scared
      mustacheScared();
      break;
    case 3:
      // Tips
      mustacheTips();
      break;
    case 4:
      // Shout
      mustacheShout();
      break;
    case 5:
      // Whisper
      mustacheWhisper();
      break;
    default:
      mustacheTalk();
      break;
  }
}

// ANIMATIONS //

// Up and down, centered, normal width
void mustacheTalk() {
  if (millis() - mustacheMillis > 10) {
    // Compute position
    switch (mustacheCurrentAction) {
      case 1:
        positionDelta++;
        // If I've finished with this action perform the next one
        if (positionDelta > 14)
          mustacheCurrentAction = 2;
        break;
      case 2:
        positionDelta--;
        // If I've finished with this action perform the next one
        if (positionDelta < -14)
          mustacheCurrentAction = 3;
        break;
      case 3:
        positionDelta++;
        // If I've finished with this action perform the next one
        if (positionDelta > 0)
          mustacheCurrentAction = 1;
        break;
      default:
        mustacheCurrentAction = 1;
        break;
    }
    // Set position
    motor.write(motorPosition + positionDelta);
    // Reset time
    mustacheMillis = millis();
  }
}


// Up and down, centered higher, higher width
void mustacheShout() {
  if (millis() - mustacheMillis > 10) {
    // Compute position
    switch (mustacheCurrentAction) {
      case 1:
        positionDelta++;
        // If I've finished with this action perform the next one
        if (positionDelta > 9)
          mustacheCurrentAction = 2;
        break;
      case 2:
        positionDelta--;
        // If I've finished with this action perform the next one
        if (positionDelta < -9)
          mustacheCurrentAction = 3;
        break;
      case 3:
        positionDelta++;
        // If I've finished with this action perform the next one
        if (positionDelta > 0)
          mustacheCurrentAction = 1;
        break;
      default:
        mustacheCurrentAction = 1;
        break;
    }
    // Set position
    motor.write(motorPositionHigh + positionDelta - 40);
    // Reset time
    mustacheMillis = millis();
  }
}

// Up and down, centered lower, smaller width
void mustacheWhisper() {
  if (millis() - mustacheMillis > 25) {
    // Compute position
    switch (mustacheCurrentAction) {
      case 1:
        positionDelta++;
        // If I've finished with this action perform the next one
        if (positionDelta > 12)
          mustacheCurrentAction = 2;
        break;
      case 2:
        positionDelta--;
        // If I've finished with this action perform the next one
        if (positionDelta < -12)
          mustacheCurrentAction = 3;
        break;
      case 3:
        positionDelta++;
        // If I've finished with this action perform the next one
        if (positionDelta > 0)
          mustacheCurrentAction = 1;
        break;
      default:
        mustacheCurrentAction = 1;
        break;
    }
    // Set position
    motor.write(motorPositionLow + positionDelta + 15);
    // Reset time
    mustacheMillis = millis();
  }
}

// Up and down, centered lower, mini width
void mustacheScared() {
  if (millis() - mustacheMillis > 2) {
    // Compute position
    switch (mustacheCurrentAction) {
      case 1:
        positionDelta++;
        // If I've finished with this action perform the next one
        if (positionDelta > 12)
          mustacheCurrentAction = 2;
        break;
      case 2:
        positionDelta--;
        // If I've finished with this action perform the next one
        if (positionDelta < -12)
          mustacheCurrentAction = 3;
        break;
      case 3:
        positionDelta++;
        // If I've finished with this action perform the next one
        if (positionDelta > 0)
          mustacheCurrentAction = 1;
        break;
      default:
        mustacheCurrentAction = 1;
        break;
    }
    // Set position
    motor.write(motorPosition + positionDelta);
    // Reset time
    mustacheMillis = millis();
  }
}

// Up and down, centered lower, mini width
void mustacheTips() {
  if (millis() - mustacheMillis > 10) {
    // Compute position
    switch (mustacheCurrentAction) {
      case 1:
        positionDelta++;
        // If I've finished with this action perform the next one
        if (positionDelta > 25)
          mustacheCurrentAction = 2;
        break;
      case 2:
        positionDelta--;
        // If I've finished with this action perform the next one
        if (positionDelta < -25)
          mustacheCurrentAction = 3;
        break;
      case 3:
        positionDelta++;
        // If I've finished with this action perform the next one
        if (positionDelta > 0)
          mustacheCurrentAction = 1;
        break;
      default:
        mustacheCurrentAction = 1;
        break;
    }
    // Set position
    motor.write(motorPosition + positionDelta + 15);
    // Reset time
    mustacheMillis = millis();
  }
}
