/*
 * =====================================================================
 *
 *				XplicitNgin C++ Game Engine
 *			Copyright XPX, all rights reserved.
 *
 *			File: App.h
 *			Purpose: Application Implementation
 *
 * =====================================================================
 */

#pragma once

#include <Application.h>
#include <Event.h>
#include <Instance.h>
#include <Foundation.h>

// instances
#include "CameraInstance.h"
#include <ServerInstance.h>
#include <NetworkInstance.h>
#include "LocalPlayerInstance.h"

// XML
#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_utils.hpp>

namespace Xplicit::App
{
	constexpr const wchar_t* XPLICIT_APP_NAME = L"XplicitNgin";

	class Application final
	{
	public:
		Application();
		~Application();

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