// * Name: TrackingManager.cpp
// * Project: Footfall
// * Author: David Haylock
// * Creation Date: 13/02/2017
// * Copyright: (c) 2017 by Watershed Arts Trust Ltd.

#include "TrackingManager.h"
//--------------------------------------------------------------
void TrackingManager::setup(Tracking_Configuration _trackingConfig)
{
	contourFinder.setMinAreaRadius(_trackingConfig.minarea);
	contourFinder.setMaxAreaRadius(_trackingConfig.maxarea);
	contourFinder.setThreshold(_trackingConfig.threshold);
	tracker.setMaximumDistance(_trackingConfig.maxdistance);
	tracker.setPersistence(_trackingConfig.persistance);

	_oneBlob = _trackingConfig.minsizeone;
	_twoBlob = _trackingConfig.minsizetwo;
	_threeBlob = _trackingConfig.minsizethree;
	_flipVertically = _trackingConfig.flipvertically;
	_historyLength = _trackingConfig.history;
	_camerawidth = _trackingConfig.camerawidth;
	_cameraheight = _trackingConfig.cameraheight;
	
	trackingHistory.setup(_oneBlob,_twoBlob,_threeBlob,_trackingConfig.startPos.y);
	
	centerRect = ofRectangle(0, _trackingConfig.startPos.y-(_trackingConfig.offset/2), _camerawidth, _trackingConfig.offset);
}
//--------------------------------------------------------------
void TrackingManager::update(Mat processedMat)
{
	if(!processedMat.empty())
	{
		if (ofGetFrameNum() > _historyLength)
		{
			contourFinder.findContours(processedMat);
			tracker.track(contourFinder.getBoundingRects());
		}
	}
	
	vector<Blob> &blobs = tracker.getFollowers();
	for(int i = 0; i < blobs.size(); i++)
	{
		if (centerRect.inside(blobs[i].getCurrentPosition().x, blobs[i].getCurrentPosition().y) && !blobs[i]._evaluating)
		{
			if (_flipVertically)
			{
				if (blobs[i].getCurrentPosition().y > blobs[i].getOriginPosition().y)
				{
					int noOfBlobs = 0;
					if (blobs[i].getWidth() > _threeBlob)
					{
						noOfBlobs = 3;
					}
					else if (blobs[i].getWidth() > _twoBlob)
					{
						noOfBlobs = 2;
					}
					else if (blobs[i].getWidth() > _oneBlob)
					{
						noOfBlobs = 1;
					}
					
					
					
					trackingHistory.addNewData(blobs[i].getWidth(), true);
					ofNotifyEvent(blobIn, noOfBlobs, this);
					blobs[i].kill();
				}
				else if (blobs[i].getCurrentPosition().y < blobs[i].getOriginPosition().y)
				{
					int noOfBlobs = 0;
					if (blobs[i].getWidth() > _threeBlob)
					{
						noOfBlobs = -3;
					}
					else if (blobs[i].getWidth() > _twoBlob)
					{
						noOfBlobs = -2;
					}
					else if (blobs[i].getWidth() > _oneBlob)
					{
						noOfBlobs = -1;
					}
					
					trackingHistory.addNewData(blobs[i].getWidth(), false);
					ofNotifyEvent(blobOut, noOfBlobs, this);
					blobs[i].kill();
				}
			}
			else {
				if (blobs[i].getCurrentPosition().y < blobs[i].getOriginPosition().y)
				{
					int noOfBlobs = 0;
					if (blobs[i].getWidth() > _threeBlob)
					{
						noOfBlobs = 3;
					}
					else if (blobs[i].getWidth() > _twoBlob)
					{
						noOfBlobs = 2;
					}
					else if (blobs[i].getWidth() > _oneBlob)
					{
						noOfBlobs = 1;
					}
					
					trackingHistory.addNewData(blobs[i].getWidth(), true);
					ofNotifyEvent(blobIn, noOfBlobs, this);
					blobs[i].kill();
				}
				else if (blobs[i].getCurrentPosition().y > blobs[i].getOriginPosition().y)
				{
					int noOfBlobs = 0;
					if (blobs[i].getWidth() > _threeBlob)
					{
						noOfBlobs = -3;
					}
					else if (blobs[i].getWidth() > _twoBlob)
					{
						noOfBlobs = -2;
					}
					else if (blobs[i].getWidth() > _oneBlob)
					{
						noOfBlobs = -1;
					}
					
					trackingHistory.addNewData(blobs[i].getWidth(), false);
					ofNotifyEvent(blobOut, noOfBlobs, this);
					blobs[i].kill();
				}
			}
			
			blobs[i]._evaluating = true;
		}
		else if(blobs[i]._evaluating)
		{
			blobs[i]._evaluating = false;
		}
	}
}
//--------------------------------------------------------------
void TrackingManager::draw()
{
	ofSetLineWidth(1);
	ofFill();
	vector<Blob> &followers = tracker.getFollowers();
	for(int i = 0; i < followers.size(); i++)
	{
		followers[i].draw();
	}
	ofSetColor(ofColor::red);
	contourFinder.draw();
	
	ofNoFill();
	ofSetColor(255, 255, 255);
	ofDrawRectangle(centerRect);
	
	trackingHistory.draw(_camerawidth,_cameraheight);
}
