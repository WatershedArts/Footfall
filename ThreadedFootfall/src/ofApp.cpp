#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    config.load("config.json");
    
    // Setup HTTP Handler
    httpHandler.setup(config.getSettings().postserver,
                      config.getSettings().postextension,
                      config.getSettings().secretkey
                      );
    
    // Setup Video and Tracking
    piCam.setup(config.getSettings().camerawidth,
                             config.getSettings().camerawidth,false);
    
    tracking.setup(config.getSettings().camerawidth,
                   config.getSettings().cameraheight,
                   config.getSettings().history,
                   config.getSettings().maskCoord
                   );
    
    tracking.setTargetLines(config.getSettings().startPos,
                            config.getSettings().endPos,
                            config.getSettings().offset,
                            config.getSettings().orientation
                            );
    
    tracking.setContourFinder(config.getSettings().minarea,
                              config.getSettings().maxarea,
                              config.getSettings().contourthreshold
                              );
    
    tracking.setTrackerVariables(config.getSettings().minsizeone,
                                 config.getSettings().minsizetwo,
                                 config.getSettings().minsizethree,
                                 config.getSettings().maxdistance,
                                 config.getSettings().persistance
                                 );
    
    pMOG2 = new BackgroundSubtractorMOG2(config.getSettings().history,
                                         config.getSettings().mogthreshold,
                                         config.getSettings().shadows
                                         );
    tracking.start();
    _lightenAmount = 25;
    
    ofAddListener(tracking.peopleIn, this, &ofApp::peopleGoingIn);
    ofAddListener(tracking.peopleOut, this, &ofApp::peopleGoingOut);
}
//--------------------------------------------------------------
void ofApp::exit()
{
    tracking.stop();
}
//--------------------------------------------------------------
void ofApp::update()
{
    Mat tempCam = piCam.grab();
    if(!tempCam.empty()){
        Mat cImage = tempCam;
        resize(cImage, resized, cv::Size(cImage.size().width, cImage.size().height));
        
        lightenMat = resized + cv::Scalar(_lightenAmount,_lightenAmount,_lightenAmount);
        
        // Activate the background substraction
        pMOG2->operator()(lightenMat, processedMog);
        
        threshold(processedMog, config.getSettings().threshold);
        blur(processedMog, config.getSettings().blur);
        dilate(processedMog);
        
        tracking.lock();
        tracking.getCameraImage(processedMog);
        tracking.unlock();
    }
}
//--------------------------------------------------------------
void ofApp::draw()
{
    ofBackground(25);
    tracking.draw(0, 0);
    tracking.drawHistory(0, 0);
    
    // Reporting
    stringstream ss;
    ss << "Footfall" << endl;
    ss << "FPS: " << ofToString(ofGetFrameRate()) << endl;
    ss << "Tally: " << tracking.getTally() << endl;
    ss << "Count in: " << tracking.getCountIn() << endl;
    ss << "Count out: " << tracking.getCountOut() << endl;
    ss << "Is Tracking: " << tracking.isTracking() << endl;
    ss << "Current Number of People: " << tracking.getCurrentNumberOfPeople() << endl;
    
    ofSetColor(ofColor::white);
    ofDrawBitmapString(ss.str(), 10,400);
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{

}
//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{

}
//--------------------------------------------------------------
void ofApp::peopleGoingIn(string &count)
{
    cout << "People Going In: " << count << endl;
    httpHandler.post(count);
}
//--------------------------------------------------------------
void ofApp::peopleGoingOut(string &count)
{
    cout << "People Going Out: " << count << endl;
    httpHandler.post(count);
}
