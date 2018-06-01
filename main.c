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
//Banderas
short procesoCorrecto = 0;
//Variables Principales.
unsigned int temperaturaDecena; 
unsigned int temperaturaUnidad;
unsigned int humedadDecena;
unsigned int humedadUnidad;
//Variable de cambio de senseo
int LedsTH=0;
//Arreglos almacenar los datos del senseo
int valorPosicional[4]={0};
int digitoMostrar[4]={0};
//Arreglo con los valores para pintar los numeros de 0 a 9
int numeros[10]={63,6,91,79,102,109,125,7,127,103};
//Interrupcion del pushBotton (temperatura o humedad))
#INT_RB
void interrupt_isr(void)
{
   if(input(PIN_B4)==1)
       LedsTH++;   
   if(LedsTH==2)
       LedsTH=0;
}
void main() 
{
   enable_interrupts(INT_RB);
   enable_interrupts(GLOBAL);
}