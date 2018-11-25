//RECEIVE ARDUINO

#include <Wire.h>
#include <wiinunchuck.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

#define CE_PIN 9
#define CSN_PIN 10

const uint64_t pipe = 0xE8E8F0F0E1LL;

RF24 radio(CE_PIN, CSN_PIN);

Servo ESC;  //create Servo-object
float k = 0.005; //set acceleration constant
float vel = 90;  //declare velocity variable - constrained between 0 and 179
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
  ESC.attach(6);  //attach servo (ESC) to pin 6
  Serial.println("Receiver Starting");
  radio.begin();
  radio.openReadingPipe(1, pipe);
  radio.startListening();
}

void loop() {
  if (radio.available()) {
    bool done = false;
    while (!done) {
      done = radio.read(Nunchuk, sizeof(Nunchuk));
      if (Nunchuk[3]) {  //if Z button is pressed - dead man's switch
        int val = Nunchuk[1];
        val = map(val, 0, 255, 0, 179);  //scale wii nunchuk reading to use with servo
        
        if (val >= 87 && val <= 93) {  //buffer region for neutral joystick position
          Serial.println(vel);
          ESC.write(int(vel));
        }
        
        else {
          float accel = k * (val - 90);  //define acceleration
          vel += accel;
          vel = constrain(vel, 0.0, 179.0);
          ESC.write(int(vel));
          Serial.println(vel);
          //Serial.println(accel);
        }
      }
      
      //else stop motor if Z button is released
      else {
        vel = 90;
        Serial.println("ON");
        ESC.write(90);  //stop motor from turning
        delay(15);
      }
    }
  }
  else {
    Serial.println("No radio available");
  }
  
}
