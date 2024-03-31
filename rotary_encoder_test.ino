/*
  Rotary encoder test
  http://www.electronoobs.com
  http://www.youtube.com/c/ELECTRONOOBS
 */
 
 #define clk 8
 #define data 9
  
 int counter = 0; 
 int State;
 int LastState;  
 
 void setup() { 
  //Define the pins as inputs
   pinMode (clk,INPUT);
   pinMode (data,INPUT);   
   Serial.begin (9600); //Start serial com so we could print the value on the serial monitor
   // Reads the initial state of the clock pin
   LastState = digitalRead(clk);   
 } 
 void loop() { 
   State = digitalRead(clk); // Reads the "current" state of the clock pin
   // If the previous and the current state of the clock are different, that means a step has occured
   if (State != LastState){     
   // If the data state is different to the clock state, that means the encoder is rotating clockwise
     if (digitalRead(data) != State) { 
       counter ++;
     } else {
       counter --;
     }
     Serial.print("Position: ");
     Serial.println(counter);
   } 
   LastState = State; // Updates the previous state of the clock with the current state
 }