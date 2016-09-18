/*
 *  OscLaunchMethod.cpp
 *  GrendlServer
 *
 *  Created by Chris Branton on 11/18/11.
 *  Copyright 2011 Chris Branton. All rights reserved.
 *
 */

#include "OscLaunchMethod.h"



/** Constructor. Sets the method name and info in the base class.            */
OscLaunchMethod::OscLaunchMethod( WOscContainer* parent, OscListener* receiverContext, std::map<std::string,Container> *cm, std::map<std::string,Sprite> *sm)
	:OscServerMethod(parent, receiverContext, "launch", "This method executes  scripts"),
     cont_map(cm),
     sprite_map(sm)
{

}

/** The launch method. */
void OscLaunchMethod::Method(const WOscMessage *msg, const WOscTimeTag& when, const NetReturnAddress* ra)
{
	std::cout << "\nLaunch message '" << msg->GetOscAddress() << "' received" << std::endl;
	std::string messageString = msg->GetString(0).GetBuffer();
	Configuration config;
	config.ParseSprites(messageString, sprite_map);
	config.SetValue("command", "launch");
}

void OscLaunchMethod::Method(const WOscMessage& msg, const WOscTimeTag& when, const WOscNetReturn& ra) 
{
	std::cout << "\nLaunch message '" << msg.GetOscAddress() << "' received" << std::endl;
	std::string messageString = msg.GetString(0).GetBuffer();
	Configuration config;
	config.ParseSprites(messageString, sprite_map);
	config.SetValue("command", "launch");
}

void OscLaunchMethod::Method(const WOscMessage* msg, const WOscTimeTag& when, const WOscNetReturn* ra) 
{
	std::cout << "\nLaunch message '" << msg->GetOscAddress() << "' received" << std::endl;
	std::string messageString = msg->GetString(0).GetBuffer();
	Configuration config;
	config.ParseSprites(messageString, sprite_map);
	config.SetValue("command", "launch");
}
