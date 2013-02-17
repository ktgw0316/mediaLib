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

#pragma ident	"@(#)mlib_s_ImageZoomTransTable_8nw.c	9.9	07/11/05 SMI"

/*
 * FUNCTION
 *      Internal functions for mlib_ImageZoomTranslateTable.
 */

#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

#include <mlib_image.h>
#include <mlib_ImageInterpTable.h>
#include <mlib_s_ImageZoom.h>
#include <mlib_ImageZoomTranslateTable.h>

/* *********************************************************** */

#define	MALLOC_EXTRA	4*8

#define	MALLOC(size)	__mlib_malloc(size + MALLOC_EXTRA)

/* *********************************************************** */

#define	FLT_BITS_X	1
#define	FLT_BITS_Y	3

/* *********************************************************** */

#define	DECLAREVAR                                              \
	__m128i *ybuff;                                         \
	__m128i dsat;                                           \
	mlib_s32 shift, y, x, xoff;                             \
	mlib_s32 xLeft, xRight, x0, y0;                         \
	mlib_s32 ySrc;                                          \
	mlib_s32 yStart = ws->yStart;                           \
	mlib_s32 yFinish = ws->yFinish;                         \
	mlib_s32 max_xsize = ws->max_xsize;                     \
	mlib_s32 dstYStride = ws->dstStride;                    \
	mlib_s32 x_shift = ws->x_shift;                         \
	mlib_s32 y_shift = ws->y_shift;                         \
	mlib_s32 filterpos;                                     \
	mlib_s32 size;                                          \
	mlib_u8 *dl;                                            \
	mlib_s32 xf_shift, xf_mask, yf_shift, yf_mask;          \
	mlib_s32 m = table->width;                              \
	mlib_s32 n = table->height;                             \
	mlib_s32 xf_bits = table->width_bits;                   \
	mlib_s32 yf_bits = table->height_bits;                  \
	mlib_s32 subsampleBitsH = table->subsampleBitsH;        \
	mlib_s32 subsampleBitsV = table->subsampleBitsV;        \
	mlib_d64 *filterX, *filterY, *fptr;                     \
	mlib_s32 i, j, l, kw, off

/* *********************************************************** */

#define	CLIP(N)                                                 \
	dstData += dstYStride;                                  \
	xLeft = leftEdges[j];                                   \
	xRight = rightEdges[j];                                 \
	size = xRight - xLeft + 1;                              \
	x0 = xStarts[j];                                        \
	y0 = yStarts[j];                                        \
	if (xLeft > xRight)                                     \
	    continue;                                           \
	dl = dstData + N * xLeft

/* *********************************************************** */

#define	DIST_BITS()                                                    \
	xf_shift = x_shift - subsampleBitsH - xf_bits - FLT_BITS_X;    \
	xf_mask =                                                      \
	    (((1 << subsampleBitsH) - 1) << (xf_bits + FLT_BITS_X));   \
	yf_shift = y_shift - subsampleBitsV - yf_bits - FLT_BITS_Y;    \
	yf_mask =                                                      \
	    (((1 << subsampleBitsV) - 1) << (yf_bits + FLT_BITS_Y))

/* *********************************************************** */

#define	MLIB_FILTER(X, filter, l)                               \
	filterpos = ((X) >> X##f_shift) & X##f_mask;            \
	fptr = (void *)((mlib_u8 *)filter + filterpos + l)

/* *********************************************************** */

#define	MUL_ALPHA_4CH(sp, dp, bmask)                                   \
	{                                                              \
	    __m128i aa, res;                                           \
	                                                               \
	    res = _mm_loadl_epi64((void *)((mlib_f32 *)sp + 2 * i));   \
	    res = _mm_unpacklo_epi8(mzero, res);                       \
	    aa = _mm_shufflelo_epi16(res, bmask);                      \
	    aa = _mm_shufflehi_epi16(aa, bmask);                       \
	    aa = _mm_or_si128(aa, amask);                              \
	    aa = _mm_mulhi_epu16(aa, scale255);                        \
	    res = _mm_mulhi_epu16(res, aa);                            \
	    dp[i] = res;                                               \
	}

/* *********************************************************** */

#define	MUL_HALF(sp, dp)                                               \
	{                                                              \
	    __m128i res;                                               \
	                                                               \
	    res = _mm_loadl_epi64((void *)((mlib_f32 *)sp + 2 * i));   \
	    res = _mm_unpacklo_epi8(mzero, res);                       \
	    res = _mm_srli_epi16(res, 1);                              \
	    dp[i] = res;                                               \
	}

/* *********************************************************** */

#undef  VPARAMS
#define	VPARAMS                                                 \
	mlib_d64 *fptr, __m128i ** buff_arr, mlib_s32 bnd,      \
	    __m128i * ybuff, mlib_s32 size

/* *********************************************************** */

#define	READ_FILTER(ptr)	_mm_set1_epi16(*(mlib_s16*)(ptr))

/* *********************************************************** */

#undef VPARAMH
#define	VPARAMH                                                    \
	mlib_s32 x, mlib_d64 *filterX, mlib_zoom_workspace * ws,   \
	    __m128i * pbuff, mlib_d64 *sp

/* *********************************************************** */

#define	DECL_LINE                                               \
	mlib_s32 i;                                             \
	mlib_s32 xSrc;                                          \
	mlib_s32 dx = ws->dx;                                   \
	mlib_s32 size = ws->size;                               \
	mlib_s32 x_shift = ws->x_shift;                         \
	mlib_s32 xf_shift = ws->xf_shift;                       \
	mlib_s32 xf_mask = ws->xf_mask;                         \
	mlib_s32 filterpos;                                     \
	mlib_s32 xoff = ws->off;                                \
	mlib_s16 *fptr

/* *********************************************************** */

static void
mlib_s_calc_h1_1ch(
    VPARAMH)
{
	DECL_LINE;
	void *buff = (void *)pbuff;
	mlib_s16 *sp1 = (void *)sp;
	mlib_s32 flt_i;
	__m128i bb, d0, xff;
	__m128i s0, s1, s2, s3, ss;
	__m128i f0, f1, f2, f3, ff;

	for (i = 0; i < (size + 3) / 4; i++) {
		MLIB_FILTER(x, filterX, xoff);
		f0 = _mm_loadl_epi64((void *)fptr);
		s0 = _mm_loadl_epi64((void *)(sp1 + (x >> x_shift)));
		x += dx;
		MLIB_FILTER(x, filterX, xoff);
		f1 = _mm_loadl_epi64((void *)fptr);
		s1 = _mm_loadl_epi64((void *)(sp1 + (x >> x_shift)));
		x += dx;
		MLIB_FILTER(x, filterX, xoff);
		f2 = _mm_loadl_epi64((void *)fptr);
		s2 = _mm_loadl_epi64((void *)(sp1 + (x >> x_shift)));
		x += dx;
		MLIB_FILTER(x, filterX, xoff);
		f3 = _mm_loadl_epi64((void *)fptr);
		s3 = _mm_loadl_epi64((void *)(sp1 + (x >> x_shift)));
		x += dx;

		ff = _mm_unpacklo_epi32(_mm_unpacklo_epi16(f0, f1),
		    _mm_unpacklo_epi16(f2, f3));
		ss = _mm_unpacklo_epi32(_mm_unpacklo_epi16(s0, s1),
		    _mm_unpacklo_epi16(s2, s3));

		bb = _mm_loadl_epi64(buff);
		d0 = _mm_mulhi_epi16(ss, ff);
		d0 = _mm_add_epi16(bb, d0);
		_mm_storel_epi64(buff, d0);
		buff = (mlib_s16 *)buff + 4;
	}
}

/* *********************************************************** */

static void
mlib_s_calc_h2_1ch(
    VPARAMH)
{
	DECL_LINE;
	void *buff = (void *)pbuff;
	mlib_s16 *sp1 = (void *)sp;
	mlib_s32 flt_i;
	__m128i bb, dd, d1;
	__m128i s0, s1, s2, s3, ss;
	__m128i f0, f1, f2, f3, ff;

	for (i = 0; i < (size + 3) / 4; i++) {
		MLIB_FILTER(x, filterX, xoff);
		f0 = _mm_loadl_epi64((void *)fptr);
		s0 = _mm_loadl_epi64((void *)(sp1 + (x >> x_shift)));
		x += dx;
		MLIB_FILTER(x, filterX, xoff);
		f1 = _mm_loadl_epi64((void *)fptr);
		s1 = _mm_loadl_epi64((void *)(sp1 + (x >> x_shift)));
		x += dx;
		MLIB_FILTER(x, filterX, xoff);
		f2 = _mm_loadl_epi64((void *)fptr);
		s2 = _mm_loadl_epi64((void *)(sp1 + (x >> x_shift)));
		x += dx;
		MLIB_FILTER(x, filterX, xoff);
		f3 = _mm_loadl_epi64((void *)fptr);
		s3 = _mm_loadl_epi64((void *)(sp1 + (x >> x_shift)));
		x += dx;

		ff = _mm_unpacklo_epi32(_mm_unpacklo_epi16(f0, f1),
		    _mm_unpacklo_epi16(f2, f3));
		ss = _mm_unpacklo_epi32(_mm_unpacklo_epi16(s0, s1),
		    _mm_unpacklo_epi16(s2, s3));

		bb = _mm_loadl_epi64(buff);
		dd = _mm_mulhi_epi16(ss, ff);
		d1 = _mm_unpackhi_epi64(dd, dd);
		dd = _mm_add_epi16(dd, d1);
		dd = _mm_add_epi16(dd, bb);
		_mm_storel_epi64(buff, dd);
		buff = (mlib_s16 *)buff + 4;
	}
}

/* *********************************************************** */

static void
mlib_s_calc_h4_1ch(
    VPARAMH)
{
	DECL_LINE;
	void *buff = (void *)pbuff;
	mlib_s16 *sp1 = (void *)sp;
	mlib_s32 flt_i;
	__m128i bb, dd, d0, d1, t0, t1;
	__m128i s0, s1, s2, s3, ss;
	__m128i f0, f1, f2, f3, ff;

	for (i = 0; i < (size + 3) / 4; i++) {
		MLIB_FILTER(x, filterX, xoff);
		f0 = _mm_loadl_epi64((void *)fptr);
		s0 = _mm_loadl_epi64((void *)(sp1 + (x >> x_shift)));
		x += dx;
		MLIB_FILTER(x, filterX, xoff);
		f1 = _mm_loadl_epi64((void *)fptr);
		s1 = _mm_loadl_epi64((void *)(sp1 + (x >> x_shift)));
		x += dx;
		MLIB_FILTER(x, filterX, xoff);
		f2 = _mm_loadl_epi64((void *)fptr);
		s2 = _mm_loadl_epi64((void *)(sp1 + (x >> x_shift)));
		x += dx;
		MLIB_FILTER(x, filterX, xoff);
		f3 = _mm_loadl_epi64((void *)fptr);
		s3 = _mm_loadl_epi64((void *)(sp1 + (x >> x_shift)));
		x += dx;

		t0 = _mm_unpacklo_epi16(f0, f1);
		t1 = _mm_unpacklo_epi16(f2, f3);
		f0 = _mm_unpacklo_epi32(t0, t1);
		f1 = _mm_unpackhi_epi32(t0, t1);

		t0 = _mm_unpacklo_epi16(s0, s1);
		t1 = _mm_unpacklo_epi16(s2, s3);
		s0 = _mm_unpacklo_epi32(t0, t1);
		s1 = _mm_unpackhi_epi32(t0, t1);

		bb = _mm_loadl_epi64(buff);
		d0 = _mm_mulhi_epi16(s0, f0);
		d1 = _mm_mulhi_epi16(s1, f1);
		dd = _mm_add_epi16(d0, d1);
		d1 = _mm_unpackhi_epi64(dd, dd);
		dd = _mm_add_epi16(dd, d1);
		dd = _mm_add_epi16(dd, bb);
		_mm_storel_epi64(buff, dd);
		buff = (mlib_s16 *)buff + 4;
	}
}

/* *********************************************************** */

static void
mlib_s_calc_h1_2ch(
    VPARAMH)
{
	DECL_LINE;
	mlib_s16 *sp1 = (void *)sp;
	__m128i bb, dd;
	__m128i s0, s1, s2, s3, ss;
	__m128i f0, f1, f2, f3, ff;

	for (i = 0; i < (size + 3) / 4; i++) {
		MLIB_FILTER(x, filterX, xoff);
		f0 = _mm_loadl_epi64((void *)fptr);
		s0 = _mm_loadl_epi64((void *)(sp1 + 2 * (x >> x_shift)));
		x += dx;
		MLIB_FILTER(x, filterX, xoff);
		f1 = _mm_loadl_epi64((void *)fptr);
		s1 = _mm_loadl_epi64((void *)(sp1 + 2 * (x >> x_shift)));
		x += dx;
		MLIB_FILTER(x, filterX, xoff);
		f2 = _mm_loadl_epi64((void *)fptr);
		s2 = _mm_loadl_epi64((void *)(sp1 + 2 * (x >> x_shift)));
		x += dx;
		MLIB_FILTER(x, filterX, xoff);
		f3 = _mm_loadl_epi64((void *)fptr);
		s3 = _mm_loadl_epi64((void *)(sp1 + 2 * (x >> x_shift)));
		x += dx;

		ff = _mm_unpacklo_epi32(_mm_unpacklo_epi16(f0, f1),
		    _mm_unpacklo_epi16(f2, f3));
		ff = _mm_unpacklo_epi16(ff, ff);
		ss = _mm_unpacklo_epi64(_mm_unpacklo_epi32(s0, s1),
		    _mm_unpacklo_epi32(s2, s3));

		bb = pbuff[i];
		dd = _mm_mulhi_epi16(ss, ff);
		dd = _mm_add_epi16(dd, bb);
		pbuff[i] = dd;
	}
}

/* *********************************************************** */

static void
mlib_s_calc_h2_2ch(
    VPARAMH)
{
	DECL_LINE;
	mlib_s16 *sp1 = (void *)sp;
	__m128i bb, dd, d0, d1, t0, t1;
	__m128i s0, s1, s2, s3, ss;
	__m128i f0, f1, f2, f3, ff;

	for (i = 0; i < (size + 3) / 4; i++) {
		MLIB_FILTER(x, filterX, xoff);
		f0 = _mm_loadl_epi64((void *)fptr);
		s0 = _mm_loadl_epi64((void *)(sp1 + 2 * (x >> x_shift)));
		x += dx;
		MLIB_FILTER(x, filterX, xoff);
		f1 = _mm_loadl_epi64((void *)fptr);
		s1 = _mm_loadl_epi64((void *)(sp1 + 2 * (x >> x_shift)));
		x += dx;
		MLIB_FILTER(x, filterX, xoff);
		f2 = _mm_loadl_epi64((void *)fptr);
		s2 = _mm_loadl_epi64((void *)(sp1 + 2 * (x >> x_shift)));
		x += dx;
		MLIB_FILTER(x, filterX, xoff);
		f3 = _mm_loadl_epi64((void *)fptr);
		s3 = _mm_loadl_epi64((void *)(sp1 + 2 * (x >> x_shift)));
		x += dx;

		ff = _mm_unpacklo_epi32(_mm_unpacklo_epi16(f0, f1),
		    _mm_unpacklo_epi16(f2, f3));
		f0 = _mm_unpacklo_epi16(ff, ff);
		f1 = _mm_unpackhi_epi16(ff, ff);
		t0 = _mm_unpacklo_epi32(s0, s1);
		t1 = _mm_unpacklo_epi32(s2, s3);
		s0 = _mm_unpacklo_epi64(t0, t1);
		s1 = _mm_unpackhi_epi64(t0, t1);

		bb = pbuff[i];
		d0 = _mm_mulhi_epi16(s0, f0);
		d1 = _mm_mulhi_epi16(s1, f1);
		dd = _mm_add_epi16(d0, d1);
		dd = _mm_add_epi16(dd, bb);
		pbuff[i] = dd;
	}
}

/* *********************************************************** */

static void
mlib_s_calc_h1_4ch(
    VPARAMH)
{
	DECL_LINE;
	mlib_s32 flt_i;
	__m128i bb, d0, xff, s0, s1, ss;

	for (i = 0; i < (size + 1) / 2; i++) {
		MLIB_FILTER(x, filterX, xoff);
		flt_i = fptr[0];
		s0 = _mm_loadl_epi64((void *)(sp + (x >> x_shift)));
		x += dx;

		MLIB_FILTER(x, filterX, xoff);
		flt_i = (flt_i & 0x0000FFFF) | (fptr[0] << 16);
		s1 = _mm_loadl_epi64((void *)(sp + (x >> x_shift)));
		x += dx;

		xff = _mm_cvtsi32_si128(flt_i);
		xff = _mm_unpacklo_epi16(xff, xff);
		xff = _mm_unpacklo_epi16(xff, xff);

		ss = _mm_unpacklo_epi64(s0, s1);

		bb = pbuff[i];
		d0 = _mm_mulhi_epi16(ss, xff);
		d0 = _mm_add_epi16(bb, d0);
		pbuff[i] = d0;
	}
}

/* *********************************************************** */

static void
mlib_s_calc_h2_4ch(
    VPARAMH)
{
	DECL_LINE;
	__m128i bb, d0, d1, xf0, xf1, xff, s0, s1, t0, t1;

	for (i = 0; i < (size + 1) / 2; i++) {
		MLIB_FILTER(x, filterX, xoff);
		xf0 = _mm_cvtsi32_si128(*(mlib_s32 *)fptr);
		s0 = _mm_loadu_si128((void *)(sp + (x >> x_shift)));
		x += dx;

		MLIB_FILTER(x, filterX, xoff);
		xf1 = _mm_cvtsi32_si128(*(mlib_s32 *)fptr);
		s1 = _mm_loadu_si128((void *)(sp + (x >> x_shift)));
		x += dx;

		xff = _mm_unpacklo_epi16(xf0, xf1);
		xff = _mm_unpacklo_epi16(xff, xff);
		xf0 = _mm_unpacklo_epi16(xff, xff);
		xf1 = _mm_unpackhi_epi16(xff, xff);

		t0 = _mm_unpacklo_epi64(s0, s1);
		t1 = _mm_unpackhi_epi64(s0, s1);

		bb = pbuff[i];
		d0 = _mm_mulhi_epi16(t0, xf0);
		d1 = _mm_mulhi_epi16(t1, xf1);
		d0 = _mm_add_epi16(d0, bb);
		d0 = _mm_add_epi16(d0, d1);
		pbuff[i] = d0;
	}
}

/* *********************************************************** */

void
mlib_s_calc_y4_u8(
    VPARAMS)
{
	__m128i d0, d1, d2, d3, bb;
	__m128i yfd0, yfd1, yfd2, yfd3;
	__m128i *buff0, *buff1, *buff2, *buff3;
	mlib_s32 i;

	buff0 = buff_arr[bnd];
	buff1 = buff_arr[bnd + 1];
	buff2 = buff_arr[bnd + 2];
	buff3 = buff_arr[bnd + 3];
	yfd0 = READ_FILTER(fptr + 0);
	yfd1 = READ_FILTER(fptr + 1);
	yfd2 = READ_FILTER(fptr + 2);
	yfd3 = READ_FILTER(fptr + 3);

	for (i = 0; i < size; i++) {
		d0 = buff0[i];
		d1 = buff1[i];
		d2 = buff2[i];
		d3 = buff3[i];
		bb = ybuff[i];

		d0 = _mm_mulhi_epi16(d0, yfd0);
		d1 = _mm_mulhi_epi16(d1, yfd1);
		d2 = _mm_mulhi_epi16(d2, yfd2);
		d3 = _mm_mulhi_epi16(d3, yfd3);

		d0 = _mm_add_epi16(d0, d1);
		d2 = _mm_add_epi16(d2, d3);
		d0 = _mm_add_epi16(d0, d2);
		ybuff[i] = _mm_add_epi16(bb, d0);
	}
}

/* *********************************************************** */

void
mlib_s_calc_y4(
    VPARAMS)
{
	__m128i d0, d1, d2, d3, b0, b1;
	__m128i t0, t1, t2, t3;
	__m128i yfd0, yfd1;
	__m128i *buff0, *buff1, *buff2, *buff3;
	mlib_s32 yfds0, yfds1, yfds2, yfds3;
	mlib_s32 i;

	buff0 = buff_arr[bnd];
	buff1 = buff_arr[bnd + 1];
	buff2 = buff_arr[bnd + 2];
	buff3 = buff_arr[bnd + 3];

	yfds0 = (*(mlib_s16*)(fptr));
	yfds1 = (*(mlib_s16*)(fptr + 1));
	yfds2 = (*(mlib_s16*)(fptr + 2));
	yfds3 = (*(mlib_s16*)(fptr + 3));
	yfd0 = _mm_setr_epi16(yfds0, yfds1, yfds0,
	    yfds1, yfds0, yfds1, yfds0, yfds1);
	yfd1 = _mm_setr_epi16(yfds2, yfds3, yfds2,
	    yfds3, yfds2, yfds3, yfds2, yfds3);

	for (i = 0; i < size; i++) {
		d0 = buff0[i];
		d1 = buff1[i];
		b0 = ybuff[2 * i];
		b1 = ybuff[2 * i + 1];

		t0 = _mm_unpacklo_epi16(d0, d1);
		t1 = _mm_unpackhi_epi16(d0, d1);
		t0 = _mm_madd_epi16(t0, yfd0);
		t1 = _mm_madd_epi16(t1, yfd0);
		b0 = _mm_add_epi32(b0, t0);
		b1 = _mm_add_epi32(b1, t1);

		d2 = buff2[i];
		d3 = buff3[i];
		t0 = _mm_unpacklo_epi16(d2, d3);
		t1 = _mm_unpackhi_epi16(d2, d3);
		t0 = _mm_madd_epi16(t0, yfd1);
		t1 = _mm_madd_epi16(t1, yfd1);
		ybuff[2 * i] = _mm_add_epi32(b0, t0);
		ybuff[2 * i + 1] = _mm_add_epi32(b1, t1);
	}
}

/* *********************************************************** */

void
mlib_s_calc_y3_u8(
    VPARAMS)
{
	__m128i d0, d1, d2, bb;
	__m128i yfd0, yfd1, yfd2;
	__m128i *buff0, *buff1, *buff2;
	mlib_s32 i;

	buff0 = buff_arr[bnd];
	buff1 = buff_arr[bnd + 1];
	buff2 = buff_arr[bnd + 2];
	yfd0 = READ_FILTER(fptr + 0);
	yfd1 = READ_FILTER(fptr + 1);
	yfd2 = READ_FILTER(fptr + 2);

	for (i = 0; i < size; i++) {
		d0 = buff0[i];
		d1 = buff1[i];
		d2 = buff2[i];
		bb = ybuff[i];

		d0 = _mm_mulhi_epi16(d0, yfd0);
		d1 = _mm_mulhi_epi16(d1, yfd1);
		d2 = _mm_mulhi_epi16(d2, yfd2);

		d0 = _mm_add_epi16(d0, d1);
		d0 = _mm_add_epi16(d0, d2);
		ybuff[i] = _mm_add_epi16(bb, d0);
	}
}

/* *********************************************************** */

void
mlib_s_calc_y3(
    VPARAMS)
{
	__m128i d0, d1, d2, b0, b1;
	__m128i t0, t1;
	__m128i yfd0, yfd1;
	__m128i *buff0, *buff1, *buff2;
	__m128i mzero = _mm_setzero_si128();
	mlib_s32 yfds0, yfds1, yfds2;
	mlib_s32 i;

	buff0 = buff_arr[bnd];
	buff1 = buff_arr[bnd + 1];
	buff2 = buff_arr[bnd + 2];

	yfds0 = (*(mlib_s16*)(fptr));
	yfds1 = (*(mlib_s16*)(fptr + 1));
	yfds2 = (*(mlib_s16*)(fptr + 2));
	yfd0 = _mm_setr_epi16(yfds0, yfds1, yfds0,
	    yfds1, yfds0, yfds1, yfds0, yfds1);
	yfd1 = _mm_setr_epi16(yfds2, 0, yfds2, 0, yfds2, 0, yfds2, 0);

	for (i = 0; i < size; i++) {
		d0 = buff0[i];
		d1 = buff1[i];
		b0 = ybuff[2 * i];
		b1 = ybuff[2 * i + 1];

		t0 = _mm_unpacklo_epi16(d0, d1);
		t1 = _mm_unpackhi_epi16(d0, d1);

		t0 = _mm_madd_epi16(t0, yfd0);
		t1 = _mm_madd_epi16(t1, yfd0);
		b0 = _mm_add_epi32(b0, t0);
		b1 = _mm_add_epi32(b1, t1);

		d2 = buff2[i];
		t0 = _mm_unpacklo_epi16(d2, mzero);
		t1 = _mm_unpackhi_epi16(d2, mzero);
		t0 = _mm_madd_epi16(t0, yfd1);
		t1 = _mm_madd_epi16(t1, yfd1);
		ybuff[2 * i] = _mm_add_epi32(b0, t0);
		ybuff[2 * i + 1] = _mm_add_epi32(b1, t1);
	}
}

/* *********************************************************** */

void
mlib_s_calc_y2_u8(
    VPARAMS)
{
	__m128i d0, d1, bb, t0, t1;
	__m128i yfd0, yfd1;
	__m128i *buff0, *buff1;
	mlib_s32 i;

	buff0 = buff_arr[bnd];
	buff1 = buff_arr[bnd + 1];
	yfd0 = READ_FILTER(fptr + 0);
	yfd1 = READ_FILTER(fptr + 1);

	for (i = 0; i < size; i++) {
		d0 = buff0[i];
		d1 = buff1[i];
		bb = ybuff[i];

		d0 = _mm_mulhi_epi16(d0, yfd0);
		d1 = _mm_mulhi_epi16(d1, yfd1);

		d0 = _mm_add_epi16(d0, d1);

		__m128i tt;
		tt = _mm_add_epi16(bb, d0);
		ybuff[i] = tt;
	}
}

/* *********************************************************** */

void
mlib_s_calc_y2(
    VPARAMS)
{
	__m128i d0, d1, b0, b1, t0, t1;
	__m128i yfd0, yfd1;
	__m128i *buff0, *buff1;
	mlib_s32 i, yfds0, yfds1;

	buff0 = buff_arr[bnd];
	buff1 = buff_arr[bnd + 1];
	yfds0 = (*(mlib_s16*)(fptr));
	yfds1 = (*(mlib_s16*)(fptr + 1));
	yfd0 = _mm_setr_epi16(yfds0, yfds1, yfds0,
	    yfds1, yfds0, yfds1, yfds0, yfds1);

	for (i = 0; i < size; i++) {
		d0 = buff0[i];
		d1 = buff1[i];
		t0 = _mm_unpacklo_epi16(d0, d1);
		t1 = _mm_unpackhi_epi16(d0, d1);
		b0 = ybuff[2 * i];
		b1 = ybuff[2 * i + 1];

		d0 = _mm_madd_epi16(t0, yfd0);
		d1 = _mm_madd_epi16(t1, yfd0);

		__m128i tt0, tt1;
		tt0 = _mm_add_epi32(b0, d0);
		tt1 = _mm_add_epi32(b1, d1);
		ybuff[2 * i] = tt0;
		ybuff[2 * i + 1] = tt1;
	}
}

/* *********************************************************** */

void
mlib_s_calc_y1_u8(
    VPARAMS)
{
	__m128i d0, bb;
	__m128i yfd0;
	__m128i *buff0;
	mlib_s32 i;

	buff0 = buff_arr[bnd];
	yfd0 = READ_FILTER(fptr + 0);

	for (i = 0; i < size; i++) {
		d0 = buff0[i];
		bb = ybuff[i];

		d0 = _mm_mulhi_epi16(d0, yfd0);

		ybuff[i] = _mm_add_epi16(bb, d0);
	}
}

/* *********************************************************** */

void
mlib_s_calc_y1(
    VPARAMS)
{
	__m128i d0, t0, t1, b0, b1;
	__m128i yfd0;
	__m128i *buff0;
	__m128i mzero = _mm_setzero_si128();
	mlib_s32 i, yfds0;

	buff0 = buff_arr[bnd];
	yfds0 = (*(mlib_s16*)(fptr));
	yfd0 = _mm_setr_epi16(yfds0, 0, yfds0, 0, yfds0, 0, yfds0, 0);

	for (i = 0; i < size; i++) {
		d0 = buff0[i];
		t0 = _mm_unpacklo_epi16(d0, mzero);
		t1 = _mm_unpackhi_epi16(d0, mzero);
		b0 = ybuff[2 * i];
		b1 = ybuff[2 * i + 1];

		t0 = _mm_madd_epi16(t0, yfd0);
		t1 = _mm_madd_epi16(t1, yfd0);
		ybuff[2 * i] = _mm_add_epi32(b0, t0);
		ybuff[2 * i + 1] = _mm_add_epi32(b1, t1);
	}
}

/* *********************************************************** */

void
mlib_shift_left_s16(
    __m128i * buff,
    mlib_s32 size,
    mlib_s32 shift)
{
	__m128i r0, r1, dd;
	__m128i const255 = _mm_set1_epi16(0xff00);
	__m128i const255sh = _mm_set1_epi16(0xff00 >> shift);
	__m128i mzero = _mm_setzero_si128();
	mlib_s32 i;

	for (i = 0; i < size; i++) {
		dd = buff[i];
		r0 = _mm_cmplt_epi16(dd, mzero);
		r1 = _mm_cmpgt_epi16(dd, const255sh);
		r0 = _mm_or_si128(r0, r1);
		r1 = _mm_and_si128(r1, const255);
		dd = _mm_slli_epi16(dd, shift);
		dd = _mm_or_si128(_mm_andnot_si128(r0, dd), r1);
		buff[i] = dd;
	}
}

/* *********************************************************** */

mlib_status
mlib_s_ImageZoomTranslateTable(
    PARAMS_NW)
{
	DECLAREVAR;
	mlib_type type = ws->type;
	__m128i *pbuff, *xbuff, **buff_arr, *tmp_ptr, *buffd, *buffs;
	void *sp0;
	mlib_u8 *tsp;
	mlib_s32 bind, y_step, k, ynew, size2, max_xsize2;
	mlib_s32 kh;
	mlib_s32 chan_d = ws->dchan;
	mlib_s32 chan_s = ws->nchan;
	mlib_s32 nchan = chan_s;
	mlib_s32 alp_ind = -(ws->alpha_shift);
	mlib_s32 row_size_max = ((max_xsize * ws->dx) >> x_shift) + m + 1;
	mlib_s32 buf_size, is_bufd, is_bufs, xSrc, cmask, row_size;
	__m128i amask;
	__m128i mzero = _mm_setzero_si128();
	__m128i fzeros = _mm_setzero_si128();
	__m128i fill = _mm_set1_epi16(0xFFFF);
	__m128i mask7FFF = _mm_set1_epi16(0x7FFF);
	__m128i scale255 =
	    _mm_set1_epi16((mlib_u16)((1 << 15) * 256.0 / 255 + 0.5));

	if (chan_s == 3)
		nchan = 4;
	max_xsize *= nchan;
	max_xsize2 = (max_xsize + 7) / 8;
	max_xsize = 2 * max_xsize2;
	buf_size = (n + 1 + 1) * max_xsize2;
/*
 * The second '+ 1' for S16/U16 ybuff
 */

	is_bufd = 0;
/*
 * if (alp_ind || ((((mlib_addr)dstData | dstYStride) & 3) != 0))
 */
	{
		buf_size += max_xsize2;
		is_bufd = 1;
	}

	is_bufs = 0;
/*
 * if (alp_ind || ((((mlib_addr)(ws -> srcData) | ws -> srcStride) & 3) != 0))
 */
	{
		buf_size += ((row_size_max + 8) >> 1);
		is_bufs = 1;
	}

	xbuff =
	    MALLOC(buf_size * sizeof (__m128i) + 2 * n * sizeof (__m128i *));

	if (xbuff == NULL)
		return (MLIB_FAILURE);

	tmp_ptr = ybuff = xbuff + n * max_xsize2;
	tmp_ptr = tmp_ptr + 2 * max_xsize2; /* ybuff for S32 inter image type */

	if (is_bufd) {
		buffd = tmp_ptr;
		tmp_ptr += max_xsize2;
	}

	if (is_bufs) {
		buffs = tmp_ptr;
		tmp_ptr += ((row_size_max + 8) >> 1);
	}

	buff_arr = (__m128i **) tmp_ptr;
	for (k = 0; k < n; k++)
		buff_arr[n + k] = buff_arr[k] = xbuff + k * max_xsize2;

	filterX = table->dataH_s16;
	filterY = table->dataV_s16;

	if (type == MLIB_SHORT) {
		shift = table->shift_vis + 8;
		dsat = _mm_set1_epi16(0);
	} else if (type == MLIB_USHORT) {
		shift = table->shift_vis + 7;
		dsat = _mm_set1_epi16(0);
	} else {
		shift = table->shift_vis - 1;
		if (!alp_ind) {
			dsat = _mm_set1_epi16(1 << (shift - 1));
		} else {
			dsat = _mm_set1_epi16(0);
		}
	}

	DIST_BITS();

/* ws -> x_shift  = x_shift; */
	ws->xf_shift = xf_shift;
	ws->xf_mask = xf_mask;
	ws->yf_shift = yf_shift;
	ws->yf_mask = yf_mask;

	y_step = n;
	bind = 0;

	if (!alp_ind) {
		chan_d = chan_s;
	}

	for (j = yStart; j <= yFinish; j++) {
		CLIP(chan_d);

		ws->size = size;
		size2 = (size * nchan + 7) / 8;

		for (i = 0; i < 2 * size2; i++) {
			ybuff[i] = dsat;
		}

/* move to kernel center */
		x0 -= ws->x_move;
		xSrc = x0 >> x_shift;

		row_size = row_size_max;

		if (row_size > (ws->srcWidth - xSrc))
			row_size = (ws->srcWidth - xSrc);

		ynew = yStarts[j + 1];

		if (y_step >= n) {
			y_step = n;
		} else {
			bind += y_step;

			if (bind >= n)
				bind -= n;
		}

		y = y0 - ws->y_move;
		ynew = (ynew - ws->y_move) >> y_shift;
		ySrc = y >> y_shift;

		for (l = n - y_step; l < n; l++) {
			pbuff = buff_arr[bind + l];

			tsp = lineAddr[ySrc + l];

			if (type == MLIB_SHORT) {
				mlib_s16 *sp = (mlib_s16 *)tsp + chan_s * xSrc;

				if (chan_s == 3) {
					for (i = 0; i < (row_size + 1) / 2;
						i++) {
						__m128i s0 =
						    _mm_loadl_epi64((void *)(sp
						    + 6 * i));
						__m128i s1 =
						    _mm_loadl_epi64((void *)(sp
						    + 6 * i + 3));
						__m128i tt;
						tt =
						    _mm_unpacklo_epi64(s0, s1);
						buffs[i] = tt;
					}
				} else {
					buffs = (void *)sp;
				}
			} else if (type == MLIB_USHORT) {
				mlib_s16 *sp = (mlib_s16 *)tsp + chan_s * xSrc;

				if (chan_s == 3) {
					for (i = 0; i < (row_size + 1) / 2;
						i++) {
						__m128i s0 =
						    _mm_loadl_epi64((void *)(sp
						    + 6 * i));
						__m128i s1 =
						    _mm_loadl_epi64((void *)(sp
						    + 6 * i + 3));
						buffs[i] =
						    _mm_srli_epi16
						    (_mm_unpacklo_epi64(s0, s1),
						    1);
					}
				} else {
					for (i = 0;
					    i < (chan_s * row_size + 7) / 8;
					    i++) {
						__m128i s0 =
						    _mm_loadl_epi64((void *)(sp
						    + 8 * i));
						__m128i s1 =
						    _mm_loadl_epi64((void *)(sp
						    + 8 * i + 4));
						buffs[i] =
						    _mm_srli_epi16
						    (_mm_unpacklo_epi64(s0, s1),
						    1);
					}
				}
			} else if (chan_s == 4) {
				tsp += 4 * xSrc;

				if (!alp_ind) {
					for (i = 0; i < (4 * row_size + 7) >> 3;
					    i++) {
						MUL_HALF(tsp, buffs);
					}
				} else if (alp_ind == 3) {
					amask =
					    _mm_setr_epi32(0, 0xff000000, 0,
					    0xff000000);
					for (i = 0; i < (4 * row_size + 7) >> 3;
					    i++) {
						MUL_ALPHA_4CH(tsp, buffs, 0xff);
					}
				} else {
					amask =
					    _mm_setr_epi32(0xff00, 0, 0xff00,
					    0);
					for (i = 0; i < (4 * row_size + 7) >> 3;
					    i++) {
						MUL_ALPHA_4CH(tsp, buffs, 0);
					}
				}
			} else if (chan_s == 3) {
				mlib_s32 *p_buffs = (void *)buffs;

				tsp += 3 * xSrc;

				if (alp_ind == -1) {
					for (i = 0; i < row_size; i++) {
						(*p_buffs++) =
						    (0xff << 7) | (tsp[0] <<
						    23);
						(*p_buffs++) =
						    (tsp[1] << 7) | (tsp[2] <<
						    23);
						tsp += 3;
					}
				} else {
					for (i = 0; i < row_size; i++) {
						(*p_buffs++) =
						    (tsp[0] << 7) | (tsp[1] <<
						    23);
						(*p_buffs++) =
						    (tsp[2] << 7) | (0xff <<
						    23);
						tsp += 3;
					}
				}
			} else {
				tsp += chan_s * xSrc;

				for (i = 0; i < (chan_s * row_size + 7) / 8;
				    i++) {
					__m128i ss;

					ss = _mm_loadl_epi64((void *)(tsp +
					    8 * i));
					buffs[i] =
					    _mm_srli_epi16(_mm_unpacklo_epi8
					    (mzero, ss), 1);
				}
			}

			for (i = 0; i < size2; i++) {
				pbuff[i] = mzero;
			}

/* offset in current kernel line */
			for (off = 0; off < m; off += kw) {
				x = x0;
				xoff = 2 * off;
				ws->off = xoff;

				kw = m - off;

				if (chan_s == 1) {
					if (kw > 4)
						kw = 4;
					if (kw == 3)
						kw = 2;
					sp0 = (mlib_s16 *)buffs + (off - xSrc);
					if (kw == 4) {
						mlib_s_calc_h4_1ch(x, filterX,
						    ws, pbuff, sp0);
					} else if (kw == 2) {
						mlib_s_calc_h2_1ch(x, filterX,
						    ws, pbuff, sp0);
					} else {	/* if (kw == 1) */

						mlib_s_calc_h1_1ch(x, filterX,
						    ws, pbuff, sp0);
					}
				} else if (chan_s == 2) {
					if (kw > 2)
						kw = 2;
					sp0 =
					    (mlib_s16 *)buffs + 2 * (off -
					    xSrc);
					if (kw == 2) {
						mlib_s_calc_h2_2ch(x, filterX,
						    ws, pbuff, sp0);
					} else {	/* if (kw == 1) */

						mlib_s_calc_h1_2ch(x, filterX,
						    ws, pbuff, sp0);
					}
				} else {
					if (kw > 2)
						kw = 2;
					sp0 =
					    (mlib_s16 *)buffs + 4 * (off -
					    xSrc);
					if (kw == 2) {
						mlib_s_calc_h2_4ch(x, filterX,
						    ws, pbuff, sp0);
					} else {	/* if (kw == 1) */

						mlib_s_calc_h1_4ch(x, filterX,
						    ws, pbuff, sp0);
					}
				}
			}
		}

		size *= nchan;

		for (l = 0; l < n; l += kh) {
			kh = n - l;

			if (kh >= 4)
				kh = 4;

			MLIB_FILTER(y, filterY, 8 * l);

			if (type == MLIB_BYTE) {
			if (kh == 4) {
				mlib_s_calc_y4_u8(fptr, buff_arr, bind + l,
				    ybuff, size2);
			} else if (kh == 3) {
				mlib_s_calc_y3_u8(fptr, buff_arr, bind + l,
				    ybuff, size2);
			} else if (kh == 2) {
				mlib_s_calc_y2_u8(fptr, buff_arr, bind + l,
				    ybuff, size2);
			} else {
/* kh == 1 */
				mlib_s_calc_y1_u8(fptr, buff_arr, bind + l,
				    ybuff, size2);
			}

			} else { /*	if (type == MLIB_BYTE) */
			if (kh == 4) {
				mlib_s_calc_y4(fptr, buff_arr, bind + l, ybuff,
				    size2);
			} else if (kh == 3) {
				mlib_s_calc_y3(fptr, buff_arr, bind + l, ybuff,
				    size2);
			} else if (kh == 2) {
				mlib_s_calc_y2(fptr, buff_arr, bind + l, ybuff,
				    size2);
			} else {
/* kh == 1 */
				mlib_s_calc_y1(fptr, buff_arr, bind + l, ybuff,
				    size2);
			}
			}
		}
		for (i = 0; i < size2; i++) {
			__m128i tt;
			tt = ybuff[i];
		}

		if (type == MLIB_SHORT) {
			mlib_u16 *dl = (mlib_u16 *)dstData + chan_d * xLeft;
			mlib_u16 *dp = dl;
			__m128i dd, d0, d1;

			if (chan_d == 3)
				dp = (void *)buffd;
			for (i = 0; i < size / 8; i++) {
				__m128i dd0, dd1;

				dd0 = ybuff[2 * i];
				dd1 = ybuff[2 * i + 1];
				d0 = _mm_srai_epi32(dd0, shift);
				d1 = _mm_srai_epi32(dd1, shift);
				dd0 = _mm_packs_epi32(d0, d1);
				_mm_storeu_si128((void *)dp, dd0);
				dp += 8;
			}
			if (size & 7) {
				__m128i dd0, dd1;
				dd0 = ybuff[2 * i];
				dd1 = ybuff[2 * i + 1];
				d0 = _mm_srai_epi32(dd0, shift);
				d1 = _mm_srai_epi32(dd1, shift);

				dd = _mm_packs_epi32(d0, d1);
				for (i = 0; i < (size & 7); i++) {
					dp[i] = ((mlib_u16 *)&dd)[i];
				}
			}

			if (chan_d == 3) {
				mlib_s_ImageChannelExtract_S16_43L_D1((void *)
				    buffd, (void *)dl, size / 4);
			}
		} else if (type == MLIB_USHORT) {
			mlib_u16 *dl = (mlib_u16 *)dstData + chan_d * xLeft;
			mlib_u16 *dp = dl;
			__m128i dd, d0, d1;
			__m128i val16 = _mm_set1_epi16(0x8000);
			__m128i val32 = _mm_set1_epi32(0x8000);

			if (chan_d == 3)
				dp = (void *)buffd;
			for (i = 0; i < size / 8; i++) {
				__m128i dd0, dd1;
				dd0 = ybuff[2 * i];
				dd1 = ybuff[2 * i + 1];
				d0 = _mm_srai_epi32(dd0, shift);
				d1 = _mm_srai_epi32(dd1, shift);
				d0 = _mm_sub_epi32(d0, val32);
				d1 = _mm_sub_epi32(d1, val32);
				dd = _mm_packs_epi32(d0, d1);
				dd = _mm_add_epi16(dd, val16);
				_mm_storeu_si128((void *)dp, dd);
				dp += 8;
			}
			if (size & 7) {
				__m128i dd0, dd1;
				dd0 = ybuff[2 * i];
				dd1 = ybuff[2 * i + 1];
				d0 = _mm_srai_epi32(dd0, shift);
				d1 = _mm_srai_epi32(dd1, shift);

				d0 = _mm_sub_epi32(d0, val32);
				d1 = _mm_sub_epi32(d1, val32);
				dd = _mm_packs_epi32(d0, d1);
				dd = _mm_add_epi16(dd, val16);
				for (i = 0; i < (size & 7); i++) {
					dp[i] = ((mlib_u16 *)&dd)[i];
				}
			}
			if (chan_d == 3) {
				mlib_s_ImageChannelExtract_S16_43L_D1((void *)
				    buffd, (void *)dl, size / 4);
			}
		} else if (!alp_ind) {
			mlib_u8 *dp = (void *)dl;

			if (chan_d == 3)
				dp = (void *)buffd;
			for (i = 0; i < size / 8; i++) {
				__m128i dd, d0, d1;

				dd = ybuff[i];

				dd = _mm_srai_epi16(dd, shift);
				dd = _mm_packus_epi16(dd, dd);
				_mm_storel_epi64((void *)dp, dd);
				dp += 8;
			}
			if (size & 7) {
				__m128i dd;

				dd = ybuff[i];
				dd = _mm_srai_epi16(dd, shift);
				dd = _mm_packus_epi16(dd, dd);
				for (i = 0; i < (size & 7); i++) {
					dp[i] = ((mlib_u8 *)&dd)[i];
				}
			}

			if (chan_d == 3) {
				mlib_s_ImageChannelExtract_U8_43L_D1((void *)
				    buffd, dl, size / 4);
			}
		} else {
			mlib_clipping current[1];
			mlib_work_image blend_param[1];

			blend_param->blend = ws->blend;
			blend_param->chan_d = ws->dchan;
			blend_param->channels = 4;
			blend_param->alp_ind = alp_ind;
			blend_param->alpha = 255;
			blend_param->current = current;
			current->width = size / 4;

			mlib_shift_left_s16((__m128i *) ybuff, size2,
			    8 - shift);

			if (alp_ind == -1 && chan_d == 4)
				dl++;
			mlib_s_ImageBlendLine(blend_param, dl, ybuff, buffd);
		}

		y_step = ynew - ySrc;
	}

	__mlib_free(xbuff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
