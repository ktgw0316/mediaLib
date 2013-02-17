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

#pragma ident	"@(#)mlib_v_VectorConvert3.c	9.5	07/11/05 SMI"

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

/* Makes packing S32->S8 with overflow control */

#define	PACK_S32_S8                                                       \
	vis_write_gsr(15 << 3);                                           \
	mask = vis_fcmpgt32(d1, d_Max_S8);                                \
	mask1 = vis_fcmpgt32(d2, d_Max_S8);                               \
	mask2 = vis_fcmpgt32(d3, d_Max_S8);                               \
	mask3 = vis_fcmpgt32(d4, d_Max_S8);                               \
	mask_ovfl = mask3 | (mask2 << 2) | (mask1 << 4) | (mask << 6);    \
	mask = vis_fcmplt32(d1, d_Min_S8);                                \
	mask1 = vis_fcmplt32(d2, d_Min_S8);                               \
	mask2 = vis_fcmplt32(d3, d_Min_S8);                               \
	mask3 = vis_fcmplt32(d4, d_Min_S8);                               \
	mask_undfl = mask3 | (mask2 << 2) | (mask1 << 4) | (mask << 6);   \
	d1 = vis_fpadd32(d1, d1);                                         \
	d2 = vis_fpadd32(d2, d2);                                         \
	d3 = vis_fpadd32(d3, d3);                                         \
	d4 = vis_fpadd32(d4, d4);                                         \
	d1 = vis_fpackfix_pair(d1, d2);                                   \
	d2 = vis_fpackfix_pair(d3, d4);                                   \
	vis_write_gsr(7 << 3);                                            \
	d4 = vis_fpadd16(d1, d_Min_S8_S32);                               \
	d5 = vis_fpadd16(d2, d_Min_S8_S32);                               \
	d3 = vis_fpack16_pair(d4, d5);                                    \
	*ddst = vis_fxor(d3, dlog_min);                                   \
	vis_pst_8(dlog_max, ddst, mask_ovfl);                             \
	vis_pst_8(dlog_min, ddst, mask_undfl);                            \
	ddst++;

/* Makes packing S32->U8 with overflow control */

#define	PACK_S32_U8                                                       \
	vis_write_gsr(gsr_for_fpackfix);                                  \
	mask = vis_fcmpgt32(d1, d_Max_U8);                                \
	mask1 = vis_fcmpgt32(d2, d_Max_U8);                               \
	mask2 = vis_fcmpgt32(d3, d_Max_U8);                               \
	mask3 = vis_fcmpgt32(d4, d_Max_U8);                               \
	mask_ovfl = mask3 | (mask2 << 2) | (mask1 << 4) | (mask << 6);    \
	mask = vis_fcmplt32(d1, d_Min_U8);                                \
	mask1 = vis_fcmplt32(d2, d_Min_U8);                               \
	mask2 = vis_fcmplt32(d3, d_Min_U8);                               \
	mask3 = vis_fcmplt32(d4, d_Min_U8);                               \
	mask_undfl = mask3 | (mask2 << 2) | (mask1 << 4) | (mask << 6);   \
	d1 = vis_fpadd32(d1, d1);                                         \
	d2 = vis_fpadd32(d2, d2);                                         \
	d3 = vis_fpadd32(d3, d3);                                         \
	d4 = vis_fpadd32(d4, d4);                                         \
	d1 = vis_fpackfix_pair(d1, d2);                                   \
	d2 = vis_fpackfix_pair(d3, d4);                                   \
	vis_write_gsr(gsr_for_fpack16);                                   \
	*ddst = vis_fpack16_pair(d1, d2);                                 \
	vis_pst_8(dlog_max, ddst, mask_ovfl);                             \
	vis_pst_8(dlog_min, ddst, mask_undfl);                            \
	ddst++;

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
		PACK_U_S(mlib_u8,
			mlib_s8,
			MLIB_S8_MAX);
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
 * Peeling the 1 - st iteration.
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
			d2 = (*dsrc++);
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
			d2 = (*dsrc++);
			d3 = vis_faligndata(d1, d2);
			d1 = d2;
			d2 = (*dsrc++);
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

mlib_status
__mlib_VectorConvert_U8_S8_Sat(
	mlib_u8 *z,
	const mlib_s8 *x,
	mlib_s32 n)
{
	mlib_s8 *src = (void *)x;
	mlib_u8 *dst = z;
	mlib_d64 *dsrc, *ddst;
	mlib_d64 d0, d1, d2, d3, d4, d5, d6, d7, d8;
	mlib_s32 len_64, even_length, rest_64, length = n, i;
	mlib_s8 c;
	mlib_d64 four_16_ones = vis_to_double_dup(0x01000100);
	mlib_f32 zero = vis_fzeros();

	if (length < 16) {
		PACK_S_U(mlib_s8,
			mlib_u8);
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
 * Peeling the 1 - st iteration.
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
		d2 = (*dsrc++);

/*
 * Peeling of 1 iteration.
 */

		if (i = (len_64 & 1)) {
			d1 = d2;
			d2 = (*dsrc++);
			d3 = vis_faligndata(d1, d2);
			d4 = vis_fmul8sux16(vis_fpmerge(vis_read_hi(d3), zero),
				four_16_ones);
			d5 = vis_fmul8sux16(vis_fpmerge(vis_read_lo(d3), zero),
				four_16_ones);
			(*ddst++) = vis_fpack16_pair(d4, d5);
		}

/*
 * Then loop with step==2.
 */

		for (i; i < len_64; i += 2) {
			d0 = d2;
			d1 = (*dsrc++);
			d2 = (*dsrc++);
			d3 = vis_faligndata(d0, d1);
			d4 = vis_faligndata(d1, d2);
			d5 = vis_fmul8sux16(vis_fpmerge(vis_read_hi(d3), zero),
				four_16_ones);
			d6 = vis_fmul8sux16(vis_fpmerge(vis_read_lo(d3), zero),
				four_16_ones);
			d7 = vis_fmul8sux16(vis_fpmerge(vis_read_hi(d4), zero),
				four_16_ones);
			d8 = vis_fmul8sux16(vis_fpmerge(vis_read_lo(d4), zero),
				four_16_ones);
			(*ddst++) = vis_fpack16_pair(d5, d6);
			(*ddst++) = vis_fpack16_pair(d7, d8);
		}
	}

	for (i = 0; i < rest_64; i++)
		dst[even_length + i] = (c = src[even_length + i]) < 0 ? 0 : c;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

/*
 *
 * For S16->U8 bits packing we apply  VIS instruction vis_fpack16 to two
 * 64-bit registers and obtain 8 bytes of a result.
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
		PACK_S_U_DF(mlib_s16,
			mlib_u8,
			MLIB_U8_MAX,
			0);
	}

/*
 * First try to align destination address for 8 bytes.
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
 * Peeling the 1 - st iteration.
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
			d2 = (*dsrc++);
			d3 = vis_faligndata(d1, d2);
			d1 = d2;
			d2 = (*dsrc++);
			d4 = vis_faligndata(d1, d2);
			d3 = vis_fpack16_pair(d3, d4);
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

/*
 *
 * For S16->S8 bits packing we work in two steps.
 * Fisrt, we make R-shift (in numeric sense) for +128 for each
 * 16-bit operand inside two 64-bit registers. Then we apply VIS
 * instruction vis_fpack16 to obtain 8 bytes of an intermediate result.
 * And then we decrease each byte of it for 128 again, and make it
 * by vis_fxor.
 *
 */

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
	mlib_d64 d_Max_S8 =
		vis_to_double_dup(((MLIB_S8_MAX << 16) | MLIB_S8_MAX));
	mlib_d64 d_Min_S8_S16 = vis_to_double_dup(0x00800080);
	mlib_d64 dlog_min = vis_to_double_dup(0x80808080);
	mlib_d64 dlog_max = vis_to_double_dup(0x7f7f7f7f);
	mlib_s32 len_64, even_length, rest_64, length = n, i;
	mlib_s32 mask, mask1;
	mlib_s16 c;

	if (n < 16) {
		PACK_S_S(mlib_s16,
			mlib_s8,
			MLIB_S8_MAX,
			MLIB_S8_MIN);
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
			d2 = (*dsrc++);
			mask = vis_fcmpgt16(d1, d_Max_S8);
			mask1 = vis_fcmpgt16(d2, d_Max_S8);
			d4 = vis_fpadd16(d1, d_Min_S8_S16);
			d5 = vis_fpadd16(d2, d_Min_S8_S16);
			mask1 |= mask << 4;
			d3 = vis_fpack16_pair(d4, d5);
			*ddst = vis_fxor(d3, dlog_min);
			vis_pst_8(dlog_max, ddst, mask1);
			ddst++;
		}

/*
 * Then loop with step==2. Unrolling for 2 iterations.
 */

#pragma pipeloop(0)
		for (; i < len_64; i += 2) {
			d1 = (*dsrc++);
			d2 = (*dsrc++);
			mask = vis_fcmpgt16(d1, d_Max_S8);
			mask1 = vis_fcmpgt16(d2, d_Max_S8);
			d4 = vis_fpadd16(d1, d_Min_S8_S16);
			d5 = vis_fpadd16(d2, d_Min_S8_S16);
			d1 = (*dsrc++);
			d2 = (*dsrc++);
			mask1 |= mask << 4;
			d3 = vis_fpack16_pair(d4, d5);
			*ddst = vis_fxor(d3, dlog_min);
			vis_pst_8(dlog_max, ddst, mask1);
			ddst++;
			mask = vis_fcmpgt16(d1, d_Max_S8);
			mask1 = vis_fcmpgt16(d2, d_Max_S8);
			d4 = vis_fpadd16(d1, d_Min_S8_S16);
			d5 = vis_fpadd16(d2, d_Min_S8_S16);
			mask1 |= mask << 4;
			d3 = vis_fpack16_pair(d4, d5);
			*ddst = vis_fxor(d3, dlog_min);
			vis_pst_8(dlog_max, ddst, mask1);
			ddst++;
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
			d2 = (*dsrc++);
			d4 = vis_faligndata(d1, d2);
			d1 = d2;
			d2 = (*dsrc++);
			d5 = vis_faligndata(d1, d2);
			mask = vis_fcmpgt16(d4, d_Max_S8);
			mask1 = vis_fcmpgt16(d5, d_Max_S8);
			d4 = vis_fpadd16(d4, d_Min_S8_S16);
			d5 = vis_fpadd16(d5, d_Min_S8_S16);
			mask1 |= mask << 4;
			d3 = vis_fpack16_pair(d4, d5);
			*ddst = vis_fxor(d3, dlog_min);
			vis_pst_8(dlog_max, ddst, mask1);
			ddst++;
		}

/*
 * Then loop with step==2.
 */

#pragma pipeloop(0)
		for (i; i < len_64; i += 2) {
			d1 = d2;
			d2 = (*dsrc++);
			d4 = vis_faligndata(d1, d2);
			d1 = d2;
			d2 = (*dsrc++);
			d5 = vis_faligndata(d1, d2);
			mask = vis_fcmpgt16(d4, d_Max_S8);
			mask1 = vis_fcmpgt16(d5, d_Max_S8);
			d6 = vis_fpadd16(d4, d_Min_S8_S16);
			d7 = vis_fpadd16(d5, d_Min_S8_S16);
			mask1 |= mask << 4;
			d3 = vis_fpack16_pair(d6, d7);
			d1 = d2;
			d2 = (*dsrc++);
			d4 = vis_faligndata(d1, d2);
			d1 = d2;
			d2 = (*dsrc++);
			d5 = vis_faligndata(d1, d2);
			*ddst = vis_fxor(d3, dlog_min);
			vis_pst_8(dlog_max, ddst, mask1);
			ddst++;
			mask = vis_fcmpgt16(d4, d_Max_S8);
			mask1 = vis_fcmpgt16(d5, d_Max_S8);
			d6 = vis_fpadd16(d4, d_Min_S8_S16);
			d7 = vis_fpadd16(d5, d_Min_S8_S16);
			mask1 |= mask << 4;
			d3 = vis_fpack16_pair(d6, d7);
			*ddst = vis_fxor(d3, dlog_min);
			vis_pst_8(dlog_max, ddst, mask1);
			ddst++;
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
	mlib_d64 d0, d_tmp, d1, d2, d3, d4, d5;
	mlib_d64 d_Min_S8 = vis_to_double_dup(0xffffff80);
	mlib_d64 d_Max_S8 = vis_fnot(d_Min_S8);
	mlib_d64 d_Min_S8_S32 = vis_to_double_dup(0x800080);
	mlib_d64 dlog_min = vis_to_double_dup(0x80808080);
	mlib_d64 dlog_max = vis_fnot(dlog_min);
	mlib_s32 len_64, even_length, rest_64, length = n, i;
	mlib_s32 mask, mask1, mask2, mask3, mask_ovfl, mask_undfl;
	mlib_s32 c;

	if (n < 8) {
		PACK_S_S(mlib_s32,
			mlib_s8,
			MLIB_S8_MAX,
			MLIB_S8_MIN);
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

/*
 * Now analyze source address alignment.
 */

	if (((mlib_addr)src & 7) == 0) {

/*
 * Source address is also 8-byte aligned.
 */

		dsrc = (mlib_d64 *)src;

#pragma pipeloop(0)
		for (i = 0; i < len_64; i++) {
			d1 = (*dsrc++);
			d2 = (*dsrc++);
			d3 = (*dsrc++);
			d4 = (*dsrc++);
			PACK_S32_S8;
		}
	} else {

/*
 * Source address is arbitrary aligned. Use vis_alignaddr() and
 * vis_faligndata() functions.
 */

		dsrc = (mlib_d64 *)vis_alignaddr(src, 0);
		d0 = (*dsrc++);

#pragma pipeloop(0)
		for (i = 0; i < len_64; i++) {
			d_tmp = (*dsrc++);
			d1 = vis_faligndata(d0, d_tmp);
			d0 = (*dsrc++);
			d2 = vis_faligndata(d_tmp, d0);
			d_tmp = (*dsrc++);
			d3 = vis_faligndata(d0, d_tmp);
			d0 = (*dsrc++);
			d4 = vis_faligndata(d_tmp, d0);
			PACK_S32_S8;
			vis_alignaddr(src, 0);
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
	mlib_d64 d_Min_U8 = vis_fzero();
	mlib_d64 d_Max_U8 = vis_to_double_dup(MLIB_U8_MAX);
	mlib_d64 dlog_min = d_Min_U8, dlog_max = vis_to_double_dup(0xffffffff);
	mlib_s32 len_64, even_length, rest_64, length = n, i;
	mlib_s32 mask, mask1, mask2, mask3, mask_ovfl, mask_undfl;
	mlib_s32 gsr_for_fpackfix, gsr_for_fpack16;
	mlib_s32 c;

	if (n < 8) {
		PACK_S_S(mlib_s32,
			mlib_u8,
			MLIB_U8_MAX,
			0);
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
	gsr_for_fpackfix = (15 << 3) | ((mlib_s32)src & 7);
	gsr_for_fpack16 = (7 << 3) | ((mlib_s32)src & 7);

/*
 * Now analyze source address alignment.
 */

	if (((mlib_addr)src & 7) == 0) {

/*
 * Source address is also 8-byte aligned.
 */

		dsrc = (mlib_d64 *)src;

#pragma pipeloop(0)
		for (i = 0; i < len_64; i++) {
			d1 = (*dsrc++);
			d2 = (*dsrc++);
			d3 = (*dsrc++);
			d4 = (*dsrc++);
			PACK_S32_U8;
		}
	} else {

/*
 * Source address is arbitrary aligned. Use vis_alignaddr() and
 * vis_faligndata() functions.
 */

		dsrc = (mlib_d64 *)vis_alignaddr(src, 0);
		d0 = (*dsrc++);

#pragma pipeloop(0)
		for (i = 0; i < len_64; i++) {
			d_tmp = (*dsrc++);
			d1 = vis_faligndata(d0, d_tmp);
			d0 = (*dsrc++);
			d2 = vis_faligndata(d_tmp, d0);
			d_tmp = (*dsrc++);
			d3 = vis_faligndata(d0, d_tmp);
			d0 = (*dsrc++);
			d4 = vis_faligndata(d_tmp, d0);
			PACK_S32_U8;
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

/*
 *
 * For S32->S16 bits packing we work in the following manner.
 * Fisrt, read 2*64-bit operands onto f-registers.
 * Because we have no left-shift for 16 bits in vis_fpackfix(),
 * we make shifting for 1 bit before applying this instruction (by
 * vis_fpadd(dd, dd) ).
 * Also before that we form masks by comparing the source 2*64 with
 * MLIB_S16_MAX and MLIB_S16_MIN.
 * Then we apply 2 times vis_fpackfix() with shifting for 15 bits
 * and obtain 4*16 (1*64) object.
 * After two vis_pst_16() with masks formed in advance we obtain
 * final result.
 *
 */

mlib_status
__mlib_VectorConvert_S16_S32_Sat(
	mlib_s16 *z,
	const mlib_s32 *x,
	mlib_s32 n)
{
	mlib_s32 *src = (void *)x;
	mlib_s16 *dst = z;
	mlib_d64 *dsrc, *ddst;
	mlib_d64 d1, d2, d3, d4, d5;
	mlib_s32 c;
	mlib_s32 len_64, even_length, rest_64, length =
		n, i, mask, mask1, mask2, mask3;
	mlib_d64 d_Min_S16 = vis_to_double_dup(MLIB_S16_MIN);
	mlib_d64 d_Max_S16 = vis_to_double_dup(MLIB_S16_MAX);
	mlib_d64 dlog_min = vis_to_double_dup(0x80008000);
	mlib_d64 dlog_max = vis_to_double_dup(0x7fff7fff);

	if (n < 16) {
		PACK_S_S(mlib_s32,
			mlib_s16,
			MLIB_S16_MAX,
			MLIB_S16_MIN);
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

	vis_write_gsr(15 << 3);

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
 * Peeling the 1 - st iteration.
 */

		if (i = (len_64 & 1)) {
			d1 = (*dsrc++);
			d2 = (*dsrc++);
			mask = vis_fcmpgt32(d1, d_Max_S16);
			mask1 = vis_fcmpgt32(d2, d_Max_S16);
			mask2 = vis_fcmplt32(d1, d_Min_S16);
			mask3 = vis_fcmplt32(d2, d_Min_S16);
			mask1 |= mask << 2;
			mask3 |= mask2 << 2;
			d3 = vis_fpadd32(d1, d1);
			d4 = vis_fpadd32(d2, d2);
			*ddst = vis_freg_pair(vis_fpackfix(d3),
				vis_fpackfix(d4));
			vis_pst_16(dlog_max, ddst, mask1);
			vis_pst_16(dlog_min, ddst, mask3);
			ddst++;
		}

/*
 * Then loop with step==2.
 */

#pragma pipeloop(0)
		for (; i < len_64; i += 2) {
			d1 = (*dsrc++);
			d2 = (*dsrc++);
			mask = vis_fcmpgt32(d1, d_Max_S16);
			mask1 = vis_fcmpgt32(d2, d_Max_S16);
			mask2 = vis_fcmplt32(d1, d_Min_S16);
			mask3 = vis_fcmplt32(d2, d_Min_S16);
			d3 = vis_fpadd32(d1, d1);
			d4 = vis_fpadd32(d2, d2);
			d1 = (*dsrc++);
			d2 = (*dsrc++);
			mask1 |= mask << 2;
			mask3 |= mask2 << 2;
			*ddst = vis_freg_pair(vis_fpackfix(d3),
				vis_fpackfix(d4));
			vis_pst_16(dlog_max, ddst, mask1);
			vis_pst_16(dlog_min, ddst, mask3);
			ddst++;
			mask = vis_fcmpgt32(d1, d_Max_S16);
			mask1 = vis_fcmpgt32(d2, d_Max_S16);
			mask2 = vis_fcmplt32(d1, d_Min_S16);
			mask3 = vis_fcmplt32(d2, d_Min_S16);
			d3 = vis_fpadd32(d1, d1);
			d4 = vis_fpadd32(d2, d2);
			mask1 |= mask << 2;
			mask3 |= mask2 << 2;
			*ddst = vis_freg_pair(vis_fpackfix(d3),
				vis_fpackfix(d4));
			vis_pst_16(dlog_max, ddst, mask1);
			vis_pst_16(dlog_min, ddst, mask3);
			ddst++;
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
			d2 = (*dsrc++);
			d4 = vis_faligndata(d1, d2);
			d1 = d2;
			d2 = (*dsrc++);
			d5 = vis_faligndata(d1, d2);
			mask = vis_fcmpgt32(d4, d_Max_S16);
			mask1 = vis_fcmpgt32(d5, d_Max_S16);
			mask2 = vis_fcmplt32(d4, d_Min_S16);
			mask3 = vis_fcmplt32(d5, d_Min_S16);
			mask1 |= mask << 2;
			mask3 |= mask2 << 2;
			d3 = vis_fpadd32(d4, d4);
			d4 = vis_fpadd32(d5, d5);
			*ddst = vis_freg_pair(vis_fpackfix(d3),
				vis_fpackfix(d4));
			vis_pst_16(dlog_max, ddst, mask1);
			vis_pst_16(dlog_min, ddst, mask3);
			ddst++;
		}

/*
 * Then loop with step==2.
 */

#pragma pipeloop(0)
		for (; i < len_64; i += 2) {
			d1 = d2;
			d2 = (*dsrc++);
			d4 = vis_faligndata(d1, d2);
			d1 = d2;
			d2 = (*dsrc++);
			d5 = vis_faligndata(d1, d2);
			mask = vis_fcmpgt32(d4, d_Max_S16);
			mask1 = vis_fcmpgt32(d5, d_Max_S16);
			mask2 = vis_fcmplt32(d4, d_Min_S16);
			mask3 = vis_fcmplt32(d5, d_Min_S16);
			d3 = vis_fpadd32(d4, d4);
			d4 = vis_fpadd32(d5, d5);
			mask1 |= mask << 2;
			mask3 |= mask2 << 2;
			*ddst = vis_freg_pair(vis_fpackfix(d3),
				vis_fpackfix(d4));
			d1 = d2;
			d2 = (*dsrc++);
			d4 = vis_faligndata(d1, d2);
			d1 = d2;
			d2 = (*dsrc++);
			d5 = vis_faligndata(d1, d2);
			vis_pst_16(dlog_max, ddst, mask1);
			vis_pst_16(dlog_min, ddst, mask3);
			ddst++;
			mask = vis_fcmpgt32(d4, d_Max_S16);
			mask1 = vis_fcmpgt32(d5, d_Max_S16);
			mask2 = vis_fcmplt32(d4, d_Min_S16);
			mask3 = vis_fcmplt32(d5, d_Min_S16);
			d3 = vis_fpadd32(d4, d4);
			d4 = vis_fpadd32(d5, d5);
			mask1 |= mask << 2;
			mask3 |= mask2 << 2;
			*ddst = vis_freg_pair(vis_fpackfix(d3),
				vis_fpackfix(d4));
			vis_pst_16(dlog_max, ddst, mask1);
			vis_pst_16(dlog_min, ddst, mask3);
			ddst++;
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
