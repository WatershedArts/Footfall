//
//  LoadConfig.cpp
//  FootfallRPi2
//
//  Created by David Haylock on 29/01/2016.
//
//

#include "LoadConfig.h"
//--------------------------------------------------------------
void LoadConfig::load(string file)
{
    ofxJSONElement config;
    if (!config.open(file)) {
        cout << "Could not find the Configuration File" << endl;
        return;
    }
    else {
        cout << "Found the Configuration File" << endl;
        
        // App Settings
        s.camerawidth = config["Footfall"]["AppSettings"]["camerawidth"].asInt();
        s.cameraheight = config["Footfall"]["AppSettings"]["cameraheight"].asInt();

        
        // CV Settings
        // Vision
        s.contourthreshold = config["Footfall"]["CVSettings"]["Vision"]["contourthreshold"].asInt();
        s.threshold = config["Footfall"]["CVSettings"]["Vision"]["threshold"].asInt();
        s.minarea = config["Footfall"]["CVSettings"]["Vision"]["minarea"].asInt();
        s.maxarea = config["Footfall"]["CVSettings"]["Vision"]["maxarea"].asInt();
        s.blur = config["Footfall"]["CVSettings"]["Vision"]["blur"].asInt();
        s.shadows = config["Footfall"]["CVSettings"]["MOG"]["shadows"].asBool();
        s.history = config["Footfall"]["CVSettings"]["MOG"]["history"].asInt();
        s.mogthreshold = config["Footfall"]["CVSettings"]["MOG"]["threshold"].asInt();
        
        // Break Line
        s.orientation = config["Footfall"]["CVSettings"]["BreakLine"]["orientation"].asBool();
        s.startPos = ofPoint(config["Footfall"]["CVSettings"]["BreakLine"]["startposx"].asInt(),config["Footfall"]["CVSettings"]["BreakLine"]["startposy"].asInt());
        s.endPos = ofPoint(config["Footfall"]["CVSettings"]["BreakLine"]["endposx"].asInt(),config["Footfall"]["CVSettings"]["BreakLine"]["endposy"].asInt());
        s.offset = config["Footfall"]["CVSettings"]["BreakLine"]["offset"].asInt();
        
        // Blob
        s.blobdyingtime = config["Footfall"]["CVSettings"]["Blob"]["blobdyingtime"].asFloat();
        s.persistance = config["Footfall"]["CVSettings"]["Blob"]["persistance"].asInt();
        s.maxdistance = config["Footfall"]["CVSettings"]["Blob"]["maxdistance"].asInt();
        s.minsizeone = config["Footfall"]["CVSettings"]["Blob"]["minsizeone"].asInt();
        s.minsizetwo = config["Footfall"]["CVSettings"]["Blob"]["minsizetwo"].asInt();
        s.minsizethree = config["Footfall"]["CVSettings"]["Blob"]["minsizethree"].asInt();
        
        vector<ofPoint> pts;
        int maskSize = config["Footfall"]["CVSettings"]["MaskArea"].size();
        for (int i = 0 ; i < maskSize; i++) {
            ofPoint pt = ofPoint(config["Footfall"]["CVSettings"]["MaskArea"][i]["coordx"].asInt(),config["Footfall"]["CVSettings"]["MaskArea"][i]["coordy"].asInt());
            pts.push_back(pt);
        }
        s.maskCoord = pts;
    
        // Post Data Settings
        s.postserver = config["Footfall"]["PostDataSettings"]["postserver"].asString();
        s.postextension = config["Footfall"]["PostDataSettings"]["postextension"].asString();
        s.secretkey = config["Footfall"]["PostDataSettings"]["secretkey"].asString();
        s.maxretries = config["Footfall"]["PostDataSettings"]["maxretries"].asInt();
    }
}
//--------------------------------------------------------------
Settings LoadConfig::getSettings()
{
    return s;
}