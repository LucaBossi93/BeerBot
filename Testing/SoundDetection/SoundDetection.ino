// SOUND DETECTION SUPPORT FUNCTIONS //

// INCLUDES //

#include <CapacitiveSensor.h>

// DEFINITIONS //
#define MIC_PIN A1                      // Arduino pin connected to the microphone

// VARIABLES //

// Miscellaneous
int volume;                             // Volume registered by the microphone
int volumeCounter = 0;                  // Counter to get volume over time
int acquisitionCounter = 0;             // Times the acquisition has been performed
int acquisitionTime = 5000;

// Millis
int testStartingTime;

// CODE //

void setup() {
  // Initialize the serial port
  Serial.begin(9600);
  // Initialize the millis
  testStartingTime = millis();
}

void loop() {
  if (millis() - testStartingTime < acquisitionTime) {
    detectVolumeContinuous();
    Serial.print("Volume: ");
    Serial.print(volume);
    Serial.print(", with counters: [");
    Serial.print(volumeCounter);
    Serial.print(", ");
    Serial.print(acquisitionCounter);
    Serial.println("]");
    // delay(10);
  } else {
    // Reset the millis
    testStartingTime = millis();
    Serial.print("GOT SOUND LEVEL: ");
    Serial.println(getVolumeContinuous());
    delay(1000);
  }
}

// SUPPORT FUNCTIONS //

// Detect a single audio input and process it
void detectVolumeContinuous() {
  // Count this acquisition
  acquisitionCounter++;
  // Acquire
  volume = analogRead(A0);
  // And process it
  if (volume < 512)
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
