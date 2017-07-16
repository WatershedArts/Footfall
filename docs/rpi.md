# Raspberry Pi


### How to setup the Raspberry Pi

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

Then ensure you have pulled the latest versions of the addons using the script above.

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
