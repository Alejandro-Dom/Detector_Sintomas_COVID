# Detector Sintomas COVID
Este repositorio contiene todo lo necesario para realizar und etector de síntomas COVID con NodeRed, ESP32CAM, el sensor MAX30102, el sensor MLx90614 y MySQL. 

## IMPORTANTE
***Este proyecto no realiza un diagnóstico médico. Se realiza un protodiagnóstico, el cual siguere al paciente ir a una revisión médica en caso de que sus signos vitales se encuentren fuera de los rangos normales***

## Software y Hardware necesarios
- Microcontrolador ESP32CAM
- Convertidor USB Serial FTDI TTL FT232RL
- Sensor MAX30100
- Sensor MLX 90614
- IDE de Arduino adaptada para ESP32CAM
- Node Red
- MySQL

# Instrucciones
La actividad se divide en varias partes.

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

5. Crear una sección en la cual el paciente pueda ingresar su nombre y correo, con el fin de usar esa información en la base de datos de MySQL.

6. Crear un botón que le ofrezca al paciente un protodiagnóstico:
    - Signos vitales normales
        - 35.5 < Temperatura < 36.5
	    - SPO2 > 90
	    - 60 < Ritmo cardíaco < 100
    - El nodo funcióón se configura con el siguiente código
    ~~~
    if((global.get("tir") > 35.5 && global.get("tir") < 36.5)&&(global.get("spo2") > 90) && (global.get("heartrate") > 60 && global.get("heartrate") < 100)){
        msg.payload = "Signos vitales normales";
        global.set("protodiagnostico", msg.payload);
        return msg; 
    }
    else {
        msg.payload = "Signos vitales anormales, asistir con un médico para confirmar diagnóstico ";
        global.set("protodiagnostico", msg.payload);
        return msg;
    }  
7. Usando el botón anterior, se mandarán todos los datos obtenidos a la base de datos con el siguiente comando:
    ~~~
    msg.topic = "INSERT INTO registro (`nombre`,`correo`,`temp`,`bpm`,`sp02`,`protodiagnostico`) VALUES ('" + global.get ("Paciente") + "','" + global.get ("Correo") + "'," + global.get ("tir") + "," + global.get ("heartrate") + "," + global.get ("spo2") + ",'" + global.get("protodiagnostico") + "');"; 
    return msg;
## Dashboard Final
![Datos mostrados en el dashboard final](https://github.com/Alejandro-Dom/Detector_Sintomas_COVID/blob/main/Imagenes/Dashboard_final) 

## Node Red
![Nodos usados en NodeRed](https://github.com/Alejandro-Dom/Detector_Sintomas_COVID/blob/main/Imagenes/Flow_MySQL) 
