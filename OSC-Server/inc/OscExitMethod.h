/*
 *  OscExitMethod.h
 *  GrendlServer
 *
 *  Created by Chris Branton on 11/16/11.
 *  Copyright 2011 Chris Branton. All rights reserved.
 *
 */

///////////////////////////////////////////////////////////////////////////////

#include "OSCServerMethod.h"
#include "wosclib-1.0/WOscContainer.h"
#include "wosclib-1.0/WOscMessage.h"

/** This message sets an exit-request, what results that the main while
 * loop condition evaluates to false and the application exits.
 */
class OscExitMethod: public OscServerMethod
{
public:
    OscExitMethod(WOscContainer* parent, OscListener* receiverContext);
    virtual void Method(const WOscMessage* msg, const WOscTimeTag& when, const NetReturnAddress* ra);
	virtual void Method(const WOscMessage& msg, const WOscTimeTag& when, const WOscNetReturn& ra);
	virtual void Method(const WOscMessage* msg, const WOscTimeTag& when, const WOscNetReturn* ra);
};

