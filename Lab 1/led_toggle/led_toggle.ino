const int buttonPin = 2;
int buttonState = 0;
int ledState = 0;
void setup() {
  // put your setup code here, to run once:
   pinMode(buttonPin, INPUT);
   pinMode(LED_BUILTIN, OUTPUT);
   //pinMode(LED_BUILTIN,INPUT);
   Serial.begin(9600);
}

void loop() {
  int buttonState = digitalRead(buttonPin);
  if(buttonState){
    delay(100);
    if(ledState){
      ledState=0;
      digitalWrite(LED_BUILTIN, LOW);
    }
    else{
      ledState=1;
      digitalWrite(LED_BUILTIN, HIGH);
    }
  }
  
 
}
