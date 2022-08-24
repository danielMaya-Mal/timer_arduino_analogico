#include<TimerOne.h>
#include <Wire.h>

#define Led 13
#define fin 12
#define tiempo A1
#define menu A0
//#define eleccion 4

int Valor=0;
int Valor_b=0;
int selector=0;

volatile int tiempo_t=0;
int acceso=0;
int seg=0;
int minut=0;
int hora=0;
int Start=0;
int hold=0;
char Text[10];

#include "DFRobot_LCD.h"

  const int colorR = 255;
  const int colorG = 0;
  const int colorB = 0;
  DFRobot_LCD lcd(16,2);  //16 characters and 2 lines of show

void setup() {
  // put your setup code here, to run once:
   pinMode(Led, OUTPUT);
   lcd.init();
   lcd.setRGB(colorR, colorG, colorB);//If the module is a monochrome screen, you need to shield it
   lcd.print("prueba");
   delay(1000);
   lcd.clear();
   lcd.print("Timer");
   delay(500);
   lcd.clear();
   sprintf(Text," seg min hor");
   lcd.print(Text);
   lcd.setCursor(1,2);
  /* seg=0;
   minut=0;
   sprintf(Text," 0%d : 0%d : 0%d",seg,minut,hora);
   lcd.print(Text);*/
   //Timer1.initialize(1000000);                      //Configura el timer en 1 segundo 
  // Timer1.attachInterrupt(Temporizador);            //Configura la interrupcion del timer 1
}

void loop() {
  // put your main code here, to run repeatedly:
  int seg=0;
  int minut=0;
  int hora=0;
  lcd.setCursor(1,2);
  sprintf(Text," %d : %d : %d",seg,minut,hora);
  lcd.print(Text);
 // selector=0;
 // lcd.print("ah?");
 // Valor_b=analogRead(tiempo);
//  delay(500);
 // lcd.print(Valor_b);
//  delay(500);
  Valor_b= analogRead(menu);
  if((Valor_b>=1009)&&(Valor_b<=1012))acceso=1;
  while(acceso==1)
  {
    Valor_b=analogRead(menu);
   // lcd.clear();
    lcd.setCursor(1,2);
    sprintf(Text," %d : %d : %d",seg,minut,hora);
    lcd.print(Text);
    if((Valor_b>=1009)&&(Valor_b<=1012))
    { 
      hold=1;
      while(hold==1)
      {
        Valor_b=analogRead(menu);
        if(!((Valor_b>=1009) && (Valor_b<=1012)))hold=0;
      }
       selector++;
      // delay(500);
    }
    
    while((selector==1)||(selector==4))
    {
      Valor=analogRead(tiempo);
      seg=(Valor*60)/1023;
      if(seg<0)seg=seg+64;
      lcd.setCursor(1,1);
      if(seg<10)lcd.print("0");
      lcd.print(seg);
      //lcd.clear();
      Valor_b=analogRead(menu);
      if((Valor_b>=1009)&&(Valor_b<=1012))break;
      if(Valor_b>=975 && Valor_b<=978)
      {
        Starter(seg,minut,hora);
        break;
      }
    }                               //fin del while de segundos 
                                                    
    while(selector==2)
    {
      Valor=analogRead(tiempo);
      minut=(Valor*60)/1023;
      if(minut<0)minut=minut+64;
      lcd.setCursor(6,1);
      if(minut<10)lcd.print("0");
      lcd.print(minut);
      Valor_b=analogRead(menu);
      if((Valor_b>=1009)&&(Valor_b<=1012))break;
      if(Valor_b>=975 && Valor_b<=978)
      {
        Starter(seg,minut,hora);
        break;
      }
      
    }//fin del while de minutos 
    
    while(selector==3)
    {
      Valor=analogRead(tiempo);
      hora=(Valor*24)/1023;
      if(hora<0)hora=hora+28;
      lcd.setCursor(10,1);
      if(hora<10)lcd.print("0");
      lcd.print(hora);
      Valor_b=analogRead(menu);
      if((Valor_b>=1009)&&(Valor_b<=1012))break;
      if(Valor_b>=975 && Valor_b<=978)
      {
        Starter(seg,minut,hora);
        break;
      }
    }
    
    if(Valor_b>=975 && Valor_b<=978)
    {
      tiempo_t=seg+(minut*60)+(hora*3600);
      lcd.clear();
      delay(500);
      lcd.print(tiempo_t);
      acceso==0;                                     // activa la condicion falsa para el while de configuracion
      Start=1;                                         //Activa en las interrupciones la ejecucion de la cuenta regresiva
      Timer1.initialize(1000000);                      //Configura el timer en 1 segundo 
      Timer1.attachInterrupt(Temporizador);            //Configura la interrupcion del timer 1
      break;
    }
    if(selector>3)
    {
      selector=1;
    }
    
    lcd.print("entra?");
  }
 // lcd.print(tiempo_t);
  /*if(Start==2)
    {
      lcd.clear();
      lcd.print("time over");
      delay(1000);
    }*/
}


void Temporizador(void)
{
  digitalWrite(Led, digitalRead(Led)^1);   //invierte el estado del led
  if(Start==1)                             //
  {
    tiempo_t--;
   // sprintf(Text,"tiempo: %d",tiempo);
   // lcd.print(tiempo);
   if(tiempo_t==0)                           //Reinicia los conteos a 0 para volver a empezar
    {
      Start=2;
      seg=0;
      minut=0;
      hora=0;
      digitalWrite(fin,HIGH);
    
    }
  }
}

void Starter(int seg,int minut, int hora)
{
  tiempo_t=seg+(minut*60)+(hora*3600);
  lcd.clear();
  delay(500);
  lcd.print(tiempo_t);
  lcd.setCursor(5,0);
  lcd.print(acceso);
  delay(1000);
 // acceso==0;                                     // activa la condicion falsa para el while de configuracion
  Start=1;                                         //Activa en las interrupciones la ejecucion de la cuenta regresiva
  Timer1.initialize(1000000);                      //Configura el timer en 1 segundo 
  Timer1.attachInterrupt(Temporizador);            //Configura la interrupcion del timer 1
}
