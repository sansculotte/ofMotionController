Video Motion Controller
=======================

This openframeworks[1] app runs motion detection on a grid cast
over the camera input and sends the motion parameters
to an OSC channel.

Initially developed at an openFrameworks-workshop at Electrolobby at ars electronica 2006
build tested with 0.7 and 0.8 on linux. older verisons built on OSX but can't remember
the details.

It's easy to hook it up to supercollider[2], but you have to change the OSC port
to 57120.

Build
-----

You need openframework installed and the ofxOSC addon installed.

- clone into OFHOME/apps/myApps/
- make
- make run

Configuration
-------------

There is a realtime sensitivity setting interface using the 
mousepointer's relative position on the "control monitor".
further up is more sensitive.

constants for OSC host and port are defined in src/testApp.h

screen size, grid size and fade factor can be tweaked in src/testApp.cpp



brought to you by sansculotte [3]

References
----------

   [1]: http://openframeworks.cc
   [2]: https://supercollider.github.io
   [3]: http://sansculotte.net
