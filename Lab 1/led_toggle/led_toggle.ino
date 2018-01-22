const int buttonPin = 2;
int button_pressed = 0;
int buttonState = 0;
int ledState = 0;
void setup() {
  // put your setup code here, to run once:
   pinMode(buttonPin, INPUT_PULLUP);
   pinMode(LED_BUILTIN, OUTPUT);
   //pinMode(LED_BUILTIN,INPUT);
   Serial.begin(9600);
}

void loop() {
  buttonState = digitalRead(buttonPin);
     //Serial.println(buttonState); 
  
  if(buttonState == button_pressed){
    
    int on=1;
    while(on){
    delay(10);
       buttonState = digitalRead(buttonPin);
       if(buttonState != button_pressed){
        on=0;
       }
    }

    //led toggle
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
