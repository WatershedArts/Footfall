// * Name: HTTPManager.cpp
// * Project: Footfall
// * Author: David Haylock
// * Creation Date: 13/02/2017
// * Copyright: (c) 2017 by Watershed Arts Trust Ltd.

#include "HTTPManager.h"
//--------------------------------------------------------------
void HTTPManager::setup(HTTP_Configuration _httpConfig)
{
	cout << "Setting Up HTTP Manager" << endl;
	_postServer = _httpConfig.postserver;
	_postExtension = _httpConfig.postextension;
	_secretKey = _httpConfig.secretkey;
	_keepBackups = _httpConfig.keepbackups;
	
	haveDeletedCSVs = false;
	networkOk = false;
	
	ofAddListener(postUtils.newResponseEvent, this, &HTTPManager::newResponse);
	postUtils.setMaxRetries(1);
	postUtils.start();
	
	// Just incase there is a network outage
	if (_keepBackups)
	{
		cout << " - Keeping Backup Logs" << endl;
		backupLogger.setup("backupCSVs");
	}
	else
	{
		cout << " - Not Keeping Backup Logs" << endl;
	}
	
	// Check if Server Alive
	ofxHttpForm formIn;
	formIn.action = _postServer +"/"+_postExtension;
	formIn.method = OFX_HTTP_POST;
	formIn.addFormField("secret", _secretKey);
	formIn.addFormField("check","1");
	postUtils.submitForm(formIn);
	
	
	if (networkOk)
	{
		uploadSavedRecords();
	}
	else
	{
		ofLogError() << "Can't upload due to no network";
	}
	
}
//--------------------------------------------------------------
void HTTPManager::close()
{
	backupLogger.close();
	ofRemoveListener(postUtils.newResponseEvent, this, &HTTPManager::newResponse);
	postUtils.stop();
}
//--------------------------------------------------------------
void HTTPManager::post(string count)
{
	_timestamp = ofGetTimestampString("%Y-%m-%d %H:%M:%S");
	_count = count;
	
	if (networkOk)
	{
		if (ofIsStringInString(count, "0") || ofIsStringInString(count, "-0"))
		{
			// Do nothing Invalid Number
		}
		else
		{
			ofxHttpForm formIn;
			formIn.action = _postServer +"/"+_postExtension;
			formIn.method = OFX_HTTP_POST;
			formIn.addFormField("secret", _secretKey);
			formIn.addFormField("location", "1");
			formIn.addFormField("count", count);
			formIn.addFormField("rawtimestamp", _timestamp);
			formIn.addFormField("submit","1");
			postUtils.addForm(formIn);
		}
	}
	else
	{
		if (_keepBackups)
		{
			// No Network Start Using Csv
			backupLogger.addRecord(_count, _timestamp);
		}
	}
}
//--------------------------------------------------------------
void HTTPManager::postSavedData(string count,string timestamp)
{
	ofxHttpForm formIn;
	formIn.action = _postServer +"/"+_postExtension;
	formIn.method = OFX_HTTP_POST;
	formIn.addFormField("secret", _secretKey);
	formIn.addFormField("location", "1");
	formIn.addFormField("count", count);
	formIn.addFormField("rawtimestamp", timestamp);
	formIn.addFormField("submit","1");
	postUtils.addForm(formIn);
}
//--------------------------------------------------------------
void HTTPManager::newResponse(ofxHttpResponse &response)
{
	string _responseStr = ofToString(response.status) + ":" + (string)response.responseBody;
	if(ofToString(response.status) == "200" && !networkOk)
	{
		cout << "Server is OK" << endl;
		cout << _responseStr << endl;
		networkOk = true;
	}
	else if(ofToString(response.status) == "-1")
	{
		cout << "Server is Down" << endl;
		cout << _responseStr << endl;
		networkOk = false;
	}
}
//--------------------------------------------------------------
bool HTTPManager::isNetworkAlive()
{
	return networkOk;
}
//--------------------------------------------------------------
bool HTTPManager::haveUploadedSavedRecords()
{
	return haveDeletedCSVs;
}
//--------------------------------------------------------------
void HTTPManager::uploadSavedRecords()
{
	ofxCsv log;
	ofDirectory csvDir(ofToDataPath("backupCSVs",true));
	if (csvDir.exists())
	{
		cout << "CSV Directory Found!" << endl;
		
		if (csvDir.getFiles().size() == 0)
		{
			haveDeletedCSVs = true;
		}
		else
		{
			for(int file = 0; file < csvDir.getFiles().size(); file++)
			{
				cout << "--------------------" << endl;
				cout << csvDir[file].getFileName() << endl;
				cout << "--------------------" << endl;
				log.load(csvDir[file].getAbsolutePath());

				cout << "--------------------" << endl;
				
				for (int i = 0; i < log.size(); i++)
				{
					postSavedData(log.getRow(i).getString(0),log.getRow(i).getString(1));
				}
				
				if (networkOk)
				{
					if (file == csvDir.getFiles().size()-1)
					{
						ofSystem("rm "+ofToDataPath("backupCSVs/")+"*.csv");
						haveDeletedCSVs = true;
					}
				}
			}
		}
	}
	else{
		ofLogError() << "CSV Directory: " << csvDir.path() << " MISSING";
	}
}
