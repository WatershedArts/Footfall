//
//  LoadConfig.h
//  FootfallRPi2
//
//  Created by David Haylock on 29/01/2016.
//
//

#ifndef LoadConfig_h
#define LoadConfig_h

#include <stdio.h>
#include "ofxJSON.h"
#include "ofMain.h"

struct Settings {
    int camerawidth;
    int cameraheight;
    
    bool shadows;
    int history;
    int mogthreshold;
    
    bool flipH;
    bool flipV;
    
    int blur;
    int contourthreshold;
    int threshold;
    int minarea;
    int maxarea;
    
    float blobdyingtime;
    int persistance;
    int maxdistance;
    int minsizeone;
    int minsizetwo;
    int minsizethree;
    vector<ofPoint> maskCoord;

    int port;
    string postserver;
    string postextension;
    string secretkey;
    int maxretries;
    
    ofPoint startPos;
    ofPoint endPos;
    
    bool orientation;
    int offset;
    
};

class LoadConfig {
    
public:
    void load(string file);
    Settings getSettings();
private:
    Settings s;
    
};

#endif /* LoadConfig_h */
