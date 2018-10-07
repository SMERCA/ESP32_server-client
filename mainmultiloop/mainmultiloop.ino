/*Simple example of multi loop
* By Evandro Luis Copercini
* Based on pcbreflux video
* Public domain license 2017
*/


#include <WiFi.h>

//WIFI DEFINITIONS
int status = WL_IDLE_STATUS;
const char* ssid     = "Aquaris X5 Plus";
const char* password = "3cdb401cb5d6";

char google[] = "www.google.com";
unsigned long lastConnectionTime = 0;               // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 5L * 1000L;   // delay between updates, in milliseconds

// Initialize the Wifi client library
WiFiClient client;
// Initialize the Wifi server library
WiFiServer server(80);

//THREADS DEFINITIONS
TaskHandle_t Task1;
TaskHandle_t Task2;

void codeForTask1( void * parameter){

  Serial.begin(115200);
  while(true){
      Serial.println("Print from core 0, TASK1");
      delay(2000);
  }
}

void serverTask2( void * parameter){

  //Setup
  Serial.begin(115200);
  server.begin();
  Serial.print("server is at ");
  Serial.println(WiFi.localIP());
  //Loop
  while(true){
    // listen for incoming clients
    WiFiClient client = server.available();
    if (client) {
        Serial.println("new client");
    }
}

///SETUP

void setup(){
  Serial.begin(115200);

  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, password);
    delay(2000);
  }
   Serial.println("Connected");

  xTaskCreatePinnedToCore(
    codeForTask1,             //Task function
    "Task_1",                 //name of task
    1000,                     //Stack size of the task
    NULL,                     //parameter of the task
    1,                        //priority of the task
    &Task1,                   //Task handle to keep track of created task
    0);                       //core

  xTaskCreatePinnedToCore(
    serverTask2,             //Task function
    "Task_2",                 //name of task
    1000,                     //Stack size of the task
    NULL,                     //parameter of the task
    1,                        //priority of the task
    &Task2,                   //Task handle to keep track of created task
    1);                       //core
}

void loop(){

  // send it out the serial port.This is for debugging purposes only:
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }
  //if ten seconds have passed since your last connection,
  // then connect again and send data:
  if (millis() - lastConnectionTime > postingInterval) {
    httpRequest();
  }
}//END

// this method makes a HTTP connection to the server:
void httpRequest() {
  // close any connection before send a new request.
  // This will free the socket on the WiFi shield
  client.stop();

  //if there's a successful connection:
  if (client.connect(google, 80)) {
    Serial.println("connecting...");
    // send the HTTP PUT request:
    client.println("GET /index.html HTTP/1.0");
    client.println("content-type: tex/plain");
    client.println("");
    client.println("");
    //client.println("DATA VALUE GOES HERE");
    // note the time that the connection was made:
    lastConnectionTime = millis();

  } else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
  }
}
