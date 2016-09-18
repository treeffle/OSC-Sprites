
#include "OscCreateVboMethod.h"



/** Constructor. Sets the method name and info in the base class.            */
OscCreateVboMethod::OscCreateVboMethod( WOscContainer* parent, OscListener* receiverContext, SpriteView *v)
	:OscServerMethod(parent, receiverContext, "createvbo", "This method executes  scripts"),
     view(v)
{

}

/** The launch method. */
void OscCreateVboMethod::Method(const WOscMessage *msg, const WOscTimeTag& when, const NetReturnAddress* ra)
{
	std::string messageString = msg->GetString(0).GetBuffer();
	Configuration config;
	config.ParseVertexData(messageString, view);
	config.SetValue("command", "launch");
}

void OscCreateVboMethod::Method(const WOscMessage& msg, const WOscTimeTag& when, const WOscNetReturn& ra) 
{

	std::string messageString = msg.GetString(0).GetBuffer();
	Configuration config;
	config.ParseVertexData(messageString, view);
	config.SetValue("command", "launch");
}

void OscCreateVboMethod::Method(const WOscMessage* msg, const WOscTimeTag& when, const WOscNetReturn* ra) 
{

	std::string messageString = msg->GetString(0).GetBuffer();
	Configuration config;
	config.ParseVertexData(messageString, view);
	config.SetValue("command", "launch");
}
