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

#pragma ident	"@(#)mlib_ImageConv_D64ext.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *   Internal functions for mlib_ImageConv* on D64/F32 type and
 *   MLIB_EDGE_SRC_EXTEND mask
 *
 */

#include <mlib_image.h>
#include <mlib_ImageConv.h>

/* *********************************************************** */

/*
 *  This define switches between functions of MLIB_DOUBLE and MLIB_FLOAT types:
 *  Files mlib_ImageConv_D64ext.c and mlib_ImageConv_F32ext.c
 */

#define	TYPE_DOUBLE

/* *********************************************************** */

#ifdef TYPE_DOUBLE

#define	FUNC_NAME(KERN)	mlib_conv##KERN##ext_d64

#define	DTYPE	mlib_d64

#else /* TYPE_DOUBLE */

#define	FUNC_NAME(KERN)	mlib_conv##KERN##ext_f32

#define	DTYPE	mlib_f32

#endif /* TYPE_DOUBLE */

/* *********************************************************** */

#define	PARAM                                                    \
	mlib_image *dst, const mlib_image *src, mlib_s32 dx_l,   \
	mlib_s32 dx_r, mlib_s32 dy_t, mlib_s32 dy_b,             \
	const mlib_d64 *kern, mlib_s32 cmask

#define	CONV_FUNC(KERN)	FUNC_NAME(KERN)(PARAM)

/* *********************************************************** */

#define	KSIZE1	(KSIZE - 1)

/* *********************************************************** */

#define	GET_SRC_DST_PARAMETERS(type)                               \
	mlib_s32 hgt = mlib_ImageGetHeight(src);                   \
	mlib_s32 wid = mlib_ImageGetWidth(src);                    \
	mlib_s32 sll = mlib_ImageGetStride(src) / sizeof (type);   \
	mlib_s32 dll = mlib_ImageGetStride(dst) / sizeof (type);   \
	type *adr_src = mlib_ImageGetData(src);                    \
	type *adr_dst = mlib_ImageGetData(dst);                    \
	mlib_s32 chan1 = mlib_ImageGetChannels(src);               \
	mlib_s32 chan2 = chan1 + chan1

/* *********************************************************** */

#define	DEF_VARS(type)                                          \
	GET_SRC_DST_PARAMETERS(type);                           \
	type *sl, *sp0, *sp1;                                   \
	type *dl, *dp;                                          \
	mlib_s32 i, j, c

/* *********************************************************** */

#undef  KSIZE
#define	KSIZE	2

mlib_status
CONV_FUNC(2x2)
{
	DEF_VARS(DTYPE);
	mlib_s32 chan3 = chan1 + chan2;
	mlib_s32 chan4 = chan3 + chan1;
	DTYPE k0, k1, k2, k3;
	DTYPE p00, p01, p02, p03, p04, p10, p11, p12, p13, p14;
	DTYPE *sp_1;

/* keep kernel in regs */
	k0 = (DTYPE) kern[0];
	k1 = (DTYPE) kern[1];
	k2 = (DTYPE) kern[2];
	k3 = (DTYPE) kern[3];

	for (c = 0; c < chan1; c++) {
		if (!(cmask & (1 << (chan1 - 1 - c))))
			continue;

		dl = adr_dst + c;
		sl = adr_src + c;

		sp_1 = sl;

		if ((hgt - dy_b) > 0)
			sl += sll;

		for (j = 0; j < hgt; j++) {
			dp = dl;
			sp0 = sp_1;
			sp_1 = sl;

			sp1 = sp_1;

			p04 = sp0[0];
			p14 = sp1[0];

			sp0 += chan1;
			sp1 += chan1;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (wid - dx_r - 4); i += 4) {
				p00 = p04;
				p10 = p14;

				p01 = sp0[0];
				p11 = sp1[0];
				p02 = sp0[chan1];
				p12 = sp1[chan1];
				p03 = sp0[chan2];
				p13 = sp1[chan2];
				p04 = sp0[chan3];
				p14 = sp1[chan3];

				dp[0] =
				    p00 * k0 + p01 * k1 + p10 * k2 + p11 * k3;
				dp[chan1] =
				    p01 * k0 + p02 * k1 + p11 * k2 + p12 * k3;
				dp[chan2] =
				    p02 * k0 + p03 * k1 + p12 * k2 + p13 * k3;
				dp[chan3] =
				    p03 * k0 + p04 * k1 + p13 * k2 + p14 * k3;

				dp += chan4;
				sp0 += chan4;
				sp1 += chan4;
			}

			p01 = p04;
			p11 = p14;

			for (; i < (wid - dx_r); i++) {
				p00 = p01;
				p10 = p11;
				p01 = sp0[0];
				p11 = sp1[0];
				dp[0] =
				    p00 * k0 + p01 * k1 + p10 * k2 + p11 * k3;

				dp += chan1;
				sp0 += chan1;
				sp1 += chan1;
			}

			sp0 -= chan1;
			sp1 -= chan1;

			for (; i < wid; i++) {
				p00 = p01;
				p10 = p11;
				p01 = sp0[0];
				p11 = sp1[0];
				dp[0] =
				    p00 * k0 + p01 * k1 + p10 * k2 + p11 * k3;

				dp += chan1;
			}

			if (j < hgt - dy_b - 1)
				sl += sll;
			dl += dll;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef  KSIZE
#define	KSIZE	3

mlib_status
CONV_FUNC(3x3)
{
	DEF_VARS(DTYPE);
	DTYPE *sp2;
	DTYPE k0, k1, k2, k3, k4, k5, k6, k7, k8;
	DTYPE p02, p03, p12, p13, p22, p23;
	DTYPE *sp_1, *sp_2;
	mlib_s32 delta_chan;

/* keep kernel in regs */
	k0 = (DTYPE) kern[0];
	k1 = (DTYPE) kern[1];
	k2 = (DTYPE) kern[2];
	k3 = (DTYPE) kern[3];
	k4 = (DTYPE) kern[4];
	k5 = (DTYPE) kern[5];
	k6 = (DTYPE) kern[6];
	k7 = (DTYPE) kern[7];
	k8 = (DTYPE) kern[8];

	delta_chan = 0;

	if ((1 > dx_l) && (1 < wid + KSIZE1 - dx_r))
		delta_chan = chan1;

	for (c = 0; c < chan1; c++) {
		if (!(cmask & (1 << (chan1 - 1 - c))))
			continue;

		sl = adr_src + c;
		dl = adr_dst + c;

		sp_1 = sl;

		if ((1 > dy_t) && (1 < hgt + KSIZE1 - dy_b))
			sl += sll;
		sp_2 = sl;

		if ((hgt - dy_b) > 0)
			sl += sll;

		for (j = 0; j < hgt; j++) {
			DTYPE s0, s1;

			dp = dl;
			sp0 = sp_1;
			sp_1 = sp_2;
			sp_2 = sl;

			sp1 = sp_1;
			sp2 = sp_2;

			p02 = sp0[0];
			p12 = sp1[0];
			p22 = sp2[0];

			p03 = sp0[delta_chan];
			p13 = sp1[delta_chan];
			p23 = sp2[delta_chan];

			s0 = p02 * k0 + p03 * k1 + p12 * k3 + p13 * k4 +
			    p22 * k6 + p23 * k7;
			s1 = p03 * k0 + p13 * k3 + p23 * k6;

			sp0 += (chan1 + delta_chan);
			sp1 += (chan1 + delta_chan);
			sp2 += (chan1 + delta_chan);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (wid - dx_r - 2); i += 2) {
				p02 = sp0[0];
				p12 = sp1[0];
				p22 = sp2[0];
				p03 = sp0[chan1];
				p13 = sp1[chan1];
				p23 = sp2[chan1];

				dp[0] = s0 + p02 * k2 + p12 * k5 + p22 * k8;
				dp[chan1] =
				    s1 + p02 * k1 + p03 * k2 + p12 * k4 +
				    p13 * k5 + p22 * k7 + p23 * k8;

				s0 = p02 * k0 + p03 * k1 + p12 * k3 + p13 * k4 +
				    p22 * k6 + p23 * k7;
				s1 = p03 * k0 + p13 * k3 + p23 * k6;

				sp0 += chan2;
				sp1 += chan2;
				sp2 += chan2;
				dp += chan2;
			}

			p02 = p03;
			p12 = p13;
			p22 = p23;

			for (; i < wid - dx_r; i++) {
				p03 = sp0[0];
				p13 = sp1[0];
				p23 = sp2[0];
				dp[0] = s0 + p03 * k2 + p13 * k5 + p23 * k8;
				s0 = p02 * k0 + p03 * k1 + p12 * k3 + p13 * k4 +
				    p22 * k6 + p23 * k7;
				p02 = p03;
				p12 = p13;
				p22 = p23;
				sp0 += chan1;
				sp1 += chan1;
				sp2 += chan1;
				dp += chan1;
			}

			sp0 -= chan1;
			sp1 -= chan1;
			sp2 -= chan1;

			for (; i < wid; i++) {
				p03 = sp0[0];
				p13 = sp1[0];
				p23 = sp2[0];
				dp[0] = s0 + p03 * k2 + p13 * k5 + p23 * k8;
				s0 = p02 * k0 + p03 * k1 + p12 * k3 + p13 * k4 +
				    p22 * k6 + p23 * k7;
				p02 = p03;
				p12 = p13;
				p22 = p23;
				dp += chan1;
			}

			if (j < hgt - dy_b - 1)
				sl += sll;
			dl += dll;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef  KSIZE
#define	KSIZE	4

mlib_status
CONV_FUNC(4x4)
{
	DTYPE k0, k1, k2, k3, k4, k5, k6, k7;
	DTYPE p00, p01, p02, p03, p04, p10, p11, p12, p13, p14;
	DTYPE *sp_1, *sp_2, *sp_3;
	mlib_s32 delta_chan1, delta_chan2;

	DEF_VARS(DTYPE);
	mlib_s32 chan3;

	if ((1 > dx_l) && (1 < wid + KSIZE1 - dx_r))
		delta_chan1 = chan1;
	else
		delta_chan1 = 0;

	if ((2 > dx_l) && (2 < wid + KSIZE1 - dx_r))
		delta_chan2 = delta_chan1 + chan1;
	else
		delta_chan2 = delta_chan1;

	chan3 = chan1 + delta_chan2;

	for (c = 0; c < chan1; c++) {
		if (!(cmask & (1 << (chan1 - 1 - c))))
			continue;

		sl = adr_src + c;
		dl = adr_dst + c;

		sp_1 = sl;

		if ((1 > dy_t) && (1 < hgt + KSIZE1 - dy_b))
			sl += sll;
		sp_2 = sl;

		if ((2 > dy_t) && (2 < hgt + KSIZE1 - dy_b))
			sl += sll;
		sp_3 = sl;

		if ((hgt - dy_b) > 0)
			sl += sll;

		for (j = 0; j < hgt; j++) {
			dp = dl;
			sp0 = sp_1;
			sp_1 = sp_2;
			sp_2 = sp_3;
			sp_3 = sl;

			sp1 = sp_1;

/*
 *  First loop on two first lines of kernel
 */

			k0 = (DTYPE) kern[0];
			k1 = (DTYPE) kern[1];
			k2 = (DTYPE) kern[2];
			k3 = (DTYPE) kern[3];
			k4 = (DTYPE) kern[4];
			k5 = (DTYPE) kern[5];
			k6 = (DTYPE) kern[6];
			k7 = (DTYPE) kern[7];

			p02 = sp0[0];
			p12 = sp1[0];
			p03 = sp0[delta_chan1];
			p13 = sp1[delta_chan1];
			p04 = sp0[delta_chan2];
			p14 = sp1[delta_chan2];

			sp0 += chan3;
			sp1 += chan3;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (wid - dx_r - 2); i += 2) {
				p00 = p02;
				p10 = p12;
				p01 = p03;
				p11 = p13;
				p02 = p04;
				p12 = p14;

				p03 = sp0[0];
				p13 = sp1[0];
				p04 = sp0[chan1];
				p14 = sp1[chan1];

				dp[0] =
				    (p00 * k0 + p01 * k1 + p02 * k2 + p03 * k3 +
				    p10 * k4 + p11 * k5 + p12 * k6 + p13 * k7);
				dp[chan1] =
				    (p01 * k0 + p02 * k1 + p03 * k2 + p04 * k3 +
				    p11 * k4 + p12 * k5 + p13 * k6 + p14 * k7);

				sp0 += chan2;
				sp1 += chan2;
				dp += chan2;
			}

			p01 = p02;
			p02 = p03;
			p03 = p04;
			p11 = p12;
			p12 = p13;
			p13 = p14;

			for (; i < wid - dx_r; i++) {
				p00 = p01;
				p10 = p11;
				p01 = p02;
				p11 = p12;
				p02 = p03;
				p12 = p13;

				p03 = sp0[0];
				p13 = sp1[0];

				dp[0] =
				    (p00 * k0 + p01 * k1 + p02 * k2 + p03 * k3 +
				    p10 * k4 + p11 * k5 + p12 * k6 + p13 * k7);

				sp0 += chan1;
				sp1 += chan1;
				dp += chan1;
			}

			sp0 -= chan1;
			sp1 -= chan1;

			for (; i < wid; i++) {
				p00 = p01;
				p10 = p11;
				p01 = p02;
				p11 = p12;
				p02 = p03;
				p12 = p13;

				p03 = sp0[0];
				p13 = sp1[0];

				dp[0] =
				    (p00 * k0 + p01 * k1 + p02 * k2 + p03 * k3 +
				    p10 * k4 + p11 * k5 + p12 * k6 + p13 * k7);

				dp += chan1;
			}

/*
 *  Second loop on two last lines of kernel
 */
			sp0 = sp_2;
			sp1 = sp_3;
			dp = dl;

			k0 = (DTYPE) kern[8];
			k1 = (DTYPE) kern[9];
			k2 = (DTYPE) kern[10];
			k3 = (DTYPE) kern[11];
			k4 = (DTYPE) kern[12];
			k5 = (DTYPE) kern[13];
			k6 = (DTYPE) kern[14];
			k7 = (DTYPE) kern[15];

			p02 = sp0[0];
			p12 = sp1[0];
			p03 = sp0[delta_chan1];
			p13 = sp1[delta_chan1];
			p04 = sp0[delta_chan2];
			p14 = sp1[delta_chan2];

			sp0 += chan3;
			sp1 += chan3;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (wid - dx_r - 2); i += 2) {
				p00 = p02;
				p10 = p12;
				p01 = p03;
				p11 = p13;
				p02 = p04;
				p12 = p14;

				p03 = sp0[0];
				p13 = sp1[0];
				p04 = sp0[chan1];
				p14 = sp1[chan1];

				dp[0] +=
				    (p00 * k0 + p01 * k1 + p02 * k2 + p03 * k3 +
				    p10 * k4 + p11 * k5 + p12 * k6 + p13 * k7);
				dp[chan1] +=
				    (p01 * k0 + p02 * k1 + p03 * k2 + p04 * k3 +
				    p11 * k4 + p12 * k5 + p13 * k6 + p14 * k7);

				sp0 += chan2;
				sp1 += chan2;
				dp += chan2;
			}

			p01 = p02;
			p02 = p03;
			p03 = p04;
			p11 = p12;
			p12 = p13;
			p13 = p14;

			for (; i < wid - dx_r; i++) {
				p00 = p01;
				p10 = p11;
				p01 = p02;
				p11 = p12;
				p02 = p03;
				p12 = p13;

				p03 = sp0[0];
				p13 = sp1[0];

				dp[0] +=
				    (p00 * k0 + p01 * k1 + p02 * k2 + p03 * k3 +
				    p10 * k4 + p11 * k5 + p12 * k6 + p13 * k7);

				sp0 += chan1;
				sp1 += chan1;
				dp += chan1;
			}

			sp0 -= chan1;
			sp1 -= chan1;

			for (; i < wid; i++) {
				p00 = p01;
				p10 = p11;
				p01 = p02;
				p11 = p12;
				p02 = p03;
				p12 = p13;

				p03 = sp0[0];
				p13 = sp1[0];

				dp[0] +=
				    (p00 * k0 + p01 * k1 + p02 * k2 + p03 * k3 +
				    p10 * k4 + p11 * k5 + p12 * k6 + p13 * k7);

				dp += chan1;
			}

/* next line */

			if (j < hgt - dy_b - 1)
				sl += sll;
			dl += dll;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef  KSIZE
#define	KSIZE	5

mlib_status
CONV_FUNC(5x5)
{
	DTYPE k0, k1, k2, k3, k4, k5, k6, k7, k8, k9;
	DTYPE p00, p01, p02, p03, p04, p05, p10, p11, p12, p13, p14, p15;

	DEF_VARS(DTYPE);
	DTYPE *sp_1, *sp_2, *sp_3, *sp_4;
	mlib_s32 delta_chan1, delta_chan2, delta_chan3;
	mlib_s32 chan4;

	if ((1 > dx_l) && (1 < wid + KSIZE1 - dx_r))
		delta_chan1 = chan1;
	else
		delta_chan1 = 0;

	if ((2 > dx_l) && (2 < wid + KSIZE1 - dx_r))
		delta_chan2 = delta_chan1 + chan1;
	else
		delta_chan2 = delta_chan1;

	if ((3 > dx_l) && (3 < wid + KSIZE1 - dx_r))
		delta_chan3 = delta_chan2 + chan1;
	else
		delta_chan3 = delta_chan2;

	chan4 = chan1 + delta_chan3;

	for (c = 0; c < chan1; c++) {
		if (!(cmask & (1 << (chan1 - 1 - c))))
			continue;

		sl = adr_src + c;
		dl = adr_dst + c;

		sp_1 = sl;

		if ((1 > dy_t) && (1 < hgt + KSIZE1 - dy_b))
			sl += sll;
		sp_2 = sl;

		if ((2 > dy_t) && (2 < hgt + KSIZE1 - dy_b))
			sl += sll;
		sp_3 = sl;

		if ((3 > dy_t) && (3 < hgt + KSIZE1 - dy_b))
			sl += sll;
		sp_4 = sl;

		if ((hgt - dy_b) > 0)
			sl += sll;

		for (j = 0; j < hgt; j++) {
			dp = dl;
			sp0 = sp_1;
			sp_1 = sp_2;
			sp_2 = sp_3;
			sp_3 = sp_4;
			sp_4 = sl;

			sp1 = sp_1;

/*
 *  First loop
 */

			k0 = (DTYPE) kern[0];
			k1 = (DTYPE) kern[1];
			k2 = (DTYPE) kern[2];
			k3 = (DTYPE) kern[3];
			k4 = (DTYPE) kern[4];
			k5 = (DTYPE) kern[5];
			k6 = (DTYPE) kern[6];
			k7 = (DTYPE) kern[7];
			k8 = (DTYPE) kern[8];
			k9 = (DTYPE) kern[9];

			p02 = sp0[0];
			p12 = sp1[0];
			p03 = sp0[delta_chan1];
			p13 = sp1[delta_chan1];
			p04 = sp0[delta_chan2];
			p14 = sp1[delta_chan2];
			p05 = sp0[delta_chan3];
			p15 = sp1[delta_chan3];

			sp0 += chan4;
			sp1 += chan4;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (wid - dx_r - 2); i += 2) {
				p00 = p02;
				p10 = p12;
				p01 = p03;
				p11 = p13;
				p02 = p04;
				p12 = p14;
				p03 = p05;
				p13 = p15;

				p04 = sp0[0];
				p14 = sp1[0];
				p05 = sp0[chan1];
				p15 = sp1[chan1];

				dp[0] =
				    (p00 * k0 + p01 * k1 + p02 * k2 + p03 * k3 +
				    p04 * k4 + p10 * k5 + p11 * k6 + p12 * k7 +
				    p13 * k8 + p14 * k9);
				dp[chan1] =
				    (p01 * k0 + p02 * k1 + p03 * k2 + p04 * k3 +
				    p05 * k4 + p11 * k5 + p12 * k6 + p13 * k7 +
				    p14 * k8 + p15 * k9);

				sp0 += chan2;
				sp1 += chan2;
				dp += chan2;
			}

			p01 = p02;
			p02 = p03;
			p03 = p04;
			p04 = p05;
			p11 = p12;
			p12 = p13;
			p13 = p14;
			p14 = p15;

			for (; i < wid - dx_r; i++) {
				p00 = p01;
				p10 = p11;
				p01 = p02;
				p11 = p12;
				p02 = p03;
				p12 = p13;
				p03 = p04;
				p13 = p14;

				p04 = sp0[0];
				p14 = sp1[0];

				dp[0] =
				    (p00 * k0 + p01 * k1 + p02 * k2 + p03 * k3 +
				    p04 * k4 + p10 * k5 + p11 * k6 + p12 * k7 +
				    p13 * k8 + p14 * k9);

				sp0 += chan1;
				sp1 += chan1;
				dp += chan1;
			}

			sp0 -= chan1;
			sp1 -= chan1;

			for (; i < wid; i++) {
				p00 = p01;
				p10 = p11;
				p01 = p02;
				p11 = p12;
				p02 = p03;
				p12 = p13;
				p03 = p04;
				p13 = p14;

				p04 = sp0[0];
				p14 = sp1[0];

				dp[0] =
				    (p00 * k0 + p01 * k1 + p02 * k2 + p03 * k3 +
				    p04 * k4 + p10 * k5 + p11 * k6 + p12 * k7 +
				    p13 * k8 + p14 * k9);

				dp += chan1;
			}

/*
 *  Second loop
 */
			dp = dl;
			sp0 = sp_2;
			sp1 = sp_3;

			k0 = (DTYPE) kern[10];
			k1 = (DTYPE) kern[11];
			k2 = (DTYPE) kern[12];
			k3 = (DTYPE) kern[13];
			k4 = (DTYPE) kern[14];
			k5 = (DTYPE) kern[15];
			k6 = (DTYPE) kern[16];
			k7 = (DTYPE) kern[17];
			k8 = (DTYPE) kern[18];
			k9 = (DTYPE) kern[19];

			p00 = sp0[0];
			p10 = sp1[0];
			p01 = sp0[delta_chan1];
			p11 = sp1[delta_chan1];
			p02 = sp0[delta_chan2];
			p12 = sp1[delta_chan2];
			p03 = sp0[delta_chan3];
			p13 = sp1[delta_chan3];

			sp0 += chan4;
			sp1 += chan4;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (wid - dx_r - 2); i += 2) {
				p04 = sp0[0];
				p14 = sp1[0];
				p05 = sp0[chan1];
				p15 = sp1[chan1];

				dp[0] +=
				    (p00 * k0 + p01 * k1 + p02 * k2 + p03 * k3 +
				    p04 * k4 + p10 * k5 + p11 * k6 + p12 * k7 +
				    p13 * k8 + p14 * k9);
				dp[chan1] +=
				    (p01 * k0 + p02 * k1 + p03 * k2 + p04 * k3 +
				    p05 * k4 + p11 * k5 + p12 * k6 + p13 * k7 +
				    p14 * k8 + p15 * k9);

				sp0 += chan2;
				sp1 += chan2;
				dp += chan2;

				p00 = p02;
				p10 = p12;
				p01 = p03;
				p11 = p13;
				p02 = p04;
				p12 = p14;
				p03 = p05;
				p13 = p15;
			}

			for (; i < wid - dx_r; i++) {
				p04 = sp0[0];
				p14 = sp1[0];

				dp[0] +=
				    (p00 * k0 + p01 * k1 + p02 * k2 + p03 * k3 +
				    p04 * k4 + p10 * k5 + p11 * k6 + p12 * k7 +
				    p13 * k8 + p14 * k9);

				sp0 += chan1;
				sp1 += chan1;
				dp += chan1;

				p00 = p01;
				p10 = p11;
				p01 = p02;
				p11 = p12;
				p02 = p03;
				p12 = p13;
				p03 = p04;
				p13 = p14;
			}

			sp0 -= chan1;
			sp1 -= chan1;

			for (; i < wid; i++) {
				p04 = sp0[0];
				p14 = sp1[0];

				dp[0] +=
				    (p00 * k0 + p01 * k1 + p02 * k2 + p03 * k3 +
				    p04 * k4 + p10 * k5 + p11 * k6 + p12 * k7 +
				    p13 * k8 + p14 * k9);

				dp += chan1;

				p00 = p01;
				p10 = p11;
				p01 = p02;
				p11 = p12;
				p02 = p03;
				p12 = p13;
				p03 = p04;
				p13 = p14;
			}

/*
 *  3 loop
 */
			dp = dl;
			sp0 = sp_4;

			k0 = (DTYPE) kern[20];
			k1 = (DTYPE) kern[21];
			k2 = (DTYPE) kern[22];
			k3 = (DTYPE) kern[23];
			k4 = (DTYPE) kern[24];

			p02 = sp0[0];
			p03 = sp0[delta_chan1];
			p04 = sp0[delta_chan2];
			p05 = sp0[delta_chan3];

			sp0 += chan4;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (wid - dx_r - 2); i += 2) {
				p00 = p02;
				p01 = p03;
				p02 = p04;
				p03 = p05;

				p04 = sp0[0];
				p05 = sp0[chan1];

				dp[0] +=
				    p00 * k0 + p01 * k1 + p02 * k2 + p03 * k3 +
				    p04 * k4;
				dp[chan1] +=
				    p01 * k0 + p02 * k1 + p03 * k2 + p04 * k3 +
				    p05 * k4;

				dp += chan2;
				sp0 += chan2;
			}

			p01 = p02;
			p02 = p03;
			p03 = p04;
			p04 = p05;

			for (; i < wid - dx_r; i++) {
				p00 = p01;
				p01 = p02;
				p02 = p03;
				p03 = p04;

				p04 = sp0[0];

				dp[0] +=
				    p00 * k0 + p01 * k1 + p02 * k2 + p03 * k3 +
				    p04 * k4;

				dp += chan1;
				sp0 += chan1;
			}

			sp0 -= chan1;

			for (; i < wid; i++) {
				p00 = p01;
				p01 = p02;
				p02 = p03;
				p03 = p04;

				p04 = sp0[0];

				dp[0] +=
				    p00 * k0 + p01 * k1 + p02 * k2 + p03 * k3 +
				    p04 * k4;

				dp += chan1;
			}

/* next line */

			if (j < hgt - dy_b - 1)
				sl += sll;
			dl += dll;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

static void
FUNC_NAME(MxN_muladd)(
    DTYPE *dst,
    const DTYPE *src,
    const mlib_d64 *kernel,
    mlib_s32 n,
    mlib_s32 m,
    mlib_s32 nch,
    mlib_s32 dnch)
{
	DTYPE *hdst1 = dst + dnch;
	mlib_s32 i, j;

	for (j = 0; j < m - 2; j += 3, src += 3 * nch, kernel += 3) {
		const DTYPE *src2 = src + 2 * nch;
		DTYPE hval0 = (DTYPE)kernel[0];
		DTYPE hval1 = (DTYPE)kernel[1];
		DTYPE hval2 = (DTYPE)kernel[2];
		DTYPE val1 = src[0];
		DTYPE val2 = src[nch];
		DTYPE val0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++) {
			val0 = val1;
			val1 = val2;
			val2 = src2[i * nch];

			dst[i * dnch] += val0 * hval0 + val1 * hval1 +
					val2 * hval2;
		}
	}

	if (j < m - 1) {
		DTYPE hval0 = (DTYPE)kernel[0];
		DTYPE hval1 = (DTYPE)kernel[1];
		DTYPE val1 = src[0];
		DTYPE val0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++) {
			val0 = val1;
			val1 = src[nch + i * nch];

			dst[i * dnch] += val0 * hval0 + val1 * hval1;
		}
	} else if (j < m) {
		DTYPE hval0 = (DTYPE)kernel[0];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++) {
			dst[i * dnch] += src[i * nch] * hval0;
		}
	}
}

/* *********************************************************** */

static void
FUNC_NAME(MxN_copy)(
    DTYPE *dst,
    const DTYPE *src,
    mlib_s32 n,
    mlib_s32 nch,
    mlib_s32 dx_l,
    mlib_s32 dx_r)
{
	mlib_s32 i;
	DTYPE val = src[0];

	for (i = 0; i < dx_l; i++)
		dst[i] = val;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i < n - dx_r; i++)
		dst[i] = src[nch * (i - dx_l)];
	val = dst[n - dx_r - 1];
	for (; i < n; i++)
		dst[i] = val;
}

/* *********************************************************** */

mlib_status
FUNC_NAME(MxN)(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *kernel,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    mlib_s32 cmask)
{
	mlib_d64 dspace[1024], *dsa = dspace;
	mlib_s32 wid_e = mlib_ImageGetWidth(src);
	DTYPE *fsa;
	DTYPE *da = mlib_ImageGetData(dst);
	DTYPE *sa = mlib_ImageGetData(src);
	mlib_s32 dlb = mlib_ImageGetStride(dst) / sizeof (DTYPE);
	mlib_s32 slb = mlib_ImageGetStride(src) / sizeof (DTYPE);
	mlib_s32 dw = mlib_ImageGetWidth(dst);
	mlib_s32 dh = mlib_ImageGetHeight(dst);
	mlib_s32 nch = mlib_ImageGetChannels(dst);
	mlib_s32 i, j, j1, k;

	if (3 * wid_e + m > 1024) {
		dsa = __mlib_malloc((3 * wid_e + m) * sizeof (mlib_d64));

		if (dsa == NULL)
			return (MLIB_FAILURE);
	}

	fsa = (DTYPE *)dsa;

	for (j = 0; j < dh; j++, da += dlb) {
		for (k = 0; k < nch; k++)
			if (cmask & (1 << (nch - 1 - k))) {
				const DTYPE *sa1 = sa + k;
				DTYPE *da1 = da + k;
				const mlib_d64 *kernel1 = kernel;

				for (i = 0; i < dw; i++)
					da1[i * nch] = 0;
				for (j1 = 0; j1 < n; j1++, kernel1 += m) {
					FUNC_NAME(MxN_copy)(fsa, sa1,
					    dw + m - 1, nch, dx_l, dx_r);
					FUNC_NAME(MxN_muladd)(da1, fsa,
					    kernel1, dw, m, 1, nch);

					if ((j + j1 >= dy_t) &&
					    (j + j1 < dh + n - dy_b - 2))
						sa1 += slb;
				}
			}

		if ((j >= dy_t) && (j < dh + n - dy_b - 2))
			sa += slb;
	}

	if (dsa != dspace)
		__mlib_free(dsa);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#if 0

void
FUNC_NAME(MxN_muladd2)(
    DTYPE *hdst,
    DTYPE *vdst,
    const DTYPE *src,
    const mlib_d64 *hfilter,
    const mlib_d64 *vfilter,
    mlib_s32 n,
    mlib_s32 m,
    mlib_s32 nch,
    mlib_s32 dnch)
{
	DTYPE *hdst1 = hdst + dnch, *vdst1 = vdst + dnch;
	mlib_s32 i, j;

	for (j = 0; j < m - 2;
	    j += 3, src += 3 * nch, hfilter += 3, vfilter += 3) {
		DTYPE *src2 = src + 2 * nch;
		DTYPE hval0 = (DTYPE)hfilter[0];
		DTYPE vval0 = (DTYPE)vfilter[0];
		DTYPE hval1 = (DTYPE)hfilter[1];
		DTYPE vval1 = (DTYPE)vfilter[1];
		DTYPE hval2 = (DTYPE)hfilter[2];
		DTYPE vval2 = (DTYPE)vfilter[2];
		DTYPE val0 = src[0];
		DTYPE val1 = src[nch];
		DTYPE hdvl = hdst[0];
		DTYPE vdvl = vdst[0];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++) {
			DTYPE hdvl0 = val0 * hval0 + hdvl;
			DTYPE vdvl0 = val0 * vval0 + vdvl;
			DTYPE val2 = src2[i * nch];

			hdvl = hdst1[i * dnch];
			vdvl = vdst1[i * dnch];
			hdvl0 += val1 * hval1;
			vdvl0 += val1 * vval1;
			hdvl0 += val2 * hval2;
			vdvl0 += val2 * vval2;
			val0 = val1;
			val1 = val2;

			hdst[i * dnch] = hdvl0;
			vdst[i * dnch] = vdvl0;
		}
	}

	if (j < m - 1) {
		DTYPE *src2 = src + 2 * nch;
		DTYPE hval0 = (DTYPE)hfilter[0];
		DTYPE vval0 = (DTYPE)vfilter[0];
		DTYPE hval1 = (DTYPE)hfilter[1];
		DTYPE vval1 = (DTYPE)vfilter[1];
		DTYPE val0 = src[0];
		DTYPE val1 = src[nch];
		DTYPE hdvl = hdst[0];
		DTYPE vdvl = vdst[0];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++) {
			DTYPE hdvl0 = val0 * hval0 + hdvl;
			DTYPE vdvl0 = val0 * vval0 + vdvl;
			DTYPE val2 = src2[i * nch];

			hdvl = hdst1[i * dnch];
			vdvl = vdst1[i * dnch];
			hdvl0 += val1 * hval1;
			vdvl0 += val1 * vval1;
			val0 = val1;
			val1 = val2;

			hdst[i * dnch] = hdvl0;
			vdst[i * dnch] = vdvl0;
		}
	} else if (j < m) {
		DTYPE *src2 = src + 2 * nch;
		DTYPE hval0 = (DTYPE)hfilter[0];
		DTYPE vval0 = (DTYPE)vfilter[0];
		DTYPE val0 = src[0];
		DTYPE val1 = src[nch];
		DTYPE hdvl = hdst[0];
		DTYPE vdvl = vdst[0];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++) {
			DTYPE hdvl0 = val0 * hval0 + hdvl;
			DTYPE vdvl0 = val0 * vval0 + vdvl;
			DTYPE val2 = src2[i * nch];

			hdvl = hdst1[i * dnch];
			vdvl = vdst1[i * dnch];
			val0 = val1;
			val1 = val2;

			hdst[i * dnch] = hdvl0;
			vdst[i * dnch] = vdvl0;
		}
	}
}

#endif

/* *********************************************************** */
