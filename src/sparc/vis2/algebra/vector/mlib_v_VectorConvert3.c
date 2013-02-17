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

#pragma ident	"@(#)mlib_v_VectorConvert3.c	9.4	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorConvert_S8_U8_Sat     - packing of 8-bit format vector
 *      mlib_VectorConvert_U8_S8_Sat     - packing of 8-bit format vector
 *      mlib_VectorConvert_U8_S16_Sat    - packing of 16-bit into 8-bit
 *                                         format vector
 *      mlib_VectorConvert_S8_S16_Sat    - packing of 16-bit into 8-bit
 *                                         format vector
 *      mlib_VectorConvert_U8_S32_Sat    - packing of 32-bit into 8-bit
 *                                         format vector
 *      mlib_VectorConvert_S8_S32_Sat    - packing of 32-bit into 8-bit
 *                                         format vector
 *      mlib_VectorConvert_S16_S32_Sat   - packing of 32-bit into 16-bit
 *                                         format vector
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorConvert_S16_S32_Sat(mlib_s16       *z,
 *                                                 const mlib_s32 *x,
 *                                                 mlib_s32       n);
 *      mlib_status mlib_VectorConvert_S8_S16_Sat(mlib_s8        *z,
 *                                                const mlib_s16 *x,
 *                                                mlib_s32       n);
 *      mlib_status mlib_VectorConvert_S8_S32_Sat(mlib_s8        *z,
 *                                                const mlib_s32 *x,
 *                                                mlib_s32       n);
 *      mlib_status mlib_VectorConvert_S8_U8_Sat(mlib_s8       *z,
 *                                               const mlib_u8 *x,
 *                                               mlib_s32      n);
 *      mlib_status mlib_VectorConvert_U8_S16_Sat(mlib_u8        *z,
 *                                                const mlib_s16 *x,
 *                                                mlib_s32       n);
 *      mlib_status mlib_VectorConvert_U8_S32_Sat(mlib_u8        *z,
 *                                                const mlib_s32 *x,
 *                                                mlib_s32       n);
 *      mlib_status mlib_VectorConvert_U8_S8_Sat(mlib_u8       *z,
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
 *      mlib_VectorConvert_S8C_U8C_Sat     - packing of 8-bit format vector
 *      mlib_VectorConvert_U8C_S8C_Sat     - packing of 8-bit format vector
 *      mlib_VectorConvert_U8C_S16C_Sat    - packing of 16-bit into 8-bit
 *                                           format vector
 *      mlib_VectorConvert_S8C_S16C_Sat    - packing of 16-bit into 8-bit
 *                                           format vector
 *      mlib_VectorConvert_U8C_S32C_Sat    - packing of 32-bit into 8-bit
 *                                           format vector
 *      mlib_VectorConvert_S8C_S32C_Sat    - packing of 32-bit into 8-bit
 *                                           format vector
 *      mlib_VectorConvert_S16C_S32C_Sat   - packing of 32-bit into 16-bit
 *                                           format vector
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorConvert_S16C_S32C_Sat(mlib_s16       *z,
 *                                                   const mlib_s32 *x,
 *                                                   mlib_s32       n);
 *      mlib_status mlib_VectorConvert_S8C_S16C_Sat(mlib_s8        *z,
 *                                                  const mlib_s16 *x,
 *                                                  mlib_s32       n);
 *      mlib_status mlib_VectorConvert_S8C_S32C_Sat(mlib_s8        *z,
 *                                                  const mlib_s32 *x,
 *                                                  mlib_s32       n);
 *      mlib_status mlib_VectorConvert_S8C_U8C_Sat(mlib_s8       *z,
 *                                                 const mlib_u8 *x,
 *                                                 mlib_s32      n);
 *      mlib_status mlib_VectorConvert_U8C_S16C_Sat(mlib_u8        *z,
 *                                                  const mlib_s16 *x,
 *                                                  mlib_s32       n);
 *      mlib_status mlib_VectorConvert_U8C_S32C_Sat(mlib_u8        *z,
 *                                                  const mlib_s32 *x,
 *                                                  mlib_s32       n);
 *      mlib_status mlib_VectorConvert_U8C_S8C_Sat(mlib_u8       *z,
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
 */

#include <mlib_algebra.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorConvert_U8_S32_Sat = __mlib_VectorConvert_U8_S32_Sat
#pragma weak mlib_VectorConvert_S8_S16_Sat = __mlib_VectorConvert_S8_S16_Sat
#pragma weak mlib_VectorConvert_U8_S16_Sat = __mlib_VectorConvert_U8_S16_Sat
#pragma weak mlib_VectorConvert_S16C_S32C_Sat = \
	__mlib_VectorConvert_S16C_S32C_Sat
#pragma weak mlib_VectorConvert_U8_S8_Sat = __mlib_VectorConvert_U8_S8_Sat
#pragma weak mlib_VectorConvert_S8_U8_Sat = __mlib_VectorConvert_S8_U8_Sat
#pragma weak mlib_VectorConvert_S8C_S32C_Sat = \
	__mlib_VectorConvert_S8C_S32C_Sat
#pragma weak mlib_VectorConvert_U8C_S32C_Sat = \
	__mlib_VectorConvert_U8C_S32C_Sat
#pragma weak mlib_VectorConvert_S8C_S16C_Sat = \
	__mlib_VectorConvert_S8C_S16C_Sat
#pragma weak mlib_VectorConvert_U8C_S16C_Sat = \
	__mlib_VectorConvert_U8C_S16C_Sat
#pragma weak mlib_VectorConvert_U8C_S8C_Sat = \
	__mlib_VectorConvert_U8C_S8C_Sat
#pragma weak mlib_VectorConvert_S8C_U8C_Sat = \
	__mlib_VectorConvert_S8C_U8C_Sat
#pragma weak mlib_VectorConvert_S16_S32_Sat = \
	__mlib_VectorConvert_S16_S32_Sat
#pragma weak mlib_VectorConvert_S8_S32_Sat = __mlib_VectorConvert_S8_S32_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_VectorConvert_U8_S32_Sat) mlib_VectorConvert_U8_S32_Sat
	__attribute__((weak, alias("__mlib_VectorConvert_U8_S32_Sat")));
__typeof__(__mlib_VectorConvert_S8_S16_Sat) mlib_VectorConvert_S8_S16_Sat
	__attribute__((weak, alias("__mlib_VectorConvert_S8_S16_Sat")));
__typeof__(__mlib_VectorConvert_U8_S16_Sat) mlib_VectorConvert_U8_S16_Sat
	__attribute__((weak, alias("__mlib_VectorConvert_U8_S16_Sat")));
__typeof__(__mlib_VectorConvert_S16C_S32C_Sat) mlib_VectorConvert_S16C_S32C_Sat
	__attribute__((weak, alias("__mlib_VectorConvert_S16C_S32C_Sat")));
__typeof__(__mlib_VectorConvert_U8_S8_Sat) mlib_VectorConvert_U8_S8_Sat
	__attribute__((weak, alias("__mlib_VectorConvert_U8_S8_Sat")));
__typeof__(__mlib_VectorConvert_S8_U8_Sat) mlib_VectorConvert_S8_U8_Sat
	__attribute__((weak, alias("__mlib_VectorConvert_S8_U8_Sat")));
__typeof__(__mlib_VectorConvert_S8C_S32C_Sat) mlib_VectorConvert_S8C_S32C_Sat
	__attribute__((weak, alias("__mlib_VectorConvert_S8C_S32C_Sat")));
__typeof__(__mlib_VectorConvert_U8C_S32C_Sat) mlib_VectorConvert_U8C_S32C_Sat
	__attribute__((weak, alias("__mlib_VectorConvert_U8C_S32C_Sat")));
__typeof__(__mlib_VectorConvert_S8C_S16C_Sat) mlib_VectorConvert_S8C_S16C_Sat
	__attribute__((weak, alias("__mlib_VectorConvert_S8C_S16C_Sat")));
__typeof__(__mlib_VectorConvert_U8C_S16C_Sat) mlib_VectorConvert_U8C_S16C_Sat
	__attribute__((weak, alias("__mlib_VectorConvert_U8C_S16C_Sat")));
__typeof__(__mlib_VectorConvert_U8C_S8C_Sat) mlib_VectorConvert_U8C_S8C_Sat
	__attribute__((weak, alias("__mlib_VectorConvert_U8C_S8C_Sat")));
__typeof__(__mlib_VectorConvert_S8C_U8C_Sat) mlib_VectorConvert_S8C_U8C_Sat
	__attribute__((weak, alias("__mlib_VectorConvert_S8C_U8C_Sat")));
__typeof__(__mlib_VectorConvert_S16_S32_Sat) mlib_VectorConvert_S16_S32_Sat
	__attribute__((weak, alias("__mlib_VectorConvert_S16_S32_Sat")));
__typeof__(__mlib_VectorConvert_S8_S32_Sat) mlib_VectorConvert_S8_S32_Sat
	__attribute__((weak, alias("__mlib_VectorConvert_S8_S32_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

/* Copy with control for operands of the same format */

#define	PACK_S_U(form_src, form_dst)                            \
	form_src *src = (void *)x;                              \
	form_dst *dst = z;                                      \
	mlib_s32 i;                                             \
	form_src c;                                             \
	                                                        \
	for (i = 0; i < n; i++) {                               \
	    dst[i] = (c = src[i]) < 0 ? 0 : c;                  \
	}                                                       \
	                                                        \
	return (n > 0) ? MLIB_SUCCESS : MLIB_FAILURE

/* *********************************************************** */

/* Copy with control for operands of the same format */

#define	PACK_U_S(form_src, form_dst, max_dst)                   \
	form_src *src = (void *)x;                              \
	form_dst *dst = z;                                      \
	form_src c;                                             \
	mlib_s32 i;                                             \
	                                                        \
	for (i = 0; i < n; i++) {                               \
	    dst[i] = (c = src[i]) > max_dst ? max_dst : c;      \
	}                                                       \
	                                                        \
	return (n > 0) ? MLIB_SUCCESS : MLIB_FAILURE

/* *********************************************************** */

/* Copy with control for operands of different format */

#define	PACK_S_U_DF(form_src, form_dst, max_dst, min_dst)       \
	form_src *src = (void *)x;                              \
	form_dst *dst = z;                                      \
	mlib_s32 i;                                             \
	form_src c;                                             \
	                                                        \
	for (i = 0; i < n; i++) {                               \
	    dst[i] = (c =                                       \
		    src[i]) < min_dst ? min_dst : (c >          \
		    max_dst ? max_dst : c);                     \
	}                                                       \
	                                                        \
	return (n > 0) ? MLIB_SUCCESS : MLIB_FAILURE

/* *********************************************************** */

/* Copy with control for operands of the same format */

#define	PACK_S_S(form_src, form_dst, max_dst, min_dst)          \
	form_src *src = (void *)x;                              \
	form_dst *dst = z;                                      \
	mlib_s32 i;                                             \
	form_src c;                                             \
	                                                        \
	for (i = 0; i < n; i++) {                               \
	    dst[i] = (c =                                       \
		    src[i]) < min_dst ? min_dst : (c >          \
		    max_dst ? max_dst : c);                     \
	}                                                       \
	                                                        \
	return (n > 0) ? MLIB_SUCCESS : MLIB_FAILURE

/* *********************************************************** */

/*
 * U8 -> U16(S16) -> S8
 */

mlib_status
__mlib_VectorConvert_S8_U8_Sat(
	mlib_s8 *z,
	const mlib_u8 *x,
	mlib_s32 n)
{
	mlib_u8 *src = (void *)x;
	mlib_s8 *dst = z;
	mlib_d64 fzero = vis_fzeros();
	mlib_d64 *dsrc, *ddst;
	mlib_d64 d1, d2, d3, d4, d5, d6;
	mlib_s32 len_64, even_length, rest_64, length = n, i;
	mlib_u8 c;
	mlib_d64 dsp = vis_to_double_dup(0x800080);
	mlib_d64 rst = vis_to_double_dup(0x80808080);
	mlib_f32 fm = vis_to_float(0x100);

	if (length < 16) {
		PACK_U_S(mlib_u8, mlib_s8, MLIB_S8_MAX);
	}

/*
 * First, try to align destination address for 8 bytes .
 */

	while ((mlib_addr)dst & 7) {
		(*dst++) = (c = (*src++)) > MLIB_S8_MAX ? MLIB_S8_MAX : c;
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
 * Peeling the 1st iteration.
 */

		if (i = (len_64 & 1)) {
			d1 = (*dsrc++);
			d2 = vis_fpmerge(fzero, vis_read_hi(d1));
			d3 = vis_fmul8x16al(vis_read_lo(d1), fm);
			d2 = vis_fpadd16(dsp, d2);
			d3 = vis_fpadd16(dsp, d3);
			d1 = vis_fpack16_pair(d2, d3);
			(*ddst++) = vis_fxor(d1, rst);
		}

/*
 * Then loop with step==2. Unroll for 2 iterations.
 */

#pragma pipeloop(0)
#pragma unroll(4)
		for (; i < len_64; i += 2) {
			d1 = (*dsrc++);
			d4 = (*dsrc++);
			d2 = vis_fpmerge(fzero, vis_read_hi(d1));
			d3 = vis_fmul8x16al(vis_read_lo(d1), fm);
			d2 = vis_fpadd16(dsp, d2);
			d3 = vis_fpadd16(dsp, d3);
			d1 = vis_fpack16_pair(d2, d3);
			d2 = vis_fpmerge(fzero, vis_read_hi(d4));
			d3 = vis_fmul8x16al(vis_read_lo(d4), fm);
			d2 = vis_fpadd16(dsp, d2);
			d3 = vis_fpadd16(dsp, d3);
			d4 = vis_fpack16_pair(d2, d3);
			(*ddst++) = vis_fxor(d1, rst);
			(*ddst++) = vis_fxor(d4, rst);
		}
	} else {

/*
 * Source address has arbitrary alignment. Use vis_alignaddr() and
 * vis_faligndata() functions.
 */

		dsrc = (mlib_d64 *)vis_alignaddr(src, 0);
		d2 = (*dsrc++);

/*
 * Peeling of 1 iteration.
 */

		if (i = (len_64 & 1)) {
			d1 = d2;
			d2 = vis_ld_d64_nf(dsrc); dsrc++;
			d1 = vis_faligndata(d1, d2);
			d3 = vis_fmul8x16al(vis_read_hi(d1), fm);
			d4 = vis_fmul8x16al(vis_read_lo(d1), fm);
			d3 = vis_fpadd16(dsp, d3);
			d4 = vis_fpadd16(dsp, d4);
			d1 = vis_fpack16_pair(d3, d4);
			(*ddst++) = vis_fxor(d1, rst);
		}

/*
 * Then loop with step==2.
 */

#pragma pipeloop(0)
#pragma unroll(2)
		for (; i < len_64; i += 2) {
			d1 = d2;
			d2 = vis_ld_d64_nf(dsrc); dsrc++;
			d3 = vis_faligndata(d1, d2);
			d1 = d2;
			d2 = vis_ld_d64_nf(dsrc); dsrc++;
			d6 = vis_faligndata(d1, d2);
			d4 = vis_fmul8x16al(vis_read_hi(d3), fm);
			d5 = vis_fmul8x16al(vis_read_lo(d3), fm);
			d4 = vis_fpadd16(dsp, d4);
			d5 = vis_fpadd16(dsp, d5);
			d3 = vis_fpack16_pair(d4, d5);
			d4 = vis_fmul8x16al(vis_read_hi(d6), fm);
			d5 = vis_fmul8x16al(vis_read_lo(d6), fm);
			d4 = vis_fpadd16(dsp, d4);
			d5 = vis_fpadd16(dsp, d5);
			d6 = vis_fpack16_pair(d4, d5);
			(*ddst++) = vis_fxor(d3, rst);
			(*ddst++) = vis_fxor(d6, rst);
		}
	}

	for (i = 0; i < rest_64; i++)
		dst[even_length + i] = (c =
			src[even_length + i]) > MLIB_S8_MAX ? MLIB_S8_MAX : c;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

/*
 * S8 -> S16 -> U8
 */

mlib_status
__mlib_VectorConvert_U8_S8_Sat(
	mlib_u8 *z,
	const mlib_s8 *x,
	mlib_s32 n)
{
	mlib_s8 *src = (void *)x;
	mlib_u8 *dst = z;
	mlib_d64 *dsrc, *ddst;
	mlib_d64 d1, d2, d3, d4, d5, d6;
	mlib_s32 len_64, even_length, rest_64, length = n, i, off;
	mlib_s8 c;
	mlib_d64 four_16_ones = vis_to_double_dup(0x01000100);
	mlib_f32 zero = vis_fzeros();

	if (length < 16) {
		PACK_S_U(mlib_s8, mlib_u8);
	}

/*
 * First, try to align destination address for 8 bytes .
 */

	while ((mlib_addr)dst & 7) {
		(*dst++) = (c = (*src++)) < 0 ? 0 : c;
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
 * Peeling the 1st iteration.
 */

		if (i = (len_64 & 1)) {
			d1 = (*dsrc++);
			d2 = vis_fmul8sux16(vis_fpmerge(vis_read_hi(d1), zero),
				four_16_ones);
			d3 = vis_fmul8sux16(vis_fpmerge(vis_read_lo(d1), zero),
				four_16_ones);
			(*ddst++) = vis_fpack16_pair(d2, d3);
		}

/*
 * Then loop with step==2. Unroll for 2 iterations.
 */
#pragma pipeloop(0)
#pragma unroll(4)
		for (; i < len_64; i += 2) {
			d1 = (*dsrc++);
			d2 = vis_fmul8sux16(vis_fpmerge(vis_read_hi(d1), zero),
				four_16_ones);
			d3 = vis_fmul8sux16(vis_fpmerge(vis_read_lo(d1), zero),
				four_16_ones);
			(*ddst++) = vis_fpack16_pair(d2, d3);
			d1 = (*dsrc++);
			d2 = vis_fmul8sux16(vis_fpmerge(vis_read_hi(d1), zero),
				four_16_ones);
			d3 = vis_fmul8sux16(vis_fpmerge(vis_read_lo(d1), zero),
				four_16_ones);
			(*ddst++) = vis_fpack16_pair(d2, d3);
		}
	} else {

/*
 * Source address has arbitrary alignment. Use vis_alignaddr() and
 * vis_faligndata() functions.
 */

		dsrc = (mlib_d64 *)vis_alignaddr(src, 0);
		off = (mlib_addr)src & 7;
		vis_alignaddr((void *)0, 1);
		vis_write_bmask(0x11111111 * off, 0x04152637);
		d2 = (*dsrc++);

/*
 * Peeling of 1 iteration.
 */

		if (i = (len_64 & 1)) {
			d1 = d2;
			d2 = vis_ld_d64_nf(dsrc); dsrc++;
			d3 = vis_bshuffle(d1, d2);
			d4 = vis_fmul8sux16(d3, four_16_ones);
			d3 = vis_faligndata(d3, d3);
			d5 = vis_fmul8sux16(d3, four_16_ones);
			(*ddst++) = vis_fpack16_pair(d4, d5);
		}

/*
 * Then loop with step==2.
 */
#pragma pipeloop(0)
#pragma unroll(4)
		for (i; i < len_64; i += 2) {
			d1 = d2;
			d2 = vis_ld_d64_nf(dsrc); dsrc++;
			d3 = vis_bshuffle(d1, d2);
			d1 = d2;
			d2 = vis_ld_d64_nf(dsrc); dsrc++;
			d6 = vis_bshuffle(d1, d2);
			d4 = vis_fmul8sux16(d3, four_16_ones);
			d3 = vis_faligndata(d3, d3);
			d5 = vis_fmul8sux16(d3, four_16_ones);
			(*ddst++) = vis_fpack16_pair(d4, d5);
			d4 = vis_fmul8sux16(d6, four_16_ones);
			d6 = vis_faligndata(d6, d6);
			d5 = vis_fmul8sux16(d6, four_16_ones);
			(*ddst++) = vis_fpack16_pair(d4, d5);
		}
	}

	for (i = 0; i < rest_64; i++)
		dst[even_length + i] = (c = src[even_length + i]) < 0 ? 0 : c;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

/*
 *
 * For S16->U8 bits packing we apply  VIS instruction vis_fpack16 to 2*64-
 * bits registers and obtain 1*8 bytes of a result.
 *
 */

mlib_status
__mlib_VectorConvert_U8_S16_Sat(
	mlib_u8 *z,
	const mlib_s16 *x,
	mlib_s32 n)
{
	mlib_s16 *src = (void *)x;
	mlib_u8 *dst = z;
	mlib_d64 *dsrc, *ddst;
	mlib_d64 d1, d2, d3, d4, d5, d6, d7;
	mlib_s32 len_64, even_length, rest_64, length = n, i;
	mlib_s16 c;

	if (n < 16) {
		PACK_S_U_DF(mlib_s16, mlib_u8, MLIB_U8_MAX, 0);
	}

/*
 * First try to align destination address for 8 bytes .
 */

	while ((mlib_addr)dst & 7) {
		(*dst++) = (c =
			(*src++)) < 0 ? 0 : (c > MLIB_U8_MAX ? MLIB_U8_MAX : c);
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
 * Peeling the 1st iteration.
 */

		if (i = (len_64 & 1)) {
			d4 = (*dsrc++);
			d5 = (*dsrc++);
			d3 = vis_fpack16_pair(d4, d5);
			(*ddst++) = d3;
		}

/*
 * Then loop with step==2. Unroll for 2 iterations.
 */

#pragma pipeloop(0)
#pragma unroll(2)
		for (; i < len_64; i += 2) {
			d1 = (*dsrc++);
			d2 = (*dsrc++);
			d5 = (*dsrc++);
			d6 = (*dsrc++);
			d3 = vis_fpack16_pair(d1, d2);
			d7 = vis_fpack16_pair(d5, d6);
			(*ddst++) = d3;
			(*ddst++) = d7;
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
			d2 = vis_ld_d64_nf(dsrc); dsrc++;
			d3 = vis_faligndata(d1, d2);
			d1 = d2;
			d2 = vis_ld_d64_nf(dsrc); dsrc++;
			d4 = vis_faligndata(d1, d2);
			d3 = vis_fpack16_pair(d3, d4);
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
			d2 = (*dsrc++);
			d6 = vis_faligndata(d1, d2);
			d3 = vis_fpack16_pair(d3, d4);
			d5 = vis_fpack16_pair(d5, d6);
			(*ddst++) = d3;
			(*ddst++) = d5;
		}
	}

	for (i = 0; i < rest_64; i++)
		dst[even_length + i] = (c =
			src[even_length + i]) < 0 ? 0 : (c >
			MLIB_U8_MAX ? MLIB_U8_MAX : c);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S8_S16_Sat(
	mlib_s8 *z,
	const mlib_s16 *x,
	mlib_s32 n)
{
	mlib_s16 *src = (void *)x;
	mlib_s8 *dst = z;
	mlib_d64 *dsrc, *ddst;
	mlib_d64 d1, d2, d3, d4, d5, d6, d7;
	mlib_s32 len_64, even_length, rest_64, length = n, i;
	mlib_s16 c;

	if (n < 16) {
		PACK_S_S(mlib_s16, mlib_s8, MLIB_S8_MAX, MLIB_S8_MIN);
	}

/*
 * First try to align destination address for 8 bytes .
 */

	while ((mlib_s32)dst & 7) {
		(*dst++) = (c =
			(*src++)) < MLIB_S8_MIN ? MLIB_S8_MIN : (c >
			MLIB_S8_MAX ? MLIB_S8_MAX : c);
		length--;
	}

	rest_64 = length & 7;
	len_64 = length >> 3;
	even_length = len_64 << 3;
	ddst = (mlib_d64 *)dst;
	vis_write_gsr64(((mlib_u64)0x082A4C6E << 32) | (8 << 3) | 2);

/*
 * Now analyze source address alignment.
 */

	if (((mlib_addr)src & 7) == 0) {

		dsrc = (mlib_d64 *)src;

		if (i = (len_64 & 1)) {
			d1 = (*dsrc++);
			d2 = (*dsrc++);
			d3 = vis_fpackfix_pair(d1, d2);
			d1 = vis_faligndata(d1, d1);
			d2 = vis_faligndata(d2, d2);
			d4 = vis_fpackfix_pair(d1, d2);
			(*ddst++) = vis_bshuffle(d3, d4);
		}
#pragma pipeloop(0)
#pragma unroll(2)
		for (; i < len_64; i += 2) {
			d1 = (*dsrc++);
			d2 = (*dsrc++);
			d3 = vis_fpackfix_pair(d1, d2);
			d1 = vis_faligndata(d1, d1);
			d2 = vis_faligndata(d2, d2);
			d4 = vis_fpackfix_pair(d1, d2);
			(*ddst++) = vis_bshuffle(d3, d4);
			d1 = (*dsrc++);
			d2 = (*dsrc++);
			d3 = vis_fpackfix_pair(d1, d2);
			d1 = vis_faligndata(d1, d1);
			d2 = vis_faligndata(d2, d2);
			d4 = vis_fpackfix_pair(d1, d2);
			(*ddst++) = vis_bshuffle(d3, d4);
		}
	} else {

/*
 * Source address is arbitrary aligned. Use vis_alignaddr() and
 * vis_faligndata() functions.
 */

		dsrc = (mlib_d64 *)vis_alignaddr(src, 0);
		d2 = (*dsrc++);

/*
 * Peeling of 1 iteration.
 */

		if (i = (len_64 & 1)) {
			d1 = d2;
			d2 = vis_ld_d64_nf(dsrc); dsrc++;
			d4 = vis_faligndata(d1, d2);
			d1 = d2;
			d2 = vis_ld_d64_nf(dsrc); dsrc++;
			d5 = vis_faligndata(d1, d2);

			d3 = vis_fpackfix_pair(d4, d5);
			d4 = vis_fpack32(d4, d4);
			d4 = vis_fpack32(d4, d4);
			d5 = vis_fpmerge(vis_read_hi(d5), vis_read_lo(d5));
			d5 = vis_fpmerge(vis_read_lo(d5), vis_read_hi(d5));
			d5 = vis_fpmerge(vis_read_hi(d5), vis_read_lo(d5));
			d4 = vis_fpackfix_pair(d4, d5);
			(*ddst++) = vis_bshuffle(d3, d4);
		}

/*
 * Then loop with step==2.
 */

#pragma pipeloop(0)
#pragma unroll(2)
		for (i; i < len_64; i += 2) {
			d1 = d2;
			d2 = (*dsrc++);
			d4 = vis_faligndata(d1, d2);
			d1 = d2;
			d2 = (*dsrc++);
			d5 = vis_faligndata(d1, d2);
			d1 = d2;
			d2 = (*dsrc++);
			d6 = vis_faligndata(d1, d2);
			d1 = d2;
			d2 = vis_ld_d64_nf(dsrc); dsrc++;
			d7 = vis_faligndata(d1, d2);

			d3 = vis_fpackfix_pair(d4, d5);
			d4 = vis_fpack32(d4, d4);
			d4 = vis_fpack32(d4, d4);
			d5 = vis_fpmerge(vis_read_hi(d5), vis_read_lo(d5));
			d5 = vis_fpmerge(vis_read_lo(d5), vis_read_hi(d5));
			d5 = vis_fpmerge(vis_read_hi(d5), vis_read_lo(d5));
			d4 = vis_fpackfix_pair(d4, d5);

			d5 = vis_fpackfix_pair(d6, d7);
			d6 = vis_fpack32(d6, d6);
			d6 = vis_fpack32(d6, d6);
			d7 = vis_fpmerge(vis_read_hi(d7), vis_read_lo(d7));
			d7 = vis_fpmerge(vis_read_lo(d7), vis_read_hi(d7));
			d7 = vis_fpmerge(vis_read_hi(d7), vis_read_lo(d7));
			d6 = vis_fpackfix_pair(d6, d7);

			(*ddst++) = vis_bshuffle(d3, d4);
			(*ddst++) = vis_bshuffle(d5, d6);
		}
	}

	for (i = 0; i < rest_64; i++) {
		c = src[even_length + i];
		dst[even_length + i] = c < MLIB_S8_MIN ? MLIB_S8_MIN
			: (c > MLIB_S8_MAX ? MLIB_S8_MAX : c);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S8_S32_Sat(
	mlib_s8 *z,
	const mlib_s32 *x,
	mlib_s32 n)
{
	mlib_s32 *src = (void *)x;
	mlib_s8 *dst = z;
	mlib_d64 *dsrc, *ddst;
	mlib_d64 d0, d_tmp, d1, d2, d3, d4;
	mlib_s32 len_64, even_length, rest_64, length = n, i;
	mlib_s32 c;

	if (n < 8) {
		PACK_S_S(mlib_s32, mlib_s8, MLIB_S8_MAX, MLIB_S8_MIN);
	}

/*
 * First try to align destination address for 8 bytes .
 */

	while ((mlib_addr)dst & 7) {
		(*dst++) = (c = (*src++)) < MLIB_S8_MIN ? MLIB_S8_MIN
			: (c > MLIB_S8_MAX ? MLIB_S8_MAX : c);
		length--;
	}

	rest_64 = length & 7;
	len_64 = length >> 3;
	even_length = len_64 << 3;
	ddst = (mlib_d64 *)dst;

	vis_write_gsr64(((mlib_u64)0x02468ACE << 32) | (24 << 3));

/*
 * Now analyze source address alignment.
 */

	if (((mlib_addr)src & 7) == 0) {

/*
 * Source address is also 8-byte aligned.
 */

		dsrc = (mlib_d64 *)src;

#pragma pipeloop(0)
#pragma unroll(4)
		for (i = 0; i < len_64; i++) {
			d1 = (*dsrc++);
			d2 = (*dsrc++);
			d3 = (*dsrc++);
			d4 = (*dsrc++);
			d1 = vis_fpackfix_pair(d1, d2);
			d2 = vis_fpackfix_pair(d3, d4);
			(*ddst++) = vis_bshuffle(d1, d2);
		}
	} else {

/*
 * Source address is arbitrary aligned. Use vis_alignaddr() and
 * vis_faligndata() functions.
 */

		dsrc = (mlib_d64 *)vis_alignaddr(src, 0);
		d0 = (*dsrc++);

#pragma pipeloop(0)
#pragma unroll(4)
		for (i = 0; i < len_64; i++) {
			d_tmp = (*dsrc++);
			d1 = vis_faligndata(d0, d_tmp);
			d0 = (*dsrc++);
			d2 = vis_faligndata(d_tmp, d0);
			d_tmp = (*dsrc++);
			d3 = vis_faligndata(d0, d_tmp);
			d0 = vis_ld_d64_nf(dsrc); dsrc++;
			d4 = vis_faligndata(d_tmp, d0);
			d1 = vis_fpackfix_pair(d1, d2);
			d2 = vis_fpackfix_pair(d3, d4);
			(*ddst++) = vis_bshuffle(d1, d2);
		}
	}

	for (i = 0; i < rest_64; i++) {
		c = src[even_length + i];
		dst[even_length + i] = c < MLIB_S8_MIN ? MLIB_S8_MIN
			: (c > MLIB_S8_MAX ? MLIB_S8_MAX : c);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_U8_S32_Sat(
	mlib_u8 *z,
	const mlib_s32 *x,
	mlib_s32 n)
{
	mlib_s32 *src = (void *)x;
	mlib_u8 *dst = z;
	mlib_d64 *dsrc, *ddst;
	mlib_d64 d0, d_tmp, d1, d2, d3, d4;
	mlib_s32 len_64, even_length, rest_64, length = n, i;
	mlib_s32 c;

	if (n < 8) {
		PACK_S_S(mlib_s32, mlib_u8, MLIB_U8_MAX, 0);
	}

/*
 * First try to align destination address for 8 bytes .
 */

	while ((mlib_addr)dst & 7) {
		(*dst++) = (c =
			(*src++)) < 0 ? 0 : (c > MLIB_U8_MAX ? MLIB_U8_MAX : c);
		length--;
	}

	rest_64 = length & 7;
	len_64 = length >> 3;
	even_length = len_64 << 3;
	ddst = (mlib_d64 *)dst;

	vis_write_gsr(23 << 3);

/*
 * Now analyze source address alignment.
 */

	if (((mlib_addr)src & 7) == 0) {

/*
 * Source address is also 8-byte aligned.
 */

		dsrc = (mlib_d64 *)src;

#pragma pipeloop(0)
#pragma unroll(4)
		for (i = 0; i < len_64; i++) {
			d1 = (*dsrc++);
			d2 = (*dsrc++);
			d3 = (*dsrc++);
			d4 = (*dsrc++);
			d1 = vis_fpack32(d1, d1);
			d2 = vis_fpack32(d1, d2);
			d3 = vis_fpack32(d2, d3);
			d4 = vis_fpack32(d3, d4);
			(*ddst++) =
				vis_fpmerge(vis_read_hi(d4), vis_read_lo(d4));
		}
	} else {

/*
 * Source address is arbitrary aligned. Use vis_alignaddr() and
 * vis_faligndata() functions.
 */

		dsrc = (mlib_d64 *)vis_alignaddr(src, 0);
		d0 = (*dsrc++);

#pragma pipeloop(0)
#pragma unroll(4)
		for (i = 0; i < len_64; i++) {
			d_tmp = (*dsrc++);
			d1 = vis_faligndata(d0, d_tmp);
			d0 = (*dsrc++);
			d2 = vis_faligndata(d_tmp, d0);
			d_tmp = (*dsrc++);
			d3 = vis_faligndata(d0, d_tmp);
			d0 = vis_ld_d64_nf(dsrc); dsrc++;
			d4 = vis_faligndata(d_tmp, d0);
			d1 = vis_fpack32(d1, d1);
			d2 = vis_fpack32(d1, d2);
			d3 = vis_fpack32(d2, d3);
			d4 = vis_fpack32(d3, d4);
			(*ddst++) =
				vis_fpmerge(vis_read_hi(d4), vis_read_lo(d4));
		}
	}

	for (i = 0; i < rest_64; i++) {
		c = src[even_length + i];
		dst[even_length + i] = c < MLIB_U8_MIN ? MLIB_U8_MIN
			: (c > MLIB_U8_MAX ? MLIB_U8_MAX : c);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S16_S32_Sat(
	mlib_s16 *z,
	const mlib_s32 *x,
	mlib_s32 n)
{
	mlib_s32 *src = (void *)x;
	mlib_s16 *dst = z;
	mlib_d64 *dsrc, *ddst;
	mlib_d64 d0, d1, d2, d3, d4, d5, d6, d7, d8;
	mlib_s32 c;
	mlib_s32 len_64, even_length, rest_64, length = n, i;

	if (n < 16) {
		PACK_S_S(mlib_s32, mlib_s16, MLIB_S16_MAX, MLIB_S16_MIN);
	}

/*
 * First try to align destination address for 8 bytes.
 */

	while ((mlib_addr)dst & 7) {
		(*dst++) = (c = *src) > MLIB_S16_MAX ? MLIB_S16_MAX
			: (c < MLIB_S16_MIN ? MLIB_S16_MIN : c);
		src++;
		length--;
	}

	vis_write_gsr(16 << 3);

	rest_64 = length & 3;
	len_64 = length >> 2;
	even_length = len_64 << 2;
	ddst = (mlib_d64 *)dst;

	if (((mlib_addr)src & 7) == 0) {

/*
 * Source address is also 8-byte aligned.
 */

		dsrc = (mlib_d64 *)src;

/*
 * Peeling the 1st iteration.
 */

		if (i = (len_64 & 1)) {
			d1 = (*dsrc++);
			d2 = (*dsrc++);
			(*ddst++) =
				vis_freg_pair(vis_fpackfix(d1),
				vis_fpackfix(d2));
		}

/*
 * Then loop with step==2.
 */

#pragma pipeloop(0)
#pragma unroll(4)
		for (; i < len_64; i += 2) {
			d1 = (*dsrc++);
			d2 = (*dsrc++);
			d3 = (*dsrc++);
			d4 = (*dsrc++);
			(*ddst++) =
				vis_freg_pair(vis_fpackfix(d1),
				vis_fpackfix(d2));
			(*ddst++) =
				vis_freg_pair(vis_fpackfix(d3),
				vis_fpackfix(d4));
		}
	} else {

/*
 * Source address is arbitrary aligned. Use vis_alignaddr() and
 * vis_faligndata() functions.
 */

		dsrc = (mlib_d64 *)vis_alignaddr(src, 0);
		d4 = (*dsrc++);

/*
 * Peeling of 1 iteration.
 */

		if (i = (len_64 & 1)) {
			d1 = d4;
			d2 = (*dsrc++);
			d4 = vis_ld_d64_nf(dsrc); dsrc++;
			d5 = vis_faligndata(d1, d2);
			d6 = vis_faligndata(d2, d4);
			(*ddst++) =
				vis_freg_pair(vis_fpackfix(d5),
				vis_fpackfix(d6));
		}

/*
 * Then loop with step==2.
 */

#pragma pipeloop(0)
#pragma unroll(4)
		for (; i < len_64; i += 2) {
			d0 = d4;
			d1 = (*dsrc++);
			d2 = (*dsrc++);
			d3 = (*dsrc++);
			d4 = vis_ld_d64_nf(dsrc); dsrc++;
			d5 = vis_faligndata(d0, d1);
			d6 = vis_faligndata(d1, d2);
			d7 = vis_faligndata(d2, d3);
			d8 = vis_faligndata(d3, d4);
			(*ddst++) =
				vis_freg_pair(vis_fpackfix(d5),
				vis_fpackfix(d6));
			(*ddst++) =
				vis_freg_pair(vis_fpackfix(d7),
				vis_fpackfix(d8));
		}
	}

	for (i = 0; i < rest_64; i++) {
		c = src[even_length + i];
		dst[even_length + i] = c > MLIB_S16_MAX ? MLIB_S16_MAX
			: (c < MLIB_S16_MIN ? MLIB_S16_MIN : c);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S8C_U8C_Sat(
	mlib_s8 *z,
	const mlib_u8 *x,
	mlib_s32 n)
{
	return (__mlib_VectorConvert_S8_U8_Sat(z, x, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_U8C_S8C_Sat(
	mlib_u8 *z,
	const mlib_s8 *x,
	mlib_s32 n)
{
	return (__mlib_VectorConvert_U8_S8_Sat(z, x, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_U8C_S16C_Sat(
	mlib_u8 *z,
	const mlib_s16 *x,
	mlib_s32 n)
{
	return (__mlib_VectorConvert_U8_S16_Sat(z, x, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S8C_S16C_Sat(
	mlib_s8 *z,
	const mlib_s16 *x,
	mlib_s32 n)
{
	return (__mlib_VectorConvert_S8_S16_Sat(z, x, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_U8C_S32C_Sat(
	mlib_u8 *z,
	const mlib_s32 *x,
	mlib_s32 n)
{
	return (__mlib_VectorConvert_U8_S32_Sat(z, x, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S8C_S32C_Sat(
	mlib_s8 *z,
	const mlib_s32 *x,
	mlib_s32 n)
{
	return (__mlib_VectorConvert_S8_S32_Sat(z, x, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorConvert_S16C_S32C_Sat(
	mlib_s16 *z,
	const mlib_s32 *x,
	mlib_s32 n)
{
	return (__mlib_VectorConvert_S16_S32_Sat(z, x, n + n));
}

/* *********************************************************** */
