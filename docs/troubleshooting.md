#Troubleshooting

Here are a couple of common issues and how to solve them.



-
###ofxCvPiCam
If you run into compiler errors with the [ofxCVPiCam](https://github.com/orgicus/ofxCvPiCam/) library on RPi2. Its because the MMAL Libraries have changed slightly. 

You can resolve these issues by following [this guide] (https://github.com/orgicus/ofxCvPiCam/issues/3). Or for the short version just rename the libs folder in the ofxCvPiCam folder.

```
cd ~/openFrameworks/addons/ofxCvPiCam/
mv libs old-libs
```
-