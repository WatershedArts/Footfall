// * Name: ofApp.cpp
// * Project: Footfall
// * Author: David Haylock
// * Creation Date: 20-11-2014
// * Copyright: (c) 2015 by Watershed Arts Trust Ltd.

#include "ofApp.h"

using namespace ofxCv;
using namespace cv;
//--------------------------------------------------------------
void ofApp::loadConfig()
{
    cout << "Loading Settings" << endl;
    if (XML.loadFile("config.xml")) {
        _contourThreshold = XML.getValue("CONFIG:CONTOURTHRESHOLD", 128);
        _threshold = XML.getValue("CONFIG:THRESHOLD", 150);
        _blur = XML.getValue("CONFIG:BLUR", 15);
        _minArea = XML.getValue("CONFIG:MINAREA", 3000);
        _maxArea = XML.getValue("CONFIG:MAXAREA", 256000);
        _persistance = XML.getValue("CONFIG:PERSISTANCE", 5);
        _maxDistance = XML.getValue("CONFIG:MAXDISTANCE", 140);
        _minBlobSize = XML.getValue("CONFIG:MINBLOBWIDTH", 50);
        _midBlobSize = XML.getValue("CONFIG:MIDBLOBWIDTH", 150);
        _maxBlobSize = XML.getValue("CONFIG:MAXBLOBWIDTH", 250);
        _history = XML.getValue("CONFIG:HISTORY", 150);
        _MOGThreshold = XML.getValue("CONFIG:MOGTHRESHOLD", 240);
        _lineYPos = XML.getValue("CONFIG:LINEYPOS", 240/2);
        _lineWidth = XML.getValue("CONFIG:LINEWIDTH", 20);
        _cameraWidth = XML.getValue("CONFIG:CAMERAWIDTH", 320);
        _cameraHeight = XML.getValue("CONFIG:CAMERAHEIGHT", 240);
        _locationID = XML.getValue("CONFIG:LOCATIONID", "");
        _uploadurl = XML.getValue("CONFIG:UPLOADURL", "");
        _uploadFileURL = XML.getValue("CONFIG:UPLOADFILEURL", "");
        _secretKey = XML.getValue("CONFIG:SECRETKEY", "");
        _lightenAmount = XML.getValue("CONFIG:LIGHTENAMOUNT",35);
        _contrast = XML.getValue("CONFIG:CONTRAST",0);
        _brightness = XML.getValue("CONFIG:BRIGHTNESS",0);
        XML.pushTag("CONFIG:MASKPOINTS");
        XML.pushTag("MASKPOINTS:POINT");
        int nbPoints = XML.getNumTags("POINT");
        for (int i = 0; i < nbPoints; i++) {
            int x, y;
            x = XML.getAttribute("POINT", "x", 0, i);
            y = XML.getAttribute("POINT", "y", 0, i);
            _maskPoints.push_back(cv::Point(x, y));
        }
        XML.popTag();
        XML.popTag();
        cout << "Loaded Settings" << endl;
    }
    else {
        cout << "Not Loaded Settings: Program will not run!" << endl;
        exit();
    }
}
//--------------------------------------------------------------
void ofApp::makeMask()
{
    mask = cvCreateMat(240, 320, CV_8UC1);
    for(int i=0; i<mask.cols; i++)
        for(int j=0; j<mask.rows; j++)
            mask.at<uchar>(cv::Point(i,j)) = 0;
    
    vector<cv::Point> polyright;
    approxPolyDP(_maskPoints, polyright, 1.0, true);
    
    fillConvexPoly(mask,&polyright[0],polyright.size(),255,8,0);
}
//--------------------------------------------------------------
void ofApp::setup()
{
    ofSetFrameRate(30);
    ofSetLogLevel(OF_LOG_ERROR);
    count = 0;
    countIn = 0;
    countOut = 0;
    countInLatch = false;
    countOutLatch = false;
    
    loadConfig();
    makeMask();
    
    startLine = ofPoint(0,_cameraHeight/2);
    endLine = ofPoint(_cameraWidth,_cameraHeight/2);
    
    for (int i = 0; i < 30; i++) {
        counterLatches[i] = true;
    }
    
    setupHttp();
    setupCV();
}
//--------------------------------------------------------------
void ofApp::setupCV()
{
    cam.setup(_cameraWidth, _cameraHeight, false);
    cam.setFlips(false, true);
    cam.setContrast(_contrast);
    cam.setBrightness(_brightness);
    
    pMOG2 = new BackgroundSubtractorMOG2(_history,_MOGThreshold,false);
    
    contourFinder.setMinAreaRadius(_minArea);
    contourFinder.setMaxAreaRadius(_maxArea);
    
    contourFinder.setThreshold(_contourThreshold);
    contourFinder.setFindHoles(true);
    
    tracker.setPersistence(_persistance);
    tracker.setMaximumDistance(_maxDistance);
}
//--------------------------------------------------------------
void ofApp::setupHttp()
{
    // Setup HTTP POST Unit
    ofAddListener(httpUtils.newResponseEvent,this,&ofApp::newResponse);
    httpUtils.start();
}
//--------------------------------------------------------------
void ofApp::newResponse(ofxHttpResponse & response)
{
    responseStr = ofToString(response.status) + ":" + (string)response.responseBody;
    cout << responseStr << endl;
}
//--------------------------------------------------------------
void ofApp::update()
{
    if (lines.size() > 10) {
        lines.pop_back();
    }
    
    if (actions.size() > 10) {
        actions.pop_back();
    }
    
    frame = cam.grab();
    
    if(!frame.empty())
    {
        resize(frame, resizeF, cv::Size(frame.size().width, frame.size().height));
        
        lightenMat = resizeF + cv::Scalar(_lightenAmount,_lightenAmount,_lightenAmount);
        
        lightenMat.copyTo(maskOutput,mask);
        
        // Activate the background substraction
        pMOG2->operator()(maskOutput, fgMaskMOG2);
        
        // Threshold the image
        threshold(fgMaskMOG2, output, _threshold);
        
        // Blur
        blur(output, _blur);
        
        // Dilate
        dilate(output);
        
        // Pass through the Contour Finder
        if (ofGetFrameNum() > 200) {
            contourFinder.findContours(output);
            // Do tracking
            tracker.track(contourFinder.getBoundingRects());
        }
    }
    
    myLine l;
    vector<Blob>& followers = tracker.getFollowers();
    for(int i = 0; i < followers.size(); i++) {
        followers[i].setLinePosition(startLine, endLine);
        followers[i].setSizes(_minBlobSize,_midBlobSize,_maxBlobSize);
        // If the tracker returns true in open latch increment, then close latch. Then kill the tracker element.
        if (followers[i].bIn) {
            if (counterLatches[i]) {
                countIn += followers[i].howManyIn();
                count += followers[i].howManyIn();
                string httpString = ofToString(followers[i].howManyIn());
                actions.push_front(ofToString(followers[i].howWide()));
                l.width = followers[i].howWide();
                lines.push_front(l);
                ofxHttpForm formIn;
                formIn.action = _uploadurl;
                formIn.method = OFX_HTTP_POST;
                formIn.addFormField("secret", _secretKey);
                formIn.addFormField("location", _locationID);
                formIn.addFormField("count", httpString);
                formIn.addFormField("rawtimestamp", ofGetTimestampString("%Y-%m-%d %H:%M:%s"));
                formIn.addFormField("submit","1");
                httpUtils.addForm(formIn);
                counterLatches[i] = false;
            }
            followers[i].kill();
            counterLatches[i] = true;
        }
        // If the tracker returns true out open latch increment, then close latch. Then kill the tracker element.
        if (followers[i].bOut) {
            if (counterLatches[i]) {
                countOut += followers[i].howManyOut();
                count -= followers[i].howManyOut();
                string httpString ="-"+ofToString(followers[i].howManyOut());
                actions.push_front(ofToString(followers[i].howWide()));
                l.width = followers[i].howWide();
                lines.push_front(l);
                ofxHttpForm formOut;
                formOut.action = _uploadurl;
                formOut.method = OFX_HTTP_POST;
                formOut.addFormField("secret", _secretKey);
                formOut.addFormField("location", _locationID);
                formOut.addFormField("count", httpString);
                formOut.addFormField("rawtimestamp", ofGetTimestampString("%Y-%m-%d %H:%M:%s"));
                formOut.addFormField("submit","1");
                httpUtils.addForm(formOut);
                counterLatches[i] = false;
            }
            followers[i].kill();
            counterLatches[i] = true;
        }
    }
    
    if (followers.empty()) {
        // CounterLatches used to increment the counter
        for (int i = 0; i < 30; i++) {
            counterLatches[i] = true;
        }
    }
}
//--------------------------------------------------------------
void ofApp::drawConfig()
{
    ofPushStyle();
    ofSetColor(255);
    ofSetLineWidth(0.5);
    
    // Center
    ofLine(_cameraWidth/2,_cameraHeight/2-5,_cameraWidth/2,_cameraHeight/2+5);
    
    ofLine(_cameraWidth/2-_minBlobSize/2,_cameraHeight/2-6,_cameraWidth/2-_minBlobSize/2 ,_cameraHeight/2+6);
    ofLine(_cameraWidth/2+_minBlobSize/2,_cameraHeight/2-6,_cameraWidth/2+_minBlobSize/2 ,_cameraHeight/2+6);
    
    ofLine(_cameraWidth/2-_midBlobSize/2,_cameraHeight/2-7,_cameraWidth/2-_midBlobSize/2 ,_cameraHeight/2+7);
    ofLine(_cameraWidth/2+_midBlobSize/2,_cameraHeight/2-7,_cameraWidth/2+_midBlobSize/2 ,_cameraHeight/2+7);
    
    ofLine(_cameraWidth/2-_maxBlobSize/2,_cameraHeight/2-8,_cameraWidth/2-_maxBlobSize/2 ,_cameraHeight/2+8);
    ofLine(_cameraWidth/2+_maxBlobSize/2,_cameraHeight/2-8,_cameraWidth/2+_maxBlobSize/2 ,_cameraHeight/2+8);
    
    ofPopStyle();
}
//--------------------------------------------------------------
void ofApp::draw()
{
    // No affect on the tracker mainly for viz
    ofSetColor(255);
    // Draw contourFinder
    contourFinder.draw();
    
    // Draw result of output
    drawMat(output, 0, 0,320,240);
    
    // Draw tracker
    vector<Blob>& followers = tracker.getFollowers();
    for(int i = 0; i < followers.size(); i++)
    {
        followers[i].draw();
    }
    
    string displayString = "Coming In: " + ofToString(countIn) + " Going Out: " + ofToString(countOut) + " Overall Count: " + ofToString(count);
    ofDrawBitmapStringHighlight(displayString,5,ofGetHeight()-15);
    
    // Threshold Lines
    ofPushStyle();
    ofSetColor(0, 200, 10);
    ofLine(startLine.x,startLine.y-20,endLine.x,endLine.y-20);
    ofSetColor(200, 0, 10);
    ofLine(startLine,endLine);
    ofSetColor(0, 200, 10);
    ofLine(startLine.x,startLine.y+20,endLine.x,endLine.y+20);
    ofPopStyle();
    
    drawConfig();
    
    for(int i = 0; i < lines.size(); i++) {
        ofSetColor(255,255,0,255-(i*25));
        lines[i].draw(320/2,240/2+(i*10));
    }
    
    int total = 0;
    int average = 0;
    
    for (int i = 0 ; i < actions.size(); i++) {
        total += ofToInt(actions[i]);
    }
    if (!actions.empty()) {
        average = total/actions.size();
        ofDrawBitmapStringHighlight(ofToString(average), 320,240/2);
    }
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key) {   }
//--------------------------------------------------------------
void ofApp::exit() {   }
