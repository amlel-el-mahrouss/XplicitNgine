/*
 * =====================================================================
 *
 *				XplicitNgin C++ Game Engine
 *			Copyright XPX, all rights reserved.
 *
 *			File: LoadingSreenInstance.cpp
 *			Purpose: Client Loading Screen
 *
 * =====================================================================
 */

#include "LoadingScreenInstance.h"
#include "CameraInstance.h"
#include "LocalActor.h"

namespace Xplicit::Client
{
	constexpr const int XPLICIT_TIMEOUT_DELAY = 10; // why?, because it takes time to lookup into instances.

	LoadingScreenInstance::LoadingScreenInstance() : m_connected(false), m_net(nullptr), m_timeout(XPLICIT_TIMEOUT_DELAY) {}

	LoadingScreenInstance::~LoadingScreenInstance() {}

	void LoadingScreenInstance::update()
	{
		if (m_connected)
			return;
		
		assert(m_net);

		NetworkPacket packet_spawn{};

		// set the timeout to zero by default, so we instantly seek for a incoming packet.
		if (m_timeout < 0)
		{
			packet_spawn.CMD = NETWORK_CMD_BEGIN;
			m_net->send(packet_spawn);

			m_timeout = XPLICIT_TIMEOUT_DELAY;
		}

		XPLICIT_SLEEP(70);
		
		m_net->read(packet_spawn);

		if (packet_spawn.CMD == NETWORK_CMD_ACCEPT)
		{
			InstanceManager::get_singleton_ptr()->add<Xplicit::Client::LocalActor>(packet_spawn.ID);
			InstanceManager::get_singleton_ptr()->add<Xplicit::Client::CameraInstance>();
			EventDispatcher::get_singleton_ptr()->add<Xplicit::Client::LocalActorMoveEvent>();

			m_connected = true;
		}

		--m_timeout;
	}

	void LoadingScreenInstance::connect(const char* ip)
	{
		m_net = InstanceManager::get_singleton_ptr()->get<NetworkInstance>("NetworkInstance");

		if (!m_net)
		{
			m_net = InstanceManager::get_singleton_ptr()->add<NetworkInstance>();
			assert(m_net);
		}
		else
		{
			InstanceManager::get_singleton_ptr()->remove<Xplicit::Client::LocalActor>("LocalActor");
			InstanceManager::get_singleton_ptr()->remove<Xplicit::Client::CameraInstance>("CameraInstance");
			EventDispatcher::get_singleton_ptr()->remove<Xplicit::Client::LocalActorMoveEvent>("LocalActorMoveEvent");

#ifdef XPLICIT_DEBUG
			XPLICIT_INFO("[CLIENT] Reconnecting to another server");
#endif

			m_net->reset();
		}

		if (m_net->connect(ip))
		{
			m_connected = false;

			NetworkPacket packet_spawn{};
			packet_spawn.CMD = NETWORK_CMD_BEGIN;
			m_net->send(packet_spawn);
		}
	}
}