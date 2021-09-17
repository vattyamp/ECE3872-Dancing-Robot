// C++ code
//


#include <Servo.h>
#include <stdio.h>
#include <math.h>


#define TempoCal 512
#define TempoPotMax 1023
#define PwmMax 255

#define speakerPIN 5


#define rest 0

#define mouthAnglePerPitch (high_C)) * 2

#define Octave 6


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

Servo mouth;

void setup()
{
  
  mouth.attach(10);
  Serial.begin(9600);
  
  
  //creates duration array for notes to be played
  for (int noteNum = 0; noteNum < songLength; noteNum++) {
    durations[noteNum] = (beats[noteNum] / bps) * 1000;
  }


//Changes angle of servo depending on current and last note
  
  
}

void loop()
{
  for (int n = 0; n < songLength; n++) 
    {
      if (n == 0) 
        {
        mouth.write(180);
      } else if( notes[n-1] == rest && notes[n] != rest)
        { 
        mouth.write(180);
      } else if (notes[n-1] != rest && notes[n] == rest)
        {
        mouth.write (0);
      }
        tone(speakerPIN, notes[n], durations[n]);
        delay(durations[n]);
  
    }
//     Iteration where the servo moves responding to the pitch
//    for (int n = 0; n < songLength; n++) 
//    {
//
//        if (n != rest){
//          mouth.write(180 * (notes[n] / mouthAnglePerPitch);
//        }
//        tone(speakerPIN, notes[n], durations[n]);
//        delay(durations[n]);
//  
//    }
}
