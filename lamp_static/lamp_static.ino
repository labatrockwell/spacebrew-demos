/*
 * Forest_Output_Arduino_02
 * Turns on LED 13 whenever it receive a serial message.
 * Based on Output_General_Arduino sketch for Maker event
 * 
 * Version: 1.0.1
 * Date: 5/5/12
 *
 */

#include <SPI.h>
#include <Spacebrew.h>
#include <Ethernet.h>
#include <WebSocketClient.h>

uint8_t mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,1, 177);
// byte gateway[] = { 10, 0, 0, 1 };
// byte subnet[] = { 255, 255, 0, 0 };

Spacebrew spacebrewConnection;

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

  //connect to spacebrew library info
  spacebrewConnection.onOpen(onOpen);
  spacebrewConnection.onClose(onClose);
  spacebrewConnection.onError(onError);
  Serial.println(F("registering spacebrew methods, onopen, onclose, onerror"));
  
  //connect to message callbacks
  spacebrewConnection.onBooleanMessage(onBooleanMessage);
  Serial.println(F("registering spacebrew methods, onBooleanMessage"));
  
  //register publishers and subscribers
  spacebrewConnection.addPublish("Blinked", SB_BOOLEAN);
  spacebrewConnection.addSubscribe("Blink", SB_BOOLEAN);
  
  //connect to the spacebrew server
  Ethernet.begin(mac, ip);
  spacebrewConnection.connect("ec2-184-72-140-184.compute-1.amazonaws.com", "Lamp", "Lamp that blinks on and off");
  Serial.println(F("connected to spacebrew"));
  
  pinMode(ledPin, OUTPUT);
  pinMode(powerPin, OUTPUT);

  digitalWrite(ledPin, LOW);
  digitalWrite(powerPin, HIGH);
}  

void loop() {
  spacebrewConnection.monitor();

  // read serial messages
  if (Serial.available() > 0) {
    activatePower();
    spacebrewConnection.send("Blinked", (bool)true);
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

void onBooleanMessage(char *name, bool value){
    activatePower();
    spacebrewConnection.send("Blinked", (bool)true);
}

void onOpen(){
  //send a message when we get connected!
  Serial.println(F("Spacebrew Connection Established"));
}

void onClose(int code, char* message){
  //turn everything off if we get disconnected
  Serial.println(F("Spacebrew Connection Closed"));
}

void onError(char* message){
  Serial.println(F("Spacebrew Error - Message:"));  
  Serial.println(message);  
}



