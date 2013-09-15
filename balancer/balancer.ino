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

//variable to store incoming audio sample
byte incomingAudio;
const int ledPin = 6; // led connected to digital pin 13

//clipping indicator variables
boolean clipping = 0;

void setup(){
  
  pinMode(13,OUTPUT);//led indicator pin
  
  cli();//disable interrupts
  
  //set up continuous sampling of analog pin 0
  
  //clear ADCSRA and ADCSRB registers
  ADCSRA = 0;
  ADCSRB = 0;
  
  ADMUX |= (1 << REFS0); //set reference voltage
  ADMUX |= (1 << ADLAR); //left align the ADC value- so we can read highest 8 bits from ADCH register only
  
  ADCSRA |= (1 << ADPS2) | (1 << ADPS0); //set ADC clock with 32 prescaler- 16mHz/32=500kHz
  ADCSRA |= (1 << ADATE); //enabble auto trigger
  ADCSRA |= (1 << ADIE); //enable interrupts when measurement complete
  ADCSRA |= (1 << ADEN); //enable ADC
  ADCSRA |= (1 << ADSC); //start ADC measurements
  
  sei();//enable interrupts

  analogReference(INTERNAL);
  pinMode(ledPin, OUTPUT); // declare the ledPin as as OUTPUT

}

ISR(ADC_vect) {//when new ADC value ready
  incomingAudio = ADCH;//store 8 bit value from analog pin 0
  if (incomingAudio+limit == 0 || incomingAudio == 255){//if clipping
    digitalWrite(13,HIGH);//set pin 13 high
    clipping = 1;//currently clipping
  }
}

void loop(){
  if (clipping){//if currently clipping
    clipping = 0;//
    digitalWrite(13,LOW);//turn off clipping led indicator (pin 13)
  }

  delay(100);
}

