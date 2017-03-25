// MUSTACHE TEMP SUPPORT FUNCTIONS //

// INCLUSIONS //

#include <Servo.h>

// DEFINITIONS //

#define SERVO_PIN_MUSTACHE 8     // First servo pin of the rigth mustache

// VARIABLES //

// Others
int i, j = 0;                                 // Counters to menage the loops
int mustacheCurrentAction = 0;                // Current action inside an animation
int mustacheCurrentAnim = 1;                  // Current animation

// Servo mototors
Servo motor;

// Sensors

// Positions
int motorPosition = 105;                       // Default position of the servo
int positionDelta = 0;                        // Delta to be applied on the default position

// Millise
long mustacheMillis;                          // Tells how often update position
long mustacheChangeAnim;                      // Tells when change animation

// PROGRAM //

/* void setup()
  {
  setupMustache();
  }

  void loop()
  {
  menageMustache();

  if (millis() - mustacheChangeAnim > 2000) {
    setMustacheAnimin(random(1, 4));
    mustacheChangeAnim = millis();
  }
  } */

// FUNCTIONS //

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
  mustacheCurrentAnim = i;
  motor.write(motorPosition);
}

// Menage the movement (call this in the loop)
void menageMustache() {
  switch (mustacheCurrentAnim) {
    case 1:
      mustacheTalk();
      break;
    case 2:
      mustacheUpDown();
      break;
    case 3:
      mustacheTips();
      break;
    default:
      mustacheTips();
      break;
  }
}

// ANIMATIOS //

// Move the mustache up and down
void mustacheUpDown() {
  if (millis() - mustacheMillis > 5) {
    // Compute position
    switch (mustacheCurrentAction) {
      case 1:
        positionDelta++;
        // If I've finished with this action perform the next one
        if (positionDelta > 20)
          mustacheCurrentAction = 2;
        break;
      case 2:
        positionDelta--;
        // If I've finished with this action perform the next one
        if (positionDelta < -20)
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

// Move the mustache like the robot is talking
void mustacheTalk() {
  if (millis() - mustacheMillis > 5) {
    // Compute position
    switch (mustacheCurrentAction) {
      case 1:
        positionDelta++;
        // If I've finished with this action perform the next one
        if (positionDelta > 7)
          mustacheCurrentAction = 2;
        break;
      case 2:
        positionDelta--;
        // If I've finished with this action perform the next one
        if (positionDelta < -7)
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

// Move the tips of the mustache
void mustacheTips() {
  if (millis() - mustacheMillis > 5) {
    // Compute position
    switch (mustacheCurrentAction) {
      case 1:
        positionDelta++;
        // If I've finished with this action perform the next one
        if (positionDelta > 40)
          mustacheCurrentAction = 2;
        break;
      case 2:
        positionDelta--;
        // If I've finished with this action perform the next one
        if (positionDelta < -40)
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
