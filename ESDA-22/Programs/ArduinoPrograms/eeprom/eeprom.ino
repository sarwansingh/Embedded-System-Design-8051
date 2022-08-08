#include <EEPROM.h>
void setup() {
 //EEPROM.write(10, 1); // write value 1 on address 10
 Serial.begin(9600);
 int v = EEPROM.read(10);
 Serial.print("value read from EEPROM address 10 ");
 Serial.println(v);
}
int r; 
void loop() {
  r = Serial.read();
  if (r > 0) {
    r = r-48;
    Serial.println(r);
    EEPROM.write(10, r);
  }
}
