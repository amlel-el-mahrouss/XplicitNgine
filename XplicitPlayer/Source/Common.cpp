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

#include "Application.h"
#include "LocalActor.h"
#include "Common.h"
#include "Camera.h"
#include "XUI.h"

namespace Xplicit::Client
{
	constexpr const int XPLICIT_TIMEOUT = 100; // connection timeout
	constexpr const int XPLICIT_MAX_RESETS = 1000; // maximum RST timeout

	LoadingInstance::LoadingInstance() 
		: m_run(true), m_network(nullptr), m_logo_tex(nullptr), m_timeout(XPLICIT_TIMEOUT)
	{
		XPLICIT_GET_DATA_DIR(data_dir);

		std::string health_path = data_dir;
		health_path += "\\Textures\\engine_logo.png";

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
			InstanceManager::get_singleton_ptr()->add<Xplicit::XUI::HUD>();
			InstanceManager::get_singleton_ptr()->add<Xplicit::Client::LocalActor>();
			InstanceManager::get_singleton_ptr()->add<Xplicit::Client::LocalCameraInstance>();

			EventDispatcher::get_singleton_ptr()->add<Xplicit::Client::LocalMenuEvent>(packet.id);
			EventDispatcher::get_singleton_ptr()->add<Xplicit::Client::LocalMoveEvent>();
			EventDispatcher::get_singleton_ptr()->add<Xplicit::Client::LocalResetEvent>();
			EventDispatcher::get_singleton_ptr()->add<Xplicit::Client::LocalWatchdogEvent>();

			m_run = false;

			return;
		}
		else if (packet.cmd[XPLICIT_NETWORK_CMD_KICK] == NETWORK_CMD_KICK)
		{
			InstanceManager::get_singleton_ptr()->add<XUI::ErrorMessage>([]()-> void {
				IRR->closeDevice();
				}, vector2di(Xplicit::Client::XPLICIT_DIM.Width / 3.45, Xplicit::Client::XPLICIT_DIM.Height / 4), XUI::ERROR_TYPE::Kicked);

			m_run = false;

			return;
		}

		packet.cmd[XPLICIT_NETWORK_CMD_BEGIN] = NETWORK_CMD_BEGIN;
		packet.cmd[XPLICIT_NETWORK_CMD_ACK] = NETWORK_CMD_ACK;

		m_network->send(packet);

		IRR->getVideoDriver()->draw2DImage(m_logo_tex, vector2di(Xplicit::Client::XPLICIT_DIM.Width * 0.02, Xplicit::Client::XPLICIT_DIM.Height * 0.825),
			core::rect<s32>(0, 0, 105, 105), 0,
			video::SColor(255, 255, 255, 255), true);

		--m_timeout;

		// peek after the ++timeout
		if (m_timeout < 0)
		{
			InstanceManager::get_singleton_ptr()->add<XUI::ErrorMessage>([]()-> void {
				IRR->closeDevice();
			}, vector2di(Xplicit::Client::XPLICIT_DIM.Width / 3.45, Xplicit::Client::XPLICIT_DIM.Height / 4), XUI::ERROR_TYPE::NetworkError);

			m_run = false; // sprious reponse
		}

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
		InstanceManager::get_singleton_ptr()->add<XUI::ErrorMessage>([]()-> void {
			IRR->closeDevice();
			}, vector2di(Xplicit::Client::XPLICIT_DIM.Width / 3.45, Xplicit::Client::XPLICIT_DIM.Height / 4), XUI::ERROR_TYPE::NetworkError);

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

		if (m_network->is_reset())
		{
			++m_num_resets;

			if (m_num_resets > XPLICIT_MAX_RESETS)
			{
				if (!InstanceManager::get_singleton_ptr()->get<XUI::ErrorMessage>("ErrorMessage"))
				{
					InstanceManager::get_singleton_ptr()->add<XUI::ErrorMessage>([]()-> void {
						IRR->closeDevice();
					}, vector2di(Xplicit::Client::XPLICIT_DIM.Width / 3.45, Xplicit::Client::XPLICIT_DIM.Height / 4), XUI::ERROR_TYPE::NetworkError);

				}
			}
		}
		else
		{
			m_num_resets = 0;
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

	bool LocalMenuEvent::enable(const bool enable) noexcept
	{
		m_enabled = enable;
		return m_enabled;
	}

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
				tween_start -= 0.01;

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