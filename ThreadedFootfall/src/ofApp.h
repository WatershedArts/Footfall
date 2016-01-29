#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "Tracking.h"
#include "LoadConfig.h"
#include "HTTPHandler.h"
#include "ofxCvPiCam.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
	        void exit();
    
		void keyPressed(int key);
		void keyReleased(int key);
		
        Tracking tracking;
        LoadConfig config;
        HTTPHandler httpHandler;
    	ofxCvPiCam piCam;
        //ofVideoGrabber videoGrabber;
        Mat camera;
        Mat lightenMat;
        Mat resized;
        Mat processedMog;
    
        int _lightenAmount;
        BackgroundSubtractorMOG2* pMOG2;
    
        void peopleGoingIn(string &count);
        void peopleGoingOut(string &count);
};
