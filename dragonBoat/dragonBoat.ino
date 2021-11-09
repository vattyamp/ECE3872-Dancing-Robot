#include <Wire.h>

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

/* 
* This part is for seven segment diplay
* The potentiometer to controloctave is connected to A1.
* Seven segment display has four inputs with pin 6, 7, 8, and 11.
* The octave scalar value is changed to choose the desired octave.
*/
int Capteur = A1;
int valeurLue;
float tension;
int inputA=6;
int inputB=7;
int inputC=8;
int inputD=11;
float octaveScalar = 1.0;


#define TempoCal 512
#define TempoPotMax 1023
#define PwmMax 255

#define speakerPIN 5


//#define rest 0

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
#define rest 0


const float bps = 5;
int songLength = 54;  
float notes[] = {C, rest, C, rest, C, rest, D, rest, E, rest, E, rest, D, rest, E, rest, F, rest, G, rest, high_C, rest, high_C, rest, high_C, rest, G, rest, G, rest, G, rest, E, rest, E, rest, E, rest, C, rest, C, rest, C, rest, G, rest, F, rest, E, rest, D, rest, C, rest};
float beats[] = {2,1,2,1,2,1,1,1,2,1,2,1,1,1,2,1,1,1,6,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,1,1,2,1,1,1,5,1};
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

/* This setup is for UI seven segment display. 
*  Pins are set up here.
*/
  pinMode(inputA,OUTPUT);
  pinMode(inputB,OUTPUT);
  pinMode(inputC,OUTPUT);
  pinMode(inputD,OUTPUT);
  
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
        //Serial.println(tempoBPS);
      }
      int timer = millis();
      PT_WAIT_UNTIL(pt, millis() - timer > 2000);
      
    }
    PT_END(pt);
}

static float change_seven_segment_display(float octaveScalar) {
    valeurLue = analogRead(Capteur);
    tension = map(valeurLue,0,1023,0,50);
    //Serial.println(tension);
    
    if (tension >=0 && tension < 5) { //Number 0
      digitalWrite(inputA,LOW);
      digitalWrite(inputB,LOW);
      digitalWrite(inputC,LOW);
      digitalWrite(inputD,LOW);
      return 0.0625;

    }
    if (tension >=5 && tension < 10) { //Number 1
      digitalWrite(inputA,HIGH);
      digitalWrite(inputB,LOW);
      digitalWrite(inputC,LOW);
      digitalWrite(inputD,LOW);
      return 0.125;

    }
    if (tension >=10 && tension < 15) { //Number 2
      digitalWrite(inputA,LOW);
      digitalWrite(inputB,HIGH);
      digitalWrite(inputC,LOW);
      digitalWrite(inputD,LOW);
      return 0.25;
    }
    if (tension >=15 && tension < 20) { //Number 3
      digitalWrite(inputA,HIGH);
      digitalWrite(inputB,HIGH);
      digitalWrite(inputC,LOW);
      digitalWrite(inputD,LOW);
      return 0.3;

    }
    if (tension >=20 && tension < 25) { //Number 4
      digitalWrite(inputA,LOW);
      digitalWrite(inputB,LOW);
      digitalWrite(inputC,HIGH);
      digitalWrite(inputD,LOW);
      return 0.5;

    }
    if (tension >=30 && tension < 35) { //Number 5
      digitalWrite(inputA,HIGH);
      digitalWrite(inputB,LOW);
      digitalWrite(inputC,HIGH);
      digitalWrite(inputD,LOW);
      return 1;
    } 
    if (tension >=35 && tension < 40) {//Number 6
      digitalWrite(inputA,LOW);
      digitalWrite(inputB,HIGH);
      digitalWrite(inputC,HIGH);
      digitalWrite(inputD,LOW);
      return 1.5;
    }
    if (tension >=40 && tension < 45) {//Number 7
      digitalWrite(inputA,HIGH);
      digitalWrite(inputB,HIGH);
      digitalWrite(inputC,HIGH);
      digitalWrite(inputD,LOW);
      return 2;
    }
    if (tension >=45 && tension < 48) {//Number 8
      digitalWrite(inputA,LOW);
      digitalWrite(inputB,LOW);
      digitalWrite(inputC,LOW);
      digitalWrite(inputD,HIGH);
      return 2.5;
    }
    
    if (tension >= 48) {                //Number 9
      digitalWrite(inputA,HIGH);
      digitalWrite(inputB,LOW);
      digitalWrite(inputC,LOW);
      digitalWrite(inputD,HIGH);
      return 3;
    }
}

void loop()
{   
//  detect_tempo_func(&detect_tempo);
  for (int n = 0; n < songLength && playAudio; n++) 
  {
      octaveScalar = change_seven_segment_display(octaveScalar);
      if (notes[n] != rest){
        surfer.write(120 * (notes[n] / surferAnglePerPitch);
      }
      if (failSafe) {
        tempoBPS = ((float) analogRead(tempoPot) / 1024.0) * 25;
      }
      int currDuration = ((float) beats[n] / (float) tempoBPS) * 1000.0;
      Serial.print("note:");
      Serial.println(notes[n]);
      Serial.print("OctaveScalar:");
      Serial.println(octaveScalar);
      
      tone(speakerPIN, (float) (notes[n] * octaveScalar), currDuration);
      delay(currDuration);
  }
}
