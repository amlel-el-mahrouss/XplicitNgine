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

namespace Xplicit::Client
{
	LocalActor::LocalActor(int64_t id)
		: Instance(), m_then(0), m_cam(nullptr), m_id(id)
	{
		m_cam = InstanceManager::get_singleton_ptr()->get<CameraInstance>("CameraInstance");
		m_net = InstanceManager::get_singleton_ptr()->get<NetworkInstance>("NetworkInstance");

		m_then = IRR->getTimer()->getTime();

#ifndef _NDEBUG
		XPLICIT_INFO("LocalActor::LocalActor");
#endif
	}

	LocalActor::~LocalActor()
	{
#ifndef _NDEBUG
		XPLICIT_INFO("LocalActor::~LocalActor");
#endif
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

	void LocalActor::update()
	{
		if (!m_cam)
			m_cam = InstanceManager::get_singleton_ptr()->get<CameraInstance>("CameraInstance");

		const u32 now = IRR->getTimer()->getTime();
		const f32 delta = (f32)(now - m_then) / 1000.f; // Time in seconds

		m_then = now;

		if (KB->is_key_down(KEY_KEY_W))
		{
			if (m_cam)
			{
				auto pos = m_cam->get()->getPosition();
				pos.Z += XPLICIT_SPEED * delta;

				m_cam->get()->setPosition(pos);
			}
		}
		else if (KB->is_key_down(KEY_KEY_S))
		{
			if (m_cam)
			{
				auto pos = m_cam->get()->getPosition();
				pos.Z -= XPLICIT_SPEED * delta;

				m_cam->get()->setPosition(pos);
			}
		}
		else if (KB->is_key_down(KEY_KEY_D))
		{
			if (m_cam)
			{
				auto pos = m_cam->get()->getPosition();
				pos.X += XPLICIT_SPEED * delta;

				m_cam->get()->setPosition(pos);
			}
		}
		else if (KB->is_key_down(KEY_KEY_A))
		{
			if (m_cam)
			{
				auto pos = m_cam->get()->getPosition();
				pos.X -= XPLICIT_SPEED * delta;

				m_cam->get()->setPosition(pos);
			}
		}

		if (m_net)
		{
			auto packet = m_net->get();

			if (packet.CMD == NETWORK_CMD_STOP)
			{
				IRR->closeDevice();
			}
		}
	}
}