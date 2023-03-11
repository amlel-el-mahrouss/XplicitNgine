/*
 * =====================================================================
 *
 *				XplicitNgin C++ Game Engine
 *			Copyright XPX, all rights reserved.
 *
 *			File: SDK.h
 *			Purpose: Xplicit SDK Headers
 *
 * =====================================================================
 */

#pragma once

#include <Event.h>
#include <Instance.h>
#include <Foundation.h>
#include <MonoInterop.h>
#include <MonoInstance.h>
#include <ApplicationContext.h>
#include <NetworkServerInstance.h>

#ifdef XPLICIT_WINDOWS
extern "C"
{
#	include <conio.h>
};
#endif