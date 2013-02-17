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

#pragma ident	"@(#)mlib_ImageSobel_Fp.c	9.4	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageSobel_Fp   - perform Sobel operation of an image
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageSobel_Fp(mlib_image       *dst,
 *                                     const mlib_image *src,
 *                                     mlib_s32         cmask,
 *                                     mlib_edge        edge)
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
 *  For the floating-point type function, the src and the dst can be
 *  images of MLIB_FLOAT or MLIB_DOUBLE type.
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
#include <mlib_ImageConvEdge.h>
#include <mlib_ImageClipping.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageSobel_Fp = __mlib_ImageSobel_Fp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageSobel_Fp) mlib_ImageSobel_Fp
    __attribute__((weak, alias("__mlib_ImageSobel_Fp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	PROCESS_CHANNEL(k)	(cmask & (1 << (nchan - (k) - 1)))

/* *********************************************************** */

#define	SOBEL_ONE_CHANNEL_F32(dp, sp0, sp1, sp2, ldelta, rdelta)       \
	{                                                              \
	    h = ((*((sp2) + (rdelta))) + (*((sp2) + (ldelta))) +       \
		((*(sp2))) + ((*(sp2)))) - ((*((sp0) + (rdelta))) +    \
		(*((sp0) + (ldelta))) + ((*(sp0))) + ((*(sp0))));      \
	    v = ((*((sp0) + (rdelta))) + (*((sp2) + (rdelta))) +       \
		((*((sp1) + (rdelta)))) + ((*((sp1) + (rdelta))))) -   \
		((*((sp0) + (ldelta))) + (*((sp2) + (ldelta))) +       \
		((*((sp1) + (ldelta)))) + ((*((sp1) + (ldelta)))));    \
	    *(dp) = mlib_sqrtf(((h)) * ((h)) + ((v)) * ((v)));         \
	}

/* *********************************************************** */

#define	SOBEL_ONE_PIXEL_F32(dp, sp0, sp1, sp2, ldelta, rdelta)       \
	{                                                            \
	    for (k = 0; k < nchan; k++) {                            \
		if (PROCESS_CHANNEL(k)) {                            \
		SOBEL_ONE_CHANNEL_F32((dp + k), (sp0 + k),           \
			(sp1 + k), (sp2 + k), (ldelta), (rdelta))}   \
	    }                                                        \
	}

/* *********************************************************** */

#define	SOBEL_ONE_CHANNEL_D64(dp, sp0, sp1, sp2, ldelta, rdelta)       \
	{                                                              \
	    h = ((*((sp2) + (rdelta))) + (*((sp2) + (ldelta))) +       \
		((*(sp2))) + ((*(sp2)))) - ((*((sp0) + (rdelta))) +    \
		(*((sp0) + (ldelta))) + ((*(sp0))) + ((*(sp0))));      \
	    v = ((*((sp0) + (rdelta))) + (*((sp2) + (rdelta))) +       \
		((*((sp1) + (rdelta)))) + ((*((sp1) + (rdelta))))) -   \
		((*((sp0) + (ldelta))) + (*((sp2) + (ldelta))) +       \
		((*((sp1) + (ldelta)))) + ((*((sp1) + (ldelta)))));    \
	    *(dp) = mlib_sqrt(((h)) * ((h)) + ((v)) * ((v)));          \
	}

/* *********************************************************** */

#define	SOBEL_ONE_PIXEL_D64(dp, sp0, sp1, sp2, ldelta, rdelta)       \
	{                                                            \
	    for (k = 0; k < nchan; k++) {                            \
		if (PROCESS_CHANNEL(k)) {                            \
		SOBEL_ONE_CHANNEL_D64((dp + k), (sp0 + k),           \
			(sp1 + k), (sp2 + k), (ldelta), (rdelta))}   \
	    }                                                        \
	}

/* *********************************************************** */

#define	SOBEL_DECLARE_LOCALS_F32()                              \
	mlib_s32 i, j, k;                                       \
	const mlib_f32 *sp0, *sp1, *sp2;                        \
	mlib_f32 *dp;                                           \
	const mlib_f32 *sp01, *sp11, *sp21;                     \
	mlib_f32 *dp1;                                          \
	mlib_f32 v, h;                                          \
	mlib_f32 v0, v1, v2;                                    \
	mlib_f32 h0, h1, h2;                                    \
	mlib_f32 s0, s1, s2

/* *********************************************************** */

#define	SOBEL_PREPARE_CHANNEL_F32()                                  \
	sp01 = sp0 + k;                                              \
	sp11 = sp1 + k;                                              \
	sp21 = sp2 + k;                                              \
	dp1 = dp + k;                                                \
	v1 = *sp01 + *sp11 + *sp11 + *sp21;                          \
	v0 = *(sp01 - nchan) + *(sp11 - nchan) + *(sp11 - nchan) +   \
	    *(sp21 - nchan);                                         \
	h1 = *sp21 - *sp01;                                          \
	h0 = *(sp21 - nchan) - *(sp01 - nchan);                      \
	sp01 += nchan;                                               \
	sp11 += nchan;                                               \
	sp21 += nchan

/* *********************************************************** */

#define	SOBEL_PROCESS_CHANNEL_FAST_F32()                        \
	for (j = 0; j < dw; j++) {                              \
	    s0 = *sp01;                                         \
	    s1 = *sp11;                                         \
	    s2 = *sp21;                                         \
	    v2 = s0 + s2 + s1 + s1;                             \
	    v = v2 - v0;                                        \
	    h2 = s2 - s0;                                       \
	    h = h0 + h1 + h1 + h2;                              \
	    v0 = v1;                                            \
	    v1 = v2;                                            \
	    h0 = h1;                                            \
	    h1 = h2;                                            \
	    *dp1 = mlib_sqrtf(h * h + v * v);                   \
	    sp01 += nchan;                                      \
	    sp11 += nchan;                                      \
	    sp21 += nchan;                                      \
	    dp1 += nchan;                                       \
	}

/* *********************************************************** */

static void mlib_ImageSobel_F32_1(
    mlib_f32 *da,
    const mlib_f32 *sa,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 dw,
    mlib_s32 dh,
    mlib_s32 cmask);

static void mlib_ImageSobel_F32_2(
    mlib_f32 *da,
    const mlib_f32 *sa,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 dw,
    mlib_s32 dh,
    mlib_s32 cmask);

static void mlib_ImageSobel_F32_3(
    mlib_f32 *da,
    const mlib_f32 *sa,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 dw,
    mlib_s32 dh,
    mlib_s32 cmask);

static void mlib_ImageSobel_F32_4(
    mlib_f32 *da,
    const mlib_f32 *sa,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 dw,
    mlib_s32 dh,
    mlib_s32 cmask);

static void mlib_ImageSobel_D64(
    mlib_d64 *da,
    const mlib_d64 *sa,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 dw,
    mlib_s32 dh,
    mlib_s32 nchan,
    mlib_s32 cmask);

static void mlib_ImageSobel_ExtEdge_F32(
    mlib_f32 *da,
    const mlib_f32 *sa,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 dw,
    mlib_s32 dh,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    mlib_s32 nchan,
    mlib_s32 cmask);

static void mlib_ImageSobel_ExtEdge_D64(
    mlib_d64 *da,
    const mlib_d64 *sa,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 dw,
    mlib_s32 dh,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    mlib_s32 nchan,
    mlib_s32 cmask);

/* *********************************************************** */

mlib_status
__mlib_ImageSobel_Fp(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 cmask,
    mlib_edge edge)
{
	mlib_image dst_i[1], src_i[1], dst_e[1], src_e[1];
	mlib_s32 edg_sizes[8];
	mlib_type type;
	mlib_s32 nchan, slb, dlb, dx_l, dx_r, dy_t, dy_b;
	mlib_u8 *dl, *sl, *dp, *sp;
	mlib_s32 dw, dh, pw, ph;
	mlib_d64 zero[4] = { 0, 0, 0, 0 };
	mlib_status ret;

	ret =
	    mlib_ImageClipping(dst_i, src_i, dst_e, src_e, edg_sizes, dst, src,
	    3);

	if (ret != MLIB_SUCCESS)
		return (ret);

	MLIB_IMAGE_GET_ALL_PARAMS(dst_i, type, nchan, pw, ph, dlb, dl);
	sl = mlib_ImageGetData(src_i);
	slb = mlib_ImageGetStride(src_i);

	pw -= 2;
	ph -= 2;

	if (nchan == 1)
		cmask = 1;

	if ((cmask & ((1 << nchan) - 1)) == 0)
		return (MLIB_SUCCESS);

	if (pw > 0 && ph > 0) {

		switch (type) {
		case MLIB_FLOAT:
			sp = sl + slb + 4 * nchan;
			dp = dl + dlb + 4 * nchan;

			switch (nchan) {
			case 1:
				mlib_ImageSobel_F32_1((void *)dp, (void *)sp,
				    dlb, slb, pw, ph, cmask);
				break;
			case 2:
				mlib_ImageSobel_F32_2((void *)dp, (void *)sp,
				    dlb, slb, pw, ph, cmask);
				break;
			case 3:
				mlib_ImageSobel_F32_3((void *)dp, (void *)sp,
				    dlb, slb, pw, ph, cmask);
				break;
			case 4:
				mlib_ImageSobel_F32_4((void *)dp, (void *)sp,
				    dlb, slb, pw, ph, cmask);
				break;
			default:
				return (MLIB_FAILURE);
			}

			if (ret != MLIB_SUCCESS)
				return (ret);
			break;

		case MLIB_DOUBLE:
			sp = sl + slb + 8 * nchan;
			dp = dl + dlb + 8 * nchan;

			mlib_ImageSobel_D64((void *)dp, (void *)sp, dlb, slb,
			    pw, ph, nchan, cmask);
			break;

		default:
			return (MLIB_FAILURE);
		}

/* process edges */
		dx_l = edg_sizes[0];
		dx_r = edg_sizes[1];
		dy_t = edg_sizes[2];
		dy_b = edg_sizes[3];

		if (dx_l + dx_r + dy_t + dy_b == 0)
			edge = MLIB_EDGE_DST_NO_WRITE;

		switch (edge) {
		case MLIB_EDGE_DST_NO_WRITE:
			break;
		case MLIB_EDGE_DST_FILL_ZERO:
			mlib_ImageConvClearEdge_Fp(dst_e, dx_l, dx_r, dy_t,
			    dy_b, zero, cmask);
			break;
		case MLIB_EDGE_DST_COPY_SRC:
			mlib_ImageConvCopyEdge_Fp(dst_e, src_e, dx_l, dx_r,
			    dy_t, dy_b, cmask);
			break;
		case MLIB_EDGE_SRC_EXTEND:
		    {
			    MLIB_IMAGE_GET_ALL_PARAMS(dst_e, type, nchan, dw,
				dh, dlb, dl);
			    sl = mlib_ImageGetData(src_e);
			    slb = mlib_ImageGetStride(src_e);

			    switch (type) {
			    case MLIB_FLOAT:
				    mlib_ImageSobel_ExtEdge_F32((void *)dl,
					(void *)sl, dlb, slb, dw, dh,
					dx_l, dx_r, dy_t, dy_b,
					nchan, cmask);
				    break;
			    case MLIB_DOUBLE:
				    mlib_ImageSobel_ExtEdge_D64((void *)dl,
					(void *)sl, dlb, slb, dw, dh,
					dx_l, dx_r, dy_t, dy_b,
					nchan, cmask);
				    break;
			    default:
				    break;
			    }

			    break;
		    }

		default:
			return (MLIB_FAILURE);
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

void
mlib_ImageSobel_F32_1(
    mlib_f32 *da,
    const mlib_f32 *sa,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 dw,
    mlib_s32 dh,
    mlib_s32 cmask)
{
	SOBEL_DECLARE_LOCALS_F32();
	mlib_s32 nchan = 1;

	slb = slb >> 2;
	dlb = dlb >> 2;
	sp0 = sa - slb;
	sp1 = sa;
	sp2 = sa + slb;
	dp = da;
	for (i = 0; i < dh; i++) {
		for (k = 0; k < nchan; k++) {

			if (PROCESS_CHANNEL(k)) {
				SOBEL_PREPARE_CHANNEL_F32();
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				SOBEL_PROCESS_CHANNEL_FAST_F32();
			}
		}

		sp0 += slb;
		sp1 += slb;
		sp2 += slb;
		dp += dlb;
	}
}

/* *********************************************************** */

void
mlib_ImageSobel_F32_2(
    mlib_f32 *da,
    const mlib_f32 *sa,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 dw,
    mlib_s32 dh,
    mlib_s32 cmask)
{
	SOBEL_DECLARE_LOCALS_F32();
	mlib_s32 nchan = 2;

	slb = slb >> 2;
	dlb = dlb >> 2;
	sp0 = sa - slb;
	sp1 = sa;
	sp2 = sa + slb;
	dp = da;
	for (i = 0; i < dh; i++) {
		for (k = 0; k < nchan; k++) {

			if (PROCESS_CHANNEL(k)) {
				SOBEL_PREPARE_CHANNEL_F32();
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				SOBEL_PROCESS_CHANNEL_FAST_F32();
			}
		}

		sp0 += slb;
		sp1 += slb;
		sp2 += slb;
		dp += dlb;
	}
}

/* *********************************************************** */

void
mlib_ImageSobel_F32_3(
    mlib_f32 *da,
    const mlib_f32 *sa,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 dw,
    mlib_s32 dh,
    mlib_s32 cmask)
{
	SOBEL_DECLARE_LOCALS_F32();
	mlib_s32 nchan = 3;

	slb = slb >> 2;
	dlb = dlb >> 2;
	sp0 = sa - slb;
	sp1 = sa;
	sp2 = sa + slb;
	dp = da;
	for (i = 0; i < dh; i++) {
		for (k = 0; k < nchan; k++) {

			if (PROCESS_CHANNEL(k)) {
				SOBEL_PREPARE_CHANNEL_F32();
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				SOBEL_PROCESS_CHANNEL_FAST_F32();
			}
		}

		sp0 += slb;
		sp1 += slb;
		sp2 += slb;
		dp += dlb;
	}
}

/* *********************************************************** */

void
mlib_ImageSobel_F32_4(
    mlib_f32 *da,
    const mlib_f32 *sa,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 dw,
    mlib_s32 dh,
    mlib_s32 cmask)
{
	SOBEL_DECLARE_LOCALS_F32();
	mlib_s32 nchan = 4;

	slb = slb >> 2;
	dlb = dlb >> 2;
	sp0 = sa - slb;
	sp1 = sa;
	sp2 = sa + slb;
	dp = da;
	for (i = 0; i < dh; i++) {
		for (k = 0; k < nchan; k++) {

			if (PROCESS_CHANNEL(k)) {
				SOBEL_PREPARE_CHANNEL_F32();
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				SOBEL_PROCESS_CHANNEL_FAST_F32();
			}
		}

		sp0 += slb;
		sp1 += slb;
		sp2 += slb;
		dp += dlb;
	}
}

/* *********************************************************** */

void
mlib_ImageSobel_D64(
    mlib_d64 *da,
    const mlib_d64 *sa,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 dw,
    mlib_s32 dh,
    mlib_s32 nchan,
    mlib_s32 cmask)
{
	mlib_s32 i, j, k;
	const mlib_d64 *sp0, *sp1, *sp2;
	mlib_d64 *dp;
	const mlib_d64 *sp01, *sp11, *sp21;
	mlib_d64 *dp1;
	mlib_d64 v, h;

	slb = slb >> 3;
	dlb = dlb >> 3;
	sp0 = sa - slb;
	sp1 = sa;
	sp2 = sa + slb;
	dp = da;
	for (i = 0; i < dh; i++) {
		for (k = 0; k < nchan; k++) {

			if (PROCESS_CHANNEL(k)) {
				sp01 = sp0 + k;
				sp11 = sp1 + k;
				sp21 = sp2 + k;
				dp1 = dp + k;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j < dw; j++) {
					SOBEL_ONE_CHANNEL_D64(dp1, sp01, sp11,
					    sp21, -nchan, nchan);
					sp01 += nchan;
					sp11 += nchan;
					sp21 += nchan;
					dp1 += nchan;
				}
			}
		}

		sp0 += slb;
		sp1 += slb;
		sp2 += slb;
		dp += dlb;
	}
}

/* *********************************************************** */

#ifdef _MSC_VER
#pragma optimize("", off)
#endif /* _MSC_VER */

void
mlib_ImageSobel_ExtEdge_F32(
    mlib_f32 *da,
    const mlib_f32 *sa,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 dw,
    mlib_s32 dh,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    mlib_s32 nchan,
    mlib_s32 cmask)
{
	mlib_s32 j, k;
	mlib_f32 v, h;
	mlib_s32 xend, syend, dyend;
	const mlib_f32 *sp01, *sp11, *sp21;
	mlib_f32 *dp1;
	mlib_s32 is_l, is_r, is_b, is_t;

	slb = slb >> 2;
	dlb = dlb >> 2;
	is_l = -nchan * (1 - dx_l);
	is_r =  nchan * (1 - dx_r);
	is_t = -slb   * (1 - dy_t);
	is_b =  slb   * (1 - dy_b);
	xend = nchan * (dw - 1);
	dyend = dlb * (dh - 1);
	syend = slb * (dh - 1);

	if ((dw == 1) && (dh == 1)) {
		SOBEL_ONE_PIXEL_F32(da, sa + is_t, sa, sa + is_b, is_l, is_r);
		return;
	}

	if (dh == 1) {

		if (dy_t) {
			for (k = 0; k < nchan; k++) {

				if (PROCESS_CHANNEL(k)) {
					sp01 = sa + k + is_t + nchan;
					sp11 = sa + k + nchan;
					sp21 = sa + k + is_b + nchan;
					dp1 = da + k + nchan;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (j = 1; j < (dw - 1); j++) {
						SOBEL_ONE_CHANNEL_F32(dp1, sp01,
						    sp11, sp21, -nchan, nchan);
						sp01 += nchan;
						sp11 += nchan;
						sp21 += nchan;
						dp1 += nchan;
					}
				}
			}
		}

		if (dy_t || dx_l)
			SOBEL_ONE_PIXEL_F32(da, sa + is_t, sa, sa + is_b, is_l,
			    nchan);

		if (dy_t || dx_r)
			SOBEL_ONE_PIXEL_F32(da + xend, sa + is_t + xend,
			    sa + xend, sa + is_b + xend, -nchan, is_r);
		return;
	}

	if (dw == 1) {

		if (dx_l) {
			for (k = 0; k < nchan; k++) {

				if (PROCESS_CHANNEL(k)) {
					sp01 = sa + k;
					sp11 = sa + k + slb;
					sp21 = sa + k + slb + slb;
					dp1 = da + k + dlb;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (j = 1; j < (dh - 1); j++) {
						SOBEL_ONE_CHANNEL_F32(dp1, sp01,
						    sp11, sp21, is_l, is_r);
						sp01 += slb;
						sp11 += slb;
						sp21 += slb;
						dp1 += dlb;
					}
				}
			}
		}

		if (dx_l || dy_t)
			SOBEL_ONE_PIXEL_F32(da, sa + is_t, sa, sa + slb, is_l,
			    is_r);

		if (dx_l || dy_b)
			SOBEL_ONE_PIXEL_F32(da + dyend, sa - slb + syend,
			    sa + syend, sa + is_b + syend, is_l, is_r);
		return;
	}

	if (dy_t) {

		for (k = 0; k < nchan; k++) {

			if (PROCESS_CHANNEL(k)) {
				sp01 = sa + k + is_t + nchan;
				sp11 = sa + k + nchan;
				sp21 = sa + k + slb + nchan;
				dp1 = da + k + nchan;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 1; j < (dw - 1); j++) {
					SOBEL_ONE_CHANNEL_F32(dp1, sp01, sp11,
					    sp21, -nchan, nchan);
					sp01 += nchan;
					sp11 += nchan;
					sp21 += nchan;
					dp1 += nchan;
				}
			}
		}
	}

	if (dy_b) {

		for (k = 0; k < nchan; k++) {

			if (PROCESS_CHANNEL(k)) {
				sp01 = sa + k - slb + nchan + syend;
				sp11 = sa + k + nchan + syend;
				sp21 = sa + k + is_b + nchan + syend;
				dp1 = da + k + nchan + dyend;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 1; j < (dw - 1); j++) {
					SOBEL_ONE_CHANNEL_F32(dp1, sp01, sp11,
					    sp21, -nchan, nchan);
					sp01 += nchan;
					sp11 += nchan;
					sp21 += nchan;
					dp1 += nchan;
				}
			}
		}
	}

	if (dx_l) {

		for (k = 0; k < nchan; k++) {

			if (PROCESS_CHANNEL(k)) {
				sp01 = sa + k;
				sp11 = sa + k + slb;
				sp21 = sa + k + slb + slb;
				dp1 = da + k + dlb;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 1; j < (dh - 1); j++) {
					SOBEL_ONE_CHANNEL_F32(dp1, sp01, sp11,
					    sp21, is_l, nchan);
					sp01 += slb;
					sp11 += slb;
					sp21 += slb;
					dp1 += dlb;
				}
			}
		}
	}

	if (dx_r) {

		for (k = 0; k < nchan; k++) {

			if (PROCESS_CHANNEL(k)) {
				sp01 = sa + k + xend;
				sp11 = sa + k + slb + xend;
				sp21 = sa + k + slb + slb + xend;
				dp1 = da + k + dlb + xend;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 1; j < (dh - 1); j++) {
					SOBEL_ONE_CHANNEL_F32(dp1, sp01, sp11,
					    sp21, -nchan, is_r);
					sp01 += slb;
					sp11 += slb;
					sp21 += slb;
					dp1 += dlb;
				}
			}
		}
	}

	if (dy_t || dx_l)
		SOBEL_ONE_PIXEL_F32(da, sa + is_t, sa, sa + slb, is_l, nchan);

	if (dy_t || dx_r)
		SOBEL_ONE_PIXEL_F32(da + xend, sa + is_t + xend, sa + xend,
		    sa + slb + xend, -nchan, is_r);

	if (dy_b || dx_l)
		SOBEL_ONE_PIXEL_F32(da + dyend, sa + syend - slb, sa + syend,
		    sa + is_b + syend, is_l, nchan);

	if (dy_b || dx_r)
		SOBEL_ONE_PIXEL_F32(da + dyend + xend, sa + syend - slb + xend,
		    sa + syend + xend, sa + is_b + syend + xend, -nchan, is_r);
}

/* *********************************************************** */

void
mlib_ImageSobel_ExtEdge_D64(
    mlib_d64 *da,
    const mlib_d64 *sa,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 dw,
    mlib_s32 dh,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    mlib_s32 nchan,
    mlib_s32 cmask)
{
	mlib_s32 j, k;
	mlib_d64 v, h;
	mlib_s32 xend, syend, dyend;
	const mlib_d64 *sp01, *sp11, *sp21;
	mlib_d64 *dp1;
	mlib_s32 is_l, is_r, is_b, is_t;

	slb = slb >> 3;
	dlb = dlb >> 3;
	is_l = -nchan * (1 - dx_l);
	is_r =  nchan * (1 - dx_r);
	is_t = -slb   * (1 - dy_t);
	is_b =  slb   * (1 - dy_b);
	xend = nchan * (dw - 1);
	dyend = dlb * (dh - 1);
	syend = slb * (dh - 1);

	if ((dw == 1) && (dh == 1)) {
		SOBEL_ONE_PIXEL_D64(da, sa + is_t, sa, sa + is_b, is_l, is_r);
		return;
	}

	if (dh == 1) {

		if (dy_t) {
			for (k = 0; k < nchan; k++) {

				if (PROCESS_CHANNEL(k)) {
					sp01 = sa + k + is_t + nchan;
					sp11 = sa + k + nchan;
					sp21 = sa + k + is_b + nchan;
					dp1 = da + k + nchan;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (j = 1; j < (dw - 1); j++) {
						SOBEL_ONE_CHANNEL_D64(dp1, sp01,
						    sp11, sp21, -nchan, nchan);
						sp01 += nchan;
						sp11 += nchan;
						sp21 += nchan;
						dp1 += nchan;
					}
				}
			}
		}

		if (dy_t || dx_l)
			SOBEL_ONE_PIXEL_D64(da, sa + is_t, sa, sa + is_b, is_l,
			    nchan);

		if (dy_t || dx_r)
			SOBEL_ONE_PIXEL_D64(da + xend, sa + is_t + xend,
			    sa + xend, sa + is_b + xend, -nchan, is_r);
		return;
	}

	if (dw == 1) {

		if (dx_l) {
			for (k = 0; k < nchan; k++) {

				if (PROCESS_CHANNEL(k)) {
					sp01 = sa + k;
					sp11 = sa + k + slb;
					sp21 = sa + k + slb + slb;
					dp1 = da + k + dlb;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
					for (j = 1; j < (dh - 1); j++) {
						SOBEL_ONE_CHANNEL_D64(dp1, sp01,
						    sp11, sp21, is_l, is_r);
						sp01 += slb;
						sp11 += slb;
						sp21 += slb;
						dp1 += dlb;
					}
				}
			}
		}

		if (dx_l || dy_t)
			SOBEL_ONE_PIXEL_D64(da, sa + is_t, sa, sa + slb, is_l,
			    is_r);

		if (dx_l || dy_b)
			SOBEL_ONE_PIXEL_D64(da + dyend, sa - slb + syend,
			    sa + syend, sa + is_b + syend, is_l, is_r);
		return;
	}

	if (dy_t) {

		for (k = 0; k < nchan; k++) {

			if (PROCESS_CHANNEL(k)) {
				sp01 = sa + k + is_t + nchan;
				sp11 = sa + k + nchan;
				sp21 = sa + k + slb + nchan;
				dp1 = da + k + nchan;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 1; j < (dw - 1); j++) {
					SOBEL_ONE_CHANNEL_D64(dp1, sp01, sp11,
					    sp21, -nchan, nchan);
					sp01 += nchan;
					sp11 += nchan;
					sp21 += nchan;
					dp1 += nchan;
				}
			}
		}
	}

	if (dy_b) {

		for (k = 0; k < nchan; k++) {

			if (PROCESS_CHANNEL(k)) {
				sp01 = sa + k - slb + nchan + syend;
				sp11 = sa + k + nchan + syend;
				sp21 = sa + k + is_b + nchan + syend;
				dp1 = da + k + nchan + dyend;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 1; j < (dw - 1); j++) {
					SOBEL_ONE_CHANNEL_D64(dp1, sp01, sp11,
					    sp21, -nchan, nchan);
					sp01 += nchan;
					sp11 += nchan;
					sp21 += nchan;
					dp1 += nchan;
				}
			}
		}
	}

	if (dx_l) {

		for (k = 0; k < nchan; k++) {

			if (PROCESS_CHANNEL(k)) {
				sp01 = sa + k;
				sp11 = sa + k + slb;
				sp21 = sa + k + slb + slb;
				dp1 = da + k + dlb;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 1; j < (dh - 1); j++) {
					SOBEL_ONE_CHANNEL_D64(dp1, sp01, sp11,
					    sp21, is_l, nchan);
					sp01 += slb;
					sp11 += slb;
					sp21 += slb;
					dp1 += dlb;
				}
			}
		}
	}

	if (dx_r) {

		for (k = 0; k < nchan; k++) {

			if (PROCESS_CHANNEL(k)) {
				sp01 = sa + k + xend;
				sp11 = sa + k + slb + xend;
				sp21 = sa + k + slb + slb + xend;
				dp1 = da + k + dlb + xend;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 1; j < (dh - 1); j++) {
					SOBEL_ONE_CHANNEL_D64(dp1, sp01, sp11,
					    sp21, -nchan, is_r);
					sp01 += slb;
					sp11 += slb;
					sp21 += slb;
					dp1 += dlb;
				}
			}
		}
	}

	if (dy_t || dx_l)
		SOBEL_ONE_PIXEL_D64(da, sa + is_t, sa, sa + slb, is_l, nchan);

	if (dy_t || dx_r)
		SOBEL_ONE_PIXEL_D64(da + xend, sa + is_t + xend, sa + xend,
		    sa + slb + xend, -nchan, is_r);

	if (dy_b || dx_l)
		SOBEL_ONE_PIXEL_D64(da + dyend, sa + syend - slb, sa + syend,
		    sa + is_b + syend, is_l, nchan);

	if (dy_b || dx_r)
		SOBEL_ONE_PIXEL_D64(da + dyend + xend, sa + syend - slb + xend,
		    sa + syend + xend, sa + is_b + syend + xend, -nchan, is_r);
}

#ifdef _MSC_VER
#pragma optimize("", on)
#endif /* _MSC_VER */

/* *********************************************************** */
