//from front of sensor
//5v, ground, signal
//2/24/2016 EDIT
//Make sure the ground all goes to the same place
//#include "DualVNH5019MotorShield.h"
//DualVNH5019MotorShield md;
//Pins for the sensors=======================================
#define Pin2 22 //left side
#define Pin3 24
#define Pin4 26
#define Pin5 28
#define Pin6 30 // right side
//===========================================================
uint64_t currentTime = 0;

void setup() {
  Serial.begin(9600);
 // md.init();
}

void loop() {
  currentTime = millis();           // Initialize Timer
  //Serial.println(RCTime(Pin2));   // Connect to pin 2, display results
  //Serial.println(RCTime(Pin3));   // Same for these
  //Serial.println(RCTime(Pin4));
  //Serial.println(RCTime(Pin5));
  //Serial.println(RCTime(Pin6));
  Serial.print("Sensor state: ");
  sensorState();
  //delay(50);
}

int RCTime(int sensorIn){         //
   pinMode(sensorIn, OUTPUT);     // Make pin OUTPUT
   digitalWrite(sensorIn, HIGH);  // Pin HIGH (discharge capacitor)
   pinMode(sensorIn, INPUT);      // Make pin INPUT
   digitalWrite(sensorIn, LOW);   // Turn off internal pullups
   if (!digitalRead(sensorIn)) {  // Pin goes HIGH which means its not on the black line
      return 1;
   }
   if(digitalRead(sensorIn)){     // Pin goes LOW which means its on the black line
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
  int temp = 0;
  temp = ((RCTime(Pin6)*pow(2,4)) + (RCTime(Pin5)*pow(2,3)) + (RCTime(Pin4)*pow(2,2)) + (RCTime(Pin3)*pow(2,1)) + RCTime(Pin2));
  Serial.println(temp);
  return temp;
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
 if (sensorState() == 31) {   //11111 second time we encounter this it has to be a
  //straight();               //right turn
 } 
 if (sensorState() == 0) {    //00000
  //();
 } 
 if (sensorState() == 0) {    //00000
  //();
 }  
}
/*
void forward() {  //Moves the robot forward
  md.setM1Speed(-300); //right side //More powerful
  md.setM2Speed(380);//left side
}

void backward() { //Moves te robot backwards
  md.setM1Speed(300); //right side
  md.setM2Speed(-380);//left side
}

void leftTurn() { //Turns the robot left from the center
  md.setM1Speed(-300); //Forward right side
  md.setM2Speed(-380);//Reverse left side
}

void rightTurn() { //Turns the robot right from the center
 md.setM1Speed(300); //Reverse right side
 md.setM2Speed(380);//Forward left side
}

void stopMotors() {//Stop the robot
 md.setM1Speed(0);
 delayMicroseconds(10);
 md.setM2Speed(0);
 delayMicroseconds(10);
}
*/
