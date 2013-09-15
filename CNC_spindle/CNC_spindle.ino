// Controlling a servo position using a potentiometer (variable resistor) 
// for the high speed brushless spindle by Alex Shure, 10.11.2011
// Update with 980-2000 pulse range, 20120801 Alex Shure
// Update with 980-1900 PPM range, without pot, 20130716 Alex Shure

#include <Servo.h> 
 
Servo myservo;  // create servo object to control a servo 
 
int potPin = 0;  // analog pin used to connect the potentiometer
int spindleStatePin=10;
int val;    // variable to read the value from the analog pin 
int spindleState=0;


void setup() 
{ 
  myservo.attach(9,980,1900);  // attaches the servo on pin 9 to the servo object 
  pinMode(spindleStatePin,INPUT);
  pinMode(potPin,INPUT);
  digitalWrite(spindleStatePin,HIGH); //activate pull-up
} 
 
void loop() 
{ 
  spindleState=digitalRead(spindleStatePin);
//  val = analogRead(potPin);            // reads the value of the potentiometer (value between 0 and 1023) 
//  val = map(val, 0, 1023, 0, 179);     // scale it to use it with the servo (value between 0 and 180) 
  val = 179;
  if (spindleState==0){
  myservo.write(val);                  // sets the servo position according to the scaled value 
  digitalWrite(13,HIGH);
  }
  if (spindleState==1){
  myservo.write(0);  // sets the servo position to 90
  digitalWrite(13,LOW);
  }
  
  delay(10);                           // waits for the servo to get there 
} 
