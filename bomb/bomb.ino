bool RevArreglo(char *ClaveD, char *ClaveU) {
  int ConArreglo = 0;
  for (int i = 0; i < 7; i++) {
    if (ClaveD[i] == ClaveU[i])
      ConArreglo = ConArreglo + 1;
  }
  if (ConArreglo == 7) {
    return true;
  }
  else {
    return false;
  }
}

#define LEDCONTA 14
#define LEDSAL 25
#define UP 13
#define DOWN 32
#define ARM 33
static uint8_t Contador = 20;
static int x = 0, y = 5, EstBomba = 1, Resul = 0, N1 = 1, N2 = 1, Btn1 = 1, Btn2 = 1, Btn3 = 1;
static char ClaveD[7] = {'u', 'u', 'd', 'd', 'u', 'd', 'a'};
static char ClaveU[7];
void setup() {
  Serial.begin(115200);

  digitalWrite(LEDCONTA, HIGH);
  digitalWrite(LEDSAL, LOW);
  pinMode(LEDCONTA, OUTPUT);
  pinMode(LEDSAL, OUTPUT);
  pinMode(UP, INPUT_PULLUP);
  pinMode(DOWN, INPUT_PULLUP);
  pinMode(ARM, INPUT_PULLUP);

}

void EstadoBomba() {
  switch (EstBomba) {
    case 1: {
        digitalWrite(LEDCONTA, HIGH);
        digitalWrite(LEDSAL, LOW);

        if (digitalRead(UP) == 0) {
          Resul = 1;
          N1 = 0;
        }
        if (digitalRead(DOWN) == 0) {
          Resul = -1;
          N2 = 0;
        }

        if (digitalRead(UP) == 1 && digitalRead(DOWN) == 1) {
          if (N1 == 0 || N2 == 0) {
            if (Contador <= 60 && Contador >= 10) {
              Serial.println(Contador);
              Contador = (Contador + Resul);
            }
            Contador = (Contador);
          }
          N1 = 1;
          N2 = 1;
        }

        if (digitalRead(ARM) == 0) {
          EstBomba = 2;
        }

        break;
      }
    case 2: {
        static uint32_t previousMillis = 0;
        static uint8_t LEDsal = LOW;
        const uint32_t interval = 1000;
        uint32_t currentMillis = millis();

        digitalWrite(LEDCONTA, LOW);
        if (currentMillis - previousMillis >= 1000) {
          previousMillis = currentMillis;
          if (LEDsal == LOW) {
            LEDsal = HIGH;
          } else {
            LEDsal = LOW;
          }
          digitalWrite(LEDSAL, LEDsal);
          if  (Contador > 0 ) {
            Contador--;
          }
          Serial.println(Contador);
        }
        ArregloClave();

        if (Contador == 0) {
          Serial.println(Contador);
          EstBomba = 1;
          Contador = 20;
        }

        break;
      }
    default:
      Serial.println("ERROR");
      break;
  }

}

void ArregloClave() {
  static char ValBoton;
  static int ConBoton = 0;

  int b1 = 1, b2 = 1, b3 = 1, bt = 1;
  if (digitalRead(UP) == 0) {
    ValBoton = 'u';
    Btn1 = 0;
  }
  if (digitalRead(DOWN) == 0) {
    ValBoton = 'd';
    Btn2 = 0;
  }
  if (digitalRead(ARM) == 0) {
    ValBoton = 'a';
    Btn3 = 0;
  }
  if (digitalRead(UP) == 1 && digitalRead(DOWN) == 1 && digitalRead(ARM) == 1) {
    if (Btn1 == 0 || Btn2 == 0 || Btn3 == 0) {
      ClaveU[ConBoton] = ValBoton;
      ConBoton++;
      Serial.println("La clave actual es:");
      Serial.println(ValBoton);
    }
  }
  if (ConBoton == 7) {
    if (RevArreglo(ClaveD, ClaveU)) {
      EstBomba = 1;
      Contador = 20;
    }
    ConBoton = 0;
  }
}

void loop() {
  EstadoBomba();
}
