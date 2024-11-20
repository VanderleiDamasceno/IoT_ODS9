Descrição dos Componentes

ESP32

Conectividade Wi-Fi embutida, ideal para aplicações IoT.

Configurado para implementar o protocolo MQTT, possibilitando comunicação remota eficiente.

Interface flexível para conectar sensores e atuadores diversos.


Protoboard

Modelo com 420 pinos.

Barras laterais utilizadas para distribuir alimentação (VCC e GND).

Área central de prototipagem para conectar resistores, LEDs e interligação com o ESP32.


Sensor de Umidade

Modelo analógico, utilizado para monitorar a umidade do solo.


Especificações Técnicas:

Tensão de operação: 3.3V a 5V.

Saída: Analógica, com valores que variam conforme o nível de umidade do solo.

Conexão: 3 pinos - VCC, GND, e A0 (sinal analógico).


Atuador (Válvula Solenóide)

Utilizada para controlar o fluxo de água no sistema de irrigação.


Especificações Técnicas:

Tensão de operação: 12V.

Tipo: Normally Closed (NC).

Pressão de operação: 0.02 - 0.8 MPa.


Método de Implementação:

Controlada por um relé, acionado pelo ESP32 com base nas leituras do sensor de umidade.


Esquema de Montagem

O circuito conecta um ESP32, um sensor de umidade, um relé e uma válvula solenóide em uma protoboard.

O sinal do módulo relé é ligado à porta 25 do ESP32. O sensor de umidade é conectado à porta 33 do ESP32.

A alimentação do ESP32 é realizada via barramento da protoboard, enquanto a válvula solenóide utiliza o relé como intermediário para controlar o fluxo de energia.

Esquema do Circuito
Incluir o arquivo de imagem correspondente ao esquema do circuito (media/esquema_circuito.png) no diretório indicado e validar os links e conexões com base na estrutura do projeto​


Certifique-se de incluir o arquivo de imagem correspondente ao esquema do circuito (media/esquema_circuito.png) no diretório indicado e validar os links e conexões com base na estrutura do projeto​ 
