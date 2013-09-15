/*
 
 This code is in the public domain.
 
 */

#define ARRAY_SIZE 500  // size of the flicker array
#define SAMPLE_RATE 50  // ms delay for collecting and reproducing the flicker

const int rgain = 255;
const int rPin = 3;
const int gPin = 5;
const int bPin = 6;
int thisPin=0;
byte brightness;
int cp=0;
PROGMEM prog_uchar candleArray[] = {119,123,125,123,120,186,149,83,94,62,62,67,82,53,62,117,75,101,123,82,99,88,107,85,98,119,114,135,133,140,93,122,83,93,80,98,85,111,103,88,120,78,85,149,107,109,125,115,125,91,107,104,106,106,96,107,119,119,152,122,135,96,91,107,83,74,83,104,69,59,101,103,156,96,103,83,115,117,99,138,127,131,136,128,99,88,88,107,122,120,123,131,138,135,138,133,131,136,138,138,138,138,138,140,138,138,138,138,138,136,131,127,125,123,122,120,119,117,112,112,114,112,112,77,65,109,104,96,53,53,61,59,65,69,82,94,65,83,93,96,101,106,80,72,75,78,53,53,49,51,51,54,59,64,80,78,82,93,96,83,86,75,53,45,46,46,53,56,56,57,59,56,49,88,64,96,77,80,90,93,67,64,83,88,90,85,78,49,49,43,46,51,49,54,53,54,56,57,57,56,64,74,65,59,80,98,64,54,41,45,69,94,93,82,62,51,59,67,53,69,56,53,49,51,54,54,57,54,56,59,65,54,54,48,57,51,43,41,51,90,99,86,65,61,80,82,85,96,101,85,59,48,46,48,59,78,54,46,43,59,57,61,74,80,61,69,88,78,75,78,74,69,88,75,85,93,91,86,88,94,96,119,94,56,90,82,70,75,80,74,74,53,78,65,46,54,54,56,82,75,70,59,51,64,78,96,90,80,77,67,74,80,75,72,64,61,64,70,82,91,98,99,99,91,82,70,91,67,85,69,38,53,59,91,67,77,64,86,88,90,93,80,70,69,69,65,59,67,114,135,144,138,143,151,157,149,115,101,107,77,70,69,85,85,65,86,85,85,86,90,90,91,94,86,98,85,101,88,90,98,104,109,109,107,104,106,111,101,109,49,111,85,72,80,85,103,90,65,85,94,96,101,104,104,99,96,106,96,103,106,127,131,140,135,136,146,141,143,136,140,140,133,130,106,78,53,35,17,25,67,59,65,103,74,74,59,46,48,69,65,74,88,72,112,91,109,114,98,117,103,86,90,94,85,77,119,83,45,54,65,93,85,64,64,64,64,64,62,65,94,130,146,136,136,144,148,136,133,123,99,85,83,86,85,80,83,85,3,52,8,58,126,255,9,96,62,62,40,16,142};

void candle(){
  while(1){
  unsigned int _rbr = map(candleArray[cp], 0, 255, 2, rgain);
  
  analogWrite(rPin,_rbr);
  cp++;
  if( cp >= ( ARRAY_SIZE - 2 ) ) // if the end of the stored array has been reached
  { 
   cp = 0;          // start again from the beginning
  }
  delay( SAMPLE_RATE );
  }
}

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
  candle();
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


