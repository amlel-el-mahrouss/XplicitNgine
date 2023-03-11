/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: App.cpp
 *			Purpose: Application Implementation
 *
 * =====================================================================
 */

#include "LoadingInstance.h"
#include "App.h"

namespace Xplicit::App
{
	Application::Application()
		: m_wsa(), m_data_path("")
	{
		XPLICIT_GET_DATA_DIR(data_tmp);

		memcpy_s(m_data_path, 4096, data_tmp, 4096);

		if (*m_data_path == 0)
			throw std::runtime_error("getenv: no such variable!");

		Xplicit::init_winsock(&m_wsa);

#ifndef _NDEBUG
		Xplicit::open_terminal();
#endif

		this->setup_xml();
		this->setup_cfg();
	}

	Application::~Application()
	{
		if (!InstanceManager::get_singleton_ptr())
			return;

		InstanceManager::get_singleton_ptr()->remove<NetworkInstance>("NetworkInstance");
		InstanceManager::get_singleton_ptr()->remove<Client::CameraInstance>("CameraInstance");
		InstanceManager::get_singleton_ptr()->remove<Client::LocalActor>("LocalActor");
		EventDispatcher::get_singleton_ptr()->remove<Client::LocalActorMoveEvent>("LocalActorMoveEvent");
	}

	void Application::setup_xml()
	{
		std::string path = m_data_path;
		path += "\\MANIFEST.xml";

		XML = IRR->getFileSystem()->createXMLReaderUTF8(path.c_str());
	}

	void Application::setup_cfg()
	{
		if (!XML)
			throw std::runtime_error("No XML provided..");

		// read until EOF.
		while (XML->read())
		{
			switch (XML->getNodeType())
			{
			//we found a new element
			case irr::io::EXN_ELEMENT:
			{
				if (std::string(XML->getNodeName()) == "CONNECT")
				{
					auto loading = InstanceManager::get_singleton_ptr()->add<Client::LoadingInstance>();
					assert(loading);

					loading->connect(XML->getAttributeValue("IP"));

#ifdef XPLICIT_DEBUG
					XPLICIT_INFO("Connecting to server: " + std::string(XML->getAttributeValue("IP")));
#endif
				}

				break;
			}
			}
		}
	}
}