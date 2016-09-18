/*
 * Command line application for GRENDL Server.
 *
 *
 */

#include <string>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <unistd.h>
#include <dlfcn.h>
#include <stdio.h>  /* defines FILENAME_MAX */



#include "platform.h"
#include "yaml-cpp/yaml.h" //Include for installed lib
//#include "yaml.h"
#include "EnsembleApplication.h"
#include "Configuration.h"

/*
 *  Managing server application. Responds to OSC messages and manages render engine
 */
EnsembleApplication *messageServer;

#ifdef WINDOWS
    #include <direct.h>
    #define GetCurrentDir _getcwd
#else
    #include <unistd.h>
    #define GetCurrentDir getcwd
 #endif

char cCurrentPath[FILENAME_MAX];

 int getWorkingDirectory () {
	std::cout << "GetWorkingDirectory..." << std::endl;
	 int err_no = 0;
	 if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
	 {
		std::cout << " oops..." << std::endl;
		 return err_no;
	 }

	 cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */

	 printf ("The current working directory is %s\n", cCurrentPath);
	 return err_no;
 }


/** 
 * Get the name of the YAML server configuration file
 */
std::string getConfig(int argc, const char* argv[]) {
	std::string fname = "grendl_server.conf";
	// ensure the correct number of parameters are used.
	if ( argc == 2 ) 
	{
		//std::cout<< "Default config file is " << fname << std::endl;
		fname =  argv[1];
		
		std::cout << "\nInitializing from configuration file " << fname << std::endl ;
	}
	else {
		std::cout << "Wrong number of command line arguments";
		// TODO: better error handling on this; or take the default
		exit(0);
	}
	return fname;
}

/**
 * main routine for starting application
 *  @param arv[1] is path to yaml config file
 *  TODO: error handling around initialization and start functions
 *
 */
int main(int argc, const char* argv[])
{

	/*test!!*/

	std::string yaml_string = 
	"- name: cont1"
	"\n  lol: what"
	"\n- name: cont2"
	"\n  lol: hi";

	
#if OS_IS_LINUX == 1 || OS_IS_MACOSX == 1 || OS_IS_CYGWIN == 1 || OS_IS_UNIX == 1
    std::cout << "\nEnsemble 1.00 (UNIX/POSIX build).\n" ;
#elif ((OS_IS_WIN32 == 1) || (OS_IS_WIN64))
    std::cout << "\nEnsemble 1.00 (Windows build)."<<std::endl ;
#endif
	//std::cout << glGetString(GL_VERSION) << std::endl;
	std::string configFile = getConfig(argc, argv);
	getWorkingDirectory();
	if (argc > 0) {
		std::cout << "Argc is " << argc << ": argv[1] = " << argv[1] << std::endl;
	}
	Configuration applicationConfig (configFile);
	std::cout<<"test!"<<std::endl;
	applicationConfig.Initialize(yaml_string); //test!!
	messageServer = new EnsembleApplication(applicationConfig, argc, argv);
	messageServer->initialize();
	// start() currently does not return until the application exits
	messageServer->start();

	// Clean up
    std::cout << "Exiting main function...\n";
    if (messageServer) {
    	messageServer->stop();
    	delete messageServer;
    }
    return 0;
}
