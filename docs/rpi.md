#Raspberry Pi


##How to Build

Download a Noobs Package from [here](https://www.raspberrypi.org/downloads/noobs/).

Install Jessie.

And wait.

Once the sd card is ready and the system has rebooted.

open the Raspberry Pi Config screen

````sudo raspi-config````

and expand the filesytem and reboot.

You will need a few extra packages to build the RPi application.

Copy the getrepos.sh file and place it inside the addons folder of your openFrameworks folder. 

Run the script by navigating to the addons folder and entering the following command into Terminal 

`./getrepos.sh`. 

This will pull down the required packages.

There are three versions of Footfall.

* 1 x Armv6l for Raspberry Pi 1 running Wheezy
* 1 x Armv7l for Raspberry Pi 2 running Jessie
* 1 x Armv7l for Raspberry Pi 2 running Jessie using Threading **(1)**

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