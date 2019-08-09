#include <Arduino_FreeRTOS.h>

// define two tasks for Blink & AnalogRead
void TaskBlink( void *pvParameters );
void TaskBlink1( void *pvParameters );

// the setup function runs once when you press reset or power the board
void setup() {

  // Now set up two tasks to run independently.
  xTaskCreate(
    TaskBlink
    ,  (const portCHAR *)"Blink"   // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 1 being the highest, and 4 being the lowest.
    ,  NULL );

  xTaskCreate(
    TaskBlink1
    ,  (const portCHAR *) "Blink1"
    ,  128  // Stack size
    ,  NULL
    ,  1  // Priority
    ,  NULL );

  // Now the task scheduler, which takes over control of scheduling individual tasks, is automatically started.
}

void loop()
{
  // Empty. Things are done in Tasks.
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void TaskBlink(void *pvParameters)  // This is a task.
{
  (void) pvParameters;

  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);

  for (;;) // A Task shall never return or exit.
  {
    digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
    vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
    digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
    vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
  }
}

void TaskBlink1(void *pvParameters)  // This is a task.
{
  (void) pvParameters;

  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  // initialize digital pin 13 as an output.
  pinMode(12, OUTPUT);
  
  for (;;)
  {
    digitalWrite(12, HIGH);   // turn the LED on (HIGH is the voltage level)
//    digitalWrite(12, HIGH);   // turn the LED on (HIGH is the voltage level)
//    vTaskDelay( 2000 / portTICK_PERIOD_MS ); // wait for one second
//    digitalWrite(12, LOW);    // turn the LED off by making the voltage LOW
//    vTaskDelay( 2000 / portTICK_PERIOD_MS ); // wait for one second
  }
}
