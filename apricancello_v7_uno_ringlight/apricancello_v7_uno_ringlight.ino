/*
APRICANCELLO BY MARCO DALL'OLMO  9-5-2024 
Aggiornato 4-10-25
*/
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>  // Required for 16 MHz Adafruit Trinket
#endif

#define PIN 5  // D_in pin

#define NUMPIXELS 45  // ring size

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 10  // Time (in milliseconds) to pause between pixels

#define INTERVALLOA 200        //intrevallo lampeggio asincriono
#define INTERVALLOB 2500       //intrevallo lampeggio asincriono
static unsigned long t1, dt;   //Timer 1 non bloccante
static unsigned long t2, dt2;  //Timer 2 non bloccante
boolean RUN = false;
boolean RUN1 = false;
int remote = 8;            //Ingresso telecomando
int apre = 3;              //Uscita motore apre
int chiude = 2;            //Uscita motore chiude
int lampeggiante = 4;      //Uscita lampeggiante
int ring = 5;              //Uscita ring
int fotocellula = 10;      //Ingresso fotocellula
int finecorsa_apre = 6;    //Ingresso finecorsa apertura
int finecorsa_chiude = 7;  //Ingresso finecorsa chiusura
int dir = 0;               //Direzione motore
int moto = 0;              //Stato motore
int i = 0;
int fase = 0;
int stp = 0;



void setup() {
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  pixels.begin();  // INITIALIZE NeoPixel strip object (REQUIRED)


  pinMode(apre, OUTPUT);
  pinMode(chiude, OUTPUT);
  pinMode(lampeggiante, OUTPUT);
  pinMode(fotocellula, INPUT_PULLUP);
  pinMode(remote, INPUT);
  pinMode(finecorsa_apre, INPUT_PULLUP);
  pinMode(finecorsa_chiude, INPUT_PULLUP);
  Serial.begin(115200);
  digitalWrite(lampeggiante, HIGH);
  digitalWrite(apre, HIGH);
  digitalWrite(chiude, HIGH);
}



void loop() {



  if (digitalRead(remote) != 0) {
    digitalWrite(lampeggiante, LOW);
    moto = 1;
    for (i = 1; i < 2; i++) {
      delay(300);
      if ((digitalRead(remote) != 0) && (moto = 1)) {
        moto = 0;
        digitalWrite(lampeggiante, HIGH);
        break;
      }
    }
  }
  if (moto != 0) {
    switch (dir) {
      case 0:
        apertura();
        break;
      case 1:
        chiusura();
        break;
    }
  }
  dt = millis() - t1;
  int tempo=dt;
  if ((dt >= 40000) && RUN) {
    chiusura();
    RUN = false;
  }
  if (dir == 1 && RUN) {
    dt2 = millis() - t2;
    if (dt2 >= stp) {
      if (fase == 0) {
        stp = INTERVALLOA;
        fase = 1;
        digitalWrite(lampeggiante, !digitalRead(lampeggiante));
        
        tempochiude(tempo);
      } else {
        stp = INTERVALLOB;
        fase = 0;
        digitalWrite(lampeggiante, !digitalRead(lampeggiante));
      }
      t2 = millis();
    }
  }
}

void apertura() {
  while (digitalRead(finecorsa_apre) != 0) {

    digitalWrite(apre, LOW);
    dt2 = millis() - t2;
    if (dt2 >= 700) {
      digitalWrite(lampeggiante, !digitalRead(lampeggiante));
      t2 = millis();
      lucechiude();
    }

    if (digitalRead(fotocellula) == 1) {
      moto = 0;
      emergenza();
      break;
    }
    if (digitalRead(remote) != 0) {
      digitalWrite(lampeggiante, HIGH);
      moto = 0;
      digitalWrite(apre, HIGH);
      delay(1000);
      break;
    }
  }

  dir = 1;
  moto = 0;
  digitalWrite(apre, HIGH);
  digitalWrite(lampeggiante, HIGH);
  t1 = millis();
  RUN = true;
}

void chiusura() {
  RUN = false;
  while (digitalRead(finecorsa_chiude) != 0) {
    digitalWrite(chiude, LOW);
    dt2 = millis() - t2;
    if (dt2 >= 700) {
      digitalWrite(lampeggiante, !digitalRead(lampeggiante));
      t2 = millis();
    luceapre();
    }

    if (digitalRead(fotocellula) == 1) {
      moto = 0;
      dir = 0;
      emergenza();
      break;
    }
    if (digitalRead(remote) != 0) {
      moto = 0;
      dir = 0;
      digitalWrite(chiude, HIGH);
      break;
    }
  }
  digitalWrite(chiude, HIGH);
  digitalWrite(lampeggiante, HIGH);
  delay(500);
  digitalWrite(lampeggiante, LOW);
  delay(500);
  digitalWrite(lampeggiante, HIGH);
  delay(500);
  digitalWrite(lampeggiante, LOW);
  delay(500);
  digitalWrite(lampeggiante, HIGH);
  delay(500);
  dir = 0;
  moto = 0;
}
void emergenza() {
  lucestop();
  while (digitalRead(fotocellula) == 1) {
    digitalWrite(apre, HIGH);
    digitalWrite(chiude, HIGH);
    digitalWrite(lampeggiante, LOW);
    delay(300);
    digitalWrite(lampeggiante, HIGH);
    delay(300);
  }
  if (digitalRead(fotocellula) != 1) {
    apertura();
  }
}
void luceapre()

{
  pixels.clear();  // Set all pixel colors to 'off'


  for (int i = 0; i < 58; i++) {

    pixels.setPixelColor(i, pixels.Color(0, 150, 0));
    pixels.setPixelColor(i - 6, pixels.Color(0, 90, 0));
    pixels.setPixelColor(i - 7, pixels.Color(0, 70, 0));
    pixels.setPixelColor(i - 8, pixels.Color(0, 50, 0));
    pixels.setPixelColor(i - 9, pixels.Color(0, 40, 0));
    pixels.setPixelColor(i - 10, pixels.Color(0, 30, 0));
    pixels.setPixelColor(i - 11, pixels.Color(0, 20, 0));
    pixels.setPixelColor(i - 12, pixels.Color(0, 10, 0));
    pixels.setPixelColor(i - 13, pixels.Color(0, 0, 0));


    pixels.show();  // Send the updated pixel colors to the hardware.

    delay(DELAYVAL);  // Pause before next pass through loop
  }
}

void lucechiude()

{
  pixels.clear();  // Set all pixel colors to 'off'
  
  for (int i = 45; i > -14; i--) {
    //Serial.println(i);
    pixels.setPixelColor(i, pixels.Color(150, 0, 0));
    pixels.setPixelColor(i + 6, pixels.Color(60, 0, 0));
    pixels.setPixelColor(i + 7, pixels.Color(70, 0, 0));
    pixels.setPixelColor(i + 8, pixels.Color(50, 0, 0));
    pixels.setPixelColor(i + 9, pixels.Color(60, 0, 0));
    pixels.setPixelColor(i + 10, pixels.Color(30, 0, 0));
    pixels.setPixelColor(i + 11, pixels.Color(20, 0, 0));
    pixels.setPixelColor(i + 12, pixels.Color(10, 0, 0));
    pixels.setPixelColor(i + 13, pixels.Color(0, 0, 0));

    pixels.show();  // Send the updated pixel colors to the hardware.

    delay(DELAYVAL);  // Pause before next pass through loop
  }
}

void lucefotocellula()

{

  pixels.clear();  // Set all pixel colors to 'off'
  for (int i = 0; i < 45; i++) {
    //Serial.println(i);
    pixels.setPixelColor(i, pixels.Color(255, 96, 00));

    pixels.show();  // Send the updated pixel colors to the hardware.

    //pixels.clear();  // Set all pixel colors to 'off'
    //pixels.show();   // Send the updated pixel colors to the hardware.
    //delay(5);
  }
  delay(500);
  pixels.clear();  // Set all pixel colors to 'off'
  pixels.show();   // Send the updated pixel colors to the hardware.
}

void lucestop()

{
  pixels.clear();  // Set all pixel colors to 'off'
  for (int i = 0; i < 45; i++) {
    //Serial.println(i);
    pixels.setPixelColor(i, pixels.Color(150, 0, 0));


    pixels.show();  // Send the updated pixel colors to the hardware.

    // delay(DELAYVAL);  // Pause before next pass through loop
  }
}

int tempochiude(int tempo)
{
int tempo1=map (tempo,0,400,0,45) ;

Serial.println((tempo1/100)+3);
 pixels.clear();  // Set all pixel colors to 'off'
  for (int i = 0; i < (tempo1/100)+3; i++) {
    //Serial.println(i);
    pixels.setPixelColor(i, pixels.Color(255, 96, 00));
    pixels.show();  // Send the updated pixel colors to the hardware.
  }
return tempo;
}
