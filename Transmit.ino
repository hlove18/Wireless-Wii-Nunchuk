//TRANSMIT ARDUINO

#include <Wire.h>
#include <wiinunchuck.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN 9
#define CSN_PIN 10

const uint64_t pipe = 0xE8E8F0F0E1LL;

RF24 radio(CE_PIN, CSN_PIN);

int REDPin = 4;    // RED pin of the LED to pin 4
int GREENPin = 7;  // GREEN pin of the LED to pin 5
int BLUEPin = 8;   // BLUE pin of the LED to pin 6

int Nunchuk[4];
/*
Nunchuk:
[0] Joystick x
[1] Joystick y
[2] C button state
[3] Z button state
*/

void setup() {
  Serial.begin(9600);
  delay(1000);
  
  //initialize RGB LED
  pinMode(REDPin, OUTPUT);
  pinMode(GREENPin, OUTPUT);
  pinMode(BLUEPin, OUTPUT);
  
  digitalWrite(REDPin, HIGH);
  digitalWrite(GREENPin, HIGH);
  digitalWrite(BLUEPin, HIGH);
  
  // Initialize the nunchuck
  nunchuk_setpowerpins();
  nunchuk_init();
  nunchuk_send_request();
  
  radio.begin();
  radio.openWritingPipe(pipe);

}

void loop() {
  nunchuk_get_data();
  
  Nunchuk[0] = nunchuk_joy_x();
  Nunchuk[1] = nunchuk_joy_y();
  Nunchuk[2] = nunchuk_cbutton();
  Nunchuk[3] = nunchuk_zbutton();
  
  radio.write(Nunchuk, sizeof(Nunchuk));
    
  //implementing RGB LED
  //OFF = controller OFF
  //RED = controller ON, but no radio signal
  //YEllOW = controller ON, and joystick is unarmed (Z button state is 0)
  //GREEN = controller ON, and joystick is armed (Z button state is 1)
  
  if (radio.write(Nunchuk, sizeof(Nunchuk)) && Nunchuk[3] == 1) {
    digitalWrite(REDPin, HIGH);
    digitalWrite(GREENPin, LOW);
    digitalWrite(BLUEPin, HIGH);
  }
  
  else if (radio.write(Nunchuk, sizeof(Nunchuk)) && Nunchuk[3] == 0) {
    digitalWrite(REDPin, HIGH);
    digitalWrite(GREENPin, HIGH);
    digitalWrite(BLUEPin, LOW);
  }
  
  else {
    digitalWrite(REDPin, LOW);
    digitalWrite(GREENPin, HIGH);
    digitalWrite(BLUEPin, HIGH);
  }
  
}
