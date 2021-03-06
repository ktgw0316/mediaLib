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

#pragma ident   "@(#)mlib_ImageConv_32ext.c	9.2    07/11/05 SMI"

/*
 * FUNCTION
 *   Internal functions for mlib_ImageConv* on S32 type and
 *   MLIB_EDGE_SRC_EXTEND mask
 *
 */

#include <mlib_image.h>
#include <mlib_ImageConv.h>

/* *********************************************************** */

#define	KSIZE1	(KSIZE - 1)

/* *********************************************************** */

#define	PARAM                                                         \
	mlib_image *dst, const mlib_image *src, mlib_s32 dx_l,        \
	mlib_s32 dx_r, mlib_s32 dy_t, mlib_s32 dy_b,                  \
	const mlib_s32 *kern, mlib_s32 scalef_expon, mlib_s32 cmask

/* *********************************************************** */

#define	BUFF_LINE	256

/* *********************************************************** */

#define	CONV_FUNC(KERN)	mlib_conv##KERN##ext_s32(PARAM)

/* *********************************************************** */
#ifndef MLIB_USE_FTOI_CLAMPING

#define	CLAMP_S32(dst, src)                                 \
	dst = (src > MLIB_S32_MAX) ?                            \
		MLIB_S32_MAX :                                      \
		((src < MLIB_S32_MIN) ?                             \
		MLIB_S32_MIN : ((mlib_s32)(src)))

#else /* MLIB_USE_FTOI_CLAMPING */

#define	CLAMP_S32(dst, src)	dst = (mlib_s32)(src)

#endif /* MLIB_USE_FTOI_CLAMPING */
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

#define	DEF_VARS(type)                                            \
	GET_SRC_DST_PARAMETERS(type);                             \
	type *sl, *sp, *sl1, *dl, *dp;                            \
	ITYPE *pbuff = buff, *buff0, *buff1, *buff2, *buffT;   \
	mlib_s32 i, j, c, swid;                                   \
	mlib_d64 scalef, d0, d1

/* *********************************************************** */

#define	DEF_VARS_MxN(type)                                      \
	GET_SRC_DST_PARAMETERS(type);                           \
	type *sl, *sp, *sl1, *dl, *dp;                          \
	ITYPE *pbuff = buff;                                 \
	mlib_s32 i, j, c, swid

/* *********************************************************** */

#define	CALC_SCALE()                                            \
	scalef = 1.0;                                           \
	while (scalef_expon > 30) {                             \
	    scalef /= (1 << 30);                                \
	    scalef_expon -= 30;                                 \
	}                                                       \
	                                                        \
	scalef /= (1 << scalef_expon)

/* *********************************************************** */
#if defined(_NO_LONGLONG)
#define	ITYPE mlib_d64
#else /* else if _NO_LONGLONG */
#define	ITYPE mlib_s64
#endif /* end _NO_LONGLONG */
/* *********************************************************** */

#undef  KSIZE
#define	KSIZE	2
mlib_status
CONV_FUNC(2x2)
{
	ITYPE buff[(KSIZE + 1) * BUFF_LINE];
	ITYPE k0, k1, k2, k3;
	ITYPE p00, p01, p02, p03, p10, p11, p12, p13;
	mlib_d64 d2;
	DEF_VARS(mlib_s32);
	mlib_s32 chan3 = chan1 + chan2;

	swid = wid + KSIZE1;

	if (swid > BUFF_LINE) {
		pbuff = __mlib_malloc((KSIZE + 1) * sizeof (ITYPE) * swid);

		if (pbuff == NULL)
			return (MLIB_FAILURE);
	}

	buff0 = pbuff;
	buff1 = buff0 + swid;
	buff2 = buff1 + swid;

	swid -= dx_r;

/* keep kernel in regs */
	CALC_SCALE();
	k0 = kern[0];
	k1 = kern[1];
	k2 = kern[2];
	k3 = kern[3];

	for (c = 0; c < chan1; c++) {
		if (!(cmask & (1 << (chan1 - 1 - c))))
			continue;

		sl = adr_src + c;
		dl = adr_dst + c;

		if ((hgt - dy_b) > 0)
			sl1 = sl + sll;
		else
			sl1 = sl;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < swid; i++) {
			buff0[i] = (ITYPE)sl[i * chan1];
			buff1[i] = (ITYPE)sl1[i * chan1];
		}

		if (dx_r != 0) {
			buff0[swid] = buff0[swid - 1];
			buff1[swid] = buff1[swid - 1];
		}

		if ((hgt - dy_b) > 1)
			sl = sl1 + sll;
		else
			sl = sl1;

		for (j = 0; j < hgt; j++) {
			p03 = buff0[0];
			p13 = buff1[0];

			sp = sl;
			dp = dl;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (wid - 3); i += 3) {

				p00 = p03;
				p10 = p13;

				p01 = buff0[i + 1];
				p11 = buff1[i + 1];
				p02 = buff0[i + 2];
				p12 = buff1[i + 2];
				p03 = buff0[i + 3];
				p13 = buff1[i + 3];

				buff2[i] = (ITYPE)sp[0];
				buff2[i + 1] = (ITYPE)sp[chan1];
				buff2[i + 2] = (ITYPE)sp[chan2];

				d0 = (p00 * k0 + p01 * k1 +
					p10 * k2 + p11 * k3);
				d1 = (p01 * k0 + p02 * k1 +
					p11 * k2 + p12 * k3);
				d2 = (p02 * k0 + p03 * k1 +
					p12 * k2 + p13 * k3);

				CLAMP_S32(dp[0], d0 * scalef);
				CLAMP_S32(dp[chan1], d1 * scalef);
				CLAMP_S32(dp[chan2], d2 * scalef);

				sp += chan3;
				dp += chan3;
			}

			for (; i < wid; i++) {
				p00 = buff0[i];
				p10 = buff1[i];
				p01 = buff0[i + 1];
				p11 = buff1[i + 1];

				buff2[i] = (ITYPE)sp[0];

				d0 = p00 * k0 + p01 * k1 + p10 * k2 + p11 * k3;
				CLAMP_S32(dp[0], d0 * scalef);

				sp += chan1;
				dp += chan1;
			}

			for (; i < swid; i++) {
				buff2[i] = (ITYPE)sp[0];
				sp += chan1;
			}

			if (dx_r != 0)
				buff2[swid] = buff2[swid - 1];

			if (j < hgt - dy_b - 2)
				sl += sll;
			dl += dll;

			buffT = buff0;
			buff0 = buff1;
			buff1 = buff2;
			buff2 = buffT;
		}
	}

	if (pbuff != buff)
		__mlib_free(pbuff);

	return (MLIB_SUCCESS);
}
/* *********************************************************** */

#undef  KSIZE
#define	KSIZE	3

mlib_status
CONV_FUNC(3x3)
{
	ITYPE buff[(KSIZE + 1) * BUFF_LINE], *buff3;
	ITYPE k0, k1, k2, k3, k4, k5, k6, k7, k8;
	ITYPE p00, p01, p02, p03, p10, p11, p12, p13, p20, p21, p22, p23;
	mlib_s32 *sl2;
	DEF_VARS(mlib_s32);

	swid = wid + KSIZE1;

	if (swid > BUFF_LINE) {
		pbuff = __mlib_malloc((KSIZE + 1) * sizeof (ITYPE) * swid);

		if (pbuff == NULL)
			return (MLIB_FAILURE);
	}

	buff0 = pbuff;
	buff1 = buff0 + swid;
	buff2 = buff1 + swid;
	buff3 = buff2 + swid;

	swid -= (dx_l + dx_r);

	CALC_SCALE();
	k0 = kern[0];
	k1 = kern[1];
	k2 = kern[2];
	k3 = kern[3];
	k4 = kern[4];
	k5 = kern[5];
	k6 = kern[6];
	k7 = kern[7];
	k8 = kern[8];

	for (c = 0; c < chan1; c++) {
		if (!(cmask & (1 << (chan1 - 1 - c))))
			continue;

		sl = adr_src + c;
		dl = adr_dst + c;

		if ((1 > dy_t) && (1 < hgt + KSIZE1 - dy_b))
			sl1 = sl + sll;
		else
			sl1 = sl;

		if ((hgt - dy_b) > 0)
			sl2 = sl1 + sll;
		else
			sl2 = sl1;

		for (i = 0; i < dx_l; i++) {
			buff0[i] = (ITYPE)sl[0];
			buff1[i] = (ITYPE)sl1[0];
			buff2[i] = (ITYPE)sl2[0];
		}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < swid; i++) {
			buff0[i + dx_l] = (ITYPE)sl[i * chan1];
			buff1[i + dx_l] = (ITYPE)sl1[i * chan1];
			buff2[i + dx_l] = (ITYPE)sl2[i * chan1];
		}

		for (i = 0; i < dx_r; i++) {
			buff0[swid + dx_l + i] = buff0[swid + dx_l - 1];
			buff1[swid + dx_l + i] = buff1[swid + dx_l - 1];
			buff2[swid + dx_l + i] = buff2[swid + dx_l - 1];
		}

		if ((hgt - dy_b) > 1)
			sl = sl2 + sll;
		else
			sl = sl2;

		for (j = 0; j < hgt; j++) {
			mlib_d64 s0, s1;

			p02 = buff0[0];
			p12 = buff1[0];
			p22 = buff2[0];

			p03 = buff0[1];
			p13 = buff1[1];
			p23 = buff2[1];

			sp = sl;
			dp = dl;

			s0 = (p02 * k0 + p03 * k1 + p12 * k3 + p13 * k4);
			s0 += (p22 * k6 + p23 * k7);
			s1 = (p03 * k0 + p13 * k3 + p23 * k6);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (wid - 2); i += 2) {
				p02 = buff0[i + 2];
				p12 = buff1[i + 2];
				p22 = buff2[i + 2];
				p03 = buff0[i + 3];
				p13 = buff1[i + 3];
				p23 = buff2[i + 3];

				buff3[i + dx_l] = (ITYPE)sp[0];
				buff3[i + dx_l + 1] = (ITYPE)sp[chan1];

				d0 = s0;
				d0 += (p02 * k2 + p12 * k5 + p22 * k8);
				d1 = s1;
				d1 += (p02 * k1 + p03 * k2 + p12 * k4 +
				    p13 * k5);
				d1 += (p22 * k7 + p23 * k8);

				CLAMP_S32(dp[0], d0 * scalef);
				CLAMP_S32(dp[chan1], d1 * scalef);

				s0 = (p02 * k0 + p03 * k1 +
					p12 * k3 + p13 * k4);
				s0 += (p22 * k6 + p23 * k7);
				s1 = (p03 * k0 + p13 * k3 + p23 * k6);

				sp += chan2;
				dp += chan2;
			}

			for (; i < wid; i++) {
				p00 = buff0[i];
				p10 = buff1[i];
				p20 = buff2[i];
				p01 = buff0[i + 1];
				p11 = buff1[i + 1];
				p21 = buff2[i + 1];
				p02 = buff0[i + 2];
				p12 = buff1[i + 2];
				p22 = buff2[i + 2];

				buff3[i + dx_l] = (ITYPE)sp[0];

				d0 = (p00 * k0 + p01 * k1 + p02 * k2 +
				    p10 * k3);
				d0 += (p11 * k4 + p12 * k5 + p20 * k6 +
				    p21 * k7);
				d0 += (p22 * k8);

				CLAMP_S32(dp[0], d0 * scalef);

				sp += chan1;
				dp += chan1;
			}

			for (; i < swid; i++) {
				buff3[i + dx_l] = (ITYPE)sp[0];
				sp += chan1;
			}

			for (i = 0; i < dx_l; i++)
				buff3[i] = buff3[dx_l];
			for (i = 0; i < dx_r; i++)
				buff3[swid + dx_l + i] = buff3[swid + dx_l - 1];

			if (j < hgt - dy_b - 2)
				sl += sll;
			dl += dll;

			buffT = buff0;
			buff0 = buff1;
			buff1 = buff2;
			buff2 = buff3;
			buff3 = buffT;
		}
	}

	if (pbuff != buff)
		__mlib_free(pbuff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef  KSIZE
#define	KSIZE	4

mlib_status
CONV_FUNC(4x4)
{
	ITYPE buff[(KSIZE + 2) * BUFF_LINE], *buff3, *buff4;
	mlib_d64 *buff5;
	ITYPE k[KSIZE * KSIZE];
	ITYPE k0, k1, k2, k3, k4, k5, k6, k7;
	ITYPE p00, p01, p02, p03, p04,
	    p10, p11, p12, p13, p14, p20, p21, p22, p23, p30, p31, p32, p33;
	mlib_s32 *sl2, *sl3;
	DEF_VARS(mlib_s32);

	swid = wid + KSIZE1;

	if (swid > BUFF_LINE) {
		pbuff = __mlib_malloc((KSIZE + 2) * sizeof (ITYPE) * swid);

		if (pbuff == NULL)
			return (MLIB_FAILURE);
	}

	buff0 = pbuff;
	buff1 = buff0 + swid;
	buff2 = buff1 + swid;
	buff3 = buff2 + swid;
	buff4 = buff3 + swid;
	buff5 = (mlib_d64 *)(buff4 + swid);

	swid -= (dx_l + dx_r);

	CALC_SCALE();
	for (j = 0; j < 16; j++)
		k[j] = kern[j];

	for (c = 0; c < chan1; c++) {
		if (!(cmask & (1 << (chan1 - 1 - c))))
			continue;

		sl = adr_src + c;
		dl = adr_dst + c;

		if ((1 > dy_t) && (1 < hgt + KSIZE1 - dy_b))
			sl1 = sl + sll;
		else
			sl1 = sl;

		if ((2 > dy_t) && (2 < hgt + KSIZE1 - dy_b))
			sl2 = sl1 + sll;
		else
			sl2 = sl1;

		if ((hgt - dy_b) > 0)
			sl3 = sl2 + sll;
		else
			sl3 = sl2;

		for (i = 0; i < dx_l; i++) {
			buff0[i] = (ITYPE)sl[0];
			buff1[i] = (ITYPE)sl1[0];
			buff2[i] = (ITYPE)sl2[0];
			buff3[i] = (ITYPE)sl3[0];
		}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < swid; i++) {
			buff0[i + dx_l] = (ITYPE)sl[i * chan1];
			buff1[i + dx_l] = (ITYPE)sl1[i * chan1];
			buff2[i + dx_l] = (ITYPE)sl2[i * chan1];
			buff3[i + dx_l] = (ITYPE)sl3[i * chan1];
		}

		for (i = 0; i < dx_r; i++) {
			buff0[swid + dx_l + i] = buff0[swid + dx_l - 1];
			buff1[swid + dx_l + i] = buff1[swid + dx_l - 1];
			buff2[swid + dx_l + i] = buff2[swid + dx_l - 1];
			buff3[swid + dx_l + i] = buff3[swid + dx_l - 1];
		}

		if ((hgt - dy_b) > 1)
			sl = sl3 + sll;
		else
			sl = sl3;

		for (j = 0; j < hgt; j++) {
/*
 *  First loop on two first lines of kernel
 */
			k0 = k[0];
			k1 = k[1];
			k2 = k[2];
			k3 = k[3];
			k4 = k[4];
			k5 = k[5];
			k6 = k[6];
			k7 = k[7];

			sp = sl;
			dp = dl;

			p02 = buff0[0];
			p12 = buff1[0];
			p03 = buff0[1];
			p13 = buff1[1];
			p04 = buff0[2];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (wid - 2); i += 2) {
				p00 = p02;
				p10 = p12;
				p01 = p03;
				p11 = p13;
				p02 = p04;
				p12 = buff1[i + 2];
				p03 = buff0[i + 3];
				p13 = buff1[i + 3];
				p04 = buff0[i + 4];
				p14 = buff1[i + 4];

				buff4[i + dx_l] = (ITYPE)sp[0];
				buff4[i + dx_l + 1] = (ITYPE)sp[chan1];

				buff5[i] =
				    (p00 * k0 + p01 * k1 + p02 * k2 + p03 * k3);
				buff5[i] +=
				    (p10 * k4 + p11 * k5 + p12 * k6 + p13 * k7);
				buff5[i + 1] =
				    (p01 * k0 + p02 * k1 + p03 * k2 + p04 * k3);
				buff5[i + 1] +=
				    (p11 * k4 + p12 * k5 + p13 * k6 + p14 * k7);

				sp += chan2;
			}

/*
 *  Second loop on two last lines of kernel
 */
			k0 = k[8];
			k1 = k[9];
			k2 = k[10];
			k3 = k[11];
			k4 = k[12];
			k5 = k[13];
			k6 = k[14];
			k7 = k[15];

			p02 = buff2[0];
			p12 = buff3[0];
			p03 = buff2[1];
			p13 = buff3[1];
			p04 = buff2[2];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (wid - 2); i += 2) {
				p00 = p02;
				p10 = p12;
				p01 = p03;
				p11 = p13;
				p02 = p04;
				p12 = buff3[i + 2];
				p03 = buff2[i + 3];
				p13 = buff3[i + 3];
				p04 = buff2[i + 4];
				p14 = buff3[i + 4];

				d0 = (p00 * k0 + p01 * k1 + p02 * k2 +
				    p03 * k3);
				d0 += (p10 * k4 + p11 * k5 + p12 * k6 +
				    p13 * k7);
				d0 += buff5[i];
				d1 = (p01 * k0 + p02 * k1 + p03 * k2 +
				    p04 * k3);
				d1 += (p11 * k4 + p12 * k5 + p13 * k6 +
				    p14 * k7);
				d1 += buff5[i + 1];

				CLAMP_S32(dp[0], d0 * scalef);
				CLAMP_S32(dp[chan1], d1 * scalef);

				dp += chan2;
			}

/* last pixels */
			for (; i < wid; i++) {
				p00 = buff0[i];
				p10 = buff1[i];
				p20 = buff2[i];
				p30 = buff3[i];
				p01 = buff0[i + 1];
				p11 = buff1[i + 1];
				p21 = buff2[i + 1];
				p31 = buff3[i + 1];
				p02 = buff0[i + 2];
				p12 = buff1[i + 2];
				p22 = buff2[i + 2];
				p32 = buff3[i + 2];
				p03 = buff0[i + 3];
				p13 = buff1[i + 3];
				p23 = buff2[i + 3];
				p33 = buff3[i + 3];

				buff4[i + dx_l] = (ITYPE)sp[0];

				d0 = (p00 * k[0] + p01 * k[1] + p02 * k[2] +
				    p03 * k[3]);
				d0 += (p10 * k[4] + p11 * k[5] + p12 * k[6] +
					p13 * k[7]);
				d0 += (p20 * k[8] + p21 * k[9] + p22 * k[10] +
					p23 * k[11]);
				d0 += (p30 * k[12] + p31 * k[13] + p32 * k[14] +
				    p33 * k[15]);

				CLAMP_S32(dp[0], d0 * scalef);

				sp += chan1;
				dp += chan1;
			}

			for (; i < swid; i++) {
				buff4[i + dx_l] = (ITYPE)sp[0];
				sp += chan1;
			}

			for (i = 0; i < dx_l; i++)
				buff4[i] = buff4[dx_l];
			for (i = 0; i < dx_r; i++)
				buff4[swid + dx_l + i] = buff4[swid + dx_l - 1];

/* next line */

			if (j < hgt - dy_b - 2)
				sl += sll;
			dl += dll;

			buffT = buff0;
			buff0 = buff1;
			buff1 = buff2;
			buff2 = buff3;
			buff3 = buff4;
			buff4 = buffT;
		}
	}

	if (pbuff != buff)
		__mlib_free(pbuff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef  KSIZE
#define	KSIZE	5

mlib_status
CONV_FUNC(5x5)
{
	ITYPE buff[(KSIZE + 2) * BUFF_LINE], *buff3, *buff4, *buff5;
	mlib_d64 *buff6;
	ITYPE k[KSIZE * KSIZE];
	ITYPE k0, k1, k2, k3, k4, k5, k6, k7, k8, k9;
	ITYPE p00, p01, p02, p03, p04, p05,
	    p10, p11, p12, p13, p14, p15,
	    p20, p21, p22, p23, p24,
	    p30, p31, p32, p33, p34, p40, p41, p42, p43, p44;
	mlib_s32 *sl2, *sl3, *sl4;
	DEF_VARS(mlib_s32);

	swid = wid + KSIZE1;

	if (swid > BUFF_LINE) {
		pbuff = __mlib_malloc((KSIZE + 2) * sizeof (ITYPE) * swid);

		if (pbuff == NULL)
			return (MLIB_FAILURE);
	}

	buff0 = pbuff;
	buff1 = buff0 + swid;
	buff2 = buff1 + swid;
	buff3 = buff2 + swid;
	buff4 = buff3 + swid;
	buff5 = buff4 + swid;
	buff6 = (mlib_d64 *)(buff5 + swid);

	swid -= (dx_l + dx_r);

	CALC_SCALE();
	for (j = 0; j < 25; j++)
		k[j] = kern[j];

	for (c = 0; c < chan1; c++) {
		if (!(cmask & (1 << (chan1 - 1 - c))))
			continue;

		sl = adr_src + c;
		dl = adr_dst + c;

		if ((1 > dy_t) && (1 < hgt + KSIZE1 - dy_b))
			sl1 = sl + sll;
		else
			sl1 = sl;

		if ((2 > dy_t) && (2 < hgt + KSIZE1 - dy_b))
			sl2 = sl1 + sll;
		else
			sl2 = sl1;

		if ((3 > dy_t) && (3 < hgt + KSIZE1 - dy_b))
			sl3 = sl2 + sll;
		else
			sl3 = sl2;

		if ((hgt - dy_b) > 0)
			sl4 = sl3 + sll;
		else
			sl4 = sl3;

		for (i = 0; i < dx_l; i++) {
			buff0[i] = (ITYPE)sl[0];
			buff1[i] = (ITYPE)sl1[0];
			buff2[i] = (ITYPE)sl2[0];
			buff3[i] = (ITYPE)sl3[0];
			buff4[i] = (ITYPE)sl4[0];
		}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < swid; i++) {
			buff0[i + dx_l] = (ITYPE)sl[i * chan1];
			buff1[i + dx_l] = (ITYPE)sl1[i * chan1];
			buff2[i + dx_l] = (ITYPE)sl2[i * chan1];
			buff3[i + dx_l] = (ITYPE)sl3[i * chan1];
			buff4[i + dx_l] = (ITYPE)sl4[i * chan1];
		}

		for (i = 0; i < dx_r; i++) {
			buff0[swid + dx_l + i] = buff0[swid + dx_l - 1];
			buff1[swid + dx_l + i] = buff1[swid + dx_l - 1];
			buff2[swid + dx_l + i] = buff2[swid + dx_l - 1];
			buff3[swid + dx_l + i] = buff3[swid + dx_l - 1];
			buff4[swid + dx_l + i] = buff4[swid + dx_l - 1];
		}

		if ((hgt - dy_b) > 1)
			sl = sl4 + sll;
		else
			sl = sl4;

		for (j = 0; j < hgt; j++) {
/*
 *  First loop
 */
			k0 = k[0];
			k1 = k[1];
			k2 = k[2];
			k3 = k[3];
			k4 = k[4];
			k5 = k[5];
			k6 = k[6];
			k7 = k[7];
			k8 = k[8];
			k9 = k[9];

			sp = sl;
			dp = dl;

			p02 = buff0[0];
			p12 = buff1[0];
			p03 = buff0[1];
			p13 = buff1[1];
			p04 = buff0[2];
			p14 = buff1[2];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (wid - 2); i += 2) {
				p00 = p02;
				p10 = p12;
				p01 = p03;
				p11 = p13;
				p02 = p04;
				p12 = p14;

				p03 = buff0[i + 3];
				p13 = buff1[i + 3];
				p04 = buff0[i + 4];
				p14 = buff1[i + 4];
				p05 = buff0[i + 5];
				p15 = buff1[i + 5];

				buff6[i] =
				    (p00 * k0 + p01 * k1 + p02 * k2 + p03 * k3);
				buff6[i] +=
				    (p04 * k4 + p10 * k5 + p11 * k6 + p12 * k7);
				buff6[i] +=
				    (p13 * k8 + p14 * k9);
				buff6[i + 1] =
				    (p01 * k0 + p02 * k1 + p03 * k2 + p04 * k3);
				buff6[i + 1] +=
				    (p05 * k4 + p11 * k5 + p12 * k6 + p13 * k7);
				buff6[i + 1] +=
				    (p14 * k8 + p15 * k9);
			}

/*
 *  Second loop
 */
			k0 = k[10];
			k1 = k[11];
			k2 = k[12];
			k3 = k[13];
			k4 = k[14];
			k5 = k[15];
			k6 = k[16];
			k7 = k[17];
			k8 = k[18];
			k9 = k[19];

			p02 = buff2[0];
			p12 = buff3[0];
			p03 = buff2[1];
			p13 = buff3[1];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (wid - 2); i += 2) {
				p00 = p02;
				p10 = p12;
				p01 = p03;
				p11 = p13;

				p02 = buff2[i + 2];
				p12 = buff3[i + 2];
				p03 = buff2[i + 3];
				p13 = buff3[i + 3];
				p04 = buff2[i + 4];
				p14 = buff3[i + 4];
				p05 = buff2[i + 5];
				p15 = buff3[i + 5];

				buff6[i] +=
				    (p00 * k0 + p01 * k1 + p02 * k2 + p03 * k3);
				buff6[i] +=
				    (p04 * k4 + p10 * k5 + p11 * k6 + p12 * k7);
				buff6[i] +=
				    (p13 * k8 + p14 * k9);
				buff6[i + 1] +=
				    (p01 * k0 + p02 * k1 + p03 * k2 + p04 * k3);
				buff6[i + 1] +=
				    (p05 * k4 + p11 * k5 + p12 * k6 + p13 * k7);
				buff6[i + 1] +=
				    (p14 * k8 + p15 * k9);
			}

/*
 *  3 loop
 */
			k0 = k[20];
			k1 = k[21];
			k2 = k[22];
			k3 = k[23];
			k4 = k[24];

			p02 = buff4[0];
			p03 = buff4[1];
			p04 = buff4[2];
			p05 = buff4[3];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (wid - 2); i += 2) {
				p00 = p02;
				p01 = p03;
				p02 = p04;
				p03 = p05;

				p04 = buff4[i + 4];
				p05 = buff4[i + 5];

				buff5[i + dx_l] = (ITYPE)sp[0];
				buff5[i + dx_l + 1] = (ITYPE)sp[chan1];

				d0 = (p00 * k0 + p01 * k1 +
					p02 * k2 + p03 * k3);
				d0 += p04 * k4 + buff6[i];
				d1 = (p01 * k0 + p02 * k1 +
					p03 * k2 + p04 * k3);
				d1 += p05 * k4 + buff6[i + 1];

				CLAMP_S32(dp[0], d0 * scalef);
				CLAMP_S32(dp[chan1], d1 * scalef);

				sp += chan2;
				dp += chan2;
			}

/* last pixels */
			for (; i < wid; i++) {
				p00 = buff0[i];
				p10 = buff1[i];
				p20 = buff2[i];
				p30 = buff3[i];
				p01 = buff0[i + 1];
				p11 = buff1[i + 1];
				p21 = buff2[i + 1];
				p31 = buff3[i + 1];
				p02 = buff0[i + 2];
				p12 = buff1[i + 2];
				p22 = buff2[i + 2];
				p32 = buff3[i + 2];
				p03 = buff0[i + 3];
				p13 = buff1[i + 3];
				p23 = buff2[i + 3];
				p33 = buff3[i + 3];
				p04 = buff0[i + 4];
				p14 = buff1[i + 4];
				p24 = buff2[i + 4];
				p34 = buff3[i + 4];

				p40 = buff4[i];
				p41 = buff4[i + 1];
				p42 = buff4[i + 2];
				p43 = buff4[i + 3];
				p44 = buff4[i + 4];

				buff5[i + dx_l] = (ITYPE)sp[0];

				d0 = (p00 * k[0] + p01 * k[1] + p02 * k[2] +
				    p03 * k[3]);
				d0 += (p04 * k[4] + p10 * k[5] + p11 * k[6] +
					p12 * k[7]);
				d0 += (p13 * k[8] + p14 * k[9] + p20 * k[10] +
					p21 * k[11]);
				d0 += (p22 * k[12] + p23 * k[13] + p24 * k[14] +
				    p30 * k[15]);
				d0 += (p31 * k[16] + p32 * k[17] + p33 * k[18] +
					p34 * k[19]);
				d0 += (p40 * k[20] + p41 * k[21] + p42 * k[22] +
					p43 * k[23]);
				d0 += p44 * k[24];

				CLAMP_S32(dp[0], d0 * scalef);

				sp += chan1;
				dp += chan1;
			}

			for (; i < swid; i++) {
				buff5[i + dx_l] = (ITYPE)sp[0];
				sp += chan1;
			}

			for (i = 0; i < dx_l; i++)
				buff5[i] = buff5[dx_l];
			for (i = 0; i < dx_r; i++)
				buff5[swid + dx_l + i] = buff5[swid + dx_l - 1];

/* next line */

			if (j < hgt - dy_b - 2)
				sl += sll;
			dl += dll;

			buffT = buff0;
			buff0 = buff1;
			buff1 = buff2;
			buff2 = buff3;
			buff3 = buff4;
			buff4 = buff5;
			buff5 = buffT;
		}
	}

	if (pbuff != buff)
		__mlib_free(pbuff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef  KSIZE
#define	KSIZE	7

mlib_status
CONV_FUNC(7x7)
{
	ITYPE buff[(KSIZE + 2) * BUFF_LINE], *buffs[2 * (KSIZE + 1)];
	mlib_d64 *buffd;
	ITYPE k[KSIZE * KSIZE];
	ITYPE k0, k1, k2, k3, k4, k5, k6;
	ITYPE p0, p1, p2, p3, p4, p5, p6, p7;
	mlib_d64 d0, d1;
	mlib_s32 l, m, buff_ind, *sl2, *sl3, *sl4, *sl5, *sl6;
	mlib_d64 scalef;
	DEF_VARS_MxN(mlib_s32);

	swid = wid + KSIZE1;

	if (swid > BUFF_LINE) {
		pbuff = __mlib_malloc((KSIZE + 2) * sizeof (ITYPE) * swid);

		if (pbuff == NULL)
			return (MLIB_FAILURE);
	}

	for (l = 0; l < KSIZE + 1; l++)
		buffs[l] = pbuff + l * swid;
	for (l = 0; l < KSIZE + 1; l++)
		buffs[l + (KSIZE + 1)] = buffs[l];
	buffd = (mlib_d64 *)(buffs[KSIZE] + swid);

	swid -= (dx_l + dx_r);

	CALC_SCALE();
	for (j = 0; j < 49; j++)
		k[j] = kern[j];

	for (c = 0; c < chan1; c++) {
		if (!(cmask & (1 << (chan1 - 1 - c))))
			continue;

		sl = adr_src + c;
		dl = adr_dst + c;

		if ((1 > dy_t) && (1 < hgt + KSIZE1 - dy_b))
			sl1 = sl + sll;
		else
			sl1 = sl;

		if ((2 > dy_t) && (2 < hgt + KSIZE1 - dy_b))
			sl2 = sl1 + sll;
		else
			sl2 = sl1;

		if ((3 > dy_t) && (3 < hgt + KSIZE1 - dy_b))
			sl3 = sl2 + sll;
		else
			sl3 = sl2;

		if ((4 > dy_t) && (4 < hgt + KSIZE1 - dy_b))
			sl4 = sl3 + sll;
		else
			sl4 = sl3;

		if ((5 > dy_t) && (5 < hgt + KSIZE1 - dy_b))
			sl5 = sl4 + sll;
		else
			sl5 = sl4;

		if ((hgt - dy_b) > 0)
			sl6 = sl5 + sll;
		else
			sl6 = sl5;

		for (i = 0; i < dx_l; i++) {
			buffs[0][i] = (ITYPE)sl[0];
			buffs[1][i] = (ITYPE)sl1[0];
			buffs[2][i] = (ITYPE)sl2[0];
			buffs[3][i] = (ITYPE)sl3[0];
			buffs[4][i] = (ITYPE)sl4[0];
			buffs[5][i] = (ITYPE)sl5[0];
			buffs[6][i] = (ITYPE)sl6[0];
		}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < swid; i++) {
			buffs[0][i + dx_l] = (ITYPE)sl[i * chan1];
			buffs[1][i + dx_l] = (ITYPE)sl1[i * chan1];
			buffs[2][i + dx_l] = (ITYPE)sl2[i * chan1];
			buffs[3][i + dx_l] = (ITYPE)sl3[i * chan1];
			buffs[4][i + dx_l] = (ITYPE)sl4[i * chan1];
			buffs[5][i + dx_l] = (ITYPE)sl5[i * chan1];
			buffs[6][i + dx_l] = (ITYPE)sl6[i * chan1];
		}

		for (i = 0; i < dx_r; i++) {
			buffs[0][swid + dx_l + i] = buffs[0][swid + dx_l - 1];
			buffs[1][swid + dx_l + i] = buffs[1][swid + dx_l - 1];
			buffs[2][swid + dx_l + i] = buffs[2][swid + dx_l - 1];
			buffs[3][swid + dx_l + i] = buffs[3][swid + dx_l - 1];
			buffs[4][swid + dx_l + i] = buffs[4][swid + dx_l - 1];
			buffs[5][swid + dx_l + i] = buffs[5][swid + dx_l - 1];
			buffs[6][swid + dx_l + i] = buffs[6][swid + dx_l - 1];
		}

		buff_ind = 0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < wid; i++)
			buffd[i] = 0.0;

		if ((hgt - dy_b) > 1)
			sl = sl6 + sll;
		else
			sl = sl6;

		for (j = 0; j < hgt; j++) {
			ITYPE **buffc = buffs + buff_ind;
			ITYPE *buffn = buffc[KSIZE];
			ITYPE *pk = k;

			for (l = 0; l < KSIZE; l++) {
				ITYPE *buff = buffc[l];

				sp = sl;
				dp = dl;

				p2 = buff[0];
				p3 = buff[1];
				p4 = buff[2];
				p5 = buff[3];
				p6 = buff[4];
				p7 = buff[5];

				k0 = (*pk++);
				k1 = (*pk++);
				k2 = (*pk++);
				k3 = (*pk++);
				k4 = (*pk++);
				k5 = (*pk++);
				k6 = (*pk++);

				if (l < (KSIZE - 1)) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (i = 0; i <= (wid - 2); i += 2) {
						p0 = p2;
						p1 = p3;
						p2 = p4;
						p3 = p5;
						p4 = p6;
						p5 = p7;

						p6 = buff[i + 6];
						p7 = buff[i + 7];

						buffd[i] +=
						    (p0 * k0 + p1 * k1 +
						    p2 * k2 + p3 * k3);
						buffd[i] +=
						    (p4 * k4 + p5 * k5 +
							p6 * k6);
						buffd[i + 1] +=
						    (p1 * k0 + p2 * k1 +
						    p3 * k2 + p4 * k3);
						buffd[i + 1] +=
						    (p5 * k4 + p6 * k5 +
							p7 * k6);
					}

				} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (i = 0; i <= (wid - 2); i += 2) {
						p0 = p2;
						p1 = p3;
						p2 = p4;
						p3 = p5;
						p4 = p6;
						p5 = p7;

						p6 = buff[i + 6];
						p7 = buff[i + 7];

						buffn[i + dx_l] =
						    (ITYPE)sp[0];
						buffn[i + dx_l + 1] =
						    (ITYPE)sp[chan1];

						d0 = (p0 * k0 + p1 * k1 +
						    p2 * k2 + p3 * k3);
						d0 += (p4 * k4 + p5 * k5 +
						    p6 * k6);
						d0 += buffd[i];
						d1 = (p1 * k0 + p2 * k1 +
						    p3 * k2 + p4 * k3);
						d1 += (p5 * k4 + p6 * k5 +
						    p7 * k6);
						d1 += buffd[i + 1];

						CLAMP_S32(dp[0],
							d0 * scalef);
						CLAMP_S32(dp[chan1],
							d1 * scalef);

						buffd[i] = 0.0;
						buffd[i + 1] = 0.0;

						sp += chan2;
						dp += chan2;
					}
				}
			}

/* last pixels */
			for (; i < wid; i++) {
				ITYPE *pk = k;
				mlib_d64 s = 0;

				for (l = 0; l < KSIZE; l++) {
					ITYPE *buff = buffc[l] + i;

					for (m = 0; m < KSIZE; m++)
						s += buff[m] * ((*pk++));
				}

				CLAMP_S32(dp[0], s * scalef);

				buffn[i + dx_l] = (ITYPE)sp[0];

				sp += chan1;
				dp += chan1;
			}

			for (; i < swid; i++) {
				buffn[i + dx_l] = (ITYPE)sp[0];
				sp += chan1;
			}

			for (i = 0; i < dx_l; i++)
				buffn[i] = buffn[dx_l];
			for (i = 0; i < dx_r; i++)
				buffn[swid + dx_l + i] = buffn[swid + dx_l - 1];

/* next line */

			if (j < hgt - dy_b - 2)
				sl += sll;
			dl += dll;

			buff_ind++;

			if (buff_ind >= KSIZE + 1)
				buff_ind = 0;
		}
	}

	if (pbuff != buff)
		__mlib_free(pbuff);

	return (MLIB_SUCCESS);
}
/* *********************************************************** */
