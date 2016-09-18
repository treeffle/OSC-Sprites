/*
 *  OscServer.cpp
 *  GrendlServer
 *
 *  Created by Chris Branton on 11/16/11.
 *  Copyright 2011 Chris Branton. All rights reserved.
 *
 */


///////////////////////////////////////////////////////////////////////////////
// general includes
///////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

#include "unistd.h"

#include "wosclib-1.0/WOscReceiver.h"
#include "wosclib-1.0/WOscNetReturn.h"
#include "wosclib-1.0/WOscReceiverMethod.h"

#include "OscListener.h"
#include "OscExitMethod.h"
#include "OscQuitMethod.h"
#include "OscLaunchMethod.h"
#include "OscModifyMethod.h"
#include "OscCreateVboMethod.h"

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

#define OSC_SERVER_PORT         10000
#define PROTOCOL_UDP            17
#define WOS_MAX_RX_UDP_PACKET   2048

#if OS_IS_LINUX == 1 || OS_IS_MACOSX == 1 || OS_IS_CYGWIN == 1
socklen_t WOS_SIZE_NRA = sizeof(sockaddr_in);
#elif OS_IS_WIN32 == 1
int WOS_SIZE_NRA = sizeof(sockaddr_in);
#else	// FIXME: confirm this last option
int WOS_SIZE_NRA = sizeof(sockaddr_in);
#endif

///////////////////////////////////////////////////////////////////////////////
// OscListener is derived from WOscReceiver
///////////////////////////////////////////////////////////////////////////////

/** Constructor. Sets up the OSC address space, allocates an UDP receive buffer
 * and initializes the exit flag to "false".
 * TODO: move the instructions for this to (parent server) initializers
 *
 */
OscListener::OscListener(): m_exit(false)
{

}

/** Destructor. Removes the address space and deallocates the UDP receive
 * buffer.
 */
OscListener::~OscListener()
{
    // remove address space //
    WOscContainer* as = GetAddressSpace();
    if ( as ) as->RemoveAll();
}

//FIXME: need to reengineer all of this to work with wosc library. Hard-coding for now.
void OscListener::Initialize(const std::list<OscMessage>& oscConfig,std::map<std::string,Container> *cm, std::map<std::string,Sprite> *sm, SpriteView *v) {
//	  for (std::list<int>::iterator it = oscConfig.begin(); it != oscConfig.end(); it++) {
//		  OscMessage currentMsg = *it;
//	  }
	   // setup OSC address space //
	    // containers
        cont_map = cm;
        sprite_map = sm;
        view = v;
	    WOscContainer* rootContainer = new WOscContainer();
	    WOscContainer* grendlContainer = new WOscContainer(rootContainer, "1");

	    // "root" methods
	    new OscExitMethod( rootContainer, this );

	    // "grendl" methods
	    new OscExitMethod (grendlContainer, this );
		new OscQuitMethod (grendlContainer, this, sprite_map);
		new OscLaunchMethod (grendlContainer, this, cont_map, sprite_map);
		new OscModifyMethod (grendlContainer, this, cont_map, &update_map);
        new OscCreateVboMethod (grendlContainer, this, view);

	    SetAddressSpace(rootContainer);

	    WOscString addressSpaceStr = rootContainer->GetAddressSpace();
	    std::cout<<"Registering OSC addresses:\n"<<addressSpaceStr.GetBuffer()<<std::flush;

	    // allocate RX buffer
	    m_rxbuffer = new char[WOS_MAX_RX_UDP_PACKET];

}


void OscListener::Start()	{
	std::cout << "Starting OSC Listener" << std::endl;
}


void OscListener::DefaultMessageHandler	(	const WOscMessage & 	msg,
const WOscNetReturn & 	networkReturnAddress)	{
	std::cout << "OscServer.DefaultMessageHandler: Unmatched OSC address:" << std::endl;
	std::cout << msg.GetOscAddress() << std::endl;
}




/** Checks the exit flag.
 * \returns
 * True if application/serve exit is requested.
 */
bool OscListener::Exit() const
{
    return m_exit;
}

/** Sets the exit flag, which is used to check for server termination.       */
void OscListener::SetExit()
{
	std::cout<< "\nOSC Server exiting...\n" << std::endl;
    m_exit = true;
}



/** Sends data over the network.
 * \param data
 * Pointer to buffer containing the data to be sent.
 *
 * \param dataLen
 * Number of bytes in the data buffer.
 *
 * \param networkReturnAddress
 * The destination network address.
 */
void OscListener::NetworkSend(
							 const char* data,
							 int dataLen,
							 const WOscNetReturn* networkReturnAddress)
{
    const NetReturnAddress* nra = (const NetReturnAddress*)networkReturnAddress;
    int actSend = sendto(
						 m_hSocket,
						 data,
						 dataLen,
						 0,
						 (struct sockaddr*)&nra->m_addr,
						 WOS_SIZE_NRA ) ;
    
    // check if transmission was successful
    if ( dataLen != actSend )
        std::cout << "Error sending packet."<<std::endl ;
}

/** Callback for OSC packets which have caused an exception in the OSC 
 * receiver.
 * \param data
 * Pointer to buffer containing the packet data.
 * \param dataLen
 * Length of the packet data in the buffer.
 * \exception
 * Exception that caused this handler to be called.
 */
void OscListener::HandleOffendingPackets(
										const char* const data,
										int dataLen,
										const WOscException& exception, const WOscNetReturn& returnAddress)
{
    std::cout<<"HandleOffendingPackets."<<std::endl;
}

/** Callback for messages not matching any address in the local OSC address
 * space.
 * \param msg
 * Message which hasn't been processed.
 * \param networkReturnAddress
 * Network origin of the message.
 */
void OscListener::HandleNonmatchedMessages(
										  const WOscMessage* msg,
										  const WOscNetReturn* networkReturnAddress)
{
    std::cout<<"HandleNonmatchedMessages."<<std::endl;
}

void OscListener::HandleOffendingPackets(const char* const data, int dataLen, const WOscException &exception)
{
  std::cout<<"HandleOffendingPackets"<<std::endl;
}
/** Initializes the underlying UDP network layer.
 * \param port
 * Port at which WOscServer should listen for OSC data.
 * \returns
 * Error code on error, WOS_ERR_NO_ERROR on success.
 */
OscListener::WOscServerErrors OscListener::NetworkInit(int port)
{
    int err;
    const int REUSE_TRUE = 1, BROADCAST_TRUE = 1;
    
#if ((OS_IS_WIN32 == 1) || (OS_IS_WIN64 == 1))
    // f***ing windows winsock startup
    WSADATA wsa;
    err = WSAStartup(MAKEWORD(2,0),&wsa);
    if ( err != 0 )
    {
        std::cout << "Error starting Windows socket subsystem."<<std::endl ;
        return WOS_ERR_SOCKET_WSASTART;
    }
#endif  // #if OS_IS_WIN32 == 1
	
    // create socket
    m_hSocket = socket (AF_INET, SOCK_DGRAM, PROTOCOL_UDP);
    if (m_hSocket < 0)
    {
        std::cout << "Create socket error."<<std::endl;
        return WOS_ERR_SOCKET_CREATE;
    }
    
    // initialize server address to localhost:port
    m_serverAddr.m_addr.sin_family         = AF_INET;
    m_serverAddr.m_addr.sin_addr.s_addr    = htonl (INADDR_ANY);
    m_serverAddr.m_addr.sin_port           = htons (port);
    
    // set socket to reuse the address
    err = setsockopt(
					 m_hSocket,
					 SOL_SOCKET,
					 SO_REUSEADDR,
					 (const char*)&REUSE_TRUE,
					 sizeof(REUSE_TRUE));
    if ( err != 0 )
    {
        std::cout << "Error setting socket reuse."<<std::endl ;
        return WOS_ERR_SOCKET_REUSE;
    }
    // enable broadcasting for this socket
    setsockopt(
			   m_hSocket,
			   SOL_SOCKET,
			   SO_BROADCAST,
			   (const char*)&BROADCAST_TRUE,
			   sizeof(BROADCAST_TRUE));
    if ( err != 0 )
    {
        std::cout << "Error setting socket broadcast."<<std::endl ;
        return WOS_ERR_SOCKET_BROADCAST;
    }
    
    // disable blocking, polling is used in this example.
#if OS_IS_LINUX == 1 || OS_IS_MACOSX == 1 || OS_IS_CYGWIN == 1
    err = fcntl(m_hSocket, F_SETFL, O_NONBLOCK);
#elif OS_IS_WIN32 == 1
    // disable blocking (for this example)
    unsigned long val = 1;
    err = ioctlsocket(m_hSocket, FIONBIO , &val);
#endif
    if ( err != 0 )
    {
        std::cout << "Error setting socket unblock."<<std::endl ;
        return WOS_ERR_SOCKET_BLOCK;
    }
    
    // bind for listening
    err = bind (
				m_hSocket,
				(struct sockaddr *)&m_serverAddr.m_addr,
				WOS_SIZE_NRA );
    if ( err != 0 )
    {
        std::cout << "Error socket bind."<<std::endl ;
        return WOS_ERR_SOCKET_BIND;
    }
	
    return WOS_ERR_NO_ERROR;
	
}

/** Stops the network interface.
 * \returns
 * WOS_ERR_NO_ERROR on success.
 */
OscListener::WOscServerErrors OscListener::NetworkHalt()
{
    // close socket... 
#if OS_IS_LINUX == 1 || OS_IS_MACOSX == 1 || OS_IS_CYGWIN == 1
    close(m_hSocket);
#elif OS_IS_WIN32 == 1
    closesocket(m_hSocket);
    WSACleanup();
#endif
    return WOS_ERR_NO_ERROR;
}

/** Check if there is an UDP packet. On packet arrival it will be passed 
 * to the OSC receiver. It returns if there are no more packets available.
 * \returns
 * WOS_ERR_NO_ERROR if no error occurred.
 */
OscListener::WOscServerErrors OscListener::CheckForPackets()
{
    // as long there are packets waiting
    bool morePackets = 1;
    while(morePackets)
    {
        // has to be allocated on heap, because is reference
        // counted
        NetReturnAddress* nra = new NetReturnAddress;
        nra->m_addr.sin_family         = AF_INET;
        nra->m_addr.sin_addr.s_addr    = htonl (INADDR_ANY);
        nra->m_addr.sin_port           = htons (0);
		
        // receive from network
        int nReceived = recvfrom(
								 m_hSocket,
								 m_rxbuffer,
								 WOS_MAX_RX_UDP_PACKET,
								 0,
								 (struct sockaddr*)&nra->m_addr,
								 (socklen_t*)&WOS_SIZE_NRA);
		
        // nReceived is the number of bytes actually read from 
        // network interface
        if (nReceived > 0)
        {
            NetworkReceive ( m_rxbuffer, nReceived, nra);
        }
        else
        {
            delete nra;
            morePackets = false;    // stop receiving process
        }
    }
    return WOS_ERR_NO_ERROR;
}



