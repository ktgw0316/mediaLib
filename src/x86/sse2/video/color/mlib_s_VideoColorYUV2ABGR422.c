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
 * Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_s_VideoColorYUV2ABGR422.c	9.4	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorYUV2ABGR422 - color conversion from YUV422 to ABGR
 *
 * SYNOPSIS
 *
 *     mlib_status mlib_VideoColorYUV2ABGR422(mlib_u8       *abgr,
 *                                            const mlib_u8 *y,
 *                                            const mlib_u8 *u,
 *                                            const mlib_u8 *v,
 *                                            mlib_s32      width,
 *                                            mlib_s32      height,
 *                                            mlib_s32      abgr_stride,
 *                                            mlib_s32      y_stride,
 *                                            mlib_s32      uv_stride)
 *
 * ARGUMENT
 *  abgr        Pointer to the output ABGR image
 *  y           Pointer to input Y component
 *  u           Pointer to input U component
 *  v           Pointer to input V component
 *  width       Width of the image. Note that width must be a multiple of 2
 *  height      Height of the image
 *  abgr_stride Stride in bytes between adjacent rows in output image
 *  y_stride    Stride in bytes between adjacent rows in Y input image
 *  uv_stride   Stride in bytes between adjacent rows in U and V input
 *              image
 *
 * DESCRIPTION
 *      mlib_ImageColorConvert2()
 *        |R|   |1.1644   0.0000   1.5966|   [|Y|   |16.0000|]
 *        |G| = |1.1644  -0.3920  -0.8132| * [|U| - |128.0000|]
 *        |B|   |1.1644   2.0184   0.0000|   [|V|   |128.0000|]
 */

#include <mlib_video.h>
#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorYUV2ABGR422 = __mlib_VideoColorYUV2ABGR422

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorYUV2ABGR422) mlib_VideoColorYUV2ABGR422
	__attribute__((weak, alias("__mlib_VideoColorYUV2ABGR422")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */
/* not write alpha data */
#define	SAVE_ABGR1(dbgr)				\
{							\
	temp = _mm_load_si128((__m128i*)pdd);	\
	temp = _mm_and_si128(temp, x_mask);		\
	temp = _mm_or_si128(temp, dbgr);		\
	_mm_store_si128((__m128i*)pdd, temp);		\
	pdd += 16;					\
}

#define	SAVE_ABGR2(dbgr)				\
{							\
	temp = _mm_loadu_si128((__m128i*)pdd);	\
	temp = _mm_and_si128(temp, x_mask);		\
	temp = _mm_or_si128(temp, dbgr);		\
	_mm_storeu_si128((__m128i*)pdd, temp);	\
	pdd += 16;					\
}

#define	CLAMP_U8(f, c)		\
	{	\
		if ((f) > 255)	\
		{	\
			c = 255;	\
		}	\
		else if ((f) < 0)	\
		{	\
			c = 0;	\
		}	\
		else	\
			c = (f);	\
	}

/* *********************************************************** */
mlib_status
mlib_VideoColorYUV2ABGR422_aligned(
	mlib_u8 *abgr,
	const mlib_u8 *y,
	const mlib_u8 *u,
	const mlib_u8 *v,
	mlib_s32 width,
	mlib_s32 height,
	mlib_s32 abgr_stride,
	mlib_s32 y_stride,
	mlib_s32 uv_stride);

mlib_status
mlib_VideoColorYUV2ABGR422_naligned(
	mlib_u8 *abgr,
	const mlib_u8 *y,
	const mlib_u8 *u,
	const mlib_u8 *v,
	mlib_s32 width,
	mlib_s32 height,
	mlib_s32 abgr_stride,
	mlib_s32 y_stride,
	mlib_s32 uv_stride);

mlib_status
__mlib_VideoColorYUV2ABGR422(
	mlib_u8 *abgr,
	const mlib_u8 *y,
	const mlib_u8 *u,
	const mlib_u8 *v,
	mlib_s32 width,
	mlib_s32 height,
	mlib_s32 abgr_stride,
	mlib_s32 y_stride,
	mlib_s32 uv_stride)
{
	if (abgr == NULL || y == NULL || u == NULL || v == NULL)
		return (MLIB_NULLPOINTER);

	if (width <= 0 || height <= 0)
		return (MLIB_FAILURE);

	if ((y_stride == width) && (uv_stride == width / 2) &&
			(abgr_stride == 3 * width)) {
		width *= height;
		height = 1;
	}

	if ((width & 1) != 0)
	    return (MLIB_FAILURE);

	if (((((mlib_addr)y | (mlib_addr)u | (mlib_addr)v |
		(mlib_addr)abgr) & 0xf) == 0) &&
		((((y_stride | uv_stride | abgr_stride) & 0xf) == 0) ||
		(height == 1))) {
		return mlib_VideoColorYUV2ABGR422_aligned(abgr, y, u, v,
			width, height, abgr_stride, y_stride, uv_stride);
	} else {
		return mlib_VideoColorYUV2ABGR422_naligned(abgr, y, u, v,
			width, height, abgr_stride, y_stride, uv_stride);
	}
}

mlib_status
mlib_VideoColorYUV2ABGR422_aligned(
	mlib_u8 *abgr,
	const mlib_u8 *y,
	const mlib_u8 *u,
	const mlib_u8 *v,
	mlib_s32 width,
	mlib_s32 height,
	mlib_s32 abgr_stride,
	mlib_s32 y_stride,
	mlib_s32 uv_stride)
{
/* 1.1644  * 8192 */
	const __m128i c0 = _mm_set1_epi16(0x2543);
	const mlib_s32 ic0 = 0x2543;

/* 2.0184  * 8192 */
	const __m128i c1 = _mm_set1_epi16(0x4097);
	const mlib_s32 ic1 = 0x4097;

/* abs( -0.3920 * 8192 ) */
	const __m128i c4 = _mm_set1_epi16(0xc8b);
	const mlib_s32 ic4 = 0xc8b;

/* abs( -0.8132 * 8192 ) */
	const __m128i c5 = _mm_set1_epi16(0x1a06);
	const mlib_s32 ic5 = 0x1a06;

/* 1.5966  * 8192 */
	const __m128i c8 = _mm_set1_epi16(0x3317);
	const mlib_s32 ic8 = 0x3317;

/* -276.9856 * 32 */
	const __m128i coff0 = _mm_set1_epi16(0xdd60);
	const mlib_s32 icoff0 = (mlib_s32)0xffffdd60;

/* 135.6352  * 32 */
	const __m128i coff1 = _mm_set1_epi16(0x10f4);
	const mlib_s32 icoff1 = 0x10f4;

/* -222.9952 * 32 */
	const __m128i coff2 = _mm_set1_epi16(0xe420);
	const mlib_s32 icoff2 = (mlib_s32)0xffffe420;

/* loop variable */
	mlib_s32 jH, iW;

/* pointers */
	mlib_u8 *pY, *pU, *pV, *pD, *pdd, *ptemp;
	__m128i *py, *pu, *pv;

/* variables */
	__m128i sy1, sy2, sy3, sy4, su1, su2, sv1, sv2;
	__m128i du0, du1, dv1, dv2;
	__m128i db1, db2, db3, db4, dr1, dr2, dr3, dr4, dg1, dg2, dg3, dg4;
	__m128i ddy1, ddy2, ddy3, ddy4, dzbl, dzbh, dgrl, dgrh, dbgrh, dbgrl;
	__m128i db_h, db_l, dg_h, dg_l, dr_h, dr_l, temp, bak;
	const __m128i x_zero = _mm_setzero_si128();
	const __m128i x_mask = _mm_set1_epi32(0xff);

/* for 4-pixel computing */
	mlib_s32 iu, iv, ig, ir, ib, iTemp;
	mlib_s32 iu0, iu1, iv1, iv2;

	pY  = (mlib_u8 *)y;
	pU  = (mlib_u8 *)u;
	pV  = (mlib_u8 *)v;
	pD = (mlib_u8 *)abgr;

	for (jH = 0; jH < height; jH++) {
		py = (__m128i *)pY;
		pu = (__m128i *)pU;
		pv = (__m128i *)pV;
		pdd = pD;
		iW = 0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		/* 32 pixels */
		for (; iW <= width - 32; iW += 32) {
			/* load y u v, and expand */
			temp = _mm_load_si128(pu);
			su1 = _mm_unpacklo_epi8(x_zero, temp);
			su2 = _mm_unpackhi_epi8(x_zero, temp);
			pu++;
			temp = _mm_load_si128(pv);
			sv1 = _mm_unpacklo_epi8(x_zero, temp);
			sv2 = _mm_unpackhi_epi8(x_zero, temp);
			pv++;
			temp = _mm_load_si128(py);
			sy1 = _mm_unpacklo_epi8(x_zero, temp);
			sy2 = _mm_unpackhi_epi8(x_zero, temp);
			py++;
			temp = _mm_load_si128(py);
			sy3 = _mm_unpacklo_epi8(x_zero, temp);
			sy4 = _mm_unpackhi_epi8(x_zero, temp);
			py++;

			/* pre-calc d[r/g/b][1234] */
			du0 = _mm_mulhi_epu16(su1, c1);
			db_l = _mm_add_epi16(du0, coff0);
			du0 = _mm_mulhi_epu16(su2, c1);
			db_h = _mm_add_epi16(du0, coff0);

			du1 = _mm_mulhi_epu16(su1, c4);
			dv1 = _mm_mulhi_epu16(sv1, c5);
			temp = _mm_add_epi16(du1, dv1);
			dg_l = _mm_sub_epi16(coff1, temp);
			du1 = _mm_mulhi_epu16(su2, c4);
			dv1 = _mm_mulhi_epu16(sv2, c5);
			temp = _mm_add_epi16(du1, dv1);
			dg_h = _mm_sub_epi16(coff1, temp);

			dv2 = _mm_mulhi_epu16(sv1, c8);
			dr_l = _mm_add_epi16(dv2, coff2);
			dv2 = _mm_mulhi_epu16(sv2, c8);
			dr_h = _mm_add_epi16(dv2, coff2);

			ddy1 = _mm_mulhi_epu16(sy1, c0);
			ddy2 = _mm_mulhi_epu16(sy2, c0);
			ddy3 = _mm_mulhi_epu16(sy3, c0);
			ddy4 = _mm_mulhi_epu16(sy4, c0);

			/* db1/2/3/4 */
			bak = _mm_unpacklo_epi16(db_l, db_l);
			db1 = _mm_add_epi16(ddy1, bak);
			bak = _mm_unpackhi_epi16(db_l, db_l);
			db2 = _mm_add_epi16(ddy2, bak);

			bak = _mm_unpacklo_epi16(db_h, db_h);
			db3 = _mm_add_epi16(ddy3, bak);
			bak = _mm_unpackhi_epi16(db_h, db_h);
			db4 = _mm_add_epi16(ddy4, bak);

			/* dg1/2/3/4 */
			bak = _mm_unpacklo_epi16(dg_l, dg_l);
			dg1 = _mm_add_epi16(ddy1, bak);
			bak = _mm_unpackhi_epi16(dg_l, dg_l);
			dg2 = _mm_add_epi16(ddy2, bak);

			bak = _mm_unpacklo_epi16(dg_h, dg_h);
			dg3 = _mm_add_epi16(ddy3, bak);
			bak = _mm_unpackhi_epi16(dg_h, dg_h);
			dg4 = _mm_add_epi16(ddy4, bak);

			/* dr1/2/3/4 */
			bak = _mm_unpacklo_epi16(dr_l, dr_l);
			dr1 = _mm_add_epi16(ddy1, bak);
			bak = _mm_unpackhi_epi16(dr_l, dr_l);
			dr2 = _mm_add_epi16(ddy2, bak);

			bak = _mm_unpacklo_epi16(dr_h, dr_h);
			dr3 = _mm_add_epi16(ddy3, bak);
			bak = _mm_unpackhi_epi16(dr_h, dr_h);
			dr4 = _mm_add_epi16(ddy4, bak);

			db1 = _mm_srai_epi16(db1, 5);
			db2 = _mm_srai_epi16(db2, 5);
			db3 = _mm_srai_epi16(db3, 5);
			db4 = _mm_srai_epi16(db4, 5);
			dg1 = _mm_srai_epi16(dg1, 5);
			dg2 = _mm_srai_epi16(dg2, 5);
			dg3 = _mm_srai_epi16(dg3, 5);
			dg4 = _mm_srai_epi16(dg4, 5);
			dr1 = _mm_srai_epi16(dr1, 5);
			dr2 = _mm_srai_epi16(dr2, 5);
			dr3 = _mm_srai_epi16(dr3, 5);
			dr4 = _mm_srai_epi16(dr4, 5);

			/* pack: 16=>8 */
			db1 = _mm_packus_epi16(db1, db2);
			db2 = _mm_packus_epi16(db3, db4);
			dr1 = _mm_packus_epi16(dr1, dr2);
			dr2 = _mm_packus_epi16(dr3, dr4);
			dg1 = _mm_packus_epi16(dg1, dg2);
			dg2 = _mm_packus_epi16(dg3, dg4);

			/* create bgr sequences : db/dr/dg[1] */
			dzbl = _mm_unpacklo_epi8(x_zero, db1);
			dzbh = _mm_unpackhi_epi8(x_zero, db1);
			dgrl = _mm_unpacklo_epi8(dg1, dr1);
			dgrh = _mm_unpackhi_epi8(dg1, dr1);

			dbgrl = _mm_unpacklo_epi16(dzbl, dgrl);
			SAVE_ABGR1(dbgrl);

			dbgrh = _mm_unpackhi_epi16(dzbl, dgrl);
			SAVE_ABGR1(dbgrh);

			dbgrl = _mm_unpacklo_epi16(dzbh, dgrh);
			SAVE_ABGR1(dbgrl);

			dbgrh = _mm_unpackhi_epi16(dzbh, dgrh);
			SAVE_ABGR1(dbgrh);

			/* create bgr sequences : db/dr/dg[2] */
			dzbl = _mm_unpacklo_epi8(x_zero, db2);
			dzbh = _mm_unpackhi_epi8(x_zero, db2);
			dgrl = _mm_unpacklo_epi8(dg2, dr2);
			dgrh = _mm_unpackhi_epi8(dg2, dr2);

			dbgrl = _mm_unpacklo_epi16(dzbl, dgrl);
			SAVE_ABGR1(dbgrl);

			dbgrh = _mm_unpackhi_epi16(dzbl, dgrl);
			SAVE_ABGR1(dbgrh);

			dbgrl = _mm_unpacklo_epi16(dzbh, dgrh);
			SAVE_ABGR1(dbgrl);

			dbgrh = _mm_unpackhi_epi16(dzbh, dgrh);
			SAVE_ABGR1(dbgrh);
		}

		/* 16 pixels */
		if (iW <= width - 16) {
			/* load y u v, and expand */
			temp = _mm_loadl_epi64(pu);
			su1 = _mm_unpacklo_epi8(x_zero, temp);
			pu = (__m128i *) (((__m64 *)pu) + 1);
			temp = _mm_loadl_epi64(pv);
			sv1 = _mm_unpacklo_epi8(x_zero, temp);
			pv = (__m128i *) (((__m64 *)pv) + 1);
			temp = _mm_load_si128(py);
			sy1 = _mm_unpacklo_epi8(x_zero, temp);
			sy2 = _mm_unpackhi_epi8(x_zero, temp);
			py++;

			/* pre-calc d[r/g/b][12] */
			du0 = _mm_mulhi_epu16(su1, c1);
			db_l = _mm_add_epi16(du0, coff0);

			du1 = _mm_mulhi_epu16(su1, c4);
			dv1 = _mm_mulhi_epu16(sv1, c5);
			temp = _mm_add_epi16(du1, dv1);
			dg_l = _mm_sub_epi16(coff1, temp);

			dv2 = _mm_mulhi_epu16(sv1, c8);
			dr_l = _mm_add_epi16(dv2, coff2);

			ddy1 = _mm_mulhi_epu16(sy1, c0);
			ddy2 = _mm_mulhi_epu16(sy2, c0);

			/* db1/2 */
			bak = _mm_unpacklo_epi16(db_l, db_l);
			db1 = _mm_add_epi16(ddy1, bak);
			bak = _mm_unpackhi_epi16(db_l, db_l);
			db2 = _mm_add_epi16(ddy2, bak);

			/* dg1/2 */
			bak = _mm_unpacklo_epi16(dg_l, dg_l);
			dg1 = _mm_add_epi16(ddy1, bak);
			bak = _mm_unpackhi_epi16(dg_l, dg_l);
			dg2 = _mm_add_epi16(ddy2, bak);

			/* dr1/2 */
			bak = _mm_unpacklo_epi16(dr_l, dr_l);
			dr1 = _mm_add_epi16(ddy1, bak);
			bak = _mm_unpackhi_epi16(dr_l, dr_l);
			dr2 = _mm_add_epi16(ddy2, bak);

			db1 = _mm_srai_epi16(db1, 5);
			db2 = _mm_srai_epi16(db2, 5);
			dg1 = _mm_srai_epi16(dg1, 5);
			dg2 = _mm_srai_epi16(dg2, 5);
			dr1 = _mm_srai_epi16(dr1, 5);
			dr2 = _mm_srai_epi16(dr2, 5);

			/* pack: 16=>8 */
			db1 = _mm_packus_epi16(db1, db2);
			dr1 = _mm_packus_epi16(dr1, dr2);
			dg1 = _mm_packus_epi16(dg1, dg2);

			/* create bgr sequences : db/dr/dg[1] */
			dzbl = _mm_unpacklo_epi8(x_zero, db1);
			dzbh = _mm_unpackhi_epi8(x_zero, db1);
			dgrl = _mm_unpacklo_epi8(dg1, dr1);
			dgrh = _mm_unpackhi_epi8(dg1, dr1);

			dbgrl = _mm_unpacklo_epi16(dzbl, dgrl);
			SAVE_ABGR1(dbgrl);

			dbgrh = _mm_unpackhi_epi16(dzbl, dgrl);
			SAVE_ABGR1(dbgrh);

			dbgrl = _mm_unpacklo_epi16(dzbh, dgrh);
			SAVE_ABGR1(dbgrl);

			dbgrh = _mm_unpackhi_epi16(dzbh, dgrh);
			SAVE_ABGR1(dbgrh);

			iW += 16;
		}

		/* 8 pixels */
		if (iW <= width - 8) {
			/* load y u v, and expand */
			iTemp = *((mlib_s32 *)pu);
			temp = _mm_cvtsi32_si128(iTemp);
			su1 = _mm_unpacklo_epi8(x_zero, temp);
			pu = (__m128i *) (((mlib_s32 *)pu) + 1);
			iTemp = *((mlib_s32 *)pv);
			temp = _mm_cvtsi32_si128(iTemp);
			sv1 = _mm_unpacklo_epi8(x_zero, temp);
			pv = (__m128i *) (((mlib_s32 *)pv) + 1);
			temp = _mm_loadl_epi64(py);
			sy1 = _mm_unpacklo_epi8(x_zero, temp);
			py = (__m128i *) (((__m64 *)py) + 1);

			/* pre-calc d[r/g/b][1] */
			du0 = _mm_mulhi_epu16(su1, c1);
			db_l = _mm_add_epi16(du0, coff0);

			du1 = _mm_mulhi_epu16(su1, c4);
			dv1 = _mm_mulhi_epu16(sv1, c5);
			temp = _mm_add_epi16(du1, dv1);
			dg_l = _mm_sub_epi16(coff1, temp);

			dv2 = _mm_mulhi_epu16(sv1, c8);
			dr_l = _mm_add_epi16(dv2, coff2);

			ddy1 = _mm_mulhi_epu16(sy1, c0);

			/* db1 */
			bak = _mm_unpacklo_epi16(db_l, db_l);
			db1 = _mm_add_epi16(ddy1, bak);

			/* dg1 */
			bak = _mm_unpacklo_epi16(dg_l, dg_l);
			dg1 = _mm_add_epi16(ddy1, bak);

			/* dr1 */
			bak = _mm_unpacklo_epi16(dr_l, dr_l);
			dr1 = _mm_add_epi16(ddy1, bak);

			db1 = _mm_srai_epi16(db1, 5);
			dg1 = _mm_srai_epi16(dg1, 5);
			dr1 = _mm_srai_epi16(dr1, 5);

			/* pack: 16=>8 */
			db1 = _mm_packus_epi16(db1, x_zero);
			dr1 = _mm_packus_epi16(dr1, x_zero);
			dg1 = _mm_packus_epi16(dg1, x_zero);

			/* create bgr sequences : db/dr/dg[1] */
			dzbl = _mm_unpacklo_epi8(x_zero, db1);
			dgrl = _mm_unpacklo_epi8(dg1, dr1);

			dbgrl = _mm_unpacklo_epi16(dzbl, dgrl);
			SAVE_ABGR1(dbgrl);

			dbgrh = _mm_unpackhi_epi16(dzbl, dgrl);
			SAVE_ABGR1(dbgrh);

			iW += 8;
		}

		/* 4 pixels */
		if (iW <= width - 4) {
			/* load y u v, and expand */
			iTemp = *((mlib_s16 *)pu);
			temp = _mm_cvtsi32_si128(iTemp);
			su1 = _mm_unpacklo_epi8(x_zero, temp);
			pu = (__m128i *) (((mlib_s16 *)pu) + 1);
			iTemp = *((mlib_s16 *)pv);
			temp = _mm_cvtsi32_si128(iTemp);
			sv1 = _mm_unpacklo_epi8(x_zero, temp);
			pv = (__m128i *) (((mlib_s16 *)pv) + 1);
			iTemp = *((mlib_s32 *)py);
			temp = _mm_cvtsi32_si128(iTemp);
			sy1 = _mm_unpacklo_epi8(x_zero, temp);
			py = (__m128i *) (((mlib_s32 *)py) + 1);

			/* pre-calc d[r/g/b][1] */
			du0 = _mm_mulhi_epu16(su1, c1);
			db_l = _mm_add_epi16(du0, coff0);

			du1 = _mm_mulhi_epu16(su1, c4);
			dv1 = _mm_mulhi_epu16(sv1, c5);
			temp = _mm_add_epi16(du1, dv1);
			dg_l = _mm_sub_epi16(coff1, temp);

			dv2 = _mm_mulhi_epu16(sv1, c8);
			dr_l = _mm_add_epi16(dv2, coff2);

			ddy1 = _mm_mulhi_epu16(sy1, c0);

			/* db1 */
			bak = _mm_unpacklo_epi16(db_l, db_l);
			db1 = _mm_add_epi16(ddy1, bak);

			/* dg1 */
			bak = _mm_unpacklo_epi16(dg_l, dg_l);
			dg1 = _mm_add_epi16(ddy1, bak);

			/* dr1 */
			bak = _mm_unpacklo_epi16(dr_l, dr_l);
			dr1 = _mm_add_epi16(ddy1, bak);

			db1 = _mm_srai_epi16(db1, 5);
			dg1 = _mm_srai_epi16(dg1, 5);
			dr1 = _mm_srai_epi16(dr1, 5);

			/* pack: 16=>8 */
			db1 = _mm_packus_epi16(db1, x_zero);
			dr1 = _mm_packus_epi16(dr1, x_zero);
			dg1 = _mm_packus_epi16(dg1, x_zero);

			/* create bgr sequences : db/dr/dg[1] */
			dzbl = _mm_unpacklo_epi8(x_zero, db1);
			dgrl = _mm_unpacklo_epi8(dg1, dr1);

			dbgrl = _mm_unpacklo_epi16(dzbl, dgrl);
			SAVE_ABGR1(dbgrl);

			iW += 4;
		}

		/* 2 pixels */
		if (iW <= width - 2) {
			/* load y u v, and expand */
			iu = *((mlib_u8 *)pu);
			pu = (__m128i *) (((mlib_u8 *)pu) + 1);
			iv = *((mlib_u8 *)pv);
			pv = (__m128i *) (((mlib_u8 *)pv) + 1);
			iTemp = *((mlib_s16 *)py);
			temp = _mm_cvtsi32_si128(iTemp);
			sy1 = _mm_unpacklo_epi8(x_zero, temp);
			py = (__m128i *) (((mlib_s16 *)py) + 1);

			/* pre-calc d[r/g/b][1] */
			iu0 = (iu * ic1) >> 8;
			ib = icoff0 + iu0;

			iu1 = (iu * ic4) >> 8;
			iv1 = (iv * ic5) >> 8;
			iTemp = iu1 + iv1;
			ig = icoff1 - iTemp;

			iv2 = (iv * ic8) >> 8;
			ir = iv2 + icoff2;

			ddy1 = _mm_mulhi_epu16(sy1, c0);

			/* db1 */
			temp = _mm_set1_epi16(ib);
			db1 = _mm_add_epi16(ddy1, temp);

			/* dg1 */
			temp = _mm_set1_epi16(ig);
			dg1 = _mm_add_epi16(ddy1, temp);

			/* dr1 */
			temp = _mm_set1_epi16(ir);
			dr1 = _mm_add_epi16(ddy1, temp);

			db1 = _mm_srai_epi16(db1, 5);
			dg1 = _mm_srai_epi16(dg1, 5);
			dr1 = _mm_srai_epi16(dr1, 5);

			/* pack: 16=>8 */
			db1 = _mm_packus_epi16(db1, x_zero);
			dr1 = _mm_packus_epi16(dr1, x_zero);
			dg1 = _mm_packus_epi16(dg1, x_zero);

			/* create bgr sequences : db/dr/dg */
			dzbl = _mm_unpacklo_epi8(x_zero, db1);
			dgrl = _mm_unpacklo_epi8(dg1, dr1);

			/* lower half of dzbl & dgrl */
			dbgrl = _mm_unpacklo_epi16(dzbl, dgrl);
			ptemp = (mlib_u8*)(&dbgrl);
			pdd += 1;
			ptemp += 1;
			*((mlib_s16*)pdd) = *((mlib_s16*)ptemp);
			pdd += 2;
			ptemp += 2;
			*((mlib_u8*)pdd) = *((mlib_u8*)ptemp);
			pdd += 2;
			ptemp += 2;
			*((mlib_s16*)pdd) = *((mlib_s16*)ptemp);
			pdd += 2;
			ptemp += 2;
			*((mlib_u8*)pdd) = *((mlib_u8*)ptemp);
			pdd += 1;

			iW += 2;
		}

		pY += y_stride;
		pU += uv_stride;
		pV += uv_stride;
		pD += abgr_stride;
	}

	return (MLIB_SUCCESS);
}

mlib_status
mlib_VideoColorYUV2ABGR422_naligned(
	mlib_u8 *abgr,
	const mlib_u8 *y,
	const mlib_u8 *u,
	const mlib_u8 *v,
	mlib_s32 width,
	mlib_s32 height,
	mlib_s32 abgr_stride,
	mlib_s32 y_stride,
	mlib_s32 uv_stride)
{
/* 1.1644  * 8192 */
	const __m128i c0 = _mm_set1_epi16(0x2543);
	const mlib_s32 ic0 = 0x2543;

/* 2.0184  * 8192 */
	const __m128i c1 = _mm_set1_epi16(0x4097);
	const mlib_s32 ic1 = 0x4097;

/* abs( -0.3920 * 8192 ) */
	const __m128i c4 = _mm_set1_epi16(0xc8b);
	const mlib_s32 ic4 = 0xc8b;

/* abs( -0.8132 * 8192 ) */
	const __m128i c5 = _mm_set1_epi16(0x1a06);
	const mlib_s32 ic5 = 0x1a06;

/* 1.5966  * 8192 */
	const __m128i c8 = _mm_set1_epi16(0x3317);
	const mlib_s32 ic8 = 0x3317;

/* -276.9856 * 32 */
	const __m128i coff0 = _mm_set1_epi16(0xdd60);
	const mlib_s32 icoff0 = (mlib_s32)0xffffdd60;

/* 135.6352  * 32 */
	const __m128i coff1 = _mm_set1_epi16(0x10f4);
	const mlib_s32 icoff1 = 0x10f4;

/* -222.9952 * 32 */
	const __m128i coff2 = _mm_set1_epi16(0xe420);
	const mlib_s32 icoff2 = (mlib_s32)0xffffe420;

/* loop variable */
	mlib_s32 jH, iW;

/* pointers */
	mlib_u8 *pY, *pU, *pV, *pD, *pdd, *ptemp;
	__m128i *py, *pu, *pv;

/* variables */
	__m128i sy1, sy2, sy3, sy4, su1, su2, sv1, sv2;
	__m128i du0, du1, dv1, dv2;
	__m128i db1, db2, db3, db4, dr1, dr2, dr3, dr4, dg1, dg2, dg3, dg4;
	__m128i ddy1, ddy2, ddy3, ddy4, dzbl, dzbh, dgrl, dgrh, dbgrh, dbgrl;
	__m128i db_h, db_l, dg_h, dg_l, dr_h, dr_l, temp, bak;
	const __m128i x_zero = _mm_setzero_si128();
	const __m128i x_mask = _mm_set1_epi32(0xff);

/* for 4-pixel computing */
	mlib_s32 iu, iv, ig, ir, ib, iTemp;
	mlib_s32 iu0, iu1, iv1, iv2;

	pY  = (mlib_u8 *)y;
	pU  = (mlib_u8 *)u;
	pV  = (mlib_u8 *)v;
	pD = (mlib_u8 *)abgr;

	for (jH = 0; jH < height; jH++) {
		py = (__m128i *)pY;
		pu = (__m128i *)pU;
		pv = (__m128i *)pV;
		pdd = pD;
		iW = 0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		/* 32 pixels */
		for (; iW <= width - 32; iW += 32) {
			/* load y u v, and expand */
			temp = _mm_loadu_si128(pu);
			su1 = _mm_unpacklo_epi8(x_zero, temp);
			su2 = _mm_unpackhi_epi8(x_zero, temp);
			pu++;
			temp = _mm_loadu_si128(pv);
			sv1 = _mm_unpacklo_epi8(x_zero, temp);
			sv2 = _mm_unpackhi_epi8(x_zero, temp);
			pv++;
			temp = _mm_loadu_si128(py);
			sy1 = _mm_unpacklo_epi8(x_zero, temp);
			sy2 = _mm_unpackhi_epi8(x_zero, temp);
			py++;
			temp = _mm_loadu_si128(py);
			sy3 = _mm_unpacklo_epi8(x_zero, temp);
			sy4 = _mm_unpackhi_epi8(x_zero, temp);
			py++;

			/* pre-calc d[r/g/b][1234] */
			du0 = _mm_mulhi_epu16(su1, c1);
			db_l = _mm_add_epi16(du0, coff0);
			du0 = _mm_mulhi_epu16(su2, c1);
			db_h = _mm_add_epi16(du0, coff0);

			du1 = _mm_mulhi_epu16(su1, c4);
			dv1 = _mm_mulhi_epu16(sv1, c5);
			temp = _mm_add_epi16(du1, dv1);
			dg_l = _mm_sub_epi16(coff1, temp);
			du1 = _mm_mulhi_epu16(su2, c4);
			dv1 = _mm_mulhi_epu16(sv2, c5);
			temp = _mm_add_epi16(du1, dv1);
			dg_h = _mm_sub_epi16(coff1, temp);

			dv2 = _mm_mulhi_epu16(sv1, c8);
			dr_l = _mm_add_epi16(dv2, coff2);
			dv2 = _mm_mulhi_epu16(sv2, c8);
			dr_h = _mm_add_epi16(dv2, coff2);

			ddy1 = _mm_mulhi_epu16(sy1, c0);
			ddy2 = _mm_mulhi_epu16(sy2, c0);
			ddy3 = _mm_mulhi_epu16(sy3, c0);
			ddy4 = _mm_mulhi_epu16(sy4, c0);

			/* db1/2/3/4 */
			bak = _mm_unpacklo_epi16(db_l, db_l);
			db1 = _mm_add_epi16(ddy1, bak);
			bak = _mm_unpackhi_epi16(db_l, db_l);
			db2 = _mm_add_epi16(ddy2, bak);

			bak = _mm_unpacklo_epi16(db_h, db_h);
			db3 = _mm_add_epi16(ddy3, bak);
			bak = _mm_unpackhi_epi16(db_h, db_h);
			db4 = _mm_add_epi16(ddy4, bak);

			/* dg1/2/3/4 */
			bak = _mm_unpacklo_epi16(dg_l, dg_l);
			dg1 = _mm_add_epi16(ddy1, bak);
			bak = _mm_unpackhi_epi16(dg_l, dg_l);
			dg2 = _mm_add_epi16(ddy2, bak);

			bak = _mm_unpacklo_epi16(dg_h, dg_h);
			dg3 = _mm_add_epi16(ddy3, bak);
			bak = _mm_unpackhi_epi16(dg_h, dg_h);
			dg4 = _mm_add_epi16(ddy4, bak);

			/* dr1/2/3/4 */
			bak = _mm_unpacklo_epi16(dr_l, dr_l);
			dr1 = _mm_add_epi16(ddy1, bak);
			bak = _mm_unpackhi_epi16(dr_l, dr_l);
			dr2 = _mm_add_epi16(ddy2, bak);

			bak = _mm_unpacklo_epi16(dr_h, dr_h);
			dr3 = _mm_add_epi16(ddy3, bak);
			bak = _mm_unpackhi_epi16(dr_h, dr_h);
			dr4 = _mm_add_epi16(ddy4, bak);

			db1 = _mm_srai_epi16(db1, 5);
			db2 = _mm_srai_epi16(db2, 5);
			db3 = _mm_srai_epi16(db3, 5);
			db4 = _mm_srai_epi16(db4, 5);
			dg1 = _mm_srai_epi16(dg1, 5);
			dg2 = _mm_srai_epi16(dg2, 5);
			dg3 = _mm_srai_epi16(dg3, 5);
			dg4 = _mm_srai_epi16(dg4, 5);
			dr1 = _mm_srai_epi16(dr1, 5);
			dr2 = _mm_srai_epi16(dr2, 5);
			dr3 = _mm_srai_epi16(dr3, 5);
			dr4 = _mm_srai_epi16(dr4, 5);

			/* pack: 16=>8 */
			db1 = _mm_packus_epi16(db1, db2);
			db2 = _mm_packus_epi16(db3, db4);
			dr1 = _mm_packus_epi16(dr1, dr2);
			dr2 = _mm_packus_epi16(dr3, dr4);
			dg1 = _mm_packus_epi16(dg1, dg2);
			dg2 = _mm_packus_epi16(dg3, dg4);

			/* create bgr sequences : db/dr/dg[1] */
			dzbl = _mm_unpacklo_epi8(x_zero, db1);
			dzbh = _mm_unpackhi_epi8(x_zero, db1);
			dgrl = _mm_unpacklo_epi8(dg1, dr1);
			dgrh = _mm_unpackhi_epi8(dg1, dr1);

			dbgrl = _mm_unpacklo_epi16(dzbl, dgrl);
			SAVE_ABGR2(dbgrl);

			dbgrh = _mm_unpackhi_epi16(dzbl, dgrl);
			SAVE_ABGR2(dbgrh);

			dbgrl = _mm_unpacklo_epi16(dzbh, dgrh);
			SAVE_ABGR2(dbgrl);

			dbgrh = _mm_unpackhi_epi16(dzbh, dgrh);
			SAVE_ABGR2(dbgrh);

			/* create bgr sequences : db/dr/dg[2] */
			dzbl = _mm_unpacklo_epi8(x_zero, db2);
			dzbh = _mm_unpackhi_epi8(x_zero, db2);
			dgrl = _mm_unpacklo_epi8(dg2, dr2);
			dgrh = _mm_unpackhi_epi8(dg2, dr2);

			dbgrl = _mm_unpacklo_epi16(dzbl, dgrl);
			SAVE_ABGR2(dbgrl);

			dbgrh = _mm_unpackhi_epi16(dzbl, dgrl);
			SAVE_ABGR2(dbgrh);

			dbgrl = _mm_unpacklo_epi16(dzbh, dgrh);
			SAVE_ABGR2(dbgrl);

			dbgrh = _mm_unpackhi_epi16(dzbh, dgrh);
			SAVE_ABGR2(dbgrh);
		}

		/* 16 pixels */
		if (iW <= width - 16) {
			/* load y u v, and expand */
			temp = _mm_loadl_epi64(pu);
			su1 = _mm_unpacklo_epi8(x_zero, temp);
			pu = (__m128i *) (((__m64 *)pu) + 1);
			temp = _mm_loadl_epi64(pv);
			sv1 = _mm_unpacklo_epi8(x_zero, temp);
			pv = (__m128i *) (((__m64 *)pv) + 1);
			temp = _mm_loadu_si128(py);
			sy1 = _mm_unpacklo_epi8(x_zero, temp);
			sy2 = _mm_unpackhi_epi8(x_zero, temp);
			py++;

			/* pre-calc d[r/g/b][12] */
			du0 = _mm_mulhi_epu16(su1, c1);
			db_l = _mm_add_epi16(du0, coff0);

			du1 = _mm_mulhi_epu16(su1, c4);
			dv1 = _mm_mulhi_epu16(sv1, c5);
			temp = _mm_add_epi16(du1, dv1);
			dg_l = _mm_sub_epi16(coff1, temp);

			dv2 = _mm_mulhi_epu16(sv1, c8);
			dr_l = _mm_add_epi16(dv2, coff2);

			ddy1 = _mm_mulhi_epu16(sy1, c0);
			ddy2 = _mm_mulhi_epu16(sy2, c0);

			/* db1/2 */
			bak = _mm_unpacklo_epi16(db_l, db_l);
			db1 = _mm_add_epi16(ddy1, bak);
			bak = _mm_unpackhi_epi16(db_l, db_l);
			db2 = _mm_add_epi16(ddy2, bak);

			/* dg1/2 */
			bak = _mm_unpacklo_epi16(dg_l, dg_l);
			dg1 = _mm_add_epi16(ddy1, bak);
			bak = _mm_unpackhi_epi16(dg_l, dg_l);
			dg2 = _mm_add_epi16(ddy2, bak);

			/* dr1/2 */
			bak = _mm_unpacklo_epi16(dr_l, dr_l);
			dr1 = _mm_add_epi16(ddy1, bak);
			bak = _mm_unpackhi_epi16(dr_l, dr_l);
			dr2 = _mm_add_epi16(ddy2, bak);

			db1 = _mm_srai_epi16(db1, 5);
			db2 = _mm_srai_epi16(db2, 5);
			dg1 = _mm_srai_epi16(dg1, 5);
			dg2 = _mm_srai_epi16(dg2, 5);
			dr1 = _mm_srai_epi16(dr1, 5);
			dr2 = _mm_srai_epi16(dr2, 5);

			/* pack: 16=>8 */
			db1 = _mm_packus_epi16(db1, db2);
			dr1 = _mm_packus_epi16(dr1, dr2);
			dg1 = _mm_packus_epi16(dg1, dg2);

			/* create bgr sequences : db/dr/dg[1] */
			dzbl = _mm_unpacklo_epi8(x_zero, db1);
			dzbh = _mm_unpackhi_epi8(x_zero, db1);
			dgrl = _mm_unpacklo_epi8(dg1, dr1);
			dgrh = _mm_unpackhi_epi8(dg1, dr1);

			dbgrl = _mm_unpacklo_epi16(dzbl, dgrl);
			SAVE_ABGR2(dbgrl);

			dbgrh = _mm_unpackhi_epi16(dzbl, dgrl);
			SAVE_ABGR2(dbgrh);

			dbgrl = _mm_unpacklo_epi16(dzbh, dgrh);
			SAVE_ABGR2(dbgrl);

			dbgrh = _mm_unpackhi_epi16(dzbh, dgrh);
			SAVE_ABGR2(dbgrh);

			iW += 16;
		}

		/* 8 pixels */
		if (iW <= width - 8) {
			/* load y u v, and expand */
			iTemp = *((mlib_s32 *)pu);
			temp = _mm_cvtsi32_si128(iTemp);
			su1 = _mm_unpacklo_epi8(x_zero, temp);
			pu = (__m128i *) (((mlib_s32 *)pu) + 1);
			iTemp = *((mlib_s32 *)pv);
			temp = _mm_cvtsi32_si128(iTemp);
			sv1 = _mm_unpacklo_epi8(x_zero, temp);
			pv = (__m128i *) (((mlib_s32 *)pv) + 1);
			temp = _mm_loadl_epi64(py);
			sy1 = _mm_unpacklo_epi8(x_zero, temp);
			py = (__m128i *) (((__m64 *)py) + 1);

			/* pre-calc d[r/g/b][1] */
			du0 = _mm_mulhi_epu16(su1, c1);
			db_l = _mm_add_epi16(du0, coff0);

			du1 = _mm_mulhi_epu16(su1, c4);
			dv1 = _mm_mulhi_epu16(sv1, c5);
			temp = _mm_add_epi16(du1, dv1);
			dg_l = _mm_sub_epi16(coff1, temp);

			dv2 = _mm_mulhi_epu16(sv1, c8);
			dr_l = _mm_add_epi16(dv2, coff2);

			ddy1 = _mm_mulhi_epu16(sy1, c0);

			/* db1 */
			bak = _mm_unpacklo_epi16(db_l, db_l);
			db1 = _mm_add_epi16(ddy1, bak);

			/* dg1 */
			bak = _mm_unpacklo_epi16(dg_l, dg_l);
			dg1 = _mm_add_epi16(ddy1, bak);

			/* dr1 */
			bak = _mm_unpacklo_epi16(dr_l, dr_l);
			dr1 = _mm_add_epi16(ddy1, bak);

			db1 = _mm_srai_epi16(db1, 5);
			dg1 = _mm_srai_epi16(dg1, 5);
			dr1 = _mm_srai_epi16(dr1, 5);

			/* pack: 16=>8 */
			db1 = _mm_packus_epi16(db1, x_zero);
			dr1 = _mm_packus_epi16(dr1, x_zero);
			dg1 = _mm_packus_epi16(dg1, x_zero);

			/* create bgr sequences : db/dr/dg[1] */
			dzbl = _mm_unpacklo_epi8(x_zero, db1);
			dgrl = _mm_unpacklo_epi8(dg1, dr1);

			dbgrl = _mm_unpacklo_epi16(dzbl, dgrl);
			SAVE_ABGR2(dbgrl);

			dbgrh = _mm_unpackhi_epi16(dzbl, dgrl);
			SAVE_ABGR2(dbgrh);

			iW += 8;
		}

		/* 4 pixels */
		if (iW <= width - 4) {
			/* load y u v, and expand */
			iTemp = *((mlib_s16 *)pu);
			temp = _mm_cvtsi32_si128(iTemp);
			su1 = _mm_unpacklo_epi8(x_zero, temp);
			pu = (__m128i *) (((mlib_s16 *)pu) + 1);
			iTemp = *((mlib_s16 *)pv);
			temp = _mm_cvtsi32_si128(iTemp);
			sv1 = _mm_unpacklo_epi8(x_zero, temp);
			pv = (__m128i *) (((mlib_s16 *)pv) + 1);
			iTemp = *((mlib_s32 *)py);
			temp = _mm_cvtsi32_si128(iTemp);
			sy1 = _mm_unpacklo_epi8(x_zero, temp);
			py = (__m128i *) (((mlib_s32 *)py) + 1);

			/* pre-calc d[r/g/b][1] */
			du0 = _mm_mulhi_epu16(su1, c1);
			db_l = _mm_add_epi16(du0, coff0);

			du1 = _mm_mulhi_epu16(su1, c4);
			dv1 = _mm_mulhi_epu16(sv1, c5);
			temp = _mm_add_epi16(du1, dv1);
			dg_l = _mm_sub_epi16(coff1, temp);

			dv2 = _mm_mulhi_epu16(sv1, c8);
			dr_l = _mm_add_epi16(dv2, coff2);

			ddy1 = _mm_mulhi_epu16(sy1, c0);

			/* db1 */
			bak = _mm_unpacklo_epi16(db_l, db_l);
			db1 = _mm_add_epi16(ddy1, bak);

			/* dg1 */
			bak = _mm_unpacklo_epi16(dg_l, dg_l);
			dg1 = _mm_add_epi16(ddy1, bak);

			/* dr1 */
			bak = _mm_unpacklo_epi16(dr_l, dr_l);
			dr1 = _mm_add_epi16(ddy1, bak);

			db1 = _mm_srai_epi16(db1, 5);
			dg1 = _mm_srai_epi16(dg1, 5);
			dr1 = _mm_srai_epi16(dr1, 5);

			/* pack: 16=>8 */
			db1 = _mm_packus_epi16(db1, x_zero);
			dr1 = _mm_packus_epi16(dr1, x_zero);
			dg1 = _mm_packus_epi16(dg1, x_zero);

			/* create bgr sequences : db/dr/dg[1] */
			dzbl = _mm_unpacklo_epi8(x_zero, db1);
			dgrl = _mm_unpacklo_epi8(dg1, dr1);

			dbgrl = _mm_unpacklo_epi16(dzbl, dgrl);
			SAVE_ABGR2(dbgrl);

			iW += 4;
		}

		/* 2 pixels */
		if (iW <= width - 2) {
			/* load y u v, and expand */
			iu = *((mlib_u8 *)pu);
			pu = (__m128i *) (((mlib_u8 *)pu) + 1);
			iv = *((mlib_u8 *)pv);
			pv = (__m128i *) (((mlib_u8 *)pv) + 1);
			iTemp = *((mlib_s16 *)py);
			temp = _mm_cvtsi32_si128(iTemp);
			sy1 = _mm_unpacklo_epi8(x_zero, temp);
			py = (__m128i *) (((mlib_s16 *)py) + 1);

			/* pre-calc d[r/g/b][1] */
			iu0 = (iu * ic1) >> 8;
			ib = icoff0 + iu0;

			iu1 = (iu * ic4) >> 8;
			iv1 = (iv * ic5) >> 8;
			iTemp = iu1 + iv1;
			ig = icoff1 - iTemp;

			iv2 = (iv * ic8) >> 8;
			ir = iv2 + icoff2;

			ddy1 = _mm_mulhi_epu16(sy1, c0);

			/* db1 */
			temp = _mm_set1_epi16(ib);
			db1 = _mm_add_epi16(ddy1, temp);

			/* dg1 */
			temp = _mm_set1_epi16(ig);
			dg1 = _mm_add_epi16(ddy1, temp);

			/* dr1 */
			temp = _mm_set1_epi16(ir);
			dr1 = _mm_add_epi16(ddy1, temp);

			db1 = _mm_srai_epi16(db1, 5);
			dg1 = _mm_srai_epi16(dg1, 5);
			dr1 = _mm_srai_epi16(dr1, 5);

			/* pack: 16=>8 */
			db1 = _mm_packus_epi16(db1, x_zero);
			dr1 = _mm_packus_epi16(dr1, x_zero);
			dg1 = _mm_packus_epi16(dg1, x_zero);

			/* create bgr sequences : db/dr/dg */
			dzbl = _mm_unpacklo_epi8(x_zero, db1);
			dgrl = _mm_unpacklo_epi8(dg1, dr1);

			/* lower half of dzbl & dgrl */
			dbgrl = _mm_unpacklo_epi16(dzbl, dgrl);
			ptemp = (mlib_u8*)(&dbgrl);
			pdd += 1;
			ptemp += 1;
			*((mlib_s16*)pdd) = *((mlib_s16*)ptemp);
			pdd += 2;
			ptemp += 2;
			*((mlib_u8*)pdd) = *((mlib_u8*)ptemp);
			pdd += 2;
			ptemp += 2;
			*((mlib_s16*)pdd) = *((mlib_s16*)ptemp);
			pdd += 2;
			ptemp += 2;
			*((mlib_u8*)pdd) = *((mlib_u8*)ptemp);
			pdd += 1;

			iW += 2;
		}

		pY += y_stride;
		pU += uv_stride;
		pV += uv_stride;
		pD += abgr_stride;
	}

	return (MLIB_SUCCESS);
}
/* *********************************************************** */
