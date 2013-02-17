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

#pragma ident	"@(#)mlib_v_VectorSplit.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *  mlib_VectorSplit_U8_U8C    - split of 2 8-bit  format vector
 *  mlib_VectorSplit_S8_S8C    - split of 2 8-bit  format vector
 *  mlib_VectorSplit_S16_S16C  - split of 2 16-bit format vector
 *  mlib_VectorSplit_S32_S32C  - split of 2 32-bit format vector
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VectorSplit_S16_S16C(mlib_s16 *r,
 *                                            mlib_s16 *ii,
 *                                            const mlib_s16 *x,
 *                                            mlib_s32 n);
 *      mlib_status mlib_VectorSplit_S32_S32C(mlib_s32 *r,
 *                                            mlib_s32 *ii,
 *                                            const mlib_s32 *x,
 *                                            mlib_s32 n);
 *      mlib_status mlib_VectorSplit_S8_S8C(mlib_s8 *r,
 *                                          mlib_s8 *ii,
 *                                          const mlib_s8 *x,
 *                                          mlib_s32 n);
 *      mlib_status mlib_VectorSplit_U8_U8C(mlib_u8 *r,
 *                                          mlib_u8 *ii,
 *                                          const mlib_u8 *x,
 *                                          mlib_s32 n);
 *
 * ARGUMENTS
 *      r    pointer to the first element of the real part.
 *      i    pointer to the first element of the imaginary part.
 *      x    pointer to the first element of the Complex input vector.
 *           x[2*i] holds the real part, and x[2*i + 1]
 *           holds the imaginary parts.
 *      n    number of elements in the vectors.
 *
 * DESCRIPTION
 *
 *      X = R + J*I
 *      X = (r1 + j*i1 r2 + j*i2 ... rn + j*in)
 *
 *      R = (r1 r2 ... rn)
 *      I = (i1 i2 ... in)
 */

#include <mlib_algebra.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorSplit_S8_S8C = __mlib_VectorSplit_S8_S8C
#pragma weak mlib_VectorSplit_S32_S32C = __mlib_VectorSplit_S32_S32C
#pragma weak mlib_VectorSplit_U8_U8C = __mlib_VectorSplit_U8_U8C
#pragma weak mlib_VectorSplit_S16_S16C = __mlib_VectorSplit_S16_S16C

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_VectorSplit_S8_S8C) mlib_VectorSplit_S8_S8C
	__attribute__((weak, alias("__mlib_VectorSplit_S8_S8C")));
__typeof__(__mlib_VectorSplit_S32_S32C) mlib_VectorSplit_S32_S32C
	__attribute__((weak, alias("__mlib_VectorSplit_S32_S32C")));
__typeof__(__mlib_VectorSplit_U8_U8C) mlib_VectorSplit_U8_U8C
	__attribute__((weak, alias("__mlib_VectorSplit_U8_U8C")));
__typeof__(__mlib_VectorSplit_S16_S16C) mlib_VectorSplit_S16_S16C
	__attribute__((weak, alias("__mlib_VectorSplit_S16_S16C")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	SPLIT(format, z, n, out_r, out_i)                       \
	format *dst_r = (format *) out_r;                       \
	format *dst_i = (format *) out_i;                       \
	format *src = (format *) z;                             \
	mlib_s32 i;                                             \
	                                                        \
	for (i = 0; i < n; i++) {                               \
	    (*dst_r++) = (*src++);                              \
	    (*dst_i++) = (*src++);                              \
	}                                                       \
	                                                        \
	return (n > 0) ? MLIB_SUCCESS : MLIB_FAILURE

/* Masks for P_stores. Array index == addr & 7 */

/*
 * static const mlib_s32 mlib_masks_arr_for_pst[8] = {
 *   0x0, 0x7f, 0x3f, 0x1f, 0xf, 0x7, 0x3, 0x1
 * };
 */

/*
 *     The following macro places 4 bytes of real part of src into
 *     Hi half of dst and 4 bytes of imaginary part of src into
 *     Lo half of dst.
 *
 */

/* *********************************************************** */

#define	MLIB_SPLIT2_U8(src, dst)                                \
	d5 = vis_fpmerge(vis_read_hi(src), vis_read_lo(src));   \
	dst = vis_fpmerge(vis_read_hi(d5), vis_read_lo(d5))

/*
 *
 * IMPLEMENTATION
 *
 * Since we have a macro MLIB_SPLIT2_U8, we can form two mlib_d64 values
 * containing 8 bytes of real part in their Hi parts and 8 bytes of
 * imaginary part in their Lo parts. It appears to be faster to consider
 * a destination array as an array of mlib_f32 to avoid using heavy
 * operations like vis_freg_pair, vis_write_hi/lo.
 *
 */

mlib_status
__mlib_VectorSplit_U8_U8C(
	mlib_u8 *r,
	mlib_u8 *ii,
	const mlib_u8 *x,
	mlib_s32 n)
{
/* pointer to real part resultant vector */
	mlib_u8 *dst_r = r;

/* pointer to imaginary part resultant vector */
	mlib_u8 *dst_i = ii;

/* pointer to source complex vector */
	mlib_u8 *src = (void *)x;

/* mlib_d64 pointer to imaginary resultant vector */
	mlib_d64 *ddst_i;

/* mlib_d64 pointer to source complex vector */
	mlib_d64 *dsrc;
	mlib_s32 len_64, even_length, rest_64, length = n, i;
	mlib_u8 emask;
	mlib_s32 odd = 0;
	mlib_d64 d1, d1_1, d2, d3, d4, d5, d6, d7, d8;
	mlib_f32 *fdst_r;
	mlib_f32 *fdst_i;
	mlib_d64 *dptr;

	if (n < 8) {
		SPLIT(mlib_u8,
			x,
			n,
			r,
			ii);
	}

/*
 * First, try to align source address for 8 bytes .
 */

	if (((mlib_addr)dst_r & 3) == ((mlib_addr)dst_i & 3)) {
		while ((mlib_addr)dst_r & 3) {
			(*dst_r++) = (*src++);
			(*dst_i++) = (*src++);
			length--;
		}

		rest_64 = length & 7;
		len_64 = length >> 3;
		even_length = len_64 << 3;

		fdst_r = (mlib_f32 *)dst_r;
		fdst_i = (mlib_f32 *)dst_i;

		if ((mlib_addr)src & 7) {
			dsrc = (mlib_d64 *)vis_alignaddr((mlib_d64 *)src, 0);

			d8 = vis_ld_d64_nf(dsrc); dsrc++;
			d7 = vis_ld_d64_nf(dsrc); dsrc++;
			d1 = vis_faligndata(d8, d7);
			d8 = vis_ld_d64_nf(dsrc); dsrc++;
			d1_1 = vis_faligndata(d7, d8);
			MLIB_SPLIT2_U8(d1, d2);
			MLIB_SPLIT2_U8(d1_1, d3);
			d7 = vis_ld_d64_nf(dsrc); dsrc++;
			d1 = vis_faligndata(d8, d7);
			d8 = vis_ld_d64_nf(dsrc); dsrc++;
			d1_1 = vis_faligndata(d7, d8);

#pragma pipeloop(0)
			for (i = 0; i < len_64; i++) {
				d7 = vis_ld_d64_nf(dsrc); dsrc++;
				MLIB_SPLIT2_U8(d1, d4);
				(*fdst_r++) = vis_read_hi(d2);
				d1 = vis_faligndata(d8, d7);
				d8 = vis_ld_d64_nf(dsrc); dsrc++;
				(*fdst_i++) = vis_read_lo(d2);
				MLIB_SPLIT2_U8(d1_1, d6);
				(*fdst_r++) = vis_read_hi(d3);
				(*fdst_i++) = vis_read_lo(d3);
				d1_1 = vis_faligndata(d7, d8);
				d2 = d4;
				d3 = d6;
			}
		} else {
			dsrc = (mlib_d64 *)src;
#pragma pipeloop(0)
			for (i = 0; i < len_64; i++) {
				d1 = (*dsrc++);
				d1_1 = (*dsrc++);
				MLIB_SPLIT2_U8(d1, d2);
				MLIB_SPLIT2_U8(d1_1, d3);
				(*fdst_r++) = vis_read_hi(d2);
				(*fdst_i++) = vis_read_lo(d2);
				(*fdst_r++) = vis_read_hi(d3);
				(*fdst_i++) = vis_read_lo(d3);
			}
		}
	} else {
		while ((mlib_addr)dst_r & 3) {
			(*dst_r++) = (*src++);
			length--;

			if (((mlib_addr)dst_r & 3) == ((mlib_addr)dst_i & 3)) {
				odd = 1;
				break;
			}

			(*dst_i++) = (*src++);
		}

		if (odd) {
			while ((mlib_addr)dst_r & 3) {
				(*dst_i++) = (*src++);
				(*dst_r++) = (*src++);
				length--;
			}

			rest_64 = length & 7;
			len_64 = length >> 3;
			even_length = len_64 << 3;

			fdst_r = (mlib_f32 *)dst_r;
			fdst_i = (mlib_f32 *)dst_i;

			if ((mlib_addr)src & 7) {
				dsrc = (mlib_d64 *)vis_alignaddr((mlib_d64 *)
					src, 0);

				d8 = vis_ld_d64_nf(dsrc); dsrc++;
				d7 = vis_ld_d64_nf(dsrc); dsrc++;
				d1 = vis_faligndata(d8, d7);
				d8 = vis_ld_d64_nf(dsrc); dsrc++;
				d1_1 = vis_faligndata(d7, d8);
				MLIB_SPLIT2_U8(d1, d2);
				MLIB_SPLIT2_U8(d1_1, d3);
				d7 = vis_ld_d64_nf(dsrc); dsrc++;
				d1 = vis_faligndata(d8, d7);
				d8 = vis_ld_d64_nf(dsrc); dsrc++;
				d1_1 = vis_faligndata(d7, d8);
#pragma pipeloop(0)
				for (i = 0; i < len_64; i++) {
					d7 = vis_ld_d64_nf(dsrc); dsrc++;
					MLIB_SPLIT2_U8(d1, d4);
					(*fdst_r++) = vis_read_lo(d2);
					d1 = vis_faligndata(d8, d7);
					d8 = vis_ld_d64_nf(dsrc); dsrc++;
					(*fdst_i++) = vis_read_hi(d2);
					MLIB_SPLIT2_U8(d1_1, d6);
					(*fdst_r++) = vis_read_lo(d3);
					(*fdst_i++) = vis_read_hi(d3);
					d1_1 = vis_faligndata(d7, d8);
					d2 = d4;
					d3 = d6;
				}
			} else {
				dsrc = (mlib_d64 *)src;

				for (i = 0; i < len_64; i++) {
					d1 = (*dsrc++);
					d1_1 = (*dsrc++);
					MLIB_SPLIT2_U8(d1, d2);
					MLIB_SPLIT2_U8(d1_1, d3);
					(*fdst_r++) = vis_read_lo(d2);
					(*fdst_i++) = vis_read_hi(d2);
					(*fdst_r++) = vis_read_lo(d3);
					(*fdst_i++) = vis_read_hi(d3);
				}
			}
		} else {

			ddst_i = (mlib_d64 *)vis_alignaddr(dst_i, 0);

			ddst_i++;

			rest_64 = length & 7;
			len_64 = length >> 3;
			even_length = len_64 << 3;

			fdst_r = (mlib_f32 *)dst_r;
			fdst_i = (mlib_f32 *)ddst_i;

			if (len_64 > 1) {
				ddst_i = (mlib_d64 *)vis_alignaddr(dst_i, 0);
				emask = vis_edge8(dst_i, dst_i + length - 1);

				ddst_i++;
				fdst_r = (mlib_f32 *)dst_r;
				fdst_i = (mlib_f32 *)ddst_i;

				if ((mlib_addr)src & 7) {
					dsrc = (mlib_d64 *)
						vis_alignaddr((mlib_d64 *)src,
						0);

					d8 = vis_ld_d64_nf(dsrc); dsrc++;
					d7 = vis_ld_d64_nf(dsrc); dsrc++;
					d1 = vis_faligndata(d8, d7);
					d8 = vis_ld_d64_nf(dsrc); dsrc++;
					d1_1 = vis_faligndata(d7, d8);
					MLIB_SPLIT2_U8(d1, d2);
					MLIB_SPLIT2_U8(d1_1, d3);
					d7 = vis_ld_d64_nf(dsrc); dsrc++;
					d1 = vis_faligndata(d8, d7);
					d8 = vis_ld_d64_nf(dsrc); dsrc++;
					d1_1 = vis_faligndata(d7, d8);
#pragma pipeloop(0)
					for (i = 0; i < len_64 - 1; i++) {
						d7 = vis_ld_d64_nf(dsrc);
						dsrc++;
						MLIB_SPLIT2_U8(d1, d4);
						(*fdst_r++) = vis_read_hi(d2);
						d1 = vis_faligndata(d8, d7);
						d8 = vis_ld_d64_nf(dsrc);
						dsrc++;
						(*fdst_i++) = vis_read_lo(d2);
						MLIB_SPLIT2_U8(d1_1, d6);
						(*fdst_r++) = vis_read_hi(d3);
						(*fdst_i++) = vis_read_lo(d3);
						d1_1 = vis_faligndata(d7, d8);
						d2 = d4;
						d3 = d6;
					}
				} else {
					dsrc = (mlib_d64 *)src;
					for (i = 0; i < len_64 - 1; i++) {
						d1 = (*dsrc++);
						d1_1 = (*dsrc++);
						MLIB_SPLIT2_U8(d1, d2);
						MLIB_SPLIT2_U8(d1_1, d3);
						(*fdst_r++) = vis_read_hi(d2);
						(*fdst_i++) = vis_read_lo(d2);
						(*fdst_r++) = vis_read_hi(d3);
						(*fdst_i++) = vis_read_lo(d3);
					}
				}

/*
 * shift back imaginary vector
 */

				dptr = vis_alignaddr(dst_i, 0);
				vis_alignaddr((mlib_d64 *)(8 -
					((mlib_addr)dst_i & 7)), 0);

				d1 = (*ddst_i++);
				i--;
				d5 = vis_faligndata(d2, d1);

				vis_pst_8(d5, dptr, emask);
				dptr++;

#pragma pipeloop(0)
				for (; i > 0; i--) {
					d5 = d1;
					d1 = ddst_i[0];
					(*dptr++) = vis_faligndata(d5, d1);
					ddst_i++;
				}

/*
 * storing next 8 bytes
 */

				if ((mlib_addr)src & 7) {
					(*fdst_r++) = vis_read_hi(d2);
					(*fdst_r++) = vis_read_hi(d3);
					d6 = vis_freg_pair(vis_read_lo(d2),
						vis_read_lo(d3));
				} else {
					d2 = vis_ld_d64_nf(dsrc); dsrc++;
					d1_1 = vis_ld_d64_nf(dsrc); dsrc++;

					MLIB_SPLIT2_U8(d2, d6);
					MLIB_SPLIT2_U8(d1_1, d3);
					(*fdst_r++) = vis_read_hi(d6);
					d6 = vis_freg_pair(vis_read_lo(d6),
						vis_read_lo(d3));
					(*fdst_r++) = vis_read_hi(d3);
				}

				d7 = vis_faligndata(d1, d6);
				(*dptr++) = d7;
				d6 = vis_faligndata(d6, d1);
				vis_pst_8(d6, dptr, ~emask);
			} else {
				mlib_u8 *rr = (mlib_u8 *)dst_r;
				mlib_u8 *iii = (mlib_u8 *)dst_i;
				mlib_u8 *xxx = (mlib_u8 *)src; {
					SPLIT(mlib_u8,
						xxx,
						length,
						rr,
						iii);
				}
			}
		}
	}

	src += (even_length << 1);
	dst_r += even_length;
	dst_i += even_length;

/*
 * If we started grouping processing with imaginary parts,
 * it is necessary to write the last imag. byte.
 */

	if (odd) {
		(*dst_i++) = (*src++);
	}

	if (!rest_64)
		return (MLIB_SUCCESS);

	for (i = 0; i < rest_64; i++) {
		(*dst_r++) = (*src++);
		(*dst_i++) = (*src++);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorSplit_S8_S8C(
	mlib_s8 *r,
	mlib_s8 *ii,
	const mlib_s8 *x,
	mlib_s32 n)
{
	return __mlib_VectorSplit_U8_U8C((mlib_u8 *)r, (mlib_u8 *)ii,
		(mlib_u8 *)x, n);
}

/* *********************************************************** */

mlib_status
__mlib_VectorSplit_S16_S16C(
	mlib_s16 *r,
	mlib_s16 *ii,
	const mlib_s16 *x,
	mlib_s32 n)
{
/* pointer to real part resultant vector */
	mlib_s16 *dst_r = r;

/* pointer to imaginary part resultant vector */
	mlib_s16 *dst_i = ii;

/* pointer to source complex vector */
	mlib_s16 *src = (void *)x;

/* mlib_d64 pointer to real resultant vector */
	mlib_d64 *ddst_r;

/* mlib_d64 pointer to imaginary resultant vector */
	mlib_d64 *ddst_i;

/* mlib_d64 pointer to source complex vector */
	mlib_d64 *dsrc;

	mlib_s16 *tmp_ptr;

	mlib_s32 len_64, even_length, rest_64, length = n, i;
	mlib_s32 odd = 0;
	mlib_d64 d0, d1, d1_1, d2, d3, d4, d5, d8;
	mlib_d64 d9, d6, d7, da, db, dc, dd, de, df;
	mlib_u16 emask;

	if (n < 8) {
		SPLIT(mlib_s16,
			x,
			n,
			r,
			ii);
	}

	vis_write_gsr(0);

/*
 * First, try to align source address for 8 bytes .
 */

	if (((mlib_addr)dst_r & 7) == ((mlib_addr)dst_i & 7) ||
		((((mlib_addr)dst_r & 7) + 2) & 7) == ((mlib_addr)dst_i & 7)) {
		if (((((mlib_addr)dst_r & 7) + 2) & 7) ==
			((mlib_addr)dst_i & 7)) {
			(*dst_r++) = (*src++);
			length--;
			odd = 1;
			tmp_ptr = dst_r;
			dst_r = dst_i;
			dst_i = tmp_ptr;
		}

		while ((mlib_addr)dst_r & 7) {
			(*dst_r++) = (*src++);
			(*dst_i++) = (*src++);
			length--;
		}

		rest_64 = length & 3;
		len_64 = length >> 2;
		even_length = len_64 << 2;

		ddst_r = (mlib_d64 *)dst_r;
		ddst_i = (mlib_d64 *)dst_i;

		if ((mlib_addr)src & 7) {
			dsrc = (mlib_d64 *)vis_alignaddr(src, 0);

			d9 = vis_ld_d64_nf(dsrc);
			d1_1 = vis_ld_d64_nf(dsrc + 1);
			d0 = vis_faligndata(d9, d1_1);
			d9 = vis_ld_d64_nf(dsrc + 2);
			d1 = vis_faligndata(d1_1, d9);
			d1_1 = vis_ld_d64_nf(dsrc + 3);
			d2 = vis_faligndata(d9, d1_1);
			d9 = vis_ld_d64_nf(dsrc + 4);
			d3 = vis_faligndata(d1_1, d9);
			dsrc += 4;

			d5 = vis_fpmerge(vis_read_hi(d0), vis_read_hi(d1));
			d6 = vis_fpmerge(vis_read_lo(d0), vis_read_lo(d1));
			d7 = vis_fpmerge(vis_read_hi(d5), vis_read_hi(d6));
			d8 = vis_fpmerge(vis_read_lo(d5), vis_read_lo(d6));
			dc = vis_fpmerge(vis_read_hi(d7), vis_read_lo(d7));
			dd = vis_fpmerge(vis_read_hi(d8), vis_read_lo(d8));

			da = vis_fpack32(d2, d2);
			da = vis_fpack32(da, da);
			db = vis_fpack32(d3, d3);
			db = vis_fpack32(db, db);
			de = vis_fpackfix_pair(d2, d3);
			df = vis_fpackfix_pair(da, db);

			d1_1 = vis_ld_d64_nf(dsrc + 1);
			d0 = vis_faligndata(d9, d1_1);
			d9 = vis_ld_d64_nf(dsrc + 2);
			d1 = vis_faligndata(d1_1, d9);
			d1_1 = vis_ld_d64_nf(dsrc + 3);
			d2 = vis_faligndata(d9, d1_1);
			d9 = vis_ld_d64_nf(dsrc + 4);
			d3 = vis_faligndata(d1_1, d9);
			dsrc += 4;

#pragma pipeloop(0)
			for (i = 0; i < len_64 - 1; i += 2) {
				d1_1 = vis_ld_d64_nf(dsrc + 1);

				d5 = vis_fpmerge(vis_read_hi(d0),
					vis_read_hi(d1));
				da = vis_fpack32(d2, d2);
				ddst_r[0] = dc;
				d6 = vis_fpmerge(vis_read_lo(d0),
					vis_read_lo(d1));
				db = vis_fpack32(d3, d3);
				ddst_i[0] = dd;

				d7 = vis_fpmerge(vis_read_hi(d5),
					vis_read_hi(d6));
				da = vis_fpack32(da, da);
				ddst_r[1] = de;
				d8 = vis_fpmerge(vis_read_lo(d5),
					vis_read_lo(d6));
				db = vis_fpack32(db, db);
				ddst_i[1] = df;
				dc = vis_fpmerge(vis_read_hi(d7),
					vis_read_lo(d7));
				de = vis_fpackfix_pair(d2, d3);
				dd = vis_fpmerge(vis_read_hi(d8),
					vis_read_lo(d8));
				df = vis_fpackfix_pair(da, db);

				d0 = vis_faligndata(d9, d1_1);
				d9 = vis_ld_d64_nf(dsrc + 2);

				d1 = vis_faligndata(d1_1, d9);
				d1_1 = vis_ld_d64_nf(dsrc + 3);

				d2 = vis_faligndata(d9, d1_1);
				d9 = vis_ld_d64_nf(dsrc + 4);
				d3 = vis_faligndata(d1_1, d9);

				dsrc += 4;
				ddst_r += 2;
				ddst_i += 2;
			}

			if (i < len_64) {
				ddst_r[0] = dc;
				ddst_i[0] = dd;
			}
		} else {
			dsrc = (mlib_d64 *)src;
			vis_write_gsr(2);
#pragma pipeloop(0)
			for (i = 0; i < len_64 - 1; i += 2) {
				d0 = dsrc[0];
				d1 = dsrc[1];
				d2 = dsrc[2];
				d3 = dsrc[3];

				d5 = vis_fpmerge(vis_read_hi(d0),
					vis_read_hi(d1));
				d6 = vis_fpmerge(vis_read_lo(d0),
					vis_read_lo(d1));
				d7 = vis_fpmerge(vis_read_hi(d5),
					vis_read_hi(d6));
				ddst_r[0] =
					vis_fpmerge(vis_read_hi(d7),
					vis_read_lo(d7));
				d8 = vis_fpmerge(vis_read_lo(d5),
					vis_read_lo(d6));
				ddst_i[0] =
					vis_fpmerge(vis_read_hi(d8),
					vis_read_lo(d8));
				d7 = vis_faligndata(d2, d2);
				d8 = vis_fpack32(d3, d3);
				d8 = vis_fpack32(d8, d8);
				ddst_r[1] = vis_fpackfix_pair(d2, d3);
				ddst_i[1] = vis_fpackfix_pair(d7, d8);
				dsrc += 4;
				ddst_r += 2;
				ddst_i += 2;
			}

			if (i < len_64) {
				d0 = dsrc[0];
				d1 = dsrc[1];
				d5 = vis_fpmerge(vis_read_hi(d0),
					vis_read_hi(d1));
				d6 = vis_fpmerge(vis_read_lo(d0),
					vis_read_lo(d1));
				d7 = vis_fpmerge(vis_read_hi(d5),
					vis_read_hi(d6));
				ddst_r[0] =
					vis_fpmerge(vis_read_hi(d7),
					vis_read_lo(d7));
				d8 = vis_fpmerge(vis_read_lo(d5),
					vis_read_lo(d6));
				ddst_i[0] =
					vis_fpmerge(vis_read_hi(d8),
					vis_read_lo(d8));
			}
		}
	} else {
		if ((((mlib_addr)dst_r & 7) == 0 &&
			((mlib_addr)dst_r & 7) == ((mlib_addr)src & 7))) {

			rest_64 = length & 3;
			len_64 = length >> 2;
			even_length = len_64 << 2;

			ddst_r = (mlib_d64 *)dst_r;
			ddst_i = vis_alignaddr(dst_i, 0);
			dsrc = (mlib_d64 *)src;
			vis_alignaddr((mlib_d64 *)(8 - ((mlib_addr)dst_i & 7)),
				0);
			emask = vis_edge16(dst_i, dst_i + length - 1);

			d0 = vis_ld_d64_nf(dsrc);
			d1 = vis_ld_d64_nf(dsrc + 1);
			d5 = vis_fpmerge(vis_read_hi(d0), vis_read_hi(d1));
			d6 = vis_fpmerge(vis_read_lo(d0), vis_read_lo(d1));
			d7 = vis_fpmerge(vis_read_hi(d5), vis_read_hi(d6));
			ddst_r[0] =
				vis_fpmerge(vis_read_hi(d7), vis_read_lo(d7));
			d8 = vis_fpmerge(vis_read_lo(d5), vis_read_lo(d6));
			da = vis_fpmerge(vis_read_hi(d8), vis_read_lo(d8));

			db = vis_faligndata(db, da);
			vis_pst_16(db, ddst_i, emask);

			ddst_i++;
			ddst_r++;
			dsrc += 2;

			d0 = vis_ld_d64_nf(dsrc);
			d1 = vis_ld_d64_nf(dsrc + 1);
			d2 = vis_ld_d64_nf(dsrc + 2);
			d3 = vis_ld_d64_nf(dsrc + 3);
			d5 = vis_fpmerge(vis_read_hi(d0), vis_read_hi(d1));
			d6 = vis_fpmerge(vis_read_lo(d0), vis_read_lo(d1));
			d7 = vis_fpmerge(vis_read_hi(d5), vis_read_hi(d6));
			dc = vis_fpmerge(vis_read_hi(d7), vis_read_lo(d7));
			d8 = vis_fpmerge(vis_read_lo(d5), vis_read_lo(d6));
			db = vis_fpmerge(vis_read_hi(d8), vis_read_lo(d8));
			dd = vis_faligndata(da, db);

			d4 = vis_fpack32(d2, d2);
			d4 = vis_fpack32(d4, d4);
			d9 = vis_fpack32(d3, d3);
			d9 = vis_fpack32(d9, d9);
			de = vis_fpackfix_pair(d2, d3);
			da = vis_fpackfix_pair(d4, d9);
			df = vis_faligndata(db, da);
			dsrc += 4;

			d0 = vis_ld_d64_nf(dsrc);
			d1 = vis_ld_d64_nf(dsrc + 1);
			d2 = vis_ld_d64_nf(dsrc + 2);
			d3 = vis_ld_d64_nf(dsrc + 3);
			dsrc += 4;
#pragma pipeloop(0)
			for (i = 1; i < len_64 - 1; i += 2) {
				ddst_r[0] = dc;
				d5 = vis_fpmerge(vis_read_hi(d0),
					vis_read_hi(d1));
				d4 = vis_fpack32(d2, d2);
				d6 = vis_fpmerge(vis_read_lo(d0),
					vis_read_lo(d1));
				ddst_i[0] = dd;
				d7 = vis_fpmerge(vis_read_hi(d5),
					vis_read_hi(d6));
				d4 = vis_fpack32(d4, d4);
				dc = vis_fpmerge(vis_read_hi(d7),
					vis_read_lo(d7));
				d9 = vis_fpack32(d3, d3);
				ddst_r[1] = de;
				d8 = vis_fpmerge(vis_read_lo(d5),
					vis_read_lo(d6));
				d9 = vis_fpack32(d9, d9);
				db = vis_fpmerge(vis_read_hi(d8),
					vis_read_lo(d8));
				de = vis_fpackfix_pair(d2, d3);
				ddst_i[1] = df;
				dd = vis_faligndata(da, db);

				da = vis_fpackfix_pair(d4, d9);
				df = vis_faligndata(db, da);
				d0 = vis_ld_d64_nf(dsrc);
				d1 = vis_ld_d64_nf(dsrc + 1);
				d2 = vis_ld_d64_nf(dsrc + 2);
				d3 = vis_ld_d64_nf(dsrc + 3);
				dsrc += 4;
				ddst_r += 2;
				ddst_i += 2;
			}

			if (i < len_64) {
				ddst_r[0] = dc;
				ddst_i[0] = dd;
				dd = df;
				ddst_r++;
				ddst_i++;
			}

			vis_pst_16(dd, ddst_i, ~emask);
		} else {
			if (((((mlib_addr)dst_r & 7) * 2 +
				((mlib_addr)src & 7) - 2) & 7) == 0) {
				(*dst_r++) = (*src++);
				odd = 1;
				length--;

				while ((mlib_addr)dst_r & 7) {
					(*dst_i++) = (*src++);
					(*dst_r++) = (*src++);
					length--;
				}

				rest_64 = length & 3;
				len_64 = length >> 2;
				even_length = len_64 << 2;

				ddst_r = (mlib_d64 *)dst_r;

				if (len_64 > 1) {
					ddst_i = vis_alignaddr(dst_i, 0);
					dsrc = (mlib_d64 *)src;
					vis_alignaddr((mlib_d64 *)(8 -
						((mlib_addr)dst_i & 7)), 0);
					emask = vis_edge16(dst_i,
						dst_i + length - 1);

					d0 = vis_ld_d64_nf(dsrc);
					d1 = vis_ld_d64_nf(dsrc + 1);
					d5 = vis_fpmerge(vis_read_hi(d0),
						vis_read_hi(d1));
					d6 = vis_fpmerge(vis_read_lo(d0),
						vis_read_lo(d1));
					d7 = vis_fpmerge(vis_read_hi(d5),
						vis_read_hi(d6));
					da = vis_fpmerge(vis_read_hi(d7),
						vis_read_lo(d7));
					d8 = vis_fpmerge(vis_read_lo(d5),
						vis_read_lo(d6));
					ddst_r[0] =
						vis_fpmerge(vis_read_hi(d8),
						vis_read_lo(d8));

					db = vis_faligndata(db, da);
					vis_pst_16(db, ddst_i, emask);

					ddst_i++;
					ddst_r++;
					dsrc += 2;

					d0 = vis_ld_d64_nf(dsrc);
					d1 = vis_ld_d64_nf(dsrc + 1);
					d2 = vis_ld_d64_nf(dsrc + 2);
					d3 = vis_ld_d64_nf(dsrc + 3);
					d5 = vis_fpmerge(vis_read_hi(d0),
						vis_read_hi(d1));
					d6 = vis_fpmerge(vis_read_lo(d0),
						vis_read_lo(d1));
					d7 = vis_fpmerge(vis_read_hi(d5),
						vis_read_hi(d6));
					db = vis_fpmerge(vis_read_hi(d7),
						vis_read_lo(d7));
					d8 = vis_fpmerge(vis_read_lo(d5),
						vis_read_lo(d6));
					dc = vis_fpmerge(vis_read_hi(d8),
						vis_read_lo(d8));
					dd = vis_faligndata(da, db);

					d4 = vis_fpack32(d2, d2);
					d4 = vis_fpack32(d4, d4);
					d9 = vis_fpack32(d3, d3);
					d9 = vis_fpack32(d9, d9);
					da = vis_fpackfix_pair(d2, d3);
					de = vis_fpackfix_pair(d4, d9);
					df = vis_faligndata(db, da);
					dsrc += 4;

					d0 = vis_ld_d64_nf(dsrc);
					d1 = vis_ld_d64_nf(dsrc + 1);
					d2 = vis_ld_d64_nf(dsrc + 2);
					d3 = vis_ld_d64_nf(dsrc + 3);
					dsrc += 4;

#pragma pipeloop(0)
					for (i = 1; i < len_64 - 1; i += 2) {
						ddst_r[0] = dc;
						d5 = vis_fpmerge(vis_read_hi
							(d0), vis_read_hi(d1));
						d4 = vis_fpack32(d2, d2);
						d6 = vis_fpmerge(vis_read_lo
							(d0), vis_read_lo(d1));
						ddst_i[0] = dd;
						d7 = vis_fpmerge(vis_read_hi
							(d5), vis_read_hi(d6));
						d4 = vis_fpack32(d4, d4);
						db = vis_fpmerge(vis_read_hi
							(d7), vis_read_lo(d7));
						d9 = vis_fpack32(d3, d3);
						ddst_r[1] = de;
						d8 = vis_fpmerge(vis_read_lo
							(d5), vis_read_lo(d6));
						d9 = vis_fpack32(d9, d9);
						dc = vis_fpmerge(vis_read_hi
							(d8), vis_read_lo(d8));
						de = vis_fpackfix_pair(d4, d9);
						ddst_i[1] = df;
						dd = vis_faligndata(da, db);

						da = vis_fpackfix_pair(d2, d3);
						df = vis_faligndata(db, da);
						d0 = vis_ld_d64_nf(dsrc);
						d1 = vis_ld_d64_nf(dsrc + 1);
						d2 = vis_ld_d64_nf(dsrc + 2);
						d3 = vis_ld_d64_nf(dsrc + 3);
						dsrc += 4;
						ddst_r += 2;
						ddst_i += 2;
					}

					if (i < len_64) {
						ddst_r[0] = dc;
						ddst_i[0] = dd;
						dd = df;
						ddst_r++;
						ddst_i++;
					}

					vis_pst_16(dd, ddst_i, ~emask);
					tmp_ptr = dst_r;
					dst_r = dst_i;
					dst_i = tmp_ptr;
				} else {
					for (i = 0; i < length; i++) {
						(*dst_i++) = (*src++);
						(*dst_r++) = (*src++);
					}

					if (odd) {
						(*dst_i++) = (*src++);
					}

					return (n >
						0) ? MLIB_SUCCESS :
						MLIB_FAILURE;
				}
			} else {

				mlib_u32 *dp;
				mlib_u32 d0, d1, reg;
				mlib_u32 *idst_r, *idst_i;
				mlib_s32 offset_r, offset_i;
				mlib_s32 j;
				mlib_u8 odd = 0;
				mlib_s16 *tmp;

				if ((mlib_addr)x & 3) {
					(*r++) = (*x++);
					odd = 1;
					tmp = r;
					r = ii;
					ii = tmp;
				}

				dp = (mlib_u32 *)x;
				offset_r = (mlib_addr)r & 3;
				offset_i = (mlib_addr)ii & 3;

				if (offset_r == offset_i) {

					if (offset_r != 0) {
						d0 = dp[0];
						(*r++) = (d0 >> 16);
						(*ii++) = (d0 & 0xffff);
						dp++;
						length--;
					}

					idst_r = (mlib_u32 *)r;
					idst_i = (mlib_u32 *)ii;

					if (!odd) {
						for (j = 0; j < (length / 2);
							j++) {
							d0 = dp[0];
							d1 = dp[1];
							*idst_r =
								(d0 & (~0xffff))
								+ (d1 >> 16);
							*idst_i =
								(d0 << 16) +
								(d1 & 0xffff);
							dp += 2;
							idst_r++;
							idst_i++;
						}

						if (length & 1) {
							*((mlib_s16 *)idst_r) =
								dp[0] >> 16;
							*((mlib_s16 *)idst_i) =
								dp[0] & 0xffff;
						}
					} else {
						for (j = 0;
							j < (length / 2) - 1;
							j++) {
							d0 = dp[0];
							d1 = dp[1];

							*idst_r =
								(d0 & (~0xffff))
								+ (d1 >> 16);
							*idst_i =
								(d0 << 16) +
								(d1 & 0xffff);
							idst_r++;
							idst_i++;
							dp += 2;
						}

						if (length & 1) {
							d0 = dp[0];
							d1 = dp[1];
							*idst_r =
								(d1 >> 16) +
								(d0 & ~0xffff);
							*idst_i =
								(d0 << 16) +
								(d1 & 0xffff);
							idst_r++;
							*((mlib_s16 *)idst_r) =
								*((mlib_s16
								*)(dp + 2));
						} else {
							d0 = dp[0];
							*idst_r =
								(d0 & ~0xffff) +
								(*((mlib_u16
								*)(dp +
								1)) & 0xffff);
							*((mlib_s16 *)idst_i) =
								d0 & 0xffff;
						}
					}

					return (n >
						0) ? MLIB_SUCCESS :
						MLIB_FAILURE;
				}

				if ((mlib_addr)r & 3) {
					d0 = *dp;
					dp++;
					*r = d0 >> 16;
					*ii = d0 & 0xffff;
					r++;
					ii++;
					length--;
				}

				idst_r = (mlib_u32 *)r;
				idst_i = (mlib_u32 *)(ii + 1);

				d1 = vis_ld_u32_nf(dp + 1);
				d0 = vis_ld_u32_nf(dp);
				*ii = d0 & 0xffff;
				*idst_r = (d0 & ~0xffff) + (d1 >> 16);
				reg = d1 << 16;
				idst_r++;
				length -= 2;
				dp += 2;

#pragma pipeloop(0)
				for (j = 0; j < (length / 2 - 1); j++) {
					d0 = dp[0];
					d1 = dp[1];
					*idst_r = (d0 & ~0xffff) + (d1 >> 16);
					*idst_i = (d0 & 0xffff) + reg;
					reg = d1 << 16;
					dp += 2;
					idst_r++;
					idst_i++;
				}

				d0 = vis_ld_u32_nf(dp);

				if (length & 1) {
					d1 = vis_ld_u32_nf(dp + 1);
					dp += 2;

					if (odd) {
						*idst_r =
							(d0 & ~0xffff) +
							(d1 >> 16);
						idst_r++;
						*idst_i = (d0 & 0xffff) + reg;
						idst_i++;
						*((mlib_s16 *)idst_i) =
							d1 & 0xffff;
						*((mlib_s16 *)idst_r) =
							*((mlib_s16 *)dp);
					} else {
						*idst_r =
							(d0 & ~0xffff) +
							(d1 >> 16);
						idst_r++;
						*idst_i = (d0 & 0xffff) + reg;
						idst_i++;
						d0 = vis_ld_u32_nf(dp);
						*idst_i =
							(d1 << 16) +
							(d0 & 0xffff);
						*((mlib_s16 *)idst_r) =
							d0 >> 16;
					}
				} else {
					dp++;
					*idst_i = (d0 & 0xffff) + reg;

					if (odd) {
						*idst_r =
							*((mlib_u16 *)dp) +
							(d0 & ~0xffff);
					} else {
						d1 = vis_ld_u32_nf(dp);
						*idst_r =
							(d1 >> 16) +
							(d0 & ~0xffff);
						idst_i++;
						*((mlib_s16 *)idst_i) = d1;
					}
				}

				return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
			}
		}
	}

	src += (even_length << 1);
	dst_r += even_length;
	dst_i += even_length;

/*
 * If we started grouping processing with imaginary parts,
 * it is necessary to write the last imag. byte.
 */

	if (odd) {
		(*dst_r++) = (*src++);
	}

	if (!rest_64)
		return (MLIB_SUCCESS);

	if (odd) {
		for (i = 0; i < rest_64; i++) {
			(*dst_i++) = (*src++);
			(*dst_r++) = (*src++);
		}
	} else {
		for (i = 0; i < rest_64; i++) {
			(*dst_r++) = (*src++);
			(*dst_i++) = (*src++);
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorSplit_S32_S32C(
	mlib_s32 *r,
	mlib_s32 *ii,
	const mlib_s32 *x,
	mlib_s32 n)
{
/* pointer to real part resultant vector */
	mlib_u32 *dst_r = (mlib_u32 *)r;

/* pointer to imaginary part resultant vector */
	mlib_u32 *dst_i = (mlib_u32 *)ii;

/* pointer to source complex vector */
	mlib_u32 *src = (mlib_u32 *)x;

/* mlib_d64 pointer to real resultant vector */
	mlib_d64 *ddst_r;

/* mlib_d64 pointer to imaginary resultant vector */
	mlib_d64 *ddst_i;

/* mlib_d64 pointer to source complex vector */
	mlib_d64 *dsrc;
	mlib_s32 len_64, even_length, rest_64, length = n, i;
	mlib_d64 d0, d1, d2, d3, d4, d5;

	if (n < 8) {
		SPLIT(mlib_s32,
			x,
			n,
			r,
			ii);
	}

/*
 * Now analyze the alignments of output and input vectors.
 */

	vis_alignaddr((mlib_d64 *)4, 0);

	if ((((mlib_addr)dst_r & 7) == ((mlib_addr)dst_i & 7))) {

		if ((mlib_addr)dst_r & 7) {
			(*dst_r++) = (*src++);
			(*dst_i++) = (*src++);
			length--;
		}

		ddst_r = (mlib_d64 *)dst_r;
		ddst_i = (mlib_d64 *)dst_i;

		rest_64 = length & 1;
		len_64 = length >> 1;
		even_length = len_64 << 1;

		dsrc = (mlib_d64 *)((mlib_addr)src & ~7);

		if (!((mlib_addr)src & 7)) {

#pragma pipeloop(0)
			for (i = 0; i < len_64; i++) {
/* r1, i1 */
				d1 = (*dsrc++);
/* r2, i2 */
				d2 = (*dsrc++);
/* i2, r1 */
				d3 = vis_faligndata(d2, d1);
/* r1, r2 */
				d4 = vis_faligndata(d3, d2);
/* i1, i2 */
				d5 = vis_faligndata(d1, d3);
				(*ddst_r++) = d4;
				(*ddst_i++) = d5;
			}
		} else {

/* i0, r1 */
			d1 = (*dsrc++);
#pragma pipeloop(0)
			for (i = 0; i < len_64; i++) {
/* i1, r2 */
				d2 = (*dsrc++);
/* i2, r3 */
				d3 = (*dsrc++);
/* r2, i1 */
				d2 = vis_faligndata(d2, d2);
/* r1, r2 */
				d4 = vis_faligndata(d1, d2);
/* r1, r2 */
				d5 = vis_faligndata(d2, d3);
				(*ddst_r++) = d4;
				(*ddst_i++) = d5;
				d1 = d3;
			}
		}

		dst_r += even_length;
		dst_i += even_length;
		src += (even_length << 1);
	} else {

		if (((mlib_addr)dst_i & 7) == ((mlib_addr)src & 7)) {
			(*dst_r++) = (*src++);
			(*dst_i++) = (*src++);
			length--;
		}

		if ((mlib_addr)dst_r & 7) {
			(*dst_r++) = (*src++);
			ddst_r = (mlib_d64 *)dst_i;
			ddst_i = (mlib_d64 *)dst_r;
			length--;

			rest_64 = length & 1;
			len_64 = length >> 1;
			even_length = len_64 << 1;

			dsrc = (mlib_d64 *)src;
#pragma pipeloop(0)
			for (i = 0; i < len_64; i++) {
/* r1, i1 */
				d1 = (*dsrc++);
/* r2, i2 */
				d2 = (*dsrc++);
/* i2, r1 */
				d3 = vis_faligndata(d2, d1);
/* r1, r2 */
				d4 = vis_faligndata(d3, d2);
/* i1, i2 */
				d5 = vis_faligndata(d1, d3);
				(*ddst_r++) = d4;
				(*ddst_i++) = d5;
			}

			*(mlib_s32 *)ddst_r = *((mlib_s32 *)dsrc);

			dst_r = (mlib_u32 *)ddst_i;
			dst_i = (mlib_u32 *)ddst_r + 1;
			src = (mlib_u32 *)dsrc + 1;
		} else {
			ddst_r = (mlib_d64 *)dst_r;
			ddst_i = (mlib_d64 *)(dst_i + 1);

			rest_64 = length & 1;
			len_64 = length >> 1;
			even_length = len_64 << 1;

			dsrc = (mlib_d64 *)src;

/* r1, i1 */
			d0 = (*dsrc++);
			dst_i[0] = src[1];
/* i1, r1 */
			d0 = vis_faligndata(d0, d0);
#pragma pipeloop(0)
			for (i = 0; i < len_64 - 1; i++) {
/* r2, i2 */
				d1 = (*dsrc++);
/* r3, i3 */
				d2 = (*dsrc++);
/* r1, r2 */
				d3 = vis_faligndata(d0, d1);
/* i3, r2 */
				d4 = vis_faligndata(d2, d1);
/* i2, i3 */
				d5 = vis_faligndata(d1, d4);
/* i3, r3 */
				d0 = vis_faligndata(d2, d2);
				(*ddst_r++) = d3;
				(*ddst_i++) = d5;
			}

/* r2, i2 */
			d1 = *dsrc;
/* r1, r2 */
			d3 = vis_faligndata(d0, d1);
			*ddst_r = d3;
			*(mlib_s32 *)ddst_i = *((mlib_s32 *)dsrc + 1);

			dst_r += even_length;
			dst_i += even_length;
			src += (even_length << 1);
		}
	}

	if (rest_64) {
		(*dst_r++) = (*src++);
		(*dst_i++) = (*src++);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
