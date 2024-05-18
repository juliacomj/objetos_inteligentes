#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <WiFi.h>
#include <PubSubClient.h>

Adafruit_MPU6050 mpu;
WiFiClient espClient;
PubSubClient client(espClient);

// Define variáveis 
#define LED 5

// Definições da rede Wi-Fi
const char* ssid = "Wokwi-GUEST"; // Nome da rede Wi-Fi (SSID)
const char* password = "";        // Senha da rede Wi-Fi

// Definições do broker MQTT
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
const char* mqtt_user = "wokwi"; // Se necessário
const char* mqtt_password = "wokwi"; // Se necessário

void setup_wifi() {
  delay(10);
  // Conecta-se à rede Wi-Fi
  Serial.print("Conectando-se a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("Endereço IP: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Mensagem recebida no tópico: ");
  Serial.print(topic);
  Serial.print(". Mensagem: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();
}

void reconnect() {
  // Loop até conseguir conectar-se ao broker
  while (!client.connected()) {
    Serial.print("Tentando conectar ao MQTT...");
    // Tenta conectar
    if (client.connect("ESP32Client", mqtt_user, mqtt_password)) {
      Serial.println("Conectado");
    } else {
      Serial.print("Falha na conexão, rc=");
      Serial.print(client.state());
      Serial.println(" tentando novamente em 5 segundos");
      // Aguarda 5 segundos antes de tentar novamente
      delay(5000);
    }
  }
}

void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(115200);

  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);

  while (!mpu.begin()) {
    Serial.println("MPU6050 não conectado!");
    delay(1000);
  }
  Serial.println("MPU6050 pronto!");
}

boolean fastAccelaration = false; 

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  long startCommand = millis();

  // Leitura das acelerações nos eixos x, y e z
  float ax = a.acceleration.x;
  float ay = a.acceleration.y;
  float az = a.acceleration.z;
  if (!client.connected()) {
    reconnect();
  }

  client.loop();

  // Verificação de queda
  float totalAcceleration = sqrt(ax * ax + ay * ay + az * az);
 
  Serial.println("---Aceleraçao---");
  Serial.print(totalAcceleration);

  if(fastAccelaration == true && totalAcceleration < 1.0){
     Serial.println("Queda detectada!");
     client.publish("queda/topic", "Queda detectada!!");
     Serial.println("--TEMPO DE RESPOSTA QUEDA DETECTADA--");
     Serial.print(millis() - startCommand);
     digitalWrite(LED, HIGH);    
  } else{
    Serial.println("--TEMPO DE RESPOSTA--");
    Serial.print(millis() - startCommand);
    fastAccelaration = false;
    digitalWrite(LED, LOW);
  }

  if (totalAcceleration > 12.0) {
      fastAccelaration = true;
  }


  delay(2000); // Aguarde um curto período antes de fazer a próxima leitura

}


