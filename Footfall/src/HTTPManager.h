// * Name: HTTPManager.h
// * Project: Footfall
// * Author: David Haylock
// * Creation Date: 13/02/2017
// * Copyright: (c) 2017 by Watershed Arts Trust Ltd.

#ifndef HTTPManager_h
#define HTTPManager_h

#include <stdio.h>
#include "ofMain.h"
#include "ofxHttpUtils.h"
#include "Configuration.h"
#include "CsvManager.h"

class HTTPManager
{
	public:
		//! Setup With Config
		void setup(HTTP_Configuration _httpConfig);
	
		//! Shutdown
		void close();
	
		//! Post Event
		void post(string count);
	
		//! Post Saved Event
		void postSavedData(string count,string timestamp);
	
		//! Event Checker
		void newResponse(ofxHttpResponse &response);
	
		//! Check the Network is Alive
		bool isNetworkAlive();
	
		//! Have the records been uploaded
		bool haveUploadedSavedRecords();
	
		//! Upload the Files
		void uploadSavedRecords();
	
	private:
		ofxHttpUtils postUtils;
		CsvManager backupLogger;
	
		string _postServer;
		string _postExtension;
		string _secretKey;
	
		string _count;
		string _timestamp;
		bool networkOk;
		bool haveDeletedCSVs;
		bool _keepBackups;
};


#endif /* HTTPManager_h */
