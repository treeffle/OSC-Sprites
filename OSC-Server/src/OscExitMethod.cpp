/*
 *  OscExitMethod.cpp
 *  GrendlServer
 *
 *  Created by Chris Branton on 11/16/11.
 *  Copyright 2011 Chris Branton. All rights reserved.
 *
 */

#include "OscExitMethod.h"
//#include "OscServerMethod.h"

/** Constructor. Sets the method name and info in the base class.            */
OscExitMethod::OscExitMethod( WOscContainer* parent, OscListener* receiverContext)
	:OscServerMethod(parent, receiverContext, "exit", "This method quits WOscServer")
{

}

/** The exit method. Terminates the WOscServer by setting the exit flag.     */
void OscExitMethod::Method(const WOscMessage* msg, const WOscTimeTag& when, const NetReturnAddress* ra)
{
	std::cout << "Exit message received";
    GetContext()->SetExit();
}

void OscExitMethod::Method(const WOscMessage& msg, const WOscTimeTag& when, const WOscNetReturn& ra) 
{
	std::cout<<"Exit Message received" << std::endl;
	GetContext()->SetExit();
}

void OscExitMethod::Method(const WOscMessage* msg, const WOscTimeTag& when, const WOscNetReturn* ra) 
{
	std::cout<<"Exit Message received" << std::endl;
	GetContext()->SetExit();
}

