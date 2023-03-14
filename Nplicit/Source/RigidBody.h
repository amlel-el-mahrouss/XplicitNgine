/*
 * =====================================================================
 *
 *			NplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: RigidBody.h
 *			Purpose:
 *
 * =====================================================================
 */

#pragma once

#include "Nplicit.h"

namespace Xplicit::Physics
{
	template <typename TypeFloat = float>
	struct Box3D
	{
		TypeFloat inertia;
		TypeFloat velocity;
		TypeFloat x, y, z;
	};
}