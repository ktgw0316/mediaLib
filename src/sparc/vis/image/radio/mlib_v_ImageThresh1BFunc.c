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

#pragma ident	"@(#)mlib_v_ImageThresh1BFunc.c	9.3	07/11/05 SMI"

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_v_ImageThreshFunc.h>

/* *********************************************************** */

static void mlib_v_ImageThresh1B_U8_1_A8D1X8(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    const mlib_s32 *th,
    mlib_s32 hc,
    mlib_s32 lc);

static void mlib_v_ImageThresh1B_U8_1_A8D2X8(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 *th,
    mlib_s32 hc,
    mlib_s32 lc);

static void mlib_v_ImageThresh1B_U8_3_A8D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    const mlib_s32 *th,
    mlib_s32 hc0,
    mlib_s32 lc0);

static void mlib_v_ImageThresh1B_U8_3_A8D2(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 *th,
    mlib_s32 hc0,
    mlib_s32 lc0);

static void mlib_v_ImageThresh1B_U8_3(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 dbit_off,
    const mlib_s32 *th,
    mlib_s32 hc0,
    mlib_s32 lc0);

static void mlib_v_ImageThresh1B_S16_1_A8D1X8(
    const mlib_s16 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    const mlib_s32 *th,
    mlib_s32 hc,
    mlib_s32 lc);

static void mlib_v_ImageThresh1B_S16_1_A8D2X8(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 *th,
    mlib_s32 hc,
    mlib_s32 lc);

static void mlib_v_ImageThresh1B_S16_1(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 dbit_off,
    const mlib_s32 *th,
    mlib_s32 hc,
    mlib_s32 lc);

static void mlib_v_ImageThresh1B_S16_3_A8D1(
    const mlib_s16 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    const mlib_s32 *th,
    mlib_s32 hc0,
    mlib_s32 lc0);

static void mlib_v_ImageThresh1B_S16_3_A8D2(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 *th,
    mlib_s32 hc0,
    mlib_s32 lc0);

static void mlib_v_ImageThresh1B_S16_3(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 dbit_off,
    const mlib_s32 *th,
    mlib_s32 hc0,
    mlib_s32 lc0);

static void mlib_v_ImageThresh1B_U16_1_A8D1X8(
    const mlib_s16 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    const mlib_s32 *th,
    mlib_s32 hc,
    mlib_s32 lc);

static void mlib_v_ImageThresh1B_U16_1_A8D2X8(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 *th,
    mlib_s32 hc,
    mlib_s32 lc);

static void mlib_v_ImageThresh1B_U16_1(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 dbit_off,
    const mlib_s32 *th,
    mlib_s32 hc,
    mlib_s32 lc);

static void mlib_v_ImageThresh1B_U16_3_A8D1(
    const mlib_s16 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    const mlib_s32 *th,
    mlib_s32 hc0,
    mlib_s32 lc0);

static void mlib_v_ImageThresh1B_U16_3_A8D2(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 *th,
    mlib_s32 hc0,
    mlib_s32 lc0);

static void mlib_v_ImageThresh1B_U16_3(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 dbit_off,
    const mlib_s32 *th,
    mlib_s32 hc0,
    mlib_s32 lc0);

static void mlib_v_ImageThresh1B_S32_1_A8D1X8(
    const mlib_s32 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    const mlib_s32 *th,
    mlib_s32 hc,
    mlib_s32 lc);

static void mlib_v_ImageThresh1B_S32_1_A8D2X8(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 *th,
    mlib_s32 hc,
    mlib_s32 lc);

static void mlib_v_ImageThresh1B_S32_1(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 dbit_off,
    const mlib_s32 *th,
    mlib_s32 hc,
    mlib_s32 lc);

static void mlib_v_ImageThresh1B_S32_3_A8D1(
    const mlib_s32 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    const mlib_s32 *th,
    mlib_s32 hc0,
    mlib_s32 lc0);

static void mlib_v_ImageThresh1B_S32_3_A8D2(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 *th,
    mlib_s32 hc0,
    mlib_s32 lc0);

static void mlib_v_ImageThresh1B_S32_3(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 dbit_off,
    const mlib_s32 *th,
    mlib_s32 hc0,
    mlib_s32 lc0);

/* *********************************************************** */

mlib_status
mlib_ImageThresh1B(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *thresh,
    const mlib_s32 *ghigh,
    const mlib_s32 *glow)
{
	mlib_s32 slb, dlb, xsize, ysize, nchan;
	mlib_type stype, dtype;
	void *sa, *da;
	const mlib_s32 X8 = 0x7;
	const mlib_s32 A8D1 = MLIB_IMAGE_ALIGNED8 | MLIB_IMAGE_ONEDVECTOR;
	const mlib_s32 A8D2 = MLIB_IMAGE_ALIGNED8 | MLIB_IMAGE_STRIDE8X;
	const mlib_s32 A8D2X8 =
	    MLIB_IMAGE_ALIGNED8 | MLIB_IMAGE_STRIDE8X | MLIB_IMAGE_WIDTH8X;
	mlib_s32 flags, flagd;
	mlib_s32 dsize, i, k;
	mlib_s32 buff[3 * 8], *th = buff, *gl = buff + 8, *gh = buff + 16;

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_SIZE_EQUAL(src, dst);
	MLIB_IMAGE_CHAN_EQUAL(src, dst);

	stype = mlib_ImageGetType(src);
	dtype = mlib_ImageGetType(dst);
	nchan = mlib_ImageGetChannels(src);
	xsize = mlib_ImageGetWidth(src);
	ysize = mlib_ImageGetHeight(src);
	slb = mlib_ImageGetStride(src);
	dlb = mlib_ImageGetStride(dst);
	sa = mlib_ImageGetData(src);
	da = mlib_ImageGetData(dst);
	flags = mlib_ImageGetFlags(src);
	flagd = mlib_ImageGetFlags(dst);
	dsize = xsize * ysize;

	if (stype == MLIB_BYTE) {

		for (i = 0; i < nchan; i++) {
			gl[i] = glow[i];

			if (thresh[i] > MLIB_U8_MAX) {
				th[i] = MLIB_U8_MAX;
			} else if (thresh[i] < 0) {
				th[i] = 0;
				gl[i] = ghigh[i];
			} else {
				th[i] = thresh[i];
			}
		}
	} else if (stype == MLIB_SHORT) {

		for (i = 0; i < nchan; i++) {
			gl[i] = glow[i];

			if (thresh[i] > MLIB_S16_MAX) {
				th[i] = MLIB_S16_MAX;
			} else if (thresh[i] < MLIB_S16_MIN) {
				th[i] = MLIB_S16_MIN;
				gl[i] = ghigh[i];
			} else {
				th[i] = thresh[i];
			}
		}
	} else if (stype == MLIB_USHORT) {

		for (i = 0; i < nchan; i++) {
			gl[i] = glow[i];

			if (thresh[i] > MLIB_U16_MAX) {
				th[i] = MLIB_U16_MAX;
			} else if (thresh[i] < MLIB_U16_MIN) {
				th[i] = MLIB_U16_MIN;
				gl[i] = ghigh[i];
			} else {
				th[i] = thresh[i];
			}
		}
	} else if (stype == MLIB_INT) {

		for (i = 0; i < nchan; i++) {
			gl[i] = glow[i];
			th[i] = thresh[i];
		}
	} else {

		return (MLIB_FAILURE);
	}

	for (k = 0; k < nchan; k++)
		gh[k] = ghigh[k];
	for (k = nchan; k < 8; k++) {
		th[k] = th[k - nchan];
		gl[k] = gl[k - nchan];
		gh[k] = gh[k - nchan];
	}

	if (nchan == 2) {
		xsize *= 2;
		dsize *= 2;
	} else if (nchan == 4) {
		xsize *= 4;
		dsize *= 4;
	}

	if (dtype != MLIB_BIT) {
		return (MLIB_FAILURE);
	} else {
		mlib_s32 inc, hc, lc, dbit_off = mlib_ImageGetBitOffset(dst);

		if (nchan != 3) {
			hc = (gh[0] > 0) ? 0x8888 : 0;

			if (gh[1] > 0)
				hc |= 0x4444;

			if (gh[2] > 0)
				hc |= 0x2222;

			if (gh[3] > 0)
				hc |= 0x1111;

			lc = (gl[0] > 0) ? 0x8888 : 0;

			if (gl[1] > 0)
				lc |= 0x4444;

			if (gl[2] > 0)
				lc |= 0x2222;

			if (gl[3] > 0)
				lc |= 0x1111;

			inc = (8 - dbit_off) & 3;
		} else {
			hc = (gh[0] > 0) ? 0x492492 : 0;

			if (gh[1] > 0)
				hc |= 0x249249;

			if (gh[2] > 0)
				hc |= 0x924924;

			lc = (gl[0] > 0) ? 0x492492 : 0;

			if (gl[1] > 0)
				lc |= 0x249249;

			if (gl[2] > 0)
				lc |= 0x924924;

			inc = 9 - dbit_off;

			if (inc >= 6)
				inc -= 6;
			else if (inc >= 3)
				inc -= 3;
		}

		th += inc;
		hc = hc >> dbit_off;
		lc = lc >> dbit_off;

		if (stype == MLIB_BYTE) {
			if (nchan != 3) {
				if (((flags & A8D1) == 0) &&
				    ((flagd & MLIB_IMAGE_ONEDVECTOR) == 0) &&
				    ((dsize & X8) == 0)) {

					mlib_v_ImageThresh1B_U8_1_A8D1X8(sa, da,
					    dsize, th, hc, lc);
				} else if ((flags & A8D2X8) == 0 &&
				    dbit_off == 0) {

					mlib_v_ImageThresh1B_U8_1_A8D2X8(sa,
					    slb, da, dlb, xsize, ysize, th, hc,
					    lc);
				} else {

					mlib_v_ImageThresh1B_U8_1(sa, slb, da,
					    dlb, xsize, ysize, dbit_off, th, hc,
					    lc);
				}
			} else {
				if (((flags & A8D1) == 0) &&
				    ((flagd & MLIB_IMAGE_ONEDVECTOR) == 0)) {

					mlib_v_ImageThresh1B_U8_3_A8D1(sa, da,
					    dsize, th, hc, lc);
				} else if ((flags & A8D2) == 0 &&
						dbit_off == 0) {

					mlib_v_ImageThresh1B_U8_3_A8D2(sa, slb,
					    da, dlb, xsize, ysize, th, hc, lc);
				} else {

					mlib_v_ImageThresh1B_U8_3(sa, slb, da,
					    dlb, xsize, ysize, dbit_off, th, hc,
					    lc);
				}
			}
		} else if (stype == MLIB_SHORT) {

			if (nchan != 3) {
				if (((flags & A8D1) == 0) &&
				    ((flagd & MLIB_IMAGE_ONEDVECTOR) == 0) &&
				    ((dsize & X8) == 0)) {

					mlib_v_ImageThresh1B_S16_1_A8D1X8(sa,
					    da, dsize, th, hc, lc);
				} else if ((flags & A8D2X8) == 0 &&
				    dbit_off == 0) {

					mlib_v_ImageThresh1B_S16_1_A8D2X8(sa,
					    slb, da, dlb, xsize, ysize, th, hc,
					    lc);
				} else {

					mlib_v_ImageThresh1B_S16_1(sa, slb, da,
					    dlb, xsize, ysize, dbit_off, th, hc,
					    lc);
				}
			} else {
				if (((flags & A8D1) == 0) &&
				    ((flagd & MLIB_IMAGE_ONEDVECTOR) == 0)) {

					mlib_v_ImageThresh1B_S16_3_A8D1(sa, da,
					    dsize, th, hc, lc);
				} else if ((flags & A8D2) == 0 &&
						dbit_off == 0) {

					mlib_v_ImageThresh1B_S16_3_A8D2(sa, slb,
					    da, dlb, xsize, ysize, th, hc, lc);
				} else {

					mlib_v_ImageThresh1B_S16_3(sa, slb, da,
					    dlb, xsize, ysize, dbit_off, th, hc,
					    lc);
				}
			}
		} else if (stype == MLIB_USHORT) {

			if (nchan != 3) {
				if (((flags & A8D1) == 0) &&
				    ((flagd & MLIB_IMAGE_ONEDVECTOR) == 0) &&
				    ((dsize & X8) == 0)) {

					mlib_v_ImageThresh1B_U16_1_A8D1X8(sa,
					    da, dsize, th, hc, lc);
				} else if ((flags & A8D2X8) == 0 &&
				    dbit_off == 0) {

					mlib_v_ImageThresh1B_U16_1_A8D2X8(sa,
					    slb, da, dlb, xsize, ysize, th, hc,
					    lc);
				} else {

					mlib_v_ImageThresh1B_U16_1(sa, slb, da,
					    dlb, xsize, ysize, dbit_off, th, hc,
					    lc);
				}
			} else {
				if (((flags & A8D1) == 0) &&
				    ((flagd & MLIB_IMAGE_ONEDVECTOR) == 0)) {

					mlib_v_ImageThresh1B_U16_3_A8D1(sa, da,
					    dsize, th, hc, lc);
				} else if ((flags & A8D2) == 0 &&
						dbit_off == 0) {

					mlib_v_ImageThresh1B_U16_3_A8D2(sa, slb,
					    da, dlb, xsize, ysize, th, hc, lc);
				} else {

					mlib_v_ImageThresh1B_U16_3(sa, slb, da,
					    dlb, xsize, ysize, dbit_off, th, hc,
					    lc);
				}
			}
		} else if (stype == MLIB_INT) {

			if (nchan != 3) {
				if (((flags & A8D1) == 0) &&
				    ((flagd & MLIB_IMAGE_ONEDVECTOR) == 0) &&
				    ((dsize & X8) == 0)) {

					mlib_v_ImageThresh1B_S32_1_A8D1X8(sa,
					    da, dsize, th, hc, lc);
				} else if ((flags & A8D2X8) == 0 &&
				    dbit_off == 0) {

					mlib_v_ImageThresh1B_S32_1_A8D2X8(sa,
					    slb, da, dlb, xsize, ysize, th, hc,
					    lc);
				} else {

					mlib_v_ImageThresh1B_S32_1(sa, slb, da,
					    dlb, xsize, ysize, dbit_off, th, hc,
					    lc);
				}
			} else {
				if (((flags & A8D1) == 0) &&
				    ((flagd & MLIB_IMAGE_ONEDVECTOR) == 0)) {

					mlib_v_ImageThresh1B_S32_3_A8D1(sa, da,
					    dsize, th, hc, lc);
				} else if ((flags & A8D2) == 0 &&
						dbit_off == 0) {

					mlib_v_ImageThresh1B_S32_3_A8D2(sa, slb,
					    da, dlb, xsize, ysize, th, hc, lc);
				} else {

					mlib_v_ImageThresh1B_S32_3(sa, slb, da,
					    dlb, xsize, ysize, dbit_off, th, hc,
					    lc);
				}
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MLIB_V_IMAGETHRESH1B_U8(sd, tdh, tdl, hc, lc, dc)       \
	sdh = vis_fexpand_hi(sd);                               \
	sdl = vis_fexpand_lo(sd);                               \
	cmaskh = vis_fcmple16(sdh, tdh);                        \
	cmaskl = vis_fcmple16(sdl, tdl);                        \
	cmask = (cmaskh << 4) | cmaskl;                         \
	dc = (hc & (~cmask)) | (lc & (cmask))

/* *********************************************************** */

void
mlib_v_ImageThresh1B_U8_1_A8D1X8(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    const mlib_s32 *th,
    mlib_s32 hc,
    mlib_s32 lc)
{
/* aligned src address */
	mlib_d64 *sp;

/* destination address */
	mlib_u8 *dp;

/* source data */
	mlib_d64 sd;

/* destination data */
	mlib_u8 dc;

/* threshold data */
	mlib_d64 td;

/* temporaries used in macro */
	mlib_d64 sdh, sdl;

/* temporaries used in macro */
	mlib_s32 cmask, cmaskh, cmaskl;

/* loop variables */
	mlib_s32 j;

/* prepare the threshold */
	td = vis_fexpand(vis_to_float((th[0] << 24) | (th[1] << 16) | (th[2] <<
	    8) | th[3]));

	sp = (mlib_d64 *)src;
	dp = (mlib_u8 *)dst;

/* 8-pixel loop */
#pragma pipeloop(0)
	for (j = 0; j < dsize / 8; j++) {
		sd = *sp;
		sp++;
		MLIB_V_IMAGETHRESH1B_U8(sd, td, td, hc, lc, dc);
		*dp = dc;
		dp++;
	}
}

/* *********************************************************** */

void
mlib_v_ImageThresh1B_U8_1_A8D2X8(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 *th,
    mlib_s32 hc,
    mlib_s32 lc)
{
/* aligned src address */
	mlib_d64 *sp, *sl;

/* destination address */
	mlib_u8 *dp, *dl;

/* source data */
	mlib_d64 sd;

/* destination data */
	mlib_u8 dc;

/* threshold data */
	mlib_d64 td;

/* temporaries used in macro */
	mlib_d64 sdh, sdl;

/* temporaries used in macro */
	mlib_s32 cmask, cmaskh, cmaskl;

/* loop variables */
	mlib_s32 i, j;

/* prepare the threshold */
	td = vis_fexpand(vis_to_float((th[0] << 24) | (th[1] << 16) | (th[2] <<
	    8) | th[3]));

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_u8 *)dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* 8-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i < xsize / 8; i++) {
			sd = *sp;
			sp++;
			MLIB_V_IMAGETHRESH1B_U8(sd, td, td, hc, lc, dc);
			*dp = dc;
			dp++;
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_u8 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageThresh1B_U8_1(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 dbit_off,
    const mlib_s32 *th,
    mlib_s32 hc,
    mlib_s32 lc)
{
/* aligned src address */
	mlib_d64 *sp;

/* pointer to begin of a src line */
	mlib_u8 *sl;

/* destination address */
	mlib_u8 *dp;

/* pointer to begin of a dst line */
	mlib_u8 *dl;

/* source data */
	mlib_d64 sd, s0, s1;

/* destination data */
	mlib_u8 dc;

/* threshold data */
	mlib_d64 td;

/* temporaries used in macro */
	mlib_d64 sdh, sdl;

/* temporaries used in macro */
	mlib_s32 cmask, cmaskh, cmaskl;

/* edge mask */
	mlib_s32 emask0, emask1, nume;

/* loop variables */
	mlib_s32 i, j;

/* prepare the threshold */
	td = vis_fexpand(vis_to_float((th[0] << 24) | (th[1] << 16) | (th[2] <<
	    8) | th[3]));

	sl = (void *)src;
	dl = dst;

	nume = (8 - dbit_off);

	if (nume > xsize)
		nume = xsize;
	emask0 = (((0xFF00 >> nume) & 0xFF) >> dbit_off);
	xsize -= nume;
	emask1 = (0xFF << (8 - (xsize & 7))) & 0xFF;

	sl -= dbit_off;

/* row loop */
	for (j = 0; j < ysize; j++) {
/* prepare source & destination address */
		sp = (mlib_d64 *)vis_alignaddr(sl, 0);
		dp = dl;

		s0 = vis_ld_d64_nf(sp);
		s1 = vis_ld_d64_nf(sp + 1);
		sd = vis_faligndata(s0, s1);
		sp++;
		MLIB_V_IMAGETHRESH1B_U8(sd, td, td, hc, lc, dc);
		dp[0] = (dc & emask0) | (dp[0] & ~emask0);
		dp++;

/* 8-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i < xsize / 8; i++) {
			s0 = sp[0];
			s1 = vis_ld_d64_nf(sp + 1);
			sd = vis_faligndata(s0, s1);
			sp++;
			MLIB_V_IMAGETHRESH1B_U8(sd, td, td, hc, lc, dc);
			*dp = dc;
			dp++;
		}

/* last 8-pixel column */

		if (emask1) {
			s0 = vis_ld_d64_nf(sp + 1);
			sd = vis_faligndata(s1, s0);
			sp++;
			MLIB_V_IMAGETHRESH1B_U8(sd, td, td, hc, lc, dc);
			dp[0] = (dc & emask1) | (dp[0] & ~emask1);
		}

		sl += slb;
		dl += dlb;
	}
}

/* *********************************************************** */

void
mlib_v_ImageThresh1B_U8_3_A8D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    const mlib_s32 *th,
    mlib_s32 hc0,
    mlib_s32 lc0)
{
/* aligned src address */
	mlib_d64 *sp;

/* destination address */
	mlib_u8 *dp;

/* source data */
	mlib_d64 sd;

/* destination data */
	mlib_u8 dc;

/* threshold data */
	mlib_d64 td0, td1, td2;

/* ghigh data */
	mlib_u8 hc1, hc2;

/* glow data */
	mlib_u8 lc1, lc2;

/* temporaries used in macro */
	mlib_d64 sdh, sdl;

/* temporaries used in macro */
	mlib_s32 cmask, cmaskh, cmaskl;

/* temporaries */
	mlib_u32 tu;

/* temporaries */
	mlib_s32 tt0, tt1, tt2;

/* loop variables */
	mlib_s32 j;

/* prepare the threshold */
	tt0 = th[0];
	tt1 = th[1];
	tt2 = th[2];
	tu = (tt0 << 24) | (tt1 << 16) | (tt2 << 8) | tt0;
	td0 = vis_fexpand(vis_to_float(tu));
	tu = (tt1 << 24) | (tt2 << 16) | (tt0 << 8) | tt1;
	td1 = vis_fexpand(vis_to_float(tu));
	td2 = vis_freg_pair(vis_read_lo(td0), vis_read_hi(td1));

	hc1 = (hc0 >> 2);
	hc2 = (hc0 >> 1);
	lc1 = (lc0 >> 2);
	lc2 = (lc0 >> 1);

	sp = (mlib_d64 *)src;
	dp = (mlib_u8 *)dst;

/* 8-pixel column loop */
#pragma pipeloop(0)
	for (j = 0; j < dsize / 8; j++) {
		sd = *sp;
		sp++;
		MLIB_V_IMAGETHRESH1B_U8(sd, td0, td1, hc0, lc0, dc);
		*dp = dc;
		dp++;

		sd = *sp;
		sp++;
		MLIB_V_IMAGETHRESH1B_U8(sd, td2, td0, hc2, lc2, dc);
		*dp = dc;
		dp++;

		sd = *sp;
		sp++;
		MLIB_V_IMAGETHRESH1B_U8(sd, td1, td2, hc1, lc1, dc);
		*dp = dc;
		dp++;
	}
}

/* *********************************************************** */

void
mlib_v_ImageThresh1B_U8_3_A8D2(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 *th,
    mlib_s32 hc0,
    mlib_s32 lc0)
{
/* aligned src address */
	mlib_d64 *sp;

/* pointer to a line of src */
	mlib_d64 *sl;

/* pointer to end of src */
	mlib_u8 *send;

/* destination address */
	mlib_u8 *dp;

/* pointer to a line of dst */
	mlib_u8 *dl;

/* source data */
	mlib_d64 sd;

/* destination data */
	mlib_u8 dc;

/* threshold data */
	mlib_d64 td0, td1, td2;

/* ghigh data */
	mlib_u8 hc1, hc2;

/* glow data */
	mlib_u8 lc1, lc2;

/* temporaries used in macro */
	mlib_d64 sdh, sdl;

/* temporaries used in macro */
	mlib_s32 cmask, cmaskh, cmaskl;

/* temporaries */
	mlib_u32 tu;

/* temporaries */
	mlib_s32 tt0, tt1, tt2;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 i, j;

/* prepare the threshold */
	tt0 = th[0];
	tt1 = th[1];
	tt2 = th[2];
	tu = (tt0 << 24) | (tt1 << 16) | (tt2 << 8) | tt0;
	td0 = vis_fexpand(vis_to_float(tu));
	tu = (tt1 << 24) | (tt2 << 16) | (tt0 << 8) | tt1;
	td1 = vis_fexpand(vis_to_float(tu));
	td2 = vis_freg_pair(vis_read_lo(td0), vis_read_hi(td1));

	hc1 = (hc0 >> 2);
	hc2 = (hc0 >> 1);
	lc1 = (lc0 >> 2);
	lc2 = (lc0 >> 1);

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_u8 *)dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

		send = (mlib_u8 *)sl + 3 * xsize - 1;

/* 8-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i < xsize / 8; i++) {
			sd = *sp;
			sp++;
			MLIB_V_IMAGETHRESH1B_U8(sd, td0, td1, hc0, lc0, dc);
			*dp = dc;
			dp++;

			sd = *sp;
			sp++;
			MLIB_V_IMAGETHRESH1B_U8(sd, td2, td0, hc2, lc2, dc);
			*dp = dc;
			dp++;

			sd = *sp;
			sp++;
			MLIB_V_IMAGETHRESH1B_U8(sd, td1, td2, hc1, lc1, dc);
			*dp = dc;
			dp++;
		}

/* last 8-pixel column */

		if ((mlib_addr)sp <= (mlib_addr)send) {
			sd = vis_ld_d64_nf(sp);
			emask = vis_edge8(sp, send);
			sp++;
			MLIB_V_IMAGETHRESH1B_U8(sd, td0, td1, hc0, lc0, dc);
			dp[0] = (dc & emask) | (dp[0] & ~emask);
			dp++;

			if ((mlib_addr)sp <= (mlib_addr)send) {
				sd = vis_ld_d64_nf(sp);
				emask = vis_edge8(sp, send);
				sp++;
				MLIB_V_IMAGETHRESH1B_U8(sd, td2, td0, hc2, lc2,
				    dc);
				dp[0] = (dc & emask) | (dp[0] & ~emask);
				dp++;

				if ((mlib_addr)sp <= (mlib_addr)send) {
					sd = vis_ld_d64_nf(sp);
					emask = vis_edge8(sp, send);
					sp++;
					MLIB_V_IMAGETHRESH1B_U8(sd, td1, td2,
					    hc1, lc1, dc);
					dp[0] = (dc & emask) | (dp[0] & ~emask);
					dp++;
				}
			}
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_u8 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageThresh1B_U8_3(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 dbit_off,
    const mlib_s32 *th,
    mlib_s32 hc0,
    mlib_s32 lc0)
{
/* aligned src address */
	mlib_d64 *sp;

/* pointer to a line of src */
	mlib_u8 *sl;

/* destination address */
	mlib_u8 *dp;

/* pointer to a line of dst */
	mlib_u8 *dl;

/* source data */
	mlib_d64 sd, s0, s1;

/* destination data */
	mlib_u8 dc;

/* threshold data */
	mlib_d64 td0, td1, td2;

/* ghigh data */
	mlib_u8 hc1, hc2;

/* glow data */
	mlib_u8 lc1, lc2;

/* temporaries used in macro */
	mlib_d64 sdh, sdl;

/* temporaries used in macro */
	mlib_s32 cmask, cmaskh, cmaskl;

/* temporaries */
	mlib_u32 tu;
	mlib_s32 esize, dsize = 3 * xsize;

/* temporaries */
	mlib_s32 tt0, tt1, tt2;

/* edge mask */
	mlib_s32 emask0, emask1, nume;

/* loop variables */
	mlib_s32 i, j;

/* prepare the threshold */
	tt0 = th[0];
	tt1 = th[1];
	tt2 = th[2];
	tu = (tt0 << 24) | (tt1 << 16) | (tt2 << 8) | tt0;
	td0 = vis_fexpand(vis_to_float(tu));
	tu = (tt1 << 24) | (tt2 << 16) | (tt0 << 8) | tt1;
	td1 = vis_fexpand(vis_to_float(tu));
	td2 = vis_freg_pair(vis_read_lo(td0), vis_read_hi(td1));

	hc1 = (hc0 >> 1);
	hc2 = (hc0 >> 2);
	lc1 = (lc0 >> 1);
	lc2 = (lc0 >> 2);

	sl = (void *)src;
	dl = dst;

	nume = (8 - dbit_off);

	if (nume > dsize)
		nume = dsize;
	emask0 = (((0xFF00 >> nume) & 0xFF) >> dbit_off);

	esize = 3 * (xsize & 7) - nume;

	if (esize < 0) {
		xsize -= 8;
		esize += 24;
	}

	emask1 = (0xFF << (8 - (esize & 7))) & 0xFF;

	sl -= dbit_off;

/* row loop */
	for (j = 0; j < ysize; j++) {
/* prepare source & destination address */
		sp = (mlib_d64 *)vis_alignaddr(sl, 0);
		dp = dl;

		s0 = vis_ld_d64_nf(sp);
		s1 = vis_ld_d64_nf(sp + 1);
		sd = vis_faligndata(s0, s1);
		sp++;
		MLIB_V_IMAGETHRESH1B_U8(sd, td0, td1, hc0, lc0, dc);
		dp[0] = (dc & emask0) | (dp[0] & ~emask0);
		dp++;

/* 8-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i < xsize / 8; i++) {
			s0 = sp[0];
			s1 = sp[1];
			sd = vis_faligndata(s0, s1);
			sp++;
			MLIB_V_IMAGETHRESH1B_U8(sd, td2, td0, hc1, lc1, dc);
			*dp = dc;
			dp++;

			s0 = sp[0];
			s1 = sp[1];
			sd = vis_faligndata(s0, s1);
			sp++;
			MLIB_V_IMAGETHRESH1B_U8(sd, td1, td2, hc2, lc2, dc);
			*dp = dc;
			dp++;

			s0 = sp[0];
			s1 = vis_ld_d64_nf(sp + 1);
			sd = vis_faligndata(s0, s1);
			sp++;
			MLIB_V_IMAGETHRESH1B_U8(sd, td0, td1, hc0, lc0, dc);
			*dp = dc;
			dp++;
		}

/* last 8-pixel column */

		if (esize) {
			s0 = vis_ld_d64_nf(sp + 1);
			sd = vis_faligndata(s1, s0);
			sp++;
			MLIB_V_IMAGETHRESH1B_U8(sd, td2, td0, hc1, lc1, dc);

			if (esize >= 8) {
				dp[0] = dc;
				dp++;

				s1 = vis_ld_d64_nf(sp + 1);
				sd = vis_faligndata(s0, s1);
				sp++;
				MLIB_V_IMAGETHRESH1B_U8(sd, td1, td2, hc2, lc2,
				    dc);

				if (esize >= 16) {
					dp[0] = dc;
					dp++;

					s0 = vis_ld_d64_nf(sp + 1);
					sd = vis_faligndata(s1, s0);
					MLIB_V_IMAGETHRESH1B_U8(sd, td0, td1,
					    hc0, lc0, dc);
				}
			}
			if (emask1)
				dp[0] = (dc & emask1) | (dp[0] & ~emask1);
		}

		sl += slb;
		dl += dlb;
	}
}

/* *********************************************************** */

#define	MLIB_V_IMAGETHRESH1B_S16(sdh, sdl, tdh, tdl, hc, lc, dc)   \
	cmaskh = vis_fcmple16(sdh, tdh);                           \
	cmaskl = vis_fcmple16(sdl, tdl);                           \
	cmask = (cmaskh << 4) | cmaskl;                            \
	dc = (hc & (~cmask)) | (lc & (cmask))

/* *********************************************************** */

#define	MLIB_V_IMAGETHRESH1B_U16(sdh, sdl, tdh, tdl, hc, lc, dc)   \
	sdh = vis_fpadd16(sdh, s16min_4);                          \
	sdl = vis_fpadd16(sdl, s16min_4);                          \
	cmaskh = vis_fcmple16(sdh, tdh);                           \
	cmaskl = vis_fcmple16(sdl, tdl);                           \
	cmask = (cmaskh << 4) | cmaskl;                            \
	dc = (hc & (~cmask)) | (lc & (cmask))

/* *********************************************************** */

void
mlib_v_ImageThresh1B_S16_1_A8D1X8(
    const mlib_s16 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    const mlib_s32 *th,
    mlib_s32 hc,
    mlib_s32 lc)
{
/* aligned src address */
	mlib_d64 *sp;

/* destination address */
	mlib_u8 *dp;

/* source data */
	mlib_d64 sdh, sdl;

/* destination data */
	mlib_u8 dc;

/* threshold data */
	mlib_d64 td;

/* temporaries used in macro */
	mlib_s32 cmaskh, cmaskl, cmask;

/* loop variables */
	mlib_s32 j;

/* prepare the threshold */
	td = vis_to_double((th[0] << 16) | (th[1] & 0xFFFF),
	    (th[2] << 16) | (th[3] & 0xFFFF));

	sp = (mlib_d64 *)src;
	dp = (mlib_u8 *)dst;

/* 8-pixel loop */
#pragma pipeloop(0)
	for (j = 0; j < dsize / 8; j++) {
		sdh = *sp;
		sp++;
		sdl = *sp;
		sp++;
		MLIB_V_IMAGETHRESH1B_S16(sdh, sdl, td, td, hc, lc, dc);
		*dp = dc;
		dp++;
	}
}

/* *********************************************************** */

void
mlib_v_ImageThresh1B_S16_1_A8D2X8(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 *th,
    mlib_s32 hc,
    mlib_s32 lc)
{
/* aligned src address */
	mlib_d64 *sp;

/* pointer to begin of a src line */
	mlib_d64 *sl;

/* destination address */
	mlib_u8 *dp;

/* pointer to begin of a dst line */
	mlib_u8 *dl;

/* source data */
	mlib_d64 sdh, sdl;

/* destination data */
	mlib_u8 dc;

/* threshold data */
	mlib_d64 td;

/* temporaries used in macro */
	mlib_s32 cmaskh, cmaskl, cmask;

/* loop variables */
	mlib_s32 i, j;

/* prepare the threshold */
	td = vis_to_double((th[0] << 16) | (th[1] & 0xFFFF),
	    (th[2] << 16) | (th[3] & 0xFFFF));

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_u8 *)dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* 8-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i < xsize / 8; i++) {
			sdh = *sp;
			sp++;
			sdl = *sp;
			sp++;
			MLIB_V_IMAGETHRESH1B_S16(sdh, sdl, td, td, hc, lc, dc);
			*dp = dc;
			dp++;
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_u8 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageThresh1B_S16_1(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 dbit_off,
    const mlib_s32 *th,
    mlib_s32 hc,
    mlib_s32 lc)
{
/* aligned src address */
	mlib_d64 *sp;

/* pointer to begin of a src line */
	mlib_s16 *sl;

/* destination address */
	mlib_u8 *dp;

/* pointer to begin of a dst line */
	mlib_u8 *dl;

/* source data */
	mlib_d64 sdh, sdl, s0, s1, s2;

/* destination data */
	mlib_u8 dc;

/* threshold data */
	mlib_d64 td;

/* temporaries used in macro */
	mlib_s32 cmaskh, cmaskl, cmask;

/* edge mask */
	mlib_s32 emask0, emask1, nume;

/* loop variables */
	mlib_s32 i, j;

/* prepare the threshold */
	td = vis_to_double((th[0] << 16) | (th[1] & 0xFFFF),
	    (th[2] << 16) | (th[3] & 0xFFFF));

	sl = (void *)src;
	dl = dst;

	nume = (8 - dbit_off);

	if (nume > xsize)
		nume = xsize;
	emask0 = (((0xFF00 >> nume) & 0xFF) >> dbit_off);
	xsize -= nume;
	emask1 = (0xFF << (8 - (xsize & 7))) & 0xFF;

	sl -= dbit_off;

/* row loop */
	for (j = 0; j < ysize; j++) {
/* prepare source & destination address */
		sp = (mlib_d64 *)vis_alignaddr(sl, 0);
		dp = dl;

		s0 = vis_ld_d64_nf(sp);
		s1 = vis_ld_d64_nf(sp + 1);
		sdh = vis_faligndata(s0, s1);
		s2 = vis_ld_d64_nf(sp + 2);
		sdl = vis_faligndata(s1, s2);
		sp += 2;
		MLIB_V_IMAGETHRESH1B_S16(sdh, sdl, td, td, hc, lc, dc);
		dp[0] = (dc & emask0) | (dp[0] & ~emask0);
		dp++;

/* 8-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i < xsize / 8; i++) {
			s0 = sp[0];
			s1 = sp[1];
			sdh = vis_faligndata(s0, s1);
			s2 = vis_ld_d64_nf(sp + 2);
			sdl = vis_faligndata(s1, s2);
			sp += 2;
			MLIB_V_IMAGETHRESH1B_S16(sdh, sdl, td, td, hc, lc, dc);
			*dp = dc;
			dp++;
		}

/* last 8-pixel column */

		if (emask1) {
			s1 = vis_ld_d64_nf(sp + 1);
			sdh = vis_faligndata(s2, s1);
			s2 = vis_ld_d64_nf(sp + 2);
			sdl = vis_faligndata(s1, s2);
			MLIB_V_IMAGETHRESH1B_S16(sdh, sdl, td, td, hc, lc, dc);
			dp[0] = (dc & emask1) | (dp[0] & ~emask1);
		}

		sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
		dl = (mlib_u8 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageThresh1B_S16_3_A8D1(
    const mlib_s16 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    const mlib_s32 *th,
    mlib_s32 hc0,
    mlib_s32 lc0)
{
/* aligned src address */
	mlib_d64 *sp;

/* destination address */
	mlib_u8 *dp;

/* source data */
	mlib_d64 sdh, sdl;

/* destination data */
	mlib_u8 dc;

/* threshold data */
	mlib_d64 td0, td1, td2;

/* ghigh data */
	mlib_u8 hc1, hc2;

/* glow data */
	mlib_u8 lc1, lc2;

/* temporaries */
	mlib_s32 tt0, tt1, tt2;

/* temporaries used in macro */
	mlib_s32 cmaskh, cmaskl, cmask;

/* loop variables */
	mlib_s32 j;

/* prepare the threshold */
	tt0 = th[0];
	tt1 = th[1];
	tt2 = th[2];
	td0 =
	    vis_to_double((tt0 << 16) | (tt1 & 0xFFFF),
	    (tt2 << 16) | (tt0 & 0xFFFF));
	td1 =
	    vis_to_double((tt1 << 16) | (tt2 & 0xFFFF),
	    (tt0 << 16) | (tt1 & 0xFFFF));
	td2 = vis_freg_pair(vis_read_lo(td0), vis_read_hi(td1));

	hc1 = (hc0 >> 1);
	hc2 = (hc0 >> 2);
	lc1 = (lc0 >> 1);
	lc2 = (lc0 >> 2);

	sp = (mlib_d64 *)src;
	dp = (mlib_u8 *)dst;

/* 8-pixel loop */
#pragma pipeloop(0)
	for (j = 0; j < dsize / 8; j++) {
		sdh = *sp;
		sp++;
		sdl = *sp;
		sp++;
		MLIB_V_IMAGETHRESH1B_S16(sdh, sdl, td0, td1, hc0, lc0, dc);
		*dp = dc;
		dp++;

		sdh = *sp;
		sp++;
		sdl = *sp;
		sp++;
		MLIB_V_IMAGETHRESH1B_S16(sdh, sdl, td2, td0, hc1, lc1, dc);
		*dp = dc;
		dp++;

		sdh = *sp;
		sp++;
		sdl = *sp;
		sp++;
		MLIB_V_IMAGETHRESH1B_S16(sdh, sdl, td1, td2, hc2, lc2, dc);
		*dp = dc;
		dp++;
	}
}

/* *********************************************************** */

void
mlib_v_ImageThresh1B_S16_3_A8D2(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 *th,
    mlib_s32 hc0,
    mlib_s32 lc0)
{
/* aligned src address */
	mlib_d64 *sp;

/* pointer to begin of a src line */
	mlib_d64 *sl;

/* pointer to end of a src line */
	mlib_s16 *send;

/* destination address */
	mlib_u8 *dp;

/* pointer to begin of a src line */
	mlib_u8 *dl;

/* source data */
	mlib_d64 sdh, sdl;

/* destination data */
	mlib_u8 dc;

/* threshold data */
	mlib_d64 td0, td1, td2;

/* ghigh data */
	mlib_u8 hc1, hc2;

/* glow data */
	mlib_u8 lc1, lc2;

/* temporaries */
	mlib_s32 tt0, tt1, tt2;

/* temporaries used in macro */
	mlib_s32 cmaskh, cmaskl, cmask;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 i, j;

/* prepare the threshold */
	tt0 = th[0];
	tt1 = th[1];
	tt2 = th[2];
	td0 =
	    vis_to_double((tt0 << 16) | (tt1 & 0xFFFF),
	    (tt2 << 16) | (tt0 & 0xFFFF));
	td1 =
	    vis_to_double((tt1 << 16) | (tt2 & 0xFFFF),
	    (tt0 << 16) | (tt1 & 0xFFFF));
	td2 = vis_freg_pair(vis_read_lo(td0), vis_read_hi(td1));

	hc1 = (hc0 >> 1);
	hc2 = (hc0 >> 2);
	lc1 = (lc0 >> 1);
	lc2 = (lc0 >> 2);

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_u8 *)dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

		send = (mlib_s16 *)sp + 3 * xsize - 1;

/* 8-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i < xsize / 8; i++) {
			sdh = *sp;
			sp++;
			sdl = *sp;
			sp++;
			MLIB_V_IMAGETHRESH1B_S16(sdh, sdl, td0, td1, hc0, lc0,
			    dc);
			*dp = dc;
			dp++;

			sdh = *sp;
			sp++;
			sdl = *sp;
			sp++;
			MLIB_V_IMAGETHRESH1B_S16(sdh, sdl, td2, td0, hc1, lc1,
			    dc);
			*dp = dc;
			dp++;

			sdh = *sp;
			sp++;
			sdl = *sp;
			sp++;
			MLIB_V_IMAGETHRESH1B_S16(sdh, sdl, td1, td2, hc2, lc2,
			    dc);
			*dp = dc;
			dp++;
		}

/* last 8-pixel column */

		if ((mlib_addr)sp <= (mlib_addr)send) {
			sdh = vis_ld_d64_nf(sp);
			emask = vis_edge16(sp, send) << 4;
			sp++;
			sdl = vis_ld_d64_nf(sp);

			if ((mlib_addr)sp <= (mlib_addr)send) {
				emask |= vis_edge16(sp, send);
			}

			sp++;
			MLIB_V_IMAGETHRESH1B_S16(sdh, sdl, td0, td1, hc0, lc0,
			    dc);
			dp[0] = (dc & emask) | (dp[0] & ~emask);
			dp++;

			if ((mlib_addr)sp <= (mlib_addr)send) {
				sdh = vis_ld_d64_nf(sp);
				emask = vis_edge16(sp, send) << 4;
				sp++;
				sdl = vis_ld_d64_nf(sp);

				if ((mlib_addr)sp <= (mlib_addr)send) {
					emask |= vis_edge16(sp, send);
				}

				sp++;
				MLIB_V_IMAGETHRESH1B_S16(sdh, sdl, td2, td0,
				    hc1, lc1, dc);
				dp[0] = (dc & emask) | (dp[0] & ~emask);
				dp++;

				if ((mlib_addr)sp <= (mlib_addr)send) {
					sdh = vis_ld_d64_nf(sp);
					emask = vis_edge16(sp, send) << 4;
					sp++;
					sdl = vis_ld_d64_nf(sp);

					if ((mlib_addr)sp <= (mlib_addr)send) {
						emask |= vis_edge16(sp, send);
					}

					sp++;
					MLIB_V_IMAGETHRESH1B_S16(sdh, sdl, td1,
					    td2, hc2, lc2, dc);
					dp[0] = (dc & emask) | (dp[0] & ~emask);
					dp++;
				}
			}
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_u8 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageThresh1B_S16_3(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 dbit_off,
    const mlib_s32 *th,
    mlib_s32 hc0,
    mlib_s32 lc0)
{
/* aligned src address */
	mlib_d64 *sp;

/* pointer to begin of a src line */
	mlib_s16 *sl;

/* destination address */
	mlib_u8 *dp;

/* pointer to begin of a dst line */
	mlib_u8 *dl;

/* source data */
	mlib_d64 sdh, sdl, s0, s1, s2;

/* destination data */
	mlib_u8 dc;

/* threshold data */
	mlib_d64 td0, td1, td2;

/* ghigh data */
	mlib_u8 hc1, hc2;

/* glow data */
	mlib_u8 lc1, lc2;

/* temporaries */
	mlib_s32 tt0, tt1, tt2;

/* temporaries used in macro */
	mlib_s32 cmaskh, cmaskl, cmask;
	mlib_s32 esize, dsize = 3 * xsize;

/* edge mask */
	mlib_s32 emask0, emask1, nume;

/* loop variables */
	mlib_s32 i, j;

/* prepare the threshold */
	tt0 = th[0];
	tt1 = th[1];
	tt2 = th[2];
	td0 =
	    vis_to_double((tt0 << 16) | (tt1 & 0xFFFF),
	    (tt2 << 16) | (tt0 & 0xFFFF));
	td1 =
	    vis_to_double((tt1 << 16) | (tt2 & 0xFFFF),
	    (tt0 << 16) | (tt1 & 0xFFFF));
	td2 = vis_freg_pair(vis_read_lo(td0), vis_read_hi(td1));

	hc1 = (hc0 >> 1);
	hc2 = (hc0 >> 2);
	lc1 = (lc0 >> 1);
	lc2 = (lc0 >> 2);

	sl = (void *)src;
	dl = dst;

	nume = (8 - dbit_off);

	if (nume > dsize)
		nume = dsize;
	emask0 = (((0xFF00 >> nume) & 0xFF) >> dbit_off);

	esize = 3 * (xsize & 7) - nume;

	if (esize < 0) {
		xsize -= 8;
		esize += 24;
	}

	emask1 = (0xFF << (8 - (esize & 7))) & 0xFF;

	sl -= dbit_off;

/* row loop */
	for (j = 0; j < ysize; j++) {
/* prepare source & destination address */
		sp = (mlib_d64 *)vis_alignaddr(sl, 0);
		dp = dl;

		s0 = vis_ld_d64_nf(sp);
		s1 = vis_ld_d64_nf(sp + 1);
		sdh = vis_faligndata(s0, s1);
		s2 = vis_ld_d64_nf(sp + 2);
		sdl = vis_faligndata(s1, s2);
		sp += 2;
		MLIB_V_IMAGETHRESH1B_S16(sdh, sdl, td0, td1, hc0, lc0, dc);
		dp[0] = (dc & emask0) | (dp[0] & ~emask0);
		dp++;

/* 8-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i < xsize / 8; i++) {
			s0 = sp[0];
			s1 = sp[1];
			sdh = vis_faligndata(s0, s1);
			s2 = sp[2];
			sdl = vis_faligndata(s1, s2);
			sp += 2;
			MLIB_V_IMAGETHRESH1B_S16(sdh, sdl, td2, td0, hc1, lc1,
			    dc);
			*dp = dc;
			dp++;

			s0 = sp[0];
			s1 = sp[1];
			sdh = vis_faligndata(s0, s1);
			s2 = sp[2];
			sdl = vis_faligndata(s1, s2);
			sp += 2;
			MLIB_V_IMAGETHRESH1B_S16(sdh, sdl, td1, td2, hc2, lc2,
			    dc);
			*dp = dc;
			dp++;

			s0 = sp[0];
			s1 = sp[1];
			sdh = vis_faligndata(s0, s1);
			s2 = vis_ld_d64_nf(sp + 2);
			sdl = vis_faligndata(s1, s2);
			sp += 2;
			MLIB_V_IMAGETHRESH1B_S16(sdh, sdl, td0, td1, hc0, lc0,
			    dc);
			*dp = dc;
			dp++;
		}

/* last 8-pixel column */

		if (esize) {
			s0 = vis_ld_d64_nf(sp + 1);
			sdh = vis_faligndata(s2, s0);
			s1 = vis_ld_d64_nf(sp + 2);
			sdl = vis_faligndata(s0, s1);
			sp += 2;
			MLIB_V_IMAGETHRESH1B_S16(sdh, sdl, td2, td0, hc1, lc1,
			    dc);

			if (esize >= 8) {
				*dp = dc;
				dp++;

				s0 = vis_ld_d64_nf(sp + 1);
				sdh = vis_faligndata(s1, s0);
				s1 = vis_ld_d64_nf(sp + 2);
				sdl = vis_faligndata(s0, s1);
				sp += 2;
				MLIB_V_IMAGETHRESH1B_S16(sdh, sdl, td1, td2,
				    hc2, lc2, dc);

				if (esize >= 16) {
					*dp = dc;
					dp++;

					s0 = vis_ld_d64_nf(sp + 1);
					sdh = vis_faligndata(s1, s0);
					s1 = vis_ld_d64_nf(sp + 2);
					sdl = vis_faligndata(s0, s1);
					MLIB_V_IMAGETHRESH1B_S16(sdh, sdl, td0,
					    td1, hc0, lc0, dc);
				}
			}
			if (emask1)
				dp[0] = (dc & emask1) | (dp[0] & ~emask1);
		}

		sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
		dl = (mlib_u8 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageThresh1B_U16_1_A8D1X8(
    const mlib_s16 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    const mlib_s32 *th,
    mlib_s32 hc,
    mlib_s32 lc)
{
/* aligned src address */
	mlib_d64 *sp;

/* destination address */
	mlib_u8 *dp;

/* source data */
	mlib_d64 sdh, sdl;

/* destination data */
	mlib_u8 dc;

/* threshold data */
	mlib_d64 td;

/* temporaries used in macro */
	mlib_s32 cmaskh, cmaskl, cmask;

/* loop variables */
	mlib_s32 j;
	mlib_d64 s16min_4 = vis_to_double_dup(0x80008000);

/* prepare the threshold */
	td = vis_to_double((th[0] << 16) | (th[1] & 0xFFFF),
	    (th[2] << 16) | (th[3] & 0xFFFF));
	td = vis_fpadd16(td, s16min_4);
	sp = (mlib_d64 *)src;
	dp = (mlib_u8 *)dst;

/* 8-pixel loop */
#pragma pipeloop(0)
	for (j = 0; j < dsize / 8; j++) {
		sdh = *sp;
		sp++;
		sdl = *sp;
		sp++;
		MLIB_V_IMAGETHRESH1B_U16(sdh, sdl, td, td, hc, lc, dc);
		*dp = dc;
		dp++;
	}
}

/* *********************************************************** */

void
mlib_v_ImageThresh1B_U16_1_A8D2X8(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 *th,
    mlib_s32 hc,
    mlib_s32 lc)
{
/* aligned src address */
	mlib_d64 *sp;

/* pointer to begin of a src line */
	mlib_d64 *sl;

/* destination address */
	mlib_u8 *dp;

/* pointer to begin of a dst line */
	mlib_u8 *dl;

/* source data */
	mlib_d64 sdh, sdl;

/* destination data */
	mlib_u8 dc;

/* threshold data */
	mlib_d64 td;

/* temporaries used in macro */
	mlib_s32 cmaskh, cmaskl, cmask;

/* loop variables */
	mlib_s32 i, j;
	mlib_d64 s16min_4 = vis_to_double_dup(0x80008000);

/* prepare the threshold */
	td = vis_to_double((th[0] << 16) | (th[1] & 0xFFFF),
	    (th[2] << 16) | (th[3] & 0xFFFF));
	td = vis_fpadd16(td, s16min_4);

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_u8 *)dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* 8-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i < xsize / 8; i++) {
			sdh = *sp;
			sp++;
			sdl = *sp;
			sp++;
			MLIB_V_IMAGETHRESH1B_U16(sdh, sdl, td, td, hc, lc, dc);
			*dp = dc;
			dp++;
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_u8 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageThresh1B_U16_1(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 dbit_off,
    const mlib_s32 *th,
    mlib_s32 hc,
    mlib_s32 lc)
{
/* aligned src address */
	mlib_d64 *sp;

/* pointer to begin of a src line */
	mlib_s16 *sl;

/* destination address */
	mlib_u8 *dp;

/* pointer to begin of a dst line */
	mlib_u8 *dl;

/* source data */
	mlib_d64 sdh, sdl, s0, s1, s2;

/* destination data */
	mlib_u8 dc;

/* threshold data */
	mlib_d64 td;

/* temporaries used in macro */
	mlib_s32 cmaskh, cmaskl, cmask;

/* edge mask */
	mlib_s32 emask0, emask1, nume;

/* loop variables */
	mlib_s32 i, j;
	mlib_d64 s16min_4 = vis_to_double_dup(0x80008000);

/* prepare the threshold */
	td = vis_to_double((th[0] << 16) | (th[1] & 0xFFFF),
	    (th[2] << 16) | (th[3] & 0xFFFF));
	td = vis_fpadd16(td, s16min_4);

	sl = (void *)src;
	dl = dst;

	nume = (8 - dbit_off);

	if (nume > xsize)
		nume = xsize;
	emask0 = (((0xFF00 >> nume) & 0xFF) >> dbit_off);
	xsize -= nume;
	emask1 = (0xFF << (8 - (xsize & 7))) & 0xFF;

	sl -= dbit_off;

/* row loop */
	for (j = 0; j < ysize; j++) {
/* prepare source & destination address */
		sp = (mlib_d64 *)vis_alignaddr(sl, 0);
		dp = dl;

		s0 = vis_ld_d64_nf(sp);
		s1 = vis_ld_d64_nf(sp + 1);
		sdh = vis_faligndata(s0, s1);
		s2 = vis_ld_d64_nf(sp + 2);
		sdl = vis_faligndata(s1, s2);
		sp += 2;
		MLIB_V_IMAGETHRESH1B_U16(sdh, sdl, td, td, hc, lc, dc);
		dp[0] = (dc & emask0) | (dp[0] & ~emask0);
		dp++;

/* 8-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i < xsize / 8; i++) {
			s0 = sp[0];
			s1 = sp[1];
			sdh = vis_faligndata(s0, s1);
			s2 = vis_ld_d64_nf(sp + 2);
			sdl = vis_faligndata(s1, s2);
			sp += 2;
			MLIB_V_IMAGETHRESH1B_U16(sdh, sdl, td, td, hc, lc, dc);
			*dp = dc;
			dp++;
		}

/* last 8-pixel column */

		if (emask1) {
			s1 = vis_ld_d64_nf(sp + 1);
			sdh = vis_faligndata(s2, s1);
			s2 = vis_ld_d64_nf(sp + 2);
			sdl = vis_faligndata(s1, s2);
			MLIB_V_IMAGETHRESH1B_U16(sdh, sdl, td, td, hc, lc, dc);
			dp[0] = (dc & emask1) | (dp[0] & ~emask1);
		}

		sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
		dl = (mlib_u8 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageThresh1B_U16_3_A8D1(
    const mlib_s16 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    const mlib_s32 *th,
    mlib_s32 hc0,
    mlib_s32 lc0)
{
/* aligned src address */
	mlib_d64 *sp;

/* destination address */
	mlib_u8 *dp;

/* source data */
	mlib_d64 sdh, sdl;

/* destination data */
	mlib_u8 dc;

/* threshold data */
	mlib_d64 td0, td1, td2;

/* ghigh data */
	mlib_u8 hc1, hc2;

/* glow data */
	mlib_u8 lc1, lc2;

/* temporaries */
	mlib_s32 tt0, tt1, tt2;

/* temporaries used in macro */
	mlib_s32 cmaskh, cmaskl, cmask;

/* loop variables */
	mlib_s32 j;
	mlib_d64 s16min_4 = vis_to_double_dup(0x80008000);

/* prepare the threshold */
	tt0 = th[0];
	tt1 = th[1];
	tt2 = th[2];
	td0 =
	    vis_to_double((tt0 << 16) | (tt1 & 0xFFFF),
	    (tt2 << 16) | (tt0 & 0xFFFF));
	td1 =
	    vis_to_double((tt1 << 16) | (tt2 & 0xFFFF),
	    (tt0 << 16) | (tt1 & 0xFFFF));
	td2 = vis_freg_pair(vis_read_lo(td0), vis_read_hi(td1));

	td0 = vis_fpadd16(td0, s16min_4);
	td1 = vis_fpadd16(td1, s16min_4);
	td2 = vis_fpadd16(td2, s16min_4);

	hc1 = (hc0 >> 1);
	hc2 = (hc0 >> 2);
	lc1 = (lc0 >> 1);
	lc2 = (lc0 >> 2);

	sp = (mlib_d64 *)src;
	dp = (mlib_u8 *)dst;

/* 8-pixel loop */
#pragma pipeloop(0)
	for (j = 0; j < dsize / 8; j++) {
		sdh = *sp;
		sp++;
		sdl = *sp;
		sp++;
		MLIB_V_IMAGETHRESH1B_U16(sdh, sdl, td0, td1, hc0, lc0, dc);
		*dp = dc;
		dp++;

		sdh = *sp;
		sp++;
		sdl = *sp;
		sp++;
		MLIB_V_IMAGETHRESH1B_U16(sdh, sdl, td2, td0, hc1, lc1, dc);
		*dp = dc;
		dp++;

		sdh = *sp;
		sp++;
		sdl = *sp;
		sp++;
		MLIB_V_IMAGETHRESH1B_U16(sdh, sdl, td1, td2, hc2, lc2, dc);
		*dp = dc;
		dp++;
	}
}

/* *********************************************************** */

void
mlib_v_ImageThresh1B_U16_3_A8D2(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 *th,
    mlib_s32 hc0,
    mlib_s32 lc0)
{
/* aligned src address */
	mlib_d64 *sp;

/* pointer to begin of a src line */
	mlib_d64 *sl;

/* pointer to end of a src line */
	mlib_s16 *send;

/* destination address */
	mlib_u8 *dp;

/* pointer to begin of a src line */
	mlib_u8 *dl;

/* source data */
	mlib_d64 sdh, sdl;

/* destination data */
	mlib_u8 dc;

/* threshold data */
	mlib_d64 td0, td1, td2;

/* ghigh data */
	mlib_u8 hc1, hc2;

/* glow data */
	mlib_u8 lc1, lc2;

/* temporaries */
	mlib_s32 tt0, tt1, tt2;

/* temporaries used in macro */
	mlib_s32 cmaskh, cmaskl, cmask;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 i, j;
	mlib_d64 s16min_4 = vis_to_double_dup(0x80008000);

/* prepare the threshold */
	tt0 = th[0];
	tt1 = th[1];
	tt2 = th[2];
	td0 =
	    vis_to_double((tt0 << 16) | (tt1 & 0xFFFF),
	    (tt2 << 16) | (tt0 & 0xFFFF));
	td1 =
	    vis_to_double((tt1 << 16) | (tt2 & 0xFFFF),
	    (tt0 << 16) | (tt1 & 0xFFFF));
	td2 = vis_freg_pair(vis_read_lo(td0), vis_read_hi(td1));

	td0 = vis_fpadd16(td0, s16min_4);
	td1 = vis_fpadd16(td1, s16min_4);
	td2 = vis_fpadd16(td2, s16min_4);

	hc1 = (hc0 >> 1);
	hc2 = (hc0 >> 2);
	lc1 = (lc0 >> 1);
	lc2 = (lc0 >> 2);

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_u8 *)dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

		send = (mlib_s16 *)sp + 3 * xsize - 1;

/* 8-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i < xsize / 8; i++) {
			sdh = *sp;
			sp++;
			sdl = *sp;
			sp++;
			MLIB_V_IMAGETHRESH1B_U16(sdh, sdl, td0, td1, hc0, lc0,
			    dc);
			*dp = dc;
			dp++;

			sdh = *sp;
			sp++;
			sdl = *sp;
			sp++;
			MLIB_V_IMAGETHRESH1B_U16(sdh, sdl, td2, td0, hc1, lc1,
			    dc);
			*dp = dc;
			dp++;

			sdh = *sp;
			sp++;
			sdl = *sp;
			sp++;
			MLIB_V_IMAGETHRESH1B_U16(sdh, sdl, td1, td2, hc2, lc2,
			    dc);
			*dp = dc;
			dp++;
		}

/* last 8-pixel column */

		if ((mlib_addr)sp <= (mlib_addr)send) {
			sdh = vis_ld_d64_nf(sp);
			emask = vis_edge16(sp, send) << 4;
			sp++;
			sdl = vis_ld_d64_nf(sp);

			if ((mlib_addr)sp <= (mlib_addr)send) {
				emask |= vis_edge16(sp, send);
			}

			sp++;
			MLIB_V_IMAGETHRESH1B_U16(sdh, sdl, td0, td1, hc0, lc0,
			    dc);
			dp[0] = (dc & emask) | (dp[0] & ~emask);
			dp++;

			if ((mlib_addr)sp <= (mlib_addr)send) {
				sdh = vis_ld_d64_nf(sp);
				emask = vis_edge16(sp, send) << 4;
				sp++;
				sdl = vis_ld_d64_nf(sp);

				if ((mlib_addr)sp <= (mlib_addr)send) {
					emask |= vis_edge16(sp, send);
				}

				sp++;
				MLIB_V_IMAGETHRESH1B_U16(sdh, sdl, td2, td0,
				    hc1, lc1, dc);
				dp[0] = (dc & emask) | (dp[0] & ~emask);
				dp++;

				if ((mlib_addr)sp <= (mlib_addr)send) {
					sdh = vis_ld_d64_nf(sp);
					emask = vis_edge16(sp, send) << 4;
					sp++;
					sdl = vis_ld_d64_nf(sp);

					if ((mlib_addr)sp <= (mlib_addr)send) {
						emask |= vis_edge16(sp, send);
					}

					sp++;
					MLIB_V_IMAGETHRESH1B_U16(sdh, sdl, td1,
					    td2, hc2, lc2, dc);
					dp[0] = (dc & emask) | (dp[0] & ~emask);
					dp++;
				}
			}
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_u8 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageThresh1B_U16_3(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 dbit_off,
    const mlib_s32 *th,
    mlib_s32 hc0,
    mlib_s32 lc0)
{
/* aligned src address */
	mlib_d64 *sp;

/* pointer to begin of a src line */
	mlib_s16 *sl;

/* destination address */
	mlib_u8 *dp;

/* pointer to begin of a dst line */
	mlib_u8 *dl;

/* source data */
	mlib_d64 sdh, sdl, s0, s1, s2;

/* destination data */
	mlib_u8 dc;

/* threshold data */
	mlib_d64 td0, td1, td2;

/* ghigh data */
	mlib_u8 hc1, hc2;

/* glow data */
	mlib_u8 lc1, lc2;

/* temporaries */
	mlib_s32 tt0, tt1, tt2;

/* temporaries used in macro */
	mlib_s32 cmaskh, cmaskl, cmask;
	mlib_s32 esize, dsize = 3 * xsize;

/* edge mask */
	mlib_s32 emask0, emask1, nume;

/* loop variables */
	mlib_s32 i, j;
	mlib_d64 s16min_4 = vis_to_double_dup(0x80008000);

/* prepare the threshold */
	tt0 = th[0];
	tt1 = th[1];
	tt2 = th[2];
	td0 =
	    vis_to_double((tt0 << 16) | (tt1 & 0xFFFF),
	    (tt2 << 16) | (tt0 & 0xFFFF));
	td1 =
	    vis_to_double((tt1 << 16) | (tt2 & 0xFFFF),
	    (tt0 << 16) | (tt1 & 0xFFFF));
	td2 = vis_freg_pair(vis_read_lo(td0), vis_read_hi(td1));

	td0 = vis_fpadd16(td0, s16min_4);
	td1 = vis_fpadd16(td1, s16min_4);
	td2 = vis_fpadd16(td2, s16min_4);

	hc1 = (hc0 >> 1);
	hc2 = (hc0 >> 2);
	lc1 = (lc0 >> 1);
	lc2 = (lc0 >> 2);

	sl = (void *)src;
	dl = dst;

	nume = (8 - dbit_off);

	if (nume > dsize)
		nume = dsize;
	emask0 = (((0xFF00 >> nume) & 0xFF) >> dbit_off);

	esize = 3 * (xsize & 7) - nume;

	if (esize < 0) {
		xsize -= 8;
		esize += 24;
	}

	emask1 = (0xFF << (8 - (esize & 7))) & 0xFF;

	sl -= dbit_off;

/* row loop */
	for (j = 0; j < ysize; j++) {
/* prepare source & destination address */
		sp = (mlib_d64 *)vis_alignaddr(sl, 0);
		dp = dl;

		s0 = vis_ld_d64_nf(sp);
		s1 = vis_ld_d64_nf(sp + 1);
		sdh = vis_faligndata(s0, s1);
		s2 = vis_ld_d64_nf(sp + 2);
		sdl = vis_faligndata(s1, s2);
		sp += 2;
		MLIB_V_IMAGETHRESH1B_U16(sdh, sdl, td0, td1, hc0, lc0, dc);
		dp[0] = (dc & emask0) | (dp[0] & ~emask0);
		dp++;

/* 8-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i < xsize / 8; i++) {
			s0 = sp[0];
			s1 = sp[1];
			sdh = vis_faligndata(s0, s1);
			s2 = sp[2];
			sdl = vis_faligndata(s1, s2);
			sp += 2;
			MLIB_V_IMAGETHRESH1B_U16(sdh, sdl, td2, td0, hc1, lc1,
			    dc);
			*dp = dc;
			dp++;

			s0 = sp[0];
			s1 = sp[1];
			sdh = vis_faligndata(s0, s1);
			s2 = sp[2];
			sdl = vis_faligndata(s1, s2);
			sp += 2;
			MLIB_V_IMAGETHRESH1B_U16(sdh, sdl, td1, td2, hc2, lc2,
			    dc);
			*dp = dc;
			dp++;

			s0 = sp[0];
			s1 = sp[1];
			sdh = vis_faligndata(s0, s1);
			s2 = vis_ld_d64_nf(sp + 2);
			sdl = vis_faligndata(s1, s2);
			sp += 2;
			MLIB_V_IMAGETHRESH1B_U16(sdh, sdl, td0, td1, hc0, lc0,
			    dc);
			*dp = dc;
			dp++;
		}

/* last 8-pixel column */

		if (esize) {
			s0 = vis_ld_d64_nf(sp + 1);
			sdh = vis_faligndata(s2, s0);
			s1 = vis_ld_d64_nf(sp + 2);
			sdl = vis_faligndata(s0, s1);
			sp += 2;
			MLIB_V_IMAGETHRESH1B_U16(sdh, sdl, td2, td0, hc1, lc1,
			    dc);

			if (esize >= 8) {
				*dp = dc;
				dp++;

				s0 = vis_ld_d64_nf(sp + 1);
				sdh = vis_faligndata(s1, s0);
				s1 = vis_ld_d64_nf(sp + 2);
				sdl = vis_faligndata(s0, s1);
				sp += 2;
				MLIB_V_IMAGETHRESH1B_U16(sdh, sdl, td1, td2,
				    hc2, lc2, dc);

				if (esize >= 16) {
					*dp = dc;
					dp++;

					s0 = vis_ld_d64_nf(sp + 1);
					sdh = vis_faligndata(s1, s0);
					s1 = vis_ld_d64_nf(sp + 2);
					sdl = vis_faligndata(s0, s1);
					MLIB_V_IMAGETHRESH1B_U16(sdh, sdl, td0,
					    td1, hc0, lc0, dc);
				}
			}
			if (emask1)
				dp[0] = (dc & emask1) | (dp[0] & ~emask1);
		}

		sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
		dl = (mlib_u8 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

#define	MLIB_V_IMAGETHRESH1B_S32(sd0, sd1, sd2, sd3, td0,                 \
	td1, td2, td3, hc, lc, dc)                                        \
	cmask0 = vis_fcmple32(sd0, td0);                                  \
	cmask1 = vis_fcmple32(sd1, td1);                                  \
	cmask2 = vis_fcmple32(sd2, td2);                                  \
	cmask3 = vis_fcmple32(sd3, td3);                                  \
	cmask = (cmask0 << 6) | (cmask1 << 4) | (cmask2 << 2) | cmask3;   \
	dc = (hc & (~cmask)) | (lc & (cmask))

/* *********************************************************** */

void
mlib_v_ImageThresh1B_S32_1_A8D1X8(
    const mlib_s32 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    const mlib_s32 *th,
    mlib_s32 hc,
    mlib_s32 lc)
{
/* aligned src address */
	mlib_d64 *sp;

/* destination address */
	mlib_u8 *dp;

/* source data */
	mlib_d64 sd0, sd1, sd2, sd3;

/* destination data */
	mlib_u8 dc;

/* threshold data */
	mlib_d64 td0, td1;

/* temporaries used in macro */
	mlib_s32 cmask0, cmask1, cmask;

/* temporaries used in macro */
	mlib_s32 cmask2, cmask3;

/* loop variables */
	mlib_s32 j;

/* prepare the threshold */
	td0 = vis_to_double(th[0], th[1]);
	td1 = vis_to_double(th[2], th[3]);

	sp = (mlib_d64 *)src;
	dp = (mlib_u8 *)dst;

/* 2-pixel loop */
#pragma pipeloop(0)
	for (j = 0; j < dsize / 8; j++) {
		sd0 = sp[0];
		sd1 = sp[1];
		sd2 = sp[2];
		sd3 = sp[3];
		sp += 4;
		MLIB_V_IMAGETHRESH1B_S32(sd0, sd1, sd2, sd3, td0, td1, td0, td1,
		    hc, lc, dc);
		*dp = dc;
		dp++;
	}
}

/* *********************************************************** */

void
mlib_v_ImageThresh1B_S32_1_A8D2X8(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 *th,
    mlib_s32 hc,
    mlib_s32 lc)
{
/* aligned src address */
	mlib_d64 *sp;

/* pointer to begin of a src line */
	mlib_d64 *sl;

/* destination address */
	mlib_u8 *dp;

/* pointer to begin of a dst line */
	mlib_u8 *dl;

/* source data */
	mlib_d64 sd0, sd1, sd2, sd3;

/* destination data */
	mlib_u8 dc;

/* threshold data */
	mlib_d64 td0, td1;

/* temporaries used in macro */
	mlib_s32 cmask0, cmask1, cmask;

/* temporaries used in macro */
	mlib_s32 cmask2, cmask3;

/* loop variables */
	mlib_s32 i, j;

/* prepare the threshold */
	td0 = vis_to_double(th[0], th[1]);
	td1 = vis_to_double(th[2], th[3]);

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_u8 *)dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* 2-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i < xsize / 8; i++) {
			sd0 = sp[0];
			sd1 = sp[1];
			sd2 = sp[2];
			sd3 = sp[3];
			sp += 4;
			MLIB_V_IMAGETHRESH1B_S32(sd0, sd1, sd2, sd3, td0, td1,
			    td0, td1, hc, lc, dc);
			*dp = dc;
			dp++;
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_u8 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageThresh1B_S32_1(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 dbit_off,
    const mlib_s32 *th,
    mlib_s32 hc,
    mlib_s32 lc)
{
/* aligned src address */
	mlib_d64 *sp;

/* pointer to begin of a src line */
	mlib_s32 *sl;

/* destination address */
	mlib_u8 *dp;

/* pointer to begin of a dst line */
	mlib_u8 *dl;

/* source data */
	mlib_d64 sd0, sd1, sd2, sd3;

/* source data */
	mlib_d64 s0, s1, s2, s3;

/* destination data */
	mlib_u8 dc;

/* threshold data */
	mlib_d64 td0, td1;

/* temporaries used in macro */
	mlib_s32 cmask0, cmask1, cmask;

/* temporaries used in macro */
	mlib_s32 cmask2, cmask3;

/* edge mask */
	mlib_s32 emask0, emask1, nume;

/* loop variables */
	mlib_s32 i, j;

/* prepare the threshold */
	td0 = vis_to_double(th[0], th[1]);
	td1 = vis_to_double(th[2], th[3]);

	sl = (void *)src;
	dl = dst;

	nume = (8 - dbit_off);

	if (nume > xsize)
		nume = xsize;
	emask0 = (((0xFF00 >> nume) & 0xFF) >> dbit_off);
	xsize -= nume;
	emask1 = (0xFF << (8 - (xsize & 7))) & 0xFF;

	sl -= dbit_off;

/* row loop */
	for (j = 0; j < ysize; j++) {
/* prepare source & destination address */
		sp = (mlib_d64 *)vis_alignaddr(sl, 0);
		dp = dl;

		s0 = vis_ld_d64_nf(sp);
		s1 = vis_ld_d64_nf(sp + 1);
		sd0 = vis_faligndata(s0, s1);
		s2 = vis_ld_d64_nf(sp + 2);
		sd1 = vis_faligndata(s1, s2);
		s3 = vis_ld_d64_nf(sp + 3);
		sd2 = vis_faligndata(s2, s3);
		s0 = vis_ld_d64_nf(sp + 4);
		sd3 = vis_faligndata(s3, s0);
		sp += 4;
		MLIB_V_IMAGETHRESH1B_S32(sd0, sd1, sd2, sd3, td0, td1, td0, td1,
		    hc, lc, dc);
		dp[0] = (dc & emask0) | (dp[0] & ~emask0);
		dp++;

/* 2-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i < xsize / 8; i++) {
			s1 = sp[1];
			sd0 = vis_faligndata(s0, s1);
			s2 = sp[2];
			sd1 = vis_faligndata(s1, s2);
			s3 = sp[3];
			sd2 = vis_faligndata(s2, s3);
			s0 = vis_ld_d64_nf(sp + 4);
			sd3 = vis_faligndata(s3, s0);
			sp += 4;
			MLIB_V_IMAGETHRESH1B_S32(sd0, sd1, sd2, sd3, td0, td1,
			    td0, td1, hc, lc, dc);
			*dp = dc;
			dp++;
		}

/* last 8-pixel column */

		if (emask1) {
			s1 = vis_ld_d64_nf(sp + 1);
			sd0 = vis_faligndata(s0, s1);
			s2 = vis_ld_d64_nf(sp + 2);
			sd1 = vis_faligndata(s1, s2);
			s3 = vis_ld_d64_nf(sp + 3);
			sd2 = vis_faligndata(s2, s3);
			s0 = vis_ld_d64_nf(sp + 4);
			sd3 = vis_faligndata(s3, s0);
			MLIB_V_IMAGETHRESH1B_S32(sd0, sd1, sd2, sd3, td0, td1,
			    td0, td1, hc, lc, dc);
			dp[0] = (dc & emask1) | (dp[0] & ~emask1);
		}

		sl = (mlib_s32 *)((mlib_u8 *)sl + slb);
		dl = (mlib_u8 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageThresh1B_S32_3_A8D1(
    const mlib_s32 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    const mlib_s32 *th,
    mlib_s32 hc0,
    mlib_s32 lc0)
{
/* aligned src address */
	mlib_d64 *sp;

/* destination address */
	mlib_u8 *dp;

/* source data */
	mlib_d64 sd0, sd1, sd2, sd3;

/* destination data */
	mlib_u8 dc;

/* threshold data */
	mlib_d64 td0, td1, td2;

/* ghigh data */
	mlib_u8 hc1, hc2;

/* glow data */
	mlib_u8 lc1, lc2;

/* temporaries used in macro */
	mlib_s32 cmask0, cmask1, cmask;

/* temporaries used in macro */
	mlib_s32 cmask2, cmask3;

/* loop variables */
	mlib_s32 j;

/* prepare the threshold */
	td0 = vis_to_double(th[0], th[1]);
	td1 = vis_to_double(th[2], th[0]);
	td2 = vis_to_double(th[1], th[2]);

/* prepare the ghigh value */
	hc1 = (hc0 >> 1);
	hc2 = (hc0 >> 2);
	lc1 = (lc0 >> 1);
	lc2 = (lc0 >> 2);

	sp = (mlib_d64 *)src;
	dp = (mlib_u8 *)dst;

/* 8-pixel loop */
#pragma pipeloop(0)
	for (j = 0; j < dsize / 8; j++) {
		sd0 = sp[0];
		sd1 = sp[1];
		sd2 = sp[2];
		sd3 = sp[3];
		sp += 4;
		MLIB_V_IMAGETHRESH1B_S32(sd0, sd1, sd2, sd3, td0, td1, td2, td0,
		    hc0, lc0, dc);
		*dp = dc;
		dp++;

		sd0 = sp[0];
		sd1 = sp[1];
		sd2 = sp[2];
		sd3 = sp[3];
		sp += 4;
		MLIB_V_IMAGETHRESH1B_S32(sd0, sd1, sd2, sd3, td1, td2, td0, td1,
		    hc1, lc1, dc);
		*dp = dc;
		dp++;

		sd0 = sp[0];
		sd1 = sp[1];
		sd2 = sp[2];
		sd3 = sp[3];
		sp += 4;
		MLIB_V_IMAGETHRESH1B_S32(sd0, sd1, sd2, sd3, td2, td0, td1, td2,
		    hc2, lc2, dc);
		*dp = dc;
		dp++;
	}
}

/* *********************************************************** */

void
mlib_v_ImageThresh1B_S32_3_A8D2(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_s32 *th,
    mlib_s32 hc0,
    mlib_s32 lc0)
{
/* aligned src address */
	mlib_d64 *sp;

/* pointer to begin of a src line */
	mlib_d64 *sl;

/* pointer to end of a src line */
	mlib_s32 *send;

/* destination address */
	mlib_u8 *dp;

/* pointer to begin of a src line */
	mlib_u8 *dl;

/* source data */
	mlib_d64 sd0, sd1, sd2, sd3;

/* destination data */
	mlib_u8 dc;

/* threshold data */
	mlib_d64 td0, td1, td2;

/* ghigh data */
	mlib_u8 hc1, hc2;

/* glow data */
	mlib_u8 lc1, lc2;

/* temporaries used in macro */
	mlib_s32 cmask0, cmask1, cmask;

/* temporaries used in macro */
	mlib_s32 cmask2, cmask3;

/* edge mask */
	mlib_s32 emask;

/* loop variables */
	mlib_s32 i, j;

/* prepare the threshold */
	td0 = vis_to_double(th[0], th[1]);
	td1 = vis_to_double(th[2], th[0]);
	td2 = vis_to_double(th[1], th[2]);

	hc1 = (hc0 >> 1);
	hc2 = (hc0 >> 2);
	lc1 = (lc0 >> 1);
	lc2 = (lc0 >> 2);

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_u8 *)dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

		send = (mlib_s32 *)sp + 3 * xsize - 1;

/* 8-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i < xsize / 8; i++) {
			sd0 = sp[0];
			sd1 = sp[1];
			sd2 = sp[2];
			sd3 = sp[3];
			sp += 4;
			MLIB_V_IMAGETHRESH1B_S32(sd0, sd1, sd2, sd3, td0, td1,
			    td2, td0, hc0, lc0, dc);
			*dp = dc;
			dp++;

			sd0 = sp[0];
			sd1 = sp[1];
			sd2 = sp[2];
			sd3 = sp[3];
			sp += 4;
			MLIB_V_IMAGETHRESH1B_S32(sd0, sd1, sd2, sd3, td1, td2,
			    td0, td1, hc1, lc1, dc);
			*dp = dc;
			dp++;

			sd0 = sp[0];
			sd1 = sp[1];
			sd2 = sp[2];
			sd3 = sp[3];
			sp += 4;
			MLIB_V_IMAGETHRESH1B_S32(sd0, sd1, sd2, sd3, td2, td0,
			    td1, td2, hc2, lc2, dc);
			*dp = dc;
			dp++;
		}

/* last 8-pixel column */

		if ((mlib_addr)sp <= (mlib_addr)send) {
			sd0 = vis_ld_d64_nf(sp);
			sd1 = vis_ld_d64_nf(sp + 1);
			sd2 = vis_ld_d64_nf(sp + 2);
			sd3 = vis_ld_d64_nf(sp + 3);
			emask = vis_edge32(sp, send) << 6;
			sp++;

			if ((mlib_addr)sp <= (mlib_addr)send) {
				emask |= vis_edge32(sp, send) << 4;
				sp++;

				if ((mlib_addr)sp <= (mlib_addr)send) {
					emask |= vis_edge32(sp, send) << 2;
					sp++;

					if ((mlib_addr)sp <= (mlib_addr)send) {
						emask |= vis_edge32(sp, send);
					}
				}
			}

			sp++;
			MLIB_V_IMAGETHRESH1B_S32(sd0, sd1, sd2, sd3, td0, td1,
			    td2, td0, hc0, lc0, dc);
			dp[0] = (dc & emask) | (dp[0] & ~emask);
			dp++;

			if ((mlib_addr)sp <= (mlib_addr)send) {
				sd0 = vis_ld_d64_nf(sp);
				sd1 = vis_ld_d64_nf(sp + 1);
				sd2 = vis_ld_d64_nf(sp + 2);
				sd3 = vis_ld_d64_nf(sp + 3);
				emask = vis_edge32(sp, send) << 6;
				sp++;

				if ((mlib_addr)sp <= (mlib_addr)send) {
					emask |= vis_edge32(sp, send) << 4;
					sp++;

					if ((mlib_addr)sp <= (mlib_addr)send) {
						emask |=
						    vis_edge32(sp, send) << 2;
						sp++;

						if ((mlib_addr)sp <=
						    (mlib_addr)send) {
							emask |=
							    vis_edge32(sp,
							    send);
						}
					}
				}

				sp++;
				MLIB_V_IMAGETHRESH1B_S32(sd0, sd1, sd2, sd3,
				    td1, td2, td0, td1, hc1, lc1, dc);
				dp[0] = (dc & emask) | (dp[0] & ~emask);
				dp++;

				if ((mlib_addr)sp <= (mlib_addr)send) {
					sd0 = vis_ld_d64_nf(sp);
					sd1 = vis_ld_d64_nf(sp + 1);
					sd2 = vis_ld_d64_nf(sp + 2);
					sd3 = vis_ld_d64_nf(sp + 3);
					emask = vis_edge32(sp, send) << 6;
					sp++;

					if ((mlib_addr)sp <= (mlib_addr)send) {
						emask |=
						    vis_edge32(sp, send) << 4;
						sp++;

						if ((mlib_addr)sp <=
						    (mlib_addr)send) {
							emask |=
							    vis_edge32(sp,
							    send) << 2;
						}
					}

					MLIB_V_IMAGETHRESH1B_S32(sd0, sd1, sd2,
					    sd3, td2, td0, td1, td2, hc2, lc2,
					    dc);
					dp[0] = (dc & emask) | (dp[0] & ~emask);
					dp++;
				}
			}
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_u8 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageThresh1B_S32_3(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 dbit_off,
    const mlib_s32 *th,
    mlib_s32 hc0,
    mlib_s32 lc0)
{
/* aligned src address */
	mlib_d64 *sp;

/* pointer to begin of a src line */
	mlib_s32 *sl;

/* destination address */
	mlib_u8 *dp;

/* pointer to begin of a dst line */
	mlib_u8 *dl;

/* source data */
	mlib_d64 s0, s1, s2, s3;

/* source data */
	mlib_d64 sd0, sd1, sd2, sd3;

/* destination data */
	mlib_u8 dc;

/* threshold data */
	mlib_d64 td0, td1, td2;

/* ghigh data */
	mlib_u8 hc1, hc2;

/* glow data */
	mlib_u8 lc1, lc2;
	mlib_s32 esize, dsize = 3 * xsize;

/* temporaries used in macro */
	mlib_s32 cmask0, cmask1, cmask;

/* temporaries used in macro */
	mlib_s32 cmask2, cmask3;

/* edge mask */
	mlib_s32 emask0, emask1, nume;

/* loop variables */
	mlib_s32 i, j;

/* prepare the threshold */
	td0 = vis_to_double(th[0], th[1]);
	td1 = vis_to_double(th[2], th[0]);
	td2 = vis_to_double(th[1], th[2]);

	hc1 = (hc0 >> 1);
	hc2 = (hc0 >> 2);
	lc1 = (lc0 >> 1);
	lc2 = (lc0 >> 2);

	sl = (void *)src;
	dl = dst;

	nume = (8 - dbit_off);

	if (nume > dsize)
		nume = dsize;
	emask0 = (((0xFF00 >> nume) & 0xFF) >> dbit_off);

	esize = 3 * (xsize & 7) - nume;

	if (esize < 0) {
		xsize -= 8;
		esize += 24;
	}

	emask1 = (0xFF << (8 - (esize & 7))) & 0xFF;

	sl -= dbit_off;

/* row loop */
	for (j = 0; j < ysize; j++) {
/* prepare the source & destination address */
		sp = (mlib_d64 *)vis_alignaddr(sl, 0);
		dp = dl;

		s0 = vis_ld_d64_nf(sp);
		s1 = vis_ld_d64_nf(sp + 1);
		sd0 = vis_faligndata(s0, s1);
		s2 = vis_ld_d64_nf(sp + 2);
		sd1 = vis_faligndata(s1, s2);
		s3 = vis_ld_d64_nf(sp + 3);
		sd2 = vis_faligndata(s2, s3);
		s0 = vis_ld_d64_nf(sp + 4);
		sd3 = vis_faligndata(s3, s0);
		sp += 4;
		MLIB_V_IMAGETHRESH1B_S32(sd0, sd1, sd2, sd3, td0, td1, td2, td0,
		    hc0, lc0, dc);
		dp[0] = (dc & emask0) | (dp[0] & ~emask0);
		dp++;

/* 8-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i < xsize / 8; i++) {
			s1 = sp[1];
			sd0 = vis_faligndata(s0, s1);
			s2 = sp[2];
			sd1 = vis_faligndata(s1, s2);
			s3 = sp[3];
			sd2 = vis_faligndata(s2, s3);
			s0 = sp[4];
			sd3 = vis_faligndata(s3, s0);
			sp += 4;
			MLIB_V_IMAGETHRESH1B_S32(sd0, sd1, sd2, sd3, td1, td2,
			    td0, td1, hc1, lc1, dc);
			*dp = dc;
			dp++;

			s1 = sp[1];
			sd0 = vis_faligndata(s0, s1);
			s2 = sp[2];
			sd1 = vis_faligndata(s1, s2);
			s3 = sp[3];
			sd2 = vis_faligndata(s2, s3);
			s0 = sp[4];
			sd3 = vis_faligndata(s3, s0);
			sp += 4;
			MLIB_V_IMAGETHRESH1B_S32(sd0, sd1, sd2, sd3, td2, td0,
			    td1, td2, hc2, lc2, dc);
			*dp = dc;
			dp++;

			s1 = sp[1];
			sd0 = vis_faligndata(s0, s1);
			s2 = sp[2];
			sd1 = vis_faligndata(s1, s2);
			s3 = sp[3];
			sd2 = vis_faligndata(s2, s3);
			s0 = vis_ld_d64_nf(sp + 4);
			sd3 = vis_faligndata(s3, s0);
			sp += 4;
			MLIB_V_IMAGETHRESH1B_S32(sd0, sd1, sd2, sd3, td0, td1,
			    td2, td0, hc0, lc0, dc);
			*dp = dc;
			dp++;
		}

/* last 8-pixel column */

		if (esize) {
			s1 = vis_ld_d64_nf(sp + 1);
			sd0 = vis_faligndata(s0, s1);
			s2 = vis_ld_d64_nf(sp + 2);
			sd1 = vis_faligndata(s1, s2);
			s3 = vis_ld_d64_nf(sp + 3);
			sd2 = vis_faligndata(s2, s3);
			s0 = vis_ld_d64_nf(sp + 4);
			sd3 = vis_faligndata(s3, s0);
			sp += 4;
			MLIB_V_IMAGETHRESH1B_S32(sd0, sd1, sd2, sd3, td1, td2,
			    td0, td1, hc1, lc1, dc);

			if (esize >= 8) {
				*dp = dc;
				dp++;

				s1 = vis_ld_d64_nf(sp + 1);
				sd0 = vis_faligndata(s0, s1);
				s2 = vis_ld_d64_nf(sp + 2);
				sd1 = vis_faligndata(s1, s2);
				s3 = vis_ld_d64_nf(sp + 3);
				sd2 = vis_faligndata(s2, s3);
				s0 = vis_ld_d64_nf(sp + 4);
				sd3 = vis_faligndata(s3, s0);
				sp += 4;
				MLIB_V_IMAGETHRESH1B_S32(sd0, sd1, sd2, sd3,
				    td2, td0, td1, td2, hc2, lc2, dc);

				if (esize >= 16) {
					*dp = dc;
					dp++;

					s1 = vis_ld_d64_nf(sp + 1);
					sd0 = vis_faligndata(s0, s1);
					s2 = vis_ld_d64_nf(sp + 2);
					sd1 = vis_faligndata(s1, s2);
					s3 = vis_ld_d64_nf(sp + 3);
					sd2 = vis_faligndata(s2, s3);
					s0 = vis_ld_d64_nf(sp + 4);
					sd3 = vis_faligndata(s3, s0);
					MLIB_V_IMAGETHRESH1B_S32(sd0, sd1, sd2,
					    sd3, td0, td1, td2, td0, hc0, lc0,
					    dc);
				}
			}
			if (emask1)
				dp[0] = (dc & emask1) | (dp[0] & ~emask1);
		}

		sl = (mlib_s32 *)((mlib_u8 *)sl + slb);
		dl = (mlib_u8 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */
