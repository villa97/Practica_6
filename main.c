#include <18F4620.h>
#fuses HS, NOFCMEN, NOIESO, PUT, NOBROWNOUT, NOWDT
#fuses NOPBADEN, STVREN, NOLVP, NODEBUG,
#use delay(clock=16000000)
#use fast_io(A)
#use fast_io(B)
#use fast_io(D)
#use fast_io(E)
#define dht11 PIN_C4
#bit inDataDht = 0x94.0
#byte dataDht = 0x82

void main() 
{
   
}