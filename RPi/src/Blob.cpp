// * Name: Blob.cpp
// * Project: Footfall
// * Author: David Haylock
// * Creation Date: 20-11-2014
// * Copyright: (c) 2015 by Watershed Arts Trust Ltd.

#include "Blob.h"
using namespace ofxCv;
using namespace cv;

const float dyingTime = 0.1f;
//--------------------------------------------------------------
void Blob::setup(const cv::Rect& track)
{
    color.setHsb(ofRandom(0, 255), 255, 255);
    currentPos = toOf(track).getCenter();
    previousPos = currentPos;
    numberOfPeople = 0;
    _one = 0;
    _two = 0;
    _three = 0;
    bIn = false;
    bOut = false;
    
    inLineLatch = false;
    outLineLatch = false;
}
//--------------------------------------------------------------
void Blob::setSizes(int one, int two, int three)
{
    _one = one;
    _two = two;
    _three = three;
}
//--------------------------------------------------------------
void Blob::setLinePosition(ofPoint startPoint, ofPoint endPoint)
{
    _lineStartPoint = startPoint;
    _lineEndPoint = endPoint;
}
//--------------------------------------------------------------
int Blob::howManyIn()
{
    return numberOfPeople;
}
//--------------------------------------------------------------
int Blob::howManyOut()
{
    return numberOfPeople;
}
//--------------------------------------------------------------
bool Blob::wasIn()
{
    return bIn;
}
//--------------------------------------------------------------
bool Blob::wasOut()
{
    return bOut;
}
//--------------------------------------------------------------
int Blob::howWide()
{
    return width;
}
//--------------------------------------------------------------
void Blob::update(const cv::Rect& track)
{
    currentPos = toOf(track).getCenter();
    area = toOf(track).getArea();
    previousPos.interpolate(currentPos, .3);
    width = toOf(track).width;
    height = toOf(track).height;
    
    if (!inLineLatch && previousPos.y >= currentPos.y && (currentPos.y >= _lineStartPoint.y-20 && currentPos.y <= _lineStartPoint.y+20)) {
        bIn = true;
        bOut = false;
        
        if (width >= _three) {
            numberOfPeople = 3;
        }
        else if(width >= _two) {
            numberOfPeople = 2;
        }
        else if(width >= _one) {
            numberOfPeople = 1;
        }
        else {
            cout << "No big enough for a person" << width << endl;
        }
        
        inLineLatch = true;
    }
    
    if (!outLineLatch && previousPos.y <= currentPos.y && (currentPos.y >= _lineStartPoint.y-20 && currentPos.y <= _lineStartPoint.y+20)) {
        bIn = false;
        bOut = true;
        if (width >= _three) {
            numberOfPeople = 3;
        }
        else if(width >= _two) {
            numberOfPeople = 2;
        }
        else if(width >= _one) {
            numberOfPeople = 1;
        }
        else {
            cout << "No big enough for a person" << width << endl;
        }
        outLineLatch = true;
    }
    
    all.addVertex(previousPos);
}
//--------------------------------------------------------------
void Blob::kill()
{
    bIn = false;
    bOut = false;
    dead = true;
}
//--------------------------------------------------------------
void Blob::draw()
{
    ofPushStyle();
    float size = 16;
    ofSetColor(255);
    if(startedDying) {
        ofSetColor(ofColor::red);
        size = ofMap(ofGetElapsedTimef() - startedDying, 0, dyingTime, size, 0, true);
    }
    ofNoFill();
    ofCircle(currentPos, size);
    ofSetColor(color);
    all.draw();
    
    ofSetColor(255);
    string blobDetails = "Blob Id: " + ofToString(label) + " Number: "+ ofToString(numberOfPeople) + " Width: " + ofToString(width);
    ofDrawBitmapStringHighlight(blobDetails, currentPos.x,currentPos.y);
}