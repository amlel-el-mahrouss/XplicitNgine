/*
 * =====================================================================
 *
 *						XplicitNgin C++ Game Engine
 *			Copyright XPX Technologies all rights reserved.
 *
 *			File: ServerInstance.cpp
 *			Purpose: UDP Networking API
 *
 * =====================================================================
 */

#include "ServerInstance.h"
#include "Actor.h"

namespace Xplicit
{
	static void xplicit_set_ioctl(SOCKET sock)
	{

		// enable non blocking i/o
		u_long ul = 1;
		ioctlsocket(sock, FIONBIO, &ul);
	}

	NetworkServerInstance::NetworkServerInstance(const char* ip)
		: m_socket(INVALID_SOCKET), m_dns(ip), m_send(false), m_server()
	{
#ifndef _NDEBUG
		std::string message;
		message += "Class NetworkServerInstance, Epoch: ";
		message += std::to_string(get_epoch());

		XPLICIT_INFO(message);
#endif

		// create ipv4 udp socket.
		m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

		if (m_socket == SOCKET_ERROR)
			throw NetworkError(NETERR_INTERNAL_ERROR);

		xplicit_set_ioctl(m_socket);

		memset(&m_server, 0, sizeof(struct sockaddr_in));

		m_server.sin_family = AF_INET;
		inet_pton(AF_INET, ip, &m_server.sin_addr.S_un.S_addr);
		m_server.sin_port = htons(NETWORK_PORT);

		auto ret_bind = bind(m_socket, reinterpret_cast<SOCKADDR*>(&m_server), sizeof(m_server));

		if (ret_bind == SOCKET_ERROR)
			throw NetworkError(NETERR_INTERNAL_ERROR);
		
		// Let's pre-allocate the clients.
		// So we don't have to.
		for (size_t i = 0; i < MAX_CONNECTIONS; i++)
		{
			NetworkClient cl{};
			memset(&cl, 0, sizeof(NetworkClient));

			cl.packet.CMD = NETWORK_CMD_INVALID;

			m_clients.push_back(cl);
		}
	}


	const char* NetworkServerInstance::name() noexcept { return ("NetworkServerInstance"); }

	NetworkServerInstance::INSTANCE_TYPE NetworkServerInstance::type() noexcept { return NETWORK; }

	void NetworkServerInstance::update() {}

	NetworkServerInstance::~NetworkServerInstance()
	{
#ifndef _NDEBUG
		std::string message;
		message += "Class NetworkServerInstance (delete), Epoch: ";
		message += std::to_string(get_epoch());

		XPLICIT_INFO(message);
#endif

		shutdown(m_socket, SD_BOTH);
		closesocket(m_socket);

		WSACleanup();
	}

	const char* NetworkServerInstance::dns() noexcept { return m_dns.c_str(); }

	void NetworkServerInstance::send() noexcept { m_send = true; }

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
					::sendto(instance->m_socket, (const char*)&
						instance->m_clients[i].packet, sizeof(GenericPacket), 0, (struct sockaddr*)&instance->m_clients[i].addr, sizeof(struct sockaddr_in));
				}
				
				instance->m_send = false;
			}
			else
			{
				struct sockaddr_in in{};
				int sz = sizeof(struct sockaddr_in);

				for (size_t i = 0; i < instance->m_clients.size(); i++)
				{
					int sz = sizeof(struct sockaddr_in);

					::recvfrom(instance->m_socket, (char*)&
						instance->m_clients[i].packet, sizeof(GenericPacket), 0, (struct sockaddr*)&instance->m_clients[i].addr, &sz);

					if (sz < 0)
						return;
				}
			}
		}
	}

	NetworkServerEvent::NetworkServerEvent(NetworkServerInstance* instance)
		: m_instance(instance), m_tmp_packet()
	{
		if (!instance)
			throw NetworkError(NETERR_INTERNAL_ERROR);

#ifndef _NDEBUG
		std::string message;
		message += "Class NetworkServerInstance, Epoch: ";
		message += std::to_string(get_epoch());

		XPLICIT_INFO(message);
#endif
	}

	NetworkServerEvent::~NetworkServerEvent() {}

	void NetworkServerEvent::operator()()
	{
		NetworkServerTraits::recv(this, m_instance);
	}

	const char* NetworkServerEvent::name() noexcept { return ("NetworkServerEvent"); }
}