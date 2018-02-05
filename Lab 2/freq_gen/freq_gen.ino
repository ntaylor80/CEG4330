/*
  Reading a serial ASCII-encoded string.

 This sketch demonstrates the Serial parseInt() function.
 It looks for an ASCII string of comma-separated values.
 It parses them into ints, and uses those to fade an RGB LED.

 Circuit: Common-Cathode RGB LED wired like so:
 * Red anode: digital pin 3
 * Green anode: digital pin 5
 * Blue anode: digital pin 6
 * Cathode : GND

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
  Serial.begin(9600);
  // make the pins outputs:

}

void loop() {

  if (Serial.available() >= 2) {
  /*pin = Stream.parseInt(Stream.readString());
  
  value = Stream.parseInt(Stream.readString()); */
 frequency = Serial.parseInt();
 duty_cycle = Serial.parseInt();
 clear_serial();
 //frequency= 31250/frequency;
 setPwmFrequency(pin,frequency);
 Serial.println(pin);
 Serial.println(frequency);
  analogWrite(pin, duty_cycle);
  }
    }

void clear_serial(){
while (Serial.available() > 0){
  Serial.read();
}
}
//from http://playground.arduino.cc/Code/PwmFrequency
void setPwmFrequency(int pin, int freq) {
  byte mode;
  int val;
    if(freq<=76){
      val=1024;
    }else if(freq<=183){
      val=256;
    }else if(freq<=367){
      val=128;
    }else if(freq<=735){
      val=64;
    }else if(freq<=2450){
      val=32;
    }else if(freq<=17646){
      val=8;
    }else{
      val=1;
    }

  
    switch(freq) {
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
  








