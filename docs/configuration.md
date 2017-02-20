##Configuration

Configuring Footfall is a labour intensive task. Hopefully this guide will make it a little bit easier.

###Positioning Footfall

Positioning Footfall is important. Ideally you would deploy it where there are only two directions of entry. For example, over a doorway or above a corridor. You do not want people to cut in from the side of camera. See the example footage given with this app.

###First Steps

**I would recommend placing your camera above the capture area and recording a minute long video of people passing through.** 

``raspivid -w 320 -h 240 -fps 25 -t 60000 -o testRecording.h264``

You'll need to convert the video.

``ffmpeg -i testRecording.h264 -c copy output.mp4``

On your desktop compile the footfall app. Commenting USE_PI_CAM and uncommenting USE_VIDEO. 

Place the converted video into the data directory of the app. Add the videos string to Line 49 of CameraManager.cpp.

The build and run. Alter the settings until you get the desired actions.

You'll then be able to copy the config file to the RPiVersion.

###Config File
--
Here are the config options you can alter.

**App Config**

| Variable Name | Description |
|---|---|
| "usehttp" | Enables the app to POST data to a server. |
| "usecsvlogging" | Enables Csv Recording.  |

**Camera Config**

| Variable Name | Description | Default Value |
|---|---|---|
| "camerawidth"| Width of the Camera. | 160 |
| "cameraheight"| Height of the Camera. | 120 |
| "fliphorizontally"| Flip the Camera View Horizontally. | false |
| "flipvertically"| Flip the Camera View Vertically. | false |
| "trackshadows"| Decides whether the MOG2 Background Subtractor finds and keeps shadows. | false |
| "shadowpixelratio" | Sets fTau ratio, i.e how much darker the pixels need to be to be considered a shadow. | 0.5 |
| "mogthreshold"| The MOG's threshold. | 16 |
| "threshold"| Threshold value for the processed Mog image. | 175 |
| "dilate" | Dilate the image.  | 5 (should not be more the 10) |
| "erode" | Erode the image.  | 3 (should not be more the 10) |
| "history"| How many frames the MOG uses for its model. | 200 |
| "blur"| Blur Value. | 5 |
| "usemask"| Flag to tell the app to use a mask on the camera image. This is useful for light sources and other unwanted items in the cameras view. | true |
| "showshadowimage"| Flag to render the unprocessed MOG image| true |
| "MaskArea"| Series of coordinates that make a mask. | See Below |

Example Coordinates

<pre>{"coordx":10,"coordy":10},
{"coordx":150,"coordy":10},
{"coordx":150,"coordy":110},
{"coordx":10,"coordy":110}
</pre>

**Tracking Config**

| Variable Name | Description | Default Value |
|---|---|---|
|"threshold"| The threshold of the contourFinder | 150 |
|"minarea"| The min radius size the contourFinder can detect | 10 |
|"maxarea"| The max radius size the contourFinder can detect | 150 |
|"startposx"| The starting x position of the evaluation box | 0 |
|"startposy"| The starting y position of the evaluation box | Generically, its best to be half of the Camera's Height |
|"offset"| How big the evalutation box is in pixels | 40 |
|"blobdyingtime"| The amount of time the blob takes to be deleted | 0.5 |
|"persistance"| *Determines how many frames an object can last without being seen until the tracker forgets about it. | 1 |
|"maxdistance"| *Determines how far an object can move until the tracker considers it a new object. | 100 |
|"minsizeone"| Is the smallest the size a person can be considered | 10 |
|"minsizetwo"| If the blob is over this size, the blob is considered to be two people | 50 |
|"minsizethree"| If the blob is over this size, the blob is considered to be three people | 150 |


**Http Config**

| Variable Name | Description | Default Value |
|---|---|---|
|"keepbackups"| If the network is down, the application will store the data in a csv file and upload the contents next time the network is connected. | false |
|"postserver"| The server's address | http://www.yoursite.co.uk or http://localhost |
|"postextension"| The php file the app will be POSTing to | upload.php |
|"secretkey"| A secret key that stops unwanted data being posted  | randomstring |
|"maxretries"| How many times the post will be sent if failed | 1 


*From KyleMcDonald's [Documentation of the ofxCv Tracker Class](https://github.com/kylemcdonald/ofxCv/blob/master/libs/ofxCv/include/ofxCv/Tracker.h#L4)
