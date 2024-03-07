/*
 * Blue Jamma as Xinput
 * Nino MegaDriver nino@nino.com.br
 * License: GPL 3.0
 * 
 * EN:
 * Firmware for the Blue Jamma project based on Arduino IDE.
 * Also compatible with the Blue Pill and other STM32F103C8T6 based boards.
 * 
 * This firmware turns the board into two Xbox 360 Controllers using the Xinput
 * protocol. Buttons mapped following the USB arcade sticks layout:
 * 
 * XBox 360 Button         => Jamma Button
 * Left  thumb stick click => Service
 * Right thumb stick click => Test
 * Start                   => Start
 * Back                    => Coin
 * Directional Pad         => Up, Down, Left, Right
 * X                       => Button 1 / Low Punch
 * Y                       => Button 2 / Medium Punch
 * L Shoulder/Button       => Button 3 / Fierce Punch
 * A                       => Button 4 / Low Kick
 * B                       => Button 5 / Medium Kick
 * R Shoulder/Button       => Button 6 / Fierce Kick
 * Left thumb stick        => Not used
 * Right thumb stick       => Not used
 * Left trigger            => Not used
 * Right trigger           => Not used
 * 
 * Arduino core used:
 * Arduino_STM32 - https://github.com/rogerclarkmelbourne/Arduino_STM32
 * 
 * Libraries used:
 * USB Composite - https://github.com/arpruss/USBComposite_stm32f1
 * 
*/

#include <USBComposite.h>

USBMultiXBox360<2> x360;

uint8_t lastState[27];

uint8_t jammaPins[] = {
//  GPIO, // Jamma Button
    PA0,  // Service
    PA1,  // Test
    PA2,  // COIN1
    PA3,  // P2 START
    PA4,  // P1 START
    PA5,  // P2 UP
    PA6,  // P1 UP
    PA7,  // P2 DOWN
    PB0,  // P1 DOWN
    PB1,  // P2 LEFT
    PB10, // P1 LEFT
    PB11, // P2 RIGHT
    PB12, // P1 RIGHT
    PB13, // P2 Button 1
    PB14, // P1 Button 1
    PB15, // P2 Button 2
    PA8,  // P1 Button 2
    PA15, // P2 Button 3
    PB3,  // P1 Button 3
    PB4,  // P2 Button 4
    PB5,  // P1 Button 4
    PB6,  // P2 Button 5
    PB7,  // P1 Button 5
    PB8,  // P2 Button 6
    PB9,  // P1 Button 6
};

// Simulates rumble over the onboard LED
void rumble(uint8 left, uint8 right) {
  digitalWrite(PC13, left+right==0);
}

void setup() {

  // Turn the onBoard LED on
  pinMode(PC13, OUTPUT);
  digitalWrite(PC13, LOW);

  pinMode(PC15, OUTPUT);

  // Initialize the GPIOs
  for(uint8_t i=0;i<sizeof jammaPins;i++) {
    // The Blue Jamma has external PULL UPs on all inputs for better
    // current control over the Jamma harness. If you intent to use
    // this code with a generic Blue Pill, you'll have to set the
    // pins to INPUT_PULLUP
    pinMode(jammaPins[i], INPUT);
  }

  // Initialize the Xbox object;
  USBComposite.setManufacturerString("Nino MegaDriver");
  USBComposite.setProductString("Blue Jamma");
  
  x360.registerComponent();
  USBComposite.begin();
  while (!USBComposite);  
  x360.controllers[0].setRumbleCallback(rumble);
}

// Update 
void updateButton(uint8_t controllerId, uint8_t buttonId, uint8_t jammaPin){
  uint8_t currentState = digitalRead(jammaPins[jammaPin]);
  if(currentState != lastState[jammaPin]){
    if(currentState == LOW) x360.controllers[controllerId].button(buttonId,1);
    else                    x360.controllers[controllerId].button(buttonId,0);
    lastState[jammaPin] = currentState;
  }
  
}

void loop() {
    updateButton(0,XBOX_L3,       0);  // Service
    updateButton(0,XBOX_R3,       1);  // Test
    updateButton(0,XBOX_BACK,     2);  // COIN1
    updateButton(1,XBOX_START,    3);  // P2 START
    updateButton(0,XBOX_START,    4);  // P1 START
    updateButton(1,XBOX_DUP,      5);  // P2 UP
    updateButton(0,XBOX_DUP,      6);  // P1 UP
    updateButton(1,XBOX_DDOWN,    7);  // P2 DOWN
    updateButton(0,XBOX_DDOWN,    8);  // P1 DOWN
    updateButton(1,XBOX_DLEFT,    9);  // P2 LEFT
    updateButton(0,XBOX_DLEFT,    10); // P1 LEFT
    updateButton(1,XBOX_DRIGHT,   11); // P2 RIGHT
    updateButton(0,XBOX_DRIGHT,   12); // P1 RIGHT
    updateButton(1,XBOX_X,        13); // P2 Button 1
    updateButton(0,XBOX_X,        14); // P1 Button 1
    updateButton(1,XBOX_Y,        15); // P2 Button 2
    updateButton(0,XBOX_Y,        16); // P1 Button 2
    updateButton(1,XBOX_LSHOULDER,17); // P2 Button 3
    updateButton(0,XBOX_LSHOULDER,18); // P1 Button 3
    updateButton(1,XBOX_A,        19); // P2 Button 4
    updateButton(0,XBOX_A,        20); // P1 Button 4
    updateButton(1,XBOX_B,        21); // P2 Button 5
    updateButton(0,XBOX_B,        22); // P1 Button 5
    updateButton(1,XBOX_RSHOULDER,23); // P2 Button 6
    updateButton(0,XBOX_RSHOULDER,24); // P1 Button 6
    x360.controllers[0].send();
    x360.controllers[1].send();
    delay(8); // deBounce  
}
