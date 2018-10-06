/*Simple example of multi loop
* By Evandro Luis Copercini
* Based on pcbreflux video
* Public domain license 2017
*/

//THREADS DEFINITIONS

TaskHandle_t Task1;

void codeForTask1( void * parameter){

  Serial.begin(115200);
  while(true){
      Serial.println("Print from core 0, TASK1");
      delay(2000);
  }
}

///SETUP

void setup(){
  Serial.begin(115200);
  xTaskCreatePinnedToCore(
    codeForTask1,             //Task function
    "Task_1",                 //name of task
    1000,                     //Stack size of the task
    NULL,                     //parameter of the task
    1,                        //priority of the task
    &Task1,                   //Task handle to keep track of created task
    0);                       //core
}

void loop(){

  Serial.println("Print from core 1, main loop");
  delay(4000);

}

//END
