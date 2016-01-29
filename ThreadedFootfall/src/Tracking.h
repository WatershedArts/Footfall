//
//  Tracking.h
//  FootfallRPi2
//
//  Created by David Haylock on 29/01/2016.
//
//

#ifndef Tracking_h
#define Tracking_h

#include <stdio.h>
#include "ofMain.h"
#include "ofxCv.h"
#include "Blob.h"

using namespace ofxCv;
using namespace cv;
struct history {
    int size;
    bool direction; // false being out, true being in
};

class Tracking : public ofThread {
public:
    
    void setup(int width,int height,int history,vector<ofPoint>maskPoints);
    void setContourFinder(int minArea,int maxArea,int contourThreshold);
    void setTrackerVariables(int minBlobArea,int midBlobArea,int maxBlobArea,int maxDistance,int persistance);
    void setTargetLines(ofPoint startPoint,ofPoint endPoint,int offset, bool orientation);
    void makeMask(vector<ofPoint>maskPoints);
    
    
    void getCameraImage(Mat camera);
    
    void draw(int x,int y);
    void drawHistory(int x,int y);

    int getCountIn();
    int getCountOut();
    int getTally();
    int getCurrentNumberOfPeople();
    string isTracking();
    
    
    void blobGoingIn(string count);
    void blobGoingOut(string count);

    // Threading Stuff
    void threadedFunction();
    void start();
    void stop();
    
    void close();
    ofEvent<string> peopleIn;
    ofEvent<string> peopleOut;
protected:
    
    
    ofxCv::ContourFinder contourFinder;
    ofxCv::RectTrackerFollower<Blob> tracker;
    
    Mat processed;
    
    vector <ofPoint> _maskPoints;
    int _width;
    int _height;
    int _history;
    int _contourThreshold;
    int _minArea;
    int _maxArea;
    
    int _persistance;
    int _maxDistance;
    int _minBlobSize;
    int _midBlobSize;
    int _maxBlobSize;
    
    ofPoint _startPos;
    ofPoint _endPos;
    int _offset;
    bool _orientation;
    
    bool counterLatches[30];
    int count;
    int countIn;
    int countOut;
    bool countInLatch;
    bool countOutLatch;
    deque <history> counterHistory;
    
};
#endif /* Tracking_h */
