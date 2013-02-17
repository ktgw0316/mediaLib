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

#pragma ident	"@(#)mlib_v_VectorConjRevC.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorConjRev_S8C_S8C_Sat - computation of complex conjugated
 *                                       reverse vector having 8-bit format
 *      mlib_VectorConjRev_S16C_S16C_Sat - computation of complex conjugated
 *                                         reverse vector having 16-bit format
 *      mlib_VectorConjRev_S32C_S32C_Sat - computation of complex conjugated
 *                                         reverse vector having 32-bit format
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VectorConjRev_S16C_S16C_Sat(mlib_s16       *z,
 *                                                   const mlib_s16 *x,
 *                                                   mlib_s32       n);
 *      mlib_status mlib_VectorConjRev_S32C_S32C_Sat(mlib_s32       *z,
 *                                                   const mlib_s32 *x,
 *                                                   mlib_s32       n);
 *      mlib_status mlib_VectorConjRev_S8C_S8C_Sat(mlib_s8       *z,
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
 *              *      *        *
 *      z = ( x[n] x[n - 1] ... x[1] )
 */

#include <mlib_algebra.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorConjRev_S32C_S32C_Sat = \
	__mlib_VectorConjRev_S32C_S32C_Sat
#pragma weak mlib_VectorConjRev_S16C_S16C_Sat = \
	__mlib_VectorConjRev_S16C_S16C_Sat
#pragma weak mlib_VectorConjRev_S8C_S8C_Sat = \
	__mlib_VectorConjRev_S8C_S8C_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_VectorConjRev_S32C_S32C_Sat) mlib_VectorConjRev_S32C_S32C_Sat
	__attribute__((weak, alias("__mlib_VectorConjRev_S32C_S32C_Sat")));
__typeof__(__mlib_VectorConjRev_S16C_S16C_Sat) mlib_VectorConjRev_S16C_S16C_Sat
	__attribute__((weak, alias("__mlib_VectorConjRev_S16C_S16C_Sat")));
__typeof__(__mlib_VectorConjRev_S8C_S8C_Sat) mlib_VectorConjRev_S8C_S8C_Sat
	__attribute__((weak, alias("__mlib_VectorConjRev_S8C_S8C_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	CHECK(x, z)                                             \
	if ((x == NULL) || (z == NULL) || (n <= 0))             \
	    return MLIB_FAILURE

/* *********************************************************** */

#define	CONJREVC(form, MAX, MIN)                                \
	form *src = (form *) xx, *dst = (form *) zz;            \
	mlib_s32 i;                                             \
	form c;                                                 \
	                                                        \
	dst = &dst[2 * n];                                      \
	for (i = 0; i < n * 2; i += 2) {                        \
	    dst -= 2;                                           \
	    dst[0] = src[i];                                    \
	    if ((c = src[i + 1]) == MIN)                        \
		dst[1] = MAX;                                   \
	    else                                                \
		dst[1] = -c;                                    \
	}                                                       \
	                                                        \
	return MLIB_SUCCESS

/* *********************************************************** */

#define	CONJ8                                                             \
	d3 = vis_fxor(d3, dxor0);                                         \
	dh = vis_fpmerge(f_null, vis_read_hi(d3));                        \
	dl = vis_fpmerge(f_null, vis_read_lo(d3));                        \
	d3 =                                                              \
	vis_fpack16_pair(vis_fpadd16(dh, done), vis_fpadd16(dl, done));   \
	d3 = vis_bshuffle(d3, d3);                                        \
	d4 = vis_fxor(d3, dcntr0)

/* *********************************************************** */

#define	CONJ16                                                            \
	d4 = vis_fxor(d3, dlog0);                                         \
	dh = vis_fmuld8ulx16(f_two, vis_read_hi(d4));                     \
	dl = vis_fmuld8ulx16(f_two, vis_read_lo(d4));                     \
	d4 =                                                              \
	vis_fpackfix_pair(vis_fpadd32(dl, dtwo), vis_fpadd32(dh, dtwo))

/* *********************************************************** */

#define	CONJ32                                                  \
	d4 = vis_fxor(d3, dlog0);                               \
	mask = vis_fcmpeq32(d4, dcntr0) & 1;                    \
	d3 = vis_fpadd32(d_one, d4);                            \
	*--ddst = d3;                                           \
	vis_pst_32(dcntr0, ddst, mask)

/*
 * IMPLEMENTATION
 *
 * If length < 16 elements, use C - version (implementation by loop
 * in terms of source format without VIS - instructions).
 *
 * For large lengths we try to take advantage of VIS.
 *
 */

mlib_status
__mlib_VectorConjRev_S8C_S8C_Sat(
	mlib_s8 *zz,
	const mlib_s8 *xx,
	mlib_s32 n)
{
	const mlib_s8 *x = xx;
	mlib_s8 *z = zz;
	mlib_s8 *src = (mlib_s8 *)x, *dst = z + 2 * (n);
	mlib_d64 *dsrc, *ddst;
	mlib_d64 d1, d2, d3, d4, dl, dh, d_rest;
	mlib_d64 dcntr0 = vis_to_double_dup(0x00800080);
	mlib_d64 dxor0 = vis_to_double_dup(0x007f007f);
	mlib_d64 done = vis_to_double_dup(1);
	mlib_s8 c;
	mlib_s32 i, rest_64, len_64, even_length, odd = 0, length =
		(mlib_s32)n * 2;
	mlib_s32 re_part;
	mlib_f32 f_null = vis_to_float(0);

	CHECK(x, z);

	if (n < 8) {
		CONJREVC(mlib_s8,
			MLIB_S8_MAX,
			MLIB_S8_MIN);
	}

	while (((mlib_addr)dst) & 7) {

		if ((c = src[1]) == MLIB_S8_MIN)
			*--dst = MLIB_S8_MAX;
		else
			*--dst = -c;
		length -= 2;
		src += 2;

		if (((mlib_addr)dst) & 7) {
			*--dst = src[-2];
		} else {
			re_part = src[-2];
			odd = 1;
			break;
		}
	}

	vis_write_gsr(7 << 3);
	vis_write_bmask(0x67452301, 0);

	ddst = (mlib_d64 *)dst;
	rest_64 = length & 7;
	len_64 = length >> 3;
	even_length = len_64 << 3;

	if (!odd) {

/*
 * Aligning loop finished with imaginary part. The following processing
 * starts with real part.
 */

		if (!((mlib_addr)src & 7)) {

/*
 * Src address is 8-byte aligned.
 */

			dsrc = (mlib_d64 *)src;

#pragma pipeloop(0)
			for (i = 0; i < len_64; i++) {
				d3 = (*dsrc++);
				CONJ8;
				*--ddst = d4;
			}
		} else {

			dsrc = (mlib_d64 *)vis_alignaddr(src, 0);
			d2 = (*dsrc++);

#pragma pipeloop(0)
			for (i = 0; i < len_64; i++) {
				d1 = d2;
				d2 = (*dsrc++);
				d3 = vis_faligndata(d1, d2);
				CONJ8;
				*--ddst = d4;
			}
		}
	} else {

/*
 * Aligning loop finished with real part. Th following processing
 * starts with imaginary part.
 */

		if (!((mlib_addr)src & 7)) {

/*
 * Src address is 8-byte aligned.
 */

			dsrc = (mlib_d64 *)vis_alignaddr(src, 1);
			d_rest = vis_to_double((re_part << 24), 0);

#pragma pipeloop(0)
			for (i = 0; i < len_64; i++) {
				d3 = (*dsrc++);
				CONJ8;
				*--ddst = vis_faligndata(d4, d_rest);
				d_rest = d4;
			}

			ddst--;
			d_rest = vis_faligndata(d_rest, d_rest);
			vis_pst_8(d_rest, ddst, 0x1);
		} else {

			dsrc = (mlib_d64 *)vis_alignaddr(src, 0);
			d2 = (*dsrc++);

#pragma pipeloop(0)
			for (i = 0; i < len_64; i++) {
				d1 = d2;
				d2 = (*dsrc++);
				d3 = vis_faligndata(d1, d2);
				CONJ8;
				*--ddst = d4;
			}

			vis_write_gsr(1);
			d2 = *ddst;
			d3 = vis_faligndata(d1, d2);
			vis_pst_8(d3, (ddst - 1), 0x1);

#pragma pipeloop(0)
			for (i = 0; i < len_64; i++) {
				d1 = d2;
				d2 = *(ddst + 1);
				(*ddst++) = vis_faligndata(d1, d2);
			}

			dst[-1] = re_part;
		}

		dst--;
	}

	if (!rest_64)
		return (MLIB_SUCCESS);

	for (i = 0; i < rest_64; i += 2) {
		dst[-even_length - 2 - i] = src[even_length + i];

		if ((c = src[even_length + i + 1]) == MLIB_S8_MIN)
			dst[-even_length - 2 - i + 1] = MLIB_S8_MAX;
		else
			dst[-even_length - 2 - i + 1] = -c;
	}

	return (MLIB_SUCCESS);
}

/*
 * IMPLEMENTATION
 *
 * If length < 16 elements, use C - version (implementation by loop
 * in terms of source format without VIS - instructions).
 *
 * For large lengths we try to take advantage of VIS.
 *
 *
 */

mlib_status
__mlib_VectorConjRev_S16C_S16C_Sat(
	mlib_s16 *zz,
	const mlib_s16 *xx,
	mlib_s32 n)
{
	mlib_s16 *x = (mlib_s16 *)xx, *z = (mlib_s16 *)zz;
	mlib_s16 *src = (mlib_s16 *)x, *dst = (mlib_s16 *)&z[2 * n];
	mlib_d64 *dsrc, *ddst;
	mlib_d64 d1, d2, d3, d4, dl, dh, d_rest;
	mlib_d64 dlog0 = vis_to_double_dup(0x0000ffff), dtwo =
		vis_to_double(0, 2);
	mlib_f32 f_two = vis_to_float(0x20002);
	mlib_s16 c;
	mlib_s32 i, rest_64, len_64, even_length, odd = 0, length =
		(mlib_s32)n * 2;
	mlib_s32 re_part;

	CHECK(x, z);

	if ((n < 16)) {
		CONJREVC(mlib_s16,
			MLIB_S16_MAX,
			MLIB_S16_MIN);
	}

	while (((mlib_addr)dst) & 7) {

		if ((c = src[1]) == MLIB_S16_MIN)
			*--dst = MLIB_S16_MAX;
		else
			*--dst = -c;
		length -= 2;
		src += 2;

		if (((mlib_addr)dst) & 7) {
			*--dst = src[-2];
		} else {
			re_part = src[-2];
			odd = 1;
			break;
		}
	}

	vis_write_gsr(15 << 3);
	ddst = (mlib_d64 *)dst;
	rest_64 = length & 3;
	len_64 = length >> 2;
	even_length = len_64 << 2;

	if (!odd) {

/*
 * Aligning loop finished with imaginary part. The following processing
 * starts with real part.
 */

		if (!((mlib_addr)src & 7)) {

/*
 * Src address is 8-byte aligned.
 */

			dsrc = (mlib_d64 *)src;

#pragma pipeloop(0)
			for (i = 0; i < len_64; i++) {
				d3 = (*dsrc++);
				CONJ16;
				*--ddst = d4;
			}
		} else {

			dsrc = (mlib_d64 *)vis_alignaddr(src, 0);
			d2 = (*dsrc++);

#pragma pipeloop(0)
			for (i = 0; i < len_64; i++) {
				d1 = d2;
				d2 = (*dsrc++);
				d3 = vis_faligndata(d1, d2);
				CONJ16;
				*--ddst = d4;
			}
		}
	} else {

/*
 * Aligning loop finished with real part. Th following processing
 * starts with imaginary part.
 */

		if (!((mlib_addr)src & 7)) {

/*
 * Src address is 8-byte aligned.
 */

			dsrc = (mlib_d64 *)vis_alignaddr(src, 2);
			d_rest = vis_to_double((re_part << 16), 0);

#pragma pipeloop(0)
			for (i = 0; i < len_64; i++) {
				d3 = (*dsrc++);
				CONJ16;
				*--ddst = vis_faligndata(d4, d_rest);
				d_rest = d4;
			}

			ddst--;
			d_rest = vis_faligndata(d_rest, d_rest);
			vis_pst_16(d_rest, ddst, 0x1);
		} else {

			dsrc = (mlib_d64 *)vis_alignaddr(src, 0);
			d2 = (*dsrc++);

#pragma pipeloop(0)
			for (i = 0; i < len_64; i++) {
				d1 = d2;
				d2 = (*dsrc++);
				d3 = vis_faligndata(d1, d2);
				CONJ16;
				*--ddst = d4;
			}

			vis_write_gsr(2);
			d2 = *ddst;
			d3 = vis_faligndata(d1, d2);
			vis_pst_16(d3, (ddst - 1), 0x1);

#pragma pipeloop(0)
			for (i = 0; i < len_64; i++) {
				d1 = d2;
				d2 = *(ddst + 1);
				(*ddst++) = vis_faligndata(d1, d2);
			}

			dst[-1] = re_part;
		}

		dst--;
	}

	if (!rest_64)
		return (MLIB_SUCCESS);

	for (i = 0; i < rest_64; i += 2) {
		dst[-even_length - 2 - i] = src[even_length + i];

		if ((c = src[even_length + i + 1]) == MLIB_S16_MIN)
			dst[-even_length - 2 - i + 1] = MLIB_S16_MAX;
		else
			dst[-even_length - 2 - i + 1] = -c;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorConjRev_S32C_S32C_Sat(
	mlib_s32 *zz,
	const mlib_s32 *xx,
	mlib_s32 n)
{
	mlib_s32 *x = (mlib_s32 *)xx, *z = (mlib_s32 *)zz;
	mlib_s32 *src = (mlib_s32 *)x, *dst = (mlib_s32 *)&z[2 * n];
	mlib_d64 d1, d2, d3, d4, *ddst, *dsrc, dlog0 =
		vis_to_double(0, 0xFFFFFFFF);
	mlib_d64 dcntr0 = vis_to_double(0, 0x7FFFFFFF);
	mlib_d64 d_one = vis_to_double(0, 1);
	mlib_s32 i, len_64, length = (mlib_s32)n * 2, mask;

	CHECK(x, z);

	if (!((mlib_addr)dst & 7)) {
		dsrc = (mlib_d64 *)vis_alignaddr(src, 0);
		ddst = (mlib_d64 *)dst;
		len_64 = length >> 1;

		d2 = (*dsrc++);

#pragma pipeloop(0)
		for (i = 0; i < len_64 - 1; i++) {
			d1 = d2;
			d2 = *dsrc;
			d3 = vis_faligndata(d1, d2);
			CONJ32;
			dsrc++;
		}

		if (!((mlib_addr)src & 7)) {
			d3 = d2;
		} else {
			d1 = d2;
			d2 = *dsrc;
			d3 = vis_faligndata(d1, d2);
		}

		CONJ32;
	} else {
		CONJREVC(mlib_s32,
			MLIB_S32_MAX,
			MLIB_S32_MIN);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
