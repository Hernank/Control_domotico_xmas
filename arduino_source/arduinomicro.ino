/*
  www.diymakers.es
  by A.García & M.García
  Crear servidor Web con Arduino
  Tutorial en: http://diymakers.es/crear-servidor-web-con-arduino/
*/

#include <SPI.h>  //Importamos librería comunicación SPI
#include <Ethernet.h>  //Importamos librería Ethernet
#include <Servo.h> 
#include <b64.h>
#include <HttpClient.h>
#include <Password.h>
#include <Keypad.h>

// COnfiguracion para Puerta y Teclado 3x3
Password password = Password( "1234" );
Servo myservo;

const byte Filas = 3;
const byte Cols = 3;

byte Pins_Filas[] = {3,5,6};
byte Pins_Cols[] = {7,8,9};

char Teclas [ Filas ][ Cols ] =
    {
        {'1','4','7'},
        {'2','5','8'},
        {'3','6','9'},
     };

Keypad Teclado1 = Keypad(makeKeymap(Teclas),Pins_Filas, Pins_Cols,Filas, Cols);
///////////////////


/*
  Config server local
*/

// Name of the server we want to connect to
const char kHostname[] = "192.168.1.20";

// Path to download (this is the bit after the hostname in the URL
// that you want to download
const char kPath[] = "/jasdso";
// Number of milliseconds to wait without receiving any data before we give up
const int kNetworkTimeout = 30 * 1000;
// Number of milliseconds to wait if no data is available before trying again
const int kNetworkDelay = 1000;
//char serverdns[] = "www.google.com";    // name address for Google (using DNS)

/////////////


byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };//Ponemos la dirección MAC de la Ethernet Shield que está con una etiqueta debajo la placa
IPAddress ip(192, 168, 0, 20); //Asingamos la IP al Arduino
EthernetServer server(80); //Creamos un servidor Web con el puerto 80 que es el puerto HTTP por defecto

int Alarma          = A0; //Pin del led 2
int Aire1     = A1; //Pin del led 3
int Aire2     = A3; //Pin del led 4
int Radio1          = A4; //Pin del led 5

int Foco            = 0; //Pin del led 7
int Arbol           = A5; //Pin del led 8
int sensoralarma  = 1; //Pin del led 12
int sensortemperatura =  A2;

int keypin1=3;
int keypin2=5;
int keypin3=6;
int keypin4=7;
int keypin5=8;
int keypin6=9;

int servoaux=0; 
int leds[9] = {Alarma, Aire1, Aire2, Radio1, Foco, Arbol,servoaux};

void setup()
{
  //Serial.begin(9600);

  // Inicializamos la comunicación Ethernet y el servidor
  Ethernet.begin(mac, ip);
  server.begin();
  //Serial.print("server esta en | ");
  //Serial.println(Ethernet.localIP());
  //Establcer el modeo de trabajo de los pines
  pinMode(Alarma, OUTPUT);
  pinMode(Aire1, OUTPUT);
  pinMode(Aire2, OUTPUT);
  pinMode(Radio1, OUTPUT);
  
  pinMode(Foco, OUTPUT);
  pinMode(Arbol, OUTPUT);

  pinMode(sensortemperatura, INPUT);
  pinMode(sensoralarma, INPUT);
  //pinMode(keypin1, INPUT);
  //pinMode(keypin2, INPUT);
  //pinMode(keypin3, INPUT);
  //pinMode(keypin4, INPUT);
  //pinMode(keypin5, INPUT);
  //pinMode(keypin6, INPUT);
  myservo.attach(2);
  Teclado1.addEventListener(keypadEvent);
  digitalWrite(Arbol,HIGH);
  //Serial.print("iniciando");

  /////////


}
void loop()
{
  EthernetClient client = server.available(); //Creamos un cliente Web
  //Cuando detecte un cliente a través de una petición HTTP

  if (client) {
    superlogica(client);
    
  }
  Teclado1.getKey();
  if(digitalRead(sensoralarma) == HIGH)
  {
    digitalWrite(Alarma, HIGH);
  }
  else
  {
    digitalWrite(Alarma, LOW);
  }

  if(getTemperatura()> 16.0)
  {
    digitalWrite(Aire1,HIGH);
  }else if (getTemperatura()> 20.0)
  {
    digitalWrite(Aire2,HIGH);
  }else
  {
    digitalWrite(Aire1,LOW);
    digitalWrite(Aire1,LOW);
  }
  
}

void keypadEvent(KeypadEvent eKey)
{
   switch(Teclado1.getState())
   {
    case PRESSED:
     switch(eKey)
     {
      case '9': guessPassword();break;
      default: password.append(eKey);
     }
   }
}

void guessPassword()
{
  if(password.evaluate())
  {
    for(int i=74;i>12;i--)
    {
      myservo.write(i);
      delay(12);
    }
    delay(1000);
    for(int i=12;i<80;i++)
    {
      myservo.write(i);
      delay(12);
    }
    password.reset();
    delay(600);
  }
  else
  {
    password.reset();
    delay(600);
  }
}

void superlogica(EthernetClient client){
    //Serial.println("Nuevo cliente");    
    boolean currentLineIsBlank = true; //Una petición HTTP acaba con una línea en blanco    
    String cadena = getcadena(client); //seteo en cadena el texto de request
    //Serial.print(cadena);    
    String valorget = getget(cadena);
    String valorset = getset(cadena);   
    //Serial.println("valores get y set");
    //Serial.print(valorget + valorset);
   
    if (valorget != "") {
      String getdata = getData();
      //Me devuelve 000101010-260-1 (valores salidas 9,temperatura,sensor de alarma)
      client.print(getdata);
      
    }
    if (valorset != "") {
      String nuevovalorset = setData(valorset);
      
      client.print(nuevovalorset);
    }

    //Dar tiempo al navegador para recibir los datos
    delay(1);
    client.stop();// Cierra la conexión
  
  
  }


bool getbolean(String valor) {
  if (valor == "True") {
    return true;
  } else {
    if (valor == "False") {
      return false;
    }

  }

}
int getTemperatura(){
   int valor=0;  
   valor = analogRead(sensortemperatura);
    float var=(5.0*valor*100.0)/1024;
   return var;
  }
  
 int getSensoralarma(){
   int valor=0;  
   valor = digitalRead(sensoralarma);
   
   return valor;
   
  
  }
String getData() {
  int str_len = (sizeof(leds) / sizeof(int));
  //Serial.println(str_len);

  String data = "";
  for (int i = 0; i < (str_len-1); i = i + 1) {
    data += digitalRead(leds[i]);
    //Serial.print(".");
    //Serial.println(leds[i]);

  }
  data+=servoaux;
  //Serial.println("data:");
  //Serial.print(data);

  int valsensor = getSensoralarma();//digitalRead(inPin);
  int valtemperatura = getTemperatura();//analogRead(analogPin);
  return data + "-" + valtemperatura + "-" + valsensor;

}
String  setData(String estadocadena) {
  //Serial.print("Seteando");

  //Serial.println(estadocadena);


  // Length (with one extra character for the null terminator)

  int str_len = (sizeof(leds) / sizeof(int)) + 1;
  //Serial.println(str_len);
  // Prepare the character array (the buffer)
  char char_array[str_len];

  String nombre = split(estadocadena, '-', 0);
  String nvalor = split(estadocadena, '-', 1);
  String valor = split(nvalor, ' ', 0);
  //Serial.println("*" + nombre);
  //Serial.println("*" + valor);
  if (nombre == "Alarma") {
    //Serial.println("Setear ventilador");
    digitalWrite(Alarma, getbolean(valor));
  }

  if (nombre == "Puerta") {
    //Serial.println("Setear Servos");
    if(getbolean(valor)){
      servoaux=1;
      for(int i=74;i>12;i--)
      {
      myservo.write(i);
      delay(12);
      }      
     }else{
      servoaux=0;
      for(int i=12;i<80;i++)
      {
      myservo.write(i);
      delay(12);
      }      
        
     }

   
  }
 
  if (nombre == "Alarma") {
    //Serial.println("Setear Alarma");
    digitalWrite(Alarma, getbolean(valor));
  }
  if (nombre == "Radio 1") {
    //Serial.println("Setear Radio 1");
    digitalWrite(Radio1, getbolean(valor));
  }
  if (nombre == "Aire 2") {
    //Serial.println("Ventilador arbol");
    
    digitalWrite(Aire2, getbolean(valor));
  }
  if (nombre == "Arbol") {
    //Serial.println("Setear Arbol");
    digitalWrite(Arbol, getbolean(valor));
  }
  if (nombre == "Foco") {
    //Serial.println("Setear Foco");
    digitalWrite(Foco, getbolean(valor));
  }
  if (nombre == "Aire 1") {
    //Serial.println("Setear Vetilador 1");
    digitalWrite(Aire1, getbolean(valor));
  }

  //Serial.println("Seteado");

  // return estadocadena;
  return "hola herna";
}
String split(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }

  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

String obtenerset(String cadena) {
  String valorset = "jaja";
  return valorset;
}

String getget(String cadenatotal) {


  int indicebusqueda = search("getdata", cadenatotal);

  if (indicebusqueda + 1)
  {
    return ":*";
  }
  else
  {
    return "" ;
  }
}
String getset(String cadenatotal) {


  //Serial.println("cadenatotalset" + cadenatotal);
  int indicebusqueda = search("setdata", cadenatotal);
  //Serial.println("mi indice: " + indicebusqueda);
  if (indicebusqueda + 1)
  {
    String hola = cadenatotal;
    String nuevo = split(hola, '/', 2);
    String nuevo2 = split(hola, '/', 3);
    String valor = split(nuevo2, ' ', 0);
    //Serial.println(valor);
    return nuevo + "-" + valor;
  }
  else
  {
    return "";
  }

}




int search(String key, String cadena) {
  String mikey = key;
  int posicion = cadena.indexOf(mikey); //Guardamos la posición de la instancia "LED=" a la variable 'posicion'
  return posicion;
}
//String cadena = ""; //Creamos una cadena de caracteres vacía

String getcadena(EthernetClient client) {
  boolean currentLineIsBlank = true;
  String cadena = ""; //Creamos una cadena de caracteres vacía

  while (client.connected()) {
    if (client.available()) {
      char c = client.read();//Leemos la petición HTTP carácter por carácter

      //Serial.write(c);//Visualizamos la petición HTTP por el Monitor //Serial
      cadena.concat(c);//Unimos el String 'cadena' con la petición HTTP (c). De esta manera convertimos la petición HTTP a un String


      //Cuando reciba una línea en blanco, quiere decir que la petición HTTP ha acabado y el servidor Web está listo para enviar una respuesta
      if (c == '\n' && currentLineIsBlank) {

        // Enviamos al cliente una respuesta HTTP
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/html");
        client.println();


        break;
      }
    }

  }
  return cadena;

}



void requesttolocal() {
  int err = 0;

  EthernetClient c;
  HttpClient http(c);

  err = http.get(kHostname,  8000, kPath, "MyUserAgent/1.0" );
  if (err == 0)
  {
    //Serial.println("startedRequest ok");

    err = http.responseStatusCode();
    if (err >= 0)
    {
      //Serial.print("Got status code: ");
      //Serial.println(err);

      // Usually you'd check that the response code is 200 or a
      // similar "success" code (200-299) before carrying on,
      // but we'll print out whatever response we get

      err = http.skipResponseHeaders();
      if (err >= 0)
      {
        int bodyLen = http.contentLength();
        //Serial.print("Content length is: ");
        //Serial.println(bodyLen);
        //Serial.println();
        //Serial.println("Body returned follows:");

        // Now we've got to the body, so we can print it out
        unsigned long timeoutStart = millis();
        char c;
        // Whilst we haven't timed out & haven't reached the end of the body
        while ( (http.connected() || http.available()) &&
                ((millis() - timeoutStart) < kNetworkTimeout) )
        {
          if (http.available())
          {
            c = http.read();
            // Print out this character
            //Serial.print(c);

            bodyLen--;
            // We read something, reset the timeout counter
            timeoutStart = millis();
          }
          else
          {
            // We haven't got any data, so let's pause to allow some to
            // arrive
            delay(kNetworkDelay);
          }
        }
      }
      else
      {
        //Serial.print("Failed to skip response headers: ");
        //Serial.println(err);
      }
    }
    else
    {
      //Serial.print("Getting response failed: ");
      //Serial.println(err);
    }
  }
  else
  {
    //Serial.print("Connect failed: ");
    //Serial.println(err);
  }
  http.stop();

  // And just stop, now that we've tried a download
  
  //Serial.print("ajajjaja");

}


