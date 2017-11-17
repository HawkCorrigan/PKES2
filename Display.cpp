#include "Display.h"
#include <avr/io.h>
#include <Arduino.h>

const uint8_t zeichen[] ={
		0x7E,
		0x42,
		0xEC,
		0xE6,
		0xD2,
		0xB6,
		0xBE,
		0x62,
		0xFE,
		0xF6
};
void initDisplay(){
    DDRB |= 1 << 1;
    DDRB |= 1 << 2;
    DDRB |= 1 << 4;
    DDRD |= 1 << 4;
    PORTD &= ~ (1<<4);
    PORTB &= ~(1 << 2);
    for (unsigned int i = 0; i<24;i++){
      PORTB &= ~(1 << 1);
      PORTB |= 1 << 1;
    }
    PORTD |= 1 << 4;
}
void writeToDisplay(uint8_t data[3]){
    PORTD &= ~ (1 << 4);
    PORTB &= ~(1 << 2);
  for (unsigned int i=0; i<3; i++){ 
    uint8_t zahl = data[i];
    for (unsigned int j=0; j<8; j++){
      if(zahl%2==1)
        PORTB |= 1<<2;
      else
        PORTB &= ~(1 << 2);
      PORTB &= ~(1 << 1);
      PORTB |= 1 << 1;
      zahl/=2;
    }
  }
  PORTD |= 1 << 4;
}
void writeValueToDisplay(int value){
  uint8_t data[3];
  int end=3;
  if(value>=1000 ||value<=-200){
    initDisplay();
    return;
  }
   if(value<0){     
      if(value<=-100){
        data[0]=0xC2;
        value=value%100;
        end=2;
      }
      else
        data[0]=0x80;
      value*=-1;
      end=2;
    }
  for (unsigned int i=0; i<end;i++)
  {
    data[2-i]=zeichen[value%10];
    value/=10;
  }
  writeToDisplay(data);
}


void writeValueToDisplay(float value){
  int dpPosition=0;
  int valueNormalized;
  uint8_t data[3];
  int end=3;
  if(value>=1000 ||value<=-200){
    initDisplay();
    return;
  }
    if(value<0){
      if(value<-100){
        data[0]=0x86;
        value-=100;
      }
      else
        data[0]=0x80;
      value*=-1;
      end=2;
      while(value<10){
        dpPosition++;
        value*=10;
      }
    }
    else{
      while(value<100){
        dpPosition++;
        value*=10;
      }
    }
  valueNormalized=(int)value;
  for (unsigned int i=0; i<end;i++)
  {
    data[2-i]=zeichen[valueNormalized%10];
    if(i==dpPosition)
      data[2-i]++;
    valueNormalized/=10;
  }
  writeToDisplay(data);
}