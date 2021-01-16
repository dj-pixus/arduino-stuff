/*
This program  allows you to draw stuff on a PAL TV, controlled from serial port.
Based on the official TVout lib, but very different.
Did not document the controlling protocol, but ez to figure out.
*/

#include "TVout.h"
#include "font6x8.h"
TVout Screen;
void setup(){
  Serial.begin(1200);
  Screen.begin(1,128,96);
  Screen.select_font(font6x8);
}
void loop(){
  while(!Serial.available());
  char command = Serial.read();
  char foo;
  while(command>15)
  {
    command-=16;
  };
  switch(command)
  {
    case 0:
    while(Serial.available()<3);
    Screen.print_char(Serial.read(),Serial.read(),Serial.read());
    break;
    
    case 1:
    while(Serial.available()<2);
    Screen.set_pixel(Serial.read(),Serial.read(),0);
    break;
    case 2:
    while(Serial.available()<2);
    Screen.set_pixel(Serial.read(),Serial.read(),1);
    break;
    case 3:
    while(Serial.available()<2);
    Screen.set_pixel(Serial.read(),Serial.read(),2);
    break;

    case 4:
    Screen.fill(0);
    break;
    case 5:
    Screen.fill(1);
    break;
    case 6:
    Screen.fill(2);
    break;

    case 7:
    while(!Serial.available());
    foo=Serial.read();
    if ((foo<49)&&(foo!=0))
      Screen.shift(foo,0);
    else if((foo>128)&&(foo<176))
      Screen.shift(foo-128,1);
    break;
    case 8:
    while(!Serial.available());
    foo=Serial.read();
    if ((foo<72)&&(foo!=0))
      Screen.shift(foo,2);
    else if((foo>128)&&(foo<200))
      Screen.shift(foo-128,3);
    break;

    case 9:
    while(Serial.available()<4);
    Screen.draw_line(Serial.read(),Serial.read(),Serial.read(),Serial.read(),0);
    break;
    case 10:
    while(Serial.available()<4);
    Screen.draw_line(Serial.read(),Serial.read(),Serial.read(),Serial.read(),1);
    break;
    case 11:
    while(Serial.available()<4);
    Screen.draw_line(Serial.read(),Serial.read(),Serial.read(),Serial.read(),2);
    break;

    case 12:
    while(Serial.available()<4);
    Screen.draw_rect(Serial.read(),Serial.read(),Serial.read()-1,Serial.read(),0);
    break;
    case 13:
    while(Serial.available()<4);
    Screen.draw_rect(Serial.read(),Serial.read(),Serial.read()-1,Serial.read(),1);
    break;
    case 14:
    while(Serial.available()<4);
    Screen.draw_rect(Serial.read(),Serial.read(),Serial.read()-1,Serial.read(),2);
    break;
  }
}
