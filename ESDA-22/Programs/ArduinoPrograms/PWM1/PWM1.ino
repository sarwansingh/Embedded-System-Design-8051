int led1 = 9, led2 = 10;
void setup() {
  pinMode(led1, OUTPUT); pinMode(led2, OUTPUT);
  digitalWrite(led2, HIGH);
}

void loop() {
  for( int i=0; i<255 ; i+=25){
    analogWrite(led1, i);   delay(700);
  }
//  analogWrite(led1, 10);   delay(1000);
//  analogWrite(led1, 50);  delay(1000);
//  analogWrite(led1, 100);   delay(1000);
//  analogWrite(led1, 150);  delay(1000);
//  analogWrite(led1, 200);   delay(1000);
//  analogWrite(led1, 250);  delay(1000);
}
