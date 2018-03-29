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
#define MAX_BUFFER 150
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
float currTime=millis();
float prevTime=currTime;
bool playSong=1;

int buttonState = LOW;
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;  // the debounce time; increase if the output flickers
int button_pressed = 0;
int button_status = 1;
byte rowPins[ROWS] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 4, 3, 2}; //connect to the column pinouts of the keypad
byte pushButton=10;
byte speaker = 11;
byte led=12;


unsigned int notes_length;
int bpm = 120;
int bpm_mod = 1;

typedef struct
{
  char name[3];
  int octive;
  float length;
} Note;
bool playing = false;
long playing_start = 0;
Note current_note;
Note notes_buffer[MAX_BUFFER];
unsigned int buffer_size = -1;

Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
char notes[7]={'C','D','E','F','G','A','B'};
void setup(){
  Serial.begin(9600);
  pinMode(pushButton, INPUT_PULLUP);
}
  
void loop(){
  
  //char key = keypad.getKey();
<<<<<<< HEAD:Lab 3/assembly/assembly.ino


=======
handle_button();
handle_serial();
handle_keypad();  
play_buffer();
>>>>>>> 695d2011978b47980d8255c523dcd58285633c69:Lab 3/assembly note stuff/assembly.ino
  
  handle_timing(length_to_mills(1));
  
}
void handle_timing(float timeToWait){
  prevTime=currTime;
  while(currTime-prevTime<timeToWait){
  currTime=millis();
  handle_button();
  handle_serial();
  handle_keypad();
  
  }
  
}

void handle_button(){
  
    //sample the state of the button - is it pressed or not?
 buttonState = digitalRead(pushButton);
  //filter out any noise by setting a time buffer
  if ( (millis() - lastDebounceTime) > debounceDelay) {
 if (button_status == button_pressed){
  if ( (buttonState != button_pressed) ) {
      button_status = 1;
      lastDebounceTime = millis(); //set the current time
    }
 }
 else if ( (buttonState == button_pressed) ) {
      octiveJmp= octiveJmp ^ 1;
      button_status = button_pressed;
      lastDebounceTime = millis(); //set the current time
    }
    
  }
 
}

void handle_serial(){
  if (Serial.available() && buffer_size < MAX_BUFFER -1){
    if (Serial.peek() != 'H' && Serial.peek() != '\n'){
      Note note = notes_buffer[buffer_size];
      note.name[0] = Serial.read();
      if (Serial.peek() != ' '){
        note.name[1] = Serial.read();
      }
      else{
        note.name[1] = 0;
      }
      note.name[2] = 0;

    note.octive = Serial.parseInt();
    note.length = Serial.parseFloat();
    Serial.read(); //remove space
    
   buffer_size = buffer_size + 1;
    }
    else{
      Serial.read();
    }
  }
  }

void play_buffer(){
  Serial.println("play");
  Serial.println(buffer_size + 1);
  Serial.println(playing);
  if(buffer_size + 1 && !playing){
  Serial.println("new note");
    current_note = notes_buffer[buffer_size];
    playing_start = millis();
    
    tone(speaker, note_to_freq(String(current_note.name), current_note.octive));
    
  }
}
int length_to_mills(float length){
  double mills_per_beat = (1/(bpm / 60.0)) * 1000;
  
  return mills_per_beat * length;
}
void handle_keypad(){
  if (kpd.getKeys())
    {
      char* msg;
        for (int i=0; i<LIST_MAX; i++)   // Scan the whole key list.
        {
            if ( kpd.key[i].stateChanged )   // Only find keys that have changed state.
            {
                if (kpd.key[i].kchar >= '1' && kpd.key[i].kchar <= '8'){ 
                noteInt = kpd.key[i].kchar - '1';
                if (noteInt == 7){
                  pitch=note_to_freq(String("C"), 5);
                }else{
                pitch=note_to_freq(String(notes[noteInt]), 4);
                }
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
                    //octive = (octive == 1) ? 2 : 1;
                    msg = " RELEASED.";
                break;
                    case IDLE:
                    msg = " IDLE.";
                }
                }
                
                

            }        
        }
        }
}
long note_to_freq(String noteIn, int octive) {
    double freq = 0;
    Serial.println(noteIn);
    const char* note;
    note=noteIn.c_str();
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

