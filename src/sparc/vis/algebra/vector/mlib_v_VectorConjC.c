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

#pragma ident	"@(#)mlib_v_VectorConjC.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorConj_S8C_S8C_Sat,
 *      mlib_VectorConj_S8C_Sat      - computation of 8-bit format
 *                                     complex conjugated vector
 *      mlib_VectorConj_S16C_S16C_Sat,
 *      mlib_VectorConj_S16C_Sat      - computation of 16-bit format
 *                                      complex conjugated vector
 *      mlib_VectorConj_S32C_S32C_Sat,
 *      mlib_VectorConj_S32C_Sat      - computation of 32-bit format
 *                                      complex conjugated vector
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorConj_S8C_S8C_Sat(mlib_s8       *z,
 *                                              const mlib_s8 *x,
 *                                              mlib_s32      n);
 *      mlib_status mlib_VectorConj_S8C_Sat(mlib_s8  *xz,
 *                                          mlib_s32 n);
 *      mlib_status mlib_VectorConj_S16C_S16C_Sat(mlib_s16       *z,
 *                                                const mlib_s16 *x,
 *                                                mlib_s32       n);
 *      mlib_status mlib_VectorConj_S16C_Sat(mlib_s16 *xz,
 *                                           mlib_s32 n);
 *      mlib_status mlib_VectorConj_S32C_S32C_Sat(mlib_s32       *z,
 *                                                const mlib_s32 *x,
 *                                                mlib_s32       n);
 *      mlib_status mlib_VectorConj_S32C_Sat(mlib_s32 *xz,
 *                                           mlib_s32 n);
 *
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
 *           *
 *      z = x
 */

#include <mlib_algebra.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorConj_S32C_Sat = __mlib_VectorConj_S32C_Sat
#pragma weak mlib_VectorConj_S8C_S8C_Sat = __mlib_VectorConj_S8C_S8C_Sat
#pragma weak mlib_VectorConj_S32C_S32C_Sat = __mlib_VectorConj_S32C_S32C_Sat
#pragma weak mlib_VectorConj_S16C_Sat = __mlib_VectorConj_S16C_Sat
#pragma weak mlib_VectorConj_S8C_Sat = __mlib_VectorConj_S8C_Sat
#pragma weak mlib_VectorConj_S16C_S16C_Sat = __mlib_VectorConj_S16C_S16C_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_VectorConj_S32C_Sat) mlib_VectorConj_S32C_Sat
	__attribute__((weak, alias("__mlib_VectorConj_S32C_Sat")));
__typeof__(__mlib_VectorConj_S8C_S8C_Sat) mlib_VectorConj_S8C_S8C_Sat
	__attribute__((weak, alias("__mlib_VectorConj_S8C_S8C_Sat")));
__typeof__(__mlib_VectorConj_S32C_S32C_Sat) mlib_VectorConj_S32C_S32C_Sat
	__attribute__((weak, alias("__mlib_VectorConj_S32C_S32C_Sat")));
__typeof__(__mlib_VectorConj_S16C_Sat) mlib_VectorConj_S16C_Sat
	__attribute__((weak, alias("__mlib_VectorConj_S16C_Sat")));
__typeof__(__mlib_VectorConj_S8C_Sat) mlib_VectorConj_S8C_Sat
	__attribute__((weak, alias("__mlib_VectorConj_S8C_Sat")));
__typeof__(__mlib_VectorConj_S16C_S16C_Sat) mlib_VectorConj_S16C_S16C_Sat
	__attribute__((weak, alias("__mlib_VectorConj_S16C_S16C_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	CHECK(x, z)                                             \
	if ((x == NULL) || (z == NULL) || (n <= 0))             \
	    return MLIB_FAILURE

/* *********************************************************** */

#define	CONTROL(MAX, MIN, obj)	(((c = obj) == MIN) ? MAX : -c)

/* *********************************************************** */

#define	CONJC(form, MAX, MIN)                                   \
	mlib_s32 i;                                             \
	                                                        \
	if (i = (n & 1) * 2) {                                  \
	    dst[0] = src[0];                                    \
	    dst[1] = CONTROL(MAX, MIN, src[1]);                 \
	}                                                       \
	for (; i < (mlib_s32)n * 2; ) {                          \
	    dst[i] = src[i++];                                  \
	    dst[i] = CONTROL(MAX, MIN, src[i]);                 \
	    i++;                                                \
	    dst[i] = src[i++];                                  \
	    dst[i] = CONTROL(MAX, MIN, src[i]);                 \
	    i++;                                                \
	}                                                       \
	                                                        \
	return MLIB_SUCCESS

/* *********************************************************** */

#define	CONJ8                                                             \
	d4 = vis_fxor(d3, dxor0);                                         \
	dh = vis_fpmerge(f_null, vis_read_hi(d4));                        \
	dl = vis_fpmerge(f_null, vis_read_lo(d4));                        \
	d4 =                                                              \
	vis_fpack16_pair(vis_fpadd16(dh, done), vis_fpadd16(dl, done));   \
	d3 = vis_fxor(d4, dcntr0);                                        \
	*ddst = d3;                                                       \
	ddst++

/* *********************************************************** */

#define	CONJ8_ODD                                                  \
	d4 = vis_fxor(d3, dxor1);                                  \
	dh = vis_fpmerge(f_null, vis_read_hi(d4));                 \
	dl = vis_fpmerge(f_null, vis_read_lo(d4));                 \
	d4 =                                                       \
	vis_fpack16_pair(vis_fpadd16(dh, done1), vis_fpadd16(dl,   \
		done1));                                           \
	d3 = vis_fxor(d4, dcntr1);                                 \
	*ddst = d3;                                                \
	ddst++

/* *********************************************************** */

#define	CONJ16                                                             \
	d4 = vis_fxor(d3, dlog0);                                          \
	dh = vis_fmuld8ulx16(f_two, vis_read_hi(d4));                      \
	dl = vis_fmuld8ulx16(f_two, vis_read_lo(d4));                      \
	d3 =                                                               \
	vis_fpackfix_pair(vis_fpadd32(dh, dtwo), vis_fpadd32(dl, dtwo));   \
	*ddst = d3;                                                        \
	ddst++

/* *********************************************************** */

#define	CONJ16_ODD                                                  \
	d4 = vis_fxor(d3, dlog1);                                   \
	dh = vis_fmuld8ulx16(f_two, vis_read_hi(d4));               \
	dl = vis_fmuld8ulx16(f_two, vis_read_lo(d4));               \
	d3 =                                                        \
	vis_fpackfix_pair(vis_fpadd32(dh, dtwo1), vis_fpadd32(dl,   \
		dtwo1));                                            \
	*ddst = d3;                                                 \
	ddst++

/* *********************************************************** */

#define	CONJ32                                                  \
	d4 = vis_fxor(d3, dlog0);                               \
	mask = vis_fcmpeq32(d4, dcntr0) & 1;                    \
	d3 = vis_fpadd32(d_one, d4);                            \
	*ddst = d3;                                             \
	vis_pst_32(dcntr0, ddst, mask);                         \
	ddst++

/* *********************************************************** */

#define	CONJ32_ODD                                              \
	d4 = vis_fxor(d3, dlog1);                               \
	mask = vis_fcmpeq32(d4, dcntr1) & 2;                    \
	d3 = vis_fpadd32(d_one1, d4);                           \
	*ddst = d3;                                             \
	vis_pst_32(dcntr1, ddst, mask);                         \
	ddst++;

/*
 *
 * IMPLEMENTATION
 *
 * If length < 16 elements, use C - version (implementation by loop
 * in terms of source format without VIS - instructions).
 *
 * For large lengths we try to take advantage of VIS.
 *
 * First, destination address is aligned by 8 bytes (dst&7 == 0).
 *
 * Then,  we read src by 2 words with the help of  vis_alignaddr and
 * vis_faligndata, change the sign of each second byte (by vis_fpsub16)
 * and  store the result by 2 words.
 *
 * Depending on aligning of src and dst vectors, there are 2 different
 * variants: ((mlib_s32)dst - (mlib_s32)src)&1 == 0 (odd = 0) and
 *           ((mlib_s32)dst - (mlib_s32)src)&1 == 1 (odd = 1).
 * In first case loading value begins with real part,
 * in second - with imaginary part. So sign inversion is implemented
 * in different ways.
 *
 *
 */

mlib_status
__mlib_VectorConj_S8C_S8C_Sat(
	mlib_s8 *z,
	const mlib_s8 *x,
	mlib_s32 n)
{
	mlib_u8 *src = (mlib_u8 *)x, *dst = (mlib_u8 *)z;
	mlib_d64 *dsrc, *ddst;
	mlib_d64 d1, d2, d3, d4, dl, dh;
	mlib_d64 dcntr0 = vis_to_double_dup(0x00800080);
	mlib_d64 dcntr1 = vis_to_double_dup(0x80008000);
	mlib_d64 dxor0 = vis_to_double_dup(0x007f007f);
	mlib_d64 dxor1 = vis_to_double_dup(0x7f007f00);
	mlib_d64 done = vis_to_double_dup(1);
	mlib_d64 done1 = vis_to_double_dup(0x10000);
	mlib_s8 odd = 0, c;
	mlib_f32 f_null = vis_to_float(0);
	mlib_s32 i, rest_64, len_64, even_length, length = (mlib_s32)n * 2;

	CHECK(x, z);

	vis_write_gsr(7 << 3);

	if (length < 16) {
		CONJC(mlib_s8,
			MLIB_S8_MAX,
			MLIB_S8_MIN);
	} else {
		while ((mlib_addr)dst & 7) {
			(*dst++) = (*src++);
			length--;

			if ((mlib_addr)dst & 7) {
				(*dst++) =
					CONTROL(MLIB_S8_MAX, MLIB_S8_MIN,
					(*src++));
				length--;
			} else {
				odd = 1;
				break;
			}
		}

		dsrc = (mlib_d64 *)vis_alignaddr(src, 0);
		ddst = (mlib_d64 *)dst;
		rest_64 = length & 7;
		len_64 = length >> 3;
		even_length = len_64 << 3;

		if (!((mlib_addr)src & 7)) {

/*
 * Src and dst vectors have the same alignments.
 * No vis_faligndata at all.
 */

			if (!odd) {

#pragma pipeloop(0)
				for (i = 0; i < len_64; i++) {
					d3 = *dsrc;
					CONJ8;
					dsrc++;
				}
			} else {

#pragma pipeloop(0)
				for (i = 0; i < len_64; i++) {
					d3 = *dsrc;
					CONJ8_ODD;
					dsrc++;
				}
			}
		} else {

/*
 * Src and dst vectors have different alignments.
 * vis_faligndata for Src vector.
 */

			d2 = (*dsrc++);

			if (!odd) {

#pragma pipeloop(0)
				for (i = 0; i < len_64; i++) {
					d1 = d2;
					d2 = (*dsrc++);
					d3 = vis_faligndata(d1, d2);
					CONJ8;
				}
			} else {

#pragma pipeloop(0)
				for (i = 0; i < len_64; i++) {
					d1 = d2;
					d2 = (*dsrc++);
					d3 = vis_faligndata(d1, d2);
					CONJ8_ODD;
				}
			}
		}

		if (!rest_64)
			return (MLIB_SUCCESS);

		if (!odd)
			for (i = 0; i < rest_64; i += 2) {
				dst[even_length + i] = src[even_length + i];

				if ((c = src[even_length + i + 1]) ==
					MLIB_S8_MIN)
					dst[even_length + i + 1] = MLIB_S8_MAX;
				else
					dst[even_length + i + 1] = -c;
		} else
			for (i = 0; i < rest_64; i++) {
				if ((c = src[even_length + i]) == MLIB_S8_MIN)
					dst[even_length + i] = MLIB_S8_MAX;
				else
					dst[even_length + i] = -c;

				if (++i < rest_64)
					dst[even_length + i] =
						src[even_length + i];
				else
					break;
			}

		return (MLIB_SUCCESS);
	}
}

/* *********************************************************** */

mlib_status
__mlib_VectorConj_S8C_Sat(
	mlib_s8 *xz,
	mlib_s32 n)
{
	return (__mlib_VectorConj_S8C_S8C_Sat(xz, xz, n));
}

/*
 *
 * IMPLEMENTATION
 *
 * If length < 16 elements, use C - version (implementation by loop
 * in terms of source format without VIS - instructions).
 *
 * For large lengths we try to take advantage of VIS.
 *
 * First, destination address is aligned by 8 bytes (dst&7 == 0).
 *
 * Then,  we read src by 2 words with the help of  vis_alignaddr and
 * vis_faligndata, change the sign of each half (by vis_fpsub32)
 * and  store the result by 2 words.
 *
 * Depending on aligning of src and dst vectors, there are 2 different
 * variants: ((mlib_s32)dst - (mlib_s32)src)&1 == 4 (odd = 0) and
 *           ((mlib_s32)dst - (mlib_s32)src)&1 == 2 (odd = 1).
 * In first case loading value begins with real part,
 * in second - with imaginary part. So sign inversion is implemented
 * in different ways.
 *
 *
 */

mlib_status
__mlib_VectorConj_S16C_S16C_Sat(
	mlib_s16 *z,
	const mlib_s16 *x,
	mlib_s32 n)
{
	mlib_u16 *src = (mlib_u16 *)x, *dst = (mlib_u16 *)z;
	mlib_d64 *dsrc, *ddst;
	mlib_d64 d1, d2, d3, d4, dl, dh;
	mlib_d64 dlog0 = vis_to_double_dup(0x0000ffff);
	mlib_d64 dlog1 = vis_to_double_dup(0xffff0000);
	mlib_d64 dtwo = vis_to_double(0, 2);
	mlib_d64 dtwo1 = vis_to_double(2, 0);
	mlib_s32 odd = 0, i, rest_64, len_64, even_length, length =
		(mlib_s32)n * 2;
	mlib_f32 f_two = vis_to_float(0x20002);
	mlib_s16 c;

	CHECK(x, z);

	vis_write_gsr(15 << 3);

	if (length < 16) {
		CONJC(mlib_s16,
			MLIB_S16_MAX,
			MLIB_S16_MIN);
	} else {
		while ((mlib_addr)dst & 7) {
			(*dst++) = (*src++);
			length--;

			if ((mlib_addr)dst & 7) {
				(*dst++) =
					CONTROL(MLIB_S16_MAX, MLIB_S16_MIN,
					(*src++));
				length--;
			} else {
				odd = 1;
				break;
			}
		}

		dsrc = (mlib_d64 *)vis_alignaddr(src, 0);
		ddst = (mlib_d64 *)dst;
		rest_64 = length & 3;
		len_64 = length >> 2;
		even_length = len_64 << 2;

		if (!((mlib_addr)src & 7)) {

/*
 * Src and dst vectors have the same alignments.
 * No vis_faligndata at all.
 */

			if (!odd) {

#pragma pipeloop(0)
				for (i = 0; i < len_64; i++) {
					d3 = *dsrc;
					CONJ16;
					dsrc++;
				}
			} else {

#pragma pipeloop(0)
				for (i = 0; i < len_64; i++) {
					d3 = *dsrc;
					CONJ16_ODD;
					dsrc++;
				}
			}
		} else {

/*
 * Src and dst vectors have different alignments.
 * vis_faligndata for Src vector.
 */

			d2 = (*dsrc++);

			if (!odd) {

#pragma pipeloop(0)
				for (i = 0; i < len_64; i++) {
					d1 = d2;
					d2 = *dsrc;
					d3 = vis_faligndata(d1, d2);
					CONJ16;
					dsrc++;
				}
			} else {

#pragma pipeloop(0)
				for (i = 0; i < len_64; i++) {
					d1 = d2;
					d2 = *dsrc;
					d3 = vis_faligndata(d1, d2);
					CONJ16_ODD;
					dsrc++;
				}
			}
		}

		if (!rest_64)
			return (MLIB_SUCCESS);

		if (!odd)
			for (i = 0; i < rest_64; i += 2) {
				dst[even_length + i] = src[even_length + i];

				if ((c = src[even_length + i + 1]) ==
					MLIB_S16_MIN)
					dst[even_length + i + 1] = MLIB_S16_MAX;
				else
					dst[even_length + i + 1] = -c;
		} else
			for (i = 0; i < rest_64; i++) {
				if ((c = src[even_length + i]) == MLIB_S16_MIN)
					dst[even_length + i] = MLIB_S16_MAX;
				else
					dst[even_length + i] = -c;

				if (++i < rest_64)
					dst[even_length + i] =
						src[even_length + i];
				else
					break;
			}

		return (MLIB_SUCCESS);
	}
}

/* *********************************************************** */

mlib_status
__mlib_VectorConj_S16C_Sat(
	mlib_s16 *xz,
	mlib_s32 n)
{
	return (__mlib_VectorConj_S16C_S16C_Sat(xz, xz, n));
}

/* *********************************************************** */

/*
 * IMPLEMENTATION
 *
 * If length < 16 elements, we use simple loop implementation.
 *
 * For large lengths we try to take advantage of VIS.
 *
 * First, destination address is aligned by 8 bytes (dst&7 == 0).
 *
 * Then,  we read src by 2 words with the help of  vis_alignaddr and
 * vis_faligndata, change the sign of each half (by vis_fpsub32)
 * and  store the result by 2 words.
 *
 * Depending on aligning of src and dst vectors, there are 2 different
 * variants: ((mlib_s32)dst - (mlib_s32)src)&1 == 4 (odd = 0) and
 *           ((mlib_s32)dst - (mlib_s32)src)&1 == 2 (odd = 1).
 * In first case loading value begins with real part,
 * in second - with imaginary part. So sign inversion is implemented
 * in different ways.
 *
 */

/* *********************************************************** */

mlib_status
__mlib_VectorConj_S32C_S32C_Sat(
	mlib_s32 *z,
	const mlib_s32 *x,
	mlib_s32 n)
{
	mlib_u32 *src = (mlib_u32 *)x, *dst = (mlib_u32 *)z;
	mlib_d64 d1, d2, d3, d4, *ddst, *dsrc;
	mlib_d64 dlog0 = vis_to_double(0, 0xFFFFFFFF);
	mlib_d64 dlog1 = vis_to_double(0xFFFFFFFF, 0);
	mlib_d64 dcntr0 = vis_to_double(0, 0x7FFFFFFF);
	mlib_d64 dcntr1 = vis_to_double(0x7FFFFFFF, 0);
	mlib_d64 d_one = vis_to_double(0, 1);
	mlib_d64 d_one1 = vis_to_double(1, 0);
	mlib_s32 odd = 0, i, rest_64, len_64, even_length, length =
		(mlib_s32)n * 2, mask;
	mlib_s32 c;

	CHECK(x, z);

	if (length < 16) {
		CONJC(mlib_s32,
			MLIB_S32_MAX,
			MLIB_S32_MIN);
	} else {
		if ((mlib_addr)dst & 7) {
			(*dst++) = (*src++);
			length--;
			odd = 1;
		}

		dsrc = (mlib_d64 *)vis_alignaddr(src, 0);
		ddst = (mlib_d64 *)dst;
		rest_64 = length & 1;
		len_64 = length >> 1;
		even_length = len_64 << 1;

		d2 = (*dsrc++);

		if (!odd) {

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

#pragma pipeloop(0)
			for (i = 0; i < len_64 - 1; i++) {
				d1 = d2;
				d2 = *dsrc;
				d3 = vis_faligndata(d1, d2);
				CONJ32_ODD;
				dsrc++;
			}

			if (!((mlib_addr)src & 7)) {
				d3 = d2;
			} else {
				d1 = d2;
				d2 = *dsrc;
				d3 = vis_faligndata(d1, d2);
			}

			CONJ32_ODD;
		}

		if (!rest_64)
			return (MLIB_SUCCESS);

		if (src[even_length] == MLIB_S32_MIN)
			dst[even_length] = MLIB_S32_MAX;
		else
			dst[even_length] = -src[even_length];
		return (MLIB_SUCCESS);
	}
}

/* *********************************************************** */

mlib_status
__mlib_VectorConj_S32C_Sat(
	mlib_s32 *xz,
	mlib_s32 n)
{
	return (__mlib_VectorConj_S32C_S32C_Sat(xz, xz, n));
}

/* *********************************************************** */
