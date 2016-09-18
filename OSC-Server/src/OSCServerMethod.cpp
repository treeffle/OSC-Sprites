/*
 *  OSCServerMethod.cpp
 *  GrendlServer
 *
 *  Created by Chris Branton on 11/16/11.
 *  Copyright 2011 Chris Branton. All rights reserved.
 *
 */

#include "OSCServerMethod.h"

///////////////////////////////////////////////////////////////////////////////
// WOscServerMethod
///////////////////////////////////////////////////////////////////////////////

/** Constructor. Initializes WOscReceiverMethod base class. */
OscServerMethod::OscServerMethod(WOscContainer* parent, OscListener* receiverContext, const char* methodName, const char* methodDescription)
	:WOscReceiverMethod(parent, (WOscReceiver*)receiverContext, methodName, methodDescription)
{
	m_server=receiverContext;
}

/** Type-safe access to receiver object, which is owning this object.        */
OscListener* OscServerMethod::GetContext()
{
    return (OscListener*)WOscReceiverMethod::GetContext();
}

/** Type-save method interface. All methods inherited from this class get
 * the correct NetReturnAddress pointers without caring about the
 * system below.
 */
void OscServerMethod::Method(const WOscMessage *msg, const WOscTimeTag& when, const NetReturnAddress* ra)
{
    Method(msg, when, (NetReturnAddress*) ra);
}

//void OscServerMethod::Method(const WOscMessageRx& msg, const WOscTimeTag& when,
//		const WOscNetReturn& ra) {std::cout << "OSCServer method" << std::endl; };

