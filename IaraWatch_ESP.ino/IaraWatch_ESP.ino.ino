#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <time.h>

#define SDA_PIN 4  // GPIO4
#define SCL_PIN 2  // GPIO2

LiquidCrystal_I2C lcd(0x27, 16, 2);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", -3 * 3600, 60000);  // UTC-3

const char* ssid = "Fernando";
const char* password = "JaN3108001";
const char* mqtt_server = "192.168.15.99";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

float ph = 0.0;
float tensao = 0.0;
int tempoReacaoLED = 0;
String condicaoAgua = "Desconhecida";

int telaAtual = 0;
unsigned long ultimoTrocaTela = 0;
const unsigned long intervaloTrocaTela = 3000;

void setup_wifi() {
  Serial.println("Conectando ao Wi-Fi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi conectado.");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando ao MQTT...");
    if (client.connect("IaraWatchClient")) {
      Serial.println("Conectado!");
    } else {
      Serial.print("Falhou, rc=");
      Serial.print(client.state());
      Serial.println(" tentando novamente em 5s...");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  Wire.begin(SDA_PIN, SCL_PIN);
  lcd.init();
  lcd.backlight();

  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  timeClient.begin();
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();
  timeClient.update();

  // --- Receber dados do ATmega via Serial ---
  if (Serial.available()) {
    String dado = Serial.readStringUntil('\n');
    dado.trim();

    int p1 = dado.indexOf(';');
    int p2 = dado.indexOf(';', p1 + 1);

    if (p1 > 0 && p2 > p1) {
      String tStr = dado.substring(0, p1);
      String phStr = dado.substring(p1 + 1, p2);
      String tempoStr = dado.substring(p2 + 1);

      float t = tStr.toFloat();
      float p = phStr.toFloat();
      int led = tempoStr.toInt();

      if (!isnan(t) && !isnan(p) && p > 0 && p < 14) {
        tensao = t;
        ph = p;
        tempoReacaoLED = led;
        condicaoAgua = (ph < 6.5 || ph > 8.5) ? "Impropria" : "Propria";

        time_t ts = timeClient.getEpochTime();
        String payload = String(tensao, 3) + ";" + String(ph, 2) + ";" + String(tempoReacaoLED) + ";" + String(ts);
        bool sucesso = client.publish("pH/valor", payload.c_str(), true);

        Serial.print("Tensão: ");
        Serial.print(tensao, 3);
        Serial.print(" V | pH: ");
        Serial.print(ph, 2);
        Serial.print(" | LED: ");
        Serial.print(tempoReacaoLED);
        Serial.print("ms | Condição: ");
        Serial.print(condicaoAgua);
        Serial.print(" | MQTT: ");
        Serial.println(sucesso ? "✔️" : "❌");
      } else {
        Serial.println("Dados inválidos recebidos: " + dado);
      }
    } else {
      Serial.println("Formato inválido: " + dado);
    }
  }

  // LCD alternância
  if (millis() - ultimoTrocaTela > intervaloTrocaTela) {
    lcd.clear();

    if (telaAtual == 0) {
      time_t epoch = timeClient.getEpochTime();
      struct tm* timeinfo = localtime(&epoch);

      char dataStr[17];
      char horaStr[17];

      snprintf(dataStr, sizeof(dataStr), "Dia: %02d/%02d/%04d",
               timeinfo->tm_mday,
               timeinfo->tm_mon + 1,
               timeinfo->tm_year + 1900);

      snprintf(horaStr, sizeof(horaStr), "Horario: %02d:%02d",
               timeinfo->tm_hour,
               timeinfo->tm_min);

      lcd.setCursor(0, 0);
      lcd.print(dataStr);
      lcd.setCursor(0, 1);
      lcd.print(horaStr);
    }
    else if (telaAtual == 1) {
      lcd.setCursor(0, 0);
      lcd.print("pH: ");
      lcd.print(ph, 2);
      lcd.setCursor(0, 1);
      lcd.print("Cond: ");
      lcd.print(condicaoAgua);
    }

    telaAtual = (telaAtual + 1) % 2;
    ultimoTrocaTela = millis();
  }

  delay(500);
}
