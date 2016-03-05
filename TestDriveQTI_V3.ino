//from front of sensor
//5v, ground, signal
//2/24/2016 EDIT
//Make sure the ground all goes to the same place
//#include "DualVNH5019MotorShield.h"
//DualVNH5019MotorShield md;
//Pins for the sensors========================================================================================
#define Pin2 22 //left side
#define Pin3 24
#define Pin4 26
#define Pin5 28
#define Pin6 30 // right side
//============================================================================================================
uint64_t currentTime = 0;


void setup() {
  Serial.begin(9600);
 // md.init();
}
//MAIN LOOP===================================================================================================
void loop() {
  currentTime = millis();           // Refresh Timer
  Serial.print("Time: "); 
  long time = currentTime; //If using currentTime print won't work by reference
  Serial.print(time );
  Serial.print("    ");
  //sensorState();
  virtualTrack();
}
//============================================================================================================
int isOnLine(int sensorIn){         //
   pinMode(sensorIn, OUTPUT);     // Make pin OUTPUT
   digitalWrite(sensorIn, HIGH);  // Pin HIGH (discharge capacitor)
   pinMode(sensorIn, INPUT);      // Make pin INPUT
   digitalWrite(sensorIn, LOW);   // Turn off internal pullups
   if (!digitalRead(sensorIn)) {  // Pin goes HIGH which means its on the black line
      return 1;
   }
   if(digitalRead(sensorIn)){     // Pin goes LOW which means its not on the black line
      return 0;
   }
}
/*
  Taking the position of each of the 5 sensors into account, I arranged the 
  function to produce a binary digit that is being created as base 2 but is 
  stored as its base 10 value. This allows us to produce a single variable 
  state for our entire array of line sensors as opposed to having to create
  a for loop that would ruin the continuity of multi tasking for the robot. 
*/
int sensorState() { 
  int currentState = 0;
  currentState = ((isOnLine(Pin6)*pow(2,4)) + (isOnLine(Pin5)*pow(2,3)) + (isOnLine(Pin4)*pow(2,2)) + (isOnLine(Pin3)*pow(2,1)) + isOnLine(Pin2));
  Serial.println(currentState);
  return currentState;
}
//PATTERN RECOGNITION=========================================================================================
void drive(int state) {
 if (state == 4) {    //00100
  //straight();
  Serial.println("Straight");
 }
 if (state == 0) {    //00000
  //Straight();
  Serial.println("Start Area");
 } 
 if (state == 28) {   //11100
  leftTurn();
  Serial.println("Left turn");
 } 
 if (state == 7) {   //00111
  rightTurn();
  Serial.println("Right turn");
 } 
 if (state == 31) {   //11111 second time we encounter this it has to be a right turn
    //straight();  
    if (currentTime < 9500) {
      Serial.println("Straight");
      //straight();
    }
    if ((currentTime > 9500) && (currentTime < 25000)) { 
      Serial.println("Right turn");
      rightTurn();
    }
    if (currentTime > 25000) {
      Serial.println("Left turn");
      leftTurn();
    }
  
 } 
 //Correction routines-------------
 if (state == 0) {    //01100
  //slightRight();
 } 
 if (state == 0) {    //01000
  //slightRight();
 } 
 if (state == 0) {    //00110
  //slightLeft();
 }  
 if (state == 0) {    //00010
  //slightLeft();
 } 
}
//MOTOR FUNCTIONS=============================================================================================

void forward() {  //Moves the robot forward
  //md.setM1Speed(-300); //right side //More powerful
  //md.setM2Speed(380);//left side
}
void backward() { //Moves te robot backwards
  //md.setM1Speed(300); //right side
  //md.setM2Speed(-380);//left side
}
void leftTurn() { //Turns the robot left from the center
  //md.setM1Speed(-300); //Forward right side
  //md.setM2Speed(-380);//Reverse left side
  delay(60);
}
void rightTurn() { //Turns the robot right from the center
 //md.setM1Speed(300); //Reverse right side
 //md.setM2Speed(380);//Forward left side
 delay(60);
}
void stopMotors() {//Stop the robot
 //md.setM1Speed(0);
 delayMicroseconds(10);
 //md.setM2Speed(0);
 delayMicroseconds(10);
}

//END OF MOTOR FUNCTIONS===============================================================
void inputValues() {
  Serial.print("Sensor 1: ");
  Serial.print(isOnLine(Pin2));   // Connect to pin 2, display results
  Serial.print(", Sensor 2: ");
  Serial.print(isOnLine(Pin3));   // Same for these
  Serial.print(", Sensor 3: ");
  Serial.print(isOnLine(Pin4));
  Serial.print(", Sensor 4: ");
  Serial.print(isOnLine(Pin5));
  Serial.print(", Sensor 5: ");
  Serial.print(isOnLine(Pin6));
  Serial.println();
}
//VIRTUAL TRACK=============================================================================
/*
  This function is a copy of isOnLine but is only used to test the track path
  pattern.
*/
int testSensor(int tPin0, int tPin1, int tPin2, int tPin3, int tPin4) {
  int currentState = 0;
  currentState = ((tPin0*pow(2,4)) + (tPin1*pow(2,3)) + (tPin2*pow(2,2)) + (tPin3*pow(2,1)) + tPin4);
  //Serial.println(currentState);
  return currentState;
}
/*
  Produces a virtual track that mimics the real thing.
  It contains the same patterns the robot would 
  encounter on the track.
*/
void virtualTrack() {
  if ((currentTime > 0) && (currentTime < 1000)) { //Start 00000
    drive(testSensor(0,0,0,0,0));
  }
  if ((currentTime > 1000) && (currentTime < 1500)) { //Out of start Area 11111
    drive(testSensor(1,1,1,1,1));
  }
  if ((currentTime > 1500) && (currentTime < 4000)) { //First Straight 00100
    drive(testSensor(0,0,1,0,0));
  }
  if ((currentTime > 4000) && (currentTime < 4100)) { //First Left Turn 11100
    drive(testSensor(1,1,1,0,0));
  }
  //First Quadrant END============================================================================
  if ((currentTime > 4100) && (currentTime < 6000)) { //Second Straight 00100
    drive(testSensor(0,0,1,0,0));
  }
  if ((currentTime > 6000) && (currentTime < 6100)) { //Ignore turn and go Straight 11100
    drive(testSensor(1,1,1,0,0));
  }
  if ((currentTime > 6100) && (currentTime < 8400)) { //Third Straight 00100
    drive(testSensor(0,0,1,0,0));
  }
  if ((currentTime > 8400) && (currentTime < 8500)) { //Second Turn Left 11100
    drive(testSensor(1,1,1,0,0));
  }
  //Second Quadrant END==========================================================================
  if ((currentTime > 8500) && (currentTime < 9500)) { //Keep Straight 00100
    drive(testSensor(0,0,1,0,0));
  }
  if ((currentTime > 9500) && (currentTime < 9600)) { //Third Left turn 11100
    drive(testSensor(1,1,1,0,0));
  }
  //Third Quadrant END===========================================================================
  if ((currentTime > 9600) && (currentTime < 10600)) { //Straight short 00100
    drive(testSensor(0,0,1,0,0));
  }
  if ((currentTime > 10600) && (currentTime < 10700)) { //First right turn 11111
    drive(testSensor(1,1,1,1,1));
  }
  if ((currentTime > 10700) && (currentTime < 11700)) { //Second straight short 00100
    drive(testSensor(0,0,1,0,0));
  }
  if ((currentTime > 11700) && (currentTime < 11800)) { //Second right turn 00111
    drive(testSensor(0,0,1,1,1));
  }
  if ((currentTime > 11800) && (currentTime < 12800)) { //Third straight short 00100
    drive(testSensor(0,0,1,0,0));
  }
  if ((currentTime > 12800) && (currentTime < 12900)) { //Fourth Left turn 11100
    drive(testSensor(1,1,1,0,0));
  }
  //Fourth Quadrant END==================================================================================
  if ((currentTime > 12900) && (currentTime < 13900)) { //Keep Straight 00100
    drive(testSensor(0,0,1,0,0));
  }
  if ((currentTime > 13900) && (currentTime < 14000)) { //Fifth Left turn 11100
    drive(testSensor(1,1,1,0,0));
  }
  //Fifth Quadrant END=========================================================================
  if ((currentTime > 14000) && (currentTime < 15900)) { //Keep Straight 00100
    drive(testSensor(0,0,1,0,0));
  }
  if ((currentTime > 15900) && (currentTime < 16000)) { //Sixth Left turn 11100
    drive(testSensor(1,1,1,0,0));
  }
//END OF VIRTUAL TRACK==========================================================================
}
/*
  Robot path
  00000 Start
  11111 Out of the start area
  00100 first straight
  11100 first left turn
  |OOOOOOOO|>>>>>>>>>>>>>>>>>>Drop rings 1 and 2
  =====First Quadrant End-----------------------------
  
  00100 second straight
  11100 ignore and go straight
  00100 third straight
  11100 second turn left 
  |OOOOOOOO|>>>>>>>>>>>>>>>>>>Drop rings 3 and 4
  =====Second Quadrant End----------------------------
  
  00100 go straight
  11100 third turn left
  |OOOOOOOO|>>>>>>>>>>>>>>>>>>Drop ring 5
  =====Third Quadrant End-----------------------------
  
  00100 go straight SHORT
  11111 turn right
  00100 go straight SHORT
  |OOOOOOOO|>>>>>>>>>>>>>>>>>>Drop ring 6
  00111 turn right
  00100 go straight SHORT
  =====Fourth Quadrant End----------------------------
  
  11100 turn left
  |OOOOOOOO|>>>>>>>>>>>>>>>>>>Drop ring 7
  00100 go straight
  11100 turn left
  |OOOOOOOO|>>>>>>>>>>>>>>>>>>Drop rings 8 and 9
  =====Fifth Quadrant END-----------------------------
  
  00100 go straight
  11100 turn left
  =====Sixth Quadrant END-----------------------------
  
  00100 go straight
  11111 turn right
  00100 go straight
  11100 turn left
  00100 go straight
  |OOOOOOOO|>>>>>>>>>>>>>>>>>>Drop ring 11
  11100 turn left
  00100 go straight
  11111 turn left
  00100 go straight
  11111 ignore and go straight
  00100 straight very short
  11111 turn left
  =====Inside track END-------------------------------
  
  00100 go straight
  11100 left turn
  |OOOOOOOO|>>>>>>>>>>>>>>>>>>Drop ring 12
  00100 go straight
  11111 END!
  //END=======================================================================================================
  
  
*/
