/*
 *  OscTransferMethod.cpp
 *  GrendlServer
 *
 *  Created by Chris Branton on 11/18/11.
 *  Copyright 2011 Chris Branton. All rights reserved.
 *
 */

#include "OscModifyMethod.h"
//#include "Grendl1.h"

/** Constructor. Sets the method name and info in the base class.            */
OscModifyMethod::OscModifyMethod( WOscContainer* parent, OscListener* receiverContext, std::map<std::string,Container> *cm, std::map<std::string,Container> *um)
	:OscServerMethod(parent, receiverContext, "transfer", "This method executes transfer scripts"),
	 cont_map(cm),
	 update_map(um)
{

}

/** The exit method. Terminates the WOscServer by setting the exit flag.     */
void OscModifyMethod::Method(const WOscMessage *msg, const WOscTimeTag& when, const NetReturnAddress* ra)
{
	std::cout << "\nTransfer message '" << msg->GetOscAddress() << "' received" << std::endl;
	std::string messageString = msg->GetString(0).GetBuffer();
	Configuration config;
	config.ParseContainers(messageString, cont_map, update_map);
	config.SetValue("command", "transfer");
}


void OscModifyMethod::Method(const WOscMessage& msg, const WOscTimeTag& when, const WOscNetReturn& ra) 
{
	std::cout << "\nTransfer message '" << msg.GetOscAddress() << "' received" << std::endl;
	std::string messageString = msg.GetString(0).GetBuffer();
	Configuration config;
	config.ParseContainers(messageString, cont_map, update_map);
	config.SetValue("command", "transfer");
}

void OscModifyMethod::Method(const WOscMessage* msg, const WOscTimeTag& when, const WOscNetReturn* ra) {
	std::cout << "\nTransfer message '" << msg->GetOscAddress() << "' received" << std::endl;
	std::string messageString = msg->GetString(0).GetBuffer();
	Configuration config;
	config.ParseContainers(messageString, cont_map, update_map);
	config.SetValue("command", "transfer");
}
