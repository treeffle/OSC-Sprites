#include "OSCServerMethod.h"
#include "wosclib-1.0/WOscContainer.h"
#include "wosclib-1.0/WOscMessage.h"
#include "wosclib-1.0/WOscMethod.h"

/** This message contains vertex/element information for creating a new vertex buffer object
 */

//treeffle: I think this works but it might not. (9/16/16) 
class OscCreateVboMethod:
public OscServerMethod
{
public:
    OscCreateVboMethod(WOscContainer* parent, OscListener* receiverContext, SpriteView*);
    virtual void Method(const WOscMessage *msg, const WOscTimeTag& when, const NetReturnAddress* ra);
    virtual void Method(const WOscMessage* msg, const WOscTimeTag& when, const WOscNetReturn* ra);
    virtual void Method(const WOscMessage& msg, const WOscTimeTag& when, const WOscNetReturn& ra);
    SpriteView *view;;
};
