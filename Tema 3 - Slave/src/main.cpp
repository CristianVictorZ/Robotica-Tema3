#include <Arduino.h>
#include<SPI.h>

//-----DATE PRIMITE-----
//0b11111111 (255) - repaus
//0b01110000 (112) - raspuns gresit
//0b11110000 (240) - asteapta
//0b11110001 (241) - raspuns corect
//0bxaaa0000 - porneste un led ('x' este 0 sau 1; dintre 'a' doar unul poate fi 1, resul sunt 0)

//-----DATE TRIMISE-----
//0b00001111 - orice buton apasat in starea de repaus
//0b0000xaaa - buton apasat in timpul rularii jocului ('x' este 0 sau 1; dintre 'a' doar unul poate fi 1, resul sunt 0)

#define BAUD 28800

#define PLAYER1_BUTTON_GREEN 940
#define PLAYER1_BUTTON_YELLOW 380
#define PLAYER1_BUTTON_BLUE 200
#define PLAYER2_BUTTON_GREEN 1000
#define PLAYER2_BUTTON_YELLOW 440
#define PLAYER2_BUTTON_BLUE 205

#define PIN_PLAYER1_BUTTONS A0
#define PIN_PLAYER2_BUTTONS A1
#define PIN_PALYER1_BLUE_LED 4
#define PIN_PALYER1_GREEN_LED 6
#define PIN_PALYER1_RED_LED 8
#define PIN_PALYER2_GREEN_LED 2
#define PIN_PALYER2_YELLOW_LED 7
#define PIN_PALYER2_BLUE_LED 9
#define PIN_PERMANENT_LEDS 3
#define BUZZER 5

byte data = 0, dataSent = 0;
int valueAnalog;
int playerRound = 0, frequency = 0;
volatile int idle = 1, wait = 0, toneChanging = 0;

unsigned long soundDuration = 500, soundStart = 0, toneChangeInterval = 50, lastToneChange = 0;

//Funcita opreste led-urile
void turnOffLeds()
{
  digitalWrite(PIN_PALYER2_GREEN_LED, LOW);
  digitalWrite(PIN_PALYER2_YELLOW_LED, LOW);
  analogWrite(PIN_PALYER2_BLUE_LED, 0);
  digitalWrite(PIN_PALYER1_RED_LED, LOW);
  analogWrite(PIN_PALYER1_GREEN_LED, 0);
  digitalWrite(PIN_PALYER1_BLUE_LED, LOW);
}

//Functia este folosita pentru a oprii buzzerul, sau pentru a modifica frecventa sunetului generat de acesta
void playSound()
{
  if(toneChanging)
  {
    if(millis() - lastToneChange > toneChangeInterval)
    {
      lastToneChange = millis();
      if(frequency == 1500)
      {
        frequency = 2000;
      }else
      {
        frequency = 1500;
      }
      tone(BUZZER, frequency);
    }
  }

  if(millis() - soundStart >= soundDuration)
  {
    noTone(BUZZER);
  }
}

ISR (SPI_STC_vect)                        
{
  playSound();
  //Serial.println(SPDR);
  if(SPDR == 255)
  {
    //REPAUS
    idle = 1;
  }else if(SPDR == 241)
  {
    //RASPUNS CORECT
    soundStart = millis();
    toneChanging = 1;
    frequency = 2000;
    tone(BUZZER, frequency);

    wait = 1;
    idle = 0;
  }else if(SPDR == 240)
  {
    //ASTEPTARE
    turnOffLeds();
    wait = 1;
    idle = 0;
  }else if(SPDR == 112)
  {
    //RASPUNS GRESIT
    toneChanging = 0;
    soundStart = millis();
    frequency = 100;
    tone(BUZZER, frequency);

    wait = 0;
    idle = 0;
  }else
  {
    //DATE CARE INDICA APRINDEREA UNUI LED
    data = SPDR;
    idle = 0;
    wait = 0;
  }

  SPDR = 0;
}

//Functie folosita pentru citirea valorilor analog de la butoane
bool valueInRange(int value, int center)
{
  int range = 50;

  return (value <= center + range && value >= center - range);
}

//Functie care initializeaza led-urile
void initLEDS()
{
  pinMode(PIN_PALYER1_BLUE_LED, OUTPUT);
  pinMode(PIN_PALYER1_GREEN_LED, OUTPUT);
  pinMode(PIN_PALYER1_RED_LED, OUTPUT);
  pinMode(PIN_PALYER2_GREEN_LED, OUTPUT);
  pinMode(PIN_PALYER2_YELLOW_LED, OUTPUT);
  pinMode(PIN_PALYER2_BLUE_LED, OUTPUT);

  pinMode(PIN_PERMANENT_LEDS, OUTPUT);
}

void setup() {

  Serial.begin(BAUD);

  initLEDS();

  analogWrite(PIN_PERMANENT_LEDS, 75);

  pinMode(MISO,OUTPUT);
  SPCR |= _BV(SPE);                  
  SPI.attachInterrupt();
  SPDR = 0;
}

void loop() {

  //led-urile de la butoane vor fi permanent aprinse
  analogWrite(PIN_PERMANENT_LEDS, 75);

  if(idle)
  {
    //Daca e in repaus, opreste led-urile pentru jucatori si citeste intrarile analog pentru a verifica daca ceva a fost apasat
    turnOffLeds();

    int somethingPressed = (analogRead(PIN_PLAYER1_BUTTONS) + analogRead(PIN_PLAYER2_BUTTONS)) / 100;

    if(somethingPressed)
    {
      //Daca ceva a fost apasat prin SPI se trimit valoarea 15
      SPDR = 15;
    }
  }else
  {
    if(!wait)
    {
      //Daca a inceput o runda noua

      toneChanging = 0;

      //Verifica daca byte-ul de date primit are unul din primii patru biti 1
      if(data >> 4)
      {
        //Daca da opreste toate led-urile jucatorilor si il aprinda doar pe cel care corespunde jucatorului curent, led-ului ales de master
        turnOffLeds();

        if(data & (1 << 7)) //Jucatorul 2
        {
          playerRound = 1;

          if(data & (1 << 6))
            digitalWrite(PIN_PALYER2_GREEN_LED, HIGH);
          else if(data & (1 << 5))
            digitalWrite(PIN_PALYER2_YELLOW_LED, HIGH);
          else if(data & (1 << 4))
            analogWrite(PIN_PALYER2_BLUE_LED, 100);

        }else //Jucatorul 1
        {
          playerRound = 0;

          if(data & (1 << 6))
            analogWrite(PIN_PALYER1_GREEN_LED, 75);
          else if(data & (1 << 5))
          {
            digitalWrite(PIN_PALYER1_RED_LED, HIGH);
            analogWrite(PIN_PALYER1_GREEN_LED, 100);
          }else if(data & (1 << 4))
            digitalWrite(PIN_PALYER1_BLUE_LED, HIGH);
        }
      }

      if(!playerRound) //Jucatorul 1
      {
        dataSent = 0;

        valueAnalog = analogRead(PIN_PLAYER1_BUTTONS);
        
        if(valueInRange(valueAnalog, PLAYER1_BUTTON_GREEN))
          dataSent |= (1 << 2);//data |= (1 << 2);
        else if(valueInRange(valueAnalog, PLAYER1_BUTTON_YELLOW))
          dataSent |= (1 << 1);//data |= (1 << 1);
        else if(valueInRange(valueAnalog, PLAYER1_BUTTON_BLUE))
          dataSent |= (1 << 0);//data |= (1 << 0);
        else
          dataSent = 0;

      }else //Jucatorul 2
      {
        dataSent = 8;

        valueAnalog = analogRead(PIN_PLAYER2_BUTTONS);

        if(valueInRange(valueAnalog, PLAYER2_BUTTON_GREEN))
          dataSent |= (1 << 2);//data |= (1 << 2);
        else if(valueInRange(valueAnalog, PLAYER2_BUTTON_YELLOW))
          dataSent |= (1 << 1);//data |= (1 << 1);
        else if(valueInRange(valueAnalog, PLAYER2_BUTTON_BLUE))
          dataSent |= (1 << 0);//data |= (1 << 0);
        else
          dataSent = 0;
      }

      SPDR = dataSent;
      
    }else
    {
      //Daca e in starea de asteptare dintre runde, led-urile jucatorilor se opresc si se trimite prin SPI 0
      turnOffLeds();
      SPDR = 0;
    }
  }
}