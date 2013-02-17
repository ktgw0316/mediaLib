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

#pragma ident	"@(#)mlib_c_ImagePolynomialWarp_BC.c	9.2	07/10/09 SMI"

#include <mlib_ImageFilters.h>
#include <mlib_ImagePolynomialWarp.h>

/* *********************************************************** */

#define	MLIB_FILTER_SHIFT	(4-1)
/* ((1 << 8) - 1) << 4 */
#define	MLIB_FILTER_MASK	4080

/* *********************************************************** */

#ifdef _MSC_VER
#define	TABLE(X)	(X)
#else /* _MSC_VER */

#define	TABLE(X)	mlib_U82D64[X]
#endif /* _MSC_VER */

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

#define	SAT8(out, in)	out = ((mlib_s32)((in) - sat) >> 24) ^ 0x80;

#define	SAT_S16(out, in)	out = ((mlib_s32)(in)) >> MLIB_SHIFT;

#define	SAT_U16(out, in)	out = ((mlib_s32)(in + MLIB_S32_MIN) - \
				MLIB_S32_MIN) >> MLIB_SHIFT

/* *********************************************************** */

#else /* MLIB_USE_FTOI_CLAMPING */

#define	SAT8(out, in)                                                     \
	in -= sat;                                                        \
	out = (in >= MLIB_S32_MAX) ? MLIB_U8_MAX : (in <=                 \
	    MLIB_S32_MIN) ? MLIB_U8_MIN : ((mlib_s32)(in) >> 24) ^ 0x80

/* *********************************************************** */

#define	SAT_S16(out, in)                                         \
	out = (in >= MLIB_S32_MAX) ? MLIB_S16_MAX : (in <=       \
	    MLIB_S32_MIN) ? MLIB_S16_MIN : ((mlib_s32)(in)) >>   \
	    MLIB_SHIFT

/* *********************************************************** */

#define	SAT_U16(out, in)                                         \
	out = (in >= MLIB_U32_MAX) ? MLIB_U16_MAX : (in <=       \
	    MLIB_U32_MIN) ? MLIB_U16_MIN : ((mlib_u32)(in)) >>   \
	    MLIB_SHIFT

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#define	COMPUTING_POINT(a0, a1, a2, a3)                         \
	(TABLE(line[a0]) * xf0 + TABLE(line[a1]) * xf1 +        \
	    TABLE(line[a2]) * xf2 + TABLE(line[a3]) * xf3)

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BC_U8_1(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	void *filter_table = (void *)pws->filter_table;
	const mlib_u8 *line;
	mlib_d64 sat = (mlib_d64)0x7F800000;
	mlib_d64 xf0, xf1, xf2, xf3, yf0, yf1, yf2, yf3, r0;
	mlib_s32 i, xsrc, ysrc, pos, stride = lineAddr[1] - lineAddr[0];
	mlib_s32 *xs = pws->src_x, *ys = pws->src_y, *xd = pws->dst_x;
	mlib_u16 *dxs = pws->dsrc_x, *dys = pws->dsrc_y;
	mlib_f32 *filter;

	if (n <= 0)
		return;

	xsrc = *xs - 1;
	xs++;
	ysrc = *ys - 1;
	ys++;

	pos = (*dxs >> MLIB_FILTER_SHIFT) & MLIB_FILTER_MASK;
	dxs++;
	filter = (mlib_f32 *)((mlib_u8 *)filter_table + pos);
	xf0 = filter[0];
	xf1 = filter[1];
	xf2 = filter[2];
	xf3 = filter[3];

	pos = (*dys >> MLIB_FILTER_SHIFT) & MLIB_FILTER_MASK;
	dys++;
	filter = (mlib_f32 *)((mlib_u8 *)filter_table + pos);
	yf0 = filter[0];
	yf1 = filter[1];
	yf2 = filter[2];
	yf3 = filter[3];

	line = lineAddr[ysrc] + xsrc;

	r0 = COMPUTING_POINT(0, 1, 2, 3) * yf0;
	line += stride;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
	for (i = 0; i < n - 1; i++) {

		r0 += COMPUTING_POINT(0, 1, 2, 3) * yf1;
		line += stride;
		r0 += COMPUTING_POINT(0, 1, 2, 3) * yf2;
		line += stride;
		r0 += COMPUTING_POINT(0, 1, 2, 3) * yf3;

		xsrc = *xs - 1;
		xs++;
		ysrc = *ys - 1;
		ys++;

		pos = (*dxs >> MLIB_FILTER_SHIFT) & MLIB_FILTER_MASK;
		dxs++;
		filter = (mlib_f32 *)((mlib_u8 *)filter_table + pos);
		xf0 = filter[0];
		xf1 = filter[1];
		xf2 = filter[2];
		xf3 = filter[3];

		pos = (*dys >> MLIB_FILTER_SHIFT) & MLIB_FILTER_MASK;
		dys++;
		filter = (mlib_f32 *)((mlib_u8 *)filter_table + pos);
		yf0 = filter[0];
		yf1 = filter[1];
		yf2 = filter[2];
		yf3 = filter[3];

		line = lineAddr[ysrc] + xsrc;

		SAT8(dstData[xd[i]], r0);

		r0 = COMPUTING_POINT(0, 1, 2, 3) * yf0;
		line += stride;
	}

	r0 += COMPUTING_POINT(0, 1, 2, 3) * yf1;
	line += stride;
	r0 += COMPUTING_POINT(0, 1, 2, 3) * yf2;
	line += stride;
	r0 += COMPUTING_POINT(0, 1, 2, 3) * yf3;

	SAT8(dstData[xd[i]], r0);
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BC_U8_2(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	void *filter_table = (void *)pws->filter_table;
	const mlib_u8 *line;
	mlib_d64 sat = (mlib_d64)0x7F800000;
	mlib_d64 xf0, xf1, xf2, xf3, yf0, yf1, yf2, yf3, r0;
	mlib_s32 i, xsrc, ysrc, pos, ch = 0, stride = lineAddr[1] - lineAddr[0];
	mlib_s32 *xs = pws->src_x, *ys = pws->src_y, *xd = pws->dst_x;
	mlib_u16 *dxs = pws->dsrc_x, *dys = pws->dsrc_y;
	mlib_f32 *filter;

	if (n <= 0)
		return;

next:
	xsrc = *xs - 1;
	xs++;
	ysrc = *ys - 1;
	ys++;

	pos = (*dxs >> MLIB_FILTER_SHIFT) & MLIB_FILTER_MASK;
	dxs++;
	filter = (mlib_f32 *)((mlib_u8 *)filter_table + pos);
	xf0 = filter[0];
	xf1 = filter[1];
	xf2 = filter[2];
	xf3 = filter[3];

	pos = (*dys >> MLIB_FILTER_SHIFT) & MLIB_FILTER_MASK;
	dys++;
	filter = (mlib_f32 *)((mlib_u8 *)filter_table + pos);
	yf0 = filter[0];
	yf1 = filter[1];
	yf2 = filter[2];
	yf3 = filter[3];

	line = lineAddr[ysrc] + xsrc * 2 + ch;

	r0 = COMPUTING_POINT(0, 2, 4, 6) * yf0;
	line += stride;

	r0 += COMPUTING_POINT(0, 2, 4, 6) * yf1;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
	for (i = 0; i < n - 1; i++) {

		pos = (*dys >> MLIB_FILTER_SHIFT) & MLIB_FILTER_MASK;
		dys++;
		filter = (mlib_f32 *)((mlib_u8 *)filter_table + pos);
		yf0 = filter[0];

		line += stride;
		yf1 = filter[1];
		r0 += COMPUTING_POINT(0, 2, 4, 6) * yf2;
		line += stride;
		yf2 = filter[2];
		r0 += COMPUTING_POINT(0, 2, 4, 6) * yf3;
		yf3 = filter[3];

		xsrc = *xs - 1;
		xs++;
		ysrc = *ys - 1;
		ys++;

		pos = (*dxs >> MLIB_FILTER_SHIFT) & MLIB_FILTER_MASK;
		dxs++;
		filter = (mlib_f32 *)((mlib_u8 *)filter_table + pos);
		xf0 = filter[0];
		xf1 = filter[1];
		xf2 = filter[2];
		xf3 = filter[3];

		line = lineAddr[ysrc] + xsrc * 2 + ch;

		SAT8(dstData[xd[i] * 2], r0);
		r0 = COMPUTING_POINT(0, 2, 4, 6) * yf0;
		line += stride;
		r0 += COMPUTING_POINT(0, 2, 4, 6) * yf1;
	}

	line += stride;
	r0 += COMPUTING_POINT(0, 2, 4, 6) * yf2;
	line += stride;
	r0 += COMPUTING_POINT(0, 2, 4, 6) * yf3;

	SAT8(dstData[xd[i] * 2], r0);

	if (ch < 1) {
		ch++;
		xs = pws->src_x;
		ys = pws->src_y;
		dxs = pws->dsrc_x;
		dys = pws->dsrc_y;
		dstData++;
		goto next;
	}
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BC_U8_3(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	void *filter_table = (void *)pws->filter_table;
	const mlib_u8 *line;
	mlib_d64 sat = (mlib_d64)0x7F800000;
	mlib_d64 xf0, xf1, xf2, xf3, yf0, yf1, yf2, yf3, r0;
	mlib_s32 i, xsrc, ysrc, pos, ch = 0, stride = lineAddr[1] - lineAddr[0];
	mlib_s32 *xs = pws->src_x, *ys = pws->src_y, *xd = pws->dst_x;
	mlib_u16 *dxs = pws->dsrc_x, *dys = pws->dsrc_y;
	mlib_f32 *filter;

	if (n <= 0)
		return;

next:
	xsrc = *xs - 1;
	xs++;
	ysrc = *ys - 1;
	ys++;

	pos = (*dxs >> MLIB_FILTER_SHIFT) & MLIB_FILTER_MASK;
	dxs++;
	filter = (mlib_f32 *)((mlib_u8 *)filter_table + pos);
	xf0 = filter[0];
	xf1 = filter[1];
	xf2 = filter[2];
	xf3 = filter[3];

	pos = (*dys >> MLIB_FILTER_SHIFT) & MLIB_FILTER_MASK;
	dys++;
	filter = (mlib_f32 *)((mlib_u8 *)filter_table + pos);
	yf0 = filter[0];
	yf1 = filter[1];
	yf2 = filter[2];
	yf3 = filter[3];

	line = lineAddr[ysrc] + xsrc * 3 + ch;

	r0 = COMPUTING_POINT(0, 3, 6, 9) * yf0;
	line += stride;

	r0 += COMPUTING_POINT(0, 3, 6, 9) * yf1;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
	for (i = 0; i < n - 1; i++) {

		pos = (*dys >> MLIB_FILTER_SHIFT) & MLIB_FILTER_MASK;
		dys++;
		filter = (mlib_f32 *)((mlib_u8 *)filter_table + pos);
		yf0 = filter[0];

		line += stride;
		yf1 = filter[1];
		r0 += COMPUTING_POINT(0, 3, 6, 9) * yf2;
		line += stride;
		yf2 = filter[2];
		r0 += COMPUTING_POINT(0, 3, 6, 9) * yf3;
		yf3 = filter[3];

		xsrc = *xs - 1;
		xs++;
		ysrc = *ys - 1;
		ys++;

		pos = (*dxs >> MLIB_FILTER_SHIFT) & MLIB_FILTER_MASK;
		dxs++;
		filter = (mlib_f32 *)((mlib_u8 *)filter_table + pos);
		xf0 = filter[0];
		xf1 = filter[1];
		xf2 = filter[2];
		xf3 = filter[3];

		line = lineAddr[ysrc] + xsrc * 3 + ch;

		SAT8(dstData[xd[i] * 3], r0);
		r0 = COMPUTING_POINT(0, 3, 6, 9) * yf0;
		line += stride;
		r0 += COMPUTING_POINT(0, 3, 6, 9) * yf1;
	}

	line += stride;
	r0 += COMPUTING_POINT(0, 3, 6, 9) * yf2;
	line += stride;
	r0 += COMPUTING_POINT(0, 3, 6, 9) * yf3;

	SAT8(dstData[xd[i] * 3], r0);

	if (ch < 2) {
		ch++;
		xs = pws->src_x;
		ys = pws->src_y;
		dxs = pws->dsrc_x;
		dys = pws->dsrc_y;
		dstData++;
		goto next;
	}
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BC_U8_4(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	void *filter_table = (void *)pws->filter_table;
	const mlib_u8 *line;
	mlib_d64 sat = (mlib_d64)0x7F800000;
	mlib_d64 xf0, xf1, xf2, xf3, yf0, yf1, yf2, yf3, r0;
	mlib_s32 i, xsrc, ysrc, pos, ch = 0, stride = lineAddr[1] - lineAddr[0];
	mlib_s32 *xs = pws->src_x, *ys = pws->src_y, *xd = pws->dst_x;
	mlib_u16 *dxs = pws->dsrc_x, *dys = pws->dsrc_y;
	mlib_f32 *filter;

	if (n <= 0)
		return;

next:
	xsrc = *xs - 1;
	xs++;
	ysrc = *ys - 1;
	ys++;

	pos = (*dxs >> MLIB_FILTER_SHIFT) & MLIB_FILTER_MASK;
	dxs++;
	filter = (mlib_f32 *)((mlib_u8 *)filter_table + pos);
	xf0 = filter[0];
	xf1 = filter[1];
	xf2 = filter[2];
	xf3 = filter[3];

	pos = (*dys >> MLIB_FILTER_SHIFT) & MLIB_FILTER_MASK;
	dys++;
	filter = (mlib_f32 *)((mlib_u8 *)filter_table + pos);
	yf0 = filter[0];
	yf1 = filter[1];
	yf2 = filter[2];
	yf3 = filter[3];

	line = lineAddr[ysrc] + xsrc * 4 + ch;

	r0 = COMPUTING_POINT(0, 4, 8, 12) * yf0;
	line += stride;

	r0 += COMPUTING_POINT(0, 4, 8, 12) * yf1;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
	for (i = 0; i < n - 1; i++) {

		pos = (*dys >> MLIB_FILTER_SHIFT) & MLIB_FILTER_MASK;
		dys++;
		filter = (mlib_f32 *)((mlib_u8 *)filter_table + pos);
		yf0 = filter[0];

		line += stride;
		yf1 = filter[1];
		r0 += COMPUTING_POINT(0, 4, 8, 12) * yf2;
		line += stride;
		yf2 = filter[2];
		r0 += COMPUTING_POINT(0, 4, 8, 12) * yf3;
		yf3 = filter[3];

		xsrc = *xs - 1;
		xs++;
		ysrc = *ys - 1;
		ys++;

		pos = (*dxs >> MLIB_FILTER_SHIFT) & MLIB_FILTER_MASK;
		dxs++;
		filter = (mlib_f32 *)((mlib_u8 *)filter_table + pos);
		xf0 = filter[0];
		xf1 = filter[1];
		xf2 = filter[2];
		xf3 = filter[3];

		line = lineAddr[ysrc] + xsrc * 4 + ch;

		SAT8(dstData[xd[i] * 4], r0);
		r0 = COMPUTING_POINT(0, 4, 8, 12) * yf0;
		line += stride;
		r0 += COMPUTING_POINT(0, 4, 8, 12) * yf1;
	}

	line += stride;
	r0 += COMPUTING_POINT(0, 4, 8, 12) * yf2;
	line += stride;
	r0 += COMPUTING_POINT(0, 4, 8, 12) * yf3;

	SAT8(dstData[xd[i] * 4], r0);

	if (ch < 3) {
		ch++;
		xs = pws->src_x;
		ys = pws->src_y;
		dxs = pws->dsrc_x;
		dys = pws->dsrc_y;
		dstData++;
		goto next;
	}
}

/* *********************************************************** */

#undef TABLE
#undef MLIB_FILTER_SHIFT
#undef MLIB_FILTER_MASK

/* *********************************************************** */

#define	TABLE(X)	(X)
#define	MLIB_FILTER_SHIFT	(3-1)

/* (((1 << 9) - 1) << 4) */
#define	MLIB_FILTER_MASK	8176

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BC_S16_1(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	void *filter_table = (void *)pws->filter_table;
	mlib_s16 *line, *dp = (void *)dstData;
	mlib_d64 xf0, xf1, xf2, xf3, yf0, yf1, yf2, yf3, r0;
	mlib_s32 i, xsrc, ysrc, pos, stride = (lineAddr[1] - lineAddr[0]) >> 1;
	mlib_s32 *xs = pws->src_x, *ys = pws->src_y, *xd = pws->dst_x;
	mlib_u16 *dxs = pws->dsrc_x, *dys = pws->dsrc_y;
	mlib_f32 *filter;

	if (n <= 0)
		return;

	xsrc = *xs - 1;
	xs++;
	ysrc = *ys - 1;
	ys++;

	pos = (*dxs >> MLIB_FILTER_SHIFT) & MLIB_FILTER_MASK;
	dxs++;
	filter = (mlib_f32 *)((mlib_u8 *)filter_table + pos);
	xf0 = filter[0];
	xf1 = filter[1];
	xf2 = filter[2];
	xf3 = filter[3];

	pos = (*dys >> MLIB_FILTER_SHIFT) & MLIB_FILTER_MASK;
	dys++;
	filter = (mlib_f32 *)((mlib_u8 *)filter_table + pos);
	yf0 = filter[0];
	yf1 = filter[1];
	yf2 = filter[2];
	yf3 = filter[3];

	line = (mlib_s16 *)lineAddr[ysrc] + xsrc;

	r0 = COMPUTING_POINT(0, 1, 2, 3) * yf0;
	line += stride;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
	for (i = 0; i < n - 1; i++) {

		r0 += COMPUTING_POINT(0, 1, 2, 3) * yf1;
		line += stride;
		r0 += COMPUTING_POINT(0, 1, 2, 3) * yf2;
		line += stride;
		r0 += COMPUTING_POINT(0, 1, 2, 3) * yf3;

		xsrc = *xs - 1;
		xs++;
		ysrc = *ys - 1;
		ys++;

		pos = (*dxs >> MLIB_FILTER_SHIFT) & MLIB_FILTER_MASK;
		dxs++;
		filter = (mlib_f32 *)((mlib_u8 *)filter_table + pos);
		xf0 = filter[0];
		xf1 = filter[1];
		xf2 = filter[2];
		xf3 = filter[3];

		pos = (*dys >> MLIB_FILTER_SHIFT) & MLIB_FILTER_MASK;
		dys++;
		filter = (mlib_f32 *)((mlib_u8 *)filter_table + pos);
		yf0 = filter[0];
		yf1 = filter[1];
		yf2 = filter[2];
		yf3 = filter[3];

		line = (mlib_s16 *)lineAddr[ysrc] + xsrc;

		SAT_S16(dp[xd[i]], r0);

		r0 = COMPUTING_POINT(0, 1, 2, 3) * yf0;
		line += stride;
	}

	r0 += COMPUTING_POINT(0, 1, 2, 3) * yf1;
	line += stride;
	r0 += COMPUTING_POINT(0, 1, 2, 3) * yf2;
	line += stride;
	r0 += COMPUTING_POINT(0, 1, 2, 3) * yf3;

	SAT_S16(dp[xd[i]], r0);
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BC_S16_2(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	void *filter_table = (void *)pws->filter_table;
	mlib_s16 *line, *dp = (void *)dstData;
	mlib_d64 xf0, xf1, xf2, xf3, yf0, yf1, yf2, yf3, r0;
	mlib_s32 i, ch = 0, xsrc, ysrc, pos, stride =
	    (lineAddr[1] - lineAddr[0]) >> 1;
	mlib_s32 *xs = pws->src_x, *ys = pws->src_y, *xd = pws->dst_x;
	mlib_u16 *dxs = pws->dsrc_x, *dys = pws->dsrc_y;
	mlib_f32 *filter;

	if (n <= 0)
		return;

next:
	xsrc = *xs - 1;
	xs++;
	ysrc = *ys - 1;
	ys++;

	pos = (*dxs >> MLIB_FILTER_SHIFT) & MLIB_FILTER_MASK;
	dxs++;
	filter = (mlib_f32 *)((mlib_u8 *)filter_table + pos);
	xf0 = filter[0];
	xf1 = filter[1];
	xf2 = filter[2];
	xf3 = filter[3];

	pos = (*dys >> MLIB_FILTER_SHIFT) & MLIB_FILTER_MASK;
	dys++;
	filter = (mlib_f32 *)((mlib_u8 *)filter_table + pos);
	yf0 = filter[0];
	yf1 = filter[1];
	yf2 = filter[2];
	yf3 = filter[3];

	line = (mlib_s16 *)lineAddr[ysrc] + xsrc * 2 + ch;

	r0 = COMPUTING_POINT(0, 2, 4, 6) * yf0;
	line += stride;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
	for (i = 0; i < n - 1; i++) {

		r0 += COMPUTING_POINT(0, 2, 4, 6) * yf1;
		line += stride;
		r0 += COMPUTING_POINT(0, 2, 4, 6) * yf2;
		line += stride;
		r0 += COMPUTING_POINT(0, 2, 4, 6) * yf3;

		xsrc = *xs - 1;
		xs++;
		ysrc = *ys - 1;
		ys++;

		pos = (*dxs >> MLIB_FILTER_SHIFT) & MLIB_FILTER_MASK;
		dxs++;
		filter = (mlib_f32 *)((mlib_u8 *)filter_table + pos);
		xf0 = filter[0];
		xf1 = filter[1];
		xf2 = filter[2];
		xf3 = filter[3];

		pos = (*dys >> MLIB_FILTER_SHIFT) & MLIB_FILTER_MASK;
		dys++;
		filter = (mlib_f32 *)((mlib_u8 *)filter_table + pos);
		yf0 = filter[0];
		yf1 = filter[1];
		yf2 = filter[2];
		yf3 = filter[3];

		line = (mlib_s16 *)lineAddr[ysrc] + xsrc * 2 + ch;

		SAT_S16(dp[xd[i] * 2], r0);

		r0 = COMPUTING_POINT(0, 2, 4, 6) * yf0;
		line += stride;
	}

	r0 += COMPUTING_POINT(0, 2, 4, 6) * yf1;
	line += stride;
	r0 += COMPUTING_POINT(0, 2, 4, 6) * yf2;
	line += stride;
	r0 += COMPUTING_POINT(0, 2, 4, 6) * yf3;

	SAT_S16(dp[xd[i] * 2], r0);

	if (ch < 1) {
		xs = pws->src_x;
		ys = pws->src_y;
		dxs = pws->dsrc_x;
		dys = pws->dsrc_y;
		ch++;
		dp++;
		goto next;
	}
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BC_S16_3(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	void *filter_table = (void *)pws->filter_table;
	mlib_s16 *line, *dp = (void *)dstData;
	mlib_d64 xf0, xf1, xf2, xf3, yf0, yf1, yf2, yf3, r0;
	mlib_s32 i, ch = 0, xsrc, ysrc, pos, stride =
	    (lineAddr[1] - lineAddr[0]) >> 1;
	mlib_s32 *xs = pws->src_x, *ys = pws->src_y, *xd = pws->dst_x;
	mlib_u16 *dxs = pws->dsrc_x, *dys = pws->dsrc_y;
	mlib_f32 *filter;

	if (n <= 0)
		return;

next:
	xsrc = *xs - 1;
	xs++;
	ysrc = *ys - 1;
	ys++;

	pos = (*dxs >> MLIB_FILTER_SHIFT) & MLIB_FILTER_MASK;
	dxs++;
	filter = (mlib_f32 *)((mlib_u8 *)filter_table + pos);
	xf0 = filter[0];
	xf1 = filter[1];
	xf2 = filter[2];
	xf3 = filter[3];

	pos = (*dys >> MLIB_FILTER_SHIFT) & MLIB_FILTER_MASK;
	dys++;
	filter = (mlib_f32 *)((mlib_u8 *)filter_table + pos);
	yf0 = filter[0];
	yf1 = filter[1];
	yf2 = filter[2];
	yf3 = filter[3];

	line = (mlib_s16 *)lineAddr[ysrc] + xsrc * 3 + ch;

	r0 = COMPUTING_POINT(0, 3, 6, 9) * yf0;
	line += stride;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
	for (i = 0; i < n - 1; i++) {

		r0 += COMPUTING_POINT(0, 3, 6, 9) * yf1;
		line += stride;
		r0 += COMPUTING_POINT(0, 3, 6, 9) * yf2;
		line += stride;
		r0 += COMPUTING_POINT(0, 3, 6, 9) * yf3;

		xsrc = *xs - 1;
		xs++;
		ysrc = *ys - 1;
		ys++;

		pos = (*dxs >> MLIB_FILTER_SHIFT) & MLIB_FILTER_MASK;
		dxs++;
		filter = (mlib_f32 *)((mlib_u8 *)filter_table + pos);
		xf0 = filter[0];
		xf1 = filter[1];
		xf2 = filter[2];
		xf3 = filter[3];

		pos = (*dys >> MLIB_FILTER_SHIFT) & MLIB_FILTER_MASK;
		dys++;
		filter = (mlib_f32 *)((mlib_u8 *)filter_table + pos);
		yf0 = filter[0];
		yf1 = filter[1];
		yf2 = filter[2];
		yf3 = filter[3];

		line = (mlib_s16 *)lineAddr[ysrc] + xsrc * 3 + ch;

		SAT_S16(dp[xd[i] * 3], r0);

		r0 = COMPUTING_POINT(0, 3, 6, 9) * yf0;
		line += stride;
	}

	r0 += COMPUTING_POINT(0, 3, 6, 9) * yf1;
	line += stride;
	r0 += COMPUTING_POINT(0, 3, 6, 9) * yf2;
	line += stride;
	r0 += COMPUTING_POINT(0, 3, 6, 9) * yf3;

	SAT_S16(dp[xd[i] * 3], r0);

	if (ch < 2) {
		xs = pws->src_x;
		ys = pws->src_y;
		dxs = pws->dsrc_x;
		dys = pws->dsrc_y;
		ch++;
		dp++;
		goto next;
	}
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BC_S16_4(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	void *filter_table = (void *)pws->filter_table;
	mlib_s16 *line, *dp = (void *)dstData;
	mlib_d64 xf0, xf1, xf2, xf3, yf0, yf1, yf2, yf3, r0;
	mlib_s32 i, ch = 0, xsrc, ysrc, pos, stride =
	    (lineAddr[1] - lineAddr[0]) >> 1;
	mlib_s32 *xs = pws->src_x, *ys = pws->src_y, *xd = pws->dst_x;
	mlib_u16 *dxs = pws->dsrc_x, *dys = pws->dsrc_y;
	mlib_f32 *filter;

	if (n <= 0)
		return;

next:
	xsrc = *xs - 1;
	xs++;
	ysrc = *ys - 1;
	ys++;

	pos = (*dxs >> MLIB_FILTER_SHIFT) & MLIB_FILTER_MASK;
	dxs++;
	filter = (mlib_f32 *)((mlib_u8 *)filter_table + pos);
	xf0 = filter[0];
	xf1 = filter[1];
	xf2 = filter[2];
	xf3 = filter[3];

	pos = (*dys >> MLIB_FILTER_SHIFT) & MLIB_FILTER_MASK;
	dys++;
	filter = (mlib_f32 *)((mlib_u8 *)filter_table + pos);
	yf0 = filter[0];
	yf1 = filter[1];
	yf2 = filter[2];
	yf3 = filter[3];

	line = (mlib_s16 *)lineAddr[ysrc] + xsrc * 4 + ch;

	r0 = COMPUTING_POINT(0, 4, 8, 12) * yf0;
	line += stride;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
	for (i = 0; i < n - 1; i++) {

		r0 += COMPUTING_POINT(0, 4, 8, 12) * yf1;
		line += stride;
		r0 += COMPUTING_POINT(0, 4, 8, 12) * yf2;
		line += stride;
		r0 += COMPUTING_POINT(0, 4, 8, 12) * yf3;

		xsrc = *xs - 1;
		xs++;
		ysrc = *ys - 1;
		ys++;

		pos = (*dxs >> MLIB_FILTER_SHIFT) & MLIB_FILTER_MASK;
		dxs++;
		filter = (mlib_f32 *)((mlib_u8 *)filter_table + pos);
		xf0 = filter[0];
		xf1 = filter[1];
		xf2 = filter[2];
		xf3 = filter[3];

		pos = (*dys >> MLIB_FILTER_SHIFT) & MLIB_FILTER_MASK;
		dys++;
		filter = (mlib_f32 *)((mlib_u8 *)filter_table + pos);
		yf0 = filter[0];
		yf1 = filter[1];
		yf2 = filter[2];
		yf3 = filter[3];

		line = (mlib_s16 *)lineAddr[ysrc] + xsrc * 4 + ch;

		SAT_S16(dp[xd[i] * 4], r0);

		r0 = COMPUTING_POINT(0, 4, 8, 12) * yf0;
		line += stride;
	}

	r0 += COMPUTING_POINT(0, 4, 8, 12) * yf1;
	line += stride;
	r0 += COMPUTING_POINT(0, 4, 8, 12) * yf2;
	line += stride;
	r0 += COMPUTING_POINT(0, 4, 8, 12) * yf3;

	SAT_S16(dp[xd[i] * 4], r0);

	if (ch < 3) {
		xs = pws->src_x;
		ys = pws->src_y;
		dxs = pws->dsrc_x;
		dys = pws->dsrc_y;
		ch++;
		dp++;
		goto next;
	}
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BC_U16_1(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	void *filter_table = (void *)pws->filter_table;
	mlib_u16 *line, *dp = (void *)dstData;
	mlib_d64 xf0, xf1, xf2, xf3, yf0, yf1, yf2, yf3, r0;
	mlib_s32 i, xsrc, ysrc, pos, stride = (lineAddr[1] - lineAddr[0]) >> 1;
	mlib_s32 *xs = pws->src_x, *ys = pws->src_y, *xd = pws->dst_x;
	mlib_u16 *dxs = pws->dsrc_x, *dys = pws->dsrc_y;
	mlib_f32 *filter;

	if (n <= 0)
		return;

	xsrc = *xs - 1;
	xs++;
	ysrc = *ys - 1;
	ys++;

	pos = (*dxs >> MLIB_FILTER_SHIFT) & MLIB_FILTER_MASK;
	dxs++;
	filter = (mlib_f32 *)((mlib_u8 *)filter_table + pos);
	xf0 = filter[0];
	xf1 = filter[1];
	xf2 = filter[2];
	xf3 = filter[3];

	pos = (*dys >> MLIB_FILTER_SHIFT) & MLIB_FILTER_MASK;
	dys++;
	filter = (mlib_f32 *)((mlib_u8 *)filter_table + pos);
	yf0 = filter[0];
	yf1 = filter[1];
	yf2 = filter[2];
	yf3 = filter[3];

	line = (mlib_u16 *)lineAddr[ysrc] + xsrc;

	r0 = COMPUTING_POINT(0, 1, 2, 3) * yf0;
	line += stride;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
	for (i = 0; i < n - 1; i++) {

		r0 += COMPUTING_POINT(0, 1, 2, 3) * yf1;
		line += stride;
		r0 += COMPUTING_POINT(0, 1, 2, 3) * yf2;
		line += stride;
		r0 += COMPUTING_POINT(0, 1, 2, 3) * yf3;

		xsrc = *xs - 1;
		xs++;
		ysrc = *ys - 1;
		ys++;

		pos = (*dxs >> MLIB_FILTER_SHIFT) & MLIB_FILTER_MASK;
		dxs++;
		filter = (mlib_f32 *)((mlib_u8 *)filter_table + pos);
		xf0 = filter[0];
		xf1 = filter[1];
		xf2 = filter[2];
		xf3 = filter[3];

		pos = (*dys >> MLIB_FILTER_SHIFT) & MLIB_FILTER_MASK;
		dys++;
		filter = (mlib_f32 *)((mlib_u8 *)filter_table + pos);
		yf0 = filter[0];
		yf1 = filter[1];
		yf2 = filter[2];
		yf3 = filter[3];

		line = (mlib_u16 *)lineAddr[ysrc] + xsrc;

		SAT_U16(dp[xd[i]], r0);

		r0 = COMPUTING_POINT(0, 1, 2, 3) * yf0;
		line += stride;
	}

	r0 += COMPUTING_POINT(0, 1, 2, 3) * yf1;
	line += stride;
	r0 += COMPUTING_POINT(0, 1, 2, 3) * yf2;
	line += stride;
	r0 += COMPUTING_POINT(0, 1, 2, 3) * yf3;

	SAT_U16(dp[xd[i]], r0);
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BC_U16_2(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	void *filter_table = (void *)pws->filter_table;
	mlib_u16 *line, *dp = (void *)dstData;
	mlib_d64 xf0, xf1, xf2, xf3, yf0, yf1, yf2, yf3, r0;
	mlib_s32 i, ch = 0, xsrc, ysrc, pos, stride =
	    (lineAddr[1] - lineAddr[0]) >> 1;
	mlib_s32 *xs = pws->src_x, *ys = pws->src_y, *xd = pws->dst_x;
	mlib_u16 *dxs = pws->dsrc_x, *dys = pws->dsrc_y;
	mlib_f32 *filter;

	if (n <= 0)
		return;

next:
	xsrc = *xs - 1;
	xs++;
	ysrc = *ys - 1;
	ys++;

	pos = (*dxs >> MLIB_FILTER_SHIFT) & MLIB_FILTER_MASK;
	dxs++;
	filter = (mlib_f32 *)((mlib_u8 *)filter_table + pos);
	xf0 = filter[0];
	xf1 = filter[1];
	xf2 = filter[2];
	xf3 = filter[3];

	pos = (*dys >> MLIB_FILTER_SHIFT) & MLIB_FILTER_MASK;
	dys++;
	filter = (mlib_f32 *)((mlib_u8 *)filter_table + pos);
	yf0 = filter[0];
	yf1 = filter[1];
	yf2 = filter[2];
	yf3 = filter[3];

	line = (mlib_u16 *)lineAddr[ysrc] + xsrc * 2 + ch;

	r0 = COMPUTING_POINT(0, 2, 4, 6) * yf0;
	line += stride;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
	for (i = 0; i < n - 1; i++) {

		r0 += COMPUTING_POINT(0, 2, 4, 6) * yf1;
		line += stride;
		r0 += COMPUTING_POINT(0, 2, 4, 6) * yf2;
		line += stride;
		r0 += COMPUTING_POINT(0, 2, 4, 6) * yf3;

		xsrc = *xs - 1;
		xs++;
		ysrc = *ys - 1;
		ys++;

		pos = (*dxs >> MLIB_FILTER_SHIFT) & MLIB_FILTER_MASK;
		dxs++;
		filter = (mlib_f32 *)((mlib_u8 *)filter_table + pos);
		xf0 = filter[0];
		xf1 = filter[1];
		xf2 = filter[2];
		xf3 = filter[3];

		pos = (*dys >> MLIB_FILTER_SHIFT) & MLIB_FILTER_MASK;
		dys++;
		filter = (mlib_f32 *)((mlib_u8 *)filter_table + pos);
		yf0 = filter[0];
		yf1 = filter[1];
		yf2 = filter[2];
		yf3 = filter[3];

		line = (mlib_u16 *)lineAddr[ysrc] + xsrc * 2 + ch;

		SAT_U16(dp[xd[i] * 2], r0);

		r0 = COMPUTING_POINT(0, 2, 4, 6) * yf0;
		line += stride;
	}

	r0 += COMPUTING_POINT(0, 2, 4, 6) * yf1;
	line += stride;
	r0 += COMPUTING_POINT(0, 2, 4, 6) * yf2;
	line += stride;
	r0 += COMPUTING_POINT(0, 2, 4, 6) * yf3;

	SAT_U16(dp[xd[i] * 2], r0);

	if (ch < 1) {
		xs = pws->src_x;
		ys = pws->src_y;
		dxs = pws->dsrc_x;
		dys = pws->dsrc_y;
		ch++;
		dp++;
		goto next;
	}
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BC_U16_3(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	void *filter_table = (void *)pws->filter_table;
	mlib_u16 *line, *dp = (void *)dstData;
	mlib_d64 xf0, xf1, xf2, xf3, yf0, yf1, yf2, yf3, r0;
	mlib_s32 i, ch = 0, xsrc, ysrc, pos, stride =
	    (lineAddr[1] - lineAddr[0]) >> 1;
	mlib_s32 *xs = pws->src_x, *ys = pws->src_y, *xd = pws->dst_x;
	mlib_u16 *dxs = pws->dsrc_x, *dys = pws->dsrc_y;
	mlib_f32 *filter;

	if (n <= 0)
		return;

next:
	xsrc = *xs - 1;
	xs++;
	ysrc = *ys - 1;
	ys++;

	pos = (*dxs >> MLIB_FILTER_SHIFT) & MLIB_FILTER_MASK;
	dxs++;
	filter = (mlib_f32 *)((mlib_u8 *)filter_table + pos);
	xf0 = filter[0];
	xf1 = filter[1];
	xf2 = filter[2];
	xf3 = filter[3];

	pos = (*dys >> MLIB_FILTER_SHIFT) & MLIB_FILTER_MASK;
	dys++;
	filter = (mlib_f32 *)((mlib_u8 *)filter_table + pos);
	yf0 = filter[0];
	yf1 = filter[1];
	yf2 = filter[2];
	yf3 = filter[3];

	line = (mlib_u16 *)lineAddr[ysrc] + xsrc * 3 + ch;

	r0 = COMPUTING_POINT(0, 3, 6, 9) * yf0;
	line += stride;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
	for (i = 0; i < n - 1; i++) {

		r0 += COMPUTING_POINT(0, 3, 6, 9) * yf1;
		line += stride;
		r0 += COMPUTING_POINT(0, 3, 6, 9) * yf2;
		line += stride;
		r0 += COMPUTING_POINT(0, 3, 6, 9) * yf3;

		xsrc = *xs - 1;
		xs++;
		ysrc = *ys - 1;
		ys++;

		pos = (*dxs >> MLIB_FILTER_SHIFT) & MLIB_FILTER_MASK;
		dxs++;
		filter = (mlib_f32 *)((mlib_u8 *)filter_table + pos);
		xf0 = filter[0];
		xf1 = filter[1];
		xf2 = filter[2];
		xf3 = filter[3];

		pos = (*dys >> MLIB_FILTER_SHIFT) & MLIB_FILTER_MASK;
		dys++;
		filter = (mlib_f32 *)((mlib_u8 *)filter_table + pos);
		yf0 = filter[0];
		yf1 = filter[1];
		yf2 = filter[2];
		yf3 = filter[3];

		line = (mlib_u16 *)lineAddr[ysrc] + xsrc * 3 + ch;

		SAT_U16(dp[xd[i] * 3], r0);

		r0 = COMPUTING_POINT(0, 3, 6, 9) * yf0;
		line += stride;
	}

	r0 += COMPUTING_POINT(0, 3, 6, 9) * yf1;
	line += stride;
	r0 += COMPUTING_POINT(0, 3, 6, 9) * yf2;
	line += stride;
	r0 += COMPUTING_POINT(0, 3, 6, 9) * yf3;

	SAT_U16(dp[xd[i] * 3], r0);

	if (ch < 2) {
		xs = pws->src_x;
		ys = pws->src_y;
		dxs = pws->dsrc_x;
		dys = pws->dsrc_y;
		ch++;
		dp++;
		goto next;
	}
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BC_U16_4(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS * pws,
    mlib_s32 n)
{
	void *filter_table = (void *)pws->filter_table;
	mlib_u16 *line, *dp = (void *)dstData;
	mlib_d64 xf0, xf1, xf2, xf3, yf0, yf1, yf2, yf3, r0;
	mlib_s32 i, ch = 0, xsrc, ysrc, pos, stride =
	    (lineAddr[1] - lineAddr[0]) >> 1;
	mlib_s32 *xs = pws->src_x, *ys = pws->src_y, *xd = pws->dst_x;
	mlib_u16 *dxs = pws->dsrc_x, *dys = pws->dsrc_y;
	mlib_f32 *filter;

	if (n <= 0)
		return;

next:
	xsrc = *xs - 1;
	xs++;
	ysrc = *ys - 1;
	ys++;

	pos = (*dxs >> MLIB_FILTER_SHIFT) & MLIB_FILTER_MASK;
	dxs++;
	filter = (mlib_f32 *)((mlib_u8 *)filter_table + pos);
	xf0 = filter[0];
	xf1 = filter[1];
	xf2 = filter[2];
	xf3 = filter[3];

	pos = (*dys >> MLIB_FILTER_SHIFT) & MLIB_FILTER_MASK;
	dys++;
	filter = (mlib_f32 *)((mlib_u8 *)filter_table + pos);
	yf0 = filter[0];
	yf1 = filter[1];
	yf2 = filter[2];
	yf3 = filter[3];

	line = (mlib_u16 *)lineAddr[ysrc] + xsrc * 4 + ch;

	r0 = COMPUTING_POINT(0, 4, 8, 12) * yf0;
	line += stride;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
	for (i = 0; i < n - 1; i++) {

		r0 += COMPUTING_POINT(0, 4, 8, 12) * yf1;
		line += stride;
		r0 += COMPUTING_POINT(0, 4, 8, 12) * yf2;
		line += stride;
		r0 += COMPUTING_POINT(0, 4, 8, 12) * yf3;

		xsrc = *xs - 1;
		xs++;
		ysrc = *ys - 1;
		ys++;

		pos = (*dxs >> MLIB_FILTER_SHIFT) & MLIB_FILTER_MASK;
		dxs++;
		filter = (mlib_f32 *)((mlib_u8 *)filter_table + pos);
		xf0 = filter[0];
		xf1 = filter[1];
		xf2 = filter[2];
		xf3 = filter[3];

		pos = (*dys >> MLIB_FILTER_SHIFT) & MLIB_FILTER_MASK;
		dys++;
		filter = (mlib_f32 *)((mlib_u8 *)filter_table + pos);
		yf0 = filter[0];
		yf1 = filter[1];
		yf2 = filter[2];
		yf3 = filter[3];

		line = (mlib_u16 *)lineAddr[ysrc] + xsrc * 4 + ch;

		SAT_U16(dp[xd[i] * 4], r0);

		r0 = COMPUTING_POINT(0, 4, 8, 12) * yf0;
		line += stride;
	}

	r0 += COMPUTING_POINT(0, 4, 8, 12) * yf1;
	line += stride;
	r0 += COMPUTING_POINT(0, 4, 8, 12) * yf2;
	line += stride;
	r0 += COMPUTING_POINT(0, 4, 8, 12) * yf3;

	SAT_U16(dp[xd[i] * 4], r0);

	if (ch < 3) {
		xs = pws->src_x;
		ys = pws->src_y;
		dxs = pws->dsrc_x;
		dys = pws->dsrc_y;
		ch++;
		dp++;
		goto next;
	}
}

/* *********************************************************** */
