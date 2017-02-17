// * Name: ConfigurationManager.h
// * Project: Footfall
// * Author: David Haylock
// * Creation Date: 13/02/2017
// * Copyright: (c) 2017 by Watershed Arts Trust Ltd.

#ifndef ConfigurationManager_h
#define ConfigurationManager_h

#include <stdio.h>
#include "ofxJSON.h"
#include "ofMain.h"
#include "Configuration.h"

class ConfigurationManager
{
	public:
	
		//! Load the Config file
		void loadConfiguration(string file);
	
		//! Print to the Console
		void printConfiguration();
	
		//! Get the Configs
		Configuration getConfiguration();
	
	private:
		ofxJSONElement configFile;
		Configuration config;
};
#endif /* ConfigurationManager_h */
