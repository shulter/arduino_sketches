/*
 */

/**
 * LED Remote
 * Every time the buttons change on the remote, the entire state of
 * buttons is send to the led board, which displays the state.
 */

#include <SPI.h>
#include "nRF24L01.h"
#include "printf.h"
#include <RF24.h>
#include <SPI.h>
#include <Adafruit_NeoPixel.h>

#define LEDPIN 6
//
// Hardware configuration
//

// Set up nRF24L01 radio on SPI bus plus pins 9 & 10
Adafruit_NeoPixel leds = Adafruit_NeoPixel(4, LEDPIN, NEO_GRB + NEO_KHZ800);

RF24 radio(A0,8); // CE, CS. CE at pin A0, CSN at pin 8

// sets the role of this unit in hardware.  Connect to GND to be the 'led' board receiver
// Leave open to be the 'remote' transmitter
const int role_pin = A4;

// Pins on the remote for buttons
const uint8_t button_pins[] = { 2,3,4,5,7 };
const uint8_t num_button_pins = sizeof(button_pins);

//
// Topology
//

// Single radio pipe address for the 2 nodes to communicate.
const uint64_t pipe = 0xE8E8F0F0E1LL;

//
// Role management
//
// Set up role.  This sketch uses the same software for all the nodes in this
// system.  Doing so greatly simplifies testing.  The hardware itself specifies
// which node it is.
//
// This is done through the role_pin
//

// The various roles supported by this sketch
typedef enum { role_remote = 1, role_led } role_e;

// The debug-friendly names of those roles
const char* role_friendly_name[] = { "invalid", "Remote", "LED Board"};

// The role of the current running sketch
role_e role;

//
// Payload
//

uint8_t button_states[num_button_pins];
uint8_t led_states[12];

//
// Setup
//

void setup(void)
{
  //
  // Role
  //

  // set up the role pin
  pinMode(role_pin, INPUT);
  digitalWrite(role_pin,HIGH);
  delay(20); // Just to get a solid reading on the role pin

  // read the address pin, establish our role
  if ( digitalRead(role_pin) )
    role = role_remote;
  else
    role = role_led;

  //
  // Print preamble
  //

  pinMode(A1, OUTPUT); // GND for the NRF24 module
  digitalWrite(A1, LOW); // GND for the NRF24 module
  pinMode(7, OUTPUT); // VCC for the NRF24 module
  digitalWrite(7,HIGH); // Power up the NRF24 module
  Serial.begin(115200);
  delay(32);
  
  
  role = role_remote;
  //role = role_led;
  
  
  Serial.begin(115200);
  printf_begin();
  printf("\n\rRF24 led_remote/\n\r");
  printf("ROLE: %s\n\r",role_friendly_name[role]);

  //
  // Setup and configure rf radio
  //

  radio.begin();
  // The amplifier gain can be set to RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_MED=-6dBM, and RF24_PA_HIGH=0dBm.
  radio.setPALevel(RF24_PA_HIGH); // transmitter gain value (see above)
  //
  // Open pipes to other nodes for communication
  //

  // This simple sketch opens a single pipes for these two nodes to communicate
  // back and forth.  One listens on it, the other talks to it.

  if ( role == role_remote )
  {
    radio.openWritingPipe(pipe);
  }
  else
  {
    radio.openReadingPipe(1,pipe);
  }

  //
  // Start listening
  //

  if ( role == role_led )
    radio.startListening();

  //
  // Dump the configuration of the rf unit for debugging
  //

  radio.printDetails();

  //
  // Set up buttons / LED's
  //

  // Set pull-up resistors for all buttons
  if ( role == role_remote )
  {
    int i = num_button_pins;
    while(i--)
    {
      pinMode(button_pins[i],INPUT);
      digitalWrite(button_pins[i],HIGH);
    }
  }

  // Turn LED's ON until we start getting keys
  if ( role == role_led )
  {
      leds.begin();
  leds.show(); // Initialize all pixels to 'off'
  colorWipe(leds.Color(50, 0, 0), 32); // Red
  colorWipe(leds.Color(0, 50, 0), 32); // Green
  colorWipe(leds.Color(0, 0, 50), 32); // Blue
  colorWipe(leds.Color(0, 0, 0), 1); // Blue
  leds.setPixelColor(0, leds.Color(32, 0, 0));
  leds.show();
   }

}

void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<leds.numPixels(); i++) {
      leds.setPixelColor(i, c);
      leds.show();
      delay(wait);
  }
}

//
// Loop
//
void loop(void)
{
  //
  // Remote role.  If the state of any button has changed, send the whole state of
  // all buttons.
  //

  if ( role == role_remote )
  {
    // Get the current state of buttons, and
    // Test if the current state is different from the last state we sent
    int i = num_button_pins;
    bool different = false;
    while(i--)
    {
      uint8_t state = ! digitalRead(button_pins[i]);
      if ( state != button_states[i] )
      {
        different = true;
        button_states[i] = state;
      }
    }

    // Send the state of the buttons to the LED board
    if ( different )
    {
      printf("Now sending...");
      bool ok = radio.write( button_states, num_button_pins );
      if (ok)
        printf("ok\n\r");
      else
        printf("failed\n\r");
    }

    // Try again in a short while
    delay(20);
  }

  //
  // LED role.  Receive the state of all buttons, and reflect that in the LEDs
  //

  if ( role == role_led )
  {
    // if there is data ready
    if ( radio.available() )
    {
      leds.setPixelColor(0, leds.Color(0,32,0));
      leds.show();
      
      bool done = false;
      while (!done)
      {
        // Fetch the payload, and see if this was the last one.
        done = radio.read( button_states, num_button_pins );

        // Spew it
        printf("Got buttons\n\r");

        // For each button, if the button now on, then toggle the LED
        uint32_t c=leds.Color(button_states[0]*200, button_states[1]*200, button_states[2]*200);
        leds.setPixelColor(3, c);
        leds.show();
      }
    }
  }
}
// vim:ai:cin:sts=2 sw=2 ft=cpp
