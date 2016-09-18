/*
 *  NetReturnAddress.h
 *  GrendlServer
 *
 *  Created by Chris Branton on 11/16/11.
 *  Copyright 2011 Chris Branton. All rights reserved.
 *
 */

/** The network return address. Through multiple inheritance this object
 * inherits the functionality required by WOscLib to keep it alive until
 * all messages of a bundle are processed (reference counting) and the
 * networking data structure which contains the actual ip/port information.
 *
 */
#ifndef NET_RETURN_ADDRESS_H
#define NET_RETURN_ADDRESS_H

#include <sys/socket.h>
//#include <sys/types.h>
#include <netinet/in.h>
#include <cstring>
#include "wosclib-1.0/WOscNetReturn.h"

class NetReturnAddress: public WOscNetReturn
{
public:
    struct sockaddr_in  m_addr;
    NetReturnAddress()
    {
        memset(&m_addr, 0, sizeof(m_addr));
        m_addr.sin_family = AF_INET;
    }
    NetReturnAddress(const NetReturnAddress& rhs)
    {
        this->m_addr = rhs.m_addr;
    }
    NetReturnAddress& operator=(const NetReturnAddress& rhs)
    {
        this->m_addr = rhs.m_addr;
        return *this;
    }
};

#endif
