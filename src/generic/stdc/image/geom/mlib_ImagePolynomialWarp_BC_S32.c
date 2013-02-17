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

#pragma ident	"@(#)mlib_ImagePolynomialWarp_BC_S32.c	9.2	07/10/09 SMI"

#include <mlib_image.h>
#include <mlib_ImagePolynomialWarp.h>

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING
#define	SAT32(out, in)	out = (mlib_s32)(in)

#else /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#define	SAT32(out, in)                                          \
	if (in >= MLIB_S32_MAX)                                 \
	    in = MLIB_S32_MAX;                                  \
	if (in <= MLIB_S32_MIN)                                 \
	    in = MLIB_S32_MIN;                                  \
	out = (mlib_s32)(in)

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#define	COMPUTING_POINT(a0, a1, a2, a3)                         \
	(line[a0] * xf0 + line[a1] * xf1 + line[a2] * xf2 +     \
	    line[a3] * xf3)

/* *********************************************************** */

#define	BC_CF0(x)	(-0.5 * (x) * (x) * (x) + 1.0 * (x) * (x) - 0.5 * (x))
#define	BC_CF1(x)	(1.5 * (x) * (x) * (x) - 2.5 * (x) * (x) + 1.0)

#define	BC_CF2(x)	(-1.5 * (x) * (x) * (x) + 2.0 * (x) * (x) + 0.5 * (x))

#define	BC_CF3(x)	(0.5 * (x) * (x) * (x) - 0.5 * (x) * (x))

/* *********************************************************** */

#define	BC2_CF0(x)	(-1.0 * (x) * (x) * (x) + 2.0 * (x) * (x) - 1.0 * (x))
#define	BC2_CF1(x)	(1.0 * (x) * (x) * (x) - 2.0 * (x) * (x) + 1.0)

#define	BC2_CF2(x)	(-1.0 * (x) * (x) * (x) + 1.0 * (x) * (x) + 1.0 * (x))

#define	BC2_CF3(x)	(1.0 * (x) * (x) * (x) - 1.0 * (x) * (x))

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BC_S32_1(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS *pws,
    mlib_s32 n)
{
	mlib_s32 *line, *dp = (void *)dstData;
	mlib_d64 *dsrc_x = pws->dsrc_x, *dsrc_y = pws->dsrc_y;
	mlib_d64 xf0, xf1, xf2, xf3, yf0, yf1, yf2, yf3, r0, dx, dy;
	mlib_s32 i, xsrc, ysrc, stride = (lineAddr[1] - lineAddr[0]) >> 2;
	mlib_s32 *xs = pws->src_x, *ys = pws->src_y, *xd = pws->dst_x;

	if (n <= 0)
		return;

	if (pws->flags) {

		dx = (*dsrc_x++);
		dy = (*dsrc_y++);
		xsrc = (*xs++);
		ysrc = (*ys++);

		xf0 = BC2_CF0(dx);
		xf1 = BC2_CF1(dx);
		xf2 = BC2_CF2(dx);
		xf3 = BC2_CF3(dx);

		yf0 = BC2_CF0(dy);
		yf1 = BC2_CF1(dy);
		yf2 = BC2_CF2(dy);
		yf3 = BC2_CF3(dy);

		line = (mlib_s32 *)lineAddr[ysrc - 1] + xsrc - 1;
		r0 = COMPUTING_POINT(0, 1, 2, 3) * yf0;
		line += stride;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
		for (i = 0; i < n - 1; i++) {
			dx = (*dsrc_x++);
			dy = (*dsrc_y++);
			xsrc = (*xs++);
			ysrc = (*ys++);

			r0 += COMPUTING_POINT(0, 1, 2, 3) * yf1;
			line += stride;
			r0 += COMPUTING_POINT(0, 1, 2, 3) * yf2;
			line += stride;
			r0 += COMPUTING_POINT(0, 1, 2, 3) * yf3;

			xf0 = BC2_CF0(dx);
			xf1 = BC2_CF1(dx);
			xf2 = BC2_CF2(dx);
			xf3 = BC2_CF3(dx);

			yf0 = BC2_CF0(dy);
			yf1 = BC2_CF1(dy);
			yf2 = BC2_CF2(dy);
			yf3 = BC2_CF3(dy);

			line = (mlib_s32 *)lineAddr[ysrc - 1] + xsrc - 1;
			SAT32(dp[xd[i]], r0);

			r0 = COMPUTING_POINT(0, 1, 2, 3) * yf0;
			line += stride;
		}

		r0 += COMPUTING_POINT(0, 1, 2, 3) * yf1;
		line += stride;
		r0 += COMPUTING_POINT(0, 1, 2, 3) * yf2;
		line += stride;
		r0 += COMPUTING_POINT(0, 1, 2, 3) * yf3;

		SAT32(dp[xd[i]], r0);
	} else {

		dx = (*dsrc_x++);
		dy = (*dsrc_y++);
		xsrc = (*xs++);
		ysrc = (*ys++);

		xf0 = BC_CF0(dx);
		xf1 = BC_CF1(dx);
		xf2 = BC_CF2(dx);
		xf3 = BC_CF3(dx);

		yf0 = BC_CF0(dy);
		yf1 = BC_CF1(dy);
		yf2 = BC_CF2(dy);
		yf3 = BC_CF3(dy);

		line = (mlib_s32 *)lineAddr[ysrc - 1] + xsrc - 1;
		r0 = COMPUTING_POINT(0, 1, 2, 3) * yf0;
		line += stride;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
		for (i = 0; i < n - 1; i++) {
			dx = (*dsrc_x++);
			dy = (*dsrc_y++);
			xsrc = (*xs++);
			ysrc = (*ys++);

			r0 += COMPUTING_POINT(0, 1, 2, 3) * yf1;
			line += stride;
			r0 += COMPUTING_POINT(0, 1, 2, 3) * yf2;
			line += stride;
			r0 += COMPUTING_POINT(0, 1, 2, 3) * yf3;

			xf0 = BC_CF0(dx);
			xf1 = BC_CF1(dx);
			xf2 = BC_CF2(dx);
			xf3 = BC_CF3(dx);

			yf0 = BC_CF0(dy);
			yf1 = BC_CF1(dy);
			yf2 = BC_CF2(dy);
			yf3 = BC_CF3(dy);

			line = (mlib_s32 *)lineAddr[ysrc - 1] + xsrc - 1;
			SAT32(dp[xd[i]], r0);

			r0 = COMPUTING_POINT(0, 1, 2, 3) * yf0;
			line += stride;
		}

		r0 += COMPUTING_POINT(0, 1, 2, 3) * yf1;
		line += stride;
		r0 += COMPUTING_POINT(0, 1, 2, 3) * yf2;
		line += stride;
		r0 += COMPUTING_POINT(0, 1, 2, 3) * yf3;

		SAT32(dp[xd[i]], r0);
	}
}

/* *********************************************************** */

#define	CALC_COEFFS(BC_NAME)                                    \
	for (i = 0; i < n; i++) {                               \
	    dx = (*dsrc_x++);                                   \
	    dy = (*dsrc_y++);                                   \
	    (*coeff++) = BC_NAME##_CF0(dx);                     \
	    (*coeff++) = BC_NAME##_CF1(dx);                     \
	    (*coeff++) = BC_NAME##_CF2(dx);                     \
	    (*coeff++) = BC_NAME##_CF3(dx);                     \
	    (*coeff++) = BC_NAME##_CF0(dy);                     \
	    (*coeff++) = BC_NAME##_CF1(dy);                     \
	    (*coeff++) = BC_NAME##_CF2(dy);                     \
	    (*coeff++) = BC_NAME##_CF3(dy);                     \
	}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BC_S32_2(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS *pws,
    mlib_s32 n)
{
	mlib_s32 *line, *dp = (void *)dstData;
	mlib_d64 *coeff = pws->buffer, *dsrc_x = pws->dsrc_x, *dsrc_y =
	    pws->dsrc_y;
	mlib_d64 xf0, xf1, xf2, xf3, yf0, yf1, yf2, yf3, r0, dx, dy;
	mlib_s32 i, X, xsrc, ysrc, ch = 0, stride =
	    (lineAddr[1] - lineAddr[0]) >> 2;
	mlib_s32 *xs = pws->src_x, *ys = pws->src_y, *xd = pws->dst_x;

	if (n <= 0)
		return;

	if (pws->flags) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
		CALC_COEFFS(BC2);
	} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
		CALC_COEFFS(BC);
	}

	coeff = pws->buffer;

next:

	X = (*xs++);

	xsrc = (X - 1) * 2 + ch;
	ysrc = (*ys++);

	xf0 = (*coeff++);
	xf1 = (*coeff++);
	xf2 = (*coeff++);
	xf3 = (*coeff++);

	yf0 = (*coeff++);
	yf1 = (*coeff++);
	yf2 = (*coeff++);
	yf3 = (*coeff++);

	line = (mlib_s32 *)lineAddr[ysrc - 1] + xsrc;
	r0 = COMPUTING_POINT(0, 2, 4, 6) * yf0;
	line += stride;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
	for (i = 0; i < n - 1; i++) {

		X = (*xs++);

		xsrc = (X - 1) * 2 + ch;
		ysrc = (*ys++);

		r0 += COMPUTING_POINT(0, 2, 4, 6) * yf1;
		line += stride;
		r0 += COMPUTING_POINT(0, 2, 4, 6) * yf2;
		line += stride;
		r0 += COMPUTING_POINT(0, 2, 4, 6) * yf3;

		xf0 = (*coeff++);
		xf1 = (*coeff++);
		xf2 = (*coeff++);
		xf3 = (*coeff++);

		yf0 = (*coeff++);
		yf1 = (*coeff++);
		yf2 = (*coeff++);
		yf3 = (*coeff++);

		line = (mlib_s32 *)lineAddr[ysrc - 1] + xsrc;
		SAT32(dp[xd[i] * 2], r0);

		r0 = COMPUTING_POINT(0, 2, 4, 6) * yf0;
		line += stride;
	}

	r0 += COMPUTING_POINT(0, 2, 4, 6) * yf1;
	line += stride;
	r0 += COMPUTING_POINT(0, 2, 4, 6) * yf2;
	line += stride;
	r0 += COMPUTING_POINT(0, 2, 4, 6) * yf3;

	SAT32(dp[xd[i] * 2], r0);

	if (ch < 1) {
		xs = pws->src_x;
		ys = pws->src_y;
		coeff = pws->buffer;
		ch++;
		dp++;
		goto next;
	}
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BC_S32_3(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS *pws,
    mlib_s32 n)
{
	mlib_s32 *line, *dp = (void *)dstData;
	mlib_d64 *coeff = pws->buffer, *dsrc_x = pws->dsrc_x, *dsrc_y =
	    pws->dsrc_y;
	mlib_d64 xf0, xf1, xf2, xf3, yf0, yf1, yf2, yf3, r0, dx, dy;
	mlib_s32 i, X, xsrc, ysrc, ch = 0, stride =
	    (lineAddr[1] - lineAddr[0]) >> 2;
	mlib_s32 *xs = pws->src_x, *ys = pws->src_y, *xd = pws->dst_x;

	if (n <= 0)
		return;

	if (pws->flags) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
		CALC_COEFFS(BC2);
	} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
		CALC_COEFFS(BC);
	}

	coeff = pws->buffer;

next:

	X = (*xs++);

	xsrc = (X - 1) * 3 + ch;
	ysrc = (*ys++);

	xf0 = (*coeff++);
	xf1 = (*coeff++);
	xf2 = (*coeff++);
	xf3 = (*coeff++);

	yf0 = (*coeff++);
	yf1 = (*coeff++);
	yf2 = (*coeff++);
	yf3 = (*coeff++);

	line = (mlib_s32 *)lineAddr[ysrc - 1] + xsrc;
	r0 = COMPUTING_POINT(0, 3, 6, 9) * yf0;
	line += stride;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
	for (i = 0; i < n - 1; i++) {
		X = (*xs++);
		ysrc = (*ys++);

		xsrc = (X - 1) * 3 + ch;

		r0 += COMPUTING_POINT(0, 3, 6, 9) * yf1;
		line += stride;
		r0 += COMPUTING_POINT(0, 3, 6, 9) * yf2;
		line += stride;
		r0 += COMPUTING_POINT(0, 3, 6, 9) * yf3;

		xf0 = (*coeff++);
		xf1 = (*coeff++);
		xf2 = (*coeff++);
		xf3 = (*coeff++);

		yf0 = (*coeff++);
		yf1 = (*coeff++);
		yf2 = (*coeff++);
		yf3 = (*coeff++);

		line = (mlib_s32 *)lineAddr[ysrc - 1] + xsrc;
		SAT32(dp[xd[i] * 3], r0);

		r0 = COMPUTING_POINT(0, 3, 6, 9) * yf0;
		line += stride;
	}

	r0 += COMPUTING_POINT(0, 3, 6, 9) * yf1;
	line += stride;
	r0 += COMPUTING_POINT(0, 3, 6, 9) * yf2;
	line += stride;
	r0 += COMPUTING_POINT(0, 3, 6, 9) * yf3;

	SAT32(dp[xd[i] * 3], r0);

	if (ch < 2) {
		coeff = pws->buffer;
		xs = pws->src_x;
		ys = pws->src_y;
		ch++;
		dp++;
		goto next;
	}
}

/* *********************************************************** */

void
mlib_ImagePolynomialWarp_BC_S32_4(
    mlib_u8 *dstData,
    const mlib_u8 **lineAddr,
    mlib_PWS *pws,
    mlib_s32 n)
{
	mlib_s32 *line, *dp = (void *)dstData;
	mlib_d64 *coeff = pws->buffer, *dsrc_x = pws->dsrc_x, *dsrc_y =
	    pws->dsrc_y;
	mlib_d64 xf0, xf1, xf2, xf3, yf0, yf1, yf2, yf3, r0, dx, dy;
	mlib_s32 i, X, xsrc, ysrc, ch = 0, stride =
	    (lineAddr[1] - lineAddr[0]) >> 2;
	mlib_s32 *xs = pws->src_x, *ys = pws->src_y, *xd = pws->dst_x;

	if (n <= 0)
		return;

	if (pws->flags) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
		CALC_COEFFS(BC2);
	} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
		CALC_COEFFS(BC);
	}

	coeff = pws->buffer;

next:

	X = (*xs++);

	xsrc = (X - 1) * 4 + ch;
	ysrc = (*ys++);

	xf0 = (*coeff++);
	xf1 = (*coeff++);
	xf2 = (*coeff++);
	xf3 = (*coeff++);

	yf0 = (*coeff++);
	yf1 = (*coeff++);
	yf2 = (*coeff++);
	yf3 = (*coeff++);

	line = (mlib_s32 *)lineAddr[ysrc - 1] + xsrc;
	r0 = COMPUTING_POINT(0, 4, 8, 12) * yf0;
	line += stride;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma nomemorydepend
#endif /* __SUNPRO_C */
	for (i = 0; i < n - 1; i++) {

		X = (*xs++);

		xsrc = (X - 1) * 4 + ch;
		ysrc = (*ys++);

		r0 += COMPUTING_POINT(0, 4, 8, 12) * yf1;
		line += stride;
		r0 += COMPUTING_POINT(0, 4, 8, 12) * yf2;
		line += stride;
		r0 += COMPUTING_POINT(0, 4, 8, 12) * yf3;

		xf0 = (*coeff++);
		xf1 = (*coeff++);
		xf2 = (*coeff++);
		xf3 = (*coeff++);

		yf0 = (*coeff++);
		yf1 = (*coeff++);
		yf2 = (*coeff++);
		yf3 = (*coeff++);

		line = (mlib_s32 *)lineAddr[ysrc - 1] + xsrc;
		SAT32(dp[xd[i] * 4], r0);

		r0 = COMPUTING_POINT(0, 4, 8, 12) * yf0;
		line += stride;
	}

	r0 += COMPUTING_POINT(0, 4, 8, 12) * yf1;
	line += stride;
	r0 += COMPUTING_POINT(0, 4, 8, 12) * yf2;
	line += stride;
	r0 += COMPUTING_POINT(0, 4, 8, 12) * yf3;

	SAT32(dp[xd[i] * 4], r0);

	if (ch < 3) {
		xs = pws->src_x;
		ys = pws->src_y;
		coeff = pws->buffer;
		ch++;
		dp++;
		goto next;
	}
}

/* *********************************************************** */
