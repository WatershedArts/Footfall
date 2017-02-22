// * Name: ofApp.cpp
// * Project: Footfall
// * Author: David Haylock
// * Creation Date: 13/02/2017
// * Copyright: (c) 2017 by Watershed Arts Trust Ltd.


#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetVerticalSync(true);
	ofSetFrameRate(25);
	
	configManager.loadConfiguration("config.json");
	
	_logToCsv = configManager.getConfiguration().useCsvLogging;
	_logToServer = configManager.getConfiguration().useHttp;
	
	cameraManager.setup(configManager.getConfiguration().cameraConfig);
	trackingManager.setup(configManager.getConfiguration().trackingConfig);
	
	if (_logToServer) httpManager.setup(configManager.getConfiguration().httpConfig);
	if (_logToCsv) csvManager.setup("csvlogs");
	
	ofAddListener(trackingManager.blobIn, this, &ofApp::blobIn);
	ofAddListener(trackingManager.blobOut, this, &ofApp::blobOut);
}
//--------------------------------------------------------------
void ofApp::exit()
{
	if (_logToServer) httpManager.close();
	if (_logToCsv) csvManager.close();
		
	ofRemoveListener(trackingManager.blobIn, this, &ofApp::blobIn);
	ofRemoveListener(trackingManager.blobOut, this, &ofApp::blobOut);
}
//--------------------------------------------------------------
void ofApp::update()
{
	cameraManager.update();
	trackingManager.update(cameraManager.getImage());
}
//--------------------------------------------------------------
void ofApp::draw()
{
	ofPushMatrix();
	ofScale(1,1);
	cameraManager.draw();
	trackingManager.draw();
	ofPopMatrix();
	
	stringstream ss;
	ss << "Footfall" << endl;
	ss << "People In: " << peopleIn;
	ss << " People Out: " << peopleOut;
	ss << " Tally: " << (peopleIn-peopleOut);
	ss << " FPS: " << ofGetFrameRate() << endl;
	ofDrawBitmapStringHighlight(ss.str(),7,ofGetHeight()-20);
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
void ofApp::blobIn(int &val)
{
	peopleIn += val;
	cout << val << " Blob(s) Came In" << endl;
	
	if (_logToServer) httpManager.post(ofToString(val));
	if (_logToCsv) csvManager.addRecord(ofToString(val), ofGetTimestampString("%Y-%m-%d %H:%M:%S"));
}
//--------------------------------------------------------------
void ofApp::blobOut(int &val)
{
	peopleOut += abs(val);
	cout << val << " Blob(s) Went Out" << endl;
	
	if (_logToServer) httpManager.post(ofToString(val));
	if (_logToCsv) csvManager.addRecord(ofToString(val), ofGetTimestampString("%Y-%m-%d %H:%M:%S"));
}
