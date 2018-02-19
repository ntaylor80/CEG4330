/*
  Reading a serial ASCII-encoded string.

  This sketch demonstrates the Serial parseInt() function.
  It looks for an ASCII string of comma-separated values.
  It parses them into ints, and uses those to fade an RGB LED.

  Circuit: Common-Cathode RGB LED wired like so:
   Red anode: digital pin 3
   Green anode: digital pin 5
   Blue anode: digital pin 6
   Cathode : GND

  created 13 Apr 2012
  by Tom Igoe

  modified 14 Mar 2016
  by Arturo Guadalupi

  This example code is in the public domain.
*/

// pins for the LEDs:

int pin = 3;
int frequency;
int duty_cycle;
int serial;
String a;


void setup() {
  // initialize serial:
  pinMode(3, OUTPUT);
  Serial.begin(9600);
  // make the pins outputs:
  //TCCR2B |= _BV(WGM13) + _BV(WGM12) + _BV(WGM11) + _BV(WGM10);
  TCCR2A = _BV(COM2A0) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(WGM22) | _BV(CS22);
  OCR2A = 180;
  OCR2B = 50;
}

void loop() {

  if (Serial.available() >= 2) {
    /*pin = Stream.parseInt(Stream.readString());

      value = Stream.parseInt(Stream.readString()); */
    frequency = Serial.parseInt();
    duty_cycle = Serial.parseInt();
    clear_serial();
    //setFreq(frequency);
    //frequency= 31250/frequency;
   // setPrescaler(pin, frequency);
    
    Serial.println(frequency);
    Serial.println(duty_cycle);
    OCR2A=frequency;
   
    analogWrite(pin, duty_cycle*2.5);
  }
}

void clear_serial() {
  while (Serial.available() > 0) {
    Serial.read();
  }
}

void setFreq(int freq) {
  int vals[7] = {1, 8, 32, 64, 128, 256, 1024};
  
  for (int val : vals) {
    for (int i = 0; i <= 255; i++) {
      if (calc_error(freq,calcFreq(i, val) < .1));
        setPrescaler(pin,val);
        OCR2A=i;
        Serial.print("OCR2A: ");
        Serial.print(i);
        return;
        
    }
  }
  Serial.println("i can't do that");
}
  float calcFreq(int N, int TOP) {
    return (16000000 / float( N *(1+ TOP)));
  }

  float calc_error(int expected, float actual){
    return (expected - actual)/ expected;
  }

  //from http://playground.arduino.cc/Code/PwmFrequency
  void setPrescaler(int pin, int freq) {
    byte mode;
    
    switch (freq) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x07; break;
      default: return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode;

  }









