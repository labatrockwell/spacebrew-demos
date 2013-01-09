import spacebrew.*;
import processing.serial.*;

String server="ec2-184-72-140-184.compute-1.amazonaws.com";
String name="LampPro";
String description ="Lamp that blinks on and off";

Spacebrew spacebrewConnection;
Serial myPort;        // The serial port 

void setup() {
  size(600, 400);

  spacebrewConnection = new Spacebrew( this );
  
  // add each thing you publish to
 spacebrewConnection.addSubscribe( "Blink", "boolean" ); 
 spacebrewConnection.addPublish( "Blinked", true ); 

  // connect!
  spacebrewConnection.connect("ws://"+server+":9000", name, description );

  println(Serial.list());
  myPort = new Serial(this, Serial.list()[0], 9600); 
  myPort.bufferUntil('\n');
}

void draw() {
}

void serialEvent (Serial myPort) {
  // get the ASCII string:
  String inString = myPort.readStringUntil('\n');
  println(inString);
  spacebrewConnection.send( "Blinked", true);
}

void onBooleanMessage( String name, boolean value ){
  myPort.write('B');
}
