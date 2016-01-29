//
//  Tracking.cpp
//  FootfallRPi2
//
//  Created by David Haylock on 29/01/2016.
//
//

#include "Tracking.h"
//--------------------------------------------------------------
// *
// *    Setup Functions
// *
//--------------------------------------------------------------
void Tracking::setup(int width, int height, int history,vector<ofPoint>maskPoints)
{
    _width = width;
    _height = height;
    _history = history;
    
    count = 0;
    countIn = 0;
    countOut = 0;
    countInLatch = false;
    countOutLatch = false;
    
    for (int i = 0; i < 30; i++) {
        counterLatches[i] = true;
    }
    
    makeMask(maskPoints);
}
//--------------------------------------------------------------
void Tracking::close()
{
    if (isThreadRunning()) {
        stopThread();
    }
}
//--------------------------------------------------------------
void Tracking::setContourFinder(int minArea, int maxArea, int contourThreshold)
{
    _minArea = minArea;
    _maxArea = maxArea;
    _contourThreshold = contourThreshold;
    
    contourFinder.setMinAreaRadius(_minArea);
    contourFinder.setMaxAreaRadius(_maxArea);
    contourFinder.setThreshold(_contourThreshold);
    contourFinder.setFindHoles(true);
}
//--------------------------------------------------------------
void Tracking::setTrackerVariables(int minBlobArea, int midBlobArea, int maxBlobArea, int maxDistance, int persistance)
{
    _minBlobSize = minBlobArea;
    _midBlobSize = midBlobArea;
    _maxBlobSize = maxBlobArea;
    _maxDistance = maxDistance;
    _persistance = persistance;
    
    tracker.setMaximumDistance(_maxDistance);
    tracker.setPersistence(_persistance);
}
//--------------------------------------------------------------
void Tracking::setTargetLines(ofPoint startPoint, ofPoint endPoint, int offset, bool orientation)
{
    _startPos = startPoint;
    _endPos = endPoint;
    _offset = offset;
    _orientation = orientation;
}
//--------------------------------------------------------------
void Tracking::makeMask(vector<ofPoint>maskPoints)
{
//    // Make the Mask
//    vector <cv::Point> maskPts;
//    for (int i = 0; i < maskPoints.size(); i++) {
//        cv::Point p = cv::Point(maskPoints[i].x,maskPoints[i].y);
//        maskPts.push_back(p);
//    }
//    
//    // Make the Mask Mat
//    mask = cvCreateMat(_height, _width, CV_8UC1);
//    for(int i=0; i<mask.cols; i++)
//        for(int j=0; j<mask.rows; j++)
//            mask.at<uchar>(cv::Point(i,j)) = 0;
//    
//    vector<cv::Point> polyright;
//    approxPolyDP(maskPts, polyright, 1.0, true);
//    
//    fillConvexPoly(mask,&polyright[0],polyright.size(),255,8,0);
}
//--------------------------------------------------------------
// *
// *    Getters
// *
//--------------------------------------------------------------
string Tracking::isTracking()
{
    string st = (ofGetFrameNum() > _history) ? "Now Tracking" : "Not Tracking";
    return st;
}
//--------------------------------------------------------------
void Tracking::getCameraImage(cv::Mat camera)
{
    processed = camera;
}
//--------------------------------------------------------------
int Tracking::getCountIn()
{
    return countIn;
}
//--------------------------------------------------------------
int Tracking::getCountOut()
{
    return countOut;
}
//--------------------------------------------------------------
int Tracking::getTally()
{
    return (countIn-countOut);
}
//--------------------------------------------------------------
int Tracking::getCurrentNumberOfPeople()
{
    return tracker.getFollowers().size();
}
//--------------------------------------------------------------
// *
// *    Threading functions
// *
//--------------------------------------------------------------
void Tracking::start()
{
    startThread();
}
//--------------------------------------------------------------
void Tracking::stop()
{
    stopThread();
}
//--------------------------------------------------------------
void Tracking::threadedFunction()
{
    while (isThreadRunning()) {
        if (lock()) {
            if(counterHistory.size() > 25) {
                counterHistory.pop_back();
            }
            
            if (!processed.empty()) {
                // Pass through the Contour Finder
                if (ofGetFrameNum() > 200) {
                    contourFinder.findContours(processed);
                    // Do tracking
                    tracker.track(contourFinder.getBoundingRects());
                }
            }
            
            vector<Blob> &followers = tracker.getFollowers();
            for(int i = 0; i < followers.size(); i++) {
                followers[i].setLinePosition(_startPos, _endPos,_offset,_orientation);
                followers[i].setSizes(_minBlobSize,_midBlobSize,_maxBlobSize);
                
                if (followers[i].bIn) {
                    if (counterLatches[i]) {
                        history h;
                        h.direction = true;
                        h.size = followers[i].howWide();
                        counterHistory.push_front(h);
                        countIn += followers[i].howManyIn();
                        count += followers[i].howManyIn();
                        string httpString = ofToString(followers[i].howManyIn());
                        ofNotifyEvent(peopleIn, httpString, this);
                        followers[i].kill();
                        counterLatches[i] = false;
                    }
                    counterLatches[i] = true;
                }
                else if (followers[i].bOut) {
                    if (counterLatches[i]) {
                        history h;
                        h.direction = false;
                        h.size = followers[i].howWide();
                        counterHistory.push_front(h);
                        countOut += followers[i].howManyOut();
                        count -= followers[i].howManyOut();
                        string httpString ="-"+ofToString(followers[i].howManyOut());
                        ofNotifyEvent(peopleOut, httpString, this);
                        followers[i].kill();
                        counterLatches[i] = false;
                        followers[i].kill();
                    }
                    counterLatches[i] = true;
                }
            }
            unlock();
            sleep(10);
        }
    }
}
//--------------------------------------------------------------
void Tracking::draw(int x, int y)
{
    ofPushMatrix();
    ofTranslate(x, y);
    // No affect on the tracker mainly for viz
    ofSetColor(255);
    // Draw contourFinder
    contourFinder.draw();
    
    // Draw result of output
    drawMat(processed, 0, 0,_width,_height);
    
    ofPushStyle();
    ofSetColor(ofColor::red);
    ofDrawLine(_startPos, _endPos);
    ofSetColor(ofColor::green);
    ofDrawLine(_startPos.x,_startPos.y+_offset,_endPos.x,_endPos.y+_offset);
    ofDrawLine(_startPos.x,_startPos.y-_offset,_endPos.x,_endPos.y-_offset);
    ofPopStyle();
    
    ofPushMatrix();
    // Draw tracker
    vector<Blob>& followers = tracker.getFollowers();
    for(int i = 0; i < followers.size(); i++)
    {
        followers[i].draw();
    }
    ofPopMatrix();
    ofPopMatrix();
}
//--------------------------------------------------------------
void Tracking::drawHistory(int x,int y)
{
    ofPushMatrix();
    ofTranslate(x, y);
    for (int i = 0; i < counterHistory.size(); i++) {
        if (counterHistory[i].direction) {
            ofSetColor(0, 255, 0);
        }
        if (!counterHistory[i].direction) {
            ofSetColor(255, 5, 0);
        }
        ofDrawLine(_width/2-counterHistory[i].size/2, _height/2+(i*20), _width/2+counterHistory[i].size/2, _height/2+(i*20));
    }
    ofPopMatrix();
}
