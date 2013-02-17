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

#pragma ident	"@(#)mlib_m_ImageSobel.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageSobel   - Perform Sobel operation of an image
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageSobel(mlib_image *dst,
 *                                  const mlib_image *src,
 *                                  mlib_s32   cmask,
 *                                  mlib_edge  edge)
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
 *  MLIB_BYTE, MLIB_SHORT, or MLIB_INT type.
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
 *  The unselected channels in the dst image are not overwritten. If
 *  both src and dst have just one channel, cmask is ignored.
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
#include <mlib_mmx_utils.h>

/* *********************************************************** */
extern mlib_u8 mlib_SqrtTable_U8[];
extern mlib_u16 mlib_SqrtTable_U8_Diap[];

/* *********************************************************** */
typedef union
{
	__m64 dv;
	mlib_u16 sv[4];
} mlib_union_type;

/* *********************************************************** */
#define	FAST_SQRT_U8(dp, v)                                     \
	{                                                       \
	    mlib_s32 diap;                                      \
	                                                        \
	    diap = mlib_SqrtTable_U8_Diap[(v) >> 8];            \
	    *(dp) = mlib_SqrtTable_U8[((v) >> diap) + diap];    \
	}

/* *********************************************************** */
#define	ADD_SRC(dst)                                            \
	((mlib_u8 *)(dst))[0] = sp[0];                          \
	((mlib_u8 *)(dst))[2] = sp[nchan];                      \
	((mlib_u8 *)(dst))[4] = sp[2 * nchan];                  \
	((mlib_u8 *)(dst))[6] = sp[3 * nchan];                  \
	sp += 4 * nchan

/* *********************************************************** */
static void
mlib_m_ImageSobelSqrt_U8(
    mlib_u8 *dst,
    __m64 * src1,
    __m64 * src2,
    mlib_s32 n,
    mlib_s32 nch)
{
	mlib_s32 i, count = n >> 2, left = n & 3;
	__m64 val1, val2;

	val1 = (*src1++);
	val2 = (*src2++);

	for (i = 0; i < count; i++, dst += 4 * nch, src1++, src2++) {
		mlib_union_type res;
		__m64 tmp1, tmp2, res1, res2;
		mlib_s32 detl0, detl1, detl2, detl3;

		tmp1 = _mm_mullo_pi16(val1, val1);
		tmp2 = _mm_mulhi_pi16(val1, val1);
		res1 = _mm_unpacklo_pi16(tmp1, tmp2);
		res2 = _mm_unpackhi_pi16(tmp1, tmp2);
		tmp1 = _mm_mullo_pi16(val2, val2);
		tmp2 = _mm_mulhi_pi16(val2, val2);
		res1 = _mm_add_pi32(res1, _mm_unpacklo_pi16(tmp1, tmp2));
		res2 = _mm_add_pi32(res2, _mm_unpackhi_pi16(tmp1, tmp2));

		res1 =
		    _mm_packs_pi32(_mm_srai_pi32(res1, 1), _mm_srai_pi32(res2,
		    1));
		res.dv = _mm_add_pi16(res1, res1);

		val1 = src1[0];
		val2 = src2[0];
		detl0 = res.sv[0];
		detl1 = res.sv[1];
		detl2 = res.sv[2];
		detl3 = res.sv[3];

		FAST_SQRT_U8(dst, detl0);
		FAST_SQRT_U8(dst + nch, detl1);
		FAST_SQRT_U8(dst + 2 * nch, detl2);
		FAST_SQRT_U8(dst + 3 * nch, detl3);
	}

	if (left) {
		mlib_union_type res;
		__m64 tmp1, tmp2, res1, res2;

		tmp1 = _mm_mullo_pi16(val1, val1);
		tmp2 = _mm_mulhi_pi16(val1, val1);
		res1 = _mm_unpacklo_pi16(tmp1, tmp2);
		res2 = _mm_unpackhi_pi16(tmp1, tmp2);
		tmp1 = _mm_mullo_pi16(val2, val2);
		tmp2 = _mm_mulhi_pi16(val2, val2);
		res1 = _mm_add_pi32(res1, _mm_unpacklo_pi16(tmp1, tmp2));
		res2 = _mm_add_pi32(res2, _mm_unpackhi_pi16(tmp1, tmp2));

		res1 =
		    _mm_packs_pi32(_mm_srai_pi32(res1, 1), _mm_srai_pi32(res2,
		    1));
		res.dv = _mm_add_pi16(res1, res1);

		for (i = 0; i < left; i++, dst += nch) {
			mlib_s32 detl = res.sv[i];

			FAST_SQRT_U8(dst, detl);
		}
	}

	_mm_empty();
}

/* *********************************************************** */
void
mlib_m_ImageSobel_U8(
    mlib_u8 *dst,
    mlib_u8 *src,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 dw,
    mlib_s32 dh,
    mlib_s32 cmask,
    mlib_s32 nchan)
{
	__m64 a0, a1, a2, b0, b2, c0, c1, c2, d0, d1, hh, vv;
	__m64 *buffh, *buffv;
	mlib_s32 i, j, k, wid4, bline;
	mlib_u8 *pbuff, *buff0, *buff1, *buff2, *buffT;
	mlib_u8 *da, *sa, *sp;

	wid4 = (dw + 3) / 4;
	bline = sizeof (__m64) * (wid4 + 2);
	pbuff = mlib_malloc(5 * bline);

	buff0 = pbuff;
	buff1 = buff0 + bline;
	buff2 = buff1 + bline;
	buffh = (void *)(buff2 + bline);
	buffv = (void *)((mlib_u8 *)buffh + bline);

	for (i = 0; i < wid4 + 1; i++) {
		((__m64 *) buff0)[i] = _m_zero;
		((__m64 *) buff1)[i] = _m_zero;
		((__m64 *) buff2)[i] = _m_zero;
	}

	src -= slb + nchan;

	for (k = 0; k < nchan; k++)
		if (cmask & (1 << (nchan - k - 1))) {
			sa = src + k;
			da = dst + k;

			for (j = 0; j < 2; j++) {
				sp = sa;
				buffT = (j == 0) ? buff0 : buff1;
				for (i = 0; i < wid4 + 1; i++) {
					ADD_SRC(buffT + 8 * i);
				}

				sa += slb;
			}

			for (j = 0; j < dh; j++) {
				sp = sa;
				ADD_SRC(buff2);
				ADD_SRC(buff2 + 8);

/*
 *          [ -1 -2 -1 ]          [ -1 0 1 ]            [ a0 a1 a2 ]
 *      H = [  0  0  0 ]      V = [ -2 0 2 ]      SRC = [ b0 b1 b2 ]
 *          [  1  2  1 ]          [ -1 0 1 ]            [ c0 c1 c2 ]
 */

				for (i = 0; i < wid4; i++) {
					ADD_SRC(buff2 + 8 * i + 16);

					a0 = *(__m64 *) (buff0 + 8 * i);
					a1 = *(__m64 *) (buff0 + 8 * i + 2);
					a2 = *(__m64 *) (buff0 + 8 * i + 4);
					b0 = *(__m64 *) (buff1 + 8 * i);
					b2 = *(__m64 *) (buff1 + 8 * i + 4);
					c0 = *(__m64 *) (buff2 + 8 * i);
					c1 = *(__m64 *) (buff2 + 8 * i + 2);
					c2 = *(__m64 *) (buff2 + 8 * i + 4);

					d0 = _mm_sub_pi16(c2, a0);
					d1 = _mm_sub_pi16(c0, a2);

					a1 = _mm_sub_pi16(c1, a1);
					a1 = _mm_add_pi16(a1, a1);
					hh = _mm_add_pi16(d0, d1);
					hh = _mm_add_pi16(hh, a1);

					b0 = _mm_sub_pi16(b2, b0);
					b0 = _mm_add_pi16(b0, b0);
					vv = _mm_sub_pi16(d0, d1);
					vv = _mm_add_pi16(vv, b0);

					buffh[i] = hh;
					buffv[i] = vv;
				}

				buffT = buff0;
				buff0 = buff1;
				buff1 = buff2;
				buff2 = buffT;

				_mm_empty();

				mlib_m_ImageSobelSqrt_U8(da, buffh, buffv, dw,
				    nchan);

				sa += slb;
				da += dlb;
			}
		}

	_mm_empty();
	mlib_free(pbuff);
}

/* *********************************************************** */
mlib_status
mlib_ImageSobel_U8_1(
    mlib_u8 *da,
    mlib_u8 *sa,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 dw,
    mlib_s32 dh,
    mlib_s32 cmask)
{
	mlib_m_ImageSobel_U8(da, sa, dlb, slb, dw, dh, cmask, 1);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
mlib_status
mlib_ImageSobel_U8_2(
    mlib_u8 *da,
    mlib_u8 *sa,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 dw,
    mlib_s32 dh,
    mlib_s32 cmask)
{
	mlib_m_ImageSobel_U8(da, sa, dlb, slb, dw, dh, cmask, 2);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
mlib_status
mlib_ImageSobel_U8_3(
    mlib_u8 *da,
    mlib_u8 *sa,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 dw,
    mlib_s32 dh,
    mlib_s32 cmask)
{
	mlib_m_ImageSobel_U8(da, sa, dlb, slb, dw, dh, cmask, 3);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
mlib_status
mlib_ImageSobel_U8_4(
    mlib_u8 *da,
    mlib_u8 *sa,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 dw,
    mlib_s32 dh,
    mlib_s32 cmask)
{
	mlib_m_ImageSobel_U8(da, sa, dlb, slb, dw, dh, cmask, 4);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
