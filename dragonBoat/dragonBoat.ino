// C++ code
//


#include <Servo.h>
#include <stdio.h>
#include <math.h>
#include "fix_fft.h"
#include <pt.h>

#define SAMPLES 128
static struct pt detect_tempo;
static struct pt audio_out;

char inputTest[SAMPLES];

const int sampleWindow = 250; // Sample window width in mS (250 mS = 4Hz)
unsigned int knock;
int ledPin = 4;
int pbPin = 2;
volatile byte failSafe = LOW;

float fftMax;

char im[SAMPLES];
char data[SAMPLES];
int barht[SAMPLES];

// Timing variables
int startTime;
int duration;
boolean isTimerStarted;
int tempoBPS;

boolean playAudio;


#define TempoCal 512
#define TempoPotMax 1023
#define PwmMax 255

#define speakerPIN 5


#define rest 0

#define surferAnglePerPitch (high_C))

#define Octave 6

#define tempoPot A2


//Music Notes based on Octave--
#define C 16.3516*pow(2,Octave)
#define D 18.35405*pow(2,Octave)
#define E 20.60172*pow(2,Octave)
#define F 21.82676*pow(2,Octave)
#define G 24.49971*pow(2,Octave)
#define A 27.5*pow(2,Octave)
#define B 30.86771*pow(2,Octave)
#define high_C 32.70320*pow(2,Octave)
#define rest 0.0


const float bps = 5;
int songLength = 54;  
int notes[] = {C, rest, C, rest, C, rest, D, rest, E, rest, E, rest, D, rest, E, rest, F, rest, G, rest, high_C, rest, high_C, rest, high_C, rest, G, rest, G, rest, G, rest, E, rest, E, rest, E, rest, C, rest, C, rest, C, rest, G, rest, F, rest, E, rest, D, rest, C, rest};
int beats[] = {2,1,2,1,2,1,1,1,2,1,2,1,1,1,2,1,1,1,6,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,1,1,2,1,1,1,5,1};
float durations[54];

Servo surfer;
Servo sail;

void setup()
{
  
  surfer.attach(10);
  sail.attach(9);
  sail.write(90);
  Serial.begin(9600);
  PT_INIT(&detect_tempo);
  pinMode(ledPin, OUTPUT);
  pinMode(pbPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pbPin), toggle_failsafe, FALLING);
  
  
  //creates duration array for notes to be played
//  for (int noteNum = 0; noteNum < songLength; noteNum++) {
//    durations[noteNum] = (beats[noteNum] / bps) * 1000;
//  }
}

void toggle_failsafe() {
  failSafe = !failSafe;
  digitalWrite(ledPin, failSafe);
  playAudio = failSafe;
  delay(1000);
}

static int play_audio(struct pt* pt) {
  PT_BEGIN(pt);
  while (true) {
    for (int n = 0; n < songLength; n++) 
    {
        if (notes[n] != rest){
          surfer.write(220 * (notes[n] / surferAnglePerPitch);
        }
        tempoBPS = ((float) analogRead(tempoPot) / 1024.0) * 25;
        int currDuration = ((float) beats[n] / (float) tempoBPS) * 1000.0;
        tone(speakerPIN, notes[n], currDuration);
        delay(currDuration);
    }
    int timer = millis();
    PT_WAIT_UNTIL(pt, millis() - timer > 2000);
  }
  PT_END(pt);
}

static int detect_tempo_func(struct pt* pt) {
    PT_BEGIN(pt);
    while(true) {
      if (failSafe == LOW || false) {
        int val;
        for(int i = 0; i < SAMPLES; i++)
        {
          val = analogRead(0); // 0-1023
          data[i] = (char)(val/4 - 128); // store as char
          im[i] = 0; // init all as 0
        }
        // run FFT
        fix_fft(inputTest, im, 7, 0);
    
        // extract absolute value of data only, for 64 results
        fftMax = -10000;
        for(int i = 0; i < SAMPLES/2; i++)
        {
          barht[i] = (int)sqrt(inputTest[i] * inputTest[i] + im[i] * im[i]);
          if (barht[i] > fftMax) {
            fftMax = i;
          }
        }
        if (abs((fftMax * 1000) - C) < 2) {
          if (!isTimerStarted) {
            startTime = millis();
            isTimerStarted = true;
          } else {
            duration = millis() - startTime;
            isTimerStarted = false;
          }
        }
      } else {
        tempoBPS = ((float) analogRead(tempoPot) / 1024.0) * 100;
        Serial.println(tempoBPS);
      }
      int timer = millis();
      PT_WAIT_UNTIL(pt, millis() - timer > 2000);
      
    }
    PT_END(pt);
}

void loop()
{   
//  detect_tempo_func(&detect_tempo);
  for (int n = 0; n < songLength && playAudio; n++) 
  {
      if (notes[n] != rest){
        surfer.write(120 * (notes[n] / surferAnglePerPitch);
      }
      if (failSafe) {
        tempoBPS = ((float) analogRead(tempoPot) / 1024.0) * 25;
      }
      int currDuration = ((float) beats[n] / (float) tempoBPS) * 1000.0;
      tone(speakerPIN, notes[n], currDuration);
      delay(currDuration);
  }
}
