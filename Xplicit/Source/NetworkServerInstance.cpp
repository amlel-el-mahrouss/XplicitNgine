/*
 * =====================================================================
 *
 *				XplicitNgin C++ Game Engine
 *			Copyright XPX, all rights reserved.
 *
 *			File: NetworkServerInstance.cpp
 *			Purpose: UDP Server
 * =====================================================================
 */

#include "NetworkServerInstance.h"

namespace Xplicit
{
	static void xplicit_set_ioctl(SOCKET sock)
	{
#ifdef XPLICIT_WINDOWS
		u_long ul = 1;
		ioctlsocket(sock, FIONBIO, &ul);

#else
#pragma error("DEFINE ME ServerInstance.cpp")
#endif
	}

	NetworkServerInstance::NetworkServerInstance(const char* ip)
		: m_socket(INVALID_SOCKET), m_dns(ip), m_send(false), m_server()
	{
#ifndef _NDEBUG
		std::string message;
		message += "Class NetworkServerInstance, Epoch: ";
		message += std::to_string(xplicit_get_epoch());

		XPLICIT_INFO(message);
#endif

#ifdef XPLICIT_WINDOWS
		// create ipv4 udp socket.
		m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

		if (m_socket == SOCKET_ERROR)
			throw NetworkError(NETERR_INTERNAL_ERROR);

		xplicit_set_ioctl(m_socket);

		memset(&m_server, 0, sizeof(struct sockaddr_in));

		m_server.sin_family = AF_INET;
		inet_pton(AF_INET, ip, &m_server.sin_addr.S_un.S_addr);
		m_server.sin_port = htons(XPLICIT_NETWORK_PORT);

		auto ret_bind = bind(m_socket, reinterpret_cast<SOCKADDR*>(&m_server), sizeof(m_server));
#else
#pragma error("DEFINE ME ServerInstance.cpp")
#endif

		if (ret_bind == SOCKET_ERROR)
			throw NetworkError(NETERR_INTERNAL_ERROR);
		
		// Let's pre-allocate the clients.
		// So we don't have to.
		for (size_t i = 0; i < MAX_CONNECTIONS; i++)
		{
			NetworkClient cl{};
			memset(&cl, 0, sizeof(NetworkClient));

			memset(cl.packet.CMD, NETWORK_CMD_INVALID, XPLICIT_NETWORK_MAX_CMDS);

			m_clients.push_back(cl);
		}
	}

	size_t NetworkServerInstance::size() noexcept { return m_clients.size(); }
	NetworkClient& NetworkServerInstance::get(size_t idx) noexcept { return m_clients[idx]; }

	const char* NetworkServerInstance::name() noexcept { return ("NetworkServerInstance"); }

	NetworkServerInstance::INSTANCE_TYPE NetworkServerInstance::type() noexcept { return INSTANCE_NETWORK; }

	void NetworkServerInstance::update() {}

	NetworkServerInstance::~NetworkServerInstance()
	{
		// don't print that in release builds.
#ifndef _NDEBUG
		std::string message;
		message += "Class NetworkServerInstance::~NetworkServerInstance(), Epoch: ";
		message += std::to_string(xplicit_get_epoch());

		XPLICIT_INFO(message);
#endif

#ifdef XPLICIT_WINDOWS
		if (shutdown(m_socket, SD_BOTH) == SOCKET_ERROR)
			closesocket(m_socket);
#else
#pragma error("DEFINE ME ServerInstance.cpp")
#endif

		WSACleanup();
	}

	const char* NetworkServerInstance::dns() noexcept { return m_dns.c_str(); }

	// mark the packet queue (m_clients) to be sent over the network.
	void NetworkServerInstance::send() noexcept 
	{
		if (!m_send)
			m_send = true; 
	}

	// we need a way to tell which client is who.
	void NetworkServerTraits::recv(NetworkServerEvent* env, NetworkServerInstance* instance)
	{
		if (instance)
		{
			// either send or recieve.
			if (instance->m_send)
			{
				for (size_t i = 0; i < instance->m_clients.size(); i++)
				{
					instance->m_clients[i].packet.Magic[0] = XPLICIT_NETWORK_MAG_0;
					instance->m_clients[i].packet.Magic[1] = XPLICIT_NETWORK_MAG_1;
					instance->m_clients[i].packet.Magic[2] = XPLICIT_NETWORK_MAG_2;

#ifdef XPLICIT_WINDOWS
					::sendto(instance->m_socket, (const char*)&
						instance->m_clients[i].packet, sizeof(NetworkPacket), 0, 
						(struct sockaddr*)&instance->m_clients[i].addr, sizeof(struct sockaddr_in));
#else
#pragma error("DEFINE ME ServerInstance.cpp")
#endif
				}
				
				instance->m_send = false;
			}
			else
			{
				int sz = sizeof(struct sockaddr_in);

				for (size_t i = 0; i < instance->m_clients.size(); ++i)
				{
#ifdef XPLICIT_WINDOWS
					int sz = sizeof(struct sockaddr_in);

					::recvfrom(instance->m_socket, (char*)&
						instance->m_clients[i].packet, sizeof(NetworkPacket), 0, 
						(struct sockaddr*)&instance->m_clients[i].addr, &sz);
#else
#pragma error("DEFINE ME ServerInstance.cpp")
#endif

					if (instance->m_clients[i].packet.Magic[0] != XPLICIT_NETWORK_MAG_0 || 
						instance->m_clients[i].packet.Magic[1] != XPLICIT_NETWORK_MAG_1 ||
						instance->m_clients[i].packet.Magic[2] != XPLICIT_NETWORK_MAG_2)
					{
						for (size_t y = 0; y < XPLICIT_NETWORK_MAX_CMDS; y++)
						{
							instance->m_clients[i].packet.CMD[y] = NETWORK_CMD_INVALID;
						}
					}

					if (sz < 0)
						return;
				}
			}
		}
	}

	NetworkServerEvent::NetworkServerEvent(NetworkServerInstance* instance)
		: m_instance(instance)
	{
		if (!instance)
			throw NetworkError(NETERR_INTERNAL_ERROR);

#ifndef _NDEBUG
		std::string message;
		message += "Class NetworkServerInstance, Epoch: ";
		message += std::to_string(xplicit_get_epoch());

		XPLICIT_INFO(message);
#endif
	}

	NetworkServerEvent::~NetworkServerEvent() {}

	void NetworkServerEvent::operator()()
	{
		NetworkServerTraits::recv(this, m_instance.get());
	}

	const char* NetworkServerEvent::name() noexcept { return ("NetworkServerEvent"); }
}