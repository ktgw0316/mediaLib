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

#pragma ident	"@(#)mlib_v_ImageReplaceColor.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageReplaceColor - replace a color in the image with
 *                               another color.
 *      mlib_ImageReplaceColor_Inp - replace a color in the image with
 *                                   another color.
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_ImageReplaceColor(mlib_image       *dst,
 *                                         const mlib_image *src,
 *                                         const mlib_s32   *color1,
 *                                         const mlib_s32   *color2);
 *      mlib_status mlib_ImageReplaceColor_Inp(mlib_image     *srcdst,
 *                                             const mlib_s32 *color1,
 *                                             const mlib_s32 *color2);
 *
 * ARGUMENT
 *      dst     pointer to output image
 *      src     pointer to input image
 *      srcdst  pointer to input and output image
 *      color1  array of color to find in the source image
 *      color2  array of color to replace color1 with
 *
 * RESTRICTION
 *      src and dst must be of the same type, the same size and
 *      the same number of channels. They can have one through four
 *      channels. They can be in MLIB_BYTE, MLIB_SHORT, MLIB_USHORT
 *      or MLIB_INT data type. The length of color array must
 *      be not less than the number of channels in the images.
 *
 *      srcdst can has one through four channels. It can be in
 *      MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 *      The length of color array must be not less than the number
 *      of channels in the image.
 *
 * DESCRIPTION
 *      Function scans the source image for all pixels with color value
 *      equal to color1 and replaces these pixels with color2.
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImageCheck.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageReplaceColor = __mlib_ImageReplaceColor
#pragma weak mlib_ImageReplaceColor_Inp = __mlib_ImageReplaceColor_Inp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageReplaceColor) mlib_ImageReplaceColor
    __attribute__((weak, alias("__mlib_ImageReplaceColor")));
__typeof__(__mlib_ImageReplaceColor_Inp) mlib_ImageReplaceColor_Inp
    __attribute__((weak, alias("__mlib_ImageReplaceColor_Inp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static const mlib_u8 mlib_mod_nchan1[] =
{ 0, 0, 0, 0, 0, 0, 0, 0, /* z */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
static const mlib_u8 mlib_mod_nchan2[] =
{ 0, 1, 0, 1, 0, 1, 0, 1, /* z */ 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1 };
static const mlib_u8 mlib_mod_nchan3[] =
{ 1, 2, 0, 1, 2, 0, 1, 2, /* z */ 0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 2 };
static const mlib_u8 mlib_mod_nchan4[] =
{ 0, 1, 2, 3, 0, 1, 2, 3, /* z */ 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3 };

const mlib_u8 *mlib_mod_nchan[5] = {
	NULL,
	mlib_mod_nchan1 + 8,
	mlib_mod_nchan2 + 8,
	mlib_mod_nchan3 + 8,
	mlib_mod_nchan4 + 8
};

/* *********************************************************** */

#if MLIB_VIS >= 0x200

const mlib_u32 mlib_bmask_cmp16[16] = {
	0x01234567, 0x012345ef, 0x0123cd67, 0x0123cdef,
	0x01ab4567, 0x01ab45ef, 0x01abcd67, 0x01abcdef,
	0x89234567, 0x892345ef, 0x8923cd67, 0x8923cdef,
	0x89ab4567, 0x89ab45ef, 0x89abcd67, 0x89abcdef
};

const mlib_u32 mlib_bmask_cmp32[4] = {
	0x01234567, 0x0123cdef, 0x89ab4567, 0x89abcdef
};

#endif /* MLIB_VIS >= 0x200 */

/* *********************************************************** */

#define	VIS_FCMP_16	vis_fcmpeq16
#define	VIS_FCMP_32	vis_fcmpeq32

/* *********************************************************** */

#define	LOAD_CONST1(N, off)                                     \
	vis_alignaddr((void *)0, off * sizeof (DTYPE));         \
	c##N = vis_faligndata(cnst[0], cnst[1]);                \
	r##N = vis_faligndata(cnst[2], cnst[3])

/* *********************************************************** */

#define	LOAD_CONST_ALGN(algn)                                    \
	mlib_d64 c0, c1, c2, r0, r1, r2;                         \
	                                                         \
	off = mlib_mod_nchan[nchan][algn];                       \
	LOAD_CONST1(0, off);                                     \
	off = mlib_mod_nchan[nchan][off + 8 / sizeof (DTYPE)];   \
	LOAD_CONST1(1, off);                                     \
	off = mlib_mod_nchan[nchan][off + 8 / sizeof (DTYPE)];   \
	LOAD_CONST1(2, off)

/* *********************************************************** */

#define	FUNC_NAME(s)	mlib_v_ImageReplaceColor_U8##s

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
	mask0 = VIS_FCMP_16(t0, c##N##a);                       \
	mask1 = VIS_FCMP_16(t1, c##N##b);                       \
	cmask = (mask0 << 4) | mask1;                           \
	PST_8(dp, cmask, s##N, r##N)

#include <mlib_v_ImageConstOper.h>

/* *********************************************************** */

#define	FUNC_NAME(s)	mlib_v_ImageReplaceColor_S16##s
#define	FUNC_A

#define	DTYPE	mlib_s16

#define	VIS_EDGE	vis_edge16

/* *********************************************************** */

#define	LOAD_CONST(algn)	LOAD_CONST_ALGN(algn)

/* *********************************************************** */

#define	PROCESS_CH(N)                                           \
	cmask = VIS_FCMP_16(s##N, c##N);                        \
	PST_16(dp, cmask, s##N, r##N)

#include <mlib_v_ImageConstOper.h>

/* *********************************************************** */

#define	FUNC_NAME(s)	mlib_v_ImageReplaceColor_S32##s
#define	FUNC_A

#define	FUNC_2

#define	DTYPE	mlib_s32

#define	VIS_EDGE	vis_edge32

/* *********************************************************** */

#undef  LOAD_CONST1
#define	LOAD_CONST1(N, off)                                     \
	vis_alignaddr((void *)0, off * sizeof (DTYPE));         \
	off1 = off >> 1;                                        \
	c##N = vis_faligndata(cnst[off1], cnst[off1 ^ 1]);      \
	r##N = vis_faligndata(cnst[off1 + 2], cnst[off1 ^ 3])

/* *********************************************************** */

#define	LOAD_CONST(algn)	mlib_s32 off1; LOAD_CONST_ALGN(algn)

/* *********************************************************** */

#define	PROCESS_CH(N)                                           \
	cmask = VIS_FCMP_32(s##N, c##N);                        \
	PST_32(dp, cmask, s##N, r##N)

#include <mlib_v_ImageConstOper.h>

/* *********************************************************** */

mlib_status
__mlib_ImageReplaceColor(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *color1,
    const mlib_s32 *color2)
{
	mlib_s32 nchan, width, height, sstride, dstride;
	void *sdata, *ddata;
	mlib_type type;
	mlib_d64 cnst[4];
	mlib_s32 i, t_sh, algn;

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_FULL_EQUAL(dst, src);

	if (color1 == NULL)
		return (MLIB_NULLPOINTER);

	if (color2 == NULL)
		return (MLIB_NULLPOINTER);

	MLIB_IMAGE_GET_ALL_PARAMS(dst, type, nchan, width, height, dstride,
	    ddata);
	sstride = mlib_ImageGetStride(src);
	sdata = mlib_ImageGetData(src);

	if (type == MLIB_BYTE) {
		mlib_u8 *pcol1 = (void *)cnst;
		mlib_u8 *pcol2 = (void *)(cnst + 2);

		t_sh = 0;

		for (i = 0; i < nchan; i++) {
			pcol1[i] = CLAMP_U8(color1[i]);
			pcol2[i] = color2[i];

			if (pcol1[i] != color1[i])
				pcol2[i] = pcol1[i];
		}

		for (i = nchan; i < 16; i++) {
			pcol1[i] = pcol1[i - nchan];
			pcol2[i] = pcol2[i - nchan];
		}
	} else if (type == MLIB_SHORT) {
		mlib_s16 *pcol1 = (void *)cnst;
		mlib_s16 *pcol2 = (void *)(cnst + 2);

		t_sh = 1;

		for (i = 0; i < nchan; i++) {
			pcol1[i] = CLAMP_S16(color1[i]);
			pcol2[i] = color2[i];

			if (pcol1[i] != color1[i])
				pcol2[i] = pcol1[i];
		}

		for (i = nchan; i < 8; i++) {
			pcol1[i] = pcol1[i - nchan];
			pcol2[i] = pcol2[i - nchan];
		}
	} else if (type == MLIB_USHORT) {
		mlib_u16 *pcol1 = (void *)cnst;
		mlib_u16 *pcol2 = (void *)(cnst + 2);

		t_sh = 1;

		for (i = 0; i < nchan; i++) {
			pcol1[i] = CLAMP_U16(color1[i]);
			pcol2[i] = color2[i];

			if (pcol1[i] != color1[i])
				pcol2[i] = pcol1[i];
		}

		for (i = nchan; i < 8; i++) {
			pcol1[i] = pcol1[i - nchan];
			pcol2[i] = pcol2[i - nchan];
		}
	} else {
		mlib_s32 *pcol1 = (void *)cnst;
		mlib_s32 *pcol2 = (void *)(cnst + 2);

		t_sh = 2;

		for (i = 0; i < nchan; i++) {
			pcol1[i] = color1[i];
			pcol2[i] = color2[i];
		}

		for (i = nchan; i < 4; i++) {
			pcol1[i] = pcol1[i - nchan];
			pcol2[i] = pcol2[i - nchan];
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
		return mlib_v_ImageReplaceColor_U8_3(sdata, ddata, sstride,
		    dstride, width, height, nchan, cnst);

	case MLIB_SHORT:
	case MLIB_USHORT:

		if (algn == 0) {
			return mlib_v_ImageReplaceColor_S16_A(sdata, ddata,
			    sstride, dstride, width, height, nchan, cnst);
		} else {
			return mlib_v_ImageReplaceColor_S16_3(sdata, ddata,
			    sstride, dstride, width, height, nchan, cnst);
		}

	case MLIB_INT:

		if (nchan == 4) {
			return mlib_v_ImageReplaceColor_S32_2(sdata, ddata,
			    sstride, dstride, width, height, nchan, cnst);
		}

		if (algn == 0) {
			return mlib_v_ImageReplaceColor_S32_A(sdata, ddata,
			    sstride, dstride, width, height, nchan, cnst);
		} else {
			return mlib_v_ImageReplaceColor_S32_3(sdata, ddata,
			    sstride, dstride, width, height, nchan, cnst);
		}

	default:
		return (MLIB_FAILURE);
	}
}

/* *********************************************************** */

mlib_status
__mlib_ImageReplaceColor_Inp(
    mlib_image *img,
    const mlib_s32 *color1,
    const mlib_s32 *color2)
{
	return (__mlib_ImageReplaceColor(img, img, color1, color2));
}

/* *********************************************************** */
