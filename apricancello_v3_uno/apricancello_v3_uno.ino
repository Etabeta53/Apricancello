/*
APRICANCELLO BY MARCO DALL'OLMO  9-5-2024 


  // initialize digital pin LED_BUILTIN as an output.
  */

static unsigned long t1, dt;
static unsigned long t2, dt2;
boolean RUN = false;
boolean RUN1 = false;
int remote = 8;
int apre = 3;
int chiude = 2;
int lampeggiante = 4;
int fotocellula = 10;
int finecorsa_apre = 6;
int finecorsa_chiude = 7;
int dir = 0;
//int motorea = 0;
int motorec = 0;
int moto = 0;
int i = 0;
char str[100];
int tempo1 = 10000;


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



// the loop function runs over and over again forever
void loop() {


  if (digitalRead(remote) != 0) {
    digitalWrite(lampeggiante, LOW);
    moto = 1;
    for (i = 1; i < 2; i++) {
      //stato();
      if ((digitalRead(remote) != 0) && (moto = 1)) {
        moto = 0;
        //motorea = 0;
        motorec = 0;
        digitalWrite(lampeggiante, HIGH);
        break;
      }
    }
  }
  if (moto != 0) {
    switch (dir) {
      case 0:

        //stato();
        apertura();
        break;
      case 1:

        //stato();
        chiusura();
        break;
    }
  }
  dt = millis() - t1;
  if ((dt >= 40000) && RUN) {
    chiusura();
    RUN = false;
  }
}

void apertura() {

  while (digitalRead(finecorsa_apre) != 0) {
    t2 = millis();
    RUN1 = true;
    digitalWrite(lampeggiante, LOW);
    //motorea = 1;
    dt2 = millis() - t2;
    if ((dt2 >= 1000) && RUN1) {
      RUN1 = false;
    }

    //Serial.println(digitalRead(fotocellula));
    if (digitalRead(fotocellula) == 1) {
      moto = 0;
      emergenza();
      //digitalWrite(lampeggiante, LOW);
      break;
    }
    digitalWrite(apre, LOW);
    //tone(11, 700, 100);
    delay(500);


    digitalWrite(lampeggiante, HIGH);
    delay(500);
    /*dt2 = millis() - t2;
    if ((dt2 >= 1000) && RUN1) {
      RUN1 = false;*/
    if (digitalRead(remote) != 0) {
      //motorea = 0;
      moto = 0;
      digitalWrite(apre, HIGH);
      delay(1000);
      break;
    }
  }
  dir = 1;
  //motorea = 0;
  moto = 0;
  digitalWrite(apre, HIGH);
  t1 = millis();
  RUN = true;

  // chiusura();
}

void chiusura() {
  RUN = false;
  while (digitalRead(finecorsa_chiude) != 0) {
    digitalWrite(lampeggiante, LOW);
    motorec = 1;
    if (digitalRead (apre) == HIGH) {
      digitalWrite(chiude, LOW);
      if (digitalRead(fotocellula) == 1) {
        moto = 0;
        dir = 0;
        emergenza();
        break;
      }
    }
    delay(700);
    digitalWrite(lampeggiante, HIGH);
    delay(700);
    if (digitalRead(remote) != 0) {
      motorec = 0;
      moto = 0;
      digitalWrite(chiude, HIGH);
      break;
    }
  }
  digitalWrite(chiude, HIGH);
  dir = 0;
  motorec = 0;
  moto = 0;
  delay(1000);
}
void emergenza() {

  while (digitalRead(fotocellula) == 1) {
    //stato();
    digitalWrite(apre, HIGH);
    digitalWrite(chiude, HIGH);
    digitalWrite(lampeggiante, LOW);
    delay(300);
    digitalWrite(lampeggiante, HIGH);
    delay(300);
  }
  //delay(5000);
  if (digitalRead(fotocellula) != 1) {
    apertura();
  }
}
/*void stato() {

  Serial.print("STATO - ");
  sprintf(str, "input %01d %01d %01d %01d ",
          digitalRead(8),
          digitalRead(7),
          digitalRead(6),
          digitalRead(5));
  Serial.println(str);
}*/
