/*
 *
 *Small Joystick emulator using two pcf8574 to gain in total 16 inputs as per spec from Kippers and Kampklar.
 *Based on an Arduino Mirco with the ATmega32u4 processor for native USB and Joystick compabillity in Windows.
 *
 *
 *
 *Ver 2 
 *9/7-16
 * by pommen
 */
#include <PCF8574.h> //PCF8574 bi-directional lib
#include <Wire.h>  //i2c
#include <Joystick.h> //joystick emulator

//i2c adresses and instace names
PCF8574 pcf27(0x27); 
PCF8574 pcf21(0x21);


//Vars:
const byte onboardLed = 9;
int buttons[16];
int prevButtons[16];
boolean state = true;
const byte okLed = 6;

void setup() {
  Joystick.begin();
  Wire.begin();
  pcf27.begin();
  pcf21.begin();

//Serial for bugfixing. Comment out for finnished product.
  Serial.begin(9600);
  Serial.println("Starting...");


  //keep theseled? Onboard led will prolly do fine.
  pinMode(onboardLed, OUTPUT);
  pinMode(okLed, OUTPUT);


//Flash led to indicated that we have loaded the bootleader and is running
  for (int i = 0; i <= 3; i++) {
    digitalWrite(okLed, state);
    state = !state;
    delay(500);
  }


  //fill the arrays with 7's to see any bitfaults after startup.
  for (int i = 0; i < 16; i++) {
    buttons[i] = 7;
    prevButtons[i] = 7;

  }

  //turn light off
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
}

void loop() {

//Fill the buffer with the buttonstates, 16 bits.

  for (int i = 0; i < 16; i++) {

    if (i < 8) {
      digitalWrite(6, HIGH);
      buttons[i] = pcf21.readButton(i); //array 0-7 comes from the first pcf (0x21)

    }
    else { //rest of the array dumps here. More filtering needed?
      digitalWrite(7, HIGH);
      buttons[i] = pcf27.readButton(i - 4);
    }

    if (prevButtons[i] != buttons[i]) { //Check if any of the buttons have changed thei state since last time we looked at them.
      Joystick.setButton(i, buttons[i]); //send the state to comuter as a joystick-button
      prevButtons[i] = buttons[i]; //update the previous state
    }

    digitalWrite(onboardLed, state);
  }


//Serial for debugging, remove in finnished product.
  for (int i = 0; i < 16; i++) {
    Serial.print(buttons[i]);
  }
  Serial.println();

  
  state = !state;
  delay(20); //sets responstime. should this be slower? Need to check cycle time. without the Serial flow running.
}





