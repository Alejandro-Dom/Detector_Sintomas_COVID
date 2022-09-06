# Detector Sintomas COVID
Este repositorio contiene todo lo necesario para realizar und etector de síntomas COVID con NodeRed, ESP32CAM, el sensor MAX30102, el sensor MLx90614 y MySQL. 

Esta actividad se divide en varias partes.

1. Instalar y crear la base de datos llamada registro. (Para ver las instrucciones detalladas ver el documento encontrado en la carpeta MySQL)

2. Crear un código en la IDE de Arduino que cumpla con las siguientes fuciones:
    - Conectar a una red WiFi
    - Enviar un mensaje en formato JSON por MQTT
        - TEMA MQTT pub: codigoIoT/detectorSintomas/flow
        - TEMA MQTT sub: codigoIoT/detectorSintomas/esp
    - Leer el ritmo cardiaco y la oxigenación de la sangre usando el sensor MAX30102. (Para ver el código revisar la carpeta DetectorSintomasCOVID)

## Evidencia de lectura de datos obtenidos por el sensor MAX30102 y enviados por MQTT
![Datos enviados por MQTT](https://github.com/Alejandro-Dom/Detector_Sintomas_COVID/blob/main/Imagenes/Valores_MQTT_max30102) 

3. Una vez completado el punto anterior, se debe modificar el código para obtener la información de temperatura del sensor MLX90614 y enviar la información obtenida en formato JSON junto a los valores anteriormente obtenidos.
4. Cuando ya se puedan enviar correctamente los valores de ritmo cardíaco, oxigenación y temperatura obtenida por los sensores MAX30100 y MLX90614 y generar un dashboar utilizando NodeRed para que visualizar los datos obtenidos

## Evidencia de datos recibidos por MQTT y mostrados en un dashboard
![Datos mostrados en un dashboard](https://github.com/Alejandro-Dom/Detector_Sintomas_COVID/blob/main/Imagenes/Dashboard_V1) 
