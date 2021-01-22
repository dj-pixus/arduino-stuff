byte del=16, i;
byte note[9]={36,41,43,44,46,47,48,50,51}; //notes to play
byte port[9]={4,8,16,32,4,8,16,32,4}; //port state for each step
int trsh;
bool on[9];

void setup() {
  DDRB|=0b00100000; //onboard led: 13
  DDRC=0;
  DDRD|=0b00111100; //motor: 2, 3, 4, 5
  Serial.begin(9600);
  trsh=analogRead(0)+10; //calibration
}

void loop() {
  if(Serial.available())
    del=Serial.read();
  for(i=0;i<8;i++) //forward
  {
    PORTD=port[i];
    delay(del);
    if (analogRead(0)>trsh) //note on
    {
      if (!on[i])
      {
        on[i]=1;
        Serial.write(0x90);
        Serial.write(note[i]);
        Serial.write(127);
        PORTB|=0b00100000;
      }
    }
    else //note off
    {
      if (on[i])
      {
        on[i]=0;
        Serial.write(0x80);
        Serial.write(note[i]);
        Serial.write(0);
        PORTB&=0b11011111;
      }
    }
  }
  for(i=8;i>0;i--) //backward
  {
    PORTD=port[i];
    delay(del);
    if (analogRead(0)>trsh) //note on
    {
      if (!on[i])
      {
        on[i]=1;
        Serial.write(0x90);
        Serial.write(note[i]);
        Serial.write(127);
        PORTB|=0b00100000;
      }
    }
    else //note off
    {
      if (on[i])
      {
        on[i]=0;
        Serial.write(0x80);
        Serial.write(note[i]);
        Serial.write(0);
        PORTB&=0b11011111;
      }
    }
  }
}
