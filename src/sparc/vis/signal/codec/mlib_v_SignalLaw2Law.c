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

#pragma ident	"@(#)mlib_v_SignalLaw2Law.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *
 *      mlib_SignalALaw2uLaw - Functions in this section perform m-law
 *      and A-law compression and decompression in compliance with
 *      the ITU (former CCITT) specification.
 *
 *      mlib_SignaluLaw2ALaw - Functions in this section perform
 *      u-law and A-law compression and decompression in compliance
 *      with the ITU (former CCITT) specification.
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalALaw2uLaw(mlib_u8       *ucode,
 *                                       const mlib_u8 *acode,
 *                                       mlib_s32      n);
 *      mlib_status mlib_SignaluLaw2ALaw(mlib_u8       *acode,
 *                                       const mlib_u8 *ucode,
 *                                       mlib_s32      n);
 * ARGUMENTS
 *
 *      ucode    m-law code array
 *      acode    A-law code array
 *      n        Number of samples in the input array
 *
 */

#include <mlib_signal.h>
#include <mlib_SignalCodec.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalALaw2uLaw = __mlib_SignalALaw2uLaw
#pragma weak mlib_SignaluLaw2ALaw = __mlib_SignaluLaw2ALaw

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalALaw2uLaw) mlib_SignalALaw2uLaw
    __attribute__((weak, alias("__mlib_SignalALaw2uLaw")));
__typeof__(__mlib_SignaluLaw2ALaw) mlib_SignaluLaw2ALaw
    __attribute__((weak, alias("__mlib_SignaluLaw2ALaw")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	VIS_LD_U8_I(ptr, i)	vis_ld_u8_i((void *)(ptr), (long)(i))

/* *********************************************************** */

#define	LAW2LAW_MAIN_LOOP(sel, i0, i1, i2, i3, i4, i5, i6, i7)           \
	for (; i < n - 7; i += 8) {                                      \
	    l1 = vis_ld_u64_nf((mlib_u64 *)(src + i + (8 - sel)));       \
	    d0 = VIS_LD_U8_I(law_table, i0);                             \
	    d1 = VIS_LD_U8_I(law_table, i1);                             \
	    d2 = VIS_LD_U8_I(law_table, i2);                             \
	    d3 = VIS_LD_U8_I(law_table, i3);                             \
	    d4 = VIS_LD_U8_I(law_table, i4);                             \
	    d5 = VIS_LD_U8_I(law_table, i5);                             \
	    d6 = VIS_LD_U8_I(law_table, i6);                             \
	    d7 = VIS_LD_U8_I(law_table, i7);                             \
	    dm0 = vis_fpmerge(vis_read_lo(d0), vis_read_lo(d4));         \
	    dm1 = vis_fpmerge(vis_read_lo(d2), vis_read_lo(d6));         \
	    dm2 = vis_fpmerge(vis_read_lo(d1), vis_read_lo(d5));         \
	    dm3 = vis_fpmerge(vis_read_lo(d3), vis_read_lo(d7));         \
	    dmm0 = vis_fpmerge(vis_read_lo(dm0), vis_read_lo(dm1));      \
	    dmm1 = vis_fpmerge(vis_read_lo(dm2), vis_read_lo(dm3));      \
	    accum = vis_fpmerge(vis_read_lo(dmm0), vis_read_lo(dmm1));   \
	    *(mlib_d64 *)(dst + i) = accum;                              \
	    l0 = l1;                                                     \
	}

/* *********************************************************** */

static void
mlib_v_SignalLaw2Law(
    mlib_u8 *dst,
    const mlib_u8 *src,
    mlib_s32 n,
    const mlib_u8 *law_table)
{
	mlib_s32 i, sel;
	mlib_d64 accum;
	mlib_u64 l0, l1;
	mlib_d64 d0, d1, d2, d3, d4, d5, d6, d7;
	mlib_d64 dm0, dm1, dm2, dm3, dmm0, dmm1;

	for (i = 0; (i < n) && ((mlib_addr)(dst + i) & 7); i++) {
		dst[i] = law_table[src[i]];
	}

	if (i < n - 7) {
		sel = (mlib_addr)(src + i) & 7;
		l0 = *(mlib_u64 *)(src + i - sel);

		if (sel == 0) {
#pragma pipeloop(0)
			LAW2LAW_MAIN_LOOP(0,
			    l0 >> 56,
			    (l0 >> 48) & 0xFF,
			    (l0 >> 40) & 0xFF,
			    (l0 >> 32) & 0xFF,
			    (l0 >> 24) & 0xFF,
			    (l0 >> 16) & 0xFF, (l0 >> 8) & 0xFF, l0 & 0xFF)
		} else if (sel == 1) {
#pragma pipeloop(0)
			LAW2LAW_MAIN_LOOP(1,
			    (l0 >> 48) & 0xFF,
			    (l0 >> 40) & 0xFF,
			    (l0 >> 32) & 0xFF,
			    (l0 >> 24) & 0xFF,
			    (l0 >> 16) & 0xFF,
			    (l0 >> 8) & 0xFF, l0 & 0xFF, l1 >> 56)
		} else if (sel == 2) {
#pragma pipeloop(0)
			LAW2LAW_MAIN_LOOP(2,
			    (l0 >> 40) & 0xFF,
			    (l0 >> 32) & 0xFF,
			    (l0 >> 24) & 0xFF,
			    (l0 >> 16) & 0xFF,
			    (l0 >> 8) & 0xFF,
			    l0 & 0xFF, l1 >> 56, (l1 >> 48) & 0xFF)
		} else if (sel == 3) {
#pragma pipeloop(0)
			LAW2LAW_MAIN_LOOP(3,
			    (l0 >> 32) & 0xFF,
			    (l0 >> 24) & 0xFF,
			    (l0 >> 16) & 0xFF,
			    (l0 >> 8) & 0xFF,
			    l0 & 0xFF,
			    l1 >> 56, (l1 >> 48) & 0xFF, (l1 >> 40) & 0xFF)
		} else if (sel == 4) {
#pragma pipeloop(0)
			LAW2LAW_MAIN_LOOP(4,
			    (l0 >> 24) & 0xFF,
			    (l0 >> 16) & 0xFF,
			    (l0 >> 8) & 0xFF,
			    l0 & 0xFF,
			    l1 >> 56,
			    (l1 >> 48) & 0xFF,
			    (l1 >> 40) & 0xFF, (l1 >> 32) & 0xFF)
		} else if (sel == 5) {
#pragma pipeloop(0)
			LAW2LAW_MAIN_LOOP(5,
			    (l0 >> 16) & 0xFF,
			    (l0 >> 8) & 0xFF,
			    l0 & 0xFF,
			    l1 >> 56,
			    (l1 >> 48) & 0xFF,
			    (l1 >> 40) & 0xFF,
			    (l1 >> 32) & 0xFF, (l1 >> 24) & 0xFF)
		} else if (sel == 6) {
#pragma pipeloop(0)
			LAW2LAW_MAIN_LOOP(6,
			    (l0 >> 8) & 0xFF,
			    l0 & 0xFF,
			    l1 >> 56,
			    (l1 >> 48) & 0xFF,
			    (l1 >> 40) & 0xFF,
			    (l1 >> 32) & 0xFF,
			    (l1 >> 24) & 0xFF, (l1 >> 16) & 0xFF)
		} else {
#pragma pipeloop(0)
			LAW2LAW_MAIN_LOOP(7,
			    l0 & 0xFF,
			    l1 >> 56,
			    (l1 >> 48) & 0xFF,
			    (l1 >> 40) & 0xFF,
			    (l1 >> 32) & 0xFF,
			    (l1 >> 24) & 0xFF,
			    (l1 >> 16) & 0xFF, (l1 >> 8) & 0xFF)
		}
	}

	for (; i < n; i++) {
		dst[i] = law_table[src[i]];
	}
}

/* *********************************************************** */

mlib_status
__mlib_SignalALaw2uLaw(
    mlib_u8 *ucode,
    const mlib_u8 *acode,
    mlib_s32 n)
{
	if ((ucode == NULL) | (acode == NULL))
		return (MLIB_FAILURE);

	if (n <= 0)
		return (MLIB_FAILURE);

	mlib_v_SignalLaw2Law(ucode, acode, n, mlib_a2u);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignaluLaw2ALaw(
    mlib_u8 *acode,
    const mlib_u8 *ucode,
    mlib_s32 n)
{
	if ((ucode == NULL) | (acode == NULL))
		return (MLIB_FAILURE);

	if (n <= 0)
		return (MLIB_FAILURE);

	mlib_v_SignalLaw2Law(acode, ucode, n, mlib_u2a);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
