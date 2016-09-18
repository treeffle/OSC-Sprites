/*
 *  OscQuitMethod.cpp
 *  GrendlServer
 *
 *  Created by Chris Branton on 11/17/11.
 *  Copyright 2011 Chris Branton. All rights reserved.
 *
 */

#include "OscQuitMethod.h"

#include "OSCServerMethod.h"
#include "wosclib-1.0/WOscMethod.h"

/** Constructor. Sets the method name and info in the base class.            */
OscQuitMethod::OscQuitMethod(WOscContainer* parent, OscListener* receiverContext, std::map<std::string,Sprite>* sm)
	:OscServerMethod(parent, receiverContext, "quit", "This method executes quit scripts"),
	 sprite_map(sm)
{

}

/** The quit method.   */
void OscQuitMethod::Method(const WOscMessage *msg, const WOscTimeTag& when, const NetReturnAddress* ra)
{
	std::cout << "Quit message '" << msg->GetOscAddress() << "' received" << std::endl;
	std::string messageString = msg->GetString(0).GetBuffer();
	Configuration config;
	config.SetValue("command", "quit");

	sprite_map->clear();

}

//FIXME: all of these are just placeholders because of library updates
void OscQuitMethod::Method(const WOscMessage& msg, const WOscTimeTag& when, const WOscNetReturn& ra) 
{
	std::cout << "Quit message '" << msg.GetOscAddress() << "' received" << std::endl;
	std::string messageString = msg.GetString(0).GetBuffer();
	Configuration config;
	config.SetValue("command", "quit");

	sprite_map->clear();
}
void OscQuitMethod::Method(const WOscMessage* msg, const WOscTimeTag& when, const WOscNetReturn* ra)
{
	std::cout << "Quit message '" << msg->GetOscAddress() << "' received" << std::endl;
	std::string messageString = msg->GetString(0).GetBuffer();
	Configuration config;
	config.SetValue("command", "quit");

	sprite_map->clear();
}
