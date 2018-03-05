long Fast_PWM_frequency;
long frequency;
int duty_cycle;
char message[50];
int sensor_pin= A1;
//int timer = 2;
void setup() {
  Serial.begin(9600);


  pinMode(3, OUTPUT);
  TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(COM2A0) | _BV(COM2B0) | _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(WGM22) | _BV(CS20);
  OCR2A = 255;
  OCR2B = 100;
 
  pinMode(10, OUTPUT);

  TCCR1A =  _BV(COM1B1) | _BV(WGM11) | _BV(WGM11);
  TCCR1B = _BV(CS11) | _BV(WGM13) | _BV(WGM12);
  ICR1 = 2000; //top
  OCR1B = 766; //duty
}
void loop() {
 if (Serial.available() >= 2) {
    frequency = Serial.parseInt();
    duty_cycle = Serial.parseInt();
    clear_serial();
    set_pin_3_pwm(frequency,duty_cycle);
   set_pin_10_pwm(frequency,duty_cycle);
    Serial.println(frequency);
    Serial.println(duty_cycle);
  }
}
void clear_serial() {
  while (Serial.available() > 0) {
    Serial.read();
  }
}
void set_pin_10_pwm(long freq,int duty){
  unsigned long max_top = 65536;
  unsigned long N_TOP = 16000000.0/ freq;
  unsigned int prescaler;
  unsigned int top;
  unsigned int duty_cycle;
  unsigned char mode;
  Serial.println("pin 10");
  sprintf(message,"N_TOP: %f",N_TOP);
  Serial.println(N_TOP);
  if(N_TOP <= 1 * max_top){
    prescaler = 1;
  }
  else if (N_TOP <= 8 * max_top){
      prescaler = 8;
  }
 
  else if (N_TOP <= 64 * max_top){
      prescaler = 64;
  }
  else if (N_TOP <= 256 * max_top){
      prescaler = 256;
  }
  else {
      prescaler = 1024;
  }

  switch(prescaler) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x04; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x07; break;
      default: return;
    }
  TCCR1B = TCCR1B & 0b11111000 | mode;
  
  top = (N_TOP / prescaler) - 1;
  duty_cycle = (duty / 100.0) * top;

  
  ICR1 = top;
  OCR1B = duty_cycle;
  
  sprintf(message,"prescaler: %d Top: %d",prescaler,top);
  Serial.println(message);
}
void set_pin_3_pwm(long freq,int duty){
  double N_TOP = 16000000.0/ freq;
   unsigned int prescaler;
   unsigned int top;
   unsigned int duty_cycle;
   unsigned char mode;
  Serial.println("pin 3");
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

  switch(prescaler) {
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
  
  top = (N_TOP / prescaler) - 1;
  duty_cycle = (duty / 100.0) * top;

  
  OCR2A = top;
  OCR2B = duty_cycle;
  
  sprintf(message,"prescaler: %d Top: %d",prescaler,top);
  Serial.println(message);
}

float getSignal(){
  long pwm_high=pulseIn(sensor_pin,HIGH);
  long pwm_low=pulseIn(sensor_pin, LOW);
  float period=pwm_high+pwm_low;
  //Period in microseconds
  period=period/1000000;
  //period in secondds
  float frequency=1/period;
  return frequency;
}
