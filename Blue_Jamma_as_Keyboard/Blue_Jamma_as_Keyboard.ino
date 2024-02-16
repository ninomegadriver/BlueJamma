/*
 * Blue Jamma as Keyboard
 * Nino MegaDriver nino@nino.com.br
 * License: GPL 3.0
 * 
 * EN:
 * Firmware for the Blue Jamma project based on Arduino IDE.
 * Also compatible with the Blue Pill and other STM32F103C8T6 based boards.
 * 
 * This firmware turns the board into an USB Keyboard with buttons mapped
 * according to MAME's default. Please note that MAME doesn't have default
 * keys mapped for P2 buttons 5 and 6, you'll have to configure that
 * manually
 * 
 * Following the JAMMA arcade standards, some combos have been defined for
 * ease configuration and navigation as follows:
 * 
 * PT-BR:
 * Firmware para o projeto Blue Jamma baseado na Arduino IDE
 * Também é compatível com a Blue Pill e outras placas baseadas no STM32F103C8T6.
 * 
 * Este firmware transforma a placa em um teclado USB, com os botões mapeados
 * de acordo com o padrão do MAME. Porém, é válido lembrar que o MAME
 * não possue teclas padrão para os botões 5 e 6 do segundo jogador, você
 * deverá configurá-lo manualmente.
 * 
 * Extra Combos:
 * P1 UP    + P1 START => TAB
 * P1 DOWN  + P1 START => ESC
 * P1 RIGHT + P1 START => ENTER
 * P1 LEFT  + P1 START => ESC
 * 
 * 
*/

#include <Keyboard.h>

// STM32F103C8T6 GPIOs vs Buttons Mapping
uint8_t pins[] = {
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

// Key Mappings
const uint8_t keys[] = {
//  Key,                  // Jamma Function  
    KEY_F2,               // SERVICE
    '9',                  // TEST 
    '5',                  // COIN1
    '2',                  // P2 START
    '1',                  // P1 START
    'r',                  // P2 UP
    KEY_UP_ARROW,         // P1 UP
    'f',                  // P2 Down
    KEY_DOWN_ARROW,       // P1 Down
    'd',                  // P2 LEFT
    KEY_LEFT_ARROW,       // P1 LEFT
    'g',                  // P2 RIGHT
    KEY_RIGHT_ARROW,      // P1 RIGHT
    'a',                  // P2 Button 1
    KEY_LEFT_CTRL,        // P1 Button 1
    's',                  // P2 Button 2
    KEY_LEFT_ALT,         // P1 Button 2
    'q',                  // P2 Button 3
    ' ',                  // P1 Button 3
    'w',                  // P2 Button 4
    KEY_LEFT_SHIFT,       // P1 Button 4
    'e',                  // P2 Button 5
    'z',                  // P1 Button 5
    'h',                  // P2 Button 6
    'x',                  // P1 Button 6
};

void setup() {
  
  // Initialize the Keyboard
  Keyboard.begin();

  // Turn the onBoard LED on
  pinMode(PC13, OUTPUT);
  digitalWrite(PC13, LOW);
  
  // Initialize the GPIOs
  for(uint8_t i=0;i<sizeof pins;i++) {
    // The Blue Jamma has external PULL UPs on all inputs for better
    // current control over the Jamma harness. If you intent to use
    // this code with a generic Blue Pill, you'll have to set the
    // pins to INPUT_PULLUP
    pinMode(pins[i], INPUT);
  }
}

int  last_state[27];       // Holds the last checked state of a GPIO
int  current_state[27];    // Holds the current state of a GPIO
long deBounce[27];         // Used for deBouncing the buttons
long deBounceLimit = 1000; // Limit in microsseconds for deBouncing

// Read the state of all GPIOs and populate the array
void updateAllKeys(){
  for(uint8_t i=0;i<sizeof pins;i++){
     current_state[i] = digitalRead(pins[i]);
  } 
}

void loop() {

    // Update Button Status
    updateAllKeys();

    // First check all the combos.
    // If a combo is pressed, send the keys individually and hold the execution to prevent unwanted
    // behaviors 
    if(current_state[4] == LOW && current_state[6] == LOW )       { // Start + P1 UP => TAB (Mame Menu)
      Keyboard.release(keys[4]);
      Keyboard.release(keys[6]);
      Keyboard.press(KEY_TAB);
      delay(8);
      Keyboard.release(KEY_TAB);
      delay(1000);
    }else if (current_state[4] == LOW && current_state[8] == LOW  ) { // Start + P1 DOWN => ESC (Mame Exit)
      Keyboard.release(keys[4]);
      Keyboard.release(keys[8]);
      Keyboard.press(KEY_ESC);
      delay(8);
      Keyboard.release(KEY_ESC);
      delay(1000);
    }else if (current_state[4] == LOW && current_state[10] == LOW  ) { // Start + P1 LEFT => ESC (Generic Use)
      Keyboard.release(keys[4]);
      Keyboard.release(keys[10]);
      Keyboard.press(KEY_ESC);
      delay(8);
      Keyboard.release(KEY_ESC);
      delay(1000);
    }else if (current_state[4] == LOW && current_state[12] == LOW  ) { // Start + P1 RIGHT => ENTER/RETURN (Generic Use)
      Keyboard.release(keys[4]);
      Keyboard.release(keys[12]);
      Keyboard.press(KEY_RETURN);
      delay(8);
      Keyboard.release(KEY_RETURN);
      delay(1000);
    }

    // Check all keys individually
    for(uint8_t i=0;i<sizeof pins;i++){
       current_state[i] = digitalRead(pins[i]);
       if(current_state[i] != last_state[i]){
         if(micros() - deBounce[i] > deBounceLimit){
           deBounce[i] = micros();
           last_state[i] = current_state[i];
           if(current_state[i] == LOW){
               Keyboard.press(keys[i]);
           }else{
               Keyboard.release(keys[i]);
           }
         }
       }
    }
    
}
