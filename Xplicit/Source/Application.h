/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: Application.h
 *			Purpose: Application Context and helpers.
 *
 * =====================================================================
 */

#pragma once

#include "Foundation.h"

namespace Xplicit
{
	class XPLICIT_API InputReceiver : public irr::IEventReceiver
	{
	public:
		virtual bool OnEvent(const SEvent& event)
		{
			if (event.EventType == irr::EET_KEY_INPUT_EVENT)
				m_keys[event.KeyInput.Key] = event.KeyInput.PressedDown;

			return false;
		}

		virtual bool is_key_down(EKEY_CODE keyCode) const
		{
			return m_keys[keyCode];
		}

	public:
		InputReceiver()
		{
			for (u32 i = 0; i < KEY_KEY_CODES_COUNT; ++i)
				m_keys[i] = false;
		}

		virtual ~InputReceiver() {}

		InputReceiver& operator=(const InputReceiver&) = default;
		InputReceiver(const InputReceiver&) = default;

	private:
		bool m_keys[KEY_KEY_CODES_COUNT];

	};

	class XPLICIT_API ApplicationContext final
	{
	private:
		ApplicationContext()
			: Keyboard(nullptr), Dev(nullptr), Reader(nullptr), Writer(nullptr), ShouldExit(false)
		{}

		~ApplicationContext() 
		{
			if (Dev)
				Dev->drop();

			if (Reader)
				Reader->drop();

			if (Writer)
				Writer->drop();
		}

		ApplicationContext& operator=(const ApplicationContext&) = default;
		ApplicationContext(const ApplicationContext&) = default;

	public:
		static ApplicationContext& get_singleton() noexcept
		{
			static ApplicationContext app;

			// return a static singleton, simple!
			return app;
		}

		void set(irr::IrrlichtDevice* dev)
		{
			if (dev)
				Dev = dev;
		}

		void set(InputReceiver* kb)
		{
			if (kb)
				Keyboard = kb;
		}

	public:
		irr::io::IIrrXMLReader<char, IReferenceCounted>* Reader;
		irr::io::IXMLWriter* Writer;
		irr::IrrlichtDevice* Dev;
		InputReceiver* Keyboard;
		bool ShouldExit;

	};

	class UUIDFactory final
	{
	public:
		template <int ver>
		static uuids::uuid version()
		{
			switch (ver)
			{
			case 4:
			{
				std::random_device rd;
				auto seed_data = std::array<int, std::mt19937::state_size> {};
				std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
				std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
				std::mt19937 generator(seq);

				auto gen = uuids::uuid_random_generator(generator);

				return gen();
			}
			}

			return {};
		}

	};
}

#define XML Xplicit::ApplicationContext::get_singleton().Reader
#define IRR  Xplicit::ApplicationContext::get_singleton().Dev
#define KB  Xplicit::ApplicationContext::get_singleton().Keyboard

#ifdef XPLICIT_WINDOWS
#ifndef XPLICIT_GET_DATA_DIR
#define XPLICIT_GET_DATA_DIR(DIR) 	char DIR[4096];\
memset(DIR, 0, 4096);\
\
GetEnvironmentVariableA("XPLICIT_DATA_DIR", DIR, 4096);\

#endif
#else
#error You need XPLICIT_GET_DATA_DIR!
#endif
