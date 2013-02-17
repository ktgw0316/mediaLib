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

#ifndef _MLIB_V_IMAGECONV_NW_H
#define	_MLIB_V_IMAGECONV_NW_H

#pragma ident	"@(#)mlib_v_ImageConv_nw.h	9.2	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <vis_proto.h>
#include <mlib_c_ImageConv.h>

/* *********************************************************** */

#if IMG_TYPE == 1

#define	DTYPE	mlib_u8
#define	CONV_FUNC(KERN)	mlib_c_conv##KERN##nw_u8
#define	DSCALE	1.0
#define	SCALE	23
#define	S64TOS32(x)	(x)
#define	SAT_OFF

#elif IMG_TYPE == 2	/* IMG_TYPE == 1 */

#define	DTYPE	mlib_s16
#define	CONV_FUNC(KERN)	mlib_conv##KERN##nw_s16
#define	DSCALE	1.0
#define	SCALE	16
#define	FROM_S16(x)	(x)
#define	S64TOS32(x)	((x) & 0xffffffff)
#define	SAT_OFF

#elif IMG_TYPE == 3	/* IMG_TYPE == 1 */

#define	DTYPE	mlib_u16
#define	CONV_FUNC(KERN)	mlib_conv##KERN##nw_u16
#define	DSCALE	1.0
#define	SCALE	16
#define	FROM_S16(x)	((x) ^ 0x8000)
#define	S64TOS32(x)	(x)
#define	SAT_OFF	-(1 << 15)

#endif		   /* IMG_TYPE == 1 */

/* *********************************************************** */

#define	BUFF_SIZE	1600
#define	CACHE_SIZE	(64*1024)

/* *********************************************************** */

#define	FTYPE	mlib_d64
#define	CLAMP_S32(x)	((mlib_s32)(x))

/* *********************************************************** */

#define	D2I(x)	CLAMP_S32((x) SAT_OFF)

/* *********************************************************** */

#define	STORE2(res0, res1)	dp[0 ] = res0; dp[chan1] = res1

/* *********************************************************** */

#define	LOAD_BUFF(buff)                                                \
	*(mlib_s64 *)(buff + i) =                                      \
	    (((mlib_s64)sp[0]) << 32) | S64TOS32((mlib_s64)sp[chan1])

/* *********************************************************** */

    typedef union
{
	mlib_d64 d64;
	struct
	{
		mlib_s32 i0;
		mlib_s32 i1;
	} i32s;
	struct
	{
		mlib_f32 f0;
		mlib_f32 f1;
	} f32s;
} d64_2x32;

/* *********************************************************** */

#define	BUFF_LINE	256

/* *********************************************************** */

#define	DEF_VARS(type)                                          \
	type *adr_src, *sl, *sp;                                \
	type *adr_dst, *dl, *dp;                                \
	FTYPE *pbuff = buff;                                    \
	mlib_s32 wid, hgt, sll, dll;                            \
	mlib_s32 nchannel, chan1;                               \
	mlib_s32 i, j, c

/* *********************************************************** */

#define	LOAD_KERNEL3()                                                 \
	FTYPE scalef = DSCALE;                                         \
	FTYPE k0, k1, k2, k3, k4, k5, k6, k7, k8;                      \
	FTYPE p00, p01, p02, p03, p10, p11, p12, p13, p20, p21, p22,   \
	    p23;                                                       \
	while (scalef_expon > 30) {                                    \
	    scalef /= (1 << 30);                                       \
	    scalef_expon -= 30;                                        \
	}                                                              \
	                                                               \
	scalef /= (1 << scalef_expon);                                 \
/*                                                                     \
 * keep kernel in regs                                                 \
 */                                                                    \
	k0 = scalef * kern[0];                                         \
	k1 = scalef * kern[1];                                         \
	k2 = scalef * kern[2];                                         \
	k3 = scalef * kern[3];                                         \
	k4 = scalef * kern[4];                                         \
	k5 = scalef * kern[5];                                         \
	k6 = scalef * kern[6];                                         \
	k7 = scalef * kern[7];                                         \
	k8 = scalef * kern[8]

/* *********************************************************** */

#define	LOAD_KERNEL(SIZE)                                       \
	FTYPE scalef = DSCALE;                                  \
	                                                        \
	while (scalef_expon > 30) {                             \
	    scalef /= (1 << 30);                                \
	    scalef_expon -= 30;                                 \
	}                                                       \
	                                                        \
	scalef /= (1 << scalef_expon);                          \
	for (j = 0; j < SIZE; j++)                              \
	    k[j] = scalef * kern[j]

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

#define	KSIZE	3

mlib_status CONV_FUNC(3x3) (
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kern,
    mlib_s32 scalef_expon,
    mlib_s32 cmask)
{
	mlib_d64 buff[(KSIZE + 1) * (BUFF_LINE >> 1)];
	mlib_s32 *buff0, *buff1, *buff2, *buff3, *buffT;

	DEF_VARS(DTYPE);
	DTYPE *sl1;
	mlib_s32 chan2;
	DTYPE *sl2;

	LOAD_KERNEL3();
	GET_SRC_DST_PARAMETERS(DTYPE);

	vis_write_gsr(SCALE << 3);

	if (wid > BUFF_LINE) {
		pbuff =
		    (mlib_d64 *)__mlib_malloc((KSIZE +
		    1) * sizeof (mlib_s32) * ((wid + 1) & ~1));

		if (pbuff == NULL)
			return (MLIB_FAILURE);
	}

	buff0 = (mlib_s32 *)pbuff;
	buff1 = buff0 + ((wid + 1) & ~1);
	buff2 = buff1 + ((wid + 1) & ~1);
	buff3 = buff2 + ((wid + 1) & ~1);

	chan1 = nchannel;
	chan2 = chan1 + chan1;

	wid -= (KSIZE - 1);
	hgt -= (KSIZE - 1);

	adr_dst += ((KSIZE - 1) / 2) * (dll + chan1);

	for (c = 0; c < nchannel; c++) {
		if (!(cmask & (1 << (nchannel - 1 - c))))
			continue;

		sl = adr_src + c;
		dl = adr_dst + c;

		sl1 = sl + sll;
		sl2 = sl1 + sll;

#pragma pipeloop(0)
		for (i = 0; i < wid + (KSIZE - 1); i++) {
			buff0[i] = (mlib_s32)sl[i * chan1];
			buff1[i] = (mlib_s32)sl1[i * chan1];
			buff2[i] = (mlib_s32)sl2[i * chan1];
		}

		sl += KSIZE * sll;

		for (j = 0; j < hgt; j++) {
			FTYPE s0, s1;

			p02 = buff0[0];
			p12 = buff1[0];
			p22 = buff2[0];

			p03 = buff0[1];
			p13 = buff1[1];
			p23 = buff2[1];

			s0 = p02 * k0 + p03 * k1 + p12 * k3 + p13 * k4 +
			    p22 * k6 + p23 * k7;
			s1 = p03 * k0 + p13 * k3 + p23 * k6;

			sp = sl;
			dp = dl;

#pragma pipeloop(0)
			for (i = 0; i <= (wid - 2); i += 2) {
				d64_2x32 dd0, dd1;

				p02 = buff0[i + 2];
				p12 = buff1[i + 2];
				p22 = buff2[i + 2];
				p03 = buff0[i + 3];
				p13 = buff1[i + 3];
				p23 = buff2[i + 3];

				LOAD_BUFF(buff3);

				dd0.i32s.i0 =
				    D2I(s0 + p02 * k2 + p12 * k5 + p22 * k8);
				dd0.i32s.i1 =
				    D2I(s1 + p02 * k1 + p03 * k2 + p12 * k4 +
				    p13 * k5 + p22 * k7 + p23 * k8);

#if IMG_TYPE != 1
				dd1.f32s.f0 = vis_fpackfix(dd0.d64);
#else /* IMG_TYPE != 1 */
				dd1.d64 = vis_fpack32(dd1.d64, dd0.d64);
#endif /* IMG_TYPE != 1 */

				s0 = p02 * k0 + p03 * k1 + p12 * k3 + p13 * k4 +
				    p22 * k6 + p23 * k7;
				s1 = p03 * k0 + p13 * k3 + p23 * k6;

#if IMG_TYPE != 1
				STORE2(FROM_S16(dd1.i32s.i0 >> 16),
				    FROM_S16(dd1.i32s.i0));
#else /* IMG_TYPE != 1 */
				STORE2(dd1.i32s.i0, dd1.i32s.i1);
#endif /* IMG_TYPE != 1 */

				sp += chan2;
				dp += chan2;
			}

			for (; i < wid; i++) {
				d64_2x32 dd0, dd1;

				p00 = buff0[i];
				p10 = buff1[i];
				p20 = buff2[i];
				p01 = buff0[i + 1];
				p11 = buff1[i + 1];
				p21 = buff2[i + 1];
				p02 = buff0[i + 2];
				p12 = buff1[i + 2];
				p22 = buff2[i + 2];

				buff3[i] = (mlib_s32)sp[0];

				dd0.i32s.i1 =
				    D2I(p00 * k0 + p01 * k1 + p02 * k2 +
				    p10 * k3 + p11 * k4 + p12 * k5 + p20 * k6 +
				    p21 * k7 + p22 * k8);

#if IMG_TYPE != 1
				dd1.f32s.f0 = vis_fpackfix(dd0.d64);
				dp[0] = FROM_S16(dd1.i32s.i0);
#else /* IMG_TYPE != 1 */
				dd1.d64 = vis_fpack32(dd1.d64, dd0.d64);
				dp[0] = dd1.i32s.i1;
#endif /* IMG_TYPE != 1 */

				sp += chan1;
				dp += chan1;
			}

			buff3[wid] = (mlib_s32)sp[0];
			buff3[wid + 1] = (mlib_s32)sp[chan1];

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

mlib_status CONV_FUNC(4x4) (
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kern,
    mlib_s32 scalef_expon,
    mlib_s32 cmask)
{
	FTYPE buff[(KSIZE + 3) * BUFF_LINE];
	FTYPE *buff0, *buff1, *buff2, *buff3, *buff4, *buffT;
	FTYPE *buffd;
	FTYPE k[KSIZE * KSIZE];
	FTYPE k0, k1, k2, k3, k4, k5, k6, k7;
	FTYPE p00, p01, p02, p03, p04;
	FTYPE p10, p11, p12, p13, p14, p20, p21, p22, p23, p30, p31, p32, p33;

	DEF_VARS(DTYPE);
	DTYPE *sl1;
	mlib_s32 chan2;
	mlib_s32 *buffi;
	DTYPE *sl2, *sl3;

	LOAD_KERNEL(KSIZE * KSIZE);
	GET_SRC_DST_PARAMETERS(DTYPE);

	vis_write_gsr(SCALE << 3);

	if (wid > BUFF_LINE) {
		pbuff = __mlib_malloc((KSIZE + 3) * sizeof (FTYPE) * wid);

		if (pbuff == NULL)
			return (MLIB_FAILURE);
	}

	buff0 = pbuff;
	buff1 = buff0 + wid;
	buff2 = buff1 + wid;
	buff3 = buff2 + wid;
	buff4 = buff3 + wid;
	buffd = buff4 + wid;
	buffi = (mlib_s32 *)(buffd + wid);

	chan1 = nchannel;
	chan2 = chan1 + chan1;

	wid -= (KSIZE - 1);
	hgt -= (KSIZE - 1);

	adr_dst += ((KSIZE - 1) / 2) * (dll + chan1);

	for (c = 0; c < nchannel; c++) {
		if (!(cmask & (1 << (nchannel - 1 - c))))
			continue;

		sl = adr_src + c;
		dl = adr_dst + c;

		sl1 = sl + sll;
		sl2 = sl1 + sll;
		sl3 = sl2 + sll;

#pragma pipeloop(0)
		for (i = 0; i < wid + (KSIZE - 1); i++) {
			buff0[i] = (FTYPE) sl[i * chan1];
			buff1[i] = (FTYPE) sl1[i * chan1];
			buff2[i] = (FTYPE) sl2[i * chan1];
			buff3[i] = (FTYPE) sl3[i * chan1];
		}

		sl += KSIZE * sll;

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

#pragma pipeloop(0)
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

				LOAD_BUFF(buffi);

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

#pragma pipeloop(0)
			for (i = 0; i <= (wid - 2); i += 2) {
				d64_2x32 dd0, dd1;
				mlib_s32 ss0, ss1;

				ss0 = *(buffi + i);
				ss1 = *(buffi + i + 1);
				buff4[i] = (FTYPE) ss0;
				buff4[i + 1] = (FTYPE) ss1;

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

				dd0.i32s.i0 =
				    D2I(p00 * k0 + p01 * k1 + p02 * k2 +
				    p03 * k3 + p10 * k4 + p11 * k5 + p12 * k6 +
				    p13 * k7 + buffd[i]);
				dd0.i32s.i1 =
				    D2I(p01 * k0 + p02 * k1 + p03 * k2 +
				    p04 * k3 + p11 * k4 + p12 * k5 + p13 * k6 +
				    p14 * k7 + buffd[i + 1]);

#if IMG_TYPE != 1
				dd1.f32s.f0 = vis_fpackfix(dd0.d64);
				STORE2(FROM_S16(dd1.i32s.i0 >> 16),
				    FROM_S16(dd1.i32s.i0));
#else /* IMG_TYPE != 1 */
				dd1.d64 = vis_fpack32(dd1.d64, dd0.d64);
				STORE2(dd1.i32s.i0, dd1.i32s.i1);
#endif /* IMG_TYPE != 1 */
				dp += chan2;
			}

/* last pixels */
			for (; i < wid; i++) {
				d64_2x32 dd0, dd1;

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

				buff4[i] = (FTYPE) sp[0];

				dd0.i32s.i1 =
				    D2I(p00 * k[0] + p01 * k[1] + p02 * k[2] +
				    p03 * k[3] + p10 * k[4] + p11 * k[5] +
				    p12 * k[6] + p13 * k[7] + p20 * k[8] +
				    p21 * k[9] + p22 * k[10] + p23 * k[11] +
				    p30 * k[12] + p31 * k[13] + p32 * k[14] +
				    p33 * k[15]);

#if IMG_TYPE != 1
				dd1.f32s.f0 = vis_fpackfix(dd0.d64);
				dp[0] = FROM_S16(dd1.i32s.i0);
#else /* IMG_TYPE != 1 */
				dd1.d64 = vis_fpack32(dd1.d64, dd0.d64);
				dp[0] = dd1.i32s.i1;
#endif /* IMG_TYPE != 1 */

				sp += chan1;
				dp += chan1;
			}

			buff4[wid] = (FTYPE) sp[0];
			buff4[wid + 1] = (FTYPE) sp[chan1];
			buff4[wid + 2] = (FTYPE) sp[chan2];

/* next line */
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

mlib_status CONV_FUNC(5x5) (
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kern,
    mlib_s32 scalef_expon,
    mlib_s32 cmask)
{
	FTYPE buff[(KSIZE + 3) * BUFF_LINE];
	FTYPE *buff0, *buff1, *buff2, *buff3, *buff4, *buff5, *buffd, *buffT;
	FTYPE k[KSIZE * KSIZE];
	FTYPE k0, k1, k2, k3, k4, k5, k6, k7, k8, k9;
	FTYPE p00, p01, p02, p03, p04, p05;
	FTYPE p10, p11, p12, p13, p14, p15;
	FTYPE p20, p21, p22, p23, p24;
	FTYPE p30, p31, p32, p33, p34, p40, p41, p42, p43, p44;

	DEF_VARS(DTYPE);
	DTYPE *sl1;
	mlib_s32 chan2;
	mlib_s32 *buffi;
	DTYPE *sl2, *sl3, *sl4;

	LOAD_KERNEL(KSIZE * KSIZE);
	GET_SRC_DST_PARAMETERS(DTYPE);

	vis_write_gsr(SCALE << 3);

	if (wid > BUFF_LINE) {
		pbuff = __mlib_malloc((KSIZE + 3) * sizeof (FTYPE) * wid);

		if (pbuff == NULL)
			return (MLIB_FAILURE);
	}

	buff0 = pbuff;
	buff1 = buff0 + wid;
	buff2 = buff1 + wid;
	buff3 = buff2 + wid;
	buff4 = buff3 + wid;
	buff5 = buff4 + wid;
	buffd = buff5 + wid;
	buffi = (mlib_s32 *)(buffd + wid);

	chan1 = nchannel;
	chan2 = chan1 + chan1;

	wid -= (KSIZE - 1);
	hgt -= (KSIZE - 1);

	adr_dst += ((KSIZE - 1) / 2) * (dll + chan1);

	for (c = 0; c < nchannel; c++) {
		if (!(cmask & (1 << (nchannel - 1 - c))))
			continue;

		sl = adr_src + c;
		dl = adr_dst + c;

		sl1 = sl + sll;
		sl2 = sl1 + sll;
		sl3 = sl2 + sll;
		sl4 = sl3 + sll;

#pragma pipeloop(0)
		for (i = 0; i < wid + (KSIZE - 1); i++) {
			buff0[i] = (FTYPE) sl[i * chan1];
			buff1[i] = (FTYPE) sl1[i * chan1];
			buff2[i] = (FTYPE) sl2[i * chan1];
			buff3[i] = (FTYPE) sl3[i * chan1];
			buff4[i] = (FTYPE) sl4[i * chan1];
		}

		sl += KSIZE * sll;

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

#pragma pipeloop(0)
			for (i = 0; i <= (wid - 2); i += 2) {
				p00 = p02;
				p10 = p12;
				p01 = p03;
				p11 = p13;
				p02 = p04;
				p12 = p14;

				LOAD_BUFF(buffi);

				p03 = buff0[i + 3];
				p13 = buff1[i + 3];
				p04 = buff0[i + 4];
				p14 = buff1[i + 4];
				p05 = buff0[i + 5];
				p15 = buff1[i + 5];

				buffd[i] =
				    (p00 * k0 + p01 * k1 + p02 * k2 + p03 * k3 +
				    p04 * k4 + p10 * k5 + p11 * k6 + p12 * k7 +
				    p13 * k8 + p14 * k9);
				buffd[i + 1] =
				    (p01 * k0 + p02 * k1 + p03 * k2 + p04 * k3 +
				    p05 * k4 + p11 * k5 + p12 * k6 + p13 * k7 +
				    p14 * k8 + p15 * k9);

				sp += chan2;
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

#pragma pipeloop(0)
			for (i = 0; i <= (wid - 2); i += 2) {
				d64_2x32 dd;

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

				dd.d64 = *(FTYPE *) (buffi + i);
				buff5[i] = (FTYPE) dd.i32s.i0;
				buff5[i + 1] = (FTYPE) dd.i32s.i1;

				buffd[i] +=
				    (p00 * k0 + p01 * k1 + p02 * k2 + p03 * k3 +
				    p04 * k4 + p10 * k5 + p11 * k6 + p12 * k7 +
				    p13 * k8 + p14 * k9);
				buffd[i + 1] +=
				    (p01 * k0 + p02 * k1 + p03 * k2 + p04 * k3 +
				    p05 * k4 + p11 * k5 + p12 * k6 + p13 * k7 +
				    p14 * k8 + p15 * k9);
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

#pragma pipeloop(0)
			for (i = 0; i <= (wid - 2); i += 2) {
				d64_2x32 dd0, dd1;

				p00 = p02;
				p01 = p03;
				p02 = p04;
				p03 = p05;

				p04 = buff4[i + 4];
				p05 = buff4[i + 5];

				dd0.i32s.i0 =
				    D2I(p00 * k0 + p01 * k1 + p02 * k2 +
				    p03 * k3 + p04 * k4 + buffd[i]);
				dd0.i32s.i1 =
				    D2I(p01 * k0 + p02 * k1 + p03 * k2 +
				    p04 * k3 + p05 * k4 + buffd[i + 1]);

#if IMG_TYPE != 1
				dd1.f32s.f0 = vis_fpackfix(dd0.d64);

				STORE2(FROM_S16(dd1.i32s.i0 >> 16),
				    FROM_S16(dd1.i32s.i0));
#else /* IMG_TYPE != 1 */
				dd1.d64 = vis_fpack32(dd1.d64, dd0.d64);

				STORE2(dd1.i32s.i0, dd1.i32s.i1);
#endif /* IMG_TYPE != 1 */
				dp += chan2;
			}

/* last pixels */
			for (; i < wid; i++) {
				d64_2x32 dd0, dd1;

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

				buff5[i] = (FTYPE) sp[0];

				dd0.i32s.i1 =
				    D2I(p00 * k[0] + p01 * k[1] + p02 * k[2] +
				    p03 * k[3] + p04 * k[4] + p10 * k[5] +
				    p11 * k[6] + p12 * k[7] + p13 * k[8] +
				    p14 * k[9] + p20 * k[10] + p21 * k[11] +
				    p22 * k[12] + p23 * k[13] + p24 * k[14] +
				    p30 * k[15] + p31 * k[16] + p32 * k[17] +
				    p33 * k[18] + p34 * k[19] + p40 * k[20] +
				    p41 * k[21] + p42 * k[22] + p43 * k[23] +
				    p44 * k[24]);

#if IMG_TYPE != 1
				dd1.f32s.f0 = vis_fpackfix(dd0.d64);

				dp[0] = FROM_S16(dd1.i32s.i0);
#else /* IMG_TYPE != 1 */
				dd1.d64 = vis_fpack32(dd1.d64, dd0.d64);

				dp[0] = dd1.i32s.i1;
#endif /* IMG_TYPE != 1 */

				sp += chan1;
				dp += chan1;
			}

			buff5[wid] = (FTYPE) sp[0];
			buff5[wid + 1] = (FTYPE) sp[chan1];
			buff5[wid + 2] = (FTYPE) sp[chan2];
			buff5[wid + 3] = (FTYPE) sp[chan2 + chan1];

/* next line */
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

#if IMG_TYPE == 1

#undef  KSIZE
#define	KSIZE	7

mlib_status CONV_FUNC(7x7) (
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kern,
    mlib_s32 scalef_expon,
    mlib_s32 cmask)
{
	FTYPE buff[(KSIZE + 3) * BUFF_LINE], *buffs[2 * (KSIZE + 1)], *buffd;
	FTYPE k[KSIZE * KSIZE];
	mlib_s32 l, m, buff_ind;
	FTYPE k0, k1, k2, k3, k4, k5, k6;
	FTYPE p0, p1, p2, p3, p4, p5, p6, p7;
	DTYPE *sl2, *sl3, *sl4, *sl5, *sl6;

	DEF_VARS(DTYPE);
	DTYPE *sl1;
	mlib_s32 chan2;
	mlib_s32 *buffo, *buffi;

	LOAD_KERNEL(KSIZE * KSIZE);
	GET_SRC_DST_PARAMETERS(DTYPE);

	vis_write_gsr(SCALE << 3);

	if (wid > BUFF_LINE) {
		pbuff = __mlib_malloc((KSIZE + 3) * sizeof (FTYPE) * wid);

		if (pbuff == NULL)
			return (MLIB_FAILURE);
	}

	for (l = 0; l < KSIZE + 1; l++)
		buffs[l] = pbuff + l * wid;
	for (l = 0; l < KSIZE + 1; l++)
		buffs[l + (KSIZE + 1)] = buffs[l];
	buffd = buffs[KSIZE] + wid;
	buffo = (mlib_s32 *)(buffd + wid);
	buffi = buffo + (wid & ~1);

	chan1 = nchannel;
	chan2 = chan1 + chan1;

	wid -= (KSIZE - 1);
	hgt -= (KSIZE - 1);

	adr_dst += ((KSIZE - 1) / 2) * (dll + chan1);

	for (c = 0; c < nchannel; c++) {
		if (!(cmask & (1 << (nchannel - 1 - c))))
			continue;

		sl = adr_src + c;
		dl = adr_dst + c;

		sl1 = sl + sll;
		sl2 = sl1 + sll;
		sl3 = sl2 + sll;
		sl4 = sl3 + sll;
		sl5 = sl4 + sll;
		sl6 = sl5 + sll;

#pragma pipeloop(0)
		for (i = 0; i < wid + (KSIZE - 1); i++) {
			buffs[0][i] = (FTYPE) sl[i * chan1];
			buffs[1][i] = (FTYPE) sl1[i * chan1];
			buffs[2][i] = (FTYPE) sl2[i * chan1];
			buffs[3][i] = (FTYPE) sl3[i * chan1];
			buffs[4][i] = (FTYPE) sl4[i * chan1];
			buffs[5][i] = (FTYPE) sl5[i * chan1];
			buffs[6][i] = (FTYPE) sl6[i * chan1];
		}

		buff_ind = 0;

#pragma pipeloop(0)
		for (i = 0; i < wid; i++)
			buffd[i] = 0.0;

		sl += KSIZE * sll;

		for (j = 0; j < hgt; j++) {
			FTYPE **buffc = buffs + buff_ind;
			FTYPE *buffn = buffc[KSIZE];
			FTYPE *pk = k;

			for (l = 0; l < KSIZE; l++) {
				FTYPE *buff = buffc[l];
				d64_2x32 dd;

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

#pragma pipeloop(0)
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

#pragma pipeloop(0)
					for (i = 0; i <= (wid - 2); i += 2) {
						d64_2x32 dd0, dd1;

						p0 = p2;
						p1 = p3;
						p2 = p4;
						p3 = p5;
						p4 = p6;
						p5 = p7;

						p6 = buff[i + 6];
						p7 = buff[i + 7];

						LOAD_BUFF(buffi);

						dd.d64 = *(FTYPE *) (buffi + i);
						buffn[i] = (FTYPE) dd.i32s.i0;
						buffn[i + 1] =
						    (FTYPE) dd.i32s.i1;

						dd0.i32s.i0 =
						    D2I(p0 * k0 + p1 * k1 +
						    p2 * k2 + p3 * k3 +
						    p4 * k4 + p5 * k5 +
						    p6 * k6 + buffd[i]);
						dd0.i32s.i1 =
						    D2I(p1 * k0 + p2 * k1 +
						    p3 * k2 + p4 * k3 +
						    p5 * k4 + p6 * k5 +
						    p7 * k6 + buffd[i + 1]);

						dd1.d64 =
						    vis_fpack32(dd1.d64,
						    dd0.d64);

						STORE2(dd1.i32s.i0,
						    dd1.i32s.i1);

						buffd[i] = 0.0;
						buffd[i + 1] = 0.0;

						sp += chan2;
						dp += chan2;
					}
				}
			}

/* last pixels */
			for (; i < wid; i++) {
				FTYPE *pk = k, s = 0;
				d64_2x32 dd0, dd1;

				for (l = 0; l < KSIZE; l++) {
					FTYPE *buff = buffc[l] + i;

					for (m = 0; m < KSIZE; m++)
						s += buff[m] * ((*pk++));
				}

				dd0.i32s.i1 = D2I(s);
				dd1.d64 = vis_fpack32(dd1.d64, dd0.d64);

				dp[0] = dd1.i32s.i1;

				buffn[i] = (FTYPE) sp[0];

				sp += chan1;
				dp += chan1;
			}

			for (l = 0; l < (KSIZE - 1); l++)
				buffn[wid + l] = sp[l * chan1];

/* next line */
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

static mlib_status
mlib_ImageConv1xN(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *k,
    mlib_s32 n,
    mlib_s32 dn,
    mlib_s32 cmask)
{
	FTYPE buff[BUFF_SIZE];
	mlib_s32 off, kh;
	const FTYPE *pk;
	FTYPE k0, k1, k2, k3;
	FTYPE p0, p1, p2, p3, p4;

	DEF_VARS(DTYPE);
	DTYPE *sl_c, *dl_c, *sl0;
	mlib_s32 l, hsize, max_hsize;

	GET_SRC_DST_PARAMETERS(DTYPE);

	hgt -= (n - 1);
	adr_dst += dn * dll;

	max_hsize = (CACHE_SIZE / sizeof (DTYPE)) / sll;

	if (!max_hsize)
		max_hsize = 1;

	if (max_hsize > BUFF_SIZE) {
		pbuff = __mlib_malloc(sizeof (FTYPE) * max_hsize);
	}

	chan1 = nchannel;

	sl_c = adr_src;
	dl_c = adr_dst;

	for (l = 0; l < hgt; l += hsize) {
		hsize = hgt - l;

		if (hsize > max_hsize)
			hsize = max_hsize;

		for (c = 0; c < nchannel; c++) {
			if (!(cmask & (1 << (chan1 - 1 - c))))
				continue;

			sl = sl_c + c;
			dl = dl_c + c;

#pragma pipeloop(0)
			for (j = 0; j < hsize; j++)
				pbuff[j] = 0.0;

			for (i = 0; i < wid; i++) {
				sl0 = sl;

				for (off = 0; off < (n - 4); off += 4) {
					pk = k + off;
					sp = sl0;

					k0 = pk[0];
					k1 = pk[1];
					k2 = pk[2];
					k3 = pk[3];
					p2 = sp[0];
					p3 = sp[sll];
					p4 = sp[2 * sll];
					sp += 3 * sll;

#pragma pipeloop(0)
					for (j = 0; j < hsize; j += 2) {
						p0 = p2;
						p1 = p3;
						p2 = p4;
						p3 = sp[0];
						p4 = sp[sll];

						pbuff[j] +=
						    p0 * k0 + p1 * k1 +
						    p2 * k2 + p3 * k3;
						pbuff[j + 1] +=
						    p1 * k0 + p2 * k1 +
						    p3 * k2 + p4 * k3;

						sp += 2 * sll;
					}

					sl0 += 4 * sll;
				}

				pk = k + off;
				sp = sl0;

				k0 = pk[0];
				k1 = pk[1];
				k2 = pk[2];
				k3 = pk[3];
				p2 = sp[0];
				p3 = sp[sll];
				p4 = sp[2 * sll];

				dp = dl;
				kh = n - off;

				if (kh == 4) {
					sp += 3 * sll;

#pragma pipeloop(0)
					for (j = 0; j <= (hsize - 2); j += 2) {
						d64_2x32 dd0, dd1;

						p0 = p2;
						p1 = p3;
						p2 = p4;
						p3 = sp[0];
						p4 = sp[sll];

						dd0.i32s.i0 =
						    D2I(p0 * k0 + p1 * k1 +
						    p2 * k2 + p3 * k3 +
						    pbuff[j]);
						dd0.i32s.i1 =
						    D2I(p1 * k0 + p2 * k1 +
						    p3 * k2 + p4 * k3 +
						    pbuff[j + 1]);

#if IMG_TYPE != 1
						dd1.f32s.f0 =
						    vis_fpackfix(dd0.d64);

						dp[0] =
						    FROM_S16((dd1.i32s.
						    i0 >> 16));
						dp[dll] = FROM_S16(dd1.i32s.i0);
#else /* IMG_TYPE != 1 */
						dd1.d64 =
						    vis_fpack32(dd1.d64,
						    dd0.d64);

						dp[0] = dd1.i32s.i0;
						dp[dll] = dd1.i32s.i1;
#endif /* IMG_TYPE != 1 */
						pbuff[j] = 0;
						pbuff[j + 1] = 0;

						sp += 2 * sll;
						dp += 2 * dll;
					}

					if (j < hsize) {
						d64_2x32 dd0, dd1;

						p0 = p2;
						p1 = p3;
						p2 = p4;
						p3 = sp[0];

						dd0.i32s.i1 =
						    D2I(p0 * k0 + p1 * k1 +
						    p2 * k2 + p3 * k3 +
						    pbuff[j]);

#if IMG_TYPE != 1
						dd1.f32s.f0 =
						    vis_fpackfix(dd0.d64);

						dp[0] = FROM_S16(dd1.i32s.i0);
#else /* IMG_TYPE != 1 */
						dd1.d64 =
						    vis_fpack32(dd1.d64,
						    dd0.d64);

						dp[0] = dd1.i32s.i1;
#endif /* IMG_TYPE != 1 */

						pbuff[j] = 0;
					}

				} else if (kh == 3) {
					sp += 2 * sll;

#pragma pipeloop(0)
					for (j = 0; j <= (hsize - 2); j += 2) {
						d64_2x32 dd0, dd1;

						p0 = p2;
						p1 = p3;
						p2 = sp[0];
						p3 = sp[sll];

						dd0.i32s.i0 =
						    D2I(p0 * k0 + p1 * k1 +
						    p2 * k2 + pbuff[j]);
						dd0.i32s.i1 =
						    D2I(p1 * k0 + p2 * k1 +
						    p3 * k2 + pbuff[j + 1]);

#if IMG_TYPE != 1
						dd1.f32s.f0 =
						    vis_fpackfix(dd0.d64);

						dp[0] =
						    FROM_S16((dd1.i32s.
						    i0 >> 16));
						dp[dll] = FROM_S16(dd1.i32s.i0);
#else /* IMG_TYPE != 1 */
						dd1.d64 =
						    vis_fpack32(dd1.d64,
						    dd0.d64);

						dp[0] = dd1.i32s.i0;
						dp[dll] = dd1.i32s.i1;
#endif /* IMG_TYPE != 1 */

						pbuff[j] = 0;
						pbuff[j + 1] = 0;

						sp += 2 * sll;
						dp += 2 * dll;
					}

					if (j < hsize) {
						d64_2x32 dd0, dd1;

						p0 = p2;
						p1 = p3;
						p2 = sp[0];

						dd0.i32s.i1 =
						    D2I(p0 * k0 + p1 * k1 +
						    p2 * k2 + pbuff[j]);

#if IMG_TYPE != 1
						dd1.f32s.f0 =
						    vis_fpackfix(dd0.d64);

						dp[0] = FROM_S16(dd1.i32s.i0);
#else /* IMG_TYPE != 1 */
						dd1.d64 =
						    vis_fpack32(dd1.d64,
						    dd0.d64);

						dp[0] = dd1.i32s.i1;
#endif /* IMG_TYPE != 1 */

						pbuff[j] = 0;
					}

				} else if (kh == 2) {
					sp += sll;

#pragma pipeloop(0)
					for (j = 0; j <= (hsize - 2); j += 2) {
						d64_2x32 dd0, dd1;

						p0 = p2;
						p1 = sp[0];
						p2 = sp[sll];

						dd0.i32s.i0 =
						    D2I(p0 * k0 + p1 * k1 +
						    pbuff[j]);
						dd0.i32s.i1 =
						    D2I(p1 * k0 + p2 * k1 +
						    pbuff[j + 1]);

#if IMG_TYPE != 1
						dd1.f32s.f0 =
						    vis_fpackfix(dd0.d64);

						dp[0] =
						    FROM_S16((dd1.i32s.
						    i0 >> 16));
						dp[dll] = FROM_S16(dd1.i32s.i0);
#else /* IMG_TYPE != 1 */
						dd1.d64 =
						    vis_fpack32(dd1.d64,
						    dd0.d64);

						dp[0] = dd1.i32s.i0;
						dp[dll] = dd1.i32s.i1;
#endif /* IMG_TYPE != 1 */

						pbuff[j] = 0;
						pbuff[j + 1] = 0;

						sp += 2 * sll;
						dp += 2 * dll;
					}

					if (j < hsize) {
						d64_2x32 dd0, dd1;

						p0 = p2;
						p1 = sp[0];

						dd0.i32s.i1 =
						    D2I(p0 * k0 + p1 * k1 +
						    pbuff[j]);

#if IMG_TYPE != 1
						dd1.f32s.f0 =
						    vis_fpackfix(dd0.d64);

						dp[0] = FROM_S16(dd1.i32s.i0);
#else /* IMG_TYPE != 1 */
						dd1.d64 =
						    vis_fpack32(dd1.d64,
						    dd0.d64);

						dp[0] = dd1.i32s.i1;
#endif /* IMG_TYPE != 1 */
						pbuff[j] = 0;
					}

				} else {
/* if (kh == 1) */

#pragma pipeloop(0)
					for (j = 0; j < hsize; j++) {
						d64_2x32 dd0, dd1;

						p0 = sp[0];

						dd0.i32s.i1 =
						    D2I(p0 * k0 + pbuff[j]);

#if IMG_TYPE != 1
						dd1.f32s.f0 =
						    vis_fpackfix(dd0.d64);

						dp[0] = FROM_S16(dd1.i32s.i0);
#else /* IMG_TYPE != 1 */
						dd1.d64 =
						    vis_fpack32(dd1.d64,
						    dd0.d64);

						dp[0] = dd1.i32s.i1;
#endif /* IMG_TYPE != 1 */

						pbuff[j] = 0;

						sp += sll;
						dp += dll;
					}
				}

				sl += chan1;
				dl += chan1;
			}
		}

		sl_c += max_hsize * sll;
		dl_c += max_hsize * dll;
	}

	if (pbuff != buff)
		__mlib_free(pbuff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MAX_KER	7
#define	MAX_N	15

mlib_status CONV_FUNC(MxN) (
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kernel,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 dm,
    mlib_s32 dn,
    mlib_s32 scale,
    mlib_s32 cmask)
{
	FTYPE buff[BUFF_SIZE], *buffs_arr[2 * (MAX_N + 1)];
	FTYPE **buffs = buffs_arr, *buffd;
	FTYPE akernel[256], *k = akernel, fscale = DSCALE;
	mlib_s32 mn, l, off, kw, bsize, buff_ind;
	FTYPE k0, k1, k2, k3, k4, k5, k6;
	FTYPE p0, p1, p2, p3, p4, p5, p6, p7;

	DEF_VARS(DTYPE);
	mlib_s32 chan2;
	mlib_s32 *buffo, *buffi;

	GET_SRC_DST_PARAMETERS(DTYPE);

	vis_write_gsr(SCALE << 3);

	if (scale > 30) {
		fscale *= 1.0 / (1 << 30);
		scale -= 30;
	}

	fscale /= (1 << scale);

	mn = m * n;

	if (mn > 256) {
		k = __mlib_malloc(mn * sizeof (mlib_d64));

		if (k == NULL)
			return (MLIB_FAILURE);
	}

	for (i = 0; i < mn; i++) {
		k[i] = kernel[i] * fscale;
	}

	if (m == 1)
		return (mlib_ImageConv1xN(dst, src, k, n, dn, cmask));

	bsize = (n + 3) * wid;

	if ((bsize > BUFF_SIZE) || (n > MAX_N)) {
		pbuff =
		    __mlib_malloc(sizeof (FTYPE) * bsize +
		    sizeof (FTYPE *) * 2 * (n + 1));

		if (pbuff == NULL)
			return (MLIB_FAILURE);
		buffs = (FTYPE **) (pbuff + bsize);
	}

	for (l = 0; l < (n + 1); l++)
		buffs[l] = pbuff + l * wid;
	for (l = 0; l < (n + 1); l++)
		buffs[l + (n + 1)] = buffs[l];
	buffd = buffs[n] + wid;
	buffo = (mlib_s32 *)(buffd + wid);
	buffi = buffo + (wid & ~1);

	chan1 = nchannel;
	chan2 = chan1 + chan1;

	wid -= (m - 1);
	hgt -= (n - 1);
	adr_dst += dn * dll + dm * nchannel;

	for (c = 0; c < nchannel; c++) {
		if (!(cmask & (1 << (chan1 - 1 - c))))
			continue;

		sl = adr_src + c;
		dl = adr_dst + c;

		for (l = 0; l < n; l++) {
			FTYPE *buff = buffs[l];

#pragma pipeloop(0)
			for (i = 0; i < wid + (m - 1); i++) {
				buff[i] = (FTYPE) sl[i * chan1];
			}

			sl += sll;
		}

		buff_ind = 0;

#pragma pipeloop(0)
		for (i = 0; i < wid; i++)
			buffd[i] = 0.0;

		for (j = 0; j < hgt; j++) {
			FTYPE **buffc = buffs + buff_ind;
			FTYPE *buffn = buffc[n];
			FTYPE *pk = k;

			for (l = 0; l < n; l++) {
				FTYPE *buff_l = buffc[l];

				for (off = 0; off < m; ) {
					FTYPE *buff = buff_l + off;

					kw = m - off;

					if (kw > 2 * MAX_KER)
						kw = MAX_KER;
					else if (kw > MAX_KER)
						kw = kw / 2;
					off += kw;

					sp = sl;
					dp = dl;

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
					pk += kw;

					if (kw == 7) {

						if (l < (n - 1) || off < m) {

#pragma pipeloop(0)
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

#pragma pipeloop(0)
							for (i = 0;
							    i <= (wid - 2);
							    i += 2) {
								d64_2x32 dd0,
								    dd1;
								mlib_s32 ss0,
								    ss1;

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

								LOAD_BUFF
								    (buffi);

								ss0 =
								    *(buffi +
								    i);
								ss1 =
								    *(buffi +
								    i + 1);
								buffn[i] =
								    (FTYPE) ss0;
								buffn[i + 1] =
								    (FTYPE) ss1;

								dd0.i32s.i0 =
								    D2I(p0 *
								    k0 +
								    p1 * k1 +
								    p2 * k2 +
								    p3 * k3 +
								    p4 * k4 +
								    p5 * k5 +
								    p6 * k6 +
								    buffd[i]);
								dd0.i32s.i1 =
								    D2I(p1 *
								    k0 +
								    p2 * k1 +
								    p3 * k2 +
								    p4 * k3 +
								    p5 * k4 +
								    p6 * k5 +
								    p7 * k6 +
								    buffd[i +
								    1]);

#if IMG_TYPE != 1
								dd1.f32s.f0 =
								    vis_fpackfix
								    (dd0.d64);

								dp[0] =
								    FROM_S16(
								    (dd1.i32s.
								    i0 >> 16));
								dp[chan1] =
								    FROM_S16
								    (dd1.i32s.
								    i0);
#else /* IMG_TYPE != 1 */
								dd1.d64 =
								    vis_fpack32
								    (dd1.d64,
								    dd0.d64);

								dp[0] =
								    dd1.i32s.i0;
								dp[chan1] =
								    dd1.i32s.i1;
#endif /* IMG_TYPE != 1 */

								buffd[i] = 0.0;
								buffd[i + 1] =
								    0.0;

								sp += chan2;
								dp += chan2;
							}
						}

					} else if (kw == 6) {

						if (l < (n - 1) || off < m) {

#pragma pipeloop(0)
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

#pragma pipeloop(0)
							for (i = 0;
							    i <= (wid - 2);
							    i += 2) {
								d64_2x32 dd0,
								    dd1;

								p0 = p2;
								p1 = p3;
								p2 = p4;
								p3 = p5;
								p4 = p6;

								p5 = buff[i +
								    5];
								p6 = buff[i +
								    6];

								buffn[i] =
								    (FTYPE)
								    sp[0];
								buffn[i + 1] =
								    (FTYPE)
								    sp[chan1];

								dd0.i32s.i0 =
								    D2I(p0 *
								    k0 +
								    p1 * k1 +
								    p2 * k2 +
								    p3 * k3 +
								    p4 * k4 +
								    p5 * k5 +
								    buffd[i]);
								dd0.i32s.i1 =
								    D2I(p1 *
								    k0 +
								    p2 * k1 +
								    p3 * k2 +
								    p4 * k3 +
								    p5 * k4 +
								    p6 * k5 +
								    buffd[i +
								    1]);

#if IMG_TYPE != 1
								dd1.f32s.f0 =
								    vis_fpackfix
								    (dd0.d64);

								dp[0] =
								    FROM_S16(
								    (dd1.i32s.
								    i0 >> 16));
								dp[chan1] =
								    FROM_S16
								    (dd1.i32s.
								    i0);
#else /* IMG_TYPE != 1 */
								dd1.d64 =
								    vis_fpack32
								    (dd1.d64,
								    dd0.d64);

								dp[0] =
								    dd1.i32s.i0;
								dp[chan1] =
								    dd1.i32s.i1;
#endif /* IMG_TYPE != 1 */

								buffd[i] = 0.0;
								buffd[i + 1] =
								    0.0;

								sp += chan2;
								dp += chan2;
							}
						}

					} else if (kw == 5) {

						if (l < (n - 1) || off < m) {

#pragma pipeloop(0)
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

#pragma pipeloop(0)
							for (i = 0;
							    i <= (wid - 2);
							    i += 2) {
								d64_2x32 dd0,
								    dd1;

								p0 = p2;
								p1 = p3;
								p2 = p4;
								p3 = p5;

								p4 = buff[i +
								    4];
								p5 = buff[i +
								    5];

								buffn[i] =
								    (FTYPE)
								    sp[0];
								buffn[i + 1] =
								    (FTYPE)
								    sp[chan1];

								dd0.i32s.i0 =
								    D2I(p0 *
								    k0 +
								    p1 * k1 +
								    p2 * k2 +
								    p3 * k3 +
								    p4 * k4 +
								    buffd[i]);
								dd0.i32s.i1 =
								    D2I(p1 *
								    k0 +
								    p2 * k1 +
								    p3 * k2 +
								    p4 * k3 +
								    p5 * k4 +
								    buffd[i +
								    1]);

#if IMG_TYPE != 1
								dd1.f32s.f0 =
								    vis_fpackfix
								    (dd0.d64);

								dp[0] =
								    FROM_S16(
								    (dd1.i32s.
								    i0 >> 16));
								dp[chan1] =
								    FROM_S16
								    (dd1.i32s.
								    i0);
#else /* IMG_TYPE != 1 */
								dd1.d64 =
								    vis_fpack32
								    (dd1.d64,
								    dd0.d64);

								dp[0] =
								    dd1.i32s.i0;
								dp[chan1] =
								    dd1.i32s.i1;
#endif /* IMG_TYPE != 1 */

								buffd[i] = 0.0;
								buffd[i + 1] =
								    0.0;

								sp += chan2;
								dp += chan2;
							}
						}

					} else if (kw == 4) {

						if (l < (n - 1) || off < m) {

#pragma pipeloop(0)
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

#pragma pipeloop(0)
							for (i = 0;
							    i <= (wid - 2);
							    i += 2) {
								d64_2x32 dd0,
								    dd1;

								p0 = p2;
								p1 = p3;
								p2 = p4;

								p3 = buff[i +
								    3];
								p4 = buff[i +
								    4];

								buffn[i] =
								    (FTYPE)
								    sp[0];
								buffn[i + 1] =
								    (FTYPE)
								    sp[chan1];

								dd0.i32s.i0 =
								    D2I(p0 *
								    k0 +
								    p1 * k1 +
								    p2 * k2 +
								    p3 * k3 +
								    buffd[i]);
								dd0.i32s.i1 =
								    D2I(p1 *
								    k0 +
								    p2 * k1 +
								    p3 * k2 +
								    p4 * k3 +
								    buffd[i +
								    1]);

#if IMG_TYPE != 1
								dd1.f32s.f0 =
								    vis_fpackfix
								    (dd0.d64);

								dp[0] =
								    FROM_S16(
								    (dd1.i32s.
								    i0 >> 16));
								dp[chan1] =
								    FROM_S16
								    (dd1.i32s.
								    i0);
#else /* IMG_TYPE != 1 */
								dd1.d64 =
								    vis_fpack32
								    (dd1.d64,
								    dd0.d64);

								dp[0] =
								    dd1.i32s.i0;
								dp[chan1] =
								    dd1.i32s.i1;
#endif /* IMG_TYPE != 1 */

								buffd[i] = 0.0;
								buffd[i + 1] =
								    0.0;

								sp += chan2;
								dp += chan2;
							}
						}

					} else if (kw == 3) {

						if (l < (n - 1) || off < m) {

#pragma pipeloop(0)
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

#pragma pipeloop(0)
							for (i = 0;
							    i <= (wid - 2);
							    i += 2) {
								d64_2x32 dd0,
								    dd1;

								p0 = p2;
								p1 = p3;

								p2 = buff[i +
								    2];
								p3 = buff[i +
								    3];

								buffn[i] =
								    (FTYPE)
								    sp[0];
								buffn[i + 1] =
								    (FTYPE)
								    sp[chan1];

								dd0.i32s.i0 =
								    D2I(p0 *
								    k0 +
								    p1 * k1 +
								    p2 * k2 +
								    buffd[i]);
								dd0.i32s.i1 =
								    D2I(p1 *
								    k0 +
								    p2 * k1 +
								    p3 * k2 +
								    buffd[i +
								    1]);

#if IMG_TYPE != 1
								dd1.f32s.f0 =
								    vis_fpackfix
								    (dd0.d64);

								dp[0] =
								    FROM_S16(
								    (dd1.i32s.
								    i0 >> 16));
								dp[chan1] =
								    FROM_S16
								    (dd1.i32s.
								    i0);
#else /* IMG_TYPE != 1 */
								dd1.d64 =
								    vis_fpack32
								    (dd1.d64,
								    dd0.d64);

								dp[0] =
								    dd1.i32s.i0;
								dp[chan1] =
								    dd1.i32s.i1;
#endif /* IMG_TYPE != 1 */

								buffd[i] = 0.0;
								buffd[i + 1] =
								    0.0;

								sp += chan2;
								dp += chan2;
							}
						}

					} else {
/* if (kw == 2) */

						if (l < (n - 1) || off < m) {

#pragma pipeloop(0)
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

#pragma pipeloop(0)
							for (i = 0;
							    i <= (wid - 2);
							    i += 2) {
								d64_2x32 dd0,
								    dd1;

								p0 = p2;

								p1 = buff[i +
								    1];
								p2 = buff[i +
								    2];

								buffn[i] =
								    (FTYPE)
								    sp[0];
								buffn[i + 1] =
								    (FTYPE)
								    sp[chan1];

								dd0.i32s.i0 =
								    D2I(p0 *
								    k0 +
								    p1 * k1 +
								    buffd[i]);
								dd0.i32s.i1 =
								    D2I(p1 *
								    k0 +
								    p2 * k1 +
								    buffd[i +
								    1]);

#if IMG_TYPE != 1
								dd1.f32s.f0 =
								    vis_fpackfix
								    (dd0.d64);

								dp[0] =
								    FROM_S16(
								    (dd1.i32s.
								    i0 >> 16));
								dp[chan1] =
								    FROM_S16
								    (dd1.i32s.
								    i0);
#else /* IMG_TYPE != 1 */
								dd1.d64 =
								    vis_fpack32
								    (dd1.d64,
								    dd0.d64);

								dp[0] =
								    dd1.i32s.i0;
								dp[chan1] =
								    dd1.i32s.i1;
#endif /* IMG_TYPE != 1 */

								buffd[i] = 0.0;
								buffd[i + 1] =
								    0.0;

								sp += chan2;
								dp += chan2;
							}
						}
					}
				}
			}

/* last pixels */
			for (; i < wid; i++) {
				FTYPE *pk = k, s = 0;
				mlib_s32 x;
				d64_2x32 dd0, dd1;

				for (l = 0; l < n; l++) {
					FTYPE *buff = buffc[l] + i;

					for (x = 0; x < m; x++)
						s += buff[x] * ((*pk++));
				}

				dd0.i32s.i1 = D2I(s);

#if IMG_TYPE != 1
				dd1.f32s.f0 = vis_fpackfix(dd0.d64);
				dp[0] = FROM_S16(dd1.i32s.i0);
#else /* IMG_TYPE != 1 */
				dd1.d64 = vis_fpack32(dd1.d64, dd0.d64);
				dp[0] = dd1.i32s.i1;
#endif /* IMG_TYPE != 1 */

				buffn[i] = (FTYPE) sp[0];

				sp += chan1;
				dp += chan1;
			}

			for (l = 0; l < (m - 1); l++)
				buffn[wid + l] = sp[l * chan1];

/* next line */
			sl += sll;
			dl += dll;

			buff_ind++;

			if (buff_ind >= n + 1)
				buff_ind = 0;
		}
	}

	if (pbuff != buff)
		__mlib_free(pbuff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_V_IMAGECONV_NW_H */
