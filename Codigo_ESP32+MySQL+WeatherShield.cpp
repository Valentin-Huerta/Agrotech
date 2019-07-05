#include <MySQL_Cursor.h>
#include <MySQL_Connection.h>
#include <MySQL_Encrypt_Sha1.h>
#include <MySQL_Packet.h>
#include <WiFi.h>
#include <SparkFunCCS811.h>
#include "SparkFunBME280.h"
#include "Wire.h"
#include <Sparkfun_APDS9301_Library.h>


const char* ssid = "TECHMAKE";
const char* password =  "034E4D89";


byte mac_addr[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress server_addr(216,172,166,15);  // IP of the MySQL *server* here
char user[] = "soporte_snsrAcnt";              // MySQL user login username
char pass[] = "p5A75067w#qkWve";        // MySQL user login password
// Sample query
char INSERT_SQL[] = "INSERT INTO soporte_bdSistema.empleado (idCompania, validado, activo, adminParcial, nombre, apellido, correo) VALUES (1,0,0,0,'Donquijote','Doflamingo','Donquijote@gmail.com')";
WiFiClient client;
MySQL_Connection conn((Client *)&client);

BME280 bme;
CCS811 ccs(0x5B);
APDS9301 apds;

// Pin assignment definitions
#define AIR_RST      4
#define AIR_WAKE     15
#define DONE_LED     5


void setup() {

/*/
  Codigo MySQL------empieza
/*/
  
Serial.begin(115200);

WiFi.begin(ssid, password);

while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.println("Connecting to WiFi..");
}
Serial.println("Connected to the WiFi network");
IPAddress ip = WiFi.localIP();
Serial.print("My IP address is: ");
Serial.println(ip);
if (conn.connect(server_addr, 3306, user, pass)) {
  delay(1000);
  Serial.println("SE CONECTO");
}
else
  Serial.println("Connection failed.");

/*/
  Codigo MySQL----termina
/*/


/*/
Codigo Weather Shield-----empieza
/*/
    delay(5);    // The CCS811 wants a brief delay after startup.
    Serial.begin(115200);
    Wire.begin();

    pinMode(DONE_LED, OUTPUT);
    digitalWrite(DONE_LED, LOW);
  
   
    // BME280 sensor setup - these are fairly conservative settings, suitable for
    //  most applications. For more information regarding the settings available
    //  for the BME280, see the example sketches in the BME280 library.
    bme.settings.commInterface = I2C_MODE;
    bme.settings.I2CAddress = 0x77;
    bme.settings.runMode = 3;
    bme.settings.tStandby = 0;
    bme.settings.filter = 0;
    bme.settings.tempOverSample = 1;
    bme.settings.pressOverSample = 1;
    bme.settings.humidOverSample = 1;
    bme.begin();
  
    // CCS811 sensor setup.
    pinMode(AIR_WAKE, OUTPUT);
    digitalWrite(AIR_WAKE, LOW);
    pinMode(AIR_RST, OUTPUT);
    digitalWrite(AIR_RST, LOW);
    delay(10);
    digitalWrite(AIR_RST, HIGH);
    delay(100);
    ccs.begin();
  
    // APDS9301 sensor setup. Leave the default settings in place.
    apds.begin(0x39);
/*/
Codigo Weather Shield-----termina
/*/

}

void loop() {

/*/
  Codigo MySQL----empieza
/*/

delay(2000);

Serial.println("Recording data.");

// Initiate the query class instance
MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
// Execute the query
cur_mem->execute(INSERT_SQL);
Serial.println("SE EJECUTO");
// Note: since there are no results, we do not need to read any data
// Deleting the cursor also frees up memory used
delete cur_mem;

/*/
  Codigo MySQL----termina
/*/

/*/
Codigo Weather Shield-----empieza
/*/

    delay(1000);
    // Update temperature. This also updates compensation values used to
    //  calculate other parameters.
    Serial.print("Temperature: ");
    Serial.print(bme.readTempF(), 2);
    Serial.println(" degrees F");
  
    // Display relative humidity.
    Serial.print("%RH: ");
    Serial.print(bme.readFloatHumidity(), 2);
    Serial.println(" %");

    // Display pressure.
    Serial.print("Pres: ");
    Serial.print(bme.readFloatPressure() * 0.0002953);
    Serial.println(" in");


    // Trigger the CCS811's internal update procedure, then
    //  dump the values to the serial port.
    ccs.readAlgorithmResults();

    Serial.print("CO2: ");
    Serial.println(ccs.getCO2());

    Serial.print("tVOC: ");
    Serial.println(ccs.getTVOC());

    Serial.print("Luminous flux: ");
    Serial.println(apds.readLuxLevel(),6);

/*/
Codigo Weather Shield-----termina
/*/
}