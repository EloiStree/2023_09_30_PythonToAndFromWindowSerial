

void setup() {
  Serial.begin(9600);
}
int i=0;
void loop() {
  // Write a character
  i++;
  if(i>5)
    i=0;
  char myChar = 'A'+ i;
  String s=String(myChar);;
  Serial.println(s);
  delay(500);
  s.toLowerCase();
  Serial.println(s);
  delay(500);

  Serial.println(s+"0");
  delay(150);
  Serial.println(s+"8");
  delay(150);
  Serial.println(s+"4");
  delay(150);
  Serial.println(s+"|");
  delay(150);
  Serial.println(s+"9");
  delay(150);
  Serial.println(s+"A");
  delay(150);
  Serial.println(s+"z");
  delay(150);
  Serial.println(s+"d");
  delay(150);
  Serial.println(s+"j");
  delay(150);
  Serial.println(s+"l");
  delay(150);
  Serial.println(s+"M");
  delay(150);
  Serial.println(s+"G");
  delay(150);
  delay(5000);

  // Write a string
  /*
  String myString = "Hello, Arduino!";
  Serial.print("String: ");
  Serial.println(myString);
  //*/
  // Wait for 1 second
}


