//from front of sensor
//5v, ground, signal
//2/24/2016 EDIT
//Pins for the sensors=======================================
#define Pin2 2
#define Pin3 3
#define Pin4 4
#define Pin5 5
#define Pin6 6
//===========================================================
uint64_t currentTime = 0;
uint8_t state = 0;
void setup() {
  Serial.begin(9600);
}

void loop() {
  currentTime = millis();           // Initialize Timer
  Serial.println(RCTime(Pin2));	    // Connect to pin 2, display results
  //Serial.println(RCTime(Pin3));   // Same for these
  //Serial.println(RCTime(Pin4));
  //Serial.println(RCTime(Pin5));
  //Serial.println(RCTime(Pin6));
}

int RCTime(int sensorIn){         //
   pinMode(sensorIn, OUTPUT);     // Make pin OUTPUT
   digitalWrite(sensorIn, HIGH);  // Pin HIGH (discharge capacitor)
   pinMode(sensorIn, INPUT);      // Make pin INPUT
   digitalWrite(sensorIn, LOW);   // Turn off internal pullups
   if(digitalRead(sensorIn)){     // Pin goes LOW which means its on the black line
      return 1;
   }
   if (!digitalRead(sensorIn)) {  // Pin goes HIGH which means its not on the black line
      return 0;
   }
}
/*
  Taking the position of each of the sensors into account, I arranged the 
  function to produce a binary digit that is being created as base 2 but is 
  stored as its base 10 value. This allows us to produce a single variable 
  state for our entire array of line sensors as opposed to having to create
  a for loop that would ruin the continuity of multi tasking for the robot. 
*/
long sensorState() { 
  state = ((RCTime(Pin6)*pow(2,4)) + (RCTime(Pin5)*pow(2,3)) + (RCTime(Pin4)*pow(2,2)) + (RCTime(Pin3)*pow(2,1)) + RCTime(Pin2));
  return state;
}

void drive() {
 if (sensorState() == 4) {    //00100
  //straight();
 }
 if (sensorState() == 0) {    //00000
  //leftTurn();
 } 
 if (sensorState() == 28) {   //11100
  //leftTurn();
 } 
 if (sensorState() == 31) {   //11111
  //straight();
 } 
 if (sensorState() == 0) {    //00000
  //();
 } 
 if (sensorState() == 0) {    //00000
  //();
 }  
}
