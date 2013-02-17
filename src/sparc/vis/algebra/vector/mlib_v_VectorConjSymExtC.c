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

#pragma ident	"@(#)mlib_v_VectorConjSymExtC.c	9.6	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorConjSymExt_S8C_S8C_Sat - computation of complex conjugated
 *                                          extended vector having
 *                                          8-bit format
 *      mlib_VectorConjSymExt_S16C_S16C_Sat - computation of complex conjugated
 *                                            extended vector having
 *                                            16-bit format
 *      mlib_VectorConjSymExt_S32C_S32C_Sat - computation of complex conjugated
 *                                            extended vector having
 *                                            32-bit format
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorConjSymExt_S16C_S16C_Sat(mlib_s16       *zz,
 *                                                      const mlib_s16 *xx,
 *                                                      mlib_s32       n);
 *      mlib_status mlib_VectorConjSymExt_S32C_S32C_Sat(mlib_s32       *zz,
 *                                                      const mlib_s32 *xx,
 *                                                      mlib_s32       n);
 *      mlib_status mlib_VectorConjSymExt_S8C_S8C_Sat(mlib_s8       *zz,
 *                                                    const mlib_s8 *xx,
 *                                                    mlib_s32      n);
 *
 * ARGUMENTS
 *      z    pointer to the first complex element of the result vector,
 *           which has 2*n complex elements when n is iven, and 2*n - 1
 *           complex elements when n is odd. z[2*i] hold the real parts,
 *           and z[2*i + 1] hold the imaginary parts
 *      x    pointer to the first complex element of the input vector.
 *           x[2*i] hold the real parts, and x[2*i + 1] hold the imaginary
 *           parts
 *      n    number of complex elements in the input vector
 *
 * DESCRIPTION
 *              *    *        *    *      *        *
 *          ( x[1] x[2] ... x[n] x[n] x[n - 1] ... x[1] )  n - even
 *     z =      *    *        *      *        *
 *          ( x[1] x[2] ... x[n] x[n - 1] ... x[1] )       n - odd
 */

#include <mlib_algebra.h>
#include <mlib_VectorConjSymExt.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorConjSymExt_S16C_S16C_Sat = \
	__mlib_VectorConjSymExt_S16C_S16C_Sat
#pragma weak mlib_VectorConjSymExt_S8C_S8C_Sat = \
	__mlib_VectorConjSymExt_S8C_S8C_Sat
#pragma weak mlib_VectorConjSymExt_S32C_S32C_Sat = \
	__mlib_VectorConjSymExt_S32C_S32C_Sat

#elif defined(__GNUC__)

__typeof__(__mlib_VectorConjSymExt_S16C_S16C_Sat)
	mlib_VectorConjSymExt_S16C_S16C_Sat
	__attribute__((weak, alias("__mlib_VectorConjSymExt_S16C_S16C_Sat")));
__typeof__(__mlib_VectorConjSymExt_S8C_S8C_Sat)
	mlib_VectorConjSymExt_S8C_S8C_Sat
	__attribute__((weak, alias("__mlib_VectorConjSymExt_S8C_S8C_Sat")));
__typeof__(__mlib_VectorConjSymExt_S32C_S32C_Sat)
	mlib_VectorConjSymExt_S32C_S32C_Sat
	__attribute__((weak, alias("__mlib_VectorConjSymExt_S32C_S32C_Sat")));

#else

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	CHECK(x, z)                                             \
	if ((x == NULL) || (z == NULL) || (n <= 0))             \
	    return MLIB_FAILURE

/* *********************************************************** */

#define	CONJSYMEXTC(form, MAX, MIN)                               \
	mlib_s32 i;                                               \
	form im;                                                  \
	                                                          \
	for (i = 0; i < (mlib_s32)n; i++) {                       \
	    z[2 * i] = z[n1 - 2 - 2 * i] = x[2 * i];              \
	    im = -x[2 * i + 1];                                   \
	    if (im == MIN)                                        \
		im = MAX;                                               \
	    z[2 * i + 1] = z[n1 - 1 - 2 * i] = im;                \
	}                                                         \
	                                                          \
	return MLIB_SUCCESS

/* *********************************************************** */

#define	CH_BYTES_ORDER                                          \
	d3 = vis_fpmerge(vis_read_lo(d11), vis_read_hi(d11));   \
	d3 = vis_fpmerge(vis_read_lo(d3), vis_read_hi(d3));     \
	d3 = vis_fpmerge(vis_read_hi(d3), vis_read_lo(d3))

/* *********************************************************** */

#define	CONJ8                                                             \
	d4 = vis_fxor(d1, dxor0);                                         \
	dh = vis_fpmerge(f_null, vis_read_hi(d4));                        \
	dl = vis_fpmerge(f_null, vis_read_lo(d4));                        \
	d4 =                                                              \
	vis_fpack16_pair(vis_fpadd16(dh, done), vis_fpadd16(dl, done));   \
	d11 = vis_fxor(d4, dcntr0);                                       \
	*ddst = d11;                                                      \
	ddst++

/* *********************************************************** */

#define	CONJ16                                                             \
	d4 = vis_fxor(d3, dlog0);                                          \
	dh = vis_fmuld8ulx16(f_two, vis_read_hi(d4));                      \
	dl = vis_fmuld8ulx16(f_two, vis_read_lo(d4));                      \
	d3 =                                                               \
	vis_fpackfix_pair(vis_fpadd32(dh, dtwo), vis_fpadd32(dl, dtwo));   \
	(*ddst++) = d3;                                                    \
	d1 = vis_faligndata(d3, d3)

/* *********************************************************** */

#define	REV16                                                   \
	d4 = d3;                                                \
	dh = vis_fmuld8ulx16(f_two, vis_read_hi(d4));           \
	dl = vis_fmuld8ulx16(f_two, vis_read_lo(d4));           \
	d4 = vis_fpackfix_pair(dl, dh)

/*
 * IMPLEMENTATION
 *
 * If length < 16 elements, use C - version (implementation by loop
 * in terms of source format without VIS - instructions).
 *
 * For large lengths we try to take advantage of VIS.
 *
 * First, we select particular case when src and dst vectors are 8-byte
 * aligned and n&3 == 0. This ensures that &z[n] is also 8-byte aligned.
 * So we form 64-bit object contained 4 conjected values and store it
 * from the beginning of the vector. In the same iteration we change
 * the order of values (12 - 34 - 56 - 78 to 78 - 56 - 34 - 12)
 * and store it from the end of the resulting vector.
 *
 * In case of arbitrary alignment we use mlib_VectorConj_S8C_S8C_Sat and
 * then make copying first half of the dst vector to the second one
 * with conjunction. Here we distinguish all possible cases of src
 * and dst alignments.
 *
 */

mlib_status
mlib_VectorConjSymExt_S8C_S8C_Sat_N(
	mlib_s8 *zz,
	const mlib_s8 *xx,
	mlib_s32 n,
	mlib_s32 n1)
{
	const mlib_s8 *x = xx;
	mlib_s8 *z = zz;
	mlib_s8 *src = (mlib_s8 *)x, *dst = (mlib_s8 *)z;
	mlib_d64 *dsrc, *ddst, *ddst_end;
	mlib_d64 d1, d2, d3, d4, d11, dh, dl;
	mlib_d64 dcntr0 = vis_to_double_dup(0x00800080);
	mlib_d64 done = vis_to_double_dup(1);
	mlib_d64 dxor0 = vis_to_double_dup(0x007f007f);
	mlib_u8 c;
	mlib_s32 i, len_64, length = n << 1, even_length, rest_64;

/*
 * odd==1 means dst - aligning loop finished with real part.
 */
	mlib_s32 odd = 0;
	mlib_f32 f_null = vis_to_float(0);

	CHECK(x, z);

	n1 *= 2;

	vis_write_gsr(7 << 3);

	if (n < 8) {
		CONJSYMEXTC(mlib_s8,
			MLIB_S8_MAX,
			MLIB_S8_MIN);
	}

	if ((!((mlib_addr)src & 7)) && (!((mlib_addr)dst & 7)) &&
				(!(n & 3)) && (!(n1 & 7))) {

/*
 * Particular and important case when we can
 * read and write information by 2 word (sending it to 2 places:
 * to the begin and to the end of destination vector).
 */

		dsrc = (mlib_d64 *)src;
		ddst = (mlib_d64 *)dst;
		ddst_end = (mlib_d64 *)&z[n1];
		len_64 = n >> 2;

		d1 = (*dsrc++);
		CONJ8;
		CH_BYTES_ORDER;

#pragma pipeloop(0)
		for (i = 1; i < len_64; i++) {
			*--ddst_end = d3;
			d1 = (*dsrc++);
			CONJ8;
			CH_BYTES_ORDER;
		}

		*--ddst_end = d3;
	} else {
/*
 * This is very 'bad' case: src and dst vectors are arbitrary aligned,
 * and complex value in memory may be allocated in any way.
 * First, make Conj_S8C_S8C. Then make second pass along our array.
 */

		__mlib_VectorConj_S8C_S8C_Sat(zz, xx, n);

		src = (mlib_s8 *)zz;
		dst = &src[n1];

/*
 * Start copying array with destination address aligning.
 */

		while (((mlib_addr)dst) & 7) {
			c = src[0];
			*--dst = src[1];
			src += 2;
			length -= 2;

			if (!(((mlib_addr)dst) & 7)) {
				odd = 1;
				break;
			}

			*--dst = c;
		}

		ddst = (mlib_d64 *)dst;
		len_64 = length >> 3;
		even_length = len_64 << 3;
		rest_64 = length & 7;

		if (!odd) {

/*
 * Aligning loop finished with imaginary part. Start coping
 * with real part.
 */

			if (!((mlib_addr)src & 7)) {
/*
 * Src vector is 8-byte aligned.
 */
				dsrc = (mlib_d64 *)src;

#pragma pipeloop(0)
				for (i = 0; i < len_64; i++) {
					d11 = (*dsrc++);
					CH_BYTES_ORDER;
					*--ddst = d3;
				}
			} else {
/*
 * Src vector is arbitrary aligned.
 */
				dsrc = vis_alignaddr(src, 0);
				d2 = (*dsrc++);

#pragma pipeloop(0)
				for (i = 0; i < len_64; i++) {
					d1 = d2;
					d2 = (*dsrc++);
					d11 = vis_faligndata(d1, d2);
					CH_BYTES_ORDER;
					*--ddst = d3;
				}
			}
		} else {
/*
 * odd==1.
 * Aligning loop finished with real part.
 * Start coping with imaginary part.
 */

/*
 * Src vector is arbitrary aligned.
 */

			dsrc = (mlib_d64 *)vis_alignaddr(src, 0);
			d2 = (*dsrc++);

#pragma pipeloop(0)
			for (i = 0; i < len_64; i++) {
				d1 = d2;
				d2 = (*dsrc++);
				d11 = vis_faligndata(d1, d2);
				CH_BYTES_ORDER;
				*--ddst = d3;
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

			*--dst = c;
		}

		if (!rest_64)
			return (MLIB_SUCCESS);

		for (i = 0; i < rest_64; i += 2) {
			dst[-even_length - 2 - i] = src[even_length + i];
			dst[-even_length - 2 - i + 1] =
				src[even_length + i + 1];
		}

	}	/* From first aligning analysis. */
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
 * First, we select particular case when src and dst vectors are 8-byte
 * aligned and n&1 == 0. This ensures that &z[n] is also 8-byte aligned.
 * So we form 64-bit object contained 2 conjected values and store it
 * from the beginning of the vector. In the same iteration we change
 * the order of values (1234 - 5678 to 5678 - 1234) and store it from
 * the end of the resulting vector.
 *
 * In case of arbitrary alignment we use mlib_VectorConj_S16C_S16C_Sat, and
 * then make copying first half of the dst vector to the second one
 * with conjunction. Here we distinguish all possible cases of src
 * and dst alignments.
 *
 */

mlib_status
mlib_VectorConjSymExt_S16C_S16C_Sat_N(
	mlib_s16 *zz,
	const mlib_s16 *xx,
	mlib_s32 n,
	mlib_s32 n1)
{
	const mlib_s16 *x = xx;
	mlib_s16 *z = zz;
	mlib_s16 *src = (mlib_s16 *)x, *dst = (mlib_s16 *)z;
	mlib_d64 *dsrc, *ddst, *ddst_end;
	mlib_d64 d1, d2, d3, d4, dh, dl;
	mlib_d64 dlog0 = vis_to_double_dup(0x0000ffff), dtwo =
		vis_to_double(0, 2);
	mlib_s32 i, length = (mlib_s32)n * 2, len_64, rest_64, even_length;

/*
 * odd==1 means dst - aligning loop finished with real part.
 */
	mlib_s32 odd = 0;
	mlib_s16 c;
	mlib_f32 f_two = vis_to_float(0x20002);

	CHECK(x, z);

	n1 *= 2;

	vis_write_gsr((15 << 3) + 4);

	if (n < 16) {
		CONJSYMEXTC(mlib_s16,
			MLIB_S16_MAX,
			MLIB_S16_MIN);
	}

	if ((!((mlib_addr)src & 7)) && (!((mlib_addr)dst & 7)) &&
				(!(n & 1)) && (!(n1 & 3))) {

/*
 * Particular and important case when we can
 * read and write information by 2 word (sending it to 2 places:
 * to the begin and to the end of destination vector).
 */

		dsrc = (mlib_d64 *)src;
		ddst = (mlib_d64 *)dst;
		ddst_end = (mlib_d64 *)&z[n1];
		len_64 = n >> 1;

		d3 = (*dsrc++);
		CONJ16;

#pragma pipeloop(0)
		for (i = 1; i < len_64; i++) {
			*--ddst_end = d1;
			d3 = (*dsrc++);
			CONJ16;
		}

		*--ddst_end = d1;
	} else {
/*
 * This is very 'bad' case: src and dst vectors are arbitrary aligned,
 * and complex value in memory may be allocated in any way.
 * First, make Conj_S16C_S16C. Then make second walk along our array.
 */

		__mlib_VectorConj_S16C_S16C_Sat(zz, xx, n);

		src = (mlib_s16 *)zz;
		dst = &src[n1];

/*
 * Start copying array with destination address aligning.
 */

		while (((mlib_addr)dst) & 7) {
			c = src[0];
			*--dst = src[1];
			src += 2;
			length -= 2;

			if (!(((mlib_addr)dst) & 7)) {
				odd = 1;
				break;
			}

			*--dst = c;
		}

		ddst = (mlib_d64 *)dst;
		len_64 = length >> 2;
		even_length = len_64 << 2;
		rest_64 = length & 3;

		if (!odd) {

/*
 * Aligning loop finished with imaginary part. Start coping
 * with real part.
 */

			if (!((mlib_addr)src & 7)) {
/*
 * Src vector is 8-byte aligned.
 */
				dsrc = (mlib_d64 *)src;

#pragma pipeloop(0)
				for (i = 0; i < len_64; i++) {
					d3 = (*dsrc++);
					REV16;
					*--ddst = d4;
				}
			} else {
/*
 * Src vector is arbitrary aligned.
 */
				dsrc = vis_alignaddr(src, 0);
				d2 = (*dsrc++);

#pragma pipeloop(0)
				for (i = 0; i < len_64; i++) {
					d1 = d2;
					d2 = (*dsrc++);
					d3 = vis_faligndata(d1, d2);
					REV16;
					*--ddst = d4;
				}
			}
		} else {
/*
 * odd==1.
 * Aligning loop finished with real part. Start coping
 * with imaginary part.
 */

/*
 * Src vector is arbitrary aligned.
 */

			dsrc = (mlib_d64 *)vis_alignaddr(src, 0);
			d2 = (*dsrc++);

#pragma pipeloop(0)
			for (i = 0; i < len_64; i++) {
				d1 = d2;
				d2 = (*dsrc++);
				d3 = vis_faligndata(d1, d2);
				REV16;
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

			*--dst = c;
		}

		if (!rest_64)
			return (MLIB_SUCCESS);

		for (i = 0; i < rest_64; i += 2) {
			dst[-even_length - 2 - i] = src[even_length + i];
			dst[-even_length - 2 - i + 1] =
				src[even_length + i + 1];
		}

	}	/* From first aligning analysis. */
	return (MLIB_SUCCESS);
}

/*
 * IMPLEMENTATION
 *
 * If length < 16 elements, use C - version (implementation by loop
 * in terms of source format without VIS - instructions).
 *
 * For large lengths we try to .
 *
 * First, we select particular case when src and dst vectors are 8-byte aligned.
 * This ensures that &z[n] is 8-byte aligned.
 * So we form 64-bit object contained complex value and store it
 * from the beginning of the vector. In the same iteration we change
 * the sign if image part and store it from
 * the end of the resulting vector.
 *
 * In case of arbitrary alignment we use mlib_VectorConj_S32C_S32C_Sat and
 * mlib_VectorConjRev_S32C_S32C_Sat correspondingly.
 *
 */

mlib_status
mlib_VectorConjSymExt_S32C_S32C_Sat_N(
	mlib_s32 *zz,
	const mlib_s32 *xx,
	mlib_s32 n,
	mlib_s32 n1)
{
	const mlib_s32 *x = xx;
	mlib_s32 *z = zz;
	mlib_s32 *src = (mlib_s32 *)x, *dst = (mlib_s32 *)z;
	mlib_s32 *dst_end = (mlib_s32 *)&z[n1 * 2];
	mlib_s32 c, c1;
	mlib_s32 i;

	CHECK(x, z);

	for (i = 0; i < n * 2; i += 2) {
		dst[i] = (c1 = src[i]);

		if ((c = src[i + 1]) == MLIB_S32_MIN) {
			dst[i + 1] = MLIB_S32_MAX;
			dst_end[-1] = MLIB_S32_MAX;
		} else {
			dst[i + 1] = -c;
			dst_end[-1] = -c;
		}

		dst_end[-2] = c1;
		dst_end -= 2;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status __mlib_VectorConjSymExt_S8C_S8C_Sat(
    mlib_s8 *z,
    const mlib_s8 *x,
    mlib_s32 n)
{
	return mlib_VectorConjSymExt_S8C_S8C_Sat_N(z, x, n,
		(n * 2 - (n & 1)));
}

mlib_status __mlib_VectorConjSymExt_S16C_S16C_Sat(
    mlib_s16 *z,
    const mlib_s16 *x,
    mlib_s32 n)
{
	return mlib_VectorConjSymExt_S16C_S16C_Sat_N(z, x, n,
		(n * 2 - (n & 1)));
}

mlib_status __mlib_VectorConjSymExt_S32C_S32C_Sat(
    mlib_s32 *z,
    const mlib_s32 *x,
    mlib_s32 n)
{
	return mlib_VectorConjSymExt_S32C_S32C_Sat_N(z, x, n,
		(n * 2 - (n & 1)));
}

/* *********************************************************** */
