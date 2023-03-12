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

#include "App.h"
#include "XUI.h"

namespace Xplicit::Client
{
	LocalActor::LocalActor()
		: Instance(), m_model(nullptr), m_node(nullptr)
	{

		XPLICIT_GET_DATA_DIR(data_dir);
		std::string mesh_path = data_dir;
		mesh_path += "\\Models\\player_model.obj";

		m_model = IRR->getSceneManager()->getMesh(mesh_path.c_str());
		m_node = IRR->getSceneManager()->addAnimatedMeshSceneNode(m_model);

		if (m_model && m_node)
		{
			m_node->setPosition(vector3df(0, 0, 0));
			m_node->setMaterialFlag(EMF_LIGHTING, false);
		}

		XPLICIT_INFO(mesh_path);

		assert(m_model);
		assert(m_node);

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
		// TODO: move the actor.
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
			m_packet.CMD[XPLICIT_NETWORK_CMD_FORWARD] = NETWORK_CMD_FORWARD;

		if (KB->key_down(KEY_KEY_S))
			m_packet.CMD[XPLICIT_NETWORK_CMD_BACKWARD] = NETWORK_CMD_BACKWARDS;

		if (KB->key_down(KEY_KEY_A))
			m_packet.CMD[XPLICIT_NETWORK_CMD_LEFT] = NETWORK_CMD_LEFT;

		if (KB->key_down(KEY_KEY_D))
			m_packet.CMD[XPLICIT_NETWORK_CMD_RIGHT] = NETWORK_CMD_RIGHT;

		m_network->send(m_packet);
	}
}