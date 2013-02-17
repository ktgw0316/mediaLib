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

#pragma ident	"@(#)mlib_v_SignalMulSShiftAdd.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_SignalMulSShiftAdd_[S16|S16S]_Sat,
 *      mlib_SignalMulSShiftAdd_[S16|S16S]_[S16|S16S]_Sat - multiplication of
 *                     signed 16-bit format signal array by a scalar
 *                     with shifting
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_SignalMulSShiftAdd_S16S_S16S_Sat(mlib_s16       *dst,
 *                                                        const mlib_s16 *src1,
 *                                                        const mlib_s16 *src2,
 *                                                        const mlib_s16 *c,
 *                                                        mlib_s32       shift,
 *                                                        mlib_s32       n);
 *      mlib_status mlib_SignalMulSShiftAdd_S16S_Sat(mlib_s16       *data,
 *                                                   const mlib_s16 *src2,
 *                                                   const mlib_s16 *c,
 *                                                   mlib_s32       shift,
 *                                                   mlib_s32       n);
 *      mlib_status mlib_SignalMulSShiftAdd_S16_S16_Sat(mlib_s16       *dst,
 *                                                      const mlib_s16 *src1,
 *                                                      const mlib_s16 *src2,
 *                                                      const mlib_s16 *c,
 *                                                      mlib_s32       shift,
 *                                                      mlib_s32       n);
 *      mlib_status mlib_SignalMulSShiftAdd_S16_Sat(mlib_s16       *data,
 *                                                  const mlib_s16 *src2,
 *                                                  const mlib_s16 *c,
 *                                                  mlib_s32       shift,
 *                                                  mlib_s32       n);
 * ARGUMENTS
 *      data  Input and output signal array
 *      dst   Output signal array
 *      src   Input signal array
 *      c     Scaling factor. The scaling factor is in Q15 format
 *      shift Left shifting factor, may be negative
 *      n     Number of samples in the input signal arrays.
 *
 * DESCRIPTION
 *      dst = src1 + src2 * c * (2 ** shift)
 */

#include <mlib_signal.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalMulSShiftAdd_S16S_S16S_Sat = \
	__mlib_SignalMulSShiftAdd_S16S_S16S_Sat
#pragma weak mlib_SignalMulSShiftAdd_S16_S16_Sat = \
	__mlib_SignalMulSShiftAdd_S16_S16_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalMulSShiftAdd_S16S_S16S_Sat)
    mlib_SignalMulSShiftAdd_S16S_S16S_Sat
    __attribute__((weak, alias("__mlib_SignalMulSShiftAdd_S16S_S16S_Sat")));
__typeof__(__mlib_SignalMulSShiftAdd_S16_S16_Sat)
    mlib_SignalMulSShiftAdd_S16_S16_Sat
    __attribute__((weak, alias("__mlib_SignalMulSShiftAdd_S16_S16_Sat")));

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

#define	MLIB_6MUL                                               \
	dr0 = vis_fmuld8sux16(vis_read_hi(v_src2), v_mul);      \
	dr1 = vis_fmuld8ulx16(vis_read_hi(v_src2), v_mul);      \
	dr2 = vis_fmuld8sux16(vis_read_lo(v_src2), v_mul);      \
	dr3 = vis_fmuld8ulx16(vis_read_lo(v_src2), v_mul);      \
	dr5 = vis_fmuld8sux16(v_shift, vis_read_hi(v_src1));    \
	dr6 = vis_fmuld8sux16(v_shift, vis_read_lo(v_src1));    \
	v_dst2 = vis_fpadd32(dr0, dr1)

/* *********************************************************** */

#define	MLIB_4ADD                                               \
	v_dst3 = vis_fpadd32(dr2, dr3);                         \
	v_dst0 = vis_fpsub32(v_dst2, dr5);                      \
	v_dst1 = vis_fpsub32(v_dst3, dr6)

/* *********************************************************** */

#define	MLIB_FIX	v_dst = vis_fpackfix_pair(v_dst0, v_dst1)

/* *********************************************************** */

#define	MLIB_6MUL2                                              \
	dr0 = vis_fmuld8sux16(vis_read_hi(v_src2), v_mul);      \
	dr1 = vis_fmuld8ulx16(vis_read_hi(v_src2), v_mul);      \
	dr2 = vis_fmuld8sux16(vis_read_lo(v_src2), v_mul);      \
	dr3 = vis_fmuld8ulx16(vis_read_lo(v_src2), v_mul);      \
	dr5 = vis_fmuld8ulx16(v_shift, vis_read_hi(v_src1));    \
	dr6 = vis_fmuld8ulx16(v_shift, vis_read_lo(v_src1));    \
	v_dst2 = vis_fpadd32(dr0, dr1)

/* *********************************************************** */

#define	MLIB_4ADD2                                              \
	v_dst3 = vis_fpadd32(dr2, dr3);                         \
	v_dst0 = vis_fpadd32(v_dst2, dr5);                      \
	v_dst1 = vis_fpadd32(v_dst3, dr6)

/* *********************************************************** */

#define	MLIB_4ADD2_2                                            \
	v_dst2 = vis_fpadd32(v_dst2, v_dst2);                   \
	v_dst3 = vis_fpadd32(dr2, dr3);                         \
	v_dst3 = vis_fpadd32(v_dst3, v_dst3);                   \
	v_dst0 = vis_fpadd32(v_dst2, dr5);                      \
	v_dst1 = vis_fpadd32(v_dst3, dr6)

/* *********************************************************** */

#define	MLIB_FIX2                                               \
	v_dst = vis_fpackfix_pair(v_dst0, v_dst1);              \
	v_dst0 = vis_fmuld8ulx16(v_four, vis_read_hi(v_dst));   \
	v_dst1 = vis_fmuld8ulx16(v_four, vis_read_lo(v_dst));   \
	v_dst = vis_fpackfix_pair(v_dst0, v_dst1)

/* *********************************************************** */

#define	MLIB_MULSSHIFTADD_AL(SHIFT, SIGN, ADD, MUL, FIX)          \
	{                                                         \
	    v_shift =                                             \
		vis_to_float((SIGN << (16 -                       \
		nshift) & 0xFFFF) | (SIGN << (32 - nshift)));     \
	    vis_write_gsr((SHIFT << 3));                          \
	    dps1 = (mlib_d64 *)((mlib_addr)src1 & ~7);            \
	    dps2 = (mlib_d64 *)((mlib_addr)src2 & ~7);            \
	    dpd = (mlib_d64 *)((mlib_addr)dst & ~7);              \
	    v_src1 = vis_ld_d64_nf(dps1);                         \
	    v_src2 = vis_ld_d64_nf(dps2);                         \
	    dps1++;                                               \
	    dps2++;                                               \
	    MUL;                                                  \
	    ADD;                                                  \
	    FIX;                                                  \
	                                                          \
	    v_src1 = vis_ld_d64_nf(dps1);                         \
	    v_src2 = vis_ld_d64_nf(dps2);                         \
	    dps1++;                                               \
	    dps2++;                                               \
	    MUL;                                                  \
	    ADD;                                                  \
	    vis_pst_16(v_dst, dpd, vis_edge16(dst, fdst));        \
	    dpd++;                                                \
	    FIX;                                                  \
	    times = ((mlib_d64 *)((mlib_addr)fdst & ~7) - dpd);   \
	                                                          \
	    v_src1 = vis_ld_d64_nf(dps1);                         \
	    v_src2 = vis_ld_d64_nf(dps2);                         \
	    dps1++;                                               \
	    dps2++;                                               \
	    MUL;                                                  \
	                                                          \
	    v_src1 = vis_ld_d64_nf(dps1);                         \
	    v_src2 = vis_ld_d64_nf(dps2);                         \
	    dps1++;                                               \
	    dps2++;                                               \
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

#define	MLIB_MULSSHIFTADD_GA(SHIFT, SIGN, ADD, MUL, FIX)          \
	{                                                         \
	    v_shift =                                             \
		vis_to_float((SIGN << (16 -                       \
		nshift) & 0xFFFF) | (SIGN << (32 - nshift)));     \
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

#define	MLIB_MULSSHIFTADD_GE(SHIFT, SIGN, ADD, MUL, FIX)          \
	{                                                         \
	    v_shift =                                             \
		vis_to_float((SIGN << (16 -                       \
		nshift) & 0xFFFF) | (SIGN << (32 - nshift)));     \
	    vis_write_gsr((SHIFT << 3));                          \
	    dpd = vis_alignaddr(dst, 0);                          \
	    t = -((mlib_addr)dst & 7);                            \
	    dps2 = vis_alignaddr((void *)src2, t);                \
	    dps1 = vis_alignaddr((void *)src1, t);                \
	    v_src1 = vis_ld_d64_nf(dps1);                         \
	    v_src10 = vis_ld_d64_nf(dps1 + 1);                    \
	    v_src2 = vis_ld_d64_nf(dps2);                         \
	    v_src20 = vis_ld_d64_nf(dps2 + 1);                    \
	    v_src1 = vis_faligndata(v_src1, v_src10);             \
	    vis_alignaddr((void *)src2, t);                       \
	    v_src2 = vis_faligndata(v_src2, v_src20);             \
	    MUL;                                                  \
	    ADD;                                                  \
	    FIX;                                                  \
	    dps1++;                                               \
	    dps2++;                                               \
								  \
	    v_src11 = vis_ld_d64_nf(dps1 + 1);                    \
	    v_src21 = vis_ld_d64_nf(dps2 + 1);                    \
	    v_src2 = vis_faligndata(v_src20, v_src21);            \
	    vis_alignaddr((void *)src1, t);                       \
	    v_src1 = vis_faligndata(v_src10, v_src11);            \
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
	    vis_alignaddr((void *)src2, t);                       \
	    v_src2 = vis_faligndata(v_src21, v_src20);            \
	    MUL;                                                  \
								  \
	    v_src11 = vis_ld_d64_nf(dps1 + 3);                    \
	    v_src21 = vis_ld_d64_nf(dps2 + 3);                    \
	    v_src2 = vis_faligndata(v_src20, v_src21);            \
	    vis_alignaddr((void *)src1, t);                       \
	    v_src1 = vis_faligndata(v_src10, v_src11);            \
	    dps1 += 4;                                            \
	    dps2 += 4;                                            \
	    for (i = 2; i < times; i += 2) {                      \
		v_src1 = vis_faligndata(v_src10, v_src11);        \
		ADD;                                              \
		MUL;                                              \
		v_src10 = vis_ld_d64_nf(dps1);                    \
		v_src20 = vis_ld_d64_nf(dps2);                    \
		v_src1 = vis_faligndata(v_src11, v_src10);        \
		v_src11 = vis_ld_d64_nf(dps1 + 1);                \
		vis_alignaddr((void *)src2, t);                   \
		v_src2 = vis_faligndata(v_src21, v_src20);        \
		v_src21 = vis_ld_d64_nf(dps2 + 1);                \
		dpd[0] = v_dst;                                   \
		FIX;                                              \
		ADD;                                              \
		MUL;                                              \
		dpd[1] = v_dst;                                   \
		v_src2 = vis_faligndata(v_src20, v_src21);        \
		vis_alignaddr((void *)src1, t);                   \
		FIX;                                              \
		dpd += 2;                                         \
		dps1 += 2;                                        \
		dps2 += 2;                                        \
	    }                                                     \
	    if (i == times) {                                     \
		v_src1 = vis_faligndata(v_src10, v_src11);        \
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
__mlib_SignalMulSShiftAdd_S16_S16_Sat(
    mlib_s16 *dst,
    const mlib_s16 *src1,
    const mlib_s16 *src2,
    const mlib_s16 *c,
    mlib_s32 shift,
    mlib_s32 n)
{
	mlib_s32 i;

	if (shift > 31)
		shift = 31;
	else if (shift < -31)
		shift = -31;

	if ((n < 16) /* || shift == 15 || (*c == -32768) */) {
		mlib_d64 d_val;
		mlib_d64 val_c = 65536.;

		if (shift >= 0)
			d_val = 2. * ((mlib_u32)1 << (shift));
		else
			d_val = 2. / ((mlib_u32)1 << (-shift));
		d_val *= *c;

		for (i = 0; i < n; i++)
			FLOAT2INT_CLAMP(dst[i],
			    src1[i] * val_c + d_val * src2[i]);

		return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
	}
	{
		mlib_d64 *dpd, *dps1, *dps2;
		mlib_d64 dr0, dr1, dr2, dr3, dr5, dr6;
		mlib_d64 v_dst, v_dst0, v_dst1, v_dst2, v_dst3, v_src1, v_src2;
		mlib_d64 v_src10, v_src11, v_src20, v_src21;
		mlib_f32 v_mul, v_shift, v_four = vis_to_float(0x40004);
		mlib_s32 times, nshift, t;
		mlib_s16 *fdst;
		mlib_s32 c0 = *c;

		fdst = dst + n - 1;

		nshift = 1 + shift;

		if (shift < 0) {
			c0 >>= -shift;
			nshift = 1;
		} else if (shift > 15) {
			c0 <<= shift - 16;
			nshift = 16;

			if (c0 > 32767)
				c0 = 32767;
			else if (c0 < -32767)
				c0 = -32767;
		}

		v_mul = vis_to_float((c0 & 0xFFFF) | (c0 << 16));

		if ((((mlib_addr)src1 ^ (mlib_addr)src2) & 7)) {
			if (nshift < 9) {
				MLIB_MULSSHIFTADD_GE(nshift, -1, MLIB_4ADD,
				    MLIB_6MUL, MLIB_FIX);
			} else {
				if (nshift < 16) {
					MLIB_MULSSHIFTADD_GE(nshift, 1,
					    MLIB_4ADD2, MLIB_6MUL2, MLIB_FIX);
				} else {
					if (shift == 15) {
						MLIB_MULSSHIFTADD_GE(15, 1,
						    MLIB_4ADD2, MLIB_6MUL2,
						    MLIB_FIX2);
					} else {
						MLIB_MULSSHIFTADD_GE(15, 1,
						    MLIB_4ADD2_2, MLIB_6MUL2,
						    MLIB_FIX2);
					}
				}
			}
			return (MLIB_SUCCESS);
		}

		if ((((mlib_addr) dst ^ (mlib_addr) src1) & 7)) {

			if (nshift < 9) {
				MLIB_MULSSHIFTADD_GA(nshift, -1, MLIB_4ADD,
				    MLIB_6MUL, MLIB_FIX);
			} else {
				if (nshift < 16) {
					MLIB_MULSSHIFTADD_GA(nshift, 1,
					    MLIB_4ADD2, MLIB_6MUL2, MLIB_FIX);
				} else {
					if (shift == 15) {
						MLIB_MULSSHIFTADD_GA(15, 1,
						    MLIB_4ADD2, MLIB_6MUL2,
						    MLIB_FIX2);
					} else {
						MLIB_MULSSHIFTADD_GA(15, 1,
						    MLIB_4ADD2_2, MLIB_6MUL2,
						    MLIB_FIX2);
					}
				}
			}
			return (MLIB_SUCCESS);
		}

		if (nshift < 9) {
			MLIB_MULSSHIFTADD_AL(nshift, -1, MLIB_4ADD, MLIB_6MUL,
			    MLIB_FIX);
		} else {
			if (nshift < 16) {
				MLIB_MULSSHIFTADD_AL(nshift, 1, MLIB_4ADD2,
				    MLIB_6MUL2, MLIB_FIX);
			} else {
				if (shift == 15) {
					MLIB_MULSSHIFTADD_AL(15, 1, MLIB_4ADD2,
					    MLIB_6MUL2, MLIB_FIX2);
				} else {
					MLIB_MULSSHIFTADD_AL(15, 1,
					    MLIB_4ADD2_2, MLIB_6MUL2,
					    MLIB_FIX2);
				}
			}
		}
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_SignalMulSShiftAdd_S16S_S16S_Sat(
    mlib_s16 *dst,
    const mlib_s16 *src1,
    const mlib_s16 *src2,
    const mlib_s16 *c,
    mlib_s32 shift,
    mlib_s32 n)
{
	mlib_s32 i;

	if (shift > 31)
		shift = 31;
	else if (shift < -31)
		shift = -31;

	if ((n < 8) /* || shift == 15 || (*c == -32768) */) {
		mlib_d64 d_val, d_val2;
		mlib_d64 val_c = 65536.;

		if (shift >= 0)
			d_val = 2. * ((mlib_u32)1 << (shift));
		else
			d_val = 2. / ((mlib_u32)1 << (-shift));
		d_val2 = d_val * c[1];
		d_val *= c[0];

		for (i = 0; i < n; i++) {
			FLOAT2INT_CLAMP(dst[2 * i],
			    src1[2 * i] * val_c + d_val * src2[2 * i]);
			FLOAT2INT_CLAMP(dst[2 * i + 1],
			    src1[2 * i + 1] * val_c + d_val2 * src2[2 * i + 1]);
		}

		return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
	}
	{
		mlib_d64 *dpd, *dps1, *dps2;
		mlib_d64 dr0, dr1, dr2, dr3, dr5, dr6;
		mlib_d64 v_dst, v_dst0, v_dst1, v_dst2, v_dst3, v_src1, v_src2;
		mlib_d64 v_src10, v_src11, v_src20, v_src21;
		mlib_f32 v_mul, v_shift, v_four = vis_to_float(0x40004);
		mlib_s32 times, nshift, t;
		mlib_s16 *fdst;
		mlib_s32 c0 = c[0], c1 = c[1];

		fdst = dst + n + n - 1;

		nshift = 1 + shift;

		if (shift < 0) {
			c0 >>= -shift;
			c1 >>= -shift;
			nshift = 1;
		} else if (shift > 15) {
			c0 <<= shift - 16;
			c1 <<= shift - 16;
			nshift = 16;

			if (c0 > 32767)
				c0 = 32767;
			else if (c0 < -32767)
				c0 = -32767;

			if (c1 > 32767)
				c1 = 32767;
			else if (c1 < -32767)
				c1 = -32767;
		}

		if ((mlib_addr)dst & 2)
			v_mul = vis_to_float((c0 & 0xFFFF) | (c1 << 16));
		else
			v_mul = vis_to_float((c1 & 0xFFFF) | (c0 << 16));

		if ((((mlib_addr)src1 ^ (mlib_addr)src2) & 7)) {
			if (nshift < 9) {
				MLIB_MULSSHIFTADD_GE(nshift, -1, MLIB_4ADD,
				    MLIB_6MUL, MLIB_FIX);
			} else {
				if (nshift < 16) {
					MLIB_MULSSHIFTADD_GE(nshift, 1,
					    MLIB_4ADD2, MLIB_6MUL2, MLIB_FIX);
				} else {
					if (shift == 15) {
						MLIB_MULSSHIFTADD_GE(15, 1,
						    MLIB_4ADD2, MLIB_6MUL2,
						    MLIB_FIX2);
					} else {
						MLIB_MULSSHIFTADD_GE(15, 1,
						    MLIB_4ADD2_2, MLIB_6MUL2,
						    MLIB_FIX2);
					}
				}
			}
			return (MLIB_SUCCESS);
		}

		if ((((mlib_addr) dst ^ (mlib_addr) src1) & 7)) {

			if (nshift < 9) {
				MLIB_MULSSHIFTADD_GA(nshift, -1, MLIB_4ADD,
				    MLIB_6MUL, MLIB_FIX);
			} else {
				if (nshift < 16) {
					MLIB_MULSSHIFTADD_GA(nshift, 1,
					    MLIB_4ADD2, MLIB_6MUL2, MLIB_FIX);
				} else {
					if (shift == 15) {
						MLIB_MULSSHIFTADD_GA(15, 1,
						    MLIB_4ADD2, MLIB_6MUL2,
						    MLIB_FIX2);
					} else {
						MLIB_MULSSHIFTADD_GA(15, 1,
						    MLIB_4ADD2_2, MLIB_6MUL2,
						    MLIB_FIX2);
					}
				}
			}
			return (MLIB_SUCCESS);
		}

		if (nshift < 9) {
			MLIB_MULSSHIFTADD_AL(nshift, -1, MLIB_4ADD, MLIB_6MUL,
			    MLIB_FIX);
		} else {
			if (nshift < 16) {
				MLIB_MULSSHIFTADD_AL(nshift, 1, MLIB_4ADD2,
				    MLIB_6MUL2, MLIB_FIX);
			} else {
				if (shift == 15) {
					MLIB_MULSSHIFTADD_AL(15, 1, MLIB_4ADD2,
					    MLIB_6MUL2, MLIB_FIX2);
				} else {
					MLIB_MULSSHIFTADD_AL(15, 1,
					    MLIB_4ADD2_2, MLIB_6MUL2,
					    MLIB_FIX2);
				}
			}
		}
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */
