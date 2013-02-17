/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License (the "License").
 * You may not use this file except in compliance with the License.
 *
 * You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
 * or http://www.opensolaris.org/os/licensing.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 *
 * When distributing Covered Code, include this CDDL HEADER in each
 * file and include the License file at usr/src/OPENSOLARIS.LICENSE.
 * If applicable, add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your own identifying
 * information: Portions Copyright [yyyy] [name of copyright owner]
 *
 * CDDL HEADER END
 */

/*
 * Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_mmx_utils.c	9.7	07/11/05 SMI"

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <mmintrin.h>
#endif

#include <mlib_types.h>
#include <mlib_mmx_utils.h>

/* *********************************************************** */

#if 0
/*
 * comment out the __sincos weak symbol to fix the coredumps
 * in amd64+sse version
 */
#if defined(__ICC) && defined(__GNUC__)
__typeof__(void *)
	__sincos __attribute__((weak));
#endif
#endif

/* *********************************************************** */

const
#if defined(__ICL) || defined(__INTEL_COMPILER)
_declspec(
	align(8))
#endif
	__m64 mask_pst_32[] = {
		0x0000000000000000, 0xffffffff00000000,
		0x00000000ffffffff, 0xffffffffffffffff
	};

/* *********************************************************** */

const
#if defined(__ICL) || defined(__INTEL_COMPILER)
_declspec(
	align(8))
#endif
	__m64 mask_pst_16[] = {
		0x0000000000000000, 0xffff000000000000,
		0x0000ffff00000000, 0xffffffff00000000,
		0x00000000ffff0000, 0xffff0000ffff0000,
		0x0000ffffffff0000, 0xffffffffffff0000,
		0x000000000000ffff, 0xffff00000000ffff,
		0x0000ffff0000ffff, 0xffffffff0000ffff,
		0x00000000ffffffff, 0xffff0000ffffffff,
		0x0000ffffffffffff, 0xffffffffffffffff
	};

/* *********************************************************** */

const
#if defined(__ICL) || defined(__INTEL_COMPILER)
_declspec(
	align(8))
#endif
	__m64 mask_pst_8[] = {
		0x0000000000000000, 0xff00000000000000, 0x00ff000000000000,
		0xffff000000000000,
		0x0000ff0000000000, 0xff00ff0000000000, 0x00ffff0000000000,
		0xffffff0000000000,
		0x000000ff00000000, 0xff0000ff00000000, 0x00ff00ff00000000,
		0xffff00ff00000000,
		0x0000ffff00000000, 0xff00ffff00000000, 0x00ffffff00000000,
		0xffffffff00000000,
		0x00000000ff000000, 0xff000000ff000000, 0x00ff0000ff000000,
		0xffff0000ff000000,
		0x0000ff00ff000000, 0xff00ff00ff000000, 0x00ffff00ff000000,
		0xffffff00ff000000,
		0x000000ffff000000, 0xff0000ffff000000, 0x00ff00ffff000000,
		0xffff00ffff000000,
		0x0000ffffff000000, 0xff00ffffff000000, 0x00ffffffff000000,
		0xffffffffff000000,
		0x0000000000ff0000, 0xff00000000ff0000, 0x00ff000000ff0000,
		0xffff000000ff0000,
		0x0000ff0000ff0000, 0xff00ff0000ff0000, 0x00ffff0000ff0000,
		0xffffff0000ff0000,
		0x000000ff00ff0000, 0xff0000ff00ff0000, 0x00ff00ff00ff0000,
		0xffff00ff00ff0000,
		0x0000ffff00ff0000, 0xff00ffff00ff0000, 0x00ffffff00ff0000,
		0xffffffff00ff0000,
		0x00000000ffff0000, 0xff000000ffff0000, 0x00ff0000ffff0000,
		0xffff0000ffff0000,
		0x0000ff00ffff0000, 0xff00ff00ffff0000, 0x00ffff00ffff0000,
		0xffffff00ffff0000,
		0x000000ffffff0000, 0xff0000ffffff0000, 0x00ff00ffffff0000,
		0xffff00ffffff0000,
		0x0000ffffffff0000, 0xff00ffffffff0000, 0x00ffffffffff0000,
		0xffffffffffff0000,
		0x000000000000ff00, 0xff0000000000ff00, 0x00ff00000000ff00,
		0xffff00000000ff00,
		0x0000ff000000ff00, 0xff00ff000000ff00, 0x00ffff000000ff00,
		0xffffff000000ff00,
		0x000000ff0000ff00, 0xff0000ff0000ff00, 0x00ff00ff0000ff00,
		0xffff00ff0000ff00,
		0x0000ffff0000ff00, 0xff00ffff0000ff00, 0x00ffffff0000ff00,
		0xffffffff0000ff00,
		0x00000000ff00ff00, 0xff000000ff00ff00, 0x00ff0000ff00ff00,
		0xffff0000ff00ff00,
		0x0000ff00ff00ff00, 0xff00ff00ff00ff00, 0x00ffff00ff00ff00,
		0xffffff00ff00ff00,
		0x000000ffff00ff00, 0xff0000ffff00ff00, 0x00ff00ffff00ff00,
		0xffff00ffff00ff00,
		0x0000ffffff00ff00, 0xff00ffffff00ff00, 0x00ffffffff00ff00,
		0xffffffffff00ff00,
		0x0000000000ffff00, 0xff00000000ffff00, 0x00ff000000ffff00,
		0xffff000000ffff00,
		0x0000ff0000ffff00, 0xff00ff0000ffff00, 0x00ffff0000ffff00,
		0xffffff0000ffff00,
		0x000000ff00ffff00, 0xff0000ff00ffff00, 0x00ff00ff00ffff00,
		0xffff00ff00ffff00,
		0x0000ffff00ffff00, 0xff00ffff00ffff00, 0x00ffffff00ffff00,
		0xffffffff00ffff00,
		0x00000000ffffff00, 0xff000000ffffff00, 0x00ff0000ffffff00,
		0xffff0000ffffff00,
		0x0000ff00ffffff00, 0xff00ff00ffffff00, 0x00ffff00ffffff00,
		0xffffff00ffffff00,
		0x000000ffffffff00, 0xff0000ffffffff00, 0x00ff00ffffffff00,
		0xffff00ffffffff00,
		0x0000ffffffffff00, 0xff00ffffffffff00, 0x00ffffffffffff00,
		0xffffffffffffff00,
		0x00000000000000ff, 0xff000000000000ff, 0x00ff0000000000ff,
		0xffff0000000000ff,
		0x0000ff00000000ff, 0xff00ff00000000ff, 0x00ffff00000000ff,
		0xffffff00000000ff,
		0x000000ff000000ff, 0xff0000ff000000ff, 0x00ff00ff000000ff,
		0xffff00ff000000ff,
		0x0000ffff000000ff, 0xff00ffff000000ff, 0x00ffffff000000ff,
		0xffffffff000000ff,
		0x00000000ff0000ff, 0xff000000ff0000ff, 0x00ff0000ff0000ff,
		0xffff0000ff0000ff,
		0x0000ff00ff0000ff, 0xff00ff00ff0000ff, 0x00ffff00ff0000ff,
		0xffffff00ff0000ff,
		0x000000ffff0000ff, 0xff0000ffff0000ff, 0x00ff00ffff0000ff,
		0xffff00ffff0000ff,
		0x0000ffffff0000ff, 0xff00ffffff0000ff, 0x00ffffffff0000ff,
		0xffffffffff0000ff,
		0x0000000000ff00ff, 0xff00000000ff00ff, 0x00ff000000ff00ff,
		0xffff000000ff00ff,
		0x0000ff0000ff00ff, 0xff00ff0000ff00ff, 0x00ffff0000ff00ff,
		0xffffff0000ff00ff,
		0x000000ff00ff00ff, 0xff0000ff00ff00ff, 0x00ff00ff00ff00ff,
		0xffff00ff00ff00ff,
		0x0000ffff00ff00ff, 0xff00ffff00ff00ff, 0x00ffffff00ff00ff,
		0xffffffff00ff00ff,
		0x00000000ffff00ff, 0xff000000ffff00ff, 0x00ff0000ffff00ff,
		0xffff0000ffff00ff,
		0x0000ff00ffff00ff, 0xff00ff00ffff00ff, 0x00ffff00ffff00ff,
		0xffffff00ffff00ff,
		0x000000ffffff00ff, 0xff0000ffffff00ff, 0x00ff00ffffff00ff,
		0xffff00ffffff00ff,
		0x0000ffffffff00ff, 0xff00ffffffff00ff, 0x00ffffffffff00ff,
		0xffffffffffff00ff,
		0x000000000000ffff, 0xff0000000000ffff, 0x00ff00000000ffff,
		0xffff00000000ffff,
		0x0000ff000000ffff, 0xff00ff000000ffff, 0x00ffff000000ffff,
		0xffffff000000ffff,
		0x000000ff0000ffff, 0xff0000ff0000ffff, 0x00ff00ff0000ffff,
		0xffff00ff0000ffff,
		0x0000ffff0000ffff, 0xff00ffff0000ffff, 0x00ffffff0000ffff,
		0xffffffff0000ffff,
		0x00000000ff00ffff, 0xff000000ff00ffff, 0x00ff0000ff00ffff,
		0xffff0000ff00ffff,
		0x0000ff00ff00ffff, 0xff00ff00ff00ffff, 0x00ffff00ff00ffff,
		0xffffff00ff00ffff,
		0x000000ffff00ffff, 0xff0000ffff00ffff, 0x00ff00ffff00ffff,
		0xffff00ffff00ffff,
		0x0000ffffff00ffff, 0xff00ffffff00ffff, 0x00ffffffff00ffff,
		0xffffffffff00ffff,
		0x0000000000ffffff, 0xff00000000ffffff, 0x00ff000000ffffff,
		0xffff000000ffffff,
		0x0000ff0000ffffff, 0xff00ff0000ffffff, 0x00ffff0000ffffff,
		0xffffff0000ffffff,
		0x000000ff00ffffff, 0xff0000ff00ffffff, 0x00ff00ff00ffffff,
		0xffff00ff00ffffff,
		0x0000ffff00ffffff, 0xff00ffff00ffffff, 0x00ffffff00ffffff,
		0xffffffff00ffffff,
		0x00000000ffffffff, 0xff000000ffffffff, 0x00ff0000ffffffff,
		0xffff0000ffffffff,
		0x0000ff00ffffffff, 0xff00ff00ffffffff, 0x00ffff00ffffffff,
		0xffffff00ffffffff,
		0x000000ffffffffff, 0xff0000ffffffffff, 0x00ff00ffffffffff,
		0xffff00ffffffffff,
		0x0000ffffffffffff, 0xff00ffffffffffff, 0x00ffffffffffffff,
		0xffffffffffffffff
	};

/* *********************************************************** */

const
#if defined(__ICL) || defined(__INTEL_COMPILER)
_declspec(
	align(4))
#endif
	mlib_s32 ltab_8[] = {
		((0xff >> 0) & 0xff), ((0xff >> 1) & 0xff),
		((0xff >> 2) & 0xff), ((0xff >> 3) & 0xff),
		((0xff >> 4) & 0xff), ((0xff >> 5) & 0xff),
		((0xff >> 6) & 0xff), ((0xff >> 7) & 0xff)
	};

const
#if defined(__ICL) || defined(__INTEL_COMPILER)
_declspec(
	align(4))
#endif
	mlib_s32 rtab_8[] = {
		((0xff << 7) & 0xff), ((0xff << 6) & 0xff),
		((0xff << 5) & 0xff), ((0xff << 4) & 0xff),
		((0xff << 3) & 0xff), ((0xff << 2) & 0xff),
		((0xff << 1) & 0xff), ((0xff << 0) & 0xff)
	};

const
#if defined(__ICL) || defined(__INTEL_COMPILER)
_declspec(
	align(4))
#endif
	mlib_s32 ltab_16[] = {
		((0x0f >> 0) & 0x0f), ((0x0f >> 0) & 0x0f),
		((0x0f >> 1) & 0x0f), ((0x0f >> 1) & 0x0f),
		((0x0f >> 2) & 0x0f), ((0x0f >> 2) & 0x0f),
		((0x0f >> 3) & 0x0f), ((0x0f >> 3) & 0x0f)
	};

const
#if defined(__ICL) || defined(__INTEL_COMPILER)
_declspec(
	align(4))
#endif
	mlib_s32 rtab_16[] = {
		((0x0f << 3) & 0x0f), ((0x0f << 3) & 0x0f),
		((0x0f << 2) & 0x0f), ((0x0f << 2) & 0x0f),
		((0x0f << 1) & 0x0f), ((0x0f << 1) & 0x0f),
		((0x0f << 0) & 0x0f), ((0x0f << 0) & 0x0f)
	};

const
#if defined(__ICL) || defined(__INTEL_COMPILER)
_declspec(
	align(4))
#endif
	mlib_s32 ltab_32[] = {
		((0x03 >> 0) & 0x03), ((0x03 >> 0) & 0x03),
		((0x03 >> 0) & 0x03), ((0x03 >> 0) & 0x03),
		((0x03 >> 1) & 0x03), ((0x03 >> 1) & 0x03),
		((0x03 >> 1) & 0x03), ((0x03 >> 1) & 0x03)
	};

const
#if defined(__ICL) || defined(__INTEL_COMPILER)
_declspec(
	align(4))
#endif
	mlib_s32 rtab_32[] = {
		((0x03 << 1) & 0x03), ((0x03 << 1) & 0x03),
		((0x03 << 1) & 0x03), ((0x03 << 1) & 0x03),
		((0x03 << 0) & 0x03), ((0x03 << 0) & 0x03),
		((0x03 << 0) & 0x03), ((0x03 << 0) & 0x03)
	};

/* *********************************************************** */

const
#if defined(__ICL) || defined(__INTEL_COMPILER)
_declspec(
	align(8))
#endif
	__m64 mlib_mask64_arr[] = {
		0x0000000000000000, 0x00000000000000ff,
		0x000000000000ffff, 0x0000000000ffffff,
		0x00000000ffffffff, 0x000000ffffffffff,
		0x0000ffffffffffff, 0x00ffffffffffffff,
		0xffffffffffffffff
	};

/* *********************************************************** */

const
#if defined(__ICL) || defined(__INTEL_COMPILER)
_declspec(
	align(16))
#endif
	__m64 mlib_mask128_arr[] = {
		0x0000000000000000, 0x0000000000000000,
		0x00000000ffffffff, 0x0000000000000000,
		0xffffffffffffffff, 0x0000000000000000,
		0xffffffffffffffff, 0x00000000ffffffff
	};

/* *********************************************************** */
