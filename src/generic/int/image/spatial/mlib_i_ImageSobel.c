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

#pragma ident   "@(#)mlib_i_ImageSobel.c	9.2    07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageSobel   - Perform Sobel operation of an image
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageSobel(mlib_image       *dst,
 *                                  const mlib_image *src,
 *                                  mlib_s32         cmask,
 *                                  mlib_edge        edge)
 *
 * ARGUMENTS
 *      dst       Pointer to destination image.
 *      src       Pointer to source image.
 *      cmask     Channel mask to indicate the channels to be processed.
 *                Each bit of which represents a channel in the image. The
 *                channels corresponded to 1 bits are those to be processed.
 *      edge      Type of edge condition.
 *
 * RESTRICTION
 *
 *  The src and the dst must be images of the same type.
 *  For the integral type function, the src and the dst can be images of
 *  MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or MLIB_INT type.
 *
 *  The operation is applied on a per-channel basis.
 *  The src and dst must have same number of channels (1, 2, 3, or 4).
 *
 *  The unselected channels are not overwritten. If both src and dst have
 *  just one channel, cmask is ignored.
 *
 *  The mapping of source image to destination image is left/top corner to
 *  left/top corner, but with a shift of the destination image if it's
 *  smaller than the source image.
 *  The exact mapping formula is
 *       xs = xd + x_offset
 *       ys = yd + y_offset
 *  where
 *       x_offset = (dst_width >= src_width) ? 0 : (kernel_size - 1)/2
 *       y_offset = (dst_height >= src_height) ? 0 : (kernel_size - 1)/2
 *
 *  The edge condition can be one of the following:
 *    MLIB_EDGE_DST_NO_WRITE  (default)
 *    MLIB_EDGE_DST_FILL_ZERO
 *    MLIB_EDGE_DST_COPY_SRC
 *    MLIB_EDGE_SRC_EXTEND
 *
 * DESCRIPTION
 *
 *  The "GradientMxN" operation is an edge detector which
 *  computes the magnitude of the image gradient vector in two
 *  orthogonal directions.
 *
 *  The result of the "GradientMxN" operation may be defined as:
 *
 *      dst[x][y][c] = ((SH(x, y,c))^2 + (SV(x, y,c))^2 )^0.5
 *
 *  where SH(x, y,c) and SV(x, y,c) are the horizontal and vertical
 *  gradient images generated from channel c of the source image by
 *  correlating it with the supplied orthogonal (horizontal and
 *  vertical) gradient masks.
 *
 *  One of the special cases of "Gradient" operation, "Sobel" operation,
 *  should be handled in an optimized way because of the speciality of
 *  the masks. For "Sobel" operation, the horizontal and vertical masks
 *  are
 *          [ -1 -2 -1 ]              [ -1 0 1 ]
 *      H = [  0  0  0 ]          V = [ -2 0 2 ]
 *          [  1  2  1 ]              [ -1 0 1 ]
 *
 *  with the central elements as key elements.
 */

#include <mlib_image.h>
#include <mlib_SysMath.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageSobel.h>
#include <mlib_ImageDivTables.h>
#include <mlib_ImageSqrtTable.h>

/* *********************************************************** */

#define	SAT_U16(x)	(mlib_s32)(((x) < 0) ? 0 : (((x) > MLIB_U16_MAX) ? \
				MLIB_U16_MAX : (x)))

/* *********************************************************** */

#define	BSIZE	256

/* *********************************************************** */

#define	PROCESS_CHANNEL(k)	(cmask & (1 << (nchan - (k) - 1)))

/* *********************************************************** */
#define	SOBEL_DECLARE_LOCALS_U8()                               \
	mlib_s32 i, j, k;                                       \
	const mlib_u8 *sp0, *sp1, *sp2;                         \
	const mlib_u8 *sp01, *sp11, *sp21;                      \
	mlib_u8 *dp;                                            \
	mlib_u8 *dp1;                                           \
	mlib_s32 diap;                                          \
	mlib_s32 r32, rv;                                       \
	mlib_s32 v, v0, v1, v2;                                 \
	mlib_s32 h, h0, h1, h2;                                 \
	mlib_s32 s0, s1, s2

/* *********************************************************** */
#define	SOBEL_PREPARE_CHANNEL_U8()                                        \
	sp01 = sp0 + k;                                                   \
	sp11 = sp1 + k;                                                   \
	sp21 = sp2 + k;                                                   \
	dp1 = dp + k;                                                     \
	v1 = sp01[0] + sp11[0] + sp11[0] + sp21[0];                       \
	v0 = sp01[-nchan] + sp11[-nchan] + sp11[-nchan] + sp21[-nchan];   \
	h1 = (mlib_s32)(sp21[0] - sp01[0]);                               \
	h0 = (mlib_s32)(sp21[-nchan] - sp01[-nchan]);                     \
	sp01 += nchan;                                                    \
	sp11 += nchan;                                                    \
	sp21 += nchan

/* *********************************************************** */
#define	SOBEL_PROCESS_CHANNEL_FAST_U8()                           \
	for (j = 0; j < dw; j++) {                                \
	    s0 = *sp01;                                           \
	    s1 = *sp11;                                           \
	    s2 = *sp21;                                           \
	    h2 = mlib_U82F32[s2] - mlib_U82F32[s0];               \
	    v2 = s0 + s2 + s1 + s1;                               \
	    v = v2 - v0;                                          \
	    h = h0 + h1 + h1 + h2;                                \
	    rv = (mlib_s32)v;                                     \
	    r32 = h * h + rv * rv;                                \
	    v = SAT_U16(r32);                                     \
	    diap = mlib_SqrtTable_U8_Diap[(v) >> 8];              \
	    *dp1 = mlib_SqrtTable_U8[((v) >> (diap)) + (diap)];   \
	    sp01 += nchan;                                        \
	    sp11 += nchan;                                        \
	    sp21 += nchan;                                        \
	    dp1 += nchan;                                         \
	    v0 = v1;                                              \
	    v1 = v2;                                              \
	    h0 = h1;                                              \
	    h1 = h2;                                              \
	}

/* *********************************************************** */

mlib_status
mlib_ImageSobel_U8_1(
    mlib_u8 *da,
    const mlib_u8 *sa,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 dw,
    mlib_s32 dh,
    mlib_s32 cmask)
{
	SOBEL_DECLARE_LOCALS_U8();
	mlib_s32 nchan = 1;

	sp0 = sa - slb;
	sp1 = sa;
	sp2 = sa + slb;
	dp = da;
	for (i = 0; i < dh; i++) {
		for (k = 0; k < nchan; k++) {

			if (PROCESS_CHANNEL(k)) {
				SOBEL_PREPARE_CHANNEL_U8();
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				SOBEL_PROCESS_CHANNEL_FAST_U8();
			}
		}

		sp0 += slb;
		sp1 += slb;
		sp2 += slb;
		dp += dlb;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageSobel_U8_2(
    mlib_u8 *da,
    const mlib_u8 *sa,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 dw,
    mlib_s32 dh,
    mlib_s32 cmask)
{
	SOBEL_DECLARE_LOCALS_U8();
	mlib_s32 nchan = 2;

	sp0 = sa - slb;
	sp1 = sa;
	sp2 = sa + slb;
	dp = da;
	for (i = 0; i < dh; i++) {
		for (k = 0; k < nchan; k++) {

			if (PROCESS_CHANNEL(k)) {
				SOBEL_PREPARE_CHANNEL_U8();
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				SOBEL_PROCESS_CHANNEL_FAST_U8();
			}
		}

		sp0 += slb;
		sp1 += slb;
		sp2 += slb;
		dp += dlb;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageSobel_U8_3(
    mlib_u8 *da,
    const mlib_u8 *sa,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 dw,
    mlib_s32 dh,
    mlib_s32 cmask)
{
	SOBEL_DECLARE_LOCALS_U8();
	mlib_s32 nchan = 3;

	sp0 = sa - slb;
	sp1 = sa;
	sp2 = sa + slb;
	dp = da;
	for (i = 0; i < dh; i++) {
		for (k = 0; k < nchan; k++) {

			if (PROCESS_CHANNEL(k)) {
				SOBEL_PREPARE_CHANNEL_U8();
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				SOBEL_PROCESS_CHANNEL_FAST_U8();
			}
		}

		sp0 += slb;
		sp1 += slb;
		sp2 += slb;
		dp += dlb;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageSobel_U8_4(
    mlib_u8 *da,
    const mlib_u8 *sa,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 dw,
    mlib_s32 dh,
    mlib_s32 cmask)
{
	SOBEL_DECLARE_LOCALS_U8();
	mlib_s32 nchan = 4;

	sp0 = sa - slb;
	sp1 = sa;
	sp2 = sa + slb;
	dp = da;
	for (i = 0; i < dh; i++) {
		for (k = 0; k < nchan; k++) {

			if (PROCESS_CHANNEL(k)) {
				SOBEL_PREPARE_CHANNEL_U8();
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				SOBEL_PROCESS_CHANNEL_FAST_U8();
			}
		}

		sp0 += slb;
		sp1 += slb;
		sp2 += slb;
		dp += dlb;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
