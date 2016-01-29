#Footfall

###Introduction

Footfall is a camera based people counting system that uses a  Raspberry Pi and Pi Cam. There is more information about the system on our [blog](http://blogs.wcode.org/2015/04/footfall-a-camera-based-people-counting-system-for-under-60/).

The source code provides a barebones system that will require some customisation.

**Disclaimer**:
The original software was intended for sole use within Watershed, therefore some of the source code has been altered for public use and differs slightly to our systems. For example our system generated event tags showing screening in conjunction with the total number of people in Watershed, to do this we had to pre-populate some timestamps and may cause an issue if the system is ran past a certain time.

###How to Build
You will need a few extra packages to build the RPi application.
Copy the getrepos.sh file and place it inside the addons folder of your openFrameworks folder. Run the script by navigating to the addons folder and entering the following command into Terminal `./getrepos.sh`. This will pull down the require packages.

There are two versions of Footfall, one for armv6l and one for armv7l.

Move the Application directory from the Repo into openFrameworks/apps/myApps/

and run 

`make clean`

Then ensure you have pulled the latest versions of the Addons using the script above.

For armv7l you will need to ensure your pi is up to date and that the camera enabled in raspi-config.

Please run the following.

`sudo apt-get update`

`sudo apt-get upgrade`

`sudo raspi-update`

`sudo raspi-config`

Enable Camera

and Reboot.

Test the camera is working. 

```raspistill -o testimage.jpg```

##FIX: ofxCvPiCam
A couple of people have run into compiler errors with the [ofxCVPiCam](https://github.com/orgicus/ofxCvPiCam/) library on RPi2. 

It looks like the MMAL Libraries may have changed slightly so are causing strange errors.

You can resolve these by following [this guide] (https://github.com/orgicus/ofxCvPiCam/issues/3).

```
cd ~/openFrameworks/addons/ofxCvPiCam/
mv libs old-libs
```
----

You will also need to update your Pi

`sudo apt-get update`

`sudo apt-get upgrade`

`sudo raspi-update`

Assuming you have [built and compiled oF](http://forum.openframeworks.cc/t/raspberry-pi-2-setup-guide/18690).

Create a new app your openFrameworks apps/myApps directory then copy the source code across, including the addons.make file.

To build the app.

* ```cd projectFolder```
* ```make```

If you are using a mouse and keyboard.
You can either launch the app by
* ```make run```
or
* bin/projectName

However, if you are sshing into the pi.
Launch the app like so.

* ```DISPLAY=:0 make run & disown```
or
* ```DISPLAY=:0 bin/projectName & disown```
This puts the app in the background.

###How to Customise
Inside the bin/data folder of the RPi App there is a config.xml file. This contains all of the relevant variables you will need to change.

Inside the <UPLOADURL> tag put your upload.php url this will fire the events into the database.
The other settings will need tweaking depending on where you place the tracker.

###Thanks
Thanks to Kyle McDonald for [ofxCv](http://github.com/kylemcdonald/ofxCv) and George Profenza for [ofxCvPiCam] (https://github.com/orgicus/ofxCvPiCam).
