# IaraWatch pH Monitor – Arduino + ESP8266 + MQTT

IARAWATCH é um sistema de monitoramento de qualidade da água baseado em Arduino, projetado para medir e transmitir valores de pH em tempo real. O projeto utiliza dois microcontroladores — ATmega328P (Arduino UNO) e ESP8266 — que atuam em conjunto: o ATmega realiza a leitura analógica do sensor de pH e controla o LED (atuador), enquanto o ESP8266 se conecta à rede Wi-Fi e envia os dados via protocolo MQTT para um broker local (Mosquitto).

Os dados enviados incluem:

  - Valor de tensão lido do sensor,

  - Valor de pH convertido via fórmula calibrada,

  - Tempo de reação do LED, caso o pH esteja fora da faixa segura (6.5–8.5),

  - Timestamp da leitura, sincronizado com um servidor NTP (hora UTC-3).

  - O sistema exibe os dados em um display LCD 16x2 e também pode ser monitorado por um script Python que calcula automaticamente o tempo médio de resposta do atuador e da comunicação MQTT.

Este projeto foi pensado para ser acessível, modular e didático, permitindo que estudantes, pesquisadores ou entusiastas em automação e IoT possam reproduzi-lo com facilidade utilizando componentes comuns e de baixo custo.

Hardware utilizado:

A) Placa Arduino Uno WiFi (ATmega328P + ESP8266)

A placa Arduino Uno WiFi combina o microcontrolador ATmega328P com o módulo Wi-Fi ESP8266, permitindo conectividade sem fio e controle de dispositivos via internet. Possui 14 pinos digitais (6 com PWM), 6 entradas analógicas, cristal de 16 MHz e interface USB-TTL CH340G. A comunicação entre o ATmega328P e o ESP8266 é configurável por meio de DIP switches, permitindo flexibilidade na programação e na comunicação serial.

![Protótipo montado](Assets/Arduino.jpg)

