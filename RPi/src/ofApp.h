// * Name: ofApp.h
// * Project: Footfall
// * Author: David Haylock
// * Creation Date: 20-11-2014
// * Copyright: (c) 2015 by Watershed Arts Trust Ltd.

#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include <stdio.h>
#include "ofxHttpUtils.h"
#include "Blob.h"
#include "ofxCvPiCam.h"
#include "ofxXmlSettings.h"

using namespace wng;
using namespace ofxCv;
using namespace cv;

//--------------------------------------------------------------
class myLine {
public:
    int width;
    void draw(int x, int y)
    {
        ofLine(x,y,x-width/2,y);
        ofLine(x,y,x+width/2,y);
    }
};

//--------------------------------------------------------------
class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();
    void exit();
    void keyPressed(int key);
    
    void setupHttp();
    void setupCV();
    void drawConfig();
    void makeMask();
    void newResponse(ofxHttpResponse & response);
    
    ofxCvPiCam cam;
    ofxCv::ContourFinder contourFinder;
    ofxCv::RectTrackerFollower<Blob> tracker;
    cv::BackgroundSubtractorMOG2* pMOG2;
    cv::Mat frame,resizeF,fgMaskMOG2,output;
    cv::Mat mask,maskOutput;
    cv::Mat lightenMat;
    ofxHttpUtils httpUtils;
    string responseStr;
    string requestStr;
    
    bool counterLatches[30];
    int count;
    int countIn;
    int countOut;
    bool countInLatch;
    bool countOutLatch;
    
    void loadConfig();
    ofxXmlSettings XML;
    int _contourThreshold;
    int _threshold;
    int _blur;
    int _minArea;
    int _maxArea;
    int _persistance;
    int _maxDistance;
    int _minBlobSize;
    int _midBlobSize;
    int _maxBlobSize;
    int _lineYPos;
    int _lineWidth;
    int _cameraWidth;
    int _cameraHeight;
    int _history;
    int _MOGThreshold;
    int _lightenAmount;
    int _contrast;
    int _brightness;
    
    string _locationID;
    string _secretKey;
    string _uploadurl;
    string _uploadFileURL;
    std::deque<myLine> lines;
    std::deque<string> actions;
    ofPoint startLine;
    ofPoint endLine;
};


