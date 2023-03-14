/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: Nplicit.h
 *			Purpose: Nplicit standard includes
 *
 * =====================================================================
 */

#pragma once

#include <Avx.h>
#include <Foundation.h>

#include "Config.h"
#include "NMath.h" /* NPlicit math library */

namespace Xplicit::Physics
{
	template <typename TypeFloat = float>
	class NPLICIT_API Vector final
	{
	public:
		TypeFloat X, Y, Z;

	public:
		Vector() = default;
		~Vector() = default;

		explicit Vector(TypeFloat x, TypeFloat y, TypeFloat z)
		{
			this->add(x, y, z);
		}

		Vector& operator=(const Vector&) = delete;
		Vector(const Vector&) = delete;

		Vector& add(TypeFloat x = 0.f, TypeFloat y = 0.f, TypeFloat z = 0.f)
		{
			__m256d x1{ X, Y, Z };
			__m256d x2{ x, y, z };

			__m256d sum = _mm256_add_pd(x1, x2);

			return *this;
		}

		Vector& sub(TypeFloat x = 0.f, TypeFloat y = 0.f, TypeFloat z = 0.f)
		{
			__m256d x1{ X, Y, Z };
			__m256d x2{ x, y, z };

			__m256d sum = _mm256_sub_pd(x1, x2);

			return *this;
		}

		Vector& mul(TypeFloat x = 0.f, TypeFloat y = 0.f, TypeFloat z = 0.f)
		{
			__m256d x1{ X, Y, Z };
			__m256d x2{ x, y, z };

			__m256d sum = _mm256_mul_pd(x1, x2);

			return *this;
		}

	};

	using VectorFloat = Vector<float>;
	using VectorDouble = Vector<double>;
}