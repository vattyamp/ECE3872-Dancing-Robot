 /*
 * The Circuit: 
 * Connect AUD to analog input 0
 * Connect GND to GND
 * Connect VCC to 3.3V (3.3V yields the best results)
 *  
 *  To adjust when the LED turns on based on audio input:
 *  Open up the serial com port (Top right hand corner of the Arduino IDE)
 *  It looks like a magnifying glass. Perform several experiments 
 *  clapping, snapping, blowing, door slamming, knocking etc and see where the
 *  resting noise level is and where the loud noises are. Adjust the if statement
 *  according to your findings.
 *  
 *  You can also adjust how long you take samples for by updating the "SampleWindow"
 * 
 * This code has been adapted from the
 * Example Sound Level Sketch for the
 * Adafruit Microphone Amplifier
 * 
 */

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
#define rest 0

 #include "fix_fft.h"
#include <pt.h>

 #define SAMPLES 128

 static struct pt detect_tempo;

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

void setup() 
{
  PT_INIT(&detect_tempo);
   Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(pbPin, INPUT_PULLUP);
}

static int detect_tempo_func(struct pt* pt) {
    PT_BEGIN(pt);
    while(duration == 0) {
        int val;
        for(int i = 0; i < SAMPLES; i++)
        {
          val = analogRead(3); // 0-1023
          data[i] = (char)(val/4 - 128); // store as char
          im[i] = 0; // init all as 0
        }
        // run FFT
        fix_fft(data, im, 7, 0);
    
        // extract absolute value of data only, for 64 results
        fftMax = -10000;
        for(int i = 0; i < SAMPLES/2; i++)
        {
          barht[i] = (int)sqrt(data[i] * data[i] + im[i] * im[i]);
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
      int timer = millis();
      PT_WAIT_UNTIL(pt, millis() - timer > 2000);
      
    }
    PT_END(pt);
}

void loop() 
{
 detect_tempo_func(&detect_tempo);
}
