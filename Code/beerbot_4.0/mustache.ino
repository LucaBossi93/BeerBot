// MUSTACHE SUPPORT FUNCTIONS //

// INCLUSIONS //

#include <Servo.h>

// DEFINITIONS //

#define SERVO_PIN_MUSTACHE_RIGTH_OUTER 22     // First servo pin of the rigth mustache
#define SERVO_PIN_MUSTACHE_RIGTH_INNER 23     // Second servo pin of the rigth mustache
#define SERVO_PIN_MUSTACHE_LEFT_OUTER 24      // First servo pin of the left mustache
#define SERVO_PIN_MUSTACHE_LEFT_INNER 25      // Second servo pin of the left mustache

// VARIABLES //

// Others
int i, j = 0;                                 // Counters to menage the loops
int mustacheCurrentAction = 0;                // Current action inside an animation
int mustacheCurrentAnim = 1;                  // Current animation

// Servo mototors
Servo rigthInner, rigthOuter, leftInner, leftOuter;

// Sensors

// Positions
int rigthInnerPosition = 80;                  // Default position of the inner rigth servo
int rigthOuterPosition = 80;                  // Default position of the outer rigth servo
int rigthMustachePositionDelta = 0;           // Delta to be applied on the default position
int leftInnerPosition = 80;                   // Default position of the inner left servo
int leftOuterPosition = 80;                   // Default position of the outer left servo
int leftMustachePositionDelta = 0;            // Delta to be applied on the default position

// Millis
long mustacheRigthMillis;                     // Tells how often update position
long mustacheLeftMillis;                      // Tells how often update position
long mustacheChangeAnim;                      // Tells when change animation

// FUNCTIONS //

// Setup the movement (call this in the setup)
void setupMustache()
{
  rigthInner.attach(SERVO_PIN_MUSTACHE_RIGTH_INNER);
  rigthOuter.attach(SERVO_PIN_MUSTACHE_RIGTH_OUTER);

  // Set initial position
  rigthInner.write(rigthInnerPosition);
  rigthOuter.write(rigthOuterPosition);

  // Initialize the millis
  mustacheRigthMillis = millis();
}

// Sets the animation and resets the position
void setMustacheAnimin(int i) {
  mustacheCurrentAnim = i;
  rigthInner.write(rigthInnerPosition);
  rigthOuter.write(rigthOuterPosition);
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
    case 4:
      mustacheWave();
      break;
    default:
      mustacheTips();
      break;
  }
}

// ANIMATIOS //

// Move the mustache up and down
void mustacheUpDown() {
  if (millis() - mustacheRigthMillis > 5) {
    // Compute position
    switch (mustacheCurrentAction) {
      case 1:
        rigthMustachePositionDelta++;
        // If I've finished with this action perform the next one
        if (rigthMustachePositionDelta > 20)
          mustacheCurrentAction = 2;
        break;
      case 2:
        rigthMustachePositionDelta--;
        // If I've finished with this action perform the next one
        if (rigthMustachePositionDelta < -20)
          mustacheCurrentAction = 3;
        break;
      case 3:
        rigthMustachePositionDelta++;
        // If I've finished with this action perform the next one
        if (rigthMustachePositionDelta > 0)
          mustacheCurrentAction = 1;
        break;
      default:
        mustacheCurrentAction = 1;
        break;
    }
    // Set position
    rigthInner.write(rigthInnerPosition + rigthMustachePositionDelta);
    rigthOuter.write(rigthOuterPosition - rigthMustachePositionDelta);
    // Reset time
    mustacheRigthMillis = millis();
  }
}

// Move the mustache like the robot is talking
void mustacheTalk() {
  if (millis() - mustacheRigthMillis > 5) {
    // Compute position
    switch (mustacheCurrentAction) {
      case 1:
        rigthMustachePositionDelta++;
        // If I've finished with this action perform the next one
        if (rigthMustachePositionDelta > 20)
          mustacheCurrentAction = 2;
        break;
      case 2:
        rigthMustachePositionDelta--;
        // If I've finished with this action perform the next one
        if (rigthMustachePositionDelta < -20)
          mustacheCurrentAction = 3;
        break;
      case 3:
        rigthMustachePositionDelta++;
        // If I've finished with this action perform the next one
        if (rigthMustachePositionDelta > 0)
          mustacheCurrentAction = 1;
        break;
      default:
        rigthOuter.write(rigthOuterPosition);
        mustacheCurrentAction = 1;
        break;
    }
    // Set position
    rigthInner.write(rigthInnerPosition + rigthMustachePositionDelta);
    // Reset time
    mustacheRigthMillis = millis();
  }
}

// Move the tips of the mustache
void mustacheTips() {
  if (millis() - mustacheRigthMillis > 5) {
    // Compute position
    switch (mustacheCurrentAction) {
      case 1:
        rigthMustachePositionDelta++;
        // If I've finished with this action perform the next one
        if (rigthMustachePositionDelta > 20)
          mustacheCurrentAction = 2;
        break;
      case 2:
        rigthMustachePositionDelta--;
        // If I've finished with this action perform the next one
        if (rigthMustachePositionDelta < -20)
          mustacheCurrentAction = 3;
        break;
      case 3:
        rigthMustachePositionDelta++;
        // If I've finished with this action perform the next one
        if (rigthMustachePositionDelta > 0)
          mustacheCurrentAction = 1;
        break;
      default:
        rigthInner.write(rigthOuterPosition);
        mustacheCurrentAction = 1;
        break;
    }
    // Set position
    rigthOuter.write(rigthOuterPosition + rigthMustachePositionDelta);
    // Reset time
    mustacheRigthMillis = millis();
  }
}

// Move the mustache in a wavy way
void mustacheWave() {
  if (millis() - mustacheRigthMillis > 5) {
    // Compute position
    switch (mustacheCurrentAction) {
      case 1:
        rigthMustachePositionDelta++;
        // If I've finished with this action perform the next one
        if (rigthMustachePositionDelta > 20)
          mustacheCurrentAction = 2;
        break;
      case 2:
        rigthMustachePositionDelta--;
        // If I've finished with this action perform the next one
        if (rigthMustachePositionDelta < -20)
          mustacheCurrentAction = 3;
        break;
      case 3:
        rigthMustachePositionDelta++;
        // If I've finished with this action perform the next one
        if (rigthMustachePositionDelta > 0)
          mustacheCurrentAction = 1;
        break;
      default:
        rigthInner.write(rigthOuterPosition);
        mustacheCurrentAction = 1;
        break;
    }
    // Set position
    rigthInner.write(rigthInnerPosition + rigthMustachePositionDelta);
    rigthOuter.write(rigthOuterPosition + rigthMustachePositionDelta);
    // Reset time
    mustacheRigthMillis = millis();
  }
}
