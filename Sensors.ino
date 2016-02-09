//library
#include "DHT.h"
//constants
#define DHTTYPE DHT11   // using DHT 11
const int DHTPIN = 4;
const int TEMP_SENSOR = 0; //A0
const int BUTTON = 8;
const int PHOTOCELL = 2; //A2
//variables
short mode = 0; // mode for which sensor to print on serial
int buttonStatus; // to show if a button has been clicked
//functions

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);
boolean buttonClick(void);

//START
void setup() {
  Serial.begin(9600);
  pinMode(BUTTON, INPUT_PULLUP); //HIGH means off, LOW means on
  dht.begin();
  buttonStatus = digitalRead(BUTTON);
}

void loop() {
  // CYCLE LOGIC: to cycle through 3 types of data with each OFF/ON click, 1. light 2. temp 3. humidity
  if (buttonClick()){
    buttonStatus = digitalRead(BUTTON);
    mode++;
    if (mode > 5){
      mode = 0;
    }
  }

  // READING DATA
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  delay(10);
  analogRead(TEMP_SENSOR);
  float ambientTemp =(5.0 * analogRead(TEMP_SENSOR) * 100.0) / 1024;
  delay(10);
  analogRead(PHOTOCELL);
  float ambientLight = analogRead(PHOTOCELL);
  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);
  
  if (mode % 2 == 0){ // Display all of the collected data on Serial Monitor if button is OFF
    Serial.print("Ambient Light: ");
    Serial.print(ambientLight);
    Serial.print(" mV\t");
    Serial.print("Ambient Temp: ");
    Serial.print(ambientTemp);
    Serial.print(" *C\t");
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.print(" *C ");
    Serial.print(f);
    Serial.print(" *F\t");
    Serial.print("Heat index: ");
    Serial.print(hic);
    Serial.print(" *C ");
    Serial.print(hif);
    Serial.println(" *F");
  } else if (mode == 1) {
    Serial.println(ambientLight);
  } else if (mode == 3){
    Serial.println(ambientTemp);
  } else if (mode == 5){
    Serial.println(h);
  }
}

//checks to see if a button has been clicked
//returns false if status of button is the same after 20 ms, true otherwise
boolean buttonClick(){
  delay(20);
  if (buttonStatus == digitalRead(BUTTON)){
    return false;
  } else {
    return true;
  }
}

