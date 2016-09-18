#OSC Stuff

Open Sound Control (OSC) is what we use to communicate to the Pi's over the network. There are numerous files/classes associated with the workings of the OSC server on the Pi, which will be described here.

One important thing to make sure of is that you have the folder *wosclib-1.0* either in same directory as your project or in the include path. WOscLib can be downloaded/installed from [here](http://sourceforge.net/projects/wosclib/). The directory may need to be renamed to *wosclib-1.0*. I have no real idea how these files work but it's important to have them.

The relevant Osc files for this project are:

* OSCServerMethod.h/.cpp
* OscListener.h/.cpp
* OscLaunchMethod.h/.cpp
* OscModifyMethod.h/.cpp
* OscQuitMethod.h/.cpp
* OscExitMethod.h/.cpp
* OscCreateVboMethod.h/.cpp

The only important thing I have to say about OscServerMethod is that all Osc*Methods are subclasses of OscServerMethod.

##OscListener

The OscListener is pretty much your OSC Server. It listens for and handles messages received on the Pi. 

###OscListener::Initialize(..., std::map<std::string, Sprite> *)

This function is called to initialize the server. Hand it a pointer to the sprite map (the same given to SpriteView).

Currently it initializes the five available methods (Exit, Quit, Launch, Modify, CreateVbo) and hands them the sprite map pointer as well. If any new methods are created, this is the appropriate place to initialize them.

##*Method::Method(...)

Each Method subclass must override three functions. These functions are:

* Method(const WOscMessage \*msg, const WOscTimeTag& when, const NetReturnAddress \*ra)
* Method(const WOscMessage& msg, const WOscTimeTag& when, const WOscNetReturn& ra)
* Method(const WOscMessage \*msg, const WOscTimeTag& when, const WOscNetReturn \*ra)


One of these functions is called when the server is sent a message. The class whose Method function gets called depends on the address given in the message (eg. ```"address: "/1/launch"``` would make the server call LaunchMethod::Method). Not really sure when *these* different functions are called exactly and if this is the correct way of doing things, but I've been making each function within an individual *Method class do the same thing - only changing the syntax to take into account whether a parameter is a reference or whatever.

The WOscMessage parameter is the most important thing for us. This parameter contains the yaml string sent to us from a remote client. To get this into a string, we can simply do:

```
std::string messageString = msg->GetString(0).GetBuffer();
```

##OscLaunchMethod

The Launch method is ideally called at launch, but right now it is used any time we want to update the sprite map.

To make sure the Launch method is called when sending a message, in your yaml string set

```
address: "/1/launch"
```

##OscModifyMethod

The Modify method is ideally called when you want to modify anything, but right now calling it would likely cause a crash. It currently utilizes an older method of updating sprites.

To make sure the Modify method is called when sending a message, in your yaml string set

```
address: "/1/transfer"
```

##OscQuitMethod

The Quit Method is ideally called when you want to quit the application. Right now it deletes all the sprites in the sprite_map.

To make sure the Quit method is called when sending a message, in your yaml string set

```
address: "/1/quit"
```

##OscExitMethod

The Exit method shuts down the whole program. 

To make sure the Exit method is called when sending a message, in your yaml string set

```
address: "/1/exit"
```

##OscCreateVboMethod

The Create Vbo Method is used to create vertex buffer objects. 

To make sure the Create Vbo method is called when sending a message, in your yaml string set

```
address: "/1/createvbo"
```

