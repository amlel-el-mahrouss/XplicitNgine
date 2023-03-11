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
#include <ApplicationContext.h>

/* Client instances */
#include "LocalActor.h"
#include "CameraInstance.h"

namespace Xplicit::Client
{
	// default diemsion for an xplicit client app.
	static inline dimension2du XPLICIT_DIM = dimension2du(1280, 720);
}

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

		class Settings final
		{
		public:
			struct Traits
			{
				int window_width{ 800 };
				int window_height{ 600 };
				float mouse_sensitivity{ 1.0f };
			};

		public:
			Settings();
			~Settings();

			void write(Traits& traits);
			void read(Traits& traits);

		private:
			std::string m_settings_path;

		private:
			irr::io::IXMLReaderUTF8* m_xml_reader;
			irr::io::IXMLWriterUTF8* m_xml_writer;

		};

	private:
		void setup_xml();
		void setup_manifest();

	private:
		WSADATA m_wsa;
		char m_data_path[4096];

	};
}