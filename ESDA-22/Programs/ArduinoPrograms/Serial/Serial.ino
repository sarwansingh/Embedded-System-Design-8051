void setup() {
  Serial.begin(9600);  //baud rate
  Serial.print("hello");
  pinMode( 13 , OUTPUT);
}

void loop() {
  int i = Serial.read();
  if (i != -1)   Serial.println(i);
  if (i==49 ) digitalWrite(13 , HIGH);
  if (i==50 ) digitalWrite(13 , LOW);
  delay(1000);

}
