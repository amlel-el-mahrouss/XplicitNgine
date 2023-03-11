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
	constexpr const int XPLICIT_TIMEOUT_DELAY = 5; // why?, because it takes time to lookup into instances.

	LoadingInstance::LoadingInstance() 
		: m_connected(false), m_network(nullptr), m_logo_tex(nullptr)
	{
		XPLICIT_GET_DATA_DIR(data_dir);

		std::string health_path = data_dir;
		health_path += "/engine_logo.png";

		m_logo_tex = IRR->getVideoDriver()->getTexture(health_path.c_str());
	}

	LoadingInstance::~LoadingInstance() {}

	void LoadingInstance::update()
	{
		if (m_connected)
			return;

		NetworkPacket packet{};

		XPLICIT_SLEEP(XPLICIT_TIMEOUT_DELAY);

		m_network->read(packet);

		if (packet.CMD == NETWORK_CMD_ACCEPT)
		{
			InstanceManager::get_singleton_ptr()->add<Xplicit::Client::LocalActor>(packet.ID);
			InstanceManager::get_singleton_ptr()->add<Xplicit::Client::CameraInstance>();
			EventDispatcher::get_singleton_ptr()->add<Xplicit::Client::LocalActorMoveEvent>();

			m_connected = true;
		}

		packet.CMD = NETWORK_CMD_BEGIN;
		m_network->send(packet);

		IRR->getVideoDriver()->draw2DImage(m_logo_tex, vector2di(20, 600),
			core::rect<s32>(0, 0, 105, 105), 0,
			video::SColor(255, 255, 255, 255), true);
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
			XPLICIT_INFO("[CLIENT] Joining another place!");
#endif
		}

		if (m_network->connect(ip))
		{
			NetworkPacket spawn{};
			spawn.CMD = NETWORK_CMD_BEGIN;
			m_network->send(spawn);

		}
	}
}