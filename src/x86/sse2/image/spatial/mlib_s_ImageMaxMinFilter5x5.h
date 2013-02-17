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

#ifndef _MLIB_S_IMAGEMAXMINFILTER5X5_H
#define	_MLIB_S_IMAGEMAXMINFILTER5X5_H

#pragma ident	"@(#)mlib_s_ImageMaxMinFilter5x5.h	9.3	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

/* *********************************************************** */

static const
#if defined(__ICL) || defined(__INTEL_COMPILER)
_declspec(align(16))
#elif __SUNPRO_C
#pragma align 16(mlib_mask128i_arr)
#endif /* defined(__ICL) || defined (__INTEL_COMPILER) */
unsigned int mlib_mask128i_arr[] = {
	0x00000000, 0x00000000, 0x00000000, 0x00000000,
	0x000000FF, 0x00000000, 0x00000000, 0x00000000,
	0x0000FFFF, 0x00000000, 0x00000000, 0x00000000,
	0x00FFFFFF, 0x00000000, 0x00000000, 0x00000000,
	0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000,
	0xFFFFFFFF, 0x000000FF, 0x00000000, 0x00000000,
	0xFFFFFFFF, 0x0000FFFF, 0x00000000, 0x00000000,
	0xFFFFFFFF, 0x00FFFFFF, 0x00000000, 0x00000000,
	0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000,
	0xFFFFFFFF, 0xFFFFFFFF, 0x000000FF, 0x00000000,
	0xFFFFFFFF, 0xFFFFFFFF, 0x0000FFFF, 0x00000000,
	0xFFFFFFFF, 0xFFFFFFFF, 0x00FFFFFF, 0x00000000,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x000000FF,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0000FFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00FFFFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
};

/* *********************************************************** */

#if MAX_FILTER

#define	C_COMP_T(a, b, type, maska, maskb)                           \
	_mm_add_epi##type(a, _mm_and_si128(_mm_sub_epi##type(b, a),  \
	    _mm_cmpgt_epi##type(maskb, maska)))
#define	F_NAMET(TYPE)   mlib_ImageMaxFilter5x5_##TYPE

#else

#define	C_COMP_T(a, b, type, maska, maskb)                           \
	_mm_add_epi##type(a, _mm_and_si128(_mm_sub_epi##type(b, a),  \
	    _mm_cmpgt_epi##type(maska, maskb)))
#define	F_NAMET(TYPE)   mlib_ImageMinFilter5x5_##TYPE

#endif /* MAX_FILTER */

/* *********************************************************** */

#if IMG_TYPE == 1  /* MLIB_BYTE */

#define	F_NAME	F_NAMET(U8)
#define	STYPE	mlib_u8
#define	MASK  0x80808080
#define	C_COMP(a, b)                                                 \
	C_COMP_T(a, b, 8, _mm_xor_si128(a, mask80),                  \
		_mm_xor_si128(b, mask80))                            \

#elif IMG_TYPE == 2	/* IMG_TYPE == 1 ( MLIB_SHORT ) */

#define	F_NAME	F_NAMET(S16)
#define	STYPE	mlib_s16
#define	MASK  0x0
#define	C_COMP(a, b)  C_COMP_T(a, b, 16, a, b)

#elif IMG_TYPE == 3	/* IMG_TYPE == 1 ( MLIB_USHORT ) */

#define	F_NAME	F_NAMET(U16)
#define	STYPE	mlib_u16
#define	MASK  0x80008000
#define	C_COMP(a, b)                                                 \
	C_COMP_T(a, b, 16, _mm_xor_si128(a, mask80),                 \
		_mm_xor_si128(b, mask80))                            \

#elif IMG_TYPE == 4	/* IMG_TYPE == 1 ( MLIB_INT ) */

#define	F_NAME	F_NAMET(S32)
#define	STYPE	mlib_s32
#define	MASK  0x0
#define	C_COMP(a, b)  C_COMP_T(a, b, 32, a, b)

#endif		   /* IMG_TYPE == 1 ( MLIB_BYTE ) */

/* *********************************************************** */
#define	SSIZE	sizeof (STYPE)
#define	KSIZE1  (5 - 1)
/* *********************************************************** */

mlib_status
F_NAME(
    void *dst,
    void *src,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 wid,
    mlib_s32 hgt)
{
	mlib_u8 *pbuff, *buff0, *buff1, *buff2, *buff3, *buffT;
	mlib_u8 *sl, *sp0, *sp1, *sp2, *sp3, *sp4, *sp5, *dl;
	__m128i *dp0, *dp1;
	__m128i aa, bb, cc, dd, e0, e1, e2, e3, e4, ee, f0, f1, f2, f3, f4, ff,
	    r0, r1, t0, t1;
	__m128i e_mask, mask80;
	mlib_s32 i, j, wid16, tail;

	wid = (wid - KSIZE1) * SSIZE;
	wid16 = (wid + 15) & ~15;
	pbuff = __mlib_malloc(4 * wid16);
	buff0 = pbuff;
	buff1 = buff0 + wid16;
	buff2 = buff1 + wid16;
	buff3 = buff2 + wid16;

	sl = (mlib_u8 *)src;
	dl = (mlib_u8 *)dst + 2 * (dlb + SSIZE);

	tail = wid & 15;
	e_mask = _mm_loadu_si128(((__m128i *) mlib_mask128i_arr) + tail);
	mask80 = _mm_set1_epi32(MASK);

	for (j = 0; j < 2; j++) {
		sp0 = buff0;
		sp1 = buff1;
		sp4 = sl;
		sp5 = sl + slb;
		sl += 2 * slb;

		for (i = 0; i < wid; i += 16) {
			e0 = _mm_loadu_si128((__m128i *)sp4);
			e1 = _mm_loadu_si128((__m128i *)(sp4 + SSIZE));
			e2 = _mm_loadu_si128((__m128i *)(sp4 + 2 * SSIZE));
			e3 = _mm_loadu_si128((__m128i *)(sp4 + 3 * SSIZE));
			e4 = _mm_loadu_si128((__m128i *)(sp4 + 4 * SSIZE));
			f0 = _mm_loadu_si128((__m128i *)sp5);
			f1 = _mm_loadu_si128((__m128i *)(sp5 + SSIZE));
			f2 = _mm_loadu_si128((__m128i *)(sp5 + 2 * SSIZE));
			f3 = _mm_loadu_si128((__m128i *)(sp5 + 3 * SSIZE));
			f4 = _mm_loadu_si128((__m128i *)(sp5 + 4 * SSIZE));

			ee = C_COMP(e0, e1);
			ff = C_COMP(f0, f1);
			e2 = C_COMP(e2, e3);
			f2 = C_COMP(f2, f3);
			ee = C_COMP(ee, e4);
			ff = C_COMP(ff, f4);
			ee = C_COMP(ee, e2);
			ff = C_COMP(ff, f2);

			_mm_storeu_si128((__m128i *)sp0, ee);
			_mm_storeu_si128((__m128i *)sp1, ff);

			sp0 += 16;
			sp1 += 16;
			sp4 += 16;
			sp5 += 16;
		}

		buffT = buff0;
		buff0 = buff2;
		buff2 = buffT;
		buffT = buff1;
		buff1 = buff3;
		buff3 = buffT;
	}

	for (j = 0; j <= (hgt - KSIZE1 - 2); j += 2) {
		dp0 = (void *)dl;
		dp1 = (void *)(dl + dlb);
		sp0 = buff0;
		sp1 = buff1;
		sp2 = buff2;
		sp3 = buff3;
		sp4 = sl;
		sp5 = sl + slb;

/*
 *    line0:        aa
 *    line1:        bb
 *    line2:        cc
 *    line3:        dd
 *    line4:  e0 e1 e2 e3 e4
 *    line5:  f0 f1 f2 f3 f4
 */

		for (i = 0; i <= wid - 16; i += 16) {
			aa = _mm_loadu_si128((__m128i *)sp0);
			bb = _mm_loadu_si128((__m128i *)sp1);
			cc = _mm_loadu_si128((__m128i *)sp2);
			dd = _mm_loadu_si128((__m128i *)sp3);
			e0 = _mm_loadu_si128((__m128i *)sp4);
			e1 = _mm_loadu_si128((__m128i *)(sp4 + SSIZE));
			e2 = _mm_loadu_si128((__m128i *)(sp4 + 2 * SSIZE));
			e3 = _mm_loadu_si128((__m128i *)(sp4 + 3 * SSIZE));
			e4 = _mm_loadu_si128((__m128i *)(sp4 + 4 * SSIZE));
			f0 = _mm_loadu_si128((__m128i *)sp5);
			f1 = _mm_loadu_si128((__m128i *)(sp5 + SSIZE));
			f2 = _mm_loadu_si128((__m128i *)(sp5 + 2 * SSIZE));
			f3 = _mm_loadu_si128((__m128i *)(sp5 + 3 * SSIZE));
			f4 = _mm_loadu_si128((__m128i *)(sp5 + 4 * SSIZE));

			ee = C_COMP(e0, e1);
			ff = C_COMP(f0, f1);
			e2 = C_COMP(e2, e3);
			f2 = C_COMP(f2, f3);
			ee = C_COMP(ee, e4);
			ff = C_COMP(ff, f4);
			ee = C_COMP(ee, e2);
			ff = C_COMP(ff, f2);

			bb = C_COMP(bb, cc);
			dd = C_COMP(dd, ee);
			bb = C_COMP(bb, dd);

			r0 = C_COMP(aa, bb);
			r1 = C_COMP(bb, ff);

			_mm_storeu_si128((__m128i *)sp0, ee);
			_mm_storeu_si128((__m128i *)sp1, ff);

			_mm_storeu_si128(dp0, r0);
			dp0++;
			_mm_storeu_si128(dp1, r1);
			dp1++;

			sp0 += 16;
			sp1 += 16;
			sp2 += 16;
			sp3 += 16;
			sp4 += 16;
			sp5 += 16;
		}

		if (tail) {
			aa = _mm_loadu_si128((__m128i *)sp0);
			bb = _mm_loadu_si128((__m128i *)sp1);
			cc = _mm_loadu_si128((__m128i *)sp2);
			dd = _mm_loadu_si128((__m128i *)sp3);
			e0 = _mm_loadu_si128((__m128i *)sp4);
			e1 = _mm_loadu_si128((__m128i *)(sp4 + SSIZE));
			e2 = _mm_loadu_si128((__m128i *)(sp4 + 2 * SSIZE));
			e3 = _mm_loadu_si128((__m128i *)(sp4 + 3 * SSIZE));
			e4 = _mm_loadu_si128((__m128i *)(sp4 + 4 * SSIZE));
			f0 = _mm_loadu_si128((__m128i *)sp5);
			f1 = _mm_loadu_si128((__m128i *)(sp5 + SSIZE));
			f2 = _mm_loadu_si128((__m128i *)(sp5 + 2 * SSIZE));
			f3 = _mm_loadu_si128((__m128i *)(sp5 + 3 * SSIZE));
			f4 = _mm_loadu_si128((__m128i *)(sp5 + 4 * SSIZE));

			ee = C_COMP(e0, e1);
			ff = C_COMP(f0, f1);
			e2 = C_COMP(e2, e3);
			f2 = C_COMP(f2, f3);
			ee = C_COMP(ee, e4);
			ff = C_COMP(ff, f4);
			ee = C_COMP(ee, e2);
			ff = C_COMP(ff, f2);

			bb = C_COMP(bb, cc);
			dd = C_COMP(dd, ee);
			bb = C_COMP(bb, dd);

			r0 = C_COMP(aa, bb);
			r1 = C_COMP(bb, ff);

			_mm_storeu_si128((__m128i *)sp0, ee);
			_mm_storeu_si128((__m128i *)sp1, ff);

			t0 = _mm_loadu_si128(dp0);
			t1 = _mm_loadu_si128(dp1);
			t0 =
			    _mm_or_si128(_mm_and_si128(e_mask, r0),
			    _mm_andnot_si128(e_mask, t0));
			t1 =
			    _mm_or_si128(_mm_and_si128(e_mask, r1),
			    _mm_andnot_si128(e_mask, t1));
			_mm_storeu_si128(dp0, t0);
			_mm_storeu_si128(dp1, t1);
		}

		buffT = buff0;
		buff0 = buff2;
		buff2 = buffT;
		buffT = buff1;
		buff1 = buff3;
		buff3 = buffT;

		sl += 2 * slb;
		dl += 2 * dlb;
	}

/* last line */

	if (j == (hgt - KSIZE1 - 1)) {
		dp0 = (void *)dl;
		dp1 = (void *)(dl + dlb);
		sp0 = buff0;
		sp1 = buff1;
		sp2 = buff2;
		sp3 = buff3;
		sp4 = sl;

		for (i = 0; i <= wid - 16; i += 16) {
			aa = _mm_loadu_si128((__m128i *)sp0);
			bb = _mm_loadu_si128((__m128i *)sp1);
			cc = _mm_loadu_si128((__m128i *)sp2);
			dd = _mm_loadu_si128((__m128i *)sp3);
			e0 = _mm_loadu_si128((__m128i *)sp4);
			e1 = _mm_loadu_si128((__m128i *)(sp4 + SSIZE));
			e2 = _mm_loadu_si128((__m128i *)(sp4 + 2 * SSIZE));
			e3 = _mm_loadu_si128((__m128i *)(sp4 + 3 * SSIZE));
			e4 = _mm_loadu_si128((__m128i *)(sp4 + 4 * SSIZE));

			ee = C_COMP(e0, e1);
			e2 = C_COMP(e2, e3);
			ee = C_COMP(ee, e4);
			ee = C_COMP(ee, e2);

			bb = C_COMP(bb, cc);
			dd = C_COMP(dd, ee);
			bb = C_COMP(bb, dd);

			r0 = C_COMP(aa, bb);

			_mm_storeu_si128(dp0, r0);
			dp0++;

			sp0 += 16;
			sp1 += 16;
			sp2 += 16;
			sp3 += 16;
			sp4 += 16;
		}

		if (tail) {
			aa = _mm_loadu_si128((__m128i *)sp0);
			bb = _mm_loadu_si128((__m128i *)sp1);
			cc = _mm_loadu_si128((__m128i *)sp2);
			dd = _mm_loadu_si128((__m128i *)sp3);
			e0 = _mm_loadu_si128((__m128i *)sp4);
			e1 = _mm_loadu_si128((__m128i *)(sp4 + SSIZE));
			e2 = _mm_loadu_si128((__m128i *)(sp4 + 2 * SSIZE));
			e3 = _mm_loadu_si128((__m128i *)(sp4 + 3 * SSIZE));
			e4 = _mm_loadu_si128((__m128i *)(sp4 + 4 * SSIZE));

			ee = C_COMP(e0, e1);
			e2 = C_COMP(e2, e3);
			ee = C_COMP(ee, e4);
			ee = C_COMP(ee, e2);

			bb = C_COMP(bb, cc);
			dd = C_COMP(dd, ee);
			bb = C_COMP(bb, dd);

			r0 = C_COMP(aa, bb);

			t0 = _mm_loadu_si128(dp0);
			t0 =
			    _mm_or_si128(_mm_and_si128(e_mask, r0),
			    _mm_andnot_si128(e_mask, t0));
			_mm_storeu_si128(dp0, t0);
		}
	}

	__mlib_free(pbuff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_S_IMAGEMAXMINFILTER5X5_H */
