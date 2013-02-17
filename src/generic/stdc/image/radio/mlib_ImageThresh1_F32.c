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
 * Copyright 2005 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_ImageThresh1_F32.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageThresh1_Fp - thresholding
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageThresh1_Fp(mlib_image       *dst,
 *                                       const mlib_image *src,
 *                                       const mlib_d64   *thresh,
 *                                       const mlib_d64   *ghigh,
 *                                       const mlib_d64   *glow);
 *
 * ARGUMENT
 *      dst     pointer to output image
 *      src     pointer to input image
 *      thresh  array of thresholds
 *      ghigh   array of values above thresholds
 *      glow    array of values below thresholds
 *
 * RESTRICTION
 *      The images must have the same size, and the same number
 *      of channels.
 *      The images can have 1, 2, 3, or 4 channels.
 *      The images can be in MLIB_FLOAT or MLIB_DOUBLE data type.
 *      The type of the output image can be MLIB_BIT, or the same as the
 *      type of the input image.
 *
 * DESCRIPTION
 *      If the pixel band value is above the threshold for that channel,
 *      set the destination to the ghigh value for that channel.
 *      Otherwise, set the destination to the glow value for that channel.
 *
 *                      +- glow[c]   src[x][y][c] <= thresh[c]
 *      dst[x][y][c]  = |
 *                      +- ghigh[c]  src[x][y][c] >  thresh[c]
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_c_ImageThresh1.h>

/* *********************************************************** */

#define	STYPE	mlib_f32
#define	TTYPE	mlib_f32

#define	DO_THRESH(s0, th, gl, gh)	((s0 > th) ? (gh) : (gl))

#define	THRESH1_CMP_SHIFT(s0, th, sh)	((s0 > th) ? (1 << (sh)) : 0)

/* *********************************************************** */

#define	STRIP(pd, ps, w, h, ch, th, gh, gl)                             \
	{                                                               \
	    STYPE s0;                                                   \
	                                                                \
	    for (i = 0; i < h; i++) {                                   \
		for (j = 0; j < w; j++) {                               \
		    for (k = 0; k < ch; k++) {                          \
			s0 = ((STYPE *) ps)[i * src_stride + j * ch +   \
			    k];                                         \
			((STYPE *) pd)[i * dst_stride + j * ch + k] =   \
			    (s0 <= th[k]) ? gl[k] : gh[k];              \
		    }                                                   \
		}                                                       \
	    }                                                           \
	}

/* *********************************************************** */

#ifndef __INTEL_COMPILER

#define	INIT_THRESH(n)                          \
	mlib_f32 thresh##n = thresh[n];         \
	mlib_f32 ghigh##n = ghigh[n];           \
	mlib_f32 glow##n = glow[n]

#else

#define	INIT_THRESH(n)                          \
	volatile mlib_f32 tmp##n = thresh[n];   \
	mlib_f32 thresh##n = tmp##n;            \
	mlib_f32 ghigh##n = ghigh[n];           \
	mlib_f32 glow##n = glow[n]

#endif

/* *********************************************************** */

#define	THRESH0(s0)	DO_THRESH(s0, thresh0, glow0, ghigh0)
#define	THRESH1(s0)	DO_THRESH(s0, thresh1, glow1, ghigh1)

#define	THRESH2(s0)	DO_THRESH(s0, thresh2, glow2, ghigh2)

#define	THRESH3(s0)	DO_THRESH(s0, thresh3, glow3, ghigh3)

/* *********************************************************** */

void
mlib_c_ImageThresh1_F321(
    PARAMS)
{
	mlib_d64 *thresh = (void *)__thresh;
	mlib_d64 *ghigh = (void *)__ghigh;
	mlib_d64 *glow = (void *)__glow;
	STYPE *psrc_row = psrc;
	STYPE *pdst_row = pdst;
	mlib_s32 i, j;
	mlib_f32 r0, r1;
	INIT_THRESH(0);

	for (i = 0; i < height; i++) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < width; j++) {
			r0 = glow0;

			r1 = psrc_row[j];

			if (r1 > thresh0)
				r0 = ghigh0;
			pdst_row[j] = r0;
		}

		psrc_row += src_stride;
		pdst_row += dst_stride;
	}
}

/* *********************************************************** */

void
mlib_c_ImageThresh1_F322(
    PARAMS)
{
	mlib_d64 *thresh = (void *)__thresh;
	mlib_d64 *ghigh = (void *)__ghigh;
	mlib_d64 *glow = (void *)__glow;
	STYPE *psrc_row = psrc;
	STYPE *pdst_row = pdst;
	mlib_s32 i, j;
	mlib_f32 r0, r1;
	INIT_THRESH(0);
	INIT_THRESH(1);

	width <<= 1;

	for (i = 0; i < height; i++) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < width; j += 2) {
			r0 = glow0;
			r1 = glow1;

			if (psrc_row[j] > thresh0)
				r0 = ghigh0;

			if (psrc_row[j + 1] > thresh1)
				r1 = ghigh1;
			pdst_row[j] = r0;
			pdst_row[j + 1] = r1;
		}

		psrc_row += src_stride;
		pdst_row += dst_stride;
	}
}

/* *********************************************************** */

void
mlib_c_ImageThresh1_F323(
    PARAMS)
{
	mlib_d64 *thresh = (void *)__thresh;
	mlib_d64 *ghigh = (void *)__ghigh;
	mlib_d64 *glow = (void *)__glow;
	STYPE *psrc_row = psrc;
	STYPE *pdst_row = pdst;
	mlib_s32 i, j;
	mlib_f32 r0, r1, r2;

	INIT_THRESH(0);
	INIT_THRESH(1);
	INIT_THRESH(2);

	width = 3 * width;

	for (i = 0; i < height; i++) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < width; j += 3) {
			r0 = glow0;
			r1 = glow1;
			r2 = glow2;

			if (psrc_row[j] > thresh0)
				r0 = ghigh0;

			if (psrc_row[j + 1] > thresh1)
				r1 = ghigh1;

			if (psrc_row[j + 2] > thresh2)
				r2 = ghigh2;
			pdst_row[j] = r0;
			pdst_row[j + 1] = r1;
			pdst_row[j + 2] = r2;
		}

		psrc_row += src_stride;
		pdst_row += dst_stride;
	}
}

/* *********************************************************** */

void
mlib_c_ImageThresh1_F324(
    PARAMS)
{
	mlib_d64 *thresh = (void *)__thresh;
	mlib_d64 *ghigh = (void *)__ghigh;
	mlib_d64 *glow = (void *)__glow;
	STYPE *psrc_row = psrc;
	STYPE *pdst_row = pdst;
	mlib_s32 i, j;
	mlib_f32 r0, r1, r2, r3;
	INIT_THRESH(0);
	INIT_THRESH(1);
	INIT_THRESH(2);
	INIT_THRESH(3);

	width *= 4;

	for (i = 0; i < height; i++) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < width; j += 4) {
			r0 = glow0;
			r1 = glow1;
			r2 = glow2;
			r3 = glow3;

			if (psrc_row[j] > thresh0)
				r0 = ghigh0;

			if (psrc_row[j + 1] > thresh1)
				r1 = ghigh1;

			if (psrc_row[j + 2] > thresh2)
				r2 = ghigh2;

			if (psrc_row[j + 3] > thresh3)
				r3 = ghigh3;
			pdst_row[j] = r0;
			pdst_row[j + 1] = r1;
			pdst_row[j + 2] = r2;
			pdst_row[j + 3] = r3;
		}

		psrc_row += src_stride;
		pdst_row += dst_stride;
	}
}

/* *********************************************************** */

void
mlib_c_ImageThresh1_F321_1B(
    PARAMS,
    mlib_s32 dbit_off)
{
	mlib_d64 *thresh = (void *)__thresh;
	mlib_d64 *ghigh = (void *)__ghigh;
	mlib_d64 *glow = (void *)__glow;
	STYPE *psrc_row = psrc;
	mlib_u8 *pdst_row = pdst;
	TTYPE thresh0 = (TTYPE) thresh[0];
	mlib_s32 mhigh, mlow, emask, dst0;
	mlib_s32 i, j, jbit, l;
	mlib_d64 r0, r1, r2, r3, r4, r5, r6, r7;

	mhigh = (ghigh[0] > 0) ? 0xff : 0;
	mlow = (glow[0] > 0) ? 0xff : 0;

	for (i = 0; i < height; i++) {
		j = 0;
		jbit = 0;

		if (dbit_off) {
			mlib_s32 nume = 8 - dbit_off;

			if (nume > width)
				nume = width;
			dst0 = 0;
			emask = 0;

			for (; j < nume; j++) {
				emask |= (1 << (7 - (dbit_off + j)));
				dst0 |=
				    THRESH1_CMP_SHIFT(psrc_row[j], thresh0,
				    7 - (dbit_off + j));
			}

			dst0 = (mhigh & dst0) | (mlow & ~dst0);
			pdst_row[0] = (dst0 & emask) | (pdst_row[0] & ~emask);
			jbit++;
		}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; j <= (width - 16); j += 16) {
			r0 = r1 = r2 = r3 = r4 = r5 = r6 = r7 = 0;

			if (psrc_row[j] > thresh0)
				r0 = 128.0;

			if (psrc_row[j + 1] > thresh0)
				r1 = 64.0;

			if (psrc_row[j + 2] > thresh0)
				r2 = 32.0;

			if (psrc_row[j + 3] > thresh0)
				r3 = 16.0;

			if (psrc_row[j + 4] > thresh0)
				r4 = 8.0;

			if (psrc_row[j + 5] > thresh0)
				r5 = 4.0;

			if (psrc_row[j + 6] > thresh0)
				r6 = 2.0;

			if (psrc_row[j + 7] > thresh0)
				r7 = 1.0;

			dst0 =
			    (mlib_s32)(r0 + r1 + r2 + r3 + r4 + r5 + r6 + r7);

			dst0 = (mhigh & dst0) | (mlow & ~dst0);
			*(pdst_row + jbit) = (mlib_u8)dst0;
			jbit++;

			r0 = r1 = r2 = r3 = r4 = r5 = r6 = r7 = 0;

			if (psrc_row[j + 8] > thresh0)
				r0 = 128.0;

			if (psrc_row[j + 9] > thresh0)
				r1 = 64.0;

			if (psrc_row[j + 10] > thresh0)
				r2 = 32.0;

			if (psrc_row[j + 11] > thresh0)
				r3 = 16.0;

			if (psrc_row[j + 12] > thresh0)
				r4 = 8.0;

			if (psrc_row[j + 13] > thresh0)
				r5 = 4.0;

			if (psrc_row[j + 14] > thresh0)
				r6 = 2.0;

			if (psrc_row[j + 15] > thresh0)
				r7 = 1.0;

			dst0 =
			    (mlib_s32)(r0 + r1 + r2 + r3 + r4 + r5 + r6 + r7);

			dst0 = (mhigh & dst0) | (mlow & ~dst0);
			*(pdst_row + jbit) = (mlib_u8)dst0;
			jbit++;
		}

		if (width - j >= 8) {
			dst0 = THRESH1_CMP_SHIFT(psrc_row[j], thresh0, 7) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 1], thresh0, 6) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 2], thresh0, 5) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 3], thresh0, 4) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 4], thresh0, 3) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 5], thresh0, 2) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 6], thresh0, 1) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 7], thresh0, 0);
			dst0 = (mhigh & dst0) | (mlow & ~dst0);
			*(pdst_row + jbit) = (mlib_u8)dst0;
			jbit++;
			j += 8;
		}

		if (j < width) {
			dst0 = 0;
			l = 7;
			for (; j < width; j++) {
				dst0 |=
				    THRESH1_CMP_SHIFT(psrc_row[j], thresh0, l);
				l--;
			}

			emask = (0xFF << (l + 1));
			dst0 = (mhigh & dst0) | (mlow & ~dst0);
			pdst_row[jbit] =
			    (dst0 & emask) | (pdst_row[jbit] & ~emask);
		}

		psrc_row += src_stride;
		pdst_row += dst_stride;
	}
}

/* *********************************************************** */

void
mlib_c_ImageThresh1_F322_1B(
    PARAMS,
    mlib_s32 dbit_off)
{
	mlib_d64 *thresh = (void *)__thresh;
	mlib_d64 *ghigh = (void *)__ghigh;
	mlib_d64 *glow = (void *)__glow;
	STYPE *psrc_row = psrc;
	mlib_u8 *pdst_row = pdst;
	TTYPE thresh0 = (TTYPE) thresh[0], thresh1 = (TTYPE) thresh[1];
	mlib_s32 mhigh0, mlow0, mhigh, mlow, emask, dst0;
	mlib_s32 i, j, jbit, l;
	mlib_d64 r0, r1, r2, r3, r4, r5, r6, r7;

	mhigh0 = (ghigh[0] > 0) ? 0xaaa : 0;
	mhigh0 |= (ghigh[1] > 0) ? 0x555 : 0;
	mlow0 = (glow[0] > 0) ? 0xaaa : 0;
	mlow0 |= (glow[1] > 0) ? 0x555 : 0;

	width *= 2;

	for (i = 0; i < height; i++) {
		thresh0 = (TTYPE) thresh[0];
		thresh1 = (TTYPE) thresh[1];

		j = 0;
		jbit = 0;
		mhigh = mhigh0 >> (dbit_off & 1);
		mlow = mlow0 >> (dbit_off & 1);

		if (dbit_off) {
			mlib_s32 nume = 8 - dbit_off;

			if (nume > width)
				nume = width;
			dst0 = 0;
			emask = 0;

			for (; j <= (nume - 2); j += 2) {
				emask |= (3 << (6 - (dbit_off + j)));
				dst0 |=
				    THRESH1_CMP_SHIFT(psrc_row[j], thresh0,
				    7 - (dbit_off + j));
				dst0 |=
				    THRESH1_CMP_SHIFT(psrc_row[j + 1], thresh1,
				    6 - (dbit_off + j));
			}

			if (j < nume) {
				emask |= (1 << (7 - (dbit_off + j)));
				dst0 |=
				    THRESH1_CMP_SHIFT(psrc_row[j], thresh0,
				    7 - (dbit_off + j));
/* swap threshes */
				thresh0 = (TTYPE) thresh[1];
				thresh1 = (TTYPE) thresh[0];
				j++;
			}

			dst0 = (mhigh & dst0) | (mlow & ~dst0);
			pdst_row[0] = (dst0 & emask) | (pdst_row[0] & ~emask);
			jbit++;
		}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; j <= (width - 16); j += 16) {
			r0 = r1 = r2 = r3 = r4 = r5 = r6 = r7 = 0;

			if (psrc_row[j] > thresh0)
				r0 = 128.0;

			if (psrc_row[j + 1] > thresh1)
				r1 = 64.0;

			if (psrc_row[j + 2] > thresh0)
				r2 = 32.0;

			if (psrc_row[j + 3] > thresh1)
				r3 = 16.0;

			if (psrc_row[j + 4] > thresh0)
				r4 = 8.0;

			if (psrc_row[j + 5] > thresh1)
				r5 = 4.0;

			if (psrc_row[j + 6] > thresh0)
				r6 = 2.0;

			if (psrc_row[j + 7] > thresh1)
				r7 = 1.0;

			dst0 =
			    (mlib_s32)(r0 + r1 + r2 + r3 + r4 + r5 + r6 + r7);

			dst0 = (mhigh & dst0) | (mlow & ~dst0);
			*(pdst_row + jbit) = (mlib_u8)dst0;
			jbit++;
			r0 = r1 = r2 = r3 = r4 = r5 = r6 = r7 = 0;

			if (psrc_row[j + 8] > thresh0)
				r0 = 128.0;

			if (psrc_row[j + 9] > thresh1)
				r1 = 64.0;

			if (psrc_row[j + 10] > thresh0)
				r2 = 32.0;

			if (psrc_row[j + 11] > thresh1)
				r3 = 16.0;

			if (psrc_row[j + 12] > thresh0)
				r4 = 8.0;

			if (psrc_row[j + 13] > thresh1)
				r5 = 4.0;

			if (psrc_row[j + 14] > thresh0)
				r6 = 2.0;

			if (psrc_row[j + 15] > thresh1)
				r7 = 1.0;

			dst0 =
			    (mlib_s32)(r0 + r1 + r2 + r3 + r4 + r5 + r6 + r7);

			dst0 = (mhigh & dst0) | (mlow & ~dst0);
			*(pdst_row + jbit) = (mlib_u8)dst0;
			jbit++;
		}

		if (width - j >= 8) {
			dst0 = THRESH1_CMP_SHIFT(psrc_row[j], thresh0, 7) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 1], thresh1, 6) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 2], thresh0, 5) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 3], thresh1, 4) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 4], thresh0, 3) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 5], thresh1, 2) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 6], thresh0, 1) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 7], thresh1, 0);
			dst0 = (mhigh & dst0) | (mlow & ~dst0);
			*(pdst_row + jbit) = (mlib_u8)dst0;
			jbit++;
			j += 8;
		}

		if (j < width) {
			dst0 = 0;
			l = 7;
			for (; j <= (width - 2); j += 2) {
				dst0 |=
				    THRESH1_CMP_SHIFT(psrc_row[j], thresh0, l);
				dst0 |=
				    THRESH1_CMP_SHIFT(psrc_row[j + 1], thresh1,
				    l - 1);
				l -= 2;
			}

			if (j < width) {
				dst0 |=
				    THRESH1_CMP_SHIFT(psrc_row[j], thresh0, l);
				l--;
			}

			emask = (0xFF << (l + 1));
			dst0 = (mhigh & dst0) | (mlow & ~dst0);
			pdst_row[jbit] =
			    (dst0 & emask) | (pdst_row[jbit] & ~emask);
		}

		psrc_row += src_stride;
		pdst_row += dst_stride;
	}
}

/* *********************************************************** */

void
mlib_c_ImageThresh1_F323_1B(
    PARAMS,
    mlib_s32 dbit_off)
{
	mlib_d64 *thresh = (void *)__thresh;
	mlib_d64 *ghigh = (void *)__ghigh;
	mlib_d64 *glow = (void *)__glow;
	STYPE *psrc_row = psrc;
	mlib_u8 *pdst_row = pdst;
	TTYPE thresh0, thresh1, thresh2, threshT;
	mlib_s32 mhigh = 0, mlow =
	    0, mhigh0, mlow0, mhigh1, mlow1, mhigh2, mlow2, emask, dst0, dst1;
	mlib_s32 i, j, jbit, k, l;
	mlib_d64 r0, r1, r2, r3, r4, r5, r6, r7;

	if (ghigh[0] > 0)
		mhigh = 0x492492;

	if (ghigh[1] > 0)
		mhigh |= 0x249249;

	if (ghigh[2] > 0)
		mhigh |= 0x924924;

	if (glow[0] > 0)
		mlow = 0x492492;

	if (glow[1] > 0)
		mlow |= 0x249249;

	if (glow[2] > 0)
		mlow |= 0x924924;

	width = 3 * width;

	for (i = 0; i < height; i++) {
		thresh0 = (TTYPE) thresh[0];
		thresh1 = (TTYPE) thresh[1];
		thresh2 = (TTYPE) thresh[2];

		j = 0;
		jbit = 0;
		mhigh0 = mhigh >> (dbit_off & 7);
		mlow0 = mlow >> (dbit_off & 7);
		mhigh1 = mhigh0 >> 1;
		mlow1 = mlow0 >> 1;
		mhigh2 = mhigh0 >> 2;
		mlow2 = mlow0 >> 2;

		if (dbit_off) {
			mlib_s32 nume = 8 - dbit_off;

			if (nume > width)
				nume = width;
			dst0 = 0;
			emask = 0;

			for (; j <= (nume - 3); j += 3) {
				emask |= (7 << (5 - (dbit_off + j)));
				dst0 |=
				    THRESH1_CMP_SHIFT(psrc_row[j], thresh0,
				    7 - (dbit_off + j));
				dst0 |=
				    THRESH1_CMP_SHIFT(psrc_row[j + 1], thresh1,
				    6 - (dbit_off + j));
				dst0 |=
				    THRESH1_CMP_SHIFT(psrc_row[j + 2], thresh2,
				    5 - (dbit_off + j));
			}

			for (; j < nume; j++) {
				emask |= (1 << (7 - (dbit_off + j)));
				dst0 |=
				    THRESH1_CMP_SHIFT(psrc_row[j], thresh0,
				    7 - (dbit_off + j));
/* swap threshes */
				threshT = thresh0;
				thresh0 = thresh1;
				thresh1 = thresh2;
				thresh2 = threshT;
			}

			dst0 = (mhigh0 & dst0) | (mlow0 & ~dst0);
			pdst_row[0] = (dst0 & emask) | (pdst_row[0] & ~emask);
			jbit++;

			mhigh0 = mhigh >> (9 - nume);
			mlow0 = mlow >> (9 - nume);
			mhigh1 = mhigh0 >> 1;
			mlow1 = mlow0 >> 1;
			mhigh2 = mhigh0 >> 2;
			mlow2 = mlow0 >> 2;
		}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; j <= (width - 24); j += 24) {
			r0 = r1 = r2 = r3 = r4 = r5 = r6 = r7 = 0;

			if (psrc_row[j] > thresh0)
				r0 = 128.0;

			if (psrc_row[j + 1] > thresh1)
				r1 = 64.0;

			if (psrc_row[j + 2] > thresh2)
				r2 = 32.0;

			if (psrc_row[j + 3] > thresh0)
				r3 = 16.0;

			if (psrc_row[j + 4] > thresh1)
				r4 = 8.0;

			if (psrc_row[j + 5] > thresh2)
				r5 = 4.0;

			if (psrc_row[j + 6] > thresh0)
				r6 = 2.0;

			if (psrc_row[j + 7] > thresh1)
				r7 = 1.0;

			dst0 =
			    (mlib_s32)(r0 + r1 + r2 + r3 + r4 + r5 + r6 + r7);
			dst0 = (mhigh0 & dst0) | (mlow0 & ~dst0);
			*(pdst_row + jbit) = dst0;
			jbit++;
			r0 = r1 = r2 = r3 = r4 = r5 = r6 = r7 = 0;

			if (psrc_row[j + 8] > thresh2)
				r0 = 128.0;

			if (psrc_row[j + 9] > thresh0)
				r1 = 64.0;

			if (psrc_row[j + 10] > thresh1)
				r2 = 32.0;

			if (psrc_row[j + 11] > thresh2)
				r3 = 16.0;

			if (psrc_row[j + 12] > thresh0)
				r4 = 8.0;

			if (psrc_row[j + 13] > thresh1)
				r5 = 4.0;

			if (psrc_row[j + 14] > thresh2)
				r6 = 2.0;

			if (psrc_row[j + 15] > thresh0)
				r7 = 1.0;

			dst0 =
			    (mlib_s32)(r0 + r1 + r2 + r3 + r4 + r5 + r6 + r7);
			dst0 = (mhigh1 & dst0) | (mlow1 & ~dst0);
			*(pdst_row + jbit) = dst0;
			jbit++;
			r0 = r1 = r2 = r3 = r4 = r5 = r6 = r7 = 0;

			if (psrc_row[j + 16] > thresh1)
				r0 = 128.0;

			if (psrc_row[j + 17] > thresh2)
				r1 = 64.0;

			if (psrc_row[j + 18] > thresh0)
				r2 = 32.0;

			if (psrc_row[j + 19] > thresh1)
				r3 = 16.0;

			if (psrc_row[j + 20] > thresh2)
				r4 = 8.0;

			if (psrc_row[j + 21] > thresh0)
				r5 = 4.0;

			if (psrc_row[j + 22] > thresh1)
				r6 = 2.0;

			if (psrc_row[j + 23] > thresh2)
				r7 = 1.0;

			dst0 =
			    (mlib_s32)(r0 + r1 + r2 + r3 + r4 + r5 + r6 + r7);
			dst0 = (mhigh2 & dst0) | (mlow2 & ~dst0);
			*(pdst_row + jbit) = dst0;
			jbit++;
		}

		if (j < width) {
			k = width - j;
			dst0 = 0;
			l = 31;
			for (; j < width - 3; j += 3) {
				dst0 |=
				    (THRESH1_CMP_SHIFT(psrc_row[j], thresh0,
				    l) | THRESH1_CMP_SHIFT(psrc_row[j + 1],
				    thresh1,
				    l - 1) | THRESH1_CMP_SHIFT(psrc_row[j + 2],
				    thresh2, l - 2));
				l -= 3;
			}

			if (j < width) {
				dst0 |= THRESH1_CMP_SHIFT(
					psrc_row[j],
					thresh0, l);
				if ((j + 1) < width) {
					dst0 |= THRESH1_CMP_SHIFT(
						psrc_row[j + 1],
						thresh1, l - 1);
					if ((j + 2) < width) {
						dst0 |= THRESH1_CMP_SHIFT(
							psrc_row[j + 2],
							thresh2, l - 2);
					}
				}
			}

			l = (k + 7) >> 3;
			k = (l << 3) - k;
			emask = (0xFF << k);

			if (l == 3) {
				dst1 = dst0 >> 24;
				dst1 = (mhigh0 & dst1) | (mlow0 & ~dst1);
				pdst_row[jbit] = dst1;
				dst1 = (dst0 >> 16);
				dst1 = (mhigh1 & dst1) | (mlow1 & ~dst1);
				pdst_row[jbit + 1] = dst1;
				dst1 = (dst0 >> 8);
				dst1 = (mhigh2 & dst1) | (mlow2 & ~dst1);
				pdst_row[jbit + 2] =
				    (dst1 & emask) | (pdst_row[jbit +
				    2] & ~emask);
			} else if (l == 2) {
				dst1 = dst0 >> 24;
				dst1 = (mhigh0 & dst1) | (mlow0 & ~dst1);
				pdst_row[jbit] = dst1;
				dst1 = (dst0 >> 16);
				dst1 = (mhigh1 & dst1) | (mlow1 & ~dst1);
				pdst_row[jbit + 1] =
				    (dst1 & emask) | (pdst_row[jbit +
				    1] & ~emask);
			} else {
				dst1 = dst0 >> 24;
				dst1 = (mhigh0 & dst1) | (mlow0 & ~dst1);
				pdst_row[jbit] =
				    (dst1 & emask) | (pdst_row[jbit] & ~emask);
			}
		}

		psrc_row += src_stride;
		pdst_row += dst_stride;
	}
}

/* *********************************************************** */

void
mlib_c_ImageThresh1_F324_1B(
    PARAMS,
    mlib_s32 dbit_off)
{
	mlib_d64 *thresh = (void *)__thresh;
	mlib_d64 *ghigh = (void *)__ghigh;
	mlib_d64 *glow = (void *)__glow;
	STYPE *psrc_row = psrc;
	mlib_u8 *pdst_row = pdst;
	TTYPE thresh0, thresh1, thresh2, thresh3, threshT;
	mlib_s32 mhigh0, mlow0, mhigh, mlow, emask, dst0;
	mlib_s32 i, j, jbit;
	mlib_d64 r0, r1, r2, r3, r4, r5, r6, r7;

	mhigh0 = (ghigh[0] > 0) ? 0x8888 : 0;
	mhigh0 |= (ghigh[1] > 0) ? 0x4444 : 0;
	mhigh0 |= (ghigh[2] > 0) ? 0x2222 : 0;
	mhigh0 |= (ghigh[3] > 0) ? 0x1111 : 0;

	mlow0 = (glow[0] > 0) ? 0x8888 : 0;
	mlow0 |= (glow[1] > 0) ? 0x4444 : 0;
	mlow0 |= (glow[2] > 0) ? 0x2222 : 0;
	mlow0 |= (glow[3] > 0) ? 0x1111 : 0;

	width *= 4;

	for (i = 0; i < height; i++) {
		thresh0 = (TTYPE) thresh[0];
		thresh1 = (TTYPE) thresh[1];
		thresh2 = (TTYPE) thresh[2];
		thresh3 = (TTYPE) thresh[3];

		j = 0;
		jbit = 0;
		mhigh = mhigh0 >> dbit_off;
		mlow = mlow0 >> dbit_off;

		if (dbit_off) {
			mlib_s32 nume = 8 - dbit_off;

			if (nume > width)
				nume = width;
			dst0 = 0;
			emask = 0;

			for (; j <= (nume - 4); j += 4) {
				emask |= (0xf << (4 - (dbit_off + j)));
				dst0 |=
				    THRESH1_CMP_SHIFT(psrc_row[j], thresh0,
				    7 - (dbit_off + j));
				dst0 |=
				    THRESH1_CMP_SHIFT(psrc_row[j + 1], thresh1,
				    6 - (dbit_off + j));
				dst0 |=
				    THRESH1_CMP_SHIFT(psrc_row[j + 2], thresh2,
				    5 - (dbit_off + j));
				dst0 |=
				    THRESH1_CMP_SHIFT(psrc_row[j + 3], thresh3,
				    4 - (dbit_off + j));
			}

			for (; j < nume; j++) {
				emask |= (1 << (7 - (dbit_off + j)));
				dst0 |=
				    THRESH1_CMP_SHIFT(psrc_row[j], thresh0,
				    7 - (dbit_off + j));
/* swap threshes */
				threshT = thresh0;
				thresh0 = thresh1;
				thresh1 = thresh2;
				thresh2 = thresh3;
				thresh3 = threshT;
			}

			dst0 = (mhigh & dst0) | (mlow & ~dst0);
			pdst_row[0] = (dst0 & emask) | (pdst_row[0] & ~emask);
			jbit++;
		}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; j <= (width - 16); j += 16) {
			r0 = r1 = r2 = r3 = r4 = r5 = r6 = r7 = 0;

			if (psrc_row[j] > thresh0)
				r0 = 128.0;

			if (psrc_row[j + 1] > thresh1)
				r1 = 64.0;

			if (psrc_row[j + 2] > thresh2)
				r2 = 32.0;

			if (psrc_row[j + 3] > thresh3)
				r3 = 16.0;

			if (psrc_row[j + 4] > thresh0)
				r4 = 8.0;

			if (psrc_row[j + 5] > thresh1)
				r5 = 4.0;

			if (psrc_row[j + 6] > thresh2)
				r6 = 2.0;

			if (psrc_row[j + 7] > thresh3)
				r7 = 1.0;

			dst0 =
			    (mlib_s32)(r0 + r1 + r2 + r3 + r4 + r5 + r6 + r7);
			dst0 = (mhigh & dst0) | (mlow & ~dst0);
			pdst_row[jbit] = dst0;
			jbit++;
			r0 = r1 = r2 = r3 = r4 = r5 = r6 = r7 = 0;

			if (psrc_row[j + 8] > thresh0)
				r0 = 128.0;

			if (psrc_row[j + 9] > thresh1)
				r1 = 64.0;

			if (psrc_row[j + 10] > thresh2)
				r2 = 32.0;

			if (psrc_row[j + 11] > thresh3)
				r3 = 16.0;

			if (psrc_row[j + 12] > thresh0)
				r4 = 8.0;

			if (psrc_row[j + 13] > thresh1)
				r5 = 4.0;

			if (psrc_row[j + 14] > thresh2)
				r6 = 2.0;

			if (psrc_row[j + 15] > thresh3)
				r7 = 1.0;

			dst0 =
			    (mlib_s32)(r0 + r1 + r2 + r3 + r4 + r5 + r6 + r7);
			dst0 = (mhigh & dst0) | (mlow & ~dst0);
			pdst_row[jbit] = dst0;
			jbit++;
		}

		if (j <= width - 8) {
			dst0 = (THRESH1_CMP_SHIFT(psrc_row[j], thresh0, 7) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 1], thresh1, 6) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 2], thresh2, 5) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 3], thresh3, 4) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 4], thresh0, 3) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 5], thresh1, 2) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 6], thresh2, 1) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 7], thresh3, 0));
			dst0 = (mhigh & dst0) | (mlow & ~dst0);
			pdst_row[jbit] = dst0;
			jbit++;
			j += 8;
		}

		if (j < width) {
		    dst0 = THRESH1_CMP_SHIFT(psrc_row[j], thresh0, 7);
		    if ((j + 1) < width) {
			dst0 |= THRESH1_CMP_SHIFT(psrc_row[j + 1], thresh1, 6);
			if ((j + 2) < width) {
			    dst0 |= THRESH1_CMP_SHIFT(
				psrc_row[j + 2], thresh2, 5);
			    if ((j + 3) < width) {
				dst0 |= THRESH1_CMP_SHIFT(
					psrc_row[j + 3], thresh3, 4);
				if ((j + 4) < width) {
				    dst0 |= THRESH1_CMP_SHIFT(
					psrc_row[j + 4], thresh0, 3);
				    if ((j + 5) < width) {
					dst0 |= THRESH1_CMP_SHIFT(
						psrc_row[j + 5], thresh1, 2);
					if ((j + 6) < width) {
					    dst0 |= THRESH1_CMP_SHIFT(
						psrc_row[j + 6], thresh2, 1);
					}
				    }
				}
			    }
			}
		    }

		    emask = (0xFF << (8 - (width - j)));
		    dst0 = (mhigh & dst0) | (mlow & ~dst0);
		    pdst_row[jbit] =
			(dst0 & emask) | (pdst_row[jbit] & ~emask);
		}

		psrc_row += src_stride;
		pdst_row += dst_stride;
	}
}

/* *********************************************************** */
