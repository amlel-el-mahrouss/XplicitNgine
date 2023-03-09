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

#include "PlayerJoinLeaveEvent.h"
#include "ServerWatchdog.h"

#ifndef get_data_dir
#define get_data_dir(DIR) 	char DIR[4096];\
memset(DIR, 0, 4096);\
\
GetEnvironmentVariableA("XPLICIT_DATA_DIR", DIR, 4096);\

#endif


// forward decl everything.
static void xplicit_create_common();
static void xplicit_attach_mono();
static void xplicit_load_cfg();

static void xplicit_load_cfg()
{
	get_data_dir(data_dir);

	if (*data_dir == 0)
		throw std::runtime_error("NoSuchVariable");

	std::string path = data_dir;
	path += "\\MANIFEST.xml";

	XML = IRR->getFileSystem()->createXMLReaderUTF8(path.c_str());

	if (!XML)
		throw std::runtime_error("No XML provided..");
	
	auto mono = Xplicit::InstanceManager::get_singleton_ptr()->find<Xplicit::MonoEngineInstance>("MonoEngineInstance");

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
						throw std::runtime_error("BadArgumentException");

					std::string str_dllPath = data_dir;

					str_dllPath += "\\Lib\\";
					str_dllPath += dllPath;

					auto assembly_file = mono->open(str_dllPath.c_str());

					if (assembly_file)
					{
						XPLICIT_INFO("Loaded DLL: " + str_dllPath);
						mono->run(assembly_file, 1, argv);
					}
				}
				catch (...)
				{
					XPLICIT_INFO("Bad DLL, skiping..");
				}
			}

			break;
		}
		}
	}
}

static void xplicit_attach_mono()
{
	get_data_dir(data_dir);

	if (*data_dir == 0)
		throw std::runtime_error("NoSuchVariable");

	std::string path = data_dir;
	path += "\\Lib\\Xplicit.dll";

	Xplicit::InstanceManager::get_singleton_ptr()->add<Xplicit::MonoEngineInstance>();
	Xplicit::InstanceManager::get_singleton_ptr()->add<Xplicit::MonoScriptInstance>(path.c_str(), false);
	Xplicit::EventDispatcher::get_singleton_ptr()->add<Xplicit::MonoUpdateEvent>();
}

static void xplicit_create_common()
{
	Xplicit::EventDispatcher::get_singleton_ptr()->add<Xplicit::PlayerJoinLeaveEvent>();
	Xplicit::EventDispatcher::get_singleton_ptr()->add<Xplicit::ServerWatchdogEvent>();
	Xplicit::EventDispatcher::get_singleton_ptr()->add<Xplicit::PhysicsActorEvent>();
	Xplicit::EventDispatcher::get_singleton_ptr()->add<Xplicit::NetworkActorEvent>();
	Xplicit::EventDispatcher::get_singleton_ptr()->add<Xplicit::PlayerActorEvent>();
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
		Xplicit::Application::get_singleton().set(irr::createDevice(irr::video::EDT_NULL));

		// the address is located in the XPLICIT_SERVER_ADDR variable.
		char* addr = getenv("XPLICIT_SERVER_ADDR");

		if (!addr)
			throw std::runtime_error("getenv: XPLICIT_SERVER_ADDR not found!");

		auto server = Xplicit::InstanceManager::get_singleton_ptr()->add<Xplicit::NetworkServerInstance>(addr);
		Xplicit::EventDispatcher::get_singleton_ptr()->add<Xplicit::NetworkServerEvent>(server);

		xplicit_attach_mono();
		xplicit_load_cfg();
		xplicit_create_common();

		while (Xplicit::InstanceManager::get_singleton_ptr() && Xplicit::EventDispatcher::get_singleton_ptr())
		{
			Xplicit::InstanceManager::get_singleton_ptr()->update();
			Xplicit::EventDispatcher::get_singleton_ptr()->update();
		}

		return 0;
	}
	catch (std::runtime_error err)
	{
#ifndef _NDEBUG
		std::string msg;
		msg += "C++ Runtime Error: ";
		msg += err.what();

		XPLICIT_CIRITICAL(msg);
#endif

#ifdef __XPLICIT_WINDOWS
		MessageBoxA(nullptr, msg.c_str(), "FATAL!", MB_OK);
#endif
		return -1;
	}
	catch (Xplicit::NetworkError err)
	{
#ifndef _NDEBUG
		std::string msg;
		msg += "A network Error occured, application will now quit..";

		XPLICIT_CIRITICAL(msg);
#endif

#ifdef __XPLICIT_WINDOWS
		MessageBoxA(nullptr, msg.c_str(), "FATAL!", MB_OK);
#endif

		return -1;
	}
}