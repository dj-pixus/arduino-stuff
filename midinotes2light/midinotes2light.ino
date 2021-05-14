byte command, note, velocity, key[10];

void setup() {
  DDRB |= 0b00011111;
  DDRD |= 0b01111100;
  PORTD = 0;
  Serial.begin(115200);
  
  PORTB = 16;
  do {
    midiWait();
  }
  while (command != 0x90 || !velocity);
    key[0] = note;
  
  PORTB = 8;
  do {
    midiWait();
  }
  while (command != 0x90 || !velocity);
    key[1] = note;
  
  PORTB = 4;
  do {
    midiWait();
  }
  while (command != 0x90 || !velocity);
    key[2] = note;
  
  PORTB = 2;
  do {
    midiWait();
  }
  while (command != 0x90 || !velocity);
    key[3] = note;
  
  PORTB = 1;
  do {
    midiWait();
  }
  while (command != 0x90 || !velocity);
    key[4] = note;

  PORTB = 0;
  
  PORTD = 64;
  do {
    midiWait();
  }
  while (command != 0x90 || !velocity);
    key[5] = note;
  
  PORTD = 32;
  do {
    midiWait();
  }
  while (command != 0x90 || !velocity);
    key[6] = note;
  
  PORTD = 16;
  do {
    midiWait();
  }
  while (command != 0x90 || !velocity);
    key[7] = note;
  
  PORTD = 8;
  do {
    midiWait();
  }
  while (command != 0x90 || !velocity);
    key[8] = note;
  
  PORTD = 4;
  do {
    midiWait();
  }
  while (command != 0x90 || !velocity);
    key[9] = note;
}

void loop() {
  midiWait();
  if (command == 0x90 && velocity) {
    if (note == key[0])
      PORTB |= 16;
    if (note == key[1])
      PORTB |= 8;
    if (note == key[2])
      PORTB |= 4;
    if (note == key[3])
      PORTB |= 2;
    if (note == key[4])
      PORTB |= 1;
    
    if (note == key[5])
      PORTD |= 64;
    if (note == key[6])
      PORTD |= 32;
    if (note == key[7])
      PORTD |= 16;
    if (note == key[8])
      PORTD |= 8;
    if (note == key[9])
      PORTD |= 4;
  }
  if (command == 0x80 || !velocity) {
    if (note == key[0])
      PORTB &= 0b11101111;
    if (note == key[1])
      PORTB &= 0b11110111;
    if (note == key[2])
      PORTB &= 0b11111011;
    if (note == key[3])
      PORTB &= 0b11111101;
    if (note == key[4])
      PORTB &= 0b11111110;
    
    if (note == key[5])
      PORTD &= 0b10111111;
    if (note == key[6])
      PORTD &= 0b11011111;
    if (note == key[7])
      PORTD &= 0b11101111;
    if (note == key[8])
      PORTD &= 0b11110111;
    if (note == key[9])
      PORTD &= 0b11111011;
  }
}

void midiWait() {
  while(Serial.available()<3);
  command = Serial.read();
  note = Serial.read();
  velocity = Serial.read();
}
