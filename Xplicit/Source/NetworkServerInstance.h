/*
 * =====================================================================
 *
 *				XplicitNgin C++ Game Engine
 *			Copyright XPX, all rights reserved.
 *
 *			File: NetworkServerInstance.h
 *			Purpose: UDP Server
 *
 * =====================================================================
 */

#pragma once

#include "NetworkInstance.h"
#include "Foundation.h"
#include "Event.h"

namespace Xplicit
{
	class NetworkServerEvent;
	class NetworkServerInstance;

	constexpr const int MAX_CONNECTIONS = 30;

	// the main instance behind the networking.
	class XPLICIT_API NetworkServerInstance final : public Instance
	{
	public:
		explicit NetworkServerInstance(const char* ip);
		virtual ~NetworkServerInstance();

		NetworkServerInstance& operator=(const NetworkServerInstance&) = default;
		NetworkServerInstance(const NetworkServerInstance&) = default;

		virtual INSTANCE_TYPE type() noexcept;
		virtual const char* name() noexcept;
		virtual void update() override;
		const char* dns() noexcept;
		void send() noexcept;

	public:
		size_t size() noexcept;
		NetworkPeer& get(size_t idx) noexcept;

	private:
		std::vector<NetworkPeer> m_clients;
		std::string m_dns;
		Socket m_socket;
		bool m_send;

	private:
		struct sockaddr_in m_server{ };

		friend class NetworkServerTraits;

	};

	class XPLICIT_API NetworkServerTraits
	{
	public:
		static void compute(NetworkServerEvent* env, NetworkServerInstance* instance);

	};

	class XPLICIT_API NetworkServerEvent : public Event
	{
	public:
		NetworkServerEvent(NetworkServerInstance* instance);
		virtual ~NetworkServerEvent();

		NetworkServerEvent& operator=(const NetworkServerEvent&) = default;
		NetworkServerEvent(const NetworkServerEvent&) = default;

		virtual void operator()() override;
		virtual const char* name() noexcept;

	private:
		Ref<NetworkServerInstance*> m_instance;

		friend NetworkServerTraits;

	};
}