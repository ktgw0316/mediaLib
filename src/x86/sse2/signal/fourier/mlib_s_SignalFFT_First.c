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

#pragma ident	"@(#)mlib_s_SignalFFT_First.c	9.4	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_v_fftK_3LayersFirst[|_nod[1|3]]
 *                                         - static function performing 3-layers
 *                                           FFT processing at first 3 layers
 *                                           where it is more simplier than
 *                                           general case
 *
 *
 * SYNOPSIS
 *      static void mlib_v_fftK_3LayersFirst
 *                  (mlib_s16 *CONST dat_r,
 *                   mlib_s16 *CONST dat_i,
 *                   mlib_s32 CONST order,
 *                   mlib_s32 CONST isign
 *                  );
 *
 *
 * ARGUMENTS
 *      dat_r
 *              - 64bit-aligned S16 array. Created dynamically inside
 *                mlib_fftDispatcherW ( from stack or __mlib_malloc ).
 *                Source and destination for kernel FFT function.
 *                pal64data_r[ i ], i = 0, ..., ( 1 << order )
 *                hold the real parts
 *
 *      dat_i
 *              - 64bit-aligned S16 array. Created dynamically inside
 *                mlib_fftDispatcherW ( from stack or __mlib_malloc ).
 *                Source and destination for kernel FFT function.
 *                pal64data_i[ i ], i = 0, ..., ( 1 << order )
 *                hold the imag parts
 *
 *      order   - Order of the transformation. The base-2 logarithm of the
 *                number of data samples
 *
 *      isign   - Controls type of the transformation - direct or inverse.
 *                Either DIRECT_FFT or INVERS_FFT values.
 */

#include <mlib_signal.h>
#include <mlib_s_sse_int.h>
#include <mlib_s_SignalFFTUtil.h>
#include <mlib_s_SignalFFTDisp.h>
#include <mlib_s_SignalFFT_Tbl.h>

/* *********************************************************** */

typedef union
{
	mlib_d64 value;
	struct
	{
		mlib_s32 int0, int1;
	} twoint;
} type_mlib_d64;

/* *********************************************************** */

/* used in _3LayersFirst to store results */
#define	TRANSPOSE4X4_SSE2(i0, i1, i2, i3, i4, i5, i6, i7, p0)          \
	{                                                              \
	    __m128i i02, i13, i0213;                                  \
 \
	    short swap[8 * 8]; \
 \
		_mm_storeu_si128((__m128i *)(swap + 0 * 8), i0); \
		_mm_storeu_si128((__m128i *)(swap + 1 * 8), i1); \
		_mm_storeu_si128((__m128i *)(swap + 2 * 8), i2); \
		_mm_storeu_si128((__m128i *)(swap + 3 * 8), i3); \
		_mm_storeu_si128((__m128i *)(swap + 4 * 8), i4); \
		_mm_storeu_si128((__m128i *)(swap + 5 * 8), i5); \
		_mm_storeu_si128((__m128i *)(swap + 6 * 8), i6); \
		_mm_storeu_si128((__m128i *)(swap + 7 * 8), i7); \
		p0[32 + 0] = swap[4 + 0 * 8]; \
		p0[32 + 1] = swap[4 + 1 * 8]; \
		p0[32 + 2] = swap[4 + 2 * 8]; \
		p0[32 + 3] = swap[4 + 3 * 8]; \
		p0[32 + 4] = swap[4 + 4 * 8]; \
		p0[32 + 5] = swap[4 + 5 * 8]; \
		p0[32 + 6] = swap[4 + 6 * 8]; \
		p0[32 + 7] = swap[4 + 7 * 8]; \
		p0[32 + 16] = swap[5 + 0 * 8]; \
		p0[32 + 16 + 1] = swap[5 + 1 * 8]; \
		p0[32 + 16 + 2] = swap[5 + 2 * 8]; \
		p0[32 + 16 + 3] = swap[5 + 3 * 8]; \
		p0[32 + 16 + 4] = swap[5 + 4 * 8]; \
		p0[32 + 16 + 5] = swap[5 + 5 * 8]; \
		p0[32 + 16 + 6] = swap[5 + 6 * 8]; \
		p0[32 + 16 + 7] = swap[5 + 7 * 8]; \
		p0[32 + 8] = swap[6 + 0 * 8]; \
		p0[32 + 8 + 1] = swap[6 + 1 * 8]; \
		p0[32 + 8 + 2] = swap[6 + 2 * 8]; \
		p0[32 + 8 + 3] = swap[6 + 3 * 8]; \
		p0[32 + 8 + 4] = swap[6 + 4 * 8]; \
		p0[32 + 8 + 5] = swap[6 + 5 * 8]; \
		p0[32 + 8 + 6] = swap[6 + 6 * 8]; \
		p0[32 + 8 + 7] = swap[6 + 7 * 8]; \
		p0[32 + 24] = swap[7 + 0 * 8]; \
		p0[32 + 24 + 1] = swap[7 + 1 * 8]; \
		p0[32 + 24 + 2] = swap[7 + 2 * 8]; \
		p0[32 + 24 + 3] = swap[7 + 3 * 8]; \
		p0[32 + 24 + 4] = swap[7 + 4 * 8]; \
		p0[32 + 24 + 5] = swap[7 + 5 * 8]; \
		p0[32 + 24 + 6] = swap[7 + 6 * 8]; \
		p0[32 + 24 + 7] = swap[7 + 7 * 8]; \
	                                                               \
		p0[0] = swap[0 + 0 * 8]; \
		p0[1] = swap[0 + 1 * 8]; \
		p0[2] = swap[0 + 2 * 8]; \
		p0[3] = swap[0 + 3 * 8]; \
		p0[4] = swap[0 + 4 * 8]; \
		p0[5] = swap[0 + 5 * 8]; \
		p0[6] = swap[0 + 6 * 8]; \
		p0[7] = swap[0 + 7 * 8]; \
		p0[16] = swap[1 + 0 * 8]; \
		p0[16 + 1] = swap[1 + 1 * 8]; \
		p0[16 + 2] = swap[1 + 2 * 8]; \
		p0[16 + 3] = swap[1 + 3 * 8]; \
		p0[16 + 4] = swap[1 + 4 * 8]; \
		p0[16 + 5] = swap[1 + 5 * 8]; \
		p0[16 + 6] = swap[1 + 6 * 8]; \
		p0[16 + 7] = swap[1 + 7 * 8]; \
		p0[8] = swap[2 + 0 * 8]; \
		p0[8 + 1] = swap[2 + 1 * 8]; \
		p0[8 + 2] = swap[2 + 2 * 8]; \
		p0[8 + 3] = swap[2 + 3 * 8]; \
		p0[8 + 4] = swap[2 + 4 * 8]; \
		p0[8 + 5] = swap[2 + 5 * 8]; \
		p0[8 + 6] = swap[2 + 6 * 8]; \
		p0[8 + 7] = swap[2 + 7 * 8]; \
		p0[24] = swap[3 + 0 * 8]; \
		p0[24 + 1] = swap[3 + 1 * 8]; \
		p0[24 + 2] = swap[3 + 2 * 8]; \
		p0[24 + 3] = swap[3 + 3 * 8]; \
		p0[24 + 4] = swap[3 + 4 * 8]; \
		p0[24 + 5] = swap[3 + 5 * 8]; \
		p0[24 + 6] = swap[3 + 6 * 8]; \
		p0[24 + 7] = swap[3 + 7 * 8]; \
\
	}

#define	TRANSPOSE4X4_SSE2_ORDER5(i0, i1, i2, i3, i4, i5, i6, i7, p0)          \
	{                                                              \
	    __m128i i02, i13, i0213;                                  \
 \
	    short swap[8 * 8]; \
 \
		_mm_storeu_si128((__m128i *)(swap + 0 * 8), i0); \
		_mm_storeu_si128((__m128i *)(swap + 1 * 8), i1); \
		_mm_storeu_si128((__m128i *)(swap + 2 * 8), i2); \
		_mm_storeu_si128((__m128i *)(swap + 3 * 8), i3); \
		_mm_storeu_si128((__m128i *)(swap + 4 * 8), i4); \
		_mm_storeu_si128((__m128i *)(swap + 5 * 8), i5); \
		_mm_storeu_si128((__m128i *)(swap + 6 * 8), i6); \
		_mm_storeu_si128((__m128i *)(swap + 7 * 8), i7); \
		p0[0] = swap[4 + 0 * 8]; \
		p0[1] = swap[4 + 1 * 8]; \
		p0[2] = swap[4 + 2 * 8]; \
		p0[3] = swap[4 + 3 * 8]; \
		p0[4] = swap[4 + 4 * 8]; \
		p0[5] = swap[4 + 5 * 8]; \
		p0[6] = swap[4 + 6 * 8]; \
		p0[7] = swap[4 + 7 * 8]; \
		p0[16] = swap[5 + 0 * 8]; \
		p0[16 + 1] = swap[5 + 1 * 8]; \
		p0[16 + 2] = swap[5 + 2 * 8]; \
		p0[16 + 3] = swap[5 + 3 * 8]; \
		p0[16 + 4] = swap[5 + 4 * 8]; \
		p0[16 + 5] = swap[5 + 5 * 8]; \
		p0[16 + 6] = swap[5 + 6 * 8]; \
		p0[16 + 7] = swap[5 + 7 * 8]; \
		p0[8] = swap[6 + 0 * 8]; \
		p0[8 + 1] = swap[6 + 1 * 8]; \
		p0[8 + 2] = swap[6 + 2 * 8]; \
		p0[8 + 3] = swap[6 + 3 * 8]; \
		p0[8 + 4] = swap[6 + 4 * 8]; \
		p0[8 + 5] = swap[6 + 5 * 8]; \
		p0[8 + 6] = swap[6 + 6 * 8]; \
		p0[8 + 7] = swap[6 + 7 * 8]; \
		p0[24] = swap[7 + 0 * 8]; \
		p0[24 + 1] = swap[7 + 1 * 8]; \
		p0[24 + 2] = swap[7 + 2 * 8]; \
		p0[24 + 3] = swap[7 + 3 * 8]; \
		p0[24 + 4] = swap[7 + 4 * 8]; \
		p0[24 + 5] = swap[7 + 5 * 8]; \
		p0[24 + 6] = swap[7 + 6 * 8]; \
		p0[24 + 7] = swap[7 + 7 * 8]; \
	}


/* *********************************************************** */

/* used in _3LayersFirst to load and preprocess data */
#define	READ_DEAL1_SSE2(part, a, b, addr)                            \
	{                                                       \
	    __m128i xr;                                        \
	                                                        \
	    a##part = _mm_loadu_si128((__m128i *)(p##part + 2 * addr)); \
	    b##part = _mm_loadu_si128((__m128i *)(p##part + 2 * addr + 8));   \
	    xr = _mm_fmul8x16_vis(a##part, fdivider2);              \
	    b##part = _mm_fmul8x16_vis(b##part, fdivider2);         \
	    a##part = _mm_add_epi16(xr, b##part);                 \
	    b##part = _mm_sub_epi16(xr, b##part);                 \
	}

/* used in _3LayersFirst to load and preprocess data */
#define	READ_DEAL1_SSE2_ORDER5(part, a, b, addr)                            \
	{                                                       \
	    __m128i xr, tmp;                                        \
	                                                        \
	    tmp = _mm_loadu_si128((__m128i *)(p##part + addr)); \
	    a##part = _mm_unpackhi_epi64(tmp, b##part);              \
	    b##part = _mm_unpacklo_epi64(tmp, b##part);         \
	    xr = _mm_fmul8x16_vis(a##part, fdivider2);              \
	    b##part = _mm_fmul8x16_vis(b##part, fdivider2);         \
	    a##part = _mm_add_epi16(xr, b##part);                 \
	    b##part = _mm_sub_epi16(xr, b##part);                 \
	}

/* *********************************************************** */

#define	READ_DEAL1NOD_SSE2(part, a, b, addr)                    \
	{                                                       \
	    __m128i xr, tmp;                                    \
	                                                        \
	    xr = _mm_loadu_si128((__m128i *)(p##part + 2 * addr));      \
	    b##part = _mm_loadu_si128((__m128i *)(p##part + 2 * addr + 8));  \
	    a##part = _mm_add_epi16(xr, b##part);               \
	    b##part = _mm_sub_epi16(xr, b##part);               \
	}

#define	READ_DEAL1NOD_SSE2_ORDER5(part, a, b, addr)                    \
	{                                                       \
	    __m128i xr, tmp;                                    \
	                                                        \
	    tmp = _mm_loadu_si128((__m128i *)(p##part + addr));              \
	    xr = _mm_unpackhi_epi64(tmp, b##part);              \
	    b##part = _mm_unpacklo_epi64(tmp, b##part);         \
	    a##part = _mm_add_epi16(xr, b##part);               \
	    b##part = _mm_sub_epi16(xr, b##part);               \
	}

/* *********************************************************** */

#define	arout	ar
#define	brout	br
#define	crout	cr

/* *********************************************************** */

#define	erout	er
#define	frout	fr
#define	grout	gr
#define	hrout	hr
#define	aiout	ai
#define	biout	bi
#define	ciout	ci
#define	diout	di
#define	eiout	ei
#define	fiout	fi
#define	giout	gi
#define	hiout	hi
#define	bidrp	brdip
#define	bidrm	brdim

/* *********************************************************** */

/* no dividing at all */
void
mlib_s_fftK_3LayersFirst_nod3(
    mlib_s16 *CONST data_r,
    mlib_s16 *CONST data_i,
    mlib_s32 CONST order,
    mlib_s32 CONST isign)
{
	const __m128i sig_mask =
		(isign == 1) ? _mm_set1_epi32(0xffffffff) : _mm_setzero_si128();
	const __m128i sig_add1 =
	    _mm_and_si128(sig_mask, _mm_set1_epi32(0x10001));
	const __m128i k = _mm_set1_epi32(0x5A825A82);

	/* order >= 7, so count >= 2 */
	mlib_s32 count = 1 << (order - 6);
	mlib_s32 j = 0;
	mlib_s16 *pr = data_r, *pi = data_i;

#pragma pipeloop(0)
	for (; j < count; ++j) {
		__m128i ar, br, cr, dr;
		__m128i er, fr, gr, hr, ai, bi, ci, di, ei, fi, gi, hi;
		__m128i acrp, egrp, acrm, egim, egim_sig, fhip, fhip_sig, fhrm,
		    fhrmipsp, fhrmipsm;
		__m128i acip, egip, acim, egrm, egrm_sig, fhrp, fhrp_sig, fhim,
		    fhimrpsm, fhimrpsp;
		__m128i brdip, brdim;
		__m128i drout;

		READ_DEAL1NOD_SSE2(r, c, d, 8)
		    READ_DEAL1NOD_SSE2(r, a, b, 0)
		    acrp = _mm_add_epi16(ar, cr);
		acrm = _mm_sub_epi16(ar, cr);

		READ_DEAL1NOD_SSE2(r, e, f, 16)

		READ_DEAL1NOD_SSE2(r, g, h, 24)
		egrp = _mm_add_epi16(er, gr);

		arout = _mm_add_epi16(acrp, egrp);

		fr = _mm_mulhi_epi16_vis(k, fr);
		hr = _mm_mulhi_epi16_vis(k, hr);
		fr = _mm_add_epi16(fr, fr);
		hr = _mm_add_epi16(hr, hr);

		READ_DEAL1NOD_SSE2(i, c, d, 8)
		READ_DEAL1NOD_SSE2(i, e, f, 16)
		READ_DEAL1NOD_SSE2(i, g, h, 24)
		egim = _mm_sub_epi16(ei, gi);
		egim_sig =
			_mm_add_epi16(sig_add1, _mm_xor_si128(sig_mask, egim));
		crout = _mm_add_epi16(acrm, egim_sig);

		fi = _mm_mulhi_epi16_vis(k, fi);
		hi = _mm_mulhi_epi16_vis(k, hi);
		fi = _mm_add_epi16(fi, fi);
		hi = _mm_add_epi16(hi, hi);

		di = _mm_add_epi16(sig_add1, _mm_xor_si128(sig_mask, di));
		dr = _mm_add_epi16(sig_add1, _mm_xor_si128(sig_mask, dr));

		brdip = _mm_add_epi16(br, di);
		brdim = _mm_sub_epi16(br, di);

		fhip = _mm_add_epi16(fi, hi);
		fhip_sig =
			_mm_add_epi16(sig_add1, _mm_xor_si128(sig_mask, fhip));
		fhrm = _mm_sub_epi16(fr, hr);

		fhrmipsp = _mm_add_epi16(fhip_sig, fhrm);
		fhrmipsm = _mm_sub_epi16(fhip_sig, fhrm);

		brout = _mm_add_epi16(brdip, fhrmipsp);
		drout = _mm_add_epi16(brdim, fhrmipsm);

		egrm = _mm_sub_epi16(er, gr);
		erout = _mm_sub_epi16(acrp, egrp);
		grout = _mm_sub_epi16(acrm, egim_sig);
		fhrp = _mm_add_epi16(fr, hr);
		frout = _mm_sub_epi16(brdip, fhrmipsp);
		hrout = _mm_sub_epi16(brdim, fhrmipsm);

		TRANSPOSE4X4_SSE2(arout, brout, crout, drout,
				erout, frout, grout, hrout, pr)

		READ_DEAL1NOD_SSE2(i, a, b, 0)
		acip = _mm_add_epi16(ai, ci);
		egip = _mm_add_epi16(ei, gi);
		acim = _mm_sub_epi16(ai, ci);
		aiout = _mm_add_epi16(acip, egip);

		egrm_sig =
			_mm_add_epi16(sig_add1, _mm_xor_si128(sig_mask, egrm));
		ciout = _mm_sub_epi16(acim, egrm_sig);

		fhrp_sig =
			_mm_add_epi16(sig_add1, _mm_xor_si128(sig_mask, fhrp));
		fhim = _mm_sub_epi16(fi, hi);

		bidrm = _mm_sub_epi16(bi, dr);
		bidrp = _mm_add_epi16(bi, dr);
		fhimrpsm = _mm_sub_epi16(fhim, fhrp_sig);
		fhimrpsp = _mm_add_epi16(fhim, fhrp_sig);

		biout = _mm_add_epi16(bidrm, fhimrpsm);
		diout = _mm_sub_epi16(bidrp, fhimrpsp);

		pr += 64;

		eiout = _mm_sub_epi16(acip, egip);
		giout = _mm_add_epi16(acim, egrm_sig);
		fiout = _mm_sub_epi16(bidrm, fhimrpsm);
		hiout = _mm_add_epi16(bidrp, fhimrpsp);

		TRANSPOSE4X4_SSE2(aiout, biout, ciout, diout,
				eiout, fiout, giout, hiout, pi)

		pi += 64;
	}

#pragma pipeloop(1)
}

/* ********************************************************************** */

/* divide twice */
void
mlib_s_fftK_3LayersFirst_nod1(
    mlib_s16 *CONST data_r,
    mlib_s16 *CONST data_i,
    mlib_s32 CONST order,
    mlib_s32 CONST isign)
{
	const mlib_s32 fdivider2 = 1;
	const mlib_s32 fdivider4 = 2;
	DEF_ADDER(fdivider2);
	DEF_ADDER(fdivider4);
	const __m128i sig_mask =
		(isign == 1) ? _mm_set1_epi32(0xffffffff) : _mm_setzero_si128();
	const __m128i sig_add1 =
	    _mm_and_si128(sig_mask, _mm_set1_epi32(0x10001));
	__m128i k = _mm_set1_epi32(0x2d412d41);

	mlib_s32 j;
	mlib_s16 *pr = data_r, *pi = data_i;

#pragma pipeloop(0)
	for (j = 0; j < (1 << (order - 6)); j++) {
		__m128i ar, br, cr, dr;
		__m128i er, fr, gr, hr, ai, bi, ci, di, ei, fi, gi, hi;
		__m128i acrp, egrp, acrm, egim, egim_sig, fhip, fhip_sig, fhrm,
		    fhrmipsp, fhrmipsm;
		__m128i acip, egip, acim, egrm, egrm_sig, fhrp, fhrp_sig, fhim,
		    fhimrpsm, fhimrpsp;
		__m128i brdip, brdim;
		__m128i drout;

		READ_DEAL1NOD_SSE2(r, c, d, 8)
		READ_DEAL1NOD_SSE2(r, a, b, 0)
		ar = _mm_fmul8x16_vis(ar, fdivider4);
		cr = _mm_fmul8x16_vis(cr, fdivider4);
		acrp = _mm_add_epi16(ar, cr);
		acrm = _mm_sub_epi16(ar, cr);

		READ_DEAL1NOD_SSE2(r, e, f, 16)
		er = _mm_fmul8x16_vis(er, fdivider4);

		READ_DEAL1NOD_SSE2(r, g, h, 24)
		gr = _mm_fmul8x16_vis(gr, fdivider4);
		egrp = _mm_add_epi16(er, gr);

		arout = _mm_add_epi16(acrp, egrp);

		fr = _mm_mulhi_epi16_vis(k, fr);
		hr = _mm_mulhi_epi16_vis(k, hr);

		READ_DEAL1NOD_SSE2(i, c, d, 8)
		READ_DEAL1NOD_SSE2(i, e, f, 16)
		ei = _mm_fmul8x16_vis(ei, fdivider4);
		READ_DEAL1NOD_SSE2(i, g, h, 24)
		gi = _mm_fmul8x16_vis(gi, fdivider4);
		egim = _mm_sub_epi16(ei, gi);
		egim_sig =
			_mm_add_epi16(sig_add1, _mm_xor_si128(sig_mask, egim));
		crout = _mm_add_epi16(acrm, egim_sig);

		fi = _mm_mulhi_epi16_vis(k, fi);
		hi = _mm_mulhi_epi16_vis(k, hi);

		br = _mm_fmul8x16_vis(br, fdivider4);
		di = _mm_fmul8x16_vis(di, fdivider4);
		dr = _mm_fmul8x16_vis(dr, fdivider4);
		di = _mm_add_epi16(sig_add1, _mm_xor_si128(sig_mask, di));
		dr = _mm_add_epi16(sig_add1, _mm_xor_si128(sig_mask, dr));

		brdip = _mm_add_epi16(br, di);
		brdim = _mm_sub_epi16(br, di);

		fhip = _mm_add_epi16(fi, hi);
		fhip_sig =
			_mm_add_epi16(sig_add1, _mm_xor_si128(sig_mask, fhip));
		fhrm = _mm_sub_epi16(fr, hr);

		fhrmipsp = _mm_add_epi16(fhip_sig, fhrm);
		fhrmipsm = _mm_sub_epi16(fhip_sig, fhrm);

		brout = _mm_add_epi16(brdip, fhrmipsp);
		drout = _mm_add_epi16(brdim, fhrmipsm);

		egrm = _mm_sub_epi16(er, gr);
		erout = _mm_sub_epi16(acrp, egrp);
		grout = _mm_sub_epi16(acrm, egim_sig);
		fhrp = _mm_add_epi16(fr, hr);
		frout = _mm_sub_epi16(brdip, fhrmipsp);
		hrout = _mm_sub_epi16(brdim, fhrmipsm);

		TRANSPOSE4X4_SSE2(arout, brout, crout, drout,
				erout, frout, grout, hrout, pr)

		READ_DEAL1NOD_SSE2(i, a, b, 0)
		ai = _mm_fmul8x16_vis(ai, fdivider4);
		ci = _mm_fmul8x16_vis(ci, fdivider4);
		acip = _mm_add_epi16(ai, ci);
		egip = _mm_add_epi16(ei, gi);
		acim = _mm_sub_epi16(ai, ci);
		aiout = _mm_add_epi16(acip, egip);

		egrm_sig =
			_mm_add_epi16(sig_add1, _mm_xor_si128(sig_mask, egrm));
		ciout = _mm_sub_epi16(acim, egrm_sig);

		bi = _mm_fmul8x16_vis(bi, fdivider4);
		fhrp_sig =
			_mm_add_epi16(sig_add1, _mm_xor_si128(sig_mask, fhrp));
		fhim = _mm_sub_epi16(fi, hi);

		bidrm = _mm_sub_epi16(bi, dr);
		bidrp = _mm_add_epi16(bi, dr);
		fhimrpsm = _mm_sub_epi16(fhim, fhrp_sig);
		fhimrpsp = _mm_add_epi16(fhim, fhrp_sig);

		biout = _mm_add_epi16(bidrm, fhimrpsm);
		diout = _mm_sub_epi16(bidrp, fhimrpsp);

		pr += 64;

		eiout = _mm_sub_epi16(acip, egip);
		giout = _mm_add_epi16(acim, egrm_sig);
		fiout = _mm_sub_epi16(bidrm, fhimrpsm);
		hiout = _mm_add_epi16(bidrp, fhimrpsp);

		TRANSPOSE4X4_SSE2(aiout, biout, ciout, diout,
				eiout, fiout, giout, hiout, pi)

		pi += 64;
	}

#pragma pipeloop(1)
}

/* ********************************************************************** */

/* divide all */
void
mlib_s_fftK_3LayersFirst(
    mlib_s16 *CONST data_r,
    mlib_s16 *CONST data_i,
    mlib_s32 CONST order,
    mlib_s32 CONST isign)
{
	const mlib_s32 fdivider2 = 1;
	const mlib_s32 fdivider4 = 2;
	const mlib_s32 dividerD = 2;
	DEF_ADDER(fdivider2);
	DEF_ADDER(fdivider4);
	DEF_ADDER(dividerD);
	const __m128i sig_mask =
		(isign == 1) ? _mm_set1_epi32(0xffffffff): _mm_setzero_si128();
	const __m128i sig_add1 =
	    _mm_and_si128(sig_mask, _mm_set1_epi32(0x10001));
	const __m128i k = _mm_set1_epi32(0x2d412d41);
	const __m128i fzero = _mm_setzero_si128();

	mlib_s32 count = 1 << (order - 6);
	mlib_s32 j = 0;
	mlib_s16 *pr = data_r, *pi = data_i;

#pragma pipeloop(0)
	for (; j < count; ++j) {
		__m128i ar, br, cr, dr;
		__m128i er, fr, gr, hr, ai, bi, ci, di, ei, fi, gi, hi;
		__m128i acrp, egrp, acrm, egim, egim_sig, fhip, fhip_sig, fhrm,
		    fhrmipsp, fhrmipsm;
		__m128i acip, egip, acim, egrm, egrm_sig, fhrp, fhrp_sig, fhim,
		    fhimrpsm, fhimrpsp;
		__m128i brdip, brdim;
		__m128i drout;

		READ_DEAL1_SSE2(r, c, d, 8)
		READ_DEAL1_SSE2(r, a, b, 0)
		ar = _mm_fmul8x16_vis(ar, fdivider4);
		cr = _mm_fmul8x16_vis(cr, fdivider4);
		acrp = _mm_add_epi16(ar, cr);
		acrm = _mm_sub_epi16(ar, cr);

		READ_DEAL1_SSE2(r, e, f, 16)
		er = _mm_fmul8x16_vis(er, fdivider4);

		READ_DEAL1_SSE2(r, g, h, 24)
		gr = _mm_fmul8x16_vis(gr, fdivider4);
		egrp = _mm_add_epi16(er, gr);

		arout = _mm_add_epi16(acrp, egrp);

		fr = _mm_mulhi_epi16_vis(k, fr);
		hr = _mm_mulhi_epi16_vis(k, hr);

		READ_DEAL1_SSE2(i, c, d, 8)
		READ_DEAL1_SSE2(i, e, f, 16)
		ei = _mm_fmul8x16_vis(ei, fdivider4);
		READ_DEAL1_SSE2(i, g, h, 24)
		gi = _mm_fmul8x16_vis(gi, fdivider4);
		egim = _mm_sub_epi16(ei, gi);
		egim_sig =
			_mm_add_epi16(sig_add1, _mm_xor_si128(sig_mask, egim));
		crout = _mm_add_epi16(acrm, egim_sig);

		fi = _mm_mulhi_epi16_vis(k, fi);
		hi = _mm_mulhi_epi16_vis(k, hi);

		br = _mm_fmul8x16_vis(br, fdivider4);
		if (isign == 1)	di = _mm_sub_epi16(fzero, di);
		di = _mm_fmul8x16_vis(di, dividerD);
		if (isign == 1)	dr = _mm_sub_epi16(fzero, dr);
		dr = _mm_fmul8x16_vis(dr, dividerD);

		brdip = _mm_add_epi16(br, di);
		brdim = _mm_sub_epi16(br, di);

		fhip = _mm_add_epi16(fi, hi);
		fhip_sig =
			_mm_add_epi16(sig_add1, _mm_xor_si128(sig_mask, fhip));
		fhrm = _mm_sub_epi16(fr, hr);

		fhrmipsp = _mm_add_epi16(fhip_sig, fhrm);
		fhrmipsm = _mm_sub_epi16(fhip_sig, fhrm);

		brout = _mm_add_epi16(brdip, fhrmipsp);
		drout = _mm_add_epi16(brdim, fhrmipsm);

		egrm = _mm_sub_epi16(er, gr);
		erout = _mm_sub_epi16(acrp, egrp);
		grout = _mm_sub_epi16(acrm, egim_sig);
		fhrp = _mm_add_epi16(fr, hr);
		frout = _mm_sub_epi16(brdip, fhrmipsp);
		hrout = _mm_sub_epi16(brdim, fhrmipsm);

		TRANSPOSE4X4_SSE2(arout, brout, crout, drout,
				erout, frout, grout, hrout, pr)

		READ_DEAL1_SSE2(i, a, b, 0)
		ai = _mm_fmul8x16_vis(ai, fdivider4);
		ci = _mm_fmul8x16_vis(ci, fdivider4);
		acip = _mm_add_epi16(ai, ci);
		egip = _mm_add_epi16(ei, gi);
		acim = _mm_sub_epi16(ai, ci);
		aiout = _mm_add_epi16(acip, egip);

		egrm_sig =
			_mm_add_epi16(sig_add1, _mm_xor_si128(sig_mask, egrm));
		ciout = _mm_sub_epi16(acim, egrm_sig);

		bi = _mm_fmul8x16_vis(bi, fdivider4);
		fhrp_sig =
			_mm_add_epi16(sig_add1, _mm_xor_si128(sig_mask, fhrp));
		fhim = _mm_sub_epi16(fi, hi);

		bidrm = _mm_sub_epi16(bi, dr);
		bidrp = _mm_add_epi16(bi, dr);
		fhimrpsm = _mm_sub_epi16(fhim, fhrp_sig);
		fhimrpsp = _mm_add_epi16(fhim, fhrp_sig);

		biout = _mm_add_epi16(bidrm, fhimrpsm);
		diout = _mm_sub_epi16(bidrp, fhimrpsp);

		pr += 64;

		eiout = _mm_sub_epi16(acip, egip);
		giout = _mm_add_epi16(acim, egrm_sig);
		fiout = _mm_sub_epi16(bidrm, fhimrpsm);
		hiout = _mm_add_epi16(bidrp, fhimrpsp);

		TRANSPOSE4X4_SSE2(aiout, biout, ciout, diout,
				eiout, fiout, giout, hiout, pi)

		pi += 64;
	}

#pragma pipeloop(1)
}


/*
 * !!!!! reminder: function below is special for order == 5 !!!!!!!!!!!
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * !!!!!!!!!!!!!!!!!!!!11
 *
 */
/* divide all */
void
mlib_v_fftK_3LayersFirst(
    mlib_s16 *CONST data_r,
    mlib_s16 *CONST data_i,
    mlib_s32 CONST order,
    mlib_s32 CONST isign)
{
	const mlib_s32 fdivider2 = 1;
	const mlib_s32 fdivider4 = 2;
	const mlib_s32 dividerD = 2;
	DEF_ADDER(fdivider2);
	DEF_ADDER(fdivider4);
	DEF_ADDER(dividerD);
	const __m128i sig_mask =
		(isign == 1) ? _mm_set1_epi32(0xffffffff): _mm_setzero_si128();
	const __m128i sig_add1 =
	    _mm_and_si128(sig_mask, _mm_set1_epi32(0x10001));
	const __m128i k = _mm_set1_epi32(0x2d412d41);
	const __m128i fzero = _mm_setzero_si128();

	mlib_s32 count = 1 << (order - 6);
	mlib_s32 j = 0;
	mlib_s16 *pr = data_r, *pi = data_i;

#pragma pipeloop(0)
		__m128i ar, br, cr, dr;
		__m128i er, fr, gr, hr, ai, bi, ci, di, ei, fi, gi, hi;
		__m128i acrp, egrp, acrm, egim, egim_sig, fhip, fhip_sig, fhrm,
		    fhrmipsp, fhrmipsm;
		__m128i acip, egip, acim, egrm, egrm_sig, fhrp, fhrp_sig, fhim,
		    fhimrpsm, fhimrpsp;
		__m128i brdip, brdim;
		__m128i drout;

		READ_DEAL1_SSE2_ORDER5(r, c, d, 8)
		READ_DEAL1_SSE2_ORDER5(r, a, b, 0)
		ar = _mm_fmul8x16_vis(ar, fdivider4);
		cr = _mm_fmul8x16_vis(cr, fdivider4);
		acrp = _mm_add_epi16(ar, cr);
		acrm = _mm_sub_epi16(ar, cr);

		READ_DEAL1_SSE2_ORDER5(r, e, f, 16)
		er = _mm_fmul8x16_vis(er, fdivider4);

		READ_DEAL1_SSE2_ORDER5(r, g, h, 24)
		gr = _mm_fmul8x16_vis(gr, fdivider4);
		egrp = _mm_add_epi16(er, gr);

		arout = _mm_add_epi16(acrp, egrp);

		fr = _mm_mulhi_epi16_vis(k, fr);
		hr = _mm_mulhi_epi16_vis(k, hr);

		READ_DEAL1_SSE2_ORDER5(i, c, d, 8)
		READ_DEAL1_SSE2_ORDER5(i, e, f, 16)
		ei = _mm_fmul8x16_vis(ei, fdivider4);
		READ_DEAL1_SSE2_ORDER5(i, g, h, 24)
		gi = _mm_fmul8x16_vis(gi, fdivider4);
		egim = _mm_sub_epi16(ei, gi);
		egim_sig =
			_mm_add_epi16(sig_add1, _mm_xor_si128(sig_mask, egim));
		crout = _mm_add_epi16(acrm, egim_sig);

		fi = _mm_mulhi_epi16_vis(k, fi);
		hi = _mm_mulhi_epi16_vis(k, hi);

		br = _mm_fmul8x16_vis(br, fdivider4);
		if (isign == 1)	di = _mm_sub_epi16(fzero, di);
		di = _mm_fmul8x16_vis(di, dividerD);
		if (isign == 1)	dr = _mm_sub_epi16(fzero, dr);
		dr = _mm_fmul8x16_vis(dr, dividerD);

		brdip = _mm_add_epi16(br, di);
		brdim = _mm_sub_epi16(br, di);

		fhip = _mm_add_epi16(fi, hi);
		fhip_sig =
			_mm_add_epi16(sig_add1, _mm_xor_si128(sig_mask, fhip));
		fhrm = _mm_sub_epi16(fr, hr);

		fhrmipsp = _mm_add_epi16(fhip_sig, fhrm);
		fhrmipsm = _mm_sub_epi16(fhip_sig, fhrm);

		brout = _mm_add_epi16(brdip, fhrmipsp);
		drout = _mm_add_epi16(brdim, fhrmipsm);

		egrm = _mm_sub_epi16(er, gr);
		erout = _mm_sub_epi16(acrp, egrp);
		grout = _mm_sub_epi16(acrm, egim_sig);
		fhrp = _mm_add_epi16(fr, hr);
		frout = _mm_sub_epi16(brdip, fhrmipsp);
		hrout = _mm_sub_epi16(brdim, fhrmipsm);

		TRANSPOSE4X4_SSE2_ORDER5(arout, brout, crout, drout,
				erout, frout, grout, hrout, pr)

		READ_DEAL1_SSE2_ORDER5(i, a, b, 0)
		ai = _mm_fmul8x16_vis(ai, fdivider4);
		ci = _mm_fmul8x16_vis(ci, fdivider4);
		acip = _mm_add_epi16(ai, ci);
		egip = _mm_add_epi16(ei, gi);
		acim = _mm_sub_epi16(ai, ci);
		aiout = _mm_add_epi16(acip, egip);

		egrm_sig =
			_mm_add_epi16(sig_add1, _mm_xor_si128(sig_mask, egrm));
		ciout = _mm_sub_epi16(acim, egrm_sig);

		bi = _mm_fmul8x16_vis(bi, fdivider4);
		fhrp_sig =
			_mm_add_epi16(sig_add1, _mm_xor_si128(sig_mask, fhrp));
		fhim = _mm_sub_epi16(fi, hi);

		bidrm = _mm_sub_epi16(bi, dr);
		bidrp = _mm_add_epi16(bi, dr);
		fhimrpsm = _mm_sub_epi16(fhim, fhrp_sig);
		fhimrpsp = _mm_add_epi16(fhim, fhrp_sig);

		biout = _mm_add_epi16(bidrm, fhimrpsm);
		diout = _mm_sub_epi16(bidrp, fhimrpsp);

		pr += 64;

		eiout = _mm_sub_epi16(acip, egip);
		giout = _mm_add_epi16(acim, egrm_sig);
		fiout = _mm_sub_epi16(bidrm, fhimrpsm);
		hiout = _mm_add_epi16(bidrp, fhimrpsp);

		TRANSPOSE4X4_SSE2_ORDER5(aiout, biout, ciout, diout,
				eiout, fiout, giout, hiout, pi)

		pi += 64;

#pragma pipeloop(1)
}

/* *********************************************************** */
