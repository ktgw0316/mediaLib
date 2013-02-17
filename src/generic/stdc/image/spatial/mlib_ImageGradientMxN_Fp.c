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

#pragma ident	"@(#)mlib_ImageGradientMxN_Fp.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageGradientMxN_Fp   - Compute gradient magnitude of an image
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageGradientMxN_Fp(mlib_image       *dst,
 *                                           const mlib_image *src,
 *                                           const mlib_d64   *hmask,
 *                                           const mlib_d64   *vmask,
 *                                           mlib_s32         m,
 *                                           mlib_s32         n,
 *                                           mlib_s32         dm,
 *                                           mlib_s32         dn,
 *                                           mlib_s32         cmask,
 *                                           mlib_edge        edge)
 *
 * ARGUMENTS
 *      dst       Pointer to destination image.
 *      src       Pointer to source image.
 *      hmask     Pointer to horizontal mask in row-major order.
 *      vmask     Pointer to vertical mask in row-major order.
 *      m         Mask width (m must be larger than 1).
 *      n         Mask height (n must be larger than 1).
 *      dm, dn    Position of key element in the masks (offsets from the
 *      top-left element in x and y directions respectively).
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
 *  The operation is applied on a per-channel basis.
 *  The src and dst must have same number of channels (1, 2, 3, or 4).
 *
 *  The masks are in rectangular shape at size least 2x2.
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
#include <mlib_ImageCheck.h>
#include <mlib_ImageCreate.h>
#include <mlib_SysMath.h>
#include <mlib_ImageDivTables.h>
#include <mlib_ImageSqrtTable.h>
#include <mlib_ImageGradientFunc.h>
#include <mlib_ImageConvEdge.h>
#include <mlib_ImageClipping.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageGradientMxN_Fp = __mlib_ImageGradientMxN_Fp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageGradientMxN_Fp) mlib_ImageGradientMxN_Fp
    __attribute__((weak, alias("__mlib_ImageGradientMxN_Fp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static void mlib_ImageGradientMxNMulAdd_F32(
    mlib_f32 *hdst,
    mlib_f32 *vdst,
    const mlib_f32 *src,
    const mlib_d64 *hfilter,
    const mlib_d64 *vfilter,
    mlib_s32 n,
    mlib_s32 m,
    mlib_s32 nch);

static void mlib_ImageGradientMxNMedian_F32(
    mlib_f32 *dst,
    mlib_f32 *src1,
    mlib_f32 *src2,
    mlib_s32 n,
    mlib_s32 nch);

static void mlib_ImageGradientMxNF322F32_ext(
    mlib_f32 *dst,
    const mlib_f32 *src,
    mlib_s32 n,
    mlib_s32 nch,
    mlib_s32 dx_l,
    mlib_s32 dx_r);

static void mlib_ImageGradientMxNMulAdd_D64(
    mlib_d64 *hdst,
    mlib_d64 *vdst,
    const mlib_d64 *src,
    const mlib_d64 *hfilter,
    const mlib_d64 *vfilter,
    mlib_s32 n,
    mlib_s32 m,
    mlib_s32 nch);

static void mlib_ImageGradientMxNMedian_D64(
    mlib_d64 *dst,
    mlib_d64 *src1,
    mlib_d64 *src2,
    mlib_s32 n,
    mlib_s32 nch);

static void mlib_ImageGradientMxND642D64_ext(
    mlib_d64 *dst,
    const mlib_d64 *src,
    mlib_s32 n,
    mlib_s32 nch,
    mlib_s32 dx_l,
    mlib_s32 dx_r);

/* *********************************************************** */

mlib_status
__mlib_ImageGradientMxN_Fp(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hmask,
    const mlib_d64 *vmask,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 dm,
    mlib_s32 dn,
    mlib_s32 cmask,
    mlib_edge edge)
{
	mlib_type type;

	if (m == 3 && n == 3 && dm == 1 && dn == 1) {
		return __mlib_ImageGradient3x3_Fp(dst, src, hmask, vmask, cmask,
		    edge);
	}

	MLIB_IMAGE_CHECK(dst);
	type = mlib_ImageGetType(dst);

	if (type != MLIB_FLOAT && type != MLIB_DOUBLE) {
		return (MLIB_FAILURE);
	}

	return mlib_ImageGradientMxN_alltypes(dst, src, hmask, vmask, m, n, dm,
	    dn, cmask, edge);
}

/* *********************************************************** */

void
mlib_ImageGradientMxNMulAdd_F32(
    mlib_f32 *hdst,
    mlib_f32 *vdst,
    const mlib_f32 *src,
    const mlib_d64 *hfilter,
    const mlib_d64 *vfilter,
    mlib_s32 n,
    mlib_s32 m,
    mlib_s32 nch)
{
	mlib_f32 *hdst1 = hdst + 1, *vdst1 = vdst + 1;
	mlib_s32 i, j;

	for (j = 0; j < m - 2;
	    j += 3, src += 3 * nch, hfilter += 3, vfilter += 3) {
		const mlib_f32 *src2 = src + 2 * nch;
		mlib_f32 hval0 = (mlib_f32)hfilter[0];
		mlib_f32 vval0 = (mlib_f32)vfilter[0];
		mlib_f32 hval1 = (mlib_f32)hfilter[1];
		mlib_f32 vval1 = (mlib_f32)vfilter[1];
		mlib_f32 hval2 = (mlib_f32)hfilter[2];
		mlib_f32 vval2 = (mlib_f32)vfilter[2];
		mlib_f32 val0 = src[0];
		mlib_f32 val1 = src[nch];
		mlib_f32 hdvl = hdst[0];
		mlib_f32 vdvl = vdst[0];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++) {
			mlib_f32 hdvl0 = val0 * hval0 + hdvl;
			mlib_f32 vdvl0 = val0 * vval0 + vdvl;
			mlib_f32 val2 = src2[i * nch];

			hdvl = hdst1[i];
			vdvl = vdst1[i];
			hdvl0 += val1 * hval1;
			vdvl0 += val1 * vval1;
			hdvl0 += val2 * hval2;
			vdvl0 += val2 * vval2;
			val0 = val1;
			val1 = val2;

			hdst[i] = hdvl0;
			vdst[i] = vdvl0;
		}
	}

	if (j < m - 1) {
		mlib_f32 hval0 = (mlib_f32)hfilter[0];
		mlib_f32 vval0 = (mlib_f32)vfilter[0];
		mlib_f32 hval1 = (mlib_f32)hfilter[1];
		mlib_f32 vval1 = (mlib_f32)vfilter[1];
		mlib_f32 val0, val1;

		val1 = src[0];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++) {
			mlib_f32 hdvl0, vdvl0, val0;

			val0 = val1;
			val1 = src[nch + i*nch];
			hdst[i] += val0 * hval0 + val1 * hval1;
			vdst[i] += val0 * vval0 + val1 * vval1;
		}
	} else if (j < m) {
		mlib_f32 hval0 = (mlib_f32)hfilter[0];
		mlib_f32 vval0 = (mlib_f32)vfilter[0];
		mlib_f32 val0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++) {
			val0 = src[i*nch];
			hdst[i] += val0 * hval0;
			vdst[i] += val0 * vval0;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageGradientMxNMedian_F32(
    mlib_f32 *dst,
    mlib_f32 *src1,
    mlib_f32 *src2,
    mlib_s32 n,
    mlib_s32 nch)
{
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		mlib_f32 detl =
		    mlib_sqrtf(src1[i] * src1[i] + src2[i] * src2[i]);

		src1[i] = 0.;
		src2[i] = 0.;
		dst[i * nch] = detl;
	}
}

/* *********************************************************** */

void
mlib_ImageGradientMxN_F32(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hmask,
    const mlib_d64 *vmask,
    mlib_s32 m,
    mlib_s32 n,
    const mlib_u8 *acmask,
    mlib_d64 *dsa)
{
	mlib_f32 *fsa = (mlib_f32 *)dsa, *fsh, *fsv;
	mlib_f32 *da = mlib_ImageGetData(dst);
	mlib_f32 *sa = mlib_ImageGetData(src);
	mlib_s32 dlb = mlib_ImageGetStride(dst) >> 2;
	mlib_s32 slb = mlib_ImageGetStride(src) >> 2;
	mlib_s32 dw = mlib_ImageGetWidth(dst);
	mlib_s32 dh = mlib_ImageGetHeight(dst);
	mlib_s32 nch = mlib_ImageGetChannels(dst);
	mlib_s32 i, j, j1, k;

	fsh = fsa + dw + m;
	fsv = fsh + dw;

	for (i = 0; i < dw; i++) {
		fsh[i] = 0.;
		fsv[i] = 0.;
	}

	for (j = 0; j < dh; j++, da += dlb, sa += slb)
		for (k = 0; k < nch; k++)
			if (acmask[k]) {
				mlib_f32 *sa1 = sa + k;
				const mlib_d64 *hmask1 = hmask;
				const mlib_d64 *vmask1 = vmask;

				for (j1 = 0; j1 < n;
				    j1++, sa1 += slb, hmask1 += m, vmask1 += m)
					mlib_ImageGradientMxNMulAdd_F32(fsh,
					    fsv, sa1, hmask1, vmask1, dw, m,
					    nch);

				mlib_ImageGradientMxNMedian_F32(da + k, fsh,
				    fsv, dw, nch);
			}
}

/* *********************************************************** */

void
mlib_ImageGradientMxNF322F32_ext(
    mlib_f32 *dst,
    const mlib_f32 *src,
    mlib_s32 n,
    mlib_s32 nch,
    mlib_s32 dx_l,
    mlib_s32 dx_r)
{
	mlib_s32 i;
	mlib_f32 val = src[0];

	for (i = 0; i < dx_l; i++)
		dst[i] = val;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i < n - dx_r; i++)
		dst[i] = src[nch * (i - dx_l)];
	val = dst[n - dx_r - 1];
	for (; i < n; i++)
		dst[i] = val;
}

/* *********************************************************** */

void
mlib_ImageGradientMxN_F32_ext(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hmask,
    const mlib_d64 *vmask,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_u8 *acmask,
    mlib_d64 *dsa)
{
	mlib_f32 *fsa = (mlib_f32 *)dsa, *fsh, *fsv;
	mlib_f32 *da = mlib_ImageGetData(dst);
	mlib_f32 *sa = mlib_ImageGetData(src);
	mlib_s32 dlb = mlib_ImageGetStride(dst) >> 2;
	mlib_s32 slb = mlib_ImageGetStride(src) >> 2;
	mlib_s32 dw = mlib_ImageGetWidth(dst);
	mlib_s32 dh = mlib_ImageGetHeight(dst);
	mlib_s32 nch = mlib_ImageGetChannels(dst);
	mlib_s32 i, j, j1, k;

	fsh = fsa + dw + m;
	fsv = fsh + dw;

	for (i = 0; i < dw; i++) {
		fsh[i] = 0.;
		fsv[i] = 0.;
	}

	for (j = 0; j < dh; j++, da += dlb) {
		for (k = 0; k < nch; k++)
			if (acmask[k]) {
				mlib_f32 *sa1 = sa + k;
				const mlib_d64 *hmask1 = hmask;
				const mlib_d64 *vmask1 = vmask;

				for (j1 = 0; j1 < n;
				    j1++, hmask1 += m, vmask1 += m) {
					mlib_ImageGradientMxNF322F32_ext(fsa,
					    sa1, dw + m - 1, nch, dx_l, dx_r);
					mlib_ImageGradientMxNMulAdd_F32(fsh,
					    fsv, fsa, hmask1, vmask1, dw, m, 1);

					if ((j + j1 >= dy_t) &&
					    (j + j1 < dh + n - dy_b - 2))
						sa1 += slb;
				}

				mlib_ImageGradientMxNMedian_F32(da + k, fsh,
				    fsv, dw, nch);
			}

		if ((j >= dy_t) && (j < dh + n - dy_b - 2))
			sa += slb;
	}
}

/* *********************************************************** */

void
mlib_ImageGradientMxNMulAdd_D64(
    mlib_d64 *hdst,
    mlib_d64 *vdst,
    const mlib_d64 *src,
    const mlib_d64 *hfilter,
    const mlib_d64 *vfilter,
    mlib_s32 n,
    mlib_s32 m,
    mlib_s32 nch)
{
	mlib_d64 *hdst1 = hdst + 1, *vdst1 = vdst + 1;
	mlib_s32 i, j;

	for (j = 0; j < m - 2;
	    j += 3, src += 3 * nch, hfilter += 3, vfilter += 3) {
		const mlib_d64 *src2 = src + 2 * nch;
		mlib_d64 hval0 = hfilter[0];
		mlib_d64 vval0 = vfilter[0];
		mlib_d64 hval1 = hfilter[1];
		mlib_d64 vval1 = vfilter[1];
		mlib_d64 hval2 = hfilter[2];
		mlib_d64 vval2 = vfilter[2];
		mlib_d64 val0 = src[0];
		mlib_d64 val1 = src[nch];
		mlib_d64 hdvl = hdst[0];
		mlib_d64 vdvl = vdst[0];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++) {
			mlib_d64 hdvl0 = val0 * hval0 + hdvl;
			mlib_d64 vdvl0 = val0 * vval0 + vdvl;
			mlib_d64 val2 = src2[i * nch];

			hdvl = hdst1[i];
			vdvl = vdst1[i];
			hdvl0 += val1 * hval1;
			vdvl0 += val1 * vval1;
			hdvl0 += val2 * hval2;
			vdvl0 += val2 * vval2;
			val0 = val1;
			val1 = val2;

			hdst[i] = hdvl0;
			vdst[i] = vdvl0;
		}
	}

	if (j < m - 1) {
		mlib_d64 hval0 = (mlib_d64)hfilter[0];
		mlib_d64 vval0 = (mlib_d64)vfilter[0];
		mlib_d64 hval1 = (mlib_d64)hfilter[1];
		mlib_d64 vval1 = (mlib_d64)vfilter[1];
		mlib_d64 val0, val1;

		val1 = src[0];
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++) {
			mlib_d64 hdvl0, vdvl0, val0;

			val0 = val1;
			val1 = src[nch + i*nch];
			hdst[i] += val0 * hval0 + val1 * hval1;
			vdst[i] += val0 * vval0 + val1 * vval1;
		}
	} else if (j < m) {
		mlib_d64 hval0 = (mlib_d64)hfilter[0];
		mlib_d64 vval0 = (mlib_d64)vfilter[0];
		mlib_d64 val0;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < n; i++) {
			val0 = src[i*nch];
			hdst[i] += val0 * hval0;
			vdst[i] += val0 * vval0;
		}
	}
}

/* *********************************************************** */

void
mlib_ImageGradientMxNMedian_D64(
    mlib_d64 *dst,
    mlib_d64 *src1,
    mlib_d64 *src2,
    mlib_s32 n,
    mlib_s32 nch)
{
	mlib_s32 i;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < n; i++) {
		mlib_d64 detl =
		    mlib_sqrt(src1[i] * src1[i] + src2[i] * src2[i]);

		src1[i] = 0.;
		src2[i] = 0.;
		dst[i * nch] = detl;
	}
}

/* *********************************************************** */

void
mlib_ImageGradientMxN_D64(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hmask,
    const mlib_d64 *vmask,
    mlib_s32 m,
    mlib_s32 n,
    const mlib_u8 *acmask,
    mlib_d64 *dsa)
{
	mlib_d64 *dsh, *dsv;
	mlib_d64 *da = mlib_ImageGetData(dst);
	mlib_d64 *sa = mlib_ImageGetData(src);
	mlib_s32 dlb = mlib_ImageGetStride(dst) >> 3;
	mlib_s32 slb = mlib_ImageGetStride(src) >> 3;
	mlib_s32 dw = mlib_ImageGetWidth(dst);
	mlib_s32 dh = mlib_ImageGetHeight(dst);
	mlib_s32 nch = mlib_ImageGetChannels(dst);
	mlib_s32 i, j, j1, k;

	dsh = dsa + dw + m;
	dsv = dsh + dw;

	for (i = 0; i < dw; i++) {
		dsh[i] = 0.;
		dsv[i] = 0.;
	}

	for (j = 0; j < dh; j++, da += dlb, sa += slb)
		for (k = 0; k < nch; k++)
			if (acmask[k]) {
				mlib_d64 *sa1 = sa + k;
				const mlib_d64 *hmask1 = hmask;
				const mlib_d64 *vmask1 = vmask;

				for (j1 = 0; j1 < n;
				    j1++, sa1 += slb, hmask1 += m, vmask1 += m)
					mlib_ImageGradientMxNMulAdd_D64(dsh,
					    dsv, sa1, hmask1, vmask1, dw, m,
					    nch);

				mlib_ImageGradientMxNMedian_D64(da + k, dsh,
				    dsv, dw, nch);
			}
}

/* *********************************************************** */

void
mlib_ImageGradientMxND642D64_ext(
    mlib_d64 *dst,
    const mlib_d64 *src,
    mlib_s32 n,
    mlib_s32 nch,
    mlib_s32 dx_l,
    mlib_s32 dx_r)
{
	mlib_s32 i;
	mlib_d64 val = src[0];

	for (i = 0; i < dx_l; i++)
		dst[i] = val;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (; i < n - dx_r; i++)
		dst[i] = src[nch * (i - dx_l)];
	val = dst[n - dx_r - 1];
	for (; i < n; i++)
		dst[i] = val;
}

/* *********************************************************** */

void
mlib_ImageGradientMxN_D64_ext(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *hmask,
    const mlib_d64 *vmask,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_u8 *acmask,
    mlib_d64 *dsa)
{
	mlib_d64 *dsh, *dsv;
	mlib_d64 *da = mlib_ImageGetData(dst);
	mlib_d64 *sa = mlib_ImageGetData(src);
	mlib_s32 dlb = mlib_ImageGetStride(dst) >> 3;
	mlib_s32 slb = mlib_ImageGetStride(src) >> 3;
	mlib_s32 dw = mlib_ImageGetWidth(dst);
	mlib_s32 dh = mlib_ImageGetHeight(dst);
	mlib_s32 nch = mlib_ImageGetChannels(dst);
	mlib_s32 i, j, j1, k;

	dsh = dsa + dw + m;
	dsv = dsh + dw;

	for (i = 0; i < dw; i++) {
		dsh[i] = 0.;
		dsv[i] = 0.;
	}

	for (j = 0; j < dh; j++, da += dlb) {
		for (k = 0; k < nch; k++)
			if (acmask[k]) {
				mlib_d64 *sa1 = sa + k;
				const mlib_d64 *hmask1 = hmask;
				const mlib_d64 *vmask1 = vmask;

				for (j1 = 0; j1 < n;
				    j1++, hmask1 += m, vmask1 += m) {
					mlib_ImageGradientMxND642D64_ext(dsa,
					    sa1, dw + m - 1, nch, dx_l, dx_r);
					mlib_ImageGradientMxNMulAdd_D64(dsh,
					    dsv, dsa, hmask1, vmask1, dw, m, 1);

					if ((j + j1 >= dy_t) &&
					    (j + j1 < dh + n - dy_b - 2))
						sa1 += slb;
				}

				mlib_ImageGradientMxNMedian_D64(da + k, dsh,
				    dsv, dw, nch);
			}

		if ((j >= dy_t) && (j < dh + n - dy_b - 2))
			sa += slb;
	}
}

/* *********************************************************** */
