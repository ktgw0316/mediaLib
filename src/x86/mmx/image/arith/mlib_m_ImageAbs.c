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

#pragma ident	"@(#)mlib_m_ImageAbs.c	9.5	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageAbs_S16 - absolute value of each 16-bit pixel
 *      mlib_ImageAbs_S32 - absolute value of each 32-bit pixel
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageAbs_S16 (mlib_s16  *dst,
 *                                     mlib_s16  *src,
 *                                     mlib_s32  dlb,
 *                                     mlib_s32  slb,
 *                                     mlib_s32  wid,
 *                                     mlib_s32  hgt);
 *      mlib_status mlib_ImageAbs_S32 (mlib_s32  *dst,
 *                                     mlib_s32  *src,
 *                                     mlib_s32  dlb,
 *                                     mlib_s32  slb,
 *                                     mlib_s32  wid,
 *                                     mlib_s32  hgt);
 *
 * ARGUMENT
 *      dst     pointer to destination image
 *      src     pointer to source image
 *      dlb     dst image line bytes (bytes / line)
 *      slb     src image line bytes (bytes / line)
 *      wid     src image width in pixels
 *      hgt     src image height in pixels
 *
 * DESCRIPTION
 *      dst(i, j) = |src(i, j)|
 */

#include <mlib_image.h>
#include <mlib_ImageAbs.h>
#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <mmintrin.h>
#endif

/* *********************************************************** */

#define	SHIFT_S16	15

/* *********************************************************** */

#define	SHIFT_S32	31

/* *********************************************************** */

#define	ABS_STDC(s)	((s ^ (s >> SHIFT_S32)) - (s >> SHIFT_S32))

/* *********************************************************** */

mlib_status
mlib_ImageAbs_S16(
    mlib_s16 *dst,
    mlib_s16 *src,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 w,
    mlib_s32 h)
{
	__m64 sm0;
	__m64 dm0;
	__m64 shift = _mm_set1_pi16(SHIFT_S16);
	__m64 *sa64, *da64;

	mlib_s32 s0;
	mlib_s32 i, j;

/* pixels count of src lines */
	mlib_s32 slp = slb >> 1;

/* pixels count of dst lines */
	mlib_s32 dlp = dlb >> 1;
	mlib_s32 wl, wlb, wl1;

/* pointers for pixel and line of src */
	mlib_s16 *sa, *sl;

/* pointers for pixel and line of dst */
	mlib_s16 *da, *dl;

	sl = sa = src;
	dl = da = dst;

	for (j = 0; j < h; j++) {

		for (wl = w; wl > 0 && ((mlib_addr)da & 7); wl--) {
			s0 = (*sa++);
			(*da++) = ABS_STDC(s0);
		}

		wlb = wl >> 2;
		wl1 = wl & 3;

		sa64 = (__m64 *) sa;
		da64 = (__m64 *) da;

		for (i = 0; i < wlb; i++) {
			sm0 = sa64[i];
			dm0 = _mm_sra_pi16(sm0, shift);
			sm0 = _mm_xor_si64(sm0, dm0);
			da64[i] = _mm_sub_pi16(sm0, dm0);
		}

		sa = (mlib_s16 *)(sa64 + i);
		da = (mlib_s16 *)(da64 + i);

		for (i = 0; i < wl1; i++) {
			s0 = sa[i];
			da[i] = ABS_STDC(s0);
		}

/* update src pointers */
		sa = (sl += slp);
/* update dst pointers */
		da = (dl += dlp);
	}

	_mm_empty();

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageAbs_S32(
    mlib_s32 *dst,
    mlib_s32 *src,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 w,
    mlib_s32 h)
{
	__m64 sm0, sm1;
	__m64 dm0, dm1;
	__m64 *sa64, *da64;

	mlib_s32 s0;
	mlib_s32 i, j;

/* pointers for pixel and line of src */
	mlib_s32 *sa, *sl;

/* pointers for pixel and line of dst */
	mlib_s32 *da, *dl;

/* pixels count of src lines */
	mlib_s32 slp = slb >> 2;

/* pixels count of dst lines */
	mlib_s32 dlp = dlb >> 2;
	mlib_s32 wl, wlb, wl1;

	sl = sa = src;
	dl = da = dst;

	for (j = 0; j < h; j++) {

		for (wl = w; wl > 0 && ((mlib_addr)da & 7); wl--) {
			s0 = (*sa++);
			(*da++) = ABS_STDC(s0);
		}

		wlb = wl >> 2;
		wl1 = wl & 3;

		sa64 = (__m64 *) sa;
		da64 = (__m64 *) da;

		for (i = 0; i < wlb; i++) {
			sm0 = sa64[0];
			sm1 = sa64[1];
			dm0 = _mm_srai_pi32(sm0, SHIFT_S32);
			dm1 = _mm_srai_pi32(sm1, SHIFT_S32);
			sm0 = _mm_xor_si64(sm0, dm0);
			sm1 = _mm_xor_si64(sm1, dm1);
			da64[0] = _mm_sub_pi32(sm0, dm0);
			da64[1] = _mm_sub_pi32(sm1, dm1);
			sa64 += 2;
			da64 += 2;
		}

		sa = (mlib_s32 *)sa64;
		da = (mlib_s32 *)da64;

		for (i = 0; i < wl1; i++) {
			s0 = sa[i];
			da[i] = ABS_STDC(s0);
		}

/* update src pointers */
		sa = (sl += slp);
/* update dst pointers */
		da = (dl += dlp);
	}

	_mm_empty();

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
