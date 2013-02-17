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

#pragma ident	"@(#)mlib_v_VectorMerge.c	9.5	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorMerge_U8C_U8    - merge of 2 8-bit  format vector
 *      mlib_VectorMerge_S8C_S8    - merge of 2 8-bit  format vector
 *      mlib_VectorMerge_S16C_S16  - merge of 2 16-bit format vector
 *      mlib_VectorMerge_S32C_S32  - merge of 2 32-bit format vector
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorMerge_S16C_S16(mlib_s16       *z,
 *                                            const mlib_s16 *rr,
 *                                            const mlib_s16 *ii,
 *                                            mlib_s32       n);
 *      mlib_status mlib_VectorMerge_S32C_S32(mlib_s32       *z,
 *                                            const mlib_s32 *rr,
 *                                            const mlib_s32 *ii,
 *                                            mlib_s32       n);
 *      mlib_status mlib_VectorMerge_S8C_S8(mlib_s8       *z,
 *                                          const mlib_s8 *r,
 *                                          const mlib_s8 *i,
 *                                          mlib_s32      n);
 *      mlib_status mlib_VectorMerge_U8C_U8(mlib_u8       *z,
 *                                          const mlib_u8 *rr,
 *                                          const mlib_u8 *ii,
 *                                          mlib_s32      n);
 *
 * ARGUMENTS
 *      z    pointer to the first element of the resultant vector
 *      r    pointer to the first element of the 1 input vector (real parts)
 *      i    pointer to the first element of the 2 input vector (imag. parts)
 *      n    number of elements in the vectors
 *
 * DESCRIPTION
 *
 *      Z = R + J*I
 *
 *      R = (r1 r2 ... rn)
 *      I = (i1 i2 ... in)
 *      Z = (r1 + j*i1 r2 + j*i2 ... rn + j*in)
 *
 * IMPLEMENTATION
 *
 * For 8-bit format we can take advantages of VIS. We read elements of real
 * and imaginary parts by 64 bits (sometimes with using alignaddr() and
 * aligndata() depending on source alignments), then apply vis_fpmerge()
 * function and store resulting information by 2*64 bits.
 *
 * For 16-bit format we try to read information by 32 bits, then by logical
 * operations extract and merge 16-bit elements of r and i vectors, and
 * then compose resultant information in a 64-bit register and store it to
 * memory.
 *
 * For 32-bit format we also use VIS instructions in case of 8-byte
 * alignments of both input vectors and output one. We read 2 times by
 * 64 bits into 2*64-bit registers and then compose 2*64-bit resultant
 * values by vis_freg_pair() and vis_read_hi()/vis_read_lo().
 * In case of arbitrary alignment of input vectors we read by 32-bit
 * words, but store the result by 2*64-bit portions.
 */

#include <mlib_algebra.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorMerge_U8C_U8 = __mlib_VectorMerge_U8C_U8
#pragma weak mlib_VectorMerge_S16C_S16 = __mlib_VectorMerge_S16C_S16
#pragma weak mlib_VectorMerge_S8C_S8 = __mlib_VectorMerge_S8C_S8
#pragma weak mlib_VectorMerge_S32C_S32 = __mlib_VectorMerge_S32C_S32

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_VectorMerge_U8C_U8) mlib_VectorMerge_U8C_U8
	__attribute__((weak, alias("__mlib_VectorMerge_U8C_U8")));
__typeof__(__mlib_VectorMerge_S16C_S16) mlib_VectorMerge_S16C_S16
	__attribute__((weak, alias("__mlib_VectorMerge_S16C_S16")));
__typeof__(__mlib_VectorMerge_S8C_S8) mlib_VectorMerge_S8C_S8
	__attribute__((weak, alias("__mlib_VectorMerge_S8C_S8")));
__typeof__(__mlib_VectorMerge_S32C_S32) mlib_VectorMerge_S32C_S32
	__attribute__((weak, alias("__mlib_VectorMerge_S32C_S32")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MERGE(format, inp_r, inp_i, n, z)                       \
	format *src_r = (format *) inp_r;                       \
	format *src_i = (format *) inp_i;                       \
	format *dst = (format *) z;                             \
	mlib_s32 i;                                             \
	                                                        \
	for (i = 0; i < n; i++) {                               \
	    (*dst++) = (*src_r++);                              \
	    (*dst++) = (*src_i++);                              \
	}                                                       \
	                                                        \
	return (n > 0) ? MLIB_SUCCESS : MLIB_FAILURE

/* *********************************************************** */

#define	MERGE8                                                  \
	d3 = vis_fpmerge(vis_read_hi(d1), vis_read_hi(d2));     \
	d4 = vis_fpmerge(vis_read_lo(d1), vis_read_lo(d2))

/* *********************************************************** */

#define	MERGE32                                                 \
	d3 = vis_freg_pair(vis_read_hi(d1), vis_read_hi(d2));   \
	d4 = vis_freg_pair(vis_read_lo(d1), vis_read_lo(d2))

/* *********************************************************** */

/*
 *
 * For 8-bit format we read elements of real
 * and imaginary parts by 64 bits (sometimes with using alignaddr() and
 * aligndata() depending on source alignments), then apply vis_fpmerge()
 * function and Store resulting information by 2*64 bits.
 *
 */

mlib_status
__mlib_VectorMerge_U8C_U8(
	mlib_u8 *z,
	const mlib_u8 *rr,
	const mlib_u8 *ii,
	mlib_s32 n)
{
/* pointer to real source vector */
	mlib_u8 *src_r = (void *)rr;

/* pointer to imaginary source vector */
	mlib_u8 *src_i = (void *)ii;

/* pointer to resultant complex vector */
	mlib_u8 *dst = z;

/* mlib_d64 pointer to real source vector */
	mlib_d64 *dsrc_r;

/* mlib_d64 pointer to imaginary source vector */
	mlib_d64 *dsrc_i;

/* mlib_d64 pointer to resultant complex vector */
	mlib_d64 *ddst;
	mlib_s32 len_64, even_length, rest_64, length = n, i;
	mlib_s32 odd = 0;
	mlib_d64 d1, d2, d3, d4, d1_r, d1_i, d2_r, d2_i;

	if (n < 8) {
		MERGE(mlib_u8,
			rr,
			ii,
			n,
			z);
	}

/*
 * First, try to align destination address for 8 bytes .
 */

	while ((mlib_addr)dst & 7) {
		(*dst++) = (*src_r++);
		length--;

		if (!((mlib_addr)dst & 7)) {
			odd = 1;
			break;
		}

		(*dst++) = (*src_i++);
	}

	rest_64 = length & 7;
	len_64 = length >> 3;
	even_length = len_64 << 3;
	ddst = (mlib_d64 *)dst;

/*
 * Now analyze the alignments of source vectors.
 */

	if ((!((mlib_addr)src_r & 7)) && (!((mlib_addr)src_i & 7))) {

/*
 * Both input vectors are 8-byte aligned.
 */

		dsrc_r = (mlib_d64 *)src_r;
		dsrc_i = (mlib_d64 *)src_i;

		if (!odd) {

/*
 * First "alignment" loop finished with imaginary part.
 * Start the next merge with real part.
 */

			for (i = 0; i < len_64; i++) {
				d1 = (*dsrc_r++);
				d2 = (*dsrc_i++);
				MERGE8;
				ddst[0] = d3;
				ddst[1] = d4;
				ddst += 2;
			}
		} else {

/*
 * First "alignment" loop finished with real part.
 * Start the next merge with imaginary part.
 */

			for (i = 0; i < len_64; i++) {
				d2 = (*dsrc_r++);
				d1 = (*dsrc_i++);
				MERGE8;
				ddst[0] = d3;
				ddst[1] = d4;
				ddst += 2;
			}
		}
	} else if (((mlib_addr)src_r & 7) == ((mlib_addr)src_i & 7)) {

/*
 * Both input vectors have the same address alignment.
 * We cam use only 1 vis_faligndata inside loop.
 */

		dsrc_r = (mlib_d64 *)vis_alignaddr((void *)src_r, 0);
		dsrc_i = (mlib_d64 *)vis_alignaddr((void *)src_i, 0);
		d2_r = (*dsrc_r++);
		d2_i = (*dsrc_i++);

		if (!odd) {

/*
 * First "alignment" loop finished with imaginary part.
 * Start the next merge with real part.
 */

#pragma pipeloop(0)
			for (i = 0; i < len_64; i++) {
				d1_r = d2_r;
				d2_r = (*dsrc_r++);
				d1 = vis_faligndata(d1_r, d2_r);
				d1_i = d2_i;
				d2_i = (*dsrc_i++);
				d2 = vis_faligndata(d1_i, d2_i);
				MERGE8;
				ddst[0] = d3;
				ddst[1] = d4;
				ddst += 2;
			}
		} else {

/*
 * First "alignment" loop finished with real part.
 * Start the next merge with imaginary part.
 */

#pragma pipeloop(0)
			for (i = 0; i < len_64; i++) {
				d1_r = d2_r;
				d2_r = (*dsrc_r++);
				d2 = vis_faligndata(d1_r, d2_r);
				d1_i = d2_i;
				d2_i = (*dsrc_i++);
				d1 = vis_faligndata(d1_i, d2_i);
				MERGE8;
				ddst[0] = d3;
				ddst[1] = d4;
				ddst += 2;
			}
		}
	} else {

/*
 * Input vectors have different arbitrary alignments.
 * We have to use vis_alignaddr() and vis_faligndata() on
 * each loop iteration.
 */

		dsrc_i = (mlib_d64 *)vis_alignaddr((void *)src_i, 0);
		dsrc_r = (mlib_d64 *)vis_alignaddr((void *)src_r, 0);
		d2_r = (*dsrc_r++);
		d2_i = (*dsrc_i++);

		if (!odd) {

/*
 * First "alignment" loop finished with imaginary part.
 * Start the next merge with real part.
 */

#pragma pipeloop(0)
			for (i = 0; i < len_64; i++) {
				d1_r = d2_r;
				d2_r = vis_ld_d64_nf(dsrc_r); dsrc_r++;
				d1 = vis_faligndata(d1_r, d2_r);
				d1_i = d2_i;
				vis_alignaddr((void *)src_i, 0);
				d2_i = vis_ld_d64_nf(dsrc_i); dsrc_i++;
				d2 = vis_faligndata(d1_i, d2_i);
				MERGE8;
				vis_alignaddr((void *)src_r, 0);
				ddst[0] = d3;
				ddst[1] = d4;
				ddst += 2;
			}
		} else {

/*
 * First "alignment" loop finished with real part.
 * Start the next merge with imaginary part.
 */

#pragma pipeloop(0)
			for (i = 0; i < len_64; i++) {
				d1_r = d2_r;
				d2_r = vis_ld_d64_nf(dsrc_r); dsrc_r++;
				d2 = vis_faligndata(d1_r, d2_r);
				vis_alignaddr((void *)src_i, 0);
				d1_i = d2_i;
				d2_i = vis_ld_d64_nf(dsrc_i); dsrc_i++;
				d1 = vis_faligndata(d1_i, d2_i);
				MERGE8;
				vis_alignaddr((void *)src_r, 0);
				ddst[0] = d3;
				ddst[1] = d4;
				ddst += 2;
			}
		}
	}

	dst += (even_length << 1);
	src_r += even_length;
	src_i += even_length;

	if (odd) {
		(*dst++) = (*src_i++);
	}

	for (i = 0; i < rest_64; i++) {
		(*dst++) = (*src_r++);
		(*dst++) = (*src_i++);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMerge_S8C_S8(
	mlib_s8 *z,
	const mlib_s8 *r,
	const mlib_s8 *i,
	mlib_s32 n)
{
	return __mlib_VectorMerge_U8C_U8((mlib_u8 *)z, (mlib_u8 *)r,
		(mlib_u8 *)i, n);
}

/* *********************************************************** */

static void
mlib_VectorMerge_S16C_S16_ge(
	mlib_d64 *ddst,
	const mlib_s16 *src_r,
	const mlib_s16 *src_i,
	mlib_s32 len_64)
{
/* mlib_d64 pointer to real source vector */
	mlib_d64 *dsrc_r;

/* mlib_d64 pointer to imaginary source vector */
	mlib_d64 *dsrc_i;
	mlib_s32 i;
	mlib_d64 d1, d2, d1_r, d1_i, d2_r, d2_i, d3_i, d3_r, d5, d6;
	mlib_d64 d3, d4, dr, dr1;
	mlib_d64 dmask = vis_to_double_dup(0xFFFF);
	mlib_f32 f11 = vis_to_float(0x10001),
		f1010 = vis_to_float(0x1000100);
	mlib_d64 dd0, dd1;
	mlib_u8 offset_i = (mlib_addr)src_i & 0x7,
		offset_r = (mlib_addr)src_r & 0x7;

	if (offset_r == 0 || offset_i == 0) {
		if (offset_r == 0) {
			dsrc_i = (mlib_d64 *)vis_alignaddr((void *)src_i, 0);
			dsrc_r = (mlib_d64 *)src_r;

			d2_i = (*dsrc_i++);
#pragma pipeloop(0)
			for (i = 0; i < len_64 - 1; i += 2) {
				d1 = dsrc_r[0];
				d1_i = dsrc_i[0];
				d2 = vis_faligndata(d2_i, d1_i);
				d3 = dsrc_r[1];
				d2_i = vis_ld_d64_nf(dsrc_i + 1);
				d4 = vis_faligndata(d1_i, d2_i);

				d5 = vis_fpmerge(vis_read_hi(d1),
					vis_read_lo(d1));
				d5 = vis_fpmerge(vis_read_hi(d5),
					vis_read_lo(d5));
				d6 = vis_fpmerge(vis_read_hi(d2),
					vis_read_lo(d2));
				d6 = vis_fpmerge(vis_read_hi(d6),
					vis_read_lo(d6));
				dr = vis_fpmerge(vis_read_hi(d5),
					vis_read_hi(d6));
				dr1 = vis_fpmerge(vis_read_lo(d5),
					vis_read_lo(d6));
				ddst[0] =
					vis_fpmerge(vis_read_hi(dr),
					vis_read_hi(dr1));
				ddst[1] =
					vis_fpmerge(vis_read_lo(dr),
					vis_read_lo(dr1));

				dd0 = vis_fmuld8sux16(f1010, vis_read_hi(d3));
				dd1 = vis_fmuld8sux16(f1010, vis_read_lo(d3));
				dr = vis_fmuld8ulx16(f11, vis_read_hi(d4));
				dr1 = vis_fmuld8ulx16(f11, vis_read_lo(d4));
				dd0 = vis_fpack32(dd0, dmask);
				dd1 = vis_fpack32(dd1, dmask);
				ddst[2] = vis_for(vis_fand(dmask, dr), dd0);
				ddst[3] = dr1;
				vis_pst_16(dd1, ddst + 3, 0xa);

				dsrc_r += 2;
				dsrc_i += 2;
				ddst += 4;
			}

/*
 * next pixels
 */

			if (i < len_64) {
				d1 = vis_ld_d64_nf(dsrc_r); dsrc_r++;
				d1_i = vis_ld_d64_nf(dsrc_i); dsrc_i++;
				d2 = vis_faligndata(d2_i, d1_i);

				d5 = vis_fpmerge(vis_read_hi(d1),
					vis_read_lo(d1));
				d5 = vis_fpmerge(vis_read_hi(d5),
					vis_read_lo(d5));
				d6 = vis_fpmerge(vis_read_hi(d2),
					vis_read_lo(d2));
				d6 = vis_fpmerge(vis_read_hi(d6),
					vis_read_lo(d6));
				dr = vis_fpmerge(vis_read_hi(d5),
					vis_read_hi(d6));
				dr1 = vis_fpmerge(vis_read_lo(d5),
					vis_read_lo(d6));
				ddst[0] =
					vis_fpmerge(vis_read_hi(dr),
					vis_read_hi(dr1));
				ddst[1] =
					vis_fpmerge(vis_read_lo(dr),
					vis_read_lo(dr1));
				ddst += 2;
				i += 1;
			}
		} else {
			dsrc_r = (mlib_d64 *)vis_alignaddr((void *)src_r, 0);
			dsrc_i = (mlib_d64 *)src_i;

			d2_r = (*dsrc_r++);
#pragma pipeloop(0)
			for (i = 0; i < len_64 - 1; i += 2) {
				d2 = dsrc_i[0];
				d4 = dsrc_i[1];

				d1_r = dsrc_r[0];
				d1 = vis_faligndata(d2_r, d1_r);
				d2_r = vis_ld_d64_nf(dsrc_r + 1);
				d3 = vis_faligndata(d1_r, d2_r);

				d5 = vis_fpmerge(vis_read_hi(d1),
					vis_read_lo(d1));
				d5 = vis_fpmerge(vis_read_hi(d5),
					vis_read_lo(d5));
				d6 = vis_fpmerge(vis_read_hi(d2),
					vis_read_lo(d2));
				d6 = vis_fpmerge(vis_read_hi(d6),
					vis_read_lo(d6));
				dr = vis_fpmerge(vis_read_hi(d5),
					vis_read_hi(d6));
				dr1 = vis_fpmerge(vis_read_lo(d5),
					vis_read_lo(d6));
				ddst[0] =
					vis_fpmerge(vis_read_hi(dr),
					vis_read_hi(dr1));
				ddst[1] =
					vis_fpmerge(vis_read_lo(dr),
					vis_read_lo(dr1));

				dd0 = vis_fmuld8sux16(f1010, vis_read_hi(d3));
				dd1 = vis_fmuld8sux16(f1010, vis_read_lo(d3));
				dr = vis_fmuld8ulx16(f11, vis_read_hi(d4));
				dr1 = vis_fmuld8ulx16(f11, vis_read_lo(d4));
				dd0 = vis_fpack32(dd0, dmask);
				dd1 = vis_fpack32(dd1, dmask);
				ddst[2] = vis_for(vis_fand(dmask, dr), dd0);
				ddst[3] = dr1;
				vis_pst_16(dd1, ddst + 3, 0xa);

				dsrc_r += 2;
				dsrc_i += 2;
				ddst += 4;
			}

/*
 * next pixels
 */

			if (i < len_64) {
				d2 = vis_ld_d64_nf(dsrc_i); dsrc_i++;
				d1_r = vis_ld_d64_nf(dsrc_r); dsrc_r++;
				d1 = vis_faligndata(d2_r, d1_r);

				d5 = vis_fpmerge(vis_read_hi(d1),
					vis_read_lo(d1));
				d5 = vis_fpmerge(vis_read_hi(d5),
					vis_read_lo(d5));
				d6 = vis_fpmerge(vis_read_hi(d2),
					vis_read_lo(d2));
				d6 = vis_fpmerge(vis_read_hi(d6),
					vis_read_lo(d6));
				dr = vis_fpmerge(vis_read_hi(d5),
					vis_read_hi(d6));
				dr1 = vis_fpmerge(vis_read_lo(d5),
					vis_read_lo(d6));
				ddst[0] =
					vis_fpmerge(vis_read_hi(dr),
					vis_read_hi(dr1));
				ddst[1] =
					vis_fpmerge(vis_read_lo(dr),
					vis_read_lo(dr1));
				ddst += 2;
				i += 1;
			}
		}
	} else {
		switch (offset_r) {
		    case 2:
			    switch (offset_i) {
				case 4:
					dsrc_i = (mlib_d64 *)vis_alignaddr((void
						*)src_i, 0);
					dsrc_r = (mlib_d64 *)vis_alignaddr((void
						*)src_r, 0);

					d2_r = (*dsrc_r++);
					d2_i = (*dsrc_i++);
#pragma pipeloop(0)
					for (i = 0; i < len_64 - 1; i += 2) {
						d1_i = dsrc_i[0];
						d4 = dsrc_i[1];

						d1_r = dsrc_r[0];
						d1 = vis_faligndata(d2_r, d1_r);
						d2_r = vis_ld_d64_nf(dsrc_r +
							1);
						d3 = vis_faligndata(d1_r, d2_r);

						d5 = vis_fpmerge(vis_read_hi
							(d1), vis_read_lo(d1));
						d5 = vis_fpmerge(vis_read_hi
							(d5), vis_read_lo(d5));
						d6 = vis_fpmerge(vis_read_lo
							(d2_i),
							vis_read_hi(d1_i));
						d6 = vis_fpmerge(vis_read_hi
							(d6), vis_read_lo(d6));
						dr = vis_fpmerge(vis_read_hi
							(d5), vis_read_hi(d6));
						dr1 = vis_fpmerge(vis_read_lo
							(d5), vis_read_lo(d6));
						ddst[0] =
							vis_fpmerge(vis_read_hi
							(dr), vis_read_hi(dr1));
						ddst[1] =
							vis_fpmerge(vis_read_lo
							(dr), vis_read_lo(dr1));

						dd0 = vis_fmuld8sux16(f1010,
							vis_read_hi(d3));
						dd1 = vis_fmuld8sux16(f1010,
							vis_read_lo(d3));
						dr = vis_fmuld8ulx16(f11,
							vis_read_lo(d1_i));
						dr1 = vis_fmuld8ulx16(f11,
							vis_read_hi(d4));
						dd0 = vis_fpack32(dd0, dmask);
						dd1 = vis_fpack32(dd1, dmask);
						ddst[2] =
							vis_for(vis_fand(dmask,
							dr), dd0);
						ddst[3] = dr1;
						vis_pst_16(dd1, ddst + 3, 0xa);

						dsrc_r += 2;
						dsrc_i += 2;
						ddst += 4;
						d2_i = d4;
					}

/*
 * next pixels
 */

					if (i < len_64) {
						d1_i = vis_ld_d64_nf(dsrc_i);
						dsrc_i++;
						d1_r = vis_ld_d64_nf(dsrc_r);
						dsrc_r++;
						d1 = vis_faligndata(d2_r, d1_r);

						d5 = vis_fpmerge(vis_read_hi
							(d1), vis_read_lo(d1));
						d5 = vis_fpmerge(vis_read_hi
							(d5), vis_read_lo(d5));
						d6 = vis_fpmerge(vis_read_lo
							(d2_i),
							vis_read_hi(d1_i));
						d6 = vis_fpmerge(vis_read_hi
							(d6), vis_read_lo(d6));
						dr = vis_fpmerge(vis_read_hi
							(d5), vis_read_hi(d6));
						dr1 = vis_fpmerge(vis_read_lo
							(d5), vis_read_lo(d6));
						ddst[0] =
							vis_fpmerge(vis_read_hi
							(dr), vis_read_hi(dr1));
						ddst[1] =
							vis_fpmerge(vis_read_lo
							(dr), vis_read_lo(dr1));
						ddst += 2;
						i += 1;
					}

					break;
				case 6:
					dsrc_i = (mlib_d64 *)vis_alignaddr((void
						*)src_i, 0);
					dsrc_r = (mlib_d64 *)vis_alignaddr((void
						*)src_r, 0);

					d2_r = (*dsrc_r++);
					d2_i = vis_ld_d64_nf(dsrc_i); dsrc_i++;
					d1_i = vis_ld_d64_nf(dsrc_i); dsrc_i++;
					d2 = vis_faligndata(d2_i, d1_i);

#pragma pipeloop(0)
					for (i = 0; i < len_64 - 1; i += 2) {
						d1_r = dsrc_r[0];
						d1 = vis_faligndata(d2_r, d1_r);
						d2_r = vis_ld_d64_nf(dsrc_r +
							1);
						d3 = vis_faligndata(d1_r, d2_r);
						d2_i = vis_ld_d64_nf(dsrc_i);
						d4 = vis_faligndata(d1_i, d2_i);
						d1_i = vis_ld_d64_nf(dsrc_i +
							1);
						d3_i = vis_faligndata(d2_i,
							d1_i);

						d5 = vis_fpmerge(vis_read_hi
							(d1), vis_read_lo(d1));
						d5 = vis_fpmerge(vis_read_hi
							(d5), vis_read_lo(d5));
						d6 = vis_fpmerge(vis_read_lo
							(d2), vis_read_hi(d4));
						d6 = vis_fpmerge(vis_read_hi
							(d6), vis_read_lo(d6));
						dr = vis_fpmerge(vis_read_hi
							(d5), vis_read_hi(d6));
						dr1 = vis_fpmerge(vis_read_lo
							(d5), vis_read_lo(d6));
						ddst[0] =
							vis_fpmerge(vis_read_hi
							(dr), vis_read_hi(dr1));
						ddst[1] =
							vis_fpmerge(vis_read_lo
							(dr), vis_read_lo(dr1));

						dd0 = vis_fmuld8sux16(f1010,
							vis_read_hi(d3));
						dd1 = vis_fmuld8sux16(f1010,
							vis_read_lo(d3));
						dr = vis_fmuld8ulx16(f11,
							vis_read_lo(d4));
						dr1 = vis_fmuld8ulx16(f11,
							vis_read_hi(d3_i));
						dd0 = vis_fpack32(dd0, dmask);
						dd1 = vis_fpack32(dd1, dmask);
						ddst[2] =
							vis_for(vis_fand(dmask,
							dr), dd0);
						ddst[3] = dr1;
						vis_pst_16(dd1, ddst + 3, 0xa);

						dsrc_r += 2;
						dsrc_i += 2;
						ddst += 4;
						d2 = d3_i;
					}

/*
 * next pixels
 */
					if (i < len_64) {
						d1_r = vis_ld_d64_nf(dsrc_r);
						dsrc_r++;
						d1 = vis_faligndata(d2_r, d1_r);
						d2_i = vis_ld_d64_nf(dsrc_i);
						d4 = vis_faligndata(d1_i, d2_i);

						d5 = vis_fpmerge(vis_read_hi
							(d1), vis_read_lo(d1));
						d5 = vis_fpmerge(vis_read_hi
							(d5), vis_read_lo(d5));
						d6 = vis_fpmerge(vis_read_lo
							(d2), vis_read_hi(d4));
						d6 = vis_fpmerge(vis_read_hi
							(d6), vis_read_lo(d6));
						dr = vis_fpmerge(vis_read_hi
							(d5), vis_read_hi(d6));
						dr1 = vis_fpmerge(vis_read_lo
							(d5), vis_read_lo(d6));
						ddst[0] =
							vis_fpmerge(vis_read_hi
							(dr), vis_read_hi(dr1));
						ddst[1] =
							vis_fpmerge(vis_read_lo
							(dr), vis_read_lo(dr1));
						ddst += 2;
						i += 1;
					}

					break;
			    }

			    break;
		    case 4:
			    switch (offset_i) {
				case 2:
					dsrc_r = (mlib_d64 *)vis_alignaddr((void
						*)src_r, 0);
					dsrc_i = (mlib_d64 *)vis_alignaddr((void
						*)src_i, 0);

					d2_r = (*dsrc_r++);
					d2_i = (*dsrc_i++);
#pragma pipeloop(0)
					for (i = 0; i < len_64 - 1; i += 2) {
						d1_r = dsrc_r[0];
						d3 = vis_ld_d64_nf(dsrc_r+1);

						d1_i = vis_ld_d64_nf(dsrc_i);
						d2 = vis_faligndata(d2_i, d1_i);
						d2_i = vis_ld_d64_nf(dsrc_i +
							1);
						d4 = vis_faligndata(d1_i, d2_i);

						d5 = vis_fpmerge(vis_read_lo
							(d2_r),
							vis_read_hi(d1_r));
						d5 = vis_fpmerge(vis_read_hi
							(d5), vis_read_lo(d5));
						d6 = vis_fpmerge(vis_read_hi
							(d2), vis_read_lo(d2));
						d6 = vis_fpmerge(vis_read_hi
							(d6), vis_read_lo(d6));
						dr = vis_fpmerge(vis_read_hi
							(d5), vis_read_hi(d6));
						dr1 = vis_fpmerge(vis_read_lo
							(d5), vis_read_lo(d6));
						ddst[0] =
							vis_fpmerge(vis_read_hi
							(dr), vis_read_hi(dr1));
						ddst[1] =
							vis_fpmerge(vis_read_lo
							(dr), vis_read_lo(dr1));

						dd0 = vis_fmuld8sux16(f1010,
							vis_read_lo(d1_r));
						dd1 = vis_fmuld8sux16(f1010,
							vis_read_hi(d3));
						dr = vis_fmuld8ulx16(f11,
							vis_read_hi(d4));
						dr1 = vis_fmuld8ulx16(f11,
							vis_read_lo(d4));
						dd0 = vis_fpack32(dd0, dmask);
						dd1 = vis_fpack32(dd1, dmask);
						ddst[2] =
							vis_for(vis_fand(dmask,
							dr), dd0);
						ddst[3] = dr1;
						vis_pst_16(dd1, ddst + 3, 0xa);

						dsrc_r += 2;
						dsrc_i += 2;
						ddst += 4;
						d2_r = d3;
					}

/*
 * next pixels
 */
					if (i < len_64) {
						d1_i = vis_ld_d64_nf(dsrc_i);
						dsrc_i++;
						d1_r = vis_ld_d64_nf(dsrc_r);
						dsrc_r++;
						d2 = vis_faligndata(d2_i, d1_i);

						d5 = vis_fpmerge(vis_read_lo
							(d2_r),
							vis_read_hi(d1_r));
						d5 = vis_fpmerge(vis_read_hi
							(d5), vis_read_lo(d5));
						d6 = vis_fpmerge(vis_read_hi
							(d2), vis_read_lo(d2));
						d6 = vis_fpmerge(vis_read_hi
							(d6), vis_read_lo(d6));
						dr = vis_fpmerge(vis_read_hi
							(d5), vis_read_hi(d6));
						dr1 = vis_fpmerge(vis_read_lo
							(d5), vis_read_lo(d6));
						ddst[0] =
							vis_fpmerge(vis_read_hi
							(dr), vis_read_hi(dr1));
						ddst[1] =
							vis_fpmerge(vis_read_lo
							(dr), vis_read_lo(dr1));
						ddst += 2;
						i += 1;
					}

					break;
				case 6:
					dsrc_r = (mlib_d64 *)vis_alignaddr((void
						*)src_r, 0);
					dsrc_i = (mlib_d64 *)vis_alignaddr((void
						*)src_i, 0);

					d2_r = (*dsrc_r++);
					d2_i = (*dsrc_i++);
#pragma pipeloop(0)
					for (i = 0; i < len_64 - 1; i += 2) {
						d1_r = dsrc_r[0];
						d3 = vis_ld_d64_nf(dsrc_r + 1);

						d1_i = vis_ld_d64_nf(dsrc_i);
						d2 = vis_faligndata(d2_i, d1_i);
						d2_i = vis_ld_d64_nf(dsrc_i +
							1);
						d4 = vis_faligndata(d1_i, d2_i);

						d5 = vis_fpmerge(vis_read_lo
							(d2_r),
							vis_read_hi(d1_r));
						d5 = vis_fpmerge(vis_read_hi
							(d5), vis_read_lo(d5));
						d6 = vis_fpmerge(vis_read_hi
							(d2), vis_read_lo(d2));
						d6 = vis_fpmerge(vis_read_hi
							(d6), vis_read_lo(d6));
						dr = vis_fpmerge(vis_read_hi
							(d5), vis_read_hi(d6));
						dr1 = vis_fpmerge(vis_read_lo
							(d5), vis_read_lo(d6));
						ddst[0] =
							vis_fpmerge(vis_read_hi
							(dr), vis_read_hi(dr1));
						ddst[1] =
							vis_fpmerge(vis_read_lo
							(dr), vis_read_lo(dr1));

						dd0 = vis_fmuld8sux16(f1010,
							vis_read_lo(d1_r));
						dd1 = vis_fmuld8sux16(f1010,
							vis_read_hi(d3));
						dr = vis_fmuld8ulx16(f11,
							vis_read_hi(d4));
						dr1 = vis_fmuld8ulx16(f11,
							vis_read_lo(d4));
						dd0 = vis_fpack32(dd0, dmask);
						dd1 = vis_fpack32(dd1, dmask);
						ddst[2] =
							vis_for(vis_fand(dmask,
							dr), dd0);
						ddst[3] = dr1;
						vis_pst_16(dd1, ddst + 3, 0xa);

						dsrc_r += 2;
						dsrc_i += 2;
						ddst += 4;
						d2_r = d3;
					}

/*
 * next pixels
 */

					if (i < len_64) {
						d1_i = vis_ld_d64_nf(dsrc_i);
						dsrc_i++;
						d1_r = vis_ld_d64_nf(dsrc_r);
						dsrc_r++;
						d2 = vis_faligndata(d2_i, d1_i);

						d5 = vis_fpmerge(vis_read_lo
							(d2_r),
							vis_read_hi(d1_r));
						d5 = vis_fpmerge(vis_read_hi
							(d5), vis_read_lo(d5));
						d6 = vis_fpmerge(vis_read_hi
							(d2), vis_read_lo(d2));
						d6 = vis_fpmerge(vis_read_hi
							(d6), vis_read_lo(d6));
						dr = vis_fpmerge(vis_read_hi
							(d5), vis_read_hi(d6));
						dr1 = vis_fpmerge(vis_read_lo
							(d5), vis_read_lo(d6));
						ddst[0] =
							vis_fpmerge(vis_read_hi
							(dr), vis_read_hi(dr1));
						ddst[1] =
							vis_fpmerge(vis_read_lo
							(dr), vis_read_lo(dr1));
						ddst += 2;
						i += 1;
					}

					break;
			    }

			    break;
		    case 6:
			    switch (offset_i) {
				case 2:
					dsrc_r = (mlib_d64 *)vis_alignaddr((void
						*)src_r, 0);
					dsrc_i = (mlib_d64 *)vis_alignaddr((void
						*)src_i, 0);

					d2_i = (*dsrc_i++);
					d2_r = (*dsrc_r++);
					d1_r = vis_ld_d64_nf(dsrc_r); dsrc_r++;
					d1 = vis_faligndata(d2_r, d1_r);

#pragma pipeloop(0)
					for (i = 0; i < len_64 - 1; i += 2) {
						d1_i = vis_ld_d64_nf(dsrc_i);
						d2 = vis_faligndata(d2_i, d1_i);
						d2_i = vis_ld_d64_nf(dsrc_i +
							1);
						d4 = vis_faligndata(d1_i, d2_i);
						d2_r = vis_ld_d64_nf(dsrc_r);
						d3 = vis_faligndata(d1_r, d2_r);
						d1_r = vis_ld_d64_nf(dsrc_r +
							1);
						d3_r = vis_faligndata(d2_r,
							d1_r);

						d5 = vis_fpmerge(vis_read_lo
							(d1), vis_read_hi(d3));
						d5 = vis_fpmerge(vis_read_hi
							(d5), vis_read_lo(d5));
						d6 = vis_fpmerge(vis_read_hi
							(d2), vis_read_lo(d2));
						d6 = vis_fpmerge(vis_read_hi
							(d6), vis_read_lo(d6));
						dr = vis_fpmerge(vis_read_hi
							(d5), vis_read_hi(d6));
						dr1 = vis_fpmerge(vis_read_lo
							(d5), vis_read_lo(d6));
						ddst[0] =
							vis_fpmerge(vis_read_hi
							(dr), vis_read_hi(dr1));
						ddst[1] =
							vis_fpmerge(vis_read_lo
							(dr), vis_read_lo(dr1));

						dd0 = vis_fmuld8sux16(f1010,
							vis_read_lo(d3));
						dd1 = vis_fmuld8sux16(f1010,
							vis_read_hi(d3_r));
						dr = vis_fmuld8ulx16(f11,
							vis_read_hi(d4));
						dr1 = vis_fmuld8ulx16(f11,
							vis_read_lo(d4));
						dd0 = vis_fpack32(dd0, dmask);
						dd1 = vis_fpack32(dd1, dmask);
						ddst[2] =
							vis_for(vis_fand(dmask,
							dr), dd0);
						ddst[3] = dr1;
						vis_pst_16(dd1, ddst + 3, 0xa);

						dsrc_r += 2;
						dsrc_i += 2;
						ddst += 4;
						d1 = d3_r;
					}

/*
 * next pixels
 */
					if (i < len_64) {
						d1_i = vis_ld_d64_nf(dsrc_i);
						dsrc_i++;
						d2 = vis_faligndata(d2_i, d1_i);
						d2_r = vis_ld_d64_nf(dsrc_r);
						d3 = vis_faligndata(d1_r, d2_r);

						d5 = vis_fpmerge(vis_read_lo
							(d1), vis_read_hi(d3));
						d5 = vis_fpmerge(vis_read_hi
							(d5), vis_read_lo(d5));
						d6 = vis_fpmerge(vis_read_hi
							(d2), vis_read_lo(d2));
						d6 = vis_fpmerge(vis_read_hi
							(d6), vis_read_lo(d6));
						dr = vis_fpmerge(vis_read_hi
							(d5), vis_read_hi(d6));
						dr1 = vis_fpmerge(vis_read_lo
							(d5), vis_read_lo(d6));
						ddst[0] =
							vis_fpmerge(vis_read_hi
							(dr), vis_read_hi(dr1));
						ddst[1] =
							vis_fpmerge(vis_read_lo
							(dr), vis_read_lo(dr1));
						ddst += 2;
						i += 1;
					}

					break;
				case 4:
					dsrc_i = (mlib_d64 *)vis_alignaddr((void
						*)src_i, 0);
					dsrc_r = (mlib_d64 *)vis_alignaddr((void
						*)src_r, 0);

					d2_r = (*dsrc_r++);
					d2_i = (*dsrc_i++);
#pragma pipeloop(0)
					for (i = 0; i < len_64 - 1; i += 2) {
						d1_i = dsrc_i[0];
						d4 = vis_ld_d64_nf(dsrc_i + 1);

						d1_r = vis_ld_d64_nf(dsrc_r);
						d1 = vis_faligndata(d2_r, d1_r);
						d2_r = vis_ld_d64_nf(dsrc_r +
							1);
						d3 = vis_faligndata(d1_r, d2_r);

						d5 = vis_fpmerge(vis_read_hi
							(d1), vis_read_lo(d1));
						d5 = vis_fpmerge(vis_read_hi
							(d5), vis_read_lo(d5));
						d6 = vis_fpmerge(vis_read_lo
							(d2_i),
							vis_read_hi(d1_i));
						d6 = vis_fpmerge(vis_read_hi
							(d6), vis_read_lo(d6));
						dr = vis_fpmerge(vis_read_hi
							(d5), vis_read_hi(d6));
						dr1 = vis_fpmerge(vis_read_lo
							(d5), vis_read_lo(d6));
						ddst[0] =
							vis_fpmerge(vis_read_hi
							(dr), vis_read_hi(dr1));
						ddst[1] =
							vis_fpmerge(vis_read_lo
							(dr), vis_read_lo(dr1));

						dd0 = vis_fmuld8sux16(f1010,
							vis_read_hi(d3));
						dd1 = vis_fmuld8sux16(f1010,
							vis_read_lo(d3));
						dr = vis_fmuld8ulx16(f11,
							vis_read_lo(d1_i));
						dr1 = vis_fmuld8ulx16(f11,
							vis_read_hi(d4));
						dd0 = vis_fpack32(dd0, dmask);
						dd1 = vis_fpack32(dd1, dmask);
						ddst[2] =
							vis_for(vis_fand(dmask,
							dr), dd0);
						ddst[3] = dr1;
						vis_pst_16(dd1, ddst + 3, 0xa);

						dsrc_r += 2;
						dsrc_i += 2;
						ddst += 4;
						d2_i = d4;
					}

/*
 * next pixels
 */
					if (i < len_64) {
						d1_r = vis_ld_d64_nf(dsrc_r);
						dsrc_r++;
						d1_i = vis_ld_d64_nf(dsrc_i);
						dsrc_i++;
						d1 = vis_faligndata(d2_r, d1_r);

						d5 = vis_fpmerge(vis_read_hi
							(d1), vis_read_lo(d1));
						d5 = vis_fpmerge(vis_read_hi
							(d5), vis_read_lo(d5));
						d6 = vis_fpmerge(vis_read_lo
							(d2_i),
							vis_read_hi(d1_i));
						d6 = vis_fpmerge(vis_read_hi
							(d6), vis_read_lo(d6));
						dr = vis_fpmerge(vis_read_hi
							(d5), vis_read_hi(d6));
						dr1 = vis_fpmerge(vis_read_lo
							(d5), vis_read_lo(d6));
						ddst[0] =
							vis_fpmerge(vis_read_hi
							(dr), vis_read_hi(dr1));
						ddst[1] =
							vis_fpmerge(vis_read_lo
							(dr), vis_read_lo(dr1));
						ddst += 2;
						i += 1;
					}

					break;
			    }

			    break;
		}
	}
}

/* *********************************************************** */

static void
mlib_mlib_VectorMerge_S16C_S16_ge_odd(
	mlib_d64 *ddst,
	const mlib_s16 *src_r,
	const mlib_s16 *src_i,
	mlib_s32 len_64)
{
/* mlib_d64 pointer to real source vector */
	mlib_d64 *dsrc_r;

/* mlib_d64 pointer to imaginary source vector */
	mlib_d64 *dsrc_i;
	mlib_s32 i;
	mlib_d64 d1, d2, d1_r, d1_i, d2_r, d2_i, d3_i, d3_r, d5, d6;
	mlib_d64 d3, d4, dr, dr1;
	mlib_d64 dmask = vis_to_double_dup(0xFFFF);
	mlib_f32 f11 = vis_to_float(0x10001),
		f1010 = vis_to_float(0x1000100);
	mlib_d64 dd0, dd1;

	mlib_u8 offset_i = (mlib_addr)src_i & 0x7,
		offset_r = (mlib_addr)src_r & 0x7;

	if (offset_r == 0 || offset_i == 0) {
		if (offset_r == 0) {
			dsrc_i = (mlib_d64 *)vis_alignaddr((void *)src_i, 0);
			dsrc_r = (mlib_d64 *)src_r;

			d2_i = (*dsrc_i++);
#pragma pipeloop(0)
			for (i = 0; i < len_64 - 1; i += 2) {
				d2 = dsrc_r[0];
				d1_i = vis_ld_d64_nf(dsrc_i);
				d1 = vis_faligndata(d2_i, d1_i);
				d4 = dsrc_r[1];
				d2_i = vis_ld_d64_nf(dsrc_i + 1);
				d3 = vis_faligndata(d1_i, d2_i);

				d5 = vis_fpmerge(vis_read_hi(d1),
					vis_read_lo(d1));
				d5 = vis_fpmerge(vis_read_hi(d5),
					vis_read_lo(d5));
				d6 = vis_fpmerge(vis_read_hi(d2),
					vis_read_lo(d2));
				d6 = vis_fpmerge(vis_read_hi(d6),
					vis_read_lo(d6));
				dr = vis_fpmerge(vis_read_hi(d5),
					vis_read_hi(d6));
				dr1 = vis_fpmerge(vis_read_lo(d5),
					vis_read_lo(d6));
				ddst[0] =
					vis_fpmerge(vis_read_hi(dr),
					vis_read_hi(dr1));
				ddst[1] =
					vis_fpmerge(vis_read_lo(dr),
					vis_read_lo(dr1));

				dd0 = vis_fmuld8sux16(f1010, vis_read_hi(d3));
				dd1 = vis_fmuld8sux16(f1010, vis_read_lo(d3));
				dr = vis_fmuld8ulx16(f11, vis_read_hi(d4));
				dr1 = vis_fmuld8ulx16(f11, vis_read_lo(d4));
				dd0 = vis_fpack32(dd0, dmask);
				dd1 = vis_fpack32(dd1, dmask);
				ddst[2] = vis_for(vis_fand(dmask, dr), dd0);
				ddst[3] = dr1;
				vis_pst_16(dd1, ddst + 3, 0xa);

				dsrc_r += 2;
				dsrc_i += 2;
				ddst += 4;
			}

/*
 * next pixels
 */

			if (i < len_64) {
				d2 = vis_ld_d64_nf(dsrc_r); dsrc_r++;
				d1_i = vis_ld_d64_nf(dsrc_i); dsrc_i++;
				d1 = vis_faligndata(d2_i, d1_i);

				d5 = vis_fpmerge(vis_read_hi(d1),
					vis_read_lo(d1));
				d5 = vis_fpmerge(vis_read_hi(d5),
					vis_read_lo(d5));
				d6 = vis_fpmerge(vis_read_hi(d2),
					vis_read_lo(d2));
				d6 = vis_fpmerge(vis_read_hi(d6),
					vis_read_lo(d6));
				dr = vis_fpmerge(vis_read_hi(d5),
					vis_read_hi(d6));
				dr1 = vis_fpmerge(vis_read_lo(d5),
					vis_read_lo(d6));
				ddst[0] =
					vis_fpmerge(vis_read_hi(dr),
					vis_read_hi(dr1));
				ddst[1] =
					vis_fpmerge(vis_read_lo(dr),
					vis_read_lo(dr1));
				ddst += 2;
				i += 1;
			}
		} else {
			dsrc_r = (mlib_d64 *)vis_alignaddr((void *)src_r, 0);
			dsrc_i = (mlib_d64 *)src_i;

			d2_r = (*dsrc_r++);
#pragma pipeloop(0)
			for (i = 0; i < len_64 - 1; i += 2) {
				d1 = dsrc_i[0];
				d3 = dsrc_i[1];

				d1_r = dsrc_r[0];
				d2 = vis_faligndata(d2_r, d1_r);
				d2_r = vis_ld_d64_nf(dsrc_r + 1);
				d4 = vis_faligndata(d1_r, d2_r);

				d5 = vis_fpmerge(vis_read_hi(d1),
					vis_read_lo(d1));
				d5 = vis_fpmerge(vis_read_hi(d5),
					vis_read_lo(d5));
				d6 = vis_fpmerge(vis_read_hi(d2),
					vis_read_lo(d2));
				d6 = vis_fpmerge(vis_read_hi(d6),
					vis_read_lo(d6));
				dr = vis_fpmerge(vis_read_hi(d5),
					vis_read_hi(d6));
				dr1 = vis_fpmerge(vis_read_lo(d5),
					vis_read_lo(d6));
				ddst[0] =
					vis_fpmerge(vis_read_hi(dr),
					vis_read_hi(dr1));
				ddst[1] =
					vis_fpmerge(vis_read_lo(dr),
					vis_read_lo(dr1));

				dd0 = vis_fmuld8sux16(f1010, vis_read_hi(d3));
				dd1 = vis_fmuld8sux16(f1010, vis_read_lo(d3));
				dr = vis_fmuld8ulx16(f11, vis_read_hi(d4));
				dr1 = vis_fmuld8ulx16(f11, vis_read_lo(d4));
				dd0 = vis_fpack32(dd0, dmask);
				dd1 = vis_fpack32(dd1, dmask);
				ddst[2] = vis_for(vis_fand(dmask, dr), dd0);
				ddst[3] = dr1;
				vis_pst_16(dd1, ddst + 3, 0xa);

				dsrc_r += 2;
				dsrc_i += 2;
				ddst += 4;
			}

/*
 * next pixels
 */

			if (i < len_64) {
				d1 = vis_ld_d64_nf(dsrc_i); dsrc_i++;
				d1_r = vis_ld_d64_nf(dsrc_r); dsrc_r++;
				d2 = vis_faligndata(d2_r, d1_r);

				d5 = vis_fpmerge(vis_read_hi(d1),
					vis_read_lo(d1));
				d5 = vis_fpmerge(vis_read_hi(d5),
					vis_read_lo(d5));
				d6 = vis_fpmerge(vis_read_hi(d2),
					vis_read_lo(d2));
				d6 = vis_fpmerge(vis_read_hi(d6),
					vis_read_lo(d6));
				dr = vis_fpmerge(vis_read_hi(d5),
					vis_read_hi(d6));
				dr1 = vis_fpmerge(vis_read_lo(d5),
					vis_read_lo(d6));
				ddst[0] =
					vis_fpmerge(vis_read_hi(dr),
					vis_read_hi(dr1));
				ddst[1] =
					vis_fpmerge(vis_read_lo(dr),
					vis_read_lo(dr1));
				ddst += 2;
				i += 1;
			}
		}
	} else {
		switch (offset_r) {
		    case 2:
			    switch (offset_i) {
				case 4:
					dsrc_i = (mlib_d64 *)vis_alignaddr((void
						*)src_i, 0);
					dsrc_r = (mlib_d64 *)vis_alignaddr((void
						*)src_r, 0);

					d2_r = (*dsrc_r++);
					d2_i = (*dsrc_i++);
#pragma pipeloop(0)
					for (i = 0; i < len_64 - 1; i += 2) {
						d1_i = dsrc_i[0];
						d4 = vis_ld_d64_nf(dsrc_i + 1);

						d1_r = dsrc_r[0];
						d1 = vis_faligndata(d2_r, d1_r);
						d2_r = vis_ld_d64_nf(dsrc_r +
							1);
						d3 = vis_faligndata(d1_r, d2_r);

						d6 = vis_fpmerge(vis_read_hi
							(d1), vis_read_lo(d1));
						d6 = vis_fpmerge(vis_read_hi
							(d6), vis_read_lo(d6));
						d5 = vis_fpmerge(vis_read_lo
							(d2_i),
							vis_read_hi(d1_i));
						d5 = vis_fpmerge(vis_read_hi
							(d5), vis_read_lo(d5));
						dr = vis_fpmerge(vis_read_hi
							(d5), vis_read_hi(d6));
						dr1 = vis_fpmerge(vis_read_lo
							(d5), vis_read_lo(d6));
						ddst[0] =
							vis_fpmerge(vis_read_hi
							(dr), vis_read_hi(dr1));
						ddst[1] =
							vis_fpmerge(vis_read_lo
							(dr), vis_read_lo(dr1));

						dd0 = vis_fmuld8sux16(f1010,
							vis_read_lo(d1_i));
						dd1 = vis_fmuld8sux16(f1010,
							vis_read_hi(d4));
						dr = vis_fmuld8ulx16(f11,
							vis_read_hi(d3));
						dr1 = vis_fmuld8ulx16(f11,
							vis_read_lo(d3));
						dd0 = vis_fpack32(dd0, dmask);
						dd1 = vis_fpack32(dd1, dmask);
						ddst[2] =
							vis_for(vis_fand(dmask,
							dr), dd0);
						ddst[3] = dr1;
						vis_pst_16(dd1, ddst + 3, 0xa);

						dsrc_r += 2;
						dsrc_i += 2;
						ddst += 4;
						d2_i = d4;
					}

/*
 * next pixels
 */

					if (i < len_64) {
						d1_i = vis_ld_d64_nf(dsrc_i);
						dsrc_i++;
						d1_r = vis_ld_d64_nf(dsrc_r);
						dsrc_r++;
						d1 = vis_faligndata(d2_r, d1_r);

						d6 = vis_fpmerge(vis_read_hi
							(d1), vis_read_lo(d1));
						d6 = vis_fpmerge(vis_read_hi
							(d6), vis_read_lo(d6));
						d5 = vis_fpmerge(vis_read_lo
							(d2_i),
							vis_read_hi(d1_i));
						d5 = vis_fpmerge(vis_read_hi
							(d5), vis_read_lo(d5));
						dr = vis_fpmerge(vis_read_hi
							(d5), vis_read_hi(d6));
						dr1 = vis_fpmerge(vis_read_lo
							(d5), vis_read_lo(d6));
						ddst[0] =
							vis_fpmerge(vis_read_hi
							(dr), vis_read_hi(dr1));
						ddst[1] =
							vis_fpmerge(vis_read_lo
							(dr), vis_read_lo(dr1));
						ddst += 2;
						i += 1;
					}

					break;
				case 6:
					dsrc_i = (mlib_d64 *)vis_alignaddr((void
						*)src_i, 0);
					dsrc_r = (mlib_d64 *)vis_alignaddr((void
						*)src_r, 0);

					d2_r = (*dsrc_r++);
					d2_i = (*dsrc_i++);
					d1_i = (*dsrc_i++);
					d2 = vis_faligndata(d2_i, d1_i);

#pragma pipeloop(0)
					for (i = 0; i < len_64 - 1; i += 2) {
						d1_r = dsrc_r[0];
						d1 = vis_faligndata(d2_r, d1_r);
						d2_r = vis_ld_d64_nf(dsrc_r +
							1);
						d3 = vis_faligndata(d1_r, d2_r);
						d2_i = dsrc_i[0];
						d4 = vis_faligndata(d1_i, d2_i);
						d1_i = vis_ld_d64_nf(dsrc_i +
							1);
						d3_i = vis_faligndata(d2_i,
							d1_i);

						d5 = vis_fpmerge(vis_read_lo
							(d2), vis_read_hi(d4));
						d5 = vis_fpmerge(vis_read_hi
							(d5), vis_read_lo(d5));
						d6 = vis_fpmerge(vis_read_hi
							(d1), vis_read_lo(d1));
						d6 = vis_fpmerge(vis_read_hi
							(d6), vis_read_lo(d6));
						dr = vis_fpmerge(vis_read_hi
							(d5), vis_read_hi(d6));
						dr1 = vis_fpmerge(vis_read_lo
							(d5), vis_read_lo(d6));
						ddst[0] =
							vis_fpmerge(vis_read_hi
							(dr), vis_read_hi(dr1));
						ddst[1] =
							vis_fpmerge(vis_read_lo
							(dr), vis_read_lo(dr1));

						dd0 = vis_fmuld8sux16(f1010,
							vis_read_lo(d4));
						dd1 = vis_fmuld8sux16(f1010,
							vis_read_hi(d3_i));
						dr = vis_fmuld8ulx16(f11,
							vis_read_hi(d3));
						dr1 = vis_fmuld8ulx16(f11,
							vis_read_lo(d3));
						dd0 = vis_fpack32(dd0, dmask);
						dd1 = vis_fpack32(dd1, dmask);
						ddst[2] =
							vis_for(vis_fand(dmask,
							dr), dd0);
						ddst[3] = dr1;
						vis_pst_16(dd1, ddst + 3, 0xa);

						dsrc_r += 2;
						dsrc_i += 2;
						ddst += 4;
						d2 = d3_i;
					}

/*
 * next pixels
 */

					if (i < len_64) {
						d1_r = vis_ld_d64_nf(dsrc_r);
						dsrc_r++;
						d1 = vis_faligndata(d2_r, d1_r);
						d2_i = vis_ld_d64_nf(dsrc_i);
						d4 = vis_faligndata(d1_i, d2_i);

						d5 = vis_fpmerge(vis_read_lo
							(d2), vis_read_hi(d4));
						d5 = vis_fpmerge(vis_read_hi
							(d5), vis_read_lo(d5));
						d6 = vis_fpmerge(vis_read_hi
							(d1), vis_read_lo(d1));
						d6 = vis_fpmerge(vis_read_hi
							(d6), vis_read_lo(d6));
						dr = vis_fpmerge(vis_read_hi
							(d5), vis_read_hi(d6));
						dr1 = vis_fpmerge(vis_read_lo
							(d5), vis_read_lo(d6));
						ddst[0] =
							vis_fpmerge(vis_read_hi
							(dr), vis_read_hi(dr1));
						ddst[1] =
							vis_fpmerge(vis_read_lo
							(dr), vis_read_lo(dr1));
						ddst += 2;
						i += 1;
					}

					break;
			    }

			    break;
		    case 4:
			    switch (offset_i) {
				case 2:
					dsrc_r = (mlib_d64 *)vis_alignaddr((void
						*)src_r, 0);
					dsrc_i = (mlib_d64 *)vis_alignaddr((void
						*)src_i, 0);

					d2_r = (*dsrc_r++);
					d2_i = (*dsrc_i++);
#pragma pipeloop(0)
					for (i = 0; i < len_64 - 1; i += 2) {
						d1_r = dsrc_r[0];
						d3 = vis_ld_d64_nf(dsrc_r + 1);

						d1_i = dsrc_i[0];
						d2 = vis_faligndata(d2_i, d1_i);
						d2_i = vis_ld_d64_nf(dsrc_i +
							1);
						d4 = vis_faligndata(d1_i, d2_i);

						d6 = vis_fpmerge(vis_read_lo
							(d2_r),
							vis_read_hi(d1_r));
						d6 = vis_fpmerge(vis_read_hi
							(d6), vis_read_lo(d6));
						d5 = vis_fpmerge(vis_read_hi
							(d2), vis_read_lo(d2));
						d5 = vis_fpmerge(vis_read_hi
							(d5), vis_read_lo(d5));
						dr = vis_fpmerge(vis_read_hi
							(d5), vis_read_hi(d6));
						dr1 = vis_fpmerge(vis_read_lo
							(d5), vis_read_lo(d6));
						ddst[0] =
							vis_fpmerge(vis_read_hi
							(dr), vis_read_hi(dr1));
						ddst[1] =
							vis_fpmerge(vis_read_lo
							(dr), vis_read_lo(dr1));

						dr = vis_fmuld8ulx16(f11,
							vis_read_lo(d1_r));
						dr1 = vis_fmuld8ulx16(f11,
							vis_read_hi(d3));
						dd0 = vis_fmuld8sux16(f1010,
							vis_read_hi(d4));
						dd1 = vis_fmuld8sux16(f1010,
							vis_read_lo(d4));
						dd0 = vis_fpack32(dd0, dmask);
						dd1 = vis_fpack32(dd1, dmask);
						ddst[2] =
							vis_for(vis_fand(dmask,
							dr), dd0);
						ddst[3] = dr1;
						vis_pst_16(dd1, ddst + 3, 0xa);

						dsrc_r += 2;
						dsrc_i += 2;
						ddst += 4;
						d2_r = d3;
					}

/*
 * next pixels
 */

					if (i < len_64) {
						d1_i = vis_ld_d64_nf(dsrc_i);
						dsrc_i++;
						d1_r = vis_ld_d64_nf(dsrc_r);
						dsrc_r++;
						d2 = vis_faligndata(d2_i, d1_i);

						d6 = vis_fpmerge(vis_read_lo
							(d2_r),
							vis_read_hi(d1_r));
						d6 = vis_fpmerge(vis_read_hi
							(d6), vis_read_lo(d6));
						d5 = vis_fpmerge(vis_read_hi
							(d2), vis_read_lo(d2));
						d5 = vis_fpmerge(vis_read_hi
							(d5), vis_read_lo(d5));
						dr = vis_fpmerge(vis_read_hi
							(d5), vis_read_hi(d6));
						dr1 = vis_fpmerge(vis_read_lo
							(d5), vis_read_lo(d6));
						ddst[0] =
							vis_fpmerge(vis_read_hi
							(dr), vis_read_hi(dr1));
						ddst[1] =
							vis_fpmerge(vis_read_lo
							(dr), vis_read_lo(dr1));
						ddst += 2;
						i += 1;
					}

					break;
				case 6:
					dsrc_r = (mlib_d64 *)vis_alignaddr((void
						*)src_r, 0);
					dsrc_i = (mlib_d64 *)vis_alignaddr((void
						*)src_i, 0);

					d2_r = (*dsrc_r++);
					d2_i = (*dsrc_i++);
#pragma pipeloop(0)
					for (i = 0; i < len_64 - 1; i += 2) {
						d1_r = dsrc_r[0];
						d3 = vis_ld_d64_nf(dsrc_r + 1);

						d1_i = dsrc_i[0];
						d2 = vis_faligndata(d2_i, d1_i);
						d2_i = vis_ld_d64_nf(dsrc_i +
							1);
						d4 = vis_faligndata(d1_i, d2_i);

						d6 = vis_fpmerge(vis_read_lo
							(d2_r),
							vis_read_hi(d1_r));
						d6 = vis_fpmerge(vis_read_hi
							(d6), vis_read_lo(d6));
						d5 = vis_fpmerge(vis_read_hi
							(d2), vis_read_lo(d2));
						d5 = vis_fpmerge(vis_read_hi
							(d5), vis_read_lo(d5));
						dr = vis_fpmerge(vis_read_hi
							(d5), vis_read_hi(d6));
						dr1 = vis_fpmerge(vis_read_lo
							(d5), vis_read_lo(d6));
						ddst[0] =
							vis_fpmerge(vis_read_hi
							(dr), vis_read_hi(dr1));
						ddst[1] =
							vis_fpmerge(vis_read_lo
							(dr), vis_read_lo(dr1));

						dr = vis_fmuld8ulx16(f11,
							vis_read_lo(d1_r));
						dr1 = vis_fmuld8ulx16(f11,
							vis_read_hi(d3));
						dd0 = vis_fmuld8sux16(f1010,
							vis_read_hi(d4));
						dd1 = vis_fmuld8sux16(f1010,
							vis_read_lo(d4));
						dd0 = vis_fpack32(dd0, dmask);
						dd1 = vis_fpack32(dd1, dmask);
						ddst[2] =
							vis_for(vis_fand(dmask,
							dr), dd0);
						ddst[3] = dr1;
						vis_pst_16(dd1, ddst + 3, 0xa);

						dsrc_r += 2;
						dsrc_i += 2;
						ddst += 4;
						d2_r = d3;
					}

/*
 * next pixels
 */

					if (i < len_64) {
						d1_i = vis_ld_d64_nf(dsrc_i);
						dsrc_i++;
						d1_r = vis_ld_d64_nf(dsrc_r);
						dsrc_r++;
						d2 = vis_faligndata(d2_i, d1_i);

						d6 = vis_fpmerge(vis_read_lo
							(d2_r),
							vis_read_hi(d1_r));
						d6 = vis_fpmerge(vis_read_hi
							(d6), vis_read_lo(d6));
						d5 = vis_fpmerge(vis_read_hi
							(d2), vis_read_lo(d2));
						d5 = vis_fpmerge(vis_read_hi
							(d5), vis_read_lo(d5));
						dr = vis_fpmerge(vis_read_hi
							(d5), vis_read_hi(d6));
						dr1 = vis_fpmerge(vis_read_lo
							(d5), vis_read_lo(d6));
						ddst[0] =
							vis_fpmerge(vis_read_hi
							(dr), vis_read_hi(dr1));
						ddst[1] =
							vis_fpmerge(vis_read_lo
							(dr), vis_read_lo(dr1));
						ddst += 2;
						i += 1;
					}

					break;
			    }

			    break;
		    case 6:
			    switch (offset_i) {
				case 2:
					dsrc_r = (mlib_d64 *)vis_alignaddr((void
						*)src_r, 0);
					dsrc_i = (mlib_d64 *)vis_alignaddr((void
						*)src_i, 0);

					d2_i = (*dsrc_i++);
					d2_r = (*dsrc_r++);
					d1_r = vis_ld_d64_nf(dsrc_r); dsrc_r++;
					d1 = vis_faligndata(d2_r, d1_r);

#pragma pipeloop(0)
					for (i = 0; i < len_64 - 1; i += 2) {
						d1_i = dsrc_i[0];
						d2 = vis_faligndata(d2_i, d1_i);
						d2_i = vis_ld_d64_nf(dsrc_i +
							1);
						d4 = vis_faligndata(d1_i, d2_i);
						d2_r = dsrc_r[0];
						d3 = vis_faligndata(d1_r, d2_r);
						d1_r = vis_ld_d64_nf(dsrc_r +
							1);
						d3_r = vis_faligndata(d2_r,
							d1_r);

						d5 = vis_fpmerge(vis_read_hi
							(d2), vis_read_lo(d2));
						d5 = vis_fpmerge(vis_read_hi
							(d5), vis_read_lo(d5));
						d6 = vis_fpmerge(vis_read_lo
							(d1), vis_read_hi(d3));
						d6 = vis_fpmerge(vis_read_hi
							(d6), vis_read_lo(d6));
						dr = vis_fpmerge(vis_read_hi
							(d5), vis_read_hi(d6));
						dr1 = vis_fpmerge(vis_read_lo
							(d5), vis_read_lo(d6));
						ddst[0] =
							vis_fpmerge(vis_read_hi
							(dr), vis_read_hi(dr1));
						ddst[1] =
							vis_fpmerge(vis_read_lo
							(dr), vis_read_lo(dr1));

						dd0 = vis_fmuld8sux16(f1010,
							vis_read_hi(d4));
						dd1 = vis_fmuld8sux16(f1010,
							vis_read_lo(d4));
						dr = vis_fmuld8ulx16(f11,
							vis_read_lo(d3));
						dr1 = vis_fmuld8ulx16(f11,
							vis_read_hi(d3_r));
						dd0 = vis_fpack32(dd0, dmask);
						dd1 = vis_fpack32(dd1, dmask);
						ddst[2] =
							vis_for(vis_fand(dmask,
							dr), dd0);
						ddst[3] = dr1;
						vis_pst_16(dd1, ddst + 3, 0xa);

						dsrc_r += 2;
						dsrc_i += 2;
						ddst += 4;
						d1 = d3_r;
					}

/*
 * next pixels
 */
					if (i < len_64) {
						d1_i = vis_ld_d64_nf(dsrc_i);
						dsrc_i++;
						d2 = vis_faligndata(d2_i, d1_i);
						d2_r = vis_ld_d64_nf(dsrc_r);
						d3 = vis_faligndata(d1_r, d2_r);

						d5 = vis_fpmerge(vis_read_hi
							(d2), vis_read_lo(d2));
						d5 = vis_fpmerge(vis_read_hi
							(d5), vis_read_lo(d5));
						d6 = vis_fpmerge(vis_read_lo
							(d1), vis_read_hi(d3));
						d6 = vis_fpmerge(vis_read_hi
							(d6), vis_read_lo(d6));
						dr = vis_fpmerge(vis_read_hi
							(d5), vis_read_hi(d6));
						dr1 = vis_fpmerge(vis_read_lo
							(d5), vis_read_lo(d6));
						ddst[0] =
							vis_fpmerge(vis_read_hi
							(dr), vis_read_hi(dr1));
						ddst[1] =
							vis_fpmerge(vis_read_lo
							(dr), vis_read_lo(dr1));
						ddst += 2;
						i += 1;
					}

					break;
				case 4:
					dsrc_i = (mlib_d64 *)vis_alignaddr((void
						*)src_i, 0);
					dsrc_r = (mlib_d64 *)vis_alignaddr((void
						*)src_r, 0);

					d2_r = (*dsrc_r++);
					d2_i = (*dsrc_i++);
#pragma pipeloop(0)
					for (i = 0; i < len_64 - 1; i += 2) {
						d1_i = dsrc_i[0];
						d4 = vis_ld_d64_nf(dsrc_i + 1);

						d1_r = dsrc_r[0];
						d1 = vis_faligndata(d2_r, d1_r);
						d2_r = vis_ld_d64_nf(dsrc_r +
							1);
						d3 = vis_faligndata(d1_r, d2_r);

						d5 = vis_fpmerge(vis_read_lo
							(d2_i),
							vis_read_hi(d1_i));
						d5 = vis_fpmerge(vis_read_hi
							(d5), vis_read_lo(d5));
						d6 = vis_fpmerge(vis_read_hi
							(d1), vis_read_lo(d1));
						d6 = vis_fpmerge(vis_read_hi
							(d6), vis_read_lo(d6));
						dr = vis_fpmerge(vis_read_hi
							(d5), vis_read_hi(d6));
						dr1 = vis_fpmerge(vis_read_lo
							(d5), vis_read_lo(d6));
						ddst[0] =
							vis_fpmerge(vis_read_hi
							(dr), vis_read_hi(dr1));
						ddst[1] =
							vis_fpmerge(vis_read_lo
							(dr), vis_read_lo(dr1));

						dd0 = vis_fmuld8sux16(f1010,
							vis_read_lo(d1_i));
						dd1 = vis_fmuld8sux16(f1010,
							vis_read_hi(d4));
						dr = vis_fmuld8ulx16(f11,
							vis_read_hi(d3));
						dr1 = vis_fmuld8ulx16(f11,
							vis_read_lo(d3));
						dd0 = vis_fpack32(dd0, dmask);
						dd1 = vis_fpack32(dd1, dmask);
						ddst[2] =
							vis_for(vis_fand(dmask,
							dr), dd0);
						ddst[3] = dr1;
						vis_pst_16(dd1, ddst + 3, 0xa);

						dsrc_r += 2;
						dsrc_i += 2;
						ddst += 4;
						d2_i = d4;
					}

/*
 * next pixels
 */

					if (i < len_64) {
						d1_r = vis_ld_d64_nf(dsrc_r);
						dsrc_r++;
						d1_i = vis_ld_d64_nf(dsrc_i);
						dsrc_i++;
						d1 = vis_faligndata(d2_r, d1_r);

						d5 = vis_fpmerge(vis_read_lo
							(d2_i),
							vis_read_hi(d1_i));
						d5 = vis_fpmerge(vis_read_hi
							(d5), vis_read_lo(d5));
						d6 = vis_fpmerge(vis_read_hi
							(d1), vis_read_lo(d1));
						d6 = vis_fpmerge(vis_read_hi
							(d6), vis_read_lo(d6));
						dr = vis_fpmerge(vis_read_hi
							(d5), vis_read_hi(d6));
						dr1 = vis_fpmerge(vis_read_lo
							(d5), vis_read_lo(d6));
						ddst[0] =
							vis_fpmerge(vis_read_hi
							(dr), vis_read_hi(dr1));
						ddst[1] =
							vis_fpmerge(vis_read_lo
							(dr), vis_read_lo(dr1));
						ddst += 2;
						i += 1;
					}

					break;
			    }

			    break;
		}
	}
}

/* *********************************************************** */

mlib_status
__mlib_VectorMerge_S16C_S16(
	mlib_s16 *z,
	const mlib_s16 *rr,
	const mlib_s16 *ii,
	mlib_s32 n)
{
/* pointer to real source vector */
	mlib_s16 *src_r = (void *)rr;

/* pointer to imaginary source vector */
	mlib_s16 *src_i = (void *)ii;

/* pointer to resultant complex vector */
	mlib_s16 *dst = z;

/* mlib_d64 pointer to real source vector */
	mlib_d64 *dsrc_r;

/* mlib_d64 pointer to imaginary source vector */
	mlib_d64 *dsrc_i;

/* mlib_d64 pointer to resultant complex vector */
	mlib_d64 *ddst;
	mlib_s32 len_64, even_length, rest_64, length = n, i;
	mlib_s32 odd = 0;
	mlib_d64 d1, d2, d1_r, d1_i, d2_r, d2_i, d5, d6;

	if (n < 8) {
		MERGE(mlib_s16,
			rr,
			ii,
			n,
			z);
	}

/*
 * First, try to align destination address for 8 bytes .
 */

	while ((mlib_addr)dst & 7) {
		(*dst++) = (*src_r++);
		length--;

		if (!((mlib_addr)dst & 7)) {
			odd = 1;
			break;
		}

		(*dst++) = (*src_i++);
	}

	len_64 = length >> 2;
	rest_64 = length & 3;
	even_length = len_64 << 2;
	ddst = (mlib_d64 *)dst;
	vis_write_gsr(0);

/*
 * Now analyze the alignments of source vectors.
 */

	if ((!((mlib_addr)src_r & 7)) && (!((mlib_addr)src_i & 7))) {

/*
 * Both input vectors are 8-byte aligned.
 */

		dsrc_r = (mlib_d64 *)src_r;
		dsrc_i = (mlib_d64 *)src_i;

		if (!odd) {

/*
 * First "alignment" loop finished with imaginary part.
 * Start the next merge with real part.
 */
			mlib_d64 d3, d4, dr, dr1;
			mlib_d64 dmask = vis_to_double_dup(0xFFFF);
			mlib_f32 f11 = vis_to_float(0x10001),
				f1010 =	vis_to_float(0x1000100);
			mlib_d64 dd0, dd1;

#pragma pipeloop(0)
			for (i = 0; i < len_64 - 1; i += 2) {
				d1 = dsrc_r[0];
				d2 = dsrc_i[0];
				d3 = dsrc_r[1];
				d4 = dsrc_i[1];
				d5 = vis_fpmerge(vis_read_hi(d1),
					vis_read_lo(d1));
				d5 = vis_fpmerge(vis_read_hi(d5),
					vis_read_lo(d5));
				d6 = vis_fpmerge(vis_read_hi(d2),
					vis_read_lo(d2));
				d6 = vis_fpmerge(vis_read_hi(d6),
					vis_read_lo(d6));
				dr = vis_fpmerge(vis_read_hi(d5),
					vis_read_hi(d6));
				dr1 = vis_fpmerge(vis_read_lo(d5),
					vis_read_lo(d6));
				ddst[0] =
					vis_fpmerge(vis_read_hi(dr),
					vis_read_hi(dr1));
				ddst[1] =
					vis_fpmerge(vis_read_lo(dr),
					vis_read_lo(dr1));

				dd0 = vis_fmuld8sux16(f1010, vis_read_hi(d3));
				dd1 = vis_fmuld8sux16(f1010, vis_read_lo(d3));
				dr = vis_fmuld8ulx16(f11, vis_read_hi(d4));
				dr1 = vis_fmuld8ulx16(f11, vis_read_lo(d4));
				dd0 = vis_fpack32(dd0, dmask);
				dd1 = vis_fpack32(dd1, dmask);
				ddst[2] = vis_for(vis_fand(dmask, dr), dd0);
				ddst[3] = dr1;
				vis_pst_16(dd1, ddst + 3, 0xa);

				dsrc_r += 2;
				dsrc_i += 2;
				ddst += 4;
			}

/*
 * next pixels
 */

			if (i < len_64) {
				d1 = (*dsrc_r++);
				d2 = (*dsrc_i++);
				d5 = vis_fpmerge(vis_read_hi(d1),
					vis_read_lo(d1));
				d5 = vis_fpmerge(vis_read_hi(d5),
					vis_read_lo(d5));
				d6 = vis_fpmerge(vis_read_hi(d2),
					vis_read_lo(d2));
				d6 = vis_fpmerge(vis_read_hi(d6),
					vis_read_lo(d6));
				dr = vis_fpmerge(vis_read_hi(d5),
					vis_read_hi(d6));
				dr1 = vis_fpmerge(vis_read_lo(d5),
					vis_read_lo(d6));
				ddst[0] =
					vis_fpmerge(vis_read_hi(dr),
					vis_read_hi(dr1));
				ddst[1] =
					vis_fpmerge(vis_read_lo(dr),
					vis_read_lo(dr1));
				ddst += 2;
				i += 1;
			}
		} else {

/*
 * First "alignment" loop finished with real part.
 * Start the next merge with imaginary part.
 */

			mlib_d64 d3, d4, dr, dr1;
			mlib_d64 dmask = vis_to_double_dup(0xFFFF);
			mlib_f32 f11 = vis_to_float(0x10001),
				f1010 =	vis_to_float(0x1000100);
			mlib_d64 dd0, dd1;

#pragma pipeloop(0)
			for (i = 0; i < len_64 - 1; i += 2) {
				d2 = dsrc_r[0];
				d1 = dsrc_i[0];
				d4 = dsrc_r[1];
				d3 = dsrc_i[1];
				d5 = vis_fpmerge(vis_read_hi(d1),
					vis_read_lo(d1));
				d5 = vis_fpmerge(vis_read_hi(d5),
					vis_read_lo(d5));
				d6 = vis_fpmerge(vis_read_hi(d2),
					vis_read_lo(d2));
				d6 = vis_fpmerge(vis_read_hi(d6),
					vis_read_lo(d6));
				dr = vis_fpmerge(vis_read_hi(d5),
					vis_read_hi(d6));
				dr1 = vis_fpmerge(vis_read_lo(d5),
					vis_read_lo(d6));
				ddst[0] =
					vis_fpmerge(vis_read_hi(dr),
					vis_read_hi(dr1));
				ddst[1] =
					vis_fpmerge(vis_read_lo(dr),
					vis_read_lo(dr1));

				dd0 = vis_fmuld8sux16(f1010, vis_read_hi(d3));
				dd1 = vis_fmuld8sux16(f1010, vis_read_lo(d3));
				dr = vis_fmuld8ulx16(f11, vis_read_hi(d4));
				dr1 = vis_fmuld8ulx16(f11, vis_read_lo(d4));
				dd0 = vis_fpack32(dd0, dmask);
				dd1 = vis_fpack32(dd1, dmask);
				ddst[2] = vis_for(vis_fand(dmask, dr), dd0);
				ddst[3] = dr1;
				vis_pst_16(dd1, ddst + 3, 0xa);

				dsrc_r += 2;
				dsrc_i += 2;
				ddst += 4;
			}

/*
 * next pixels
 */

			if (i < len_64) {
				d2 = (*dsrc_r++);
				d1 = (*dsrc_i++);
				d5 = vis_fpmerge(vis_read_hi(d1),
					vis_read_lo(d1));
				d5 = vis_fpmerge(vis_read_hi(d5),
					vis_read_lo(d5));
				d6 = vis_fpmerge(vis_read_hi(d2),
					vis_read_lo(d2));
				d6 = vis_fpmerge(vis_read_hi(d6),
					vis_read_lo(d6));
				dr = vis_fpmerge(vis_read_hi(d5),
					vis_read_hi(d6));
				dr1 = vis_fpmerge(vis_read_lo(d5),
					vis_read_lo(d6));
				ddst[0] =
					vis_fpmerge(vis_read_hi(dr),
					vis_read_hi(dr1));
				ddst[1] =
					vis_fpmerge(vis_read_lo(dr),
					vis_read_lo(dr1));
				ddst += 2;
				i += 1;
			}
		}
	} else if (((mlib_addr)src_r & 7) == ((mlib_addr)src_i & 7)) {

/*
 * Both input vectors have the same address alignment.
 * We cam use only 1 vis_faligndata inside loop.
 */

		dsrc_r = (mlib_d64 *)vis_alignaddr((void *)src_r, 0);
		dsrc_i = (mlib_d64 *)vis_alignaddr((void *)src_i, 0);
		d2_r = (*dsrc_r++);
		d2_i = (*dsrc_i++);

		if (!odd) {

/*
 * First "alignment" loop finished with imaginary part.
 * Start the next merge with real part.
 */
			mlib_d64 d3, d4, dr, dr1;
			mlib_d64 dmask = vis_to_double_dup(0xFFFF);
			mlib_f32 f11 = vis_to_float(0x10001),
				f1010 =	vis_to_float(0x1000100);
			mlib_d64 dd0, dd1;

#pragma pipeloop(0)
			for (i = 0; i < len_64 - 1; i += 2) {
				d1_r = dsrc_r[0];
				d1_i = dsrc_i[0];
				d1 = vis_faligndata(d2_r, d1_r);
				d2 = vis_faligndata(d2_i, d1_i);
				d2_r = vis_ld_d64_nf(dsrc_r + 1);
				d2_i = vis_ld_d64_nf(dsrc_i + 1);
				d3 = vis_faligndata(d1_r, d2_r);
				d4 = vis_faligndata(d1_i, d2_i);

				d5 = vis_fpmerge(vis_read_hi(d1),
					vis_read_lo(d1));
				d5 = vis_fpmerge(vis_read_hi(d5),
					vis_read_lo(d5));
				d6 = vis_fpmerge(vis_read_hi(d2),
					vis_read_lo(d2));
				d6 = vis_fpmerge(vis_read_hi(d6),
					vis_read_lo(d6));
				dr = vis_fpmerge(vis_read_hi(d5),
					vis_read_hi(d6));
				dr1 = vis_fpmerge(vis_read_lo(d5),
					vis_read_lo(d6));
				ddst[0] =
					vis_fpmerge(vis_read_hi(dr),
					vis_read_hi(dr1));
				ddst[1] =
					vis_fpmerge(vis_read_lo(dr),
					vis_read_lo(dr1));

				dd0 = vis_fmuld8sux16(f1010, vis_read_hi(d3));
				dd1 = vis_fmuld8sux16(f1010, vis_read_lo(d3));
				dr = vis_fmuld8ulx16(f11, vis_read_hi(d4));
				dr1 = vis_fmuld8ulx16(f11, vis_read_lo(d4));
				dd0 = vis_fpack32(dd0, dmask);
				dd1 = vis_fpack32(dd1, dmask);
				ddst[2] = vis_for(vis_fand(dmask, dr), dd0);
				ddst[3] = dr1;
				vis_pst_16(dd1, ddst + 3, 0xa);

				dsrc_r += 2;
				dsrc_i += 2;
				ddst += 4;
			}

/*
 * next pixels
 */

			if (i < len_64) {
				d1_r = vis_ld_d64_nf(dsrc_r); dsrc_r++;
				d1_i = vis_ld_d64_nf(dsrc_i); dsrc_i++;
				d1 = vis_faligndata(d2_r, d1_r);
				d2 = vis_faligndata(d2_i, d1_i);

				d5 = vis_fpmerge(vis_read_hi(d1),
					vis_read_lo(d1));
				d5 = vis_fpmerge(vis_read_hi(d5),
					vis_read_lo(d5));
				d6 = vis_fpmerge(vis_read_hi(d2),
					vis_read_lo(d2));
				d6 = vis_fpmerge(vis_read_hi(d6),
					vis_read_lo(d6));
				dr = vis_fpmerge(vis_read_hi(d5),
					vis_read_hi(d6));
				dr1 = vis_fpmerge(vis_read_lo(d5),
					vis_read_lo(d6));
				ddst[0] =
					vis_fpmerge(vis_read_hi(dr),
					vis_read_hi(dr1));
				ddst[1] =
					vis_fpmerge(vis_read_lo(dr),
					vis_read_lo(dr1));
				ddst += 2;
				i += 1;
			}
		} else {

/*
 * First "alignment" loop finished with real part.
 * Start the next merge with imaginary part.
 */

			mlib_d64 d3, d4, dr, dr1;
			mlib_d64 dmask = vis_to_double_dup(0xFFFF);
			mlib_f32 f11 = vis_to_float(0x10001),
				f1010 =	vis_to_float(0x1000100);
			mlib_d64 dd0, dd1;

#pragma pipeloop(0)
			for (i = 0; i < len_64 - 1; i += 2) {
				d1_r = dsrc_r[0];
				d1_i = dsrc_i[0];
				d2 = vis_faligndata(d2_r, d1_r);
				d1 = vis_faligndata(d2_i, d1_i);
				d2_r = vis_ld_d64_nf(dsrc_r + 1);
				d2_i = vis_ld_d64_nf(dsrc_i + 1);
				d4 = vis_faligndata(d1_r, d2_r);
				d3 = vis_faligndata(d1_i, d2_i);

				d5 = vis_fpmerge(vis_read_hi(d1),
					vis_read_lo(d1));
				d5 = vis_fpmerge(vis_read_hi(d5),
					vis_read_lo(d5));
				d6 = vis_fpmerge(vis_read_hi(d2),
					vis_read_lo(d2));
				d6 = vis_fpmerge(vis_read_hi(d6),
					vis_read_lo(d6));
				dr = vis_fpmerge(vis_read_hi(d5),
					vis_read_hi(d6));
				dr1 = vis_fpmerge(vis_read_lo(d5),
					vis_read_lo(d6));
				ddst[0] =
					vis_fpmerge(vis_read_hi(dr),
					vis_read_hi(dr1));
				ddst[1] =
					vis_fpmerge(vis_read_lo(dr),
					vis_read_lo(dr1));

				dd0 = vis_fmuld8sux16(f1010, vis_read_hi(d3));
				dd1 = vis_fmuld8sux16(f1010, vis_read_lo(d3));
				dr = vis_fmuld8ulx16(f11, vis_read_hi(d4));
				dr1 = vis_fmuld8ulx16(f11, vis_read_lo(d4));
				dd0 = vis_fpack32(dd0, dmask);
				dd1 = vis_fpack32(dd1, dmask);
				ddst[2] = vis_for(vis_fand(dmask, dr), dd0);
				ddst[3] = dr1;
				vis_pst_16(dd1, ddst + 3, 0xa);

				dsrc_r += 2;
				dsrc_i += 2;
				ddst += 4;
			}

/*
 * next pixels
 */

			if (i < len_64) {
				d1_r = vis_ld_d64_nf(dsrc_r); dsrc_r++;
				d1_i = vis_ld_d64_nf(dsrc_i); dsrc_i++;
				d2 = vis_faligndata(d2_r, d1_r);
				d1 = vis_faligndata(d2_i, d1_i);

				d5 = vis_fpmerge(vis_read_hi(d1),
					vis_read_lo(d1));
				d5 = vis_fpmerge(vis_read_hi(d5),
					vis_read_lo(d5));
				d6 = vis_fpmerge(vis_read_hi(d2),
					vis_read_lo(d2));
				d6 = vis_fpmerge(vis_read_hi(d6),
					vis_read_lo(d6));
				dr = vis_fpmerge(vis_read_hi(d5),
					vis_read_hi(d6));
				dr1 = vis_fpmerge(vis_read_lo(d5),
					vis_read_lo(d6));
				ddst[0] =
					vis_fpmerge(vis_read_hi(dr),
					vis_read_hi(dr1));
				ddst[1] =
					vis_fpmerge(vis_read_lo(dr),
					vis_read_lo(dr1));
				ddst += 2;
				i += 1;
			}
		}
	} else {

/*
 * Input vectors have different arbitrary alignments.
 * We have to use vis_alignaddr() and vis_faligndata() on
 * each loop iteration.
 */

		if (!odd) {

/*
 * First "alignment" loop finished with imaginary part.
 * Start the next merge with real part.
 */

			mlib_VectorMerge_S16C_S16_ge(ddst, src_r, src_i,
				len_64);
		} else {

/*
 * First "alignment" loop finished with real part.
 * Start the next merge with imaginary part.
 */

			mlib_mlib_VectorMerge_S16C_S16_ge_odd(ddst, src_r,
				src_i, len_64);
		}
	}

	dst += (even_length << 1);
	src_r += even_length;
	src_i += even_length;

	if (odd) {
		(*dst++) = (*src_i++);
	}

	for (i = 0; i < rest_64; i++) {
		(*dst++) = (*src_r++);
		(*dst++) = (*src_i++);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

/*
 *
 * For 32-bit format we also use VIS instructions in case of 8-byte
 * alignments of both input vectors and output one. We read 2 times by
 * 64 bits into 2*64-bit registers and then compose 2*64-bit resultant
 * values by vis_freg_pair() and vis_read_hi()/vis_read_lo().
 * In case of arbitrary alignment of input vectors we use C version of
 * the function.
 *
 */

mlib_status
__mlib_VectorMerge_S32C_S32(
	mlib_s32 *z,
	const mlib_s32 *rr,
	const mlib_s32 *ii,
	mlib_s32 n)
{
/* pointer to real source vector */
	mlib_u32 *src_r = (mlib_u32 *)rr;

/* pointer to imaginary source vector */
	mlib_u32 *src_i = (mlib_u32 *)ii;

/* pointer to resultant complex vector */
	mlib_u32 *dst = (mlib_u32 *)z;
	mlib_s32 len = n + n, odd = 0, i;
	mlib_d64 d1, d2, d3, d4;

/*
 * First, try to align destination address for 8 bytes .
 */

	if (n < 4) {
		MERGE(mlib_s32,
			rr,
			ii,
			n,
			z);
	}

	if (((mlib_addr)dst) & 7) {
		(*dst++) = (*src_r++);
		len--;
		odd = 1;
	}

	if (!(((mlib_addr)src_r ^ (mlib_addr)src_i) & 7)) {
		if (odd) {
			if (((mlib_addr)src_i & 7) && (len >= 2)) {
				(*dst++) = (*src_i++);
				(*dst++) = (*src_r++);
				len -= 2;
			}
#pragma pipeloop(0)
			for (i = 0; i <= (len - 4); i += 4) {
				d1 = *((mlib_d64 *)src_i);
				src_i += 2;
				d2 = *((mlib_d64 *)src_r);
				src_r += 2;
				MERGE32;
				((mlib_d64 *)dst)[0] = d3;
				((mlib_d64 *)dst)[1] = d4;
				dst += 4;
			}

			if (i <= len - 2) {
				(*dst++) = (*src_i++);
				(*dst++) = (*src_r++);
			}

			(*dst++) = (*src_i++);
		} else {
			if ((mlib_addr)src_i & 7) {
				(*dst++) = (*src_r++);
				(*dst++) = (*src_i++);
				len -= 2;
			}
#pragma pipeloop(0)
			for (i = 0; i <= (len - 4); i += 4) {
				d1 = *((mlib_d64 *)src_r);
				src_r += 2;
				d2 = *((mlib_d64 *)src_i);
				src_i += 2;
				MERGE32;
				((mlib_d64 *)dst)[0] = d3;
				((mlib_d64 *)dst)[1] = d4;
				dst += 4;
			}

			if (i <= len - 2) {
				(*dst++) = (*src_r++);
				(*dst++) = (*src_i++);
			}
		}
	} else {
		mlib_f32 fsrc_r, fsrc_i;

		if (odd) {
#pragma pipeloop(0)
			for (i = 0; i <= (len - 2); i += 2) {
				fsrc_r = *((mlib_f32 *)src_r);
				src_r++;
				fsrc_i = *((mlib_f32 *)src_i);
				src_i++;
				d1 = vis_freg_pair(fsrc_i, fsrc_r);
				((mlib_d64 *)dst)[0] = d1;
				dst += 2;
			}

			(*dst++) = (*src_i++);
		} else {
#pragma pipeloop(0)
			for (i = 0; i < len; i += 2) {
				fsrc_r = *((mlib_f32 *)src_r);
				src_r++;
				fsrc_i = *((mlib_f32 *)src_i);
				src_i++;
				d1 = vis_freg_pair(fsrc_r, fsrc_i);
				((mlib_d64 *)dst)[0] = d1;
				dst += 2;
			}
		}
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */
