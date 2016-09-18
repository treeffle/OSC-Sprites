/*
 *  OscQuitMethod.h
 *  GrendlServer
 *
 *  Created by Chris Branton on 11/17/11.
 *  Copyright 2011 Chris Branton. All rights reserved.
 *
 */

#ifndef _OSC_QUIT_METHOD_H__
#define _OSC_QUIT_METHOD_H__

#include "OSCServerMethod.h"
#include "wosclib-1.0/WOscContainer.h"
#include "wosclib-1.0/WOscMessage.h"

/** 
 * This message runs the quit scripts.
 */
class OscQuitMethod:
public OscServerMethod
{
public:
    OscQuitMethod(WOscContainer* parent, OscListener* receiverContext, std::map<std::string,Sprite>*);
    virtual void Method(const WOscMessage *msg, const WOscTimeTag& when, const NetReturnAddress* ra);
    virtual void Method(const WOscMessage& msg, const WOscTimeTag& when, const WOscNetReturn& ra);
	virtual void Method(const WOscMessage* msg, const WOscTimeTag& when, const WOscNetReturn* ra);
	std::map<std::string,Sprite> *sprite_map;
};

#endif
