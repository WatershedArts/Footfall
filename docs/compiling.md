##Compiling

This guide assumes that you have compiled openFrameworks and it is in the /home/pi directory.

###Simple Compiling
First Download the project folder to the apps/ folder.

``cd openFrameworks/apps/``

Clone the repo.

`` git clone https://github.com/WatershedArts/Footfall.git``

Move into the project folder.

``cd Footfall/Footfall/``

Compile.

`` make -j3``

Then you are ready to run the app.
Either:

<pre>DISPLAY=:0 make RunRelease
of
DISPLAY=:0 bin/Footfall
</pre>

###Building the App Folder
Alternatively, if you run into compiling errors, try generating a new project folder and transfer the source files across.

``cd openFrameworks/apps/myApps/ ``

``cp -r emptyExample/ Footfall/``

``cd Footfall``

``mkdir bin``

``mkdir bin/data``

``pico addons.make``

Add the following on new lines

```
ofxCv
ofxCsv
ofxOpenCv
ofxJSON
ofxHttpUtils
ofxCvPiCam
```
``CTRL + x`` 

To save

Navigate to the src folder of the Footfall app

``cd openFrameworks/apps/myApps/Footfall/src``

Copy all of the Source files into the folder.

``cp ../../../../Footfall/Footfall/src/* .``

Now you should be ready to compile.

``cd ..``

Go up one directory.

``make -j3`` 

This will take a bit of time.

Once it has compiled, you'll need to move the config.json file into the data folder.

``cp ../../../../Footfall/Footfall/bin/data/config.json ./bin/data/``


If there are errors with ofxCvPiCam. 
Enter the addon directory and move the libs folder. 

``mv libs/ old-libs/``

This will hide the libs folder and stop the compilier from using them.

