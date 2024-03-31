const int photo_diode = A0;
int input_Val = 0 ;
//defining variables

void setup(){
  Serial.begin(9600);
}
//starts serial monitor

void loop(){
  input_Val = analogRead(photo_diode);
  Serial.print("Input Value = ");
  Serial.println(input_Val);
  delay(1000);
  exit(0);
}
//loops the code
