/*
 * Version: 1.0.1
 * Date: 5/5/12
 *
 */

// control powerSocket
const int powerPin = A0;
const int ledPin = 13;
boolean powerOn = false;
long powerActiveMillis = 0;
long powerActiveInterval = 5000;

// control LEDs

int incomingByte;  //a variable to read incoming serial data into

void setup() {
  //initiate serial communication:
  Serial.begin(9600);
  Serial.println(F("starting up"));
  
  pinMode(ledPin, OUTPUT);
  pinMode(powerPin, OUTPUT);

  digitalWrite(ledPin, LOW);
  digitalWrite(powerPin, HIGH);
}  

void loop() {

  // read serial messages
  if (Serial.available() > 0) {
    activatePower();
    Serial.println(F("Blinked"));

    //read all the bytes in the serial buffer (we don't care about the contents of the data)
    while (Serial.available()) incomingByte = Serial.read();
  }                       
  deactivatePower();
}

// turns on Power
void activatePower() {
  // turn on the light by setting this pin to LOW
  digitalWrite(ledPin, HIGH);
  digitalWrite(powerPin, LOW);
  powerActiveMillis = millis();
  powerOn = true;
}

// turns off Power
void deactivatePower() {
  if (powerOn) {
    if(millis() - powerActiveMillis > powerActiveInterval) {
      digitalWrite(powerPin, HIGH);
      digitalWrite(ledPin, LOW);
      powerOn = false;
    }  
  }
}
