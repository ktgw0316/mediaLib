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
 * Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident   "@(#)mlib_s_ImageSobel.c	9.2	07/11/05 SMI"

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
#ifdef  __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

/* *********************************************************** */
extern mlib_u8 mlib_SqrtTable_U8[];
extern mlib_u16 mlib_SqrtTable_U8_Diap[];

/* *********************************************************** */
typedef union
{
	__m128i dv;
	mlib_u16 sv[8];
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
#define	ADD_SRC(dst) {                                      \
	switch (nchan) {                                        \
		case 1:                                             \
			r = _mm_loadl_epi64((__m128i *)sp);             \
			r = _mm_unpacklo_epi8(r, _s_zero);              \
			break;                                          \
		case 2:                                             \
			s0 = _mm_loadu_si128((__m128i *)sp);            \
			r = _mm_unpacklo_epi8(s0, _s_zero);             \
			g = _mm_unpackhi_epi8(s0, _s_zero);             \
			rl = _mm_unpacklo_epi16(r, g);                  \
			gl = _mm_unpackhi_epi16(r, g);                  \
			rh = _mm_unpacklo_epi16(rl, gl);                \
			gh = _mm_unpackhi_epi16(rl, gl);                \
			r = _mm_unpacklo_epi16(rh, gh);                 \
			break;                                          \
		case 3:                                             \
			s0 = _mm_loadu_si128((__m128i *)sp);            \
			s1 = _mm_loadu_si128((__m128i *)(sp + 16));     \
			r = _mm_unpacklo_epi8(s0, _s_zero);             \
			g = _mm_unpackhi_epi8(s0, _s_zero);             \
			b = _mm_unpacklo_epi8(s1, _s_zero);             \
                                                            \
			rl = _mm_unpacklo_epi16(r,                      \
				_mm_srli_si128(g, 8));                      \
			gl = _mm_unpacklo_epi16(                        \
				_mm_srli_si128(r, 8), b);                   \
			bl = _mm_unpacklo_epi16(g,                      \
				_mm_srli_si128(b, 8));                      \
                                                            \
			rh = _mm_unpacklo_epi16(rl,                     \
				_mm_srli_si128(gl, 8));                     \
			gh = _mm_unpacklo_epi16(                        \
				_mm_srli_si128(rl, 8), bl);                 \
                                                            \
			r = _mm_unpacklo_epi16(rh,                      \
				_mm_srli_si128(gh, 8));                     \
			break;                                          \
		case 4:                                             \
			s0 = _mm_loadu_si128((__m128i *)sp);            \
			s1 = _mm_loadu_si128((__m128i *)(sp + 16));     \
			r = _mm_unpacklo_epi8(s0, _s_zero);             \
			g = _mm_unpackhi_epi8(s0, _s_zero);             \
			b = _mm_unpacklo_epi8(s1, _s_zero);             \
			a = _mm_unpackhi_epi8(s1, _s_zero);             \
			rl = _mm_unpacklo_epi16(r, g);                  \
			gl = _mm_unpackhi_epi16(r, g);                  \
			bl = _mm_unpacklo_epi16(b, a);                  \
			al = _mm_unpackhi_epi16(b, a);                  \
                                                            \
			rh = _mm_unpacklo_epi16(rl, gl);                \
			gh = _mm_unpacklo_epi16(bl, al);                \
                                                            \
			r = _mm_unpacklo_epi64(rh, gh);                 \
			break;                                          \
                                                            \
	}                                                       \
		_mm_storeu_si128((__m128i *)(dst), r);              \
		sp += 8 * nchan;                                    \
}


/* *********************************************************** */
static void
mlib_s_ImageSobelSqrt_U8(
    mlib_u8 *dst,
    __m128i * src1,
    __m128i * src2,
    mlib_s32 n,
    mlib_s32 nch)
{
	mlib_s32 i, count = n >> 3, left = n & 7;
	__m128i val1, val2;

	val1 = _mm_loadu_si128(src1);
	src1++;
	val2 = _mm_loadu_si128(src2);
	src2++;

	for (i = 0; i < count; i++, dst += 8 * nch, src1++, src2++) {
		mlib_union_type res;
		__m128i tmp1, tmp2, res1, res2;
		mlib_s32 detl0, detl1, detl2, detl3,
			detl4, detl5, detl6, detl7;

		tmp1 = _mm_mullo_epi16(val1, val1);
		tmp2 = _mm_mulhi_epi16(val1, val1);
		res1 = _mm_unpacklo_epi16(tmp1, tmp2);
		res2 = _mm_unpackhi_epi16(tmp1, tmp2);
		tmp1 = _mm_mullo_epi16(val2, val2);
		tmp2 = _mm_mulhi_epi16(val2, val2);
		res1 = _mm_add_epi32(res1,
				_mm_unpacklo_epi16(tmp1, tmp2));
		res2 = _mm_add_epi32(res2,
				_mm_unpackhi_epi16(tmp1, tmp2));

		res1 = _mm_packs_epi32(
				_mm_srai_epi32(res1, 1),
				_mm_srai_epi32(res2, 1));

		res.dv = _mm_add_epi16(res1, res1);

		val1 = _mm_loadu_si128(src1);
		val2 = _mm_loadu_si128(src2);
		detl0 = res.sv[0];
		detl1 = res.sv[1];
		detl2 = res.sv[2];
		detl3 = res.sv[3];
		detl4 = res.sv[4];
		detl5 = res.sv[5];
		detl6 = res.sv[6];
		detl7 = res.sv[7];

		FAST_SQRT_U8(dst, detl0);
		FAST_SQRT_U8(dst + nch, detl1);
		FAST_SQRT_U8(dst + 2 * nch, detl2);
		FAST_SQRT_U8(dst + 3 * nch, detl3);
		FAST_SQRT_U8(dst + 4 * nch, detl4);
		FAST_SQRT_U8(dst + 5 * nch, detl5);
		FAST_SQRT_U8(dst + 6 * nch, detl6);
		FAST_SQRT_U8(dst + 7 * nch, detl7);
	}

	if (left) {
		mlib_union_type res;
		__m128i tmp1, tmp2, res1, res2;

		tmp1 = _mm_mullo_epi16(val1, val1);
		tmp2 = _mm_mulhi_epi16(val1, val1);
		res1 = _mm_unpacklo_epi16(tmp1, tmp2);
		res2 = _mm_unpackhi_epi16(tmp1, tmp2);
		tmp1 = _mm_mullo_epi16(val2, val2);
		tmp2 = _mm_mulhi_epi16(val2, val2);
		res1 = _mm_add_epi32(res1,
				_mm_unpacklo_epi16(tmp1, tmp2));
		res2 = _mm_add_epi32(res2,
				_mm_unpackhi_epi16(tmp1, tmp2));

		res1 =
		    _mm_packs_epi32(
			_mm_srai_epi32(res1, 1),
			_mm_srai_epi32(res2, 1));
		res.dv = _mm_add_epi16(res1, res1);

		for (i = 0; i < left; i++, dst += nch) {
			mlib_s32 detl = res.sv[i];

			FAST_SQRT_U8(dst, detl);
		}
	}
}

/* *********************************************************** */
void
mlib_s_ImageSobel_U8(
    mlib_u8 *dst,
    mlib_u8 *src,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 dw,
    mlib_s32 dh,
    mlib_s32 cmask,
    mlib_s32 nchan)
{
	__m128i a0, a1, a2, b0, b2, c0, c1, c2, d0, d1, hh, vv;
	__m128i r, g, b, a, rl, gl, bl, al, rh, gh, bh, ah, s0, s1;
	__m128i *buffh, *buffv;
	mlib_s32 i, j, k, wid8, bline;
	mlib_u8 *pbuff, *buff0, *buff1, *buff2, *buffT;
	mlib_u8 *da, *sa, *sp;
	__m128i _s_zero = _mm_setzero_si128();

	wid8 = (dw + 7) / 8;
	bline = sizeof (__m128i) * (wid8 + 2);
	pbuff = __mlib_malloc(5 * bline);

	buff0 = pbuff;
	buff1 = buff0 + bline;
	buff2 = buff1 + bline;
	buffh = (void *)(buff2 + bline);
	buffv = (void *)((mlib_u8 *)buffh + bline);

	src -= slb + nchan;

	for (k = 0; k < nchan; k++)
		if (cmask & (1 << (nchan - k - 1))) {
			sa = src + k;
			da = dst + k;

			for (j = 0; j < 2; j++) {
				sp = sa;
				buffT = (j == 0) ? buff0 : buff1;
				for (i = 0; i < wid8 + 1; i++) {
					ADD_SRC(buffT + 16 * i);
				}

				sa += slb;
			}

			for (j = 0; j < dh; j++) {
				sp = sa;
				ADD_SRC(buff2);
				ADD_SRC(buff2 + 16);

/*
 *          [ -1 -2 -1 ]          [ -1 0 1 ]            [ a0 a1 a2 ]
 *      H = [  0  0  0 ]      V = [ -2 0 2 ]      SRC = [ b0 b1 b2 ]
 *          [  1  2  1 ]          [ -1 0 1 ]            [ c0 c1 c2 ]
 */

				for (i = 0; i < wid8; i++) {
					ADD_SRC(buff2 + 16 * i + 32);

					a0 = _mm_loadu_si128(
						(__m128i *)(buff0 +
						16 * i));
					a1 = _mm_loadu_si128(
						(__m128i *)(buff0 +
						16 * i + 2));
					a2 = _mm_loadu_si128(
						(__m128i *)(buff0 +
						16 * i + 4));
					b0 = _mm_loadu_si128(
						(__m128i *)(buff1 +
						16 * i));
					b2 = _mm_loadu_si128(
						(__m128i *)(buff1 +
						16 * i + 4));
					c0 = _mm_loadu_si128(
						(__m128i *)(buff2 +
						16 * i));
					c1 = _mm_loadu_si128(
						(__m128i *)(buff2 +
						16 * i + 2));
					c2 = _mm_loadu_si128(
						(__m128i *)(buff2 +
						16 * i + 4));

					d0 = _mm_sub_epi16(c2, a0);
					d1 = _mm_sub_epi16(c0, a2);

					a1 = _mm_sub_epi16(c1, a1);
					a1 = _mm_add_epi16(a1, a1);
					hh = _mm_add_epi16(d0, d1);
					hh = _mm_add_epi16(hh, a1);

					b0 = _mm_sub_epi16(b2, b0);
					b0 = _mm_add_epi16(b0, b0);
					vv = _mm_sub_epi16(d0, d1);
					vv = _mm_add_epi16(vv, b0);

					_mm_storeu_si128(buffh + i, hh);
					_mm_storeu_si128(buffv + i, vv);
				}

				buffT = buff0;
				buff0 = buff1;
				buff1 = buff2;
				buff2 = buffT;

				mlib_s_ImageSobelSqrt_U8(da, buffh, buffv, dw,
				    nchan);

				sa += slb;
				da += dlb;
			}
		}

	__mlib_free(pbuff);
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
	mlib_s_ImageSobel_U8(da, sa, dlb, slb, dw, dh, cmask, 1);

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
	mlib_s_ImageSobel_U8(da, sa, dlb, slb, dw, dh, cmask, 2);

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
	mlib_s_ImageSobel_U8(da, sa, dlb, slb, dw, dh, cmask, 3);

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
	mlib_s_ImageSobel_U8(da, sa, dlb, slb, dw, dh, cmask, 4);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
