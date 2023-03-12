/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: DriverD3D11.h
 *			Purpose: C++ Rendering Driver for Direct 3D 11
 *
 * =====================================================================
 */

#pragma once

#include "DriverSystem.h"

#ifdef XPLICIT_WINDOWS

#include <wrl.h> /* Microsoft::WRL::ComPtr */
#include <Avx.h>
#include <d3d11.h>

#include <nuklear/nuklear.h>
#include <nuklear/nuklear_d3d11.h>
#include <nuklear/nuklear_d3d11_pixel_shader.h>
#include <nuklear/nuklear_d3d11_vertex_shader.h>

#else

#error You need Vulkan support!

#endif

#include <nuklear/nuklear.h>
#include <Instance.h>

namespace Xplicit::Renderer
{
	class DriverSystemD3D11 : public DriverSystem
	{
	public:
		DriverSystemD3D11();
		virtual ~DriverSystemD3D11();

		DriverSystemD3D11& operator=(const DriverSystemD3D11&) = default;
		DriverSystemD3D11(const DriverSystemD3D11&) = default;

		virtual const char* name() noexcept override;
		virtual RENDER_SYSTEM api() override;

	};

	std::unique_ptr<DriverSystemD3D11> make_driver_system_d3d11();
}