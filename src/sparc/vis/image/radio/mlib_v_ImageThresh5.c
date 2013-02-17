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

#pragma ident	"@(#)mlib_v_ImageThresh5.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageThresh5 - thresholding
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageThresh5(mlib_image       *dst,
 *                                    const mlib_image *src,
 *                                    const mlib_s32   *thigh,
 *                                    const mlib_s32   *tlow,
 *                                    const mlib_s32   *gmid);
 *
 * ARGUMENT
 *      dst     pointer to output image
 *      src     pointer to input image
 *      thigh   array of high thresholds
 *      tlow    array of low thresholds
 *      gmid    array of values between thresholds
 *
 * RESTRICTION
 *      The images must have the same type, the same size, and the same number
 *      of channels.
 *      The images can have 1, 2, 3, or 4 channels.
 *      The images can be in MLIB_BYTE, MLIB_SHORT or MLIB_INT data type.
 *
 * DESCRIPTION
 *      If the pixel channel value is below the thigh for that channel
 *      and the pixel channel value is above the tlow for that channel,
 *      set the destination to the gmid value for that channel.
 *
 *                      +- src[x][y][c] src[x][y][c] < tlow[c]
 *                      |
 *      dst[x][y][c]  = +- gmid[c]      tlow[c] <= src[x][y][c] <= thigh[c]
 *                      |
 *                      +- src[x][y][c] src[x][y][c] > thigh[c]
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImageCheck.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageThresh5 = __mlib_ImageThresh5

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageThresh5) mlib_ImageThresh5
    __attribute__((weak, alias("__mlib_ImageThresh5")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	LOAD_CONST1(N, off)                                     \
	vis_alignaddr((void *)0, off * sizeof (DTYPE));         \
	y##N = vis_faligndata(cnst[0], cnst[1]);                \
	x##N = vis_faligndata(cnst[2], cnst[3]);                \
	m##N = vis_faligndata(cnst[4], cnst[5])

/* *********************************************************** */

#define	LOAD_CONST_ALGN(algn)                                    \
	mlib_d64 y0, y1, y2, x0, x1, x2, m0, m1, m2;             \
	                                                         \
	off = mlib_mod_nchan[nchan][algn];                       \
	LOAD_CONST1(0, off);                                     \
	off = mlib_mod_nchan[nchan][off + 8 / sizeof (DTYPE)];   \
	LOAD_CONST1(1, off);                                     \
	off = mlib_mod_nchan[nchan][off + 8 / sizeof (DTYPE)];   \
	LOAD_CONST1(2, off)

/* *********************************************************** */

#define	FUNC_NAME(s)	mlib_v_ImageThresh5_U8##s
#define	FUNC_A

#define	DTYPE	mlib_u8

#define	VIS_EDGE	vis_edge8

/* *********************************************************** */

#define	LOAD_CONST(algn)                                        \
	mlib_d64 mask8000 = vis_to_double_dup(0x80008000);      \
	mlib_d64 x0a, x0b, x1a, t0, t1;                         \
	mlib_d64 y0a, y0b, y1a;                                 \
	mlib_s32 mask0, mask1;                                  \
	                                                        \
	LOAD_CONST_ALGN(algn);                                  \
	x0a = vis_fexpand_hi(x0);                               \
	x0b = vis_fexpand_lo(x0);                               \
	x1a = vis_fexpand_hi(x1);                               \
	y0a = vis_fexpand_hi(y0);                               \
	y0b = vis_fexpand_lo(y0);                               \
	y1a = vis_fexpand_hi(y1);                               \
	x0a = vis_fxor(x0a, mask8000);                          \
	x0b = vis_fxor(x0b, mask8000);                          \
	x1a = vis_fxor(x1a, mask8000);                          \
	y0a = vis_fpsub16(y0a, x0a);                            \
	y0b = vis_fpsub16(y0b, x0b);                            \
	y1a = vis_fpsub16(y1a, x1a)

/* *********************************************************** */

#define	y1b	y0a
#define	y2a	y0b

#define	y2b	y1a

#define	x1b	x0a

#define	x2a	x0b

#define	x2b	x1a

/* *********************************************************** */

#define	PROCESS_CH(N)                                           \
	t0 = vis_fexpand_hi(s##N);                              \
	t1 = vis_fexpand_lo(s##N);                              \
	t0 = vis_fpsub16(t0, x##N##a);                          \
	t1 = vis_fpsub16(t1, x##N##b);                          \
	mask0 = vis_fcmple16(t0, y##N##a);                      \
	mask1 = vis_fcmple16(t1, y##N##b);                      \
	cmask = (mask0 << 4) | mask1;                           \
	PST_8(dp, cmask, s##N, m##N)

#include <mlib_v_ImageConstOper.h>

/* *********************************************************** */

#define	FUNC_NAME(s)	mlib_v_ImageThresh5_S16##s
#define	FUNC_A

#define	DTYPE	mlib_s16

#define	VIS_EDGE	vis_edge16

/* *********************************************************** */

#define	LOAD_CONST(algn)                                        \
	mlib_d64 mask8000 = vis_to_double_dup(0x80008000);      \
	mlib_d64 ss;                                            \
	                                                        \
	LOAD_CONST_ALGN(algn);                                  \
	x0 = vis_fxor(x0, mask8000);                            \
	x1 = vis_fxor(x1, mask8000);                            \
	x2 = vis_fxor(x2, mask8000);                            \
	y0 = vis_fpsub16(y0, x0);                               \
	y1 = vis_fpsub16(y1, x1);                               \
	y2 = vis_fpsub16(y2, x2)

/* *********************************************************** */

#define	PROCESS_CH(N)                                           \
	ss = vis_fpsub16(s##N, x##N);                           \
	cmask = vis_fcmple16(ss, y##N);                         \
	PST_16(dp, cmask, s##N, m##N)

#include <mlib_v_ImageConstOper.h>

/* *********************************************************** */

#define	FUNC_NAME(s)	mlib_v_ImageThresh5_S32##s
#define	FUNC_A

#define	FUNC_2

#define	DTYPE	mlib_s32

#define	VIS_EDGE	vis_edge32

/* *********************************************************** */

#undef  LOAD_CONST1
#define	LOAD_CONST1(N, off)                                      \
	vis_alignaddr((void *)0, off * sizeof (DTYPE));          \
	off1 = off >> 1;                                         \
	y##N = vis_faligndata(cnst[off1], cnst[off1 ^ 1]);       \
	x##N = vis_faligndata(cnst[off1 + 2], cnst[off1 ^ 3]);   \
	m##N = vis_faligndata(cnst[off1 + 4], cnst[off1 ^ 5]);   \
	x##N = vis_fxor(x##N, mask80000000);                     \
	y##N = vis_fpsub32(y##N, x##N)

/* *********************************************************** */

#define	LOAD_CONST(algn)                                         \
	mlib_d64 mask80000000 = vis_to_double_dup(0x80000000);   \
	mlib_d64 ss;                                             \
	mlib_s32 off1;                                           \
	                                                         \
	LOAD_CONST_ALGN(algn)

/* *********************************************************** */

#define	PROCESS_CH(N)                                           \
	ss = vis_fpsub32(s##N, x##N);                           \
	cmask = vis_fcmple32(ss, y##N);                         \
	PST_32(dp, cmask, s##N, m##N)

#include <mlib_v_ImageConstOper.h>

/* *********************************************************** */

mlib_status
__mlib_ImageThresh5(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *thigh,
    const mlib_s32 *tlow,
    const mlib_s32 *gmid)
{
	mlib_s32 nchan, width, height, sstride, dstride;
	void *sdata, *ddata;
	mlib_type type;
	mlib_d64 cnst[8];
	mlib_s32 i, t_sh, algn;

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_SIZE_EQUAL(src, dst);
	MLIB_IMAGE_CHAN_EQUAL(src, dst);
	MLIB_IMAGE_TYPE_DSTBIT_OR_EQ(src, dst);

	if (thigh == NULL)
		return (MLIB_NULLPOINTER);
	if (tlow == NULL)
		return (MLIB_NULLPOINTER);
	if (gmid == NULL)
		return (MLIB_NULLPOINTER);

	MLIB_IMAGE_GET_ALL_PARAMS(dst, type, nchan, width, height, dstride,
	    ddata);
	sstride = mlib_ImageGetStride(src);
	sdata = mlib_ImageGetData(src);

	for (i = 0; i < nchan; i++) {
		if (thigh[i] < tlow[i])
			return (MLIB_FAILURE);
	}

	if (type == MLIB_BYTE) {
		mlib_u8 *pcol1 = (void *)cnst;
		mlib_u8 *pcol2 = (void *)(cnst + 2);
		mlib_u8 *pcol3 = (void *)(cnst + 4);

		t_sh = 0;

		for (i = 0; i < nchan; i++) {
			pcol1[i] = CLAMP_U8(thigh[i]);
			pcol2[i] = CLAMP_U8(tlow[i]);
			pcol3[i] = gmid[i];

			if (thigh[i] < MLIB_U8_MIN || tlow[i] > MLIB_U8_MAX) {
				pcol1[i] = pcol2[i] = pcol3[i] = 0;
			}
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
			pcol1[i] = CLAMP_S16(thigh[i]);
			pcol2[i] = CLAMP_S16(tlow[i]);
			pcol3[i] = gmid[i];

			if (thigh[i] < MLIB_S16_MIN || tlow[i] > MLIB_S16_MAX) {
				pcol1[i] = pcol2[i] = pcol3[i] = 0;
			}
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
			pcol1[i] = CLAMP_U16(thigh[i]);
			pcol2[i] = CLAMP_U16(tlow[i]);
			pcol3[i] = gmid[i];

			if (thigh[i] < MLIB_U16_MIN || tlow[i] > MLIB_U16_MAX) {
				pcol1[i] = pcol2[i] = pcol3[i] = 0;
			}
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
			pcol1[i] = thigh[i];
			pcol2[i] = tlow[i];
			pcol3[i] = gmid[i];
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

	algn = (mlib_s32)sdata | (mlib_s32)ddata;

	if (height > 1)
		algn |= (sstride | dstride);
	algn &= 7;

	if (nchan == 3)
		algn = 1;

	sstride >>= t_sh;
	dstride >>= t_sh;

	switch (type) {
	case MLIB_BYTE:

		if (algn == 0) {
			return mlib_v_ImageThresh5_U8_A(sdata, ddata, sstride,
			    dstride, width, height, nchan, cnst);
		} else {
			return mlib_v_ImageThresh5_U8_3(sdata, ddata, sstride,
			    dstride, width, height, nchan, cnst);
		}

	case MLIB_SHORT:
	case MLIB_USHORT:

		if (algn == 0) {
			return mlib_v_ImageThresh5_S16_A(sdata, ddata, sstride,
			    dstride, width, height, nchan, cnst);
		} else {
			return mlib_v_ImageThresh5_S16_3(sdata, ddata, sstride,
			    dstride, width, height, nchan, cnst);
		}

	case MLIB_INT:

		if (nchan == 4) {
			return mlib_v_ImageThresh5_S32_2(sdata, ddata, sstride,
			    dstride, width, height, nchan, cnst);
		}

		if (algn == 0) {
			return mlib_v_ImageThresh5_S32_A(sdata, ddata, sstride,
			    dstride, width, height, nchan, cnst);
		} else {
			return mlib_v_ImageThresh5_S32_3(sdata, ddata, sstride,
			    dstride, width, height, nchan, cnst);
		}

	default:
		return (MLIB_FAILURE);
	}
}

/* *********************************************************** */
