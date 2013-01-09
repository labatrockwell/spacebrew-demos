Tweet Lamp Demo
---------------

This demo shows how spacebrew can be used to connect objects to data from webservices, such as twitter. Steps to prepare the demo:  

##Materials Requirements
* Computer with a Arduino, Processing (optional), Chrome browser
* Arduino Uno or Ethernet
* PowerSwitch Tail
* Hook-up Cable
* USB Cable
* AC-powered lamp
* DC Power Supply 12v, 0.5a (for Ethernet Arduino only)
  
##Figure Out How Arduino Will Connect to Spacebrew  
  
###Connecting Via Ethernet using DHCP  
Make following updates to `lamp' sketch and re-upload:  
* Update the MAC address if necessary  
* Configure spacebrew server if not using amazon cloud server.  
  
###Connecting Via Ethernet using Static IP
Make following updates to `lamp_static' sketch and upload:  
* Update the MAC address if necessary  
* Configure spacebrew server if not using amazon cloud server.  
* Set the ip address (subnet and gateway as well if necessary).  
  
###Connecting Via Serial/Processing  
Upload the `lamp_forwarder_ard' project to the Arduino  
  
Make following updates to `lamp_forwarder_ard' sketch (but don't run it yet):  
* Configure spacebrew server if not using amazon cloud server.  
   
##Hook-up Arduino and PowerSwitch Tail  
Now that the Arduino is ready to go, let's hook-up the electronics  
* Hook-up arduino to PowerSwitch tail (5v = +in, ground = ground, pin A0 = -in)  
* Plug-in light to PowerSwitch tail  
* Plug-in PowerSwitch tail to power   
* Connect Arduino to Internet via USB/computer or Ethernet/switch  
* Power Arduino using 12v adapter if using Ethernet sketch  

##Launch Processing Sketch  
If connecting to Spacebrew view Serial/Processing, then launch the processing sketch `lamp_forwarder_pro`. Make sure that you only have one Arduion connected to the computer. [Check Spacebrew admin](http://spacebrew.cc/master/spacebrew/admin/admin.html?server=ec2-184-72-140-184.compute-1.amazonaws.com) to make sure that the `lampPro` client showed up.

##Launch the Twitter App  
Go to the terminal app and navigate to the root folder of the twitter app and run the command below. This command launch the spacebrew_twitter app on [`http://localhost:3002`](http://localhost:3002/twitter/?server=ec2-184-72-140-184.compute-1.amazonaws.com&name=new_tweets&), it sets the base spacebrew server to `ec2-184-72-140-184.compute-1.amazonaws.com`, and the front-end websockets connection to `ws://localhost:3009`.  

```
Personal Set-up Folder Location:
/Developer/LAB_Projects/prototypes_experiments/Spacebrew/_dev/javascript/spacebrew-twitter_app
```
  
```
node app.js port=3002 server=ec2-184-72-140-184.compute-1.amazonaws.com portUI=3009  
```  

Now open a browser and go to the URL below to start-up a twitter connected to the cloud spacebrew server. [Check Spacebrew admin](http://spacebrew.cc/master/spacebrew/admin/admin.html?server=ec2-184-72-140-184.compute-1.amazonaws.com) to make sure that the `new_tweets` client showed up.
``` 
http://localhost:3002/twitter/?server=ec2-184-72-140-184.compute-1.amazonaws.com&name=new_tweets&
```

##Configure Spacebrew via Admin
Go to the spacebrew admin at [`http://spacebrew.cc/master/spacebrew/admin/admin.html?server=ec2-184-72-140-184.compute-1.amazonaws.com`](http://spacebrew.cc/master/spacebrew/admin/admin.html?server=ec2-184-72-140-184.compute-1.amazonaws.com) to link the twitter app to the lamp. Make sure that the server setting in the query string (`server=ec2-184-72-140-184.compute-1.amazonaws.com`) is pointing to the appropriate Spacebrew Server.



