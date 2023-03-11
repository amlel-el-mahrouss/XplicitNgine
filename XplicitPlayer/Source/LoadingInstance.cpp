/*
 * =====================================================================
 *
 *				XplicitNgin C++ Game Engine
 *			Copyright XPX, all rights reserved.
 *
 *			File: LoadingSreenInstance.cpp
 *			Purpose: Client Loading Logic
 *
 * =====================================================================
 */

#include "LoadingInstance.h"
#include "CameraInstance.h"
#include "LocalActor.h"

namespace Xplicit::Client
{
	constexpr const int XPLICIT_TIMEOUT_DELAY = 10; // why? because it takes time to lookup into instances.

	LoadingInstance::LoadingInstance() : m_connected(false), m_network(nullptr), m_timeout(XPLICIT_TIMEOUT_DELAY) {}

	LoadingInstance::~LoadingInstance() {}

	void LoadingInstance::update()
	{
		if (m_connected)
			return;
		
		assert(m_network);

		NetworkPacket packet_spawn{};

		// set the timeout to zero by default, so we instantly seek for a incoming packet.
		if (m_timeout < 0)
		{
			packet_spawn.CMD = NETWORK_CMD_BEGIN;
			m_network->send(packet_spawn);

			m_timeout = XPLICIT_TIMEOUT_DELAY;
		}

		XPLICIT_SLEEP(70);
		
		m_network->read(packet_spawn);

		if (packet_spawn.CMD == NETWORK_CMD_ACCEPT)
		{
			InstanceManager::get_singleton_ptr()->add<Xplicit::Client::LocalActor>(packet_spawn.ID);
			InstanceManager::get_singleton_ptr()->add<Xplicit::Client::CameraInstance>();
			EventDispatcher::get_singleton_ptr()->add<Xplicit::Client::LocalActorMoveEvent>();

			m_connected = true;
		}

		--m_timeout;
	}

	void LoadingInstance::connect(const char* ip)
	{
		m_network = InstanceManager::get_singleton_ptr()->get<NetworkInstance>("NetworkInstance");

		if (!m_network)
		{
			m_network = InstanceManager::get_singleton_ptr()->add<NetworkInstance>();
			assert(m_network);
		}
		else
		{
			InstanceManager::get_singleton_ptr()->remove<Xplicit::Client::LocalActor>("LocalActor");
			InstanceManager::get_singleton_ptr()->remove<Xplicit::Client::CameraInstance>("CameraInstance");
			EventDispatcher::get_singleton_ptr()->remove<Xplicit::Client::LocalActorMoveEvent>("LocalActorMoveEvent");

#ifdef XPLICIT_DEBUG
			XPLICIT_INFO("[CLIENT] Reconnecting to another server");
#endif

			m_network->reset();
		}

		if (m_network->connect(ip))
		{
			m_connected = false;

			NetworkPacket packet_spawn{};
			packet_spawn.CMD = NETWORK_CMD_BEGIN;
			m_network->send(packet_spawn);
		}
	}
}