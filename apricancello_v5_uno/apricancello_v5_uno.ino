/*
APRICANCELLO BY MARCO DALL'OLMO  9-5-2024 
Aggiornato 4-10-25
  */
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
int fotocellula = 10;      //Ingresso fotocellula
int finecorsa_apre = 6;    //Ingresso finecorsa apertura
int finecorsa_chiude = 7;  //Ingresso finecorsa chiusura
int dir = 0;               //Direzione motore
int moto = 0;              //Stato motore
int i = 0;
int fase = 0;
int stp = 0;



void setup() {


  pinMode(apre, OUTPUT);
  pinMode(chiude, OUTPUT);
  pinMode(lampeggiante, OUTPUT);
  pinMode(fotocellula, INPUT_PULLUP);
  pinMode(remote, INPUT_PULLUP);
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