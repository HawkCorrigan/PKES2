#include "Odometry.h"
#include "Distance.h"
#include <avr/interrupt.h>

volatile bool stateA=((PINB>>PB3)&1);
volatile bool stateB=((PINB>>PB4)&1);

volatile int32_t templeft=0;
volatile int32_t tempright=0;
OdomData odomData = {.left=0,.right=0};

void initOdom(){
  
DDRB &= ~(1<<PB0); //Odometry to input
DDRB &= ~(1<<PB3);
DDRB &= ~(1<<PB4);
DDRB &= ~(1<<PB7);

PCMSK0  |=  (1<<PCINT3)// enable odometry Interrupts 
        | (1<<PCINT4); 
PCICR   |= (1<<PCIE0); //Pin Change Interrupts

sei();

};

VelocityData getVelocityData(){
    VelocityData velocityData;
  cli();
  velocityData.left=(float)templeft/2195/0.075;
  velocityData.right=(float)tempright/2195/0.075;
  templeft=0;
  tempright=0;
  sei();
    return velocityData;            
}

ISR(PCINT0_vect){

if(stateB != ((PINB >> PB4)&1)){ //If Change in Motor A
    stateB = !stateB;
    //Both forward criteria:

if(stateB == ((PINB >> PB7)&1))
         tempright++;
else //backwards
        tempright--;
}
// cf Bstate
if(stateA != ((PINB >> PB3)&1)){
    stateA = !stateA;

if(stateA == ((PINB >> PB0)&1))
         templeft++;
else 
    templeft--;
}
}