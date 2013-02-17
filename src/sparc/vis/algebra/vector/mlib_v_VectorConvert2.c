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
 *
 * DESCRIPTION
 *      z = x
 *
 * IMPLEMENTATION
 *
 *      For 8->8 bits transformation we simply use mlib_Vector_Copy_U8_U8.
 *
 *      For 16->8 bits packing we apply  VIS instruction vis_fpack16 to 2*64-
 *      bits registers and obtain 1*8 bytes of a result.
 *
 *      For 32->8 bits packing we use VIS instructions for composing of the
 *      resulting mlib_d64 word (8 elements). We read source vector
 *      8 times by 1 byte and then form a result by fis_to_double(op1, op2),
 *      where op1 and op2 are obtained by logical multiplication and shift.
 *
 *      For 32->16 bits packing we don't use VIS at all.
 *      The most quickly way is to read source vector 4 times
 *      by 1/2 word and to obtain 2*32-bit words by logical operations.
 *
 *      In all cases we've made unroll of loops for 2 iterations.
 *
 *      All functions of Complex arguments were obtained from the corresponding
 *      functions of real arguments.
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

/* *********************************************************** */

/*
 * Simple defines for C implementation.
 * Assignment statement in loop for operands of different
 * formats.
 */

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

/*
 *
 * For 16->8 bits packing we apply  VIS instruction vis_fpack16 to 2*64-
 * bits registers and obtain 1*8 bytes of a result.
 *
 */

mlib_status
__mlib_VectorConvert_U8_S16_Mod(
	mlib_u8 *z,
	const mlib_s16 *x,
	mlib_s32 n)
{
	mlib_s16 *src = (void *)x;
	mlib_u8 *dst = z;
	mlib_s32 *isrc;
	mlib_d64 *dsrc, *ddst;
	mlib_d64 dlog1 = vis_to_double_dup(0x00ff00ff);
	mlib_d64 d1, d2, d3, d4, d5, d6, d7;
	mlib_s32 len_64, even_length, rest_64, length = n, i;

	if (n < 16) {
		PACK(mlib_s16,
			mlib_u8);
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
			d4 = vis_fand(d1, dlog1);
			d2 = (*dsrc++);
			d5 = vis_fand(d2, dlog1);
			d3 = vis_freg_pair(vis_fpack16(d4), vis_fpack16(d5));
			(*ddst++) = d3;
		}

/*
 * Then loop with step==2. Internal peeling for 2 iterations.
 */

#pragma pipeloop(0)
		for (; i < len_64; i += 2) {
			d1 = (*dsrc++);
			d1 = vis_fand(d1, dlog1);
			d2 = (*dsrc++);
			d2 = vis_fand(d2, dlog1);
			d5 = (*dsrc++);
			d5 = vis_fand(d5, dlog1);
			d6 = (*dsrc++);
			d6 = vis_fand(d6, dlog1);
			d3 = vis_freg_pair(vis_fpack16(d1), vis_fpack16(d2));
			d7 = vis_freg_pair(vis_fpack16(d5), vis_fpack16(d6));
			(*ddst++) = d3;
			(*ddst++) = d7;
		}
	} else if (((mlib_s32)src & 3) == 0) {

/*
 * Source address is 4-byte aligned.
 */

		isrc = (mlib_s32 *)src;

#pragma pipeloop(0)
		for (i = 0; i < len_64; i++) {
			d1 = vis_to_double(((*isrc++)), ((*isrc++)));
			d1 = vis_fand(d1, dlog1);
			d2 = vis_to_double(((*isrc++)), ((*isrc++)));
			d2 = vis_fand(d2, dlog1);
			d3 = vis_freg_pair(vis_fpack16(d1), vis_fpack16(d2));
			(*ddst++) = d3;
		}
	} else {

/*
 * Source address is 2-byte aligned. Use vis_alignaddr() and
 * vis_faligndata() functions.
 */

		dsrc = (mlib_d64 *)vis_alignaddr(src, 0);
		d2 = (*dsrc++);

/*
 * Peeling of 1 iteration.
 */

		if (i = (len_64 & 1)) {
			d1 = d2;
			d2 = (*dsrc++);
			d3 = vis_faligndata(d1, d2);
			d1 = d2;
			d2 = (*dsrc++);
			d4 = vis_faligndata(d1, d2);
			d3 = vis_fand(d3, dlog1);
			d4 = vis_fand(d4, dlog1);
			d3 = vis_freg_pair(vis_fpack16(d3), vis_fpack16(d4));
			(*ddst++) = d3;
		}

/*
 * Then loop with step==2.
 */

#pragma pipeloop(0)
		for (; i < len_64; i += 2) {
			d1 = d2;
			d2 = (*dsrc++);
			d3 = vis_faligndata(d1, d2);
			d3 = vis_fand(d3, dlog1);
			d1 = d2;
			d2 = (*dsrc++);
			d4 = vis_faligndata(d1, d2);
			d4 = vis_fand(d4, dlog1);
			d1 = d2;
			d2 = (*dsrc++);
			d5 = vis_faligndata(d1, d2);
			d5 = vis_fand(d5, dlog1);
			d1 = d2;
			d2 = (*dsrc++);
			d6 = vis_faligndata(d1, d2);
			d6 = vis_fand(d6, dlog1);
			d3 = vis_freg_pair(vis_fpack16(d3), vis_fpack16(d4));
			d5 = vis_freg_pair(vis_fpack16(d5), vis_fpack16(d6));
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

/*
 *
 * For 32->8 bits packing we use VIS instructions for composing of the
 * resulting mlib_d64 word (8 elements). We read source vector 8 times by 1 byte
 * and then form a result by fis_to_double(op1, op2), where op1 and op2
 * are obtained by logical multiplication and shift.
 *
 */

mlib_status
__mlib_VectorConvert_U8_S32_Mod(
	mlib_u8 *z,
	const mlib_s32 *x,
	mlib_s32 n)
{
	mlib_s32 *src = (void *)x;
	mlib_u8 *dst = z, *u8src;
	mlib_s32 *dst_32;
	mlib_s32 i1, i2, i3, i4, i5, i6, i7, i8;
	mlib_s32 len_64, even_length, rest_64, length = n, i;

	if (n < 16) {
		PACK(mlib_s32,
			mlib_u8);
	}

/*
 * First try to align for 4 bytes destination address.
 */

	while ((mlib_addr)dst & 3) {
		(*dst++) = (*src++);
		length--;
	}

	rest_64 = length & 7;
	len_64 = length >> 3;
	even_length = len_64 << 3;
	dst_32 = (mlib_s32 *)dst;

	u8src = (mlib_u8 *)src;
	u8src += 3;

	if (i = (len_64 & 1)) {
		i1 = *(u8src);
		i2 = *(u8src + 4);
		i3 = *(u8src + 8);
		i4 = *(u8src + 12);
		i5 = *(u8src + 16);
		i6 = *(u8src + 20);
		i7 = *(u8src + 24);
		i8 = *(u8src + 28);
		(*dst_32++) = ((i1 << 24) | (i2 << 16) | (i3 << 8) | (i4));
		(*dst_32++) = ((i5 << 24) | (i6 << 16) | (i7 << 8) | (i8));
		u8src += 32;
	}
#pragma pipeloop(0)
	for (; i < len_64; i += 2) {
		i1 = *(u8src);
		i2 = *(u8src + 4);
		i3 = *(u8src + 8);
		i4 = *(u8src + 12);
		i5 = *(u8src + 16);
		i6 = *(u8src + 20);
		i7 = *(u8src + 24);
		i8 = *(u8src + 28);
		(*dst_32++) = ((i1 << 24) | (i2 << 16) | (i3 << 8) | (i4));
		(*dst_32++) = ((i5 << 24) | (i6 << 16) | (i7 << 8) | (i8));
		u8src += 32;
		i1 = *(u8src);
		i2 = *(u8src + 4);
		i3 = *(u8src + 8);
		i4 = *(u8src + 12);
		i5 = *(u8src + 16);
		i6 = *(u8src + 20);
		i7 = *(u8src + 24);
		i8 = *(u8src + 28);
		(*dst_32++) = ((i1 << 24) | (i2 << 16) | (i3 << 8) | (i4));
		(*dst_32++) = ((i5 << 24) | (i6 << 16) | (i7 << 8) | (i8));
		u8src += 32;
	}

	for (i = 0; i < rest_64; i++)
		dst[even_length + i] = *(u8src + (i << 2));

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

/*
 *
 * For 32->16 bits packing we don't use VIS at all. The most quickly way is
 * to read source vector 4 times by 1/2 word and to obtain 2*32-bit words by
 * logical operations.
 *
 */

mlib_status
__mlib_VectorConvert_S16_S32_Mod(
	mlib_s16 *z,
	const mlib_s32 *x,
	mlib_s32 n)
{
	mlib_s32 *src = (void *)x, *idst;
	mlib_s16 *dst = z;
	mlib_u16 *u16src;
	mlib_s32 i1, i2, i3, i4, i5, i6, i7, i8;
	mlib_s32 len_64, even_length, rest_64, length = n, i;

	if (n < 16) {
		PACK(mlib_s32,
			mlib_s16);
	}

/*
 * First try to align destination address for 4 bytes .
 */

	while ((mlib_addr)dst & 3) {
		(*dst++) = (*src++);
		length--;
	}

	rest_64 = length & 3;
	len_64 = length >> 2;
	even_length = len_64 << 2;
	u16src = (mlib_u16 *)src;
	u16src++;

	idst = (mlib_s32 *)dst;

	if (i = (len_64 & 1)) {
		i1 = *(u16src);
		i2 = *(u16src + 2);
		i3 = *(u16src + 4);
		i4 = *(u16src + 6);
		(*idst++) = ((i1 << 16) | (i2));
		(*idst++) = ((i3 << 16) | (i4));
		u16src += 8;
	}
#pragma pipeloop(0)
	for (; i < len_64; i += 2) {
		i1 = *(u16src);
		i2 = *(u16src + 2);
		i3 = *(u16src + 4);
		i4 = *(u16src + 6);
		i5 = *(u16src + 8);
		i6 = *(u16src + 10);
		i7 = *(u16src + 12);
		i8 = *(u16src + 14);
		(*idst++) = ((i1 << 16) | (i2));
		(*idst++) = ((i3 << 16) | (i4));
		(*idst++) = ((i5 << 16) | (i6));
		(*idst++) = ((i7 << 16) | (i8));
		u16src += 16;
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
