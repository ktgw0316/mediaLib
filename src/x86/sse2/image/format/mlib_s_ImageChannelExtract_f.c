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

#pragma ident	"@(#)mlib_s_ImageChannelExtract_f.c	9.4	07/11/05 SMI"

/*
 * DESCRIPTION
 *      Internal functions.
 *
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_s_ImageChannelExtract.h>
#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */

typedef union
{
	mlib_d64 d64;
	struct
	{
		mlib_f32 f0;
		mlib_f32 f1;
	} f32s;
} d64_2_f32;

/* *********************************************************** */

void
mlib_s_ImageChannelExtract_U8_2_1(
    const mlib_u8 *sl,
    mlib_s32 slb,
    mlib_u8 *dl,
    mlib_s32 dlb,
    mlib_s32 width,
    mlib_s32 height)
{
	mlib_u8 *sp = (void *)sl;
	mlib_u8 *dp = dl;
	mlib_s32 j;

	for (j = 0; j < height; j++) {
		mlib_u8 *dend = dl + width;
		mlib_u32 *sp2;

		while (((mlib_addr)sp & 7) > 1) {
			(*dp++) = *sp;
			sp += 2;

			if (dp >= dend)
				break;
		}

		if ((mlib_addr)sp & 7) {
			sp2 = (mlib_u32 *)(sp - 1);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; dp <= (dend - 2); dp += 2) {
				mlib_u32 s0;

				s0 = (*sp2++);
				dp[0] = s0 >> 8;
				dp[1] = s0 >> 24;
			}

			if (dp < dend) {
				dp[0] = sp2[0] >> 8;
			}
		} else {
			sp2 = (mlib_u32 *)sp;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; dp <= (dend - 2); dp += 2) {
				mlib_u32 s0;

				s0 = (*sp2++);
				dp[0] = s0;
				dp[1] = s0 >> 16;
			}

			if (dp < dend) {
				dp[0] = sp2[0];
			}
		}

		sp = (void *)(sl += slb);
		dp = dl += dlb;
	}
}

/* *********************************************************** */

void
mlib_s_ImageChannelExtract_U8_3_2(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	mlib_s32 j, n1;
	mlib_u8 *dp, *dl;
	mlib_u8 *sp, *sl;

	sp = sl = (mlib_u8 *)src;
	dp = dl = (mlib_u8 *)dst;

	if (cmask == 6) {
		for (j = 0; j < ysize; j++) {
			n1 = xsize;

			for (; n1 > 4; n1 -= 4) {
				*((mlib_s16 *)(dp + 0)) =
				    *((mlib_s16 *)(sp + 0));
				*((mlib_s16 *)(dp + 2)) =
				    *((mlib_s16 *)(sp + 3));
				*((mlib_s16 *)(dp + 4)) =
				    *((mlib_s16 *)(sp + 6));
				*((mlib_s16 *)(dp + 6)) =
				    *((mlib_s16 *)(sp + 9));
				dp += 8;
				sp += 12;
			}

			for (; n1 > 0; n1--) {
				*((mlib_s16 *)(dp + 0)) =
				    *((mlib_s16 *)(sp + 0));
				dp += 2;
				sp += 3;
			}

			sp = sl = (mlib_u8 *)((mlib_u8 *)sl + slb);
			dp = dl = (mlib_u8 *)((mlib_u8 *)dl + dlb);
		}
	} else if (cmask == 3) {
		for (j = 0; j < ysize; j++) {
			n1 = xsize;

			for (; n1 > 4; n1 -= 4) {
				*((mlib_s16 *)(dp + 0)) =
				    *((mlib_s16 *)(sp + 1));
				*((mlib_s16 *)(dp + 2)) =
				    *((mlib_s16 *)(sp + 4));
				*((mlib_s16 *)(dp + 4)) =
				    *((mlib_s16 *)(sp + 7));
				*((mlib_s16 *)(dp + 6)) =
				    *((mlib_s16 *)(sp + 10));
				dp += 8;
				sp += 12;
			}

			for (; n1 > 0; n1--) {
				*((mlib_s16 *)(dp + 0)) =
				    *((mlib_s16 *)(sp + 1));
				dp += 2;
				sp += 3;
			}

			sp = sl = (mlib_u8 *)((mlib_u8 *)sl + slb);
			dp = dl = (mlib_u8 *)((mlib_u8 *)dl + dlb);
		}
	} else {
		if (xsize > 0) {
			for (j = 0; j < ysize; j++) {
				n1 = xsize - 1;

				*((mlib_u8 *)(dp + 0)) =
				    *((mlib_u8 *)(sp + 0));

				dp += 1;
				sp += 2;

				for (; n1 > 4; n1 -= 4) {
					*((mlib_s16 *)(dp + 0)) =
					    *((mlib_s16 *)(sp + 0));
					*((mlib_s16 *)(dp + 2)) =
					    *((mlib_s16 *)(sp + 3));
					*((mlib_s16 *)(dp + 4)) =
					    *((mlib_s16 *)(sp + 6));
					*((mlib_s16 *)(dp + 6)) =
					    *((mlib_s16 *)(sp + 9));
					dp += 8;
					sp += 12;
				}

				for (; n1 > 0; n1--) {
					*((mlib_s16 *)(dp + 0)) =
					    *((mlib_s16 *)(sp + 0));
					dp += 2;
					sp += 3;
				}

				*((mlib_u8 *)(dp + 0)) =
				    *((mlib_u8 *)(sp + 0));

				sp = sl =
				    (mlib_u8 *)((mlib_u8 *)sl + slb);
				dp = dl =
				    (mlib_u8 *)((mlib_u8 *)dl + dlb);
			}
		}
	}
}

/* *********************************************************** */

void
mlib_s_ImageChannelExtract_U8_4_3(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	mlib_s32 *sp, *sl;
	mlib_s32 j, n1;
	mlib_u8 *dp, *dl;

	sp = sl = (mlib_s32 *)src;
	dp = dl = (mlib_u8 *)dst;

	if (cmask == 11) {
		for (j = 0; j < ysize; j++) {
			n1 = xsize;

			for (; n1 > 4; n1 -= 4) {
				*((mlib_s32 *)(dp + 0)) =
				    (sp[0] & 0x000000ff) | ((sp[0] & 0xffff0000)
				    >> 8);
				*((mlib_s32 *)(dp + 3)) =
				    (sp[1] & 0x000000ff) | ((sp[1] & 0xffff0000)
				    >> 8);
				*((mlib_s32 *)(dp + 6)) =
				    (sp[2] & 0x000000ff) | ((sp[2] & 0xffff0000)
				    >> 8);
				*((mlib_s32 *)(dp + 9)) =
				    (sp[3] & 0x000000ff) | ((sp[3] & 0xffff0000)
				    >> 8);
				dp += 12;
				sp += 4;
			}

			for (; n1 > 0; n1--) {
				*(dp + 0) = *((mlib_u8 *)sp + 0);
				*(dp + 1) = *((mlib_u8 *)sp + 2);
				*(dp + 2) = *((mlib_u8 *)sp + 3);
				dp += 3;
				sp += 1;
			}

			sp = sl = (mlib_s32 *)((mlib_u8 *)sl + slb);
			dp = dl = (mlib_u8 *)((mlib_u8 *)dl + dlb);
		}
	} else {
		for (j = 0; j < ysize; j++) {
			n1 = xsize;

			for (; n1 > 4; n1 -= 4) {
				*((mlib_s32 *)(dp + 0)) =
				    (sp[0] & 0x0000ffff) | ((sp[0] & 0xff000000)
				    >> 8);
				*((mlib_s32 *)(dp + 3)) =
				    (sp[1] & 0x0000ffff) | ((sp[1] & 0xff000000)
				    >> 8);
				*((mlib_s32 *)(dp + 6)) =
				    (sp[2] & 0x0000ffff) | ((sp[2] & 0xff000000)
				    >> 8);
				*((mlib_s32 *)(dp + 9)) =
				    (sp[3] & 0x0000ffff) | ((sp[3] & 0xff000000)
				    >> 8);
				dp += 12;
				sp += 4;
			}

			for (; n1 > 0; n1--) {
				*(dp + 0) = *((mlib_u8 *)sp + 0);
				*(dp + 1) = *((mlib_u8 *)sp + 1);
				*(dp + 2) = *((mlib_u8 *)sp + 3);
				dp += 3;
				sp += 1;
			}

			sp = sl = (mlib_s32 *)((mlib_u8 *)sl + slb);
			dp = dl = (mlib_u8 *)((mlib_u8 *)dl + dlb);
		}
	}
}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_S32_21R_4(ld, st)              \
	{                                                  \
		sd0 = _mm_##ld##_si128((void *)sp);            \
		sd1 = _mm_##ld##_si128((void *)(sp + 4));      \
		sd2 = _mm_unpacklo_epi32(sd0, sd1);            \
		sd3 = _mm_unpackhi_epi32(sd0, sd1);            \
		dd0 = _mm_unpacklo_epi32(sd2, sd3);            \
		_mm_##st##_si128((void *)dp, dd0);             \
		sp += 8;                                       \
		dp += 4;                                       \
	}

#define	CHANNEL_EXTRACT_S32_21L_4(ld, st)              \
	{                                                  \
		sd0 = _mm_##ld##_si128((void *)sp);            \
		sd1 = _mm_##ld##_si128((void *)(sp + 4));      \
		sd2 = _mm_unpacklo_epi32(sd0, sd1);            \
		sd3 = _mm_unpackhi_epi32(sd0, sd1);            \
		dd0 = _mm_unpackhi_epi32(sd2, sd3);            \
		_mm_##st##_si128((void *)dp, dd0);             \
		sp += 8;                                       \
		dp += 4;                                       \
	}

#define	CHANNEL_EXTRACT_S32_21R_2                      \
	{                                                  \
		sd0 = _mm_loadl_epi64((void *)sp);             \
		sd1 = _mm_loadl_epi64((void *)(sp + 2));       \
		dd0 = _mm_unpacklo_epi32(sd0, sd1);            \
		_mm_storel_epi64((void *)dp, dd0);             \
		sp += 4;                                       \
		dp += 2;                                       \
	}

#define	CHANNEL_EXTRACT_S32_21L_2                      \
	{                                                  \
		sd0 = _mm_loadl_epi64((void *)(sp + 1));       \
		sd1 = _mm_loadl_epi64((void *)(sp + 3));       \
		dd0 = _mm_unpacklo_epi32(sd0, sd1);            \
		_mm_storel_epi64((void *)dp, dd0);             \
		sp += 4;                                       \
		dp += 2;                                       \
	}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_S32_21R_4A(ld)                 \
	{                                                  \
		sd0 = _mm_##ld##_si128((void *)sp);            \
		sd1 = _mm_##ld##_si128((void *)(sp + 4));      \
		sd2 = _mm_unpacklo_epi32(sd0, sd1);            \
		sd3 = _mm_unpackhi_epi32(sd0, sd1);            \
		dd0 = _mm_unpacklo_epi32(sd2, sd3);            \
		_mm_store_si128((void *)dp, dd0);              \
		sp += 8;                                       \
		dp += 4;                                       \
	}

#define	CHANNEL_EXTRACT_S32_21L_4A(ld)                 \
	{                                                  \
		sd0 = _mm_##ld##_si128((void *)sp);            \
		sd1 = _mm_##ld##_si128((void *)(sp + 4));      \
		sd2 = _mm_unpacklo_epi32(sd0, sd1);            \
		sd3 = _mm_unpackhi_epi32(sd0, sd1);            \
		dd0 = _mm_unpackhi_epi32(sd2, sd3);            \
		_mm_store_si128((void *)dp, dd0);              \
		sp += 8;                                       \
		dp += 4;                                       \
	}

/* *********************************************************** */

/*
 * extract one channel from a 2-channel MLIB_INT image.
 * both source and destination image dpta are 16-byte aligned.
 * both stride is equal to width in bytes.
 */

void
mlib_s_ImageChannelExtract_S32_21_A16D1(
    const mlib_s32 *src,
    mlib_s32 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_s32 *sp, *dp;
	mlib_s32 i;
	__m128i sd0, sd1, sd2, sd3, dd0;

	sp = (mlib_s32 *) src;
	dp = dst;

	if (cmask == 2) {
		for (i = 0; i < dsize - 3; i += 4) {
			CHANNEL_EXTRACT_S32_21R_4(load, store);
		}

		if (i < dsize - 1) {
			CHANNEL_EXTRACT_S32_21R_2;
			i += 2;
		}

		if (i < dsize) {
			dp[0] = sp[0];
		}

	} else {

		for (i = 0; i < dsize - 3; i += 4) {
			CHANNEL_EXTRACT_S32_21L_4(load, store);
		}

		if (i < dsize - 1) {
			CHANNEL_EXTRACT_S32_21L_2;
			i += 2;
		}

		if (i < dsize) {
			dp[0] = sp[1];
		}
	}
}

/* *********************************************************** */

/*
 * extract one channel from a 2-channel MLIB_INT image.
 * both source and destination image dpta are 16-byte aligned.
 * both stride is multiple of 16.
 */

void
mlib_s_ImageChannelExtract_S32_21_A16D2(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	mlib_s32 *sp, *dp;
	mlib_s32 *sl, *dl;
	mlib_s32 i, j;
	__m128i sd0, sd1, sd2, sd3, dd0;

	sp = sl = (mlib_s32 *) src;
	dp = dl = dst;

	if (cmask == 2) {
		for (i = 0; i < ysize; i++) {
			for (j = 0; j < xsize - 3; j += 4) {
				CHANNEL_EXTRACT_S32_21R_4(load, store);
			}

			if (j < xsize - 1) {
				CHANNEL_EXTRACT_S32_21R_2;
				j += 2;
			}

			if (j < xsize) {
				dp[0] = sp[0];
			}

			sp = sl = (mlib_s32 *) ((mlib_u8 *)sl + slb);
			dp = dl = (mlib_s32 *) ((mlib_u8 *)dl + dlb);
		}

	} else {
		for (i = 0; i < ysize; i++) {
			for (j = 0; j < xsize - 3; j += 4) {
				CHANNEL_EXTRACT_S32_21L_4(load, store);
			}

			if (j < xsize - 1) {
				CHANNEL_EXTRACT_S32_21L_2;
				j += 2;
			}

			if (j < xsize) {
				dp[0] = sp[1];
			}

			sp = sl = (mlib_s32 *) ((mlib_u8 *)sl + slb);
			dp = dl = (mlib_s32 *) ((mlib_u8 *)dl + dlb);
		}
	}
}

/* *********************************************************** */

/*
 * extract one channel from a 2-channel MLIB_INT image.
 * not both source and destination image dpta are 16-byte aligned.
 * both stride is equal to xsize in bytes.
 */

void
mlib_s_ImageChannelExtract_S32_21_D1(
    const mlib_s32 *src,
    mlib_s32 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_s32 *sp, *dp;
	mlib_s32 j, k, n1;
	mlib_s32 ns, nmask;
	__m128i sd0, sd1, sd2, sd3, dd0;

	sp = (mlib_s32 *) src;
	dp = dst;

	if (cmask == 1) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
		for (j = 0; j < dsize - 3; j += 4) {
			CHANNEL_EXTRACT_S32_21L_4(loadu, storeu);
		}
		if (j < dsize - 1) {
			CHANNEL_EXTRACT_S32_21L_2;
			j += 2;
		}
		if (j < dsize) {
			dp[0] = sp[1];
		}

	} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
		for (j = 0; j < dsize - 3; j += 4) {
			CHANNEL_EXTRACT_S32_21R_4(loadu, storeu);
		}
		if (j < dsize - 1) {
			CHANNEL_EXTRACT_S32_21R_2;
			j += 2;
		}
		if (j < dsize) {
			dp[0] = sp[0];
		}
	}
}

/* *********************************************************** */

/*
 * extract one channel from a 2-channel MLIB_INT image.
 * not both source and destination image dpta are 16-byte aligned.
 * both stride is not equal to xsize in bytes.
 */

void
mlib_s_ImageChannelExtract_S32_21(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	mlib_s32 *sp, *dp;
	mlib_s32 *sl, *dl;
	mlib_s32 j;

	sp = sl = (mlib_s32 *) src;
	dp = dl = dst;
	for (j = 0; j < ysize; j++) {
	mlib_s_ImageChannelExtract_S32_21_D1(sp, dp, xsize, cmask);
		sp = sl = (mlib_s32 *) ((mlib_u8 *)sl + slb);
		dp = dl = (mlib_s32 *) ((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_s32_41_4(ld, pk1, pk2, pk3, st)\
	{                                                  \
		__m128i sd0, sd1, sd2, sd3, sd4, sd5, dd0;     \
		sd0 = _mm_##ld##_si128((void *)sp);            \
		sd1 = _mm_##ld##_si128((void *)(sp + 4));      \
		sd2 = _mm_##ld##_si128((void *)(sp + 8));      \
		sd3 = _mm_##ld##_si128((void *)(sp + 12));     \
		sd4 = _mm_##pk1##_epi32(sd0, sd1);             \
		sd5 = _mm_##pk2##_epi32(sd2, sd3);             \
		dd0 = _mm_##pk3##_epi64(sd4, sd5);             \
		_mm_##st##_si128((void *)dp, dd0);             \
		sp += 16;                                      \
		dp += 4;                                       \
	}

#define	CHANNEL_EXTRACT_s32_41_2L(ld, pk1)             \
	{                                                  \
		__m128i sd0, sd1, dd0;                         \
		sd0 = _mm_##ld##_si128((void *)sp);            \
		sd1 = _mm_##ld##_si128((void *)(sp + 4));      \
		dd0 = _mm_##pk1##_epi32(sd0, sd1);             \
		_mm_storel_epi64((void *)dp, dd0);             \
		sp += 8;                                       \
		dp += 2;                                       \
	}

#define	CHANNEL_EXTRACT_s32_41_2R(ld, pk1, pk2)        \
	{                                                  \
		__m128i sd0, sd1, sd2, dd0;                    \
		sd0 = _mm_##ld##_si128((void *)sp);            \
		sd1 = _mm_##ld##_si128((void *)(sp + 4));      \
		sd2 = _mm_##pk1##_epi32(sd0, sd1);             \
		dd0 = _mm_##pk2##_epi64(sd2, sd2);             \
		_mm_storel_epi64((void *)dp, dd0);             \
		sp += 8;                                       \
		dp += 2;                                       \
	}

/* *********************************************************** */

void
mlib_s_ImageChannelExtract_S32_41_A16D1(
    const mlib_s32 *src,
    mlib_s32 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_s32 *sp, *dp;
	mlib_s32 i;

	sp = (mlib_s32 *) src;
	dp = dst;

	switch (cmask) {
	case 8:
		{
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
		for (i = dsize; i > 3; i -= 4) {
			CHANNEL_EXTRACT_s32_41_4(load, unpacklo,
			    unpacklo, unpacklo, store);
		}

		if (i > 1) {
			CHANNEL_EXTRACT_s32_41_2L(load, unpacklo);
			i -= 2;
		}

		if (i > 0) {
			dp[0] = sp[0];
			sp += 4;
			dp++;
		}
		}

		break;

	case 4:
		{
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
			for (i = dsize; i > 3; i -= 4) {
				CHANNEL_EXTRACT_s32_41_4(load, unpacklo,
				    unpacklo, unpackhi, store);
			}

			if (i > 1) {
				CHANNEL_EXTRACT_s32_41_2R(
				    load, unpacklo, unpackhi);
				i -= 2;
			}

			if (i > 0) {
				dp[0] = sp[1];
				sp += 4;
				dp++;
			}
		}
		break;
	case 2:
		{
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
			for (i = dsize; i > 3; i -= 4) {
				CHANNEL_EXTRACT_s32_41_4(load, unpackhi,
				    unpackhi, unpacklo, store);
			}

			if (i > 1) {
				CHANNEL_EXTRACT_s32_41_2L(load, unpackhi);
				i -= 2;
			}

			if (i > 0) {
				dp[0] = sp[2];
				sp += 4;
				dp++;
			}
		}
		break;
	case 1:
		{
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
			for (i = dsize; i > 3; i -= 4) {
				CHANNEL_EXTRACT_s32_41_4(load, unpackhi,
				    unpackhi, unpackhi, store);
			}

			if (i > 1) {
				CHANNEL_EXTRACT_s32_41_2R(
				    load, unpackhi, unpackhi);
				i -= 2;
			}

			if (i > 0) {
				dp[0] = sp[3];
				sp += 4;
				dp++;
			}
		}
	}
}

/* *********************************************************** */

void
mlib_s_ImageChannelExtract_S32_41_A16D2(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	mlib_s32 *sp, *dp;
	mlib_s32 *sl, *dl;
	mlib_s32 i, j;

	sp = sl = (mlib_s32 *) src;
	dp = dl = dst;

	switch (cmask) {
	case 8:
		for (i = 0; i < ysize; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
			for (j = xsize; j > 3; j -= 4) {
				CHANNEL_EXTRACT_s32_41_4(load, unpacklo,
				    unpacklo, unpacklo, store);
			}

			if (j > 1) {
				CHANNEL_EXTRACT_s32_41_2L(load, unpacklo);
				j -= 2;
			}

			if (j > 0) {
				dp[0] = sp[0];
				sp += 4;
				dp++;
			}
			sp = sl = (mlib_s32 *) ((mlib_u8 *)sl + slb);
			dp = dl = (mlib_s32 *) ((mlib_u8 *)dl + dlb);
		}

		break;

	case 4:
		for (i = 0; i < ysize; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
			for (j = xsize; j > 3; j -= 4) {
				CHANNEL_EXTRACT_s32_41_4(load, unpacklo,
				    unpacklo, unpackhi, store);
			}

			if (j > 1) {
				CHANNEL_EXTRACT_s32_41_2R(
				    load, unpacklo, unpackhi);
				j -= 2;
			}

			if (j > 0) {
				dp[0] = sp[1];
				sp += 4;
				dp++;
			}
			sp = sl = (mlib_s32 *) ((mlib_u8 *)sl + slb);
			dp = dl = (mlib_s32 *) ((mlib_u8 *)dl + dlb);
		}
		break;
	case 2:
		for (i = 0; i < ysize; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
			for (j = xsize; j > 3; j -= 4) {
				CHANNEL_EXTRACT_s32_41_4(load, unpackhi,
				    unpackhi, unpacklo, store);
			}

			if (j > 1) {
				CHANNEL_EXTRACT_s32_41_2L(load, unpackhi);
				j -= 2;
			}

			if (j > 0) {
				dp[0] = sp[2];
				sp += 4;
				dp++;
			}
			sp = sl = (mlib_s32 *) ((mlib_u8 *)sl + slb);
			dp = dl = (mlib_s32 *) ((mlib_u8 *)dl + dlb);
		}
		break;
	case 1:
		for (i = 0; i < ysize; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
			for (j = xsize; j > 3; j -= 4) {
				CHANNEL_EXTRACT_s32_41_4(load, unpackhi,
				    unpackhi, unpackhi, store);
			}

			if (j > 1) {
				CHANNEL_EXTRACT_s32_41_2R(
				    load, unpackhi, unpackhi);
				j -= 2;
			}

			if (j > 0) {
				dp[0] = sp[3];
				sp += 4;
				dp++;
			}
			sp = sl = (mlib_s32 *) ((mlib_u8 *)sl + slb);
			dp = dl = (mlib_s32 *) ((mlib_u8 *)dl + dlb);
		}
	}
}

/* *********************************************************** */

void
mlib_s_ImageChannelExtract_S32_41_D1(
    const mlib_s32 *src,
    mlib_s32 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_s32 *sp, *dp;
	mlib_s32 i;

	sp = (mlib_s32 *) src;
	dp = dst;

	switch (cmask) {
	case 8:
		{
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
		for (i = dsize; i > 3; i -= 4) {
			CHANNEL_EXTRACT_s32_41_4(loadu, unpacklo,
			    unpacklo, unpacklo, storeu);
		}

		if (i > 1) {
			CHANNEL_EXTRACT_s32_41_2L(loadu, unpacklo);
			i -= 2;
		}

		if (i > 0) {
			dp[0] = sp[0];
			sp += 4;
			dp++;
		}
		}

		break;

	case 4:
		{
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
			for (i = dsize; i > 3; i -= 4) {
				CHANNEL_EXTRACT_s32_41_4(loadu, unpacklo,
				    unpacklo, unpackhi, storeu);
			}

			if (i > 1) {
				CHANNEL_EXTRACT_s32_41_2R(
				    loadu, unpacklo, unpackhi);
				i -= 2;
			}

			if (i > 0) {
				dp[0] = sp[1];
				sp += 4;
				dp++;
			}
		}
		break;
	case 2:
		{
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
			for (i = dsize; i > 3; i -= 4) {
				CHANNEL_EXTRACT_s32_41_4(loadu, unpackhi,
				    unpackhi, unpacklo, storeu);
			}

			if (i > 1) {
				CHANNEL_EXTRACT_s32_41_2L(
				    loadu, unpackhi);
				i -= 2;
			}

			if (i > 0) {
				dp[0] = sp[2];
				sp += 4;
				dp++;
			}
		}
		break;
	case 1:
		{
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
			for (i = dsize; i > 3; i -= 4) {
				CHANNEL_EXTRACT_s32_41_4(loadu, unpackhi,
				    unpackhi, unpackhi, storeu);
			}

			if (i > 1) {
				CHANNEL_EXTRACT_s32_41_2R(
				    loadu, unpackhi, unpackhi);
				i -= 2;
			}

			if (i > 0) {
				dp[0] = sp[3];
				sp += 4;
				dp++;
			}
		}
	}
}

/* *********************************************************** */

void
mlib_s_ImageChannelExtract_S32_41(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	mlib_s32 *sp, *dp;
	mlib_s32 *sl, *dl;
	mlib_s32 i, j;

	sp = sl = (mlib_s32 *) src;
	dp = dl = dst;

	switch (cmask) {
	case 8:
		for (i = 0; i < ysize; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
			for (j = xsize; j > 3; j -= 4) {
				CHANNEL_EXTRACT_s32_41_4(loadu, unpacklo,
				    unpacklo, unpacklo, storeu);
			}

			if (j > 1) {
				CHANNEL_EXTRACT_s32_41_2L(
				    loadu, unpacklo);
				j -= 2;
			}

			if (j > 0) {
				dp[0] = sp[0];
				sp += 4;
				dp++;
			}
			sp = sl = (mlib_s32 *) ((mlib_u8 *)sl + slb);
			dp = dl = (mlib_s32 *) ((mlib_u8 *)dl + dlb);
		}

		break;

	case 4:
		for (i = 0; i < ysize; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
			for (j = xsize; j > 3; j -= 4) {
				CHANNEL_EXTRACT_s32_41_4(loadu, unpacklo,
				    unpacklo, unpackhi, storeu);
			}

			if (j > 1) {
				CHANNEL_EXTRACT_s32_41_2R(
				    loadu, unpacklo, unpackhi);
				j -= 2;
			}

			if (j > 0) {
				dp[0] = sp[1];
				sp += 4;
				dp++;
			}
			sp = sl = (mlib_s32 *) ((mlib_u8 *)sl + slb);
			dp = dl = (mlib_s32 *) ((mlib_u8 *)dl + dlb);
		}
		break;
	case 2:
		for (i = 0; i < ysize; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
			for (j = xsize; j > 3; j -= 4) {
				CHANNEL_EXTRACT_s32_41_4(loadu, unpackhi,
				    unpackhi, unpacklo, storeu);
			}

			if (j > 1) {
				CHANNEL_EXTRACT_s32_41_2L(
				    loadu, unpackhi);
				j -= 2;
			}

			if (j > 0) {
				dp[0] = sp[2];
				sp += 4;
				dp++;
			}
			sp = sl = (mlib_s32 *) ((mlib_u8 *)sl + slb);
			dp = dl = (mlib_s32 *) ((mlib_u8 *)dl + dlb);
		}
		break;
	case 1:
		for (i = 0; i < ysize; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
			for (j = xsize; j > 3; j -= 4) {
				CHANNEL_EXTRACT_s32_41_4(loadu, unpackhi,
				    unpackhi, unpackhi, storeu);
			}

			if (j > 1) {
				CHANNEL_EXTRACT_s32_41_2R(
				    loadu, unpackhi, unpackhi);
				j -= 2;
			}

			if (j > 0) {
				dp[0] = sp[3];
			}
			sp = sl = (mlib_s32 *) ((mlib_u8 *)sl + slb);
			dp = dl = (mlib_s32 *) ((mlib_u8 *)dl + dlb);
		}
	}
}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_32_42_2(ld, pk1, st)           \
	{                                                  \
		__m128i sd0, sd1, dd0;                         \
		sd0 = _mm_##ld##_si128((void *)sp);            \
		sd1 = _mm_##ld##_si128((void *)(sp + 4));      \
		dd0 = _mm_##pk1##_epi64(sd0, sd1);             \
		_mm_##st##_si128((void *)dp, dd0);             \
		sp += 8;                                       \
		dp += 4;                                       \
	}

#define	CHANNEL_EXTRACT_32_42M_2(ld, st)               \
	{                                                  \
		__m128i sd0, sd1, sd2, sd3, dd0;               \
		sd0 = _mm_##ld##_si128((void *)sp);            \
		sd1 = _mm_##ld##_si128((void *)(sp + 4));      \
		sd2 = _mm_srli_si128(sd0, 4);                  \
		sd3 = _mm_srli_si128(sd1, 4);                  \
		dd0 = _mm_unpacklo_epi64(sd2, sd3);            \
		_mm_##st##_si128((void *)dp, dd0);             \
		sp += 8;                                       \
		dp += 4;                                       \
	}

#define	CHANNEL_EXTRACT_32_42B_2(ld, st)               \
	{                                                  \
		__m128i sd0, sd1, sd2, sd3, sd4, sd5, dd0;     \
		sd0 = _mm_##ld##_si128((void *)sp);            \
		sd1 = _mm_##ld##_si128((void *)(sp + 4));      \
		sd2 = _mm_srli_si128(sd0, 12);                 \
		sd3 = _mm_srli_si128(sd1, 12);                 \
		sd4 = _mm_unpacklo_epi32(sd0, sd2);            \
		sd5 = _mm_unpacklo_epi32(sd1, sd3);            \
		dd0 = _mm_unpacklo_epi64(sd4, sd5);            \
		_mm_##st##_si128((void *)dp, dd0);             \
		sp += 8;                                       \
		dp += 4;                                       \
	}

#define	CHANNEL_EXTRACT_32_42_1(tag)                   \
	{                                                  \
		__m128i sd0;                                   \
		sd0 = _mm_loadu_si128((void *)(sp + tag));     \
		_mm_storel_epi64((void *)dp, sd0);             \
		sp += 4;                                       \
		dp += 2;                                       \
	}

/* *********************************************************** */

void
mlib_s_ImageChannelExtract_S32_42_A16D1(
    const mlib_s32 *src,
    mlib_s32 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_s32 *sp, *dp;
	mlib_s32 n1;

	sp = (mlib_s32 *) src;
	dp = dst;

	switch (cmask) {
	case 3:
		{
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
			for (n1 = dsize; n1 > 1; n1 -= 2) {
				CHANNEL_EXTRACT_32_42_2(load, unpackhi, store);
			}

			if (n1 > 0) {
				CHANNEL_EXTRACT_32_42_1(2);
				n1 -= 2;
			}
		}
		break;
	case 6:
		{
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
			for (n1 = dsize; n1 > 1; n1 -= 2) {
				CHANNEL_EXTRACT_32_42M_2(load, store);
			}

			if (n1 > 0) {
				CHANNEL_EXTRACT_32_42_1(1);
				n1 -= 2;
			}
		}

		break;
	case 9:
		{
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
			for (n1 = dsize; n1 > 1; n1 -= 2) {
				CHANNEL_EXTRACT_32_42B_2(load, store);
			}

			if (n1 > 0) {
				dp[0] = sp[0];
				dp[1] = sp[3];
			}
		}

		break;
	case 12:
		{
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
			for (n1 = dsize; n1 > 1; n1 -= 2) {
				CHANNEL_EXTRACT_32_42_2(load, unpacklo, store);
			}

			if (n1 > 0) {
				CHANNEL_EXTRACT_32_42_1(0);
				n1 -= 2;
			}
		}
	}
}

/* *********************************************************** */

void
mlib_s_ImageChannelExtract_S32_42_A16D2(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 cmask)
{
	mlib_s32 *sp, *dp;
	mlib_s32 *sl, *dl;
	mlib_s32 j, n1;

	sp = sl = (mlib_s32 *) src;
	dp = dl = dst;

	switch (cmask) {
	case 3:
		for (j = 0; j < height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
			for (n1 = width; n1 > 1; n1 -= 2) {
				CHANNEL_EXTRACT_32_42_2(load, unpackhi, store);
			}

			if (n1 > 0) {
				CHANNEL_EXTRACT_32_42_1(2);
				n1 -= 2;
			}

			sp = sl = (mlib_s32 *) ((mlib_u8 *)sl + slb);
			dp = dl = (mlib_s32 *) ((mlib_u8 *)dl + dlb);
		}
		break;

	case 6:
		for (j = 0; j < height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
			for (n1 = width; n1 > 1; n1 -= 2) {
				CHANNEL_EXTRACT_32_42M_2(load, store);
			}

			if (n1 > 0) {
				CHANNEL_EXTRACT_32_42_1(1);
				n1 -= 2;
			}

			sp = sl = (mlib_s32 *) ((mlib_u8 *)sl + slb);
			dp = dl = (mlib_s32 *) ((mlib_u8 *)dl + dlb);
		}

		break;
	case 9:
		for (j = 0; j < height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
			for (n1 = width; n1 > 1; n1 -= 2) {
				CHANNEL_EXTRACT_32_42B_2(load, store);
			}

			if (n1 > 0) {
				dp[0] = sp[0];
				dp[1] = sp[3];
			}

			sp = sl = (mlib_s32 *) ((mlib_u8 *)sl + slb);
			dp = dl = (mlib_s32 *) ((mlib_u8 *)dl + dlb);
		}

		break;
	case 12:
		for (j = 0; j < height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
			for (n1 = width; n1 > 1; n1 -= 2) {
				CHANNEL_EXTRACT_32_42_2(load, unpacklo, store);
			}

			if (n1 > 0) {
				CHANNEL_EXTRACT_32_42_1(0);
				n1 -= 2;
			}

			sp = sl = (mlib_s32 *) ((mlib_u8 *)sl + slb);
			dp = dl = (mlib_s32 *) ((mlib_u8 *)dl + dlb);
		}
	}
}

/* *********************************************************** */

void
mlib_s_ImageChannelExtract_S32_42_D1(
    const mlib_s32 *src,
    mlib_s32 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_s32 *sp, *dp;
	mlib_s32 n1;

	sp = (mlib_s32 *) src;
	dp = dst;

	switch (cmask) {
	case 3:
		{
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
			for (n1 = dsize; n1 > 1; n1 -= 2) {
				CHANNEL_EXTRACT_32_42_2(
				    loadu, unpackhi, storeu);
			}

			if (n1 > 0) {
				CHANNEL_EXTRACT_32_42_1(2);
				n1 -= 2;
			}
		}
		break;
	case 6:
		{
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
			for (n1 = dsize; n1 > 1; n1 -= 2) {
				CHANNEL_EXTRACT_32_42M_2(loadu, storeu);
			}

			if (n1 > 0) {
				CHANNEL_EXTRACT_32_42_1(1);
				n1 -= 2;
			}
		}

		break;
	case 9:
		{
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
			for (n1 = dsize; n1 > 1; n1 -= 2) {
				CHANNEL_EXTRACT_32_42B_2(loadu, storeu);
			}

			if (n1 > 0) {
				dp[0] = sp[0];
				dp[1] = sp[3];
			}
		}

		break;
	case 12:
		{
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
			for (n1 = dsize; n1 > 1; n1 -= 2) {
				CHANNEL_EXTRACT_32_42_2(
				    loadu, unpacklo, storeu);
			}

			if (n1 > 0) {
				CHANNEL_EXTRACT_32_42_1(0);
				n1 -= 2;
			}
		}
	}
}

/* *********************************************************** */

void
mlib_s_ImageChannelExtract_S32_42(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 cmask)
{
	mlib_s32 *sp, *dp;
	mlib_s32 *sl, *dl;
	mlib_s32 j, n1;

	sp = sl = (mlib_s32 *) src;
	dp = dl = dst;

	switch (cmask) {
	case 3:
		for (j = 0; j < height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
			for (n1 = width; n1 > 1; n1 -= 2) {
				CHANNEL_EXTRACT_32_42_2(
				    loadu, unpackhi, storeu);
			}

			if (n1 > 0) {
				CHANNEL_EXTRACT_32_42_1(2);
				n1 -= 2;
			}

			sp = sl = (mlib_s32 *) ((mlib_u8 *)sl + slb);
			dp = dl = (mlib_s32 *) ((mlib_u8 *)dl + dlb);
		}
		break;

	case 6:
		for (j = 0; j < height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
			for (n1 = width; n1 > 1; n1 -= 2) {
				CHANNEL_EXTRACT_32_42M_2(loadu, storeu);
			}

			if (n1 > 0) {
				CHANNEL_EXTRACT_32_42_1(1);
				n1 -= 2;
			}

			sp = sl = (mlib_s32 *) ((mlib_u8 *)sl + slb);
			dp = dl = (mlib_s32 *) ((mlib_u8 *)dl + dlb);
		}

		break;
	case 9:
		for (j = 0; j < height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
			for (n1 = width; n1 > 1; n1 -= 2) {
				CHANNEL_EXTRACT_32_42B_2(loadu, storeu);
			}

			if (n1 > 0) {
				dp[0] = sp[0];
				dp[1] = sp[3];
			}

			sp = sl = (mlib_s32 *) ((mlib_u8 *)sl + slb);
			dp = dl = (mlib_s32 *) ((mlib_u8 *)dl + dlb);
		}

		break;
	case 12:
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
		for (j = 0; j < height; j++) {
			for (n1 = width; n1 > 1; n1 -= 2) {
				CHANNEL_EXTRACT_32_42_2(
				    loadu, unpacklo, storeu);
			}

			if (n1 > 0) {
				CHANNEL_EXTRACT_32_42_1(0);
				n1 -= 2;
			}

			sp = sl = (mlib_s32 *) ((mlib_u8 *)sl + slb);
			dp = dl = (mlib_s32 *) ((mlib_u8 *)dl + dlb);
		}
	}
}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_S32_31L(ld)               \
	__m128i sd0, sd1, sd2, sd3, sd4, dd0;         \
	sd0 = _mm_##ld##_si128((void *)sp);           \
	sd1 = _mm_##ld##_si128((void *)(sp + 4));     \
	sd2 = _mm_##ld##_si128((void *)(sp + 8));     \
	sd3 = _mm_unpacklo_epi64(sd0, sd2);           \
	sd4 = _mm_unpackhi_epi64(sd0, sd1);           \
	dd0 = _mm_or_si128(_mm_and_si128(sd3, mask),  \
	    _mm_andnot_si128(mask, sd4));             \
    _mm_storeu_si128((void *)dp, dd0);            \
	sp += 12;                                     \
	dp += 4;

#define	CHANNEL_EXTRACT_S32_31M(ld)               \
	__m128i sd0, sd1, sd2, sd3, sd4, dd0;         \
	sd0 = _mm_##ld##_si128((void *)sp);           \
	sd1 = _mm_##ld##_si128((void *)(sp + 4));     \
	sd2 = _mm_##ld##_si128((void *)(sp + 8));     \
	sd3 = _mm_unpacklo_epi64(sd0, sd1);           \
	sd4 = _mm_unpackhi_epi64(sd1, sd2);           \
	sd3 = _mm_srli_si128(sd3, 4);                 \
	sd4 = _mm_srli_si128(sd4, 4);                 \
	dd0 = _mm_unpacklo_epi64(sd3, sd4);           \
	_mm_storeu_si128((void *)dp, dd0);            \
	sp += 12;                                     \
	dp += 4;

#define	CHANNEL_EXTRACT_S32_31R(ld)               \
	__m128i sd0, sd1, sd2, sd3, sd4, dd0;         \
	sd0 = _mm_##ld##_si128((void *)sp);           \
	sd1 = _mm_##ld##_si128((void *)(sp + 4));     \
	sd2 = _mm_##ld##_si128((void *)(sp + 8));     \
	sd3 = _mm_unpackhi_epi64(sd0, sd2);           \
	sd4 = _mm_unpacklo_epi64(sd1, sd2);           \
	dd0 = _mm_or_si128(_mm_and_si128(sd3, mask),  \
	    _mm_andnot_si128(mask, sd4));             \
	_mm_storeu_si128((void *)dp, dd0);            \
	sp += 12;                                     \
	dp += 4;

/* *********************************************************** */

void
mlib_s_ImageChannelExtract_S32_3_1(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	mlib_s32 *sp, *dp;
	mlib_s32 *sl, *dl;
	mlib_s32 i, j;

	sp = sl = (mlib_s32 *) src;
	dp = dl = dst;
	__m128i mask = _mm_set_epi32(0xFFFFFFFF,
	    0, 0, 0xFFFFFFFF);

	switch (cmask) {
	case 4:
		for (i = 0; i < ysize; i++) {
			if (((mlib_addr)sp & 15) == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = 0; j < xsize - 3; j += 4) {
					CHANNEL_EXTRACT_S32_31L(load);
				}
			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = 0; j < xsize - 3; j += 4) {
					CHANNEL_EXTRACT_S32_31L(loadu);
				}
			}
			for (; j < xsize; j++) {
				dp[0] = sp[0];
				dp++;
				sp += 3;
			}

			sp = sl = (mlib_s32 *) ((mlib_u8 *)sl + slb);
			dp = dl = (mlib_s32 *) ((mlib_u8 *)dl + dlb);
		}

		break;
	case 2:
		for (i = 0; i < ysize; i++) {
			if (((mlib_addr)sp & 15) == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = 0; j < xsize - 3; j += 4) {
					CHANNEL_EXTRACT_S32_31M(load);
				}
			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = 0; j < xsize - 3; j += 4) {
					CHANNEL_EXTRACT_S32_31M(loadu);
				}
			}
			for (; j < xsize; j++) {
				dp[0] = sp[1];
				dp++;
				sp += 3;
			}

			sp = sl = (mlib_s32 *) ((mlib_u8 *)sl + slb);
			dp = dl = (mlib_s32 *) ((mlib_u8 *)dl + dlb);
		}
		break;

	case 1:
		for (i = 0; i < ysize; i++) {
			if (((mlib_addr)sp & 15) == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = 0; j < xsize - 3; j += 4) {
					CHANNEL_EXTRACT_S32_31R(load);
				}
			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = 0; j < xsize - 3; j += 4) {
					CHANNEL_EXTRACT_S32_31R(loadu);
				}
			}
			for (; j < xsize; j++) {
				dp[0] = sp[2];
				dp++;
				sp += 3;
			}

			sp = sl = (mlib_s32 *) ((mlib_u8 *)sl + slb);
			dp = dl = (mlib_s32 *) ((mlib_u8 *)dl + dlb);
		}
	}
}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_S32_32L(ld)              \
	__m128i sd0, sd1, sd2, sd3, dd0, dd1;        \
	sd0 = _mm_##ld##_si128((void *)sp);          \
	sd1 = _mm_##ld##_si128((void *)(sp + 4));    \
	sd2 = _mm_##ld##_si128((void *)(sp + 8));    \
	sd3 = _mm_unpacklo_epi32(                    \
	    _mm_srli_si128(sd0, 12), sd1);           \
	dd0 = _mm_unpacklo_epi64(sd0, sd3);          \
	dd1 = _mm_unpackhi_epi64(sd1,                \
	    _mm_slli_si128(sd2, 4));                 \
    _mm_storeu_si128((void *)dp, dd0);           \
    _mm_storeu_si128((void *)(dp + 4), dd1);     \
	sp += 12;                                    \
	dp += 8

#define	CHANNEL_EXTRACT_S32_32B(ld)              \
	__m128i sd0, sd1, sd2, sd3, sd4, sd5, sd6;   \
	__m128i dd0, dd1;                            \
	__m128i mask = _mm_set_epi32(0xFFFFFFFF,     \
	    0, 0, 0xFFFFFFFF);                       \
	sd0 = _mm_##ld##_si128((void *)sp);          \
	sd1 = _mm_##ld##_si128((void *)(sp + 4));    \
	sd2 = _mm_##ld##_si128((void *)(sp + 8));    \
	sd3 = _mm_unpacklo_epi32(sd0, sd1);          \
	sd4 = _mm_unpackhi_epi32(sd1, sd2);          \
	sd5 = _mm_srli_si128(sd0, 4);                \
	sd6 = _mm_slli_si128(sd2, 4);                \
	dd0 = _mm_or_si128(_mm_and_si128(sd3, mask), \
	    _mm_andnot_si128(mask, sd5));            \
	dd1 = _mm_or_si128(_mm_and_si128(sd4, mask), \
	    _mm_andnot_si128(mask, sd6));            \
    _mm_storeu_si128((void *)dp, dd0);           \
    _mm_storeu_si128((void *)(dp + 4), dd1);     \
	sp += 12;                                    \
	dp += 8

#define	CHANNEL_EXTRACT_S32_32R(ld)              \
	__m128i sd0, sd1, sd2, sd3, dd0, dd1;        \
	sd0 = _mm_##ld##_si128((void *)sp);          \
	sd1 = _mm_##ld##_si128((void *)(sp + 4));    \
	sd2 = _mm_##ld##_si128((void *)(sp + 8));    \
	dd0 = _mm_unpacklo_epi64(                    \
	    _mm_srli_si128(sd0, 4), sd1);            \
	sd3 = _mm_unpackhi_epi32(sd1,                \
	    _mm_slli_si128(sd2, 12)),                \
	dd1 = _mm_unpackhi_epi64(sd3, sd2);          \
    _mm_storeu_si128((void *)dp, dd0);           \
    _mm_storeu_si128((void *)(dp + 4), dd1);     \
	sp += 12;                                    \
	dp += 8

/* *********************************************************** */

void
mlib_s_ImageChannelExtract_S32_3_2(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	mlib_s32 *sp, *dp;
	mlib_s32 *sl, *dl;
	mlib_s32 i, j;

	sp = sl = (mlib_s32 *) src;
	dp = dl = dst;

	switch (cmask) {
	case 6:
		for (i = 0; i < ysize; i++) {
			if (((mlib_addr)sp & 15) == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = 0; j < xsize - 3; j += 4) {
					CHANNEL_EXTRACT_S32_32L(load);
				}
			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = 0; j < xsize - 3; j += 4) {
					CHANNEL_EXTRACT_S32_32L(loadu);
				}
			}
			for (; j < xsize; j++) {
				dp[0] = sp[0];
				dp[1] = sp[1];
				dp += 2;
				sp += 3;
			}

			sp = sl = (mlib_s32 *) ((mlib_u8 *)sl + slb);
			dp = dl = (mlib_s32 *) ((mlib_u8 *)dl + dlb);
		}

		break;
	case 5:
		for (i = 0; i < ysize; i++) {
			if (((mlib_addr)sp & 15) == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = 0; j < xsize - 3; j += 4) {
					CHANNEL_EXTRACT_S32_32B(load);
				}
			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = 0; j < xsize - 3; j += 4) {
					CHANNEL_EXTRACT_S32_32B(loadu);
				}
			}
			for (; j < xsize; j++) {
				dp[0] = sp[0];
				dp[1] = sp[2];
				dp += 2;
				sp += 3;
			}

			sp = sl = (mlib_s32 *) ((mlib_u8 *)sl + slb);
			dp = dl = (mlib_s32 *) ((mlib_u8 *)dl + dlb);
		}
		break;

	case 3:
		for (i = 0; i < ysize; i++) {
			if (((mlib_addr)sp & 15) == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = 0; j < xsize - 3; j += 4) {
					CHANNEL_EXTRACT_S32_32R(load);
				}
			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = 0; j < xsize - 3; j += 4) {
					CHANNEL_EXTRACT_S32_32R(loadu);
				}
			}
			for (; j < xsize; j++) {
				dp[0] = sp[1];
				dp[1] = sp[2];
				dp += 2;
				sp += 3;
			}

			sp = sl = (mlib_s32 *) ((mlib_u8 *)sl + slb);
			dp = dl = (mlib_s32 *) ((mlib_u8 *)dl + dlb);
		}
	}
}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_S16_43L_2(ld)           \
	__m128i sd0 = _mm_##ld##_si128((void *)sp); \
	_mm_storel_epi64((void *)dp, sd0);          \
	dp += 3;                                    \
	__m128i dd0 = _mm_srli_si128(sd0, 8);       \
	_mm_storel_epi64((void *)dp, dd0);          \
	sp += 8;                                    \
	dp += 3

#define	CHANNEL_EXTRACT_S16_43R_2(ld)           \
	__m128i sd0 = _mm_##ld##_si128((void *)sp); \
	__m128i dd0 = _mm_srli_si128(sd0, 2);       \
	__m128i dd1 = _mm_srli_si128(sd0, 10);      \
	_mm_storel_epi64((void *)dp, dd0);          \
	dp += 3;                                    \
	_mm_storel_epi64((void *)dp, dd1);          \
	sp += 8;                                    \
	dp += 3

#define	CHANNEL_EXTRACT_S16_43BL_2(ld)          \
	__m128i sd0 = _mm_##ld##_si128((void *)sp); \
	_mm_storel_epi64((void *)dp, sd0);          \
	dp += 2;                                    \
	__m128i dd0 = _mm_srli_si128(sd0, 6);       \
	_mm_storel_epi64((void *)dp, dd0);          \
	dp += 3;                                    \
	dp[0] = sp[7];                              \
	sp += 8;                                    \
	dp++

#define	CHANNEL_EXTRACT_S16_43BR_2(ld)          \
	__m128i sd0 = _mm_##ld##_si128((void *)sp); \
	dp[0] = sp[0];                              \
	dp++;                                       \
	__m128i dd0 = _mm_srli_si128(sd0, 4);       \
	_mm_storel_epi64((void *)dp, dd0);          \
	dp += 3;                                    \
	__m128i dd1 = _mm_srli_si128(sd0, 12);      \
	_mm_storel_epi64((void *)dp, dd1);          \
	dp += 2;                                    \
	sp += 8

/* *********************************************************** */

void
mlib_s_ImageChannelExtract_S16_4_3(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	mlib_s16 *sp, *dp;
	mlib_s16 *sl, *dl;
	mlib_s32 i, j;

	sp = sl = (mlib_s16 *) src;
	dp = dl = dst;

	switch (cmask) {
	case 7:
		for (i = 0; i < ysize; i++) {
			if (((mlib_addr)sp & 15) == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = 0; j < xsize - 2; j += 2) {
					CHANNEL_EXTRACT_S16_43R_2(load);
				}

			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = 0; j < xsize - 2; j += 2) {
					CHANNEL_EXTRACT_S16_43R_2(loadu);
				}
			}
			for (; j < xsize; j++) {
				dp[0] = sp[1];
				dp[1] = sp[2];
				dp[2] = sp[3];
				dp += 3;
				sp += 4;
			}
			sp = sl = (mlib_s16 *) ((mlib_u8 *)sl + slb);
			dp = dl = (mlib_s16 *) ((mlib_u8 *)dl + dlb);
		}

		break;

	case 14:
		for (i = 0; i < ysize; i++) {
			if (((mlib_addr)sp & 15) == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = 0; j < xsize - 2; j += 2) {
					CHANNEL_EXTRACT_S16_43L_2(load);
				}

			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = 0; j < xsize - 2; j += 2) {
					CHANNEL_EXTRACT_S16_43L_2(loadu);
				}
			}
			for (; j < xsize; j++) {
				dp[0] = sp[0];
				dp[1] = sp[1];
				dp[2] = sp[2];
				dp += 3;
				sp += 4;
			}

			sp = sl = (mlib_s16 *) ((mlib_u8 *)sl + slb);
			dp = dl = (mlib_s16 *) ((mlib_u8 *)dl + dlb);
		}
		break;
	case 13:
		for (i = 0; i < ysize; i++) {
			if (((mlib_addr)sp & 15) == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = 0; j < xsize - 2; j += 2) {
					CHANNEL_EXTRACT_S16_43BL_2(load);
				}
			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = 0; j < xsize - 2; j += 2) {
					CHANNEL_EXTRACT_S16_43BL_2(loadu);
				}
			}
			for (; j < xsize; j++) {
				dp[0] = sp[0];
				dp[1] = sp[1];
				dp[2] = sp[3];
				dp += 3;
				sp += 4;
			}

			sp = sl = (mlib_s16 *) ((mlib_u8 *)sl + slb);
			dp = dl = (mlib_s16 *) ((mlib_u8 *)dl + dlb);
		}
		break;
	case 11:
		for (i = 0; i < ysize; i++) {

			if (((mlib_addr)sp & 15) == 0) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = 0; j < xsize - 2; j += 2) {
					CHANNEL_EXTRACT_S16_43BR_2(load);
				}
			} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = 0; j < xsize - 2; j += 2) {
					CHANNEL_EXTRACT_S16_43BR_2(loadu);
				}

			}
			for (; j < xsize; j++) {
				dp[0] = sp[0];
				dp[1] = sp[2];
				dp[2] = sp[3];
				dp += 3;
				sp += 4;
			}

			sp = sl = (mlib_s16 *) ((mlib_u8 *)sl + slb);
			dp = dl = (mlib_s16 *) ((mlib_u8 *)dl + dlb);
		}
	}
}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_S32_43R_1(ld)        \
	__m128i sd0, sd1;                        \
	sd0 = _mm_##ld##_si128((void *)sp);      \
	sd1 = _mm_srli_si128(sd0, 4);            \
	_mm_storeu_si128((void *)dp, sd1);       \
	sp += 4;                                 \
	dp += 3

#define	CHANNEL_EXTRACT_S32_43L_1(ld)        \
	__m128i sd0;                             \
	sd0 = _mm_##ld##_si128((void *)sp);      \
	_mm_storeu_si128((void *)dp, sd0);       \
	sp += 4;                                 \
	dp += 3

#define	CHANNEL_EXTRACT_S32_43BR_1(ld)       \
	__m128i sd0, sd1;                        \
	sd0 = _mm_##ld##_si128((void *)sp);      \
	sd1 = _mm_shuffle_epi32(sd0, 0x38);      \
	_mm_storeu_si128((void *)dp, sd1);       \
	sp += 4;                                 \
	dp += 3

#define	CHANNEL_EXTRACT_S32_43BL_1(ld)       \
	__m128i sd0, sd1;                        \
	sd0 = _mm_##ld##_si128((void *)sp);      \
	sd1 = _mm_shuffle_epi32(sd0, 0x34);      \
	_mm_storeu_si128((void *)dp, sd1);       \
	sp += 4;                                 \
	dp += 3

/* *********************************************************** */

void
mlib_s_ImageChannelExtract_S32_4_3(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	mlib_s32 *sp, *dp;
	mlib_s32 *sl, *dl;
	mlib_s32 i, j;

	sp = sl = (mlib_s32 *) src;
	dp = dl = dst;

	switch (cmask) {
	case 7:
		for (i = 0; i < ysize; i++) {
			if (((mlib_addr)sp & 15) == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = xsize; j > 1; j--) {
					CHANNEL_EXTRACT_S32_43R_1(load);
				}
				if (j > 0) {
					__m128i sd0, sd1;
					sd0 = _mm_load_si128((void *)sp);
					sd1 = _mm_srli_si128(sd0, 4);
					_mm_storel_epi64((void *)dp, sd1);
					dp[2] = sp[3];
				}

			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = xsize; j > 1; j--) {
					CHANNEL_EXTRACT_S32_43R_1(loadu);
				}
				if (j > 0) {
					__m128i sd0, sd1;
					sd0 = _mm_loadu_si128((void *)sp);
					sd1 = _mm_srli_si128(sd0, 4);
					_mm_storel_epi64((void *)dp, sd1);
					dp[2] = sp[3];
				}
			}
			sp = sl = (mlib_s32 *) ((mlib_u8 *)sl + slb);
			dp = dl = (mlib_s32 *) ((mlib_u8 *)dl + dlb);
		}

		break;

	case 14:
		for (i = 0; i < ysize; i++) {
			if (((mlib_addr)sp & 15) == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = xsize; j > 1; j--) {
					CHANNEL_EXTRACT_S32_43L_1(load);
				}
				if (j > 0) {
					__m128i sd0;
					sd0 = _mm_loadu_si128((void *)sp);
					_mm_storel_epi64((void *)dp, sd0);
					dp[2] = sp[2];
				}

			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = xsize; j > 1; j--) {
					CHANNEL_EXTRACT_S32_43L_1(loadu);
				}
				if (j > 0) {
					__m128i sd0;
					sd0 = _mm_loadu_si128((void *)sp);
					_mm_storel_epi64((void *)dp, sd0);
					dp[2] = sp[2];
				}
			}

			sp = sl = (mlib_s32 *) ((mlib_u8 *)sl + slb);
			dp = dl = (mlib_s32 *) ((mlib_u8 *)dl + dlb);
		}
		break;
	case 13:
		for (i = 0; i < ysize; i++) {
			if (((mlib_addr)sp & 15) == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = xsize; j > 1; j--) {
					CHANNEL_EXTRACT_S32_43BL_1(load);
				}
			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = xsize; j > 1; j--) {
					CHANNEL_EXTRACT_S32_43BL_1(loadu);
				}
			}
			if (j > 0) {
				__m128i sd0 = _mm_loadl_epi64((void *)sp);
				_mm_storel_epi64((void *)dp, sd0);
				dp[2] = sp[3];
			}

			sp = sl = (mlib_s32 *) ((mlib_u8 *)sl + slb);
			dp = dl = (mlib_s32 *) ((mlib_u8 *)dl + dlb);
		}
		break;
	case 11:
		for (i = 0; i < ysize; i++) {

			if (((mlib_addr)sp & 15) == 0) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = xsize; j > 1; j--) {
					CHANNEL_EXTRACT_S32_43BR_1(load);
				}
			} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = xsize; j > 1; j--) {
					CHANNEL_EXTRACT_S32_43BR_1(loadu);
				}

			}
			if (j > 0) {
				dp[0] = sp[0];
				__m128i sd0 = _mm_loadl_epi64((void *)(sp + 2));
				_mm_storel_epi64((void *)(dp + 1), sd0);
			}

			sp = sl = (mlib_s32 *) ((mlib_u8 *)sl + slb);
			dp = dl = (mlib_s32 *) ((mlib_u8 *)dl + dlb);
		}
	}
}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_F32_31L(ld)         \
	__m128 sd0, sd1, sd2, sd3, sd4, dd0;    \
	sd0 = _mm_##ld##_ps((void *)sp);        \
	sd1 = _mm_##ld##_ps((void *)(sp + 4));  \
	sd2 = _mm_##ld##_ps((void *)(sp + 8));  \
	sd3 = _mm_unpacklo_ps(sd0, sd2);        \
	sd4 = _mm_movehl_ps(sd1, sd0);          \
	dd0 = _mm_or_ps(_mm_and_ps(mask, sd3),  \
	    _mm_andnot_ps(mask, sd4));          \
    _mm_storeu_ps((void *)dp, dd0);         \
	sp += 12;                               \
	dp += 4;

#define	CHANNEL_EXTRACT_F32_31M(ld)         \
	__m128 sd0, sd1, sd2, sd3, sd4, dd0;    \
	sd0 = _mm_##ld##_ps((void *)sp);        \
	sd1 = _mm_##ld##_ps((void *)(sp + 4));  \
	sd2 = _mm_##ld##_ps((void *)(sp + 8));  \
	sd3 = _mm_unpacklo_ps(sd0, sd1);        \
	sd4 = _mm_unpackhi_ps(sd1, sd2);        \
	dd0 = _mm_shuffle_ps(sd3, sd4, 0x66);   \
	_mm_storeu_ps((void *)dp, dd0);         \
	sp += 12;                               \
	dp += 4;

#define	CHANNEL_EXTRACT_F32_31R(ld)         \
	__m128 sd0, sd1, sd2, sd3, sd4, dd0;    \
	sd0 = _mm_##ld##_ps(sp);                \
	sd1 = _mm_##ld##_ps(sp + 4);            \
	sd2 = _mm_##ld##_ps(sp + 8);            \
	sd3 = _mm_movelh_ps(sd1, sd2);          \
	sd4 = _mm_unpackhi_ps(sd0, sd2);        \
	dd0 = _mm_or_ps(_mm_and_ps(mask, sd4),  \
	    _mm_andnot_ps(mask, sd3));          \
	_mm_storeu_ps((void *)dp, dd0);         \
	sp += 12;                               \
	dp += 4;

/* *********************************************************** */

void
mlib_s_ImageChannelExtract_F32_3_1(
    const mlib_f32 *src,
    mlib_s32 slb,
    mlib_f32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	mlib_f32 *sp, *dp;
	mlib_f32 *sl, *dl;
	mlib_s32 i, j;

	sp = sl = (mlib_f32 *) src;
	dp = dl = dst;

	unsigned int im = 0xFFFFFFFF;
	mlib_f32 m = *((mlib_f32 *)&im);
	__m128 mask = _mm_set_ps(m, 0, 0, m);

	switch (cmask) {
	case 4:
		for (i = 0; i < ysize; i++) {
			if (((mlib_addr)sp & 15) == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = 0; j < xsize - 3; j += 4) {
					CHANNEL_EXTRACT_F32_31L(load);
				}
			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = 0; j < xsize - 3; j += 4) {
					CHANNEL_EXTRACT_F32_31L(loadu);
				}
			}
			for (; j < xsize; j++) {
				dp[0] = sp[0];
				dp++;
				sp += 3;
			}

			sp = sl = sl + slb;
			dp = dl = dl + dlb;
		}

		break;
	case 2:
		for (i = 0; i < ysize; i++) {
			if (((mlib_addr)sp & 15) == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = 0; j < xsize - 3; j += 4) {
					CHANNEL_EXTRACT_F32_31M(load);
				}
			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = 0; j < xsize - 3; j += 4) {
					CHANNEL_EXTRACT_F32_31M(loadu);
				}
			}
			for (; j < xsize; j++) {
				dp[0] = sp[1];
				dp++;
				sp += 3;
			}

			sp = sl = sl + slb;
			dp = dl = dl + dlb;
		}
		break;

	case 1:

		for (i = 0; i < ysize; i++) {
			if (((mlib_addr)sp & 15) == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = 0; j < xsize - 3; j += 4) {
					CHANNEL_EXTRACT_F32_31R(load);
				}
			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = 0; j < xsize - 3; j += 4) {
					CHANNEL_EXTRACT_F32_31R(loadu);
				}
			}
			for (; j < xsize; j++) {
				dp[0] = sp[2];
				dp++;
				sp += 3;
			}

			sp = sl = sl + slb;
			dp = dl = dl + dlb;
		}
	}
}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_F32_32L(ld)         \
	__m128 sd0, sd1, sd2, sd3, sd4;         \
	__m128 dd0, dd1;                        \
	sd0 = _mm_##ld##_ps(sp);                \
	sd1 = _mm_##ld##_ps((sp + 4));          \
	sd2 = _mm_##ld##_ps((sp + 8));          \
	dd1 = _mm_shuffle_ps(sd1, sd2, 0x9E);   \
	sd3 = _mm_movelh_ps(sd2, sd1);          \
	sd4 = _mm_movehl_ps(sd3, sd0);          \
	dd0 = _mm_shuffle_ps(sd0, sd4, 0x94);   \
    _mm_storeu_ps(dp, dd0);                 \
    _mm_storeu_ps((dp + 4), dd1);           \
	sp += 12;                               \
	dp += 8

#define	CHANNEL_EXTRACT_F32_32B(ld)         \
	__m128 sd0, sd1, sd2, sd3, sd4;         \
	__m128 dd0, dd1;                        \
	sd0 = _mm_##ld##_ps(sp);                \
	sd1 = _mm_##ld##_ps((sp + 4));          \
	sd2 = _mm_##ld##_ps((sp + 8));          \
	sd3 = _mm_movehl_ps(sd1, sd0);          \
	sd4 = _mm_movelh_ps(sd3, sd1);          \
	dd0 = _mm_shuffle_ps(sd0, sd4, 0xD8);   \
	sd3 = _mm_movehl_ps(sd2, sd1);          \
	sd4 = _mm_movelh_ps(sd3, sd2);          \
	dd1 = _mm_shuffle_ps(sd4, sd2, 0xD8);   \
    _mm_storeu_ps(dp, dd0);                 \
    _mm_storeu_ps((dp + 4), dd1);           \
	sp += 12;                               \
	dp += 8

#define	CHANNEL_EXTRACT_F32_32R(ld)         \
	__m128 sd0, sd1, sd2, sd3, sd4;         \
	__m128 dd0, dd1;                        \
	sd0 = _mm_##ld##_ps(sp);                \
	sd1 = _mm_##ld##_ps((sp + 4));          \
	sd2 = _mm_##ld##_ps((sp + 8));          \
	dd0 = _mm_shuffle_ps(sd0, sd1, 0x49);   \
	sd3 = _mm_movehl_ps(sd2, sd1);          \
	sd4 = _mm_movelh_ps(sd3, sd2);          \
	dd1 = _mm_shuffle_ps(sd4, sd2, 0xE9);   \
    _mm_storeu_ps(dp, dd0);                 \
    _mm_storeu_ps((dp + 4), dd1);           \
	sp += 12;                               \
	dp += 8

/* *********************************************************** */

void
mlib_s_ImageChannelExtract_F32_3_2(
    const mlib_f32 *src,
    mlib_s32 slb,
    mlib_f32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	mlib_f32 *sp, *dp;
	mlib_f32 *sl, *dl;
	mlib_s32 i, j;

	sp = sl = (mlib_f32 *) src;
	dp = dl = dst;

	switch (cmask) {
	case 6:
		for (i = 0; i < ysize; i++) {
			if (((mlib_addr)sp & 15) == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = 0; j < xsize - 3; j += 4) {
					CHANNEL_EXTRACT_F32_32L(load);
				}
			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = 0; j < xsize - 3; j += 4) {
					CHANNEL_EXTRACT_F32_32L(loadu);
				}
			}
			for (; j < xsize; j++) {
				dp[0] = sp[0];
				dp[1] = sp[1];
				dp += 2;
				sp += 3;
			}

			sp = sl = sl + slb;
			dp = dl = dl + dlb;
		}

		break;
	case 5:
		for (i = 0; i < ysize; i++) {
			if (((mlib_addr)sp & 15) == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = 0; j < xsize - 3; j += 4) {
					CHANNEL_EXTRACT_F32_32B(load);
				}
			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = 0; j < xsize - 3; j += 4) {
					CHANNEL_EXTRACT_F32_32B(loadu);
				}
			}
			for (; j < xsize; j++) {
				dp[0] = sp[0];
				dp[1] = sp[2];
				dp += 2;
				sp += 3;
			}

			sp = sl = sl + slb;
			dp = dl = dl + dlb;
		}
		break;

	case 3:
		for (i = 0; i < ysize; i++) {
			if (((mlib_addr)sp & 15) == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = 0; j < xsize - 3; j += 4) {
					CHANNEL_EXTRACT_F32_32R(load);
				}
			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = 0; j < xsize - 3; j += 4) {
					CHANNEL_EXTRACT_F32_32R(loadu);
				}
			}
			for (; j < xsize; j++) {
				dp[0] = sp[1];
				dp[1] = sp[2];
				dp += 2;
				sp += 3;
			}

			sp = sl = sl + slb;
			dp = dl = dl + dlb;
		}
	}
}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_F32_42L(ld)        \
	__m128 sd0, sd1, dd0;                  \
	sd0 = _mm_##ld##_ps(sp);               \
	sd1 = _mm_##ld##_ps(sp + 4);           \
	dd0 = _mm_movelh_ps(sd0, sd1);         \
	_mm_storeu_ps(dp, dd0);                \
	sp += 8;                               \
	dp += 4

#define	CHANNEL_EXTRACT_F32_42R(ld)        \
	__m128 sd0, sd1, dd0;                  \
	sd0 = _mm_##ld##_ps(sp);               \
	sd1 = _mm_##ld##_ps(sp + 4);           \
	dd0 = _mm_movehl_ps(sd1, sd0);         \
	_mm_storeu_ps(dp, dd0);                \
	sp += 8;                               \
	dp += 4

#define	CHANNEL_EXTRACT_F32_42M(ld)        \
	__m128 sd0, sd1, dd0;                  \
	sd0 = _mm_##ld##_ps(sp);               \
	sd1 = _mm_##ld##_ps(sp + 4);           \
	dd0 = _mm_shuffle_ps(sd0, sd1, 0x99);  \
	_mm_storeu_ps(dp, dd0);                \
	sp += 8;                               \
	dp += 4

#define	CHANNEL_EXTRACT_F32_42B(ld)        \
	__m128 sd0, sd1, dd0;                  \
	sd0 = _mm_##ld##_ps(sp);               \
	sd1 = _mm_##ld##_ps(sp + 4);           \
	dd0 = _mm_shuffle_ps(sd0, sd1, 0xCC);  \
	_mm_storeu_ps(dp, dd0);                \
	sp += 8;                               \
	dp += 4

/* *********************************************************** */

void
mlib_s_ImageChannelExtract_F32_4_2(
    const mlib_f32 *src,
    mlib_s32 slb,
    mlib_f32 *dst,
    mlib_s32 dlb,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 cmask)
{
	mlib_f32 *sp, *dp;
	mlib_f32 *sl, *dl;
	mlib_s32 j, n1;

	sp = sl = (mlib_f32 *) src;
	dp = dl = dst;

	switch (cmask) {
	case 3:
		for (j = 0; j < height; j++) {
			if (((mlib_addr)sp & 15) == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (n1 = width; n1 > 1; n1 -= 2) {
					CHANNEL_EXTRACT_F32_42R(load);
				}
			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (n1 = width; n1 > 1; n1 -= 2) {
					CHANNEL_EXTRACT_F32_42R(loadu);
				}
			}
			if (n1 > 0) {
				dp[0] = sp[2];
				dp[1] = sp[3];
				dp += 2;
				sp += 4;
			}

			sp = sl = sl + slb;
			dp = dl = dl + dlb;
		}
		break;

	case 6:
		for (j = 0; j < height; j++) {
			if (((mlib_addr)sp & 15) == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (n1 = width; n1 > 1; n1 -= 2) {
					CHANNEL_EXTRACT_F32_42M(load);
				}

			} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (n1 = width; n1 > 1; n1 -= 2) {
					CHANNEL_EXTRACT_F32_42M(loadu);
				}
			}
			if (n1 > 0) {
				dp[0] = sp[1];
				dp[1] = sp[2];
				dp += 2;
				sp += 4;
			}

			sp = sl = sl + slb;
			dp = dl = dl + dlb;
		}

		break;
	case 9:
		for (j = 0; j < height; j++) {
			if (((mlib_addr)sp & 15) == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (n1 = width; n1 > 1; n1 -= 2) {
					CHANNEL_EXTRACT_F32_42B(load);
				}
			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (n1 = width; n1 > 1; n1 -= 2) {
					CHANNEL_EXTRACT_F32_42B(loadu);
				}
			}
			if (n1 > 0) {
				dp[0] = sp[0];
				dp[1] = sp[3];
				dp += 2;
				sp += 4;
			}

			sp = sl = sl + slb;
			dp = dl = dl + dlb;
		}

		break;
	case 12:
		for (j = 0; j < height; j++) {
			if (((mlib_addr)sp & 15) == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (n1 = width; n1 > 1; n1 -= 2) {
					CHANNEL_EXTRACT_F32_42L(load);
				}
			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (n1 = width; n1 > 1; n1 -= 2) {
					CHANNEL_EXTRACT_F32_42L(loadu);
				}
			}
			if (n1 > 0) {
				dp[0] = sp[0];
				dp[1] = sp[1];
				dp += 2;
				sp += 4;
			}

			sp = sl = sl + slb;
			dp = dl = dl + dlb;
		}
	}
}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_F32_43R_1(ld)        \
	__m128 sd0, sd1;                         \
	sd0 = _mm_##ld##_ps(sp);                 \
	/* 0x39 = _MM_SHUFFLE(01, 10, 11, 00) */ \
	sd1 = _mm_shuffle_ps(sd0, sd0, 0x39);    \
	_mm_storeu_ps(dp, sd1);                  \
	sp += 4;                                 \
	dp += 3

#define	CHANNEL_EXTRACT_F32_43L_1(ld)        \
	__m128 sd0;                              \
	sd0 = _mm_##ld##_ps(sp);                 \
	_mm_storeu_ps(dp, sd0);                  \
	sp += 4;                                 \
	dp += 3

#define	CHANNEL_EXTRACT_F32_43BR_1(ld)       \
	__m128 sd0, sd1;                         \
	sd0 = _mm_##ld##_ps(sp);                 \
	sd1 = _mm_shuffle_ps(sd0, sd0, 0x38);    \
	_mm_storeu_ps(dp, sd1);                  \
	sp += 4;                                 \
	dp += 3

#define	CHANNEL_EXTRACT_F32_43BL_1(ld)       \
	__m128 sd0, sd1;                         \
	sd0 = _mm_##ld##_ps(sp);                 \
	sd1 = _mm_shuffle_ps(sd0, sd0, 0x34);    \
	_mm_storeu_ps(dp, sd1);                  \
	sp += 4;                                 \
	dp += 3

/* *********************************************************** */

void
mlib_s_ImageChannelExtract_F32_4_3(
    const mlib_f32 *src,
    mlib_s32 slb,
    mlib_f32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	mlib_f32 *sp, *dp;
	mlib_f32 *sl, *dl;
	mlib_s32 i, j;

	sp = sl = (mlib_f32 *) src;
	dp = dl = dst;

	switch (cmask) {
	case 7:
		for (i = 0; i < ysize; i++) {
			if (((mlib_addr)sp & 15) == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = xsize; j > 1; j--) {
					CHANNEL_EXTRACT_F32_43R_1(load);
				}

			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = xsize; j > 1; j--) {
					CHANNEL_EXTRACT_F32_43R_1(loadu);
				}
			}
			if (j > 0) {
				dp[0] = sp[1];
				dp[1] = sp[2];
				dp[2] = sp[3];
			}
			sp = sl = sl + slb;
			dp = dl = dl + dlb;
		}

		break;

	case 14:
		for (i = 0; i < ysize; i++) {
			if (((mlib_addr)sp & 15) == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = xsize; j > 1; j--) {
					CHANNEL_EXTRACT_F32_43L_1(load);
				}
			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = xsize; j > 1; j--) {
					CHANNEL_EXTRACT_F32_43L_1(loadu);
				}
			}
			if (j > 0) {
				dp[0] = sp[0];
				dp[1] = sp[1];
				dp[2] = sp[2];
			}

			sp = sl = sl + slb;
			dp = dl = dl + dlb;
		}
		break;
	case 13:
		for (i = 0; i < ysize; i++) {
			if (((mlib_addr)sp & 15) == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = xsize; j > 1; j--) {
					CHANNEL_EXTRACT_F32_43BL_1(load);
				}
			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = xsize; j > 1; j--) {
					CHANNEL_EXTRACT_F32_43BL_1(loadu);
				}
			}
			if (j > 0) {
				dp[0] = sp[0];
				dp[1] = sp[1];
				dp[2] = sp[3];
			}

			sp = sl = sl + slb;
			dp = dl = dl + dlb;
		}
		break;
	case 11:
		for (i = 0; i < ysize; i++) {

			if (((mlib_addr)sp & 15) == 0) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = xsize; j > 1; j--) {
					CHANNEL_EXTRACT_F32_43BR_1(load);
				}
			} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = xsize; j > 1; j--) {
					CHANNEL_EXTRACT_F32_43BR_1(loadu);
				}

			}

			if (j > 0) {
				dp[0] = sp[0];
				dp[1] = sp[2];
				dp[2] = sp[3];
			}

			sp = sl = sl + slb;
			dp = dl = dl + dlb;
		}
	}
}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_F32_21R_4(ld, st)   \
	{                                       \
		sd0 = _mm_##ld##_ps(sp);            \
		sd1 = _mm_##ld##_ps((sp + 4));      \
		sd2 = _mm_unpacklo_ps(sd0, sd1);    \
		sd3 = _mm_unpackhi_ps(sd0, sd1);    \
		dd0 = _mm_unpacklo_ps(sd2, sd3);    \
		_mm_##st##_ps(dp, dd0);             \
		sp += 8;                            \
		dp += 4;                            \
	}

#define	CHANNEL_EXTRACT_F32_21L_4(ld, st)   \
	{                                       \
		sd0 = _mm_##ld##_ps(sp);            \
		sd1 = _mm_##ld##_ps((sp + 4));      \
		sd2 = _mm_unpacklo_ps(sd0, sd1);    \
		sd3 = _mm_unpackhi_ps(sd0, sd1);    \
		dd0 = _mm_unpackhi_ps(sd2, sd3);    \
		_mm_##st##_ps(dp, dd0);             \
		sp += 8;                            \
		dp += 4;                            \
	}

#define	CHANNEL_EXTRACT_F32_21R_2           \
	{                                       \
		sd0 = _mm_loadl_ps(sp);             \
		sd1 = _mm_loadl_ps((sp + 2));       \
		dd0 = _mm_unpacklo_ps(sd0, sd1);    \
		_mm_storel_ps(dp, dd0);             \
		sp += 4;                            \
		dp += 2;                            \
	}

#define	CHANNEL_EXTRACT_F32_21L_2           \
	{                                       \
		sd0 = _mm_loadl_ps((sp + 1));       \
		sd1 = _mm_loadl_ps((sp + 3));       \
		dd0 = _mm_unpacklo_ps(sd0, sd1);    \
		_mm_storel_ps(dp, dd0);             \
		sp += 4;                            \
		dp += 2;                            \
	}

/* *********************************************************** */

/*
 * extract one channel from a 2-channel MLIB_INT image.
 * both source and destination image dpta are 16-byte aligned.
 * both stride is equal to width in bytes.
 */

void
mlib_s_ImageChannelExtract_F32_21_A16D1(
    const mlib_f32 *src,
    mlib_f32 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_f32 *sp, *dp;
	mlib_s32 i;
	__m128 sd0, sd1, sd2, sd3, dd0;

	sp = (mlib_f32 *) src;
	dp = dst;

	if (cmask == 2) {
		for (i = 0; i < dsize - 3; i += 4) {
			CHANNEL_EXTRACT_F32_21R_4(load, store);
		}

		for (; i < dsize; i++) {
			dp[0] = sp[0];
			sp += 2;
			dp++;
		}

	} else {

		for (i = 0; i < dsize - 3; i += 4) {
			CHANNEL_EXTRACT_F32_21L_4(load, store);
		}

		for (; i < dsize; i++) {
			dp[0] = sp[1];
			sp += 2;
			dp++;
		}
	}
}

/* *********************************************************** */

/*
 * extract one channel from a 2-channel MLIB_FLOAT image.
 * both source and destination image dpta are 16-byte aligned.
 * both stride is multiple of 16.
 */

void
mlib_s_ImageChannelExtract_F32_21_A16D2(
    const mlib_f32 *src,
    mlib_s32 slb,
    mlib_f32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	mlib_f32 *sp, *dp;
	mlib_f32 *sl, *dl;
	mlib_s32 i, j;
	__m128 sd0, sd1, sd2, sd3, dd0;

	sp = sl = (mlib_f32 *) src;
	dp = dl = dst;

	if (cmask == 2) {
		for (i = 0; i < ysize; i++) {
			for (j = 0; j < xsize - 3; j += 4) {
				CHANNEL_EXTRACT_F32_21R_4(load, store);
			}

			for (; j < xsize; j++) {
				dp[0] = sp[0];
				sp += 2;
				dp++;
			}

			sp = sl = sl + slb;
			dp = dl = dl + dlb;
		}

	} else {
		for (i = 0; i < ysize; i++) {
			for (j = 0; j < xsize - 3; j += 4) {
				CHANNEL_EXTRACT_F32_21L_4(load, store);
			}

			for (; j < xsize; j++) {
				dp[0] = sp[1];
				sp += 2;
				dp++;
			}

			sp = sl = sl + slb;
			dp = dl = dl + dlb;
		}
	}
}

/* *********************************************************** */

/*
 * extract one channel from a 2-channel MLIB_FLOAT image.
 * not both source and destination image dpta are 16-byte aligned.
 * both stride is equal to xsize in bytes.
 */

void
mlib_s_ImageChannelExtract_F32_21_D1(
    const mlib_f32 *src,
    mlib_f32 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_f32 *sp, *dp;
	mlib_s32 j;
	__m128 sd0, sd1, sd2, sd3, dd0;

	sp = (mlib_f32 *) src;
	dp = dst;

	if (cmask == 1) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
		for (j = 0; j < dsize - 3; j += 4) {
			CHANNEL_EXTRACT_F32_21L_4(loadu, storeu);
		}

		for (; j < dsize; j++) {
			dp[0] = sp[1];
			sp += 2;
			dp++;
		}

	} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
		for (j = 0; j < dsize - 3; j += 4) {
			CHANNEL_EXTRACT_F32_21R_4(loadu, storeu);
		}
		for (; j < dsize; j++) {
			dp[0] = sp[0];
			sp += 2;
			dp++;
		}
	}
}

/* *********************************************************** */

/*
 * extract one channel from a 2-channel MLIB_FLOAT image.
 * not both source and destination image dpta are 16-byte aligned.
 * both stride is not equal to xsize in bytes.
 */

void
mlib_s_ImageChannelExtract_F32_21(
    const mlib_f32 *src,
    mlib_s32 slb,
    mlib_f32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	mlib_f32 *sp, *dp;
	mlib_f32 *sl, *dl;
	mlib_s32 i, j, k, n1;
	mlib_s32 ns, nmask;
	__m128 sd0, sd1, sd2, sd3, dd0;

	sp = sl = (mlib_f32 *) src;
	dp = dl = dst;
	for (j = 0; j < ysize; j++) {
		mlib_s_ImageChannelExtract_F32_21_D1(sp, dp, xsize, cmask);
		sp = sl = sl + slb;
		dp = dl = dl + dlb;
	}
}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_f32_41_4(ld, pk1, pk2, pk3, st) \
	{                                                   \
		__m128 sd0, sd1, sd2, sd3, sd4, sd5, dd0;       \
		sd0 = _mm_##ld##_ps(sp);                        \
		sd1 = _mm_##ld##_ps((sp + 4));                  \
		sd2 = _mm_##ld##_ps((sp + 8));                  \
		sd3 = _mm_##ld##_ps((sp + 12));                 \
		sd4 = _mm_##pk1##_ps(sd0, sd2);                 \
		sd5 = _mm_##pk2##_ps(sd1, sd3);                 \
		dd0 = _mm_##pk3##_ps(sd4, sd5);                 \
		_mm_##st##_ps(dp, dd0);                         \
		sp += 16;                                       \
		dp += 4;                                        \
	}

#define	CHANNEL_EXTRACT_f32_41_2L(ld, pk1)             \
	{                                                  \
		__m128 sd0, sd1, dd0;                          \
		sd0 = _mm_##ld##_ps(sp);                       \
		sd1 = _mm_##ld##_ps((sp + 4));                 \
		dd0 = _mm_##pk1##_ps(sd0, sd1);                \
		dp[0] = *((mlib_f32 *)&dd0);                   \
		dp[1] = *(((mlib_f32 *)&dd0) + 1);             \
		sp += 8;                                       \
		dp += 2;                                       \
	}

#define	CHANNEL_EXTRACT_f32_41_2R(ld, pk1, pk2)        \
	{                                                  \
		__m128 sd0, sd1, sd2, dd0;                     \
		sd0 = _mm_##ld##_ps(sp);                       \
		sd1 = _mm_##ld##_ps((sp + 4));                 \
		dd0 = _mm_##pk1##_ps(sd0, sd1);                \
		dp[0] = *(((mlib_f32 *)&dd0) + 2);             \
		dp[1] = *(((mlib_f32 *)&dd0) + 3);             \
		sp += 8;                                       \
		dp += 2;                                       \
	}

/* *********************************************************** */

void
mlib_s_ImageChannelExtract_F32_41_A16D1(
    const mlib_f32 *src,
    mlib_f32 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_f32 *sp, *dp;
	mlib_s32 i;

	sp = (mlib_f32 *) src;
	dp = dst;

	switch (cmask) {
	case 8:
		{
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
		for (i = dsize; i > 3; i -= 4) {
			CHANNEL_EXTRACT_f32_41_4(
			    load, unpacklo, unpacklo, unpacklo, store);
		}

		for (; i > 0; i--) {
			dp[0] = sp[0];
			sp += 4;
			dp++;
		}
		}

		break;

	case 4:
		{
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
			for (i = dsize; i > 3; i -= 4) {
				CHANNEL_EXTRACT_f32_41_4(
				    load, unpacklo, unpacklo, unpackhi, store);
			}

			for (; i > 0; i--) {
				dp[0] = sp[1];
				sp += 4;
				dp++;
			}
		}
		break;
	case 2:
		{
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
			for (i = dsize; i > 3; i -= 4) {
				CHANNEL_EXTRACT_f32_41_4(load, unpackhi,
				    unpackhi, unpacklo, store);
			}

			for (; i > 0; i--) {
				dp[0] = sp[2];
				sp += 4;
				dp++;
			}
		}
		break;
	case 1:
		{
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
			for (i = dsize; i > 3; i -= 4) {
				CHANNEL_EXTRACT_f32_41_4(load, unpackhi,
				    unpackhi, unpackhi, store);
			}

			for (; i > 0; i--) {
				dp[0] = sp[3];
				sp += 4;
				dp++;
			}
		}
	}
}

/* *********************************************************** */

void
mlib_s_ImageChannelExtract_F32_41_A16D2(
    const mlib_f32 *src,
    mlib_s32 slb,
    mlib_f32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	mlib_f32 *sp, *dp;
	mlib_f32 *sl, *dl;
	mlib_s32 i, j;

	sp = sl = (mlib_f32 *) src;
	dp = dl = dst;

	switch (cmask) {
	case 8:
		for (i = 0; i < ysize; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
			for (j = xsize; j > 3; j -= 4) {
				CHANNEL_EXTRACT_f32_41_4(load, unpacklo,
				    unpacklo, unpacklo, store);
			}

			for (j; j > 0; j--) {
				dp[0] = sp[0];
				sp += 4;
				dp++;
			}
			sp = sl = sl + slb;
			dp = dl = dl + dlb;
		}

		break;

	case 4:
		for (i = 0; i < ysize; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
			for (j = xsize; j > 3; j -= 4) {
				CHANNEL_EXTRACT_f32_41_4(load, unpacklo,
				    unpacklo, unpackhi, store);
			}

			for (j; j > 0; j--) {
				dp[0] = sp[1];
				sp += 4;
				dp++;
			}
			sp = sl = sl + slb;
			dp = dl = dl + dlb;
		}
		break;
	case 2:
		for (i = 0; i < ysize; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
			for (j = xsize; j > 3; j -= 4) {
				CHANNEL_EXTRACT_f32_41_4(load, unpackhi,
				    unpackhi, unpacklo, store);
			}

			for (j; j > 0; j--) {
				dp[0] = sp[2];
				sp += 4;
				dp++;
			}
			sp = sl = sl + slb;
			dp = dl = dl + dlb;
		}
		break;
	case 1:
		for (i = 0; i < ysize; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
			for (j = xsize; j > 3; j -= 4) {
				CHANNEL_EXTRACT_f32_41_4(load, unpackhi,
				    unpackhi, unpackhi, store);
			}

			for (j; j > 0; j--) {
				dp[0] = sp[3];
				sp += 4;
				dp++;
			}
			sp = sl = sl + slb;
			dp = dl = dl + dlb;
		}
	}
}

/* *********************************************************** */

void
mlib_s_ImageChannelExtract_F32_41_D1(
    const mlib_f32 *src,
    mlib_f32 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
	mlib_f32 *sp, *dp;
	mlib_s32 i;

	sp = (mlib_f32 *) src;
	dp = dst;

	switch (cmask) {
	case 8:
		{
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
		for (i = dsize; i > 3; i -= 4) {
			CHANNEL_EXTRACT_f32_41_4(loadu, unpacklo,
			    unpacklo, unpacklo, storeu);
		}

		for (; i > 0; i--) {
			dp[0] = sp[0];
			sp += 4;
			dp++;
		}
		}

		break;

	case 4:
		{
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
			for (i = dsize; i > 3; i -= 4) {
				CHANNEL_EXTRACT_f32_41_4(loadu, unpacklo,
					unpacklo, unpackhi, storeu);
			}

			for (; i > 0; i--) {
				dp[0] = sp[1];
				sp += 4;
				dp++;
			}
		}
		break;
	case 2:
		{
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
			for (i = dsize; i > 3; i -= 4) {
				CHANNEL_EXTRACT_f32_41_4(loadu, unpackhi,
				    unpackhi, unpacklo, storeu);
			}

			for (; i > 0; i--) {
				dp[0] = sp[2];
				sp += 4;
				dp++;
			}
		}
		break;
	case 1:
		{
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
			for (i = dsize; i > 3; i -= 4) {
				CHANNEL_EXTRACT_f32_41_4(loadu, unpackhi,
				    unpackhi, unpackhi, storeu);
			}

			for (; i > 0; i--) {
				dp[0] = sp[3];
				sp += 4;
				dp++;
			}
		}
	}
}

/* *********************************************************** */

void
mlib_s_ImageChannelExtract_F32_41(
    const mlib_f32 *src,
    mlib_s32 slb,
    mlib_f32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	mlib_f32 *sp, *dp;
	mlib_f32 *sl, *dl;
	mlib_s32 i, j;

	sp = sl = (mlib_f32 *) src;
	dp = dl = dst;

	switch (cmask) {
	case 8:
		for (i = 0; i < ysize; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
			for (j = xsize; j > 3; j -= 4) {
				CHANNEL_EXTRACT_f32_41_4(loadu, unpacklo,
				    unpacklo, unpacklo, storeu);
			}

			for (; j > 0; j--) {
				dp[0] = sp[0];
				sp += 4;
				dp++;
			}
			sp = sl = sl + slb;
			dp = dl = dl + dlb;
		}

		break;

	case 4:
		for (i = 0; i < ysize; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
			for (j = xsize; j > 3; j -= 4) {
				CHANNEL_EXTRACT_f32_41_4(loadu, unpacklo,
				    unpacklo, unpackhi, storeu);
			}

			for (; j > 0; j--) {
				dp[0] = sp[1];
				sp += 4;
				dp++;
			}
			sp = sl = sl + slb;
			dp = dl = dl + dlb;
		}
		break;
	case 2:
		for (i = 0; i < ysize; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
			for (j = xsize; j > 3; j -= 4) {
				CHANNEL_EXTRACT_f32_41_4(loadu, unpackhi,
				    unpackhi, unpacklo, storeu);
			}

			for (; j > 0; j--) {
				dp[0] = sp[2];
				sp += 4;
				dp++;
			}
			sp = sl = sl + slb;
			dp = dl = dl + dlb;
		}
		break;
	case 1:
		for (i = 0; i < ysize; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
			for (j = xsize; j > 3; j -= 4) {
				CHANNEL_EXTRACT_f32_41_4(
				    loadu, unpackhi, unpackhi,
				    unpackhi, storeu);
			}

			for (; j > 0; j--) {
				dp[0] = sp[3];
				sp += 4;
				dp++;
			}
			sp = sl = sl + slb;
			dp = dl = dl + dlb;
		}
	}
}

/* *********************************************************** */

void
mlib_s_ImageChannelExtract_S16_42(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	mlib_s16 *sp, *dp;
	mlib_s16 *sl, *dl;
	mlib_s32 j, n1;

	sp = sl = (mlib_s16 *) src;
	dp = dl = dst;

	for (j = 0; j < ysize; j++) {
		if (xsize > 1) {
			dp[0] = sp[0];
			sp++;
			dp++;
			mlib_s_ImageChannelExtract_S32_21_D1(
			    (mlib_s32 *)sp, (mlib_s32 *)dp, (xsize - 1), 1);
			sp += (4 * xsize - 2);
			dp += (2 * xsize - 2);
			dp[0] = sp[0];
		} else {
			dp[0] = sp[0];
			dp[1] = sp[3];
		}

		sp = sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

/* general channel extraction: slower due to the inner loop */

void
mlib_s_ImageChannelExtract_S16(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_s32 channels,
    mlib_s32 channeld,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 cmask)
{
/* pointer for pixel in src */
	mlib_u16 *sp;

/* pointer for line in src  */
	mlib_u16 *sl;

/* pointer for pixel in dst */
	mlib_u16 *dp;

/* pointer for line in dst  */
	mlib_u16 *dl;

/* indices for x, y, channel */
	mlib_s32 i, j, k;
	mlib_s32 deltac[5] = { 0, 1, 1, 1, 1 };
	mlib_s32 inc0, inc1, inc2;
	mlib_u16 s0, s1, s2;

	slb >>= 1;
	dlb >>= 1;

	deltac[channeld] = 1;
	for (i = (channels - 1), k = 0; i >= 0; i--) {
		if ((cmask & (1 << i)) == 0)
			deltac[k]++;
		else
			k++;
	}

	deltac[channeld] = channels;
	for (i = 1; i < channeld; i++) {
		deltac[channeld] -= deltac[i];
	}

	sp = sl = (void *)(src + deltac[0]);
	dp = dl = dst;

	if (channeld == 1) {
		for (j = 0; j < height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				s0 = sp[0];
				dp[i] = s0;
				sp += channels;
			}

			sp = sl = sl + slb;
			dp = dl = dl + dlb;
		}
	} else if (channeld == 2) {
		inc0 = deltac[1];
		inc1 = deltac[2] + inc0;
		for (j = 0; j < height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				s0 = sp[0];
				s1 = sp[inc0];
				dp[0] = s0;
				dp[1] = s1;
				sp += inc1;
				dp += 2;
			}

			sp = sl = sl + slb;
			dp = dl = dl + dlb;
		}
	} else if (channeld == 3) {
		inc0 = deltac[1];
		inc1 = deltac[2] + inc0;
		inc2 = deltac[3] + inc1;
		for (j = 0; j < height; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < width; i++) {
				s0 = sp[0];
				s1 = sp[inc0];
				s2 = sp[inc1];
				dp[0] = s0;
				dp[1] = s1;
				dp[2] = s2;
				sp += inc2;
				dp += 3;
			}

			sp = sl = sl + slb;
			dp = dl = dl + dlb;
		}
	}
}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_D64_21R_2(ld)         \
	__m128d sd0, sd1, dd0;                    \
	sd0 = _mm_##ld##_pd(sp);                  \
	sd1 = _mm_##ld##_pd(sp + 2);              \
	dd0 = _mm_unpacklo_pd(sd0, sd1);          \
	_mm_storeu_pd(dp, dd0);                   \
	sp += 4;                                  \
	dp += 2;

#define	CHANNEL_EXTRACT_D64_21L_2(ld)         \
	__m128d sd0, sd1, dd0;                    \
	sd0 = _mm_##ld##_pd(sp);                  \
	sd1 = _mm_##ld##_pd(sp + 2);              \
	dd0 = _mm_unpackhi_pd(sd0, sd1);          \
	_mm_storeu_pd(dp, dd0);                   \
	sp += 4;                                  \
	dp += 2;

#define	CHANNEL_EXTRACT_D64_21R_1(ld)         \
	__m128d sd0 = _mm_##ld##_pd(sp);          \
	_mm_storel_pd(dp, sd0);                   \
	sp += 2;                                  \
	dp++;

#define	CHANNEL_EXTRACT_D64_21L_1(ld)         \
	__m128d sd0 = _mm_##ld##_pd(sp);          \
	_mm_storeh_pd(dp, sd0);                   \
	sp += 2;                                  \
	dp++;

/* *********************************************************** */

void
mlib_s_ImageChannelExtract_D64_2_1(
    const mlib_d64 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	mlib_d64 *sp, *dp;
	mlib_d64 *sl, *dl;
	mlib_s32 i, j;

	sp = sl = (mlib_d64 *) src;
	dp = dl = dst;

	if (cmask == 2) {
		for (i = 0; i < ysize; i++) {
			if (((mlib_addr)sp & 15) == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = xsize; j > 1; j -= 2) {
					CHANNEL_EXTRACT_D64_21R_2(load);
				}

				if (j > 0) {
					CHANNEL_EXTRACT_D64_21R_1(load);
				}

			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = xsize; j > 1; j -= 2) {
					CHANNEL_EXTRACT_D64_21R_2(loadu);
				}
				if (j > 0) {
					CHANNEL_EXTRACT_D64_21R_1(loadu);
				}
			}
			sp = sl += slb;
			dp = dl += dlb;
		}

	} else {
		for (i = 0; i < ysize; i++) {
			if (((mlib_addr)sp & 15) == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = xsize; j > 1; j -= 2) {
					CHANNEL_EXTRACT_D64_21L_2(load);
				}

				if (j > 0) {
					CHANNEL_EXTRACT_D64_21L_1(load);
				}

			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = xsize; j > 1; j -= 2) {
					CHANNEL_EXTRACT_D64_21L_2(loadu);
				}
				if (j > 0) {
					CHANNEL_EXTRACT_D64_21L_1(loadu);
				}
			}
			sp = sl += slb;
			dp = dl += dlb;
		}
	}
}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_D64_31L(ld)     \
	__m128d sd0, sd1, dd0;              \
	sd0 = _mm_##ld##_pd(sp);            \
	sd1 = _mm_##ld##_pd(sp + 2);        \
	dd0 = _mm_move_sd(sd1, sd0);        \
	_mm_storeu_pd(dp, dd0);             \
	sp += 6;                            \
	dp += 2

#define	CHANNEL_EXTRACT_D64_31M(ld)     \
	__m128d sd0, sd1, dd0;              \
	sd0 = _mm_##ld##_pd(sp + 1);        \
	sd1 = _mm_##ld##_pd(sp + 3);        \
	dd0 = _mm_move_sd(sd1, sd0);        \
	_mm_storeu_pd(dp, dd0);             \
	sp += 6;                            \
	dp += 2

#define	CHANNEL_EXTRACT_D64_31R(ld)     \
	__m128d sd0, sd1, dd0;              \
	sd0 = _mm_##ld##_pd(sp + 2);        \
	sd1 = _mm_##ld##_pd(sp + 4);        \
	dd0 = _mm_move_sd(sd1, sd0);        \
	_mm_storeu_pd(dp, dd0);             \
	sp += 6;                            \
	dp += 2

/* *********************************************************** */

void
mlib_s_ImageChannelExtract_D64_3_1(
    const mlib_d64 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	mlib_d64 *sp, *dp;
	mlib_d64 *sl, *dl;
	mlib_s32 i, j;

	sp = sl = (mlib_d64 *) src;
	dp = dl = dst;

	switch (cmask) {
	case 4:
		for (i = 0; i < ysize; i++) {
			if (((mlib_addr)sp & 15) == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = 0; j < xsize - 1; j += 2) {
					CHANNEL_EXTRACT_D64_31L(load);
				}
			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = 0; j < xsize - 1; j += 2) {
					CHANNEL_EXTRACT_D64_31L(loadu);
				}
			}
			if (j < xsize) {
				dp[0] = sp[0];
			}

			sp = sl = sl + slb;
			dp = dl = dl + dlb;
		}

		break;
	case 2:
		for (i = 0; i < ysize; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
			if (((mlib_addr)sp & 15) == 0) {
				for (j = 0; j < xsize - 1; j += 2) {
					CHANNEL_EXTRACT_D64_31M(loadu);
				}
			} else {
				for (j = 0; j < xsize - 1; j += 2) {
					CHANNEL_EXTRACT_D64_31M(load);
				}
			}

			if (j < xsize) {
				dp[0] = sp[1];
			}

			sp = sl = sl + slb;
			dp = dl = dl + dlb;
		}
		break;

	case 1:

		for (i = 0; i < ysize; i++) {
			if (((mlib_addr)sp & 15) == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = 0; j < xsize - 1; j += 2) {
					CHANNEL_EXTRACT_D64_31R(load);
				}
			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = 0; j < xsize - 1; j += 2) {
					CHANNEL_EXTRACT_D64_31R(loadu);
				}
			}
			if (j < xsize) {
				dp[0] = sp[2];
			}

			sp = sl = sl + slb;
			dp = dl = dl + dlb;
		}
	}
}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_D64_32L(ld1, ld2)   \
	__m128d sd0, sd1;                       \
	sd0 = _mm_##ld1##_pd(sp);               \
	_mm_storeu_pd(dp, sd0);                 \
	sd1 = _mm_##ld2##_pd(sp + 3);           \
	_mm_storeu_pd((dp + 2), sd1);           \
	dp += 4;                                \
	sp += 6

#define	CHANNEL_EXTRACT_D64_32R(ld1, ld2)   \
	__m128d sd0, sd1;                       \
	sd0 = _mm_##ld2##_pd(sp + 1);           \
	_mm_storeu_pd(dp, sd0);                 \
	sd1 = _mm_##ld1##_pd(sp + 4);           \
	_mm_storeu_pd((dp + 2), sd1);           \
	dp += 4;                                \
	sp += 6

#define	CHANNEL_EXTRACT_D64_32B(ld)         \
	__m128d sd0, sd1, sd2, sd3, dd0, dd1;   \
	sd0 = _mm_##ld##_pd(sp);                \
	sd1 = _mm_##ld##_pd(sp + 2);            \
	sd2 = _mm_##ld##_pd(sp + 4);            \
	dd0 = _mm_unpacklo_pd(sd0, sd1);        \
	sd3 = _mm_unpackhi_pd(sd0, sd1);        \
	dd1 = _mm_unpackhi_pd(sd3, sd2);        \
	_mm_storeu_pd(dp, dd0);                 \
	_mm_storeu_pd((dp + 2), dd1);           \
	dp += 4;                                \
	sp += 6

/* *********************************************************** */

void
mlib_s_ImageChannelExtract_D64_3_2(
    const mlib_d64 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	mlib_d64 *sp, *dp;
	mlib_d64 *sl, *dl;
	mlib_s32 i, j;

	sp = sl = (mlib_d64 *) src;
	dp = dl = dst;

	switch (cmask) {
	case 6:
		for (i = 0; i < ysize; i++) {
			if (((mlib_addr)sp & 15) == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = 0; j < xsize - 1; j += 2) {
					CHANNEL_EXTRACT_D64_32L(load, loadu);
				}
				if (j < xsize) {
					__m128d sd0 = _mm_load_pd(sp);
					_mm_storeu_pd(dp, sd0);
				}
			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = 0; j < xsize - 1; j += 2) {
					CHANNEL_EXTRACT_D64_32L(loadu, load);
				}
				if (j < xsize) {
					__m128d sd0 = _mm_loadu_pd(sp);
					_mm_storeu_pd(dp, sd0);
				}
			}
			sp = sl = sl + slb;
			dp = dl = dl + dlb;
		}

		break;
	case 5:
		for (i = 0; i < ysize; i++) {
			if (((mlib_addr)sp & 15) == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = 0; j < xsize - 1; j += 2) {
					CHANNEL_EXTRACT_D64_32B(load);
				}
			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = 0; j < xsize - 1; j += 2) {
					CHANNEL_EXTRACT_D64_32B(loadu);
				}
			}
			if (j < xsize) {
				__m128d sd0 = _mm_loadu_pd(sp);
				__m128d sd1 = _mm_loadu_pd(sp + 2);
				__m128d dd0 = _mm_unpacklo_pd(sd0, sd1);
				_mm_storeu_pd(dp, dd0);
			}
			sp = sl = sl + slb;
			dp = dl = dl + dlb;
		}
		break;

	case 3:
		for (i = 0; i < ysize; i++) {
			if (((mlib_addr)sp & 15) == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = 0; j < xsize - 1; j += 2) {
					CHANNEL_EXTRACT_D64_32R(load, loadu);
				}
			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = 0; j < xsize - 1; j += 2) {
					CHANNEL_EXTRACT_D64_32R(loadu, load);
				}
			}
			if (j < xsize) {
				__m128d sd0 = _mm_loadu_pd(sp + 1);
				_mm_storeu_pd(dp, sd0);
			}

			sp = sl = sl + slb;
			dp = dl = dl + dlb;
		}
	}
}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_D64_41(ld, pk, delta) \
	__m128d sd0, sd1, dd0;                    \
	sd0 = _mm_##ld##_pd(sp + delta);          \
	sd1 = _mm_##ld##_pd(sp + delta + 4);      \
	dd0 = _mm_##pk##_pd(sd0, sd1);            \
	_mm_storeu_pd(dp, dd0);                   \
	sp += 8;                                  \
	dp += 2

/* *********************************************************** */

void
mlib_s_ImageChannelExtract_D64_4_1(
    const mlib_d64 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	mlib_d64 *sp, *dp;
	mlib_d64 *sl, *dl;
	mlib_s32 i, j;

	sp = sl = (mlib_d64 *) src;
	dp = dl = dst;

	switch (cmask) {
	case 8:
		for (i = 0; i < ysize; i++) {
			if (((mlib_addr)sp & 15) == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = xsize; j > 1; j -= 2) {
					CHANNEL_EXTRACT_D64_41(
					    load, unpacklo, 0);
				}
			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = xsize; j > 1; j -= 2) {
					CHANNEL_EXTRACT_D64_41(
					    loadu, unpacklo, 0);
				}
			}
			if (j > 0) {
				dp[0] = sp[0];
				sp += 4;
				dp++;
			}

			sp = sl = sl + slb;
			dp = dl = dl + dlb;
		}
		break;

	case 4:
		for (i = 0; i < ysize; i++) {
			if (((mlib_addr)sp & 15) == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = xsize; j > 1; j -= 2) {
					CHANNEL_EXTRACT_D64_41(
					    load, unpackhi, 0);
				}
			} else {
				for (j = xsize; j > 1; j -= 2) {
					CHANNEL_EXTRACT_D64_41(
					    loadu, unpackhi, 0);
				}
			}

			if (j > 0) {
				dp[0] = sp[1];
				sp += 4;
				dp++;
			}
			sp = sl = sl + slb;
			dp = dl = dl + dlb;
		}
		break;
	case 2:
		for (i = 0; i < ysize; i++) {
			if (((mlib_addr)sp & 15) == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = xsize; j > 1; j -= 2) {
					CHANNEL_EXTRACT_D64_41(
					    load, unpacklo, 2);
				}
			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = xsize; j > 1; j -= 2) {
					CHANNEL_EXTRACT_D64_41(
					    loadu, unpacklo, 2);
				}
			}

			if (j > 0) {
				dp[0] = sp[2];
				sp += 4;
				dp++;
			}
			sp = sl = sl + slb;
			dp = dl = dl + dlb;
		}
		break;
	case 1:
		for (i = 0; i < ysize; i++) {
			if (((mlib_addr)sp & 15) == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = xsize; j > 1; j -= 2) {
					CHANNEL_EXTRACT_D64_41(
					    load, unpackhi, 2);
				}
			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = xsize; j > 1; j -= 2) {
					CHANNEL_EXTRACT_D64_41(
					    loadu, unpackhi, 2);
				}
			}

			if (j > 0) {
				dp[0] = sp[3];
				sp += 4;
				dp++;
			}
			sp = sl = sl + slb;
			dp = dl = dl + dlb;
		}
	}
}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_D64_42(ld, delta)     \
	__m128d sd0 = _mm_##ld##_pd(sp + delta);  \
	_mm_storeu_pd(dp, sd0);                   \
	sp += 4;                                  \
	dp += 2

#define	CHANNEL_EXTRACT_D64_42B(ld)           \
	__m128d sd0, sd1, dd0;                    \
	sd0 = _mm_##ld##_pd(sp);                  \
	sd1 = _mm_##ld##_pd(sp + 2);              \
	dd0 = _mm_move_sd(sd1, sd0);              \
	_mm_storeu_pd(dp, dd0);                   \
	sp += 4;                                  \
	dp += 2

/* *********************************************************** */

void
mlib_s_ImageChannelExtract_D64_4_2(
    const mlib_d64 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 cmask)
{
	mlib_d64 *sp, *dp;
	mlib_d64 *sl, *dl;
	mlib_s32 j, n1;

	sp = sl = (mlib_d64 *) src;
	dp = dl = dst;

	switch (cmask) {
	case 12:
		for (j = 0; j < height; j++) {
			if (((mlib_addr)sp & 15) == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (n1 = width; n1 > 0; n1--) {
					CHANNEL_EXTRACT_D64_42(load, 0);
				}
			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (n1 = width; n1 > 0; n1--) {
					CHANNEL_EXTRACT_D64_42(loadu, 0);
				}
			}

			sp = sl = sl + slb;
			dp = dl = dl + dlb;
		}
		break;

	case 6:
		for (j = 0; j < height; j++) {
			if (((mlib_addr)sp & 15) == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (n1 = width; n1 > 0; n1--) {
					CHANNEL_EXTRACT_D64_42(loadu, 1);
				}

			} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (n1 = width; n1 > 0; n1--) {
					CHANNEL_EXTRACT_D64_42(load, 1);
				}
			}

			sp = sl = sl + slb;
			dp = dl = dl + dlb;
		}

		break;
	case 9:
		for (j = 0; j < height; j++) {
			if (((mlib_addr)sp & 15) == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (n1 = width; n1 > 0; n1--) {
					CHANNEL_EXTRACT_D64_42B(load);
				}
			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (n1 = width; n1 > 0; n1--) {
					CHANNEL_EXTRACT_D64_42B(loadu);
				}
			}

			sp = sl = sl + slb;
			dp = dl = dl + dlb;
		}

		break;
	case 3:
		for (j = 0; j < height; j++) {
			if (((mlib_addr)sp & 15) == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (n1 = width; n1 > 0; n1--) {
					CHANNEL_EXTRACT_D64_42(load, 2);
				}
			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (n1 = width; n1 > 0; n1--) {
					CHANNEL_EXTRACT_D64_42(loadu, 2);
				}
			}

			sp = sl = sl + slb;
			dp = dl = dl + dlb;
		}
	}
}

/* *********************************************************** */

#define	CHANNEL_EXTRACT_D64_43L(ld)  \
	__m128d sd0;                     \
	sd0 = _mm_##ld##_pd(sp);         \
	_mm_storeu_pd(dp, sd0);          \
	dp[2] = sp[2];                   \
	sp += 4;                         \
	dp += 3

#define	CHANNEL_EXTRACT_D64_43R(ld)  \
	__m128d sd0;                     \
	dp[0] = sp[1];                   \
	sd0 = _mm_##ld##_pd(sp + 2);     \
	_mm_storeu_pd((dp + 1), sd0);    \
	sp += 4;                         \
	dp += 3

#define	CHANNEL_EXTRACT_D64_43BL(ld) \
	__m128d sd0;                     \
	sd0 = _mm_##ld##_pd(sp);         \
	_mm_storeu_pd(dp, sd0);          \
	dp[2] = sp[3];                   \
	sp += 4;                         \
	dp += 3

#define	CHANNEL_EXTRACT_D64_43BR(ld) \
	__m128d sd0;                     \
	dp[0] = sp[0];                   \
	sd0 = _mm_##ld##_pd(sp + 2);     \
	_mm_storeu_pd((dp + 1), sd0);    \
	sp += 4;                         \
	dp += 3

/* *********************************************************** */

void
mlib_s_ImageChannelExtract_D64_4_3(
    const mlib_d64 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
	mlib_d64 *sp, *dp;
	mlib_d64 *sl, *dl;
	mlib_s32 i, j;

	sp = sl = (mlib_d64 *) src;
	dp = dl = dst;

	switch (cmask) {
	case 7:
		for (i = 0; i < ysize; i++) {
			if (((mlib_addr)sp & 15) == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = xsize; j > 0; j--) {
					CHANNEL_EXTRACT_D64_43R(load);
				}

			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = xsize; j > 0; j--) {
					CHANNEL_EXTRACT_D64_43R(loadu);
				}
			}
			sp = sl = sl + slb;
			dp = dl = dl + dlb;
		}

		break;

	case 14:
		for (i = 0; i < ysize; i++) {
			if (((mlib_addr)sp & 15) == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = xsize; j > 0; j--) {
					CHANNEL_EXTRACT_D64_43L(load);
				}
			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = xsize; j > 0; j--) {
					CHANNEL_EXTRACT_D64_43L(loadu);
				}
			}

			sp = sl = sl + slb;
			dp = dl = dl + dlb;
		}
		break;
	case 13:
		for (i = 0; i < ysize; i++) {
			if (((mlib_addr)sp & 15) == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = xsize; j > 0; j--) {
					CHANNEL_EXTRACT_D64_43BL(load);
				}
			} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = xsize; j > 0; j--) {
					CHANNEL_EXTRACT_D64_43BL(loadu);
				}
			}

			sp = sl = sl + slb;
			dp = dl = dl + dlb;
		}
		break;
	case 11:
		for (i = 0; i < ysize; i++) {

			if (((mlib_addr)sp & 15) == 0) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = xsize; j > 0; j--) {
					CHANNEL_EXTRACT_D64_43BR(load);
				}
			} else {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#pragma unroll(2)
#endif /* __SUNPRO_C */
				for (j = xsize; j > 0; j--) {
					CHANNEL_EXTRACT_D64_43BR(loadu);
				}

			}

			sp = sl = sl + slb;
			dp = dl = dl + dlb;
		}
	}
}

/* *********************************************************** */
