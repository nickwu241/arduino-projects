//constants
const int LED1 = 4; //BLUE
const int LED2 = 12; //GREEN
const int LED3 = 8; //RED
const int SWITCH1 = 5; 
const int SWITCH2 = 6;
const int LCLED = 9; //light controlled LED
const int analogPin = 2; //A2

//variables
int val = 0, switch1 = 0, switch2 = 0, counter = 0, blinkCounter = 0;
boolean changeSwitches = false;

//functions
boolean changeStatus(int, int);
void checkPhotoCell();
void displayStatus();

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);
  //initialize inputs and outputs
  pinMode(SWITCH1, INPUT);
  pinMode(SWITCH2, INPUT);
  pinMode(13, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LCLED, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  //if switches are changed or a pattern cycle (3 flashes or 3 seconds) is complete
  //each counter is 0.2s 
  if (changeSwitches == true || counter == 15){
    counter = 0; // reset pattern
  }
  //reset blinkCounter every 2 seconds
  //each blinkCounter is 0.2s
  if (blinkCounter == 10){
    blinkCounter = 0;
  }
  //logic for PhotoCell LED
  checkPhotoCell();

  //reads status of switches
  switch1 = digitalRead(SWITCH1);
  switch2 = digitalRead(SWITCH2);

  if (blinkCounter < 5){ // 1 second
      digitalWrite(13, LOW); // blinking LED is 13 by default
  } else if (5 <= blinkCounter < 10){ // 1 second
      digitalWrite(13, HIGH);
  } 
  
  // logic for both switch off
  if (switch1 == LOW && switch2 == LOW){
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, HIGH);
  } // logic for switch1 off, switch2 on
  else if (switch1 == LOW && switch2 == HIGH){
    if (counter < 5){
      digitalWrite(LED1, LOW); //BLUE
      digitalWrite(LED2, HIGH);
      digitalWrite(LED3, HIGH);
    } 
    else if (5 <= counter && counter < 10){
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, LOW); //GREEN
    } 
    else if (10 <= counter && counter < 15) {
      digitalWrite(LED2, HIGH);
      digitalWrite(LED3, LOW); //RED
    }
  } // logic for switch1 on, switch2 off
  else if (switch1 == HIGH && switch2 == LOW){
    if (counter < 5){
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, LOW); //GREEN
      digitalWrite(LED3, HIGH);
    } 
    else if (5 <= counter && counter < 10){
      digitalWrite(LED2, HIGH);
      digitalWrite(LED1, LOW); //BLUE
    } 
    else if (10 <= counter && counter < 15) {
      digitalWrite(LED1, HIGH);
      digitalWrite(LED3, LOW); //RED 
    }
  } //logic for both switch on
  else if (switch1 == HIGH && switch2 == HIGH){
    if (counter < 5){
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED3, LOW); //RED
    } 
    else if (5 <= counter && counter < 10){
      digitalWrite(LED3, HIGH);
      digitalWrite(LED1, LOW); //BLUE
    } 
    else if (10 <= counter && counter < 15) {
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, LOW); //GREEN
    }
  }
  blinkCounter++;
  counter++; //one counter is 0.2s, i.e. 5 counters = 1 flash => flashes every 1s
  delay(200); // loops every 0.2s
  changeSwitches = changeStatus(SWITCH1, SWITCH2); //checks if switches have been changed
}
// See if status of SW1 and SW2 has changed
boolean changeStatus(int SW1, int SW2){
  displayStatus();
  if (switch1 != digitalRead(SW1) || switch2 != digitalRead(SW2)){
    return true;
  } 
  else {
    return false;
  }
}
//PART C PHOTOCELL
//LCLED is brighter when photocell is covered
void checkPhotoCell(){
  val = analogRead(analogPin);
  if (val < 200){
    analogWrite(LCLED, 255);
  } 
  else if ( 200 <= val && val < 400){
    analogWrite(LCLED, 191);
  } 
  else if ( 400 <= val && val < 600){
    analogWrite(LCLED, 127);
  } 
  else if ( 600 <= val && val < 800){
    analogWrite(LCLED, 64);
  } 
  else if ( 800 <= val){
    analogWrite(LCLED, 0);
  }
}
//Shows status in Serial Monitor (PART D)
void displayStatus(){
  Serial.print("Switches:");
  if (switch1 == HIGH){
    Serial.print(" ON");
  } 
  else {
    Serial.print(" OFF");
  }
  if (switch2 == HIGH){
    Serial.print(" ON\n");
  } 
  else {
    Serial.print(" OFF\n");
  }
  Serial.print("LEDs:");
  if (digitalRead(LED1) == LOW){
    Serial.print(" ON");
  } 
  else {
    Serial.print(" OFF");
  }
  if (digitalRead(LED2) == LOW){
    Serial.print(" ON");
  } 
  else {
    Serial.print(" OFF");
  }
  if (digitalRead(LED3) == LOW){
    Serial.print(" ON\n");
  } 
  else {
    Serial.print(" OFF\n");
  }
  Serial.print("Photocell value: ");
  Serial.print(val);
  Serial.print(" mV\n");
  Serial.print("Light-controlled LED:");
  if (val < 800){
    Serial.print(" ON\n");
  } 
  else {
    Serial.print(" OFF\n");
  }
  Serial.print("\n");
}

