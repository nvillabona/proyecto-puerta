// Se incluyen las librerias requeridas
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Servo.h>
#include <SPI.h>
#include <Ethernet.h>
// Se crean las instacias
LiquidCrystal_I2C lcd(0x3F, 16, 2);//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
MFRC522 mfrc522(53, 9); // MFRC522 mfrc522(SS_PIN, RST_PIN)//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = {192, 168, 1, 68 }; // Ingresa IP Shield ethernet arduino (Se obtiene de Arduino-Archivos-Ejemplos-Ethernet-DhcpAdressPrinter)
byte serv[] = {192, 168, 1, 69} ; //Ingresa IPV4 donde esta ubicada la base de datos (Se obtiene en Ejecutar-CMD-Ipconfig)
EthernetClient cliente;
Servo sg90;
String dbReport = ""; // Creo variable para reporte a la BD
// Se inicializan los pins para los leds, servomotor y buzzer
// Led azul es conectado a 5V
constexpr uint8_t greenLed = 7;
constexpr uint8_t redLed = 6;
constexpr uint8_t servoPin = 8;
constexpr uint8_t buzzerPin = 5;
char initial_password[4] = {'1', '2', '3', '4'};  // Variable de contraseña //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
String tagUID = "8D 25 43 42";  // Cadena para almacenar la UID de la tarjeta. Cambiar por la tarjeta que se posee
char password[4];   // Variable para almacenar la contraseña
boolean RFIDMode = true; // Boolean para cambiar modos de lectura
char key_pressed = 0; // Variable para guardar la clave a digitar
uint8_t i = 0;  // Variable usada para contador.
// Definicion de cuantas columnas y filas tiene el teclado
const byte rows = 4;
const byte columns = 4;
// Mapa de pines del teclado
char hexaKeys[rows][columns] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
// Inicializacion de pines para el teclado
// Con el teclado hacia al frente los pines se cuentan de izquierda a derecha, empezando desde el pin 1 hasta el pin 8
// Los primeros 4 pines simbolizan las filas, los ultimos 4 pines simbolizan las columnas.
byte row_pins[rows] = {A0, A1, A2, A3}; //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
byte column_pins[columns] = {A4, A5, A8, A9};//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// Se crea la instancia para el teclado
Keypad keypad_key = Keypad( makeKeymap(hexaKeys), row_pins, column_pins, rows, columns);
void setup() {
  Serial.begin(9600); //Iniciamos la comunicación  serial
  // Configuracion pin Arduino
  pinMode(buzzerPin, OUTPUT);
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  sg90.attach(servoPin);  //Se declara el pin 8 para el servomotor
  sg90.write(0); // Se inicia el servomotor en 90 grados
  lcd.begin();   // Se inicia la pantalla LCD
  lcd.backlight();
  SPI.begin();      // Se inicia el SPI bus (Comunicacion)
  mfrc522.PCD_Init();   // Se inicia el lector RFID MFRC522
  Ethernet.begin(mac, ip);
  lcd.clear(); // Se limpia la pantalla LCD
}
void loop() {
  // El sistema primero buscará el modo
  if (RFIDMode == true) {
    lcd.setCursor(0, 0);
    lcd.print(" Puerta Cerrada");
    lcd.setCursor(0, 1);
    lcd.print("Acerque tarjeta");
    // Busca nuevas tarjetas
    if ( ! mfrc522.PICC_IsNewCardPresent()) {
      return;
    }
    // Selecciona una de las tarjetas
    if ( ! mfrc522.PICC_ReadCardSerial()) {
      return;
    }
    //Lectura de la tarjeta
    String tag = "";
    for (byte j = 0; j < mfrc522.uid.size; j++)
    {
      tag.concat(String(mfrc522.uid.uidByte[j] < 0x10 ? " 0" : " "));
      tag.concat(String(mfrc522.uid.uidByte[j], HEX));
    }
    tag.toUpperCase();
////////////////////////////////////////////////

    dbReport = tag.substring(1);
    Serial.println(dbReport);
    dbReport.replace(" ", "");
    Serial.println(dbReport);

////////////////////////////////////////////////    
    // Chequeo de la tarjeta
    if (tag.substring(1) == tagUID)
    {
      // Si el UID de la tarjeta coincide....
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(" Emparejamiento");
      lcd.setCursor(0, 1);
      lcd.print("    Exitoso");
      digitalWrite(greenLed, HIGH);
      delay(3000);
      digitalWrite(greenLed, LOW);
      lcd.clear();
      lcd.print("Ingresa la clave:");
      lcd.setCursor(0, 1);
      RFIDMode = false; // Variable RFIDmode cambia a FALSE
    }
    else
    {
      // Si el UID de la tarjeta no coincide....
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(" No coincidente");
      lcd.setCursor(0, 1);
      lcd.print("Acceso denegado");
      digitalWrite(buzzerPin, HIGH);
      digitalWrite(redLed, HIGH);
      delay(3000);
      digitalWrite(buzzerPin, LOW);
      digitalWrite(redLed, LOW);
      lcd.clear();
    }
  }
  // Si la variable RFIDmode es falsa, mirará lo que se tecleará
  if (RFIDMode == false) {
    key_pressed = keypad_key.getKey(); // Almacenamiento de tecleo
    if (key_pressed)
    {
      password[i++] = key_pressed; // Almacenando en variable de tecleo
      Serial.print(key_pressed);
      lcd.print("*");
    }
    if (i == 4) // Si los 4 caracteres son tecleados...
    {
      delay(200);
      if (!(strncmp(password, initial_password, 4))) // Si la contraseña original coincide...
      {
////////////////////////////////////////////////

        if (cliente.connect(serv, 50)) { // El '50' cambia dependiendo del puerto que se use
                    
                      Serial.println("¡Conectado a la base de datos!");
                      cliente.print("GET /ethernet/data.php?"); //Conexion y envio de valores al servidor

                      cliente.print("temperature=");
                      cliente.print("Tarjeta-acercada");
                      cliente.print("&humidity=");
                      cliente.print(dbReport);
                      cliente.print("&heat_index=");
                      cliente.println(3);
                      //Imprimiendo en consola lo enviado a la base de datos
                      Serial.print("Estado");
                      Serial.println("Tarjeta-acercada");
                      Serial.print("Numero_tarjeta");
                      Serial.println(dbReport);
                      Serial.print("&heat_index=");
                      Serial.println(3);
                      }

                      else
                      {// Si no se logra una conexion al servidor se imprime:
                        Serial.println("Conexion fallida :(");
                      }
                      cliente.stop(); //Cierra la conexion con el servidor
           
                 
                 

////////////////////////////////////////////////    
        lcd.clear();
        lcd.print(" Pase aceptado");
        sg90.write(90); // Puerta abierta
        digitalWrite(greenLed, HIGH);
        delay(3000); //                                                             TIEMPO DE APERTURA DE LA PUERTA 
        digitalWrite(greenLed, LOW);
        sg90.write(0); // Puerta cerrada
        lcd.clear();
        i = 0;
        RFIDMode = true; //  Variable RFIDmode cambia a TRUE
      }
      else    // Si las contraseñas no coinciden...
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("  Contrasena");
        lcd.setCursor(0, 1);
        lcd.print("  Incorrecta");
        digitalWrite(buzzerPin, HIGH);
        digitalWrite(redLed, HIGH);
        delay(3000);
        digitalWrite(buzzerPin, LOW);
        digitalWrite(redLed, LOW);
        lcd.clear();
        i = 0;
        RFIDMode = true;  // Variable RFIDmode cambia a TRUE
      }
    }
  }
}
