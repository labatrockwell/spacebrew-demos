JS Slider Demo
---------------

This demo shows how spacebrew can be used to connect two web pages. 

##Materials Requirements
* Computer with Chrome browser
  
###Load Slider JS App  
The slider js app features two sliders with a range from 0 to 1024
[Load the slider app](http://labatrockwell.github.com/spacebrew-javascript-examples/spacebrew_gui/index.html?server=ec2-184-72-140-184.compute-1.amazonaws.com&name=sliders) connecting to the cloud server and using the app name `slider`.

Slider app URL: http://labatrockwell.github.com/spacebrew-javascript-examples/spacebrew_gui/index.html?server=ec2-184-72-140-184.compute-1.amazonaws.com&name=sliders  

###Load Graph JS App  
[Load the graph app](http://labatrockwell.github.com/spacebrew-javascript-examples/spacebrew_graph/index.html?server=ec2-184-72-140-184.compute-1.amazonaws.com&name=graph) connecting to the cloud server and using the app name `graph`.

Graph app URL: http://labatrockwell.github.com/spacebrew-javascript-examples/spacebrew_graph/index.html?server=ec2-184-72-140-184.compute-1.amazonaws.com&name=graph  

###Load Color JS App  
[Load the color app](http://labatrockwell.github.com/spacebrew-javascript-examples/spacebrew_colored_screen/index.html?server=ec2-184-72-140-184.compute-1.amazonaws.com&name=colors) connecting to the cloud server and using the app name `colors`. Note that the `sat` and `light` inlets are flipped.

Graph app URL: http://labatrockwell.github.com/spacebrew-javascript-examples/spacebrew_colored_screen/index.html?server=ec2-184-72-140-184.compute-1.amazonaws.com&name=colors  

##Configure Connections Spacebrew via Admin
Go to the spacebrew admin at [`http://spacebrew.cc/master/spacebrew/admin/admin.html?server=ec2-184-72-140-184.compute-1.amazonaws.com`](http://spacebrew.cc/master/spacebrew/admin/admin.html?server=ec2-184-72-140-184.compute-1.amazonaws.com) to link the apps. Make sure that the server setting in the query string (`server=ec2-184-72-140-184.compute-1.amazonaws.com`) is pointing to the appropriate Spacebrew Server.



