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
//Funcion Comunicacion con sensor.
void comunicacion(void)
{
   inDataDht = 0;
   output_high(dht11);
   output_low(dht11);
   delay_ms(18);
   output_high(dht11);
   delay_us(30);
   inDataDht = 1;
   
}
//Funcion Proceso Correcto
void proceso(void)
{
   delay_us(40);
   if(dataDht==0)
      {
         delay_us(80);
         if(input(dht11) == 1)
            procesoCorrecto=1;
         delay_us(40);        
      }
}
//Funcion Lectura del sensor.
char lecturaDeSenseo()
{   
   inDataDht = 1;
   char i;
   for(int j=0; j<8; j++)
      {
         while(!input(dht11));
         delay_us(30);
         if(input(dht11) == 0)
            {
               i&= ~(1<<(7-j));
            }
         else
            {
               i|= (1<<(7-j));
              while(input(dht11)==1);
            }
      }
   return i;
}
//-------------Funciones para mostrar cada digito
void display1(int led)
{
    output_c(1+led);
    output_d(digitoMostrar[0]);
}
void display2(int led)
{
    output_c(2+led);
    output_d(digitoMostrar[1]+128);
}
void display3(int led)
{
    output_c(4+led);
    output_d(digitoMostrar[2]);
}
void display4(int led)
{
    output_c(8+led);
    output_d(digitoMostrar[3]);
}
//----------------------------
//Funcion Humedad
void humedad(void)
{
    
    valorPosicional[0] = humedadDecena/10; //decenas
    valorPosicional[1] = humedadDecena%10; //unidades
    valorPosicional[2] = humedadUnidad/10; //decimas
    valorPosicional[3] = humedadUnidad%10; //centesimas 
    for(int recorrido=0; recorrido<4; recorrido++)
    {
      digitoMostrar[recorrido] =numeros[valorPosicional[recorrido]];
    }
    display1(64);
     delay_ms(10);
    display2(64);
     delay_ms(10);
    display3(64);
     delay_ms(10);
    display4(64);
     delay_ms(10);
}
//Funcion Temperatura
void temperatura(void)
{
    valorPosicional[0] = temperaturaDecena/10; //decenas
    valorPosicional[1] = temperaturaDecena%10; //unidades
    valorPosicional[2] = temperaturaUnidad/10; //decimas
    valorPosicional[3] = temperaturaUnidad%10; //centesimas
    for(int recorrido=0; recorrido<4; recorrido++)
    {
      digitoMostrar[recorrido] =numeros[valorPosicional[recorrido]];
    }
    display1(32);
     delay_ms(10);
    display2(32);
     delay_ms(10);
    display3(32);
     delay_ms(10);
    display4(32);
     delay_ms(10);
}
//Funcion principal
void main() 
{
   set_tris_b(0x08);
   set_tris_c(0x00);
   set_tris_d(0x00);
   setup_oscillator(OSC_16MHZ);
   enable_interrupts(INT_RB);
   enable_interrupts(GLOBAL);
   unsigned int validacionContenido;
   while(1)
   {       
      comunicacion();
      proceso();
      if(procesoCorrecto==1)
      {
         humedadDecena = lecturaDeSenseo();
         humedadUnidad = lecturaDeSenseo();
         temperaturaDecena = lecturaDeSenseo();
         temperaturaUnidad = lecturaDeSenseo();
         validacionContenido = lecturaDeSenseo();
         if(validacionContenido == ((humedadDecena+humedadUnidad+temperaturaDecena+temperaturaUnidad) & 0xFF))
         {
            if(LedsTH==0)
               humedad();
            else
               temperatura();
         }
      } 
   } 
}