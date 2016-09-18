/*
 *  OscServer.h
 *  GrendlServer
 *
 *  Created by Chris Branton on 11/16/11.
 *  Copyright 2011 Chris Branton. All rights reserved.
 *
 */

///////////////////////////////////////////////////////////////////////////////
#ifndef OSC_SERVER_H
#define OSC_SERVER_H



#include "wosclib-1.0/WOscReceiver.h"
//#include "WOscReceiver.h"

#include "NetReturnAddress.h"
#include "Configuration.h"
#include "sprite.h"

/**
 * This is the central class which implements a full featured OSC server
 * including the network-layer implementation.
 *
 * The underlying wosclib library uses containers and dedicated handlers
 * to set up the OSC address space. A WOscContainer at the root will
 * contain other containers and/or OscReceiverMethod(s). Each container or
 * method binds a string to the OSC address space. For example,
 * a method with a methodName "msg" contained by a root
 * container with the name "app" would respond to the OSC message "/app/msg".
 *
 * The member function SetAddressSpace binds the root container and all
 * of its hierarchy of handlers to the OSC receiver. Setting the root container
 * to NULL disables message response.
 *
 */
class OscListener  :public WOscReceiver
{
public:
    OscListener();
    virtual ~OscListener();

    void SetExit();
    void Initialize(const std::list<OscMessage>& oscConfig, std::map<std::string,Container>*, std::map<std::string,Sprite>*, SpriteView *);
    void Start();


    void Stop() {SetExit();}
    bool Exit() const ;

    /** Errors which can occur in the network layer. */
    enum WOscServerErrors
    {
        WOS_ERR_NO_ERROR         =  0, /**< No error.                        */
        WOS_ERR_SOCKET_CREATE    = -1, /**< Error when creating the socket.  */
        WOS_ERR_SOCKET_BIND      = -2, /**< Error when binding the local
                                        * address and port to the socket.    */
        WOS_ERR_SOCKET_REUSE     = -3, /**< Error when setting the socket-
                                        * reuse option.                      */
        WOS_ERR_SOCKET_BROADCAST = -4, /**< Error when setting the socket-
                                        * broadcast option.                  */
        WOS_ERR_SOCKET_BLOCK     = -5, /**< Error when setting the socket to
                                        * the non-blocking mode.             */
        WOS_ERR_SOCKET_WSASTART  = -6, /**< Error when starting the Windoze
                                        * winsock subsystem (Windows builds
                                        * only).                             */
    };

    WOscServerErrors NetworkInit(int port);
    WOscServerErrors NetworkHalt();
    WOscServerErrors CheckForPackets();
	
    virtual void NetworkSend(
							 const char* data,
							 int dataLen,
							 const WOscNetReturn* networkReturnAddress);
	
	void addOscHandlers (std::list<OscMessage>& messageSpecs);


protected:
    void HandleOffendingPackets(
										const char* const data,
										int dataLen,
										const WOscException& exception, const WOscNetReturn&);
	void HandleOffendingPackets(
										const char* const data,
										int dataLen,
										const WOscException& exception);


    void HandleNonmatchedMessages(
										  const WOscMessage* msg,
										  const WOscNetReturn* networkReturnAddress);

    void DefaultMessageHandler	(	const WOscMessage & 	msg, const WOscNetReturn & 	networkReturnAddress);

    virtual int 	NetworkTransmit (const WOscPacket &packet, const WOscNetReturn *ra) {
    	std::cout << "OscServer.NetworkTransmit"<< std::endl;
    	return ERR_NO_ERROR;
    }

private:
    /** The network address of the server when initialized.                  */
    NetReturnAddress     m_serverAddr;
    /** Databuffer for received data.                                        */
    char*                   m_rxbuffer;
    /** The socket handler of this OSC server.                               */
    int                     m_hSocket;
    /** Flag set when termination of the application or server is desired.   */
    bool                    m_exit;
	std::map<std::string,Container>* cont_map;
	std::map<std::string,Sprite>* sprite_map;
	std::map<std::string,Container> update_map;
    SpriteView *view;

};

#endif

