/*
 * =====================================================================
 *
 *						XplicitNgin C++ Game Engine
 *			Copyright XPX Technologies all rights reserved.
 *
 *			File: Client.h
 *			Purpose: Client header
 *
 * =====================================================================
 */

#pragma once

#include <Irr.h>
#include <Event.h>
#include <Instance.h>
#include <Foundation.h>

// instances
#include <CameraInstance.h>
#include <ServerInstance.h>
#include <NetworkInstance.h>
#include <LocalPlayerInstance.h>

// XML
#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_utils.hpp>

namespace Xplicit::App
{
	constexpr const char* XPLICIT_APP_NAME = "XplicitNgin";
	constexpr const wchar_t* XPLICIT_APP_NAME_WIDE = L"XplicitNgin";

	class Application final
	{
	public:
		Application()
			: m_wsa(), m_data_path("")
		{
			memset(m_data_path, 0, 4096);
			GetEnvironmentVariableA("XPLICIT_DATA_DIR", m_data_path, 4096);

			if (*m_data_path == 0)
				throw std::runtime_error("getenv: no such variable!");

			Xplicit::init_winsock(&m_wsa);

#ifndef _NDEBUG
			Xplicit::open_terminal();
#endif

			this->setup_xml();
			this->setup_read_cfg();
		}

		~Application()
		{
			if (!InstanceManager::get_singleton_ptr())
				return;

			InstanceManager::get_singleton_ptr()->remove<NetworkInstance>("NetworkInstance");
			InstanceManager::get_singleton_ptr()->remove<CameraInstance>("CameraInstance");
			InstanceManager::get_singleton_ptr()->remove<LocalPlayerInstance>("LocalPlayerInstance");
			EventDispatcher::get_singleton_ptr()->remove<LocalPlayerMoveEvent>("LocalPlayerMoveEvent");
		}

	private:
		void setup_xml();
		void setup_read_cfg();
		bool setup_network(NetworkInstance* net);
		void setup(const char* ip);

	private:
		WSADATA m_wsa;
		char m_data_path[4096];

	};
}