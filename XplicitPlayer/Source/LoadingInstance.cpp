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
#include "CameraInstance.h"
#include "LocalActor.h"
#include "App.h"
#include "XUI.h"

namespace Xplicit::Client
{
	constexpr const int XPLICIT_TIMEOUT_DELAY = 5; // why?, because it takes time to lookup into instances.
	constexpr const int XPLICIT_TIMEOUT = 1000; // maximum timeout.

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

		XPLICIT_SLEEP(XPLICIT_TIMEOUT_DELAY);

		m_network->read(packet);

		for (size_t i = 0; i < XPLICIT_NETWORK_MAX_CMDS; i++)
		{
			if (packet.CMD[i] == NETWORK_CMD_ACCEPT)
			{
				InstanceManager::get_singleton_ptr()->add<Xplicit::UI::InternalHUD>();
				InstanceManager::get_singleton_ptr()->add<Xplicit::Client::LocalActor>();
				InstanceManager::get_singleton_ptr()->add<Xplicit::Client::CameraInstance>();

				m_run = false;
			}
		}

		packet.CMD[0] = NETWORK_CMD_BEGIN;
		m_network->send(packet);

		IRR->getVideoDriver()->draw2DImage(m_logo_tex, vector2di(Xplicit::Client::XPLICIT_DIM.Width * 0.02, Xplicit::Client::XPLICIT_DIM.Height * 0.825),
			core::rect<s32>(0, 0, 105, 105), 0,
			video::SColor(255, 255, 255, 255), true);

		--m_timeout;

		// peek after the ++timeout
		if (m_timeout < 0)
		{
			InstanceManager::get_singleton_ptr()->add<UI::InternalPopup>([]()-> void {
				IRR->closeDevice();
			}, vector2di(Xplicit::Client::XPLICIT_DIM.Width / 3.45, Xplicit::Client::XPLICIT_DIM.Height / 4), UI::POPUP_TYPE::NetworkError);

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
			spawn.CMD[4] = NETWORK_CMD_BEGIN;
			m_network->send(spawn);

		}
	}

	void LoadingInstance::reset() noexcept
	{
		InstanceManager::get_singleton_ptr()->add<UI::InternalPopup>([]()-> void {
			IRR->closeDevice();
			}, vector2di(Xplicit::Client::XPLICIT_DIM.Width / 3.45, Xplicit::Client::XPLICIT_DIM.Height / 4), UI::POPUP_TYPE::NetworkError);

	}
}