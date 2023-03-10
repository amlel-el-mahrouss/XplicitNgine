/*
 * =====================================================================
 *
 *			XplicitNgin C++ Game Engine
 *			Copyright XPX, all rights reserved.
 *
 *			File: Avx.h
 *			Purpose: C functions using AVX, keep in mind that AVX
 *			is not supported on every machine.
 *
 * =====================================================================
 */

#pragma once

#include "Foundation.h"

#ifdef XPLICIT_WINDOWS

#include <immintrin.h>

static inline bool avx_strcmp(const char* str1, const char* str2)
{
	__m256i a = _mm256_load_si256((__m256i*)str2);
	__m256i b = _mm256_load_si256((__m256i*)str2);

	__m256i pcmp = _mm256_cmpeq_epi32(a, b);  // epi8 is fine too
	unsigned bitmask = _mm256_movemask_epi8(pcmp);
	return (bitmask == 0xffffffffU);
}

#ifdef XPLICIT_WANT_AVX
#define XPLICIT_USE_AVX (1)
#endif // idef XPLICIT_WANT_AVX

#endif