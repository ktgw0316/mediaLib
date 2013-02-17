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

#pragma ident	"@(#)mlib_v_VideoColorBGR2JFIFYCC444.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorBGR2JFIFYCC444 - color conversion from BGR to JFIF
 *                                      YCrCb space
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VideoColorBGR2JFIFYCC444(mlib_u8       *y,
 *                                                mlib_u8       *cb,
 *                                                mlib_u8       *cr,
 *                                                const mlib_u8 *rgb,
 *                                                mlib_s32      n);
 * ARGUMENTS
 *      y         Pointer to Y component row, 8-byte aligned
 *      cb        Pointer to Cb component row, 8-byte aligned
 *      cr        Pointer to Cr component row, 8-byte aligned
 *      rgb       Pointer to BGR multicomponent row, 8-byte aligned
 *      n         Length of the y, cb, cr arrays
 *                Note: Length of the rgb array must be 3*n
 *
 * DESCRIPTION
 *
 *      mlib_VideoColorBGR2JFIFYCC444
 *        Y  =  0.29900 * R + 0.58700 * G + 0.11400 * B
 *        Cb = -0.16874 * R - 0.33126 * G + 0.50000 * B  + 128
 *        Cr =  0.50000 * R - 0.41869 * G - 0.08131 * B  + 128
 *
 *        R  = rgb[3*i], G = rgb[3*i+1], B = rgb[3*i+2]
 *        y[i] = Y, cr[i] = Cr, cb[i] = Cb, 0 <= i < n
 */

#include <mlib_video.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorBGR2JFIFYCC444 = __mlib_VideoColorBGR2JFIFYCC444

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorBGR2JFIFYCC444) mlib_VideoColorBGR2JFIFYCC444
	__attribute__((weak, alias("__mlib_VideoColorBGR2JFIFYCC444")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	CHANNELSEPARATE_U8(sd0, sd1, sd2, bdd, gdd, rdd)         \
	sda = vis_fpmerge(vis_read_hi(sd0), vis_read_lo(sd1));   \
	sdb = vis_fpmerge(vis_read_lo(sd0), vis_read_hi(sd2));   \
	sdc = vis_fpmerge(vis_read_hi(sd1), vis_read_lo(sd2));   \
	sdd = vis_fpmerge(vis_read_hi(sda), vis_read_lo(sdb));   \
	sde = vis_fpmerge(vis_read_lo(sda), vis_read_hi(sdc));   \
	bdd = vis_fpmerge(vis_read_hi(sdd), vis_read_lo(sde));   \
	sde = vis_fpmerge(vis_read_hi(sdb), vis_read_lo(sdc));   \
	gdd = vis_fpmerge(vis_read_lo(sdd), vis_read_hi(sde));   \
	sdd = vis_fpmerge(vis_read_lo(sda), vis_read_hi(sdc));   \
	rdd = vis_fpmerge(vis_read_hi(sdd), vis_read_lo(sde))

/* *********************************************************** */

#define	CHANNELWEIGHT_U8_SEPAR(rdd, gdd, bdd, alpha, beta,      \
	gamma, _off, res, r0, a0, b0, r1, a1, b1, r2, a2, b2)   \
	{                                                       \
	    mlib_d64 ds, dm, es, em;                            \
	    mlib_f32 df, ef;                                    \
	    mlib_d64 off = _off;                                \
	                                                        \
	    r0 = vis_fpmerge(a0, b0);                           \
	    dm = vis_fmul8x16al(vis_read_hi(rdd), alpha);       \
	    em = vis_fmul8x16al(vis_read_lo(rdd), alpha);       \
	    ds = vis_fpadd16(dm, off);                          \
	    es = vis_fpadd16(em, off);                          \
	    em = vis_fmul8x16al(vis_read_lo(gdd), beta);        \
	    dm = vis_fmul8x16al(vis_read_hi(gdd), beta);        \
	    r1 = vis_fpmerge(a1, b1);                           \
	    ds = vis_fpadd16(ds, dm);                           \
	    es = vis_fpadd16(es, em);                           \
	    r2 = vis_fpmerge(a2, b2);                           \
	    dm = vis_fmul8x16al(vis_read_hi(bdd), gamma);       \
	    em = vis_fmul8x16al(vis_read_lo(bdd), gamma);       \
	    ds = vis_fpadd16(ds, dm);                           \
	    es = vis_fpadd16(es, em);                           \
	    df = vis_fpack16(ds);                               \
	    ef = vis_fpack16(es);                               \
	    res = vis_freg_pair(df, ef);                        \
	}

/* *********************************************************** */

#define	CHANNELWEIGHT_U8(rdd, gdd, bdd, alpha, beta, gamma,     \
	off, res)                                               \
	{                                                       \
	    mlib_d64 dd, ddt;                                   \
	    mlib_f32 d32, e32;                                  \
	                                                        \
	    ddt = vis_fmul8x16al(vis_read_hi(rdd), alpha);      \
	    dd = vis_fpadd16(off, ddt);                         \
	    ddt = vis_fmul8x16al(vis_read_hi(gdd), beta);       \
	    dd = vis_fpadd16(dd, ddt);                          \
	    ddt = vis_fmul8x16al(vis_read_hi(bdd), gamma);      \
	    dd = vis_fpadd16(dd, ddt);                          \
	    d32 = vis_fpack16(dd);                              \
	    ddt = vis_fmul8x16al(vis_read_lo(rdd), alpha);      \
	    dd = vis_fpadd16(off, ddt);                         \
	    ddt = vis_fmul8x16al(vis_read_lo(gdd), beta);       \
	    dd = vis_fpadd16(dd, ddt);                          \
	    ddt = vis_fmul8x16al(vis_read_lo(bdd), gamma);      \
	    ddt = vis_fpadd16(dd, ddt);                         \
	    e32 = vis_fpack16(ddt);                             \
	    res = vis_freg_pair(d32, e32);                      \
	}

/* *********************************************************** */

#define	K11	0.29900
#define	K12	0.58700
#define	K13	0.11400
#define	K21	-0.16874
#define	K22	-0.33126
#define	K23	0.50000
#define	K31	0.50000
#define	K32	-0.41869
#define	K33	-0.08131

/* *********************************************************** */

mlib_status
__mlib_VideoColorBGR2JFIFYCC444(
	mlib_u8 *y,
	mlib_u8 *cb,
	mlib_u8 *cr,
	const mlib_u8 *rgb,
	mlib_s32 n)
{
	mlib_d64 *sp = (mlib_d64 *)rgb;
	mlib_d64 *py = (mlib_d64 *)y;
	mlib_d64 *pcb = (mlib_d64 *)cb;
	mlib_d64 *pcr = (mlib_d64 *)cr;
	mlib_u8 *cbend = cb + n - 1;
	mlib_d64 sd0, sd1, sd2;
	mlib_d64 sda, sdb, sdc, sdd, sde;
	mlib_d64 _rdd, _gdd, _bdd;
	mlib_s32 i;

	mlib_f32 k11 = vis_to_float((mlib_s32)(K11 * 8192));
	mlib_f32 k12 = vis_to_float((mlib_s32)(K12 * 8192));
	mlib_f32 k13 = vis_to_float((mlib_s32)(K13 * 8192));
	mlib_f32 k21 = vis_to_float((mlib_s32)(K21 * 8192));
	mlib_f32 k22 = vis_to_float((mlib_s32)(K22 * 8192));
	mlib_f32 k23 = vis_to_float((mlib_s32)(K23 * 8192));
	mlib_f32 k31 = vis_to_float((mlib_s32)(K31 * 8192));
	mlib_f32 k32 = vis_to_float((mlib_s32)(K32 * 8192));
	mlib_f32 k33 = vis_to_float((mlib_s32)(K33 * 8192));
	mlib_d64 off128 = vis_to_double_dup(0x10101010);
	mlib_d64 off0 = vis_to_double_dup(0x00100010);

	if (n <= 0)
		return (MLIB_FAILURE);

	vis_write_gsr(2 << 3);

	n = n >> 3;

	sd0 = vis_ld_d64_nf(sp); sp++;
	sd1 = vis_ld_d64_nf(sp); sp++;
	sd2 = vis_ld_d64_nf(sp); sp++;
	CHANNELSEPARATE_U8(sd0, sd1, sd2, _bdd, _gdd, _rdd);

#pragma pipeloop(0)
	for (i = 0; i < n; i++) {
		mlib_d64 rdd, gdd, bdd;
		mlib_d64 sda, sdb, sdc, sdd, sde, dt0;

		sd0 = vis_ld_d64_nf(sp); sp++;
		sd1 = vis_ld_d64_nf(sp); sp++;
		sd2 = vis_ld_d64_nf(sp); sp++;
		CHANNELWEIGHT_U8_SEPAR(_rdd, _gdd, _bdd, k11, k12, k13, off0,
			py[0], sda, vis_read_hi(sd0), vis_read_lo(sd1), sdb,
			vis_read_lo(sd0), vis_read_hi(sd2), sdc,
			vis_read_hi(sd1), vis_read_lo(sd2));
		CHANNELWEIGHT_U8_SEPAR(_rdd, _gdd, _bdd, k21, k22, k23, off128,
			pcb[0], sdd, vis_read_hi(sda), vis_read_lo(sdb), sde,
			vis_read_lo(sda), vis_read_hi(sdc), bdd,
			vis_read_hi(sdd), vis_read_lo(sde));
		CHANNELWEIGHT_U8_SEPAR(_rdd, _gdd, _bdd, k31, k32, k33, off128,
			pcr[0], sde, vis_read_hi(sdb), vis_read_lo(sdc), dt0,
			vis_read_lo(sda), vis_read_hi(sdc), gdd,
			vis_read_lo(sdd), vis_read_hi(sde));
		rdd = vis_fpmerge(vis_read_hi(dt0), vis_read_lo(sde));
		py++;
		pcb++;
		pcr++;
		_rdd = rdd;
		_gdd = gdd;
		_bdd = bdd;
	}

	if ((mlib_u8 *)pcb <= cbend) {
		mlib_d64 yd, cbd, crd;
		mlib_s32 emask;

		emask = vis_edge8(pcb, cbend);
		CHANNELWEIGHT_U8(_rdd, _gdd, _bdd, k11, k12, k13, off0, yd);
		CHANNELWEIGHT_U8(_rdd, _gdd, _bdd, k21, k22, k23, off128, cbd);
		CHANNELWEIGHT_U8(_rdd, _gdd, _bdd, k31, k32, k33, off128, crd);
		vis_pst_8(yd, py, emask);
		vis_pst_8(cbd, pcb, emask);
		vis_pst_8(crd, pcr, emask);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
