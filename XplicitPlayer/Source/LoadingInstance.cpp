/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: LoadingSreenInstance.cpp
 *			Purpose: Client Loading Logic
 *
 * =====================================================================
 */

#include "LoadingInstance.h"
#include "LocalInstance.h"
#include "LocalActor.h"
#include "App.h"
#include "XUI.h"

namespace Xplicit::Client
{
	constexpr const int XPLICIT_TIMEOUT_DELAY = 5; // why?, because it takes time to lookup into instances.
	constexpr const int XPLICIT_TIMEOUT = 1000; // maximum timeout.
	constexpr const int XPLICIT_MAX_RESETS = 5000; // maximum RST.

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

		if (packet.CMD[XPLICIT_NETWORK_CMD_ACCEPT] == NETWORK_CMD_ACCEPT)
		{
			InstanceManager::get_singleton_ptr()->add<Xplicit::XUI::HUD>();

			InstanceManager::get_singleton_ptr()->add<Xplicit::Client::LocalActor>(packet.ID);
			InstanceManager::get_singleton_ptr()->add<Xplicit::Client::LocalInstance>();

			EventDispatcher::get_singleton_ptr()->add<Xplicit::Client::LocalMoveEvent>();
			EventDispatcher::get_singleton_ptr()->add<Xplicit::Client::LocalResetEvent>();
			EventDispatcher::get_singleton_ptr()->add<Xplicit::Client::LocalWatchdogEvent>();

			m_run = false;
		}
		
		if (packet.CMD[XPLICIT_NETWORK_CMD_KICK] == NETWORK_CMD_KICK)
		{
			InstanceManager::get_singleton_ptr()->add<XUI::Popup>([]()-> void {
				IRR->closeDevice();
				}, vector2di(Xplicit::Client::XPLICIT_DIM.Width / 3.45, Xplicit::Client::XPLICIT_DIM.Height / 4), XUI::POPUP_TYPE::Kicked);

			m_run = false;

			return;
		}

		m_network->send(packet);

		IRR->getVideoDriver()->draw2DImage(m_logo_tex, vector2di(Xplicit::Client::XPLICIT_DIM.Width * 0.02, Xplicit::Client::XPLICIT_DIM.Height * 0.825),
			core::rect<s32>(0, 0, 105, 105), 0,
			video::SColor(255, 255, 255, 255), true);

		--m_timeout;

		// peek after the ++timeout
		if (m_timeout < 0)
		{
			InstanceManager::get_singleton_ptr()->add<XUI::Popup>([]()-> void {
				IRR->closeDevice();
			}, vector2di(Xplicit::Client::XPLICIT_DIM.Width / 3.45, Xplicit::Client::XPLICIT_DIM.Height / 4), XUI::POPUP_TYPE::NetworkError);

			m_run = false; // sprious reponse
		}
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

			spawn.CMD[XPLICIT_NETWORK_CMD_BEGIN] = NETWORK_CMD_BEGIN;
			spawn.CMD[XPLICIT_NETWORK_CMD_ACK] = NETWORK_CMD_ACK;
		
			m_network->send(spawn);

		}
	}

	void LoadingInstance::reset() noexcept
	{
		InstanceManager::get_singleton_ptr()->add<XUI::Popup>([]()-> void {
			IRR->closeDevice();
			}, vector2di(Xplicit::Client::XPLICIT_DIM.Width / 3.45, Xplicit::Client::XPLICIT_DIM.Height / 4), XUI::POPUP_TYPE::NetworkError);

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
				if (!InstanceManager::get_singleton_ptr()->get<XUI::Popup>("Popup"))
				{
					InstanceManager::get_singleton_ptr()->add<XUI::Popup>([]()-> void {
						IRR->closeDevice();
					}, vector2di(Xplicit::Client::XPLICIT_DIM.Width / 3.45, Xplicit::Client::XPLICIT_DIM.Height / 4), XUI::POPUP_TYPE::NetworkError);

				}
			}
		}
		else
		{
			m_num_resets = 0;
		}
	}
}