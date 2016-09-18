OSC-Sprites!

This is a “Sprite Server” intended to be run on one or more Raspberry Pis with displays. It inits an OSC Server on the pi and listens for messages from any clients. These messages tell the server how to render sprites (eg. what color, shape, size, etc.)

Follow the instructions in PiSetup.txt to configure your pi for use with this.

There are two Makefiles since, at the time, the general consensus seemed to be that it would take some sort of config voodoo magic to detect whether the device was specifically a Pi during build. Makefile_Npi is if you want to compile for a non-pi device. Makefile_Pi is for compiling on the Pi.

The executable is placed in the ./bin/ folder. To run, execute the command

	render sprite.yaml

sprite.yaml is the configuration used to set up the OSC-Server and can be modified as needed. 

The display options are unfortunately hard coded right now. You can pass the dimensions into the SpriteView constructor (called in the EnsembleApplication constructor). For the origin position of the display (eg (0,0) if using one display), that is set inside SpriteView::init_engine().

I worked on this project as a part of a small research team back in 2013-2014, so not all code is mine. Files I didn’t create are headed with copyright notices from those who did. 

There is slightly more incomplete-documentation inside the ./doc/ directory.
