/*
 * =====================================================================
 *
 *				XplicitNgin C++ Game Engine
 *			Copyright XPX, all rights reserved.
 *
 *			File: LocalPlayer.cpp
 *			Purpose: Client-side Player Instance
 *
 * =====================================================================
 */

#include "LocalActor.h"
#include "Application.h"
#include "XUI.h"

namespace Xplicit::Client
{
	LocalActor::LocalActor()
		: Instance(), m_model(nullptr), m_node(nullptr), m_packet()
	{
		XPLICIT_GET_DATA_DIR(data_dir);
		std::string mesh_path = data_dir;
		mesh_path += "\\Models\\player.obj";

		m_model = IRR->getSceneManager()->getMesh(mesh_path.c_str());
		m_node = IRR->getSceneManager()->addAnimatedMeshSceneNode(m_model);

		m_network = InstanceManager::get_singleton_ptr()->get<NetworkInstance>("NetworkInstance");

		XPLICIT_ASSERT(m_network);
		XPLICIT_ASSERT(m_model);
		XPLICIT_ASSERT(m_node);

		m_node->setPosition(vector3df(0, 0, 0));
		m_node->setMaterialFlag(EMF_LIGHTING, false);

#ifdef XPLICIT_DEBUG
		XPLICIT_INFO("LocalActor::LocalActor");
#endif
	}

	LocalActor::~LocalActor()
	{
#ifdef XPLICIT_DEBUG
		XPLICIT_INFO("LocalActor::~LocalActor");
#endif

		if (m_model)
			m_model->drop();

	}

	void LocalActor::update()
	{
		if (!m_network)
			return;

		
	}

	IAnimatedMeshSceneNode* LocalActor::operator->() const 
	{ 
		assert(m_node);
		return m_node;
	}

	LocalMoveEvent::LocalMoveEvent() 
		: m_packet(), m_network(nullptr)
	{
		m_network = InstanceManager::get_singleton_ptr()->get<NetworkInstance>("NetworkInstance");
		assert(m_network);
	}

	LocalMoveEvent::~LocalMoveEvent() {}

	const char* LocalMoveEvent::name() noexcept { return ("LocalMoveEvent"); }

	void LocalMoveEvent::operator()()
	{
		if (!m_network)
			return;

		if (KB->key_down(KEY_KEY_W))
			m_packet.cmd[XPLICIT_NETWORK_CMD_FORWARD] = NETWORK_CMD_FORWARD;

		if (KB->key_down(KEY_KEY_S))
			m_packet.cmd[XPLICIT_NETWORK_CMD_BACKWARD] = NETWORK_CMD_BACKWARDS;

		if (KB->key_down(KEY_KEY_A))
			m_packet.cmd[XPLICIT_NETWORK_CMD_LEFT] = NETWORK_CMD_LEFT;

		if (KB->key_down(KEY_KEY_D))
			m_packet.cmd[XPLICIT_NETWORK_CMD_RIGHT] = NETWORK_CMD_RIGHT;

		m_network->send(m_packet);
	}
}