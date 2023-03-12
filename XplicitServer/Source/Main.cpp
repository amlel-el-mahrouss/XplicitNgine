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

static void xplicit_send_stop_packet(Xplicit::NetworkServerInstance* server); // called when the server stops.
static void xplicit_attach_mono(); // attach mono plugins
static void xplicit_load_shell(); // loads a shell..
static void xplicit_load_cfg(); // load from MANIFEST.xml

// loads everything from the MANIFEST.xml file.
static void xplicit_load_cfg()
{
	XPLICIT_GET_DATA_DIR(data_dir);

	if (*data_dir == 0)
		throw std::runtime_error("getenv: XPLICIT_DATA_DIR doesn't exist!");

	std::string path = data_dir;
	path += "\\MANIFEST.xml";

	XML = IRR->getFileSystem()->createXMLReaderUTF8(path.c_str());

	if (!XML)
		throw std::runtime_error("No XML provided..");
	
	auto mono = Xplicit::InstanceManager::get_singleton_ptr()->get<Xplicit::MonoEngineInstance>("MonoEngineInstance");

	const char* argv[] = {"XplicitNgin"};

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

	if (*data_dir == 0)
		throw std::runtime_error("getenv: XPLICIT_DATA_DIR doesn't exist! Make sure to define it!");

	std::string path = data_dir;
	path += "\\Lib\\Xplicit.dll"; // The Game API dll.

	Xplicit::InstanceManager::get_singleton_ptr()->add<Xplicit::MonoEngineInstance>();
	Xplicit::InstanceManager::get_singleton_ptr()->add<Xplicit::MonoScriptInstance>(path.c_str(), false);
}

static void xplicit_load_shell()
{
#ifdef XPLICIT_WINDOWS
	// change window title to 'Xplicit Server'
	HWND wnd = GetConsoleWindow();
	SetWindowTextA(wnd, "Xplicit Dedicated server");
#endif

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
					xplicit_send_stop_packet(Xplicit::InstanceManager::get_singleton_ptr()->get<Xplicit::NetworkServerInstance>("NetworkServerInstance"));
					Xplicit::ApplicationContext::get_singleton().ShouldExit = true;
				}

				if (strcmp(cmd_buf, "help") == 0)
				{
					puts("-------------- HELP --------------");
					puts("exit: Kills the server.");
					puts("-------------- HELP --------------");
				}

			}
		}
	);

	shell.detach();
}

static void xplicit_send_stop_packet(Xplicit::NetworkServerInstance* server)
{
	if (!server)
		return;

	for (size_t i = 0; i < server->size(); i++)
	{
		server->get(i).packet.CMD[XPLICIT_NETWORK_CMD_STOP] = Xplicit::NETWORK_CMD_STOP;
	}

	server->send();

	auto env = Xplicit::EventDispatcher::get_singleton_ptr()->get<Xplicit::NetworkServerEvent>("NetworkServerEvent");
	XPLICIT_ASSERT(env);

	env->update();
}

// our main entrypoint.
int main(int argc, char** argv)
{
	try
	{
		// init winsock for networking.
		WSADATA wsa;
		Xplicit::init_winsock(&wsa);

		// create a NULL device.
		Xplicit::ApplicationContext::get_singleton().set(irr::createDevice(irr::video::EDT_NULL));

		// the address is located in the XPLICIT_SERVER_ADDR variable.
		char* addr = getenv("XPLICIT_SERVER_ADDR");

		if (!addr)
			throw std::runtime_error("getenv: XPLICIT_SERVER_ADDR does not exist");

		auto server = Xplicit::InstanceManager::get_singleton_ptr()->add<Xplicit::NetworkServerInstance>(addr);
		Xplicit::EventDispatcher::get_singleton_ptr()->add<Xplicit::NetworkServerEvent>(server);

		Xplicit::EventDispatcher::get_singleton_ptr()->add<Xplicit::PlayerJoinLeaveEvent>();
		Xplicit::EventDispatcher::get_singleton_ptr()->add<Xplicit::ServerWatchdogEvent>();
		Xplicit::EventDispatcher::get_singleton_ptr()->add<Xplicit::ActorEvent>();

		xplicit_attach_mono();
		xplicit_load_cfg();

		xplicit_load_shell();

		while (Xplicit::InstanceManager::get_singleton_ptr() && Xplicit::EventDispatcher::get_singleton_ptr())
		{
			Xplicit::EventDispatcher::get_singleton_ptr()->update();
			Xplicit::InstanceManager::get_singleton_ptr()->update();

			if (Xplicit::ApplicationContext::get_singleton().ShouldExit)
				break;
		}

		return 0;
	}
	catch (std::runtime_error err)
	{
#ifndef _NDEBUG
		std::string msg;
		msg += "C++ Exception! ";
		msg += err.what();

		XPLICIT_CRITICAL(msg);
#endif

#ifdef XPLICIT_DEBUG
#ifdef XPLICIT_WINDOWS
		MessageBoxA(nullptr, msg.c_str(), "XplicitNgin", MB_OK);
#endif
#endif

		return -1;
	}
}