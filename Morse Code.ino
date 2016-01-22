//constants, I/O ports
const int HEX_A = 13;
const int HEX_B = 12;
const int HEX_C = 11;
const int HEX_D = 9;
const int HEX_E = 8;
const int HEX_F = 6;
const int HEX_G = 7;
const int HEX_dec = 10;
const int LED = 2; //LED Port
const int BUZZER = 5; //Piezo Buzzer Port

boolean hexCode[][8] = { //lookup array for 7-seg hex
  {true, true, true, true, true, true, false, false},     //0
  {false, true, true, false, false, false, false, false}, //1
  {true, true, false, true, true, false, true, false},    //2
  {true, true, true, true, false, false, true, false},    //3
  {false, true, true, false, false, true, true, false},   //4
  {true, false, true, true, false, true, true, false},    //5
  {true, false, true, true, true, true, true, false},     //6 
  {true, true, true, false, false, false, false, false},  //7
  {true, true, true, true, true, true, true, false},      //8
  {true, true, true, true, false, true, true, false},     //9
  {true, true, true, false, true, true, true, false},     //a (index 10)
  {false, false, true, true, true, true, true, false},    //b
  {true, false, false, true, true, true, false, false},   //c
  {false, true, true, true, true, false, true, false},    //d
  {true, false, false, true, true, true, true, false},    //e
  {true, false, false, false, true, true, true, false},   //f
  {true, true, true, true, false, true, true, false},     //g
  {false, true, true, false, true, true, true, false},    //h
  {false, false, false, false, true, true, false, false}, //i
  {false, true, true, true, true, false, false, false},   //j
  {false, true, true, false, true, true, true, false},    //k
  {false, false, false, true, true, true, false, false},  //l
  {true, false, true, false, true, false, false, false},  //m
  {false, false, true, false, true, false, true, false},  //n
  {true, true, true, true, true, true, false, false},     //o
  {true, true, false, false, true, true, true, false},    //p
  {true, true, true, false, false, true, true, false},    //q
  {false, false, false, false, true, false, true, false}, //r
  {true, false, true, true, false, true, true, false},    //s
  {false, false, false, true, true, true, true, false},   //t
  {false, true, true, true, true, true, false, false},    //u
  {false, false, true, true, true, false, false, false},  //v
  {false, true, false, true, false, true, false, false},  //w
  {false, true, true, false, true, true, true, false},    //x
  {false, true, true, true, false, true, true, false},    //y
  {true, true, false, true, true, false, true, false},    //z
  {false, false, false, false, false, false, false, false},//(index 36) off
  {false, false, false, false, false, false, false, false},// off 
  {false, false, false, false, false, false, false, true}, // .
  {false, false, false, false, false, false, false, false},// off
  {false, false, false, false, false, false, false, false},// index 40 for space which is off
  {false, false, false, false, false, false, false, true} // . for <EE5> error prosign
};
char morse[][9] = { //morse code lookup array
  "-----", //0
  ".----", //1
  "..---", //2
  "...--", //3
  "....-", //4
  ".....", //5
  "-....", //6
  "--...", //7
  "---..", //8
  "----.", //9
  ".-",    //a (index 10)
  "-...",  //b
  "-.-.",  //c
  "-..",   //d
  ".",     //e
  "..-.",  //f
  "--.",   //g
  "....",  //h
  "..",    //i
  ".---",  //j
  "-.-",   //k
  ".-..",  //l
  "--",    //m
  "-.",    //n
  "---",   //o
  ".--.",  //p
  "--.-",  //q
  ".-.",   //r
  "...",   //s
  "-",     //t
  "..-",   //u
  "...-",  //v
  ".--",   //w
  "-..-",  //x
  "-.--",  //y
  "--..",  //z
  "--..--", //, (index 36)
  "-....-", //-
  ".-.-.-", //. 
  "---...", //: (index 39)
  "*",      // for spaces (index 40)
  "........" // for <EE5> error prosign
};

//variables
int wpm, timeUnit, dot, dash, interChar, interWord; // timing depends on WPM
boolean userInput = false, selectedWPM = false;
char *morseChar = (char*)malloc(9*sizeof(char));    // stores morse translation of a character
boolean *hex = (boolean*)malloc(8*sizeof(boolean)); // stores hex translation of a character

//functions
void setUpTimings(int);
void parseCharToMorse(char[], int);
void signalOut(char);
int getHex(int);

void setup() {
  Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps
  delay(2000);
  pinMode(LED, OUTPUT);
  pinMode(HEX_A, OUTPUT);
  pinMode(HEX_B, OUTPUT);
  pinMode(HEX_C, OUTPUT);
  pinMode(HEX_D, OUTPUT);
  pinMode(HEX_E, OUTPUT);
  pinMode(HEX_F, OUTPUT);
  pinMode(HEX_G, OUTPUT);
  pinMode(HEX_dec, OUTPUT);
}
//REAL CODE
void loop() {
  //everything off initially
  digitalWrite(HEX_A, HIGH);
  digitalWrite(HEX_B, HIGH);
  digitalWrite(HEX_C, HIGH);
  digitalWrite(HEX_D, HIGH);
  digitalWrite(HEX_E, HIGH);
  digitalWrite(HEX_F, HIGH);
  digitalWrite(HEX_G, HIGH);
  digitalWrite(HEX_dec, HIGH);
  digitalWrite(LED, HIGH);
  while (!selectedWPM){  //prompts user to select desired WPM if WPM not selected yet
    Serial.print("\nPlease enter WPM (Word Per Minute): ");
    userInput = false;
    while (!userInput){ //waits for user input
      if (Serial.available() > 0){
        userInput = true;
        selectedWPM = userInput;
        wpm = Serial.parseInt();
        Serial.println(wpm);
        timeUnit = 1200 / wpm;  //using PARIS as a standard word
        setUpTimings(timeUnit); //sets up timing for morse code messages
      }
    }
  }
  userInput = false; 
  Serial.print("Enter your message: "); //prompts user for message
  while (!userInput){ //waits for user input
    while (Serial.available() > 0){
      userInput = true;
      String message = Serial.readString(); //converts message into a string
      Serial.print(message);
      for (int index = 0; index < message.length(); index++){ //processing user's message
        String checkError = message.substring(index, index + 5);
        Serial.println();
        if (checkError == "<EE5>"){ //if there's an error, outputs 8 running E's
          Serial.println("<EE5>");
          hex = hexCode[41];
          morseChar = morse[41];
          index += 5;
        } else { // if not, process next char
          int ascii = message.charAt(index);
          Serial.print("Morse code for :");
          Serial.println((char)ascii);
          //depending on the character ascii value, find the appropiate index from lookup arrays
          if (65 <= ascii && ascii <= 90){ // for uppercase letters
            hex = hexCode[ascii - 55];
            morseChar = morse[ascii - 55];
          } 
          else if (97 <= ascii && ascii <= 122){ // for lowercase letters
            hex = hexCode[ascii - 87];
            morseChar = morse[ascii - 87];
          } 
          else if (44 <= ascii && ascii <= 46){  // for , - .
            hex = hexCode[ascii - 8];
            morseChar = morse[ascii - 8];
          } 
          else if (ascii == 58){                 // for :
            hex = hexCode[39];
            morseChar = morse[39];
          } 
          else if (ascii == 32){                 // for space
            hex = hexCode[40];
            morseChar = morse[40];
          } 
          else if (48 <= ascii && ascii <= 57){  // for 0-9
            hex = hexCode[ascii-48];
            morseChar = morse[ascii-48];
          }
        }
        int hexIndex;  // displays hex for a certain char
        for (hexIndex = 0; hexIndex < 8; hexIndex++){
          if (hex[hexIndex]){
            digitalWrite(getHex(hexIndex), LOW);
          } else {
            digitalWrite(getHex(hexIndex), HIGH);
          }
        }
        int j = 0;
        while (morseChar[j] != '\0'){ // checks how long the morse translation is
          j++;
        }
        for (int i = 0; i < j; i++){ // loops through morse translation and provides signal from LED and Buzzer
          Serial.print("current signal is");
          Serial.print(morseChar[i]);
          Serial.print("\n");
          signalOut(morseChar[i]);
        }
        delay(interChar - dot); // rest after every character processed, note: -dot because there's a dot delay after every signal
      }
    }
  }
  delay(1000); // waits 1 second before prompting user for next message
}

// given what the timeUnit is, sets up proper delays for each signal
void setUpTimings(int timeUnit){
  dot = timeUnit;
  dash = 3 * dot;
  interChar = dash;
  interWord = 7 * timeUnit;
}
//output of LED and piezo buzzer depending on signal given
void signalOut(char signal){
  // logic for flash and buzz times
  if (signal == '.'){
    digitalWrite(LED, LOW);
    analogWrite(BUZZER, 127);
    delay(dot);
  } 
  else if (signal == '-'){
    digitalWrite(LED, LOW);
    analogWrite(BUZZER, 127);
    delay(dash);
  }
  digitalWrite(LED, HIGH);
  analogWrite(BUZZER, 0);
  if (signal == '*'){ // if signal is a space (new word)
    delay(interWord - 2 * (interChar - dot)); // - 2 * (dash - dot) b/c always a (interChar - dot) rest after a char
  } 
  else { // if not a space
    delay(dot); // intermediate delay between dots and dashes
  }
}

//returns appropiate hex ports
int getHex(int hexIndex){
  switch(hexIndex){
    case 0: return HEX_A;
    case 1: return HEX_B;
    case 2: return HEX_C;
    case 3: return HEX_D;
    case 4: return HEX_E;
    case 5: return HEX_F;
    case 6: return HEX_G;
    case 7: return HEX_dec;
  }
}
