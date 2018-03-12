// This is a code when the serial monitor is used to
// control the Bluetooth modem. 

void setup() {
  // initialize serial:
  Serial.begin(115200);
  
  Serial.print("Start UNO\n");
  
  delay(1000);

  UCSR0B &= ~ ( 1 << TXEN0); // disable transmitter
  //UCSR0B &= ~ ( 1 << RXEN0); // disable receiver
}

void loop() { }








