/*
 * Detector de Síntomas COVID
 * por: Alejandro Domínguez
 * Fecha: 30 de agosto de 2022
 * 
 * Basado en: Optical SP02 Detection (SPK Algorithm) using the MAX30105 Breakout
  By: Nathan Seidle @ SparkFun Electronics
  Date: October 19th, 2016
  https://github.com/sparkfun/MAX30105_Breakout
 * 
 * Este programa realizará la lectura de los sensores MAX30102 y MLX90614
 * para medir los principales sintomas de COVID, los cuales son ritmo cárdiaco, temperatura y oxigenación en la sangre. 
 * Se enviará un JSON por MQTT con todas las variables. Este programa es para el microcontrolador
 * ESP32 CAM
 * 
 * 
 * Componente     PinESP32CAM     Estados lógicos
 * ledStatus------GPIO 33---------On=>LOW, Off=>HIGH
 * ledFlash-------GPIO 4----------On=>HIGH, Off=>LOW
 * 
 * Conexión del Hardware 
 * MAX30102     ESP32CAM
 * 5v-----------5v
 * GND----------GND
 * SDA----------SDA
 * SCL----------SCL
 */

 //Bibliotecas
#include <WiFi.h>           //Biblioteca para el control de WiFi
#include <PubSubClient.h>   //Biblioteca para conexión MQTT
#include <Wire.h>           //Biblioteca para comunicación I2C
#include "MAX30105.h"       //Biblioteca del sensor 
#include "spo2_algorithm.h" //Biblioteca para interpretación de señales

//Datos del WiFi
const char* ssid = "CASADOMXC"; //Se debe poner el nombre de la red
const char* password = "kn72FCkggp"; // Se debe poner la contraseña de la red

//Datos del broker MQTT
const char* mqtt_server = "192.168.1.109"; // Si estas en una red local, coloca la IP asignada, en caso contrario, coloca la IP publica
IPAddress server(192,168,1,109);

//Objetos
WiFiClient espClient;           // Este objeto maneja los datos de conexion WiFi
PubSubClient client(espClient); // Este objeto maneja los datos de conexion al broker
MAX30105 particleSensor;        //


//Variables
int flashLed = 4;  // Para indicar el estatus de conexión el Led flash
int statusLed = 33; // Para mostrar mensajes recibidos
long timeNow, timeLast; // Variables de control de tiempo no bloqueante
int wait = 5000;  // Indica la espera cada 5 segundos para envío de mensajes MQTT


//Valores iniciales del programa
void setup() {
  // Iniciar la comunicación serial
  Serial.begin(115200);
  Serial.println ("Programa iniciado");
  
  pinMode (flashLed, OUTPUT);
  pinMode (statusLed, OUTPUT);
  digitalWrite (flashLed, LOW);
  digitalWrite (statusLed, HIGH);

  Serial.println();
  Serial.println();
  Serial.print("Conectar a ");
  Serial.print(ssid);

  WiFi.begin(ssid, password); // Esta función realiza la conexión a la red WiFi

  while  (WiFi.status() != WL_CONNECTED){ //Este bucle espera a que se realice la conexion
  digitalWrite (statusLed, HIGH);
  delay(500); //dado que es de suma importancia esperar a la conexión, debe usarse espera bloqueante
  digitalWrite (statusLed, LOW);
  Serial.print(".");  // Indicador de progreso
   delay (5);
  }

  // Cuando se haya logrado la conexión, el programa avanzará, por lo tanto, puede informarse lo siguiente
  Serial.println();
  Serial.println("WiFi conectado");
  Serial.println("Dirección IP");
  Serial.println(WiFi.localIP());

   // Si se logra la conexión, se va a encender el statusLed
   if (WiFi.status () > 0){
    digitalWrite (statusLed, LOW);
   }

   delay (1000); // Esta espera es solo una formalidad antes de iniciar la comunicación con el broker

   // Conexión con el broker MQTT
  client.setServer(server, 1883); // Conectarse a la IP del broker en el puerto indicado
  client.setCallback(callback); // Activar función de CallBack, permite recibir mensajes MQTT y ejecutar funciones a partir de ellos
  delay(1500);  // Esta espera es preventiva, espera a la conexión para no perder información

  timeLast = millis(); //Inicia el control de tiempo
  
 // Fin del void setup
}

// Cuerpo del programa, bucle principal
void loop() {
  //Verificar siempre que haya conexión al broker
  if (!client.connected()) {
    reconnect();  // En caso de que no haya conexión, ejecutar la función de reconexión, definida despues del void setup ()
  }// fin del if (!client.connected())
  client.loop(); // Esta función es muy importante, ejecuta de manera no bloqueante las funciones necesarias para la comunicación con el broker

 timeNow = millis(); // Control de tiempo para esperas no bloqueantes
  if (timeNow - timeLast > wait) { // Manda un mensaje por MQTT cada cinco segundos
    timeLast = timeNow; // Actualización de seguimiento de tiempo
    
   //Se construye el string correspondiente al JSON que contiene 3 variables
   String json = "{\"id\":\"Alex\",\"temp\":"+String(random(18, 23))+",\"hum\":"+String(random (38,52))+"}";
   Serial.println(json); // Se imprime en monitor solo para poder visualizar que el string esta correctamente creado
   int str_len = json.length() + 1;//Se calcula la longitud del string
   char char_array[str_len];//Se crea un arreglo de caracteres de dicha longitud
   json.toCharArray(char_array, str_len);//Se convierte el string a char array    
   client.publish("codigoIoT/detectorSintomas/flow", char_array); // Esta es la función que envía los datos por MQTT, especifica el tema y el valor
  }// fin del if (timeNow - timeLast > wait)
}

// Funciones de usuario

// Esta función permite tomar acciones en caso de que se reciba un mensaje correspondiente a un tema al cual se hará una suscripción
void callback(char* topic, byte* message, unsigned int length) {


  // Indicar por serial que llegó un mensaje
  Serial.print("Llegó un mensaje en el tema: ");
  Serial.print(topic);

  // Concatenar los mensajes recibidos para conformarlos como una varialbe String
  String messageTemp; // Se declara la variable en la cual se generará el mensaje completo  
  for (int i = 0; i < length; i++) {  // Se imprime y concatena el mensaje
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }

  // Se comprueba que el mensaje se haya concatenado correctamente
  Serial.println();
  Serial.print ("Mensaje concatenado en una sola variable: ");
  Serial.println (messageTemp);

  // En esta parte puedes agregar las funciones que requieras para actuar segun lo necesites al recibir un mensaje MQTT

  // Ejemplo, en caso de recibir el mensaje true - false, se cambiará el estado del led soldado en la placa.
  if (String(topic) == "codigoIoT/detectorSintomas/esp") {  // En caso de recibirse mensaje en el tema codigoIoT/detectorSintomas/esp
    if(messageTemp == "true"){
      Serial.println("Led encendido");
      digitalWrite(flashLed, HIGH);
    }// fin del if (String(topic) == "codigoIoT/detectorSintomas/esp")
    else if(messageTemp == "false"){
      Serial.println("Led apagado");
      digitalWrite(flashLed, LOW);
    }// fin del else if(messageTemp == "false")
  }// fin del if (String(topic) == "codigoIoT/detectorSintomas/esp")
}// fin del void callback

// Función para reconectarse
void reconnect() {
  // Bucle hasta lograr conexión
  while (!client.connected()) { // Pregunta si hay conexión
    Serial.print("Tratando de contectarse...");
    // Intentar reconexión
    if (client.connect("ESP32CAMClient")) { //Pregunta por el resultado del intento de conexión
      Serial.println("Conectado");
      client.subscribe("codigoIoT/detectorSintomas/esp"); // Esta función realiza la suscripción al tema
    }// fin del  if (client.connect("ESP32CAMClient"))
    else {  //en caso de que la conexión no se logre
      Serial.print("Conexion fallida, Error rc=");
      Serial.print(client.state()); // Muestra el codigo de error
      Serial.println(" Volviendo a intentar en 5 segundos");
      // Espera de 5 segundos bloqueante
      delay(5000);
      Serial.println (client.connected ()); // Muestra estatus de conexión
    }// fin del else
  }// fin del bucle while (!client.connected())
}// fin de void reconnect(
