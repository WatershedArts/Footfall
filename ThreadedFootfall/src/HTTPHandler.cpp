//
//  HTTPHandler.cpp
//  FootfallRPi2
//
//  Created by David Haylock on 29/01/2016.
//
//

#include "HTTPHandler.h"
//--------------------------------------------------------------
void HTTPHandler::setup(string postServer, string postExtension, string secretKey)
{
    _postServer = postServer;
    _postExtension = postExtension;
    _secretKey = secretKey;
    _responseStr = "";
    
    ofAddListener(postUtils.newResponseEvent, this, &HTTPHandler::newResponse);
    postUtils.setMaxRetries(3);
    postUtils.start();
}
//--------------------------------------------------------------
void HTTPHandler::close()
{
    ofRemoveListener(postUtils.newResponseEvent, this, &HTTPHandler::newResponse);
    postUtils.stop();
}
//--------------------------------------------------------------
void HTTPHandler::post(string count)
{
    ofxHttpForm formIn;
    formIn.action = _postServer +"/"+_postExtension;
    formIn.method = OFX_HTTP_POST;
    formIn.addFormField("secret", _secretKey);
    formIn.addFormField("location", "1");
    formIn.addFormField("count", count);
    formIn.addFormField("rawtimestamp", ofGetTimestampString("%Y-%m-%d %H:%M:%s"));
    formIn.addFormField("submit","1");
    postUtils.addForm(formIn);
}
//--------------------------------------------------------------
string HTTPHandler::getResponse()
{
    return _responseStr;
}
//--------------------------------------------------------------
void HTTPHandler::newResponse(ofxHttpResponse &response)
{
    _responseStr = ofToString(response.status) + ":" + (string)response.responseBody;
    cout << _responseStr << endl;
}
//--------------------------------------------------------------
void HTTPHandler::drawPost(int x,int y)
{
    ofPushMatrix();
    ofTranslate(x, y);
    ofSetColor(ofColor::white);
    ofDrawBitmapString("Post Data", 0, 0);
    ofDrawBitmapString(_responseStr, 0, 15);
    ofPopMatrix();
}
