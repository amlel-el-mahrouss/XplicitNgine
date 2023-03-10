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

#include <Event.h>
#include <Instance.h>
#include <Foundation.h>
#include <Application.h>

/* Client instances */
#include "LocalActor.h"
#include "CameraInstance.h"
#include <NetworkInstance.h>

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