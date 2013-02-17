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

#pragma ident	"@(#)mlib_ImageZoomEdgeIndex.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageZoomIndexExtend_[U8 | S16]_[Bilinear | Bicubic | Bicubic2] -
 *             pseudo - extended source index image and computing
 *
 * SYNOPSIS
 *      void mlib_ImageZoomIndexExtend_U8_Bilinear(mlib_work_image *param)
 *      void mlib_ImageZoomIndexExtend_S16_Bilinear(mlib_work_image *param)
 *      void mlib_ImageZoomIndexExtend_U8_Bicubic(mlib_work_image *param)
 *      void mlib_ImageZoomIndexExtend_S16_Bicubic(mlib_work_image *param)
 *
 * ARGUMENTS
 *      param     Internal structure
 *
 * DESCRIPTION
 *  The center of the source image is mapped to the center of the
 *  destination image.
 *
 *  Computing border rules.
 *
 */

#include <mlib_image.h>
#include <mlib_ImageFilters.h>
#include <mlib_ImageZoom.h>
#include <mlib_ImageColormap.h>

/* *********************************************************** */

#define	VARIABLE_SRC_EXTEND(FORMAT)                                   \
	mlib_s32 sw = GetElemStruct(width) - 1,                       \
		sh = GetElemStruct(height) - 1,                       \
		src_stride = GetElemStruct(src_stride),               \
		dx = GetElemStruct(DX),                               \
		dy = GetElemStruct(DY),                               \
		x0 = GetElemSubStruct(current, srcX) - dx * w2,       \
		x1 = GetElemSubStruct(current, srcX) +                \
			GetElemSubStruct(current, width) * dx,        \
		y0 = GetElemSubStruct(current, srcY) - dy * h1,       \
	y1 = GetElemSubStruct(current, srcY),                         \
		y2 = y1 + GetElemSubStruct(current, height) * dy;     \
	FORMAT *sp = (FORMAT *) GetElemStruct(sp)

/* *********************************************************** */

#define	MLIB_SRC_EXTEND_INDEX_BL(FMT, dp, x, y, w, h, xx, yy,             \
	ww, hh, MACRO)                                                    \
	{                                                                 \
	    mlib_s32 tdy, tdx, cx, cy, sx, sy;                            \
	    mlib_f32 a0, a1, a2, a3, fx, fy;                              \
	                                                                  \
	    td##y = yy;                                                   \
	    for (j = 0; j < hh; j++) {                                    \
		td##x = xx;                                               \
		COORD(y, h);                                              \
		for (i = 0; i < ww; i++) {                                \
		    COORD(x, w);                                          \
		    for (ch = 0; ch < channels; ch++) {                   \
			csp = (FMT *) (ctable + sp[cy * src_stride +      \
				cx]);                                     \
			a0 = csp[ch + 4 - channels];                      \
			csp = (FMT *) (ctable + sp[cy * src_stride +      \
				cx + sx]);                                \
			a1 = csp[ch + 4 - channels];                      \
			csp = (FMT *) (ctable + sp[(cy +                  \
				sy) * src_stride + cx]);                  \
			a3 = csp[ch + 4 - channels];                      \
			csp = (FMT *) (ctable + sp[(cy +                  \
				sy) * src_stride + cx + sx]);             \
			a2 = csp[ch + 4 - channels];                      \
			dpbuf[i * channels + ch] =                        \
				(FMT) (a0 + fy * (a3 - a0) + fx * (a1 -   \
				a0) + fx * fy * (a0 + a2 - a3 - a1));     \
		    }                                                     \
		    td##x += d##x;                                        \
		}                                                         \
		MACRO(ww, dp);                                            \
		td##y += d##y;                                            \
	    }                                                             \
	}

/* *********************************************************** */

#define	COORD(y, h)                                             \
	c##y = td##y >> MLIB_SHIFT;                             \
	f##y = (mlib_f32)((td##y & MLIB_MASK) * MLIB_SCALE);    \
	s##y = (mlib_u32)(c##y - s##h) >> MLIB_SIGN_SHIFT;      \
	c##y &= ~(td##y >> MLIB_SIGN_SHIFT);                    \
	s##y &= ~(td##y >> MLIB_SIGN_SHIFT)

/* *********************************************************** */

#define	HOR(w, dp)	pack_func(dpbuf, dp + j * dst_stride, w, colormap)

/* *********************************************************** */

#define	VER(w, dp)                                              \
	pack_func(dpbuf, tdpbuf, w, colormap);                  \
	for (i = 0; i < w; i++)                                 \
	    dp[i * dst_stride + j] = tdpbuf[i]

/* *********************************************************** */

void
mlib_ImageZoomIndexExtend_U8_Bilinear(
	mlib_work_image * param)
{
	VARIABLE_EDGE(mlib_u8);
	VARIABLE_SRC_EXTEND(mlib_u8);
	void *colormap = GetElemStruct(colormap);
	mlib_type colortype = mlib_ImageGetLutType(colormap);
	mlib_pack_func pack_func;
	mlib_u8 *tdpbuf = GetElemStruct(buffer_dp);

	channels = mlib_ImageGetLutChannels(colormap);

	if (colortype == MLIB_BYTE) {
		mlib_u8 *csp, *dpbuf = GetElemStruct(buffer_dp);
		mlib_f32 *ctable =
			(mlib_f32 *)mlib_ImageGetLutNormalTable(colormap) -
			mlib_ImageGetLutOffset(colormap);

		if (channels == 3)
			pack_func =
				(mlib_pack_func) &
				mlib_ImageColorTrue2IndexLine_U8_U8_3;
		else
			pack_func =
				(mlib_pack_func) &
				mlib_ImageColorTrue2IndexLine_U8_U8_4;

		MLIB_SRC_EXTEND_INDEX_BL(mlib_u8,
			dp0,
			x,
			y,
			w,
			h,
			x0,
			y0,
			w1,
			h1,
			HOR);
		MLIB_SRC_EXTEND_INDEX_BL(mlib_u8,
			dp3,
			x,
			y,
			w,
			h,
			x0,
			y2,
			w1,
			h3,
			HOR);
		MLIB_SRC_EXTEND_INDEX_BL(mlib_u8,
			dp1,
			y,
			x,
			h,
			w,
			y1,
			x0,
			h2,
			w2,
			VER);
		MLIB_SRC_EXTEND_INDEX_BL(mlib_u8,
			dp2,
			y,
			x,
			h,
			w,
			y1,
			x1,
			h2,
			w3,
			VER);
	} else {
		mlib_s16 *csp, *dpbuf = GetElemStruct(buffer_dp);
		mlib_d64 *ctable =
			(mlib_d64 *)mlib_ImageGetLutNormalTable(colormap) -
			mlib_ImageGetLutOffset(colormap);

		if (channels == 3)
			pack_func =
				(mlib_pack_func) &
				mlib_ImageColorTrue2IndexLine_S16_U8_3;
		else
			pack_func =
				(mlib_pack_func) &
				mlib_ImageColorTrue2IndexLine_S16_U8_4;

		MLIB_SRC_EXTEND_INDEX_BL(mlib_s16,
			dp0,
			x,
			y,
			w,
			h,
			x0,
			y0,
			w1,
			h1,
			HOR);
		MLIB_SRC_EXTEND_INDEX_BL(mlib_s16,
			dp3,
			x,
			y,
			w,
			h,
			x0,
			y2,
			w1,
			h3,
			HOR);
		MLIB_SRC_EXTEND_INDEX_BL(mlib_s16,
			dp1,
			y,
			x,
			h,
			w,
			y1,
			x0,
			h2,
			w2,
			VER);
		MLIB_SRC_EXTEND_INDEX_BL(mlib_s16,
			dp2,
			y,
			x,
			h,
			w,
			y1,
			x1,
			h2,
			w3,
			VER);
	}
}

/* *********************************************************** */

void
mlib_ImageZoomIndexExtend_S16_Bilinear(
	mlib_work_image * param)
{
	VARIABLE_EDGE(mlib_s16);
	VARIABLE_SRC_EXTEND(mlib_s16);
	void *colormap = GetElemStruct(colormap);
	mlib_type colortype = mlib_ImageGetLutType(colormap);
	mlib_pack_func pack_func;
	mlib_s16 *tdpbuf = GetElemStruct(buffer_dp);

	channels = mlib_ImageGetLutChannels(colormap);

	if (colortype == MLIB_BYTE) {
		mlib_u8 *csp, *dpbuf = GetElemStruct(buffer_dp);
		mlib_f32 *ctable =
			(mlib_f32 *)mlib_ImageGetLutNormalTable(colormap) -
			mlib_ImageGetLutOffset(colormap);

		if (channels == 3)
			pack_func =
				(mlib_pack_func) &
				mlib_ImageColorTrue2IndexLine_U8_S16_3;
		else
			pack_func =
				(mlib_pack_func) &
				mlib_ImageColorTrue2IndexLine_U8_S16_4;

		MLIB_SRC_EXTEND_INDEX_BL(mlib_u8,
			dp0,
			x,
			y,
			w,
			h,
			x0,
			y0,
			w1,
			h1,
			HOR);
		MLIB_SRC_EXTEND_INDEX_BL(mlib_u8,
			dp3,
			x,
			y,
			w,
			h,
			x0,
			y2,
			w1,
			h3,
			HOR);
		MLIB_SRC_EXTEND_INDEX_BL(mlib_u8,
			dp1,
			y,
			x,
			h,
			w,
			y1,
			x0,
			h2,
			w2,
			VER);
		MLIB_SRC_EXTEND_INDEX_BL(mlib_u8,
			dp2,
			y,
			x,
			h,
			w,
			y1,
			x1,
			h2,
			w3,
			VER);
	} else {
		mlib_s16 *csp, *dpbuf = GetElemStruct(buffer_dp);
		mlib_d64 *ctable =
			(mlib_d64 *)mlib_ImageGetLutNormalTable(colormap) -
			mlib_ImageGetLutOffset(colormap);

		if (channels == 3)
			pack_func =
				(mlib_pack_func) &
				mlib_ImageColorTrue2IndexLine_S16_S16_3;
		else
			pack_func =
				(mlib_pack_func) &
				mlib_ImageColorTrue2IndexLine_S16_S16_4;

		MLIB_SRC_EXTEND_INDEX_BL(mlib_s16,
			dp0,
			x,
			y,
			w,
			h,
			x0,
			y0,
			w1,
			h1,
			HOR);
		MLIB_SRC_EXTEND_INDEX_BL(mlib_s16,
			dp3,
			x,
			y,
			w,
			h,
			x0,
			y2,
			w1,
			h3,
			HOR);
		MLIB_SRC_EXTEND_INDEX_BL(mlib_s16,
			dp1,
			y,
			x,
			h,
			w,
			y1,
			x0,
			h2,
			w2,
			VER);
		MLIB_SRC_EXTEND_INDEX_BL(mlib_s16,
			dp2,
			y,
			x,
			h,
			w,
			y1,
			x1,
			h2,
			w3,
			VER);
	}
}

/* *********************************************************** */

#define	MLIB_FILTER(X, Y, filter)                               \
	filterpos = ((Y) >> MLIB_XY_SHIFT) & MLIB_XY_MASK;      \
	fPtr = (mlib_f32 *)((mlib_u8 *)filter + filterpos);     \
	X##f0 = fPtr[0];                                        \
	X##f1 = fPtr[1];                                        \
	X##f2 = fPtr[2];                                        \
	X##f3 = fPtr[3]

/* *********************************************************** */

#define	BC_VAR(r, k, l)                                         \
	m1 = (k) >> MLIB_SIGN_SHIFT;                            \
	m2 = (l - (k)) >> MLIB_SIGN_SHIFT;                      \
	r = ((k) & ~(m1 | m2)) | (l & m2)

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

/* *********************************************************** */

#define	RES_U8_INDEX(dp)                                                  \
	(dp)[ch] = ((mlib_s32)(fx - (mlib_f32)0x7F800000) >> 24) ^ 0x80

/* *********************************************************** */

#define	RES_S16_INDEX(dp)	(dp)[ch] = (mlib_s32)fx >> MLIB_SHIFT

#else /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#define	RES_U8_INDEX(dp)                                        \
	{                                                       \
	    mlib_d64 sat_tmp = fx - (mlib_d64)0x7F800000;       \
	                                                        \
	    if (sat_tmp >= MLIB_S32_MAX)                        \
		sat_tmp = MLIB_S32_MAX;                         \
	    if (sat_tmp <= MLIB_S32_MIN)                        \
		sat_tmp = MLIB_S32_MIN;                         \
	    (dp)[ch] = ((mlib_s32)sat_tmp >> 24) ^ 0x80;        \
	}

/* *********************************************************** */

#define	RES_S16_INDEX(dp)                                       \
	{                                                       \
	    mlib_d64 sat_tmp = fx;                              \
	                                                        \
	    if (sat_tmp >= MLIB_S32_MAX)                        \
		sat_tmp = MLIB_S32_MAX;                         \
	    if (sat_tmp <= MLIB_S32_MIN)                        \
		sat_tmp = MLIB_S32_MIN;                         \
	    (dp)[ch] = (mlib_s32)sat_tmp >> MLIB_SHIFT;         \
	}

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#define	BC_CALC_INDEX(FMT, r, k)                                \
	csp = (FMT *) (ctable + sp[r * src_stride + c0]);       \
	a0 = csp[ch + 4 - channels];                            \
	k = a0 * xf0;                                           \
	csp = (FMT *) (ctable + sp[r * src_stride + c1]);       \
	a1 = csp[ch + 4 - channels];                            \
	k += a1 * xf1;                                          \
	csp = (FMT *) (ctable + sp[r * src_stride + c2]);       \
	a2 = csp[ch + 4 - channels];                            \
	k += a2 * xf2;                                          \
	csp = (FMT *) (ctable + sp[r * src_stride + c3]);       \
	a3 = csp[ch + 4 - channels];                            \
	k += a3 * xf3

/* *********************************************************** */

#define	MLIB_SRC_EXTEND_INDEX_BC(FMT, dp, x, y, w, h, rr, cc,             \
	xx, yy, ww, hh, filter, RES, MACRO)                               \
	{                                                                 \
	    mlib_s32 tdx, tdy, cx, cy;                                    \
	    mlib_f32 a0, a1, a2, a3, fx;                                  \
	                                                                  \
	    td##y = yy;                                                   \
	    for (j = 0; j < hh; j++) {                                    \
		td##x = xx;                                               \
		c##y = td##y >> MLIB_SHIFT;                               \
		MLIB_FILTER(y, td##y, filter);                            \
		BC_VAR(rr##0, c##y - 1, s##h);                            \
	/*                                                                \
	 * the next three lines overcome SC5.0 compile bug on x86         \
	 */                                                               \
		m1 = c##y >> (MLIB_SIGN_SHIFT - MLIB_SHIFT);              \
		m2 = (s##h - c##y) >> MLIB_SIGN_SHIFT;                    \
		rr##1 = (c##y & ~(m1 | m2)) | (s##h & m2);                \
		BC_VAR(rr##2, c##y + 1, s##h);                            \
		BC_VAR(rr##3, c##y + 2, s##h);                            \
		for (i = 0; i < ww; i++) {                                \
		    c##x = td##x >> MLIB_SHIFT;                           \
		    MLIB_FILTER(x, td##x, filter);                        \
		    BC_VAR(cc##0, c##x - 1, s##w);                        \
									  \
/*	the next three lines overcome SC5.0 compile bug on x86 */	  \
									  \
		    m1 = c##x >> (MLIB_SIGN_SHIFT - MLIB_SHIFT);          \
		    m2 = (s##w - c##x) >> MLIB_SIGN_SHIFT;                \
		    cc##1 =                                               \
			    (c##x & ~(m1 | m2)) | (s##w & m2);            \
		    BC_VAR(cc##2, c##x + 1, s##w);                        \
		    BC_VAR(cc##3, c##x + 2, s##w);                        \
		    for (ch = 0; ch < channels; ch++) {                   \
			BC_CALC_INDEX(FMT, r0, k0);                       \
			BC_CALC_INDEX(FMT, r1, k1);                       \
			BC_CALC_INDEX(FMT, r2, k2);                       \
			BC_CALC_INDEX(FMT, r3, k3);                       \
			fx = k0 * yf0 + k1 * yf1 + k2 * yf2 + k3 * yf3;   \
			RES(dpbuf + i * channels);                        \
		    }                                                     \
		    td##x += d##x;                                        \
		}                                                         \
		MACRO(ww, dp);                                            \
		td##y += d##y;                                            \
	    }                                                             \
	}

/* *********************************************************** */

#define	VARIABLE_SRC_EXTEND_BC                                            \
	mlib_s32 m1, m2, r0, r1, r2, r3, c0, c1, c2, c3, filterpos;       \
	mlib_f32 *fPtr;                                                   \
	mlib_f32 k0, k1, k2, k3, xf0, xf1, xf2, xf3, yf0, yf1, yf2, yf3

/* *********************************************************** */

void
mlib_ImageZoomIndexExtend_U8_Bicubic(
	mlib_work_image * param)
{
	VARIABLE_EDGE(mlib_u8);
	VARIABLE_SRC_EXTEND(mlib_u8);

	VARIABLE_SRC_EXTEND_BC;
	void *colormap = GetElemStruct(colormap);
	mlib_type colortype = mlib_ImageGetLutType(colormap);
	mlib_pack_func pack_func;
	mlib_u8 *tdpbuf = GetElemStruct(buffer_dp);

	channels = mlib_ImageGetLutChannels(colormap);

	if (colortype == MLIB_BYTE) {
		mlib_u8 *csp, *dpbuf = GetElemStruct(buffer_dp);
		mlib_f32 *ctable =
			(mlib_f32 *)mlib_ImageGetLutNormalTable(colormap) -
			mlib_ImageGetLutOffset(colormap);

		if (channels == 3)
			pack_func =
				(mlib_pack_func) &
				mlib_ImageColorTrue2IndexLine_U8_U8_3;
		else
			pack_func =
				(mlib_pack_func) &
				mlib_ImageColorTrue2IndexLine_U8_U8_4;

/* *********************************************************** */

#define	MLIB_XY_SHIFT	4
#define	MLIB_XY_MASK	(((1 << 8) - 1) << 4)

		MLIB_SRC_EXTEND_INDEX_BC(mlib_u8,
			dp0,
			x,
			y,
			w,
			h,
			r,
			c,
			x0,
			y0,
			w1,
			h1,
			mlib_filters_u8f_bc,
			RES_U8_INDEX,
			HOR);
		MLIB_SRC_EXTEND_INDEX_BC(mlib_u8,
			dp3,
			x,
			y,
			w,
			h,
			r,
			c,
			x0,
			y2,
			w1,
			h3,
			mlib_filters_u8f_bc,
			RES_U8_INDEX,
			HOR);
		MLIB_SRC_EXTEND_INDEX_BC(mlib_u8,
			dp1,
			y,
			x,
			h,
			w,
			c,
			r,
			y1,
			x0,
			h2,
			w2,
			mlib_filters_u8f_bc,
			RES_U8_INDEX,
			VER);
		MLIB_SRC_EXTEND_INDEX_BC(mlib_u8,
			dp2,
			y,
			x,
			h,
			w,
			c,
			r,
			y1,
			x1,
			h2,
			w3,
			mlib_filters_u8f_bc,
			RES_U8_INDEX,
			VER);
	} else {
		mlib_s16 *csp, *dpbuf = GetElemStruct(buffer_dp);
		mlib_d64 *ctable =
			(mlib_d64 *)mlib_ImageGetLutNormalTable(colormap) -
			mlib_ImageGetLutOffset(colormap);

		if (channels == 3)
			pack_func =
				(mlib_pack_func) &
				mlib_ImageColorTrue2IndexLine_S16_U8_3;
		else
			pack_func =
				(mlib_pack_func) &
				mlib_ImageColorTrue2IndexLine_S16_U8_4;

/* *********************************************************** */

#if defined(MLIB_XY_SHIFT)
#undef MLIB_XY_SHIFT
#define	MLIB_XY_SHIFT	3
#endif /* defined(MLIB_XY_SHIFT) */

/* *********************************************************** */

#if defined(MLIB_XY_MASK)
#undef MLIB_XY_MASK
#define	MLIB_XY_MASK	(((1 << 9) - 1) << 4)
#endif /* defined(MLIB_XY_MASK) */

		MLIB_SRC_EXTEND_INDEX_BC(mlib_s16,
			dp0,
			x,
			y,
			w,
			h,
			r,
			c,
			x0,
			y0,
			w1,
			h1,
			mlib_filters_s16f_bc,
			RES_S16_INDEX,
			HOR);
		MLIB_SRC_EXTEND_INDEX_BC(mlib_s16,
			dp3,
			x,
			y,
			w,
			h,
			r,
			c,
			x0,
			y2,
			w1,
			h3,
			mlib_filters_s16f_bc,
			RES_S16_INDEX,
			HOR);
		MLIB_SRC_EXTEND_INDEX_BC(mlib_s16,
			dp1,
			y,
			x,
			h,
			w,
			c,
			r,
			y1,
			x0,
			h2,
			w2,
			mlib_filters_s16f_bc,
			RES_S16_INDEX,
			VER);
		MLIB_SRC_EXTEND_INDEX_BC(mlib_s16,
			dp2,
			y,
			x,
			h,
			w,
			c,
			r,
			y1,
			x1,
			h2,
			w3,
			mlib_filters_s16f_bc,
			RES_S16_INDEX,
			VER);
	}
}

/* *********************************************************** */

void
mlib_ImageZoomIndexExtend_S16_Bicubic(
	mlib_work_image * param)
{
	VARIABLE_EDGE(mlib_s16);
	VARIABLE_SRC_EXTEND(mlib_s16);

	VARIABLE_SRC_EXTEND_BC;
	void *colormap = GetElemStruct(colormap);
	mlib_type colortype = mlib_ImageGetLutType(colormap);
	mlib_pack_func pack_func;
	mlib_s16 *tdpbuf = GetElemStruct(buffer_dp);

	channels = mlib_ImageGetLutChannels(colormap);

	if (colortype == MLIB_BYTE) {
		mlib_u8 *csp, *dpbuf = GetElemStruct(buffer_dp);
		mlib_f32 *ctable =
			(mlib_f32 *)mlib_ImageGetLutNormalTable(colormap) -
			mlib_ImageGetLutOffset(colormap);

		if (channels == 3)
			pack_func =
				(mlib_pack_func) &
				mlib_ImageColorTrue2IndexLine_U8_S16_3;
		else
			pack_func =
				(mlib_pack_func) &
				mlib_ImageColorTrue2IndexLine_U8_S16_4;

/* *********************************************************** */

#if defined(MLIB_XY_SHIFT)
#undef MLIB_XY_SHIFT
#define	MLIB_XY_SHIFT	4
#endif /* defined(MLIB_XY_SHIFT) */

/* *********************************************************** */

#if defined(MLIB_XY_MASK)
#undef MLIB_XY_MASK
#define	MLIB_XY_MASK	(((1 << 8) - 1) << 4)
#endif /* defined(MLIB_XY_MASK) */

		MLIB_SRC_EXTEND_INDEX_BC(mlib_u8,
			dp0,
			x,
			y,
			w,
			h,
			r,
			c,
			x0,
			y0,
			w1,
			h1,
			mlib_filters_u8f_bc,
			RES_U8_INDEX,
			HOR);
		MLIB_SRC_EXTEND_INDEX_BC(mlib_u8,
			dp3,
			x,
			y,
			w,
			h,
			r,
			c,
			x0,
			y2,
			w1,
			h3,
			mlib_filters_u8f_bc,
			RES_U8_INDEX,
			HOR);
		MLIB_SRC_EXTEND_INDEX_BC(mlib_u8,
			dp1,
			y,
			x,
			h,
			w,
			c,
			r,
			y1,
			x0,
			h2,
			w2,
			mlib_filters_u8f_bc,
			RES_U8_INDEX,
			VER);
		MLIB_SRC_EXTEND_INDEX_BC(mlib_u8,
			dp2,
			y,
			x,
			h,
			w,
			c,
			r,
			y1,
			x1,
			h2,
			w3,
			mlib_filters_u8f_bc,
			RES_U8_INDEX,
			VER);
	} else {
		mlib_s16 *csp, *dpbuf = GetElemStruct(buffer_dp);
		mlib_d64 *ctable =
			(mlib_d64 *)mlib_ImageGetLutNormalTable(colormap) -
			mlib_ImageGetLutOffset(colormap);

		if (channels == 3)
			pack_func =
				(mlib_pack_func) &
				mlib_ImageColorTrue2IndexLine_S16_S16_3;
		else
			pack_func =
				(mlib_pack_func) &
				mlib_ImageColorTrue2IndexLine_S16_S16_4;

/* *********************************************************** */

#if defined(MLIB_XY_SHIFT)
#undef MLIB_XY_SHIFT
#define	MLIB_XY_SHIFT	3
#endif /* defined(MLIB_XY_SHIFT) */

/* *********************************************************** */

#if defined(MLIB_XY_MASK)
#undef MLIB_XY_MASK
#define	MLIB_XY_MASK	(((1 << 9) - 1) << 4)
#endif /* defined(MLIB_XY_MASK) */

		MLIB_SRC_EXTEND_INDEX_BC(mlib_s16,
			dp0,
			x,
			y,
			w,
			h,
			r,
			c,
			x0,
			y0,
			w1,
			h1,
			mlib_filters_s16f_bc,
			RES_S16_INDEX,
			HOR);
		MLIB_SRC_EXTEND_INDEX_BC(mlib_s16,
			dp3,
			x,
			y,
			w,
			h,
			r,
			c,
			x0,
			y2,
			w1,
			h3,
			mlib_filters_s16f_bc,
			RES_S16_INDEX,
			HOR);
		MLIB_SRC_EXTEND_INDEX_BC(mlib_s16,
			dp1,
			y,
			x,
			h,
			w,
			c,
			r,
			y1,
			x0,
			h2,
			w2,
			mlib_filters_s16f_bc,
			RES_S16_INDEX,
			VER);
		MLIB_SRC_EXTEND_INDEX_BC(mlib_s16,
			dp2,
			y,
			x,
			h,
			w,
			c,
			r,
			y1,
			x1,
			h2,
			w3,
			mlib_filters_s16f_bc,
			RES_S16_INDEX,
			VER);
	}
}

/* *********************************************************** */

void
mlib_ImageZoomIndexExtend_U8_Bicubic2(
	mlib_work_image * param)
{
	VARIABLE_EDGE(mlib_u8);
	VARIABLE_SRC_EXTEND(mlib_u8);

	VARIABLE_SRC_EXTEND_BC;
	void *colormap = GetElemStruct(colormap);
	mlib_type colortype = mlib_ImageGetLutType(colormap);
	mlib_pack_func pack_func;
	mlib_u8 *tdpbuf = GetElemStruct(buffer_dp);

	channels = mlib_ImageGetLutChannels(colormap);

	if (colortype == MLIB_BYTE) {
		mlib_u8 *csp, *dpbuf = GetElemStruct(buffer_dp);
		mlib_f32 *ctable =
			(mlib_f32 *)mlib_ImageGetLutNormalTable(colormap) -
			mlib_ImageGetLutOffset(colormap);

		if (channels == 3)
			pack_func =
				(mlib_pack_func) &
				mlib_ImageColorTrue2IndexLine_U8_U8_3;
		else
			pack_func =
				(mlib_pack_func) &
				mlib_ImageColorTrue2IndexLine_U8_U8_4;

/* *********************************************************** */

#if defined(MLIB_XY_SHIFT)
#undef MLIB_XY_SHIFT
#define	MLIB_XY_SHIFT	4
#endif /* defined(MLIB_XY_SHIFT) */

/* *********************************************************** */

#if defined(MLIB_XY_MASK)
#undef MLIB_XY_MASK
#define	MLIB_XY_MASK	(((1 << 8) - 1) << 4)
#endif /* defined(MLIB_XY_MASK) */

		MLIB_SRC_EXTEND_INDEX_BC(mlib_u8,
			dp0,
			x,
			y,
			w,
			h,
			r,
			c,
			x0,
			y0,
			w1,
			h1,
			mlib_filters_u8f_bc2,
			RES_U8_INDEX,
			HOR);
		MLIB_SRC_EXTEND_INDEX_BC(mlib_u8,
			dp3,
			x,
			y,
			w,
			h,
			r,
			c,
			x0,
			y2,
			w1,
			h3,
			mlib_filters_u8f_bc2,
			RES_U8_INDEX,
			HOR);
		MLIB_SRC_EXTEND_INDEX_BC(mlib_u8,
			dp1,
			y,
			x,
			h,
			w,
			c,
			r,
			y1,
			x0,
			h2,
			w2,
			mlib_filters_u8f_bc2,
			RES_U8_INDEX,
			VER);
		MLIB_SRC_EXTEND_INDEX_BC(mlib_u8,
			dp2,
			y,
			x,
			h,
			w,
			c,
			r,
			y1,
			x1,
			h2,
			w3,
			mlib_filters_u8f_bc2,
			RES_U8_INDEX,
			VER);
	} else {
		mlib_s16 *csp, *dpbuf = GetElemStruct(buffer_dp);
		mlib_d64 *ctable =
			(mlib_d64 *)mlib_ImageGetLutNormalTable(colormap) -
			mlib_ImageGetLutOffset(colormap);

		if (channels == 3)
			pack_func =
				(mlib_pack_func) &
				mlib_ImageColorTrue2IndexLine_S16_U8_3;
		else
			pack_func =
				(mlib_pack_func) &
				mlib_ImageColorTrue2IndexLine_S16_U8_4;

/* *********************************************************** */

#if defined(MLIB_XY_SHIFT)
#undef MLIB_XY_SHIFT
#define	MLIB_XY_SHIFT	3
#endif /* defined(MLIB_XY_SHIFT) */

/* *********************************************************** */

#if defined(MLIB_XY_MASK)
#undef MLIB_XY_MASK
#define	MLIB_XY_MASK	(((1 << 9) - 1) << 4)
#endif /* defined(MLIB_XY_MASK) */

		MLIB_SRC_EXTEND_INDEX_BC(mlib_s16,
			dp0,
			x,
			y,
			w,
			h,
			r,
			c,
			x0,
			y0,
			w1,
			h1,
			mlib_filters_s16f_bc2,
			RES_S16_INDEX,
			HOR);
		MLIB_SRC_EXTEND_INDEX_BC(mlib_s16,
			dp3,
			x,
			y,
			w,
			h,
			r,
			c,
			x0,
			y2,
			w1,
			h3,
			mlib_filters_s16f_bc2,
			RES_S16_INDEX,
			HOR);
		MLIB_SRC_EXTEND_INDEX_BC(mlib_s16,
			dp1,
			y,
			x,
			h,
			w,
			c,
			r,
			y1,
			x0,
			h2,
			w2,
			mlib_filters_s16f_bc2,
			RES_S16_INDEX,
			VER);
		MLIB_SRC_EXTEND_INDEX_BC(mlib_s16,
			dp2,
			y,
			x,
			h,
			w,
			c,
			r,
			y1,
			x1,
			h2,
			w3,
			mlib_filters_s16f_bc2,
			RES_S16_INDEX,
			VER);
	}
}

/* *********************************************************** */

void
mlib_ImageZoomIndexExtend_S16_Bicubic2(
	mlib_work_image * param)
{
	VARIABLE_EDGE(mlib_s16);
	VARIABLE_SRC_EXTEND(mlib_s16);

	VARIABLE_SRC_EXTEND_BC;
	void *colormap = GetElemStruct(colormap);
	mlib_type colortype = mlib_ImageGetLutType(colormap);
	mlib_pack_func pack_func;
	mlib_s16 *tdpbuf = GetElemStruct(buffer_dp);

	channels = mlib_ImageGetLutChannels(colormap);

	if (colortype == MLIB_BYTE) {
		mlib_u8 *csp, *dpbuf = GetElemStruct(buffer_dp);
		mlib_f32 *ctable =
			(mlib_f32 *)mlib_ImageGetLutNormalTable(colormap) -
			mlib_ImageGetLutOffset(colormap);

		if (channels == 3)
			pack_func =
				(mlib_pack_func) &
				mlib_ImageColorTrue2IndexLine_U8_S16_3;
		else
			pack_func =
				(mlib_pack_func) &
				mlib_ImageColorTrue2IndexLine_U8_S16_4;

/* *********************************************************** */

#if defined(MLIB_XY_SHIFT)
#undef MLIB_XY_SHIFT
#define	MLIB_XY_SHIFT	4
#endif /* defined(MLIB_XY_SHIFT) */

/* *********************************************************** */

#if defined(MLIB_XY_MASK)
#undef MLIB_XY_MASK
#define	MLIB_XY_MASK	(((1 << 8) - 1) << 4)
#endif /* defined(MLIB_XY_MASK) */

		MLIB_SRC_EXTEND_INDEX_BC(mlib_u8,
			dp0,
			x,
			y,
			w,
			h,
			r,
			c,
			x0,
			y0,
			w1,
			h1,
			mlib_filters_u8f_bc2,
			RES_U8_INDEX,
			HOR);
		MLIB_SRC_EXTEND_INDEX_BC(mlib_u8,
			dp3,
			x,
			y,
			w,
			h,
			r,
			c,
			x0,
			y2,
			w1,
			h3,
			mlib_filters_u8f_bc2,
			RES_U8_INDEX,
			HOR);
		MLIB_SRC_EXTEND_INDEX_BC(mlib_u8,
			dp1,
			y,
			x,
			h,
			w,
			c,
			r,
			y1,
			x0,
			h2,
			w2,
			mlib_filters_u8f_bc2,
			RES_U8_INDEX,
			VER);
		MLIB_SRC_EXTEND_INDEX_BC(mlib_u8,
			dp2,
			y,
			x,
			h,
			w,
			c,
			r,
			y1,
			x1,
			h2,
			w3,
			mlib_filters_u8f_bc2,
			RES_U8_INDEX,
			VER);
	} else {
		mlib_s16 *csp, *dpbuf = GetElemStruct(buffer_dp);
		mlib_d64 *ctable =
			(mlib_d64 *)mlib_ImageGetLutNormalTable(colormap) -
			mlib_ImageGetLutOffset(colormap);

		if (channels == 3)
			pack_func =
				(mlib_pack_func) &
				mlib_ImageColorTrue2IndexLine_S16_S16_3;
		else
			pack_func =
				(mlib_pack_func) &
				mlib_ImageColorTrue2IndexLine_S16_S16_4;

/* *********************************************************** */

#if defined(MLIB_XY_SHIFT)
#undef MLIB_XY_SHIFT
#define	MLIB_XY_SHIFT	3
#endif /* defined(MLIB_XY_SHIFT) */

/* *********************************************************** */

#if defined(MLIB_XY_MASK)
#undef MLIB_XY_MASK
#define	MLIB_XY_MASK	(((1 << 9) - 1) << 4)
#endif /* defined(MLIB_XY_MASK) */

		MLIB_SRC_EXTEND_INDEX_BC(mlib_s16,
			dp0,
			x,
			y,
			w,
			h,
			r,
			c,
			x0,
			y0,
			w1,
			h1,
			mlib_filters_s16f_bc2,
			RES_S16_INDEX,
			HOR);
		MLIB_SRC_EXTEND_INDEX_BC(mlib_s16,
			dp3,
			x,
			y,
			w,
			h,
			r,
			c,
			x0,
			y2,
			w1,
			h3,
			mlib_filters_s16f_bc2,
			RES_S16_INDEX,
			HOR);
		MLIB_SRC_EXTEND_INDEX_BC(mlib_s16,
			dp1,
			y,
			x,
			h,
			w,
			c,
			r,
			y1,
			x0,
			h2,
			w2,
			mlib_filters_s16f_bc2,
			RES_S16_INDEX,
			VER);
		MLIB_SRC_EXTEND_INDEX_BC(mlib_s16,
			dp2,
			y,
			x,
			h,
			w,
			c,
			r,
			y1,
			x1,
			h2,
			w3,
			mlib_filters_s16f_bc2,
			RES_S16_INDEX,
			VER);
	}
}

/* *********************************************************** */
