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
int bpm = 60;
int bpm_mod = 1;

struct Note
{
  char name;
  bool flat;
  int octive;
  float length;
};
bool playing = false;
byte current_note = -1;
long note_start = 0;
Note notes_buffer[MAX_BUFFER];
unsigned int buffer_size = 0;

Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
char notes[7]={'C','D','E','F','G','A','B'};
void setup(){
  Serial.begin(9600);
  pinMode(pushButton, INPUT_PULLUP);

  buffer_size = 8;
  notes_buffer[0].name = 'B';
  notes_buffer[0].flat = 1;
  notes_buffer[0].octive = 4;
  notes_buffer[0].length = 1;

  notes_buffer[1].name = 'F';
  notes_buffer[1].flat = 0;
  notes_buffer[1].octive = 3;
  notes_buffer[1].length = 1.25;
  
notes_buffer[2].name = 'B';
notes_buffer[2].flat = 1;
  notes_buffer[2].octive = 4;
  notes_buffer[2].length = .25;

notes_buffer[3].name = 'B';
notes_buffer[3].flat = 1;
  notes_buffer[3].octive = 4;
  notes_buffer[3].length = .25;

  
notes_buffer[4].name = 'C';
notes_buffer[4].flat = 0;
  notes_buffer[4].octive = 4;
  notes_buffer[4].length = .25;

  
notes_buffer[5].name = 'D';
notes_buffer[5].flat = 0;
  notes_buffer[5].octive = 3;
  notes_buffer[5].length = .25;

notes_buffer[6].name = 'E';
notes_buffer[6].flat = 1;
  notes_buffer[6].octive = 3;
  notes_buffer[6].length = .25;

notes_buffer[7].name = 'F';
notes_buffer[7].flat = 0;
  notes_buffer[7].octive = 3;
  notes_buffer[7].length = 2;
  playing = true;
}
  
void loop(){
  
  //char key = keypad.getKey();


  
  handle_timing(length_to_mills(1));
  
}
void handle_timing(float timeToWait){
  prevTime=currTime;
  while(currTime-prevTime<timeToWait){
  currTime=millis();
  handle_button();
  //handle_serial();
  handle_keypad();
  handle_playing();
  }
  
}
void handle_playing(){
  if (playing){
    
    Note note = notes_buffer[current_note];
    if ((note_start + length_to_mills(note.length)) < millis()){
      if (current_note ==buffer_size){
        current_note = 0;
      }
      else{
        current_note = current_note + 1;
      }
      Serial.println("playing note");
      Serial.println(current_note);
      Serial.println("flat");
      Serial.println(notes_buffer[current_note].flat);
      Serial.println(notes_buffer[current_note].name);
      note_start = millis();
      if(notes_buffer[current_note].name == 'R'){
        noTone(speaker);
      }else{
      note_start = millis();
      tone(speaker,note_to_freq(String(notes_buffer[current_note].name), notes_buffer[current_note].octive,notes_buffer[current_note].flat));
    }}
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
  if (Serial.available()){
    if (Serial.peek() != 'H'){
      Note note = notes_buffer[buffer_size];
      note.name = Serial.read();
      if (Serial.peek() != ' '){
        note.flat = true;
      }
      else{
         note.flat = false;
      }
    
    }
    else{
      Serial.read();
    }
  }
   
/*
    note_octive = Serial.parseInt();
    note_length = Serial.parseFloat();
    Serial.print(Serial.findUntil())
    */
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
                  pitch=note_to_freq(String("C"), 5,false);
                }else{
                pitch=note_to_freq(String(notes[noteInt]), 4,false);
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
long note_to_freq(String noteIn, int octive,bool flat) {
    double freq = 0;
    Serial.println(noteIn);
    Serial.println(octive);
    const char* note;
    note=noteIn.c_str();
    if(octiveJmp)
    {
      octive++;
    }
    if (strcmp(note, "C") == 0) {
        freq = 16.35;
    } else if ((strcmp(note, "D") == 0) && flat) {
        freq = 17.32;
    } else if (strcmp(note, "D") == 0) {
        freq = 18.35;
    } else if ((strcmp(note, "E") == 0) && flat) {
        freq = 19.45;
    } else if (strcmp(note, "E") == 0) {
        freq = 20.60;
    } else if (strcmp(note, "F") == 0) {
        freq = 21.83;
    } else if ((strcmp(note, "G") == 0) && flat) {
        freq = 23.12;
    } else if ((strcmp(note, "G") == 0) && flat) {
        freq = 24.50;
    } else if ((strcmp(note, "A") == 0) && flat) {
        freq = 25.96;
    } else if (strcmp(note, "A") == 0) {
        freq = 27.50;
    } else if ((strcmp(note, "B") == 0) && flat) {
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

