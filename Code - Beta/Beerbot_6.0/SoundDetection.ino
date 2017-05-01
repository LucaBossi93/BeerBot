// SOUND DETECTION SUPPORT FUNCTIONS //

// DEFINITIONS //
#define MIC_PIN 11                       // Arduino pin connected to the microphone

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
  volume = digitalRead(MIC_PIN);
  // And process it
  if (!volume)
    volumeCounter++;
}

// Return the value based on all the samples processed up to now
int getVolumeContinuous() {
  if (float(volumeCounter) / acquisitionCounter < 0.1) {
    // Reset the counters for the next call
    acquisitionCounter = 0;
    volumeCounter = 0;
    return 0;
  }
  else if (float(volumeCounter) / acquisitionCounter < 0.2) {
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
