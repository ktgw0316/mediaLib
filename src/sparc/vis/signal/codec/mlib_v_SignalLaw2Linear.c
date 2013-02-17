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

#pragma ident	"@(#)mlib_v_SignalLaw2Linear.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalALaw2Linear - Functions in this section perform A-law
 *      decompression in compliance with the ITU (former CCITT) specification.
 *
 *      mlib_SignaluLaw2Linear - Functions in this section perform u-law
 *      decompression in compliance with the ITU (former CCITT) specification.
 *
 * SYNOPSIS
 *      mlib_status mlib_SignalALaw2Linear(mlib_s16      *pcm,
 *                                         const mlib_u8 *acode,
 *                                         mlib_s32      n)
 *
 *      mlib_status mlib_SignaluLaw2Linear(mlib_s16      *pcm,
 *                                         const mlib_u8 *ucode,
 *                                         mlib_s32      n)
 *
 * ARGUMENTS
 *      pcm      Linear PCM sample array
 *      acode    A-law code array
 *      ucode    u-law code array
 *      n        Number of samples in the input array
 */

#include <mlib_signal.h>
#include <mlib_SignalCodec.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalALaw2Linear = __mlib_SignalALaw2Linear
#pragma weak mlib_SignaluLaw2Linear = __mlib_SignaluLaw2Linear

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalALaw2Linear) mlib_SignalALaw2Linear
    __attribute__((weak, alias("__mlib_SignalALaw2Linear")));
__typeof__(__mlib_SignaluLaw2Linear) mlib_SignaluLaw2Linear
    __attribute__((weak, alias("__mlib_SignaluLaw2Linear")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	VIS_LD_U16_I(ptr, i)	vis_ld_u16_i((void *)(ptr), (long)(i))

/* *********************************************************** */

#define	LAW2LINEAR_MAIN_LOOP(sel, i0, i1, i2, i3, i4, i5, i6, i7)         \
	for (; i < n - 7; i += 8) {                                       \
	    l1 = vis_ld_u64_nf((mlib_u64 *)(sp + i + (8 - sel)));         \
	    d0 = VIS_LD_U16_I(linearTable, i0);                           \
	    d1 = VIS_LD_U16_I(linearTable, i1);                           \
	    d2 = VIS_LD_U16_I(linearTable, i2);                           \
	    d3 = VIS_LD_U16_I(linearTable, i3);                           \
	    d4 = VIS_LD_U16_I(linearTable, i4);                           \
	    d5 = VIS_LD_U16_I(linearTable, i5);                           \
	    d6 = VIS_LD_U16_I(linearTable, i6);                           \
	    d7 = VIS_LD_U16_I(linearTable, i7);                           \
	    dm0 = vis_fpmerge(vis_read_lo(d0), vis_read_lo(d2));          \
	    dm1 = vis_fpmerge(vis_read_lo(d1), vis_read_lo(d3));          \
	    dm2 = vis_fpmerge(vis_read_lo(d4), vis_read_lo(d6));          \
	    dm3 = vis_fpmerge(vis_read_lo(d5), vis_read_lo(d7));          \
	    dmm0 = vis_fpmerge(vis_read_lo(dm0), vis_read_lo(dm1));       \
	    dmm1 = vis_fpmerge(vis_read_lo(dm2), vis_read_lo(dm3));       \
	    accum0 = vis_fpmerge(vis_read_hi(dmm0), vis_read_lo(dmm0));   \
	    accum1 = vis_fpmerge(vis_read_hi(dmm1), vis_read_lo(dmm1));   \
	    *(mlib_d64 *)(dp + i) = accum0;                               \
	    *(mlib_d64 *)(dp + i + 4) = accum1;                           \
	    l0 = l1;                                                      \
	}

/* *********************************************************** */

static void
mlib_v_SignalLaw2Linear(
    mlib_s16 *pcm,
    const mlib_u8 *code,
    mlib_s32 n,
    const mlib_u16 *linearTable)
{
	mlib_u8 *sp = (mlib_u8 *)code;
	mlib_u16 *dp = (mlib_u16 *)pcm;
	mlib_s32 i, sel;
	mlib_d64 accum0, accum1;
	mlib_u64 l0, l1;
	mlib_d64 d0, d1, d2, d3, d4, d5, d6, d7;
	mlib_d64 dm0, dm1, dm2, dm3, dmm0, dmm1;

	for (i = 0; (i < n) && ((mlib_addr)(dp + i) & 7); i++) {
		dp[i] = linearTable[sp[i]];
	}

	if (i < n - 7) {
		sel = (mlib_addr)(sp + i) & 7;
		l0 = *(mlib_u64 *)(sp + i - sel);

		if (sel == 0) {
#pragma pipeloop(0)
			LAW2LINEAR_MAIN_LOOP(0,
			    (l0 >> 55) & 0x1FE,
			    (l0 >> 47) & 0x1FE,
			    (l0 >> 39) & 0x1FE,
			    (l0 >> 31) & 0x1FE,
			    (l0 >> 23) & 0x1FE,
			    (l0 >> 15) & 0x1FE,
			    (l0 >> 7) & 0x1FE, (l0 << 1) & 0x1FE)
		} else if (sel == 1) {
#pragma pipeloop(0)
			LAW2LINEAR_MAIN_LOOP(1,
			    (l0 >> 47) & 0x1FE,
			    (l0 >> 39) & 0x1FE,
			    (l0 >> 31) & 0x1FE,
			    (l0 >> 23) & 0x1FE,
			    (l0 >> 15) & 0x1FE,
			    (l0 >> 7) & 0x1FE,
			    (l0 << 1) & 0x1FE, (l1 >> 55) & 0x1FE)
		} else if (sel == 2) {
#pragma pipeloop(0)
			LAW2LINEAR_MAIN_LOOP(2,
			    (l0 >> 39) & 0x1FE,
			    (l0 >> 31) & 0x1FE,
			    (l0 >> 23) & 0x1FE,
			    (l0 >> 15) & 0x1FE,
			    (l0 >> 7) & 0x1FE,
			    (l0 << 1) & 0x1FE,
			    (l1 >> 55) & 0x1FE, (l1 >> 47) & 0x1FE)
		} else if (sel == 3) {
#pragma pipeloop(0)
			LAW2LINEAR_MAIN_LOOP(3,
			    (l0 >> 31) & 0x1FE,
			    (l0 >> 23) & 0x1FE,
			    (l0 >> 15) & 0x1FE,
			    (l0 >> 7) & 0x1FE,
			    (l0 << 1) & 0x1FE,
			    (l1 >> 55) & 0x1FE,
			    (l1 >> 47) & 0x1FE, (l1 >> 39) & 0x1FE)
		} else if (sel == 4) {
#pragma pipeloop(0)
			LAW2LINEAR_MAIN_LOOP(4,
			    (l0 >> 23) & 0x1FE,
			    (l0 >> 15) & 0x1FE,
			    (l0 >> 7) & 0x1FE,
			    (l0 << 1) & 0x1FE,
			    (l1 >> 55) & 0x1FE,
			    (l1 >> 47) & 0x1FE,
			    (l1 >> 39) & 0x1FE, (l1 >> 31) & 0x1FE)
		} else if (sel == 5) {
#pragma pipeloop(0)
			LAW2LINEAR_MAIN_LOOP(5,
			    (l0 >> 15) & 0x1FE,
			    (l0 >> 7) & 0x1FE,
			    (l0 << 1) & 0x1FE,
			    (l1 >> 55) & 0x1FE,
			    (l1 >> 47) & 0x1FE,
			    (l1 >> 39) & 0x1FE,
			    (l1 >> 31) & 0x1FE, (l1 >> 23) & 0x1FE)
		} else if (sel == 6) {
#pragma pipeloop(0)
			LAW2LINEAR_MAIN_LOOP(6,
			    (l0 >> 7) & 0x1FE,
			    (l0 << 1) & 0x1FE,
			    (l1 >> 55) & 0x1FE,
			    (l1 >> 47) & 0x1FE,
			    (l1 >> 39) & 0x1FE,
			    (l1 >> 31) & 0x1FE,
			    (l1 >> 23) & 0x1FE, (l1 >> 15) & 0x1FE)
		} else {
#pragma pipeloop(0)
			LAW2LINEAR_MAIN_LOOP(7,
			    (l0 << 1) & 0x1FE,
			    (l1 >> 55) & 0x1FE,
			    (l1 >> 47) & 0x1FE,
			    (l1 >> 39) & 0x1FE,
			    (l1 >> 31) & 0x1FE,
			    (l1 >> 23) & 0x1FE,
			    (l1 >> 15) & 0x1FE, (l1 >> 7) & 0x1FE)
		}
	}

	for (; i < n; i++) {
		dp[i] = linearTable[sp[i]];
	}
}

/* *********************************************************** */

mlib_status
__mlib_SignalALaw2Linear(
    mlib_s16 *pcm,
    const mlib_u8 *acode,
    mlib_s32 n)
{
	if ((pcm == NULL) | (acode == NULL))
		return (MLIB_FAILURE);

	if (n <= 0)
		return (MLIB_OUTOFRANGE);

	mlib_v_SignalLaw2Linear(pcm, acode, n, mlib_A2l);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignaluLaw2Linear(
    mlib_s16 *pcm,
    const mlib_u8 *ucode,
    mlib_s32 n)
{
	if ((pcm == NULL) | (ucode == NULL))
		return (MLIB_FAILURE);

	if (n <= 0)
		return (MLIB_OUTOFRANGE);

	mlib_v_SignalLaw2Linear(pcm, ucode, n, mlib_u2l);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
