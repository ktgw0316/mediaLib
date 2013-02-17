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

#pragma ident   "@(#)mlib_ImageConv_8ext.c	9.2    07/11/05 SMI"

/*
 * FUNCTION
 *   Internal functions for mlib_ImageConv* on U8/S16/U16 type and
 *   MLIB_EDGE_SRC_EXTEND mask
 */

#include <mlib_image.h>
#include <mlib_ImageConv.h>
#include <mlib_c_ImageConv.h>
/* *********************************************************** */

/*
 * This define switches between functions of different data types
 */

/* *********************************************************** */
#define	IMG_TYPE	1

#define	DTYPE	mlib_u8
#define	CONV_FUNC(KERN)	mlib_c_conv##KERN##ext_u8(PARAM)

#define	CONV_FUNC_MxN	mlib_c_convMxNext_u8(PARAM_MxN)

#define	CONV_FUNC_I(KERN)	mlib_i_conv##KERN##ext_u8(PARAM)

#define	CONV_FUNC_MxN_I	mlib_i_convMxNext_u8(PARAM_MxN)

/* *********************************************************** */

#define	KSIZE1	(KSIZE - 1)

/* *********************************************************** */

#define	PARAM                                                         \
	mlib_image *dst, const mlib_image *src, mlib_s32 dx_l,        \
	mlib_s32 dx_r, mlib_s32 dy_t, mlib_s32 dy_b,                  \
	const mlib_s32 *kern, mlib_s32 scalef_expon, mlib_s32 cmask

/* *********************************************************** */

#define	PARAM_MxN                                                       \
	mlib_image *dst, const mlib_image *src, const mlib_s32 *kernel, \
	mlib_s32 m, mlib_s32 n, mlib_s32 dx_l, mlib_s32 dx_r,           \
	mlib_s32 dy_t, mlib_s32 dy_b, mlib_s32 scale, mlib_s32 cmask

/* *********************************************************** */

#define	FTYPE	mlib_s32

/* *********************************************************** */

#define	BUFF_LINE	256

/* *********************************************************** */

#define	DEF_VARS(type)                                          \
	type *adr_src, *sl, *sp, *sl1;                          \
	type *adr_dst, *dl, *dp;                                \
	FTYPE *pbuff = buff;                                    \
	mlib_s32 *buffi, *buffo;                                \
	mlib_s32 wid, hgt, sll, dll;                            \
	mlib_s32 nchannel, chan1, chan2;                        \
	mlib_s32 i, j, c, swid

/* *********************************************************** */

#define	GET_SRC_DST_PARAMETERS(type)                            \
	hgt = mlib_ImageGetHeight(src);                         \
	wid = mlib_ImageGetWidth(src);                          \
	nchannel = mlib_ImageGetChannels(src);                  \
	sll = mlib_ImageGetStride(src) / sizeof (type);         \
	dll = mlib_ImageGetStride(dst) / sizeof (type);         \
	adr_src = (type *) mlib_ImageGetData(src);              \
	adr_dst = (type *) mlib_ImageGetData(dst)

/* *********************************************************** */
#define	CLAMP_STORE(dst, val)                                   \
	if (val & 0xffffff00) {                                 \
	    if (val < MLIB_U8_MIN)                              \
		dst = MLIB_U8_MIN;                              \
	    else                                                \
		dst = MLIB_U8_MAX;                              \
	} else {                                                \
	    dst = (mlib_u8)val;                                 \
	}

/* *********************************************************** */

#if IMG_TYPE != 1
#define	SHT	16
#else /* (IMG_TYPE == 1) */
#define	SHT	8
#endif /* (IMG_TYPE == 1) */

/* *********************************************************** */

#define	KSIZE	3

mlib_status
CONV_FUNC(3x3)
{
	DTYPE *adr_src, *sl, *sp0, *sp1, *sp2, *sp_1, *sp_2;
	DTYPE *adr_dst, *dl, *dp;
	mlib_s32 wid, hgt, sll, dll;
	mlib_s32 nchannel, chan1, chan2, delta_chan;
	mlib_s32 i, j, c;
	mlib_s32 shift1, shift2;
	mlib_s32 k0, k1, k2, k3, k4, k5, k6, k7, k8;
	mlib_s32 p02, p03, p12, p13, p22, p23;


	shift1 = SHT;
	shift2 = scalef_expon - shift1;

/* keep kernel in regs */
	k0 = kern[0] >> shift1;
	k1 = kern[1] >> shift1;
	k2 = kern[2] >> shift1;
	k3 = kern[3] >> shift1;
	k4 = kern[4] >> shift1;
	k5 = kern[5] >> shift1;
	k6 = kern[6] >> shift1;
	k7 = kern[7] >> shift1;
	k8 = kern[8] >> shift1;

	GET_SRC_DST_PARAMETERS(DTYPE);

	chan1 = nchannel;
	chan2 = chan1 + chan1;
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
			mlib_s32 s0, s1;
			mlib_s32 pix0, pix1;

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

				pix0 =
				    (s0 + p02 * k2 + p12 * k5 +
				    p22 * k8) >> shift2;
				pix1 =
				    (s1 + p02 * k1 + p03 * k2 + p12 * k4 +
				    p13 * k5 + p22 * k7 + p23 * k8) >> shift2;

				CLAMP_STORE(dp[0], pix0)
				CLAMP_STORE(dp[chan1], pix1)

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
				pix0 =
				    (s0 + p03 * k2 + p13 * k5 +
				    p23 * k8) >> shift2;
				CLAMP_STORE(dp[0], pix0)
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
				pix0 =
				    (s0 + p03 * k2 + p13 * k5 +
				    p23 * k8) >> shift2;
				CLAMP_STORE(dp[0], pix0)
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
#ifndef __sparc	/* for x86, using integer multiplies is faster */

mlib_status
CONV_FUNC_I(3x3)
{
	DTYPE *adr_src, *sl, *sp0, *sp1, *sp2, *sp_1, *sp_2;
	DTYPE *adr_dst, *dl, *dp;
	mlib_s32 wid, hgt, sll, dll;
	mlib_s32 nchannel, chan1, chan2, delta_chan;
	mlib_s32 i, j, c;
	mlib_s32 shift1, shift2;
	mlib_s32 k0, k1, k2, k3, k4, k5, k6, k7, k8;
	mlib_s32 p02, p03, p12, p13, p22, p23;


	shift1 = SHT;
	shift2 = scalef_expon - shift1;

/* keep kernel in regs */
	k0 = kern[0] >> shift1;
	k1 = kern[1] >> shift1;
	k2 = kern[2] >> shift1;
	k3 = kern[3] >> shift1;
	k4 = kern[4] >> shift1;
	k5 = kern[5] >> shift1;
	k6 = kern[6] >> shift1;
	k7 = kern[7] >> shift1;
	k8 = kern[8] >> shift1;

	GET_SRC_DST_PARAMETERS(DTYPE);

	chan1 = nchannel;
	chan2 = chan1 + chan1;
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
			mlib_s32 s0, s1;
			mlib_s32 pix0, pix1;

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

				pix0 =
				    (s0 + p02 * k2 + p12 * k5 +
				    p22 * k8) >> shift2;
				pix1 =
				    (s1 + p02 * k1 + p03 * k2 + p12 * k4 +
				    p13 * k5 + p22 * k7 + p23 * k8) >> shift2;

				CLAMP_STORE(dp[0], pix0)
				CLAMP_STORE(dp[chan1], pix1)

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
				pix0 =
				    (s0 + p03 * k2 + p13 * k5 +
				    p23 * k8) >> shift2;
				CLAMP_STORE(dp[0], pix0)
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
				pix0 =
				    (s0 + p03 * k2 + p13 * k5 +
				    p23 * k8) >> shift2;
				CLAMP_STORE(dp[0], pix0)
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

#endif /* __sparc ( for x86, using integer multiplies is faster ) */
/* *********************************************************** */

#undef  KSIZE
#define	KSIZE	4

mlib_status
CONV_FUNC(4x4)
{
	mlib_s32 buff[(KSIZE + 3) * BUFF_LINE];
	mlib_s32 *buff0, *buff1, *buff2, *buff3, *buff4, *buffd, *buffT;
	mlib_s32 k[KSIZE * KSIZE];
	mlib_s32 d0, d1;
	mlib_s32 k0, k1, k2, k3, k4, k5, k6, k7;
	mlib_s32 p00, p01, p02, p03, p04,
	    p10, p11, p12, p13, p14, p20,
		p21, p22, p23, p30, p31, p32, p33;
	DEF_VARS(DTYPE);
	DTYPE *sl2, *sl3;
	mlib_s32 shift1, shift2;


	shift1 = SHT;
	shift2 = scalef_expon - shift1;

	for (j = 0; j < KSIZE * KSIZE; j++)
		k[j] = kern[j] >> shift1;

	GET_SRC_DST_PARAMETERS(DTYPE);

	swid = wid + KSIZE1;

	if (swid > BUFF_LINE) {
		pbuff = __mlib_malloc((KSIZE + 3) * sizeof (mlib_s32) * swid);

		if (pbuff == NULL)
			return (MLIB_FAILURE);
	}

	buff0 = pbuff;
	buff1 = buff0 + swid;
	buff2 = buff1 + swid;
	buff3 = buff2 + swid;
	buff4 = buff3 + swid;
	buffd = buff4 + swid;
	buffo = (mlib_s32 *)(buffd + swid);
	buffi = buffo + (swid & ~1);

	swid -= (dx_l + dx_r);

	chan1 = nchannel;
	chan2 = chan1 + chan1;

	for (c = 0; c < nchannel; c++) {
		if (!(cmask & (1 << (nchannel - 1 - c))))
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
			buff0[i] = (mlib_s32) sl[0];
			buff1[i] = (mlib_s32) sl1[0];
			buff2[i] = (mlib_s32) sl2[0];
			buff3[i] = (mlib_s32) sl3[0];
		}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < swid; i++) {
			buff0[i + dx_l] = (mlib_s32) sl[i * chan1];
			buff1[i + dx_l] = (mlib_s32) sl1[i * chan1];
			buff2[i + dx_l] = (mlib_s32) sl2[i * chan1];
			buff3[i + dx_l] = (mlib_s32) sl3[i * chan1];
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
				buff4[i + dx_l] = (mlib_s32) sp[0];
				buff4[i + dx_l + 1] = (mlib_s32) sp[chan1];

				buffd[i] =
				    (p00 * k0 + p01 * k1 + p02 * k2 + p03 * k3 +
				    p10 * k4 + p11 * k5 + p12 * k6 + p13 * k7);
				buffd[i + 1] =
				    (p01 * k0 + p02 * k1 + p03 * k2 + p04 * k3 +
				    p11 * k4 + p12 * k5 + p13 * k6 + p14 * k7);

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
				    p03 * k3 + p10 * k4 + p11 * k5 + p12 * k6 +
				    p13 * k7 + buffd[i]) >> shift2;
				d1 = (p01 * k0 + p02 * k1 + p03 * k2 +
				    p04 * k3 + p11 * k4 + p12 * k5 + p13 * k6 +
				    p14 * k7 + buffd[i + 1]) >> shift2;

				CLAMP_STORE(dp[0], d0);
				CLAMP_STORE(dp[chan1], d1);

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

				buff4[i + dx_l] = (mlib_s32) sp[0];

				buffo[i] =
				    (p00 * k[0] + p01 * k[1] + p02 * k[2] +
				    p03 * k[3] + p10 * k[4] + p11 * k[5] +
				    p12 * k[6] + p13 * k[7] + p20 * k[8] +
				    p21 * k[9] + p22 * k[10] + p23 * k[11] +
				    p30 * k[12] + p31 * k[13] + p32 * k[14] +
				    p33 * k[15]) >> shift2;

				CLAMP_STORE(dp[0], buffo[i]);

				sp += chan1;
				dp += chan1;
			}

			for (; i < swid; i++) {
				buff4[i + dx_l] = (mlib_s32) sp[0];
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
	mlib_s32 buff[BUFF_LINE];
	mlib_s32 *buffd;
	mlib_s32 k[KSIZE * KSIZE];
	mlib_s32 shift1, shift2;
	mlib_s32 k0, k1, k2, k3, k4, k5, k6, k7, k8, k9;
	mlib_s32 p00, p01, p02, p03, p04, p05, p10, p11, p12, p13, p14, p15;
	DTYPE *adr_src, *sl, *sp0, *sp1, *sp2, *sp3, *sp4;
	DTYPE *sp_1, *sp_2, *sp_3, *sp_4;
	DTYPE *adr_dst, *dl, *dp;
	mlib_s32 *pbuff = buff;
	mlib_s32 wid, hgt, sll, dll;
	mlib_s32 nchannel, chan1, chan2, chan4;
	mlib_s32 delta_chan1, delta_chan2, delta_chan3;
	mlib_s32 i, j, c;


	shift1 = SHT;
	shift2 = scalef_expon - shift1;

	for (j = 0; j < KSIZE * KSIZE; j++)
		k[j] = kern[j] >> shift1;

	GET_SRC_DST_PARAMETERS(DTYPE);

	if (wid > BUFF_LINE) {
		pbuff = __mlib_malloc(sizeof (mlib_s32) * wid);

		if (pbuff == NULL)
			return (MLIB_FAILURE);
	}

	buffd = pbuff;

	chan1 = nchannel;
	chan2 = chan1 + chan1;

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
			mlib_s32 pix0, pix1;

			dp = dl;
			sp0 = sp_1;
			sp_1 = sp_2;
			sp_2 = sp_3;
			sp_3 = sp_4;
			sp_4 = sl;

			sp1 = sp_1;
			sp2 = sp_2;
			sp3 = sp_3;
			sp4 = sp_4;
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

				buffd[i] =
				    (p00 * k0 + p01 * k1 + p02 * k2 + p03 * k3 +
				    p04 * k4 + p10 * k5 + p11 * k6 + p12 * k7 +
				    p13 * k8 + p14 * k9);
				buffd[i + 1] =
				    (p01 * k0 + p02 * k1 + p03 * k2 + p04 * k3 +
				    p05 * k4 + p11 * k5 + p12 * k6 + p13 * k7 +
				    p14 * k8 + p15 * k9);

				sp0 += chan2;
				sp1 += chan2;
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

				buffd[i] =
				    (p00 * k0 + p01 * k1 + p02 * k2 + p03 * k3 +
				    p04 * k4 + p10 * k5 + p11 * k6 + p12 * k7 +
				    p13 * k8 + p14 * k9);

				sp0 += chan1;
				sp1 += chan1;
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

				buffd[i] =
				    (p00 * k0 + p01 * k1 + p02 * k2 + p03 * k3 +
				    p04 * k4 + p10 * k5 + p11 * k6 + p12 * k7 +
				    p13 * k8 + p14 * k9);
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

			p02 = sp2[0];
			p12 = sp3[0];
			p03 = sp2[delta_chan1];
			p13 = sp3[delta_chan1];
			p04 = sp2[delta_chan2];
			p14 = sp3[delta_chan2];
			p05 = sp2[delta_chan3];
			p15 = sp3[delta_chan3];

			sp2 += chan4;
			sp3 += chan4;

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

				p04 = sp2[0];
				p14 = sp3[0];
				p05 = sp2[chan1];
				p15 = sp3[chan1];

				buffd[i] +=
				    (p00 * k0 + p01 * k1 + p02 * k2 + p03 * k3 +
				    p04 * k4 + p10 * k5 + p11 * k6 + p12 * k7 +
				    p13 * k8 + p14 * k9);
				buffd[i + 1] +=
				    (p01 * k0 + p02 * k1 + p03 * k2 + p04 * k3 +
				    p05 * k4 + p11 * k5 + p12 * k6 + p13 * k7 +
				    p14 * k8 + p15 * k9);

				sp2 += chan2;
				sp3 += chan2;
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

				p04 = sp2[0];
				p14 = sp3[0];

				buffd[i] +=
				    (p00 * k0 + p01 * k1 + p02 * k2 + p03 * k3 +
				    p04 * k4 + p10 * k5 + p11 * k6 + p12 * k7 +
				    p13 * k8 + p14 * k9);

				sp2 += chan1;
				sp3 += chan1;
			}

			sp2 -= chan1;
			sp3 -= chan1;

			for (; i < wid; i++) {
				p00 = p01;
				p10 = p11;
				p01 = p02;
				p11 = p12;
				p02 = p03;
				p12 = p13;
				p03 = p04;
				p13 = p14;

				p04 = sp2[0];
				p14 = sp3[0];

				buffd[i] +=
				    (p00 * k0 + p01 * k1 + p02 * k2 + p03 * k3 +
				    p04 * k4 + p10 * k5 + p11 * k6 + p12 * k7 +
				    p13 * k8 + p14 * k9);
			}
/*
 *  3 loop
 */
			k0 = k[20];
			k1 = k[21];
			k2 = k[22];
			k3 = k[23];
			k4 = k[24];

			p02 = sp4[0];
			p03 = sp4[delta_chan1];
			p04 = sp4[delta_chan2];
			p05 = sp4[delta_chan3];

			sp4 += chan4;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (wid - dx_r - 2); i += 2) {
				p00 = p02;
				p01 = p03;
				p02 = p04;
				p03 = p05;

				p04 = sp4[0];
				p05 = sp4[chan1];

				pix0 =
				    (buffd[i] + p00 * k0 + p01 * k1 + p02 * k2 +
				    p03 * k3 + p04 * k4) >> shift2;
				pix1 =
				    (buffd[i + 1] + p01 * k0 + p02 * k1 +
				    p03 * k2 + p04 * k3 + p05 * k4) >> shift2;

				CLAMP_STORE(dp[0], pix0)
				CLAMP_STORE(dp[chan1], pix1)

				dp += chan2;
				sp4 += chan2;
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

				p04 = sp4[0];

				pix0 =
				    (buffd[i] + p00 * k0 + p01 * k1 + p02 * k2 +
				    p03 * k3 + p04 * k4) >> shift2;
				CLAMP_STORE(dp[0], pix0)

				dp += chan1;
				sp4 += chan1;
			}

			sp4 -= chan1;

			for (; i < wid; i++) {
				p00 = p01;
				p01 = p02;
				p02 = p03;
				p03 = p04;

				p04 = sp4[0];

				pix0 =
				    (buffd[i] + p00 * k0 + p01 * k1 + p02 * k2 +
				    p03 * k3 + p04 * k4) >> shift2;
				CLAMP_STORE(dp[0], pix0)

				dp += chan1;
			}
/* next line */
			if (j < hgt - dy_b - 1)
				sl += sll;
			dl += dll;
		}
	}

	if (pbuff != buff)
		__mlib_free(pbuff);

	return (MLIB_SUCCESS);
}
/* *********************************************************** */


#ifndef __sparc	/* for x86, using integer multiplies is faster */

mlib_status
CONV_FUNC_I(5x5)
{
	mlib_s32 buff[BUFF_LINE];
	mlib_s32 *buffd;
	mlib_s32 k[KSIZE * KSIZE];
	mlib_s32 shift1, shift2;
	mlib_s32 k0, k1, k2, k3, k4, k5, k6, k7, k8, k9;
	mlib_s32 p00, p01, p02, p03, p04, p05, p10, p11, p12, p13, p14, p15;
	DTYPE *adr_src, *sl, *sp0, *sp1, *sp2, *sp3, *sp4;
	DTYPE *sp_1, *sp_2, *sp_3, *sp_4;
	DTYPE *adr_dst, *dl, *dp;
	mlib_s32 *pbuff = buff;
	mlib_s32 wid, hgt, sll, dll;
	mlib_s32 nchannel, chan1, chan2, chan4;
	mlib_s32 delta_chan1, delta_chan2, delta_chan3;
	mlib_s32 i, j, c;


	shift1 = SHT;
	shift2 = scalef_expon - shift1;

	for (j = 0; j < KSIZE * KSIZE; j++)
		k[j] = kern[j] >> shift1;

	GET_SRC_DST_PARAMETERS(DTYPE);

	if (wid > BUFF_LINE) {
		pbuff = __mlib_malloc(sizeof (mlib_s32) * wid);

		if (pbuff == NULL)
			return (MLIB_FAILURE);
	}

	buffd = pbuff;

	chan1 = nchannel;
	chan2 = chan1 + chan1;

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
			mlib_s32 pix0, pix1;

			dp = dl;
			sp0 = sp_1;
			sp_1 = sp_2;
			sp_2 = sp_3;
			sp_3 = sp_4;
			sp_4 = sl;

			sp1 = sp_1;
			sp2 = sp_2;
			sp3 = sp_3;
			sp4 = sp_4;
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

				buffd[i] =
				    (p00 * k0 + p01 * k1 + p02 * k2 + p03 * k3 +
				    p04 * k4 + p10 * k5 + p11 * k6 + p12 * k7 +
				    p13 * k8 + p14 * k9);
				buffd[i + 1] =
				    (p01 * k0 + p02 * k1 + p03 * k2 + p04 * k3 +
				    p05 * k4 + p11 * k5 + p12 * k6 + p13 * k7 +
				    p14 * k8 + p15 * k9);

				sp0 += chan2;
				sp1 += chan2;
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

				buffd[i] =
				    (p00 * k0 + p01 * k1 + p02 * k2 + p03 * k3 +
				    p04 * k4 + p10 * k5 + p11 * k6 + p12 * k7 +
				    p13 * k8 + p14 * k9);

				sp0 += chan1;
				sp1 += chan1;
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

				buffd[i] =
				    (p00 * k0 + p01 * k1 + p02 * k2 + p03 * k3 +
				    p04 * k4 + p10 * k5 + p11 * k6 + p12 * k7 +
				    p13 * k8 + p14 * k9);
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

			p02 = sp2[0];
			p12 = sp3[0];
			p03 = sp2[delta_chan1];
			p13 = sp3[delta_chan1];
			p04 = sp2[delta_chan2];
			p14 = sp3[delta_chan2];
			p05 = sp2[delta_chan3];
			p15 = sp3[delta_chan3];

			sp2 += chan4;
			sp3 += chan4;

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

				p04 = sp2[0];
				p14 = sp3[0];
				p05 = sp2[chan1];
				p15 = sp3[chan1];

				buffd[i] +=
				    (p00 * k0 + p01 * k1 + p02 * k2 + p03 * k3 +
				    p04 * k4 + p10 * k5 + p11 * k6 + p12 * k7 +
				    p13 * k8 + p14 * k9);
				buffd[i + 1] +=
				    (p01 * k0 + p02 * k1 + p03 * k2 + p04 * k3 +
				    p05 * k4 + p11 * k5 + p12 * k6 + p13 * k7 +
				    p14 * k8 + p15 * k9);

				sp2 += chan2;
				sp3 += chan2;
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

				p04 = sp2[0];
				p14 = sp3[0];

				buffd[i] +=
				    (p00 * k0 + p01 * k1 + p02 * k2 + p03 * k3 +
				    p04 * k4 + p10 * k5 + p11 * k6 + p12 * k7 +
				    p13 * k8 + p14 * k9);

				sp2 += chan1;
				sp3 += chan1;
			}

			sp2 -= chan1;
			sp3 -= chan1;

			for (; i < wid; i++) {
				p00 = p01;
				p10 = p11;
				p01 = p02;
				p11 = p12;
				p02 = p03;
				p12 = p13;
				p03 = p04;
				p13 = p14;

				p04 = sp2[0];
				p14 = sp3[0];

				buffd[i] +=
				    (p00 * k0 + p01 * k1 + p02 * k2 + p03 * k3 +
				    p04 * k4 + p10 * k5 + p11 * k6 + p12 * k7 +
				    p13 * k8 + p14 * k9);
			}
/*
 *  3 loop
 */
			k0 = k[20];
			k1 = k[21];
			k2 = k[22];
			k3 = k[23];
			k4 = k[24];

			p02 = sp4[0];
			p03 = sp4[delta_chan1];
			p04 = sp4[delta_chan2];
			p05 = sp4[delta_chan3];

			sp4 += chan4;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (wid - dx_r - 2); i += 2) {
				p00 = p02;
				p01 = p03;
				p02 = p04;
				p03 = p05;

				p04 = sp4[0];
				p05 = sp4[chan1];

				pix0 =
				    (buffd[i] + p00 * k0 + p01 * k1 + p02 * k2 +
				    p03 * k3 + p04 * k4) >> shift2;
				pix1 =
				    (buffd[i + 1] + p01 * k0 + p02 * k1 +
				    p03 * k2 + p04 * k3 + p05 * k4) >> shift2;

				CLAMP_STORE(dp[0], pix0)
				CLAMP_STORE(dp[chan1], pix1)

				dp += chan2;
				sp4 += chan2;
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

				p04 = sp4[0];

				pix0 =
				    (buffd[i] + p00 * k0 + p01 * k1 + p02 * k2 +
				    p03 * k3 + p04 * k4) >> shift2;
				CLAMP_STORE(dp[0], pix0)

				dp += chan1;
				sp4 += chan1;
			}

			sp4 -= chan1;

			for (; i < wid; i++) {
				p00 = p01;
				p01 = p02;
				p02 = p03;
				p03 = p04;

				p04 = sp4[0];

				pix0 =
				    (buffd[i] + p00 * k0 + p01 * k1 + p02 * k2 +
				    p03 * k3 + p04 * k4) >> shift2;
				CLAMP_STORE(dp[0], pix0)

				dp += chan1;
			}
/* next line */
			if (j < hgt - dy_b - 1)
				sl += sll;
			dl += dll;
		}
	}

	if (pbuff != buff)
		__mlib_free(pbuff);

	return (MLIB_SUCCESS);
}

#endif /* __sparc ( for x86, using integer multiplies is faster ) */
/* *********************************************************** */

#if IMG_TYPE == 1

/* *********************************************************** */

#undef  KSIZE
#define	KSIZE	7

mlib_status
CONV_FUNC(7x7)
{
	mlib_s32 buff[(KSIZE + 3) * BUFF_LINE], *buffs[2 * (KSIZE + 1)], *buffd;
	mlib_s32 k[KSIZE * KSIZE];
	mlib_s32 l, m, buff_ind;
	mlib_s32 d0, d1;
	mlib_s32 k0, k1, k2, k3, k4, k5, k6;
	mlib_s32 p0, p1, p2, p3, p4, p5, p6, p7;
	DTYPE *sl2, *sl3, *sl4, *sl5, *sl6;
	mlib_s32 shift1, shift2;
	DEF_VARS(DTYPE);


	shift1 = SHT;
	shift2 = scalef_expon - shift1;

	for (j = 0; j < KSIZE * KSIZE; j++)
		k[j] = kern[j] >> shift1;

	GET_SRC_DST_PARAMETERS(DTYPE);

	swid = wid + KSIZE1;

	if (wid > BUFF_LINE) {
		pbuff = __mlib_malloc((KSIZE + 3) * sizeof (mlib_s32) * wid);

		if (pbuff == NULL)
			return (MLIB_FAILURE);
	}

	for (l = 0; l < KSIZE + 1; l++)
		buffs[l] = pbuff + l * swid;
	for (l = 0; l < KSIZE + 1; l++)
		buffs[l + (KSIZE + 1)] = buffs[l];
	buffd = buffs[KSIZE] + swid;
	buffo = (mlib_s32 *)(buffd + swid);
	buffi = buffo + (swid & ~1);

	swid -= (dx_l + dx_r);

	chan1 = nchannel;
	chan2 = chan1 + chan1;

	for (c = 0; c < nchannel; c++) {
		if (!(cmask & (1 << (nchannel - 1 - c))))
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
			buffs[0][i] = (mlib_s32) sl[0];
			buffs[1][i] = (mlib_s32) sl1[0];
			buffs[2][i] = (mlib_s32) sl2[0];
			buffs[3][i] = (mlib_s32) sl3[0];
			buffs[4][i] = (mlib_s32) sl4[0];
			buffs[5][i] = (mlib_s32) sl5[0];
			buffs[6][i] = (mlib_s32) sl6[0];
		}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < swid; i++) {
			buffs[0][i + dx_l] = (mlib_s32) sl[i * chan1];
			buffs[1][i + dx_l] = (mlib_s32) sl1[i * chan1];
			buffs[2][i + dx_l] = (mlib_s32) sl2[i * chan1];
			buffs[3][i + dx_l] = (mlib_s32) sl3[i * chan1];
			buffs[4][i + dx_l] = (mlib_s32) sl4[i * chan1];
			buffs[5][i + dx_l] = (mlib_s32) sl5[i * chan1];
			buffs[6][i + dx_l] = (mlib_s32) sl6[i * chan1];
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
			buffd[i] = 0;

		if ((hgt - dy_b) > 1)
			sl = sl6 + sll;
		else
			sl = sl6;

		for (j = 0; j < hgt; j++) {
			mlib_s32 **buffc = buffs + buff_ind;
			mlib_s32 *buffn = buffc[KSIZE];
			mlib_s32 *pk = k;

			for (l = 0; l < KSIZE; l++) {
				mlib_s32 *buff = buffc[l];

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
						    p0 * k0 + p1 * k1 +
						    p2 * k2 + p3 * k3 +
						    p4 * k4 + p5 * k5 + p6 * k6;
						buffd[i + 1] +=
						    p1 * k0 + p2 * k1 +
						    p3 * k2 + p4 * k3 +
						    p5 * k4 + p6 * k5 + p7 * k6;
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
						    (mlib_s32) sp[0];
						buffn[i + dx_l + 1] =
						    (mlib_s32) sp[chan1];

						d0 = (p0 * k0 + p1 * k1 +
						    p2 * k2 + p3 * k3 +
						    p4 * k4 + p5 * k5 +
						    p6 * k6 + buffd[i]) >>
							shift2;
						d1 = (p1 * k0 + p2 * k1 +
						    p3 * k2 + p4 * k3 +
						    p5 * k4 + p6 * k5 +
						    p7 * k6 + buffd[i + 1]) >>
							shift2;

						CLAMP_STORE(dp[0], d0);
						CLAMP_STORE(dp[chan1], d1);

						buffd[i] = 0;
						buffd[i + 1] = 0;

						sp += chan2;
						dp += chan2;
					}
				}
			}

/* last pixels */
			for (; i < wid; i++) {
				mlib_s32 *pk = k, s = 0;
				mlib_s32 d0;

				for (l = 0; l < KSIZE; l++) {
					mlib_s32 *buff = buffc[l] + i;

					for (m = 0; m < KSIZE; m++)
						s += buff[m] * ((*pk++));
				}

				d0 = (s) >> shift2;
				CLAMP_STORE(dp[0], d0);

				buffn[i + dx_l] = (mlib_s32) sp[0];

				sp += chan1;
				dp += chan1;
			}

			for (; i < swid; i++) {
				buffn[i + dx_l] = (mlib_s32) sp[0];
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
#endif /* IMG_TYPE == 1 */

/* *********************************************************** */

#define	MAX_KER	7
#define	MAX_N	15

#define	BUFF_SIZE	1600

#define	CACHE_SIZE	(64*1024)
/* *********************************************************** */
#define	STORE_RES(res, x)	x >>= shift2; CLAMP_STORE(res, x)

mlib_status CONV_FUNC_MxN
{
	DTYPE *adr_src, *sl, *sp = NULL;
	DTYPE *adr_dst, *dl, *dp = NULL;
	mlib_s32 buff[BUFF_SIZE], *buffs_arr[2 * (MAX_N + 1)];
	mlib_s32 *pbuff = buff;
	mlib_s32 **buffs = buffs_arr, *buffd;
	mlib_s32 l, off, kw, bsize, buff_ind;
	mlib_s32 d0, d1, shift1, shift2;
	mlib_s32 k0, k1, k2, k3, k4, k5, k6;
	mlib_s32 p0, p1, p2, p3, p4, p5, p6, p7;
	mlib_s32 wid, hgt, sll, dll;
	mlib_s32 nchannel, chan1;
	mlib_s32 i, j, c, swid;
	mlib_s32 chan2;
	mlib_s32 k_locl[MAX_N * MAX_N], *k = k_locl;

	GET_SRC_DST_PARAMETERS(DTYPE);

	shift1 = SHT;
	shift2 = scale - shift1;

	chan1 = nchannel;
	chan2 = chan1 + chan1;

	swid = wid + (m - 1);

	bsize = (n + 2) * swid;

	if ((bsize > BUFF_SIZE) || (n > MAX_N)) {
		pbuff =
		    __mlib_malloc(sizeof (mlib_s32) * bsize +
		    sizeof (mlib_s32 *) * 2 * (n + 1));

		if (pbuff == NULL)
			return (MLIB_FAILURE);
		buffs = (mlib_s32 **)(pbuff + bsize);
	}

	for (l = 0; l < (n + 1); l++)
		buffs[l] = pbuff + l * swid;
	for (l = 0; l < (n + 1); l++)
		buffs[l + (n + 1)] = buffs[l];
	buffd = buffs[n] + swid;

	if (m * n > MAX_N * MAX_N) {
		k = __mlib_malloc(sizeof (mlib_s32) * (m * n));

		if (k == NULL) {
			if (pbuff != buff)
				__mlib_free(pbuff);
			return (MLIB_FAILURE);
		}
	}

	for (i = 0; i < m * n; i++) {
		k[i] = kernel[i] >> shift1;
	}

	swid -= (dx_l + dx_r);

	for (c = 0; c < nchannel; c++) {
		if (!(cmask & (1 << (nchannel - 1 - c))))
			continue;

		sl = adr_src + c;
		dl = adr_dst + c;

		for (l = 0; l < n; l++) {
			mlib_s32 *buff = buffs[l];

			for (i = 0; i < dx_l; i++) {
				buff[i] = (mlib_s32)sl[0];
			}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < swid; i++) {
				buff[i + dx_l] = (mlib_s32)sl[i * chan1];
			}

			for (i = 0; i < dx_r; i++) {
				buff[swid + dx_l + i] = buff[swid + dx_l - 1];
			}

			if ((l >= dy_t) && (l < hgt + n - dy_b - 2))
				sl += sll;
		}

		buff_ind = 0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < wid; i++)
			buffd[i] = 0;

		for (j = 0; j < hgt; j++) {
			mlib_s32 **buffc = buffs + buff_ind;
			mlib_s32 *buffn = buffc[n];
			mlib_s32 *pk = k;

			for (l = 0; l < n; l++) {
				mlib_s32 *buff_l = buffc[l];

				for (off = 0; off < m; ) {
					mlib_s32 *buff = buff_l + off;

					sp = sl;
					dp = dl;

					kw = m - off;

					if (kw > 2 * MAX_KER)
						kw = MAX_KER;
					else if (kw > MAX_KER)
						kw = kw / 2;
					off += kw;

					if (kw == 7) {

						p2 = buff[0];
						p3 = buff[1];
						p4 = buff[2];
						p5 = buff[3];
						p6 = buff[4];
						p7 = buff[5];

						k0 = pk[0];
						k1 = pk[1];
						k2 = pk[2];
						k3 = pk[3];
						k4 = pk[4];
						k5 = pk[5];
						k6 = pk[6];

						if (l < (n - 1) || off < m) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
							for (i = 0;
							    i <= (wid - 2);
							    i += 2) {
								p0 = p2;
								p1 = p3;
								p2 = p4;
								p3 = p5;
								p4 = p6;
								p5 = p7;

								p6 = buff[i +
								    6];
								p7 = buff[i +
								    7];

								buffd[i] +=
								    p0 * k0 +
								    p1 * k1 +
								    p2 * k2 +
								    p3 * k3 +
								    p4 * k4 +
								    p5 * k5 +
								    p6 * k6;
								buffd[i + 1] +=
								    p1 * k0 +
								    p2 * k1 +
								    p3 * k2 +
								    p4 * k3 +
								    p5 * k4 +
								    p6 * k5 +
								    p7 * k6;
							}

						} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
							for (i = 0;
							    i <= (wid - 2);
							    i += 2) {
								p0 = p2;
								p1 = p3;
								p2 = p4;
								p3 = p5;
								p4 = p6;
								p5 = p7;

								p6 = buff[i +
								    6];
								p7 = buff[i +
								    7];

								buffn[i +
								    dx_l] =
								    (mlib_s32)
								    sp[0];
								buffn[i + dx_l +
								    1] =
								    (mlib_s32)
								    sp[chan1];

								d0 = (p0 * k0 +
								    p1 * k1 +
								    p2 * k2 +
								    p3 * k3 +
								    p4 * k4 +
								    p5 * k5 +
								    p6 * k6 +
								    buffd[i]);
								d1 = (p1 * k0 +
								    p2 * k1 +
								    p3 * k2 +
								    p4 * k3 +
								    p5 * k4 +
								    p6 * k5 +
								    p7 * k6 +
								    buffd[i +
								    1]);

								STORE_RES(dp[0],
								    d0);
								STORE_RES(dp
								    [chan1],
								    d1);

								buffd[i] = 0;
								buffd[i + 1] =
								    0;

								sp += chan2;
								dp += chan2;
							}
						}

					} else if (kw == 6) {

						p2 = buff[0];
						p3 = buff[1];
						p4 = buff[2];
						p5 = buff[3];
						p6 = buff[4];

						k0 = pk[0];
						k1 = pk[1];
						k2 = pk[2];
						k3 = pk[3];
						k4 = pk[4];
						k5 = pk[5];

						if (l < (n - 1) || off < m) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
							for (i = 0;
							    i <= (wid - 2);
							    i += 2) {
								p0 = p2;
								p1 = p3;
								p2 = p4;
								p3 = p5;
								p4 = p6;

								p5 = buff[i +
								    5];
								p6 = buff[i +
								    6];

								buffd[i] +=
								    p0 * k0 +
								    p1 * k1 +
								    p2 * k2 +
								    p3 * k3 +
								    p4 * k4 +
								    p5 * k5;
								buffd[i + 1] +=
								    p1 * k0 +
								    p2 * k1 +
								    p3 * k2 +
								    p4 * k3 +
								    p5 * k4 +
								    p6 * k5;
							}

						} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
							for (i = 0;
							    i <= (wid - 2);
							    i += 2) {
								p0 = p2;
								p1 = p3;
								p2 = p4;
								p3 = p5;
								p4 = p6;

								p5 = buff[i +
								    5];
								p6 = buff[i +
								    6];

								buffn[i +
								    dx_l] =
								    (mlib_s32)
								    sp[0];
								buffn[i + dx_l +
								    1] =
								    (mlib_s32)
								    sp[chan1];

								d0 = (p0 * k0 +
								    p1 * k1 +
								    p2 * k2 +
								    p3 * k3 +
								    p4 * k4 +
								    p5 * k5 +
								    buffd[i]);
								d1 = (p1 * k0 +
								    p2 * k1 +
								    p3 * k2 +
								    p4 * k3 +
								    p5 * k4 +
								    p6 * k5 +
								    buffd[i +
								    1]);

								STORE_RES(dp[0],
								    d0);
								STORE_RES(dp
								    [chan1],
								    d1);

								buffd[i] = 0;
								buffd[i + 1] =
								    0;

								sp += chan2;
								dp += chan2;
							}
						}

					} else if (kw == 5) {

						p2 = buff[0];
						p3 = buff[1];
						p4 = buff[2];
						p5 = buff[3];

						k0 = pk[0];
						k1 = pk[1];
						k2 = pk[2];
						k3 = pk[3];
						k4 = pk[4];

						if (l < (n - 1) || off < m) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
							for (i = 0;
							    i <= (wid - 2);
							    i += 2) {
								p0 = p2;
								p1 = p3;
								p2 = p4;
								p3 = p5;

								p4 = buff[i +
								    4];
								p5 = buff[i +
								    5];

								buffd[i] +=
								    p0 * k0 +
								    p1 * k1 +
								    p2 * k2 +
								    p3 * k3 +
								    p4 * k4;
								buffd[i + 1] +=
								    p1 * k0 +
								    p2 * k1 +
								    p3 * k2 +
								    p4 * k3 +
								    p5 * k4;
							}

						} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
							for (i = 0;
							    i <= (wid - 2);
							    i += 2) {
								p0 = p2;
								p1 = p3;
								p2 = p4;
								p3 = p5;

								p4 = buff[i +
								    4];
								p5 = buff[i +
								    5];

								buffn[i +
								    dx_l] =
								    (mlib_s32)
								    sp[0];
								buffn[i + dx_l +
								    1] =
								    (mlib_s32)
								    sp[chan1];

								d0 = (p0 * k0 +
								    p1 * k1 +
								    p2 * k2 +
								    p3 * k3 +
								    p4 * k4 +
								    buffd[i]);
								d1 = (p1 * k0 +
								    p2 * k1 +
								    p3 * k2 +
								    p4 * k3 +
								    p5 * k4 +
								    buffd[i +
								    1]);

								STORE_RES(dp[0],
								    d0);
								STORE_RES(dp
								    [chan1],
								    d1);

								buffd[i] = 0;
								buffd[i + 1] =
								    0;

								sp += chan2;
								dp += chan2;
							}
						}

					} else if (kw == 4) {

						p2 = buff[0];
						p3 = buff[1];
						p4 = buff[2];

						k0 = pk[0];
						k1 = pk[1];
						k2 = pk[2];
						k3 = pk[3];

						if (l < (n - 1) || off < m) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
							for (i = 0;
							    i <= (wid - 2);
							    i += 2) {
								p0 = p2;
								p1 = p3;
								p2 = p4;

								p3 = buff[i +
								    3];
								p4 = buff[i +
								    4];

								buffd[i] +=
								    p0 * k0 +
								    p1 * k1 +
								    p2 * k2 +
								    p3 * k3;
								buffd[i + 1] +=
								    p1 * k0 +
								    p2 * k1 +
								    p3 * k2 +
								    p4 * k3;
							}

						} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
							for (i = 0;
							    i <= (wid - 2);
							    i += 2) {
								p0 = p2;
								p1 = p3;
								p2 = p4;

								p3 = buff[i +
								    3];
								p4 = buff[i +
								    4];

								buffn[i +
								    dx_l] =
								    (mlib_s32)
								    sp[0];
								buffn[i + dx_l +
								    1] =
								    (mlib_s32)
								    sp[chan1];

								d0 = (p0 * k0 +
								    p1 * k1 +
								    p2 * k2 +
								    p3 * k3 +
								    buffd[i]);
								d1 = (p1 * k0 +
								    p2 * k1 +
								    p3 * k2 +
								    p4 * k3 +
								    buffd[i +
								    1]);

								STORE_RES(dp[0],
								    d0);
								STORE_RES(dp
								    [chan1],
								    d1);

								buffd[i] = 0;
								buffd[i + 1] =
								    0;

								sp += chan2;
								dp += chan2;
							}
						}

					} else if (kw == 3) {

						p2 = buff[0];
						p3 = buff[1];
						k0 = pk[0];
						k1 = pk[1];
						k2 = pk[2];

						if (l < (n - 1) || off < m) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
							for (i = 0;
							    i <= (wid - 2);
							    i += 2) {
								p0 = p2;
								p1 = p3;

								p2 = buff[i +
								    2];
								p3 = buff[i +
								    3];

								buffd[i] +=
								    p0 * k0 +
								    p1 * k1 +
								    p2 * k2;
								buffd[i + 1] +=
								    p1 * k0 +
								    p2 * k1 +
								    p3 * k2;
							}

						} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
							for (i = 0;
							    i <= (wid - 2);
							    i += 2) {
								p0 = p2;
								p1 = p3;

								p2 = buff[i +
								    2];
								p3 = buff[i +
								    3];

								buffn[i +
								    dx_l] =
								    (mlib_s32)
								    sp[0];
								buffn[i + dx_l +
								    1] =
								    (mlib_s32)
								    sp[chan1];

								d0 = (p0 * k0 +
								    p1 * k1 +
								    p2 * k2 +
								    buffd[i]);
								d1 = (p1 * k0 +
								    p2 * k1 +
								    p3 * k2 +
								    buffd[i +
								    1]);

								STORE_RES(dp[0],
								    d0);
								STORE_RES(dp
								    [chan1],
								    d1);

								buffd[i] = 0;
								buffd[i + 1] =
								    0;

								sp += chan2;
								dp += chan2;
							}
						}

					} else if (kw == 2) {

						p2 = buff[0];
						k0 = pk[0];
						k1 = pk[1];

						if (l < (n - 1) || off < m) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
							for (i = 0;
							    i <= (wid - 2);
							    i += 2) {
								p0 = p2;

								p1 = buff[i +
								    1];
								p2 = buff[i +
								    2];

								buffd[i] +=
								    p0 * k0 +
								    p1 * k1;
								buffd[i + 1] +=
								    p1 * k0 +
								    p2 * k1;
							}

						} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
							for (i = 0;
							    i <= (wid - 2);
							    i += 2) {
								p0 = p2;

								p1 = buff[i +
								    1];
								p2 = buff[i +
								    2];

								buffn[i +
								    dx_l] =
								    (mlib_s32)
								    sp[0];
								buffn[i + dx_l +
								    1] =
								    (mlib_s32)
								    sp[chan1];

								d0 = (p0 * k0 +
								    p1 * k1 +
								    buffd[i]);
								d1 = (p1 * k0 +
								    p2 * k1 +
								    buffd[i +
								    1]);

								STORE_RES(dp[0],
								    d0);
								STORE_RES(dp
								    [chan1],
								    d1);

								buffd[i] = 0;
								buffd[i + 1] =
								    0;

								sp += chan2;
								dp += chan2;
							}
						}

					} else {	/* kw == 1 */

						k0 = pk[0];

						if (l < (n - 1) || off < m) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
							for (i = 0;
							    i <= (wid - 2);
							    i += 2) {
								p0 = buff[i];
								p1 = buff[i +
								    1];

								buffd[i] +=
								    p0 * k0;
								buffd[i + 1] +=
								    p1 * k0;
							}

						} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
							for (i = 0;
							    i <= (wid - 2);
							    i += 2) {
								p0 = buff[i];
								p1 = buff[i +
								    1];

								buffn[i +
								    dx_l] =
								    (mlib_s32)
								    sp[0];
								buffn[i + dx_l +
								    1] =
								    (mlib_s32)
								    sp[chan1];

								d0 = (p0 * k0 +
								    buffd[i]);
								d1 = (p1 * k0 +
								    buffd[i +
								    1]);

								STORE_RES(dp[0],
								    d0);
								STORE_RES(dp
								    [chan1],
								    d1);

								buffd[i] = 0;
								buffd[i + 1] =
								    0;

								sp += chan2;
								dp += chan2;
							}
						}
					}

					pk += kw;
				}
			}

/* last pixels */
			for (; i < wid; i++) {
				mlib_s32 *pk = k, x, s = 0;

				for (l = 0; l < n; l++) {
					mlib_s32 *buff = buffc[l] + i;

					for (x = 0; x < m; x++)
						s += buff[x] * ((*pk++));
				}

				STORE_RES(dp[0], s);

				buffn[i + dx_l] = (mlib_s32)sp[0];

				sp += chan1;
				dp += chan1;
			}

			for (; i < swid; i++) {
				buffn[i + dx_l] = (mlib_s32)sp[0];
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

			if (buff_ind >= n + 1)
				buff_ind = 0;
		}
	}

	if (pbuff != buff)
		__mlib_free(pbuff);
	if (k != k_locl)
		__mlib_free(k);

	return (MLIB_SUCCESS);
}
/* *********************************************************** */

#ifndef __sparc	/* for x86, using integer multiplies is faster */

/* *********************************************************** */

#define	STORE_RES(res, x)	x >>= shift2; CLAMP_STORE(res, x)

mlib_status CONV_FUNC_MxN_I
{
	DTYPE *adr_src, *sl, *sp = NULL;
	DTYPE *adr_dst, *dl, *dp = NULL;
	mlib_s32 buff[BUFF_SIZE], *buffs_arr[2 * (MAX_N + 1)];
	mlib_s32 *pbuff = buff;
	mlib_s32 **buffs = buffs_arr, *buffd;
	mlib_s32 l, off, kw, bsize, buff_ind;
	mlib_s32 d0, d1, shift1, shift2;
	mlib_s32 k0, k1, k2, k3, k4, k5, k6;
	mlib_s32 p0, p1, p2, p3, p4, p5, p6, p7;
	mlib_s32 wid, hgt, sll, dll;
	mlib_s32 nchannel, chan1;
	mlib_s32 i, j, c, swid;
	mlib_s32 chan2;
	mlib_s32 k_locl[MAX_N * MAX_N], *k = k_locl;

	GET_SRC_DST_PARAMETERS(DTYPE);

	shift1 = SHT;
	shift2 = scale - shift1;

	chan1 = nchannel;
	chan2 = chan1 + chan1;

	swid = wid + (m - 1);

	bsize = (n + 2) * swid;

	if ((bsize > BUFF_SIZE) || (n > MAX_N)) {
		pbuff =
		    __mlib_malloc(sizeof (mlib_s32) * bsize +
		    sizeof (mlib_s32 *) * 2 * (n + 1));

		if (pbuff == NULL)
			return (MLIB_FAILURE);
		buffs = (mlib_s32 **)(pbuff + bsize);
	}

	for (l = 0; l < (n + 1); l++)
		buffs[l] = pbuff + l * swid;
	for (l = 0; l < (n + 1); l++)
		buffs[l + (n + 1)] = buffs[l];
	buffd = buffs[n] + swid;

	if (m * n > MAX_N * MAX_N) {
		k = __mlib_malloc(sizeof (mlib_s32) * (m * n));

		if (k == NULL) {
			if (pbuff != buff)
				__mlib_free(pbuff);
			return (MLIB_FAILURE);
		}
	}

	for (i = 0; i < m * n; i++) {
		k[i] = kernel[i] >> shift1;
	}

	swid -= (dx_l + dx_r);

	for (c = 0; c < nchannel; c++) {
		if (!(cmask & (1 << (nchannel - 1 - c))))
			continue;

		sl = adr_src + c;
		dl = adr_dst + c;

		for (l = 0; l < n; l++) {
			mlib_s32 *buff = buffs[l];

			for (i = 0; i < dx_l; i++) {
				buff[i] = (mlib_s32)sl[0];
			}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < swid; i++) {
				buff[i + dx_l] = (mlib_s32)sl[i * chan1];
			}

			for (i = 0; i < dx_r; i++) {
				buff[swid + dx_l + i] = buff[swid + dx_l - 1];
			}

			if ((l >= dy_t) && (l < hgt + n - dy_b - 2))
				sl += sll;
		}

		buff_ind = 0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < wid; i++)
			buffd[i] = 0;

		for (j = 0; j < hgt; j++) {
			mlib_s32 **buffc = buffs + buff_ind;
			mlib_s32 *buffn = buffc[n];
			mlib_s32 *pk = k;

			for (l = 0; l < n; l++) {
				mlib_s32 *buff_l = buffc[l];

				for (off = 0; off < m; ) {
					mlib_s32 *buff = buff_l + off;

					sp = sl;
					dp = dl;

					kw = m - off;

					if (kw > 2 * MAX_KER)
						kw = MAX_KER;
					else if (kw > MAX_KER)
						kw = kw / 2;
					off += kw;

					if (kw == 7) {

						p2 = buff[0];
						p3 = buff[1];
						p4 = buff[2];
						p5 = buff[3];
						p6 = buff[4];
						p7 = buff[5];

						k0 = pk[0];
						k1 = pk[1];
						k2 = pk[2];
						k3 = pk[3];
						k4 = pk[4];
						k5 = pk[5];
						k6 = pk[6];

						if (l < (n - 1) || off < m) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
							for (i = 0;
							    i <= (wid - 2);
							    i += 2) {
								p0 = p2;
								p1 = p3;
								p2 = p4;
								p3 = p5;
								p4 = p6;
								p5 = p7;

								p6 = buff[i +
								    6];
								p7 = buff[i +
								    7];

								buffd[i] +=
								    p0 * k0 +
								    p1 * k1 +
								    p2 * k2 +
								    p3 * k3 +
								    p4 * k4 +
								    p5 * k5 +
								    p6 * k6;
								buffd[i + 1] +=
								    p1 * k0 +
								    p2 * k1 +
								    p3 * k2 +
								    p4 * k3 +
								    p5 * k4 +
								    p6 * k5 +
								    p7 * k6;
							}

						} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
							for (i = 0;
							    i <= (wid - 2);
							    i += 2) {
								p0 = p2;
								p1 = p3;
								p2 = p4;
								p3 = p5;
								p4 = p6;
								p5 = p7;

								p6 = buff[i +
								    6];
								p7 = buff[i +
								    7];

								buffn[i +
								    dx_l] =
								    (mlib_s32)
								    sp[0];
								buffn[i + dx_l +
								    1] =
								    (mlib_s32)
								    sp[chan1];

								d0 = (p0 * k0 +
								    p1 * k1 +
								    p2 * k2 +
								    p3 * k3 +
								    p4 * k4 +
								    p5 * k5 +
								    p6 * k6 +
								    buffd[i]);
								d1 = (p1 * k0 +
								    p2 * k1 +
								    p3 * k2 +
								    p4 * k3 +
								    p5 * k4 +
								    p6 * k5 +
								    p7 * k6 +
								    buffd[i +
								    1]);

								STORE_RES(dp[0],
								    d0);
								STORE_RES(dp
								    [chan1],
								    d1);

								buffd[i] = 0;
								buffd[i + 1] =
								    0;

								sp += chan2;
								dp += chan2;
							}
						}

					} else if (kw == 6) {

						p2 = buff[0];
						p3 = buff[1];
						p4 = buff[2];
						p5 = buff[3];
						p6 = buff[4];

						k0 = pk[0];
						k1 = pk[1];
						k2 = pk[2];
						k3 = pk[3];
						k4 = pk[4];
						k5 = pk[5];

						if (l < (n - 1) || off < m) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
							for (i = 0;
							    i <= (wid - 2);
							    i += 2) {
								p0 = p2;
								p1 = p3;
								p2 = p4;
								p3 = p5;
								p4 = p6;

								p5 = buff[i +
								    5];
								p6 = buff[i +
								    6];

								buffd[i] +=
								    p0 * k0 +
								    p1 * k1 +
								    p2 * k2 +
								    p3 * k3 +
								    p4 * k4 +
								    p5 * k5;
								buffd[i + 1] +=
								    p1 * k0 +
								    p2 * k1 +
								    p3 * k2 +
								    p4 * k3 +
								    p5 * k4 +
								    p6 * k5;
							}

						} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
							for (i = 0;
							    i <= (wid - 2);
							    i += 2) {
								p0 = p2;
								p1 = p3;
								p2 = p4;
								p3 = p5;
								p4 = p6;

								p5 = buff[i +
								    5];
								p6 = buff[i +
								    6];

								buffn[i +
								    dx_l] =
								    (mlib_s32)
								    sp[0];
								buffn[i + dx_l +
								    1] =
								    (mlib_s32)
								    sp[chan1];

								d0 = (p0 * k0 +
								    p1 * k1 +
								    p2 * k2 +
								    p3 * k3 +
								    p4 * k4 +
								    p5 * k5 +
								    buffd[i]);
								d1 = (p1 * k0 +
								    p2 * k1 +
								    p3 * k2 +
								    p4 * k3 +
								    p5 * k4 +
								    p6 * k5 +
								    buffd[i +
								    1]);

								STORE_RES(dp[0],
								    d0);
								STORE_RES(dp
								    [chan1],
								    d1);

								buffd[i] = 0;
								buffd[i + 1] =
								    0;

								sp += chan2;
								dp += chan2;
							}
						}

					} else if (kw == 5) {

						p2 = buff[0];
						p3 = buff[1];
						p4 = buff[2];
						p5 = buff[3];

						k0 = pk[0];
						k1 = pk[1];
						k2 = pk[2];
						k3 = pk[3];
						k4 = pk[4];

						if (l < (n - 1) || off < m) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
							for (i = 0;
							    i <= (wid - 2);
							    i += 2) {
								p0 = p2;
								p1 = p3;
								p2 = p4;
								p3 = p5;

								p4 = buff[i +
								    4];
								p5 = buff[i +
								    5];

								buffd[i] +=
								    p0 * k0 +
								    p1 * k1 +
								    p2 * k2 +
								    p3 * k3 +
								    p4 * k4;
								buffd[i + 1] +=
								    p1 * k0 +
								    p2 * k1 +
								    p3 * k2 +
								    p4 * k3 +
								    p5 * k4;
							}

						} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
							for (i = 0;
							    i <= (wid - 2);
							    i += 2) {
								p0 = p2;
								p1 = p3;
								p2 = p4;
								p3 = p5;

								p4 = buff[i +
								    4];
								p5 = buff[i +
								    5];

								buffn[i +
								    dx_l] =
								    (mlib_s32)
								    sp[0];
								buffn[i + dx_l +
								    1] =
								    (mlib_s32)
								    sp[chan1];

								d0 = (p0 * k0 +
								    p1 * k1 +
								    p2 * k2 +
								    p3 * k3 +
								    p4 * k4 +
								    buffd[i]);
								d1 = (p1 * k0 +
								    p2 * k1 +
								    p3 * k2 +
								    p4 * k3 +
								    p5 * k4 +
								    buffd[i +
								    1]);

								STORE_RES(dp[0],
								    d0);
								STORE_RES(dp
								    [chan1],
								    d1);

								buffd[i] = 0;
								buffd[i + 1] =
								    0;

								sp += chan2;
								dp += chan2;
							}
						}

					} else if (kw == 4) {

						p2 = buff[0];
						p3 = buff[1];
						p4 = buff[2];

						k0 = pk[0];
						k1 = pk[1];
						k2 = pk[2];
						k3 = pk[3];

						if (l < (n - 1) || off < m) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
							for (i = 0;
							    i <= (wid - 2);
							    i += 2) {
								p0 = p2;
								p1 = p3;
								p2 = p4;

								p3 = buff[i +
								    3];
								p4 = buff[i +
								    4];

								buffd[i] +=
								    p0 * k0 +
								    p1 * k1 +
								    p2 * k2 +
								    p3 * k3;
								buffd[i + 1] +=
								    p1 * k0 +
								    p2 * k1 +
								    p3 * k2 +
								    p4 * k3;
							}

						} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
							for (i = 0;
							    i <= (wid - 2);
							    i += 2) {
								p0 = p2;
								p1 = p3;
								p2 = p4;

								p3 = buff[i +
								    3];
								p4 = buff[i +
								    4];

								buffn[i +
								    dx_l] =
								    (mlib_s32)
								    sp[0];
								buffn[i + dx_l +
								    1] =
								    (mlib_s32)
								    sp[chan1];

								d0 = (p0 * k0 +
								    p1 * k1 +
								    p2 * k2 +
								    p3 * k3 +
								    buffd[i]);
								d1 = (p1 * k0 +
								    p2 * k1 +
								    p3 * k2 +
								    p4 * k3 +
								    buffd[i +
								    1]);

								STORE_RES(dp[0],
								    d0);
								STORE_RES(dp
								    [chan1],
								    d1);

								buffd[i] = 0;
								buffd[i + 1] =
								    0;

								sp += chan2;
								dp += chan2;
							}
						}

					} else if (kw == 3) {

						p2 = buff[0];
						p3 = buff[1];
						k0 = pk[0];
						k1 = pk[1];
						k2 = pk[2];

						if (l < (n - 1) || off < m) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
							for (i = 0;
							    i <= (wid - 2);
							    i += 2) {
								p0 = p2;
								p1 = p3;

								p2 = buff[i +
								    2];
								p3 = buff[i +
								    3];

								buffd[i] +=
								    p0 * k0 +
								    p1 * k1 +
								    p2 * k2;
								buffd[i + 1] +=
								    p1 * k0 +
								    p2 * k1 +
								    p3 * k2;
							}

						} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
							for (i = 0;
							    i <= (wid - 2);
							    i += 2) {
								p0 = p2;
								p1 = p3;

								p2 = buff[i +
								    2];
								p3 = buff[i +
								    3];

								buffn[i +
								    dx_l] =
								    (mlib_s32)
								    sp[0];
								buffn[i + dx_l +
								    1] =
								    (mlib_s32)
								    sp[chan1];

								d0 = (p0 * k0 +
								    p1 * k1 +
								    p2 * k2 +
								    buffd[i]);
								d1 = (p1 * k0 +
								    p2 * k1 +
								    p3 * k2 +
								    buffd[i +
								    1]);

								STORE_RES(dp[0],
								    d0);
								STORE_RES(dp
								    [chan1],
								    d1);

								buffd[i] = 0;
								buffd[i + 1] =
								    0;

								sp += chan2;
								dp += chan2;
							}
						}

					} else if (kw == 2) {

						p2 = buff[0];
						k0 = pk[0];
						k1 = pk[1];

						if (l < (n - 1) || off < m) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
							for (i = 0;
							    i <= (wid - 2);
							    i += 2) {
								p0 = p2;

								p1 = buff[i +
								    1];
								p2 = buff[i +
								    2];

								buffd[i] +=
								    p0 * k0 +
								    p1 * k1;
								buffd[i + 1] +=
								    p1 * k0 +
								    p2 * k1;
							}

						} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
							for (i = 0;
							    i <= (wid - 2);
							    i += 2) {
								p0 = p2;

								p1 = buff[i +
								    1];
								p2 = buff[i +
								    2];

								buffn[i +
								    dx_l] =
								    (mlib_s32)
								    sp[0];
								buffn[i + dx_l +
								    1] =
								    (mlib_s32)
								    sp[chan1];

								d0 = (p0 * k0 +
								    p1 * k1 +
								    buffd[i]);
								d1 = (p1 * k0 +
								    p2 * k1 +
								    buffd[i +
								    1]);

								STORE_RES(dp[0],
								    d0);
								STORE_RES(dp
								    [chan1],
								    d1);

								buffd[i] = 0;
								buffd[i + 1] =
								    0;

								sp += chan2;
								dp += chan2;
							}
						}

					} else {	/* kw == 1 */

						k0 = pk[0];

						if (l < (n - 1) || off < m) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
							for (i = 0;
							    i <= (wid - 2);
							    i += 2) {
								p0 = buff[i];
								p1 = buff[i +
								    1];

								buffd[i] +=
								    p0 * k0;
								buffd[i + 1] +=
								    p1 * k0;
							}

						} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
							for (i = 0;
							    i <= (wid - 2);
							    i += 2) {
								p0 = buff[i];
								p1 = buff[i +
								    1];

								buffn[i +
								    dx_l] =
								    (mlib_s32)
								    sp[0];
								buffn[i + dx_l +
								    1] =
								    (mlib_s32)
								    sp[chan1];

								d0 = (p0 * k0 +
								    buffd[i]);
								d1 = (p1 * k0 +
								    buffd[i +
								    1]);

								STORE_RES(dp[0],
								    d0);
								STORE_RES(dp
								    [chan1],
								    d1);

								buffd[i] = 0;
								buffd[i + 1] =
								    0;

								sp += chan2;
								dp += chan2;
							}
						}
					}

					pk += kw;
				}
			}

/* last pixels */
			for (; i < wid; i++) {
				mlib_s32 *pk = k, x, s = 0;

				for (l = 0; l < n; l++) {
					mlib_s32 *buff = buffc[l] + i;

					for (x = 0; x < m; x++)
						s += buff[x] * ((*pk++));
				}

				STORE_RES(dp[0], s);

				buffn[i + dx_l] = (mlib_s32)sp[0];

				sp += chan1;
				dp += chan1;
			}

			for (; i < swid; i++) {
				buffn[i + dx_l] = (mlib_s32)sp[0];
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

			if (buff_ind >= n + 1)
				buff_ind = 0;
		}
	}

	if (pbuff != buff)
		__mlib_free(pbuff);
	if (k != k_locl)
		__mlib_free(k);

	return (MLIB_SUCCESS);
}

#endif /* __sparc ( for x86, using integer multiplies is faster ) */
