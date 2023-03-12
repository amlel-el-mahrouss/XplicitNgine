/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: Protocol.h
 *			Purpose: Lightwieght Game Network Protocol
 *
 * =====================================================================
 */

#pragma once

#include "Foundation.h"

#ifndef XPLICIT_NETWORK_PORT
#define XPLICIT_NETWORK_PORT (60000)
#endif // ifndef XPLICIT_NETWORK_PORT

#define XPLICIT_NETWORK_MAG_0 ('X')
#define XPLICIT_NETWORK_MAG_1 ('N')
#define XPLICIT_NETWORK_MAG_2 ('P')

#define XPLICIT_NETWORK_MAG_COUNT (3U)
#define XPLICIT_NETWORK_MAX_CMDS (32U)

namespace Xplicit 
{

#ifdef XPLICIT_WINDOWS
    using PrivateAddressData = SOCKADDR_IN;
    using Socket = SOCKET;
#else
    typedef struct sockaddr_in PrivateAddressData;
    using Socket = socket;
#endif

    enum NETWORK_CMD : int 
    {
        // Network Start/End messages.
        NETWORK_CMD_BEGIN = 9, // start network, handshake
        NETWORK_CMD_STOP, // abort connection
        // Player versus player
        NETWORK_CMD_DEAD,
        NETWORK_CMD_DAMAGE,
        NETWORK_CMD_SPAWN,
        NETWORK_CMD_ERROR,
        // Forward, Backwards, Right, Left
        NETWORK_CMD_FORWARD,
        NETWORK_CMD_BACKWARDS,
        NETWORK_CMD_LEFT,
        NETWORK_CMD_RIGHT,
        // Connection Status
        NETWORK_CMD_ACCEPT, // handshake has been accepted.
        NETWORK_CMD_REFUSE,
        NETWORK_CMD_WATCHDOG,
        NETWORK_CMD_ACK, // acknowledge
        // Kick
        NETWORK_CMD_KICK,
        NETWORK_CMD_INVALID = 0xFFFFFFF,
    };

    // for the Watchdog protocol
    enum NETWORK_STAT : int
    {
        NETWORK_STAT_CONNECTED,
        NETWORK_STAT_CONNECTING,
        NETWORK_STAT_DISCONNECTED,
    };

    class NetworkPacket
    {
    public:
        char Magic[XPLICIT_NETWORK_MAG_COUNT]; /* magic numbers. */
        NETWORK_CMD CMD[XPLICIT_NETWORK_MAX_CMDS]; /* The current network command. */
        int64_t Health; /* The current actor's health, if sent by an actor */

        float X; /* X position */
        float Y; /* Y position */
        float Z; /* Z position */


    };

    class XPLICIT_API NetworkClient final
    {
    public:
        PrivateAddressData addr; /* the current socket address. */
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

    XPLICIT_API bool equals(struct sockaddr_in& lhs, struct sockaddr_in& rhs);
}

// reserved slots

#define XPLICIT_NETWORK_CMD_FORWARD (0)
#define XPLICIT_NETWORK_CMD_BACKWARD (1)
#define XPLICIT_NETWORK_CMD_LEFT (2)
#define XPLICIT_NETWORK_CMD_RIGHT (3)
#define XPLICIT_NETWORK_CMD_BEGIN (4)
#define XPLICIT_NETWORK_CMD_STOP (5)
#define XPLICIT_NETWORK_CMD_ACK (6)
#define XPLICIT_NETWORK_CMD_WATCHDOG (7)
#define XPLICIT_NETWORK_CMD_KICK (8)
#define XPLICIT_NETWORK_CMD_ACCEPT (9)

#define XPLICIT_LAST_RESERVED_CMD XPLICIT_NETWORK_CMD_ACCEPT