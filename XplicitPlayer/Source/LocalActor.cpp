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
		: Instance(), m_then(0), m_camera(nullptr), m_id(id), m_player_model(nullptr),
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

		m_then = IRR->getTimer()->getTime();

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

		const u32 now = IRR->getTimer()->getTime();
		const f32 delta = (f32)(now - m_then) / 1000.f; // Time in seconds

		m_then = now;

		if (KB->key_down(KEY_KEY_W))
		{
			if (m_camera)
			{
				auto pos = m_player_model_node->getPosition();
				pos.Z += XPLICIT_SPEED * delta;

				m_player_model_node->setPosition(pos);
			}
		}
		else if (KB->key_down(KEY_KEY_S))
		{
			if (m_camera)
			{
				auto pos = m_camera->get()->getPosition();
				pos.Z -= XPLICIT_SPEED * delta;

				m_player_model_node->setPosition(pos);
			}
		}
		else if (KB->key_down(KEY_KEY_D))
		{
			if (m_camera)
			{
				auto pos = m_player_model_node->getPosition();
				pos.X += XPLICIT_SPEED * delta;

				m_player_model_node->setPosition(pos);
			}
		}
		else if (KB->key_down(KEY_KEY_A))
		{
			if (m_camera)
			{
				auto pos = m_player_model_node->getPosition();
				pos.X -= XPLICIT_SPEED * delta;

				m_player_model_node->setPosition(pos);
			}
		}

		if (m_network)
		{
			auto packet = m_network->get();

			if (packet.CMD == NETWORK_CMD_STOP)
			{
				if (!InstanceManager::get_singleton_ptr()->get<UI::InternalPopup>("NetworkPopup"))
				{
					InstanceManager::get_singleton_ptr()->add<UI::InternalPopup>([]()-> void {
						IRR->closeDevice();
					}, vector2di(Xplicit::Client::XPLICIT_DIM.Width / 3.45, Xplicit::Client::XPLICIT_DIM.Height / 4), UI::POPUP_TYPE::Shutdown);
				}
			}

			if (packet.CMD == NETWORK_CMD_KICK)
			{
				if (!InstanceManager::get_singleton_ptr()->get<UI::InternalPopup>("NetworkPopup"))
				{
					InstanceManager::get_singleton_ptr()->add<UI::InternalPopup>([]()-> void {
						IRR->closeDevice();
						}, vector2di(Xplicit::Client::XPLICIT_DIM.Width / 3.45, Xplicit::Client::XPLICIT_DIM.Height / 4), UI::POPUP_TYPE::Kicked);
				}
			}
		}
	}

	LocalActorMoveEvent::LocalActorMoveEvent() : m_last_pos(0, 0, 0), m_packet() {}

	LocalActorMoveEvent::~LocalActorMoveEvent() {}

	const char* LocalActorMoveEvent::name() noexcept { return ("LocalActorMoveEvent"); }

	void LocalActorMoveEvent::operator()()
	{
		auto net = InstanceManager::get_singleton_ptr()->get<NetworkInstance>("NetworkInstance");
		if (!net) return;

		auto cam = InstanceManager::get_singleton_ptr()->get<CameraInstance>("CameraInstance");
		if (cam->get()->getPosition() == m_last_pos) return;

		auto pos = cam->get()->getPosition();
		if (pos == m_last_pos) return;

		m_packet.CMD = NETWORK_CMD_POS;

		m_packet.X = pos.X;
		m_packet.Y = pos.Y;
		m_packet.Z = pos.Z;

		net->send(m_packet);

		m_last_pos = pos;
	}
}