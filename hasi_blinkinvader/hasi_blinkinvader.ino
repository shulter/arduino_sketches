/*
 This code is in the Open Source ISC License.
 Feel free to do anything you want to, but change the name of the project.
 201305171614 Alex Shure
 */

#define ARRAY_SIZE 500  // size of the playback
#define SAMPLE_RATE 50  // ms delay for the array playback

byte brightness = 0;
const int gain = 255; // 0..255, whereas 255 equals a gain of 1 - thus this is an attenuation.
const int rPin = 3; // arms up \o/
const int gPin = 5; // arms down /o\
int cp=0;

void l(byte _pin, byte _br=255){
  analogWrite(_pin, _br);
}

void invade(int _m=255,int _s=10){ 
int _i=brightness;
if ( _m > brightness ){
  for(_i = brightness ; _i <= _m; _i+=1) { 
    l(rPin,_i); // set lower arm's PWM (range from 0 to 255):
    l(gPin,(255-_i)); // set upper arm's PWM (range from 0 to 255):
    delayMicroseconds(_s); // stepover duration
  } 
}
else {
  for(_i = brightness ; _i >= _m; _i-=1) { 
    l(rPin,_i); // set lower arm's PWM (range from 0 to 255):
    l(gPin,(255-_i)); // set upper arm's PWM (range from 0 to 255):
    delayMicroseconds(_s); // stepover duration
  } 
}
 brightness=_i;
}

void setup() {
//  Serial.begin(115200);   // initialize the serial communication
  pinMode(rPin, OUTPUT);   // set pins as outputs
  pinMode(gPin, OUTPUT); 
  digitalWrite(rPin,LOW);
  digitalWrite(gPin,LOW);
  }

void loop() {
  unsigned int fadeSpeed=analogRead(0);
  unsigned int interval=analogRead(1);
  interval=interval*10;
  // for debugging purposes:
  interval=1000;
  fadeSpeed=20;
  
  invade(255,fadeSpeed);
  delay(interval);
  invade(0,fadeSpeed);
  delay(interval);
}
