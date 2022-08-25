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
  /* sprintf(Text," seg min hor");
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
  lcd.setCursor(0,0);
  sprintf(Text," seg min hor");
  lcd.print(Text);
  lcd.setCursor(1,2);
  sprintf(Text," 0%d : 0%d : 0%d",seg,minut,hora);
  lcd.print(Text);
  Valor_b= analogRead(menu);
  if((Valor_b>=1009)&&(Valor_b<=1012))acceso=1;
  while(acceso==1)                                      //Ciclo de configuración del tiempo
  {
    Valor_b=analogRead(menu);
   // lcd.clear();
    digitalWrite(fin,LOW);
    //lcd.print("entra?");
   // delay(500);
    lcd.setCursor(1,2);
    sprintf(Text," %d : %d : %d",seg,minut,hora);
    lcd.print(Text);
    if((Valor_b>=1009)&&(Valor_b<=1012))               //condición para poder configurar el tiempo dependiendo de cuantas veces lo presiones 
    { 
      hold=1;
      while(hold==1)                                   //ciclo condicional para evitar que se repita la intruccion muchas veces y tengamos valores gigantes 
      {
        Valor_b=analogRead(menu);
        if(!((Valor_b>=1009) && (Valor_b<=1012)))hold=0;
      }
       selector++;                                      //incremento de una variable para poder acceder a las configuraciones de segundos, minutos y horas
      // delay(500);
    }
    
    while((selector==1)||(selector==4))
    {
      Valor=analogRead(tiempo);
      seg=(Valor*60)/1023;                           //transforma la lectura del potenciometro en una escala de 60 para SEGUNDOS
      if(seg<0)seg=seg+64;                           //esta linea es para compensar el extraño error de conversion, no me explico el por que descompensa a partir de la mitad un valor de 64
      lcd.setCursor(1,1);
      if(seg<10)lcd.print("0");                     //condicional estetico para valores inferiores a 10
      lcd.print(seg);
      //lcd.clear();
      Valor_b=analogRead(menu);
      if((Valor_b>=1009)&&(Valor_b<=1012))break;
      if(Valor_b>=975 && Valor_b<=978)
      {
        Starter(seg,minut,hora);
        break;
      }
    }                                               //fin del while de segundos 
                                                    
    while(selector==2)
    {
      Valor=analogRead(tiempo);
      minut=(Valor*60)/1023;                        //transforma la lectura del potenciometro en una escala de 60 para MINUTOS 
      if(minut<0)minut=minut+64;                    //esta linea es para compensar el extraño error de conversion, no me explico el por que descompensa a partir de la mitad un valor de 64
      lcd.setCursor(6,1);
      if(minut<10)lcd.print("0");                   //condicional estetico para valores inferiores a 10
      lcd.print(minut);
      Valor_b=analogRead(menu);
     if((Valor_b>=1009)&&(Valor_b<=1012))break;
      if(Valor_b>=975 && Valor_b<=978)
      {
        Starter(seg,minut,hora);
        break;
      }
      
    }                                                 //fin del while de minutos 
    
    while(selector==3)
    {
      Valor=analogRead(tiempo);
      hora=(Valor*24)/1023;                           //transforma la lectura del potenciometro en una escala de 60 para HORAS
      if(hora<0)hora=hora+28;                         //esta linea es para compensar el extraño error de conversion, no me explico el por que descompensa a partir de la mitad un valor de 28
      lcd.setCursor(10,1);
      if(hora<10)lcd.print("0");                      //condicional estetico para valores inferiores a 10
      lcd.print(hora);
      Valor_b=analogRead(menu);
      if((Valor_b>=1009)&&(Valor_b<=1012))break;
      if(Valor_b>=975 && Valor_b<=978)
      {
        Starter(seg,minut,hora);
        break;
      }
    }                                                 //Fin del while de horas
    
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
    if(selector>3)                                     //realiza el reseteo de la variable de selección en caso de querer volver a modificar el tiempo
    {
      selector=1;
    }
    
  }                                                      //fin del while de configuracion de tiempo
  if((tiempo_t==0)&&(Start==2))                          //condicional para notificar que el tiempo se ha acabado 
    {
      lcd.clear();
      lcd.print("time over");
      delay(1000);
      lcd.clear();
      Start=0;
    }
}


void Temporizador(void)
{
  digitalWrite(Led, digitalRead(Led)^1);   //invierte el estado del led
  if(Start==1)                             //condicional para que se efectue la cuenta regresiva 
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
  delay(1000);
  acceso=0;                                     // activa la condicion falsa para el while de configuracion
  selector=0;
  Start=1;                                         //Activa en las interrupciones la ejecucion de la cuenta regresiva
  Timer1.initialize(1000000);                      //Configura el timer en 1 segundo 
  Timer1.attachInterrupt(Temporizador);            //Configura la interrupcion del timer 1
}
