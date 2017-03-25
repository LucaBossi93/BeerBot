// MUSTACHE SUPPORT FUNCTIONS //

// INCLUSIONS //

#include <Servo.h>

// DEFINITIONS //

#define SERVO_PIN_EYEBROW 22                  // First servo pin of the rigth mustache

// VARIABLES //

// Others
int eyebrowCurrentPosition = 0;               // Current eyebrow position
bool inPosition = true;                       // Tells if the eyebrow are in the deisred position

// Servo mototors
Servo eyebrowMotor;

// Sensors

// Positions
int eyebrowPosition = 80;                     // Default position of the eyebrow
int eyebrowPositionDelta = 0;                 // Delta to be applied on the default position

// Millis
long eyebrowMillis;                           // Tells how often update position

// FUNCTIONS //

// Setup the movement (call this in the setup)
void setupEyebrow()
{
  eyebrowMotor.attach(SERVO_PIN_EYEBROW);

  // Set initial position
  eyebrowMotor.write(eyebrowPosition);

  // Initialize the millis
  eyebrowMillis = millis();
}

// Sets the animation and resets the position
void setEybrowPosition(int i) {
  inPosition = false;
  eyebrowCurrentPosition = i;
}

// Menage the movement (call this in the loop)
void menageEyebrow() {
  switch (eyebrowCurrentPosition) {
    case 1:
      setPosition(0);
      break;
    case 2:
      setPosition(25);
      break;
    case 3:
      setPosition(-25);
      break;
    default:
//      mustacheTips(0);
      break;
  }
}

void setPosition(int p) {
  if (millis() - eyebrowMillis > 5 && !inPosition) {
    if (eyebrowPositionDelta - p < 0)
      eyebrowPositionDelta++;
  } else
    eyebrowPositionDelta--;
  if (eyebrowPositionDelta == p)
    inPosition = true;
}
