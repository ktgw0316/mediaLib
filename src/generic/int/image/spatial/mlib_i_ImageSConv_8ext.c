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

#pragma ident   "@(#)mlib_i_ImageSConv_8ext.c	9.2    07/11/05 SMI"

/*
 * FUNCTION
 *   Internal functions for mlib_ImageSConv* with MLIB_EDGE_DST_NO_WRITE
 */

#include <mlib_image.h>
#include <mlib_ImageSConv.h>
/* *********************************************************** */
/*
 *  This define switches between functions of different data types:
 *  Files:
 *        mlib_c_ImageSConv_8ext.c mlib_c_ImageSConv_16ext.c
 *        mlib_c_ImageSConv_u16ext.c mlib_ImageSConv_32ext.c
 *        mlib_ImageSConv_F32ext.c mlib_ImageSConv_D64ext.c
 */
/* *********************************************************** */
#define	IMG_TYPE	1
/* *********************************************************** */

#define	PARAM_INT                                                \
	mlib_image *dst, const mlib_image *src, mlib_s32 dx_l,   \
	mlib_s32 dx_r, mlib_s32 dy_t, mlib_s32 dy_b,             \
	const mlib_s32 *hkernel, const mlib_s32 *vkernel,        \
	mlib_s32 scalef_expon, mlib_s32 cmask

/* *********************************************************** */

#define	DTYPE	mlib_u8
#define	FTYPE	mlib_s32

#define	CONV_FUNC(KERN)	mlib_sconv##KERN##ext_u8(PARAM_INT)

/* *********************************************************** */
#define	BUFF_LINE	256
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

#define	CLAMP_STORE(dst, val)                                   \
	if (val >= MLIB_U8_MAX)                                 \
	    dst = MLIB_U8_MAX;                                  \
	else if (val <= MLIB_U8_MIN)                            \
	    dst = MLIB_U8_MIN;                                  \
	else                                                    \
	    dst = (mlib_u8)val

/* *********************************************************** */

#if (IMG_TYPE == 1)
#define	SHT 8
#else /* (IMG_TYPE == 1) */
#define	SHT 16
#endif /* (IMG_TYPE == 1) */

/* *********************************************************** */

#undef  KSIZE
#define	KSIZE	3

/* *********************************************************** */

#undef  KSIZE1
#define	KSIZE1	2

mlib_status
CONV_FUNC(3x3)
{
	GET_SRC_DST_PARAMETERS(DTYPE);
	DTYPE *sl, *sp, *sl1, *dl, *dp;
	mlib_s32 buff[KSIZE * BUFF_LINE];
	mlib_s32 *pbuff = buff, *buff0, *buff1, *buff2, *buffT;
	mlib_s32 h0, h1, h2;
	mlib_s32 v0, v1, v2;
	mlib_s32 a0, a1, a2;
	mlib_s32 b0, b1, b2;
	mlib_s32 s0, s1, s2, s3;
	mlib_s32 pix0, pix1;
	mlib_s32 shift1, shift2;
	mlib_s32 i, j, c, swid;

	swid = wid + KSIZE1;

	if (swid > BUFF_LINE) {
		pbuff = __mlib_malloc(KSIZE * sizeof (FTYPE) * swid);

		if (pbuff == NULL)
			return (MLIB_FAILURE);
	}

	buff0 = pbuff;
	buff1 = buff0 + swid;
	buff2 = buff1 + swid;

	swid -= (dx_l + dx_r);

	shift1 = SHT;
	shift2 = 2 * scalef_expon - shift1 - 32;

	h0 = hkernel[0] >> shift1;
	h1 = hkernel[1] >> shift1;
	h2 = hkernel[2] >> shift1;
	v0 = vkernel[0] >> 16;
	v1 = vkernel[1] >> 16;
	v2 = vkernel[2] >> 16;

	for (c = 0; c < chan1; c++) {
		if (!(cmask & (1 << (chan1 - 1 - c))))
			continue;

		sl = adr_src + c;
		dl = adr_dst + c;

		if ((1 > dy_t) && (1 < hgt + KSIZE1 - dy_b))
			sl1 = sl + sll;
		else
			sl1 = sl;

		for (i = 0; i < dx_l; i++) {
			buff0[i] = sl[0];
			buff1[i] = sl1[0];
		}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < swid; i++) {
			buff0[i + dx_l] = sl[i * chan1];
			buff1[i + dx_l] = sl1[i * chan1];
		}

		for (i = 0; i < dx_r; i++) {
			buff0[swid + dx_l + i] = buff0[swid + dx_l - 1];
			buff1[swid + dx_l + i] = buff1[swid + dx_l - 1];
		}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < wid; i++) {
			buff0[i] =
			    (buff0[i] * h0 + buff0[i + 1] * h1 + buff0[i +
			    2] * h2) >> 16;
			buff1[i] =
			    (buff1[i] * h0 + buff1[i + 1] * h1 + buff1[i +
			    2] * h2) >> 16;
		}

		if ((hgt - dy_b) > 0)
			sl = sl1 + sll;
		else
			sl = sl1;

		for (j = 0; j < hgt; j++) {
			sp = sl;
			dp = dl;

			s2 = sp[0];

			if ((1 > dx_l) && (1 < wid + KSIZE1 - dx_r))
				sp += chan1;
			s3 = sp[0];
			sp += chan1;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (wid - dx_r - 2); i += 2) {
				a0 = buff0[i];
				b0 = buff0[i + 1];
				a1 = buff1[i];
				b1 = buff1[i + 1];

				s0 = s2;
				s1 = s3;
				s2 = sp[0];
				s3 = sp[chan1];

				a2 = (s0 * h0 + s1 * h1 + s2 * h2) >> 16;
				b2 = (s1 * h0 + s2 * h1 + s3 * h2) >> 16;
				buff2[i] = a2;
				buff2[i + 1] = b2;

				pix0 = (a0 * v0 + a1 * v1 + a2 * v2) >> shift2;
				pix1 = (b0 * v0 + b1 * v1 + b2 * v2) >> shift2;
				CLAMP_STORE(dp[0], pix0);
				CLAMP_STORE(dp[chan1], pix1);

				sp += chan2;
				dp += chan2;
			}

			s1 = s2;
			s2 = s3;

			for (; i < wid - dx_r; i++) {
				a0 = buff0[i];
				a1 = buff1[i];

				s0 = s1;
				s1 = s2;
				s2 = sp[0];

				a2 = (s0 * h0 + s1 * h1 + s2 * h2) >> 16;
				buff2[i] = a2;

				pix0 = (a0 * v0 + a1 * v1 + a2 * v2) >> shift2;
				CLAMP_STORE(dp[0], pix0);

				sp += chan1;
				dp += chan1;
			}

			sp -= chan1;

			for (; i < wid; i++) {
				a0 = buff0[i];
				a1 = buff1[i];

				s0 = s1;
				s1 = s2;
				s2 = sp[0];

				a2 = (s0 * h0 + s1 * h1 + s2 * h2) >> 16;
				buff2[i] = a2;

				pix0 = (a0 * v0 + a1 * v1 + a2 * v2) >> shift2;
				CLAMP_STORE(dp[0], pix0);

				dp += chan1;
			}

			if (j < hgt - dy_b - 1)
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
#define	KSIZE	5

/* *********************************************************** */

#undef  KSIZE1
#define	KSIZE1	4

mlib_status
CONV_FUNC(5x5)
{
	GET_SRC_DST_PARAMETERS(DTYPE);
	DTYPE *sl, *sp, *sl1, *dl, *dp;
	DTYPE *sl2, *sl3;
	mlib_s32 buff[KSIZE * BUFF_LINE];
	mlib_s32 *pbuff = buff, *buff0, *buff1, *buff2, *buff3, *buff4, *buffT;
	mlib_s32 h0, h1, h2, h3, h4;
	mlib_s32 v0, v1, v2, v3, v4;
	mlib_s32 a0, a1, a2, a3, a4;
	mlib_s32 b0, b1, b2, b3, b4;
	mlib_s32 s0, s1, s2, s3, s4, s5;
	mlib_s32 pix0, pix1;
	mlib_s32 shift1, shift2;
	mlib_s32 i, j, c, swid;

	swid = wid + KSIZE1;

	if (swid > BUFF_LINE) {
		pbuff = __mlib_malloc(KSIZE * sizeof (FTYPE) * swid);

		if (pbuff == NULL)
			return (MLIB_FAILURE);
	}

	buff0 = pbuff;
	buff1 = buff0 + swid;
	buff2 = buff1 + swid;
	buff3 = buff2 + swid;
	buff4 = buff3 + swid;

	swid -= (dx_l + dx_r);

	shift1 = SHT;
	shift2 = 2 * scalef_expon - shift1 - 32;

	h0 = hkernel[0] >> shift1;
	h1 = hkernel[1] >> shift1;
	h2 = hkernel[2] >> shift1;
	h3 = hkernel[3] >> shift1;
	h4 = hkernel[4] >> shift1;
	v0 = vkernel[0] >> 16;
	v1 = vkernel[1] >> 16;
	v2 = vkernel[2] >> 16;
	v3 = vkernel[3] >> 16;
	v4 = vkernel[4] >> 16;

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

		for (i = 0; i < dx_l; i++) {
			buff0[i] = sl[0];
			buff1[i] = sl1[0];
			buff2[i] = sl2[0];
			buff3[i] = sl3[0];
		}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < swid; i++) {
			buff0[i + dx_l] = sl[i * chan1];
			buff1[i + dx_l] = sl1[i * chan1];
			buff2[i + dx_l] = sl2[i * chan1];
			buff3[i + dx_l] = sl3[i * chan1];
		}

		for (i = 0; i < dx_r; i++) {
			buff0[swid + dx_l + i] = buff0[swid + dx_l - 1];
			buff1[swid + dx_l + i] = buff1[swid + dx_l - 1];
			buff2[swid + dx_l + i] = buff2[swid + dx_l - 1];
			buff3[swid + dx_l + i] = buff3[swid + dx_l - 1];
		}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < wid; i++) {
			buff0[i] = (buff0[i] * h0 + buff0[i + 1] * h1 +
			    buff0[i + 2] * h2 + buff0[i + 3] * h3 +
			    buff0[i + 4] * h4) >> 16;
			buff1[i] = (buff1[i] * h0 + buff1[i + 1] * h1 +
			    buff1[i + 2] * h2 + buff1[i + 3] * h3 +
			    buff1[i + 4] * h4) >> 16;
			buff2[i] = (buff2[i] * h0 + buff2[i + 1] * h1 +
			    buff2[i + 2] * h2 + buff2[i + 3] * h3 +
			    buff2[i + 4] * h4) >> 16;
			buff3[i] = (buff3[i] * h0 + buff3[i + 1] * h1 +
			    buff3[i + 2] * h2 + buff3[i + 3] * h3 +
			    buff3[i + 4] * h4) >> 16;
		}

		if ((hgt - dy_b) > 0)
			sl = sl3 + sll;
		else
			sl = sl3;

		for (j = 0; j < hgt; j++) {
			sp = sl;
			dp = dl;

			s2 = sp[0];

			if ((1 > dx_l) && (1 < wid + KSIZE1 - dx_r))
				sp += chan1;
			s3 = sp[0];

			if ((2 > dx_l) && (2 < wid + KSIZE1 - dx_r))
				sp += chan1;
			s4 = sp[0];

			if ((3 > dx_l) && (3 < wid + KSIZE1 - dx_r))
				sp += chan1;
			s5 = sp[0];
			sp += chan1;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (wid - dx_r - 2); i += 2) {
				a0 = buff0[i];
				b0 = buff0[i + 1];
				a1 = buff1[i];
				b1 = buff1[i + 1];
				a2 = buff2[i];
				b2 = buff2[i + 1];
				a3 = buff3[i];
				b3 = buff3[i + 1];

				s0 = s2;
				s1 = s3;
				s2 = s4;
				s3 = s5;
				s4 = sp[0];
				s5 = sp[chan1];

				a4 = (s0 * h0 + s1 * h1 + s2 * h2 + s3 * h3 +
				    s4 * h4) >> 16;
				b4 = (s1 * h0 + s2 * h1 + s3 * h2 + s4 * h3 +
				    s5 * h4) >> 16;
				buff4[i] = a4;
				buff4[i + 1] = b4;

				pix0 =
				    (a0 * v0 + a1 * v1 + a2 * v2 + a3 * v3 +
				    a4 * v4) >> shift2;
				pix1 =
				    (b0 * v0 + b1 * v1 + b2 * v2 + b3 * v3 +
				    b4 * v4) >> shift2;
				CLAMP_STORE(dp[0], pix0);
				CLAMP_STORE(dp[chan1], pix1);

				sp += chan2;
				dp += chan2;
			}

			s1 = s2;
			s2 = s3;
			s3 = s4;
			s4 = s5;

			for (; i < wid - dx_r; i++) {
				a0 = buff0[i];
				a1 = buff1[i];
				a2 = buff2[i];
				a3 = buff3[i];

				s0 = s1;
				s1 = s2;
				s2 = s3;
				s3 = s4;
				s4 = sp[0];

				a4 = (s0 * h0 + s1 * h1 + s2 * h2 + s3 * h3 +
				    s4 * h4) >> 16;
				buff4[i] = a4;

				pix0 =
				    (a0 * v0 + a1 * v1 + a2 * v2 + a3 * v3 +
				    a4 * v4) >> shift2;
				CLAMP_STORE(dp[0], pix0);

				sp += chan1;
				dp += chan1;
			}

			sp -= chan1;

			for (; i < wid; i++) {
				a0 = buff0[i];
				a1 = buff1[i];
				a2 = buff2[i];
				a3 = buff3[i];

				s0 = s1;
				s1 = s2;
				s2 = s3;
				s3 = s4;
				s4 = sp[0];

				a4 = (s0 * h0 + s1 * h1 + s2 * h2 + s3 * h3 +
				    s4 * h4) >> 16;
				buff4[i] = a4;

				pix0 =
				    (a0 * v0 + a1 * v1 + a2 * v2 + a3 * v3 +
				    a4 * v4) >> shift2;
				CLAMP_STORE(dp[0], pix0);

				dp += chan1;
			}

			if (j < hgt - dy_b - 1)
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
#define	KSIZE	7

/* *********************************************************** */

#undef  KSIZE1
#define	KSIZE1	6

#define	MAX_KER	7

/* *********************************************************** */

#define	MAX_N	15
#define	BUFF_SIZE	1500
mlib_status
CONV_FUNC(7x7)
{
	GET_SRC_DST_PARAMETERS(DTYPE);
	DTYPE *sl, *sp, *sl1, *dl, *dp;
	DTYPE *sl2, *sl3, *sl4, *sl5;
	mlib_s32 buff[KSIZE * BUFF_LINE];
	mlib_s32 *pbuff = buff, *buffT;
	mlib_s32 *buff0, *buff1, *buff2, *buff3, *buff4, *buff5, *buff6;
	mlib_s32 h0, h1, h2, h3, h4, h5, h6;
	mlib_s32 v0, v1, v2, v3, v4, v5, v6;
	mlib_s32 a0, a1, a2, a3, a4, a5, a6;
	mlib_s32 b0, b1, b2, b3, b4, b5, b6;
	mlib_s32 s0, s1, s2, s3, s4, s5, s6, s7;
	mlib_s32 pix0, pix1;
	mlib_s32 shift1, shift2;
	mlib_s32 i, j, c, swid;

	swid = wid + KSIZE1;

	if (swid > BUFF_LINE) {
		pbuff = __mlib_malloc(KSIZE * sizeof (FTYPE) * swid);

		if (pbuff == NULL)
			return (MLIB_FAILURE);
	}

	buff0 = pbuff;
	buff1 = buff0 + swid;
	buff2 = buff1 + swid;
	buff3 = buff2 + swid;
	buff4 = buff3 + swid;
	buff5 = buff4 + swid;
	buff6 = buff5 + swid;

	swid -= (dx_l + dx_r);

	shift1 = SHT;
	shift2 = 2 * scalef_expon - shift1 - 32;

	h0 = hkernel[0] >> shift1;
	h1 = hkernel[1] >> shift1;
	h2 = hkernel[2] >> shift1;
	h3 = hkernel[3] >> shift1;
	h4 = hkernel[4] >> shift1;
	h5 = hkernel[5] >> shift1;
	h6 = hkernel[6] >> shift1;

	v0 = vkernel[0] >> 16;
	v1 = vkernel[1] >> 16;
	v2 = vkernel[2] >> 16;
	v3 = vkernel[3] >> 16;
	v4 = vkernel[4] >> 16;
	v5 = vkernel[5] >> 16;
	v6 = vkernel[6] >> 16;

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


		for (i = 0; i < dx_l; i++) {
			buff0[i] = sl[0];
			buff1[i] = sl1[0];
			buff2[i] = sl2[0];
			buff3[i] = sl3[0];
			buff4[i] = sl4[0];
			buff5[i] = sl5[0];

		}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < swid; i++) {
			buff0[i + dx_l] = sl[i * chan1];
			buff1[i + dx_l] = sl1[i * chan1];
			buff2[i + dx_l] = sl2[i * chan1];
			buff3[i + dx_l] = sl3[i * chan1];
			buff4[i + dx_l] = sl4[i * chan1];
			buff5[i + dx_l] = sl5[i * chan1];

		}

		for (i = 0; i < dx_r; i++) {
			buff0[swid + dx_l + i] = buff0[swid + dx_l - 1];
			buff1[swid + dx_l + i] = buff1[swid + dx_l - 1];
			buff2[swid + dx_l + i] = buff2[swid + dx_l - 1];
			buff3[swid + dx_l + i] = buff3[swid + dx_l - 1];
			buff4[swid + dx_l + i] = buff4[swid + dx_l - 1];
			buff5[swid + dx_l + i] = buff5[swid + dx_l - 1];

		}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < wid; i++) {
			buff0[i] = (buff0[i] * h0 + buff0[i + 1] * h1 +
			    buff0[i + 2] * h2 + buff0[i + 3] * h3 +
			    buff0[i + 4] * h4 + buff0[i + 5] * h5 +
				buff0[i + 6] * h6) >> 16;
			buff1[i] = (buff1[i] * h0 + buff1[i + 1] * h1 +
			    buff1[i + 2] * h2 + buff1[i + 3] * h3 +
			    buff1[i + 4] * h4 + buff1[i + 5] * h5 +
				buff1[i + 6] * h6) >> 16;
			buff2[i] = (buff2[i] * h0 + buff2[i + 1] * h1 +
			    buff2[i + 2] * h2 + buff2[i + 3] * h3 +
			    buff2[i + 4] * h4 + buff2[i + 5] * h5 +
				buff2[i + 6] * h6) >> 16;
			buff3[i] = (buff3[i] * h0 + buff3[i + 1] * h1 +
			    buff3[i + 2] * h2 + buff3[i + 3] * h3 +
			    buff3[i + 4] * h4 + buff3[i + 5] * h5 +
				buff3[i + 6] * h6) >> 16;
			buff4[i] = (buff4[i] * h0 + buff4[i + 1] * h1 +
			    buff4[i + 2] * h2 + buff4[i + 3] * h3 +
			    buff4[i + 4] * h4 + buff4[i + 5] * h5 +
				buff4[i + 6] * h6) >> 16;
			buff5[i] = (buff5[i] * h0 + buff5[i + 1] * h1 +
			    buff5[i + 2] * h2 + buff5[i + 3] * h3 +
			    buff5[i + 4] * h4 + buff5[i + 5] * h5 +
				buff5[i + 6] * h6) >> 16;

		}

		if ((hgt - dy_b) > 0)
			sl = sl5 + sll;
		else
			sl = sl5;

		for (j = 0; j < hgt; j++) {
			sp = sl;
			dp = dl;

			s2 = sp[0];

			if ((1 > dx_l) && (1 < wid + KSIZE1 - dx_r))
				sp += chan1;
			s3 = sp[0];

			if ((2 > dx_l) && (2 < wid + KSIZE1 - dx_r))
				sp += chan1;
			s4 = sp[0];

			if ((3 > dx_l) && (3 < wid + KSIZE1 - dx_r))
				sp += chan1;
			s5 = sp[0];

			if ((4 > dx_l) && (4 < wid + KSIZE1 - dx_r))
				sp += chan1;
			s6 = sp[0];

			if ((5 > dx_l) && (5 < wid + KSIZE1 - dx_r))
				sp += chan1;
			s7 = sp[0];

			sp += chan1;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (wid - dx_r - 2); i += 2) {
				a0 = buff0[i];
				b0 = buff0[i + 1];
				a1 = buff1[i];
				b1 = buff1[i + 1];
				a2 = buff2[i];
				b2 = buff2[i + 1];
				a3 = buff3[i];
				b3 = buff3[i + 1];
				a4 = buff4[i];
				b4 = buff4[i + 1];
				a5 = buff5[i];
				b5 = buff5[i + 1];

				s0 = s2;
				s1 = s3;
				s2 = s4;
				s3 = s5;
				s4 = s6;
				s5 = s7;
				s6 = sp[0];
				s7 = sp[chan1];

				a6 = (s0 * h0 + s1 * h1 + s2 * h2 + s3 * h3 +
				    s4 * h4 + s5 * h5 + s6 * h6) >> 16;
				b6 = (s1 * h0 + s2 * h1 + s3 * h2 + s4 * h3 +
				    s5 * h4 + s6 * h5 + s7 * h6) >> 16;
				buff6[i] = a6;
				buff6[i + 1] = b6;

				pix0 =
				    (a0 * v0 + a1 * v1 + a2 * v2 + a3 * v3 +
				    a4 * v4 + a5 * v5 + a6 * v6) >> shift2;
				pix1 =
				    (b0 * v0 + b1 * v1 + b2 * v2 + b3 * v3 +
				    b4 * v4 + b5 * v5 + b6 * v6) >> shift2;
				CLAMP_STORE(dp[0], pix0);
				CLAMP_STORE(dp[chan1], pix1);

				sp += chan2;
				dp += chan2;
			}

			s1 = s2;
			s2 = s3;
			s3 = s4;
			s4 = s5;
			s5 = s6;
			s6 = s7;

			for (; i < wid - dx_r; i++) {
				a0 = buff0[i];
				a1 = buff1[i];
				a2 = buff2[i];
				a3 = buff3[i];
				a4 = buff4[i];
				a5 = buff5[i];

				s0 = s1;
				s1 = s2;
				s2 = s3;
				s3 = s4;
				s4 = s5;
				s5 = s6;
				s6 = sp[0];

				a6 = (s0 * h0 + s1 * h1 + s2 * h2 + s3 * h3 +
				    s4 * h4 + s5 * h5 + s6 * h6) >> 16;
				buff6[i] = a6;

				pix0 =
				    (a0 * v0 + a1 * v1 + a2 * v2 + a3 * v3 +
				    a4 * v4 + a5 * v5 + a6 * v6) >> shift2;
				CLAMP_STORE(dp[0], pix0);

				sp += chan1;
				dp += chan1;
			}

			sp -= chan1;

			for (; i < wid; i++) {
				a0 = buff0[i];
				a1 = buff1[i];
				a2 = buff2[i];
				a3 = buff3[i];
				a4 = buff4[i];
				a5 = buff5[i];

				s0 = s1;
				s1 = s2;
				s2 = s3;
				s3 = s4;
				s4 = s5;
				s5 = s6;
				s7 = sp[0];

				a6 = (s0 * h0 + s1 * h1 + s2 * h2 + s3 * h3 +
				    s4 * h4 + s5 * h5 + s6 * h6) >> 16;
				buff6[i] = a6;

				pix0 =
				    (a0 * v0 + a1 * v1 + a2 * v2 + a3 * v3 +
				    a4 * v4 + a5 * v5 + a6 * v6) >> shift2;
				CLAMP_STORE(dp[0], pix0);

				dp += chan1;
			}

			if (j < hgt - dy_b - 1)
				sl += sll;
			dl += dll;

			buffT = buff0;
			buff0 = buff1;
			buff1 = buff2;
			buff2 = buff3;
			buff3 = buff4;
			buff4 = buff5;
			buff5 = buff6;
			buff6 = buffT;
		}
	}

	if (pbuff != buff)
		__mlib_free(pbuff);

	return (MLIB_SUCCESS);
}
/* *********************************************************** */
