#include<TimerOne.h>
#include <Wire.h>
#include "DFRobot_LCD.h"

  const int colorR = 255;
  const int colorG = 0;
  const int colorB = 0;
  DFRobot_LCD lcd(16,2);  //16 characters and 2 lines of show


#define Led 13
#define Led1 2
#define Led2 3
#define Led3 4
#define Led4 5 

#define fin 12
#define Botonera_A A1

volatile int tiempo=0;
int seg=0;
int minut=0;
int hora=0;
int Start=0;
int Valor=0;
int acceso=0;
int hold=0;
char Text[10];

void setup() {
  // put your setup code here, to run once:
  pinMode(Led, OUTPUT);
   lcd.init();
   lcd.setRGB(colorR, colorG, colorB);//If the module is a monochrome screen, you need to shield it
   lcd.print("pues haber");
   delay(1000);
   lcd.clear();
  // Timer1.initialize(1000000);              //Configura el timer en 1 segundo 
  // Timer1.attachInterrupt(Temporizador);    //Configura la interrupcion del timer 1
   
}

void loop() {
  // put your main code here, to run repeatedly:
  Valor=analogRead(Botonera_A);
  if((Valor>=1009)&&(Valor<=1012))acceso=1;
  while(acceso==1){
    digitalWrite(fin,LOW);
    Valor=analogRead(Botonera_A);
   if(Valor>=983 && Valor<=985)       //se presiona el aumento de segundo
  {
    hold=1;
    digitalWrite(Led1,HIGH);
    while(hold==1)
    {
      Valor=analogRead(Botonera_A);
      if(!((Valor>=983) && (Valor<=985)))hold=0;
    }
    seg++;
  }
  else
  {
    digitalWrite(Led1,LOW);
  }

  if(Valor >=998 && Valor<=1003)      //se presiona el aumento de minuto 
  {
    hold=1;
    digitalWrite(Led2,HIGH);
    while(hold==1)
    {
      Valor=analogRead(Botonera_A);
      if(!((Valor>=998) && (Valor<=1003)))hold=0;
    }
    minut++;
    }
  else
  {
    digitalWrite(Led2,LOW);
  }

  if(Valor >=995 && Valor<=997)
  {
    hold=1;
    digitalWrite(Led3,HIGH);
    while(hold==1)
    {
      Valor=analogRead(Botonera_A);
      if(!((Valor>=995) && (Valor<=997)))hold=0;
    }
    hora++;
  }
  else
  {
    digitalWrite(Led3,LOW);
  }
   
  if(Valor >=975 && Valor<=978)
  {
   tiempo=seg+(minut*60)+(hora*3600);
   digitalWrite(Led3,HIGH);
  // lcd.print(tiempo);  // todo bien aqui parece 
  // delay(1000);
   sprintf(Text,"S:%d M:%d H:%d",seg,minut,hora);
   lcd.print(Text);
   delay(500);
   tiempo=seg+(minut*60)+(hora*3600);    //extraño, pero por que tendria que repetir la instrucción si no hay forma de que cambie? parece que aqui falla... 
   lcd.clear();
  // lcd.print(tiempo);  //justo aqui ocurre la falla ? 
  // delay(1000);
   Timer1.initialize(1000000);              //Configura el timer en 1 segundo 
   Timer1.attachInterrupt(Temporizador);    //Configura la interrupcion del timer 1
   lcd.clear();
   Start=1;
   lcd.print("time on");
   delay(500);
   lcd.clear();
   acceso=0;  //acaba el ciclo 
  }
  else
  {
    digitalWrite(Led3,LOW);
  }
  }//FINAL DEL WHILE
}

void Temporizador(void)
{
  digitalWrite(Led, digitalRead(Led)^1);   //invierte el estado del led
  if(Start==1)
  {
    tiempo--;
   // sprintf(Text,"tiempo: %d",tiempo);
   // lcd.print(tiempo);
   if(tiempo==0)
    {
      Start=0;
      seg=0;
      minut=0;
      hora=0;
      digitalWrite(fin,HIGH);
    
    }
  }
}
