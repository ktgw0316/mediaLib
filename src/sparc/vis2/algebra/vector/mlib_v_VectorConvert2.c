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

#pragma ident	"@(#)mlib_v_VectorConvert2.c	9.4	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorConvert_S8_U8_Mod     - packing of 8-bit format vector
 *      mlib_VectorConvert_U8_S8_Mod     - packing of 8-bit format vector
 *      mlib_VectorConvert_U8_S16_Mod    - packing of 16-bit into 8-bit
 *                                         format vector
 *      mlib_VectorConvert_S8_S16_Mod    - packing of 16-bit into 8-bit
 *                                         format vector
 *      mlib_VectorConvert_U8_S32_Mod    - packing of 32-bit into 8-bit
 *                                         format vector
 *      mlib_VectorConvert_S8_S32_Mod    - packing of 32-bit into 8-bit
 *                                         format vector
 *      mlib_VectorConvert_S16_S32_Mod   - packing of 32-bit into 16-bit
 *                                         format vector
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorConvert_S16_S32_Mod(mlib_s16       *z,
 *                                                 const mlib_s32 *x,
 *                                                 mlib_s32       n);
 *      mlib_status mlib_VectorConvert_S8_S16_Mod(mlib_s8        *z,
 *                                                const mlib_s16 *x,
 *                                                mlib_s32       n);
 *      mlib_status mlib_VectorConvert_S8_S32_Mod(mlib_s8        *z,
 *                                                const mlib_s32 *x,
 *                                                mlib_s32       n);
 *      mlib_status mlib_VectorConvert_S8_U8_Mod(mlib_s8       *z,
 *                                               const mlib_u8 *x,
 *                                               mlib_s32      n);
 *      mlib_status mlib_VectorConvert_U8_S16_Mod(mlib_u8        *z,
 *                                                const mlib_s16 *x,
 *                                                mlib_s32       n);
 *      mlib_status mlib_VectorConvert_U8_S32_Mod(mlib_u8        *z,
 *                                                const mlib_s32 *x,
 *                                                mlib_s32       n);
 *      mlib_status mlib_VectorConvert_U8_S8_Mod(mlib_u8       *z,
 *                                               const mlib_s8 *x,
 *                                               mlib_s32      n);
 *
 * ARGUMENTS
 *      z    pointer to the first element of the result vector
 *      x    pointer to the first element of the input vector
 *      n    number of elements in the vectors
 *
 * DESCRIPTION
 *      z = x
 */

/*
 * FUNCTIONS
 *      mlib_VectorConvert_S8C_U8C_Mod     - packing of 8-bit format vector
 *      mlib_VectorConvert_U8C_S8C_Mod     - packing of 8-bit format vector
 *      mlib_VectorConvert_U8C_S16C_Mod    - packing of 16-bit into 8-bit
 *                                           format vector
 *      mlib_VectorConvert_S8C_S16C_Mod    - packing of 16-bit into 8-bit
 *                                           format vector
 *      mlib_VectorConvert_U8C_S32C_Mod    - packing of 32-bit into 8-bit
 *                                           format vector
 *      mlib_VectorConvert_S8C_S32C_Mod    - packing of 32-bit into 8-bit
 *                                           format vector
 *      mlib_VectorConvert_S16C_S32C_Mod   - packing of 32-bit into 16-bit
 *                                           format vector
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorConvert_S16C_S32C_Mod(mlib_s16       *z,
 *                                                   const mlib_s32 *x,
 *                                                   mlib_s32       n);
 *      mlib_status mlib_VectorConvert_S8C_S16C_Mod(mlib_s8        *z,
 *                                                  const mlib_s16 *x,
 *                                                  mlib_s32       n);
 *      mlib_status mlib_VectorConvert_S8C_S32C_Mod(mlib_s8        *z,
 *                                                  const mlib_s32 *x,
 *                                                  mlib_s32       n);
 *      mlib_status mlib_VectorConvert_S8C_U8C_Mod(mlib_s8       *z,
 *                                                 const mlib_u8 *x,
 *                                                 mlib_s32      n);
 *      mlib_status mlib_VectorConvert_U8C_S16C_Mod(mlib_u8        *z,
 *                                                  const mlib_s16 *x,
 *                                                  mlib_s32       n);
 *      mlib_status mlib_VectorConvert_U8C_S32C_Mod(mlib_u8        *z,
 *                                                  const mlib_s32 *x,
 *                                                  mlib_s32       n);
 *      mlib_status mlib_VectorConvert_U8C_S8C_Mod(mlib_u8       *z,
 *                                                 const mlib_s8 *x,
 *                                                 mlib_s32      n);
 *
 * ARGUMENTS
 *      z    pointer to the first complex element of the result vector.
 *           z[2*i] hold the real parts, and z[2*i + 1] hold the imaginary
 *           parts
 *      x    pointer to the first complex element of the input vector.
 *           x[2*i] hold the real parts, and x[2*i + 1] hold the imaginary
 *           parts
 *      n    number of complex elements in the vectors
 */

#include <mlib_algebra.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorConvert_U8C_S16C_Mod = \
	__mlib_VectorConvert_U8C_S16C_Mod
#pragma weak mlib_VectorConvert_U8C_S8C_Mod = \
	__mlib_VectorConvert_U8C_S8C_Mod
#pragma weak mlib_VectorConvert_S8C_U8C_Mod = \
	__mlib_VectorConvert_S8C_U8C_Mod
#pragma weak mlib_VectorConvert_S16_S32_Mod = \
	__mlib_VectorConvert_S16_S32_Mod
#pragma weak mlib_VectorConvert_S8_S32_Mod = __mlib_VectorConvert_S8_S32_Mod
#pragma weak mlib_VectorConvert_S8_S16_Mod = __mlib_VectorConvert_S8_S16_Mod
#pragma weak mlib_VectorConvert_U8_S32_Mod = __mlib_VectorConvert_U8_S32_Mod
#pragma weak mlib_VectorConvert_U8_S16_Mod = __mlib_VectorConvert_U8_S16_Mod
#pragma weak mlib_VectorConvert_S16C_S32C_Mod = \
	__mlib_VectorConvert_S16C_S32C_Mod
#pragma weak mlib_VectorConvert_S8_U8_Mod = __mlib_VectorConvert_S8_U8_Mod
#pragma weak mlib_VectorConvert_U8_S8_Mod = __mlib_VectorConvert_U8_S8_Mod
#pragma weak mlib_VectorConvert_S8C_S32C_Mod = \
	__mlib_VectorConvert_S8C_S32C_Mod
#pragma weak mlib_VectorConvert_U8C_S32C_Mod = \
	__mlib_VectorConvert_U8C_S32C_Mod
#pragma weak mlib_VectorConvert_S8C_S16C_Mod = \
	__mlib_VectorConvert_S8C_S16C_Mod

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_VectorConvert_U8C_S16C_Mod) mlib_VectorConvert_U8C_S16C_Mod
	__attribute__((weak, alias("__mlib_VectorConvert_U8C_S16C_Mod")));
__typeof__(__mlib_VectorConvert_U8C_S8C_Mod) mlib_VectorConvert_U8C_S8C_Mod
	__attribute__((weak, alias("__mlib_VectorConvert_U8C_S8C_Mod")));
__typeof__(__mlib_VectorConvert_S8C_U8C_Mod) mlib_VectorConvert_S8C_U8C_Mod
	__attribute__((weak, alias("__mlib_VectorConvert_S8C_U8C_Mod")));
__typeof__(__mlib_VectorConvert_S16_S32_Mod) mlib_VectorConvert_S16_S32_Mod
	__attribute__((weak, alias("__mlib_VectorConvert_S16_S32_Mod")));
__typeof__(__mlib_VectorConvert_S8_S32_Mod) mlib_VectorConvert_S8_S32_Mod
	__attribute__((weak, alias("__mlib_VectorConvert_S8_S32_Mod")));
__typeof__(__mlib_VectorConvert_S8_S16_Mod) mlib_VectorConvert_S8_S16_Mod
	__attribute__((weak, alias("__mlib_VectorConvert_S8_S16_Mod")));
__typeof__(__mlib_VectorConvert_U8_S32_Mod) mlib_VectorConvert_U8_S32_Mod
	__attribute__((weak, alias("__mlib_VectorConvert_U8_S32_Mod")));
__typeof__(__mlib_VectorConvert_U8_S16_Mod) mlib_VectorConvert_U8_S16_Mod
	__attribute__((weak, alias("__mlib_VectorConvert_U8_S16_Mod")));
__typeof__(__mlib_VectorConvert_S16C_S32C_Mod) mlib_VectorConvert_S16C_S32C_Mod
	__attribute__((weak, alias("__mlib_VectorConvert_S16C_S32C_Mod")));
__typeof__(__mlib_VectorConvert_S8_U8_Mod) mlib_VectorConvert_S8_U8_Mod
	__attribute__((weak, alias("__mlib_VectorConvert_S8_U8_Mod")));
__typeof__(__mlib_VectorConvert_U8_S8_Mod) mlib_VectorConvert_U8_S8_Mod
	__attribute__((weak, alias("__mlib_VectorConvert_U8_S8_Mod")));
__typeof__(__mlib_VectorConvert_S8C_S32C_Mod) mlib_VectorConvert_S8C_S32C_Mod
	__attribute__((weak, alias("__mlib_VectorConvert_S8C_S32C_Mod")));
__typeof__(__mlib_VectorConvert_U8C_S32C_Mod) mlib_VectorConvert_U8C_S32C_Mod
	__attribute__((weak, alias("__mlib_VectorConvert_U8C_S32C_Mod")));
__typeof__(__mlib_VectorConvert_S8C_S16C_Mod) mlib_VectorConvert_S8C_S16C_Mod
	__attribute__((weak, alias("__mlib_VectorConvert_S8C_S16C_Mod")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/*
 * Simple defines for C implementation.
 * Assignment statement in loop for operands of different
 * formats.
 */

/* *********************************************************** */

#define	PACK(form_src, form_dst)                                \
	form_src *src = (void *)x;                              \
	form_dst *dst = z;                                      \
	mlib_s32 i;                                             \
	                                                        \
	for (i = 0; i < n; i++)                                 \
	    dst[i] = src[i];                                    \
	return (n > 0) ? MLIB_SUCCESS : MLIB_FAILURE

/*
 *
 * For 8->8 bits transformation we simply use mlib_Vector_Copy_U8_U8.
 *
 */

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S8_U8_Mod(
	mlib_s8 *z,
	const mlib_u8 *x,
	mlib_s32 n)
{
	return (__mlib_VectorCopy_U8((mlib_u8 *)z, x, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_U8_S8_Mod(
	mlib_u8 *z,
	const mlib_s8 *x,
	mlib_s32 n)
{
	return (__mlib_VectorCopy_U8(z, (mlib_u8 *)x, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_U8_S16_Mod(
	mlib_u8 *z,
	const mlib_s16 *x,
	mlib_s32 n)
{
	mlib_s16 *src = (void *)x;
	mlib_u8 *dst = z;
	mlib_d64 *dsrc, *ddst;
	mlib_d64 d1, d2, d3, d4, d5, d6, d7;
	mlib_s32 len_64, even_length, rest_64, length = n, i;

	if (n < 16) {
		PACK(mlib_s16, mlib_u8);
	}

/*
 * First try to align destination address for 8 bytes .
 */

	while ((mlib_addr)dst & 7) {
		(*dst++) = (*src++);
		length--;
	}

	rest_64 = length & 7;
	len_64 = length >> 3;
	even_length = len_64 << 3;
	ddst = (mlib_d64 *)dst;
	vis_write_gsr(7 << 3);
	vis_write_bmask(0x13579BDF, 0);

/*
 * Now analyze source address alignment.
 */

	if (((mlib_addr)src & 7) == 0) {

/*
 * Source address is also 8-byte aligned.
 */

		dsrc = (mlib_d64 *)src;

/*
 * Peeling the 1 - st iteration.
 */

		if (i = (len_64 & 1)) {
			d1 = (*dsrc++);
			d2 = (*dsrc++);
			d3 = vis_bshuffle(d1, d2);
			(*ddst++) = d3;
		}

/*
 * Then loop with step==2. Internal peeling for 2 iterations.
 */

#pragma pipeloop(0)
#pragma unroll(2)
		for (; i < len_64; i += 2) {
			d1 = (*dsrc++);
			d2 = (*dsrc++);
			d5 = (*dsrc++);
			d6 = (*dsrc++);
			d3 = vis_bshuffle(d1, d2);
			d7 = vis_bshuffle(d5, d6);
			(*ddst++) = d3;
			(*ddst++) = d7;
		}
	} else {

		dsrc = (mlib_d64 *)vis_alignaddr(src, 0);
		d2 = (*dsrc++);

/*
 * Peeling of 1 iteration.
 */

		if (i = (len_64 & 1)) {
			d1 = d2;
			d2 = vis_ld_d64_nf(dsrc); dsrc++;
			d3 = vis_faligndata(d1, d2);
			d1 = d2;
			d2 = vis_ld_d64_nf(dsrc); dsrc++;
			d4 = vis_faligndata(d1, d2);
			d3 = vis_bshuffle(d3, d4);
			(*ddst++) = d3;
		}

/*
 * Then loop with step==2.
 */

#pragma pipeloop(0)
#pragma unroll(4)
		for (; i < len_64; i += 2) {
			d1 = d2;
			d2 = (*dsrc++);
			d3 = vis_faligndata(d1, d2);
			d1 = d2;
			d2 = (*dsrc++);
			d4 = vis_faligndata(d1, d2);
			d1 = d2;
			d2 = (*dsrc++);
			d5 = vis_faligndata(d1, d2);
			d1 = d2;
			d2 = vis_ld_d64_nf(dsrc); dsrc++;
			d6 = vis_faligndata(d1, d2);
			d3 = vis_bshuffle(d3, d4);
			d5 = vis_bshuffle(d5, d6);
			(*ddst++) = d3;
			(*ddst++) = d5;
		}
	}

	for (i = 0; i < rest_64; i++)
		dst[even_length + i] = src[even_length + i];

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S8_S16_Mod(
	mlib_s8 *z,
	const mlib_s16 *x,
	mlib_s32 n)
{
	return (__mlib_VectorConvert_U8_S16_Mod((mlib_u8 *)z, x, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_U8_S32_Mod(
	mlib_u8 *z,
	const mlib_s32 *x,
	mlib_s32 n)
{
	mlib_s32 *src = (void *)x;
	mlib_u8 *dst = z;
	mlib_d64 *dsrc, *ddst;
	mlib_d64 dd0, dd1, d0, d1, d2, d3, d4, d5, d6, d7;
	mlib_s32 len_64, even_length, rest_64, length = n, i, off;

	if (n < 16) {
		PACK(mlib_s32, mlib_u8);
	}

/*
 * First try to align destination address for 8 bytes .
 */

	while ((mlib_addr)dst & 7) {
		(*dst++) = (*src++);
		length--;
	}

	rest_64 = length & 7;
	len_64 = length >> 3;
	even_length = len_64 << 3;
	ddst = (mlib_d64 *)dst;
	off = (mlib_addr)src & 7;

/*
 * Now analyze source address alignment.
 */

	if (off == 0) {
		vis_alignaddr((void *)0, 4);
		vis_write_bmask(0x000037BF, 0);
		dsrc = (mlib_d64 *)src;

		if (i = (len_64 & 1)) {
			d1 = (*dsrc++);
			d2 = (*dsrc++);
			d3 = (*dsrc++);
			d4 = (*dsrc++);
			d1 = vis_bshuffle(d1, d2);
			d3 = vis_bshuffle(d3, d4);
			d3 = vis_faligndata(d3, d3);
			d3 = vis_freg_pair(vis_read_lo(d1), vis_read_hi(d3));
			(*ddst++) = d3;
		}
#pragma pipeloop(0)
#pragma unroll(2)
		for (; i < len_64; i += 2) {
			d0 = (*dsrc++);
			d1 = (*dsrc++);
			d2 = (*dsrc++);
			d3 = (*dsrc++);
			d4 = (*dsrc++);
			d5 = (*dsrc++);
			d6 = (*dsrc++);
			d7 = (*dsrc++);
			d0 = vis_bshuffle(d0, d1);
			d2 = vis_bshuffle(d2, d3);
			d4 = vis_bshuffle(d4, d5);
			d6 = vis_bshuffle(d6, d7);
			d2 = vis_faligndata(d2, d2);
			d6 = vis_faligndata(d6, d6);
			d2 = vis_freg_pair(vis_read_lo(d0), vis_read_hi(d2));
			d6 = vis_freg_pair(vis_read_lo(d4), vis_read_hi(d6));
			(*ddst++) = d2;
			(*ddst++) = d6;
		}
	} else {
		vis_write_bmask(0x11111111 * off, 0x37000000);
		dsrc = (mlib_d64 *)vis_alignaddr(src, 0);
		dd1 = (*dsrc++);

		if (i = (len_64 & 1)) {
			dd0 = dd1;
			dd1 = (*dsrc++);
			d0 = vis_bshuffle(dd0, dd1);
			dd0 = dd1;
			dd1 = (*dsrc++);
			d1 = vis_bshuffle(dd0, dd1);
			dd0 = dd1;
			dd1 = (*dsrc++);
			d2 = vis_bshuffle(dd0, dd1);
			dd0 = dd1;
			dd1 = vis_ld_d64_nf(dsrc); dsrc++;
			d3 = vis_bshuffle(dd0, dd1);
			d0 = vis_fpmerge(vis_read_hi(d0), vis_read_hi(d2));
			d1 = vis_fpmerge(vis_read_hi(d1), vis_read_hi(d3));
			d2 = vis_fpmerge(vis_read_hi(d0), vis_read_hi(d1));
			d3 = vis_fpmerge(vis_read_hi(d2), vis_read_lo(d2));
			(*ddst++) = d3;
		}
#pragma pipeloop(0)
#pragma unroll(2)
		for (; i < len_64; i += 2) {
			dd0 = dd1;
			dd1 = (*dsrc++);
			d0 = vis_bshuffle(dd0, dd1);
			dd0 = dd1;
			dd1 = (*dsrc++);
			d1 = vis_bshuffle(dd0, dd1);
			dd0 = dd1;
			dd1 = (*dsrc++);
			d2 = vis_bshuffle(dd0, dd1);
			dd0 = dd1;
			dd1 = (*dsrc++);
			d3 = vis_bshuffle(dd0, dd1);

			dd0 = dd1;
			dd1 = (*dsrc++);
			d4 = vis_bshuffle(dd0, dd1);
			dd0 = dd1;
			dd1 = (*dsrc++);
			d5 = vis_bshuffle(dd0, dd1);
			dd0 = dd1;
			dd1 = (*dsrc++);
			d6 = vis_bshuffle(dd0, dd1);
			dd0 = dd1;
			dd1 = vis_ld_d64_nf(dsrc); dsrc++;
			d7 = vis_bshuffle(dd0, dd1);

			d0 = vis_fpmerge(vis_read_hi(d0), vis_read_hi(d2));
			d1 = vis_fpmerge(vis_read_hi(d1), vis_read_hi(d3));
			d2 = vis_fpmerge(vis_read_hi(d0), vis_read_hi(d1));
			d3 = vis_fpmerge(vis_read_hi(d2), vis_read_lo(d2));

			d4 = vis_fpmerge(vis_read_hi(d4), vis_read_hi(d6));
			d5 = vis_fpmerge(vis_read_hi(d5), vis_read_hi(d7));
			d6 = vis_fpmerge(vis_read_hi(d4), vis_read_hi(d5));
			d7 = vis_fpmerge(vis_read_hi(d6), vis_read_lo(d6));

			(*ddst++) = d3;
			(*ddst++) = d7;
		}
	}

	for (i = 0; i < rest_64; i++)
		dst[even_length + i] = src[even_length + i];

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S8_S32_Mod(
	mlib_s8 *z,
	const mlib_s32 *x,
	mlib_s32 n)
{
	return (__mlib_VectorConvert_U8_S32_Mod((mlib_u8 *)z, x, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S16_S32_Mod(
	mlib_s16 *z,
	const mlib_s32 *x,
	mlib_s32 n)
{
	mlib_s32 *src = (void *)x;
	mlib_s16 *dst = z;
	mlib_d64 *dsrc, *ddst;
	mlib_d64 dd0, dd1, dd2, dd3, dd4, dd5;
	mlib_s32 len_64, even_length, rest_64, length = n, i;

	if (n < 16) {
		PACK(mlib_s32, mlib_s16);
	}

/*
 * First try to align destination address for 8 bytes .
 */

	while ((mlib_addr)dst & 7) {
		(*dst++) = (*src++);
		length--;
	}

	rest_64 = length & 3;
	len_64 = length >> 2;
	even_length = len_64 << 2;
	ddst = (mlib_d64 *)dst;
	vis_write_bmask(0x2367ABEF, 0);

	if ((mlib_addr)src & 7 == 0) {
		dsrc = (mlib_d64 *)src;

		if (i = (len_64 & 1)) {
			dd1 = (*dsrc++);
			dd2 = (*dsrc++);
			dd3 = vis_bshuffle(dd1, dd2);
			(*ddst++) = dd3;
		}
#pragma pipeloop(0)
#pragma unroll(2)
		for (; i < len_64; i += 2) {
			dd0 = (*dsrc++);
			dd1 = (*dsrc++);
			dd2 = (*dsrc++);
			dd3 = (*dsrc++);
			dd4 = vis_bshuffle(dd0, dd1);
			dd5 = vis_bshuffle(dd2, dd3);
			(*ddst++) = dd4;
			(*ddst++) = dd5;
		}
	} else {
		dsrc = (mlib_d64 *)vis_alignaddr(src, 0);
		dd1 = (*dsrc++);

		if (i = (len_64 & 1)) {
			dd0 = dd1;
			dd1 = vis_ld_d64_nf(dsrc); dsrc++;
			dd2 = vis_faligndata(dd0, dd1);
			dd0 = dd1;
			dd1 = vis_ld_d64_nf(dsrc); dsrc++;
			dd3 = vis_faligndata(dd0, dd1);
			dd3 = vis_bshuffle(dd2, dd3);
			(*ddst++) = dd3;
		}
#pragma pipeloop(0)
#pragma unroll(2)
		for (; i < len_64; i += 2) {
			dd0 = dd1;
			dd1 = (*dsrc++);
			dd2 = vis_faligndata(dd0, dd1);
			dd0 = dd1;
			dd1 = (*dsrc++);
			dd3 = vis_faligndata(dd0, dd1);
			dd0 = dd1;
			dd1 = (*dsrc++);
			dd4 = vis_faligndata(dd0, dd1);
			dd0 = dd1;
			dd1 = vis_ld_d64_nf(dsrc); dsrc++;
			dd5 = vis_faligndata(dd0, dd1);
			dd2 = vis_bshuffle(dd2, dd3);
			dd4 = vis_bshuffle(dd4, dd5);
			(*ddst++) = dd2;
			(*ddst++) = dd4;
		}
	}

	for (i = 0; i < rest_64; i++)
		dst[even_length + i] = src[even_length + i];

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S8C_U8C_Mod(
	mlib_s8 *z,
	const mlib_u8 *x,
	mlib_s32 n)
{
	return (__mlib_VectorCopy_U8((mlib_u8 *)z, x, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_U8C_S8C_Mod(
	mlib_u8 *z,
	const mlib_s8 *x,
	mlib_s32 n)
{
	return (__mlib_VectorCopy_U8(z, (mlib_u8 *)x, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_U8C_S16C_Mod(
	mlib_u8 *z,
	const mlib_s16 *x,
	mlib_s32 n)
{
	return (__mlib_VectorConvert_U8_S16_Mod(z, x, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S8C_S16C_Mod(
	mlib_s8 *z,
	const mlib_s16 *x,
	mlib_s32 n)
{
	return (__mlib_VectorConvert_U8_S16_Mod((mlib_u8 *)z, x, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_U8C_S32C_Mod(
	mlib_u8 *z,
	const mlib_s32 *x,
	mlib_s32 n)
{
	return (__mlib_VectorConvert_U8_S32_Mod(z, x, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S8C_S32C_Mod(
	mlib_s8 *z,
	const mlib_s32 *x,
	mlib_s32 n)
{
	return (__mlib_VectorConvert_U8_S32_Mod((mlib_u8 *)z, x, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S16C_S32C_Mod(
	mlib_s16 *z,
	const mlib_s32 *x,
	mlib_s32 n)
{
	return (__mlib_VectorConvert_S16_S32_Mod(z, x, n + n));
}

/* *********************************************************** */
