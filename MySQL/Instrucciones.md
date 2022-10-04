# Instrucciones para configurar la base de datos

1. Intalar MySQL server
    ~~~
    sudo apt update

    sudo apt install mysql-server
    ~~~
2. Entrar a MySQL
    ~~~
    sudo mysql
3. Crear una nueva base de datos
    ~~~
    CREATE DATABASE detector_sintomas;

    show DATABASES; // para comprobar que se ha creado la base de datos exitosamente
    ~~~
4. Seleccionar la base de datos para poder trabajar con ella
    ~~~
    use detector_sintomas;

    show tables; 
    ~~~
5. Crear una tabla llamada Registro, que contenga todos los campos necesarios 
-		- ID
		- Fecha de prueba
		- Nombre
		- Correo
		- Temperatura
		- BPM
		- SP02
		- Protodiágnostico 
    ~~~
    CREATE TABLE [nombre] (id INT(6) UNSIGNED AUTO_INCREMENT PRIMARY KEY, fecha TIMESTAMP DEFAULT CURRENT_TIMESTAMP, nombre CHAR (248) NOT NULL, correo CHAR (248) NOT NULL, temp FLOAT(4,2) NOT NULL, bpm INT(1) UNSIGNED NOT NULL, sp02 INT(1) UNSIGNED NOT NULL, protodiagnostico CHAR (248) NOT NULL); 

    show tables;

    describe [nombre]; 
    ~~~
6.  Instalar el paquete de nodos de MySQL en Node RED
    - Buscar node-red-node-mysql
7. Configurar el nodo mysql con el siguiente código
    ~~~
    msg.topic = "INSERT INTO registro (`nombre`,`correo`,`temp`,`bpm`,`sp02`,`protodiagnostico`) VALUES ('" + global.get ("Paciente") + "','" + global.get ("Correo") + "'," + global.get ("tir") + "," + global.get ("heartrate") + "," + global.get ("spo2") + ",'" + global.get("protodiagnostico") + "');";)
    return msg;
8. Crear un nuevo usuario en MySQL y otorgar todos los privilegios
    ~~~
    CREATE USER 'alex'@'localhost' IDENTIFIED BY '';

	GRANT ALL PRIVILEGES ON *.* TO 'usuario'@'localhost';
    ~~~



## Notas
- show DATABASES; // Muesta la lista de bases de datos
- show tables; // Muestra la lista de tablas en la base de datos seleccionada
- Los datos se guardan en tablas
- describe [nombre]; // Muestra los detalles de una tabla de la base de datos seleccionada
- Para salir usar el comando exit;
- SELECT * FROM registro; // Para mostrar todos los datos de la base de datos


