  int pin = 3;
int frequency;
int duty_cycle;
int serial;
String a;
void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(3, OUTPUT);
  pinMode(11, OUTPUT);
  TCCR2A = _BV(COM2A0) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(WGM22) | _BV(CS22);
  TCCR2B = TCCR2B & 0b11111000 | 0x07;
  OCR2A = 80;
  OCR2B = 50;


}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() >= 2) {
    frequency = Serial.parseInt();
    duty_cycle = Serial.parseInt();
    clear_serial();
    setFreq(frequency);
    Serial.println(frequency);
    Serial.println(duty_cycle);
  }
}
void clear_serial() {
  while (Serial.available() > 0) {
    Serial.read();
  }
}
void setFreq(int freq) {
  int vals[7] = {1, 8, 32, 64, 128, 256, 1024};

  for (int val : vals) {
    for (int i = 1; i <= 255; i++) {
      float error = calc_error(freq, calcFreq(i, val));
     // Serial.print("error: ");
     //   Serial.println(error);
      if ( error < 5){
      setPrescaler(pin, val);
      OCR2A = i;
      Serial.println("OCR2A: ");
        Serial.print(i);
      Serial.print("\nVAL: ");
        Serial.println(val);
          
     // OCR2B=i*0.5;
      return;
      }
    }
  }
  Serial.println("i can't do that");
}
float calcFreq(int N, int TOP) {
  return (16000000 / float( N * (1 + TOP)));
}

float calc_error(int expected, float actual) {
  return abs((expected - actual) / expected);
}


void setPrescaler(int pin, int freq) {
  byte mode;

  switch (freq) {
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
