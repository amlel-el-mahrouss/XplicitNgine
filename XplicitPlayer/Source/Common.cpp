/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: Common.cpp
 *			Purpose: Common components
 *
 * =====================================================================
 */

#include "Common.h"

#include "LocalWatchdogEvent.h"
#include "Application.h"
#include "LocalActor.h"
#include "Camera.h"
#include "CoreUI.h"

namespace Xplicit::Client
{
	constexpr const int XPLICIT_TIMEOUT = 100; // connection timeout
	constexpr const int XPLICIT_MAX_RESETS = 100; // maximum RST timeout

	LoadingInstance::LoadingInstance() 
		: m_run(true), m_network(nullptr), m_logo_tex(nullptr), m_timeout(XPLICIT_TIMEOUT)
	{
		XPLICIT_GET_DATA_DIR(data_dir);

		std::string health_path = data_dir;
		health_path += "\\Textures\\logo.png";

		m_logo_tex = IRR->getVideoDriver()->getTexture(health_path.c_str());
	}

	LoadingInstance::~LoadingInstance() {}

	void LoadingInstance::update()
	{
		if (!m_run)
			return;
		
		NetworkPacket packet{};
		m_network->read(packet);

		if (packet.cmd[XPLICIT_NETWORK_CMD_ACCEPT] == NETWORK_CMD_ACCEPT)
		{
			InstanceManager::get_singleton_ptr()->add<Xplicit::CoreUI::HUD>();
			auto actor = InstanceManager::get_singleton_ptr()->add<Xplicit::Client::LocalActor>(packet.id);
			XPLICIT_ASSERT(actor);

			actor->attach(InstanceManager::get_singleton_ptr()->add<Xplicit::Client::CameraInstance>());

			EventDispatcher::get_singleton_ptr()->add<Xplicit::Client::LocalMenuEvent>(packet.id);
			EventDispatcher::get_singleton_ptr()->add<Xplicit::Client::LocalMoveEvent>(packet.id);
			EventDispatcher::get_singleton_ptr()->add<Xplicit::Client::LocalResetEvent>();
			EventDispatcher::get_singleton_ptr()->add<Xplicit::Client::LocalWatchdogEvent>(packet.id);

			m_run = false;

			return;
		}

		IRR->getVideoDriver()->draw2DImage(m_logo_tex, vector2di(Xplicit::Client::XPLICIT_DIM.Width * 0.02, Xplicit::Client::XPLICIT_DIM.Height * 0.825),
			core::rect<s32>(0, 0, 105, 105), 0,
			video::SColor(255, 255, 255, 255), true);

		--m_timeout;

		// peek after the ++timeout
		if (m_timeout < 0)
		{
			InstanceManager::get_singleton_ptr()->add<CoreUI::ErrorMessage>([]()-> void {
				IRR->closeDevice();
			}, vector2di(Xplicit::Client::XPLICIT_DIM.Width / 3.45, Xplicit::Client::XPLICIT_DIM.Height / 4), CoreUI::ERROR_TYPE::NetworkError);

			m_run = false; // sprious reponse
		}

		// retry...
		packet.cmd[XPLICIT_NETWORK_CMD_BEGIN] = NETWORK_CMD_BEGIN;
		packet.cmd[XPLICIT_NETWORK_CMD_ACK] = NETWORK_CMD_ACK;

		m_network->send(packet);

		XPLICIT_SLEEP(100);
	}

	void LoadingInstance::connect(const char* ip)
	{
		m_network = InstanceManager::get_singleton_ptr()->get<NetworkInstance>("NetworkInstance");

		if (!m_network)
		{
			m_network = InstanceManager::get_singleton_ptr()->add<NetworkInstance>();
			assert(m_network);
		}

		if (m_network->connect(ip))
		{
			NetworkPacket spawn{};

			spawn.cmd[XPLICIT_NETWORK_CMD_BEGIN] = NETWORK_CMD_BEGIN;
			spawn.cmd[XPLICIT_NETWORK_CMD_ACK] = NETWORK_CMD_ACK;
		
			m_network->send(spawn);
		}
	}

	void LoadingInstance::reset() noexcept
	{
		InstanceManager::get_singleton_ptr()->add<CoreUI::ErrorMessage>([]()-> void {
			IRR->closeDevice();
			}, vector2di(Xplicit::Client::XPLICIT_DIM.Width / 3.45, Xplicit::Client::XPLICIT_DIM.Height / 4), CoreUI::ERROR_TYPE::NetworkError);

	}

	LocalResetEvent::LocalResetEvent()
		: m_network(nullptr), m_num_resets(0)
	{
		m_network = InstanceManager::get_singleton_ptr()->get<NetworkInstance>("NetworkInstance");
		assert(m_network);
	}

	LocalResetEvent::~LocalResetEvent() {}

	const char* LocalResetEvent::name() noexcept { return ("LocalResetEvent"); }

	void LocalResetEvent::operator()()
	{
		if (!m_network)
			return;

		auto& packet = m_network->get();

		if (packet.cmd[XPLICIT_NETWORK_CMD_STOP] == NETWORK_CMD_STOP)
		{
			if (!InstanceManager::get_singleton_ptr()->get<CoreUI::ErrorMessage>("ErrorMessage"))
			{
				InstanceManager::get_singleton_ptr()->add<CoreUI::ErrorMessage>([]()-> void {
					IRR->closeDevice();
					}, vector2di(Xplicit::Client::XPLICIT_DIM.Width / 3.45, Xplicit::Client::XPLICIT_DIM.Height / 4), CoreUI::ERROR_TYPE::Shutdown);

				return;
			}
		}

		if (m_network->is_reset())
		{
			++m_num_resets;

			if (m_num_resets > XPLICIT_MAX_RESETS)
			{
				if (!InstanceManager::get_singleton_ptr()->get<CoreUI::ErrorMessage>("ErrorMessage"))
				{
					InstanceManager::get_singleton_ptr()->add<CoreUI::ErrorMessage>([]()-> void {
						IRR->closeDevice();
					}, vector2di(Xplicit::Client::XPLICIT_DIM.Width / 3.45, Xplicit::Client::XPLICIT_DIM.Height / 4), CoreUI::ERROR_TYPE::NetworkError);

				}
			}
		}
	}

	LocalMenuEvent::LocalMenuEvent(const int64_t& id)
		: m_network(nullptr), m_enabled(false), m_menu(nullptr), m_timeout(0), m_hash(id)
	{
		m_network = InstanceManager::get_singleton_ptr()->get<NetworkInstance>("NetworkInstance");
		assert(m_network);

		XPLICIT_GET_DATA_DIR(data_dir);
		std::string frame_path = data_dir;
		frame_path += "\\Textures\\leave_menu.png";

		m_menu = IRR->getVideoDriver()->getTexture(frame_path.c_str());
	}

	LocalMenuEvent::~LocalMenuEvent() {}

	const char* LocalMenuEvent::name() noexcept { return ("LocalMenuEvent"); }

	void LocalMenuEvent::enable(const bool enable) noexcept
	{
		m_enabled = enable;
	}

	bool LocalMenuEvent::enabled() noexcept { return m_enabled; }

	static const int XPLICIT_TIMEOUT_MENU = 2000;

	void LocalMenuEvent::operator()()
	{
		if (!m_network)
			return;

		static float tween_start = 8;

		if (KB->key_down(KEY_ESCAPE) && m_timeout < 0)
		{
			tween_start = 8;
			m_enabled = !m_enabled;
			m_timeout = XPLICIT_TIMEOUT_MENU;
		}

		if (m_enabled)
		{
			IRR->getVideoDriver()->draw2DImage(m_menu, vector2di(Xplicit::Client::XPLICIT_DIM.Width / 3.45, Xplicit::Client::XPLICIT_DIM.Height / tween_start));

			if (tween_start > 4)
				tween_start -= 0.01f;

			if (KB->key_down(KEY_KEY_L))
			{
				NetworkPacket pckt{};
				pckt.cmd[XPLICIT_NETWORK_CMD_STOP] = NETWORK_CMD_STOP;
				pckt.id = m_hash;

				m_network->send(pckt);

				m_enabled = false;
				IRR->closeDevice();
			}
			else if (KB->key_down(KEY_KEY_N))
			{
				m_enabled = false;
			}
		}

		--m_timeout;
	}
}