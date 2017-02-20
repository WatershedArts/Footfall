// * Name: TrackingManager.h
// * Project: Footfall
// * Author: David Haylock
// * Creation Date: 13/02/2017
// * Copyright: (c) 2017 by Watershed Arts Trust Ltd.

#ifndef TrackingManager_h
#define TrackingManager_h

#include <stdio.h>
#include "ofMain.h"
#include "ofxCv.h"
#include "Blob.h"
#include "Configuration.h"
#include "TrackingHistory.h"

using namespace cv;
using namespace ofxCv;

class TrackingManager
{
	public:
		//! Setup the tracker
		void setup(Tracking_Configuration _trackingConfig);
	
		//! Pass the Camera image into the tracker
		void update(Mat processedMat);
	
		//! Draw the Tracker
		void draw();
	
		//! Close the Tracker
		void close();
	
		ofEvent<int> blobIn;
		ofEvent<int> blobOut;
	
	private:
		ofxCv::ContourFinder contourFinder;
		ofxCv::RectTrackerFollower<Blob> tracker;
		ofRectangle centerRect;
	
		TrackingHistory trackingHistory;
		bool _flipVertically;
	
		int _oneBlob;
		int _twoBlob;
		int _threeBlob;
		int _historyLength;
		int _camerawidth;
		int _cameraheight;
};

#endif /* TrackingManager_h */
