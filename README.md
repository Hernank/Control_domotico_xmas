# Arduino_Uno_XMAS
Proyecto orientado al control de casas inteligentes
  


# Como funciona

Tenemos el control de una casa el cual entre varias de las cosas a controlar son
  * **Acceso al interior por medio de una contraseña** (por medio de un teclado se captara una contraseña previamente establecida para que habilite el servomotor y pueda abrir la puerta)
  * **Ambiente climatizado por medio de un sensor de temperatura** (Teniendo como entrada un sensor de temperatura permitira procesar este dato para habilitar o inhabilitar 2 ventiladores a diferente temperatura respecto al otro, es decir el primer ventilador se enciende a una temperatura x , y el segundo ventilador se habilita a una temperaura mayor a x)
  * **Encendido de un arbol de navidad** (Se puede encender un arbol de navidad por medio de una aplicacion  )
  * **Encendido de la alarma al detectar intrusos** (Provisto de 2 sensores de contacto en 2 ventanas[en complementacion de una compuerta or 7432 solo habra  1 entrada al arduino] y otra sensor de contacto en el cerco electrico le permitiran al arduino encender la alarma en caso de que se quieran abrir las ventanas o se quiera burlar el cerco electrico )
  * 

# Proyecto arduino

[Aqui](https://github.com/Hernank/Control_domotico_xmas/tree/master/arduino_source/arduinomicro) se podra encontrar los recursos correspondientes para el grabado en el arduino consta de varias librerias, que en el caso de no tenerlas aqui les dejo los links 
  * [Password](http://playground.arduino.cc/uploads/Code/Password.zip).
  * [Keypad](http://playground.arduino.cc/uploads/Code/keypad.zip).
  * [HttpClient](https://github.com/amcewen/HttpClient/archive/2.2.0.zip). 
Esto hara que funcione como webserver en la ip 192.168.0.20 puerto 80

Proyecto django
=======

El proyecto django es una [aplicacion](https://github.com/Hernank/Control_domotico_xmas/tree/master/serverlocal "") al estilo web appp diseñado con [MATERIAL DESIGN LITE](http://www.getmdl.io/) que le da la posibilidad de tener una apariencia de web app totalmente adaptable a cualquier tipo de dispositivo. En los recursos se la identifica como [server local](https://github.com/Hernank/Control_domotico_xmas/tree/master/serverlocal )


Aplicacion
=======
La aplicacion es completamente repsonsive acontinuacion unos screenshots 


![ScreenShot](http://1.bp.blogspot.com/-R9JDd3MUZwE/VoXGaXRcuqI/AAAAAAAAImE/gUch2sJAr8s/s1600/micro2.png "Optional Title")
---------
![ScreenShot](http://2.bp.blogspot.com/-GQunUyChYNU/VoXGp9r49JI/AAAAAAAAImM/9WVc6VBhicI/s1600/micro3.png "Optional Title")
----------
![ScreenShot](http://4.bp.blogspot.com/-NsYa55wytlY/VoXGYJ5SEDI/AAAAAAAAIl8/yXJ5v2Q-8Dk/s1600/micro.png "Optional Title")

Ejecucion
=======
Se asume que se esta trabajando en una red local en la cual esta disponible la ip `192.168.0.20` que sera utilizada por el arduino
  * Para poner en funcinamiento se debe quemar el archivo en el arduino
  * Verificamos en el navegador poniendo la ip `192.168.0.20/getdata/x` y nos debe salir la lectura de pines del arduino
  * Ejecutar la app django con el comando `python manage.py runserver 0.0.0.0:4000`
  * Verificamos en el navegador colocando `127.0.0.1:4000 `
  * Listo teniendo la interfaz de inicio le damos a ingresar y nos saldra el panel de administracion del arduino desde donde se podra leer la temperatura tambien se podra encender la alarma ,los ventiladores foco, etc
