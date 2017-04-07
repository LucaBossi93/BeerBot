// TOUCH DETECTION SUPPORT FUNCTIONS //

// INCLUDES //

#include <CapacitiveSensor.h>

// DEFINITIONS //
#define THRESHOLD_MEDIUM 700            // Threshold under which the volume is considered low
#define THRESHOLD_HIGH 1000             // Threshold over which the volume is considered high
#define MIC_PIN A1                      // Arduino pin connected to the microphone

// VARIABLES //

// Miscellaneous
int volume;                             // Volume registered by the microphone
int volumeCounter = 0;                  // Counter to get volume over time
int acquisitionCounter = 0;             // Times the acquisition has been performed

// SUPPORT FUNCTIONS //

// Detect a single audio input and process it
void detectVolumeContinuous() {
  // Count this acquisition
  acquisitionCounter++;
  // Acquire
  volume = analogRead(A0);
  // And process it
  if (volume < THRESHOLD_MEDIUM)
    volumeCounter -= 1;
  else if (volume < THRESHOLD_HIGH)
    volumeCounter += 2;
  else
    volumeCounter += 4;
}

// Return the value based on all the samples processed up to now
int getVolumeContinuous() {
  if (float(volumeCounter) / acquisitionCounter < 1) {
    // Reset the counters for the next call
    acquisitionCounter = 0;
    volumeCounter = 0;
    return 0;
  }
  else if (float(volumeCounter) / acquisitionCounter < 2) {
    // Reset the counters for the next call
    acquisitionCounter = 0;
    volumeCounter = 0;
    return 1;
  } else {
    // Reset the counters for the next call
    acquisitionCounter = 0;
    volumeCounter = 0;
    return 2;
  }
}
