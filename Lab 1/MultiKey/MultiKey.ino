/* @file MultiKey.ino
|| @version 1.0
|| @author Mark Stanley
|| @contact mstanley@technologist.com
||
|| @description
|| | The latest version, 3.0, of the keypad library supports up to 10
|| | active keys all being pressed at the same time. This sketch is an
|| | example of how you can get multiple key presses from a keypad or
|| | keyboard.
|| #
*/
#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {9, 2, 3, 5}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {4, 6, 7, 8}; //connect to the column pinouts of the keypad

Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
int note;
double pitch;
unsigned long loopCount;
unsigned long startTime;
byte speaker = 11;
String msg;
int note_vals[]= {0,2,4,5,7,9,11,12};
int octive = 1;
const int buttonPin = 12;
int button_pressed = 0;
int lastButtonState = 0;
int buttonState = 0;
int button_released = 1;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50; 

void setup() {
    Serial.begin(9600);
    loopCount = 0;
    startTime = millis();
    msg = "";
    pinMode(buttonPin, INPUT_PULLUP);
}
void loop() {
    loopCount++;
    /*if ( (millis()-startTime)>5000 ) {
        Serial.print("Average loops per second = ");
        Serial.println(loopCount/5);
        startTime = millis();
        loopCount = 0;
    }*/
      buttonState = digitalRead(buttonPin);
    
    button();
    if (kpd.getKeys())
    {
        for (int i=0; i<LIST_MAX; i++)   // Scan the whole key list.
        {
            if ( kpd.key[i].stateChanged )   // Only find keys that have changed state.
            {
                if (kpd.key[i].kchar >= '1' && kpd.key[i].kchar <= '7'){ 
                note = kpd.key[i].kchar - '1';
                pitch = octive * 261.2 * pow(2.0,(note_vals[note]/12.0));
                switch (kpd.key[i].kstate) {  // Report active key state : IDLE, PRESSED, HOLD, or RELEASED
                    case PRESSED:
                     tone(speaker, pitch);
                    msg = " PRESSED.";
                break;
                    case HOLD:
                   
                    msg = " HOLD.";
                break;
                    case RELEASED:
                    noTone(speaker);
                    msg = " RELEASED.";
                break;
                    case IDLE:
                    msg = " IDLE.";
                }
                }else if (kpd.key[i].kchar == 'A'){
                   switch (kpd.key[i].kstate) {  // Report active key state : IDLE, PRESSED, HOLD, or RELEASED
                    case PRESSED:
                    
                    msg = " PRESSED.";
                break;
                    case HOLD:
                    msg = " HOLD.";
                break;
                    case RELEASED:
                    Serial.println("octive change");
                    octive = (octive == 1) ? 2 : 1;
                    msg = " RELEASED.";
                break;
                    case IDLE:
                    msg = " IDLE.";
                }
                }
                
                
                Serial.print(note);
                Serial.println();
               Serial.println(pitch);
            }        
        }
        }
    }
void button(){
 Serial.println("In button");
  int reading = digitalRead(buttonPin);
 while(reading==LOW){
  delay(10);
  reading = digitalRead(buttonPin);
  if(reading != LOW){
    Serial.println("octive change");
        octive = (octive == 1) ? 2 : 1; 
  }
 }
}
