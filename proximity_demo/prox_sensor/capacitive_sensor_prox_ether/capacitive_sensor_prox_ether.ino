
/*!
 *  Furniture Sensing Kit :: Spacebrew Connect - on Ethernet Arduino  *
 * 
 *  This sketch was developed for a capacitive sensing prototype that uses capacitive sensors to 
 *  perform proximity sensing functions. 
 *  
 *  Whenever proximity is sensed, the sketch sends a serial message with the id char 'P' (for presence)
 *  followed by a 0 or 1. The state "1" denotes proximity, while state "0" denotes that the proximity 
 *  has ended. 
 *  
 *  Proximity measurements are cumulative readings from all sensors. Therefore, all sensors must be 
 *  hooked up correctly in order for this measurement to work properly.
 *
 *  This prototype will feature two separate Arduinos connected together. An Arduino Uno will manage the
 *  proximity sensor (MPR121), while an Ehternet Arduino will handle the Spacebrew connection. These devices 
 *  will be connected to each other via Serial. 
 *
 *  ** This is the sketch for the Ethernet Arduino that will connect to Spacebrew
 *
 *  ** Based on Spacebrew Range Forwarder Sketch, with minor modifications to support boolean 
 *     messages for presence.
 *  
 *  About the Proximity Sensor: 
 *  This prototype uses an MR121 IC from Freescale, mounted on a sparkfun proto board which is 
 *  connected to the Arduino via I2C. This sketch was adapted from a sketch developed by Jim Bloom and 
 *  published here: http://bildr.org/2011/05/mpr121_arduino/
 *  
 *  <br />Copyright (C) 2012 LAB at Rockwell Group http://lab.rockwellgroup.com
 *
 * @filename    capacitive_sensor_prox.ino
 * @author      Julio Terra (Lab at Rockwell Group)
 * @modified    11/10/2012
 * @version     0.0.1
 * 
*/

#include <SPI.h>
#include <Spacebrew.h>
#include <Ethernet.h>
#include <WebSocketClient.h>

/**************************************
 * CUSTOMIZABLE CONSTANTS AND VARIABLES
 **************************************/

#define MSG_LEN       4       // maximum length of the serial messages
#define MSG_END       '\n'    // message end character
#define VAL_LEN       1       // maximum length of each int value, add one to the total for an end byte
#define PUB_CNT       1       // number of publish channels (or outlets)
#define MAX_NAME_LEN  20  

boolean debug =     true;  // flag that turns on, and off, the debug messages

// mac address of Arduino (just has to be a unique address, not important if it is actual address)
uint8_t mac[] = {0x92, 0xA2, 0xDA, 0x00, 0x68, 0xF8};
IPAddress ip(128, 122, 151, 115);

char sbName [MAX_NAME_LEN + 1] = "furniture_kit_"; // base name of the app
int sbNameLen = 14;                                // length of base name
// IMPORTANT: don't forget to update the server address and description in the spacebrewConnection.connect method

// serial to spacebrew message parsing variables
char pubNames [PUB_CNT][11] =     {"presence"}; // name of publish channels (outlets) 
char pubSerialIds [PUB_CNT] =     {'P'};           // char ids used in serial message to identify each value
enum SBType pubTypes [PUB_CNT] =  {SB_BOOLEAN}; // type of publish channels (outlets)
int pubData [PUB_CNT] =           {0};                // current state, also last published state


/*****************************
 * SET CONSTANTS AND VARIABLES
 *****************************/

Spacebrew spacebrewConnection;

char serialMsg [MSG_LEN];
int serialMsgLen = 0;

boolean msgStarted = false;
boolean msgComplete = false;

void setup()   {
  randomSeed(analogRead(0));

  if (sbNameLen >= (MAX_NAME_LEN - 5)) sbNameLen = MAX_NAME_LEN - 5;
  for (int i = sbNameLen; i < MAX_NAME_LEN; i++) {    
    if (i < (sbNameLen + 4)) sbName[i] = char(int(random(int('0'),int('9'))));
    else sbName[i] = '\0';
  }

  Serial.begin(9600);
  while (!Serial) {}
  if (debug) Serial.println(F("serial: connected"));
  resetMsg();

  Ethernet.begin(mac, ip);
  // if (Ethernet.begin(mac) == 0) {
  //   if (debug) Serial.println(F("ERROR: ethernet"));      
  // } else {
  //   if (debug) {
  //     Serial.print(F("ethernet: connected\n  - ip: "));
  //     Serial.println(Ethernet.localIP());
  //   }
  // }

  //connect to SB callback methods
  spacebrewConnection.onOpen(onOpen);
  spacebrewConnection.onClose(onClose);
  spacebrewConnection.onError(onError);
  spacebrewConnection.onBooleanMessage(onBooleanMessage);
  spacebrewConnection.onStringMessage(onStringMessage);
  spacebrewConnection.onRangeMessage(onRangeMessage);

  //register publishers and subscribers
  for (int i = 0; i < PUB_CNT; i++) spacebrewConnection.addPublish(pubNames[i], pubTypes[i]);

  // spacebrewConnection.connect("10.0.1.11", "check_in_station_4", "check in station prototype");    
  spacebrewConnection.connect("ec2-184-72-140-184.compute-1.amazonaws.com", sbName, "furniture kit prototype");    

  if (debug) { Serial.print(F("spacebrew: attempting connection\n  - name: ")); Serial.println(sbName);}

  delay(1000);

  if (debug) Serial.println(F("set-up complete"));
}

void loop() {
  spacebrewConnection.monitor();
  readSerial();
  processSerial();
}

void readSerial() {
  if (Serial.available()) {
    while (Serial.available()){
      serialMsg[serialMsgLen] = char(Serial.read());


      for (int j = 0; j < PUB_CNT; j++) {
        if (serialMsg[serialMsgLen] == pubSerialIds[j]) {
          msgStarted = true;
          if (debug) Serial.println(F("serialMsg started: "));
          break;
        } 
      }

      if (msgStarted) {

        if (serialMsg[serialMsgLen] == MSG_END) {
          msgComplete = true;
          if (debug) Serial.println();
          if (debug) Serial.print(F("serialMsg completed"));
          if (debug) Serial.println(serialMsg);
          if (debug) Serial.println(serialMsgLen);
          break;        
        } 

        serialMsgLen ++;
        if (debug) Serial.print(serialMsg[serialMsgLen]);

        if (serialMsgLen >= MSG_LEN) {
          if (debug) Serial.println(F("serialMsg reset"));
          resetMsg();
        }
      }
    }
  }  
}

void processSerial() {
  if (msgStarted && msgComplete) {
    if (debug) { Serial.print(F("processing message ")); Serial.println(serialMsg); }

    for (int j = 0; j < PUB_CNT; j++) {
      char curMsg [VAL_LEN + 1];
      for (int k = 0; k < VAL_LEN + 1; k++) curMsg[k] = '\0';
      char curId = '\0';
      int curMsgLen = 0;

      int i = 0;
      for (bool readingVal = true; (i < serialMsgLen) && readingVal; i++) {

        if (serialMsg[i] == pubSerialIds[j]) {
          curId = serialMsg[i];
        }

        else if (curId != '\0' && serialMsg[i] >= '0' && serialMsg[i] <= '9') {
          curMsg[curMsgLen] = serialMsg[i];
          curMsgLen++;
        }

        else if (curId != '\0') {
          readingVal = false;

          if (curMsgLen > 0) {
            int curValue = atoi(curMsg);
            if (pubData[j] != curValue) {
              pubData[j] = curValue;
              spacebrewConnection.send(pubNames[j], ((pubData[j] == 0) ? (bool)false : (bool)true) );
              if (debug) { Serial.print(F("status for '")); Serial.print(pubNames[j]); }
              if (debug) { Serial.print(F("' is ")); Serial.println(pubData[j]); }                  
            }
          }

          if (i < (serialMsgLen - 1)) i--;
          else i = serialMsgLen;
        }
      }
    }
    resetMsg();
  }
}

void resetMsg() {
  for (int i = 0; i < MSG_LEN; i++) {
    serialMsg[i] = '\0';
  }
  msgStarted = false;
  msgComplete = false;
  serialMsgLen = 0;
}

void onOpen(){
  if (debug) Serial.println(F("Connected to SB"));
}

void onClose(int code, char* message){
  if (debug) Serial.println(F("Disconnected from SB"));
}

void onError(char* message){
  if (debug) Serial.println(F("SB error, msg:"));  
  if (debug) Serial.println(message);  
}

void onBooleanMessage (char *inlet, bool serialMsg) {}

void onStringMessage (char *inlet, char *serialMsg) {}

void onRangeMessage (char *inlet, int serialMsg) {}

