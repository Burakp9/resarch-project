void setup() 
{
// initialize the serial communication:
Serial.begin(9600);
pinMode(14, INPUT); // Setup for leads off detection LO +
pinMode(12, INPUT); // Setup for leads off detection LO -
  
}
  
void loop() {
  
if((digitalRead(14) == 1)||(digitalRead(12) == 1)){
Serial.println('!');
}
else{
// send the value of analog input 0:
Serial.println(analogRead(13));/A0
}
//Wait for a bit to keep serial data from saturating
delay(1);
}