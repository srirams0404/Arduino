#define SensorPin A0      //pH meter Analog output to Arduino Analog Input 0
float calibration_value = 21.34 + 3.2;
unsigned long int avgValue;  //Store the average value of the sensor feedback
int buf[10], temp;

void setup(){
  Serial.begin(9600);
  pinMode(SensorPin, INPUT);
}


void loop(){
  for(int i=0;i<10;i++){ //Get 10 sample value from the sensor for smooth the value
  buf[i]=analogRead(SensorPin);
  delay(10);
  }
  for(int i=0;i<9;i++){ //sort the analog from small to large
    for(int j=i+1;j<10;j++){
      if(buf[i]>buf[j]){
        temp=buf[i];
        buf[i]=buf[j];
        buf[j]=temp;
      }
    }
  }
  avgValue=0;
  for(int i=2;i<8;i++)avgValue+=buf[i];  //take the average value of 6 center sample
    
  float phValue=(float)avgValue*5.0/1024/6; //convert the analog into millivolt
  phValue = -5.70 * phValue + calibration_value; //convert the millivolt into pH value
  Serial.print("pH value:");
  Serial.println(phValue);
  delay(1000);
}