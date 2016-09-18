/*
 *  OSCServerMethod.h
 *  GrendlServer
 *
 *  Created by Chris Branton on 11/16/11.
 *  Copyright 2011 Chris Branton. All rights reserved.
 *
 */

#ifndef OSC_SERVER_METHOD_H
#define OSC_SERVER_METHOD_H

///////////////////////////////////////////////////////////////////////////////
#include "OscListener.h"
#include "wosclib-1.0/WOscReceiverMethod.h"
//class OscServer;

/** All methods used in WOscServer are derived from this class. In a real
 * project the methods should be implemented in a header file to inline them
 * and eliminate the c++ overhead.
 *
 * This class avoids some pointer-casting but wouldn't be necessary but makes
 * all methods used type-safe.
 */
class OscServerMethod: public  WOscReceiverMethod
{
public:
    OscServerMethod(WOscContainer* parent, OscListener* receiverContext, const char* methodName, const char* methodDescription);
    virtual void Method(const WOscMessage *msg, const WOscTimeTag& when, const NetReturnAddress* ra) = 0;
	virtual void Method(const WOscMessage& msg, const WOscTimeTag& when, const WOscNetReturn& ra) = 0;
    virtual OscListener* GetContext();
	
protected:
 /*   virtual void Method(
						const WOscMessage *message,
						const WOscTimeTag& when,
						const WOscNetReturn* networkReturnAddress);*/
	OscListener* m_server;
};

#endif
