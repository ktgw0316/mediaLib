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

#pragma ident	"@(#)mlib_ImageConvIndex_8_16ext.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      Internal functions for mlib_ImageConv*Index on S16/U8 type
 *      and MLIB_EDGE_SRC_EXTEND mask
 *
 */

#include <mlib_image.h>
#include <mlib_ImageColormap.h>
#include <mlib_ImageConvIndex.h>

/* *********************************************************** */

/*
 *  This defines switches between functions of S16 and U8 types:
 *  Files: mlib_ImageConvIndex_16_16ext.c,   mlib_ImageConvIndex_8_16ext.c
 *         mlib_c_ImageConvIndex_16_8ext.c,  mlib_c_ImageConvIndex_8_8ext.c
 */
#define	TYPE_SHORT

#define	DTYPE	mlib_u8

/* *********************************************************** */

#define	CONV_FUNC(KERN)                                          \
	mlib_conv##KERN##Index_16_8ext(const mlib_image *src,     \
	    mlib_image *dst, const mlib_s32 *kern,               \
	    mlib_s32 scalef_expon, const void *colormap)

/* *********************************************************** */

#define	GET_ColorTrueFunc()                                     \
	(nchan == 3) ?                                          \
	    mlib_ImageColorTrue2IndexLine_S16_U8_3 :             \
	    mlib_ImageColorTrue2IndexLine_S16_U8_4

/* *********************************************************** */

#define	FTYPE	mlib_f32

/* *********************************************************** */

#ifdef TYPE_SHORT

/* *********************************************************** */

#define	LTYPE	mlib_s16
#define	DSCALE	65536.0

#define	FROM_S32(x)	((x) >> 16)

#define	S64TOS32(x)	((x) & 0xffffffff)

#define	SAT_OFF

#else /* TYPE_SHORT */

/* *********************************************************** */

#define	LTYPE	mlib_u8
#define	DSCALE	(1 << 24)

#define	FROM_S32(x)	(((x) >> 24) ^ 128)

#define	S64TOS32(x)	(x)

#define	SAT_OFF	- (1u << 31)

#endif /* TYPE_SHORT */

/* *********************************************************** */

#ifndef MLIB_USE_FTOI_CLAMPING

/* *********************************************************** */

#define	CLAMP_S32(dst, src)                                     \
	{                                                       \
	    mlib_d64 dsrc = (mlib_d64)(src);                    \
	                                                        \
	    if (dsrc > (mlib_d64)MLIB_S32_MAX)                  \
		dsrc = (mlib_d64)MLIB_S32_MAX;                  \
	    if (dsrc < (mlib_d64)MLIB_S32_MIN)                  \
		dsrc = (mlib_d64)MLIB_S32_MIN;                  \
	    dst = (mlib_s32)dsrc;                               \
	}

#else /* MLIB_USE_FTOI_CLAMPING */

#define	CLAMP_S32(dst, src)	dst = ((mlib_s32)(src))

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#define	D2I(dst, src)	CLAMP_S32(dst, (src) SAT_OFF)

/* *********************************************************** */

#if defined(_LITTLE_ENDIAN) && !defined(_NO_LONGLONG)

/* *********************************************************** */

#define	STORE2(res0, res1)	dp[0 ] = res1; dp[chan1] = res0

#else /* defined(_LITTLE_ENDIAN) && !defined(_NO_LONGLONG) */

/* *********************************************************** */

#define	STORE2(res0, res1)	dp[0 ] = res0; dp[chan1] = res1

#endif /* defined(_LITTLE_ENDIAN) && !defined(_NO_LONGLONG) */

/* *********************************************************** */

#define	LTBL(x)	ltbl[x]

/* *********************************************************** */

#ifdef _NO_LONGLONG

/* *********************************************************** */

#define	LOAD_BUFF(buff)                                         \
	buff[i] = LTBL(sp[0]);                                  \
	buff[i + 1] = LTBL(sp[1])

#else /* _NO_LONGLONG */

#ifdef _LITTLE_ENDIAN

/* *********************************************************** */

#define	LOAD_BUFF(buff)                                             \
	* (mlib_s64 *)(buff + i) =                                  \
	    (((mlib_s64)LTBL(sp[1])) << 32) | S64TOS32((mlib_s64)   \
	    LTBL(sp[0]))

#else /* _LITTLE_ENDIAN */

/* *********************************************************** */

#define	LOAD_BUFF(buff)                                             \
	* (mlib_s64 *)(buff + i) =                                  \
	    (((mlib_s64)LTBL(sp[0])) << 32) | S64TOS32((mlib_s64)   \
	    LTBL(sp[1]))

#endif /* _LITTLE_ENDIAN */
#endif /* _NO_LONGLONG */

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

#define	DEF_VARS(type)                                                   \
	mlib_s32 wid1, xsize, bsize, buff_ind = 0;                       \
	DTYPE *adr_src, *adr_dst, *sl = NULL, *sp = NULL;                \
	FTYPE *pbuff;                                                    \
	mlib_s32 *buffi;                                                 \
	mlib_s32 wid, hgt, sll, dll;                                     \
	mlib_s32 nchan, chan1, chan2;                                    \
	mlib_s32 i, j, c;                                                \
	mlib_s32 offset = mlib_ImageGetLutOffset(colormap);              \
	LTYPE *dl, *dp = NULL;                                           \
	LTYPE **lut_table =                                              \
	    (LTYPE **) mlib_ImageGetLutData(colormap), *ltbl;            \
	void (*ColorTrue2IndexLine) (const LTYPE *, DTYPE *, mlib_s32,   \
	    const void *)

/* *********************************************************** */

#define	LOAD_KERNEL2()                                          \
	FTYPE scalef = DSCALE;                                  \
	FTYPE k0, k1, k2, k3;                                   \
	FTYPE p00, p01, p02, p10, p11, p12;                     \
	                                                        \
	while (scalef_expon > 30) {                             \
	    scalef /= (1 << 30);                                \
	    scalef_expon -= 30;                                 \
	}                                                       \
	                                                        \
	scalef /= (1 << scalef_expon);                          \
/*                                                              \
 * keep kernel in regs                                          \
 */                                                             \
	k0 = scalef * kern[0];                                  \
	k1 = scalef * kern[1];                                  \
	k2 = scalef * kern[2];                                  \
	k3 = scalef * kern[3]

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
	{                                                       \
	    mlib_d64 scalef = DSCALE;                           \
	                                                        \
	    while (scalef_expon > 30) {                         \
		scalef /= (1 << 30);                            \
		scalef_expon -= 30;                             \
	    }                                                   \
	    scalef /= (1 << scalef_expon);                      \
	    for (j = 0; j < SIZE; j++)                          \
		k[j] = (FTYPE) (scalef * kern[j]);              \
	}

/* *********************************************************** */

#define	GET_SRC_DST_PARAMETERS(type)                            \
	hgt = mlib_ImageGetHeight(src);                         \
	wid = mlib_ImageGetWidth(src);                          \
	nchan = mlib_ImageGetChannels(src);                     \
	sll = mlib_ImageGetStride(src) / sizeof (type);         \
	dll = mlib_ImageGetStride(dst) / sizeof (type);         \
	adr_src = (type *) mlib_ImageGetData(src);              \
	adr_dst = (type *) mlib_ImageGetData(dst)

/* *********************************************************** */

#define	KSIZE1	(KSIZE - 1)
#define	LEFT	((KSIZE - 1)/2)

#define	RIGHT	((KSIZE - 1) - LEFT)

#define	TOP	LEFT

#define	BOTTOM	RIGHT

/* *********************************************************** */

#define	KSIZE	2

mlib_status
CONV_FUNC(2x2)
{
	FTYPE *buff0, *buff1, *buff2;
	FTYPE *buffs[8 * (KSIZE + 1)];
	LTYPE *bufft;
	mlib_s32 *buffo;
	DTYPE *da;

	DEF_VARS(DTYPE);
	DTYPE *sl1;

	LOAD_KERNEL2();
	GET_SRC_DST_PARAMETERS(DTYPE);

	nchan = mlib_ImageGetLutChannels(colormap);
	ColorTrue2IndexLine = GET_ColorTrueFunc();
	chan1 = nchan;
	chan2 = chan1 + chan1;
	da = adr_dst;

	wid1 = (wid + KSIZE + 2) & ~1;
	xsize = wid1 * 4;
	bsize =
	    (KSIZE + 1) * xsize * sizeof (FTYPE) +
	    2 * wid1 * sizeof (mlib_s32) + xsize * sizeof (LTYPE);
	pbuff = __mlib_malloc(bsize);

	if (pbuff == NULL)
		return (MLIB_FAILURE);

	for (i = 0; i < (KSIZE + 1); i++) {
		for (c = 0; c < 4; c++)
			buffs[4 * i + c] =
			    pbuff + i * wid1 + c * ((KSIZE + 1) * wid1) + 2;
	}

	for (i = 0; i < 4 * (KSIZE + 1); i++)
		buffs[4 * (KSIZE + 1) + i] = buffs[i];
	buffo = (mlib_s32 *)(buffs[4 * (KSIZE + 1) - 1] + wid1);

	buffi = buffo + wid1;
	bufft = (LTYPE *) (buffi + wid1);

	sl = adr_src;

	if (hgt > 1)
		sl1 = sl + sll;
	else
		sl1 = sl;

	for (c = 0; c < nchan; c++) {
		ltbl = lut_table[c] - offset;

		buff0 = buffs[c];
		buff1 = buffs[c + 4];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < wid; i++) {
			buff0[i - 1] = (FTYPE) LTBL(sl[i]);
			buff1[i - 1] = (FTYPE) LTBL(sl1[i]);
		}

		buff0[wid - 1] = buff0[wid - 2];
		buff1[wid - 1] = buff1[wid - 2];
	}

	if (hgt > 2)
		sl = sl1 + sll;
	else
		sl = sl1;
	dl = bufft;

	for (j = 0; j < hgt; j++) {
		FTYPE **buffc = buffs + buff_ind;

		for (c = 0; c < nchan; c++) {
			ltbl = lut_table[c] - offset;

			buff0 = buffc[c];
			buff1 = buffc[c + 4];
			buff2 = buffc[c + 8];

			p02 = buff0[-1];
			p12 = buff1[-1];

			sp = sl;
			dp = dl + c;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (wid - 2); i += 2) {
#ifdef _NO_LONGLONG
				mlib_s32 o64_1, o64_2;
#else /* _NO_LONGLONG */
				mlib_s64 o64;
#endif /* _NO_LONGLONG */
				d64_2x32 sd0, sd1, sd, sd2, dd;

				sd0.d64 = *(mlib_d64 *)(buff0 + i);
				sd1.d64 = *(mlib_d64 *)(buff1 + i);
				p00 = p02;
				p10 = p12;
				p01 = sd0.f32s.f0;
				p11 = sd1.f32s.f0;
				p02 = sd0.f32s.f1;
				p12 = sd1.f32s.f1;

				buffi[i    ] = LTBL(sp[0]);
				buffi[i + 1] = LTBL(sp[1]);
				buff2[i - 1] = (FTYPE) buffi[i];
				buff2[i    ] = (FTYPE) buffi[i + 1];

				D2I(dd.i32s.i0,
				    p00 * k0 + p01 * k1 + p10 * k2 + p11 * k3);
				D2I(dd.i32s.i1,
				    p01 * k0 + p02 * k1 + p11 * k2 + p12 * k3);
				*(mlib_d64 *)(buffo + i) = dd.d64;

#ifdef _NO_LONGLONG

				o64_1 = buffo[i];
				o64_2 = buffo[i + 1];
				STORE2(FROM_S32(o64_1), FROM_S32(o64_2));

#else /* _NO_LONGLONG */

				o64 = *(mlib_s64 *)(buffo + i);
				STORE2(FROM_S32(o64 >> 32), FROM_S32(o64));

#endif /* _NO_LONGLONG */

				sp += 2;
				dp += chan2;
			}

			for (; i < wid; i++) {
				p00 = buff0[i - 1];
				p10 = buff1[i - 1];
				p01 = buff0[i];
				p11 = buff1[i];

				buffi[i    ] = LTBL(sp[0]);
				buff2[i - 1] = (FTYPE) buffi[i];

				D2I(buffo[i],
				    p00 * k0 + p01 * k1 + p10 * k2 + p11 * k3);

				dp[0] = FROM_S32(buffo[i]);

				sp++;
				dp += chan1;
			}

			buff2[wid - 1] = buff2[wid - 2];
		}

		ColorTrue2IndexLine(bufft, da, wid, colormap);

		if (j < hgt - 3)
			sl += sll;
		da += dll;

		buff_ind += 4;

		if (buff_ind >= 4 * (KSIZE + 1))
			buff_ind = 0;
	}

	__mlib_free(pbuff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef  KSIZE
#define	KSIZE	3

mlib_status
CONV_FUNC(3x3)
{
	FTYPE *buff0, *buff1, *buff2, *buff3;
	FTYPE *buffs[8 * (KSIZE + 1)];
	FTYPE s0, s1;
	LTYPE *bufft;
	mlib_s32 *buffo;
	DTYPE *da;

	DEF_VARS(DTYPE);
	DTYPE *sl1;
	DTYPE *sl2;

	LOAD_KERNEL3();
	GET_SRC_DST_PARAMETERS(DTYPE);

	nchan = mlib_ImageGetLutChannels(colormap);
	ColorTrue2IndexLine = GET_ColorTrueFunc();
	chan1 = nchan;
	chan2 = chan1 + chan1;
	da = adr_dst;

	wid1 = (wid + KSIZE) & ~1;
	xsize = wid1 * 4;
	bsize =
	    (KSIZE + 1) * xsize * sizeof (FTYPE) +
	    2 * wid1 * sizeof (mlib_s32) + xsize * sizeof (LTYPE);
	pbuff = __mlib_malloc(bsize);

	if (pbuff == NULL)
		return (MLIB_FAILURE);

	for (i = 0; i < (KSIZE + 1); i++) {
		for (c = 0; c < 4; c++)
			buffs[4 * i + c] =
			    pbuff + i * wid1 + c * ((KSIZE + 1) * wid1);
	}

	for (i = 0; i < 4 * (KSIZE + 1); i++)
		buffs[4 * (KSIZE + 1) + i] = buffs[i];
	buffo = (mlib_s32 *)(buffs[4 * (KSIZE + 1) - 1] + wid1);

	buffi = buffo + wid1;
	bufft = (LTYPE *) (buffi + wid1);

	sl = adr_src;
	sl1 = sl;

	if (hgt > 1)
		sl2 = sl1 + sll;
	else
		sl2 = sl1;

	for (c = 0; c < nchan; c++) {
		ltbl = lut_table[c] - offset;

		buff0 = buffs[c];
		buff1 = buffs[c + 4];
		buff2 = buffs[c + 8];

		buff0[0] = (FTYPE) LTBL(sl[0]);
		buff1[0] = (FTYPE) LTBL(sl1[0]);
		buff2[0] = (FTYPE) LTBL(sl2[0]);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < wid; i++) {
			buff0[i + 1] = (FTYPE) LTBL(sl[i]);
			buff1[i + 1] = (FTYPE) LTBL(sl1[i]);
			buff2[i + 1] = (FTYPE) LTBL(sl2[i]);
		}

		buff0[wid + 1] = buff0[wid];
		buff1[wid + 1] = buff1[wid];
		buff2[wid + 1] = buff2[wid];
	}

	if (hgt > 2)
		sl = sl2 + sll;
	else
		sl = sl2;
	dl = bufft;

	for (j = 0; j < hgt; j++) {
		FTYPE **buffc = buffs + buff_ind;

		for (c = 0; c < nchan; c++) {
			ltbl = lut_table[c] - offset;

			buff0 = buffc[c];
			buff1 = buffc[c + 4];
			buff2 = buffc[c + 8];
			buff3 = buffc[c + 12];

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
			dp = dl + c;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (wid - 2); i += 2) {
#ifdef _NO_LONGLONG
				mlib_s32 o64_1, o64_2;
#else /* _NO_LONGLONG */
				mlib_s64 o64;
#endif /* _NO_LONGLONG */
				d64_2x32 sd, dd;

				p02 = buff0[i + 2];
				p12 = buff1[i + 2];
				p22 = buff2[i + 2];
				p03 = buff0[i + 3];
				p13 = buff1[i + 3];
				p23 = buff2[i + 3];

				LOAD_BUFF(buffi);

				sd.d64 = *(mlib_d64 *)(buffi + i);
				buff3[i + 1] = (FTYPE) sd.i32s.i0;
				buff3[i + 2] = (FTYPE) sd.i32s.i1;

				D2I(dd.i32s.i0,
				    s0 + p02 * k2 + p12 * k5 + p22 * k8);
				D2I(dd.i32s.i1,
				    s1 + p02 * k1 + p03 * k2 + p12 * k4 +
				    p13 * k5 + p22 * k7 + p23 * k8);
				*(mlib_d64 *)(buffo + i) = dd.d64;

				s0 = p02 * k0 + p03 * k1 + p12 * k3 + p13 * k4 +
				    p22 * k6 + p23 * k7;
				s1 = p03 * k0 + p13 * k3 + p23 * k6;

#ifdef _NO_LONGLONG

				o64_1 = buffo[i];
				o64_2 = buffo[i + 1];
/* #ifdef TYPE_SHORT */
#if 1
				STORE2(FROM_S32(o64_1), FROM_S32(o64_2));
#else /* 1 */
				STORE2(o64_1 >> 24, o64_2 >> 24);
#endif /* 1 */

#else /* _NO_LONGLONG */

				o64 = *(mlib_s64 *)(buffo + i);
/* #ifdef TYPE_SHORT */
#if 1
				STORE2(FROM_S32(o64 >> 32), FROM_S32(o64));
#else /* 1 */
				STORE2(o64 >> 56, o64 >> 24);
#endif /* 1 */
#endif /* _NO_LONGLONG */

				sp += 2;
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

				buffi[i] = LTBL(sp[0]);
				buff3[i + 1] = (FTYPE) buffi[i];

				D2I(buffo[i],
				    p00 * k0 + p01 * k1 + p02 * k2 + p10 * k3 +
				    p11 * k4 + p12 * k5 + p20 * k6 + p21 * k7 +
				    p22 * k8);

/* #ifdef TYPE_SHORT */
#if 1
				dp[0] = FROM_S32(buffo[i]);
#else /* 1 */
				dp[0] = buffo[i] >> 24;
#endif /* 1 */

				sp++;
				dp += chan1;
			}

			buff3[0] = buff3[1];
			buff3[wid + 1] = buff3[wid];
		}

		ColorTrue2IndexLine(bufft, da, wid, colormap);

		if (j < hgt - 3)
			sl += sll;
		da += dll;

		buff_ind += 4;

		if (buff_ind >= 4 * (KSIZE + 1))
			buff_ind = 0;
	}

	__mlib_free(pbuff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef  KSIZE
#define	KSIZE	4

mlib_status
CONV_FUNC(4x4)
{
	FTYPE *buff0, *buff1, *buff2, *buff3, *buff4, *buffd;
	FTYPE *buffs[8 * (KSIZE + 1)];
	FTYPE k[KSIZE * KSIZE];
	FTYPE k0, k1, k2, k3, k4, k5, k6, k7;
	FTYPE p00, p01, p02, p03, p04,
	    p10, p11, p12, p13, p14, p20, p21, p22, p23, p30, p31, p32, p33;
	mlib_s32 d0, d1;
	LTYPE *bufft;
	mlib_s32 *buffo;
	DTYPE *da;

	DEF_VARS(DTYPE);
	DTYPE *sl1;
	DTYPE *sl2, *sl3;

	LOAD_KERNEL(KSIZE * KSIZE);
	GET_SRC_DST_PARAMETERS(DTYPE);

	nchan = mlib_ImageGetLutChannels(colormap);
	ColorTrue2IndexLine = GET_ColorTrueFunc();
	chan1 = nchan;
	chan2 = chan1 + chan1;
	da = adr_dst;

	wid1 = (wid + KSIZE) & ~1;
	xsize = wid1 * 4;
	bsize = (KSIZE + 1) * xsize * sizeof (FTYPE) + wid1 * sizeof (FTYPE) +
	    2 * wid1 * sizeof (mlib_s32) + xsize * sizeof (LTYPE);
	pbuff = __mlib_malloc(bsize);

	if (pbuff == NULL)
		return (MLIB_FAILURE);

	for (i = 0; i < (KSIZE + 1); i++) {
		for (c = 0; c < 4; c++)
			buffs[4 * i + c] =
			    pbuff + i * wid1 + c * ((KSIZE + 1) * wid1);
	}

	for (i = 0; i < 4 * (KSIZE + 1); i++)
		buffs[4 * (KSIZE + 1) + i] = buffs[i];
	buffd = buffs[4 * (KSIZE + 1) - 1] + wid1;
	buffo = (mlib_s32 *)(buffd + wid1);
	buffi = buffo + wid1;
	bufft = (LTYPE *) (buffi + wid1);

	sl = adr_src;
	sl1 = sl;

	if (hgt > 1)
		sl2 = sl1 + sll;
	else
		sl2 = sl1;

	if (hgt > 2)
		sl3 = sl2 + sll;
	else
		sl3 = sl2;

	for (c = 0; c < nchan; c++) {
		ltbl = lut_table[c] - offset;

		buff0 = buffs[c];
		buff1 = buffs[c + 4];
		buff2 = buffs[c + 8];
		buff3 = buffs[c + 12];

		buff0[0] = (FTYPE) LTBL(sl[0]);
		buff1[0] = (FTYPE) LTBL(sl1[0]);
		buff2[0] = (FTYPE) LTBL(sl2[0]);
		buff3[0] = (FTYPE) LTBL(sl3[0]);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < wid; i++) {
			buff0[i + 1] = (FTYPE) LTBL(sl[i]);
			buff1[i + 1] = (FTYPE) LTBL(sl1[i]);
			buff2[i + 1] = (FTYPE) LTBL(sl2[i]);
			buff3[i + 1] = (FTYPE) LTBL(sl3[i]);
		}

		buff0[wid + 1] = buff0[wid + 2] = buff0[wid];
		buff1[wid + 1] = buff1[wid + 2] = buff1[wid];
		buff2[wid + 1] = buff2[wid + 2] = buff2[wid];
		buff3[wid + 1] = buff3[wid + 2] = buff3[wid];
	}

	if (hgt > 3)
		sl = sl3 + sll;
	else
		sl = sl3;

	dl = bufft;

	for (j = 0; j < hgt; j++) {
		FTYPE **buffc = buffs + buff_ind;

		for (c = 0; c < nchan; c++) {
			d64_2x32 dd;

			ltbl = lut_table[c] - offset;

			buff0 = buffc[c];
			buff1 = buffc[c + 4];
			buff2 = buffc[c + 8];
			buff3 = buffc[c + 12];
			buff4 = buffc[c + 16];

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
			dp = dl + c;

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

				LOAD_BUFF(buffi);

				dd.d64 = *(mlib_d64 *)(buffi + i);
				buff4[i + 1] = (FTYPE) dd.i32s.i0;
				buff4[i + 2] = (FTYPE) dd.i32s.i1;

				buffd[i] =
				    (p00 * k0 + p01 * k1 + p02 * k2 + p03 * k3 +
				    p10 * k4 + p11 * k5 + p12 * k6 + p13 * k7);
				buffd[i + 1] =
				    (p01 * k0 + p02 * k1 + p03 * k2 + p04 * k3 +
				    p11 * k4 + p12 * k5 + p13 * k6 + p14 * k7);

				sp += 2;
				dp += chan2;
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

			sp = sl;
			dp = dl + c;

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

				D2I(d0,
				    p00 * k0 + p01 * k1 + p02 * k2 + p03 * k3 +
				    p10 * k4 + p11 * k5 + p12 * k6 + p13 * k7 +
				    buffd[i]);
				D2I(d1,
				    p01 * k0 + p02 * k1 + p03 * k2 + p04 * k3 +
				    p11 * k4 + p12 * k5 + p13 * k6 + p14 * k7 +
				    buffd[i + 1]);

				dp[0] = FROM_S32(d0);
				dp[chan1] = FROM_S32(d1);

				sp += 2;
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

				buff4[i + 1] = (FTYPE) LTBL(sp[0]);

				D2I(buffo[i],
				    p00 * k[0] + p01 * k[1] + p02 * k[2] +
				    p03 * k[3] + p10 * k[4] + p11 * k[5] +
				    p12 * k[6] + p13 * k[7] + p20 * k[8] +
				    p21 * k[9] + p22 * k[10] + p23 * k[11] +
				    p30 * k[12] + p31 * k[13] + p32 * k[14] +
				    p33 * k[15]);

				dp[0] = FROM_S32(buffo[i]);

				sp++;
				dp += chan1;
			}

			buff4[0] = buff4[1];
			buff4[wid + 1] = buff4[wid + 2] = buff4[wid];
		}

		ColorTrue2IndexLine(bufft, da, wid, colormap);

		if (j < hgt - 4)
			sl += sll;
		da += dll;

		buff_ind += 4;

		if (buff_ind >= 4 * (KSIZE + 1))
			buff_ind = 0;
	}

	__mlib_free(pbuff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef  KSIZE
#define	KSIZE	5

mlib_status
CONV_FUNC(5x5)
{
	FTYPE *buff0, *buff1, *buff2, *buff3, *buff4, *buff5, *buffd;
	FTYPE *buffs[8 * (KSIZE + 1)];
	FTYPE k[KSIZE * KSIZE];
	FTYPE k0, k1, k2, k3, k4, k5, k6, k7, k8, k9;
	FTYPE p00, p01, p02, p03, p04, p05,
	    p10, p11, p12, p13, p14, p15,
	    p20, p21, p22, p23, p24,
	    p30, p31, p32, p33, p34, p40, p41, p42, p43, p44;
	mlib_s32 d0, d1;
	LTYPE *bufft;
	mlib_s32 *buffo;
	DTYPE *da;

	DEF_VARS(DTYPE);
	DTYPE *sl1;
	DTYPE *sl2, *sl3, *sl4;

	LOAD_KERNEL(KSIZE * KSIZE);
	GET_SRC_DST_PARAMETERS(DTYPE);

	nchan = mlib_ImageGetLutChannels(colormap);
	ColorTrue2IndexLine = GET_ColorTrueFunc();
	chan1 = nchan;
	chan2 = chan1 + chan1;
	da = adr_dst;

	wid1 = (wid + KSIZE) & ~1;
	xsize = wid1 * 4;
	bsize = (KSIZE + 1) * xsize * sizeof (FTYPE) + wid1 * sizeof (FTYPE) +
	    2 * wid1 * sizeof (mlib_s32) + xsize * sizeof (LTYPE);
	pbuff = __mlib_malloc(bsize);

	if (pbuff == NULL)
		return (MLIB_FAILURE);

	for (i = 0; i < (KSIZE + 1); i++) {
		for (c = 0; c < 4; c++)
			buffs[4 * i + c] =
			    pbuff + i * wid1 + c * ((KSIZE + 1) * wid1);
	}

	for (i = 0; i < 4 * (KSIZE + 1); i++)
		buffs[4 * (KSIZE + 1) + i] = buffs[i];
	buffd = buffs[4 * (KSIZE + 1) - 1] + wid1;
	buffo = (mlib_s32 *)(buffd + wid1);
	buffi = buffo + wid1;
	bufft = (LTYPE *) (buffi + wid1);

	sl = adr_src;
	sl1 = sl;
	sl2 = sl;

	if (hgt > 1)
		sl3 = sl2 + sll;
	else
		sl3 = sl2;

	if (hgt > 2)
		sl4 = sl3 + sll;
	else
		sl4 = sl3;

	for (c = 0; c < nchan; c++) {
		ltbl = lut_table[c] - offset;

		buff0 = buffs[c];
		buff1 = buffs[c + 4];
		buff2 = buffs[c + 8];
		buff3 = buffs[c + 12];
		buff4 = buffs[c + 16];

		buff0[0] = buff0[1] = (FTYPE) LTBL(sl[0]);
		buff1[0] = buff1[1] = (FTYPE) LTBL(sl1[0]);
		buff2[0] = buff2[1] = (FTYPE) LTBL(sl2[0]);
		buff3[0] = buff3[1] = (FTYPE) LTBL(sl3[0]);
		buff4[0] = buff4[1] = (FTYPE) LTBL(sl4[0]);

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < wid; i++) {
			buff0[i + 2] = (FTYPE) LTBL(sl[i]);
			buff1[i + 2] = (FTYPE) LTBL(sl1[i]);
			buff2[i + 2] = (FTYPE) LTBL(sl2[i]);
			buff3[i + 2] = (FTYPE) LTBL(sl3[i]);
			buff4[i + 2] = (FTYPE) LTBL(sl4[i]);
		}

		buff0[wid + 2] = buff0[wid + 3] = buff0[wid + 1];
		buff1[wid + 2] = buff1[wid + 3] = buff1[wid + 1];
		buff2[wid + 2] = buff2[wid + 3] = buff2[wid + 1];
		buff3[wid + 2] = buff3[wid + 3] = buff3[wid + 1];
		buff4[wid + 2] = buff4[wid + 3] = buff4[wid + 1];
	}

	if (hgt > 3)
		sl = sl4 + sll;
	else
		sl = sl4;

	dl = bufft;

	for (j = 0; j < hgt; j++) {
		FTYPE **buffc = buffs + buff_ind;

		for (c = 0; c < nchan; c++) {
			d64_2x32 dd;

			ltbl = lut_table[c] - offset;

			buff0 = buffc[c];
			buff1 = buffc[c + 4];
			buff2 = buffc[c + 8];
			buff3 = buffc[c + 12];
			buff4 = buffc[c + 16];
			buff5 = buffc[c + 20];

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
			dp = dl + c;

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

				sp += 2;
				dp += chan2;
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

			sp = sl;
			dp = dl + c;

			p02 = buff2[0];
			p12 = buff3[0];
			p03 = buff2[1];
			p13 = buff3[1];
			p04 = buff2[2];
			p14 = buff3[2];

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

				dd.d64 = *(mlib_d64 *)(buffi + i);
				buff5[i + 2] = (FTYPE) dd.i32s.i0;
				buff5[i + 3] = (FTYPE) dd.i32s.i1;

				buffd[i] +=
				    (p00 * k0 + p01 * k1 + p02 * k2 + p03 * k3 +
				    p04 * k4 + p10 * k5 + p11 * k6 + p12 * k7 +
				    p13 * k8 + p14 * k9);
				buffd[i + 1] +=
				    (p01 * k0 + p02 * k1 + p03 * k2 + p04 * k3 +
				    p05 * k4 + p11 * k5 + p12 * k6 + p13 * k7 +
				    p14 * k8 + p15 * k9);

				sp += 2;
				dp += chan2;
			}

/*
 *  3 loop
 */
			k0 = k[20];
			k1 = k[21];
			k2 = k[22];
			k3 = k[23];
			k4 = k[24];

			sp = sl;
			dp = dl + c;

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

				D2I(d0,
				    p00 * k0 + p01 * k1 + p02 * k2 + p03 * k3 +
				    p04 * k4 + buffd[i]);
				D2I(d1,
				    p01 * k0 + p02 * k1 + p03 * k2 + p04 * k3 +
				    p05 * k4 + buffd[i + 1]);

				dp[0] = FROM_S32(d0);
				dp[chan1] = FROM_S32(d1);

				sp += 2;
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

				buff5[i + 2] = (FTYPE) LTBL(sp[0]);

				D2I(buffo[i],
				    p00 * k[0] + p01 * k[1] + p02 * k[2] +
				    p03 * k[3] + p04 * k[4] + p10 * k[5] +
				    p11 * k[6] + p12 * k[7] + p13 * k[8] +
				    p14 * k[9] + p20 * k[10] + p21 * k[11] +
				    p22 * k[12] + p23 * k[13] + p24 * k[14] +
				    p30 * k[15] + p31 * k[16] + p32 * k[17] +
				    p33 * k[18] + p34 * k[19] + p40 * k[20] +
				    p41 * k[21] + p42 * k[22] + p43 * k[23] +
				    p44 * k[24]);

				dp[0] = FROM_S32(buffo[i]);

				sp++;
				dp += chan1;
			}

			buff5[0] = buff5[1] = buff5[2];
			buff5[wid + 2] = buff5[wid + 3] = buff5[wid + 1];
		}

		ColorTrue2IndexLine(bufft, da, wid, colormap);

		if (j < hgt - 4)
			sl += sll;
		da += dll;

		buff_ind += 4;

		if (buff_ind >= 4 * (KSIZE + 1))
			buff_ind = 0;
	}

	__mlib_free(pbuff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MAX_KER	7

/* *********************************************************** */

#define	MAX_N	9
#define	MAX_MN	(9*9)

mlib_status
mlib_convMxNIndex_16_8ext(
    const mlib_image *src,
    mlib_image *dst,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_s32 *kern,
    mlib_s32 scalef_expon,
    const void *colormap)
{
	FTYPE k_arr[MAX_MN], *k = k_arr;
	FTYPE *buffs_arr[8 * (MAX_N + 1)], **buffs = buffs_arr;
	FTYPE *buffd;
	FTYPE k0, k1, k2, k3, k4, k5, k6;
	FTYPE p0, p1, p2, p3, p4, p5, p6, p7;
	LTYPE *bufft;
	DTYPE *da;
	mlib_s32 l, off, kw;
	mlib_s32 d0, d1;

	DEF_VARS(DTYPE);

	if ((m * n) > MAX_MN) {
		k = __mlib_malloc((m * n) * sizeof (FTYPE));

		if (k == NULL)
			return (MLIB_FAILURE);
	}

	LOAD_KERNEL(m * n);
	GET_SRC_DST_PARAMETERS(DTYPE);

	nchan = mlib_ImageGetLutChannels(colormap);
	ColorTrue2IndexLine = GET_ColorTrueFunc();
	chan1 = nchan;
	chan2 = chan1 + chan1;
	da = adr_dst;

	wid1 = (wid + m) & ~1;
	xsize = wid1 * 4;
	bsize = (n + 1) * xsize * sizeof (FTYPE) + wid1 * sizeof (FTYPE) +
	    2 * wid1 * sizeof (mlib_s32) + xsize * sizeof (LTYPE);
	pbuff = __mlib_malloc(bsize);

	if (pbuff == NULL) {
		if (k != k_arr)
			__mlib_free(k);
		return (MLIB_FAILURE);
	}

	if (n > MAX_N) {
		buffs = __mlib_malloc(8 * (n + 1) * sizeof (FTYPE *));

		if (buffs == NULL) {
			__mlib_free(pbuff);

			if (k != k_arr)
				__mlib_free(k);
			return (MLIB_FAILURE);
		}
	}

	for (i = 0; i < (n + 1); i++) {
		for (c = 0; c < 4; c++) {
			buffs[4 * i + c] =
			    pbuff + i * wid1 + c * ((n + 1) * wid1);
		}
	}

	for (i = 0; i < 4 * (n + 1); i++)
		buffs[4 * (n + 1) + i] = buffs[i];
	buffd = buffs[4 * (n + 1) - 1] + wid1;
	buffi = (mlib_s32 *)(buffd + wid1);
	bufft = (LTYPE *) (buffi + wid1);

	for (c = 0; c < nchan; c++) {
		ltbl = lut_table[c] - offset;

		sl = adr_src;

		for (l = 0; l < n; l++) {
			FTYPE *buff;

			buff = buffs[c + 4 * l];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < dx_l; i++) {
				*buff++ = (FTYPE) LTBL(sl[0]);
			}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < wid + m - 1 - dx_l - dx_r; i++) {
				*buff++ = (FTYPE) LTBL(sl[i]);
			}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < dx_r; i++) {
				buff[i] = buff[-1];
			}

			if ((l >= dy_t) && (l < hgt + n - dy_b - 2))
				sl += sll;
		}
	}

	buff_ind = 0;
	dl = bufft;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < wid; i++)
		buffd[i] = 0.;

	for (j = 0; j < hgt; j++) {
		FTYPE **buffc = buffs + buff_ind;

		for (c = 0; c < nchan; c++) {
			FTYPE *buffn = buffc[c + 4 * n];
			FTYPE *pk = k;

			ltbl = lut_table[c] - offset;

			for (l = 0; l < n; l++) {
				FTYPE *buff_l = buffc[c + 4 * l];

				for (off = 0; off < m; ) {
					FTYPE *buff = buff_l + off;

					kw = m - off;

					if (kw > 2 * MAX_KER)
						kw = MAX_KER;
					else if (kw > MAX_KER)
						kw = kw / 2;
					off += kw;

					sp = sl;
					dp = dl + c;

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
								    (FTYPE)
								    LTBL(sp[0]);
								buffn[i + dx_l +
								    1] =
								    (FTYPE)
								    LTBL(sp[1]);

								D2I(d0,
								    p0 * k0 +
								    p1 * k1 +
								    p2 * k2 +
								    p3 * k3 +
								    p4 * k4 +
								    p5 * k5 +
								    p6 * k6 +
								    buffd[i]);
								D2I(d1,
								    p1 * k0 +
								    p2 * k1 +
								    p3 * k2 +
								    p4 * k3 +
								    p5 * k4 +
								    p6 * k5 +
								    p7 * k6 +
								    buffd[i +
								    1]);

								dp[0] =
								    FROM_S32
								    (d0);
								dp[chan1] =
								    FROM_S32
								    (d1);

								buffd[i] = 0;
								buffd[i + 1] =
								    0;

								sp += 2;
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
								    (FTYPE)
								    LTBL(sp[0]);
								buffn[i + dx_l +
								    1] =
								    (FTYPE)
								    LTBL(sp[1]);

								D2I(d0,
								    p0 * k0 +
								    p1 * k1 +
								    p2 * k2 +
								    p3 * k3 +
								    p4 * k4 +
								    p5 * k5 +
								    buffd[i]);
								D2I(d1,
								    p1 * k0 +
								    p2 * k1 +
								    p3 * k2 +
								    p4 * k3 +
								    p5 * k4 +
								    p6 * k5 +
								    buffd[i +
								    1]);

								dp[0] =
								    FROM_S32
								    (d0);
								dp[chan1] =
								    FROM_S32
								    (d1);

								buffd[i] = 0;
								buffd[i + 1] =
								    0;

								sp += 2;
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
								    (FTYPE)
								    LTBL(sp[0]);
								buffn[i + dx_l +
								    1] =
								    (FTYPE)
								    LTBL(sp[1]);

								D2I(d0,
								    p0 * k0 +
								    p1 * k1 +
								    p2 * k2 +
								    p3 * k3 +
								    p4 * k4 +
								    buffd[i]);
								D2I(d1,
								    p1 * k0 +
								    p2 * k1 +
								    p3 * k2 +
								    p4 * k3 +
								    p5 * k4 +
								    buffd[i +
								    1]);

								dp[0] =
								    FROM_S32
								    (d0);
								dp[chan1] =
								    FROM_S32
								    (d1);

								buffd[i] = 0;
								buffd[i + 1] =
								    0;

								sp += 2;
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
								    (FTYPE)
								    LTBL(sp[0]);
								buffn[i + dx_l +
								    1] =
								    (FTYPE)
								    LTBL(sp[1]);

								D2I(d0,
								    p0 * k0 +
								    p1 * k1 +
								    p2 * k2 +
								    p3 * k3 +
								    buffd[i]);
								D2I(d1,
								    p1 * k0 +
								    p2 * k1 +
								    p3 * k2 +
								    p4 * k3 +
								    buffd[i +
								    1]);

								dp[0] =
								    FROM_S32
								    (d0);
								dp[chan1] =
								    FROM_S32
								    (d1);

								buffd[i] = 0;
								buffd[i + 1] =
								    0;

								sp += 2;
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
								    (FTYPE)
								    LTBL(sp[0]);
								buffn[i + dx_l +
								    1] =
								    (FTYPE)
								    LTBL(sp[1]);

								D2I(d0,
								    p0 * k0 +
								    p1 * k1 +
								    p2 * k2 +
								    buffd[i]);
								D2I(d1,
								    p1 * k0 +
								    p2 * k1 +
								    p3 * k2 +
								    buffd[i +
								    1]);

								dp[0] =
								    FROM_S32
								    (d0);
								dp[chan1] =
								    FROM_S32
								    (d1);

								buffd[i] = 0;
								buffd[i + 1] =
								    0;

								sp += 2;
								dp += chan2;
							}
						}

					} else {
/* kw == 2 */

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
								    (FTYPE)
								    LTBL(sp[0]);
								buffn[i + dx_l +
								    1] =
								    (FTYPE)
								    LTBL(sp[1]);

								D2I(d0,
								    p0 * k0 +
								    p1 * k1 +
								    buffd[i]);
								D2I(d1,
								    p1 * k0 +
								    p2 * k1 +
								    buffd[i +
								    1]);

								dp[0] =
								    FROM_S32
								    (d0);
								dp[chan1] =
								    FROM_S32
								    (d1);

								buffd[i] = 0;
								buffd[i + 1] =
								    0;

								sp += 2;
								dp += chan2;
							}
						}
					}

					pk += kw;
				}
			}

/* last pixels */
			for (; i < wid; i++) {
				FTYPE *pk = k, s = 0;

				for (l = 0; l < n; l++) {
					FTYPE *buff = buffc[c + 4 * l] + i;
					mlib_s32 x;

					for (x = 0; x < m; x++)
						s += buff[x] * ((*pk++));
				}

				D2I(d0, s);
				dp[0] = FROM_S32(d0);

				buffn[i + dx_l] = (FTYPE) LTBL(sp[0]);

				sp++;
				dp += chan1;
			}

			for (i = 0; i < dx_l; i++) {
				buffn[i] = buffn[dx_l];
			}
			buffn += dx_l + wid;
			for (i = 0; i < m - 1 - dx_l - dx_r; i++) {
				*buffn++ = (FTYPE) LTBL(sp[0]);
				sp++;
			}
			for (i = 0; i < dx_r; i++) {
				buffn[i] = buffn[-1];
			}
		}

		ColorTrue2IndexLine(bufft, da, wid, colormap);

/* next line */

		if (j < hgt - dy_b - 2)
			sl += sll;
		da += dll;

		buff_ind += 4;

		if (buff_ind >= 4 * (n + 1))
			buff_ind = 0;
	}

	if (buffs != buffs_arr)
		__mlib_free(buffs);
	__mlib_free(pbuff);

	if (k != k_arr)
		__mlib_free(k);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
