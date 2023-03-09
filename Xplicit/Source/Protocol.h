/*
 * =====================================================================
 *
 *						XplicitNgin C++ Game Engine
 *			Copyright XPX, all rights reserved.
 *
 *			File: Protocol.h
 *			Purpose: Game Network Protocol
 *
 * =====================================================================
 */

#pragma once

#include "Foundation.h"

#ifndef NETWORK_PORT
#define NETWORK_PORT (60000)
#endif // !NETWORK_PORT

namespace Xplicit {
    enum NETWORK_CMD : int 
    {
        NETWORK_CMD_BEGIN = 9, // start network, handshake
        NETWORK_CMD_STOP, // abort connection
        // PvP
        NETWORK_CMD_DEAD,
        NETWORK_CMD_DAMAGE,
        NETWORK_CMD_SPAWN,
        NETWORK_CMD_ERROR,
        // Tools
        NETWORK_CMD_TOOL_MOUNT,
        NETWORK_CMD_TOOL_UNMOUNT,
        NETWORK_CMD_TOOL_USE,
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
        NETWORK_CMD CMD; /* The Command. */
        int64_t ID;

    public:
        float X; /* X position */
        float Y; /* Y position */
        float Z; /* Z position */

    };

    class XPLICIT_API NetworkClient final
    {
    public:
        struct sockaddr_in addr;
        NetworkPacket packet;
        NETWORK_STAT stat;

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