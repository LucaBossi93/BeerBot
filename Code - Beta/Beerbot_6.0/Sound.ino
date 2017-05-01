// SOUND SUPPORT FUNCTIONS //

/* Indices of each pool to be called in setTalk:
   1 - moveforwarddelay
   2 - lookforground
   3 - lookforpeople
   4 - beerfact
   5 - mustacheshow
   6 - greet
   7 - invite
   8 - goodbye
   9 - help
   10 - singleinteraction
   11 - multipleinteraction
   12 - staticpeople
   13 - beergameinvite
   14 - beergameselected
   15 - beergameend
   16 - foamtouch
   17 - foamtouched
   18 - foamtouchedsurprised
   19 - askgender
   20 - maninteraction
   21 - womaninteraction
   22 - askpreference
   23 - winepreference
   24 - beerpreference
   25 - beerliftrequest
   26 - handletouchedinteraction
   27 - handletouchedsurprised
   28 - requestclapping
   29 - lowclapping
   30 - highclapping
   31 - requestscreaming
   32 - lowscreaming
   33 - medscreaming
   34 - highscreaming
   35 - reactclapping
   36 - magicshow
   37 - magicshowend
   38 - timeout
*/

// INCLUSIONS //

#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>

// DEFINITIONS //

#define POOLS 38   //number of pools 

// VARIABLES //

int pool_sizes[POOLS] = {3, 3, 3, 12, 3, 9, 6, 4, 1, 3, 3,
                         3, 1, 2, 1, 2, 1, 5, 1, 1, 5, 1, 3, 3,
                         2, 2, 5, 4, 3, 1, 1, 1, 2, 1, 4, 1, 1,
                         5
                        };                    // Number of audio files for each pool
                        
// Miscellaneous
int currentTalk;                              // Current talk being played
int currentSound;                             // Current sound in a talk being played

// Millis
long soundMillis;                             // Tells how often change sound

// Durations
int duration [112] =  {2, 1, 1, 2, 3, 3, 2, 2, 3, 5, 10, 6, 8, 9,
                       8, 8, 6, 6, 23, 19, 16, 2, 2, 4, 1, 1, 1, 2,
                       1, 1, 1, 1, 1, 3, 8, 5, 6, 6, 9, 3, 3, 2,
                       2, 3, 8, 8, 5, 4, 3, 4, 8, 3, 6, 3, 1, 1,
                       5, 4, 2, 6, 2, 2, 3, 2, 3, 8, 6, 4, 10, 6,
                       7, 11, 10, 2, 4, 3, 5, 6, 5, 2, 4, 4, 6,
                       2, 2, 3, 2, 3, 3,
                       2, 4, 3, 4, 6, 3, 13, 6, 5, 2, 2, 10, 2, 2,
                       2, 3, 6, 5, 3, 1, 2, 3, 4
                      };                       // Duration of each audio file

// SUPPORT FUNCTIONS //

// Setup the sound system (call this in the setup)
void setupSound()
{
  // Set Serial for DFPlayer-mini mp3 module
  mp3_set_serial(Serial);
  // Sets the volume of the speaker (30)
  mp3_set_volume(16);
}

// Sets the current talk
void setTalk(int i, boolean rep) {
  if (i != currentTalk) {
    currentTalk = i;
    currentSound = 1;
    soundMillis = millis();
  }
  playAudio(currentTalk, rep);
}

// Return the duration of the current sound
int getPlayDuration() {
  Serial.println("currentSound: ");
  Serial.println(currentSound);
  Serial.println("duration: ");
  Serial.println(duration[currentSound]);
  return duration[currentSound] * 1000;
}

// CASE 0 - Silence
void silence() {
  //do nothing
}

void playAudio(int index, boolean rep) {
  if (rep) {
    if (millis() - soundMillis > duration[currentSound]) {
      currentSound = random(pool_sizes[index - 1]) + computeStartingPoint(index - 1);
      mp3_play (currentSound);
      soundMillis = millis();
    }
  } else {
    if (index == 0) {
      silence();
    } else {
      currentSound = random(pool_sizes[index - 1]) + computeStartingPoint(index - 1);
      mp3_play(currentSound);
    }
  }
}

// Compute the starting point of each pool
int computeStartingPoint(int index) {
  int sum = 0;
  for (int i = 0; i < index; i++) {
    sum += pool_sizes[i];
  }

  return sum;
}
