int in1 = 5;
int in2 = 4;
int ENA = 11;

int FaseA = 2;
int FaseB = 3;

long int count = 0;
float vel = 0; 
float velchida = 0;

void setup() {
 

  // put your setup code here, to run once: 
  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);
  pinMode(ENA,OUTPUT);

  pinMode(FaseA,INPUT);
  pinMode(FaseB,INPUT);
  Serial.begin(1000000);
  attachInterrupt(digitalPinToInterrupt(FaseA),callback_A,FALLING);
  attachInterrupt(digitalPinToInterrupt(FaseB),callback_B,FALLING);

  cli();
  TCCR1A = 0;  
  TCCR1B = 0;  
  TCNT1  = 0;  

  // Configuración del valor de comparación
  OCR1A = 1135;  // Para 55 Hz (prescaler de 256)

  // Modo de comparación de salida (CTC - Clear Timer on Compare Match)
  TCCR1B |= (1 << WGM12);

  // Prescaler de 256
  TCCR1B |= (1 << CS12);

  // Habilita la interrupción por comparación de Timer1
  TIMSK1 |= (1 << OCIE1A);

  sei();

}

ISR(TIMER1_COMPA_vect){
  vel = count*(55.0);
  count = 0;

  velchida = vel/(22*45);

}



void callback_A(){
  if(digitalRead(FaseB)==1){
    count++;

  }else{
    count--;
  }
}

void callback_B(){
  if(digitalRead(FaseA)==0){
    count++;

  }else{
    count--;
  }
}

void loop() {
  
  if(Serial.available() > 0){
    Serial.println(velchida);
    String str = Serial.readStringUntil('\n');
    float data = str.toFloat();
    float pwm = max(-1.0, min(data,1.0));
    float pow = pwm*(255);
    if(pwm > 0){
      digitalWrite(in1,LOW);
      digitalWrite(in2,HIGH);
      analogWrite(ENA,pow);

    }else if(pwm < 0){
      digitalWrite(in1,HIGH);
      digitalWrite(in2,LOW);
      analogWrite(ENA,-pow);

    }else if(pwm == 0){
      digitalWrite(in1,LOW);
      digitalWrite(in2,LOW);
      analogWrite(ENA,0);
    }
  }
}
