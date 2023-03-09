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
#include "XplicitApp.h"

namespace Xplicit 
{
	static void xplicit_set_ioctl(SOCKET sock)
	{
		u_long ul = 1;
		ioctlsocket(sock, FIOASYNC, &ul);

		ul = 1;
		ioctlsocket(sock, FIONBIO, &ul);
	}


	NetworkInstance::NetworkInstance()
		: Instance(), m_packet(), m_inaddr(), m_tmp_inaddr()
	{
		m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

		if (m_socket == SOCKET_ERROR)
			throw NetworkError(NETERR_INTERNAL_ERROR);

		xplicit_set_ioctl(m_socket);

#ifndef _NDEBUG
		XPLICIT_INFO("Created NetworkInstance");
#endif

	}

	NetworkInstance::~NetworkInstance()
	{
		// Shutdown and close socket if shutdown failed.
		if (shutdown(m_socket, SD_BOTH) == SOCKET_ERROR)
			closesocket(m_socket);

#ifndef _NDEBUG
		XPLICIT_INFO("~NetworkInstance, Epoch: " + std::to_string(get_epoch()));
#endif
	}

	bool NetworkInstance::connect(const char* ip)
	{
		if (m_socket == SOCKET_ERROR)
			return false;

		memset(&m_inaddr, 0, sizeof(SOCKADDR_IN));

		m_inaddr.sin_family = AF_INET;
		inet_pton(AF_INET, ip, &m_inaddr.sin_addr);
		m_inaddr.sin_port = htons(XPLICIT_NETWORK_PORT);

		int result = ::connect(m_socket, reinterpret_cast<SOCKADDR*>(&m_inaddr), sizeof(m_inaddr));

		if (result == SOCKET_ERROR)
			throw NetworkError(NETERR_INTERNAL_ERROR);

		XPLICIT_INFO("[NetworkInstance] Connected!");

		return true;
	}

	bool NetworkInstance::send(NetworkPacket& cmd)
	{
		cmd.MAG[0] = XPLICIT_NETWORK_MAG_0;
		cmd.MAG[1] = XPLICIT_NETWORK_MAG_1;
		cmd.MAG[2] = XPLICIT_NETWORK_MAG_2;

		int res = ::sendto(m_socket, (const char*)&cmd, sizeof(NetworkPacket), 0, reinterpret_cast<SOCKADDR*>(&m_inaddr), sizeof(m_inaddr));

		if (res == SOCKET_ERROR)
			throw NetworkError(NETERR_INTERNAL_ERROR);

		return true;
	}

	void NetworkInstance::update()
	{
		this->read(m_packet);
	}

	bool NetworkInstance::read(NetworkPacket& packet)
	{
		int length{ sizeof(struct sockaddr_in) };

		int res = ::recvfrom(m_socket, (char*)&packet, sizeof(NetworkPacket), 0,
			(SOCKADDR*)&m_tmp_inaddr, &length);


		if (length > 0)
		{
			if (res == SOCKET_ERROR)
			{
				int err = WSAGetLastError();
				XPLICIT_INFO("recvfrom failed with code: " + std::to_string(err));

				return false;
			}

		}

		return packet.MAG[0] == XPLICIT_NETWORK_MAG_0 && packet.MAG[1] == XPLICIT_NETWORK_MAG_1 &&
				packet.MAG[2] == XPLICIT_NETWORK_MAG_2;
	}

	void NetworkEvent::operator()()
	{
		auto net = InstanceManager::get_singleton_ptr()->find<NetworkInstance>("NetworkInstance");

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