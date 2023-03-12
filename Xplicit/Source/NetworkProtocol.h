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
#include "ApplicationContext.h"

#ifndef XPLICIT_NETWORK_PORT
#define XPLICIT_NETWORK_PORT (30002)
#endif // ifndef XPLICIT_NETWORK_PORT

#define XPLICIT_NETWORK_MAG_0 ('X')
#define XPLICIT_NETWORK_MAG_1 ('G')
#define XPLICIT_NETWORK_MAG_2 ('P')

#define XPLICIT_NETWORK_MAG_COUNT (3U)
#define XPLICIT_NETWORK_MAX_CMDS (16U)

namespace Xplicit 
{

#ifdef XPLICIT_WINDOWS
    using PrivateAddressData = SOCKADDR_IN;
    using Socket = SOCKET;
#else
    typedef struct sockaddr_in PrivateAddressData;
    using Socket = socket;
#endif

    enum NETWORK_CMD : int16_t
    {
        // Network Start/End messages.
        NETWORK_CMD_BEGIN = 15, // start network, handshake
        NETWORK_CMD_STOP, // abort connection
        // Player versus player
        NETWORK_CMD_DEAD,
        NETWORK_CMD_DAMAGE,
        NETWORK_CMD_SPAWN,
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
        // Invalid
        NETWORK_CMD_INVALID,
        NETWORK_CMD_COUNT,
    };

    // for the Watchdog protocol
    enum NETWORK_STAT : int16_t
    {
        NETWORK_STAT_CONNECTED,
        NETWORK_STAT_DISCONNECTED,
        NETWORK_STAT_COUNT,
    };

    class NetworkPacket
    {
    public:
        char Magic[XPLICIT_NETWORK_MAG_COUNT]; /* magic numbers. */
        NETWORK_CMD CMD[XPLICIT_NETWORK_MAX_CMDS]; /* The current network command. */

        int64_t Health; /* The current actor's health, if sent by an actor */
        int64_t ID; /* Clientside: the local actor targeted (for position commands only) */

        float X; /* X position */
        float Y; /* Y position */
        float Z; /* Z position */


    };

    class XPLICIT_API NetworkPeer final
    {
    public:
        class XPLICIT_API UniqueAddress final
        {
        public:
            UniqueAddress()
                : uuid(UUIDFactory::version<4>()), name("Peer")
            {}

            ~UniqueAddress() = default;

            UniqueAddress& operator=(const UniqueAddress&) = default;
            UniqueAddress(const UniqueAddress&) = default;

            uuids::uuid uuid;
            std::string name;

        };

    public:
        UniqueAddress unique_addr; /* unique network address of this peer */
        PrivateAddressData addr; /* current socket address. */
        NetworkPacket packet; /* current packet. */
        NETWORK_STAT stat; /* current network status */
        int64_t id; /* peer id */

    public:
        NetworkPeer();
        ~NetworkPeer();

        NetworkPeer& operator=(const NetworkPeer&) = default;
        NetworkPeer(const NetworkPeer&) = default;

        bool operator==(const NetworkPeer& cl);
        bool operator!=(const NetworkPeer& cl);

    public:
        void reset() noexcept;

    };

    XPLICIT_API bool equals(PrivateAddressData& lhs, PrivateAddressData& rhs);
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