// This is a code when the serial monitor has already been used to
// set up the Bluetooth modem and the modem has been added as a
// new Bluetooth device on the Windows.

// Open a termite window and connect it to the vitural COM port
// used by the modem.

char mych;

void setup() {
  // initialize serial:
  Serial.begin(115200);
  
  mych = Serial.read();
  Serial.print("Start UNO\n");
  
  delay(1000);
}

void loop() {
  // if there's any serial available, read it:

  while (Serial.available() > 0) {

    mych = Serial.read();
    
    mych++;
    
    Serial.print(mych);

  }
}








