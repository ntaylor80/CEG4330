
/*
 * Example code to communicate with LCD display over SPI using Arduino Uno
 * For CEG 4330/6330L
 *
 * Code continuously prints a message to the LCD using 
 * the globally specified pins.
 * 
 * The SCLK of the Uno's hardware SPI has a min speed of 125 kHz, with default 
 * bus clock, but provided LCD can only operate at a max clock speed of 100 kHz.
 * So the below software implementation is provided to allow for a slower SCLK.
 */
 
// #include <SoftwareSerial.h>  
//////// GLOBALS ////////

// data out pin
const int DATA_PIN = A3;
// clock pin
const int CLK_PIN = A2;
// slave select pin
const int SS_PIN = A1;


/// make bluetooth object////

//////// Function prototypes ////////

// convenience function for char2LCD using global pin variables
void char2LCD(const byte);
void char2LCD(const int dataPin, const int clockPin, 
              const int ssPin, const byte ch, 
              const int byteOrder);
void str2LCD(unsigned char *str);
void turnBlinkingCursorOn();
void turnBlinkingCursorOff();
// Put cursor at location position
// Values 0x00-0x0F for row 1, 0x40-0x4F for row 2
void setCursorPos(unsigned char pos);
// clear LCD and move cursor to line 1 column 1
void clearScreen();
// setup code for communication with LCD
void lcdSPISetup();
                

//////// CODE ////////

//bluetooth setup code



// Send character to LCD over SPI. Software implementation for slower SCLK
void char2LCD(const int dataPin, const int clockPin, 
              const int ssPin, const byte ch, 
              const int byteOrder = MSBFIRST)
{
  byte compareValue = 0x80;
  // initialize compareValue
  if(byteOrder == MSBFIRST)
  {
    compareValue = 0x80;
  } else 
  {
    compareValue = 0x01;
  }
  
  // enable slave select
  digitalWrite(ssPin, LOW);
  
  // shift out data
  for (int i = 0; i < 8; i++)
  {
    // send bit to data pin
    digitalWrite(dataPin, (ch&compareValue)?HIGH:LOW);
    
    // shift compare value
    if(byteOrder == MSBFIRST)
    {
        compareValue = compareValue >> 1;
    } else 
    {
        compareValue = compareValue << 1;
    }
    
    // trigger clk rising edge(toggle low then high)
    digitalWrite(clockPin, LOW);
    // wait before trigger
    delayMicroseconds(4);
    digitalWrite(clockPin, HIGH);
    // wait, LCD can only handle up to 100KHz
    delayMicroseconds(14);
    
  }
  // disable slave select
  digitalWrite(ssPin, HIGH);
}

// convenience function for char2LCD using global pin variables
void char2LCD(const byte ch)
{
  char2LCD(DATA_PIN, CLK_PIN, SS_PIN, ch);
}

// Output string to LCD
void str2LCD(const char *str) 
{
    int i = 0;
    while(str[i]) {
        char2LCD(str[i]);
        i++;
    }  
}

void turnBlinkingCursorOn() 
{
    char2LCD(0xFE);
    char2LCD(0x4B);
    // 100 micro second execution time
    delayMicroseconds(101);      
}

void turnBlinkingCursorOff() 
{
    char2LCD(0xFE);
    char2LCD(0x4C);
    // 100 micro second execution time
    delayMicroseconds(101);      
}

// Put cursor at location position
// Values 0x00-0x0F for row 1, 0x40-0x4F for row 2
void setCursorPos(unsigned char pos) 
{
    char2LCD(0xFE);
    char2LCD(0x45);
    char2LCD(pos);
    // 100 us execution time
    delayMicroseconds(110);  
}

// clear LCD and move cursor to line 1 column 1
void clearScreen()
{
    char2LCD(0xFE);
    char2LCD(0x51);
    // 1.5 ms execution time
    delayMicroseconds(2000);
}

// setup code for communication with LCD
void lcdSPISetup()
{
  
  // Setup SPI pins as output
  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLK_PIN, OUTPUT);
  pinMode(SS_PIN, OUTPUT); 
  
  
  turnBlinkingCursorOn();
}


// Arduino setup()
void setup(){
  lcdSPISetup();
  
}


// Arduino loop()
void loop(){

  
  // Clear screen
  clearScreen();
  
  
  // print a 'c' to the LCD
  char2LCD('c');
  
  // Wait 1 second
  delay(1000);
  
  // Go to second line
  setCursorPos(0x40);
    // print a 'b' to the LCD
  char2LCD(DATA_PIN, CLK_PIN, SS_PIN, 'b');

  // Print a string to the LCD
  str2LCD("Hello World");
  
  // wait 1.5 second
  delay(1500);

//  while(true){
//  if(bluetooth.available())  // If the bluetooth sent any characters
//  {
//    // Send any characters the bluetooth prints to the serial monitor
//    Serial.print((char)bluetooth.read());  
//  }
//  if(Serial.available())  // If stuff was typed in the serial monitor
//  {
//    // Send any characters the Serial monitor prints to the bluetooth
//    bluetooth.print((char)Serial.read());
//  }
//  }
  

}
