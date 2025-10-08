/*
APRICANCELLO BY MARCO DALL'OLMO  9-5-2024 
Aggiornato 4-10-25
*/
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




void setup() {


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
}

void apertura() {
  while (digitalRead(finecorsa_apre) != 0) {

    digitalWrite(apre, LOW);
    digitalWrite(lampeggiante, LOW);


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
}

void chiusura() {
  //RUN = false;
  while (digitalRead(finecorsa_chiude) != 0) {
    digitalWrite(chiude, LOW);
    digitalWrite(lampeggiante, LOW);
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