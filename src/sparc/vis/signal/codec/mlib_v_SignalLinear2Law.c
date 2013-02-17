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

#pragma ident	"@(#)mlib_v_SignalLinear2Law.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *
 *      mlib_SignalLinear2ALaw - Functions in this section perform A-law
 *      compression in compliance with the ITU (former CCITT) specification.
 *
 *      mlib_SignalLinear2uLaw - Functions in this section perform u-law
 *      compression in compliance with the ITU (former CCITT) specification.
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalLinear2ALaw(mlib_u8        *acode,
 *                                         const mlib_s16 *pcm,
 *                                         mlib_s32       n);
 *      mlib_status mlib_SignalLinear2uLaw(mlib_u8        *ucode,
 *                                         const mlib_s16 *pcm,
 *                                         mlib_s32       n);
 *
 * ARGUMENTS
 *
 *      pcm      Linear PCM sample array
 *
 *      acode    A-law code array
 *
 *      ucode    u-law code array
 *
 *      n        Number of samples in the input array
 *
 */

#include <mlib_signal.h>
#include <vis_proto.h>
#include <mlib_SignalCodec.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalLinear2ALaw = __mlib_SignalLinear2ALaw
#pragma weak mlib_SignalLinear2uLaw = __mlib_SignalLinear2uLaw

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalLinear2ALaw) mlib_SignalLinear2ALaw
    __attribute__((weak, alias("__mlib_SignalLinear2ALaw")));
__typeof__(__mlib_SignalLinear2uLaw) mlib_SignalLinear2uLaw
    __attribute__((weak, alias("__mlib_SignalLinear2uLaw")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	CALC_u(ret, idx)                                        \
	ind = sp[idx] + 4;                                      \
	zn = sp[idx] >> 15;                                     \
	ind -= zn;                                              \
	ind >>= 3;                                              \
	ind += zn;                                              \
	ret = VIS_LD_U8_I(mlib_l2u, ind)

/* *********************************************************** */

#define	VIS_LD_U8_I(ptr, i)	vis_ld_u8_i((void *)(ptr), (long)(i))

/* *********************************************************** */

#define	LINEAR2LAW_MAIN_LOOP(sel, i0, i1, i2, i3, i4, i5, i6, i7)        \
	for (; i < n - 7; i += 8) {                                      \
	    l1 = *(mlib_u64 *)(sp + i + (4 - sel));                      \
	    l2 = vis_ld_u64_nf((mlib_u64 *)(sp + i + (8 - sel)));        \
	    d0 = VIS_LD_U8_I(mlib_l2A, i0);                              \
	    d1 = VIS_LD_U8_I(mlib_l2A, i1);                              \
	    d2 = VIS_LD_U8_I(mlib_l2A, i2);                              \
	    d3 = VIS_LD_U8_I(mlib_l2A, i3);                              \
	    d4 = VIS_LD_U8_I(mlib_l2A, i4);                              \
	    d5 = VIS_LD_U8_I(mlib_l2A, i5);                              \
	    d6 = VIS_LD_U8_I(mlib_l2A, i6);                              \
	    d7 = VIS_LD_U8_I(mlib_l2A, i7);                              \
	    dm0 = vis_fpmerge(vis_read_lo(d0), vis_read_lo(d4));         \
	    dm1 = vis_fpmerge(vis_read_lo(d2), vis_read_lo(d6));         \
	    dm2 = vis_fpmerge(vis_read_lo(d1), vis_read_lo(d5));         \
	    dm3 = vis_fpmerge(vis_read_lo(d3), vis_read_lo(d7));         \
	    dmm0 = vis_fpmerge(vis_read_lo(dm0), vis_read_lo(dm1));      \
	    dmm1 = vis_fpmerge(vis_read_lo(dm2), vis_read_lo(dm3));      \
	    accum = vis_fpmerge(vis_read_lo(dmm0), vis_read_lo(dmm1));   \
	    *(mlib_d64 *)(dp + i) = accum;                               \
	    l0 = l2;                                                     \
	}

/* *********************************************************** */

mlib_status
__mlib_SignalLinear2ALaw(
    mlib_u8 *acode,
    const mlib_s16 *pcm,
    mlib_s32 n)
{
	mlib_u8 *dp = acode;
	mlib_u16 *sp = (mlib_u16 *)pcm;
	mlib_s32 i, ind;
	mlib_d64 accum;
	mlib_u64 l0, l1, l2;
	mlib_d64 d0, d1, d2, d3, d4, d5, d6, d7;
	mlib_d64 dm0, dm1, dm2, dm3, dmm0, dmm1;
	mlib_s32 sel;

	if ((sp == NULL) | (dp == NULL))
		return (MLIB_FAILURE);

	if (n <= 0)
		return (MLIB_OUTOFRANGE);

	for (i = 0; (i < n) && ((mlib_addr)(dp + i) & 7); i++) {
		ind = sp[i];
		ind >>= 4;

		dp[i] = mlib_l2A[ind];
	}

	if (i < n - 7) {
		sel = ((mlib_addr)(sp + i) & 7) >> 1;
		l0 = *(mlib_u64 *)(sp + i - sel);

		if (sel == 0) {

#pragma pipeloop(0)
			LINEAR2LAW_MAIN_LOOP(0,
			    l0 >> 52,
			    (l0 >> 36) & 0xFFF,
			    (l0 >> 20) & 0xFFF,
			    (l0 >> 4) & 0xFFF,
			    l1 >> 52,
			    (l1 >> 36) & 0xFFF,
			    (l1 >> 20) & 0xFFF, (l1 >> 4) & 0xFFF);
		} else if (sel == 1) {

#pragma pipeloop(0)
			LINEAR2LAW_MAIN_LOOP(1,
			    (l0 >> 36) & 0xFFF,
			    (l0 >> 20) & 0xFFF,
			    (l0 >> 4) & 0xFFF,
			    l1 >> 52,
			    (l1 >> 36) & 0xFFF,
			    (l1 >> 20) & 0xFFF, (l1 >> 4) & 0xFFF, l2 >> 52);
		} else if (sel == 2) {

#pragma pipeloop(0)
			LINEAR2LAW_MAIN_LOOP(2,
			    (l0 >> 20) & 0xFFF,
			    (l0 >> 4) & 0xFFF,
			    l1 >> 52,
			    (l1 >> 36) & 0xFFF,
			    (l1 >> 20) & 0xFFF,
			    (l1 >> 4) & 0xFFF, l2 >> 52, (l2 >> 36) & 0xFFF);
		} else {

#pragma pipeloop(0)
			LINEAR2LAW_MAIN_LOOP(3,
			    (l0 >> 4) & 0xFFF,
			    l1 >> 52,
			    (l1 >> 36) & 0xFFF,
			    (l1 >> 20) & 0xFFF,
			    (l1 >> 4) & 0xFFF,
			    l2 >> 52, (l2 >> 36) & 0xFFF, (l2 >> 20) & 0xFFF);
		}
	}

	for (; i < n; i++) {
		ind = sp[i];
		ind >>= 4;

		dp[i] = mlib_l2A[ind];
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalLinear2uLaw(
    mlib_u8 *ucode,
    const mlib_s16 *pcm,
    mlib_s32 n)
{
	mlib_u8 *dp = ucode;
	mlib_u16 *sp = (mlib_u16 *)pcm;
	mlib_s32 i, ind, zn;
	mlib_d64 accum;
	mlib_d64 d0, d1, d2, d3, d4, d5, d6, d7;
	mlib_d64 dm0, dm1, dm2, dm3, dmm0, dmm1;

	if ((sp == NULL) | (dp == NULL))
		return (MLIB_FAILURE);

	if (n <= 0)
		return (MLIB_OUTOFRANGE);

	for (i = 0; (i < n) && ((mlib_addr)(dp + i) & 7); i++) {
		ind = sp[i] + 4;
		zn = sp[i] >> 15;
		ind -= zn;
		ind >>= 3;
		ind += zn;

		dp[i] = mlib_l2u[ind];
	}

#pragma pipeloop(0)
	for (; i < (n - 8); i += 8) {
		CALC_u(d0, i);
		CALC_u(d1, i + 1);
		CALC_u(d2, i + 2);
		CALC_u(d3, i + 3);
		CALC_u(d4, i + 4);
		CALC_u(d5, i + 5);
		CALC_u(d6, i + 6);
		CALC_u(d7, i + 7);

		dm0 = vis_fpmerge(vis_read_lo(d0), vis_read_lo(d4));
		dm1 = vis_fpmerge(vis_read_lo(d2), vis_read_lo(d6));
		dm2 = vis_fpmerge(vis_read_lo(d1), vis_read_lo(d5));
		dm3 = vis_fpmerge(vis_read_lo(d3), vis_read_lo(d7));

		dmm0 = vis_fpmerge(vis_read_lo(dm0), vis_read_lo(dm1));
		dmm1 = vis_fpmerge(vis_read_lo(dm2), vis_read_lo(dm3));

		accum = vis_fpmerge(vis_read_lo(dmm0), vis_read_lo(dmm1));

		*(mlib_d64 *)(dp + i) = accum;
	}

	for (; i < n; i++) {
		ind = sp[i] + 4;
		zn = sp[i] >> 15;
		ind -= zn;
		ind >>= 3;
		ind += zn;

		dp[i] = mlib_l2u[ind];
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
