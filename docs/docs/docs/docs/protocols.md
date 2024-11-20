# IoT para Monitoramento e Otimização de Processos em Vinícola

## 1. O Protocolo MQTT

O **MQTT** (Message Queuing Telemetry Transport) é um protocolo de mensagens leve, utilizado principalmente em sistemas de Internet das Coisas (IoT) para a troca de dados entre dispositivos. Ele segue o modelo de comunicação **publish/subscribe**, onde os dispositivos podem **publicar** mensagens em tópicos e **inscrever-se** em tópicos para receber essas mensagens.

### Características do MQTT:
- **Modelo Publish/Subscribe**: Em vez de comunicação ponto a ponto, a comunicação acontece através de tópicos.
- **Leveza**: Ideal para dispositivos com recursos limitados, como sensores.
- **Assíncrono**: Os dispositivos não precisam estar sempre conectados, o que facilita a comunicação em tempo real.
- **Escalabilidade**: Permite a comunicação entre milhares de dispositivos sem grandes custos.

### Qualidade de Serviço (QoS)
O MQTT define três níveis de QoS:
- **QoS 0**: A mensagem é entregue no máximo uma vez, sem garantia de entrega.
- **QoS 1**: A mensagem é entregue pelo menos uma vez.
- **QoS 2**: A mensagem é entregue exatamente uma vez.

## 2. Funcionamento do Sistema

No contexto do monitoramento de uma vinícola, os sensores de umidade do solo, temperatura e outros dispositivos IoT coletam dados em tempo real. Esses dados são enviados via MQTT para um broker, que os distribui aos dispositivos ou sistemas que se inscrevem nos tópicos relevantes.

### Fluxo de Dados:
1. **Dispositivos Sensores**: Dispositivos como o **ESP32** capturam dados de sensores.
2. **Broker MQTT**: Os dados são enviados ao broker, como o **Mosquitto**, que gerencia as mensagens.
3. **Aplicações de Monitoramento**: Interfaces gráficas, como uma aplicação **Streamlit**, podem se inscrever nos tópicos para visualizar os dados em tempo real e controlar o sistema.

## 3. Exemplos de Mensagens MQTT

As mensagens MQTT para o monitoramento e controle da vinícola podem ser enviadas em formato **JSON**. Aqui estão exemplos de mensagens:

### Exemplo 1: Dados de Umidade do Solo
json
{
  "sensor_id": "1",
  "umidade": 45.5,
  "data_hora": "2024-11-20T10:00:00"
}

### Exemplo 2: Dados de Umidade do Solo
json
{
  "irrigacao": "ON",
  "sendor_id": "1",
  "data_hora": "2024-11-20T10:05:00"
}

## 4. Configuração do Broker MQTT

O **Mosquitto** é um broker MQTT popular para gerenciamento de mensagens. Ele pode ser instalado e configurado facilmente em sistemas Linux.

### Instalação do Mosquitto:

Execute os seguintes comandos no terminal para instalar o Mosquitto:

-bash
  -sudo apt-get update
  -sudo apt-get install mosquitto mosquitto-clients

### Configuração do Mosquitto:
O arquivo de configuração do Mosquitto está localizado em /etc/mosquitto/mosquitto.conf. Um exemplo de configuração básica seria:

-conf

  -listener 1883
  -allow_anonymous true
  -persistence true
  
Após configurar o Mosquitto, você pode iniciar o serviço com:

-bash

  -sudo systemctl start mosquitto

### Testando o Broker MQTT:

-Para **publicar** uma mensagem:

  -bash

  -mosquitto_pub -h localhost -t "vinicola/umidade/solo" -m '{"sensor_id": "1", "umidade": 45.5}'

-Para **assinar** um tópico:

  -bash

  -mosquitto_sub -h localhost -t "vinicola/umidade/solo"

  -Esses comandos permitem verificar se a comunicação está funcionando corretamente entre o publisher e o subscriber.

## 5.Aplicação de Monitoramento com Streamlit

Para visualizar os dados de umidade e irrigação em tempo real, podemos usar Streamlit. O código abaixo exemplifica como configurar uma interface gráfica com Streamlit que se inscreve nos tópicos MQTT e exibe um gráfico em tempo real.

### Código do Cliente MQTT com Streamlit:

## Exemplo 2: Subscrever a um Tópico e Exibir Dados em Tempo Real com Streamlit

Este exemplo demonstra como configurar um cliente MQTT para assinar um tópico e exibir dados em tempo real com o Streamlit, além de usar o **gmqtt** para comunicação MQTT.

### Código Completo

**python**

import asyncio
import streamlit as st
import pandas as pd
import matplotlib.pyplot as plt
from datetime import datetime
from gmqtt import Client as MQTTClient

- Armazenamento de dados para o gráfico
data = {
    "time": [],
    "humidity": [],
    "is_watering": []
}

- Configuração do Streamlit
st.set_page_config(page_title="Real-Time Humidity Graph", layout="wide")
st.title("Real-Time Humidity Graph")

- Inicialização do gráfico
graph_placeholder = st.empty()
data_df = pd.DataFrame(data)

- Callback para lidar com mensagens recebidas
async def on_message(client, topic, payload, qos, properties):
    msg = payload.decode()
    print(f"Received message: {msg} on topic: {topic}")
    if topic == "graph/data":
        handle_new_data(msg)
        update_graph()

- Configuração do cliente MQTT
async def mqtt_main():
    client = MQTTClient("subscriber")
    client.on_message = on_message
    await client.connect("127.0.0.1", port=1883)
    client.subscribe("graph/data")
    await asyncio.Future()  # Mantém o script rodando

- Função para processar dados recebidos
def handle_new_data(msg):
    time_str, humidity_str, is_watering = msg.split(",")
    
    - Converter para formato de hora
    try:
        time_obj = datetime.strptime(time_str, "%Y-%m-%d %H:%M:%S")
        formatted_time = time_obj.strftime("%H:%M:%S")
    except ValueError:
        formatted_time = time_str

    humidity = int(humidity_str)
    data["time"].append(formatted_time)
    data["humidity"].append(humidity)
    data["is_watering"].append(is_watering == "1")

    - Limitar os dados a 150 pontos
    if len(data["time"]) > 150:
        data["time"].pop(0)
        data["humidity"].pop(0)
        data["is_watering"].pop(0)

- Função para atualizar o gráfico
def update_graph():
    data_df = pd.DataFrame(data)
    fig, ax = plt.subplots(figsize=(12, 3))

    - Desenhar as linhas com base no status de irrigação
    for i in range(1, len(data_df)):
        color = "blue" if data["is_watering"][i-1] and data["is_watering"][i] else "red"
        ax.plot(data_df["time"][i-1:i+1], data_df["humidity"][i-1:i+1], color=color, linewidth=1)

    - Adicionar pontos individuais com cores diferentes
    colors = ["blue" if is_watering else "red" for is_watering in data["is_watering"]]
    ax.scatter(data_df["time"], data_df["humidity"], c=colors, marker="o", zorder=5)

    - Configurações do gráfico
    ax.set_xlabel("Time")
    ax.set_ylabel("Humidity")
    ax.set_title("Real-Time Humidity Data")

    - Ajustar a exibição dos rótulos do eixo X
    num_labels = 6
    ax.set_xticks(data_df["time"][::max(1, len(data_df) // num_labels)])
    ax.tick_params(axis='x', rotation=45)

    - Exibir o gráfico
    graph_placeholder.pyplot(fig)
    plt.close(fig)

- Função principal
async def main():
    asyncio.create_task(mqtt_main())  # Executar MQTT em segundo plano
    while True:
        await asyncio.sleep(1)  # Manter o app Streamlit em execução

if __name__ == "__main__":
    asyncio.run(main())

## 6. Exemplo de Configuração do Publisher MQTT com Streamlit

Em um projeto de automação, você pode criar um publisher MQTT para controlar ações, como iniciar ou parar a irrigação. O exemplo abaixo demonstra um publisher simples que envia comandos para o broker MQTT.

### Código do Publisher MQTT:
import asyncio
from gmqtt import Client as MQTTClient

- Função principal para o publisher
async def main():
    client = MQTTClient("publisher")

    - Conectar ao broker MQTT
    await client.connect("192.168.15.7", port=1883)

    - Publicar uma mensagem de controle
    client.publish("test/topic", "Hello from the Publisher!")

    - Desconectar após a publicação
    await asyncio.sleep(1)  # Dar tempo para enviar a mensagem
    await client.disconnect()

if __name__ == "__main__":
    asyncio.run(main())
