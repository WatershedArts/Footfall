//
//  Blob.h
//  FootfallRPi2
//
//  Created by David Haylock on 29/01/2016.
//
//

#ifndef Blob_h
#define Blob_h

#include <stdio.h>
#include "ofMain.h"
#include "ofxCv.h"
//--------------------------------------------------------------
class Blob: public ofxCv::RectFollower {
protected:
    ofColor color;
    ofVec2f currentPos, previousPos;
    ofPoint _lineStartPoint, _lineEndPoint;
    float startedDying;
    ofPolyline all;
    float area;
    int width, height;
    int numberOfPeople;
public:
    Blob()
    :startedDying(0) {
    }
    void setup(const cv::Rect& track);
    void update(const cv::Rect& track);
    void kill();
    void draw();
    void setSizes(int one, int two, int three);
    void setLinePosition(ofPoint startPoint, ofPoint endPoint,int offset,bool orientation);
    
    bool bIn;
    bool bOut;
    
    bool wasIn();
    int howManyIn();
    bool wasOut();
    int howManyOut();
    
    int howWide();
    bool inLineLatch;
    bool outLineLatch;
    
    int _one;
    int _two;
    int _three;
    int _offset;
    bool _orientation;
    
};
#endif /* Blob_h */
