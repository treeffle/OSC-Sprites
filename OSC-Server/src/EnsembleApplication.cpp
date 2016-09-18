/*
 *  GrendlServer.cpp
 *  GrendlServer
 *
 *  Created by Chris Branton on 4/27/11.
 *  Copyright 2011 Chris Branton. All rights reserved.
 *
 *  2013-07-29. Changing C++ version to take config in constructor and
 *    parameterless initialize.
 *    Seems to work better when using references
 *
 */

#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include "EnsembleApplication.h"

std::vector<std::string> EnsembleApplication::userathosts;

// TODO: work on defaults for default constructor. Also add copy constructor
EnsembleApplication::EnsembleApplication(): config(config) {
	server=new OscListener();
	// TODO: Have viewport dimensions/positions be initialized from config
	mainView = new SpriteView(500,500, &sprite_map);
	//for transparent LCD
	//mainView = new SpriteView( 3*768+ 2*768/15.4*1.235*2, 1366, &sprite_map /*map_pointer here*/);
}

EnsembleApplication::EnsembleApplication(Configuration& applicationConfig, int argc, const char* argv[]): config(applicationConfig) {
	server=new OscListener();
	// TODO: Have viewport dimensions/positions be initialized from config
	mainView = new SpriteView(500,500, &sprite_map);
	//for transparent LCD
	//mainView = new SpriteView(3*768 + 2*768/15.4*1.235*2, 1366, &sprite_map /*map_pointer here*/);
}

EnsembleApplication::~EnsembleApplication() {
	if (mainView) {
		delete mainView;
	}
//	if (grendl) {
//		delete grendl;
//	}
	if (server) {
		delete server;
	}
}


void EnsembleApplication::initialize() {
//void EnsembleApplication::initialize(Configuration& applicationConfig) {
	// Set port number. TODO: handle busy port
	int port = 7770;
	std::string portString("7770");
	config.GetValue("port", portString);
	port = atoi(portString.c_str());
	
	std::list<OscMessage> oscSpecs = config.GetOscConfiguration();

	// set up message handlers
	server->Initialize(oscSpecs, &cont_map, &sprite_map, mainView);
	std::cout << "Initializing Network Layer (port: " << portString << ")" <<std::endl ;

    if ( server->NetworkInit( port ) != OscListener::WOS_ERR_NO_ERROR ) {
        std::cout << "Exit."<<std::endl ;
        stop();
    }
	else {
		std::cout << "Starting OSC listener and waiting for OSC packets..."<<std::endl ;
	}

	
	
	
	std::list<Configuration::EnsembleMember>::const_iterator member;
	for(member=(config.ensembleMembers).begin(); member!= config.ensembleMembers.end(); member++)
	{
		//std::cout << member->name << std::endl;
		EnsembleApplication::userathosts.push_back(member->userAtHost);
	}
	mainView->start();
}

// Refactor -- with polling this never stops
void EnsembleApplication::start() {

	doneFlag = false;
	std::cout << "Starting Ensemble Server" << std::endl;
	
	// run osc server as long the "exit" flag isn't set.
	while (! server->Exit() ) {
	// get the next message
//		mainView->run();
		server->CheckForPackets();
		//std::cout << "." << std::endl;
			// poll for new packets every millisecond
		#if OS_IS_LINUX == 1 || OS_IS_MACOSX == 1 || OS_IS_CYGWIN == 1
			usleep(1000);
		#elif OS_IS_WIN32 == 1
			Sleep(1);
		#endif
	}
	
	// stop network layer
	server->NetworkHalt();
	mainView->exit_func();
	
	std::cout << "\nEnsemble Server exiting..." << std::endl ;
	doneFlag = true;
	
}

/*
 * Set the server's stop flag. Now that the start() method
 * does not exit until done, this method can likely be deprecated
 */ 
int EnsembleApplication::stop() {
	int returnVal = 0;
	doneFlag = true;
	// TODO: fix all of this
//	if (server !=NULL) {
	server->Stop();
	returnVal = 0;
//	}
	return returnVal;
}

bool EnsembleApplication::isDone() {
	return doneFlag;
}

