// SOUND SUPPORT FUNCTIONS //

// INCLUSIONS //

#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>

// VARIABLES //

// Others
int currentTalk;                              // Current talk being played
int currentSound;                             // Current sound in a talk being played

// Millis
long soundMillis;                             // Tells how often change sound

// Durations
int duration [9] =  {4, 4, 4, 4, 4, 4, 4, 4, 4 };

// FUNCTIONS //

// Setup the sound system (call this in the setup)
void setupSound()
{
  // Set Serial for DFPlayer-mini mp3 module
  mp3_set_serial(Serial);
  // Sets the volume of the speaker
  mp3_set_volume(30);
}

// Sets the current talk
void setTalk(int i) {
  if (i != currentTalk) {
    currentTalk = i;
    currentSound = 1;
    soundMillis = millis();
  }
}

// Menage the sound (call this in the loop)
void menageSound() {
  switch (mustacheCurrentAnim) {
    case 1:
      talk();
      break;
    case 2:
      walk();
      break;
    default:
      walk();
      break;
  }
}

// ANIMATIONS //

// CASE 1 - Play "talk" sound
void talk() {
  if (millis() - soundMillis > duration[currentSound]) {
    currentSound = random(6) + 1;
    mp3_play (currentSound);
    soundMillis = millis();
  }
}

// CASE 2 -  Play "walk" sound
void walk() {
  if (millis() - soundMillis > duration[currentSound]) {
    currentSound = random(3) + 1;
    mp3_play (currentSound);
    soundMillis = millis();
  }
}
