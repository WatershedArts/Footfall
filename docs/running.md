## Running the App

If you are sshing into the pi.
Launch the app like so.

* ```DISPLAY=:0 make run & disown```
or
* ```DISPLAY=:0 bin/Footfall & disown```

This puts the app in the background.

Or if you want to launch the app automatically, use crontab.

``crontab -e ``

```
00 07	*	*	*	* /home/pi/startFootfall.sh
59 23	*	*	*	* /home/pi/stopFootfall.sh

00	03 *	*	*	* /sbin/reboot
```

``CTRL + x`` to save

### Scripts
Create these scripts in your **/home/pi/** directory.

```
touch startFootfall.sh stopFootfall.sh
chmod 755 startFootfall.sh stopFootfall.sh
```

**startFootfall.sh**
```
#!/bin/sh
cd ~/openFrameworks/apps/myApps/Footfall/bin/ 
ret=1
while [ $ret -ne 0 ]; do
    ./Footfall
    ret=$?
done
```

**stopFootfall.sh**
```
#!/bin/sh
myarr=$(ps -ef | pgrep 'Footfall')
for i in $myarr
do 
	kill -9 $i
done
```
