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

#pragma ident	"@(#)mlib_v_VideoSignMagnitudeConvert.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VideoSignMagnitudeConvert -
 *              Conversion from standard 2's Complement
 *              mlib_s32 integer representation
 *              to Sign Magnitude representation and back.
 *
 * SYNOPSIS
 *
 *      mlib_status
 *      mlib_v_VideoSignMagnitudeConvert_S16(mlib_s16 *srcdst,
 *                                           mlib_s32 n);
 *      mlib_status
 *      mlib_v_VideoSignMagnitudeConvert_S16_S16(mlib_s16       *dst,
 *                                               const mlib_s16 *src,
 *                                               mlib_s32       n);
 *      mlib_status
 *      mlib_v_VideoSignMagnitudeConvert_S32(mlib_s32 *srcdst,
 *                                           mlib_s32 n);
 *      mlib_status
 *      mlib_v_VideoSignMagnitudeConvert_S32_S32(mlib_s32       *dst,
 *                                               const mlib_s32 *src,
 *                                               mlib_s32       n);
 * ARGUMENT
 *      dst     pointer to output data array
 *      src     pointer to input data array
 *      srcdst  pointer to input and output data array
 *      n       array size
 *
 * DESCRIPTION
 *
 * Conversion from standard 2's Complement signed integer representation
 * to Sign Magnitude representation and back.
 *
 *  Example for 4-bit signed integer representation :
 *
 *  Decimal    2's       Sign
 *         Complement  Magnitude
 *   -8       1000      N/A
 *   -7       1001      1111
 *   -6       1010      1110
 *   -5       1011      1101
 *   -4       1100      1100
 *   -3       1101      1011
 *   -2       1110      1010
 *   -1       1111      1001
 *    0       0000      0000
 *    1       0001      0001
 *    2       0010      0010
 *    3       0011      0011
 *    4       0100      0100
 *    5       0101      0101
 *    6       0110      0110
 *    7       0111      0111
 *
 */

#include <mlib_video.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoSignMagnitudeConvert_S16 = \
	__mlib_VideoSignMagnitudeConvert_S16
#pragma weak mlib_VideoSignMagnitudeConvert_S16_S16 = \
	__mlib_VideoSignMagnitudeConvert_S16_S16
#pragma weak mlib_VideoSignMagnitudeConvert_S32 = \
	__mlib_VideoSignMagnitudeConvert_S32
#pragma weak mlib_VideoSignMagnitudeConvert_S32_S32 = \
	__mlib_VideoSignMagnitudeConvert_S32_S32

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoSignMagnitudeConvert_S16)
	mlib_VideoSignMagnitudeConvert_S16
	__attribute__((weak, alias("__mlib_VideoSignMagnitudeConvert_S16")));
__typeof__(__mlib_VideoSignMagnitudeConvert_S16_S16)
	mlib_VideoSignMagnitudeConvert_S16_S16
	__attribute__((weak,
	alias("__mlib_VideoSignMagnitudeConvert_S16_S16")));
__typeof__(__mlib_VideoSignMagnitudeConvert_S32)
	mlib_VideoSignMagnitudeConvert_S32
	__attribute__((weak, alias("__mlib_VideoSignMagnitudeConvert_S32")));
__typeof__(__mlib_VideoSignMagnitudeConvert_S32_S32)
	mlib_VideoSignMagnitudeConvert_S32_S32
	__attribute__((weak,
	alias("__mlib_VideoSignMagnitudeConvert_S32_S32")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_VideoSignMagnitudeConvert_S16(
	mlib_s16 *srcdst,
	mlib_s32 n)
{
	mlib_d64 dval1, dval2;
	mlib_d64 zero = vis_fzero();
	mlib_d64 x8000 = vis_to_double_dup(0x80008000);
	mlib_s32 i, count, val1, val2, mask;

	if (n <= 0)
		return (MLIB_FAILURE);

	i = 0;

	count = 3 & (-(((mlib_addr)srcdst) >> 1));

	for (; i < count && i < n; i++) {
		val1 = *srcdst;
		val2 = 0x8000 - val1;
		mask = (val1 >> 15);
		*srcdst = (mlib_s16)((val1 & ~mask) + (mask & val2));
		srcdst++;
	}

#pragma pipeloop(0)
	for (; i < n - 3; i += 4) {
		dval1 = *(mlib_d64 *)srcdst;
		dval2 = vis_fpsub16(x8000, dval1);
		mask = vis_fcmplt16(dval1, zero);
		vis_pst_16(dval2, srcdst, mask);
		srcdst += 4;
	}

	for (; i < n; i++) {
		val1 = *srcdst;
		val2 = 0x8000 - val1;
		mask = (val1 >> 15);
		*srcdst = (mlib_s16)((val1 & ~mask) + (mask & val2));
		srcdst++;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoSignMagnitudeConvert_S32(
	mlib_s32 *srcdst,
	mlib_s32 n)
{
	mlib_d64 dval1, dval2;
	mlib_d64 zero = vis_fzero();
	mlib_d64 x8000 = vis_to_double_dup(0x80000000);
	mlib_s32 i, val1, val2, mask;

	if (n <= 0)
		return (MLIB_FAILURE);

	i = 0;

	if (7 & (mlib_addr)srcdst) {
		val1 = *srcdst;
		val2 = 0x80000000 - val1;
		mask = (val1 >> 31);
		*srcdst = (val1 & ~mask) + (mask & val2);
		srcdst++;
		i++;
	}
#pragma pipeloop(0)
	for (; i < n - 1; i += 2) {
		dval1 = *(mlib_d64 *)srcdst;
		dval2 = vis_fpsub32(x8000, dval1);
		mask = vis_fcmplt32(dval1, zero);
		vis_pst_32(dval2, srcdst, mask);
		srcdst += 2;
	}

	if (i < n) {
		val1 = *srcdst;
		val2 = 0x80000000 - val1;
		mask = (val1 >> 31);
		*srcdst = (val1 & ~mask) + (mask & val2);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoSignMagnitudeConvert_S16_S16(
	mlib_s16 *dst,
	const mlib_s16 *src,
	mlib_s32 n)
{
	mlib_d64 dval1, dval2;
	mlib_d64 zero = vis_fzero();
	mlib_d64 x8000 = vis_to_double_dup(0x80008000);
	mlib_s32 i, count, val1, val2, mask;
	mlib_d64 *psrc_aligned;
	mlib_d64 data_hi, data_lo;

	if (n <= 0)
		return (MLIB_FAILURE);

	i = 0;

	count = 3 & (-(((mlib_addr)dst) >> 1));

	for (; i < count && i < n; i++) {
		val1 = (*src++);
		val2 = 0x8000 - val1;
		mask = (val1 >> 15);
		(*dst++) = (mlib_s16)((val1 & ~mask) + (mask & val2));
	}

	psrc_aligned = vis_alignaddr((void *)src, 0);
	data_hi = vis_ld_d64_nf(psrc_aligned);
	psrc_aligned++;

#pragma pipeloop(0)
	for (; i < n - 3; i += 4) {
		data_lo = vis_ld_d64_nf(psrc_aligned);
		psrc_aligned++;
		dval1 = vis_faligndata(data_hi, data_lo);
		data_hi = data_lo;
		dval2 = vis_fpsub16(x8000, dval1);
		mask = vis_fcmplt16(dval1, zero);
		*(mlib_d64 *)dst = dval1;
		vis_pst_16(dval2, dst, mask);
		dst += 4;
	}

	if (i < n) {
		data_lo = vis_ld_d64_nf(psrc_aligned);
		dval1 = vis_faligndata(data_hi, data_lo);
		dval2 = vis_fpsub16(x8000, dval1);
		mask = vis_edge16(dst, dst + n - i - 1);
		vis_pst_16(dval1, dst, mask);
		mask &= vis_fcmplt16(dval1, zero);
		vis_pst_16(dval2, dst, mask);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VideoSignMagnitudeConvert_S32_S32(
	mlib_s32 *dst,
	const mlib_s32 *src,
	mlib_s32 n)
{
	mlib_d64 dval1, dval2;
	mlib_d64 zero = vis_fzero();
	mlib_d64 x8000 = vis_to_double_dup(0x80000000);
	mlib_s32 i, val1, val2, mask;
	mlib_d64 *psrc_aligned;
	mlib_d64 data_hi, data_lo;

	if (n <= 0)
		return (MLIB_FAILURE);

	i = 0;

	if (7 & (mlib_addr)dst) {
		val1 = (*src++);
		val2 = 0x80000000 - val1;
		mask = (val1 >> 31);
		(*dst++) = (val1 & ~mask) + (mask & val2);
		i++;
	}

	psrc_aligned = vis_alignaddr((void *)src, 0);
	data_hi = vis_ld_d64_nf(psrc_aligned);
	psrc_aligned++;

#pragma pipeloop(0)
	for (; i < n - 1; i += 2) {
		data_lo = vis_ld_d64_nf(psrc_aligned);
		psrc_aligned++;
		dval1 = vis_faligndata(data_hi, data_lo);
		data_hi = data_lo;

		dval2 = vis_fpsub32(x8000, dval1);
		mask = vis_fcmplt32(dval1, zero);
		*(mlib_d64 *)dst = dval1;
		vis_pst_32(dval2, dst, mask);

		dst += 2;
	}

	if (i < n) {
		dval1 = vis_faligndata(data_hi, zero);
		dval2 = vis_fpsub32(x8000, dval1);
		mask = vis_fcmplt32(dval1, zero);
		vis_pst_32(dval1, dst, 2);
		vis_pst_32(dval2, dst, mask & 2);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
