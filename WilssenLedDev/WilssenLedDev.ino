/*
 
 This code is in the public domain.
 
 */
// These constants won't change.  They're used to give names
// to the pins used:
const int rPin = 3;
const int gPin = 5;
const int bPin = 6;
int thisPin=0;
byte brightness;


void fadeUp(int _m=255,int _s=10){ 
int _i=0;
  for(_i = 0 ; _i <= _m; _i+=1) { 
    // sets the value (range from 0 to 255):
 setLed(_i);
    // wait to see the dimming effect    
    delay(_s);
  } 
 brightness=_i;
}

void fadeDown(int _m=0,int _s=10){ 
  int _i=brightness;
  for(int _i =  brightness ; _i >= _m; _i-=1) { 
    // sets the value (range from 0 to 255):
 setLed(_i);
    // wait to see the dimming effect    
    delay(_s);                            
  } 
 brightness=_i;
}

void bl(int _i=10,int _s=50,int _t=50){ // fade blink
int _sUp=_s/_i; 
int _sDown=_t/_i;
  fadeUp(_i,_sUp);
  fadeDown(0,_sDown);
}

void setup() {
  // initialize the serial communication:
  Serial.begin(115200);
  // set pins 2 through 13 as outputs:
  pinMode(rPin, OUTPUT); 
  pinMode(gPin, OUTPUT); 
  pinMode(bPin, OUTPUT); 
  digitalWrite(rPin,LOW);
  digitalWrite(gPin,LOW);
  digitalWrite(bPin,LOW);
  thisPin=rPin;
  bl(20);
  delay(200);
  bl(40);
  delay(200);
  bl(60);  
  delay(200);
  }

void loop() {
  // iterate over the pins:

  //  testPWM();
  readSerial();

}

void readSerial(){
  if (Serial.available()) {
    // read the most recent byte (which will be from 0 to 255):
    int _ser = Serial.read();
    Serial.print("Set ");
    Serial.println(_ser,DEC);
    // set the brightness of the LED:
    bl(_ser,10000,1000);

  }
}

void setLed(byte _br){
  unsigned int _r=_br*255/255;
  unsigned int _g=_br*240UL/255UL;
  unsigned int _b=_br*255UL/255UL;
  analogWrite(rPin, _r);
  analogWrite(gPin, _g);
  analogWrite(bPin, _b);
}

void testPWM(){
  analogWrite(thisPin, 1);
  delay(500);

  analogWrite(thisPin, 2);
  delay(500);

  analogWrite(thisPin, 3);
  delay(500);

  analogWrite(thisPin, 4);
  delay(500);

  analogWrite(thisPin, 5);
  delay(500);

  analogWrite(thisPin, 6);
  delay(500);

  analogWrite(thisPin, 0);
  delay(1000);

}


