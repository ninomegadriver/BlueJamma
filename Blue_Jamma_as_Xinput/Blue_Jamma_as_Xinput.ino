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
 * DPad or Left Analog     => Up, Down, Left, Right
 * X                       => Button 1 / Low Punch
 * Y                       => Button 2 / Medium Punch
 * R Shoulder              => Button 3 / Fierce Punch
 * A                       => Button 4 / Low Kick
 * B                       => Button 5 / Medium Kick
 * R Trigger               => Button 6 / Fierce Kick
 * Left button             => Not used (Usually Punch x3)
 * Left trigger            => Not used (Usually Kick  x3)
 * Left thumb stick        => Not used
 * Right thumb stick       => Not used
 * 
 * PA9                     => Swap player controls
 * PA10                    => Switch between DPAD or Analog Mode
 * 
 * For a quick reference check:
 * https://github.com/ninomegadriver/BlueJamma
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

// Sometimes Windows or some specific game may
// randonly change the order of connected controllers
// to workwarround this problem we can swapped the
// controles at any time by changing the status
// o PA9 gpio
uint8_t idPlayer1=0;
uint8_t idPlayer2=1;

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


  // Initialize the GPIOs
  for(uint8_t i=0;i<sizeof jammaPins;i++) {
    pinMode(jammaPins[i], INPUT_PULLUP);
  }

  pinMode(PA9 , INPUT_PULLUP); // Control the player order
  pinMode(PA10, INPUT_PULLUP); // Control the directional mode

  // Initialize the Xbox object;
  USBComposite.setManufacturerString("\xA9Microsoft");
  USBComposite.setProductString("Controller");
  
  x360.registerComponent();
  USBComposite.begin();
  while (!USBComposite);  
  x360.controllers[0].setRumbleCallback(rumble);
  
}

// Update button status
void updateButton(uint8_t controllerId, uint8_t buttonId, uint8_t jammaPin){
  uint8_t currentState = digitalRead(jammaPins[jammaPin]);
  if(currentState != lastState[jammaPin]){
    if(currentState == LOW) x360.controllers[controllerId].button(buttonId,1);
    else                    x360.controllers[controllerId].button(buttonId,0);
    lastState[jammaPin] = currentState;
  }
}

void loop() {

  // Check the order of the controllers
  if(digitalRead(PA9) == LOW){
    idPlayer1 = 1;
    idPlayer2 = 0;
  }else{
    idPlayer1 = 0;
    idPlayer2 = 1;
  }
  
    updateButton(idPlayer1,XBOX_L3,       0);  // Service
    updateButton(idPlayer1,XBOX_R3,       1);  // Test
    updateButton(idPlayer1,XBOX_BACK,     2);  // COIN1
    updateButton(idPlayer2,XBOX_START,    3);  // P2 START
    updateButton(idPlayer1,XBOX_START,    4);  // P1 START

  // Check the directional mode.
  if(digitalRead(PA10) == LOW){
    
    if(digitalRead(jammaPins[5])  == LOW)       x360.controllers[idPlayer2].Y(+32767); // P2 UP
    else if(digitalRead(jammaPins[7])  == LOW)  x360.controllers[idPlayer2].Y(-32767); // P2 DOWN
    else                                        x360.controllers[idPlayer2].Y(0);      // P2 ZERO Y
    
    if(digitalRead(jammaPins[6])  == LOW)       x360.controllers[idPlayer1].Y(+32767); // P1 UP
    else if(digitalRead(jammaPins[8])  == LOW)  x360.controllers[idPlayer1].Y(-32767); // P1 DOWN
    else                                        x360.controllers[idPlayer1].Y(0);      // P1 ZERO Y

    if(digitalRead(jammaPins[9])  == LOW)       x360.controllers[idPlayer2].X(-32767); // P2 LEFT
    else if(digitalRead(jammaPins[11]) == LOW)  x360.controllers[idPlayer2].X(+32767); // P2 RIGHT
    else                                        x360.controllers[idPlayer2].X(0);      // P2 ZEROX

    if(digitalRead(jammaPins[10]) == LOW)       x360.controllers[idPlayer1].X(-32767);  // P1 LEFT
    else if(digitalRead(jammaPins[12]) == LOW)  x360.controllers[idPlayer1].X(+32767);  // P1 RIGHT
    else                                        x360.controllers[idPlayer1].X(0);       // P1 ZERO X

  } else {

    x360.controllers[idPlayer2].Y(0);
    x360.controllers[idPlayer1].Y(0);
    x360.controllers[idPlayer2].X(0);
    x360.controllers[idPlayer1].X(0);
    updateButton(idPlayer2,XBOX_DUP,      5);  // P2 UP
    updateButton(idPlayer1,XBOX_DUP,      6);  // P1 UP
    updateButton(idPlayer2,XBOX_DDOWN,    7);  // P2 DOWN
    updateButton(idPlayer1,XBOX_DDOWN,    8);  // P1 DOWN
    updateButton(idPlayer2,XBOX_DLEFT,    9);  // P2 LEFT
    updateButton(idPlayer1,XBOX_DLEFT,    10); // P1 LEFT
    updateButton(idPlayer2,XBOX_DRIGHT,   11); // P2 RIGHT
    updateButton(idPlayer1,XBOX_DRIGHT,   12); // P1 RIGHT
    
  }

   updateButton(idPlayer1,XBOX_X,        14); // P1 Button 1
   updateButton(idPlayer1,XBOX_Y,        16); // P1 Button 2
   updateButton(idPlayer1,XBOX_RSHOULDER,18); // P1 Button 3
  
   updateButton(idPlayer2,XBOX_X,        13); // P2 Button 1
   updateButton(idPlayer2,XBOX_Y,        15); // P2 Button 2
   updateButton(idPlayer2,XBOX_RSHOULDER,17); // P2 Button 3


   updateButton(idPlayer2,XBOX_A,        19); // P2 Button 4
   updateButton(idPlayer2,XBOX_B,        21); // P2 Button 5
   if(digitalRead(jammaPins[23])  == LOW) x360.controllers[idPlayer2].sliderRight(32767); // P2 Button 6
   else                                   x360.controllers[idPlayer2].sliderRight(0);
    
   updateButton(idPlayer1,XBOX_A,        20); // P1 Button 4
   updateButton(idPlayer1,XBOX_B,        22); // P1 Button 5
   if(digitalRead(jammaPins[24])  == LOW) x360.controllers[idPlayer1].sliderRight(32767); // P1 Button 6
   else                                   x360.controllers[idPlayer1].sliderRight(0);
    
   x360.controllers[idPlayer1].send();
   x360.controllers[idPlayer2].send();
   delay(8); // deBounce  
    
}
