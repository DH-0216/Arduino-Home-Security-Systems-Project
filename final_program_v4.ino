#include <RCSwitch.h>
#include <SoftwareSerial.h>

RCSwitch mySwitch = RCSwitch();
SoftwareSerial gsmSerial(7, 8);

const int echoPin = 3;               // Ultrasonic sensor echo pin
const int trigPin = 4;               // ultrasonic sensor trigger pin
const int sensor1 = 5;               // Door Magnet Sensor               
const int sensor2 = 6;               // Motion Sensor               
const int redLED1 = 10;
const int redLED2 = 11;                
const int whiteLED = 12;               
const int buzzer = 13;
const int A = 21952;                  // Remote Button A
const int B = 21808;                  // Remote Button B 
const int C = 21772;                  // Remote Button C
const int D = 21763;                  // Remote Button D 
int distance;
long duration;

bool isProgramRunning = true;

void setup() {
  pinMode(redLED1, OUTPUT);
  pinMode(redLED2, OUTPUT);
  pinMode(whiteLED, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(sensor1, INPUT_PULLUP);
  pinMode(sensor2, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  mySwitch.enableReceive(0);
  Serial.begin(9600);
  gsmSerial.begin(9600);
}


void turnOffProgram() {
  isProgramRunning = false;
}

void turnOnProgram() {
  isProgramRunning = true;
}

void SendMessage1 () {
  gsmSerial.println("AT+CMGF=1");
  delay(1000);
  gsmSerial.println("AT+CMGS=\"+94718639489\"");
  delay(1000);
  gsmSerial.println("Security Alert 1");
  Serial.println("Sending Message 1  ");
  delay(100);
  gsmSerial.write(26); // ASCII code for Ctrl+Z to end the SMS
  delay(1000);
}

void SendMessage2 () {
  gsmSerial.println("AT+CMGF=1");
  delay(1000);
  gsmSerial.println("AT+CMGS=\"+94718639489\"");
  delay(1000);
  gsmSerial.println("Security Alert 2");
  Serial.println("Sending Message 2  ");
  delay(100);
  gsmSerial.write(26);
  delay(1000);
}

void MakeCall () {
  gsmSerial.println("ATD+94718639489;");
  Serial.println("Calling   ");
  delay(1000);

}

void loop() {
  // Check if there's any received RF signal
  if (mySwitch.available()) {
    int remotevalue = mySwitch.getReceivedValue();
    if (gsmSerial.available() > 0)
    Serial.write(gsmSerial.read());
    // Process the received RF signal
    switch (remotevalue) {
      case A:
        turnOnProgram();
        break;
      case B:
        turnOffProgram();
        digitalWrite(whiteLED, LOW);
        digitalWrite(redLED2, LOW);
        break;
      case C:
        digitalWrite(whiteLED, HIGH);
        break;
      case D:
        digitalWrite(whiteLED, LOW);
        break;
      default:
        // Other codes or invalid signal received
        break;
    }

    // Reset the received value for the next signal
    mySwitch.resetAvailable();
  }

if (isProgramRunning) {

      
  if (digitalRead(sensor1) == HIGH) {
  
      digitalWrite(redLED1,HIGH);
      tone(buzzer, 600,600);
      delay(800);
      digitalWrite(redLED1,LOW);
      tone(buzzer, 600,600);
      delay(800);
      SendMessage2();
  }


  // Check if any motion detect
  if (digitalRead(sensor2) == HIGH) {
  
      digitalWrite(redLED2,HIGH);
     
      delay(800);
      digitalWrite(redLED2,LOW);
     
      delay(800);
      
  }
    
  delay(100); // Delay for stability


  // Clear the trigger pin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Set trigger pin to high for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure the duration of the echo pulse
  duration = pulseIn(echoPin, HIGH);

  // Calculate distance in centimeters
  distance = duration * 0.034 / 2;

  
   if (distance < 20) {
   digitalWrite(whiteLED, HIGH);
   SendMessage1();
   
  } else {
    digitalWrite(whiteLED,LOW);
    
  } 

  }
}

