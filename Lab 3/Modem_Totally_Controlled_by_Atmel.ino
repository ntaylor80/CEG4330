// This is a code when the serial monitor is NEVER used to
// switch the Bluetooth modem from HID to SPP mode.

// Connect Modem Rx to UNO Tx (and Tx to Rx)
// Connect GND together
// Connect 5V to modem only after code uploading, but 
// need to do it within ten seconds.

// Open a termite window after adding the Bluetooth device
// so as to test the code.  (Use the virtual COM port
// of the modem.)

char mych;

void setup() {

  delay(10000); // 10 seconds to connect 5V to the modem
  
  Serial.begin(115200); // initialize serial
  
  delay(100);
  Serial.print("$$$");  // gets modem into command mode

  (void) Serial.read(); // skip the "CMD" string
  (void) Serial.read(); // returned from the modem
  (void) Serial.read();

  Serial.print("S~,0\n");  // modem switches to SPP mode

  Serial.print("R,1\n");  // modem reboots
  
  delay(5000); // allow some reboot time
}

void loop() {
  // if there's any serial available, read it:

  while (Serial.available() > 0) {

    mych = Serial.read();
    
    mych++;
    
    Serial.print(mych);

  }
}








