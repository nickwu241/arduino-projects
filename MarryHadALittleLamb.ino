//used https://www.arduino.cc/en/Tutorial/PlayMelody

const int c =   3830;    // 261 Hz 
const int d =   3400;    // 294 Hz 
const int e =   3038;    // 329 Hz 
const int f =   2864;    // 349 Hz
const int fs =  2703;    // 370 Hz
const int g =   2550;    // 392 Hz 
const int a =   2272;    // 440 Hz 
const int b =   2028;    // 493 Hz 
const int C =   1912;    // 523 Hz 
// Define a special note, 'R', to represent a rest
const int R =    0;

int speakerOut = 5;

void setup() { 
  pinMode(speakerOut, OUTPUT);
}

// MELODY and TIMING  =======================================
//  melody[] is an array of notes, accompanied by beats[], 
//  which sets each note's relative length (higher #, longer note) 
//BARNEY
//int melody[] = { a, fs, a, a, fs, d, b, a, g, fs, e, fs, g, fs, g, a, d, d, d, d, d, e, fs, g, a, a, e, e, g, fs, e, d };
//int beats[]  = {32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32}; 
//MARY HAD A LITTLE LAMB
int melody[] = { e, d, c, d, e, e, e, d, d, d, e, e, e, e, d, c, d, e, e, e, d, d, e, d, c, R};
int beats[]  = {32,32,32,32,32,32,64,32,32,64,32,32,64,32,32,32,32,32,32,64,32,32,32,32,64,256}; 

int MAX_COUNT = sizeof(melody) / 2; // Melody length, for looping.

// Set overall tempo
long tempo = 10000;
// Set length of pause between notes
int pause = 1000;
// Loop variable to increase Rest length
int rest_count = 100; //<-BLETCHEROUS HACK; See NOTES

// Initialize core variables
int tone_ = 0;
int beat = 0;
long duration  = 0;

// PLAY TONE  ==============================================
// Pulse the speaker to play a tone for a particular duration
void playTone() {
  long elapsed_time = 0;
  if (tone_ > 0) { // if this isn't a Rest beat, while the tone has 
    //  played less long than 'duration', pulse speaker HIGH and LOW
    while (elapsed_time < duration) {

      digitalWrite(speakerOut,HIGH);
      delayMicroseconds(tone_ / 2);

      // DOWN
      digitalWrite(speakerOut, LOW);
      delayMicroseconds(tone_ / 2);

      // Keep track of how long we pulsed
      elapsed_time += (tone_);
    } 
  }
  else { // Rest beat; loop times delay
    for (int j = 0; j < rest_count; j++) { // See NOTE on rest_count
      delayMicroseconds(duration);  
    }                                
  }                                 
}
void loop() {
  // Set up a counter to pull from melody[] and beats[]
  for (int i=0; i<MAX_COUNT; i++) {
    tone_ = melody[i];
    beat = beats[i];

    duration = beat * tempo; // Set up timing

    playTone(); 
    // A pause between notes...
    delayMicroseconds(pause);
  }
}
