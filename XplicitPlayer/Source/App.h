/*
 * =====================================================================
 *
 *			XplicitNgin
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

namespace Xplicit::App
{
	constexpr const wchar_t* XPLICIT_APP_NAME = L"Xplicit";

	class Application final
	{
	public:
		Application();
		~Application();

		Application& operator=(const Application&) = delete;
		Application(const Application&) = delete;

	private:
		void setup_xml();
		void setup_cfg();

	private:
		WSADATA m_wsa;
		char m_data_path[4096];

	};
}