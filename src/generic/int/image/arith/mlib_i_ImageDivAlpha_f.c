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

#pragma ident	"@(#)mlib_i_ImageDivAlpha_f.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageDivAlpha - divide color channels by the alpha channel
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageDivAlpha(mlib_image       *dst,
 *                                     const mlib_image *src,
 *                                     mlib_s32         cmask)
 *
 * ARGUMENTS
 *  dst     Pointer to destination image.
 *  src     Pointer to source image.
 *  cmask   Channel mask to indicate the alpha channel.
 *          Each bit of which represents a channel in the image. The
 *          channel corresponded to the 1 bit is the alpha channel.
 *
 * RESTRICTION
 *      src, and dst must be the same type, the same size and the same
 *      number of channels.
 *      They can have 2, 3 or 4 channels. They can be in MLIB_BYTE,
 *      MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 *
 * DESCRIPTION
 *      Divide color channels by the alpha channel in the way of
 *      pixel by pixel.
 *
 * The image must have at least two channels.
 *
 * For an MLIB_BYTE image
 *      dst->data[i][j] = src->data[i][j] / src->data[i][a] / pow(2, -8)
 * For an MLIB_SHORT image
 *      dst->data[i][j] = src->data[i][j] / src->data[i][a] / pow(2, -15)
 * For an MLIB_USHORT image
 *      dst->data[i][j] = src->data[i][j] / src->data[i][a] / pow(2, -16)
 * For an MLIB_INT image
 *      dst->data[i][j] = src->data[i][j] / src->data[i][a] / pow(2, -31)
 *  where, j != a
 */
#ifndef _NO_LONGLONG

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageDivAlpha_f.h>
#include <mlib_i_ImageArith_Div_Tab_U8.h>

/* *********************************************************** */

#define	SAT_U8(DST, SRC) \
	{	\
		mlib_s64 dd = SRC;	\
	if (dd > MLIB_U8_MAX)	\
		dd = MLIB_U8_MAX;	\
	if (dd < MLIB_U8_MIN)	\
		dd = MLIB_U8_MIN;	\
		DST = (mlib_s32)dd;	\
	}
/* *********************************************************** */

static void mlib_ImageDivAlpha_U8_2(
    mlib_u8 *src,
    mlib_s32 s_stride,
    mlib_u8 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha);

static void mlib_ImageDivAlpha_U8_3(
    mlib_u8 *src,
    mlib_s32 s_stride,
    mlib_u8 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha);

static void mlib_ImageDivAlpha_U8_4(
    mlib_u8 *src,
    mlib_s32 s_stride,
    mlib_u8 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha);

/* *********************************************************** */

mlib_status
mlib_ImageDivAlpha_U8(
    mlib_u8 *src,
    mlib_s32 s_stride,
    mlib_u8 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 nchan,
    mlib_s32 alpha)
{
	switch (nchan) {
	case 2:
		mlib_ImageDivAlpha_U8_2(src, s_stride, dst, d_stride,
		    width, height, alpha);
		return (MLIB_SUCCESS);
	case 3:
		mlib_ImageDivAlpha_U8_3(src, s_stride, dst, d_stride,
		    width, height, alpha);
		return (MLIB_SUCCESS);
	case 4:
		mlib_ImageDivAlpha_U8_4(src, s_stride, dst, d_stride,
		    width, height, alpha);
		return (MLIB_SUCCESS);
	default:
		return (MLIB_FAILURE);
	}
}

/* *********************************************************** */

void
mlib_ImageDivAlpha_U8_2(
    mlib_u8 *src,
    mlib_s32 s_stride,
    mlib_u8 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha)
{
	mlib_s32 i, j;
	mlib_u8 s_0, s_1;
	mlib_s64 d_0, d_1;

	s_stride -= width << 1;
	d_stride -= width << 1;

	if (alpha == 0) {
		for (i = 0; i < height; i++) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width; j++) {

				s_0 = src[0];
				s_1 = src[1];

				d_1 = s_1
					* mlib_ImageArith_DivAlpha_U8[s_0]
					>> 23;
				SAT_U8(dst[1], d_1);

				dst[0] = s_0;

				src += 2;
				dst += 2;
			}

			src += s_stride;
			dst += d_stride;
		}

	} else {
		for (i = 0; i < height; i++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width; j++) {

				s_0 = src[0];
				s_1 = src[1];

				d_0 = s_0
					* mlib_ImageArith_DivAlpha_U8[s_1]
					>> 23;

				SAT_U8(dst[0], d_0);

				dst[1] = s_1;

				src += 2;
				dst += 2;
			}

			src += s_stride;
			dst += d_stride;
		}
	}
}


/* ********************************************************** */

void
mlib_ImageDivAlpha_U8_3(
    mlib_u8 *src,
    mlib_s32 s_stride,
    mlib_u8 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha)
{
	mlib_s32 i, j;
	mlib_u8 s_0, s_1, s_2;
	mlib_s64 d_0, d_1, d_2;

	s_stride -= 3 * width;
	d_stride -= 3 * width;

	if (alpha == 0) {
		for (i = 0; i < height; i++) {
			s_0 = src[0];
			s_1 = src[1];
			s_2 = src[2];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 1; j++) {

				d_1 = s_1
					* mlib_ImageArith_DivAlpha_U8[s_0]
					>> 23;
				d_2 = s_2
					* mlib_ImageArith_DivAlpha_U8[s_0]
					>> 23;

				SAT_U8(dst[1], d_1);
				SAT_U8(dst[2], d_2);

				s_1 = src[4];
				s_2 = src[5];

				dst[0] = s_0;

				s_0 = src[3];

				src += 3;
				dst += 3;
			}

			d_1 = s_1 * mlib_ImageArith_DivAlpha_U8[s_0] >> 23;
			d_2 = s_2 * mlib_ImageArith_DivAlpha_U8[s_0] >> 23;

			SAT_U8(dst[1], d_1);
			SAT_U8(dst[2], d_2);

			dst[0] = s_0;

			src += s_stride + 3;
			dst += d_stride + 3;
		}

	} else if (alpha == 1) {
		for (i = 0; i < height; i++) {
			s_0 = src[0];
			s_1 = src[1];
			s_2 = src[2];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 1; j++) {
				d_0 = s_0
					* mlib_ImageArith_DivAlpha_U8[s_1]
					>> 23;
				d_2 = s_2
					* mlib_ImageArith_DivAlpha_U8[s_1]
					>> 23;

				SAT_U8(dst[0], d_0);
				SAT_U8(dst[2], d_2);

				s_0 = src[3];
				s_2 = src[5];

				dst[1] = s_1;

				s_1 = src[4];

				src += 3;
				dst += 3;
			}

			d_0 = s_0 * mlib_ImageArith_DivAlpha_U8[s_1] >> 23;
			d_2 = s_2 * mlib_ImageArith_DivAlpha_U8[s_1] >> 23;

			SAT_U8(dst[0], d_0);
			SAT_U8(dst[2], d_2);

			dst[1] = s_1;

			src += s_stride + 3;
			dst += d_stride + 3;
		}
	} else {
		for (i = 0; i < height; i++) {
			s_0 = src[0];
			s_1 = src[1];
			s_2 = src[2];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 1; j++) {
				d_0 = s_0
					* mlib_ImageArith_DivAlpha_U8[s_2]
					>> 23;
				d_1 = s_1
					* mlib_ImageArith_DivAlpha_U8[s_2]
					>> 23;

				SAT_U8(dst[0], d_0);
				SAT_U8(dst[1], d_1);

				s_0 = src[3];
				s_1 = src[4];

				dst[2] = s_2;

				s_2 = src[5];

				src += 3;
				dst += 3;
			}

			d_0 = s_0 * mlib_ImageArith_DivAlpha_U8[s_2] >> 23;
			d_1 = s_1 * mlib_ImageArith_DivAlpha_U8[s_2] >> 23;

			SAT_U8(dst[0], d_0);
			SAT_U8(dst[1], d_1);

			dst[2] = s_2;

			src += s_stride + 3;
			dst += d_stride + 3;
		}
	}

}

/* ********************************************************** */

void
mlib_ImageDivAlpha_U8_4(
    mlib_u8 *src,
    mlib_s32 s_stride,
    mlib_u8 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha)
{
	mlib_s32 i, j;
	mlib_s64 d_0, d_1, d_2, d_3;

	s_stride -= 4 * width;
	d_stride -= 4 * width;

	if (alpha == 0) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif  /* __SUNPRO_C */
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {

				d_1 = src[1]
					* mlib_ImageArith_DivAlpha_U8[src[0]]
					>> 23;
				d_2 = src[2]
					* mlib_ImageArith_DivAlpha_U8[src[0]]
					>> 23;
				d_3 = src[3]
					* mlib_ImageArith_DivAlpha_U8[src[0]]
					>> 23;

				SAT_U8(dst[1], d_1);
				SAT_U8(dst[2], d_2);
				SAT_U8(dst[3], d_3);

				dst[0] = src[0];

				src += 4;
				dst += 4;
			}

			src += s_stride;
			dst += d_stride;
		}

	} else if (alpha == 1) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif  /* __SUNPRO_C */
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				d_0 = src[0]
					* mlib_ImageArith_DivAlpha_U8[src[1]]
					>> 23;
				d_2 = src[2]
					* mlib_ImageArith_DivAlpha_U8[src[1]]
					>> 23;
				d_3 = src[3]
					* mlib_ImageArith_DivAlpha_U8[src[1]]
					>> 23;

				SAT_U8(dst[0], d_0);
				SAT_U8(dst[2], d_2);
				SAT_U8(dst[3], d_3);

				dst[1] = src[1];

				src += 4;
				dst += 4;
			}

			src += s_stride;
			dst += d_stride;
		}

	} else if (alpha == 2) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif  /* __SUNPRO_C */
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {

				d_0 = src[0]
					* mlib_ImageArith_DivAlpha_U8[src[2]]
					>> 23;
				d_1 = src[1]
					* mlib_ImageArith_DivAlpha_U8[src[2]]
					>> 23;
				d_3 = src[3]
					* mlib_ImageArith_DivAlpha_U8[src[2]]
					>> 23;

				SAT_U8(dst[0], d_0);
				SAT_U8(dst[1], d_1);
				SAT_U8(dst[3], d_3);

				dst[2] = src[2];

				src += 4;
				dst += 4;
			}

			src += s_stride;
			dst += d_stride;
		}

	} else {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif  /* __SUNPRO_C */

		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				d_0 = src[0]
					* mlib_ImageArith_DivAlpha_U8[src[3]]
					>> 23;
				d_1 = src[1]
					* mlib_ImageArith_DivAlpha_U8[src[3]]
					>> 23;
				d_2 = src[2]
					* mlib_ImageArith_DivAlpha_U8[src[3]]
					>> 23;

				SAT_U8(dst[0], d_0);
				SAT_U8(dst[1], d_1);
				SAT_U8(dst[2], d_2);

				dst[3] = src[3];

				src += 4;
				dst += 4;
			}

			src += s_stride;
			dst += d_stride;
		}
	}
}


/* ********************************************************** */

#else

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageDivAlpha_f.h>
#include <mlib_c_ImageBlendTable.h>

/* *********************************************************** */

const mlib_f32 mlib_div_tab_Q8[] = {
	256.0000000000f, 256.0000000000f, 128.0000000000f, 85.3333358765f,
	64.0000000000f, 51.2000007629f, 42.6666679382f, 36.5714302063f,
	32.0000000000f, 28.4444446564f, 25.6000003815f, 23.2727279663f,
	21.3333339691f, 19.6923084259f, 18.2857151031f, 17.0666675568f,
	16.0000000000f, 15.0588235855f, 14.2222223282f, 13.4736843109f,
	12.8000001907f, 12.1904764175f, 11.6363639832f, 11.1304349899f,
	10.6666669846f, 10.2399997711f, 9.8461542130f, 9.4814815521f,
	9.1428575516f, 8.8275861740f, 8.5333337784f, 8.2580642700f,
	8.0000000000f, 7.7575759888f, 7.5294117928f, 7.3142857552f,
	7.1111111641f, 6.9189190865f, 6.7368421555f, 6.5641026497f,
	6.4000000954f, 6.2439022064f, 6.0952382088f, 5.9534883499f,
	5.8181819916f, 5.6888890266f, 5.5652174950f, 5.4468083382f,
	5.3333334923f, 5.2244896889f, 5.1199998856f, 5.0196080208f,
	4.9230771065f, 4.8301887512f, 4.7407407761f, 4.6545453072f,
	4.5714287758f, 4.4912281036f, 4.4137930870f, 4.3389830589f,
	4.2666668892f, 4.1967210770f, 4.1290321350f, 4.0634922981f,
	4.0000000000f, 3.9384615421f, 3.8787879944f, 3.8208954334f,
	3.7647058964f, 3.7101449966f, 3.6571428776f, 3.6056337357f,
	3.5555555820f, 3.5068492889f, 3.4594595432f, 3.4133334160f,
	3.3684210777f, 3.3246753216f, 3.2820513248f, 3.2405064106f,
	3.2000000477f, 3.1604938507f, 3.1219511032f, 3.0843372345f,
	3.0476191044f, 3.0117647648f, 2.9767441750f, 2.9425287247f,
	2.9090909958f, 2.8764045238f, 2.8444445133f, 2.8131868839f,
	2.7826087475f, 2.7526881695f, 2.7234041691f, 2.6947369576f,
	2.6666667461f, 2.6391751766f, 2.6122448444f, 2.5858585835f,
	2.5599999428f, 2.5346534252f, 2.5098040104f, 2.4854369164f,
	2.4615385532f, 2.4380953312f, 2.4150943756f, 2.3925232887f,
	2.3703703880f, 2.3486237526f, 2.3272726536f, 2.3063063622f,
	2.2857143879f, 2.2654867172f, 2.2456140518f, 2.2260868549f,
	2.2068965435f, 2.1880342960f, 2.1694915295f, 2.1512606144f,
	2.1333334446f, 2.1157023907f, 2.0983605385f, 2.0813007355f,
	2.0645160675f, 2.0480000973f, 2.0317461491f, 2.0157480240f,
	2.0000000000f, 1.9844961166f, 1.9692307711f, 1.9541984797f,
	1.9393939972f, 1.9248120785f, 1.9104477167f, 1.8962962627f,
	1.8823529482f, 1.8686131239f, 1.8550724983f, 1.8417266607f,
	1.8285714388f, 1.8156027794f, 1.8028168678f, 1.7902097702f,
	1.7777777910f, 1.7655172348f, 1.7534246445f, 1.7414965630f,
	1.7297297716f, 1.7181208134f, 1.7066667080f, 1.6953642368f,
	1.6842105389f, 1.6732026339f, 1.6623376608f, 1.6516128778f,
	1.6410256624f, 1.6305732727f, 1.6202532053f, 1.6100628376f,
	1.6000000238f, 1.5900621414f, 1.5802469254f, 1.5705521107f,
	1.5609755516f, 1.5515151024f, 1.5421686172f, 1.5329341888f,
	1.5238095522f, 1.5147929192f, 1.5058823824f, 1.4970760345f,
	1.4883720875f, 1.4797687531f, 1.4712643623f, 1.4628571272f,
	1.4545454979f, 1.4463276863f, 1.4382022619f, 1.4301675558f,
	1.4222222567f, 1.4143646955f, 1.4065934420f, 1.3989070654f,
	1.3913043737f, 1.3837838173f, 1.3763440847f, 1.3689839840f,
	1.3617020845f, 1.3544973135f, 1.3473684788f, 1.3403141499f,
	1.3333333731f, 1.3264248371f, 1.3195875883f, 1.3128205538f,
	1.3061224222f, 1.2994923592f, 1.2929292917f, 1.2864321470f,
	1.2799999714f, 1.2736318111f, 1.2673267126f, 1.2610837221f,
	1.2549020052f, 1.2487804890f, 1.2427184582f, 1.2367149591f,
	1.2307692766f, 1.2248803377f, 1.2190476656f, 1.2132701874f,
	1.2075471878f, 1.2018779516f, 1.1962616444f, 1.1906976700f,
	1.1851851940f, 1.1797235012f, 1.1743118763f, 1.1689497232f,
	1.1636363268f, 1.1583710909f, 1.1531531811f, 1.1479821205f,
	1.1428571939f, 1.1377778053f, 1.1327433586f, 1.1277532578f,
	1.1228070259f, 1.1179039478f, 1.1130434275f, 1.1082251072f,
	1.1034482718f, 1.0987124443f, 1.0940171480f, 1.0893616676f,
	1.0847457647f, 1.0801687241f, 1.0756303072f, 1.0711296797f,
	1.0666667223f, 1.0622407198f, 1.0578511953f, 1.0534979105f,
	1.0491802692f, 1.0448979139f, 1.0406503677f, 1.0364372730f,
	1.0322580338f, 1.0281124115f, 1.0240000486f, 1.0199203491f,
	1.0158730745f, 1.0118577480f, 1.0078740120f, 1.0039216280f
};

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

/* *********************************************************** */

#define	SAT8(DST, SRC)	DST = (mlib_s32) (SRC) >> 23

#else /* MLIB_USE_FTOI_CLAMPING */

#ifdef i386

/* *********************************************************** */

#define	SAT8(DST, SRC)                                          \
	if ((mlib_d64)(SRC) >= (mlib_d64)MLIB_S32_MAX)          \
	    DST = MLIB_U8_MAX;                                  \
	else                                                    \
	    DST = (mlib_s32)(SRC) >> 23

#else /* i386 */

/* *********************************************************** */

#define	SAT8(DST, SRC)                                          \
	if ((SRC) >= MLIB_S32_MAX)                              \
	    DST = MLIB_U8_MAX;                                  \
	else                                                    \
	    DST = (mlib_s32)(SRC) >> 23
#endif /* i386 */
#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#define	FL_MIN	1e - 20F
#define	DB_MIN	1e - 100

/* *********************************************************** */

static void mlib_ImageDivAlpha_U8_2(
    mlib_u8 *src,
    mlib_s32 s_stride,
    mlib_u8 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha);

static void mlib_ImageDivAlpha_U8_3(
    mlib_u8 *src,
    mlib_s32 s_stride,
    mlib_u8 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha);

static void mlib_ImageDivAlpha_U8_4(
    mlib_u8 *src,
    mlib_s32 s_stride,
    mlib_u8 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha);

/* *********************************************************** */

mlib_status
mlib_ImageDivAlpha_U8(
    mlib_u8 *src,
    mlib_s32 s_stride,
    mlib_u8 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 nchan,
    mlib_s32 alpha)
{
	switch (nchan) {
	case 2:
		mlib_ImageDivAlpha_U8_2(src, s_stride, dst, d_stride,
		    width, height, alpha);
		return (MLIB_SUCCESS);
	case 3:
		mlib_ImageDivAlpha_U8_3(src, s_stride, dst, d_stride,
		    width, height, alpha);
		return (MLIB_SUCCESS);
	case 4:
		mlib_ImageDivAlpha_U8_4(src, s_stride, dst, d_stride,
		    width, height, alpha);
		return (MLIB_SUCCESS);
	default:
		return (MLIB_FAILURE);
	}
}

/* *********************************************************** */

void
mlib_ImageDivAlpha_U8_2(
    mlib_u8 *src,
    mlib_s32 s_stride,
    mlib_u8 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha)
{
	mlib_s32 i, j;
	mlib_s32 d_0, d_1;
	mlib_d64 s_0, s_1;

	s_stride -= width << 1;
	d_stride -= width << 1;

	if (alpha == 0) {
		for (i = 0; i < height; i++) {
			d_0 = src[0];
			d_1 = src[1];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 1; j++) {

				s_0 = mlib_div_tab_Q8[d_0];
				s_1 = mlib_c_blend_u8_sat[d_1] * s_0;
				SAT8(d_1, s_1);

				dst[0] = d_0;
				dst[1] = d_1;

				d_0 = src[2];
				d_1 = src[3];

				src += 2;
				dst += 2;
			}


			s_0 = mlib_div_tab_Q8[d_0];
			s_1 = mlib_c_blend_u8_sat[d_1] * s_0;
			SAT8(d_1, s_1);

			dst[0] = d_0;
			dst[1] = d_1;

			src += s_stride + 2;
			dst += d_stride + 2;
		}

	} else {
		for (i = 0; i < height; i++) {
			d_0 = src[0];
			d_1 = src[1];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 1; j++) {

				s_0 = mlib_c_blend_u8_sat[d_0];
				s_1 = mlib_div_tab_Q8[d_1] * s_0;
				SAT8(d_0, s_1);

				dst[0] = d_0;
				dst[1] = d_1;

				d_0 = src[2];
				d_1 = src[3];

				src += 2;
				dst += 2;
			}

			s_0 = mlib_c_blend_u8_sat[d_0];
			s_1 = mlib_div_tab_Q8[d_1] * s_0;
			SAT8(d_0, s_1);

			dst[0] = d_0;
			dst[1] = d_1;

			src += s_stride + 2;
			dst += d_stride + 2;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageDivAlpha_U8_3(
    mlib_u8 *src,
    mlib_s32 s_stride,
    mlib_u8 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha)
{
	mlib_s32 d_0, d_1, d_2;
	mlib_d64 s_0, s_1, s_2;
	mlib_s32 i, j;

	s_stride -= 3 * width;
	d_stride -= 3 * width;

	if (alpha == 0) {
		for (i = 0; i < height; i++) {
			d_0 = src[0];
			d_1 = src[1];
			d_2 = src[2];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 1; j++) {

				s_0 = mlib_div_tab_Q8[d_0];
				s_1 = mlib_c_blend_u8_sat[d_1] * s_0;
				s_2 = mlib_c_blend_u8_sat[d_2] * s_0;
				SAT8(d_1, s_1);
				SAT8(d_2, s_2);

				dst[0] = d_0;
				dst[1] = d_1;
				dst[2] = d_2;

				d_0 = src[3];
				d_1 = src[4];
				d_2 = src[5];

				src += 3;
				dst += 3;
			}

			s_0 = mlib_div_tab_Q8[d_0];
			s_1 = mlib_c_blend_u8_sat[d_1] * s_0;
			s_2 = mlib_c_blend_u8_sat[d_2] * s_0;
			SAT8(d_1, s_1);
			SAT8(d_2, s_2);

			dst[0] = d_0;
			dst[1] = d_1;
			dst[2] = d_2;

			src += s_stride + 3;
			dst += d_stride + 3;
		}

	} else if (alpha == 1) {
		for (i = 0; i < height; i++) {
			d_0 = src[0];
			d_1 = src[1];
			d_2 = src[2];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 1; j++) {

				s_1 = mlib_div_tab_Q8[d_1];
				s_0 = mlib_c_blend_u8_sat[d_0] * s_1;
				s_2 = mlib_c_blend_u8_sat[d_2] * s_1;
				SAT8(d_0, s_0);
				SAT8(d_2, s_2);

				dst[0] = d_0;
				dst[1] = d_1;
				dst[2] = d_2;

				d_0 = src[3];
				d_1 = src[4];
				d_2 = src[5];

				src += 3;
				dst += 3;
			}

			s_1 = mlib_div_tab_Q8[d_1];
			s_0 = mlib_c_blend_u8_sat[d_0] * s_1;
			s_2 = mlib_c_blend_u8_sat[d_2] * s_1;
			SAT8(d_0, s_0);
			SAT8(d_2, s_2);

			dst[0] = d_0;
			dst[1] = d_1;
			dst[2] = d_2;

			src += s_stride + 3;
			dst += d_stride + 3;
		}

	} else {
		for (i = 0; i < height; i++) {
			d_0 = src[0];
			d_1 = src[1];
			d_2 = src[2];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 1; j++) {

				s_2 = mlib_div_tab_Q8[d_2];
				s_0 = mlib_c_blend_u8_sat[d_0] * s_2;
				s_1 = mlib_c_blend_u8_sat[d_1] * s_2;
				SAT8(d_0, s_0);
				SAT8(d_1, s_1);

				dst[0] = d_0;
				dst[1] = d_1;
				dst[2] = d_2;

				d_0 = src[3];
				d_1 = src[4];
				d_2 = src[5];

				src += 3;
				dst += 3;
			}

			s_2 = mlib_div_tab_Q8[d_2];
			s_0 = mlib_c_blend_u8_sat[d_0] * s_2;
			s_1 = mlib_c_blend_u8_sat[d_1] * s_2;
			SAT8(d_0, s_0);
			SAT8(d_1, s_1);

			dst[0] = d_0;
			dst[1] = d_1;
			dst[2] = d_2;

			src += s_stride + 3;
			dst += d_stride + 3;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageDivAlpha_U8_4(
    mlib_u8 *src,
    mlib_s32 s_stride,
    mlib_u8 *dst,
    mlib_s32 d_stride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 alpha)
{
	mlib_s32 d_0, d_1, d_2, d_3;
	mlib_d64 s_0, s_1, s_2, s_3;
	mlib_s32 i, j;

	s_stride -= 4 * width;
	d_stride -= 4 * width;

	if (alpha == 0) {
		for (i = 0; i < height; i++) {
			d_0 = src[0];
			d_1 = src[1];
			d_2 = src[2];
			d_3 = src[3];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 1; j++) {

				s_0 = mlib_div_tab_Q8[d_0];
				s_1 = mlib_c_blend_u8_sat[d_1] * s_0;
				s_2 = mlib_c_blend_u8_sat[d_2] * s_0;
				s_3 = mlib_c_blend_u8_sat[d_3] * s_0;

				SAT8(d_1, s_1);
				SAT8(d_2, s_2);
				SAT8(d_3, s_3);

				dst[0] = d_0;
				dst[1] = d_1;
				dst[2] = d_2;
				dst[3] = d_3;

				d_0 = src[4];
				d_1 = src[5];
				d_2 = src[6];
				d_3 = src[7];
				src += 4;
				dst += 4;
			}

			s_0 = mlib_div_tab_Q8[d_0];
			s_1 = mlib_c_blend_u8_sat[d_1] * s_0;
			s_2 = mlib_c_blend_u8_sat[d_2] * s_0;
			s_3 = mlib_c_blend_u8_sat[d_3] * s_0;

			SAT8(d_1, s_1);
			SAT8(d_2, s_2);
			SAT8(d_3, s_3);

			dst[0] = d_0;
			dst[1] = d_1;
			dst[2] = d_2;
			dst[3] = d_3;

			src += s_stride + 4;
			dst += d_stride + 4;
		}

	} else if (alpha == 1) {
		for (i = 0; i < height; i++) {
			d_0 = src[0];
			d_1 = src[1];
			d_2 = src[2];
			d_3 = src[3];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 1; j++) {

				s_1 = mlib_div_tab_Q8[d_1];
				s_0 = mlib_c_blend_u8_sat[d_0] * s_1;
				s_2 = mlib_c_blend_u8_sat[d_2] * s_1;
				s_3 = mlib_c_blend_u8_sat[d_3] * s_1;

				SAT8(d_0, s_0);
				SAT8(d_2, s_2);
				SAT8(d_3, s_3);

				dst[0] = d_0;
				dst[1] = d_1;
				dst[2] = d_2;
				dst[3] = d_3;

				d_0 = src[4];
				d_1 = src[5];
				d_2 = src[6];
				d_3 = src[7];

				src += 4;
				dst += 4;
			}

			s_1 = mlib_div_tab_Q8[d_1];
			s_0 = mlib_c_blend_u8_sat[d_0] * s_1;
			s_2 = mlib_c_blend_u8_sat[d_2] * s_1;
			s_3 = mlib_c_blend_u8_sat[d_3] * s_1;

			SAT8(d_0, s_0);
			SAT8(d_2, s_2);
			SAT8(d_3, s_3);

			dst[0] = d_0;
			dst[1] = d_1;
			dst[2] = d_2;
			dst[3] = d_3;

			src += s_stride + 4;
			dst += d_stride + 4;
		}

	} else if (alpha == 2) {
		for (i = 0; i < height; i++) {
			d_0 = src[0];
			d_1 = src[1];
			d_2 = src[2];
			d_3 = src[3];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 1; j++) {

				s_2 = mlib_div_tab_Q8[d_2];
				s_0 = mlib_c_blend_u8_sat[d_0] * s_2;
				s_1 = mlib_c_blend_u8_sat[d_1] * s_2;
				s_3 = mlib_c_blend_u8_sat[d_3] * s_2;

				SAT8(d_0, s_0);
				SAT8(d_1, s_1);
				SAT8(d_3, s_3);

				dst[0] = d_0;
				dst[1] = d_1;
				dst[2] = d_2;
				dst[3] = d_3;

				d_0 = src[4];
				d_1 = src[5];
				d_2 = src[6];
				d_3 = src[7];

				src += 4;
				dst += 4;
			}

			s_2 = mlib_div_tab_Q8[d_2];
			s_0 = mlib_c_blend_u8_sat[d_0] * s_2;
			s_1 = mlib_c_blend_u8_sat[d_1] * s_2;
			s_3 = mlib_c_blend_u8_sat[d_3] * s_2;

			SAT8(d_0, s_0);
			SAT8(d_1, s_1);
			SAT8(d_3, s_3);

			dst[0] = d_0;
			dst[1] = d_1;
			dst[2] = d_2;
			dst[3] = d_3;

			src += s_stride + 4;
			dst += d_stride + 4;
		}

	} else {
		for (i = 0; i < height; i++) {
			d_0 = src[0];
			d_1 = src[1];
			d_2 = src[2];
			d_3 = src[3];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < width - 1; j++) {

				s_3 = mlib_div_tab_Q8[d_3];
				s_0 = mlib_c_blend_u8_sat[d_0] * s_3;
				s_1 = mlib_c_blend_u8_sat[d_1] * s_3;
				s_2 = mlib_c_blend_u8_sat[d_2] * s_3;

				SAT8(d_0, s_0);
				SAT8(d_1, s_1);
				SAT8(d_2, s_2);

				dst[0] = d_0;
				dst[1] = d_1;
				dst[2] = d_2;
				dst[3] = d_3;

				d_0 = src[4];
				d_1 = src[5];
				d_2 = src[6];
				d_3 = src[7];

				src += 4;
				dst += 4;
			}

			s_3 = mlib_div_tab_Q8[d_3];
			s_0 = mlib_c_blend_u8_sat[d_0] * s_3;
			s_1 = mlib_c_blend_u8_sat[d_1] * s_3;
			s_2 = mlib_c_blend_u8_sat[d_2] * s_3;

			SAT8(d_0, s_0);
			SAT8(d_1, s_1);
			SAT8(d_2, s_2);

			dst[0] = d_0;
			dst[1] = d_1;
			dst[2] = d_2;
			dst[3] = d_3;

			src += s_stride + 4;
			dst += d_stride + 4;
		}
	}
}

#endif
/* *********************************************************** */
