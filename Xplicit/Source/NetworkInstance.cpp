/*
 * =====================================================================
 *
 *				XplicitNgin C++ Game Engine
 *			Copyright XPX, all rights reserved.
 *
 *			File: NetworkInstance.cpp
 *			Purpose: UDP Networking API
 *
 * =====================================================================
 */

#include "NetworkInstance.h"

namespace Xplicit 
{
	// common operations for NetworkInstance.
	static void xplicit_set_ioctl(SOCKET sock)
	{
#ifdef XPLICIT_WINDOWS
		u_long ul = 1;
		ioctlsocket(sock, FIONBIO, &ul);

#else
#pragma error("DEFINE ME NetworkInstance.cpp")
#endif
	}

	// NetworkInstance Constructor
	NetworkInstance::NetworkInstance()
		: Instance(), m_packet(), m_inaddr()
	{
#ifdef XPLICIT_WINDOWS
		m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

		if (m_socket == SOCKET_ERROR)
			throw NetworkError(NETERR_INTERNAL_ERROR);

		xplicit_set_ioctl(m_socket);
#else
#pragma error("DEFINE ME NetworkInstance.cpp")
#endif

#ifndef _NDEBUG
		XPLICIT_INFO("Created NetworkInstance");
#endif

	}

	NetworkInstance::~NetworkInstance()
	{
#ifdef XPLICIT_WINDOWS
		// Shutdown and close socket if shutdown failed.
		if (shutdown(m_socket, SD_BOTH) == SOCKET_ERROR)
			closesocket(m_socket);
#else
#pragma error("DEFINE ME NetworkInstance.cpp")
#endif

#ifndef _NDEBUG
		XPLICIT_INFO("~NetworkInstance, Epoch: " + std::to_string(get_epoch()));
#endif
	}

	bool NetworkInstance::connect(const char* ip)
	{
		if (m_socket == SOCKET_ERROR)
			return false;

#ifdef XPLICIT_WINDOWS
		memset(&m_inaddr, 0, sizeof(SOCKADDR_IN));

		m_inaddr.sin_family = AF_INET;
		inet_pton(AF_INET, ip, &m_inaddr.sin_addr);
		m_inaddr.sin_port = htons(XPLICIT_NETWORK_PORT);

		int result = ::connect(m_socket, reinterpret_cast<SOCKADDR*>(&m_inaddr), sizeof(m_inaddr));

		if (result == SOCKET_ERROR)
			throw NetworkError(NETERR_INTERNAL_ERROR);
#else
#pragma error("DEFINE ME NetworkInstance.cpp")
#endif

		XPLICIT_INFO("[NetworkInstance] Connected!");

		return true;
	}

	bool NetworkInstance::send(NetworkPacket& packet)
	{
		packet.MAG[0] = XPLICIT_NETWORK_MAG_0;
		packet.MAG[1] = XPLICIT_NETWORK_MAG_1;
		packet.MAG[2] = XPLICIT_NETWORK_MAG_2;

#ifdef XPLICIT_WINDOWS
		int res = ::sendto(m_socket, (const char*)&packet, sizeof(NetworkPacket), 0, reinterpret_cast<SOCKADDR*>(&m_inaddr), sizeof(m_inaddr));

		if (res == SOCKET_ERROR)
			throw NetworkError(NETERR_INTERNAL_ERROR);
#else
#pragma error("DEFINE ME NetworkInstance.cpp")
#endif

		return true;
	}

	void NetworkInstance::update() 
	{
		this->read(m_packet);
	}

	bool NetworkInstance::read(NetworkPacket& packet)
	{
		int length{ sizeof(struct sockaddr_in) };

#ifdef XPLICIT_WINDOWS
		int res = ::recvfrom(m_socket, (char*)&packet, sizeof(NetworkPacket), 0,
			(struct sockaddr*)&m_inaddr, &length);
#else
#pragma error("DEFINE ME NetworkInstance.cpp")
#endif

		if (length > 0)
		{
			if (res == SOCKET_ERROR)
			{
				int err = WSAGetLastError();
				XPLICIT_INFO("recvfrom failed with code: " + std::to_string(err));

				switch (err)
				{
				case WSAEWOULDBLOCK:
				{
					XPLICIT_INFO("Unblocking socket...");
					
#ifdef XPLICIT_WINDOWS
					struct timeval timeout = { .tv_sec = 0, .tv_usec = 200 };

					fd_set wr_set{};

					FD_ZERO(&wr_set);
					FD_SET(m_socket, &wr_set);

					select(m_socket, &wr_set, nullptr, nullptr, &timeout);
#else
#pragma error("DEFINE ME NetworkInstance.cpp")
#endif
					break;
				}
				}

				return false;
			}

		}

		XPLICIT_INFO(std::to_string(packet.CMD));
		XPLICIT_INFO("Something got read!");
		return packet.MAG[0] == XPLICIT_NETWORK_MAG_0 && packet.MAG[1] == XPLICIT_NETWORK_MAG_1 &&
				packet.MAG[2] == XPLICIT_NETWORK_MAG_2;
	}


	NetworkPacket& NetworkInstance::get() noexcept { return m_packet; }

	void NetworkEvent::operator()()
	{
		auto net = InstanceManager::get_singleton_ptr()->get<NetworkInstance>("NetworkInstance");

		if (net)
		{
			NetworkPacket packet = net->get();

			if (packet.CMD == NETWORK_CMD_KICK || packet.CMD == NETWORK_CMD_STOP)
			{
				XPLICIT_INFO(packet.CMD == NETWORK_CMD_KICK ? "[CLIENT] Server kicked us!" :
							 "[CLIENT] Server shutdown!");

				InstanceManager::get_singleton_ptr()->remove<NetworkInstance>("NetworkInstance");
			}
		}
	}
}