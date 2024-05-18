# Detector de quedas

Este projeto foi desenvolvido para detectar quedas usando um módulo MPU6050 (acelerômetro e giroscópio) e uma placa ESP32 para comunicação WiFi. Quando uma queda é detectada, a ESP32 envia uma mensagem a um broker MQTT HiveMQ. Todo o código foi desenvolvido e testado usando o simulador Wokwi e está disponível neste repositório para reprodução e estudos. 

## Software Desenvolvido
O software desenvolvido para este projeto inclui código para:

- Leitura dos dados do sensor MPU6050.
- Processamento dos dados para detectar quedas.
- Conexão à rede WiFi.
- Envio de mensagens MQTT ao broker HiveMQ.

## Hardware Utilizado
- ESP32: Placa de desenvolvimento com WiFi e Bluetooth integrados.
- MPU6050: Módulo acelerômetro e giroscópio para detectar movimentos e quedas.
- LEDs: Para indicação visual (opcional).

## Documentação das Interfaces e Protocolos de Comunicação
### Interface WiFi
- Utilizada para conectar a ESP32 à rede local.
### Protocolo MQTT
- Broker MQTT: HiveMQ (broker.hivemq.com).
- Porta: 1883 (porta padrão para MQTT).
- Tópico: "queda/topic" (tópico usado para publicar mensagens de detecção de queda).
### Configuração do MQTT
A configuração do MQTT é feita na função setup() onde se define o servidor MQTT e o tópico de publicação. A função reconnect() assegura que a ESP32 esteja sempre conectada ao broker, tentando reconectar em caso de desconexão.

### Comunicação/Controle via Internet
Comunicação TCP/IP
A comunicação entre a ESP32 e o broker MQTT é realizada via TCP/IP, utilizando a biblioteca WiFi.h para gerenciar a conexão WiFi.

### Protocolo MQTT
O protocolo MQTT é utilizado para a troca de mensagens de forma eficiente e leve. A biblioteca PubSubClient.h é utilizada para implementar o cliente MQTT na ESP32.

### Integração com Internet
A ESP32 conecta-se ao broker MQTT através da internet, permitindo o envio de notificações de queda para qualquer dispositivo conectado à rede que esteja inscrito no tópico específico.

## Para acessar o projeto no wokwi:
### [Detector de queda](https://wokwi.com/projects/398051066286793729)
- Clique no botão de seta "Start Simulation"
- Após a compilação, a conexão WiFi e a conexão com o broker, clique no sensor MPU6050 para alterar a aceleração da queda. 

## Para realizar a conexão a com broker

### Importe o arquivo `flows.json` no node-red
