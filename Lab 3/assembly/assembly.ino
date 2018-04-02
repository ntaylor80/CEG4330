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
#include "LCD.h"
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

int proximity_pin = A3;
byte rowPins[ROWS] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 4, 3, 2}; //connect to the column pinouts of the keypad
byte pushButton=12;
byte speaker = 11;
byte led=10;
bool led_state=0;
byte freq_pin = 13;
//
//const int DATA_PIN = A1;
//// clock pin
//const int CLK_PIN = A2;
//// slave select pin
//const int SS_PIN = A0;

unsigned int notes_length;
int bpm = 140;
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
long led_start = 0;
Note notes_buffer[MAX_BUFFER];
unsigned int buffer_size = 0;

char songname[15] = "Zelda Theme";

Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
char notes[7]={'C','D','E','F','G','A','B'};

int print_delay=100;




void setup(){
//pwm setup 
  pinMode(10, OUTPUT);

  TCCR1A =  _BV(COM1B1) | _BV(WGM11) | _BV(WGM11);
  TCCR1B = _BV(CS11) | _BV(WGM13) | _BV(WGM12);
  ICR1 = 62499; //top
  OCR1B = 30878; //duty
  TCCR1B = TCCR1B & 0b11111000 | 0x07;


/// here be all the zelda stuff
  
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

add_note('F', false, 5, 1);

add_note('E', true, 5, .5);
add_note('E', true, 5, .25);
add_note('F', false, 5, .25);
add_note('G', true, 5, 2);
add_note('F', false, 5, .5);
add_note('E', true, 5, .5);

add_note('D', true, 5, .5);
add_note('D', true, 5, .25);
add_note('E', true, 5, .25);
add_note('F', false, 5, 2);
add_note('E', true, 5, .5);
add_note('D', true, 5, .5);
add_note('C', false, 5, .5);
add_note('C', false, 5, .25);
add_note('D', true, 5, .25);
add_note('E', true, 5, 2);
add_note('G', false, 5, 1);

add_note('F', true, 4, .5);
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

  //playing = true;
lcdSPISetup();
write_name(songname);

}
  
void loop(){
  
  //char key = keypad.getKey();

  handle_button();
  handle_serial();
  handle_keypad();
  handle_playing();
  handle_led();
 if (print_delay + length_to_mills(1) < millis()){
// Serial.print(getSignal());
 }
    
}
float getSignal(){
  long pwm_high=pulseIn(freq_pin,HIGH,1000);
  long pwm_low=pulseIn(freq_pin, LOW,1000);
  float period=pwm_high+pwm_low;
  //Period in microseconds
  period=period/1000000;
  //period in secondds
  float frequency=1/period;
  return frequency;
}
void write_name(const char *str){
  char2LCD(0xFE);
  char2LCD(0x51);
  delayMicroseconds(1500);
  str2LCD(str);
  setCursorPos(0x40);
  str2LCD("Note:");
}
void write_note(char note,bool flat){

  Serial.println(note);
  setCursorPos(0x45);
  delayMicroseconds(150);
  char2LCD(note);
  if (flat){
    char2LCD('b');
  }else{
    char2LCD(' ');
  }
}
int get_proximity(){
  
  return map(analogRead(proximity_pin),0,700,0,100);
}
void handle_led(){
  if (led_start + length_to_mills(1) < millis()){
    led_start=millis();
    led_state=!led_state;
    
     int freqIn=getSignal();
     if(freqIn>100000){
      freqIn=2000;
     }
     
     
    led_start+=freqIn;
     set_pin_10_pwm(freqIn,led_state*get_proximity());
  }
}

void clear_notes(){
  buffer_size = 0;
}
void add_note(char name, bool flat, int octive, float length){
  notes_buffer[buffer_size].name = name;
  notes_buffer[buffer_size].flat = flat;
  notes_buffer[buffer_size].octive = octive;
  notes_buffer[buffer_size].length = length;
  buffer_size++;
}

void next_note(){
  if (current_note == buffer_size - 1){
        current_note = 0;
      }
      else{
        current_note = current_note + 1;
      }
      write_note(notes_buffer[current_note].name,notes_buffer[current_note].flat);
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
    if (Serial.peek() == '$'){
      Serial.read();
      while(true){
        if (!Serial.available()){
          break;
          playing = true;
        }
        if (Serial.peek() == '$'){
          playing = true;
          break;
        }else{
           add_note(Serial.read(),Serial.parseInt(),Serial.parseInt(),Serial.parseFloat());
        if(Serial.peek() == ':'){
            Serial.read();
    }
        }
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
                  write_note('C',false);
                }else{
                pitch=note_to_freq(notes[noteInt], 4,false);
                write_note(notes[noteInt],false);
                }
                switch (kpd.key[i].kstate) {  // Report active key state : IDLE, PRESSED, HOLD, or RELEASED
                    case PRESSED:
                     playing=false;
                     tone(speaker, pitch);
                     
                     Serial.println(kpd.key[i].kchar);
                    msg = " PRESSED.";
                break;
                    case HOLD:
                   
                    msg = " HOLD.";
                break;
                    case RELEASED:
                    noTone(speaker);
                    write_note(' ',false);
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
                    Serial.println("pause zelda");//octive change");
                    playing=!playing;
                    noTone(speaker);
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
void set_pin_10_pwm(float freq,int duty){
  unsigned long max_top = 65536;
  unsigned long N_TOP = 16000000.0/ freq;
  unsigned int prescaler;
  unsigned int top;
  unsigned int duty_cycle;
  unsigned char mode;
  Serial.println("pin 10");

  Serial.println(N_TOP);
  if(N_TOP <= 1 * max_top){
    prescaler = 1;
  }
  else if (N_TOP <= 8 * max_top){
      prescaler = 8;
  }
 
  else if (N_TOP <= 64 * max_top){
      prescaler = 64;
  }
  else if (N_TOP <= 256 * max_top){
      prescaler = 256;
  }
  else {
      prescaler = 1024;
  }

  switch(prescaler) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x04; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x07; break;
      default: return;
    }
  TCCR1B = TCCR1B & 0b11111000 | mode;
  
  top = (N_TOP / prescaler) - 1;
  duty_cycle = (duty / 100.0) * top;

  
  ICR1 = top;
  OCR1B = duty_cycle;
  
 
 
  Serial.print("top: ");
  Serial.println(top);
    Serial.print("duty: ");
  Serial.println(duty_cycle);
}

//
//void char2LCD(const int dataPin, const int clockPin, 
//              const int ssPin, const byte ch, 
//              const int byteOrder = MSBFIRST)
//{
//  byte compareValue = 0x80;
//  // initialize compareValue
//  if(byteOrder == MSBFIRST)
//  {
//    compareValue = 0x80;
//  } else 
//  {
//    compareValue = 0x01;
//  }
//  
//  // enable slave select
//  digitalWrite(ssPin, LOW);
//  
//  // shift out data
//  for (int i = 0; i < 8; i++)
//  {
//    // send bit to data pin
//    digitalWrite(dataPin, (ch&compareValue)?HIGH:LOW);
//    
//    // shift compare value
//    if(byteOrder == MSBFIRST)
//    {
//        compareValue = compareValue >> 1;
//    } else 
//    {
//        compareValue = compareValue << 1;
//    }
//    
//    // trigger clk rising edge(toggle low then high)
//    digitalWrite(clockPin, LOW);
//    // wait before trigger
//    delayMicroseconds(4);
//    digitalWrite(clockPin, HIGH);
//    // wait, LCD can only handle up to 100KHz
//    delayMicroseconds(14);
//    
//  }
//  // disable slave select
//  digitalWrite(ssPin, HIGH);
//}
//
//// convenience function for char2LCD using global pin variables
//void char2LCD(const byte ch)
//{
//  char2LCD(DATA_PIN, CLK_PIN, SS_PIN, ch);
//}
//
//// Output string to LCD
//void str2LCD(const char *str) 
//{
//    int i = 0;
//    while(str[i]) {
//        char2LCD(str[i]);
//        i++;
//    }  
//}

