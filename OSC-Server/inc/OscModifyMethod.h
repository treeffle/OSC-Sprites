/*
 *  OscTransferMethod.h
 *  GrendlServer
 *
 *  Created by Chris Branton on 11/18/11.
 *  Copyright 2011 Chris Branton. All rights reserved.
 *
 */

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
class OscModifyMethod:
public OscServerMethod
{
public:
    OscModifyMethod(WOscContainer* parent, OscListener* receiverContext, std::map<std::string,Container>*, std::map<std::string,Container>*);
    virtual void Method(const WOscMessage *msg, const WOscTimeTag& when, const NetReturnAddress* ra);
    virtual void Method(const WOscMessage& msg, const WOscTimeTag& when, const WOscNetReturn& ra);
	virtual void Method(const WOscMessage* msg, const WOscTimeTag& when, const WOscNetReturn* ra);
	std::map<std::string,Container>* cont_map;
	std::map<std::string,Container>* update_map;
};

