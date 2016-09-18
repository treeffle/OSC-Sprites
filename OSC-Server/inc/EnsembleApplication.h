/*
 *  EnsembleApplication.h
 *
 *  Primary application class. Manages OscListener
 *
 *  Created by Chris Branton on 4/27/11.
 *  Copyright 2011 Chris Branton. All rights reserved.
 *
 *  TODO: this is where we will add the render engine
 *
 */

#ifndef _GRENDL_SERVER_H_
#define  _GRENDL_SERVER_H_

#include <list>

#include "OscListener.h"
#include "Configuration.h"
#include "OscListener.h"
#include "SpriteView.h"


class EnsembleApplication {

public:
	EnsembleApplication();
	// TODO: dirty hack -- added command params to constructor
	EnsembleApplication(Configuration& applicationConfig, int argc, const char* argv[]);
	~EnsembleApplication();
	
//	void initialize(Configuration& applicationConfig);
	int stop();
	void initialize();
	
	void start();
	bool isDone();
	
	static int quit(void *ptr);
	static int transfer(void *ptr);

	static std::vector<std::string> userathosts;
	
protected:
	OscListener* server;
	Configuration& config;
	bool doneFlag;
	// TODO: decide if we need to keep the config member
	void addOscHandlers (OscListener& listener, std::list<OscMessage>& messageSpecs);
	//std::list<OscListener> listenerList;
//	Ensemble* grendl;
	SpriteView *mainView;
	std::map<std::string, Container> cont_map;
	std::map<std::string, Sprite> sprite_map;
};

#endif // _GRENDL_SERVER_H_
