// * Name: Blob.h
// * Project: Footfall
// * Author: David Haylock
// * Creation Date: 13/02/2017
// * Copyright: (c) 2017 by Watershed Arts Trust Ltd.

#ifndef Blob_h
#define Blob_h

#include <stdio.h>
#include "ofMain.h"
#include "ofxCv.h"

class Blob : public ofxCv::RectFollower
{
	public:
		//--------------------------------------------------------------
		void setup(const cv::Rect &track)
		{
			_currentPos = ofxCv::toOf(track).getCenter();
			_originPos = ofxCv::toOf(track).getCenter();
			_evaluating = false;
		}
		//--------------------------------------------------------------
		void update(const cv::Rect &track)
		{
			_currentPos = ofxCv::toOf(track).getCenter();
			_blobWidth = ofxCv::toOf(track).width;
			_blobHeight = ofxCv::toOf(track).height;
		}
		//--------------------------------------------------------------
		void draw()
		{
			ofPushStyle();
			
			if(_evaluating) {
				ofSetColor(255);
			}
			else {
				ofSetColor(0,255,0);
			}
			
			ofDrawCircle(_currentPos, 5);
			ofSetColor(255,0,0);
			ofDrawCircle(_originPos, 5);
			ofPopStyle();
		}
		//--------------------------------------------------------------
		void kill()
		{
			dead = true;
		}
	
		//--------------------------------------------------------------
		ofVec2f getCurrentPosition()
		{
			return _currentPos;
		}
	
		//--------------------------------------------------------------
		ofVec2f getOriginPosition()
		{
			return _originPos;
		}
	
		//--------------------------------------------------------------
		int getWidth()
		{
			return _blobWidth;
		}
	
		bool _evaluating = false;
	private:
		ofVec2f _currentPos;
		ofVec2f _originPos;
		int _blobWidth = 0;
		int _blobHeight = 0;
};


#endif /* Blob_h */
