#include <Arduino_FreeRTOS.h>

void setup()
{
  Serial.begin(9600);
  Serial.println(F("In Setup function"));
  /* Create two tasks with priorities 1 and 2. An idle task is also created, 
     which will be run when there are no tasks in RUN state */
  xTaskCreate(MyTask1, "Task1", 100, NULL, 1, NULL);
  xTaskCreate(MyTask2, "Task2", 100, NULL, 2, NULL);
  xTaskCreate(MyIdleTask, "IdleTask", 100, NULL, 0, NULL);
}
void loop()
{
  // DO nothing
}
/* Task1 with priority 1 */
static void MyTask1(void* pvParameters)
{
  while(1)
  {
    Serial.println(F("Task1"));
    vTaskDelay(100/portTICK_PERIOD_MS);
  }
}
/* Task2 with priority 2 */
static void MyTask2(void* pvParameters)
{
  while(1)
  {    
    Serial.println(F("Task2"));
    vTaskDelay(150/portTICK_PERIOD_MS);
  }
}
/* Idle Task with priority Zero */ 
static void MyIdleTask(void* pvParameters)
{
  while(1)
  {
    Serial.println(F("Idle state"));
    delay(50);
  }
}
