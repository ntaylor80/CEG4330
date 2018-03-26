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
#include <string.h>
bool octiveJmp=0;
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
int noteInt;
long pitch;
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
char notes[7]={'C','D','E','F','G','A','B'};
void setup(){
  Serial.begin(9600);
  
}
  
void loop(){
  char key = keypad.getKey();
  if(digitalRead(pushButton)){
    octiveJmp=1;
  }else{
    octiveJmp=0;
  }
  
  if (key){
    if (key <= '9' && key >= '1'){
       noteInt = key - '1';
       if(noteInt==7){
       pitch=note_to_freq("C", 5);
      }else{
       pitch=note_to_freq(String(notes[noteInt]), 4);
      
       }
       
      tone(speaker, pitch);
       Serial.println(pitch);
       Serial.println(notes[noteInt]);
    }
     
  }else{
    noTone(speaker);//,a261.2 * pow(2.0,(5/12.0)));
  }
  
  delay(100);
  
}


long note_to_freq(String noteIn, int octive) {
    double freq = 0;
    Serial.println(noteIn);
    const char* note;
    note=noteIn.c_str();
    Serial.println(strcmp(note, "D"));
    if(octiveJmp)
    {
      octive++;
    }
    if (strcmp(note, "C") == 0) {
        freq = 16.35;
    } else if (strcmp(note, "Db") == 0) {
        freq = 17.32;
    } else if (strcmp(note, "D") == 0) {
        freq = 18.35;
    } else if (strcmp(note, "Eb") == 0) {
        freq = 19.45;
    } else if (strcmp(note, "E") == 0) {
        freq = 20.60;
    } else if (strcmp(note, "F") == 0) {
        freq = 21.83;
    } else if (strcmp(note, "Gb") == 0) {
        freq = 23.12;
    } else if (strcmp(note, "G") == 0) {
        freq = 24.50;
    } else if (strcmp(note, "Ab") == 0) {
        freq = 25.96;
    } else if (strcmp(note, "A") == 0) {
        freq = 27.50;
    } else if (strcmp(note, "Bb") == 0) {
        freq = 29.14;
    } else if (strcmp(note, "B") == 0) {
        freq = 30.87;
    }else{
      freq=30.87;
    }
    for (int i = 0; i < octive; ++i) {
        freq = freq * 2;
    }
    return freq;
}
