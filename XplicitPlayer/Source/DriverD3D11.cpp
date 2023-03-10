/*
 * =====================================================================
 *
 *				XplicitNgin C++ Game Engine
 *			Copyright XPX, all rights reserved.
 *
 *			File: DriverD3D11.cpp
 *			Purpose: C++ Rendering Driver for Direct 3D 11
 *
 * =====================================================================
 */

#include "DriverD3D11.h"

namespace Xplicit::Renderer
{
	DriverSystemD3D11::DriverSystemD3D11() {}
	DriverSystemD3D11::~DriverSystemD3D11() {}

	const char* DriverSystemD3D11::name() noexcept { return ("DriverSystemD3D11"); }
	RENDER_SYSTEM DriverSystemD3D11::api() { return RENDER_SYSTEM::DIRECT3D11; }

	std::unique_ptr<DriverSystemD3D11> make_driver_system_d3d11() { return std::make_unique<DriverSystemD3D11>(); }
}