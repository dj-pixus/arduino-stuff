#include <math.h>
byte cmd, note, vcty, noteold;
int freq;

void setup()
{
  DDRB |= 34; // 9 - clock, 13 - gate
  PORTB &= 223; //gate off
  //Serial.begin(31250); //midi
  Serial.begin(115200); //usb
}
void loop()
{
  if (Serial.available() > 2)
  {
    cmd = Serial.read();
    note = Serial.read();
    vcty = Serial.read();
    if (((cmd == 128) || vcty == 0) & (note == noteold)) //note off, but only if it's the currently active note
    {
      PORTB &= 223; //gate off
    }
    if ((cmd == 144) && (vcty > 0)) //note on
    {
      freq = pow(pow(2.0, (1.0 / 12.0)), (note - 9)) * 275.0; //This gives 10x freq to drive a 4017. Trust me, it's correct.
      tone(9, freq);
      PORTB |= 32; //gate on
      noteold = note;
    }
  }
}