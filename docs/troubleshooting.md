#Troubleshooting

Here are a couple of common issues and how to solve them.

-
###Issue 1: ofxCvPiCam

If you run into compiler errors with the [ofxCVPiCam](https://github.com/orgicus/ofxCvPiCam/) library on RPi2. Its because the MMAL Libraries have changed slightly. 

You can resolve these issues by following [this guide] (https://github.com/orgicus/ofxCvPiCam/issues/3). Or for the short version just rename the libs folder in the ofxCvPiCam folder.

<pre>
cd ~/openFrameworks/addons/ofxCvPiCam/
mv libs old-libs
</pre>


-
###Issue 2: ofxCv

There are some issues with ofxCv and later versions of openFrameworks.

You can resolve these issues by checking out a different branch of the repo.

If you have a version of oF which is less than 0.9.8, checkout the VS2012-static branch.

Otherwise use the origin/stable branch.