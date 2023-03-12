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
    NetworkPeer::NetworkPeer() : packet(), addr(), stat(NETWORK_STAT_COUNT), unique_addr(), id(-1) {}
    NetworkPeer::~NetworkPeer() {}
    
    bool NetworkPeer::operator==(const NetworkPeer& cl)
    {
        return cl.addr.sin_addr.S_un.S_addr == this->addr.sin_addr.S_un.S_addr;
    }

    bool NetworkPeer::operator!=(const NetworkPeer& cl)
    {
        return cl.addr.sin_addr.S_un.S_addr != this->addr.sin_addr.S_un.S_addr;
    }

    void NetworkPeer::reset() noexcept
    {
        packet.ID = -1;
        id = -1;

        stat = NETWORK_STAT_DISCONNECTED;

        memset(&packet.CMD, 0, XPLICIT_NETWORK_MAX_CMDS);
        memset(&addr, 0, sizeof(PrivateAddressData));
    }

    bool equals(PrivateAddressData& lhs, PrivateAddressData& rhs)
    {
#ifdef XPLICIT_WINDOWS
        return lhs.sin_addr.S_un.S_addr == rhs.sin_addr.S_un.S_addr;
#endif
    }
}