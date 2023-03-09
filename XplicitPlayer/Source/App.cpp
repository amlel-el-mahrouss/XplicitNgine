/*
 * =====================================================================
 *
 *				XplicitNgin C++ Game Engine
 *			Copyright XPX, all rights reserved.
 *
 *			File: App.cpp
 *			Purpose: Application Implementation
 *
 * =====================================================================
 */

#include "App.h"

namespace Xplicit::App
{
	Application::Application()
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

	Application::~Application()
	{
		if (!InstanceManager::get_singleton_ptr())
			return;

		InstanceManager::get_singleton_ptr()->remove<NetworkInstance>("NetworkInstance");
		InstanceManager::get_singleton_ptr()->remove<CameraInstance>("CameraInstance");
		InstanceManager::get_singleton_ptr()->remove<LocalPlayerInstance>("LocalPlayerInstance");
		EventDispatcher::get_singleton_ptr()->remove<LocalPlayerMoveEvent>("LocalPlayerMoveEvent");
	}

	void Application::setup_xml()
	{
		InstanceManager::get_singleton_ptr()->add<NetworkInstance>();

		std::string path = m_data_path;
		path += "\\MANIFEST.xml";

		XML = IRR->getFileSystem()->createXMLReaderUTF8(path.c_str());
	}

	void Application::setup_read_cfg()
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
					this->setup(XML->getAttributeValue("IP"));

					return;
				}

				break;
			}
			}
		}
	}

	bool Application::setup_network(NetworkInstance* net)
	{
		if (!net)
			return false;

		NetworkPacket packet_spawn{};
		packet_spawn.CMD = NETWORK_CMD_BEGIN;

		net->send(packet_spawn);

		int32_t timeout = 0;

		while (packet_spawn.CMD != NETWORK_CMD_ACCEPT)
		{
			net->read(packet_spawn);

			if (timeout > 3000)
				return false;
		}

		InstanceManager::get_singleton_ptr()->add<Xplicit::LocalPlayerInstance>(packet_spawn.ID);

		return true;
	}

	void Application::setup(const char* ip)
	{
		if (!ip)
			throw std::runtime_error("Invalid DNS!");

		auto net = InstanceManager::get_singleton_ptr()->find<NetworkInstance>("NetworkInstance");
		assert(net);

		net->connect(ip);

		if (setup_network(net))
		{
			InstanceManager::get_singleton_ptr()->add<Xplicit::CameraInstance>();
			EventDispatcher::get_singleton_ptr()->add<Xplicit::LocalPlayerMoveEvent>();
		}
	}
}