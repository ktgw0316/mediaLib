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

#pragma ident	"@(#)mlib_v_VectorReverseByteOrder.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorReverseByteOrder_S16_S16 - change the endianness
 *      mlib_VectorReverseByteOrder_U16_U16 - change the endianness
 *      mlib_VectorReverseByteOrder_S32_S32 - change the endianness
 *      mlib_VectorReverseByteOrder_U32_U32 - change the endianness
 *      mlib_VectorReverseByteOrder_S64_S64 - change the endianness
 *      mlib_VectorReverseByteOrder_U64_U64 - change the endianness
 *      mlib_VectorReverseByteOrder_F32_F32 - change the endianness
 *      mlib_VectorReverseByteOrder_D64_D64 - change the endianness
 *      mlib_VectorReverseByteOrder         - change the endianness
 *      mlib_VectorReverseByteOrder_S16     - change the endianness
 *      mlib_VectorReverseByteOrder_U16     - change the endianness
 *      mlib_VectorReverseByteOrder_S32     - change the endianness
 *      mlib_VectorReverseByteOrder_U32     - change the endianness
 *      mlib_VectorReverseByteOrder_S64     - change the endianness
 *      mlib_VectorReverseByteOrder_U64     - change the endianness
 *      mlib_VectorReverseByteOrder_F32     - change the endianness
 *      mlib_VectorReverseByteOrder_D64     - change the endianness
 *      mlib_VectorReverseByteOrder_Inp     - change the endianness
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorReverseByteOrder_S16_S16(mlib_s16       *z,
 *                                                      const mlib_s16 *x,
 *                                                      mlib_s32       n);
 *      mlib_status mlib_VectorReverseByteOrder_U16_U16(mlib_u16       *z,
 *                                                      const mlib_u16 *x,
 *                                                      mlib_s32       n);
 *      mlib_status mlib_VectorReverseByteOrder_S32_S32(mlib_s32       *z,
 *                                                      const mlib_s32 *x,
 *                                                      mlib_s32       n);
 *      mlib_status mlib_VectorReverseByteOrder_U32_U32(mlib_u32       *z,
 *                                                      const mlib_u32 *x,
 *                                                      mlib_s32       n);
 *      mlib_status mlib_VectorReverseByteOrder_S64_S64(mlib_s64       *z,
 *                                                      const mlib_s64 *x,
 *                                                      mlib_s32       n);
 *      mlib_status mlib_VectorReverseByteOrder_U64_U64(mlib_u64       *z,
 *                                                      const mlib_u64 *x,
 *                                                      mlib_s32       n);
 *      mlib_status mlib_VectorReverseByteOrder_F32_F32(mlib_f32       *z,
 *                                                      const mlib_f32 *x,
 *                                                      mlib_s32       n);
 *      mlib_status mlib_VectorReverseByteOrder_D64_D64(mlib_d64       *z,
 *                                                      const mlib_d64 *x,
 *                                                      mlib_s32       n);
 *      mlib_status mlib_VectorReverseByteOrder(void       *z,
 *                                              const void *x,
 *                                              mlib_s32   n,
 *                                              mlib_s32   s);
 *      mlib_status mlib_VectorReverseByteOrder_S16(mlib_s16 *xz,
 *                                                  mlib_s32  n);
 *      mlib_status mlib_VectorReverseByteOrder_U16(mlib_u16 *xz,
 *                                                  mlib_s32  n);
 *      mlib_status mlib_VectorReverseByteOrder_S32(mlib_s32 *xz,
 *                                                  mlib_s32  n);
 *      mlib_status mlib_VectorReverseByteOrder_U32(mlib_u32 *xz,
 *                                                  mlib_s32  n);
 *      mlib_status mlib_VectorReverseByteOrder_S64(mlib_s64 *xz,
 *                                                  mlib_s32  n);
 *      mlib_status mlib_VectorReverseByteOrder_U64(mlib_u64 *xz,
 *                                                  mlib_s32  n);
 *      mlib_status mlib_VectorReverseByteOrder_F32(mlib_f32 *xz,
 *                                                  mlib_s32  n);
 *      mlib_status mlib_VectorReverseByteOrder_D64(mlib_d64 *xz,
 *                                                  mlib_s32  n);
 *      mlib_status mlib_VectorReverseByteOrder_Inp(void     *xz,
 *                                                  mlib_s32 n,
 *                                                  mlib_s32 s);
 *
 * ARGUMENT
 *      z    pointer to output vector
 *      x    pointer to input vector
 *      xz   pointer to input and output vector
 *      n    number of elements in the vectors
 *      s    size of elements in bytes
 *
 * DESCRIPTION
 *      Change the encoding of each element from big endian to
 *      little endian, or from little endian to big endian.
 *
 * RESTRICTION
 *      The vector element size can be from 2 to 8 bytes for
 *      mlib_VectorReverseByteOrder{_Inp} functions.
 */

#include <mlib_algebra.h>
#include <mlib_VectorReverseByteOrder.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorReverseByteOrder_Inp = \
	__mlib_VectorReverseByteOrder_Inp
#pragma weak mlib_VectorReverseByteOrder_U64 = \
	__mlib_VectorReverseByteOrder_U64
#pragma weak mlib_VectorReverseByteOrder_U16_U16 = \
	__mlib_VectorReverseByteOrder_U16_U16
#pragma weak mlib_VectorReverseByteOrder = __mlib_VectorReverseByteOrder
#pragma weak mlib_VectorReverseByteOrder_U32 = \
	__mlib_VectorReverseByteOrder_U32
#pragma weak mlib_VectorReverseByteOrder_U32_U32 = \
	__mlib_VectorReverseByteOrder_U32_U32
#pragma weak mlib_VectorReverseByteOrder_U16 = \
	__mlib_VectorReverseByteOrder_U16
#pragma weak mlib_VectorReverseByteOrder_U64_U64 = \
	__mlib_VectorReverseByteOrder_U64_U64

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_VectorReverseByteOrder_Inp) mlib_VectorReverseByteOrder_Inp
	__attribute__((weak, alias("__mlib_VectorReverseByteOrder_Inp")));
__typeof__(__mlib_VectorReverseByteOrder_U64) mlib_VectorReverseByteOrder_U64
	__attribute__((weak, alias("__mlib_VectorReverseByteOrder_U64")));
__typeof__(__mlib_VectorReverseByteOrder_U16_U16)
	mlib_VectorReverseByteOrder_U16_U16
	__attribute__((weak, alias("__mlib_VectorReverseByteOrder_U16_U16")));
__typeof__(__mlib_VectorReverseByteOrder) mlib_VectorReverseByteOrder
	__attribute__((weak, alias("__mlib_VectorReverseByteOrder")));
__typeof__(__mlib_VectorReverseByteOrder_U32) mlib_VectorReverseByteOrder_U32
	__attribute__((weak, alias("__mlib_VectorReverseByteOrder_U32")));
__typeof__(__mlib_VectorReverseByteOrder_U32_U32)
	mlib_VectorReverseByteOrder_U32_U32
	__attribute__((weak, alias("__mlib_VectorReverseByteOrder_U32_U32")));
__typeof__(__mlib_VectorReverseByteOrder_U16) mlib_VectorReverseByteOrder_U16
	__attribute__((weak, alias("__mlib_VectorReverseByteOrder_U16")));
__typeof__(__mlib_VectorReverseByteOrder_U64_U64)
	mlib_VectorReverseByteOrder_U64_U64
	__attribute__((weak, alias("__mlib_VectorReverseByteOrder_U64_U64")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	REVERSEBYTEORDER_C_U16(i)                               \
	{                                                       \
	    mlib_s32 tmp = ((mlib_u16 *)src)[i];                \
	                                                        \
	    ((mlib_u16 *)dst)[i] = (tmp << 8) | (tmp >> 8);     \
	}

/* *********************************************************** */

#define	REVERSEBYTEORDER_C_U16_INP(i)                            \
	{                                                        \
	    mlib_s32 tmp = ((mlib_u16 *)srcdst)[i];              \
	                                                         \
	    ((mlib_u16 *)srcdst)[i] = (tmp << 8) | (tmp >> 8);   \
	}

/* *********************************************************** */

#define	REVERSEBYTEORDER_C_U32(i)                                       \
	{                                                               \
	    mlib_s32 tmp1 = ((mlib_u16 *)src)[2 * i];                   \
	    mlib_s32 tmp2 = ((mlib_u16 *)src)[2 * i + 1];               \
	                                                                \
	    ((mlib_u16 *)dst)[2 * i + 1] = (tmp1 << 8) | (tmp1 >> 8);   \
	    ((mlib_u16 *)dst)[2 * i] = (tmp2 << 8) | (tmp2 >> 8);       \
	}

/* *********************************************************** */

#define	REVERSEBYTEORDER_C_U32_INP(i)                                      \
	{                                                                  \
	    mlib_s32 tmp1 = ((mlib_u16 *)srcdst)[2 * i];                   \
	    mlib_s32 tmp2 = ((mlib_u16 *)srcdst)[2 * i + 1];               \
	                                                                   \
	    ((mlib_u16 *)srcdst)[2 * i + 1] = (tmp1 << 8) | (tmp1 >> 8);   \
	    ((mlib_u16 *)srcdst)[2 * i] = (tmp2 << 8) | (tmp2 >> 8);       \
	}

/* *********************************************************** */

#if defined(MLIB_VIS) && MLIB_VIS >= 0x200

#define	WRITE_BMASK(mask)	vis_write_bmask(mask, 0)

#define	REVERSEBYTEORDER_V_U16()	ddst = vis_bshuffle(dsrc, dsrc)

#define	PREPARE_V_U24(pdst, psrc)                               \
	dpsrc = (mlib_d64 *)psrc;                               \
	(void *)vis_alignaddr(pdst, 6)

#define	REVERSEBYTEORDER_V_U24()                                \
	dsrc0 = dpsrc[3 * i];                                   \
	dsrc1 = dpsrc[3 * i + 1];                               \
	dsrc2 = dpsrc[3 * i + 2];                               \
	WRITE_BMASK(0x21054387);                                \
	ddst0 = vis_bshuffle(dsrc0, dsrc1);                     \
	ddst1 = vis_faligndata(dsrc0, dsrc2);                   \
	WRITE_BMASK(0x87cbafed);                                \
	ddst2 = vis_bshuffle(dsrc1, dsrc2);                     \
	WRITE_BMASK(0x0ba9edc3);                                \
	ddst1 = vis_bshuffle(ddst1, dsrc1)

#define	PREPARE_V_U24_NA()                                      \
	dpsrc = (mlib_d64 *)vis_alignaddr((void *)x, 0);        \
	dsrc0 = dpsrc[0]

#define	REVERSEBYTEORDER_V_U24_NA()                             \
	dsrc1 = dpsrc[3 * i + 1];                               \
	dsrc2 = dpsrc[3 * i + 2];                               \
	dsrc3 = dpsrc[3 * i + 3];                               \
	dsrc0 = vis_faligndata(dsrc0, dsrc1);                   \
	dsrc1 = vis_faligndata(dsrc1, dsrc2);                   \
	dsrc2 = vis_faligndata(dsrc2, dsrc3);                   \
	WRITE_BMASK(0x21054387);                                \
	ddst0 = vis_bshuffle(dsrc0, dsrc1);                     \
	WRITE_BMASK(0x6ba9edcc);                                \
	ddst1 = vis_bshuffle(dsrc0, dsrc1);                     \
	WRITE_BMASK(0x87cbafed);                                \
	ddst2 = vis_bshuffle(dsrc1, dsrc2);                     \
	WRITE_BMASK(0x01234569);                                \
	ddst1 = vis_bshuffle(ddst1, dsrc2);                     \
	dsrc0 = dsrc3

#define	REVERSEBYTEORDER_V_U32_AA(src1, src2, ind)              \
	dpdst[ind] = vis_bshuffle(src1, src2)

#define	REVERSEBYTEORDER_V_U32_NA(src1, src2, ind)              \
	dpdst[ind] = vis_bshuffle(src1, src2)

#else /* defined(MLIB_VIS) && MLIB_VIS >= 0x200 */

#define	WRITE_BMASK(mask)

#define	REVERSEBYTEORDER_V_U16()                                    \
	ddst = vis_fpmerge(vis_read_hi(dsrc), vis_read_lo(dsrc));   \
	ddst = vis_fpmerge(vis_read_hi(ddst), vis_read_lo(ddst));   \
	ddst = vis_fpmerge(vis_read_lo(ddst), vis_read_hi(ddst))

#define	PREPARE_V_U24(pdst, psrc)                               \
	dpsrc = (mlib_d64 *)psrc;                               \
	(void *)vis_alignaddr(dpsrc, 4);                        \
	dsrc0 = vis_ld_d64_nf_le(dpsrc);                        \
	/*                                                      \
	 * 00 01 02 03 04 05 06 07                              \
	 */                                                     \
	dsrc1 = vis_ld_d64_nf_le(dpsrc + 1);                    \
	/*                                                      \
	 * 10 11 12 13 14 15 16 17                              \
	 */                                                     \
	dsrc2 = vis_ld_d64_nf_le(dpsrc + 2)                     \
	/* \ 20 21 22 23 24 25 26 27 \ */

#define	REVERSEBYTEORDER_V_U24()                                \
	{                                                       \
	    mlib_d64 t1, t2, t3, t4, t5, t6;                    \
	                                                        \
	    t1 = vis_faligndata(dsrc1, dsrc1);                  \
	/*                                                      \
	 * 14 15 16 xx xx 11 12 13                              \
	 */                                                     \
	    (void *)vis_alignaddr(dpsrc, 7);                    \
	    t2 = vis_faligndata(dsrc1, dsrc0);                  \
	/*                                                      \
	 * 17 00 xx xx xx xx xx xx                              \
	 */                                                     \
	    (void *)vis_alignaddr(dpsrc, 6);                    \
	    t3 = vis_faligndata(dsrc2, dsrc2);                  \
	/*                                                      \
	 * 26 xx xx xx xx 23 24 25                              \
	 */                                                     \
	    (void *)vis_alignaddr(dpsrc, 1);                    \
	    t4 = vis_faligndata(dsrc2, dsrc1);                  \
	/*                                                      \
	 * xx xx xx xx xx xx 27 10                              \
	 */                                                     \
	    (void *)vis_alignaddr(dpsrc, 5);                    \
	    t5 = vis_faligndata(dsrc0, dsrc0);                  \
	/*                                                      \
	 * xx xx xx xx xx 02 03 04                              \
	 */                                                     \
	    t2 = vis_faligndata(t5, t2);                        \
	/*                                                      \
	 * 02 03 04 17 00 xx xx xx                              \
	 */                                                     \
	    ddst0 = vis_faligndata(dsrc0, t2);                  \
	/*                                                      \
	 * 05 06 07 02 03 04 17 00                              \
	 */                                                     \
	    t2 = vis_faligndata(t3, dsrc2);                     \
	/*                                                      \
	 * 23 24 25 20 21 22 xx xx                              \
	 */                                                     \
	    t6 = vis_faligndata(t1, t3);                        \
	/*                                                      \
	 * 11 12 13 26 xx xx xx xx                              \
	 */                                                     \
	    t3 = vis_faligndata(dsrc0, t5);                     \
	/*                                                      \
	 * xx xx xx xx xx xx xx 01                              \
	 */                                                     \
	    (void *)vis_alignaddr(dpsrc, 6);                    \
	    ddst2 = vis_faligndata(t4, t2);                     \
	/*                                                      \
	 * 27 10 23 24 25 20 21 22                              \
	 */                                                     \
	    (void *)vis_alignaddr(dpsrc, 3);                    \
	    t1 = vis_faligndata(t3, t1);                        \
	/*                                                      \
	 * xx xx xx xx 01 14 15 16                              \
	 */                                                     \
	    (void *)vis_alignaddr(dpsrc, 4);                    \
	    ddst1 = vis_faligndata(t1, t6);                     \
	/*                                                      \
	 * 01 14 15 16 11 12 13 26                              \
	 */                                                     \
	    dsrc0 = vis_ld_d64_nf_le(dpsrc + 3 * i + 3);        \
	/*                                                      \
	 * 00 01 02 03 04 05 06 07                              \
	 */                                                     \
	    dsrc1 = vis_ld_d64_nf_le(dpsrc + 3 * i + 4);        \
	/*                                                      \
	 * 10 11 12 13 14 15 16 17                              \
	 */                                                     \
	    dsrc2 = vis_ld_d64_nf_le(dpsrc + 3 * i + 5);        \
	/*                                                      \
	 * 20 21 22 23 24 25 26 27                              \
	 */                                                     \
	}

#define	PREPARE_V_U24_NA()                                      \
	dpsrc = (mlib_d64 *)vis_alignaddrl((void *)x, 0);       \
	dsrc0 = vis_ld_d64_nf_le(dpsrc)                         \
	/* \ 00 01 02 03 04 05 06 07 \ */

#define	REVERSEBYTEORDER_V_U24_NA()                             \
	{                                                       \
	    mlib_d64 t1, t2, t3, t4, t5, t6;                    \
	                                                        \
	    dsrc1 = vis_ld_d64_le(dpsrc + 3 * i + 1);           \
	/*                                                      \
	 * 10 11 12 13 14 15 16 17                              \
	 */                                                     \
	    dsrc2 = vis_ld_d64_le(dpsrc + 3 * i + 2);           \
	/*                                                      \
	 * 20 21 22 23 24 25 26 27                              \
	 */                                                     \
	    dsrc3 = vis_ld_d64_le(dpsrc + 3 * i + 3);           \
	/*                                                      \
	 * 20 21 22 23 24 25 26 27                              \
	 */                                                     \
	    dsrc0 = vis_faligndata(dsrc1, dsrc0);               \
	    dsrc1 = vis_faligndata(dsrc2, dsrc1);               \
	    dsrc2 = vis_faligndata(dsrc3, dsrc2);               \
	    (void *)vis_alignaddr(dpsrc, 4);                    \
	    t1 = vis_faligndata(dsrc1, dsrc1);                  \
	/*                                                      \
	 * 14 15 16 xx xx 11 12 13                              \
	 */                                                     \
	    (void *)vis_alignaddr(dpsrc, 7);                    \
	    t2 = vis_faligndata(dsrc1, dsrc0);                  \
	/*                                                      \
	 * 17 00 xx xx xx xx xx xx                              \
	 */                                                     \
	    (void *)vis_alignaddr(dpsrc, 6);                    \
	    t3 = vis_faligndata(dsrc2, dsrc2);                  \
	/*                                                      \
	 * 26 xx xx xx xx 23 24 25                              \
	 */                                                     \
	    (void *)vis_alignaddr(dpsrc, 1);                    \
	    t4 = vis_faligndata(dsrc2, dsrc1);                  \
	/*                                                      \
	 * xx xx xx xx xx xx 27 10                              \
	 */                                                     \
	    (void *)vis_alignaddr(dpsrc, 5);                    \
	    t5 = vis_faligndata(dsrc0, dsrc0);                  \
	/*                                                      \
	 * xx xx xx xx xx 02 03 04                              \
	 */                                                     \
	    t2 = vis_faligndata(t5, t2);                        \
	/*                                                      \
	 * 02 03 04 17 00 xx xx xx                              \
	 */                                                     \
	    ddst0 = vis_faligndata(dsrc0, t2);                  \
	/*                                                      \
	 * 05 06 07 02 03 04 17 00                              \
	 */                                                     \
	    t2 = vis_faligndata(t3, dsrc2);                     \
	/*                                                      \
	 * 23 24 25 20 21 22 xx xx                              \
	 */                                                     \
	    t6 = vis_faligndata(t1, t3);                        \
	/*                                                      \
	 * 11 12 13 26 xx xx xx xx                              \
	 */                                                     \
	    t3 = vis_faligndata(dsrc0, t5);                     \
	/*                                                      \
	 * xx xx xx xx xx xx xx 01                              \
	 */                                                     \
	    (void *)vis_alignaddr(dpsrc, 6);                    \
	    ddst2 = vis_faligndata(t4, t2);                     \
	/*                                                      \
	 * 27 10 23 24 25 20 21 22                              \
	 */                                                     \
	    (void *)vis_alignaddr(dpsrc, 3);                    \
	    t1 = vis_faligndata(t3, t1);                        \
	/*                                                      \
	 * xx xx xx xx 01 14 15 16                              \
	 */                                                     \
	    (void *)vis_alignaddr(dpsrc, 4);                    \
	    ddst1 = vis_faligndata(t1, t6);                     \
	/*                                                      \
	 * 01 14 15 16 11 12 13 26                              \
	 */                                                     \
	    dsrc0 = dsrc3;                                      \
	    (void *)vis_alignaddrl((void *)x, 0);               \
	}

#define	REVERSEBYTEORDER_V_U32_AA(src1, src2, ind)                         \
	dpdst[ind] = vis_freg_pair(vis_read_lo(src1), vis_read_hi(src2))

#define	REVERSEBYTEORDER_V_U32_NA(src1, src2, ind)                         \
	dpdst[ind] = vis_freg_pair(vis_read_hi(src1), vis_read_lo(src2))

#endif /* defined(MLIB_VIS) && MLIB_VIS >= 0x200 */

/* *********************************************************** */

mlib_status
__mlib_VectorReverseByteOrder_U16_U16(
	mlib_u16 *z,
	const mlib_u16 *x,
	mlib_s32 n)
{
/* end points in dst */
	mlib_u16 *dend;
	mlib_u16 *dst = (mlib_u16 *)z;
	mlib_u16 *src = (mlib_u16 *)x;
	mlib_d64 dsrc, ddst, s0, s1;

/* edge mask */
	mlib_s32 j, emask;

	if (n > 4) {

		WRITE_BMASK(0x10325476);

		dend = dst + n - 1;

/* common case */
		if (((mlib_addr)src | (mlib_addr)dst) & 7) {

/* 8-byte aligned start points in dst */
			mlib_d64 *dpdst;

/* 8-byte aligned start point in src */
			mlib_d64 *dpsrc;

/* prepare the destination addresses */
			dpdst = (mlib_d64 *)((mlib_addr)dst & (~7));
			j = (mlib_addr)dpdst - (mlib_addr)dst;

/* prepare the source address */
			dpsrc = (mlib_d64 *)vis_alignaddr((void *)src, j);

/* generate edge mask for the start point */
			emask = vis_edge16(dst, dend);
			s1 = vis_ld_d64_nf(dpsrc);

			if (j) {
				s0 = s1;
				s1 = vis_ld_d64_nf(dpsrc + 1);
				dsrc = vis_faligndata(s0, s1);
				REVERSEBYTEORDER_V_U16();
				vis_pst_16(ddst, dpdst++, emask);
				dpsrc++;
				j = (j >> 1) + 4;
			}
#pragma pipeloop(0)
			for (; j <= (n - 4); j += 4) {
				s0 = s1;
				s1 = vis_ld_d64_nf(dpsrc + 1);
				dsrc = vis_faligndata(s0, s1);
				REVERSEBYTEORDER_V_U16();
				(*dpdst++) = ddst;
				dpsrc++;
			}

			if (j < n) {
				dsrc = vis_faligndata(s1,
				    vis_ld_d64_nf(dpsrc + 1));
				REVERSEBYTEORDER_V_U16();
				emask = vis_edge16(dpdst, dend);
				vis_pst_16(ddst, dpdst, emask);
			}
		} else {
/* aligned case */

#pragma pipeloop(0)
			for (j = 0; j < (n - 1) >> 2; j++) {
				dsrc = ((mlib_d64 *)src)[j];
				REVERSEBYTEORDER_V_U16();
				((mlib_d64 *)dst)[j] = ddst;
			}

			dsrc = ((mlib_d64 *)src)[j];
			REVERSEBYTEORDER_V_U16();
			emask = vis_edge16((void *)0,
				(void *)((mlib_addr)dend & 7));
			vis_pst_16(ddst, ((mlib_d64 *)dst) + j, emask);
		}

		return (MLIB_SUCCESS);
	}

	switch (n) {
	    case 1:
		    REVERSEBYTEORDER_C_U16(0);
		    return (MLIB_SUCCESS);
	    case 2:
		    REVERSEBYTEORDER_C_U16(0);
		    REVERSEBYTEORDER_C_U16(1);
		    return (MLIB_SUCCESS);
	    case 3:
		    REVERSEBYTEORDER_C_U16(0);
		    REVERSEBYTEORDER_C_U16(1);
		    REVERSEBYTEORDER_C_U16(2);
		    return (MLIB_SUCCESS);
	    case 4:
		    REVERSEBYTEORDER_C_U16(0);
		    REVERSEBYTEORDER_C_U16(1);
		    REVERSEBYTEORDER_C_U16(2);
		    REVERSEBYTEORDER_C_U16(3);
		    return (MLIB_SUCCESS);
	    default:
		    return (MLIB_FAILURE);
	}
}

/* *********************************************************** */

mlib_status
__mlib_VectorReverseByteOrder_U32_U32(
	mlib_u32 *z,
	const mlib_u32 *x,
	mlib_s32 n)
{
	mlib_s32 *dst = (mlib_s32 *)z;
	mlib_s32 *src = (mlib_s32 *)x;
	mlib_d64 *dpdst, s0, s1, s2;
	mlib_s32 j;

	if (n > 2) {

		if (((mlib_addr)dst) & 7) {
			REVERSEBYTEORDER_C_U32(0);
			dst++;
			src++;
			n--;
		}

		dpdst = (mlib_d64 *)dst;

/* not aligned case */
		if (((mlib_addr)src) & 7) {

			WRITE_BMASK(0x0123cdef);
			src++;
			s2 = vis_ld_d64_nf_le(src - 2);

#pragma pipeloop(0)
/* hand's unroll by 2 */
			for (j = 0; j < (n >> 2); j++) {
				s0 = s2;
				s1 = vis_ld_d64_nf_le(src);
				s2 = vis_ld_d64_nf_le(src + 2);
				REVERSEBYTEORDER_V_U32_NA(s0, s1, 0);
				REVERSEBYTEORDER_V_U32_NA(s1, s2, 1);
				src += 4;
				dpdst += 2;
			}

			if (n & 2) {
				s1 = s2;
				s2 = vis_ld_d64_nf_le(src);
				REVERSEBYTEORDER_V_U32_NA(s1, s2, 0);
				src += 2;
				dpdst++;
			}

			if (n & 1) {
				*((mlib_f32 *)dpdst) = vis_read_hi(s2);
			}
		} else {
/* aligned case */

			WRITE_BMASK(0x456789ab);

#pragma pipeloop(0)
			for (j = 0; j < (n >> 2); j++) {
				s0 = vis_ld_d64_nf_le(src);
				s1 = vis_ld_d64_nf_le(src + 2);
				REVERSEBYTEORDER_V_U32_AA(s0, s0, 0);
				REVERSEBYTEORDER_V_U32_AA(s1, s1, 1);
				src += 4;
				dpdst += 2;
			}

			if (n & 2) {
				s0 = vis_ld_d64_nf_le(src);
				REVERSEBYTEORDER_V_U32_AA(s0, s0, 0);
				src += 2;
				dpdst++;
			}

			if (n & 1) {
				s0 = vis_ld_d64_nf_le(src);
				*((mlib_f32 *)dpdst) = vis_read_lo(s0);
			}
		}

		return (MLIB_SUCCESS);
	}

	switch (n) {
	    case 1:
		    REVERSEBYTEORDER_C_U32(0);
		    return (MLIB_SUCCESS);
	    case 2:
		    REVERSEBYTEORDER_C_U32(0);
		    REVERSEBYTEORDER_C_U32(1);
		    return (MLIB_SUCCESS);
	    default:
		    return (MLIB_FAILURE);
	}
}

/* *********************************************************** */

mlib_status
__mlib_VectorReverseByteOrder_U64_U64(
	mlib_u64 *z,
	const mlib_u64 *x,
	mlib_s32 n)
{
	mlib_d64 *dpdst = (mlib_d64 *)z;
	mlib_d64 *dpsrc = (mlib_d64 *)x;
	mlib_s32 j;

#if defined(MLIB_VIS) && MLIB_VIS >= 0x200

	mlib_d64 s0, s1, s2, s3;

	if (n <= 0)
		return (MLIB_FAILURE);

#pragma pipeloop(0)
	for (j = 0; j <= (n - 4); j += 4) {
		s0 = vis_ld_d64_le(dpsrc + j);
		s1 = vis_ld_d64_le(dpsrc + j + 1);
		s2 = vis_ld_d64_le(dpsrc + j + 2);
		s3 = vis_ld_d64_le(dpsrc + j + 3);
		dpdst[j + 0] = s0;
		dpdst[j + 1] = s1;
		dpdst[j + 2] = s2;
		dpdst[j + 3] = s3;
	}

#pragma unroll(1)
	for (; j < n; j++) {
		dpdst[j] = vis_ld_d64_le(dpsrc + j);
	}

#else /* defined(MLIB_VIS) && MLIB_VIS >= 0x200 */

	if (n <= 0)
		return (MLIB_FAILURE);

	if (j = n & 1)
		dpdst[0] = vis_ld_d64_le(dpsrc);

#pragma pipeloop(0)
	for (; j < n; j += 2) {
		dpdst[j] = vis_ld_d64_le(dpsrc + j);
		dpdst[j + 1] = vis_ld_d64_le(dpsrc + j + 1);
	}

#endif /* defined(MLIB_VIS) && MLIB_VIS >= 0x200 */

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

static mlib_status
mlib_VectorReverseByteOrder_U24_U24(
	mlib_u8 *z,
	const mlib_u8 *x,
	mlib_s32 n)
{
	mlib_d64 *dpsrc, *dpdst;
	mlib_d64 dsrc0, dsrc1, dsrc2, dsrc3;
	mlib_d64 ddst0, ddst1, ddst2;
	mlib_u32 v0, v1, v2;
	mlib_s32 i;

	if (n <= 8) {

		for (i = 0; i < n; i++) {
			v0 = x[3 * i];
			v1 = x[3 * i + 1];
			v2 = x[3 * i + 2];
			z[3 * i + 2] = v0;
			z[3 * i + 1] = v1;
			z[3 * i] = v2;
		}

		return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
	}

	while ((mlib_addr)z & 7) {
		v0 = x[0];
		v1 = x[1];
		v2 = x[2];
		z[2] = v0;
		z[1] = v1;
		z[0] = v2;
		x += 3;
		z += 3;
		n--;
	}

	dpdst = (mlib_d64 *)z;

	if (((mlib_addr)x & 7) == 0) {

		PREPARE_V_U24(z, x);

#pragma pipeloop(0)
		for (i = 0; i < n / 8; i++) {
			REVERSEBYTEORDER_V_U24();
			dpdst[3 * i] = ddst0;
			dpdst[3 * i + 1] = ddst1;
			dpdst[3 * i + 2] = ddst2;
		}
	} else {

		PREPARE_V_U24_NA();

#pragma pipeloop(0)
		for (i = 0; i < n / 8; i++) {
			REVERSEBYTEORDER_V_U24_NA();
			dpdst[3 * i] = ddst0;
			dpdst[3 * i + 1] = ddst1;
			dpdst[3 * i + 2] = ddst2;
		}
	}

	for (i = n & ~7; i < n; i++) {
		v0 = x[3 * i];
		v1 = x[3 * i + 1];
		v2 = x[3 * i + 2];
		z[3 * i + 2] = v0;
		z[3 * i + 1] = v1;
		z[3 * i] = v2;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorReverseByteOrder(
	void *z,
	const void *x,
	mlib_s32 n,
	mlib_s32 s)
{
	mlib_s32 i, v0, v1, v2, v3, v4, v5, v6, v7;

	switch (s) {
	    case 2:

		    if ((((mlib_addr)x | (mlib_addr)z) & 1) == 0)
			    return __mlib_VectorReverseByteOrder_U16_U16(
				    (mlib_u16 *)z, (const mlib_u16 *)x, n);

#pragma pipeloop(0)
		    for (i = 0; i < n; i++) {
			    v0 = ((const mlib_u8 *)x)[2 * i];
			    v1 = ((const mlib_u8 *)x)[2 * i + 1];
			    ((mlib_u8 *)z)[2 * i + 1] = v0;
			    ((mlib_u8 *)z)[2 * i] = v1;
		    }

		    break;

	    case 3:
		    return mlib_VectorReverseByteOrder_U24_U24((mlib_u8 *)z,
			    (const mlib_u8 *)x, n);

	    case 4:

		    if ((((mlib_addr)x | (mlib_addr)z) & 3) == 0)
			    return __mlib_VectorReverseByteOrder_U32_U32(
				    (mlib_u32 *)z, (const mlib_u32 *)x, n);

#pragma pipeloop(0)
		    for (i = 0; i < n; i++) {
			    v0 = ((const mlib_u8 *)x)[4 * i];
			    v1 = ((const mlib_u8 *)x)[4 * i + 1];
			    v2 = ((const mlib_u8 *)x)[4 * i + 2];
			    v3 = ((const mlib_u8 *)x)[4 * i + 3];
			    ((mlib_u8 *)z)[4 * i + 3] = v0;
			    ((mlib_u8 *)z)[4 * i + 2] = v1;
			    ((mlib_u8 *)z)[4 * i + 1] = v2;
			    ((mlib_u8 *)z)[4 * i] = v3;
		    }

		    break;

	    case 5:
#pragma pipeloop(0)
		    for (i = 0; i < n; i++) {
			    v0 = ((const mlib_u8 *)x)[5 * i];
			    v1 = ((const mlib_u8 *)x)[5 * i + 1];
			    v2 = ((const mlib_u8 *)x)[5 * i + 2];
			    v3 = ((const mlib_u8 *)x)[5 * i + 3];
			    v4 = ((const mlib_u8 *)x)[5 * i + 4];
			    ((mlib_u8 *)z)[5 * i + 4] = v0;
			    ((mlib_u8 *)z)[5 * i + 3] = v1;
			    ((mlib_u8 *)z)[5 * i + 2] = v2;
			    ((mlib_u8 *)z)[5 * i + 1] = v3;
			    ((mlib_u8 *)z)[5 * i] = v4;
		    }

		    break;

	    case 6:
#pragma pipeloop(0)
		    for (i = 0; i < n; i++) {
			    v0 = ((const mlib_u8 *)x)[6 * i];
			    v1 = ((const mlib_u8 *)x)[6 * i + 1];
			    v2 = ((const mlib_u8 *)x)[6 * i + 2];
			    v3 = ((const mlib_u8 *)x)[6 * i + 3];
			    v4 = ((const mlib_u8 *)x)[6 * i + 4];
			    v5 = ((const mlib_u8 *)x)[6 * i + 5];
			    ((mlib_u8 *)z)[6 * i + 5] = v0;
			    ((mlib_u8 *)z)[6 * i + 4] = v1;
			    ((mlib_u8 *)z)[6 * i + 3] = v2;
			    ((mlib_u8 *)z)[6 * i + 2] = v3;
			    ((mlib_u8 *)z)[6 * i + 1] = v4;
			    ((mlib_u8 *)z)[6 * i] = v5;
		    }

		    break;

	    case 7:
#pragma pipeloop(0)
		    for (i = 0; i < n; i++) {
			    v0 = ((const mlib_u8 *)x)[7 * i];
			    v1 = ((const mlib_u8 *)x)[7 * i + 1];
			    v2 = ((const mlib_u8 *)x)[7 * i + 2];
			    v3 = ((const mlib_u8 *)x)[7 * i + 3];
			    v4 = ((const mlib_u8 *)x)[7 * i + 4];
			    v5 = ((const mlib_u8 *)x)[7 * i + 5];
			    v6 = ((const mlib_u8 *)x)[7 * i + 6];
			    ((mlib_u8 *)z)[7 * i + 6] = v0;
			    ((mlib_u8 *)z)[7 * i + 5] = v1;
			    ((mlib_u8 *)z)[7 * i + 4] = v2;
			    ((mlib_u8 *)z)[7 * i + 3] = v3;
			    ((mlib_u8 *)z)[7 * i + 2] = v4;
			    ((mlib_u8 *)z)[7 * i + 1] = v5;
			    ((mlib_u8 *)z)[7 * i] = v6;
		    }

		    break;

	    case 8:

		    if ((((mlib_addr)x | (mlib_addr)z) & 7) == 0)
			    return __mlib_VectorReverseByteOrder_U64_U64(
				    (mlib_u64 *)z, (const mlib_u64 *)x, n);

#pragma pipeloop(0)
		    for (i = 0; i < n; i++) {
			    v0 = ((const mlib_u8 *)x)[8 * i];
			    v1 = ((const mlib_u8 *)x)[8 * i + 1];
			    v2 = ((const mlib_u8 *)x)[8 * i + 2];
			    v3 = ((const mlib_u8 *)x)[8 * i + 3];
			    v4 = ((const mlib_u8 *)x)[8 * i + 4];
			    v5 = ((const mlib_u8 *)x)[8 * i + 5];
			    v6 = ((const mlib_u8 *)x)[8 * i + 6];
			    v7 = ((const mlib_u8 *)x)[8 * i + 7];
			    ((mlib_u8 *)z)[8 * i + 7] = v0;
			    ((mlib_u8 *)z)[8 * i + 6] = v1;
			    ((mlib_u8 *)z)[8 * i + 5] = v2;
			    ((mlib_u8 *)z)[8 * i + 4] = v3;
			    ((mlib_u8 *)z)[8 * i + 3] = v4;
			    ((mlib_u8 *)z)[8 * i + 2] = v5;
			    ((mlib_u8 *)z)[8 * i + 1] = v6;
			    ((mlib_u8 *)z)[8 * i] = v7;
		    }

		    break;

	    default:
		    return (MLIB_FAILURE);
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorReverseByteOrder_U16(
	mlib_u16 *xz,
	mlib_s32 n)
{
/* end point */
	mlib_u16 *dend;
	mlib_u16 *srcdst = (mlib_u16 *)xz;
	mlib_d64 dsrc, ddst;

/* edge mask */
	mlib_s32 j, emask;

	if (n > 4) {

		WRITE_BMASK(0x10325476);

		dend = srcdst + n - 1;

/* common case */
		if ((mlib_addr)srcdst & 7) {

/* 8-byte aligned start point */
			mlib_d64 *dp;

/* prepare the source address */
			dp = (mlib_d64 *)vis_alignaddr((void *)srcdst, 0);

/* prepare the destination addresses */
			j = (mlib_addr)dp - (mlib_addr)srcdst;

/* generate edge mask for the start point */
			emask = vis_edge16(srcdst, dend);

			if (j) {
				dsrc = dp[0];
				REVERSEBYTEORDER_V_U16();
				vis_pst_16(ddst, dp++, emask);
				j = (j >> 1) + 4;
			}
#pragma pipeloop(0)
			for (; j <= (n - 4); j += 4) {
				dsrc = dp[0];
				REVERSEBYTEORDER_V_U16();
				(*dp++) = ddst;
			}

			if (j < n) {
				dsrc = dp[0];
				REVERSEBYTEORDER_V_U16();
				emask = vis_edge16(dp, dend);
				vis_pst_16(ddst, dp, emask);
			}
		} else {
/* aligned case */

#pragma pipeloop(0)
			for (j = 0; j < (n - 1) >> 2; j++) {
				dsrc = ((mlib_d64 *)srcdst)[j];
				REVERSEBYTEORDER_V_U16();
				((mlib_d64 *)srcdst)[j] = ddst;
			}

			dsrc = ((mlib_d64 *)srcdst)[j];
			REVERSEBYTEORDER_V_U16();
			emask = vis_edge16((void *)0,
				(void *)((mlib_addr)dend & 7));
			vis_pst_16(ddst, ((mlib_d64 *)srcdst) + j, emask);
		}

		return (MLIB_SUCCESS);
	}

	switch (n) {
	    case 1:
		    REVERSEBYTEORDER_C_U16_INP(0);
		    return (MLIB_SUCCESS);
	    case 2:
		    REVERSEBYTEORDER_C_U16_INP(0);
		    REVERSEBYTEORDER_C_U16_INP(1);
		    return (MLIB_SUCCESS);
	    case 3:
		    REVERSEBYTEORDER_C_U16_INP(0);
		    REVERSEBYTEORDER_C_U16_INP(1);
		    REVERSEBYTEORDER_C_U16_INP(2);
		    return (MLIB_SUCCESS);
	    case 4:
		    REVERSEBYTEORDER_C_U16_INP(0);
		    REVERSEBYTEORDER_C_U16_INP(1);
		    REVERSEBYTEORDER_C_U16_INP(2);
		    REVERSEBYTEORDER_C_U16_INP(3);
		    return (MLIB_SUCCESS);
	    default:
		    return (MLIB_FAILURE);
	}
}

/* *********************************************************** */

mlib_status
__mlib_VectorReverseByteOrder_U32(
	mlib_u32 *xz,
	mlib_s32 n)
{
	mlib_s32 *srcdst = (mlib_s32 *)xz;
	mlib_d64 *dpdst, s0, s1;
	mlib_s32 j;

	if (n > 2) {

		if (((mlib_addr)srcdst) & 7) {
			REVERSEBYTEORDER_C_U32_INP(0);
			srcdst++;
			n--;
		}

		dpdst = (mlib_d64 *)srcdst;
		WRITE_BMASK(0x456789ab);

#pragma pipeloop(0)
		for (j = 0; j < (n >> 2); j++) {
			s0 = vis_ld_d64_le(dpdst);
			s1 = vis_ld_d64_le(dpdst + 1);
			REVERSEBYTEORDER_V_U32_AA(s0, s0, 0);
			REVERSEBYTEORDER_V_U32_AA(s1, s1, 1);
			dpdst += 2;
		}

		if (n & 2) {
			s0 = vis_ld_d64_le(dpdst);
			REVERSEBYTEORDER_V_U32_AA(s0, s0, 0);
			dpdst++;
		}

		if (n & 1) {
			s0 = vis_ld_d64_le(dpdst);
			*((mlib_f32 *)dpdst) = vis_read_lo(s0);
		}

		return (MLIB_SUCCESS);
	}

	switch (n) {
	    case 1:
		    REVERSEBYTEORDER_C_U32_INP(0);
		    return (MLIB_SUCCESS);
	    case 2:
		    REVERSEBYTEORDER_C_U32_INP(0);
		    REVERSEBYTEORDER_C_U32_INP(1);
		    return (MLIB_SUCCESS);
	    default:
		    return (MLIB_FAILURE);
	}
}

/* *********************************************************** */

mlib_status
__mlib_VectorReverseByteOrder_U64(
	mlib_u64 *xz,
	mlib_s32 n)
{
	mlib_d64 *dp = (mlib_d64 *)xz;
	mlib_s32 j;

#if defined(MLIB_VIS) && MLIB_VIS >= 0x200

	mlib_d64 s0, s1, s2, s3;

	if (n <= 0)
		return (MLIB_FAILURE);

#pragma pipeloop(0)
	for (j = 0; j <= (n - 4); j += 4) {
		s0 = vis_ld_d64_le(dp + j);
		s1 = vis_ld_d64_le(dp + j + 1);
		s2 = vis_ld_d64_le(dp + j + 2);
		s3 = vis_ld_d64_le(dp + j + 3);
		dp[j + 0] = s0;
		dp[j + 1] = s1;
		dp[j + 2] = s2;
		dp[j + 3] = s3;
	}

#pragma unroll(1)
	for (; j < n; j++) {
		dp[j] = vis_ld_d64_le(dp + j);
	}

#else /* defined(MLIB_VIS) && MLIB_VIS >= 0x200 */

	if (n <= 0)
		return (MLIB_FAILURE);

	if (j = n & 1)
		dp[0] = vis_ld_d64_le(dp);

#pragma pipeloop(0)
	for (; j < n; j += 2) {
		dp[j] = vis_ld_d64_le(dp + j);
		dp[j + 1] = vis_ld_d64_le(dp + j + 1);
	}

#endif /* defined(MLIB_VIS) && MLIB_VIS >= 0x200 */

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

static mlib_status
mlib_VectorReverseByteOrder_U24(
	mlib_u8 *xz,
	mlib_s32 n)
{
	mlib_d64 *dpsrc;
	mlib_d64 dsrc0, dsrc1, dsrc2;
	mlib_d64 ddst0, ddst1, ddst2;
	mlib_u32 v0, v1;
	mlib_s32 i;

	if (n <= 8) {

		for (i = 0; i < n; i++) {
			v0 = xz[3 * i];
			v1 = xz[3 * i + 2];
			xz[3 * i + 2] = v0;
			xz[3 * i] = v1;
		}

		return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
	}

	while ((mlib_addr)xz & 7) {
		v0 = xz[0];
		v1 = xz[2];
		xz[2] = v0;
		xz[0] = v1;
		xz += 3;
		n--;
	}

	PREPARE_V_U24(xz, xz);

#pragma pipeloop(0)
	for (i = 0; i < n / 8; i++) {
		REVERSEBYTEORDER_V_U24();
		dpsrc[3 * i] = ddst0;
		dpsrc[3 * i + 1] = ddst1;
		dpsrc[3 * i + 2] = ddst2;
	}

	for (i *= 8; i < n; i++) {
		v0 = xz[3 * i];
		v1 = xz[3 * i + 2];
		xz[3 * i + 2] = v0;
		xz[3 * i] = v1;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorReverseByteOrder_Inp(
	void *xz,
	mlib_s32 n,
	mlib_s32 s)
{
	mlib_s32 i, v0, v1, v2, v3, v4, v5, v6, v7;

	switch (s) {
	    case 2:

		    if (((mlib_addr)xz & 1) == 0)
			    return __mlib_VectorReverseByteOrder_U16((mlib_u16
				    *)xz, n);

#pragma pipeloop(0)
		    for (i = 0; i < n; i++) {
			    v0 = ((const mlib_u8 *)xz)[2 * i];
			    v1 = ((const mlib_u8 *)xz)[2 * i + 1];
			    ((mlib_u8 *)xz)[2 * i + 1] = v0;
			    ((mlib_u8 *)xz)[2 * i] = v1;
		    }

		    break;

	    case 3:
		    return (mlib_VectorReverseByteOrder_U24((mlib_u8 *)xz, n));

	    case 4:

		    if (((mlib_addr)xz & 3) == 0)
			    return __mlib_VectorReverseByteOrder_U32((mlib_u32
				    *)xz, n);

#pragma pipeloop(0)
		    for (i = 0; i < n; i++) {
			    v0 = ((const mlib_u8 *)xz)[4 * i];
			    v1 = ((const mlib_u8 *)xz)[4 * i + 1];
			    v2 = ((const mlib_u8 *)xz)[4 * i + 2];
			    v3 = ((const mlib_u8 *)xz)[4 * i + 3];
			    ((mlib_u8 *)xz)[4 * i + 3] = v0;
			    ((mlib_u8 *)xz)[4 * i + 2] = v1;
			    ((mlib_u8 *)xz)[4 * i + 1] = v2;
			    ((mlib_u8 *)xz)[4 * i] = v3;
		    }

		    break;

	    case 5:
#pragma pipeloop(0)
		    for (i = 0; i < n; i++) {
			    v0 = ((const mlib_u8 *)xz)[5 * i];
			    v1 = ((const mlib_u8 *)xz)[5 * i + 1];
			    v3 = ((const mlib_u8 *)xz)[5 * i + 3];
			    v4 = ((const mlib_u8 *)xz)[5 * i + 4];
			    ((mlib_u8 *)xz)[5 * i + 4] = v0;
			    ((mlib_u8 *)xz)[5 * i + 3] = v1;
			    ((mlib_u8 *)xz)[5 * i + 1] = v3;
			    ((mlib_u8 *)xz)[5 * i] = v4;
		    }

		    break;

	    case 6:
#pragma pipeloop(0)
		    for (i = 0; i < n; i++) {
			    v0 = ((const mlib_u8 *)xz)[6 * i];
			    v1 = ((const mlib_u8 *)xz)[6 * i + 1];
			    v2 = ((const mlib_u8 *)xz)[6 * i + 2];
			    v3 = ((const mlib_u8 *)xz)[6 * i + 3];
			    v4 = ((const mlib_u8 *)xz)[6 * i + 4];
			    v5 = ((const mlib_u8 *)xz)[6 * i + 5];
			    ((mlib_u8 *)xz)[6 * i + 5] = v0;
			    ((mlib_u8 *)xz)[6 * i + 4] = v1;
			    ((mlib_u8 *)xz)[6 * i + 3] = v2;
			    ((mlib_u8 *)xz)[6 * i + 2] = v3;
			    ((mlib_u8 *)xz)[6 * i + 1] = v4;
			    ((mlib_u8 *)xz)[6 * i] = v5;
		    }

		    break;

	    case 7:
#pragma pipeloop(0)
		    for (i = 0; i < n; i++) {
			    v0 = ((const mlib_u8 *)xz)[7 * i];
			    v1 = ((const mlib_u8 *)xz)[7 * i + 1];
			    v2 = ((const mlib_u8 *)xz)[7 * i + 2];
			    v4 = ((const mlib_u8 *)xz)[7 * i + 4];
			    v5 = ((const mlib_u8 *)xz)[7 * i + 5];
			    v6 = ((const mlib_u8 *)xz)[7 * i + 6];
			    ((mlib_u8 *)xz)[7 * i + 6] = v0;
			    ((mlib_u8 *)xz)[7 * i + 5] = v1;
			    ((mlib_u8 *)xz)[7 * i + 4] = v2;
			    ((mlib_u8 *)xz)[7 * i + 2] = v4;
			    ((mlib_u8 *)xz)[7 * i + 1] = v5;
			    ((mlib_u8 *)xz)[7 * i] = v6;
		    }

		    break;

	    case 8:

		    if (((mlib_addr)xz & 7) == 0)
			    return __mlib_VectorReverseByteOrder_U64((mlib_u64
				    *)xz, n);

#pragma pipeloop(0)
		    for (i = 0; i < n; i++) {
			    v0 = ((const mlib_u8 *)xz)[8 * i];
			    v1 = ((const mlib_u8 *)xz)[8 * i + 1];
			    v2 = ((const mlib_u8 *)xz)[8 * i + 2];
			    v3 = ((const mlib_u8 *)xz)[8 * i + 3];
			    v4 = ((const mlib_u8 *)xz)[8 * i + 4];
			    v5 = ((const mlib_u8 *)xz)[8 * i + 5];
			    v6 = ((const mlib_u8 *)xz)[8 * i + 6];
			    v7 = ((const mlib_u8 *)xz)[8 * i + 7];
			    ((mlib_u8 *)xz)[8 * i + 7] = v0;
			    ((mlib_u8 *)xz)[8 * i + 6] = v1;
			    ((mlib_u8 *)xz)[8 * i + 5] = v2;
			    ((mlib_u8 *)xz)[8 * i + 4] = v3;
			    ((mlib_u8 *)xz)[8 * i + 3] = v4;
			    ((mlib_u8 *)xz)[8 * i + 2] = v5;
			    ((mlib_u8 *)xz)[8 * i + 1] = v6;
			    ((mlib_u8 *)xz)[8 * i] = v7;
		    }

		    break;

	    default:
		    return (MLIB_FAILURE);
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */
