# Descrição dos Componentes

**_<h1>ESP32</h1>_**

Conectividade Wi-Fi embutida, ideal para aplicações IoT.

Configurado para implementar o protocolo MQTT, possibilitando comunicação remota eficiente.

Interface flexível para conectar sensores e atuadores diversos.



**_<h1>Protoboard</h1>_**

Modelo com 420 pinos.

Barras laterais utilizadas para distribuir alimentação (VCC e GND).

Área central de prototipagem para conectar resistores, LEDs e interligação com o ESP32.



**_<h1>Sensor de Umidade</h1>_**

Modelo analógico, utilizado para monitorar a umidade do solo.


Especificações Técnicas:

Tensão de operação: 3.3V a 5V.

Saída: Analógica, com valores que variam conforme o nível de umidade do solo.

Conexão: 3 pinos - VCC, GND, e A0 (sinal analógico).



**_<h1>Atuador (Válvula Solenóide)</h1>_**

Utilizada para controlar o fluxo de água no sistema de irrigação.


Especificações Técnicas:

Tensão de operação: 12V.

Tipo: Normally Closed (NC).

Pressão de operação: 0.02 - 0.8 MPa.



**_<h1>Método de Implementação:</h1>_**

Controlada por um relé, acionado pelo ESP32 com base nas leituras do sensor de umidade.


**<h2>Esquema de Montagem</h2>**

O circuito conecta um ESP32, um sensor de umidade, um relé e uma válvula solenóide em uma protoboard.

O sinal do módulo relé é ligado à porta 25 do ESP32. O sensor de umidade é conectado à porta 33 do ESP32.

A alimentação do ESP32 é realizada via barramento da protoboard, enquanto a válvula solenóide utiliza o relé como intermediário para controlar o fluxo de energia.

Esquema do Circuito
<img src="media/media/Esquema de circuito.jpg" />


