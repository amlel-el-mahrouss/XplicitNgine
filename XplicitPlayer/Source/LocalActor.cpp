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
	LocalActor::LocalActor(int64_t id)
		: Instance(), m_camera(nullptr), m_id(id), m_player_model(nullptr),
		m_player_model_node(nullptr)
	{
		m_camera = InstanceManager::get_singleton_ptr()->get<CameraInstance>("CameraInstance");
		m_network = InstanceManager::get_singleton_ptr()->get<NetworkInstance>("NetworkInstance");

		XPLICIT_GET_DATA_DIR(data_dir);
		std::string mesh_path = data_dir;
		mesh_path += "\\Models\\player_model.obj";

		m_player_model = IRR->getSceneManager()->getMesh(mesh_path.c_str());
		m_player_model_node = IRR->getSceneManager()->addAnimatedMeshSceneNode(m_player_model);

		if (m_player_model && m_player_model_node)
		{
			m_player_model_node->setPosition(vector3df(0, 0, 0));
			m_player_model_node->setMaterialFlag(EMF_LIGHTING, false);
		}

		XPLICIT_INFO(mesh_path);

		assert(m_network);
		assert(m_player_model);
		assert(m_player_model_node);

#ifdef XPLICIT_DEBUG
		XPLICIT_INFO("LocalActor::LocalActor");
#endif
	}

	LocalActor::~LocalActor()
	{
#ifdef XPLICIT_DEBUG
		XPLICIT_INFO("LocalActor::~LocalActor");
#endif

		if (m_player_model)
			m_player_model->drop();

	}

	// Actor update function
	void LocalActor::update()
	{
		if (!m_camera)
			m_camera = InstanceManager::get_singleton_ptr()->get<CameraInstance>("CameraInstance");

		if (m_network)
		{
			auto packet = m_network->get();

			switch (packet.CMD)
			{
			case NETWORK_CMD_POS:
			{
				auto vec3d_pos = vector3df(packet.X, packet.Y, packet.Z);
				m_player_model_node->setPosition(vec3d_pos);

				break;
			}

			case NETWORK_CMD_STOP:
			{
				if (!InstanceManager::get_singleton_ptr()->get<UI::InternalPopup>("NetworkPopup"))
				{
					InstanceManager::get_singleton_ptr()->add<UI::InternalPopup>([]()-> void {
						IRR->closeDevice();
						}, vector2di(Xplicit::Client::XPLICIT_DIM.Width / 3.45, Xplicit::Client::XPLICIT_DIM.Height / 4), UI::POPUP_TYPE::Shutdown);
				}
				break;
			}
			case NETWORK_CMD_KICK:
			{
				if (!InstanceManager::get_singleton_ptr()->get<UI::InternalPopup>("NetworkPopup"))
				{
					InstanceManager::get_singleton_ptr()->add<UI::InternalPopup>([]()-> void {
						IRR->closeDevice();
						}, vector2di(Xplicit::Client::XPLICIT_DIM.Width / 3.45, Xplicit::Client::XPLICIT_DIM.Height / 4), UI::POPUP_TYPE::Kicked);
				}
			}
			case NETWORK_CMD_WATCHDOG:
			{
				packet.CMD = NETWORK_CMD_ACK;
				m_network->send(packet);
			}
			}

			if (KB->key_down(irr::KEY_ESCAPE))
			{
				// send a stop command.
				NetworkPacket cmd = {  };
				cmd.CMD = NETWORK_CMD_STOP; // client is shutting down.

				m_network->send(cmd);

				IRR->closeDevice();
			}
		}
	}

	LocalActorMoveEvent::LocalActorMoveEvent() : m_packet() {}

	LocalActorMoveEvent::~LocalActorMoveEvent() {}

	const char* LocalActorMoveEvent::name() noexcept { return ("LocalActorMoveEvent"); }

	void LocalActorMoveEvent::operator()()
	{
		auto net = InstanceManager::get_singleton_ptr()->get<NetworkInstance>("NetworkInstance");
		if (!net) return;

		auto cam = InstanceManager::get_singleton_ptr()->get<CameraInstance>("CameraInstance");

		if (KB->key_down(KEY_KEY_W))
			m_packet.CMD = NETWORK_CMD_FORWARD;

		if (KB->key_down(KEY_KEY_S))
			m_packet.CMD = NETWORK_CMD_BACKWARDS;

		if (KB->key_down(KEY_KEY_A))
			m_packet.CMD = NETWORK_CMD_LEFT;

		if (KB->key_down(KEY_KEY_D))
			m_packet.CMD = NETWORK_CMD_RIGHT;

		net->send(m_packet);

	}
}