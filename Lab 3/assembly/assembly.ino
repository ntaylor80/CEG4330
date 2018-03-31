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
float space = 0.0078125;
float prevTime=currTime;
bool playSong=1;
bool pause = false;

int buttonState = LOW;
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;  // the debounce time; increase if the output flickers
int button_pressed = 0;
int button_status = 1;

//pins

int proximity_pin = A0;
byte rowPins[ROWS] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 4, 3, 2}; //connect to the column pinouts of the keypad
byte pushButton=12;
byte speaker = 11;
byte led=10;
byte freq_pin = 13;

unsigned int notes_length;
int bpm = 120;
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
  
  current_note = -1;
  buffer_size = 0;

add_note('B', true, 4, 2);
add_note('R', false, 4, .666);
add_note('B', true, 4, .333);
add_note('B', true, 4, .333);
add_note('B', true, 4, .333);
add_note('B', true, 4, .333);

add_note('B', true, 4, .666);
add_note('A', true, 4, .333);
add_note('B', true, 4, 1.666);
add_note('B', true, 4, .333);
add_note('B', true, 4, .333);
add_note('B', true, 4, .333);
add_note('B', true, 4, .333);

add_note('B',true,4,.666);
add_note('A', true, 4, .333);
add_note('B', true, 4, 1.666);
add_note('B', true, 4, .333);
add_note('B', true, 4, .333);
add_note('B', true, 4, .333);
add_note('B', true, 4, .333);

add_note('B', true, 4, .5);
add_note('F', true, 4, .25);
add_note('F', true, 4, .25);

add_note('F', true, 4, .5);
add_note('F', true, 4, .25);
add_note('F', true, 4, .25);


add_note('F', true, 4, .5);
add_note('F', true, 4, .25);
add_note('F', true, 4, .25);


add_note('F', true, 4, .5);
add_note('F', true, 4, .5);

add_note('B',true,4,1);
add_note('F',true,4,1.75);

add_note('B',true,4,.25);
add_note('B',true,4,.25);
add_note('C',false,5,.25);
add_note('D',false,5,.25);
add_note('E',true,5,.25);

add_note('F',false,5,2);
add_note('R',true,5,.5);
add_note('F',false,5,.5);
add_note('F',false,5,.333);
add_note('G',true,5,.333);
add_note('A',true,5,.333);

add_note('B',true,5,2.333);
add_note('B',true,5,.333);
add_note('B',true,5,.333);
add_note('B',true,5,.333);
add_note('A',true,5,.333);
add_note('G',true,5,.333);

add_note('A',true,5,.666);
add_note('G',true,5,.333);
add_note('F',true,5,2);

add_note('R',true,5,2);

  playing = true;
}
  
void loop(){

  //char key = keypad.getKey();


  
  handle_timing(length_to_mills(1));
  
}

void add_note(char name, bool flat, int octive, float length){
  notes_buffer[buffer_size].name = name;
  notes_buffer[buffer_size].flat = flat;
  notes_buffer[buffer_size].octive = octive;
  notes_buffer[buffer_size].length = length;
  buffer_size++;
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
void next_note(){
  if (current_note == buffer_size - 1){
        current_note = 0;
      }
      else{
        current_note = current_note + 1;
      }
}
void playNote(){
  note_start = millis();
      if (notes_buffer[current_note].name == 'R'){
        noTone(speaker);
      }
      else{
      tone(speaker,note_to_freq(notes_buffer[current_note].name, notes_buffer[current_note].octive,notes_buffer[current_note].flat));
}
}
void handle_playing(){

  if (playing){
    
    if (pause){
      Serial.println("pause");
      noTone(speaker);
      if((note_start + length_to_mills(space)) < millis()){
        pause = false;
       note_start = millis();
       playNote();
      }else{
        return;
      }
    }
    
   if(current_note == 255){
    current_note = 0;
    playNote();
    return;
   }
    Serial.println(current_note);
    Serial.println(notes_buffer[current_note].name);

    Note note = notes_buffer[current_note];
    if ((note_start + length_to_mills(note.length - space)) < millis()){
      Serial.println(notes_buffer[current_note].flat);
      Serial.println(notes_buffer[current_note].name);
      note_start = millis();
     
        pause = true;
        next_note();
   }
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
                  pitch=note_to_freq('C', 5,false);
                }else{
                pitch=note_to_freq(notes[noteInt], 4,false);
                }
                switch (kpd.key[i].kstate) {  // Report active key state : IDLE, PRESSED, HOLD, or RELEASED
                    case PRESSED:
                     tone(speaker, pitch);
                     Serial.println(kpd.key[i].kchar);
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
long note_to_freq(char note, int octive,bool flat) {
    double freq = 0;
    Serial.println(note);
    Serial.println(octive);
    if(octiveJmp)
    {
      octive++;
    }
    if (note == 'C') {
        freq = 16.35;
    } else if (note == 'D' && flat) {
        freq = 17.32;
    } else if (note == 'D') {
        freq = 18.35;
    } else if (note == 'E' && flat) {
        freq = 19.45;
    } else if (note == 'E') {
        freq = 20.60;
    } else if (note == 'F') {
        freq = 21.83;
    } else if (note == 'G' && flat) {
        freq = 23.12;
    } else if (note == 'G') {
        freq = 24.50;
    } else if (note == 'A' && flat) {
        freq = 25.96;
    } else if (note == 'A') {
        freq = 27.50;
    } else if (note == 'B' && flat) {
        freq = 29.14;
    } else if (note == 'B') {
        freq = 30.87;
    }else{
      freq=30.87;
    }
    for (int i = 0; i < octive; ++i) {
        freq = freq * 2;
    }
    return freq;
}

