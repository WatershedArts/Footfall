# Troubleshooting

Here are a couple of common issues and how to solve them.


### Issue 1: ofxCvPiCam

If you run into compiler errors with the [ofxCVPiCam](https://github.com/orgicus/ofxCvPiCam/) library on RPi2. Its because the MMAL Libraries have changed slightly. 

You can resolve these issues by following [this guide] (https://github.com/orgicus/ofxCvPiCam/issues/3). Or for the short version just rename the libs folder in the ofxCvPiCam folder.

```
cd ~/openFrameworks/addons/ofxCvPiCam/
mv libs old-libs
```

### Issue 2: ofxCv

There are some issues with ofxCv and later versions of openFrameworks. You can resolve these issues by checking out a different branch of the repo. 

*  First remove the current installation of ofxCv: 
  ```
  cd ~/openFrameworks/addons
  sudo rm -r ofxCv 
  ```
* Clone a fresh copy of ofxCv

  ```
  git clone https://github.com/kylemcdonald/ofxCv.git
  ```
* Checkout branch according to oF version: 
  * oF version < 0.9.8, checkout the VS2012-static branch.
  * oF version = 0.9.8, checkout stable. 


### Issue 3: glm:: errors in compile

Using oF 0.9.8 version's stable branch will negate the glm:: errors. If you were following oF's download guide previously, please remove the `opensourceFrameworks` folder as this is a fresh installation of oF. 

```
git clone -b stable --single-branch https://github.com/openframeworks/openFrameworks.git
```

Follow the same instructions of building and compiling oF. No need to install dependencies for oF again if you had previously installed oF. Then follow the instructions the [same instructions to build Footfall](https://github.com/WatershedArts/Footfall/blob/master/docs/rpi.md). 
