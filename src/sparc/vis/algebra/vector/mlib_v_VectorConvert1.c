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

#pragma ident	"@(#)mlib_v_VectorConvert1.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorConvert_S16_U8_Mod  - copy with expand U8->S16
 *      mlib_VectorConvert_S32_U8_Mod  - copy with expand U8->S32
 *      mlib_VectorConvert_S16_S8_Mod  - copy with expand S8->S16
 *      mlib_VectorConvert_S32_S8_Mod  - copy with expand S8->S32
 *      mlib_VectorConvert_S32_S16_Mod - copy with expand S16->S32
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorConvert_S16C_S8C_Mod(mlib_s16      *z,
 *                                                  const mlib_s8 *x,
 *                                                  mlib_s32      n);
 *      mlib_status mlib_VectorConvert_S16C_U8C_Mod(mlib_s16      *z,
 *                                                  const mlib_u8 *x,
 *                                                  mlib_s32      n);
 *      mlib_status mlib_VectorConvert_S16_S8_Mod(mlib_s16      *z,
 *                                                const mlib_s8 *x,
 *                                                mlib_s32      n);
 *      mlib_status mlib_VectorConvert_S16_U8_Mod(mlib_s16      *z,
 *                                                const mlib_u8 *x,
 *                                                mlib_s32      n);
 *      mlib_status mlib_VectorConvert_S32C_S16C_Mod(mlib_s32       *z,
 *                                                   const mlib_s16 *x,
 *                                                   mlib_s32       n);
 *      mlib_status mlib_VectorConvert_S32C_S8C_Mod(mlib_s32      *z,
 *                                                  const mlib_s8 *x,
 *                                                  mlib_s32      n);
 *      mlib_status mlib_VectorConvert_S32C_U8C_Mod(mlib_s32      *z,
 *                                                  const mlib_u8 *x,
 *                                                  mlib_s32      n);
 *      mlib_status mlib_VectorConvert_S32_S16_Mod(mlib_s32       *z,
 *                                                 const mlib_s16 *x,
 *                                                 mlib_s32       n);
 *      mlib_status mlib_VectorConvert_S32_S8_Mod(mlib_s32      *z,
 *                                                const mlib_s8 *x,
 *                                                mlib_s32      n);
 *      mlib_status mlib_VectorConvert_S32_U8_Mod(mlib_s32      *z,
 *                                                const mlib_u8 *x,
 *                                                mlib_s32      n);
 *
 * ARGUMENTS
 *      z    pointer to the first element of the result vector
 *      x    pointer to the first element of the input vector
 *      n    number of elements in the vectors
 *
 * DESCRIPTION
 *      z = x
 */

#include <mlib_algebra.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorConvert_S16C_U8C_Mod = \
	__mlib_VectorConvert_S16C_U8C_Mod
#pragma weak mlib_VectorConvert_S32_S16_Mod = \
	__mlib_VectorConvert_S32_S16_Mod
#pragma weak mlib_VectorConvert_S32_S8_Mod = __mlib_VectorConvert_S32_S8_Mod
#pragma weak mlib_VectorConvert_S32_U8_Mod = __mlib_VectorConvert_S32_U8_Mod
#pragma weak mlib_VectorConvert_S16_S8_Mod = __mlib_VectorConvert_S16_S8_Mod
#pragma weak mlib_VectorConvert_S16_U8_Mod = __mlib_VectorConvert_S16_U8_Mod
#pragma weak mlib_VectorConvert_S32C_S16C_Mod = \
	__mlib_VectorConvert_S32C_S16C_Mod
#pragma weak mlib_VectorConvert_S32C_S8C_Mod = \
	__mlib_VectorConvert_S32C_S8C_Mod
#pragma weak mlib_VectorConvert_S32C_U8C_Mod = \
	__mlib_VectorConvert_S32C_U8C_Mod
#pragma weak mlib_VectorConvert_S16C_S8C_Mod = \
	__mlib_VectorConvert_S16C_S8C_Mod

#elif defined(__GNUC__)

__typeof__(__mlib_VectorConvert_S16C_U8C_Mod) mlib_VectorConvert_S16C_U8C_Mod
	__attribute__((weak, alias("__mlib_VectorConvert_S16C_U8C_Mod")));
__typeof__(__mlib_VectorConvert_S32_S16_Mod) mlib_VectorConvert_S32_S16_Mod
	__attribute__((weak, alias("__mlib_VectorConvert_S32_S16_Mod")));
__typeof__(__mlib_VectorConvert_S32_S8_Mod) mlib_VectorConvert_S32_S8_Mod
	__attribute__((weak, alias("__mlib_VectorConvert_S32_S8_Mod")));
__typeof__(__mlib_VectorConvert_S32_U8_Mod) mlib_VectorConvert_S32_U8_Mod
	__attribute__((weak, alias("__mlib_VectorConvert_S32_U8_Mod")));
__typeof__(__mlib_VectorConvert_S16_S8_Mod) mlib_VectorConvert_S16_S8_Mod
	__attribute__((weak, alias("__mlib_VectorConvert_S16_S8_Mod")));
__typeof__(__mlib_VectorConvert_S16_U8_Mod) mlib_VectorConvert_S16_U8_Mod
	__attribute__((weak, alias("__mlib_VectorConvert_S16_U8_Mod")));
__typeof__(__mlib_VectorConvert_S32C_S16C_Mod) mlib_VectorConvert_S32C_S16C_Mod
	__attribute__((weak, alias("__mlib_VectorConvert_S32C_S16C_Mod")));
__typeof__(__mlib_VectorConvert_S32C_S8C_Mod) mlib_VectorConvert_S32C_S8C_Mod
	__attribute__((weak, alias("__mlib_VectorConvert_S32C_S8C_Mod")));
__typeof__(__mlib_VectorConvert_S32C_U8C_Mod) mlib_VectorConvert_S32C_U8C_Mod
	__attribute__((weak, alias("__mlib_VectorConvert_S32C_U8C_Mod")));
__typeof__(__mlib_VectorConvert_S16C_S8C_Mod) mlib_VectorConvert_S16C_S8C_Mod
	__attribute__((weak, alias("__mlib_VectorConvert_S16C_S8C_Mod")));

#else

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	EXPAND(form1, form2)                                    \
	form1 *src = (form1 *) x;                               \
	form2 *ddst = (form2 *) z;                              \
	mlib_s32 i;                                             \
	                                                        \
	for (i = 0; i < n; i++)                                 \
	    (*ddst++) = (*src++);                               \
	return (n > 0) ? MLIB_SUCCESS : MLIB_FAILURE

/*
 *
 * IMPLEMENTATION
 *
 * If length < 16, use loop element assignment in terms of
 * input and output format.
 *
 * For large lengths we use reading by 2 words (or in case
 * of bad alignment vis_aligndata) and then transformation of it
 * into 8*16 elements via vis_fpmerge, and 2 64-bit stores to memory.
 *
 */

mlib_status
__mlib_VectorConvert_S16_U8_Mod(
	mlib_s16 *z,
	const mlib_u8 *x,
	mlib_s32 n)
{
	mlib_s32 i;
	const mlib_u8 *src = x;
	mlib_s16 *dst = z;
	mlib_d64 *ddsrc, *ddst;
	mlib_s32 len_64, even_length, rest_64, length = n;
	mlib_f32 fzero = vis_fzeros();
	mlib_d64 dd1, dd2, dd3, dd4;

	if (length < 16) {
		EXPAND(mlib_u8,
			mlib_s16);
	}

	while ((mlib_addr)dst & 7) {
		(*dst++) = (*src++);
		length--;
	}

	ddsrc = (mlib_d64 *)vis_alignaddr((void *)src, 0);
	ddst = (mlib_d64 *)dst;
	rest_64 = length & 7;
	len_64 = length >> 3;
	even_length = len_64 << 3;
	dd2 = ddsrc[0];

	if (!((mlib_addr)src & 7)) {

/*
 * Both vectors are 64-bit aligned. We can process without
 * vis_faligndata
 * Peeling the 1 iteration. Then loop with step==2.
 */

		if (i = (len_64 & 1)) {
			dd1 = (*ddsrc++);
			(*ddst++) = vis_fpmerge(fzero, vis_read_hi(dd1));
			(*ddst++) = vis_fpmerge(fzero, vis_read_lo(dd1));
		}
#pragma pipeloop(0)
		for (; i < len_64; i += 2) {
			dd1 = (*ddsrc++);
			dd2 = (*ddsrc++);
			(*ddst++) = vis_fpmerge(fzero, vis_read_hi(dd1));
			(*ddst++) = vis_fpmerge(fzero, vis_read_lo(dd1));
			(*ddst++) = vis_fpmerge(fzero, vis_read_hi(dd2));
			(*ddst++) = vis_fpmerge(fzero, vis_read_lo(dd2));
		}
	} else {

/*
 * Source vector is not 64-bit aligned. Use vis_faligndata.
 * Peeling the 1 iteration. Then loop with step==2.
 */

		i = 1;

		if (len_64 & 1) {
			dd1 = dd2;
			dd2 = ddsrc[i++];
			dd3 = vis_faligndata(dd1, dd2);
			(*ddst++) = vis_fpmerge(fzero, vis_read_hi(dd3));
			(*ddst++) = vis_fpmerge(fzero, vis_read_lo(dd3));
		}
#pragma pipeloop(0)
		for (; i <= len_64; i += 2) {
			dd1 = dd2;
			dd2 = ddsrc[i];
			dd3 = vis_faligndata(dd1, dd2);
			dd1 = dd2;
			dd2 = ddsrc[i + 1];
			dd4 = vis_faligndata(dd1, dd2);
			(*ddst++) = vis_fpmerge(fzero, vis_read_hi(dd3));
			(*ddst++) = vis_fpmerge(fzero, vis_read_lo(dd3));
			(*ddst++) = vis_fpmerge(fzero, vis_read_hi(dd4));
			(*ddst++) = vis_fpmerge(fzero, vis_read_lo(dd4));
		}
	}

	for (i = 0; i < rest_64; i++)
		dst[even_length + i] = src[even_length + i];

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S32_U8_Mod(
	mlib_s32 *z,
	const mlib_u8 *x,
	mlib_s32 n)
{
	mlib_u8 *psrc = (mlib_u8 *)x;
	mlib_s32 *pdst = (mlib_s32 *)z;
	mlib_f32 fzero = vis_fzero(), fone1 = vis_to_float(0x100), fone2 =
		vis_to_float(0x10001);
	mlib_d64 *dpsrc, dsrc0, dsrc1, dsrc, dst0, dst1, dst2, dst3;
	mlib_s32 i = 0;

	if (n <= 0)
		return (MLIB_FAILURE);

	if ((mlib_addr)pdst & 7) {
		(*pdst++) = (*psrc++);
		i = 1;
	}

	dpsrc = (mlib_d64 *)vis_alignaddr(psrc, 0);
	dsrc = vis_ld_d64_nf(dpsrc);

	if ((mlib_addr)psrc & 7) {
		dsrc1 = vis_ld_d64_nf(dpsrc + 1);
		dsrc = vis_faligndata(dsrc, dsrc1);
#pragma pipeloop(0)
		for (; i <= (n - 8); i += 8) {
			dst1 = vis_fmul8x16al(vis_read_hi(dsrc), fone1);
			dst0 = vis_fpmerge(fzero, vis_read_hi(dst1));
			dst1 = vis_fpmerge(fzero, vis_read_lo(dst1));
			dst3 = vis_fpmerge(vis_read_lo(dsrc),
				vis_read_lo(dsrc));
			dst2 = vis_fmuld8ulx16(vis_read_hi(dst3), fone2);
			dst3 = vis_fmuld8ulx16(vis_read_lo(dst3), fone2);

			dsrc0 = vis_ld_d64_nf(dpsrc + 1);
			dsrc1 = vis_ld_d64_nf(dpsrc + 2);
			dsrc = vis_faligndata(dsrc0, dsrc1);

			((mlib_d64 *)pdst)[0] = dst0;
			((mlib_d64 *)pdst)[1] = dst1;
			((mlib_d64 *)pdst)[2] = dst2;
			((mlib_d64 *)pdst)[3] = dst3;
			pdst += 8;
			psrc += 8;
			dpsrc++;
		}
	} else {
#pragma pipeloop(0)
		for (; i <= (n - 8); i += 8) {
			dst1 = vis_fmul8x16al(vis_read_hi(dsrc), fone1);
			dst0 = vis_fpmerge(fzero, vis_read_hi(dst1));
			dst1 = vis_fpmerge(fzero, vis_read_lo(dst1));
			dst3 = vis_fpmerge(vis_read_lo(dsrc),
				vis_read_lo(dsrc));
			dst2 = vis_fmuld8ulx16(vis_read_hi(dst3), fone2);
			dst3 = vis_fmuld8ulx16(vis_read_lo(dst3), fone2);

			dsrc = vis_ld_d64_nf(dpsrc + 1);
			((mlib_d64 *)pdst)[0] = dst0;
			((mlib_d64 *)pdst)[1] = dst1;
			((mlib_d64 *)pdst)[2] = dst2;
			((mlib_d64 *)pdst)[3] = dst3;
			pdst += 8;
			psrc += 8;
			dpsrc++;
		}
	}

	for (; i < n; i++)
		(*pdst++) = (*psrc++);

	return (MLIB_SUCCESS);
}

/*
 *
 * IMPLEMENTATION
 *
 * If length < 16, use loop element assignment in terms of
 * input and output format.
 *
 * For large lengths we use reading by 2 words (or in case
 * of bad alignment vis_aligndata) and then transformation of it
 * into 8*16 elements via vis_fpmerge, vis_fmul8sux16(<merge>, <1>),
 * and 2 64-bit stores to memory.
 *
 */

mlib_status
__mlib_VectorConvert_S16_S8_Mod(
	mlib_s16 *z,
	const mlib_s8 *x,
	mlib_s32 n)
{
	mlib_s32 i;
	const mlib_s8 *src = x;
	mlib_s16 *dst = z;
	mlib_d64 *ddsrc, *ddst;
	mlib_d64 four_16_ones = vis_to_double_dup(0x01000100);
	mlib_f32 fzero = vis_fzeros();
	mlib_s32 len_64, even_length, rest_64, length = n;
	mlib_d64 dd1, dd2, dd3, dd4, dd5, dd6, dd7;

	if (length < 16) {
		EXPAND(mlib_s8,
			mlib_s16);
	}

	while ((mlib_addr)dst & 7) {
		(*dst++) = (*src++);
		length--;
	}

	ddsrc = (mlib_d64 *)vis_alignaddr((void *)src, 0);
	ddst = (mlib_d64 *)dst;
	rest_64 = length & 7;
	len_64 = length >> 3;
	even_length = len_64 << 3;
	dd2 = ddsrc[0];

	if (!((mlib_addr)src & 7)) {

/*
 * Both vectors are 64-bit aligned. We can process without
 * vis_faligndata
 */

/*
 * Peeling of 1 iteration.
 */

		if (i = (len_64 & 1)) {
			dd1 = (*ddsrc++);
			(*ddst++) =
				vis_fmul8sux16(vis_fpmerge(vis_read_hi(dd1),
				fzero), four_16_ones);
			(*ddst++) =
				vis_fmul8sux16(vis_fpmerge(vis_read_lo(dd1),
				fzero), four_16_ones);
		}
#pragma pipeloop(0)
		for (; i < len_64; i += 2) {
			dd1 = (*ddsrc++);
			dd2 = (*ddsrc++);
			(*ddst++) =
				vis_fmul8sux16(vis_fpmerge(vis_read_hi(dd1),
				fzero), four_16_ones);
			(*ddst++) =
				vis_fmul8sux16(vis_fpmerge(vis_read_lo(dd1),
				fzero), four_16_ones);
			(*ddst++) =
				vis_fmul8sux16(vis_fpmerge(vis_read_hi(dd2),
				fzero), four_16_ones);
			(*ddst++) =
				vis_fmul8sux16(vis_fpmerge(vis_read_lo(dd2),
				fzero), four_16_ones);
		}
	} else {

/*
 * Source vector is not 64-bit aligned. Use vis_faligndata.
 * Peeling of 1 iteration. Then loop with step==2.
 */

		i = 1;

		if (len_64 & 1) {
			dd1 = dd2;
			dd2 = ddsrc[i++];
			dd3 = vis_faligndata(dd1, dd2);
			dd4 = vis_fpmerge(vis_read_hi(dd3), fzero);
			(*ddst++) = vis_fmul8sux16(dd4, four_16_ones);
			dd5 = vis_fpmerge(vis_read_lo(dd3), fzero);
			(*ddst++) = vis_fmul8sux16(dd5, four_16_ones);
		}
#pragma pipeloop(0)
		for (; i <= len_64; i += 2) {
			dd1 = dd2;
			dd2 = ddsrc[i];
			dd3 = vis_faligndata(dd1, dd2);
			dd1 = dd2;
			dd2 = ddsrc[i + 1];
			dd1 = vis_faligndata(dd1, dd2);
			dd4 = vis_fpmerge(vis_read_hi(dd3), fzero);
			dd5 = vis_fpmerge(vis_read_lo(dd3), fzero);
			dd6 = vis_fpmerge(vis_read_hi(dd1), fzero);
			dd7 = vis_fpmerge(vis_read_lo(dd1), fzero);
			(*ddst++) = vis_fmul8sux16(dd4, four_16_ones);
			(*ddst++) = vis_fmul8sux16(dd5, four_16_ones);
			(*ddst++) = vis_fmul8sux16(dd6, four_16_ones);
			(*ddst++) = vis_fmul8sux16(dd7, four_16_ones);
		}
	}

	for (i = 0; i < rest_64; i++)
		dst[even_length + i] = src[even_length + i];

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S32_S8_Mod(
	mlib_s32 *z,
	const mlib_s8 *x,
	mlib_s32 n)
{
	mlib_s8 *psrc = (mlib_s8 *)x;
	mlib_s32 *pdst = (mlib_s32 *)z;
	mlib_f32 fone = vis_to_float(0x10001);
	mlib_d64 *dpsrc, dsrc0, dsrc1, dsrc, dst0, dst1, dst2, dst3, done =
		vis_to_double_dup(0x1000100);
	mlib_s32 i = 0;

	if (n <= 0)
		return (MLIB_FAILURE);

	if ((mlib_addr)pdst & 7) {
		(*pdst++) = (*psrc++);
		i = 1;
	}

	dpsrc = (mlib_d64 *)vis_alignaddr(psrc, 0);
	dsrc = vis_ld_d64_nf(dpsrc);

	if ((mlib_addr)psrc & 7) {
		dsrc1 = vis_ld_d64_nf(dpsrc + 1);
		dsrc = vis_faligndata(dsrc, dsrc1);
#pragma pipeloop(0)
		for (; i <= (n - 8); i += 8) {
			dst1 = vis_fpmerge(vis_read_hi(dsrc),
				vis_read_hi(dsrc));
			dst1 = vis_fmul8sux16(dst1, done);
			dst0 = vis_fmuld8ulx16(fone, vis_read_hi(dst1));
			dst1 = vis_fmuld8ulx16(fone, vis_read_lo(dst1));
			dst3 = vis_fpmerge(vis_read_lo(dsrc),
				vis_read_lo(dsrc));
			dst3 = vis_fmul8sux16(dst3, done);
			dst2 = vis_fmuld8ulx16(fone, vis_read_hi(dst3));
			dst3 = vis_fmuld8ulx16(fone, vis_read_lo(dst3));

			dsrc0 = vis_ld_d64_nf(dpsrc + 1);
			dsrc1 = vis_ld_d64_nf(dpsrc + 2);
			dsrc = vis_faligndata(dsrc0, dsrc1);

			((mlib_d64 *)pdst)[0] = dst0;
			((mlib_d64 *)pdst)[1] = dst1;
			((mlib_d64 *)pdst)[2] = dst2;
			((mlib_d64 *)pdst)[3] = dst3;
			pdst += 8;
			psrc += 8;
			dpsrc++;
		}
	} else {
#pragma pipeloop(0)
		for (; i <= (n - 8); i += 8) {
			dst1 = vis_fpmerge(vis_read_hi(dsrc),
				vis_read_hi(dsrc));
			dst1 = vis_fmul8sux16(dst1, done);
			dst0 = vis_fmuld8ulx16(fone, vis_read_hi(dst1));
			dst1 = vis_fmuld8ulx16(fone, vis_read_lo(dst1));
			dst3 = vis_fpmerge(vis_read_lo(dsrc),
				vis_read_lo(dsrc));
			dst3 = vis_fmul8sux16(dst3, done);
			dst2 = vis_fmuld8ulx16(fone, vis_read_hi(dst3));
			dst3 = vis_fmuld8ulx16(fone, vis_read_lo(dst3));

			dsrc = vis_ld_d64_nf(dpsrc + 1);
			((mlib_d64 *)pdst)[0] = dst0;
			((mlib_d64 *)pdst)[1] = dst1;
			((mlib_d64 *)pdst)[2] = dst2;
			((mlib_d64 *)pdst)[3] = dst3;
			pdst += 8;
			psrc += 8;
			dpsrc++;
		}
	}

	for (; i < n; i++)
		(*pdst++) = (*psrc++);

	return (MLIB_SUCCESS);
}

/*
 *
 * IMPLEMENTATION
 *
 * If length < 16, use loop element assignment in terms of
 * input and output format.
 *
 * For large lengths we use reading by 2 words (or in case
 * of bad alignment vis_aligndata) and then transformation of it
 * into 4*32 elements via vis_fpmerge -> vis_fmuld8ulx16
 * and 2 64-bit stores to memory.
 *
 */

mlib_status
__mlib_VectorConvert_S32_S16_Mod(
	mlib_s32 *z,
	const mlib_s16 *x,
	mlib_s32 n)
{
	mlib_s32 i;
	const mlib_s16 *src = x;
	mlib_s32 *dst = z;
	mlib_d64 *ddsrc, *ddst;
	mlib_s32 len_64, even_length, rest_64, length = n;
	mlib_d64 dd1, dd2, dd3, dd4;
	mlib_f32 two_16_ones = vis_to_float(0x10001);

	if (length < 16) {

		EXPAND(mlib_s16,
			mlib_s32);
	}

	while ((mlib_addr)dst & 7) {
		(*dst++) = (*src++);
		length--;
	}

	ddsrc = (mlib_d64 *)vis_alignaddr((void *)src, 0);
	ddst = (mlib_d64 *)dst;
	rest_64 = length & 3;
	len_64 = length >> 2;
	even_length = len_64 << 2;
	dd2 = ddsrc[0];

	if (!((mlib_addr)(src) & 7)) {

/*
 * Source vector is 64-bit aligned. We can process it without
 * vis_faligndata.
 * Peeling of 1 iteration.
 */

		if (i = (len_64 & 1)) {
			dd3 = (*ddsrc++);
/*
 * Now obtaining of the 4*32 - signed objects
 */
			(*ddst++) =
				vis_fmuld8ulx16(two_16_ones, vis_read_hi(dd3));
			(*ddst++) =
				vis_fmuld8ulx16(two_16_ones, vis_read_lo(dd3));
		}
#pragma pipeloop(0)
		for (; i < len_64; i += 2) {
			dd3 = (*ddsrc++);
			dd4 = (*ddsrc++);
/*
 * Now obtaining of the 4*32 - signed objects
 */
			(*ddst++) =
				vis_fmuld8ulx16(two_16_ones, vis_read_hi(dd3));
			(*ddst++) =
				vis_fmuld8ulx16(two_16_ones, vis_read_lo(dd3));
/*
 * Now obtaining of the 4*32 - signed objects
 */
			(*ddst++) =
				vis_fmuld8ulx16(two_16_ones, vis_read_hi(dd4));
			(*ddst++) =
				vis_fmuld8ulx16(two_16_ones, vis_read_lo(dd4));
		}
	} else {

/*
 * Source vector is not 64-bit aligned. Use vis_faligndata.
 * Peeling of 1 iteration.
 */

		i = 1;

		if ((len_64 & 1)) {
			i++;
			dd1 = dd2;
			dd2 = ddsrc[1];
			dd3 = vis_faligndata(dd1, dd2);
/*
 * Now obtaining of the 4*32 - signed objects
 */
			(*ddst++) =
				vis_fmuld8ulx16(two_16_ones, vis_read_hi(dd3));
			(*ddst++) =
				vis_fmuld8ulx16(two_16_ones, vis_read_lo(dd3));
		}

/*
 * Now loop with step == 2.
 */

#pragma pipeloop(0)
		for (; i <= len_64; i += 2) {
			dd1 = dd2;
			dd2 = ddsrc[i];
			dd3 = vis_faligndata(dd1, dd2);
			dd1 = dd2;
			dd2 = ddsrc[i + 1];
			dd4 = vis_faligndata(dd1, dd2);
/*
 * Now obtaining of the 4*32 - signed objects
 */
			(*ddst++) =
				vis_fmuld8ulx16(two_16_ones, vis_read_hi(dd3));
			(*ddst++) =
				vis_fmuld8ulx16(two_16_ones, vis_read_lo(dd3));
			(*ddst++) =
				vis_fmuld8ulx16(two_16_ones, vis_read_hi(dd4));
			(*ddst++) =
				vis_fmuld8ulx16(two_16_ones, vis_read_lo(dd4));
		}
	}

	for (i = 0; i < rest_64; i++)
		dst[even_length + i] = src[even_length + i];

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S16C_U8C_Mod(
	mlib_s16 *z,
	const mlib_u8 *x,
	mlib_s32 n)
{
	return (__mlib_VectorConvert_S16_U8_Mod(z, x, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S32C_U8C_Mod(
	mlib_s32 *z,
	const mlib_u8 *x,
	mlib_s32 n)
{
	return (__mlib_VectorConvert_S32_U8_Mod(z, x, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S16C_S8C_Mod(
	mlib_s16 *z,
	const mlib_s8 *x,
	mlib_s32 n)
{
	return (__mlib_VectorConvert_S16_S8_Mod(z, x, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S32C_S8C_Mod(
	mlib_s32 *z,
	const mlib_s8 *x,
	mlib_s32 n)
{
	return (__mlib_VectorConvert_S32_S8_Mod(z, x, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S32C_S16C_Mod(
	mlib_s32 *z,
	const mlib_s16 *x,
	mlib_s32 n)
{
	return (__mlib_VectorConvert_S32_S16_Mod(z, x, n + n));
}

/* *********************************************************** */
