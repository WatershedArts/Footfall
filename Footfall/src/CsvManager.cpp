// * Name: CsvManager.cpp
// * Project: Footfall
// * Author: David Haylock
// * Creation Date: 13/02/2017
// * Copyright: (c) 2017 by Watershed Arts Trust Ltd.

#include "CsvManager.h"
//--------------------------------------------------------------
void CsvManager::setup(string saveFolder)
{
	cout << "Setting Up CSV Manager";
	_saveFolder = saveFolder;
	ofDirectory csvDirectory(ofToDataPath(_saveFolder));
	
	if (!csvDirectory.exists())
	{
		csvDirectory.createDirectory(_saveFolder);
		cout << " - Creating " << _saveFolder << " Folder" << endl;
	}
	else
	{
		cout << " - " << _saveFolder << " Folder Already Exists" << endl;
	}
}
//--------------------------------------------------------------
void CsvManager::addRecord(string count, string timestamp)
{
	loggedData.push_back(Post_Data(count, timestamp));
}
//--------------------------------------------------------------
void CsvManager::saveRecords()
{
	logFile.createFile(ofToDataPath(_saveFolder+"/"+ofGetTimestampString("%Y-%m-%d-%H-%M")+".csv"));
	
	for (int i = 0; i < loggedData.size(); i++)
	{
		ofxCsvRow row;
		row.setString(0, loggedData[i]._count);
		row.setString(1, loggedData[i]._timestamp);
		logFile.addRow(row);
	}
	logFile.save(ofToDataPath(_saveFolder+"/"+ofGetTimestampString("%Y-%m-%d-%H-%M")+".csv"));
}
//--------------------------------------------------------------
void CsvManager::close()
{
	saveRecords();
}
