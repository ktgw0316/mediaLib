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

#pragma ident	"@(#)mlib_v_SignalMulShift.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalMulShift_[S16|S16S]_Sat,
 *      mlib_SignalMulShift_[S16|S16S]_[S16|S16S]_Sat - multiplication of
 *                     signed 16-bit format signal arrays
 *
 * SYNOPSIS
 *      mlib_status mlib_SignalMulShift_[S16|S16S]_[S16|S16S]_Sat
 *                                                (mlib_s16       *dst,
 *                                                 const mlib_s16 *src1,
 *                                                 const mlib_s16 *src2,
 *                                                 mlib_s32       shift,
 *                                                 mlib_s32       n)
 *      mlib_status mlib_SignalMulShift_[S16|S16S]_Sat
 *                                                (mlib_s16       *data,
 *                                                 const mlib_s16 *src2,
 *                                                 mlib_s32       shift,
 *                                                 mlib_s32       n)
 *
 * ARGUMENTS
 *      data  Input and output signal array
 *      dst   Output signal array
 *      src   Input signal array
 *      shift Left shifting factor, may be negative
 *      n     Number of samples in the input signal arrays.
 *
 * DESCRIPTION
 *      dst = src1 * src2 * (2 ** shift)
 */

#include <vis_proto.h>
#include <mlib_signal.h>

#if MLIB_VIS < 0x200
#include <mlib_algebra.h>
#endif /* MLIB_VIS < 0x200 */

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalMulShift_S16_S16_Sat = \
	__mlib_SignalMulShift_S16_S16_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalMulShift_S16_S16_Sat) mlib_SignalMulShift_S16_S16_Sat
    __attribute__((weak, alias("__mlib_SignalMulShift_S16_S16_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

#define	FLOAT2INT_CLAMP(DST, SRC)	DST = ((mlib_s32)(SRC) >> 16)

#else /* MLIB_USE_FTOI_CLAMPING */

#define	FLOAT2INT_CLAMP(DST, SRC)                               \
	{                                                       \
	    mlib_d64 dsrc = (mlib_d64)(SRC);                    \
	                                                        \
	    if (dsrc > (mlib_d64)MLIB_S32_MAX)                  \
		dsrc = (mlib_d64)MLIB_S32_MAX;                  \
	    if (dsrc < (mlib_d64)MLIB_S32_MIN)                  \
		dsrc = (mlib_d64)MLIB_S32_MIN;                  \
	    DST = (mlib_s32)dsrc >> 16;                         \
	}

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#define	MLIB_SCALMUL                                                       \
	dr0 = vis_fmuld8sux16(vis_read_hi(v_src1), v_mul3);                \
	dr1 = vis_fmuld8ulx16(vis_read_hi(v_src1), v_mul3);                \
	dr2 = vis_fmuld8sux16(vis_read_lo(v_src1), v_mul3);                \
	dr3 = vis_fmuld8ulx16(vis_read_lo(v_src1), v_mul3);                \
	dr0 = vis_fpadd32(dr0, dr1);                                       \
	dr2 = vis_fpadd32(dr2, dr3);                                       \
	dr0 = vis_fpadd32(dr0, dr0);                                       \
	dr2 = vis_fpadd32(dr2, dr2);                                       \
	v_src1 = vis_fpackfix_pair(dr0, dr2);                              \
	dr0 = vis_fmuld8sux16(vis_read_hi(v_src2), vis_read_hi(v_src1));   \
	dr1 = vis_fmuld8ulx16(vis_read_hi(v_src2), vis_read_hi(v_src1));   \
	dr2 = vis_fmuld8sux16(vis_read_lo(v_src2), vis_read_lo(v_src1));   \
	dr3 = vis_fmuld8ulx16(vis_read_lo(v_src2), vis_read_lo(v_src1))

/* *********************************************************** */

#define	MLIB_4MUL                                                          \
	dr0 = vis_fmuld8sux16(vis_read_hi(v_src2), vis_read_hi(v_src1));   \
	dr1 = vis_fmuld8ulx16(vis_read_hi(v_src2), vis_read_hi(v_src1));   \
	dr2 = vis_fmuld8sux16(vis_read_lo(v_src2), vis_read_lo(v_src1));   \
	dr3 = vis_fmuld8ulx16(vis_read_lo(v_src2), vis_read_lo(v_src1))

/* *********************************************************** */

#define	MLIB_2ADD                                               \
	v_dst0 = vis_fpadd32(dr0, dr1);                         \
	v_dst1 = vis_fpadd32(dr2, dr3)

/* *********************************************************** */

#define	MLIB_5MUL                                                          \
	v_src1 = vis_fmul8x16(v_mul1, v_src1);                             \
	dr0 = vis_fmuld8sux16(vis_read_hi(v_src2), vis_read_hi(v_src1));   \
	dr1 = vis_fmuld8ulx16(vis_read_hi(v_src2), vis_read_hi(v_src1));   \
	dr2 = vis_fmuld8sux16(vis_read_lo(v_src2), vis_read_lo(v_src1));   \
	dr3 = vis_fmuld8ulx16(vis_read_lo(v_src2), vis_read_lo(v_src1))

/* *********************************************************** */

#define	MLIB_6MUL                                                          \
	v_src1 = vis_fmul8x16(v_mul1, v_src1);                             \
	v_src2 = vis_fmul8x16(v_mul2, v_src2);                             \
	dr0 = vis_fmuld8sux16(vis_read_hi(v_src2), vis_read_hi(v_src1));   \
	dr1 = vis_fmuld8ulx16(vis_read_hi(v_src2), vis_read_hi(v_src1));   \
	dr2 = vis_fmuld8sux16(vis_read_lo(v_src2), vis_read_lo(v_src1));   \
	dr3 = vis_fmuld8ulx16(vis_read_lo(v_src2), vis_read_lo(v_src1))

/* *********************************************************** */

#define	MLIB_6ADD                                               \
	v_dst0 = vis_fpadd32(dr0, dr1);                         \
	v_dst1 = vis_fpadd32(dr2, dr3);                         \
	v_dst0 = vis_fpadd32(v_dst0, v_dst0);                   \
	v_dst1 = vis_fpadd32(v_dst1, v_dst1);                   \
	v_dst0 = vis_fpadd32(v_dst0, v_one);                    \
	v_dst1 = vis_fpadd32(v_dst1, v_one)

/* *********************************************************** */

#define	MLIB_FIX	v_dst = vis_fpackfix_pair(v_dst0, v_dst1)

/* *********************************************************** */

#define	MLIB_MULSHIFT_AL(SHIFT, ADD, MUL, FIX)                    \
	{                                                         \
	    vis_write_gsr((SHIFT << 3));                          \
	    dpd = (mlib_d64 *)((mlib_addr)dst & ~7);              \
	    dps1 = (mlib_d64 *)((mlib_addr)src1 & ~7);            \
	    dps2 = (mlib_d64 *)((mlib_addr)src2 & ~7);            \
	    v_src1 = vis_ld_d64_nf(dps1);                         \
	    v_src2 = vis_ld_d64_nf(dps2);                         \
	    MUL;                                                  \
	    ADD;                                                  \
	    FIX;                                                  \
	    dps1++;                                               \
	    dps2++;                                               \
								  \
	    v_src1 = vis_ld_d64_nf(dps1);                         \
	    v_src2 = vis_ld_d64_nf(dps2);                         \
	    MUL;                                                  \
	    ADD;                                                  \
	    vis_pst_16(v_dst, dpd, vis_edge16(dst, fdst));        \
	    dpd++;                                                \
	    FIX;                                                  \
	    times = ((mlib_d64 *)((mlib_addr)fdst & ~7) - dpd);   \
								  \
	    v_src1 = vis_ld_d64_nf(dps1 + 1);                     \
	    v_src2 = vis_ld_d64_nf(dps2 + 1);                     \
	    MUL;                                                  \
								  \
	    v_src1 = vis_ld_d64_nf(dps1 + 2);                     \
	    v_src2 = vis_ld_d64_nf(dps2 + 2);                     \
	    dps1 += 3;                                            \
	    dps2 += 3;                                            \
	    for (i = 2; i < times; i++) {                         \
		ADD;                                              \
		MUL;                                              \
		*dpd = v_dst;                                     \
		v_src1 = vis_ld_d64_nf(dps1);                     \
		v_src2 = vis_ld_d64_nf(dps2);                     \
		FIX;                                              \
		dpd++;                                            \
		dps1++;                                           \
		dps2++;                                           \
	    }                                                     \
	    *dpd = v_dst;                                         \
	    dpd++;                                                \
	    ADD;                                                  \
	    MUL;                                                  \
	    FIX;                                                  \
	    *dpd = v_dst;                                         \
	    dpd++;                                                \
	    ADD;                                                  \
	    FIX;                                                  \
	    vis_pst_16(v_dst, dpd, vis_edge16(dpd, fdst));        \
	}

/* *********************************************************** */

#define	MLIB_MULSHIFT_GA(SHIFT, ADD, MUL, FIX)                    \
	{                                                         \
	    vis_write_gsr((SHIFT << 3));                          \
	    dpd = (mlib_d64 *)((mlib_addr)dst & ~7);              \
	    t = -((mlib_addr)dst & 7);                            \
	    dps1 = vis_alignaddr((void *)src1, t);                \
	    dps2 = vis_alignaddr((void *)src2, t);                \
	    v_src1 = vis_ld_d64_nf(dps1);                         \
	    v_src10 = vis_ld_d64_nf(dps1 + 1);                    \
	    v_src2 = vis_ld_d64_nf(dps2);                         \
	    v_src20 = vis_ld_d64_nf(dps2 + 1);                    \
	    v_src1 = vis_faligndata(v_src1, v_src10);             \
	    v_src2 = vis_faligndata(v_src2, v_src20);             \
	    MUL;                                                  \
	    ADD;                                                  \
	    FIX;                                                  \
	    dps1++;                                               \
	    dps2++;                                               \
								  \
	    v_src11 = vis_ld_d64_nf(dps1 + 1);                    \
	    v_src21 = vis_ld_d64_nf(dps2 + 1);                    \
	    v_src1 = vis_faligndata(v_src10, v_src11);            \
	    v_src2 = vis_faligndata(v_src20, v_src21);            \
	    MUL;                                                  \
	    ADD;                                                  \
	    vis_pst_16(v_dst, dpd, vis_edge16(dst, fdst));        \
	    dpd++;                                                \
	    FIX;                                                  \
	    times = ((mlib_d64 *)((mlib_addr)fdst & ~7) - dpd);   \
								  \
	    v_src10 = vis_ld_d64_nf(dps1 + 2);                    \
	    v_src20 = vis_ld_d64_nf(dps2 + 2);                    \
	    v_src1 = vis_faligndata(v_src11, v_src10);            \
	    v_src2 = vis_faligndata(v_src21, v_src20);            \
	    MUL;                                                  \
								  \
	    v_src11 = vis_ld_d64_nf(dps1 + 3);                    \
	    v_src21 = vis_ld_d64_nf(dps2 + 3);                    \
	    v_src1 = vis_faligndata(v_src10, v_src11);            \
	    v_src2 = vis_faligndata(v_src20, v_src21);            \
	    dps1 += 4;                                            \
	    dps2 += 4;                                            \
	    for (i = 2; i < times; i++) {                         \
		ADD;                                              \
		MUL;                                              \
		v_src10 = v_src11;                                \
		v_src11 = vis_ld_d64_nf(dps1);                    \
		v_src20 = v_src21;                                \
		v_src21 = vis_ld_d64_nf(dps2);                    \
		*dpd = v_dst;                                     \
		FIX;                                              \
		v_src1 = vis_faligndata(v_src10, v_src11);        \
		v_src2 = vis_faligndata(v_src20, v_src21);        \
		dpd++;                                            \
		dps1++;                                           \
		dps2++;                                           \
	    }                                                     \
	    *dpd = v_dst;                                         \
	    dpd++;                                                \
	    ADD;                                                  \
	    MUL;                                                  \
	    FIX;                                                  \
	    *dpd = v_dst;                                         \
	    dpd++;                                                \
	    ADD;                                                  \
	    FIX;                                                  \
	    vis_pst_16(v_dst, dpd, vis_edge16(dpd, fdst));        \
	}

/* *********************************************************** */

#define	MLIB_MULSHIFT_GE(SHIFT, ADD, MUL, FIX)                    \
	{                                                         \
	    vis_write_gsr((SHIFT << 3));                          \
	    dpd = vis_alignaddr((void *)dst, 0);                  \
	    t = -((mlib_addr)dst & 7);                            \
	    dps1 = vis_alignaddr((void *)src1, t);                \
	    dps2 = vis_alignaddr((void *)src2, t);                \
	    v_src2 = vis_ld_d64_nf(dps2);                         \
	    v_src20 = vis_ld_d64_nf(dps2 + 1);                    \
	    v_src1 = vis_ld_d64_nf(dps1);                         \
	    v_src10 = vis_ld_d64_nf(dps1 + 1);                    \
	    v_src2 = vis_faligndata(v_src2, v_src20);             \
	    vis_alignaddr((void *)src1, t);                       \
	    v_src1 = vis_faligndata(v_src1, v_src10);             \
	    MUL;                                                  \
	    ADD;                                                  \
	    FIX;                                                  \
	    dps1++;                                               \
	    dps2++;                                               \
								  \
	    v_src11 = vis_ld_d64_nf(dps1 + 1);                    \
	    v_src1 = vis_faligndata(v_src10, v_src11);            \
	    v_src21 = vis_ld_d64_nf(dps2 + 1);                    \
	    vis_alignaddr((void *)src2, t);                       \
	    v_src2 = vis_faligndata(v_src20, v_src21);            \
	    MUL;                                                  \
	    ADD;                                                  \
	    vis_pst_16(v_dst, dpd, vis_edge16(dst, fdst));        \
	    dpd++;                                                \
	    FIX;                                                  \
	    times = ((mlib_d64 *)((mlib_addr)fdst & ~7) - dpd);   \
								  \
	    v_src20 = vis_ld_d64_nf(dps2 + 2);                    \
	    v_src2 = vis_faligndata(v_src21, v_src20);            \
	    v_src10 = vis_ld_d64_nf(dps1 + 2);                    \
	    vis_alignaddr((void *)src1, t);                       \
	    v_src1 = vis_faligndata(v_src11, v_src10);            \
	    MUL;                                                  \
								  \
	    v_src11 = vis_ld_d64_nf(dps1 + 3);                    \
	    v_src21 = vis_ld_d64_nf(dps2 + 3);                    \
	    dps1 += 4;                                            \
	    dps2 += 4;                                            \
	    v_src1 = vis_faligndata(v_src10, v_src11);            \
	    vis_alignaddr((void *)src2, t);                       \
	    v_src2 = vis_faligndata(v_src20, v_src21);            \
	    for (i = 2; i < times; i += 2) {                      \
		ADD;                                              \
		MUL;                                              \
		v_src20 = vis_ld_d64_nf(dps2);                    \
		v_src10 = vis_ld_d64_nf(dps1);                    \
		v_src2 = vis_faligndata(v_src21, v_src20);        \
		vis_alignaddr((void *)src1, t);                   \
		v_src1 = vis_faligndata(v_src11, v_src10);        \
		dpd[0] = v_dst;                                   \
		FIX;                                              \
		ADD;                                              \
		MUL;                                              \
		v_src11 = vis_ld_d64_nf(dps1 + 1);                \
		v_src21 = vis_ld_d64_nf(dps2 + 1);                \
		v_src1 = vis_faligndata(v_src10, v_src11);        \
		vis_alignaddr((void *)src2, t);                   \
		v_src2 = vis_faligndata(v_src20, v_src21);        \
		dpd[1] = v_dst;                                   \
		FIX;                                              \
		dpd += 2;                                         \
		dps1 += 2;                                        \
		dps2 += 2;                                        \
	    }                                                     \
	    if (i == times) {                                     \
		*dpd = v_dst;                                     \
		dpd++;                                            \
		ADD;                                              \
		MUL;                                              \
		FIX;                                              \
	    }                                                     \
	    *dpd = v_dst;                                         \
	    dpd++;                                                \
	    ADD;                                                  \
	    FIX;                                                  \
	    vis_pst_16(v_dst, dpd, vis_edge16(dpd, fdst));        \
	}

/* *********************************************************** */

mlib_status
__mlib_SignalMulShift_S16_S16_Sat(
    mlib_s16 *dst,
    const mlib_s16 *src1,
    const mlib_s16 *src2,
    mlib_s32 shift,
    mlib_s32 n)
{
	mlib_s32 i;

	if (shift > 31)
		shift = 31;
	else if (shift < -31)
		shift = -31;

	if (n < 16) {
		mlib_d64 d_val;

		if (shift >= 0)
			d_val = 2. * ((mlib_u32)1 << (shift));
		else
			d_val = 2. / ((mlib_u32)1 << (-shift));

		for (i = 0; i < n; i++)
			FLOAT2INT_CLAMP(dst[i], d_val * src1[i] * src2[i]);

		return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
	}
	{
		mlib_d64 *dpd, *dps1, *dps2;
		mlib_d64 dr0, dr1, dr2, dr3;
		mlib_d64 v_dst, v_dst0, v_dst1, v_src1, v_src2;
		mlib_d64 v_src10, v_src11, v_src20, v_src21;
		mlib_d64 v_one = vis_fone();
		mlib_f32 v_mul1, v_mul2, v_mul3;
		mlib_s32 times, nshift, t;
		mlib_s16 *fdst;

		fdst = dst + n - 1;

		if (shift >= 15) {
			if (shift >= 29)
				v_mul3 = vis_to_float(0x7fff7fff);
			else
				v_mul3 =
				    vis_to_float(0x40004000 >> (29 - shift));
			shift = 16;
		} else if (shift < -16)
			shift = -16;
		nshift = 1 + shift;

		if ((((mlib_addr)src1 ^ (mlib_addr)src2) & 7)) {

			if (nshift == 17) {
				MLIB_MULSHIFT_GE(15, MLIB_6ADD, MLIB_SCALMUL,
				    MLIB_FIX);
			} else {
				if (nshift >= 0) {
					MLIB_MULSHIFT_GE(nshift, MLIB_2ADD,
					    MLIB_4MUL, MLIB_FIX);
				} else {
					if (nshift >= -8) {
						v_mul1 =
						    vis_to_float(0x1010101 <<
						    (nshift + 8));
						MLIB_MULSHIFT_GE(0, MLIB_2ADD,
						    MLIB_5MUL, MLIB_FIX);
					} else {
						v_mul1 =
						    vis_to_float(0x1010101);
						v_mul2 =
						    vis_to_float(0x1010101 <<
						    (nshift + 16));
						MLIB_MULSHIFT_GE(0, MLIB_2ADD,
						    MLIB_6MUL, MLIB_FIX)
					}
				}
			}
			return (MLIB_SUCCESS);
		}

		if ((((mlib_addr)dst ^ (mlib_addr)src1) & 7)) {
			if (nshift == 17) {
				MLIB_MULSHIFT_GA(15, MLIB_6ADD, MLIB_SCALMUL,
				    MLIB_FIX);
			} else {
				if (nshift >= 0) {
					MLIB_MULSHIFT_GA(nshift, MLIB_2ADD,
					    MLIB_4MUL, MLIB_FIX);
				} else {
					if (nshift >= -8) {
						v_mul1 =
						    vis_to_float(0x1010101 <<
						    (nshift + 8));
						MLIB_MULSHIFT_GA(0, MLIB_2ADD,
						    MLIB_5MUL, MLIB_FIX);
					} else {
						v_mul1 =
						    vis_to_float(0x1010101);
						v_mul2 =
						    vis_to_float(0x1010101 <<
						    (nshift + 16));
						MLIB_MULSHIFT_GA(0, MLIB_2ADD,
						    MLIB_6MUL, MLIB_FIX);
					}
				}
			}
		} else {
			if (nshift == 17) {
				MLIB_MULSHIFT_AL(15, MLIB_6ADD, MLIB_SCALMUL,
				    MLIB_FIX);
			} else {
				if (nshift >= 0) {
					MLIB_MULSHIFT_AL(nshift, MLIB_2ADD,
					    MLIB_4MUL, MLIB_FIX);
				} else {
					if (nshift >= -8) {
						v_mul1 =
						    vis_to_float(0x1010101 <<
						    (nshift + 8));
						MLIB_MULSHIFT_AL(0, MLIB_2ADD,
						    MLIB_5MUL, MLIB_FIX);
					} else {
						v_mul1 =
						    vis_to_float(0x1010101);
						v_mul2 =
						    vis_to_float(0x1010101 <<
						    (nshift + 16));
						MLIB_MULSHIFT_AL(0, MLIB_2ADD,
						    MLIB_6MUL, MLIB_FIX);
					}
				}
			}
		}
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
