const int ledPin = 9;  // Pino para LED

bool foraFaixaAnterior = false;
bool estadoLed = false;
bool tempoReacaoRegistrado = false;

unsigned long tempoInicioCritico = 0;
unsigned long ultimoBlink = 0;
unsigned long tempoReacao = 0;
unsigned long ultimoEnvioSerial = 0;

const unsigned long intervaloLeitura = 2000;
const unsigned long intervaloBlink = 1000;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  analogWrite(ledPin, 0);  // LED desligado inicialmente
}

void loop() {
  unsigned long agora = millis();

  static float tensao = 0;
  static float ph = 0;
  static int leitura = 0;

  leitura = analogRead(A0);
  tensao = leitura * (5.0 / 1023.0);
  ph = -5.964 * tensao + 22.875;

  bool foraDaFaixa = (ph < 6.5 || ph > 8.5);

  // Detecta entrada na faixa crítica
  if (foraDaFaixa && !foraFaixaAnterior) {
    tempoInicioCritico = agora;
    tempoReacaoRegistrado = false;
    foraFaixaAnterior = true;
  }

  // Detecta retorno à faixa segura
  if (!foraDaFaixa && foraFaixaAnterior) {
    analogWrite(ledPin, 0);
    estadoLed = false;
    foraFaixaAnterior = false;
    tempoReacao = 0;
    tempoReacaoRegistrado = false;
  }

  // Zera tempoReacao caso esteja em faixa segura (extra verificação)
  if (!foraDaFaixa) {
    tempoReacao = 0;
    tempoReacaoRegistrado = false;
  }

  // Piscar o LED enquanto estiver fora da faixa
  if (foraDaFaixa) {
    if (millis() - ultimoBlink >= intervaloBlink) {
      ultimoBlink = millis();
      estadoLed = !estadoLed;
      analogWrite(ledPin, estadoLed ? 64 : 0);

      // Captura tempo de reação na primeira piscada
      if (estadoLed && !tempoReacaoRegistrado) {
        tempoReacao = millis() - tempoInicioCritico;
        tempoReacaoRegistrado = true;
      }
    }
  } else {
    analogWrite(ledPin, 0);
    estadoLed = false;
  }

  // Envia dados a cada intervalo definido
  if (agora - ultimoEnvioSerial >= intervaloLeitura) {
    ultimoEnvioSerial = agora;

    Serial.print("Tensao: ");
    Serial.print(tensao, 3);
    Serial.print(" V | pH: ");
    Serial.print(ph, 2);

    if (tempoReacaoRegistrado) {
      Serial.print(" | LED: ");
      Serial.print(tempoReacao);
      Serial.println(" ms");

      Serial.print(tensao, 3);
      Serial.print(";");
      Serial.print(ph, 2);
      Serial.print(";");
      Serial.println(tempoReacao);
    } else {
      Serial.println(" | LED: 0 ms");

      Serial.print(tensao, 3);
      Serial.print(";");
      Serial.print(ph, 2);
      Serial.print(";");
      Serial.println("0");
    }
  }
}
