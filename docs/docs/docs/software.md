Documentação do Software
1. Explicação do Código-Fonte
O software desenvolvido foi implementado utilizando a plataforma ESP32 e a IDE Arduino, configurada para suportar as funcionalidades específicas do protocolo MQTT. A estrutura do código é composta pelos seguintes elementos principais:

Bibliotecas Utilizadas:

WiFi.h: Gerencia a conexão Wi-Fi do ESP32.
PubSubClient.h: Implementa o protocolo MQTT, possibilitando a publicação e assinatura de tópicos.
Fluxo do Código:

Inicialização do ESP32 com os parâmetros da rede Wi-Fi (SSID e senha).
Conexão ao Broker MQTT especificado (ex.: test.mosquitto.org).
Leitura contínua dos dados do sensor de umidade conectado ao pino A0.
Publicação dos dados do sensor no tópico MQTT configurado (ex.: vinicola/umidade).
Assinatura de tópicos para receber comandos de ativação/desativação do relé.
Controle automático da válvula solenóide com base nos valores de umidade e comandos recebidos.
2. Como Configurar e Carregar o Código no ESP32
Instalação das Ferramentas Necessárias:

Baixe e instale a IDE Arduino (disponível em arduino.cc).
Instale os drivers necessários para o ESP32.
Adicione o suporte à placa ESP32:
Vá em File > Preferences.
No campo "Additional Board Manager URLs", insira: https://dl.espressif.com/dl/package_esp32_index.json.
Acesse Tools > Board > Board Manager, pesquise por "ESP32" e instale.
Configuração do Ambiente:

Na IDE, selecione a placa ESP32: Tools > Board > ESP32 Dev Module.
Configure a porta serial correta em Tools > Port.
Carregando o Código:

Abra o código-fonte do projeto (arquivo .ino).
Edite as seguintes seções para sua rede e broker MQTT:
SSID e Senha da rede Wi-Fi.
Endereço do Broker MQTT e as credenciais (se necessário).
Clique em Upload para compilar e enviar o código ao ESP32.
Testando o Sistema:

Verifique a conexão via monitor serial (Tools > Serial Monitor).
Confirme as publicações e assinaturas nos tópicos MQTT configurados.
