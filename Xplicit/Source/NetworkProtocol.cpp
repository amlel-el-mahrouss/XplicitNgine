/*
 * =====================================================================
 *
 *			XplicitNgin
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
    NetworkClient::NetworkClient() : packet(), addr(), stat(NETWORK_STAT_DISCONNECTED) {}
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
        stat = NETWORK_STAT_DISCONNECTED;

        packet.ID = -1;

        for (size_t i = 0; i < XPLICIT_NETWORK_MAX_CMDS; i++)
        {
            packet.CMD[i] = NETWORK_CMD_INVALID;
        }
    }

    bool equals(struct sockaddr_in& lhs, struct sockaddr_in& rhs)
    {
        return lhs.sin_addr.S_un.S_addr == rhs.sin_addr.S_un.S_addr;
    }
}