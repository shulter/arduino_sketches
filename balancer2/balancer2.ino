//Audio in with 38.5kHz sampling rate, interrupts, and clipping indicator
//by Amanda Ghassaei
//http://www.instructables.com/id/Arduino-Audio-Input/
//Sept 2012

/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
*/
// #define DEBUG
#define mid 107
#define UPD 100
int clipValue = 5;
//variable to store incoming audio sample
volatile byte incomingAudio;
//clipping indicator variables
volatile boolean clipping = 0;


void setup(){
  #ifdef DEBUG
    Serial.begin(115200); 
    Serial.print("init.");
  #endif
  pinMode(A1,OUTPUT);
  pinMode(A2,INPUT);
  pinMode(A3,OUTPUT);
  digitalWrite(A1,LOW);
  digitalWrite(A3,HIGH); //^pot
  pinMode(13,OUTPUT);//led indicator pin
  pinMode(5,OUTPUT);//led indicator pin
  cli();//disable interrupts
  //set up continuous sampling of analog pin 0
  //clear ADCSRA and ADCSRB registers
  ADCSRA = 0;
  ADCSRB = 0;
  ADMUX |= (1 << REFS1); //set reference voltage, REFS1=1.1
  ADMUX |= (1 << ADLAR); //left align the ADC value- so we can read highest 8 bits from ADCH register only
  ADCSRA |= (1 << ADPS2) | (1 << ADPS0); //set ADC clock with 32 prescaler- 16mHz/32=500kHz
  ADCSRA |= (1 << ADATE); //enabble auto trigger
  ADCSRA |= (1 << ADIE); //enable interrupts when measurement complete
  ADCSRA |= (1 << ADEN); //enable ADC
  ADCSRA |= (1 << ADSC); //start ADC measurements

  sei();//enable interrupts
  //  analogReference(INTERNAL);
  

}

ISR(ADC_vect) {//when new ADC value ready
  incomingAudio = ADCH;//store 8 bit value from analog pin 0, 107 is mid at 0.4313V 107down, 148up
  if (int ((incomingAudio <= mid-clipValue) || (incomingAudio >= mid+clipValue))){//if clipping above or below the threshhold
//    digitalWrite(13,HIGH);//set pin 13 high
    PORTD = (1<<PIND5); //5
    clipping = 1;//currently clipping
  }
}

void loop(){
  if (clipping){//if currently clipping
    clipping = 0;//
//    digitalWrite(13,LOW);//turn off clipping led indicator (pin 13)
    PORTD = (0<<PIND5); //5
    PORTB = (0<<PINB5); //13
  }
//  clipValue=analogRead(A2);
  #ifdef DEBUG
  Serial.print("A0: ");
  Serial.print(incomingAudio);
  Serial.print("\t A2: ");
  Serial.println(clipValue);
  #endif
  delayMicroseconds(UPD);
}




