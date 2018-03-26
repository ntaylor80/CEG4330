/* @file HelloKeypad.pde
|| @version 1.0
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Demonstrates the simplest use of the matrix Keypad library.
|| #
*/
#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
int note;
int pitch;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
bool playSong=1;
byte pushButton=10;
byte rowPins[ROWS] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 4, 3, 2}; //connect to the column pinouts of the keypad
byte speaker = 11;

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup(){
  Serial.begin(9600);
}
  
void loop(){
  char key = keypad.getKey();
  if (key){
    if (key <= '9' && key >= '1'){
       note = key - '0';
       
       pitch = 261.2 * pow(2.0,(note/12.0));
       tone(speaker, pitch);
       Serial.println(pitch);
       Serial.println(note);
    }
     
  }else{
    noTone(speaker);//,a261.2 * pow(2.0,(5/12.0)));
  }
  
  delay(100);
  
}

