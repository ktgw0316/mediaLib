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

#pragma ident	"@(#)mlib_v_SignalFFT_First.c	9.2	07/11/05 SMI"

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
#include <vis_proto.h>

#include <mlib_SignalFFTUtil.h>
#include <mlib_v_SignalFFTDisp.h>

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

/* marks types transform */
#define	D64_TO_S32(val)	((mlib_s32) ((val)))

/* *********************************************************** */

/* 16 multiplying */
#define	MULT16(d0, d1)                                                \
	vis_fpadd16(vis_fmul8sux16(d0, d1), vis_fmul8ulx16(d0, d1))

/* *********************************************************** */

/* used in _3LayersFirst to store results */
#define	TRANSPOSE4X4(i0, i1, i2, i3, p0, p2, p1, p3)                   \
	{                                                              \
	    mlib_d64 i02, i13, i0213;                                  \
	                                                               \
	    i02 = vis_fpmerge(vis_read_hi(i0), vis_read_hi(i2));       \
	    i13 = vis_fpmerge(vis_read_hi(i1), vis_read_hi(i3));       \
	    i0213 = vis_fpmerge(vis_read_hi(i02), vis_read_hi(i13));   \
	    *(mlib_d64 *)(p0) =                                        \
		vis_fpmerge(vis_read_hi(i0213), vis_read_lo(i0213));   \
	    i0213 = vis_fpmerge(vis_read_lo(i02), vis_read_lo(i13));   \
	    *(mlib_d64 *)(p1) =                                        \
		vis_fpmerge(vis_read_hi(i0213), vis_read_lo(i0213));   \
	    i02 = vis_fpmerge(vis_read_lo(i0), vis_read_lo(i2));       \
	    i13 = vis_fpmerge(vis_read_lo(i1), vis_read_lo(i3));       \
	    i0213 = vis_fpmerge(vis_read_hi(i02), vis_read_hi(i13));   \
	    *(mlib_d64 *)(p2) =                                        \
		vis_fpmerge(vis_read_hi(i0213), vis_read_lo(i0213));   \
	    i0213 = vis_fpmerge(vis_read_lo(i02), vis_read_lo(i13));   \
	    *(mlib_d64 *)(p3) =                                        \
		vis_fpmerge(vis_read_hi(i0213), vis_read_lo(i0213));   \
	}

/* *********************************************************** */

/* used in _3LayersFirst to load and preprocess data */
#define	READ_DEAL1(part, a, b, addr)                            \
	{                                                       \
	    mlib_d64 xr;                                        \
	                                                        \
	    a##part = *(mlib_d64 *)(p##part + addr);            \
	    b##part = *(mlib_d64 *)(p##part + addr + 4);        \
	    xr = vis_fmul8x16(fdivider2, a##part);              \
	    b##part = vis_fmul8x16(fdivider2, b##part);         \
	    a##part = vis_fpadd16(xr, b##part);                 \
	    b##part = vis_fpsub16(xr, b##part);                 \
	}

/* *********************************************************** */

#define	READ_DEAL1NOD(part, a, b, addr)                         \
	{                                                       \
	    mlib_d64 xr;                                        \
	                                                        \
	    xr = *(mlib_d64 *)(p##part + addr);                 \
	    b##part = *(mlib_d64 *)(p##part + addr + 4);        \
	    a##part = vis_fpadd16(xr, b##part);                 \
	    b##part = vis_fpsub16(xr, b##part);                 \
	}

/* *********************************************************** */

/* internal ( static ) data - sines and cosines of Pi^0, ..., 30 */
static const mlib_d64 mlib_v_fftK_SinCosTable[];

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
mlib_v_fftK_3LayersFirst_nod3(
    mlib_s16 *CONST data_r,
    mlib_s16 *CONST data_i,
    mlib_s32 CONST order,
    mlib_s32 CONST isign)
{
	const mlib_d64 sig_mask = (isign == 1) ? vis_fone() : vis_fzero();
	const mlib_d64 sig_add1 =
	    vis_fand(sig_mask, vis_to_double_dup(0x10001));
	const mlib_d64 k = vis_to_double_dup(0x5A825A82);

	mlib_s32 count = 1 << (order - 5);
	mlib_s32 j = 0;
	mlib_s16 *pr = data_r, *pi = data_i;

#pragma pipeloop(0)
	for (; j < count; ++j) {
		mlib_d64 ar, br, cr, dr;
		mlib_d64 er, fr, gr, hr, ai, bi, ci, di, ei, fi, gi, hi;
		mlib_d64 acrp, egrp, acrm, egim, egim_sig, fhip, fhip_sig, fhrm,
		    fhrmipsp, fhrmipsm;
		mlib_d64 acip, egip, acim, egrm, egrm_sig, fhrp, fhrp_sig, fhim,
		    fhimrpsm, fhimrpsp;
		mlib_d64 brdip, brdim;
		mlib_d64 drout;

		READ_DEAL1NOD(r, c, d, 8)
		    READ_DEAL1NOD(r, a, b, 0)
		    acrp = vis_fpadd16(ar, cr);
		acrm = vis_fpsub16(ar, cr);

		READ_DEAL1NOD(r, e, f, 16)

		    READ_DEAL1NOD(r, g, h, 24)
		    egrp = vis_fpadd16(er, gr);

		arout = vis_fpadd16(acrp, egrp);

		fr = MULT16(k, fr);
		hr = MULT16(k, hr);
		fr = vis_fpadd16(fr, fr);
		hr = vis_fpadd16(hr, hr);

		READ_DEAL1NOD(i, c, d, 8)
		    READ_DEAL1NOD(i, e, f, 16)
		    READ_DEAL1NOD(i, g, h, 24)
		    egim = vis_fpsub16(ei, gi);
		egim_sig = vis_fpadd16(sig_add1, vis_fxor(sig_mask, egim));
		crout = vis_fpadd16(acrm, egim_sig);

		fi = MULT16(k, fi);
		hi = MULT16(k, hi);
		fi = vis_fpadd16(fi, fi);
		hi = vis_fpadd16(hi, hi);

		di = vis_fpadd16(sig_add1, vis_fxor(sig_mask, di));
		dr = vis_fpadd16(sig_add1, vis_fxor(sig_mask, dr));

		brdip = vis_fpadd16(br, di);
		brdim = vis_fpsub16(br, di);

		fhip = vis_fpadd16(fi, hi);
		fhip_sig = vis_fpadd16(sig_add1, vis_fxor(sig_mask, fhip));
		fhrm = vis_fpsub16(fr, hr);

		fhrmipsp = vis_fpadd16(fhip_sig, fhrm);
		fhrmipsm = vis_fpsub16(fhip_sig, fhrm);

		brout = vis_fpadd16(brdip, fhrmipsp);
		drout = vis_fpadd16(brdim, fhrmipsm);

		TRANSPOSE4X4(arout, brout, crout, drout, pr, pr + 8, pr + 16,
		    pr + 24)

		    egrm = vis_fpsub16(er, gr);
		erout = vis_fpsub16(acrp, egrp);
		grout = vis_fpsub16(acrm, egim_sig);
		fhrp = vis_fpadd16(fr, hr);
		frout = vis_fpsub16(brdip, fhrmipsp);
		hrout = vis_fpsub16(brdim, fhrmipsm);

		TRANSPOSE4X4(erout, frout, grout, hrout, pr + 4, pr + 8 + 4,
		    pr + 16 + 4, pr + 24 + 4)

		    READ_DEAL1NOD(i, a, b, 0)
		    acip = vis_fpadd16(ai, ci);
		egip = vis_fpadd16(ei, gi);
		acim = vis_fpsub16(ai, ci);
		aiout = vis_fpadd16(acip, egip);

		egrm_sig = vis_fpadd16(sig_add1, vis_fxor(sig_mask, egrm));
		ciout = vis_fpsub16(acim, egrm_sig);

		fhrp_sig = vis_fpadd16(sig_add1, vis_fxor(sig_mask, fhrp));
		fhim = vis_fpsub16(fi, hi);

		bidrm = vis_fpsub16(bi, dr);
		bidrp = vis_fpadd16(bi, dr);
		fhimrpsm = vis_fpsub16(fhim, fhrp_sig);
		fhimrpsp = vis_fpadd16(fhim, fhrp_sig);

		biout = vis_fpadd16(bidrm, fhimrpsm);
		diout = vis_fpsub16(bidrp, fhimrpsp);

		TRANSPOSE4X4(aiout, biout, ciout, diout, pi, pi + 8, pi + 16,
		    pi + 24)

		    pr += 32;

		eiout = vis_fpsub16(acip, egip);
		giout = vis_fpadd16(acim, egrm_sig);
		fiout = vis_fpsub16(bidrm, fhimrpsm);
		hiout = vis_fpadd16(bidrp, fhimrpsp);

		TRANSPOSE4X4(eiout, fiout, giout, hiout, pi + 4, pi + 8 + 4,
		    pi + 16 + 4, pi + 24 + 4)

		    pi += 32;
	}

#pragma pipeloop(1)
}

/* *********************************************************** */

/* divide twice */
void
mlib_v_fftK_3LayersFirst_nod1(
    mlib_s16 *CONST data_r,
    mlib_s16 *CONST data_i,
    mlib_s32 CONST order,
    mlib_s32 CONST isign)
{
	mlib_f32 fdivider4 = vis_to_float(0x40404040);
	const mlib_d64 sig_mask = (isign == 1) ? vis_fone() : vis_fzero();
	const mlib_d64 sig_add1 =
	    vis_fand(sig_mask, vis_to_double_dup(0x10001));
	mlib_d64 k = vis_to_double_dup(0x2d412d41);

	mlib_s32 count = 1 << (order - 5);
	mlib_s32 j = 0;
	mlib_s16 *pr = data_r, *pi = data_i;

#pragma pipeloop(0)
	for (; j < count; ++j) {
		mlib_d64 ar, br, cr, dr;
		mlib_d64 er, fr, gr, hr, ai, bi, ci, di, ei, fi, gi, hi;
		mlib_d64 acrp, egrp, acrm, egim, egim_sig, fhip, fhip_sig, fhrm,
		    fhrmipsp, fhrmipsm;
		mlib_d64 acip, egip, acim, egrm, egrm_sig, fhrp, fhrp_sig, fhim,
		    fhimrpsm, fhimrpsp;
		mlib_d64 brdip, brdim;
		mlib_d64 drout;

		READ_DEAL1NOD(r, c, d, 8)
		    READ_DEAL1NOD(r, a, b, 0)
		    ar = vis_fmul8x16(fdivider4, ar);
		cr = vis_fmul8x16(fdivider4, cr);
		acrp = vis_fpadd16(ar, cr);
		acrm = vis_fpsub16(ar, cr);

		READ_DEAL1NOD(r, e, f, 16)
		    er = vis_fmul8x16(fdivider4, er);

		READ_DEAL1NOD(r, g, h, 24)
		    gr = vis_fmul8x16(fdivider4, gr);
		egrp = vis_fpadd16(er, gr);

		arout = vis_fpadd16(acrp, egrp);

		fr = MULT16(k, fr);
		hr = MULT16(k, hr);

		READ_DEAL1NOD(i, c, d, 8)
		    READ_DEAL1NOD(i, e, f, 16)
		    ei = vis_fmul8x16(fdivider4, ei);
		READ_DEAL1NOD(i, g, h, 24)
		    gi = vis_fmul8x16(fdivider4, gi);
		egim = vis_fpsub16(ei, gi);
		egim_sig = vis_fpadd16(sig_add1, vis_fxor(sig_mask, egim));
		crout = vis_fpadd16(acrm, egim_sig);

		fi = MULT16(k, fi);
		hi = MULT16(k, hi);

		br = vis_fmul8x16(fdivider4, br);
		di = vis_fmul8x16(fdivider4, di);
		dr = vis_fmul8x16(fdivider4, dr);
		di = vis_fpadd16(sig_add1, vis_fxor(sig_mask, di));
		dr = vis_fpadd16(sig_add1, vis_fxor(sig_mask, dr));

		brdip = vis_fpadd16(br, di);
		brdim = vis_fpsub16(br, di);

		fhip = vis_fpadd16(fi, hi);
		fhip_sig = vis_fpadd16(sig_add1, vis_fxor(sig_mask, fhip));
		fhrm = vis_fpsub16(fr, hr);

		fhrmipsp = vis_fpadd16(fhip_sig, fhrm);
		fhrmipsm = vis_fpsub16(fhip_sig, fhrm);

		brout = vis_fpadd16(brdip, fhrmipsp);
		drout = vis_fpadd16(brdim, fhrmipsm);

		TRANSPOSE4X4(arout, brout, crout, drout, pr, pr + 8, pr + 16,
		    pr + 24)

		    egrm = vis_fpsub16(er, gr);
		erout = vis_fpsub16(acrp, egrp);
		grout = vis_fpsub16(acrm, egim_sig);
		fhrp = vis_fpadd16(fr, hr);
		frout = vis_fpsub16(brdip, fhrmipsp);
		hrout = vis_fpsub16(brdim, fhrmipsm);

		TRANSPOSE4X4(erout, frout, grout, hrout, pr + 4, pr + 8 + 4,
		    pr + 16 + 4, pr + 24 + 4)

		    READ_DEAL1NOD(i, a, b, 0)
		    ai = vis_fmul8x16(fdivider4, ai);
		ci = vis_fmul8x16(fdivider4, ci);
		acip = vis_fpadd16(ai, ci);
		egip = vis_fpadd16(ei, gi);
		acim = vis_fpsub16(ai, ci);
		aiout = vis_fpadd16(acip, egip);

		egrm_sig = vis_fpadd16(sig_add1, vis_fxor(sig_mask, egrm));
		ciout = vis_fpsub16(acim, egrm_sig);

		bi = vis_fmul8x16(fdivider4, bi);
		fhrp_sig = vis_fpadd16(sig_add1, vis_fxor(sig_mask, fhrp));
		fhim = vis_fpsub16(fi, hi);

		bidrm = vis_fpsub16(bi, dr);
		bidrp = vis_fpadd16(bi, dr);
		fhimrpsm = vis_fpsub16(fhim, fhrp_sig);
		fhimrpsp = vis_fpadd16(fhim, fhrp_sig);

		biout = vis_fpadd16(bidrm, fhimrpsm);
		diout = vis_fpsub16(bidrp, fhimrpsp);

		TRANSPOSE4X4(aiout, biout, ciout, diout, pi, pi + 8, pi + 16,
		    pi + 24)

		    pr += 32;

		eiout = vis_fpsub16(acip, egip);
		giout = vis_fpadd16(acim, egrm_sig);
		fiout = vis_fpsub16(bidrm, fhimrpsm);
		hiout = vis_fpadd16(bidrp, fhimrpsp);

		TRANSPOSE4X4(eiout, fiout, giout, hiout, pi + 4, pi + 8 + 4,
		    pi + 16 + 4, pi + 24 + 4)

		    pi += 32;
	}

#pragma pipeloop(1)
}

/* *********************************************************** */

/* divide all */
void
mlib_v_fftK_3LayersFirst(
    mlib_s16 *CONST data_r,
    mlib_s16 *CONST data_i,
    mlib_s32 CONST order,
    mlib_s32 CONST isign)
{
	const mlib_f32 fdivider2 = vis_to_float(0x80808080);
	const mlib_f32 fdivider4 = vis_to_float(0x40404040);
	const mlib_d64 sig_mask = (isign == 1) ? vis_fone() : vis_fzero();
	const mlib_d64 sig_add1 =
	    vis_fand(sig_mask, vis_to_double_dup(0x10001));
	const mlib_d64 dividerD = vis_fpadd16(sig_add1, vis_fxor(sig_mask,
	    vis_to_double_dup(0x40004000)));
	const mlib_d64 k = vis_to_double_dup(0x2d412d41);

	mlib_s32 count = 1 << (order - 5);
	mlib_s32 j = 0;
	mlib_s16 *pr = data_r, *pi = data_i;

#pragma pipeloop(0)
	for (; j < count; ++j) {
		mlib_d64 ar, br, cr, dr;
		mlib_d64 er, fr, gr, hr, ai, bi, ci, di, ei, fi, gi, hi;
		mlib_d64 acrp, egrp, acrm, egim, egim_sig, fhip, fhip_sig, fhrm,
		    fhrmipsp, fhrmipsm;
		mlib_d64 acip, egip, acim, egrm, egrm_sig, fhrp, fhrp_sig, fhim,
		    fhimrpsm, fhimrpsp;
		mlib_d64 brdip, brdim;
		mlib_d64 drout;

		READ_DEAL1(r, c, d, 8)
		    READ_DEAL1(r, a, b, 0)
		    ar = vis_fmul8x16(fdivider4, ar);
		cr = vis_fmul8x16(fdivider4, cr);
		acrp = vis_fpadd16(ar, cr);
		acrm = vis_fpsub16(ar, cr);

		READ_DEAL1(r, e, f, 16)
		    er = vis_fmul8x16(fdivider4, er);

		READ_DEAL1(r, g, h, 24)
		    gr = vis_fmul8x16(fdivider4, gr);
		egrp = vis_fpadd16(er, gr);

		arout = vis_fpadd16(acrp, egrp);

		fr = MULT16(k, fr);
		hr = MULT16(k, hr);

		READ_DEAL1(i, c, d, 8)
		    READ_DEAL1(i, e, f, 16)
		    ei = vis_fmul8x16(fdivider4, ei);
		READ_DEAL1(i, g, h, 24)
		    gi = vis_fmul8x16(fdivider4, gi);
		egim = vis_fpsub16(ei, gi);
		egim_sig = vis_fpadd16(sig_add1, vis_fxor(sig_mask, egim));
		crout = vis_fpadd16(acrm, egim_sig);

		fi = MULT16(k, fi);
		hi = MULT16(k, hi);

		br = vis_fmul8x16(fdivider4, br);
		di = vis_fmul8sux16(dividerD, di);
		dr = vis_fmul8sux16(dividerD, dr);

		brdip = vis_fpadd16(br, di);
		brdim = vis_fpsub16(br, di);

		fhip = vis_fpadd16(fi, hi);
		fhip_sig = vis_fpadd16(sig_add1, vis_fxor(sig_mask, fhip));
		fhrm = vis_fpsub16(fr, hr);

		fhrmipsp = vis_fpadd16(fhip_sig, fhrm);
		fhrmipsm = vis_fpsub16(fhip_sig, fhrm);

		brout = vis_fpadd16(brdip, fhrmipsp);
		drout = vis_fpadd16(brdim, fhrmipsm);

		TRANSPOSE4X4(arout, brout, crout, drout, pr, pr + 8, pr + 16,
		    pr + 24)

		    egrm = vis_fpsub16(er, gr);
		erout = vis_fpsub16(acrp, egrp);
		grout = vis_fpsub16(acrm, egim_sig);
		fhrp = vis_fpadd16(fr, hr);
		frout = vis_fpsub16(brdip, fhrmipsp);
		hrout = vis_fpsub16(brdim, fhrmipsm);

		TRANSPOSE4X4(erout, frout, grout, hrout, pr + 4, pr + 8 + 4,
		    pr + 16 + 4, pr + 24 + 4)

		    READ_DEAL1(i, a, b, 0)
		    ai = vis_fmul8x16(fdivider4, ai);
		ci = vis_fmul8x16(fdivider4, ci);
		acip = vis_fpadd16(ai, ci);
		egip = vis_fpadd16(ei, gi);
		acim = vis_fpsub16(ai, ci);
		aiout = vis_fpadd16(acip, egip);

		egrm_sig = vis_fpadd16(sig_add1, vis_fxor(sig_mask, egrm));
		ciout = vis_fpsub16(acim, egrm_sig);

		bi = vis_fmul8x16(fdivider4, bi);
		fhrp_sig = vis_fpadd16(sig_add1, vis_fxor(sig_mask, fhrp));
		fhim = vis_fpsub16(fi, hi);

		bidrm = vis_fpsub16(bi, dr);
		bidrp = vis_fpadd16(bi, dr);
		fhimrpsm = vis_fpsub16(fhim, fhrp_sig);
		fhimrpsp = vis_fpadd16(fhim, fhrp_sig);

		biout = vis_fpadd16(bidrm, fhimrpsm);
		diout = vis_fpsub16(bidrp, fhimrpsp);

		TRANSPOSE4X4(aiout, biout, ciout, diout, pi, pi + 8, pi + 16,
		    pi + 24)

		    pr += 32;

		eiout = vis_fpsub16(acip, egip);
		giout = vis_fpadd16(acim, egrm_sig);
		fiout = vis_fpsub16(bidrm, fhimrpsm);
		hiout = vis_fpadd16(bidrp, fhimrpsp);

		TRANSPOSE4X4(eiout, fiout, giout, hiout, pi + 4, pi + 8 + 4,
		    pi + 16 + 4, pi + 24 + 4)

		    pi += 32;
	}

#pragma pipeloop(1)
}

/* *********************************************************** */

static const mlib_d64 mlib_v_fftK_SinCosTable[] = {
	0.0,
	1.0,
	0.707106781186547573,
	0.707106781186547462,
	0.923879532511286738,
	0.382683432365089782,
	0.980785280403230431,
	0.195090322016128248,
	0.995184726672196929,
	0.098017140329560604,
	0.998795456205172405,
	0.049067674327418015,
	0.999698818696204250,
	0.024541228522912288,
	0.999924701839144503,
	0.012271538285719925,
	0.999981175282601109,
	0.006135884649154475,
	0.999995293809576191,
	0.003067956762965976,
	0.999998823451701879,
	0.001533980186284766,
	0.999999705862882227,
	0.000766990318742704,
	0.999999926465717892,
	0.000383495187571396,
	0.999999981616429334,
	0.000191747597310703,
	0.999999995404107334,
	0.000095873799095977,
	0.999999998851026861,
	0.000047936899603067,
	0.999999999712756660,
	0.000023968449808418,
	0.999999999928189220,
	0.000011984224905070,
	0.999999999982047250,
	0.000005992112452642,
	0.999999999995511812,
	0.000002996056226335,
	0.999999999998878009,
	0.000001498028113169,
	0.999999999999719447,
	0.000000749014056585,
	0.999999999999929834,
	0.000000374507028292,
	0.999999999999982458,
	0.000000187253514146,
	0.999999999999995670,
	0.000000093626757073,
	0.999999999999998890,
	0.000000046813378537,
	0.999999999999999778,
	0.000000023406689268,
	0.999999999999999889,
	0.000000011703344634,
	1.000000000000000000,
	0.000000005851672317,
	1.000000000000000000,
	0.000000002925836159
};

/* *********************************************************** */
