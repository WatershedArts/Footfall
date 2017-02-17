// * Name: CsvManager.h
// * Project: Footfall
// * Author: David Haylock
// * Creation Date: 13/02/2017
// * Copyright: (c) 2017 by Watershed Arts Trust Ltd.

#ifndef CsvManager_h
#define CsvManager_h

#include <stdio.h>
#include "ofMain.h"
#include "ofxCsv.h"
#include "Configuration.h"

class CsvManager
{
	public:
		//! Setup the Csv Manager
		void setup(string saveFolder);
	
		//! Add a new Record
		void addRecord(string count,string timestamp);
	
		//! Save the records
		void saveRecords();
	
		//! Close the Csv Manager
		void close();
	
	private:
		ofxCsv logFile;
		string _saveFolder;
		vector<Post_Data> loggedData;
};

#endif /* CsvManager_h */
