//
// * Name: TrackingHistory.h
// * Project: Footfall
// * Author: David Haylock
// * Creation Date: 13/02/2017
// * Copyright: (c) 2017 by Watershed Arts Trust Ltd.

#ifndef TrackingHistory_h
#define TrackingHistory_h

#include "ofMain.h"

struct TrackingData
{
	TrackingData(int _size,bool _direction)
	{
		size = _size;
		direction = _direction;
	}
	
	int size;
	bool direction;
};

class TrackingHistory
{
	public:
		//--------------------------------------------------------------
		void setup(int minsize,int midsize, int maxsize,int startposy)
		{
			_minsize = minsize;
			_midsize = midsize;
			_maxsize = maxsize;
			_startposy = startposy;
			trackingHistory.clear();
		}
	
		//--------------------------------------------------------------
		void addNewData(int size,bool direction)
		{
			trackingHistory.push_front(TrackingData(size,direction));
		}
	
		//--------------------------------------------------------------
		void draw(int width,int height)
		{
			if(trackingHistory.size() > 15)
			{
				trackingHistory.pop_back();
			}
			
			ofPushStyle();
			ofSetLineWidth(2);
			// Draw the Centre Line
			ofSetColor(ofColor::yellow);
			ofDrawLine(0,_startposy,width,_startposy);
			
			// Draw the Centre X
			ofSetColor(ofColor::yellow);
			ofDrawLine(width*0.5,_startposy-5,width*0.5,_startposy+5);
			
			// Draw Min Size Guide
			ofSetColor(ofColor::cyan);
			ofDrawLine((width*0.5)-(_minsize/2),_startposy-10,(width*0.5)-(_minsize/2),_startposy+10);
			ofDrawLine((width*0.5)+(_minsize/2),_startposy-10,(width*0.5)+(_minsize/2),_startposy+10);
			
			// Draw Mid Size Guide
			ofSetColor(ofColor::purple);
			ofDrawLine((width*0.5)-(_midsize/2),_startposy-10,(width*0.5)-(_midsize/2),_startposy+10);
			ofDrawLine((width*0.5)+(_midsize/2),_startposy-10,(width*0.5)+(_midsize/2),_startposy+10);
			
			// Draw Max Size Guide
			ofSetColor(ofColor::limeGreen);
			ofDrawLine((width*0.5)-(_maxsize/2),_startposy-10,(width*0.5)-(_maxsize/2),_startposy+10);
			ofDrawLine((width*0.5)+(_maxsize/2),_startposy-10,(width*0.5)+(_maxsize/2),_startposy+10);
			ofPopStyle();
			
			// Draw Size Guides these are in 25pixel increments.
			int division = width/50;
			for (int i = 0; i < division; i++)
			{
				ofSetLineWidth(1);
				ofSetColor(ofColor::white);
				ofDrawLine((width*0.5)-(((1+i)*50)/2),_startposy-5,(width*0.5)-(((1+i)*50)/2),_startposy+5);
				ofDrawLine((width*0.5)+(((1+i)*50)/2),_startposy-5,(width*0.5)+(((1+i)*50)/2),_startposy+5);
			}
			
			ofPushStyle();
			ofSetLineWidth(2);
			for(int i = 0; i < trackingHistory.size(); i++)
			{
				if (trackingHistory[i].direction)
				{
					ofSetColor(ofColor::green);
				}
				else
				{
					ofSetColor(ofColor::red);
				}
				ofDrawLine((width*0.5)-(trackingHistory[i].size/2),_startposy+(i*5),(width*0.5)+(trackingHistory[i].size/2),_startposy+(i*5));
			}
			ofPopStyle();
		}
	
	
	private:
		deque <TrackingData> trackingHistory;
		int _minsize;
		int _midsize;
		int _maxsize;
		int _startposy;
	
};

#endif /* TrackingHistory_h */
