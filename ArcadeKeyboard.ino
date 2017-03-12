#include <usb_keyboard.h>

// Defines a mapping between an input pin and a keyboard press
typedef struct {
    int pin;        // Signal pin the key button is connected to
    int keyCode;    // Key code to output on keypress
} KeyMapping;

//
// MAME key mappings
//
const int NUM_KEYS                      = 12;
const KeyMapping KEYMAPS[NUM_KEYS]      = {
    { 0,    KEY_LEFT },                     // Joystick Left
    { 1,    KEY_RIGHT },                    // Joystick Right
    { 2,    KEY_UP },                       // Joystick Up
    { 3,    KEY_DOWN },                     // Joystick Down
    { 4,    KEY_LEFT_CTRL },                // Fire 1
    { 5,    KEY_LEFT_ALT },                 // Fire 2
    { 6,    KEY_5 },                        // Coin
    { 7,    KEY_1 },                        // 1P Start
    { 8,    KEY_2 },                        // 2P Start
    { 9,    KEY_ESC },                      // Quit game
    { 10,   KEY_TAB },                      // In-Game Menu
    { 11,   KEY_F2 }                        // Service switch
};

int keyState                    = 0;        // Bitmask that maintains a cached set/unset value for each key
int key                         = 0;        // Loop counter for iterating over keys
const KeyMapping *currentKey    = 0;        // Key iteration pointer
const int LED_PIN               = 13;       // Pin the debug LED is connected to


                ///////////////////////
                // Utility functions //
                ///////////////////////

// Checks whether the button connected to the supplied pin
// is pressed or not
inline boolean isPressed(int pin) {
    return (digitalRead(pin) == LOW);
}

// Returns the cached state value for the supplied key number
inline boolean cachedKeyState(int key) {
    return (keyState & (1 << key));
}

inline boolean anyKeysSet() {
    return (keyState != 0);    
}

// Sets the cached key state value for the supplied key number
inline void setKeyState(int key) {
    keyState |= (1 << key);    
}

// Unsets the cached key state value for the supplied key number
inline void unsetKeyState(int key) {
    keyState &= ~(1 << key);
}


                //////////////////
                // MAIN PROGRAM //
                //////////////////
void setup() {

    // Initialize all button pins as inputs
    for (key = 0; key < NUM_KEYS; key++) {
        pinMode(KEYMAPS[key].pin, INPUT_PULLUP);
    }

    // Initialize LED pin as output
    pinMode(LED_PIN, OUTPUT);
    
    delay(1000);        // Add a small delay to allow the USB host enough time to recognize this device
}

void loop() {
    currentKey = KEYMAPS;
    for (key = 0; key < NUM_KEYS; key++) {
        if(isPressed(currentKey->pin)) {
            if(!cachedKeyState(key)) {
                Keyboard.press(currentKey->keyCode);
                setKeyState(key);
            }
        } else {
            if(cachedKeyState(key)) {
                Keyboard.release(currentKey->keyCode);
                unsetKeyState(key);
            }
        }
        ++currentKey;
    }

    // Set LED state
    if(anyKeysSet()) {
        digitalWrite(LED_PIN, HIGH);    
    } else {
        digitalWrite(LED_PIN, LOW);    
    }
    
    delay(2);
}

