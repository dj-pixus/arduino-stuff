void setup() {
  Serial.begin(115200);
  DDRD=252;
}

void loop() {
  while(!Serial.available());
  PORTD=Serial.read();
}
