byte a,b,c;
void setup()
{
	Serial.begin(115200);
}
void loop()
{
	while(Serial.available()<3);
	a=Serial.read();
	b=Serial.read();
	c=Serial.read();
	if(a==176)
		analogWrite(b,c?1+c*2:0); //beutifool
}
