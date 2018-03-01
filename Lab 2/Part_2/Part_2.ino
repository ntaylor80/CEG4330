long Fast_PWM_frequency;
int frequency;
int duty_cycle;
char message[50];
int timer = 1;
void setup() {
  Serial.begin(9600);

if (timer == 2){
  pinMode(3, OUTPUT);
  TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(WGM22) | _BV(CS20);
  OCR2A = 0;
  OCR2B = 0;
}

else if (timer == 1){
  Serial.println("timer 1");
 
  pinMode(10, OUTPUT);

  TCCR1A = _BV(COM1A1) | _BV(COM1B1) | _BV(WGM11) | _BV(WGM10);
  TCCR1B = _BV(CS11) | _BV(WGM12);
  OCR1B = 766;
}
}
void loop() {
 if (Serial.available() >= 2) {
    frequency = Serial.parseInt();
    duty_cycle = Serial.parseInt();
    clear_serial();
    setFreq(frequency,duty_cycle);
    Serial.println(frequency);
    Serial.println(duty_cycle);
  }
}
void clear_serial() {
  while (Serial.available() > 0) {
    Serial.read();
  }
}
void setFreq(int freq,int duty){
  double N_TOP = 16000000.0/ freq;
  int prescaler;
  int top;
  int duty_cycle;
  
  sprintf(message,"N_TOP: %f",N_TOP);
  Serial.println(N_TOP);
  if(N_TOP <= 256){
    prescaler = 1;
  }
  else if (N_TOP <= 2048){
      prescaler = 8;
  }
  else if (N_TOP <= 8192){
      prescaler = 32;
  }
  else if (N_TOP <= 16384){
      prescaler = 64;
  }
  else if (N_TOP <= 32768){
      prescaler = 128;
  }
  else if (N_TOP <= 65536){
      prescaler = 256;
  }
  else {
      prescaler = 1024;
  }

  top = (N_TOP / prescaler) - 1;
  OCR2A = top;
  duty_cycle = (duty / 100.0) * top;
  OCR2B = duty_cycle;
  sprintf(message,"prescaler: %d Top: %d",prescaler,top);
  Serial.println(message);
}

