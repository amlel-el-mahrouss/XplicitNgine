/*
 * =====================================================================
 *
 *				XplicitNgin C++ Game Engine
 *			Copyright XPX, all rights reserved.
 *
 *			File: Protocol.cpp
 *			Purpose: Game Network Protocol
 *
 * =====================================================================
 */

#include "NetworkProtocol.h"

namespace Xplicit
{
    NetworkClient::NetworkClient() : packet(), addr() {}
    NetworkClient::~NetworkClient() {}
    
    bool NetworkClient::operator==(const NetworkClient& cl)
    {
        return cl.addr.sin_addr.S_un.S_addr == this->addr.sin_addr.S_un.S_addr;
    }

    bool NetworkClient::operator!=(const NetworkClient& cl)
    {
        return cl.addr.sin_addr.S_un.S_addr != this->addr.sin_addr.S_un.S_addr;
    }

    void NetworkClient::reset() noexcept
    {
        packet.CMD = NETWORK_CMD_INVALID;
        packet.ID = -1;

        memset(&addr, 0, sizeof(struct sockaddr_in));
    }
}