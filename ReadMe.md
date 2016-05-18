##Footfall


#Update 1


**I have added some new documentation about how to create the server side system  database etc... There is also a new guide about how to setup the raspberry pi as a server.**

**I will be updating this repo with more detailed instructions and improved tracking over the coming weeks.**

###Warning

The original software was intended for sole use within **[Watershed](http://www.watershed.co.uk)**, therefore some of the source code has been altered for public use and differs slightly to our systems. For example our system generated event tags showing screening in conjunction with the total number of people in Watershed, to do this we had to pre-populate some timestamps and may cause an issue if the system is ran past a certain time.

#Introduction

Footfall is a camera based people counting system that uses a  Raspberry Pi and Pi Cam. There is more information about the system on our [blog](http://blogs.wcode.org/2015/04/footfall-a-camera-based-people-counting-system-for-under-60/).

**Important**: The source code provides a barebones system that **will require some** customisation for each installation.

###Documentation
Find attached the relevant readmes to help build Footfall system.

* [Raspberry Pi](./docs/rpi.md)
* [Server](./docs/server.md)
* [Troubleshooting](./docs/troubleshooting.md)

###How to Customise
Inside the bin/data folder of the RPi App there is a config.xml file. This contains all of the relevant variables you will need to change.

Inside the <UPLOADURL> tag put your upload.php url this will fire the events into the database.
The other settings will need tweaking depending on where you place the tracker.

The ThreadedFootfall example uses JSON for its config.

###Thanks
Thanks to Kyle McDonald for [ofxCv](http://github.com/kylemcdonald/ofxCv) and George Profenza for [ofxCvPiCam] (https://github.com/orgicus/ofxCvPiCam).
