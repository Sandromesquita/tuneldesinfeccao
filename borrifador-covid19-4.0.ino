/*
   PROJECT OF SPRAY AGAINST COVID19
   Version: 4.0
   Commit: Add send to internet.
   Client: Pixels Escola de Design e Tecnologia
   Start: 6/17/2020
   Finish: 
   Update: 

*/
//Definition of the Arduino pins
const byte pinInputSensor = 2; //Digital pin 2 of the Arduino connected to sense 1 (Thread blue)
const byte pinMiddleSensor = 3; //Digital pin 3 of the Arduino connected to sense 2 (Thread yellow)
const byte pinOutputSensor = 4; //Digital pin 4 of the Arduino connected to sense 3 (Thread orange)
const byte pinRelayPump = 5; //Digital pin 5 of the Arduino connected to pin Relay of Pump Water
const byte pinBuzzer = 6; //Digital pin 6 of the Arduino connected to buzzer (Thread green)
const byte pinGreenLed = 8; // Digital pin 8 of the Arduino connected to Led Green
const byte pinYellowLed = 9; // Digital pin 9 of the Arduino connected to Led Yellow
const byte pinRedLed = 10; // Digital pin 10 of the Arduino connected to Led Red
const byte pinMedLevelSensor = 12; // Digital pin 12 of the Arduino connected to Sensor of Medium Level (Thread white)
const byte pinLowLevelSensor = 13; // Digital pin 11 of the Arduino connected to Sensor of Low Level (Thread orange)
const byte pinSensorPeopleEntering = A0; // Digital pin 14 of the Arduino connected to Infrared Sensor of people entering
const byte pinSensorPeopleOut = A1; // Digital pin 15 of the Arduino connected to Infrared Sensor of people comming out.

//Definition of variables of the sensors
bool inputSensor, middleSensor, outputSensor; //Variables that keep the state of the of presence sensors
bool lowLevelSensor, medLevelSensor; //Variables that keep the state of the of water level sensors
bool sensorPeopleEntering, sensorPeopleOut, people=0, count=0; // Variables that keep the state of the Infrared Sensor that counts people.

void setup() {
  Serial.begin(9600); //Starting serial communication
  //Sets the Sensors pins
  pinMode(pinInputSensor, INPUT); //Define the pin 2 as input
  pinMode(pinMiddleSensor, INPUT); //Define the pin 3 as input
  pinMode(pinOutputSensor, INPUT); //Define the pin 4 as input
  pinMode(pinLowLevelSensor, INPUT_PULLUP); //Define the pin 11 as input
  pinMode(pinMedLevelSensor, INPUT_PULLUP); //Define the pin 12 as input
  pinMode(pinSensorPeopleEntering, INPUT); //Define the pin 14 as input
  pinMode(pinSensorPeopleOut, INPUT); //Define the pin 15 as input

  //Sets the Actuators pins
  pinMode(pinRelayPump, OUTPUT); //Define the pin 5 as output
  pinMode(pinBuzzer, OUTPUT); //Define the pin 6 as output
  pinMode(pinGreenLed, OUTPUT); //Define the pin 8 as output
  pinMode(pinYellowLed, OUTPUT); //Define the pin 9 as output
  pinMode(pinRedLed, OUTPUT); //Define the pin 10 as output

}//End of the setup

void loop() {
  //Checks water level sensors
  lowLevelSensor = digitalRead(pinLowLevelSensor);
  medLevelSensor = digitalRead(pinMedLevelSensor);

  //Checks infrared sensor if passed person
  sensorPeopleEntering = analogRead(pinSensorPeopleEntering);
  sensorPeopleOut = analogRead(pinSensorPeopleOut);

  //If input sensor is activated and the output sensor has not been activated.
  if ((sensorPeopleEntering < 300)&&(people == 0)) {
    people = 1;
    countPeople();
  }

  //If input sensor is activated and the output sensor is activated.
  if ((sensorPeopleEntering < 300)&&(people == -1)) {
    people = 0;
    Serial.println("Person leaving.");
  }

  //If output sensor is activated and the input sensor has not been activated.
  if ((sensorPeopleOut < 300)&&(people == 0)){
    people = -1;
    countPeople();
  }

  //If output sensor is activated and the input sensor is activated.
  if ((sensorPeopleOut < 300)&&(people == 1)){
    people = 0;
    Serial.println("Person entering.");
    
      //==PRÓXIMA FASE APÓS TESTES DO CONTADOR============    
          //Body temparature reading.
          //Ultrasonic gel sensor reading.
          //Seding the data to the server.
      //==^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^=============
      
  }
    //Logic of water level sensors: 0 --> With water | 1 --> No water

    // If both sensors are active (With water)
    if ((lowLevelSensor == 0) and (medLevelSensor == 0)) {
      digitalWrite(pinGreenLed, HIGH);
      digitalWrite(pinYellowLed, LOW);
      digitalWrite(pinRedLed, LOW);
      enabledTunnel();// Tunnel operating function
    }

    // If the medium level sensor not activated (Low water)
    if ((lowLevelSensor == 0) and (medLevelSensor == 1)) {
      digitalWrite(pinGreenLed, LOW);
      digitalWrite(pinYellowLed, HIGH);
      digitalWrite(pinRedLed, LOW);
      enabledTunnel();// Tunnel operating function
    }

    // If the low level sensor not activated (no water) but medium level sensor are active
    if ((lowLevelSensor == 1) and (medLevelSensor == 0)) {
      //Error in sensors
      digitalWrite(pinGreenLed, HIGH);
      digitalWrite(pinYellowLed, HIGH);
      digitalWrite(pinRedLed, HIGH);
      Serial.println("Level sensor in ERROR!");
      disableTunnel();
    }

    // If both sensors not activated (empty tank)
    if ((lowLevelSensor == 1) and (medLevelSensor == 1)) {
      digitalWrite(pinGreenLed, LOW);
      digitalWrite(pinYellowLed, LOW);
      digitalWrite(pinRedLed, HIGH);
      disableTunnel();// Tunnel no operating function
    }
    Serial.println("======================================================");
  } //End of the loop

  void enabledTunnel() {
    Serial.println("Tunnel enabled.");

    //Reading the status of the sensors
    inputSensor = digitalRead(pinInputSensor);
    middleSensor = digitalRead(pinMiddleSensor);
    outputSensor = digitalRead(pinOutputSensor);

    //Prints sensor readings on the serial monitor
    Serial.println("Input Sensor: " + String(inputSensor) + " | Middle Sensor: " + String(middleSensor) + " | Output Sensor: " + String(outputSensor));

    //If any of the 3 motion sensors is trigged
    if ((inputSensor == HIGH) || (middleSensor == HIGH) || (outputSensor == HIGH)) {
      Serial.println("Detected person presence in the tunel.");
      digitalWrite(pinRelayPump, HIGH); //Start the pump
      noTone(pinBuzzer);
      tone(pinBuzzer, 440, 200);
      delay(300);
    }

    else { //If you don't
      Serial.println("No movement in the tunel.");
      digitalWrite(pinRelayPump, LOW); //Turn off the pump
      noTone(pinBuzzer); //Disable the buzzer
      delay(300);
    }

  }//end enabledTunnel

  void disableTunnel() {
    Serial.println("Tunnel disabled.");
    digitalWrite(pinRelayPump, LOW); //Turn off the pump
    noTone(pinBuzzer); //Disable the buzzer
    delay(500);
    tone(pinBuzzer, 3000, 200);
    delay(500);
  }

  void countPeople(){
    count = count + people;
  }
