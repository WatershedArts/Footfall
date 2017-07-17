# Raspberry Pi Setup 

This guide is tested on RPI 1,2 & 3. 

### How to setup the Raspberry Pi

* Download a Noobs Package from [here](https://www.raspberrypi.org/downloads/noobs/).
* Install Jessie. And wait.
* Once the sd card is ready and the system has rebooted.
*   ``sudo raspi-config`` to open the Raspberry Pi Config screen. 
 * Expand the filesytem and reboot.

## Install openSourceFrameworks
[Build and compile oF](http://forum.openframeworks.cc/t/raspberry-pi-2-setup-guide/18690).

## Installing dependencis for Footfall 
You will need a few extra packages to build the RPi application.

* Copy the `getrepos.sh` [file's data](https://github.com/WatershedArts/Footfall/blob/master/getrepos.sh) and place it inside the addons folder of your openFrameworks folder. 

  ```
  cd ~/openFrameworks/addons
  touch getrepos.sh && vim getrepos.sh
  ```
This opens up vim editor for editing getrepos.sh. Press <kbd>A</kbd> for Insert and paste the contents from the link. Press <kbd>ESC</kbd> then <kbd>SHIFT</kbd>+<kbd>:</kbd>. Type wq and then press enter to save the file. 

* Run the script by:  `./getrepos.sh`. 

This will pull down the required packages. Then ensure you have pulled the latest versions of the addons using the script above.

* Please run the following.

  ```
  sudo apt-get update
  sudo apt-get upgrade
  sudo raspi-update
  ```
  
* Enable Camera by `sudo raspi-config` and Reboot.

Assuming you have [built and compiled oF](http://forum.openframeworks.cc/t/raspberry-pi-2-setup-guide/18690).

* Test the camera is working by ``raspistill -o testimage.jpg``. Open testimage.jpg to check the output. 

