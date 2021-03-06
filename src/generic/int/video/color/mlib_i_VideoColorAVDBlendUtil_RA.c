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
#pragma ident	"@(#)mlib_i_VideoColorAVDBlendUtil_RA.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorBlend_[ZERO, ONE, DA, SA, OMDA, OMSA]_
 *      [ZERO, ONE, DA, SA, OMDA, OMSA]_RA
 *          blend two images with blend factors with alpha reset
 *
 * SYNOPSIS
 *      void mlib_VideoColorBlend_[ZERO, ONE, DA, SA, OMDA, OMSA]_
 *           [ZERO, ONE, DA, SA, OMDA, OMSA]_RA
 *                                             (mlib_u32 *dst,
 *                                              mlib_u32 *src1,
 *                                              mlib_u32 *src2,
 *                                              mlib_s32 w,
 *                                              mlib_s32 h,
 *                                              mlib_s32 dlb,
 *                                              mlib_s32 slb1,
 *                                              mlib_s32 slb2)
 *
 * ARGUMENT
 *      dst     pointer to output image
 *      src1    pointer to first input image
 *      src2    pointer to second input image
 *      w       image width
 *      h       image height
 *      dlb     linebytes for output image
 *      slb1    linebytes for 1st input image
 *      slb2    linebytes for 2nd input image
 *
 * DESCRIPTION
 *        dst = (src1 * src1_blend) + (src2 * src2_blend)
 */

#include <mlib_algebra.h>
#include <mlib_VideoColorAVDBlend_proto.h>
#include <mlib_c_ImageBlendTable.h>
#include <mlib_ImageDivTables.h>

/* *********************************************************** */

void
mlib_VideoColorBlend_ZERO_ZERO_RA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_s32 i;

	for (i = 0; i < h; i++) {
		__mlib_VectorZero_S32((mlib_s32 *)dst, w);
		dst += dlb / 4;
	}
}

/* *********************************************************** */

#define	SAT_U8_U8C(dst, src)	dst = (((src << 23) >> 31) | src)

#ifdef MLIB_USE_FTOI_CLAMPING

#define	FLOAT2INT_CLAMP(X)	((mlib_s32)(X))

#else /* MLIB_USE_FTOI_CLAMPING */

#define	FLOAT2INT_CLAMP(X)                                       \
	(((X) >= MLIB_S32_MAX) ? MLIB_S32_MAX : (mlib_s32)(X))

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#define	BLEND_VALIDATE                                          \
	mlib_u8 *psrc1, *psrc2, *pdst;                          \
	mlib_s32 i, j;                                          \
	                                                        \
	pdst = (mlib_u8 *)dst;                                  \
	psrc1 = (mlib_u8 *)src1;                                \
	psrc2 = (mlib_u8 *)src2

/* *********************************************************** */

#ifdef _LITTLE_ENDIAN

#define	COPY_CONST32	0xFFFFFF00
#define	COPY_CONST64	MLIB_U64_CONST(0xFFFFFF00FFFFFF00)

#else /* _LITTLE_ENDIAN */

#define	COPY_CONST32	0xFFFFFF
#define	COPY_CONST64	MLIB_U64_CONST(0xFFFFFF00FFFFFF)

#endif /* _LITTLE_ENDIAN */

/* *********************************************************** */

static void
mlib_VideoColorCopy_RA_U32(
	mlib_u32 *dst,
	const mlib_u32 *src,
	mlib_s32 n)
{
	mlib_s32 i;

	if (n <= 0)
		return;

	if (dst != src) {

#ifndef _NO_LONGLONG

		if ((((mlib_addr)dst | (mlib_addr)src) & 7) == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < n / 2; i++) {
				((mlib_u64 *)dst)[i] =
					((mlib_u64 *)src)[i] & COPY_CONST64;
			}
		} else
#endif /* _NO_LONGLONG */
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < n / 2; i++) {
				dst[2 * i + 1] = src[2 * i + 1] & COPY_CONST32;
				dst[2 * i] = src[2 * i] & COPY_CONST32;
			}

		if (n & 1) {
			dst[2 * i] = src[2 * i] & COPY_CONST32;
		}
	} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++)
			((mlib_u8 *)dst)[4 * i] = 0;
	}
}

/* *********************************************************** */

void
mlib_VideoColorCopy_RA(
	mlib_u32 *dst,
	const mlib_u32 *src,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb)
{
	mlib_s32 i;

	if (dlb == (4 * w) && slb == dlb) {
		w *= h;
		h = 1;
	}

	for (i = 0; i < h; i++) {
		mlib_VideoColorCopy_RA_U32(dst, src, w);
		dst += dlb / 4;
		src += slb / 4;
	}
}

/* *********************************************************** */

void
mlib_VideoColorCopy2_RA(
	mlib_u32 *dst,
	const mlib_u32 *src,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 istop,
	mlib_s32 istart,
	mlib_s32 dlb,
	mlib_s32 slb)
{
	mlib_s32 i;

	for (i = 0; i < h; i++) {
		mlib_VideoColorCopy_RA_U32(dst, src, istop);
		mlib_VideoColorCopy_RA_U32(dst + istart, src + istart,
			w - istart);
		dst += dlb / 4;
		src += slb / 4;
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_ZERO_ONE_RA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_VideoColorCopy_RA(dst, src2, w, h, dlb, slb2);
}

/* *********************************************************** */

#ifdef _LITTLE_ENDIAN

#define	DST_VAL	(v3 << 24) | (v2 << 16) | (v1 << 8)

#define	READ_SRC123                                             \
	{                                                       \
	    v1 = (((val0 >> 8) & 0xff) * a) >> 8;	        \
	    v2 = (((val0 >> 16) & 0xff) * a) >> 8;	        \
	    v3 = (((val0 >> 24) & 0xff) * a) >> 8;	        \
	}

#else /* _LITTLE_ENDIAN */

#define	DST_VAL	(v1 << 16) | (v2 << 8) | v3

#define	READ_SRC123                                             \
	{                                                       \
	    v1 = (((val0 >> 16) & 0xFF) * a) >> 8;             \
	    v2 = (((val0 >> 8) & 0xFF) * a) >> 8;             \
	    v3 = (((val0) & 0xFF) * a) >> 8;		        \
	}

#endif /* _LITTLE_ENDIAN */

/* *********************************************************** */

void
mlib_VideoColorBlend_ZERO_SA_RA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_s32 i, j;

	for (i = 0; i < h; i++, src2 += slb2 / 4, dst += dlb / 4) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < w; j++) {
			mlib_u32 val0;
			mlib_s32 v1, v2, v3;
			mlib_s32 a;

			val0 = *(mlib_u32 *)((mlib_u8 *)src2 + 4 * j);
#ifdef _LITTLE_ENDIAN
			a = val0 & 0xff;
#else /* _LITTLE_ENDIAN */
			a = (val0 >> 24) & 0xff;
#endif /* _LITTLE_ENDIAN */
			READ_SRC123;

			*(mlib_u32 *)((mlib_u8 *)dst + 4 * j) = DST_VAL;
		}
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_ZERO_OMSA_RA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_s32 i, j;

	for (i = 0; i < h; i++, src2 += slb2 / 4, dst += dlb / 4) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < w; j++) {
			mlib_u32 val0;
			mlib_s32 v1, v2, v3;
			mlib_s32 a;

			val0 = *(mlib_u32 *)((mlib_u8 *)src2 + 4 * j);
#ifdef _LITTLE_ENDIAN
			a = 256 - (val0 & 0xff);
#else /* _LITTLE_ENDIAN */
			a = 256 - ((val0 >> 24) & 0xFF);
#endif /* _LITTLE_ENDIAN */
			READ_SRC123;

			*(mlib_u32 *)((mlib_u8 *)dst + 4 * j) = DST_VAL;
		}
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_ZERO_DA_RA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_s32 i, j;

	for (i = 0; i < h;
		i++, src1 += slb1 / 4, src2 += slb2 / 4, dst += dlb / 4) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < w; j++) {
			mlib_u32 val0, val1;
			mlib_s32 v1, v2, v3;
			mlib_s32 a;

			val1 = *((mlib_u8 *)src1 + 4 * j);
			val0 = *(mlib_u32 *)((mlib_u8 *)src2 + 4 * j);

			a  = val1;
			READ_SRC123;

			*(mlib_u32 *)((mlib_u8 *)dst + 4 * j) = DST_VAL;
		}
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_ZERO_OMDA_RA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_s32 i, j;

	for (i = 0; i < h;
		i++, src1 += slb1 / 4, src2 += slb2 / 4, dst += dlb / 4) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < w; j++) {
			mlib_u32 val0, val1;
			mlib_s32 v1, v2, v3;
			mlib_s32 a;

			val1 = *((mlib_u8 *)src1 + 4 * j);
			val0 = *(mlib_u32 *)((mlib_u8 *)src2 + 4 * j);
			a = 256  - val1;
			READ_SRC123;

			*(mlib_u32 *)((mlib_u8 *)dst + 4 * j) = DST_VAL;
		}
	}
}

/* *********************************************************** */

#define	ADD_U8_SAT(DST, SRC0, SRC1)                             \
	{                                                       \
	    mlib_s32 sum, sr;                                   \
	                                                        \
	    sum = (SRC0) + (SRC1);                              \
	    sr = sum << 23;                                     \
	    DST = (sr >> 31) | sum;                             \
	}

/* *********************************************************** */

void
mlib_VideoColorBlend_ONE_ONE_RA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_s32 i;

	for (i = 0; i < h;
		i++, src1 += slb1 / 4, src2 += slb2 / 4, dst += dlb / 4) {
		mlib_u8 *src1_8 = (mlib_u8 *)src1;
		mlib_u8 *src2_8 = (mlib_u8 *)src2;
		mlib_u8 *dst_8 = (mlib_u8 *)dst;
		mlib_u8 *enddst = dst_8 + (w - 1) * 4;
		mlib_s32 v1, v2, v3, z1, z2, z3;
		mlib_s32 s1 = src1_8[1];
		mlib_s32 s2 = src2_8[1];
		mlib_s32 s3 = src1_8[2];
		mlib_s32 s4 = src2_8[2];
		mlib_s32 s5 = src1_8[3];
		mlib_s32 s6 = src2_8[3];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; dst_8 < enddst; dst_8 += 4) {
			src1_8 += 4;
			src2_8 += 4;

			v1 = s1 + s2;
			v2 = s3 + s4;
			v3 = s5 + s6;

			s1 = src1_8[1];
			s2 = src2_8[1];
			s3 = src1_8[2];
			s4 = src2_8[2];
			s5 = src1_8[3];
			s6 = src2_8[3];

			z1 = (v1 << 23) >> 31;
			z2 = (v2 << 23) >> 31;
			z3 = (v3 << 23) >> 31;

			dst_8[0] = 0;
			dst_8[1] = v1 | z1;
			dst_8[2] = v2 | z2;
			dst_8[3] = v3 | z3;
		}

		v1 = s1 + s2;
		v2 = s3 + s4;
		v3 = s5 + s6;

		z1 = (v1 << 23) >> 31;
		z2 = (v2 << 23) >> 31;
		z3 = (v3 << 23) >> 31;

		dst_8[0] = 0;
		dst_8[1] = v1 | z1;
		dst_8[2] = v2 | z2;
		dst_8[3] = v3 | z3;
	}
}

/* *********************************************************** */

#define	BLEND8	(mlib_u8*)mlib_c_blend_u8_sat

#ifdef _LITTLE_ENDIAN

#define	GET_ALPHA_P(VAL)	((VAL) & 0xFF)

#define	ADD_F123                                                   \
	f1 = (((val2 >> 8) & 0xFF) * a) >> 8;			   \
	f2 = (((val2 >> 16) & 0xFF) * a) >> 8;			   \
	f3 = (((val2 >> 24) & 0xFF) * a) >> 8;			   \
	f1 += ((val1 >> 8) & 0xFF);				   \
	f2 += ((val1 >> 16) & 0xFF);				   \
	f3 += ((val1 >> 24) & 0xFF);				   \

#define	ADD_F123_SA                                                 \
	f1 = (((val2 >> 8) & 0xFF) * a1 + 128) >> 8;			   \
	f2 = (((val2 >> 16) & 0xFF) * a1 + 128) >> 8;			   \
	f3 = (((val2 >> 24) & 0xFF) * a1 + 128) >> 8;			   \
	f1 += (((val1 >> 8) & 0xFF) * a + 128) >> 8;			   \
	f2 += (((val1 >> 16) & 0xFF) * a + 128) >> 8;			   \
	f3 += (((val1 >> 24) & 0xFF) * a + 128) >> 8;			   \

#define	CLAMP_V123                                      	    \
	SAT_U8_U8C(v1, f1);				            \
	SAT_U8_U8C(v2, f2);				            \
	SAT_U8_U8C(v3, f3)

#define	STORE_V123	dst[i] = (v1 << 8) | (v2 << 16) | (v3 << 24)

#else /* _LITTLE_ENDIAN */

#define	GET_ALPHA_P(VAL)	((VAL >> 24) & 0xFF)

#define	ADD_F123                                                   \
	f1 = (((val2 >> 16) & 0xFF) * a) >> 8;  		   \
	f2 = (((val2 >> 8) & 0xFF) * a) >> 8;  			   \
	f3 = (((val2) & 0xFF) * a) >> 8;    			   \
	f1 +=  ((val1 >> 16) & 0xFF); 				   \
	f2 +=  ((val1 >> 8) & 0xFF); 				   \
	f3 +=  ((val1) & 0xFF); 				   \

#define	ADD_F123_SA                                                 \
	f1 = (((val2 >> 16) & 0xFF) * a1 + 128) >> 8;  		   \
	f2 = (((val2 >> 8) & 0xFF) * a1 + 128) >> 8; 			   \
	f3 = (((val2) & 0xFF) * a1 + 128) >> 8;    			   \
	f1 += (((val1 >> 16) & 0xFF) * a + 128) >> 8;  		   \
	f2 += (((val1 >> 8) & 0xFF) * a + 128) >> 8;  		   \
	f3 += (((val1) & 0xFF) * a + 128) >> 8;    			   \

#define	CLAMP_V123                                              \
	SAT_U8_U8C(v1, f1);             			    \
	SAT_U8_U8C(v2, f2); 				            \
	SAT_U8_U8C(v3, f3)

#define	STORE_V123	dst[i] = (v3) | (v2 << 8) | (v1 << 16)

#endif /* _LITTLE_ENDIAN */

/* *********************************************************** */

void
mlib_VideoColorBlend_ONE_SA_RA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_s32 i, j;

	for (j = 0; j < h;
		j++, src1 += slb1 / 4, src2 += slb2 / 4, dst += dlb / 4) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < w; i++) {
			mlib_u32 val1 = src1[i];
			mlib_u32 val2 = src2[i];
			mlib_s32 f1, f2, f3;
			mlib_u8 v1, v2, v3;
			mlib_s32 a;

			a = GET_ALPHA_P(val2);
			ADD_F123;
			CLAMP_V123;
			STORE_V123;
		}
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_ONE_OMSA_RA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_s32 i, j;

	for (j = 0; j < h;
		j++, src1 += slb1 / 4, src2 += slb2 / 4, dst += dlb / 4) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < w; i++) {
			mlib_u32 val1 = src1[i];
			mlib_u32 val2 = src2[i];
			mlib_s32 f1, f2, f3;
			mlib_u8 v1, v2, v3;
			mlib_s32 a;

			a = 256 - GET_ALPHA_P(val2);

			ADD_F123;
			CLAMP_V123;

			STORE_V123;
		}
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_ONE_DA_RA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_s32 i, j;

	for (j = 0; j < h;
		j++, src1 += slb1 / 4, src2 += slb2 / 4, dst += dlb / 4) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < w; i++) {
			mlib_u32 val1 = src1[i];
			mlib_u32 val2 = src2[i];
			mlib_s32 f1, f2, f3;
			mlib_u8 v1, v2, v3;
			mlib_s32 a;

			a = GET_ALPHA_P(val1);

			ADD_F123;
			CLAMP_V123;

			STORE_V123;
		}
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_ONE_OMDA_RA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_s32 i, j;

	for (j = 0; j < h;
		j++, src1 += slb1 / 4, src2 += slb2 / 4, dst += dlb / 4) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < w; i++) {
			mlib_u32 val1 = src1[i];
			mlib_u32 val2 = src2[i];
			mlib_s32 f1, f2, f3;
			mlib_u8 v1, v2, v3;
			mlib_s32 a;

			a = 256 - GET_ALPHA_P(val1);

			ADD_F123;
			CLAMP_V123;

			STORE_V123;
		}
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_SA_SA_RA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_s32 i, j;

	for (j = 0; j < h;
		j++, src1 += slb1 / 4, src2 += slb2 / 4, dst += dlb / 4) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < w; i++) {
			mlib_u32 val1 = src1[i];
			mlib_u32 val2 = src2[i];
			mlib_s32 f1, f2, f3;
			mlib_u8 v1, v2, v3;
			mlib_s32 a, a1;

			a = GET_ALPHA_P(val2);
			a1 = a;

			ADD_F123_SA;
			CLAMP_V123;

			STORE_V123;
		}
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_SA_OMSA_RA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_s32 i, j;

	for (j = 0; j < h;
		j++, src1 += slb1 / 4, src2 += slb2 / 4, dst += dlb / 4) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < w; i++) {
			mlib_u32 val1 = src1[i];
			mlib_u32 val2 = src2[i];
			mlib_s32 f1, f2, f3;
			mlib_s32 v1, v2, v3;
			mlib_s32 a, a1;

			a = GET_ALPHA_P(val2);
			a1 = 256 - a;

			ADD_F123_SA;
			CLAMP_V123;

			STORE_V123;
		}
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_SA_DA_RA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_s32 i, j;

	for (j = 0; j < h;
		j++, src1 += slb1 / 4, src2 += slb2 / 4, dst += dlb / 4) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < w; i++) {
			mlib_u32 val1 = src1[i];
			mlib_u32 val2 = src2[i];
			mlib_s32 f1, f2, f3;
			mlib_u8 v1, v2, v3;
			mlib_s32 a, a1;

			a = GET_ALPHA_P(val2);
			a1 = GET_ALPHA_P(val1);

			ADD_F123_SA;
			CLAMP_V123;

			STORE_V123;
		}
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_SA_OMDA_RA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_s32 i, j;

	for (j = 0; j < h;
		j++, src1 += slb1 / 4, src2 += slb2 / 4, dst += dlb / 4) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < w; i++) {
			mlib_u32 val1 = src1[i];
			mlib_u32 val2 = src2[i];
			mlib_s32 f1, f2, f3;
			mlib_u8 v1, v2, v3;
			mlib_s32 a, a1;

			a = GET_ALPHA_P(val2);
			a1 = 256 - GET_ALPHA_P(val1);

			ADD_F123_SA;
			CLAMP_V123;

			STORE_V123;
		}
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_OMSA_SA_RA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_s32 i, j;

	for (j = 0; j < h;
		j++, src1 += slb1 / 4, src2 += slb2 / 4, dst += dlb / 4) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < w; i++) {
			mlib_u32 val1 = src1[i];
			mlib_u32 val2 = src2[i];
			mlib_s32 f1, f2, f3;
			mlib_u8 v1, v2, v3;
			mlib_s32 a, a1;

			a1 = GET_ALPHA_P(val2);
			a = 256 - a1;

			ADD_F123_SA;
			CLAMP_V123;

			STORE_V123;
		}
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_OMSA_OMSA_RA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_s32 i, j;

	for (j = 0; j < h;
		j++, src1 += slb1 / 4, src2 += slb2 / 4, dst += dlb / 4) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < w; i++) {
			mlib_u32 val1 = src1[i];
			mlib_u32 val2 = src2[i];
			mlib_s32 f1, f2, f3;
			mlib_u8 v1, v2, v3;
			mlib_s32 a, a1;

			a = 256 - GET_ALPHA_P(val2);
			a1 = a;

			ADD_F123_SA;
			CLAMP_V123;

			STORE_V123;
		}
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_OMSA_OMDA_RA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_s32 i, j;

	for (j = 0; j < h;
		j++, src1 += slb1 / 4, src2 += slb2 / 4, dst += dlb / 4) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < w; i++) {
			mlib_u32 val1 = src1[i];
			mlib_u32 val2 = src2[i];
			mlib_s32 f1, f2, f3;
			mlib_u8 v1, v2, v3;
			mlib_s32 a, a1;

			a = 256 - GET_ALPHA_P(val2);
			a1 = 256 - GET_ALPHA_P(val1);

			ADD_F123_SA;
			CLAMP_V123;

			STORE_V123;
		}
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_DA_SA_RA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_s32 i, j;

	for (j = 0; j < h;
		j++, src1 += slb1 / 4, src2 += slb2 / 4, dst += dlb / 4) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < w; i++) {
			mlib_u32 val1 = src1[i];
			mlib_u32 val2 = src2[i];
			mlib_s32 f1, f2, f3;
			mlib_u8 v1, v2, v3;
			mlib_s32 a, a1;

			a = GET_ALPHA_P(val1);
			a1 = GET_ALPHA_P(val2);

			ADD_F123_SA;
			CLAMP_V123;

			STORE_V123;
		}
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_DA_OMSA_RA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_s32 i, j;

	for (j = 0; j < h;
		j++, src1 += slb1 / 4, src2 += slb2 / 4, dst += dlb / 4) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < w; i++) {
			mlib_u32 val1 = src1[i];
			mlib_u32 val2 = src2[i];
			mlib_s32 f1, f2, f3;
			mlib_u8 v1, v2, v3;
			mlib_s32 a, a1;

			a = GET_ALPHA_P(val1);
			a1 = 256 - GET_ALPHA_P(val2);

			ADD_F123_SA;
			CLAMP_V123;

			STORE_V123;
		}
	}
}

/* *********************************************************** */

void
mlib_VideoColorBlend_OMDA_OMSA_RA(
	mlib_u32 *dst,
	const mlib_u32 *src1,
	const mlib_u32 *src2,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb1,
	mlib_s32 slb2)
{
	mlib_s32 i, j;

	for (j = 0; j < h;
		j++, src1 += slb1 / 4, src2 += slb2 / 4, dst += dlb / 4) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < w; i++) {
			mlib_u32 val1 = src1[i];
			mlib_u32 val2 = src2[i];
			mlib_s32 f1, f2, f3;
			mlib_u8 v1, v2, v3;
			mlib_s32 a, a1;

			a = 256 - GET_ALPHA_P(val1);
			a1 = 256 - GET_ALPHA_P(val2);

			ADD_F123_SA;
			CLAMP_V123;

			STORE_V123;
		}
	}
}

/* *********************************************************** */
