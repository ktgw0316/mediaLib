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

#pragma ident	"@(#)mlib_m_ImageColorConvert2_f.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageColorConvert - color conversion
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageColorConvert1(mlib_image *dst,
 *                                          const mlib_image *src,
 *                                          const mlib_d64 *fmat);
 *      mlib_status mlib_ImageColorConvert2(mlib_image *dst,
 *                                          const mlib_image *src,
 *                                          const mlib_d64 *fmat,
 *                                          const mlib_d64 *offset);
 *      mlib_status mlib_ImageColorRGB2XYZ(mlib_image *dst,
 *                                         const mlib_image *src);
 *      mlib_status mlib_ImageColorXYZ2RGB(mlib_image *dst,
 *                                         const mlib_image *src);
 *      mlib_status mlib_ImageColorRGB2YCC(mlib_image *dst,
 *                                         const mlib_image *src);
 *      mlib_status mlib_ImageColorYCC2RGB(mlib_image *dst,
 *                                         const mlib_image *src);
 *
 * ARGUMENT
 *      dst     pointer to output image
 *      src     pointer to input image
 *      fmat    9 parameters of color matrix in row major order
 *      offset  3 offset values
 *
 * RESTRICTION
 *      src and dst must be the same type, the same size and the same
 *      number of channels.
 *      They must be 3-channel images.
 *      They can be in MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 *
 * DESCRIPTION
 *      mlib_ImageColorConvert1()
 *        |X|   |fmat[0] fmat[1] fmat[2]|   |R|
 *        |Y| = |fmat[3] fmat[4] fmat[5]| * |G|
 *        |Z|   |fmat[6] fmat[7] fmat[8]|   |B|
 *      mlib_ImageColorConvert2()
 *        |X|   |fmat[0] fmat[1] fmat[2]|   |R|   |offset[0]|
 *        |Y| = |fmat[3] fmat[4] fmat[5]| * |G| + |offset[1]|
 *        |Z|   |fmat[6] fmat[7] fmat[8]|   |B|   |offset[2]|
 *      mlib_ImageColorRGB2XYZ()
 *        convert ITU-R Rec.708 RGB with D64 white point into CIE XYZ
 *      mlib_ImageColorXYZ2RGB()
 *        convert CIE XYZ into ITU-R Rec.708 RGB with D64 white point
 *      mlib_ImageColorRGB2YCC()
 *        convert computer R'G'B' into ITU-R Rec.601 Y'CbCr
 *      mlib_ImageColorYCC2RGB()
 *        convert ITU-R Rec.601 Y'CbCr into computer R'G'B'
 */

#include <mlib_image.h>
#include <mlib_SysMath.h>
#include <mlib_ImageCheck.h>
#include <mlib_mmx_utils.h>
#include <stdio.h>

extern mlib_d64 mlib_U82D64[];

#define	USE_TAIL_SAVING

/* *********************************************************** */

#define	MLIB_RINT_S16(res, i, x, scalef)                        \
	if (x[i] >= 0)                                          \
	    tmp = (mlib_s32)(x[i] * scalef + 0.5);              \
	else                                                    \
	    tmp = (mlib_s32)(x[i] * scalef - 0.5);              \
	tmp &= 0xffff;                                          \
	tmp |= tmp << 16;                                       \
	((mlib_s32 *)&res##i)[0] = tmp;                         \
	((mlib_s32 *)&res##i)[1] = tmp

/* *********************************************************** */

#define	MLIB_RINT1_S16(res, i, x, scalef)                       \
	if (x[i] >= 0)                                          \
	    tmp0 = (mlib_s32)(x[i] * scalef + 0.5);             \
	else                                                    \
	    tmp0 = (mlib_s32)(x[i] * scalef - 0.5);             \
	tmp0 &= 0xffff;                                         \
	if (x[i + 1] >= 0)                                      \
	    tmp1 = (mlib_s32)(x[i + 1] * scalef + 0.5);         \
	else                                                    \
	    tmp1 = (mlib_s32)(x[i + 1] * scalef - 0.5);         \
	tmp1 &= 0xffff;                                         \
	if (x[i + 2] >= 0)                                      \
	    tmp2 = (mlib_s32)(x[i + 2] * scalef + 0.5);         \
	else                                                    \
	    tmp2 = (mlib_s32)(x[i + 2] * scalef - 0.5);         \
	tmp2 &= 0xffff;                                         \
	((mlib_s32 *)&res##i)[0] = (tmp1 << 16) | tmp0;         \
	((mlib_s32 *)&res##i)[1] = tmp2;

/* *********************************************************** */

#define	MLIB_RINT_S32(res, i, x, scalef)                        \
	if (x[i] >= 0)                                          \
	    tmp = (mlib_s32)(x[i] * scalef + 0.5);              \
	else                                                    \
	    tmp = (mlib_s32)(x[i] * scalef - 0.5);              \
	((mlib_s32 *)&res##i)[0] = tmp;                         \
	((mlib_s32 *)&res##i)[1] = tmp

/* *********************************************************** */

mlib_status
mlib_m_ImageColorConvert2_U8(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *fmat,
    const mlib_d64 *offset,
    mlib_s32 scale)
{
	const mlib_u8 *sa, *sl;
	mlib_u8 *da, *dl;
	mlib_s32 i, j, shift, xsize4, tmp;
	__m64 k0, k1, k2, k3, k4, k5, k6, k7, k8;
	__m64 off0, off1, off2;
	__m64 s0, s1, s2, s3, p0, p1, p2, pa;
	__m64 sum0, sum1, sum2, r01, r2x, ra, rb, rc, rd;
	__m64 zero;
	mlib_d64 scalef;

	shift = 6 - scale;

	scalef = 1 << (9 + shift);
	MLIB_RINT_S16(k, 0, fmat, scalef);
	MLIB_RINT_S16(k, 1, fmat, scalef);
	MLIB_RINT_S16(k, 2, fmat, scalef);
	MLIB_RINT_S16(k, 3, fmat, scalef);
	MLIB_RINT_S16(k, 4, fmat, scalef);
	MLIB_RINT_S16(k, 5, fmat, scalef);
	MLIB_RINT_S16(k, 6, fmat, scalef);
	MLIB_RINT_S16(k, 7, fmat, scalef);
	MLIB_RINT_S16(k, 8, fmat, scalef);

	scalef = 1 << shift;
	MLIB_RINT_S16(off, 0, offset, scalef);
	MLIB_RINT_S16(off, 1, offset, scalef);
	MLIB_RINT_S16(off, 2, offset, scalef);

#ifdef USE_TAIL_SAVING
	xsize4 = (xsize + 3) / 4;
#else
	xsize4 = (xsize - 1) / 4;
#endif

	zero = _m_zero;

	sa = sl = src;
	da = dl = dst;

	for (j = 0; j < ysize; j++) {
		mlib_s32 tail;

#ifdef USE_TAIL_SAVING
		mlib_u8 *dend = dl + 3 * xsize;
		__m64 tail0, tail1;

		tail0 = ((__m64 *) dend)[0];
		tail1 = ((__m64 *) dend)[1];
#endif

		for (i = 0; i < xsize4; i++) {
			s0 = _m_from_int(*(mlib_s32 *)sa);
			s1 = _m_from_int(*(mlib_s32 *)(sa + 3));
			s2 = _m_from_int(*(mlib_s32 *)(sa + 6));
			s3 = _m_from_int(*(mlib_s32 *)(sa + 9));
			sa += 12;
			s0 = _mm_unpacklo_pi8(s0, s2);
			s1 = _mm_unpacklo_pi8(s1, s3);
			pa = _mm_unpacklo_pi8(s0, s1);
			p2 = _mm_unpackhi_pi8(s0, s1);
			p0 = _mm_unpacklo_pi8(pa, zero);
			p1 = _mm_unpackhi_pi8(pa, zero);
			p2 = _mm_unpacklo_pi8(p2, zero);
			p0 = _mm_slli_pi16(p0, 7);
			p1 = _mm_slli_pi16(p1, 7);
			p2 = _mm_slli_pi16(p2, 7);
			sum0 = _mm_mulhi_pi16(p0, k0);
			sum1 = _mm_mulhi_pi16(p0, k3);
			sum2 = _mm_mulhi_pi16(p0, k6);
			sum0 = _mm_add_pi16(sum0, _mm_mulhi_pi16(p1, k1));
			sum1 = _mm_add_pi16(sum1, _mm_mulhi_pi16(p1, k4));
			sum2 = _mm_add_pi16(sum2, _mm_mulhi_pi16(p1, k7));
			sum0 = _mm_add_pi16(sum0, _mm_mulhi_pi16(p2, k2));
			sum1 = _mm_add_pi16(sum1, _mm_mulhi_pi16(p2, k5));
			sum2 = _mm_add_pi16(sum2, _mm_mulhi_pi16(p2, k8));
			sum0 = _mm_add_pi16(sum0, off0);
			sum1 = _mm_add_pi16(sum1, off1);
			sum2 = _mm_add_pi16(sum2, off2);
			sum0 = _mm_srai_pi16(sum0, shift);
			sum1 = _mm_srai_pi16(sum1, shift);
			sum2 = _mm_srai_pi16(sum2, shift);

			r01 = _mm_packs_pu16(sum0, sum1);
			r2x = _mm_packs_pu16(sum2, zero);
			ra = _mm_unpacklo_pi8(r01, r2x);
			rb = _mm_unpackhi_pi8(r01, zero);
			rc = _mm_unpacklo_pi8(ra, rb);
			rd = _mm_unpackhi_pi8(ra, rb);

			*(mlib_s32 *)(da) = _m_to_int(rc);
			*(mlib_s32 *)(da + 3) =
			    _m_to_int(_mm_srli_si64(rc, 32));
			*(mlib_s32 *)(da + 6) = _m_to_int(rd);
			*(mlib_s32 *)(da + 9) =
			    _m_to_int(_mm_srli_si64(rd, 32));
			da += 12;
		}

#ifdef USE_TAIL_SAVING
		((__m64 *) dend)[0] = tail0;
		((__m64 *) dend)[1] = tail1;
#else
		tail = xsize - 4 * i;
		{
			s0 = _m_from_int(*(mlib_s32 *)sa);
			s1 = _m_from_int(*(mlib_s32 *)(sa + 3));
			s2 = _m_from_int(*(mlib_s32 *)(sa + 6));
			s3 = _m_from_int(*(mlib_s32 *)(sa + 9));
			sa += 12;
			s0 = _mm_unpacklo_pi8(s0, s2);
			s1 = _mm_unpacklo_pi8(s1, s3);
			pa = _mm_unpacklo_pi8(s0, s1);
			p2 = _mm_unpackhi_pi8(s0, s1);
			p0 = _mm_unpacklo_pi8(pa, zero);
			p1 = _mm_unpackhi_pi8(pa, zero);
			p2 = _mm_unpacklo_pi8(p2, zero);
			p0 = _mm_sll_pi16(p0, 7);
			p1 = _mm_sll_pi16(p1, 7);
			p2 = _mm_sll_pi16(p2, 7);
			sum0 = _mm_mulhi_pi16(p0, k0);
			sum1 = _mm_mulhi_pi16(p0, k3);
			sum2 = _mm_mulhi_pi16(p0, k6);
			sum0 = _mm_add_pi16(sum0, _mm_mulhi_pi16(p1, k1));
			sum1 = _mm_add_pi16(sum1, _mm_mulhi_pi16(p1, k4));
			sum2 = _mm_add_pi16(sum2, _mm_mulhi_pi16(p1, k7));
			sum0 = _mm_add_pi16(sum0, _mm_mulhi_pi16(p2, k2));
			sum1 = _mm_add_pi16(sum1, _mm_mulhi_pi16(p2, k5));
			sum2 = _mm_add_pi16(sum2, _mm_mulhi_pi16(p2, k8));
			sum0 = _mm_add_pi16(sum0, off0);
			sum1 = _mm_add_pi16(sum1, off1);
			sum2 = _mm_add_pi16(sum2, off2);
			sum0 = _mm_srai_pi16(sum0, shift);
			sum1 = _mm_srai_pi16(sum1, shift);
			sum2 = _mm_srai_pi16(sum2, shift);

			r01 = _mm_packs_pu16(sum0, sum1);
			r2x = _mm_packs_pu16(sum2, zero);
			ra = _mm_unpacklo_pi8(r01, r2x);
			rb = _mm_unpackhi_pi8(r01, zero);
			rc = _mm_unpacklo_pi8(ra, rb);
			rd = _mm_unpackhi_pi8(ra, rb);

			da[0] = ((mlib_u8 *)&rc)[0];
			da[1] = ((mlib_u8 *)&rc)[1];
			da[2] = ((mlib_u8 *)&rc)[2];
			if (tail > 1) {
				da[3] = ((mlib_u8 *)&rc)[4];
				da[4] = ((mlib_u8 *)&rc)[5];
				da[5] = ((mlib_u8 *)&rc)[6];
			}
			if (tail > 2) {
				da[6] = ((mlib_u8 *)&rd)[0];
				da[7] = ((mlib_u8 *)&rd)[1];
				da[8] = ((mlib_u8 *)&rd)[2];
			}
			if (tail > 3) {
				da[9] = ((mlib_u8 *)&rd)[4];
				da[10] = ((mlib_u8 *)&rd)[5];
				da[11] = ((mlib_u8 *)&rd)[6];
			}
		}
#endif
		sl = sa = sl + slb;
		dl = da = dl + dlb;
	}

	_mm_empty();

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_m_ImageColorConvert2_S16(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *fmat,
    const mlib_d64 *offset,
    mlib_s32 scale)
{
	const mlib_s16 *sl, *sa;
	mlib_s16 *dl, *da;
	mlib_s32 i, j, shift, xsize4, tmp0, tmp1, tmp2;
	__m64 k0, k3, k6, off0, off1, off2;
	__m64 s0, s1, s2, s3, t0, t1, t2, t3, p0, p1, p2, p3;
	mlib_d64 scalef;

	shift = 16 - scale;
	scalef = 1 << shift;

	MLIB_RINT1_S16(k, 0, fmat, scalef);
	MLIB_RINT1_S16(k, 3, fmat, scalef);
	MLIB_RINT1_S16(k, 6, fmat, scalef);

	if (offset[0] >= 0)
		tmp0 = (mlib_s32)(offset[0] * scalef + 0.5);
	else
		tmp0 = (mlib_s32)(offset[0] * scalef - 0.5);

	if (offset[1] >= 0)
		tmp1 = (mlib_s32)(offset[1] * scalef + 0.5);
	else
		tmp1 = (mlib_s32)(offset[1] * scalef - 0.5);

	if (offset[2] >= 0)
		tmp2 = (mlib_s32)(offset[2] * scalef + 0.5);
	else
		tmp2 = (mlib_s32)(offset[2] * scalef - 0.5);

	((mlib_s32 *)&off0)[0] = tmp0;
	((mlib_s32 *)&off0)[1] = tmp1;

	((mlib_s32 *)&off1)[0] = tmp2;
	((mlib_s32 *)&off1)[1] = tmp0;

	((mlib_s32 *)&off2)[0] = tmp1;
	((mlib_s32 *)&off2)[1] = tmp2;

	xsize4 = (xsize + 3) / 4;

	sa = sl = src;
	da = dl = dst;

	for (j = 0; j < ysize; j++) {
		mlib_s16 *dend = dl + 3 * xsize;
		__m64 tail0, tail1, tail2;

		tail0 = ((__m64 *) dend)[0];
		tail1 = ((__m64 *) dend)[1];
		tail2 = ((__m64 *) dend)[2];

		for (i = 0; i < xsize4; i++) {
			s0 = *(__m64 *) (sa);
			s1 = *(__m64 *) (sa + 3);
			s2 = *(__m64 *) (sa + 6);
			s3 = *(__m64 *) (sa + 9);
			sa += 12;

			t0 = _mm_madd_pi16(s0, k0);
			t1 = _mm_madd_pi16(s0, k3);
			t2 = _mm_madd_pi16(s0, k6);
			t3 = _mm_madd_pi16(s1, k0);

			p0 = _mm_unpackhi_pi32(t0, t1);
			p1 = _mm_unpacklo_pi32(t0, t1);
			p2 = _mm_unpackhi_pi32(t2, t3);
			p3 = _mm_unpacklo_pi32(t2, t3);

			t0 = _mm_add_pi32(p0, p1);
			t1 = _mm_add_pi32(p2, p3);

			t0 = _mm_add_pi32(t0, off0);
			t1 = _mm_add_pi32(t1, off1);

			p0 = _mm_srai_pi32(t0, shift);
			p1 = _mm_srai_pi32(t1, shift);

			*(__m64 *) (da) = _mm_packs_pi32(p0, p1);

			t0 = _mm_madd_pi16(s1, k3);
			t1 = _mm_madd_pi16(s1, k6);
			t2 = _mm_madd_pi16(s2, k0);
			t3 = _mm_madd_pi16(s2, k3);

			p0 = _mm_unpackhi_pi32(t0, t1);
			p1 = _mm_unpacklo_pi32(t0, t1);
			p2 = _mm_unpackhi_pi32(t2, t3);
			p3 = _mm_unpacklo_pi32(t2, t3);

			t0 = _mm_add_pi32(p0, p1);
			t1 = _mm_add_pi32(p2, p3);

			t0 = _mm_add_pi32(t0, off2);
			t1 = _mm_add_pi32(t1, off0);

			p0 = _mm_srai_pi32(t0, shift);
			p1 = _mm_srai_pi32(t1, shift);

			*(__m64 *) (da + 4) = _mm_packs_pi32(p0, p1);

			t0 = _mm_madd_pi16(s2, k6);
			t1 = _mm_madd_pi16(s3, k0);
			t2 = _mm_madd_pi16(s3, k3);
			t3 = _mm_madd_pi16(s3, k6);

			p0 = _mm_unpackhi_pi32(t0, t1);
			p1 = _mm_unpacklo_pi32(t0, t1);
			p2 = _mm_unpackhi_pi32(t2, t3);
			p3 = _mm_unpacklo_pi32(t2, t3);

			t0 = _mm_add_pi32(p0, p1);
			t1 = _mm_add_pi32(p2, p3);

			t0 = _mm_add_pi32(t0, off1);
			t1 = _mm_add_pi32(t1, off2);

			p0 = _mm_srai_pi32(t0, shift);
			p1 = _mm_srai_pi32(t1, shift);

			*(__m64 *) (da + 8) = _mm_packs_pi32(p0, p1);

			da += 12;
		}

		((__m64 *) dend)[0] = tail0;
		((__m64 *) dend)[1] = tail1;
		((__m64 *) dend)[2] = tail2;

		sl = sa = sl + slb;
		dl = da = dl + dlb;
	}

	_mm_empty();

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_m_ImageColorConvert2_U16(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    const mlib_d64 *fmat,
    const mlib_d64 *offset,
    mlib_s32 scale)
{
	const mlib_u16 *sl, *sa;
	mlib_u16 *dl, *da;
	mlib_s32 i, j, shift, xsize4, tmp0, tmp1, tmp2;
	__m64 k0, k3, k6, off0, off1, off2;
	__m64 s0, s1, s2, s3, t0, t1, t2, t3, p0, p1, p2, p3;
	__m64 mask_8000;
	mlib_d64 scalef;

	shift = 16 - scale;
	scalef = 1 << shift;

	MLIB_RINT1_S16(k, 0, fmat, scalef);
	MLIB_RINT1_S16(k, 3, fmat, scalef);
	MLIB_RINT1_S16(k, 6, fmat, scalef);

	if (offset[0] >= 0)
		tmp0 = (mlib_s32)(offset[0] * scalef + 0.5);
	else
		tmp0 = (mlib_s32)(offset[0] * scalef - 0.5);

	if (offset[1] >= 0)
		tmp1 = (mlib_s32)(offset[1] * scalef + 0.5);
	else
		tmp1 = (mlib_s32)(offset[1] * scalef - 0.5);

	if (offset[2] >= 0)
		tmp2 = (mlib_s32)(offset[2] * scalef + 0.5);
	else
		tmp2 = (mlib_s32)(offset[2] * scalef - 0.5);

	((mlib_s32 *)&off0)[0] = tmp0;
	((mlib_s32 *)&off0)[1] = tmp1;

	((mlib_s32 *)&off1)[0] = tmp2;
	((mlib_s32 *)&off1)[1] = tmp0;

	((mlib_s32 *)&off2)[0] = tmp1;
	((mlib_s32 *)&off2)[1] = tmp2;

	xsize4 = (xsize + 3) / 4;

	sa = sl = src;
	da = dl = dst;

	mask_8000 = _m_from_int(0x80008000);
	mask_8000 = _m_por(mask_8000, _mm_slli_si64(mask_8000, 32));

	for (j = 0; j < ysize; j++) {
		mlib_u16 *dend = dl + 3 * xsize;
		__m64 tail0, tail1, tail2;

		tail0 = ((__m64 *) dend)[0];
		tail1 = ((__m64 *) dend)[1];
		tail2 = ((__m64 *) dend)[2];

		for (i = 0; i < xsize4; i++) {
/*
 * s0 = *(__m64*)(sa);
 *      s1 = *(__m64*)(sa + 3);
 *      s2 = *(__m64*)(sa + 6);
 *      s3 = *(__m64*)(sa + 9);
 *
 *      s0 = _mm_xor_si64(mask_8000, s0);
 *      s1 = _mm_xor_si64(mask_8000, s1);
 *      s2 = _mm_xor_si64(mask_8000, s2);
 * s3 = _mm_xor_si64(mask_8000, s3);
 */

			s0 = _mm_xor_si64(mask_8000, *(__m64 *) (sa));
			s1 = _mm_xor_si64(mask_8000, *(__m64 *) (sa + 3));
			s2 = _mm_xor_si64(mask_8000, *(__m64 *) (sa + 6));
			s3 = _mm_xor_si64(mask_8000, *(__m64 *) (sa + 9));
			sa += 12;

			t0 = _mm_madd_pi16(s0, k0);
			t1 = _mm_madd_pi16(s0, k3);
			t2 = _mm_madd_pi16(s0, k6);
			t3 = _mm_madd_pi16(s1, k0);

			p0 = _mm_unpackhi_pi32(t0, t1);
			p1 = _mm_unpacklo_pi32(t0, t1);
			p2 = _mm_unpackhi_pi32(t2, t3);
			p3 = _mm_unpacklo_pi32(t2, t3);

			t0 = _mm_add_pi32(p0, p1);
			t1 = _mm_add_pi32(p2, p3);

			t0 = _mm_add_pi32(t0, off0);
			t1 = _mm_add_pi32(t1, off1);

			p0 = _mm_srai_pi32(t0, shift);
			p1 = _mm_srai_pi32(t1, shift);

			*(__m64 *) (da) =
			    _mm_xor_si64(mask_8000, _mm_packs_pi32(p0, p1));

			t0 = _mm_madd_pi16(s1, k3);
			t1 = _mm_madd_pi16(s1, k6);
			t2 = _mm_madd_pi16(s2, k0);
			t3 = _mm_madd_pi16(s2, k3);

			p0 = _mm_unpackhi_pi32(t0, t1);
			p1 = _mm_unpacklo_pi32(t0, t1);
			p2 = _mm_unpackhi_pi32(t2, t3);
			p3 = _mm_unpacklo_pi32(t2, t3);

			t0 = _mm_add_pi32(p0, p1);
			t1 = _mm_add_pi32(p2, p3);

			t0 = _mm_add_pi32(t0, off2);
			t1 = _mm_add_pi32(t1, off0);

			p0 = _mm_srai_pi32(t0, shift);
			p1 = _mm_srai_pi32(t1, shift);

			*(__m64 *) (da + 4) =
			    _mm_xor_si64(mask_8000, _mm_packs_pi32(p0, p1));

			t0 = _mm_madd_pi16(s2, k6);
			t1 = _mm_madd_pi16(s3, k0);
			t2 = _mm_madd_pi16(s3, k3);
			t3 = _mm_madd_pi16(s3, k6);

			p0 = _mm_unpackhi_pi32(t0, t1);
			p1 = _mm_unpacklo_pi32(t0, t1);
			p2 = _mm_unpackhi_pi32(t2, t3);
			p3 = _mm_unpacklo_pi32(t2, t3);

			t0 = _mm_add_pi32(p0, p1);
			t1 = _mm_add_pi32(p2, p3);

			t0 = _mm_add_pi32(t0, off1);
			t1 = _mm_add_pi32(t1, off2);

			p0 = _mm_srai_pi32(t0, shift);
			p1 = _mm_srai_pi32(t1, shift);

			*(__m64 *) (da + 8) =
			    _mm_xor_si64(mask_8000, _mm_packs_pi32(p0, p1));

			da += 12;
		}

		((__m64 *) dend)[0] = tail0;
		((__m64 *) dend)[1] = tail1;
		((__m64 *) dend)[2] = tail2;

		sl = sa = sl + slb;
		dl = da = dl + dlb;
	}

	_mm_empty();

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
