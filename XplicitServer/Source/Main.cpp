/*
 * =====================================================================
 *
 *				XplicitNgin C++ Game Engine
 *			Copyright XPX, all rights reserved.
 *
 *			File: Server.cpp
 *			Purpose: Server Main Procedure
 *
 * =====================================================================
 */

#include "SDK.h"

#include "Actor.h"
#include "ServerWatchdog.h"
#include "PlayerJoinLeaveEvent.h"

static void xplicit_send_stop_packet(Xplicit::NetworkServerInstance* server);
static void xplicit_attach_mono();
static void xplicit_load_shell();
static void xplicit_read_xml();

static void xplicit_read_xml()
{
	XPLICIT_GET_DATA_DIR(data_dir);

	std::string path = data_dir;
	path += "\\Server.xml";

	XML = IRR->getFileSystem()->createXMLReaderUTF8(path.c_str());

	if (!XML)
		throw std::runtime_error("No XML provided..");
	
	auto mono = Xplicit::InstanceManager::get_singleton_ptr()->get<Xplicit::MonoEngineInstance>("MonoEngineInstance");

	const char* argv[] = { "XplicitNgin" };

	// read until nothing cant be read.
	while (XML->read())
	{
		switch (XML->getNodeType())
		{
			//we found a new element
		case irr::io::EXN_ELEMENT:
		{
			if (std::string(XML->getNodeName()) == "PLUGIN")
			{
				try
				{
					auto dllPath = XML->getAttributeValue("VFS");
					if (!dllPath)
						throw std::runtime_error("Plugin: Can't find the plugin");

					std::string str_dll_path = data_dir;

					str_dll_path += "\\Lib\\";
					str_dll_path += dllPath;
					
					Xplicit::InstanceManager::get_singleton_ptr()->add<Xplicit::MonoScriptInstance>(str_dll_path.c_str(), false);

					auto assembly_file = mono->open(str_dll_path.c_str());

					if (assembly_file)
					{
						XPLICIT_INFO("Loaded DLL: " + str_dll_path);
						mono->run(assembly_file, 1, argv);
					}
				}
				catch (...)
				{
					XPLICIT_INFO("Skipping bad DLL..");
				}
			}

			break;
		}
		}
	}
}

static void xplicit_attach_mono()
{
	XPLICIT_GET_DATA_DIR(data_dir);

	std::string path = data_dir;
	path += "\\Lib\\Xplicit.dll"; // The Game API dll.

	Xplicit::InstanceManager::get_singleton_ptr()->add<Xplicit::MonoEngineInstance>();
	Xplicit::InstanceManager::get_singleton_ptr()->add<Xplicit::MonoScriptInstance>(path.c_str(), false);
}

static void xplicit_print_help()
{
	puts("-------------- HELP --------------");
	puts("stop: stops the server.");
	puts("-------------- HELP --------------");
}

static void xplicit_load_shell()
{
	std::thread shell(
		[]() -> void
		{
			char cmd_buf[1024];

			while (Xplicit::InstanceManager::get_singleton_ptr() && Xplicit::EventDispatcher::get_singleton_ptr())
			{
				if (!Xplicit::ApplicationContext::get_singleton().ShouldExit)
					std::cout << "# ";

				std::cin.getline(cmd_buf, 1024);

				if (strcmp(cmd_buf, "stop") == 0)
				{
					auto server = Xplicit::InstanceManager::get_singleton_ptr()->get<Xplicit::NetworkServerInstance>("NetworkServerInstance");
					XPLICIT_ASSERT(server);

					xplicit_send_stop_packet(server);
					Xplicit::NetworkServerTraits::send(server);

					Xplicit::ApplicationContext::get_singleton().ShouldExit = true;

				}

				if (strcmp(cmd_buf, "help") == 0)
				{
					xplicit_print_help();
				}

			}
		}
	);

	shell.detach();
}

static void xplicit_send_stop_packet(Xplicit::NetworkServerInstance* server)
{
	XPLICIT_ASSERT(server);

	for (size_t i = 0; i < server->size(); i++)
	{
		server->get(i)->packet.cmd[XPLICIT_NETWORK_CMD_STOP] = Xplicit::NETWORK_CMD_STOP;
	}
}

// our main entrypoint.
int main(int argc, char** argv)
{
	try
	{
		WSADATA wsa;
		Xplicit::init_winsock(&wsa);

		Xplicit::ApplicationContext::get_singleton().set(irr::createDevice(irr::video::EDT_NULL));

		// the address to bind to is located in the XPLICIT_SERVER_ADDR environement variable.
		char* addr = getenv("XPLICIT_SERVER_ADDR");

		if (!addr) 
			throw std::runtime_error("getenv: XPLICIT_SERVER_ADDR does not exist");

		auto server = Xplicit::InstanceManager::get_singleton_ptr()->add<Xplicit::NetworkServerInstance>(addr);
		XPLICIT_ASSERT(server);

		Xplicit::EventDispatcher::get_singleton_ptr()->add<Xplicit::PlayerJoinLeaveEvent>();
		Xplicit::EventDispatcher::get_singleton_ptr()->add<Xplicit::ServerWatchdogEvent>();

		xplicit_attach_mono();
		xplicit_read_xml();

		xplicit_load_shell();

		while (Xplicit::InstanceManager::get_singleton_ptr() && Xplicit::EventDispatcher::get_singleton_ptr())
		{
			Xplicit::NetworkServerTraits::recv(server);

			Xplicit::EventDispatcher::get_singleton_ptr()->update();
			Xplicit::InstanceManager::get_singleton_ptr()->update();

			Xplicit::NetworkServerTraits::send(server);

			if (Xplicit::ApplicationContext::get_singleton().ShouldExit)
				break;
		}

		return 0;
	}
	catch (std::runtime_error err)
	{
#ifdef XPLICIT_DEBUG
		XPLICIT_CRITICAL(err.what());

#ifdef XPLICIT_WINDOWS
		MessageBoxA(nullptr, err.what(), "XplicitNgin", MB_OK);
#endif
#endif

		return -1;
	}
}