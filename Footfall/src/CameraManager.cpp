// * Name: CameraManager.cpp
// * Project: Footfall
// * Author: David Haylock
// * Creation Date: 13/02/2017
// * Copyright: (c) 2017 by Watershed Arts Trust Ltd.

#include "CameraManager.h"
//--------------------------------------------------------------
void CameraManager::setup(Camera_Configuration _cameraConfig)
{
	
	_useMask = _cameraConfig.bUseMask;
	
	// Check whether the mask should be generated
	if (_useMask)
	{
		// Get the Mask Coordinates
		for (int i = 0; i < _cameraConfig.maskCoord.size(); i++)
		{
			cv::Point p = cv::Point(_cameraConfig.maskCoord[i].x,_cameraConfig.maskCoord[i].y);
			_maskPts.push_back(p);
		}
		
		mask = cvCreateMat(_cameraConfig.cameraheight, _cameraConfig.camerawidth, CV_8UC1);
		combinedMask = cvCreateMat(_cameraConfig.cameraheight, _cameraConfig.camerawidth, CV_8UC1);
		
		// Fill the Mat with black
		for(int i=0; i<mask.cols; i++)
			for(int j=0; j<mask.rows; j++)
				mask.at<uchar>(cv::Point(i,j)) = 0;
		
		vector<cv::Point> polyright;
		approxPolyDP(_maskPts, polyright, 1.0, true);
		fillConvexPoly(mask,&polyright[0],polyright.size(),255,8,0);
	}
	
	// Setup the Background MOG2
	pMOG2 = new BackgroundSubtractorMOG2(_cameraConfig.history,
										 _cameraConfig.mogThreshold,
										 _cameraConfig.bTrackShadows
										 );
	// This is the ratio of the shadow detection. I.e how many times lighter the shadow needs to be to be considered a blob.
	pMOG2->setDouble("fTau", _cameraConfig.shadowPixelRatio);
	
#ifdef USE_VIDEO
	videoPlayer.load("yourtestfootage.mov");
	videoPlayer.setLoopState(OF_LOOP_NORMAL);
	videoPlayer.play();
#endif
	
#ifdef USE_WEBCAM
	videoGrabber.setVerbose(true);
	videoGrabber.setup(320,240);
#endif
	
#ifdef USE_PI_CAM
	piCamera.setup(_cameraConfig.camerawidth,_cameraConfig.cameraheight,true);
	piCamera.setFlips(_cameraConfig.bFlipH,_cameraConfig.bFlipV);
#endif
	
	
	_dilateAmount = _cameraConfig.dilateAmount;
	_erodeAmount = _cameraConfig.erodeAmount;
	_blur = _cameraConfig.blur;
}
//--------------------------------------------------------------
void CameraManager::update()
{
#ifdef USE_VIDEO
	videoPlayer.update();
	
	if (videoPlayer.isFrameNew())
	{
		copy(videoPlayer, videoMatrix);
	}
#endif

#ifdef USE_WEBCAM
	videoGrabber.update();
	
	if (videoGrabber.isFrameNew())
	{
		copy(videoGrabber, videoMatrix);
	}
#endif

#ifdef USE_PI_CAM
	videoMatrix = piCamera.grab();
#endif
	
	if (!videoMatrix.empty())
	{
		// Blur the original image
		GaussianBlur(videoMatrix, _blur);
		
		// Process the MOG
		if (_useMask)
		{
			videoMatrix.copyTo(combinedMask, mask);
			pMOG2->operator()(combinedMask,processedMog);
		}
		else
		{
			pMOG2->operator()(videoMatrix,processedMog);
		}
		
		// Get the background image
		pMOG2->getBackgroundImage(background);
		
		// Copy the original MOG to the unprocessed Mat before processing
		// copy(processedMog,unprocessed_MOG); Debug
		
		// Image processing
		threshold(processedMog,_threshold);
		blur(processedMog,_blur);
		dilate(processedMog,_dilateAmount);
		erode(processedMog,_erodeAmount);
		dilate(processedMog,_dilateAmount);
		GaussianBlur(processedMog, _blur*2);
		
		// Leave these two
		threshold(processedMog,50);
		erode(processedMog,2);
	}
}
//--------------------------------------------------------------
void CameraManager::draw()
{
	ofPushMatrix();
	ofSetColor(255, 255, 255);
	drawMat(videoMatrix, 0, 0,320,240);
	drawMat(processedMog, 320, 0,320,240);
	drawMat(background, 320*2, 0,320,240);
	ofDrawBitmapStringHighlight("Input Image", 160-45,255);
	ofDrawBitmapStringHighlight("Processed Image", (160*3)-65,255);
	ofDrawBitmapStringHighlight("Background", (160*5)-45,255);
	ofPopMatrix();
}
//--------------------------------------------------------------
Mat CameraManager::getImage()
{
	return processedMog;
}
