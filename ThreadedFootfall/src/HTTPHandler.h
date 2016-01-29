//
//  HTTPHandler.h
//  FootfallRPi2
//
//  Created by David Haylock on 29/01/2016.
//
//

#ifndef HTTPHandler_h
#define HTTPHandler_h

#include <stdio.h>
#include "ofMain.h"
#include "ofxHttpUtils.h"

class HTTPHandler {
    
public:
    void setup(string postServer,string postExtension,string secretKey);
    void close();
    void post(string count);
    void newResponse(ofxHttpResponse & response);
    void drawPost(int x,int y);
    string getResponse();
    
private:
    string _postServer;
    string _postExtension;
    string _secretKey;
    string _responseStr;
    string _requestStr;
    ofxHttpUtils postUtils;
    ofTrueTypeFont font;
};
#endif /* HTTPHandler_h */
