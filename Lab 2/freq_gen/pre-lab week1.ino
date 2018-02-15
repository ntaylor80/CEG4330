/* Jason Elting & Noah Taylor
Registers TCCR2B and TCCR2A will be used to control the frequency,
by changing their value and the prescalar to meet the closest frequency possible.

*/
int pin = 3;
int value;

void setup() {
  // initialize serial:
  Serial.begin(9600);
  // make the pins outputs:

}

void loop() {

  if (Serial.available() >= 2) {
    pin = Stream.parseInt(Stream.readString());
  
    value = Stream.parseInt(Stream.readString()); 
    clear_serial();
    Serial.println(pin);
    analogWrite(pin,value );
  }
}

void clear_serial(){
  while (Serial.available() > 0){
    Serial.read();
  }
}
