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

#pragma ident	"@(#)mlib_v_ImageThresh1.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageThresh1 - thresholding
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageThresh1(mlib_image       *dst,
 *                                    const mlib_image *src,
 *                                    const mlib_s32   *thresh,
 *                                    const mlib_s32   *ghigh,
 *                                    const mlib_s32   *glow);
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
 *      The images can be in MLIB_BYTE,
 *      MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
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

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_v_ImageThreshFunc.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageThresh1 = __mlib_ImageThresh1

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageThresh1) mlib_ImageThresh1
    __attribute__((weak, alias("__mlib_ImageThresh1")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	LOAD_CONST1(N, off)                                     \
	vis_alignaddr((void *)0, off * sizeof (DTYPE));         \
	c##N = vis_faligndata(cnst[0], cnst[1]);                \
	h##N = vis_faligndata(cnst[2], cnst[3]);                \
	l##N = vis_faligndata(cnst[4], cnst[5])

/* *********************************************************** */

#define	LOAD_CONST_ALGN(algn)                                    \
	mlib_d64 c0, c1, c2, h0, h1, h2, l0, l1, l2;             \
	                                                         \
	off = mlib_mod_nchan[nchan][algn];                       \
	LOAD_CONST1(0, off);                                     \
	off = mlib_mod_nchan[nchan][off + 8 / sizeof (DTYPE)];   \
	LOAD_CONST1(1, off);                                     \
	off = mlib_mod_nchan[nchan][off + 8 / sizeof (DTYPE)];   \
	LOAD_CONST1(2, off)

/* *********************************************************** */

#define	FUNC_NAME(s)	mlib_v_ImageThresh1_U8##s

#define	DTYPE	mlib_u8

#define	VIS_EDGE	vis_edge8

/* *********************************************************** */

#define	LOAD_CONST(algn)                                        \
	mlib_d64 c0a, c0b, c1a, t0, t1;                         \
	mlib_s32 mask0, mask1;                                  \
	                                                        \
	LOAD_CONST_ALGN(algn);                                  \
	c0a = vis_fexpand_hi(c0);                               \
	c0b = vis_fexpand_lo(c0);                               \
	c1a = vis_fexpand_hi(c1)

/* *********************************************************** */

#define	c1b	c0a
#define	c2a	c0b

#define	c2b	c1a

/* *********************************************************** */

#define	PROCESS_CH(N)                                           \
	t0 = vis_fexpand_hi(s##N);                              \
	t1 = vis_fexpand_lo(s##N);                              \
	mask0 = vis_fcmpgt16(t0, c##N##a);                      \
	mask1 = vis_fcmpgt16(t1, c##N##b);                      \
	cmask = (mask0 << 4) | mask1;                           \
	PST_8(dp, cmask, l##N, h##N)

#include <mlib_v_ImageConstOper.h>

/* *********************************************************** */

#define	FUNC_NAME(s)	mlib_v_ImageThresh1_S16##s
#define	FUNC_A

#define	DTYPE	mlib_s16

#define	VIS_EDGE	vis_edge16

/* *********************************************************** */

#define	LOAD_CONST(algn)	LOAD_CONST_ALGN(algn)

/* *********************************************************** */

#define	PROCESS_CH(N)                                           \
	cmask = vis_fcmpgt16(s##N, c##N);                       \
	PST_16(dp, cmask, l##N, h##N)

#include <mlib_v_ImageConstOper.h>

/* *********************************************************** */

#define	FUNC_NAME(s)	mlib_v_ImageThresh1_U16##s
#define	FUNC_A

#define	DTYPE	mlib_u16

#define	VIS_EDGE	vis_edge16

/* *********************************************************** */

#define	LOAD_CONST(algn)                                        \
	mlib_d64 mask8000 = vis_to_double_dup(0x80008000);      \
	mlib_d64 ss;                                            \
	                                                        \
	LOAD_CONST_ALGN(algn);                                  \
	c0 = vis_fxor(c0, mask8000);                            \
	c1 = vis_fxor(c1, mask8000);                            \
	c2 = vis_fxor(c2, mask8000)

/* *********************************************************** */

#define	PROCESS_CH(N)                                           \
	ss = vis_fxor(s##N, mask8000);                          \
	cmask = vis_fcmpgt16(ss, c##N);                         \
	PST_16(dp, cmask, l##N, h##N)

#include <mlib_v_ImageConstOper.h>

/* *********************************************************** */

#define	FUNC_NAME(s)	mlib_v_ImageThresh1_S32##s
#define	FUNC_A

#define	FUNC_2

#define	DTYPE	mlib_s32

#define	VIS_EDGE	vis_edge32

/* *********************************************************** */

#undef  LOAD_CONST1
#define	LOAD_CONST1(N, off)                                      \
	vis_alignaddr((void *)0, off * sizeof (DTYPE));          \
	off1 = off >> 1;                                         \
	c##N = vis_faligndata(cnst[off1], cnst[off1 ^ 1]);       \
	h##N = vis_faligndata(cnst[off1 + 2], cnst[off1 ^ 3]);   \
	l##N = vis_faligndata(cnst[off1 + 4], cnst[off1 ^ 5])

/* *********************************************************** */

#define	LOAD_CONST(algn)	mlib_s32 off1; LOAD_CONST_ALGN(algn)

/* *********************************************************** */

#define	PROCESS_CH(N)                                           \
	cmask = vis_fcmpgt32(s##N, c##N);                       \
	PST_32(dp, cmask, l##N, h##N)

#include <mlib_v_ImageConstOper.h>

/* *********************************************************** */

mlib_status
__mlib_ImageThresh1(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *thresh,
    const mlib_s32 *ghigh,
    const mlib_s32 *glow)
{
	mlib_s32 nchan, width, height, sstride, dstride;
	void *sdata, *ddata;
	mlib_type type;
	mlib_d64 cnst[6];
	mlib_s32 i, t_sh, algn;

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_SIZE_EQUAL(src, dst);
	MLIB_IMAGE_CHAN_EQUAL(src, dst);
	MLIB_IMAGE_TYPE_DSTBIT_OR_EQ(src, dst);

	if (thresh == NULL)
		return (MLIB_NULLPOINTER);
	if (ghigh == NULL)
		return (MLIB_NULLPOINTER);
	if (glow == NULL)
		return (MLIB_NULLPOINTER);

	MLIB_IMAGE_GET_ALL_PARAMS(dst, type, nchan, width, height, dstride,
	    ddata);
	sstride = mlib_ImageGetStride(src);
	sdata = mlib_ImageGetData(src);

	if (type == MLIB_BIT) {
		return (mlib_ImageThresh1B(dst, src, thresh, ghigh, glow));

	} else if (type == MLIB_BYTE) {
		mlib_u8 *pcol1 = (void *)cnst;
		mlib_u8 *pcol2 = (void *)(cnst + 2);
		mlib_u8 *pcol3 = (void *)(cnst + 4);

		t_sh = 0;

		for (i = 0; i < nchan; i++) {
			pcol1[i] = CLAMP_U8(thresh[i]);
			pcol2[i] = ghigh[i];
			pcol3[i] = glow[i];

			if (thresh[i] < MLIB_U8_MIN)
				pcol3[i] = pcol2[i];
		}

		for (i = nchan; i < 16; i++) {
			pcol1[i] = pcol1[i - nchan];
			pcol2[i] = pcol2[i - nchan];
			pcol3[i] = pcol3[i - nchan];
		}

	} else if (type == MLIB_SHORT) {
		mlib_s16 *pcol1 = (void *)cnst;
		mlib_s16 *pcol2 = (void *)(cnst + 2);
		mlib_s16 *pcol3 = (void *)(cnst + 4);

		t_sh = 1;

		for (i = 0; i < nchan; i++) {
			pcol1[i] = CLAMP_S16(thresh[i]);
			pcol2[i] = ghigh[i];
			pcol3[i] = glow[i];

			if (thresh[i] < MLIB_S16_MIN)
				pcol3[i] = pcol2[i];
		}

		for (i = nchan; i < 8; i++) {
			pcol1[i] = pcol1[i - nchan];
			pcol2[i] = pcol2[i - nchan];
			pcol3[i] = pcol3[i - nchan];
		}

	} else if (type == MLIB_USHORT) {
		mlib_s16 *pcol1 = (void *)cnst;
		mlib_s16 *pcol2 = (void *)(cnst + 2);
		mlib_s16 *pcol3 = (void *)(cnst + 4);

		t_sh = 1;

		for (i = 0; i < nchan; i++) {
			pcol1[i] = CLAMP_U16(thresh[i]);
			pcol2[i] = ghigh[i];
			pcol3[i] = glow[i];

			if (thresh[i] < MLIB_U16_MIN)
				pcol3[i] = pcol2[i];
		}

		for (i = nchan; i < 8; i++) {
			pcol1[i] = pcol1[i - nchan];
			pcol2[i] = pcol2[i - nchan];
			pcol3[i] = pcol3[i - nchan];
		}

	} else {
		mlib_s32 *pcol1 = (void *)cnst;
		mlib_s32 *pcol2 = (void *)(cnst + 2);
		mlib_s32 *pcol3 = (void *)(cnst + 4);

		t_sh = 2;

		for (i = 0; i < nchan; i++) {
			pcol1[i] = thresh[i];
			pcol2[i] = ghigh[i];
			pcol3[i] = glow[i];
		}

		for (i = nchan; i < 4; i++) {
			pcol1[i] = pcol1[i - nchan];
			pcol2[i] = pcol2[i - nchan];
			pcol3[i] = pcol3[i - nchan];
		}
	}

	if (sstride == dstride && sstride == ((nchan * width) << t_sh)) {
		width *= height;
		height = 1;
	}

	algn = (int)sdata | (int)ddata;

	if (height > 1)
		algn |= (sstride | dstride);
	algn &= 7;

	if (nchan == 3)
		algn = 1;

	sstride >>= t_sh;
	dstride >>= t_sh;

	switch (type) {
	case MLIB_BYTE:
		return mlib_v_ImageThresh1_U8_3(sdata, ddata, sstride, dstride,
		    width, height, nchan, cnst);

	case MLIB_SHORT:

		if (algn == 0) {
			return mlib_v_ImageThresh1_S16_A(sdata, ddata, sstride,
			    dstride, width, height, nchan, cnst);
		} else {
			return mlib_v_ImageThresh1_S16_3(sdata, ddata, sstride,
			    dstride, width, height, nchan, cnst);
		}

	case MLIB_USHORT:

		if (algn == 0) {
			return mlib_v_ImageThresh1_U16_A(sdata, ddata, sstride,
			    dstride, width, height, nchan, cnst);
		} else {
			return mlib_v_ImageThresh1_U16_3(sdata, ddata, sstride,
			    dstride, width, height, nchan, cnst);
		}

	case MLIB_INT:

		if (nchan == 4) {
			return mlib_v_ImageThresh1_S32_2(sdata, ddata, sstride,
			    dstride, width, height, nchan, cnst);
		}

		if (algn == 0) {
			return mlib_v_ImageThresh1_S32_A(sdata, ddata, sstride,
			    dstride, width, height, nchan, cnst);
		} else {
			return mlib_v_ImageThresh1_S32_3(sdata, ddata, sstride,
			    dstride, width, height, nchan, cnst);
		}

	default:
		return (MLIB_FAILURE);
	}
}

/* *********************************************************** */
