# IaraWatch pH Monitor – Arduino + ESP8266 + MQTT

IARAWATCH é um sistema de monitoramento de qualidade da água baseado em Arduino, projetado para medir e transmitir valores de pH em tempo real. O projeto utiliza dois microcontroladores — ATmega328P (Arduino UNO) e ESP8266 — que atuam em conjunto: o ATmega realiza a leitura analógica do sensor de pH e controla o LED (atuador), enquanto o ESP8266 se conecta à rede Wi-Fi e envia os dados via protocolo MQTT para um broker local (Mosquitto).

Os dados enviados incluem:

- Valor de tensão lido do sensor,

- Valor de pH convertido via fórmula calibrada,

- Tempo de reação do LED, caso o pH esteja fora da faixa segura (6.5–8.5),

- Timestamp da leitura, sincronizado com um servidor NTP (hora UTC-3).

- O sistema exibe os dados em um display LCD 16x2 e também pode ser monitorado por um script Python que calcula automaticamente o tempo médio de resposta do atuador e da comunicação MQTT.

Este projeto foi pensado para ser acessível, modular e didático, permitindo que estudantes, pesquisadores ou entusiastas em automação e IoT possam reproduzi-lo com facilidade utilizando componentes comuns e de baixo custo.

## Hardware utilizado:

### A) Placa Arduino Uno WiFi (ATmega328P + ESP8266)

A placa Arduino Uno WiFi combina o microcontrolador ATmega328P com o módulo Wi-Fi ESP8266, permitindo conectividade sem fio e controle de dispositivos via internet. Possui 14 pinos digitais (6 com PWM), 6 entradas analógicas, cristal de 16 MHz e interface USB-TTL CH340G. A comunicação entre o ATmega328P e o ESP8266 é configurável por meio de DIP switches, permitindo flexibilidade na programação e na comunicação serial.

![Placa Arduino Uno WiFi (ATmega328P + ESP8266)](Assets/Arduino.jpg) 

#### Especificações Arduino Uno WiFi (ATmega328P + ESP8266):

- Modelo: Placa Uno Wifi Atmega328P;
- Alimentação Jack P4: 6-9VDC;
- Entrada de energia USB: 5V (500mA máx.);
- Microcontrolador: ATmega328P SMD;
- Wi-Fi IC: ESP8266;
- Conversor USB-TTL: CH340G;
- Nível lógico: 5V;
- Wi-Fi: 802.11 b/g/n 2.4 GHz;
- Conexão: Micro USB;
- Frequência de clock: 16MHz;
- Pinos: Digitais = 14 (6 PWM) / Analógicos = 6;
- Memória: 32kb;
- RAM: 2Kb;
- ROM: 2Kb;
- Temperatura de operação: -40 a 125°C;
- Dimensões (CxLxE): 68x53x10mm;
- Peso: 22g.

### B) Sensor de pH PH-4502C

![Sensor PH-4502C](Imagens/PH-4502C.jpg) 

O sensor de pH PH-4502C é utilizado para medir o nível de acidez ou alcalinidade de soluções líquidas, convertendo o valor de pH em um sinal analógico compreendido pelo microcontrolador. Ele opera com tensão de 5V DC, possui faixa de medição entre pH 0 e 14, tempo de resposta ≤5s e precisão de ±0,1 pH a 25°C. O módulo acompanha dois potenciômetros integrados: um para ajuste de offset (calibração) e outro para definir o limite de pH de atuação.

Neste projeto, o sensor é o responsável pela coleta do valor de pH da água, enviando os dados ao ATmega328P, que realiza a conversão e posterior transmissão ao módulo ESP8266 para exibição e monitoramento via MQTT.

#### Especificações PH-4502C:

- Modelo: PH-4502C;
- Tensão de aquecimento: 5 ±0.2V (AC/DC);
- Corrente de trabalho: 5-10mA;
- Faixa de temperatura: 0-60℃;
- Tempo de resposta: 5S;
- Tempo de sedimentação: 60S;
- Componente Potência: 0,5 W;
- Saída: Analógica;
- Faixa de medição: 0,00 ~ 14,00 pH
- Zero pontos: 7 +-0.5ph;
- Erro alcalino: 0.2pH;
- Resistência interna: <250MOhm;
- Blocos de terminais: Plug BNC;
- Comprimento do cabo: 70cm;
- Dimensões do módulo (CxLxE): 42x32x13mm (ignorando-se conectores);
- Dimensões do sensor (CxD): 170x12,5mm;
- Peso: 75g;
- Peso com embalagem: 85g.

### C) Display LCD 16x2 com Interface I2C

![Display LCD 16x2 com Interface I2C](Imagens/DisplayLCD.jpg) 

O display LCD 16x2 com interface I2C é utilizado neste projeto para exibição local das medições realizadas pelo sensor, como o valor de pH da água e a condição atual ("Própria" ou "Imprópria"). Sua integração com o ESP8266 via barramento I2C (utilizando apenas os pinos SDA e SCL) permite uma comunicação eficiente com o microcontrolador, economizando pinos digitais para outras conexões.

Além disso, o módulo conta com retroiluminação azul, caracteres brancos e ajuste de contraste via potenciômetro integrado, o que garante boa visualização em ambientes com pouca luz, mesmo quando o sistema estiver sem conexão com o servidor.

#### Especificações LCD 16x2 I2C:

- Modelo: DFR0063
- Tipo de Display: Alfanumérico (16 caracteres por 2 linhas)
- Controlador: HD44780
- Interface de Comunicação: I2C (TWI)
- Tensão de Operação: 5V DC
- Corrente de Operação: ≤60mA
- Endereço I2C padrão: 0x20 (configurável entre 0x20 e 0x27)
- Pinos de Conexão: VCC (5V), GND, SDA (dados), SCL (clock)
- Ajuste de Contraste: Potenciômetro integrado
- Retroiluminação: LED azul com caracteres brancos
- Dimensões: 80 x 36 x 20 mm
- Compatibilidade: Arduino UNO, Mega, Nano e similares

### D) Protoboard

![Protoboard 400 pontos](Imagens/Protoboard.jpg)

A protoboard é amplamente utilizada na fase de prototipagem do projeto, permitindo a montagem dos circuitos sem necessidade de solda. Isso facilita o teste, ajuste e organização dos componentes como sensores, atuadores, jumpers e o microcontrolador.

Sua estrutura conta com trilhas verticais e horizontais, barramentos laterais de alimentação (positiva e negativa) e furos que suportam cabos jumper padrão. No projeto, ela é responsável por distribuir a alimentação fornecida pelo Arduino para o display, sensor de pH e LED, de maneira modular e segura.

#### Especificações Técnicas Protoboard 400 Pontos:

- Protoboard 400 Pontos Solderless Breadboard
- Modelo: Solderless Breadboard
- Marca: OEM
- Ideal para protótipos de circuitos eletrônicos para futuros projetos
- Base adesivada
- Furo padrão 0.8mm
- Corrente máxima: 5A
- Tensão máxima: 300V
- Cor: Branco
- Composição: ABS
- Origem: China
- Tamanho: 82mm Comprimento x 55mm Largura x 10mm de Altura
- Peso: 43g

### E) LED Difuso 5mm

![Protoboard 400 pontos](Imagens/LED.jpg)

O LED difuso de 5mm é utilizado como atuador visual no projeto. Ele acende sempre que o valor de pH sai da faixa considerada segura, indicando uma condição de alerta. É conectado ao pino digital D9 do ATmega328P e tem seu brilho controlado por meio de sinal PWM.

Este componente torna visível e imediata a resposta do sistema às medições do sensor de pH, sendo essencial para testes de tempo de reação e validação do funcionamento do atuador.

#### Especificações Técnicas do LED Difuso 5mm:

- Tamanho: 5 mm
- Tensão de operação: 2.0 ~ 2.21 V
- Corrente típica: 20 mA
- Intensidade luminosa: 1000 mcd
- Vida útil: 50.000 horas
- Ângulo de abertura: 120°
- Cor: Vermelho difuso
