/*
 * =====================================================================
 *
 *				XplicitNgin C++ Game Engine
 *			Copyright XPX, all rights reserved.
 *
 *			File: NetworkInstance.h
 *			Purpose: UDP Networking API
 *
 * =====================================================================
 */

#pragma once

#include "Foundation.h"

#include "Event.h"
#include "Instance.h"
#include "NetworkProtocol.h"

#ifdef XPLICIT_WINDOWS
#define XPLICIT_NET_ASSERT(EXPR) if (!(EXPR)) { MessageBoxA(nullptr, #EXPR, "C++ Network Exception", 0); std::terminate(); }
#else
#define XPLICIT_NET_ASSERT(EXPR) if (!(EXPR)) { std::string err = #EXPR; err += "C++ Network Exception"; XPLICIT_CRITICAL(err); std::terminate(); }
#endif


#ifndef XPLICIT_SLEEP
#define XPLICIT_SLEEP Sleep
#endif

namespace Xplicit {
    enum NETWORK_ERR : int
    {
        NETERR_BAD_CHALLENGE,
        NETERR_PING_TOO_HIGH,
        NETERR_INTERNAL_ERROR,
        NETERR_BAN,
        NETERR_KICK,
        NETERR_COUNT,
    };

    class XPLICIT_API NetworkError final : public std::runtime_error 
    {
    public:
        NetworkError(const int what = NETERR_INTERNAL_ERROR) : std::runtime_error("Network Error") 
        { 
            m_iErr = WSAGetLastError(); 
            std::string err = "Error: ";
            err += std::to_string(m_iErr);
        }

        virtual ~NetworkError() = default; // let the ABI define that.

        NetworkError& operator=(const NetworkError&) = default;
        NetworkError(const NetworkError&) = default;

    public:
        int error() const noexcept { return m_iErr; }

    private:
        int m_iErr{ 200 };

    };

    class XPLICIT_API NetworkInstance : public Instance
    {
    public:
        NetworkInstance();

        virtual ~NetworkInstance();

        NetworkInstance& operator=(const NetworkInstance& ctx) = default;
        NetworkInstance(const NetworkInstance& ctx) = default;

        virtual const char* name() noexcept override { return ("NetworkInstance"); }
        virtual INSTANCE_TYPE type() noexcept override { return INSTANCE_NETWORK; }

        virtual void update() override;

        void reset() noexcept;
        bool connect(const char* ip);
        bool send(NetworkPacket& cmd);
        bool read(NetworkPacket& packet);

        NetworkPacket& get() noexcept;

    private:
        SOCKET m_socket;
        NetworkPacket m_packet;
        struct sockaddr_in m_inaddr; // socket descriptor.

        friend class NetworkEvent;

    };

    class XPLICIT_API NetworkEvent : public Event
    {
    public:
        NetworkEvent() {}
        virtual ~NetworkEvent() {}

        NetworkEvent& operator=(const NetworkEvent&) = default;
        NetworkEvent(const NetworkEvent&) = default;

        virtual void operator()();

        virtual const char* name() noexcept { return ("NetworkEvent"); }

    };
}