#include "App.h"

namespace Xplicit::App
{
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

		// read until nothing cant be read.
		while (XML->read())
		{
			switch (XML->getNodeType())
			{
				//we found a new element
			case irr::io::EXN_ELEMENT:
			{
				if (std::string(XML->getNodeName()) == "DNS")
				{
					this->setup(XML->getAttributeValue("IP"));
				}
				else if (std::string(XML->getNodeName()) == "SCENE")
				{
					try
					{
						std::string scene_path = m_data_path;
						scene_path += "\\";
						scene_path += XML->getAttributeValue("VFS");

						if (!IRR->getSceneManager()->loadScene(scene_path.c_str()))
							throw std::runtime_error("SceneException");
					}
					catch (std::runtime_error& err)
					{
#ifndef _NDEBUG
						XPLICIT_INFO(std::string(err.what()).append(": Bad Scene!, exiting.."));
#endif

						std::exit(3);
					}
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

		return true;
	}

	void Application::setup(const char* ip)
	{
		if (!ip)
			throw std::runtime_error("DNSError: Invalid Host!");

		auto net = InstanceManager::get_singleton_ptr()->find<NetworkInstance>("NetworkInstance");
		assert(net);

		net->connect(ip);

		if (setup_network(net))
		{
			InstanceManager::get_singleton_ptr()->add<Xplicit::CameraInstance>();
			InstanceManager::get_singleton_ptr()->add<Xplicit::LocalPlayerInstance>(0);
			EventDispatcher::get_singleton_ptr()->add<Xplicit::LocalPlayerMoveEvent>();
		}
	}
}