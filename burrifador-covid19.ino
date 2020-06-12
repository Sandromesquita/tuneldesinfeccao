/*
 * PROJECT OF SPRAY AGAINST COVID19
 * Client: Pixels Escola de Design e Tecnologia
 * Start: 5/26/2020
 * Finish:
 * Update:
 * 
 */
const byte pinSense1 = 2; //Digital pin 2 of the Arduino connected to sense 1 (blue)
const byte pinSense2 = 3; //Digital pin 3 of the Arduino connected to sense 2 (yellow)
const byte pinSense3 = 4; //Digital pin 4 of the Arduino connected to sense 3 (orange)
const byte pinRelayPump = 5; //Digital pin 5 of the Arduino connected to pin Relay of Pump Water
const byte pinBuzzer= 6; //Digital pin 6 of the Arduino connected to buzzer (green)
bool inputSensor, middleSensor, outputSensor;
 
void setup(){
  Serial.begin(9600); //Starting serial communication
  pinMode(pinSense1, INPUT); //Define the pin 2 as input
  pinMode(pinSense2, INPUT); //Define the pin 3 as input
  pinMode(pinSense3, INPUT); //Define the pin 4 as input
  pinMode(pinRelayPump, OUTPUT); //Define the pin 5 as output
  pinMode(pinBuzzer, OUTPUT); //Define the pin 5 as output
}
void loop(){
  inputSensor = digitalRead(pinSense1);
  middleSensor = digitalRead(pinSense2);
  outputSensor = digitalRead(pinSense3);
  Serial.println("Input Sensor: " + String(inputSensor) + " | Middle Sensor: " + String(middleSensor) + " | Output Sensor: " + String(outputSensor));
  //If any of the 3 sensors is trigged
 if((inputSensor == HIGH) || (middleSensor == HIGH) || (outputSensor == HIGH)){
    Serial.println("Detected person presence in the tunel.");
    digitalWrite(pinRelayPump, HIGH); //Start the pump
    noTone(pinBuzzer);
    tone(pinBuzzer, 440, 200);
    delay(300);
    
    
 }else{ //If you don't
  Serial.println("No movement in the tunel.");
  digitalWrite(pinRelayPump, LOW); //Turn off the pump
  noTone(pinBuzzer); //Disable the buzzer
 }
}
