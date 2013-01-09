
/*!
 *  Furniture Sensing Kit :: Arduino Sensor Sketch  *
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
 *  ** This is the sketch for the Arduino Uno that manages the capacitive sensor
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

#include "mpr121.h"
#include <Wire.h>
#include <SHT1x.h>

#define SENSORS       13
#define TOU_THRESH    0x2F
#define REL_THRESH    0x2A
#define PROX_THRESH   0x19
#define PREL_THRESH   0x16

bool debug = true;

// variables: capacitive sensing
bool touchStates[SENSORS];    // holds the current touch/prox state of all sensors
bool activeSensors[SENSORS] = {0,0,0,0,0,0,0,0,0,0,0,0,1}; // holds which sensors are active (0=inactive, 1=active)
bool newData = false;         // flag that is set to true when new data is available from capacitive sensor
int irqpin = 3;               // pin that connects to notifies when data is available from capacitive sensor


void setup(){

  // set IRQ pin to input, turn on the pullup resistor, and attach interrupt to pin
  pinMode(irqpin, INPUT);
  digitalWrite(irqpin, HIGH); 
  attachInterrupt(1, dataAvailable, FALLING);

  // set-up the Serial and I2C/Wire connections
  Serial.begin(9600);
  Wire.begin();

  // set the registers on the capacitive sensing IC
  setupCapacitiveRegisters();
}

void loop(){
  readCapacitiveSensor();
}

/**
 * dataAvailable Callback method that runs whenever new data becomes available on from the capacitive sensor. 
 *   This method was attached to the interrupt on pin 3, and is called whenever that pins goes low.
 */
void dataAvailable() {
  newData = true;
}

/**
 * readCapacitiveSensor Reads the capacitive sensor values from the MP121 IC. It makes a request to
 *   the sensor chip via the I2C/Wire connection, and then parses the sensor values which are stored on
 *   the first 13 bits of the 16-bit response msg.
 */
void readCapacitiveSensor(){
  if(newData){    
    //read the touch state from the MPR121
    Wire.requestFrom(0x5A,2); 
    byte tLSB = Wire.read();
    byte tMSB = Wire.read();
    uint16_t touched = ((tMSB << 8) | tLSB); //16bits that make up the touch states

    for (int i = 0; i < SENSORS; i++){  // Check what electrodes were pressed
      if (activeSensors[i] == 0) continue;
      char sensor_id;
      switch (i) {
        case 12:
          sensor_id = 'P';
          break;
        default:
          sensor_id = '\0';
      }
      if (sensor_id != '\0') {
        // read the humidity level

        // if current sensor was touched (check appropriate bit on touched var)
        if(touched & (1<<i)){      
          // if current pin was not previously touched send a serial message
          if(touchStates[i] == 0){          
            Serial.print(sensor_id);        
            Serial.println("1");

          } 
          touchStates[i] = 1;      
        } else {
          // if current pin was just released send serial message
          if(touchStates[i] == 1){
            Serial.print(sensor_id);
            Serial.println("0");
          }
          touchStates[i] = 0;
        }        
      }
    }
  newData = false;
//  Serial.println("end readCapacitiveSensor (new_data)");
  
  }
//  Serial.println("end readCapacitiveSensor (no new_data)");
}

/**
 * setupCapacitiveRegisters Updates all of configurations on the MP121 capacitive sensing IC. This includes
 *   setting levels for all filters, touch and proximity sensing activation and release thresholds, debounce,
 *   and auto-configurations options. At the end it activates all of the electrodes.
 */
void setupCapacitiveRegisters(){

  set_register(0x5A, ELE_CFG, 0x00); 
  
  // Section A - filtering when data is > baseline.
    // touch sensing
    set_register(0x5A, MHD_R, 0x01);
    set_register(0x5A, NHD_R, 0x01);
    set_register(0x5A, NCL_R, 0x00);
    set_register(0x5A, FDL_R, 0x00);

    // prox sensing 
    set_register(0x5A, PROX_MHDR, 0xFF);
    set_register(0x5A, PROX_NHDAR, 0xFF);
    set_register(0x5A, PROX_NCLR, 0x00);
    set_register(0x5A, PROX_FDLR, 0x00);

  // Section B - filtering when data is < baseline.
    // touch sensing
    set_register(0x5A, MHD_F, 0x01);
    set_register(0x5A, NHD_F, 0x01);
    set_register(0x5A, NCL_F, 0xFF);
    set_register(0x5A, FDL_F, 0x02);
  
    // prox sensing
    set_register(0x5A, PROX_MHDF, 0x01);
    set_register(0x5A, PROX_NHDAF, 0x01);
    set_register(0x5A, PROX_NCLF, 0xFF);
    set_register(0x5A, PROX_NDLF, 0xFF);

  // Section C - Sets touch and release thresholds for each electrode
    set_register(0x5A, ELE0_T, TOU_THRESH);
    set_register(0x5A, ELE0_R, REL_THRESH);
   
    set_register(0x5A, ELE1_T, TOU_THRESH);
    set_register(0x5A, ELE1_R, REL_THRESH);
    
    set_register(0x5A, ELE2_T, TOU_THRESH);
    set_register(0x5A, ELE2_R, REL_THRESH);
    
    set_register(0x5A, ELE3_T, TOU_THRESH);
    set_register(0x5A, ELE3_R, REL_THRESH);
    
    set_register(0x5A, ELE4_T, TOU_THRESH);
    set_register(0x5A, ELE4_R, REL_THRESH);
    
    set_register(0x5A, ELE5_T, TOU_THRESH);
    set_register(0x5A, ELE5_R, REL_THRESH);
    
    set_register(0x5A, ELE6_T, TOU_THRESH);
    set_register(0x5A, ELE6_R, REL_THRESH);
    
    set_register(0x5A, ELE7_T, TOU_THRESH);
    set_register(0x5A, ELE7_R, REL_THRESH);
    
    set_register(0x5A, ELE8_T, TOU_THRESH);
    set_register(0x5A, ELE8_R, REL_THRESH);
    
    set_register(0x5A, ELE9_T, TOU_THRESH);
    set_register(0x5A, ELE9_R, REL_THRESH);
    
    set_register(0x5A, ELE10_T, TOU_THRESH);
    set_register(0x5A, ELE10_R, REL_THRESH);
    
    set_register(0x5A, ELE11_T, TOU_THRESH);
    set_register(0x5A, ELE11_R, REL_THRESH);

  // Section D - Set filter Configuration
    set_register(0x5A, FIL_CFG, 0x04);  

  // Section E - Set proximity sensing threshold and release
    set_register(0x5A, PRO_T, PROX_THRESH);   // sets the proximity sensor threshold
    set_register(0x5A, PRO_R, PREL_THRESH);   // sets the proximity sensor release

  // Section F - Set proximity sensor debounce
    set_register(0x59, PROX_DEB, 0x50);  // PROX debounce

  // Section G - Set Auto Config and Auto Reconfig for prox sensing
    // auto_config values for 3.3v set-ups
    // set_register(0x5A, ATO_CFGU, 0xC9);  // USL = (Vdd-0.7)/vdd*256 = 0xC9 @3.3V   
    // set_register(0x5A, ATO_CFGT, 0xB5);  // Target = 0.9*USL = 0xB5 @3.3V
    // set_register(0x5A, ATO_CFGL, 0x82);  // LSL = 0.65*USL = 0x82 @3.3V
    // set_register(0x5A, ATO_CFG0, 0x0B);  // auto-config controls

    // auto_config values for 5v set-ups
    set_register(0x5A, ATO_CFGU, 0xDC);  // USL = (Vdd-0.7)/vdd*256 = 0xDC @5V   
    set_register(0x5A, ATO_CFGT, 0xC6);  // Target = 0.9*USL = 0xC6 @5V
    set_register(0x5A, ATO_CFGL, 0x8F);  // LSL = 0.65*USL = 0x8F @5V
    set_register(0x5A, ATO_CFG0, 0x0B);  // auto-config controls

  // Section H - Start listening to all electrodes and the proximity sensor
    set_register(0x5A, ELE_CFG, 0x3C);
}

/**
 * set_register Sets a register on a device connected via I2C. It accepts the device's address, 
 *   register location, and the register value.
 * @param address The address of the I2C device
 * @param r       The register's address on the I2C device
 * @param v       The new value for the register
 */
void set_register(int address, unsigned char r, unsigned char v){
  Wire.beginTransmission(address);
  Wire.write(r);
  Wire.write(v);
  Wire.endTransmission();
}
