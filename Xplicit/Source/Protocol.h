/*
 * =====================================================================
 *
 *				XplicitNgin C++ Game Engine
 *			Copyright XPX, all rights reserved.
 *
 *			File: Protocol.h
 *			Purpose: Game Network Protocol
 *
 * =====================================================================
 */

#pragma once

#include "Foundation.h"

#ifndef XPLICIT_NETWORK_PORT
#define XPLICIT_NETWORK_PORT (60000)
#endif // !XPLICIT_NETWORK_PORT

#define XPLICIT_NETWORK_MAG_0 ('X')
#define XPLICIT_NETWORK_MAG_1 ('N')
#define XPLICIT_NETWORK_MAG_2 ('P')

namespace Xplicit 
{
    enum NETWORK_CMD : int 
    {
        // Network Start/End messages.
        NETWORK_CMD_BEGIN = 9, // start network, handshake
        NETWORK_CMD_STOP, // abort connection
        // PvP
        NETWORK_CMD_DEAD,
        NETWORK_CMD_DAMAGE,
        NETWORK_CMD_SPAWN,
        NETWORK_CMD_ERROR,
        // Script message
        NETWORK_CMD_SCRIPT,
        // Position Update
        NETWORK_CMD_POS,
        // Connection Status
        NETWORK_CMD_ACCEPT,
        NETWORK_CMD_REFUSE,
        NETWORK_CMD_WATCHDOG,
        NETWORK_CMD_ACK,
        // Kick
        NETWORK_CMD_KICK,
        NETWORK_CMD_INVALID = 0xFFFFFFF,
    };

    enum NETWORK_STAT : int
    {
        NETWORK_STAT_CONNECTED,
        NETWORK_STAT_CONNECTING,
        NETWORK_STAT_DISCONNECTED,
    };

    class NetworkPacket
    {
    public:
        char MAG[3]; /* The Packet magic numbers. */

        /* Command and Actor ID. */
        NETWORK_CMD CMD; /* The current network command. */
        int64_t ID; /* NOTE: The server must ignore the ID! */

    public:
        float X; /* X position */
        float Y; /* Y position */
        float Z; /* Z position */

    public:
        char Data[256]; /* Optional Data array */

    };

    class XPLICIT_API NetworkClient final
    {
    public:
        struct sockaddr_in addr; /* the current socket address. */
        NetworkPacket packet; /* the current packet. */
        NETWORK_STAT stat; /* used by the watchdog to prevent unused actor to be unuseable. */

    public:
        NetworkClient();
        ~NetworkClient();

        NetworkClient& operator=(const NetworkClient&) = default;
        NetworkClient(const NetworkClient&) = default;

        bool operator==(const NetworkClient& cl);
        bool operator!=(const NetworkClient& cl);

        void reset() noexcept;

    };

}