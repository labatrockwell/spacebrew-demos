Proximity Demo
---------------

This demo shows how spacebrew can be used to connect input from the physical world to actions in the virtual world. 

##Materials Requirements
* Computer with Chrome browser
* Furniture Sensing Kit, including
	* Data Arduino hooked-up with MPR121 capacitive controller, and 12 electrodes
	* Ethernet Arduino hooked-up to data Arduino to publish data to spacebrew
* Power source 12v, 0.5a

##Figure Out How Arduino Will Connect to Spacebrew  
  
###Configure Arduino for Connecting Via Ethernet using DHCP  
Make following updates to `lamp' sketch and re-upload:  
* Update the MAC address if necessary  
* Configure spacebrew server if not using amazon cloud server.

###Configure Arduino Connecting Via Ethernet using Static IP
Make following updates to `lamp_static' sketch and upload:  
* Update the MAC address if necessary  
* Configure spacebrew server if not using amazon cloud server.  
* Set the ip address (subnet and gateway as well if necessary).  

###Connect Arduino to Ethernet and power brick.   

## Load and Prep the JS Slideshow App

###Load Slideshow JS App  
The slideshow js app accepts image urls and adds them to a slideshow. It also accepts boolean values to play/pause the slide show, and go to next or previous image. Lastly, it accepts a range to set the speed of the slideshow. 

[Load slideshow app](http://labatrockwell.github.com/spacebrew-javascript-examples/spacebrew_slideshow/index.html?server=ec2-184-72-140-184.compute-1.amazonaws.com&name=slideshow) connecting to the cloud server and using the app name `slideshow`.

Slideshow app URL: http://labatrockwell.github.com/spacebrew-javascript-examples/spacebrew_slideshow/index.html?server=ec2-184-72-140-184.compute-1.amazonaws.com&name=slideshow  

###Load String App to Load Images into Slideshow  
[Load string app](http://labatrockwell.github.com/spacebrew-javascript-examples/spacebrew_string/index.html?server=ec2-184-72-140-184.compute-1.amazonaws.com&name=image_names) connecting to the cloud server and using the app name `image_names`.

String app URL: http://labatrockwell.github.com/spacebrew-javascript-examples/spacebrew_string/index.html?server=ec2-184-72-140-184.compute-1.amazonaws.com&name=image_names  

Image urls that can be uploaded to slideshow:  
* http://3.bp.blogspot.com/-enR9s5-dntk/TxBMrM_wIsI/AAAAAAAABqc/Hp-15qFSDHw/s1600/120113_jamescameron_filmingonboat_v4.jpg
* http://fatcatart.ru/wp-content/uploads/2012/12/Hokusai-Great_Wave_off_Kanagawa-cat-end.jpg
* http://3.bp.blogspot.com/-1CxJVzqW_AI/TrPx68OczFI/AAAAAAAABnw/liSNOOdtC98/s1600/111104_occupyBallStreet14.jpg
* http://fatcatart.ru/wp-content/uploads/2013/01/Adriaen-van-Utrecht-Still-life-cat-w.jpg
* http://3.bp.blogspot.com/-VOkuD0KkWgY/TkS0J5GWZdI/AAAAAAAABkE/UcddkM__Ebs/s1600/110812_HorseTakesCrimeTour15.jpg
* http://fatcatart.ru/wp-content/uploads/2012/06/Vermeer-the-milkmaid-cat.jpg
* http://4.bp.blogspot.com/-PzQnRiiTh_M/Tur2IYcZIsI/AAAAAAAABpA/nl8IhDRsaEI/s1600/111216_sodaClausComesToTown12.jpg
* http://3.bp.blogspot.com/-1CxJVzqW_AI/TrPx68OczFI/AAAAAAAABnw/liSNOOdtC98/s1600/111104_occupyBallStreet14.jpg
  
###Load Button App to Pause the Slideshow  
[Load button app](http://labatrockwell.github.com/spacebrew-javascript-examples/spacebrew_button/index.html?server=ec2-184-72-140-184.compute-1.amazonaws.com&name=play_pause) connecting to the cloud server and using the app name `play_pause`.

Button app URL: http://labatrockwell.github.com/spacebrew-javascript-examples/spacebrew_button/index.html?server=ec2-184-72-140-184.compute-1.amazonaws.com&name=play_pause  

##Configure Connections Spacebrew via Admin
Go to the spacebrew admin at [`http://spacebrew.cc/master/spacebrew/admin/admin.html?server=ec2-184-72-140-184.compute-1.amazonaws.com`](http://spacebrew.cc/master/spacebrew/admin/admin.html?server=ec2-184-72-140-184.compute-1.amazonaws.com) to link the apps. Make sure that the server setting in the query string (`server=ec2-184-72-140-184.compute-1.amazonaws.com`) is pointing to the appropriate Spacebrew Server.



