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
 * Copyright 2005 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_v_ImageScale_Fp.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageScale_Fp - linear scaling
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageScale_Fp(mlib_image       *dst,
 *                                     const mlib_image *src,
 *                                     const mlib_d64   *alpha,
 *                                     const mlib_d64   *beta);
 *
 * ARGUMENT
 *      dst     pointer to the output image
 *      src     pointer to the input image
 *      alpha   array of scaling factors to be multiplied to each channel
 *              of the pixels
 *      beta    array of offsets to be added to each channel of the pixels
 *
 * RESTRICTION
 *      The images must have the same size, and the same number
 *      of channels.
 *      The images can have 1, 2, 3, or 4 channels.
 *      The input image can be in MLIB_FLOAT or MLIB_DOUBLE data type.
 *      The output image can be in MLIB_BYTE, MLIB_SHORT, MLIB_USHORT, MLIB_INT,
 *      MLIB_FLOAT or MLIB_DOUBLE data type.
 *
 * DESCRIPTION
 *      dst[x][y][c] = src[x][y][c] * alpha[c] + beta[c]
 *
 *      If the result of the operation underflows/overflows the
 *      minimum/maximum value supported by the destination image, then it
 *      will be clamped to the minimum/maximum value respectively.
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImageCheck.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageScale_Fp = __mlib_ImageScale_Fp

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageScale_Fp) mlib_ImageScale_Fp
    __attribute__((weak, alias("__mlib_ImageScale_Fp")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static void mlib_v_ImageScale_Fp_F32_S16(
    mlib_u8 *dst,
    const mlib_u8 *src,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 nchan,
    mlib_s32 slb,
    mlib_s32 dlb,
    const mlib_d64 *alpha,
    const mlib_d64 *beta);

static void mlib_v_ImageScale_Fp_F32_U16(
    mlib_u8 *dst,
    const mlib_u8 *src,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 nchan,
    mlib_s32 slb,
    mlib_s32 dlb,
    const mlib_d64 *alpha,
    const mlib_d64 *beta);

static void mlib_v_ImageScale_Fp_F32_U8(
    mlib_u8 *dst,
    const mlib_u8 *src,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 nchan,
    mlib_s32 slb,
    mlib_s32 dlb,
    const mlib_d64 *alpha,
    const mlib_d64 *beta);

static void mlib_v_ImageScale_Fp_D64_S16(
    mlib_u8 *dst,
    const mlib_u8 *src,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 nchan,
    mlib_s32 slb,
    mlib_s32 dlb,
    const mlib_d64 *alpha,
    const mlib_d64 *beta);

static void mlib_v_ImageScale_Fp_D64_U16(
    mlib_u8 *dst,
    const mlib_u8 *src,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 nchan,
    mlib_s32 slb,
    mlib_s32 dlb,
    const mlib_d64 *alpha,
    const mlib_d64 *beta);

static void mlib_v_ImageScale_Fp_D64_U8(
    mlib_u8 *dst,
    const mlib_u8 *src,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 nchan,
    mlib_s32 slb,
    mlib_s32 dlb,
    const mlib_d64 *alpha,
    const mlib_d64 *beta);

static void mlib_v_ImageScale_Fp_F32_F32(
    mlib_u8 *dst,
    const mlib_u8 *src,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 nchan,
    mlib_s32 slb,
    mlib_s32 dlb,
    const mlib_d64 *alpha,
    const mlib_d64 *beta);

static void mlib_v_ImageScale_Fp_F32_S32(
    mlib_u8 *dst,
    const mlib_u8 *src,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 nchan,
    mlib_s32 slb,
    mlib_s32 dlb,
    const mlib_d64 *alpha,
    const mlib_d64 *beta);

static void mlib_v_ImageScale_Fp_F32_D64(
    mlib_u8 *dst,
    const mlib_u8 *src,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 nchan,
    mlib_s32 slb,
    mlib_s32 dlb,
    const mlib_d64 *alpha,
    const mlib_d64 *beta);

static void mlib_v_ImageScale_Fp_D64_S32(
    mlib_u8 *dst,
    const mlib_u8 *src,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 nchan,
    mlib_s32 slb,
    mlib_s32 dlb,
    const mlib_d64 *alpha,
    const mlib_d64 *beta);

static void mlib_v_ImageScale_Fp_D64_F32(
    mlib_u8 *dst,
    const mlib_u8 *src,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 nchan,
    mlib_s32 slb,
    mlib_s32 dlb,
    const mlib_d64 *alpha,
    const mlib_d64 *beta);

static void mlib_v_ImageScale_Fp_D64_D64(
    mlib_u8 *dst,
    const mlib_u8 *src,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 nchan,
    mlib_s32 slb,
    mlib_s32 dlb,
    const mlib_d64 *alpha,
    const mlib_d64 *beta);

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

#define	FLOAT2INT_CLAMP(DST, SRC)	DST = ((mlib_s32)(SRC))

#else /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#define	FLOAT2INT_CLAMP(DST, SRC)                               \
	{                                                       \
	    mlib_d64 dsrc = (mlib_d64)(SRC);                    \
	                                                        \
	    if (dsrc > (mlib_d64)MLIB_S32_MAX)                  \
		dsrc = (mlib_d64)MLIB_S32_MAX;                  \
	    if (dsrc < (mlib_d64)MLIB_S32_MIN)                  \
		dsrc = (mlib_d64)MLIB_S32_MIN;                  \
	    DST = (mlib_s32)dsrc;                               \
	}

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#define	ARITH_FUN(x, a, b)	((x)*(a) + (b))

/* *********************************************************** */

#define	ARITH_FUN_U8(dst, x, a, b)                              \
	{                                                       \
	    d64_2x32 sat_u8_tmp;                                \
	                                                        \
	    sat_u8_tmp.i32s.f1 = (mlib_s32)((x) * (a) + (b));   \
	    sat_u8_tmp.d64 =                                    \
		vis_fpack32(sat_u8_tmp.d64, sat_u8_tmp.d64);    \
	    dst = sat_u8_tmp.i32s.f1;                           \
	}

/* *********************************************************** */

#define	ARITH_FUN_16(dst, x, a, b)                                 \
	{                                                          \
	    d64_2x32 sat_s16_tmp;                                  \
	                                                           \
	    sat_s16_tmp.i32s.f1 = (mlib_s32)((x) * (a) + (b));     \
	    sat_s16_tmp.f32s.f1 = vis_fpackfix(sat_s16_tmp.d64);   \
	    dst = sat_s16_tmp.i32s.f1;                             \
	}

/* *********************************************************** */

#define	ARITH_FUN_U16(dst, x, a, b)                                \
	{                                                          \
	    d64_2x32 sat_s16_tmp;                                  \
	                                                           \
	    sat_s16_tmp.i32s.f1 = (mlib_s32)((x) * (a) + (b));     \
	    sat_s16_tmp.f32s.f1 = vis_fpackfix(sat_s16_tmp.d64);   \
	    sat_s16_tmp.f32s.f1 =                                  \
		vis_fxors(sat_s16_tmp.f32s.f1, mask_8000);         \
	    dst = sat_s16_tmp.i32s.f1;                             \
	}

#define	ARITH_FUN_INT(x, a, b)	(mlib_s32)((x)*(a) + (b))

/* *********************************************************** */

#define	BLOCK_SIZE	256
#define	BLOCK_SIZE_3	255
#define	SHIFT_16	8

/* *********************************************************** */

typedef union
{
	mlib_d64 d64;
	struct
	{
		mlib_f32 f0, f1;
	} f32s;
	struct
	{
		mlib_s32 f0, f1;
	} i32s;
} d64_2x32;

/* *********************************************************** */

#define	MLIB_PACK_8x32_8(dd, d01, d23, d45, d67)                \
	dd = vis_fpack32(dd, d01);                              \
	dd = vis_fpack32(dd, d23);                              \
	dd = vis_fpack32(dd, d45);                              \
	dd = vis_fpack32(dd, d67);                              \
	dd = vis_fpmerge(vis_read_hi(dd), vis_read_lo(dd))

/* *********************************************************** */

#define	MLIB_PACK_4x32_8(df, dd, d01, d23)                      \
	dd = vis_fpack32(dd, d01);                              \
	dd = vis_fpack32(dd, d23);                              \
	dd = vis_fpmerge(vis_read_hi(dd), vis_read_lo(dd));     \
	df = vis_read_lo(dd)

/* *********************************************************** */

#define	MLIB_PACK_2x32_8(pd, dd, d01)                           \
	dd = vis_fpack32(dd, d01);                              \
	dd = vis_fpmerge(vis_read_hi(dd), vis_read_lo(dd));     \
	vis_st_u16(dd, pd)

/* *********************************************************** */

mlib_status
__mlib_ImageScale_Fp(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *alpha,
    const mlib_d64 *beta)
{
	mlib_type stype = mlib_ImageGetType(src),
	    dtype = mlib_ImageGetType(dst);
	mlib_s32 slb, dlb, xsize, ysize, nchan;
	mlib_u8 *psrc, *pdst;

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_SIZE_EQUAL(dst, src);
	MLIB_IMAGE_CHAN_EQUAL(dst, src);

	if (alpha == NULL)
		return (MLIB_FAILURE);
	if (beta == NULL)
		return (MLIB_FAILURE);

	MLIB_IMAGE_GET_ALL_PARAMS(dst, dtype, nchan, xsize, ysize, dlb, pdst);
	stype = mlib_ImageGetType(src);
	slb = mlib_ImageGetStride(src);
	psrc = mlib_ImageGetData(src);

	if (stype == MLIB_FLOAT) {

		if (dtype == MLIB_BYTE) {

			mlib_v_ImageScale_Fp_F32_U8(pdst, psrc,
			    xsize, ysize, nchan, slb, dlb, alpha, beta);
			return (MLIB_SUCCESS);

		} else if (dtype == MLIB_SHORT) {

			mlib_v_ImageScale_Fp_F32_S16(pdst, psrc,
			    xsize, ysize, nchan, slb, dlb, alpha, beta);
			return (MLIB_SUCCESS);

		} else if (dtype == MLIB_USHORT) {

			mlib_v_ImageScale_Fp_F32_U16(pdst, psrc,
			    xsize, ysize, nchan, slb, dlb, alpha, beta);
			return (MLIB_SUCCESS);

		} else if (dtype == MLIB_INT) {

			mlib_v_ImageScale_Fp_F32_S32(pdst, psrc,
			    xsize, ysize, nchan, slb, dlb, alpha, beta);

			return (MLIB_SUCCESS);

		} else if (dtype == MLIB_FLOAT) {

			mlib_v_ImageScale_Fp_F32_F32(pdst, psrc,
			    xsize, ysize, nchan, slb, dlb, alpha, beta);
			return (MLIB_SUCCESS);

		} else if (dtype == MLIB_DOUBLE) {

			mlib_v_ImageScale_Fp_F32_D64(pdst, psrc,
			    xsize, ysize, nchan, slb, dlb, alpha, beta);

			return (MLIB_SUCCESS);
		}

	} else if (stype == MLIB_DOUBLE) {

		if (dtype == MLIB_BYTE) {

			mlib_v_ImageScale_Fp_D64_U8(pdst, psrc,
			    xsize, ysize, nchan, slb, dlb, alpha, beta);
			return (MLIB_SUCCESS);

		} else if (dtype == MLIB_SHORT) {

			mlib_v_ImageScale_Fp_D64_S16(pdst, psrc,
			    xsize, ysize, nchan, slb, dlb, alpha, beta);
			return (MLIB_SUCCESS);

		} else if (dtype == MLIB_USHORT) {

			mlib_v_ImageScale_Fp_D64_U16(pdst, psrc,
			    xsize, ysize, nchan, slb, dlb, alpha, beta);
			return (MLIB_SUCCESS);

		} else if (dtype == MLIB_INT) {

			mlib_v_ImageScale_Fp_D64_S32(pdst, psrc,
			    xsize, ysize, nchan, slb, dlb, alpha, beta);

			return (MLIB_SUCCESS);

		} else if (dtype == MLIB_FLOAT) {

			mlib_v_ImageScale_Fp_D64_F32(pdst, psrc,
			    xsize, ysize, nchan, slb, dlb, alpha, beta);

			return (MLIB_SUCCESS);

		} else if (dtype == MLIB_DOUBLE) {

			mlib_v_ImageScale_Fp_D64_D64(pdst, psrc,
			    xsize, ysize, nchan, slb, dlb, alpha, beta);

			return (MLIB_SUCCESS);
		}
	}

	return (MLIB_FAILURE);
}

/* *********************************************************** */

void
mlib_v_ImageScale_Fp_F32_S16(
    mlib_u8 *dst,
    const mlib_u8 *src,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 nchan,
    mlib_s32 slb,
    mlib_s32 dlb,
    const mlib_d64 *alpha,
    const mlib_d64 *beta)
{
	mlib_f32 *ps;
	mlib_s16 *pd, *pend;
	mlib_f32 alpha2[9], beta2[9];
	mlib_d64 arr[BLOCK_SIZE];
	mlib_s32 i, j, k, n, b, off;

	vis_write_gsr(15 << 3);

/* prepare the alpha and beta values */
	for (k = 0; k < 9; k++) {
		alpha2[k] = alpha[k % nchan] * 2.0;
		beta2[k] = beta[k % nchan] * 2.0;
	}

	for (j = 0; j < ysize; j++) {
		ps = (mlib_f32 *)src;
		pd = (mlib_s16 *)dst;
		pend = pd + xsize * nchan;

		off = ((mlib_addr)pd & 7) >> 1;

		if (off) {
			off = 4 - off;
			for (k = 0; (k < off) && (pd < pend); k++) {
				ARITH_FUN_16(*pd, *ps, alpha2[k], beta2[k])
				    pd++;
				ps++;
			}
		}

		if (nchan == 3) {
			{
/* nchan = 3, any aligning for src */
				mlib_f32 a0, a1, a2, a3, a4, a5, b0, b1, b2, b3,
				    b4, b5;
				d64_2x32 d0;

				a0 = alpha2[off];
				b0 = beta2[off];
				a1 = alpha2[off + 1];
				b1 = beta2[off + 1];
				a2 = alpha2[off + 2];
				b2 = beta2[off + 2];
				a3 = alpha2[off + 3];
				b3 = beta2[off + 3];
				a4 = alpha2[off + 4];
				b4 = beta2[off + 4];
				a5 = alpha2[off + 5];
				b5 = beta2[off + 5];

				n = (pend - pd) / (BLOCK_SIZE_3 * 2);
				for (i = 0; i < n; i++) {
#pragma pipeloop(0)
					for (b = 0; b < BLOCK_SIZE_3;
					    b += 3, ps += 6) {
						d0.i32s.f0 =
						    ARITH_FUN_INT(ps[0], a0,
						    b0);
						d0.i32s.f1 =
						    ARITH_FUN_INT(ps[1], a1,
						    b1);
						arr[b] = d0.d64;

						d0.i32s.f0 =
						    ARITH_FUN_INT(ps[2], a2,
						    b2);
						d0.i32s.f1 =
						    ARITH_FUN_INT(ps[3], a3,
						    b3);
						arr[b + 1] = d0.d64;

						d0.i32s.f0 =
						    ARITH_FUN_INT(ps[4], a4,
						    b4);
						d0.i32s.f1 =
						    ARITH_FUN_INT(ps[5], a5,
						    b5);
						arr[b + 2] = d0.d64;
					}

#pragma pipeloop(0)
					for (b = 0; b < BLOCK_SIZE_3;
					    b++, pd += 2) {
						*(mlib_f32 *)pd =
						    vis_fpackfix(arr[b]);
					}
				}

				n = (pend - pd) >> 1;
#pragma pipeloop(0)
				for (b = 0; b <= (n - 3); b += 3, ps += 6) {
					d0.i32s.f0 =
					    ARITH_FUN_INT(ps[0], a0, b0);
					d0.i32s.f1 =
					    ARITH_FUN_INT(ps[1], a1, b1);
					arr[b] = d0.d64;

					d0.i32s.f0 =
					    ARITH_FUN_INT(ps[2], a2, b2);
					d0.i32s.f1 =
					    ARITH_FUN_INT(ps[3], a3, b3);
					arr[b + 1] = d0.d64;

					d0.i32s.f0 =
					    ARITH_FUN_INT(ps[4], a4, b4);
					d0.i32s.f1 =
					    ARITH_FUN_INT(ps[5], a5, b5);
					arr[b + 2] = d0.d64;
				}

#pragma pipeloop(0)
				for (b = 0; b <= (n - 3); b += 3, pd += 6) {
					*(mlib_f32 *)pd = vis_fpackfix(arr[b]);
					*(mlib_f32 *)(pd + 2) =
					    vis_fpackfix(arr[b + 1]);
					*(mlib_f32 *)(pd + 4) =
					    vis_fpackfix(arr[b + 2]);
				}

				if (pd < pend) {
					ARITH_FUN_16(*pd, *ps, a0, b0)
					    pd++;
					ps++;

					if (pd < pend) {
						ARITH_FUN_16(*pd, *ps, a1, b1)
						    pd++;
						ps++;

						if (pd < pend) {
							ARITH_FUN_16(*pd, *ps,
							    a2, b2)
							    pd++;
							ps++;

							if (pd < pend) {
								ARITH_FUN_16
								    (*pd, *ps,
								    a3, b3)
								    pd++;
								ps++;

								if (pd < pend) {
								    ARITH_FUN_16
									(*pd,
									*ps,
									a4,
									b4)
								    pd++;
								    ps++;
								}
							}
						}
					}
				}
			}

		} else {

/* nchan != 3, all aligned */
			if (((mlib_addr)ps & 7) == 0) {

				mlib_f32 a0, a1, a2, a3, b0, b1, b2, b3;
				d64_2x32 d0, d1;

				a0 = alpha2[off];
				b0 = beta2[off];
				a1 = alpha2[off + 1];
				b1 = beta2[off + 1];
				a2 = alpha2[off + 2];
				b2 = beta2[off + 2];
				a3 = alpha2[off + 3];
				b3 = beta2[off + 3];

				n = (pend - pd) >> (SHIFT_16 + 1);
				for (i = 0; i < n; i++) {
#pragma pipeloop(0)
					for (b = 0; b < BLOCK_SIZE;
					    b += 2, ps += 4) {
						d0.d64 = *(mlib_d64 *)ps;
						d0.i32s.f0 =
						    ARITH_FUN_INT(d0.f32s.f0,
						    a0, b0);
						d0.i32s.f1 =
						    ARITH_FUN_INT(d0.f32s.f1,
						    a1, b1);
						arr[b] = d0.d64;

						d1.d64 = *(mlib_d64 *)(ps + 2);
						d1.i32s.f0 =
						    ARITH_FUN_INT(d1.f32s.f0,
						    a2, b2);
						d1.i32s.f1 =
						    ARITH_FUN_INT(d1.f32s.f1,
						    a3, b3);
						arr[b + 1] = d1.d64;
					}

#pragma pipeloop(0)
					for (b = 0; b < BLOCK_SIZE;
					    b += 2, pd += 4) {
						*(mlib_d64 *)pd =
						    vis_fpackfix_pair(arr[b],
						    arr[b + 1]);
					}
				}

				n = (pend - pd) >> 1;
#pragma pipeloop(0)
				for (b = 0; b <= (n - 2); b += 2, ps += 4) {
					d0.d64 = *(mlib_d64 *)ps;
					d0.i32s.f0 =
					    ARITH_FUN_INT(d0.f32s.f0, a0, b0);
					d0.i32s.f1 =
					    ARITH_FUN_INT(d0.f32s.f1, a1, b1);
					arr[b] = d0.d64;

					d1.d64 = *(mlib_d64 *)(ps + 2);
					d1.i32s.f0 =
					    ARITH_FUN_INT(d1.f32s.f0, a2, b2);
					d1.i32s.f1 =
					    ARITH_FUN_INT(d1.f32s.f1, a3, b3);
					arr[b + 1] = d1.d64;
				}

#pragma pipeloop(0)
				for (b = 0; b <= (n - 2); b += 2, pd += 4) {
					*(mlib_d64 *)pd =
					    vis_fpackfix_pair(arr[b],
					    arr[b + 1]);
				}

				if (pd < pend) {
					ARITH_FUN_16(*pd, *ps, a0, b0)
					    pd++;
					ps++;

					if (pd < pend) {
						ARITH_FUN_16(*pd, *ps, a1, b1)
						    pd++;
						ps++;

						if (pd < pend) {
							ARITH_FUN_16(*pd, *ps,
							    a2, b2)
							    pd++;
							ps++;
						}
					}
				}

			} else {
/* nchan != 3, src unaligned */

				mlib_f32 a0, a1, a2, a3, b0, b1, b2, b3;
				d64_2x32 d0, d1;

				a0 = alpha2[off];
				b0 = beta2[off];
				a1 = alpha2[off + 1];
				b1 = beta2[off + 1];
				a2 = alpha2[off + 2];
				b2 = beta2[off + 2];
				a3 = alpha2[off + 3];
				b3 = beta2[off + 3];

				n = (pend - pd) >> (SHIFT_16 + 1);
				for (i = 0; i < n; i++) {
#pragma pipeloop(0)
					for (b = 0; b < BLOCK_SIZE;
					    b += 2, ps += 4) {
						d0.i32s.f0 =
						    ARITH_FUN_INT(ps[0], a0,
						    b0);
						d0.i32s.f1 =
						    ARITH_FUN_INT(ps[1], a1,
						    b1);
						arr[b] = d0.d64;

						d1.i32s.f0 =
						    ARITH_FUN_INT(ps[2], a2,
						    b2);
						d1.i32s.f1 =
						    ARITH_FUN_INT(ps[3], a3,
						    b3);
						arr[b + 1] = d1.d64;
					}

#pragma pipeloop(0)
					for (b = 0; b < BLOCK_SIZE;
					    b += 2, pd += 4) {
						*(mlib_d64 *)pd =
						    vis_fpackfix_pair(arr[b],
						    arr[b + 1]);
					}
				}

				n = (pend - pd) >> 1;
#pragma pipeloop(0)
				for (b = 0; b <= (n - 2); b += 2, ps += 4) {
					d0.i32s.f0 =
					    ARITH_FUN_INT(ps[0], a0, b0);
					d0.i32s.f1 =
					    ARITH_FUN_INT(ps[1], a1, b1);
					arr[b] = d0.d64;

					d1.i32s.f0 =
					    ARITH_FUN_INT(ps[2], a2, b2);
					d1.i32s.f1 =
					    ARITH_FUN_INT(ps[3], a3, b3);
					arr[b + 1] = d1.d64;
				}

#pragma pipeloop(0)
				for (b = 0; b <= (n - 2); b += 2, pd += 4) {
					*(mlib_d64 *)pd =
					    vis_fpackfix_pair(arr[b],
					    arr[b + 1]);
				}

				if (pd < pend) {
					ARITH_FUN_16(*pd, *ps, a0, b0)
					    pd++;
					ps++;

					if (pd < pend) {
						ARITH_FUN_16(*pd, *ps, a1, b1)
						    pd++;
						ps++;

						if (pd < pend) {
							ARITH_FUN_16(*pd, *ps,
							    a2, b2)
							    pd++;
							ps++;
						}
					}
				}
			}
		}

		src += slb;
		dst += dlb;
	}
}

/* *********************************************************** */

void
mlib_v_ImageScale_Fp_F32_U16(
    mlib_u8 *dst,
    const mlib_u8 *src,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 nchan,
    mlib_s32 slb,
    mlib_s32 dlb,
    const mlib_d64 *alpha,
    const mlib_d64 *beta)
{
	mlib_f32 *ps;
	mlib_s16 *pd, *pend;
	mlib_f32 alpha2[9], beta2[9];
	mlib_d64 arr[BLOCK_SIZE];
	mlib_s32 i, j, k, n, b, off;
	mlib_f32 mask_8000 = vis_to_float(0x80008000);
	mlib_d64 maskd_8000 = vis_to_double_dup(0x80008000);

	vis_write_gsr(15 << 3);

/* prepare the alpha and beta values */
	for (k = 0; k < 9; k++) {
		alpha2[k] = alpha[k % nchan] * 2.0;
		beta2[k] = beta[k % nchan] * 2.0 - 65536.0;
	}

	for (j = 0; j < ysize; j++) {
		ps = (mlib_f32 *)src;
		pd = (mlib_s16 *)dst;
		pend = pd + xsize * nchan;

		off = ((mlib_addr)pd & 7) >> 1;

		if (off) {
			off = 4 - off;
			for (k = 0; (k < off) && (pd < pend); k++) {
				ARITH_FUN_U16(*pd, *ps, alpha2[k], beta2[k])
				    pd++;
				ps++;
			}
		}

		if (nchan == 3) {
			{
/* nchan = 3, any aligning for src */
				mlib_f32 a0, a1, a2, a3, a4, a5, b0, b1, b2, b3,
				    b4, b5;
				d64_2x32 d0;

				a0 = alpha2[off];
				b0 = beta2[off];
				a1 = alpha2[off + 1];
				b1 = beta2[off + 1];
				a2 = alpha2[off + 2];
				b2 = beta2[off + 2];
				a3 = alpha2[off + 3];
				b3 = beta2[off + 3];
				a4 = alpha2[off + 4];
				b4 = beta2[off + 4];
				a5 = alpha2[off + 5];
				b5 = beta2[off + 5];

				n = (pend - pd) / (BLOCK_SIZE_3 * 2);
				for (i = 0; i < n; i++) {
#pragma pipeloop(0)
					for (b = 0; b < BLOCK_SIZE_3;
					    b += 3, ps += 6) {
						d0.i32s.f0 =
						    ARITH_FUN_INT(ps[0], a0,
						    b0);
						d0.i32s.f1 =
						    ARITH_FUN_INT(ps[1], a1,
						    b1);
						arr[b] = d0.d64;

						d0.i32s.f0 =
						    ARITH_FUN_INT(ps[2], a2,
						    b2);
						d0.i32s.f1 =
						    ARITH_FUN_INT(ps[3], a3,
						    b3);
						arr[b + 1] = d0.d64;

						d0.i32s.f0 =
						    ARITH_FUN_INT(ps[4], a4,
						    b4);
						d0.i32s.f1 =
						    ARITH_FUN_INT(ps[5], a5,
						    b5);
						arr[b + 2] = d0.d64;
					}

#pragma pipeloop(0)
					for (b = 0; b < BLOCK_SIZE_3;
					    b++, pd += 2) {
						*(mlib_f32 *)pd =
						    vis_fxors(vis_fpackfix(arr
						    [b]), mask_8000);
					}
				}

				n = (pend - pd) >> 1;
#pragma pipeloop(0)
				for (b = 0; b <= (n - 3); b += 3, ps += 6) {
					d0.i32s.f0 =
					    ARITH_FUN_INT(ps[0], a0, b0);
					d0.i32s.f1 =
					    ARITH_FUN_INT(ps[1], a1, b1);
					arr[b] = d0.d64;

					d0.i32s.f0 =
					    ARITH_FUN_INT(ps[2], a2, b2);
					d0.i32s.f1 =
					    ARITH_FUN_INT(ps[3], a3, b3);
					arr[b + 1] = d0.d64;

					d0.i32s.f0 =
					    ARITH_FUN_INT(ps[4], a4, b4);
					d0.i32s.f1 =
					    ARITH_FUN_INT(ps[5], a5, b5);
					arr[b + 2] = d0.d64;
				}

#pragma pipeloop(0)
				for (b = 0; b <= (n - 3); b += 3, pd += 6) {
					*(mlib_f32 *)pd =
					    vis_fxors(vis_fpackfix(arr[b]),
					    mask_8000);
					*(mlib_f32 *)(pd + 2) =
					    vis_fxors(vis_fpackfix(arr[b + 1]),
					    mask_8000);
					*(mlib_f32 *)(pd + 4) =
					    vis_fxors(vis_fpackfix(arr[b + 2]),
					    mask_8000);
				}

				if (pd < pend) {
					ARITH_FUN_U16(*pd, *ps, a0, b0)
					    pd++;
					ps++;

					if (pd < pend) {
						ARITH_FUN_U16(*pd, *ps, a1, b1)
						    pd++;
						ps++;

						if (pd < pend) {
							ARITH_FUN_U16(*pd, *ps,
							    a2, b2)
							    pd++;
							ps++;

							if (pd < pend) {
							    ARITH_FUN_U16
								(*pd, *ps,
								a3, b3)
							    pd++;
							    ps++;

							    if (pd < pend) {
								ARITH_FUN_U16
									(*pd,
									*ps,
									a4,
									b4)
								pd++;
								ps++;
							    }
							}
						}
					}
				}
			}

		} else {

/* nchan != 3, all aligned */
			if (((mlib_addr)ps & 7) == 0) {

				mlib_f32 a0, a1, a2, a3, b0, b1, b2, b3;
				d64_2x32 d0, d1;

				a0 = alpha2[off];
				b0 = beta2[off];
				a1 = alpha2[off + 1];
				b1 = beta2[off + 1];
				a2 = alpha2[off + 2];
				b2 = beta2[off + 2];
				a3 = alpha2[off + 3];
				b3 = beta2[off + 3];

				n = (pend - pd) >> (SHIFT_16 + 1);
				for (i = 0; i < n; i++) {
#pragma pipeloop(0)
					for (b = 0; b < BLOCK_SIZE;
					    b += 2, ps += 4) {
						d0.d64 = *(mlib_d64 *)ps;
						d0.i32s.f0 =
						    ARITH_FUN_INT(d0.f32s.f0,
						    a0, b0);
						d0.i32s.f1 =
						    ARITH_FUN_INT(d0.f32s.f1,
						    a1, b1);
						arr[b] = d0.d64;

						d1.d64 = *(mlib_d64 *)(ps + 2);
						d1.i32s.f0 =
						    ARITH_FUN_INT(d1.f32s.f0,
						    a2, b2);
						d1.i32s.f1 =
						    ARITH_FUN_INT(d1.f32s.f1,
						    a3, b3);
						arr[b + 1] = d1.d64;
					}

#pragma pipeloop(0)
					for (b = 0; b < BLOCK_SIZE;
					    b += 2, pd += 4) {
						*(mlib_d64 *)pd =
						    vis_fxor(vis_fpackfix_pair
						    (arr[b], arr[b + 1]),
						    maskd_8000);
					}
				}

				n = (pend - pd) >> 1;
#pragma pipeloop(0)
				for (b = 0; b <= (n - 2); b += 2, ps += 4) {
					d0.d64 = *(mlib_d64 *)ps;
					d0.i32s.f0 =
					    ARITH_FUN_INT(d0.f32s.f0, a0, b0);
					d0.i32s.f1 =
					    ARITH_FUN_INT(d0.f32s.f1, a1, b1);
					arr[b] = d0.d64;

					d1.d64 = *(mlib_d64 *)(ps + 2);
					d1.i32s.f0 =
					    ARITH_FUN_INT(d1.f32s.f0, a2, b2);
					d1.i32s.f1 =
					    ARITH_FUN_INT(d1.f32s.f1, a3, b3);
					arr[b + 1] = d1.d64;
				}

#pragma pipeloop(0)
				for (b = 0; b <= (n - 2); b += 2, pd += 4) {
					*(mlib_d64 *)pd =
					    vis_fxor(vis_fpackfix_pair(arr[b],
					    arr[b + 1]), maskd_8000);
				}

				if (pd < pend) {
					ARITH_FUN_U16(*pd, *ps, a0, b0)
					    pd++;
					ps++;

					if (pd < pend) {
						ARITH_FUN_U16(*pd, *ps, a1, b1)
						    pd++;
						ps++;

						if (pd < pend) {
							ARITH_FUN_U16(*pd, *ps,
							    a2, b2)
							    pd++;
							ps++;
						}
					}
				}

			} else {
/* nchan != 3, src unaligned */

				mlib_f32 a0, a1, a2, a3, b0, b1, b2, b3;
				d64_2x32 d0, d1;

				a0 = alpha2[off];
				b0 = beta2[off];
				a1 = alpha2[off + 1];
				b1 = beta2[off + 1];
				a2 = alpha2[off + 2];
				b2 = beta2[off + 2];
				a3 = alpha2[off + 3];
				b3 = beta2[off + 3];

				n = (pend - pd) >> (SHIFT_16 + 1);
				for (i = 0; i < n; i++) {
#pragma pipeloop(0)
					for (b = 0; b < BLOCK_SIZE;
					    b += 2, ps += 4) {
						d0.i32s.f0 =
						    ARITH_FUN_INT(ps[0], a0,
						    b0);
						d0.i32s.f1 =
						    ARITH_FUN_INT(ps[1], a1,
						    b1);
						arr[b] = d0.d64;

						d1.i32s.f0 =
						    ARITH_FUN_INT(ps[2], a2,
						    b2);
						d1.i32s.f1 =
						    ARITH_FUN_INT(ps[3], a3,
						    b3);
						arr[b + 1] = d1.d64;
					}

#pragma pipeloop(0)
					for (b = 0; b < BLOCK_SIZE;
					    b += 2, pd += 4) {
						*(mlib_d64 *)pd =
						    vis_fxor(vis_fpackfix_pair
						    (arr[b], arr[b + 1]),
						    maskd_8000);
					}
				}

				n = (pend - pd) >> 1;
#pragma pipeloop(0)
				for (b = 0; b <= (n - 2); b += 2, ps += 4) {
					d0.i32s.f0 =
					    ARITH_FUN_INT(ps[0], a0, b0);
					d0.i32s.f1 =
					    ARITH_FUN_INT(ps[1], a1, b1);
					arr[b] = d0.d64;

					d1.i32s.f0 =
					    ARITH_FUN_INT(ps[2], a2, b2);
					d1.i32s.f1 =
					    ARITH_FUN_INT(ps[3], a3, b3);
					arr[b + 1] = d1.d64;
				}

#pragma pipeloop(0)
				for (b = 0; b <= (n - 2); b += 2, pd += 4) {
					*(mlib_d64 *)pd =
					    vis_fxor(vis_fpackfix_pair(arr[b],
					    arr[b + 1]), maskd_8000);
				}

				if (pd < pend) {
					ARITH_FUN_U16(*pd, *ps, a0, b0)
					    pd++;
					ps++;

					if (pd < pend) {
						ARITH_FUN_U16(*pd, *ps, a1, b1)
						    pd++;
						ps++;

						if (pd < pend) {
							ARITH_FUN_U16(*pd, *ps,
							    a2, b2)
							    pd++;
							ps++;
						}
					}
				}
			}
		}

		src += slb;
		dst += dlb;
	}
}

/* *********************************************************** */

void
mlib_v_ImageScale_Fp_F32_U8(
    mlib_u8 *dst,
    const mlib_u8 *src,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 nchan,
    mlib_s32 slb,
    mlib_s32 dlb,
    const mlib_d64 *alpha,
    const mlib_d64 *beta)
{
	mlib_f32 *ps;
	mlib_u8 *pd, *pend;
	mlib_f32 alpha2[9], beta2[9];
	mlib_d64 arr[BLOCK_SIZE];
	mlib_s32 i, j, k, n, b, off, emask;

	vis_write_gsr(15 << 3);

/* prepare the alpha and beta values */
	for (k = 0; k < 9; k++) {
		alpha2[k] = alpha[k % nchan] * (mlib_d64)(1 << 8);
		beta2[k] = beta[k % nchan] * (mlib_d64)(1 << 8);
	}

	for (j = 0; j < ysize; j++) {
		ps = (mlib_f32 *)src;
		pd = (mlib_u8 *)dst;
		pend = pd + xsize * nchan;

		off = ((mlib_addr)pd & 7);

		if (off) {
			off = 8 - off;
			for (k = 0; (k < off) && (pd < pend); k++) {
				ARITH_FUN_U8(*pd, *ps, alpha2[k], beta2[k])
				    pd++;
				ps++;
			}

			off = off % nchan;
		}

		if (nchan == 3) {
			{
/* nchan = 3, any aligning for src */
				mlib_f32 a0, a1, a2, a3, a4, a5, b0, b1, b2, b3,
				    b4, b5;
				d64_2x32 d0;

				a0 = alpha2[off];
				b0 = beta2[off];
				a1 = alpha2[off + 1];
				b1 = beta2[off + 1];
				a2 = alpha2[off + 2];
				b2 = beta2[off + 2];
				a3 = alpha2[off + 3];
				b3 = beta2[off + 3];
				a4 = alpha2[off + 4];
				b4 = beta2[off + 4];
				a5 = alpha2[off + 5];
				b5 = beta2[off + 5];

				n = (pend - pd) / (252 * 2);
				for (i = 0; i < n; i++) {
#pragma pipeloop(0)
					for (b = 0; b < 255; b += 3, ps += 6) {
						d0.i32s.f0 =
						    ARITH_FUN_INT(ps[0], a0,
						    b0);
						d0.i32s.f1 =
						    ARITH_FUN_INT(ps[1], a1,
						    b1);
						arr[b] = d0.d64;

						d0.i32s.f0 =
						    ARITH_FUN_INT(ps[2], a2,
						    b2);
						d0.i32s.f1 =
						    ARITH_FUN_INT(ps[3], a3,
						    b3);
						arr[b + 1] = d0.d64;

						d0.i32s.f0 =
						    ARITH_FUN_INT(ps[4], a4,
						    b4);
						d0.i32s.f1 =
						    ARITH_FUN_INT(ps[5], a5,
						    b5);
						arr[b + 2] = d0.d64;
					}

					ps -= 6;
#pragma pipeloop(0)
					for (b = 0; b < 252; b += 2, pd += 4) {
						mlib_d64 dd;

						MLIB_PACK_4x32_8(*(mlib_f32 *)
						    pd, dd, arr[b], arr[b + 1]);
					}
				}

				n = ((pend - pd - 1) / 6) + 1;

#pragma pipeloop(0)
				for (b = 0; b < 3 * n; b += 3, ps += 6) {
					d0.i32s.f0 =
					    ARITH_FUN_INT(ps[0], a0, b0);
					d0.i32s.f1 =
					    ARITH_FUN_INT(
						vis_ld_f32_nf(ps + 1), a1, b1);
					arr[b] = d0.d64;

					d0.i32s.f0 =
					    ARITH_FUN_INT(
						vis_ld_f32_nf(ps + 2), a2, b2);
					d0.i32s.f1 =
					    ARITH_FUN_INT(
						vis_ld_f32_nf(ps + 3), a3, b3);
					arr[b + 1] = d0.d64;

					d0.i32s.f0 =
					    ARITH_FUN_INT(
						vis_ld_f32_nf(ps + 4), a4, b4);
					d0.i32s.f1 =
					    ARITH_FUN_INT(
						vis_ld_f32_nf(ps + 5), a5, b5);
					arr[b + 2] = d0.d64;
				}

#pragma pipeloop(0)
				for (b = 0; pd <= (pend - 4); b += 2, pd += 4) {
					mlib_d64 dd;

					MLIB_PACK_4x32_8(*(mlib_f32 *)pd, dd,
					    arr[b], arr[b + 1]);
				}

				if (pd < pend) {
					mlib_d64 dd;
					mlib_f32 df;

					MLIB_PACK_4x32_8(df, dd, arr[b],
					    arr[b + 1]);
					emask = vis_edge8(pd, pend - 1);

					if ((mlib_addr)pd & 7) {
						pd -= 4;
						dd = vis_freg_pair(0, df);
					} else {
						dd = vis_freg_pair(df, 0);
					}

					vis_pst_8(dd, pd, emask);
					pd = pend;
				}
			}

		} else {

/* nchan != 3, all aligned */
			if (((mlib_addr)ps & 7) == 0) {

				mlib_f32 a0, a1, a2, a3, b0, b1, b2, b3;
				d64_2x32 d0, d1;

				a0 = alpha2[off];
				b0 = beta2[off];
				a1 = alpha2[off + 1];
				b1 = beta2[off + 1];
				a2 = alpha2[off + 2];
				b2 = beta2[off + 2];
				a3 = alpha2[off + 3];
				b3 = beta2[off + 3];

				n = (pend - pd) >> (SHIFT_16 + 1);
				for (i = 0; i < n; i++) {
#pragma pipeloop(0)
					for (b = 0; b < BLOCK_SIZE;
					    b += 2, ps += 4) {
						d0.d64 = *(mlib_d64 *)ps;
						d0.i32s.f0 =
						    ARITH_FUN_INT(d0.f32s.f0,
						    a0, b0);
						d0.i32s.f1 =
						    ARITH_FUN_INT(d0.f32s.f1,
						    a1, b1);
						arr[b] = d0.d64;

						d1.d64 = *(mlib_d64 *)(ps + 2);
						d1.i32s.f0 =
						    ARITH_FUN_INT(d1.f32s.f0,
						    a2, b2);
						d1.i32s.f1 =
						    ARITH_FUN_INT(d1.f32s.f1,
						    a3, b3);
						arr[b + 1] = d1.d64;
					}

#pragma pipeloop(0)
					for (b = 0; b < BLOCK_SIZE;
					    b += 2, pd += 4) {
						mlib_d64 dd;

						MLIB_PACK_4x32_8(*(mlib_f32 *)
						    pd, dd, arr[b], arr[b + 1]);
					}
				}

				n = (pend - pd) >> 1;
#pragma pipeloop(0)
				for (b = 0; b <= (n - 2); b += 2, ps += 4) {
					d0.d64 = *(mlib_d64 *)ps;
					d0.i32s.f0 =
					    ARITH_FUN_INT(d0.f32s.f0, a0, b0);
					d0.i32s.f1 =
					    ARITH_FUN_INT(d0.f32s.f1, a1, b1);
					arr[b] = d0.d64;

					d1.d64 = *(mlib_d64 *)(ps + 2);
					d1.i32s.f0 =
					    ARITH_FUN_INT(d1.f32s.f0, a2, b2);
					d1.i32s.f1 =
					    ARITH_FUN_INT(d1.f32s.f1, a3, b3);
					arr[b + 1] = d1.d64;
				}

#pragma pipeloop(0)
				for (b = 0; b <= (n - 2); b += 2, pd += 4) {
					mlib_d64 dd;

					MLIB_PACK_4x32_8(*(mlib_f32 *)pd, dd,
					    arr[b], arr[b + 1]);
				}

			} else {
/* nchan != 3, src unaligned */

				mlib_f32 a0, a1, a2, a3, b0, b1, b2, b3;
				d64_2x32 d0, d1;

				a0 = alpha2[off];
				b0 = beta2[off];
				a1 = alpha2[off + 1];
				b1 = beta2[off + 1];
				a2 = alpha2[off + 2];
				b2 = beta2[off + 2];
				a3 = alpha2[off + 3];
				b3 = beta2[off + 3];

				n = (pend - pd) >> (SHIFT_16 + 1);
				for (i = 0; i < n; i++) {
#pragma pipeloop(0)
					for (b = 0; b < BLOCK_SIZE;
					    b += 2, ps += 4) {
						d0.i32s.f0 =
						    ARITH_FUN_INT(ps[0], a0,
						    b0);
						d0.i32s.f1 =
						    ARITH_FUN_INT(ps[1], a1,
						    b1);
						arr[b] = d0.d64;

						d1.i32s.f0 =
						    ARITH_FUN_INT(ps[2], a2,
						    b2);
						d1.i32s.f1 =
						    ARITH_FUN_INT(ps[3], a3,
						    b3);
						arr[b + 1] = d1.d64;
					}

#pragma pipeloop(0)
					for (b = 0; b < BLOCK_SIZE;
					    b += 2, pd += 4) {
						mlib_d64 dd;

						MLIB_PACK_4x32_8(*(mlib_f32 *)
						    pd, dd, arr[b], arr[b + 1]);
					}
				}

				n = (pend - pd) >> 1;
#pragma pipeloop(0)
				for (b = 0; b <= (n - 2); b += 2, ps += 4) {
					d0.i32s.f0 =
					    ARITH_FUN_INT(ps[0], a0, b0);
					d0.i32s.f1 =
					    ARITH_FUN_INT(ps[1], a1, b1);
					arr[b] = d0.d64;

					d1.i32s.f0 =
					    ARITH_FUN_INT(ps[2], a2, b2);
					d1.i32s.f1 =
					    ARITH_FUN_INT(ps[3], a3, b3);
					arr[b + 1] = d1.d64;
				}

#pragma pipeloop(0)
				for (b = 0; b <= (n - 2); b += 2, pd += 4) {
					mlib_d64 dd;

					MLIB_PACK_4x32_8(*(mlib_f32 *)pd, dd,
					    arr[b], arr[b + 1]);
				}
			}
		}

		k = off;
		while (pd < pend) {
			ARITH_FUN_U8(*pd, *ps, alpha2[k], beta2[k])
			    pd++;
			ps++;
			k = (k + 1) % nchan;
		}

		src += slb;
		dst += dlb;
	}
}

/* *********************************************************** */

void
mlib_v_ImageScale_Fp_D64_S16(
    mlib_u8 *dst,
    const mlib_u8 *src,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 nchan,
    mlib_s32 slb,
    mlib_s32 dlb,
    const mlib_d64 *alpha,
    const mlib_d64 *beta)
{
	mlib_d64 *ps;
	mlib_s16 *pd, *pend;
	mlib_d64 alpha2[9], beta2[9];
	mlib_d64 arr[BLOCK_SIZE];
	mlib_s32 i, j, k, n, b, off;

	vis_write_gsr(15 << 3);

/* prepare the alpha and beta values */
	for (k = 0; k < 9; k++) {
		alpha2[k] = alpha[k % nchan] * 2.0;
		beta2[k] = beta[k % nchan] * 2.0;
	}

	for (j = 0; j < ysize; j++) {
		ps = (mlib_d64 *)src;
		pd = (mlib_s16 *)dst;
		pend = pd + xsize * nchan;

		off = ((mlib_addr)pd & 7) >> 1;

		if (off) {
			off = 4 - off;
			for (k = 0; (k < off) && (pd < pend); k++) {
				ARITH_FUN_16(*pd, *ps, alpha2[k], beta2[k])
				    pd++;
				ps++;
			}
		}

		if (nchan == 3) {
			{
/* nchan = 3, any aligning for src */
				mlib_d64 a0, a1, a2, a3, a4, a5, b0, b1, b2, b3,
				    b4, b5;
				d64_2x32 d0;

				a0 = alpha2[off];
				b0 = beta2[off];
				a1 = alpha2[off + 1];
				b1 = beta2[off + 1];
				a2 = alpha2[off + 2];
				b2 = beta2[off + 2];
				a3 = alpha2[off + 3];
				b3 = beta2[off + 3];
				a4 = alpha2[off + 4];
				b4 = beta2[off + 4];
				a5 = alpha2[off + 5];
				b5 = beta2[off + 5];

				n = (pend - pd) / (BLOCK_SIZE_3 * 2);
				for (i = 0; i < n; i++) {
#pragma pipeloop(0)
					for (b = 0; b < BLOCK_SIZE_3;
					    b += 3, ps += 6) {
						d0.i32s.f0 =
						    ARITH_FUN_INT(ps[0], a0,
						    b0);
						d0.i32s.f1 =
						    ARITH_FUN_INT(ps[1], a1,
						    b1);
						arr[b] = d0.d64;

						d0.i32s.f0 =
						    ARITH_FUN_INT(ps[2], a2,
						    b2);
						d0.i32s.f1 =
						    ARITH_FUN_INT(ps[3], a3,
						    b3);
						arr[b + 1] = d0.d64;

						d0.i32s.f0 =
						    ARITH_FUN_INT(ps[4], a4,
						    b4);
						d0.i32s.f1 =
						    ARITH_FUN_INT(ps[5], a5,
						    b5);
						arr[b + 2] = d0.d64;
					}

#pragma pipeloop(0)
					for (b = 0; b < BLOCK_SIZE_3;
					    b++, pd += 2) {
						*(mlib_f32 *)pd =
						    vis_fpackfix(arr[b]);
					}
				}

				n = (pend - pd) >> 1;
#pragma pipeloop(0)
				for (b = 0; b <= (n - 3); b += 3, ps += 6) {
					d0.i32s.f0 =
					    ARITH_FUN_INT(ps[0], a0, b0);
					d0.i32s.f1 =
					    ARITH_FUN_INT(ps[1], a1, b1);
					arr[b] = d0.d64;

					d0.i32s.f0 =
					    ARITH_FUN_INT(ps[2], a2, b2);
					d0.i32s.f1 =
					    ARITH_FUN_INT(ps[3], a3, b3);
					arr[b + 1] = d0.d64;

					d0.i32s.f0 =
					    ARITH_FUN_INT(ps[4], a4, b4);
					d0.i32s.f1 =
					    ARITH_FUN_INT(ps[5], a5, b5);
					arr[b + 2] = d0.d64;
				}

#pragma pipeloop(0)
				for (b = 0; b <= (n - 3); b += 3, pd += 6) {
					*(mlib_f32 *)pd = vis_fpackfix(arr[b]);
					*(mlib_f32 *)(pd + 2) =
					    vis_fpackfix(arr[b + 1]);
					*(mlib_f32 *)(pd + 4) =
					    vis_fpackfix(arr[b + 2]);
				}

				if (pd < pend) {
					ARITH_FUN_16(*pd, *ps, a0, b0)
					    pd++;
					ps++;

					if (pd < pend) {
						ARITH_FUN_16(*pd, *ps, a1, b1)
						    pd++;
						ps++;

						if (pd < pend) {
							ARITH_FUN_16(*pd, *ps,
							    a2, b2)
							    pd++;
							ps++;

							if (pd < pend) {
								ARITH_FUN_16
								    (*pd, *ps,
								    a3, b3)
								    pd++;
								ps++;

								if (pd < pend) {
								    ARITH_FUN_16
									(*pd,
									*ps,
									a4,
									b4)
								    pd++;
								    ps++;
								}
							}
						}
					}
				}
			}

		} else {

			{
/* nchan != 3, src unaligned */

				mlib_d64 a0, a1, a2, a3, b0, b1, b2, b3;
				d64_2x32 d0, d1;

				a0 = alpha2[off];
				b0 = beta2[off];
				a1 = alpha2[off + 1];
				b1 = beta2[off + 1];
				a2 = alpha2[off + 2];
				b2 = beta2[off + 2];
				a3 = alpha2[off + 3];
				b3 = beta2[off + 3];

				n = (pend - pd) >> (SHIFT_16 + 1);
				for (i = 0; i < n; i++) {
#pragma pipeloop(0)
					for (b = 0; b < BLOCK_SIZE;
					    b += 2, ps += 4) {
						d0.i32s.f0 =
						    ARITH_FUN_INT(ps[0], a0,
						    b0);
						d0.i32s.f1 =
						    ARITH_FUN_INT(ps[1], a1,
						    b1);
						arr[b] = d0.d64;

						d1.i32s.f0 =
						    ARITH_FUN_INT(ps[2], a2,
						    b2);
						d1.i32s.f1 =
						    ARITH_FUN_INT(ps[3], a3,
						    b3);
						arr[b + 1] = d1.d64;
					}

#pragma pipeloop(0)
					for (b = 0; b < BLOCK_SIZE;
					    b += 2, pd += 4) {
						*(mlib_d64 *)pd =
						    vis_fpackfix_pair(arr[b],
						    arr[b + 1]);
					}
				}

				n = (pend - pd) >> 1;
#pragma pipeloop(0)
				for (b = 0; b <= (n - 2); b += 2, ps += 4) {
					d0.i32s.f0 =
					    ARITH_FUN_INT(ps[0], a0, b0);
					d0.i32s.f1 =
					    ARITH_FUN_INT(ps[1], a1, b1);
					arr[b] = d0.d64;

					d1.i32s.f0 =
					    ARITH_FUN_INT(ps[2], a2, b2);
					d1.i32s.f1 =
					    ARITH_FUN_INT(ps[3], a3, b3);
					arr[b + 1] = d1.d64;
				}

#pragma pipeloop(0)
				for (b = 0; b <= (n - 2); b += 2, pd += 4) {
					*(mlib_d64 *)pd =
					    vis_fpackfix_pair(arr[b],
					    arr[b + 1]);
				}

				if (pd < pend) {
					ARITH_FUN_16(*pd, *ps, a0, b0)
					    pd++;
					ps++;

					if (pd < pend) {
						ARITH_FUN_16(*pd, *ps, a1, b1)
						    pd++;
						ps++;

						if (pd < pend) {
							ARITH_FUN_16(*pd, *ps,
							    a2, b2)
							    pd++;
							ps++;
						}
					}
				}
			}
		}

		src += slb;
		dst += dlb;
	}
}

/* *********************************************************** */

void
mlib_v_ImageScale_Fp_D64_U16(
    mlib_u8 *dst,
    const mlib_u8 *src,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 nchan,
    mlib_s32 slb,
    mlib_s32 dlb,
    const mlib_d64 *alpha,
    const mlib_d64 *beta)
{
	mlib_d64 *ps;
	mlib_s16 *pd, *pend;
	mlib_d64 alpha2[9], beta2[9];
	mlib_d64 arr[BLOCK_SIZE];
	mlib_s32 i, j, k, n, b, off;
	mlib_f32 mask_8000 = vis_to_float(0x80008000);
	mlib_d64 maskd_8000 = vis_to_double_dup(0x80008000);

	vis_write_gsr(15 << 3);

/* prepare the alpha and beta values */
	for (k = 0; k < 9; k++) {
		alpha2[k] = alpha[k % nchan] * 2.0;
		beta2[k] = beta[k % nchan] * 2.0 - 65536.0;
	}

	for (j = 0; j < ysize; j++) {
		ps = (mlib_d64 *)src;
		pd = (mlib_s16 *)dst;
		pend = pd + xsize * nchan;

		off = ((mlib_addr)pd & 7) >> 1;

		if (off) {
			off = 4 - off;
			for (k = 0; (k < off) && (pd < pend); k++) {
				ARITH_FUN_U16(*pd, *ps, alpha2[k], beta2[k])
				    pd++;
				ps++;
			}
		}

		if (nchan == 3) {
			{
/* nchan = 3, any aligning for src */
				mlib_d64 a0, a1, a2, a3, a4, a5, b0, b1, b2, b3,
				    b4, b5;
				d64_2x32 d0;

				a0 = alpha2[off];
				b0 = beta2[off];
				a1 = alpha2[off + 1];
				b1 = beta2[off + 1];
				a2 = alpha2[off + 2];
				b2 = beta2[off + 2];
				a3 = alpha2[off + 3];
				b3 = beta2[off + 3];
				a4 = alpha2[off + 4];
				b4 = beta2[off + 4];
				a5 = alpha2[off + 5];
				b5 = beta2[off + 5];

				n = (pend - pd) / (BLOCK_SIZE_3 * 2);
				for (i = 0; i < n; i++) {
#pragma pipeloop(0)
					for (b = 0; b < BLOCK_SIZE_3;
					    b += 3, ps += 6) {
						d0.i32s.f0 =
						    ARITH_FUN_INT(ps[0], a0,
						    b0);
						d0.i32s.f1 =
						    ARITH_FUN_INT(ps[1], a1,
						    b1);
						arr[b] = d0.d64;

						d0.i32s.f0 =
						    ARITH_FUN_INT(ps[2], a2,
						    b2);
						d0.i32s.f1 =
						    ARITH_FUN_INT(ps[3], a3,
						    b3);
						arr[b + 1] = d0.d64;

						d0.i32s.f0 =
						    ARITH_FUN_INT(ps[4], a4,
						    b4);
						d0.i32s.f1 =
						    ARITH_FUN_INT(ps[5], a5,
						    b5);
						arr[b + 2] = d0.d64;
					}

#pragma pipeloop(0)
					for (b = 0; b < BLOCK_SIZE_3;
					    b++, pd += 2) {
						*(mlib_f32 *)pd =
						    vis_fxors(vis_fpackfix(arr
						    [b]), mask_8000);
					}
				}

				n = (pend - pd) >> 1;
#pragma pipeloop(0)
				for (b = 0; b <= (n - 3); b += 3, ps += 6) {
					d0.i32s.f0 =
					    ARITH_FUN_INT(ps[0], a0, b0);
					d0.i32s.f1 =
					    ARITH_FUN_INT(ps[1], a1, b1);
					arr[b] = d0.d64;

					d0.i32s.f0 =
					    ARITH_FUN_INT(ps[2], a2, b2);
					d0.i32s.f1 =
					    ARITH_FUN_INT(ps[3], a3, b3);
					arr[b + 1] = d0.d64;

					d0.i32s.f0 =
					    ARITH_FUN_INT(ps[4], a4, b4);
					d0.i32s.f1 =
					    ARITH_FUN_INT(ps[5], a5, b5);
					arr[b + 2] = d0.d64;
				}

#pragma pipeloop(0)
				for (b = 0; b <= (n - 3); b += 3, pd += 6) {
					*(mlib_f32 *)pd =
					    vis_fxors(vis_fpackfix(arr[b]),
					    mask_8000);
					*(mlib_f32 *)(pd + 2) =
					    vis_fxors(vis_fpackfix(arr[b + 1]),
					    mask_8000);
					*(mlib_f32 *)(pd + 4) =
					    vis_fxors(vis_fpackfix(arr[b + 2]),
					    mask_8000);
				}

				if (pd < pend) {
					ARITH_FUN_U16(*pd, *ps, a0, b0)
					    pd++;
					ps++;

					if (pd < pend) {
						ARITH_FUN_U16(*pd, *ps, a1, b1)
						    pd++;
						ps++;

						if (pd < pend) {
							ARITH_FUN_U16(*pd, *ps,
							    a2, b2)
							    pd++;
							ps++;

							if (pd < pend) {
							    ARITH_FUN_U16
								(*pd, *ps,
								a3, b3)
							    pd++;
							    ps++;

							    if (pd < pend) {
								ARITH_FUN_U16
									(*pd,
									*ps,
									a4,
									b4)
								pd++;
								ps++;
							    }
							}
						}
					}
				}
			}

		} else {

			{
/* nchan != 3, src unaligned */

				mlib_d64 a0, a1, a2, a3, b0, b1, b2, b3;
				d64_2x32 d0, d1;

				a0 = alpha2[off];
				b0 = beta2[off];
				a1 = alpha2[off + 1];
				b1 = beta2[off + 1];
				a2 = alpha2[off + 2];
				b2 = beta2[off + 2];
				a3 = alpha2[off + 3];
				b3 = beta2[off + 3];

				n = (pend - pd) >> (SHIFT_16 + 1);
				for (i = 0; i < n; i++) {
#pragma pipeloop(0)
					for (b = 0; b < BLOCK_SIZE;
					    b += 2, ps += 4) {
						d0.i32s.f0 =
						    ARITH_FUN_INT(ps[0], a0,
						    b0);
						d0.i32s.f1 =
						    ARITH_FUN_INT(ps[1], a1,
						    b1);
						arr[b] = d0.d64;

						d1.i32s.f0 =
						    ARITH_FUN_INT(ps[2], a2,
						    b2);
						d1.i32s.f1 =
						    ARITH_FUN_INT(ps[3], a3,
						    b3);
						arr[b + 1] = d1.d64;
					}

#pragma pipeloop(0)
					for (b = 0; b < BLOCK_SIZE;
					    b += 2, pd += 4) {
						*(mlib_d64 *)pd =
						    vis_fxor(vis_fpackfix_pair
						    (arr[b], arr[b + 1]),
						    maskd_8000);
					}
				}

				n = (pend - pd) >> 1;
#pragma pipeloop(0)
				for (b = 0; b <= (n - 2); b += 2, ps += 4) {
					d0.i32s.f0 =
					    ARITH_FUN_INT(ps[0], a0, b0);
					d0.i32s.f1 =
					    ARITH_FUN_INT(ps[1], a1, b1);
					arr[b] = d0.d64;

					d1.i32s.f0 =
					    ARITH_FUN_INT(ps[2], a2, b2);
					d1.i32s.f1 =
					    ARITH_FUN_INT(ps[3], a3, b3);
					arr[b + 1] = d1.d64;
				}

#pragma pipeloop(0)
				for (b = 0; b <= (n - 2); b += 2, pd += 4) {
					*(mlib_d64 *)pd =
					    vis_fxor(vis_fpackfix_pair(arr[b],
					    arr[b + 1]), maskd_8000);
				}

				if (pd < pend) {
					ARITH_FUN_U16(*pd, *ps, a0, b0)
					    pd++;
					ps++;

					if (pd < pend) {
						ARITH_FUN_U16(*pd, *ps, a1, b1)
						    pd++;
						ps++;

						if (pd < pend) {
							ARITH_FUN_U16(*pd, *ps,
							    a2, b2)
							    pd++;
							ps++;
						}
					}
				}
			}
		}

		src += slb;
		dst += dlb;
	}
}

/* *********************************************************** */

void
mlib_v_ImageScale_Fp_D64_U8(
    mlib_u8 *dst,
    const mlib_u8 *src,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 nchan,
    mlib_s32 slb,
    mlib_s32 dlb,
    const mlib_d64 *alpha,
    const mlib_d64 *beta)
{
	mlib_d64 *ps;
	mlib_u8 *pd, *pend;
	mlib_d64 alpha2[9], beta2[9];
	mlib_d64 arr[BLOCK_SIZE];
	mlib_s32 i, j, k, n, b, off, emask;

	vis_write_gsr(15 << 3);

/* prepare the alpha and beta values */
	for (k = 0; k < 9; k++) {
		alpha2[k] = alpha[k % nchan] * (mlib_d64)(1 << 8);
		beta2[k] = beta[k % nchan] * (mlib_d64)(1 << 8);
	}

	for (j = 0; j < ysize; j++) {
		ps = (mlib_d64 *)src;
		pd = (mlib_u8 *)dst;
		pend = pd + xsize * nchan;

		off = ((mlib_addr)pd & 7);

		if (off) {
			off = 8 - off;
			for (k = 0; (k < off) && (pd < pend); k++) {
				ARITH_FUN_U8(*pd, *ps, alpha2[k], beta2[k])
				    pd++;
				ps++;
			}

			off = off % nchan;
		}

		if (nchan == 3) {
			{
/* nchan = 3, any aligning for src */
				mlib_d64 a0, a1, a2, a3, a4, a5, b0, b1, b2, b3,
				    b4, b5;
				d64_2x32 d0;

				a0 = alpha2[off];
				b0 = beta2[off];
				a1 = alpha2[off + 1];
				b1 = beta2[off + 1];
				a2 = alpha2[off + 2];
				b2 = beta2[off + 2];
				a3 = alpha2[off + 3];
				b3 = beta2[off + 3];
				a4 = alpha2[off + 4];
				b4 = beta2[off + 4];
				a5 = alpha2[off + 5];
				b5 = beta2[off + 5];

				n = (pend - pd) / (252 * 2);
				for (i = 0; i < n; i++) {
#pragma pipeloop(0)
					for (b = 0; b < 252; b += 3, ps += 6) {
						d0.i32s.f0 =
						    ARITH_FUN_INT(ps[0], a0,
						    b0);
						d0.i32s.f1 =
						    ARITH_FUN_INT(ps[1], a1,
						    b1);
						arr[b] = d0.d64;

						d0.i32s.f0 =
						    ARITH_FUN_INT(ps[2], a2,
						    b2);
						d0.i32s.f1 =
						    ARITH_FUN_INT(ps[3], a3,
						    b3);
						arr[b + 1] = d0.d64;

						d0.i32s.f0 =
						    ARITH_FUN_INT(ps[4], a4,
						    b4);
						d0.i32s.f1 =
						    ARITH_FUN_INT(ps[5], a5,
						    b5);
						arr[b + 2] = d0.d64;
					}

#pragma pipeloop(0)
					for (b = 0; b < 252; b += 2, pd += 4) {
						mlib_d64 dd;

						MLIB_PACK_4x32_8(*(mlib_f32 *)
						    pd, dd, arr[b], arr[b + 1]);
					}
				}

				n = ((pend - pd - 1) / 6) + 1;
#pragma pipeloop(0)
				for (b = 0; b < 3 * n; b += 3, ps += 6) {
					d0.i32s.f0 =
					    ARITH_FUN_INT(ps[0], a0, b0);
					d0.i32s.f1 =
					    ARITH_FUN_INT(
						vis_ld_d64_nf(ps + 1), a1, b1);
					arr[b] = d0.d64;

					d0.i32s.f0 =
					    ARITH_FUN_INT(
						vis_ld_d64_nf(ps + 2), a2, b2);
					d0.i32s.f1 =
					    ARITH_FUN_INT(
						vis_ld_d64_nf(ps + 3), a3, b3);
					arr[b + 1] = d0.d64;

					d0.i32s.f0 =
					    ARITH_FUN_INT(
						vis_ld_d64_nf(ps + 4), a4, b4);
					d0.i32s.f1 =
					    ARITH_FUN_INT(
						vis_ld_d64_nf(ps + 5), a5, b5);
					arr[b + 2] = d0.d64;
				}

#pragma pipeloop(0)
				for (b = 0; pd <= (pend - 4); b += 2, pd += 4) {
					mlib_d64 dd;

					MLIB_PACK_4x32_8(*(mlib_f32 *)pd, dd,
					    arr[b], arr[b + 1]);
				}

				if (pd < pend) {
					mlib_d64 dd;
					mlib_f32 df;

					MLIB_PACK_4x32_8(df, dd, arr[b],
					    arr[b + 1]);
					emask = vis_edge8(pd, pend - 1);

					if ((mlib_addr)pd & 7) {
						pd -= 4;
						dd = vis_freg_pair(0, df);
					} else {
						dd = vis_freg_pair(df, 0);
					}

					vis_pst_8(dd, pd, emask);
					pd = pend;
				}
			}

		} else {

			{
/* nchan != 3, src unaligned */

				mlib_d64 a0, a1, a2, a3, b0, b1, b2, b3;
				d64_2x32 d0, d1;

				a0 = alpha2[off];
				b0 = beta2[off];
				a1 = alpha2[off + 1];
				b1 = beta2[off + 1];
				a2 = alpha2[off + 2];
				b2 = beta2[off + 2];
				a3 = alpha2[off + 3];
				b3 = beta2[off + 3];

				n = (pend - pd) >> (SHIFT_16 + 1);
				for (i = 0; i < n; i++) {
#pragma pipeloop(0)
					for (b = 0; b < BLOCK_SIZE;
					    b += 2, ps += 4) {
						d0.i32s.f0 =
						    ARITH_FUN_INT(ps[0], a0,
						    b0);
						d0.i32s.f1 =
						    ARITH_FUN_INT(ps[1], a1,
						    b1);
						arr[b] = d0.d64;

						d1.i32s.f0 =
						    ARITH_FUN_INT(ps[2], a2,
						    b2);
						d1.i32s.f1 =
						    ARITH_FUN_INT(ps[3], a3,
						    b3);
						arr[b + 1] = d1.d64;
					}

#pragma pipeloop(0)
					for (b = 0; b < BLOCK_SIZE;
					    b += 2, pd += 4) {
						mlib_d64 dd;

						MLIB_PACK_4x32_8(*(mlib_f32 *)
						    pd, dd, arr[b], arr[b + 1]);
					}
				}

				n = (pend - pd) >> 1;
#pragma pipeloop(0)
				for (b = 0; b <= (n - 2); b += 2, ps += 4) {
					d0.i32s.f0 =
					    ARITH_FUN_INT(ps[0], a0, b0);
					d0.i32s.f1 =
					    ARITH_FUN_INT(ps[1], a1, b1);
					arr[b] = d0.d64;

					d1.i32s.f0 =
					    ARITH_FUN_INT(ps[2], a2, b2);
					d1.i32s.f1 =
					    ARITH_FUN_INT(ps[3], a3, b3);
					arr[b + 1] = d1.d64;
				}

#pragma pipeloop(0)
				for (b = 0; b <= (n - 2); b += 2, pd += 4) {
					mlib_d64 dd;

					MLIB_PACK_4x32_8(*(mlib_f32 *)pd, dd,
					    arr[b], arr[b + 1]);
				}
			}
		}

		k = off;
		while (pd < pend) {
			ARITH_FUN_U8(*pd, *ps, alpha2[k], beta2[k])
			    pd++;
			ps++;
			k = (k + 1) % nchan;
		}

		src += slb;
		dst += dlb;
	}
}

/* *********************************************************** */

void
mlib_v_ImageScale_Fp_F32_F32(
    mlib_u8 *dst,
    const mlib_u8 *src,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 nchan,
    mlib_s32 slb,
    mlib_s32 dlb,
    const mlib_d64 *alpha,
    const mlib_d64 *beta)
{
	mlib_f32 *ps, *pd, *pend;
	mlib_f32 alpha2[7], beta2[7];
	mlib_s32 j, k, off;

/* prepare the alpha and beta values */
	for (k = 0; k < 7; k++) {
		alpha2[k] = alpha[k % nchan];
		beta2[k] = beta[k % nchan];
	}

	for (j = 0; j < ysize; j++) {
		ps = (mlib_f32 *)src;
		pd = (mlib_f32 *)dst;
		pend = pd + xsize * nchan;

		if ((mlib_addr)pd & 7) {
			(*pd++) = ARITH_FUN(*ps, alpha2[0], beta2[0]);
			ps++;
			off = 1;
		} else {
			off = 0;
		}

		if (nchan == 3) {
			{
/* nchan = 3, any aligning for src */
				mlib_f32 a0, a1, a2, a3, a4, a5, b0, b1, b2, b3,
				    b4, b5;
				a0 = alpha2[off];
				b0 = beta2[off];
				a1 = alpha2[off + 1];
				b1 = beta2[off + 1];
				a2 = alpha2[off + 2];
				b2 = beta2[off + 2];
				a3 = alpha2[off + 3];
				b3 = beta2[off + 3];
				a4 = alpha2[off + 4];
				b4 = beta2[off + 4];
				a5 = alpha2[off + 5];
				b5 = beta2[off + 5];
#pragma pipeloop(0)
				for (; pd <= (pend - 6); pd += 6, ps += 6) {
					d64_2x32 d0, d1, d2;

					d0.f32s.f0 = ARITH_FUN(*(ps), a0, b0);
					d0.f32s.f1 =
					    ARITH_FUN(*(ps + 1), a1, b1);
					*(mlib_d64 *)pd = d0.d64;

					d1.f32s.f0 =
					    ARITH_FUN(*(ps + 2), a2, b2);
					d1.f32s.f1 =
					    ARITH_FUN(*(ps + 3), a3, b3);
					*(mlib_d64 *)(pd + 2) = d1.d64;

					d2.f32s.f0 =
					    ARITH_FUN(*(ps + 4), a4, b4);
					d2.f32s.f1 =
					    ARITH_FUN(*(ps + 5), a5, b5);
					*(mlib_d64 *)(pd + 4) = d2.d64;
				}

				if (pd < pend) {
					(*pd++) = ARITH_FUN(*ps, a0, b0);
					ps++;

					if (pd < pend) {
						(*pd++) =
						    ARITH_FUN(*ps, a1, b1);
						ps++;

						if (pd < pend) {
							(*pd++) =
							    ARITH_FUN(*ps, a2,
							    b2);
							ps++;

							if (pd < pend) {
							    (*pd++) =
								ARITH_FUN
								(*ps, a3,
								b3);
							    ps++;

							    if (pd < pend) {
								(*pd++) =
								    ARITH_FUN
									(*ps,
									a4,
									b4);
								ps++;
							    }
							}
						}
					}
				}
			}

		} else {

/* nchan != 3, all aligned */
			if (((mlib_addr)ps & 7) == 0) {

				mlib_f32 a0, a1, a2, a3, b0, b1, b2, b3;

				a0 = alpha2[off];
				b0 = beta2[off];
				a1 = alpha2[off + 1];
				b1 = beta2[off + 1];
				a2 = alpha2[off + 2];
				b2 = beta2[off + 2];
				a3 = alpha2[off + 3];
				b3 = beta2[off + 3];

#pragma pipeloop(0)
				for (; pd <= (pend - 4); pd += 4, ps += 4) {
					d64_2x32 d0, d1;

					d0.d64 = *(mlib_d64 *)ps;
					d0.f32s.f0 =
					    ARITH_FUN(d0.f32s.f0, a0, b0);
					d0.f32s.f1 =
					    ARITH_FUN(d0.f32s.f1, a1, b1);
					*(mlib_d64 *)pd = d0.d64;

					d1.d64 = *(mlib_d64 *)(ps + 2);
					d1.f32s.f0 =
					    ARITH_FUN(d1.f32s.f0, a2, b2);
					d1.f32s.f1 =
					    ARITH_FUN(d1.f32s.f1, a3, b3);
					*(mlib_d64 *)(pd + 2) = d1.d64;
				}

				if (pd < pend) {
					(*pd++) = ARITH_FUN(*ps, a0, b0);
					ps++;

					if (pd < pend) {
						(*pd++) =
						    ARITH_FUN(*ps, a1, b1);
						ps++;

						if (pd < pend) {
							(*pd++) =
							    ARITH_FUN(*ps, a2,
							    b2);
							ps++;
						}
					}
				}

			} else {
/* nchan != 3, src unaligned */

				mlib_f32 a0, a1, a2, a3, b0, b1, b2, b3;

				a0 = alpha2[off];
				b0 = beta2[off];
				a1 = alpha2[off + 1];
				b1 = beta2[off + 1];
				a2 = alpha2[off + 2];
				b2 = beta2[off + 2];
				a3 = alpha2[off + 3];
				b3 = beta2[off + 3];
#pragma pipeloop(0)
				for (; pd <= (pend - 4); pd += 4, ps += 4) {
					d64_2x32 d0, d1;

					d0.f32s.f0 = ARITH_FUN(*(ps), a0, b0);
					d0.f32s.f1 =
					    ARITH_FUN(*(ps + 1), a1, b1);
					*(mlib_d64 *)pd = d0.d64;

					d1.f32s.f0 =
					    ARITH_FUN(*(ps + 2), a2, b2);
					d1.f32s.f1 =
					    ARITH_FUN(*(ps + 3), a3, b3);
					*(mlib_d64 *)(pd + 2) = d1.d64;
				}

				if (pd < pend) {
					(*pd++) = ARITH_FUN(*ps, a0, b0);
					ps++;

					if (pd < pend) {
						(*pd++) =
						    ARITH_FUN(*ps, a1, b1);
						ps++;

						if (pd < pend) {
							(*pd++) =
							    ARITH_FUN(*ps, a2,
							    b2);
							ps++;
						}
					}
				}
			}
		}

		src += slb;
		dst += dlb;
	}
}

/* *********************************************************** */

void
mlib_v_ImageScale_Fp_F32_S32(
    mlib_u8 *dst,
    const mlib_u8 *src,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 nchan,
    mlib_s32 slb,
    mlib_s32 dlb,
    const mlib_d64 *alpha,
    const mlib_d64 *beta)
{
	mlib_f32 *ps;
	mlib_s32 *pd;
	mlib_d64 a0, a1, a2, a3;
	mlib_d64 b0, b1, b2, b3;
	mlib_s32 i, j;

/* prepare the alpha and beta values */
	a0 = alpha[0];
	b0 = beta[0];
	a1 = alpha[1 % nchan];
	b1 = beta[1 % nchan];
	a2 = alpha[2 % nchan];
	b2 = beta[2 % nchan];
	a3 = alpha[3 % nchan];
	b3 = beta[3 % nchan];

	xsize *= nchan;

	if (nchan != 3) {
		for (j = 0; j < ysize; j++) {
			ps = (mlib_f32 *)src;
			pd = (mlib_s32 *)dst;

#pragma pipeloop(0)
			for (i = 0; i <= xsize - 4; i += 4) {
				FLOAT2INT_CLAMP(pd[i], ps[i] * a0 + b0);
				FLOAT2INT_CLAMP(pd[i + 1], ps[i + 1] * a1 + b1);
				FLOAT2INT_CLAMP(pd[i + 2], ps[i + 2] * a2 + b2);
				FLOAT2INT_CLAMP(pd[i + 3], ps[i + 3] * a3 + b3);
			}

			if (i < xsize) {
				FLOAT2INT_CLAMP(pd[i], ps[i] * a0 + b0);
				i++;
			}

			if (i < xsize) {
				FLOAT2INT_CLAMP(pd[i], ps[i] * a1 + b1);
				i++;
			}

			if (i < xsize) {
				FLOAT2INT_CLAMP(pd[i], ps[i] * a2 + b2);
				i++;
			}

			src += slb;
			dst += dlb;
		}

	} else {
		for (j = 0; j < ysize; j++) {
			ps = (mlib_f32 *)src;
			pd = (mlib_s32 *)dst;

#pragma pipeloop(0)
			for (i = 0; i < xsize; i += 3) {
				FLOAT2INT_CLAMP(pd[i], ps[i] * a0 + b0);
				FLOAT2INT_CLAMP(pd[i + 1], ps[i + 1] * a1 + b1);
				FLOAT2INT_CLAMP(pd[i + 2], ps[i + 2] * a2 + b2);
			}

			src += slb;
			dst += dlb;
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageScale_Fp_F32_D64(
    mlib_u8 *dst,
    const mlib_u8 *src,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 nchan,
    mlib_s32 slb,
    mlib_s32 dlb,
    const mlib_d64 *alpha,
    const mlib_d64 *beta)
{
	mlib_f32 *ps;
	mlib_d64 *pd;
	mlib_d64 a0, a1, a2, a3;
	mlib_d64 b0, b1, b2, b3;
	mlib_s32 i, j;

/* prepare the alpha and beta values */
	a0 = alpha[0];
	b0 = beta[0];
	a1 = alpha[1 % nchan];
	b1 = beta[1 % nchan];
	a2 = alpha[2 % nchan];
	b2 = beta[2 % nchan];
	a3 = alpha[3 % nchan];
	b3 = beta[3 % nchan];

	xsize *= nchan;

	if (nchan != 3) {
		for (j = 0; j < ysize; j++) {
			ps = (mlib_f32 *)src;
			pd = (mlib_d64 *)dst;

#pragma pipeloop(0)
			for (i = 0; i <= xsize - 4; i += 4) {
				pd[i] = ps[i] * a0 + b0;
				pd[i + 1] = ps[i + 1] * a1 + b1;
				pd[i + 2] = ps[i + 2] * a2 + b2;
				pd[i + 3] = ps[i + 3] * a3 + b3;
			}

			if (i < xsize) {
				pd[i] = ps[i] * a0 + b0;
				i++;
			}

			if (i < xsize) {
				pd[i] = ps[i] * a1 + b1;
				i++;
			}

			if (i < xsize) {
				pd[i] = ps[i] * a2 + b2;
				i++;
			}

			src += slb;
			dst += dlb;
		}

	} else {
		for (j = 0; j < ysize; j++) {
			ps = (mlib_f32 *)src;
			pd = (mlib_d64 *)dst;

#pragma pipeloop(0)
			for (i = 0; i < xsize; i += 3) {
				pd[i] = ps[i] * a0 + b0;
				pd[i + 1] = ps[i + 1] * a1 + b1;
				pd[i + 2] = ps[i + 2] * a2 + b2;
			}

			src += slb;
			dst += dlb;
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageScale_Fp_D64_S32(
    mlib_u8 *dst,
    const mlib_u8 *src,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 nchan,
    mlib_s32 slb,
    mlib_s32 dlb,
    const mlib_d64 *alpha,
    const mlib_d64 *beta)
{
	mlib_d64 *ps;
	mlib_s32 *pd;
	mlib_d64 a0, a1, a2, a3;
	mlib_d64 b0, b1, b2, b3;
	mlib_s32 i, j;

/* prepare the alpha and beta values */
	a0 = alpha[0];
	b0 = beta[0];
	a1 = alpha[1 % nchan];
	b1 = beta[1 % nchan];
	a2 = alpha[2 % nchan];
	b2 = beta[2 % nchan];
	a3 = alpha[3 % nchan];
	b3 = beta[3 % nchan];

	xsize *= nchan;

	if (nchan != 3) {
		for (j = 0; j < ysize; j++) {
			ps = (mlib_d64 *)src;
			pd = (mlib_s32 *)dst;

#pragma pipeloop(0)
			for (i = 0; i <= xsize - 4; i += 4) {
				FLOAT2INT_CLAMP(pd[i], ps[i] * a0 + b0);
				FLOAT2INT_CLAMP(pd[i + 1], ps[i + 1] * a1 + b1);
				FLOAT2INT_CLAMP(pd[i + 2], ps[i + 2] * a2 + b2);
				FLOAT2INT_CLAMP(pd[i + 3], ps[i + 3] * a3 + b3);
			}

			if (i < xsize) {
				FLOAT2INT_CLAMP(pd[i], ps[i] * a0 + b0);
				i++;
			}

			if (i < xsize) {
				FLOAT2INT_CLAMP(pd[i], ps[i] * a1 + b1);
				i++;
			}

			if (i < xsize) {
				FLOAT2INT_CLAMP(pd[i], ps[i] * a2 + b2);
				i++;
			}

			src += slb;
			dst += dlb;
		}

	} else {
		for (j = 0; j < ysize; j++) {
			ps = (mlib_d64 *)src;
			pd = (mlib_s32 *)dst;

#pragma pipeloop(0)
			for (i = 0; i < xsize; i += 3) {
				FLOAT2INT_CLAMP(pd[i], ps[i] * a0 + b0);
				FLOAT2INT_CLAMP(pd[i + 1], ps[i + 1] * a1 + b1);
				FLOAT2INT_CLAMP(pd[i + 2], ps[i + 2] * a2 + b2);
			}

			src += slb;
			dst += dlb;
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageScale_Fp_D64_F32(
    mlib_u8 *dst,
    const mlib_u8 *src,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 nchan,
    mlib_s32 slb,
    mlib_s32 dlb,
    const mlib_d64 *alpha,
    const mlib_d64 *beta)
{
	mlib_d64 *ps;
	mlib_f32 *pd;
	mlib_d64 a0, a1, a2, a3;
	mlib_d64 b0, b1, b2, b3;
	mlib_s32 i, j;

/* prepare the alpha and beta values */
	a0 = alpha[0];
	b0 = beta[0];
	a1 = alpha[1 % nchan];
	b1 = beta[1 % nchan];
	a2 = alpha[2 % nchan];
	b2 = beta[2 % nchan];
	a3 = alpha[3 % nchan];
	b3 = beta[3 % nchan];

	xsize *= nchan;

	if (nchan != 3) {
		for (j = 0; j < ysize; j++) {
			ps = (mlib_d64 *)src;
			pd = (mlib_f32 *)dst;

#pragma pipeloop(0)
			for (i = 0; i <= xsize - 4; i += 4) {
				pd[i] = ps[i] * a0 + b0;
				pd[i + 1] = ps[i + 1] * a1 + b1;
				pd[i + 2] = ps[i + 2] * a2 + b2;
				pd[i + 3] = ps[i + 3] * a3 + b3;
			}

			if (i < xsize) {
				pd[i] = ps[i] * a0 + b0;
				i++;
			}

			if (i < xsize) {
				pd[i] = ps[i] * a1 + b1;
				i++;
			}

			if (i < xsize) {
				pd[i] = ps[i] * a2 + b2;
				i++;
			}

			src += slb;
			dst += dlb;
		}

	} else {
		for (j = 0; j < ysize; j++) {
			ps = (mlib_d64 *)src;
			pd = (mlib_f32 *)dst;

#pragma pipeloop(0)
			for (i = 0; i < xsize; i += 3) {
				pd[i] = ps[i] * a0 + b0;
				pd[i + 1] = ps[i + 1] * a1 + b1;
				pd[i + 2] = ps[i + 2] * a2 + b2;
			}

			src += slb;
			dst += dlb;
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageScale_Fp_D64_D64(
    mlib_u8 *dst,
    const mlib_u8 *src,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 nchan,
    mlib_s32 slb,
    mlib_s32 dlb,
    const mlib_d64 *alpha,
    const mlib_d64 *beta)
{
	mlib_d64 *ps;
	mlib_d64 *pd;
	mlib_d64 a0, a1, a2, a3;
	mlib_d64 b0, b1, b2, b3;
	mlib_s32 i, j;

/* prepare the alpha and beta values */
	a0 = alpha[0];
	b0 = beta[0];
	a1 = alpha[1 % nchan];
	b1 = beta[1 % nchan];
	a2 = alpha[2 % nchan];
	b2 = beta[2 % nchan];
	a3 = alpha[3 % nchan];
	b3 = beta[3 % nchan];

	xsize *= nchan;

	if (nchan != 3) {
		for (j = 0; j < ysize; j++) {
			ps = (mlib_d64 *)src;
			pd = (mlib_d64 *)dst;

#pragma pipeloop(0)
			for (i = 0; i <= xsize - 4; i += 4) {
				pd[i] = ps[i] * a0 + b0;
				pd[i + 1] = ps[i + 1] * a1 + b1;
				pd[i + 2] = ps[i + 2] * a2 + b2;
				pd[i + 3] = ps[i + 3] * a3 + b3;
			}

			if (i < xsize) {
				pd[i] = ps[i] * a0 + b0;
				i++;
			}

			if (i < xsize) {
				pd[i] = ps[i] * a1 + b1;
				i++;
			}

			if (i < xsize) {
				pd[i] = ps[i] * a2 + b2;
				i++;
			}

			src += slb;
			dst += dlb;
		}

	} else {
		for (j = 0; j < ysize; j++) {
			ps = (mlib_d64 *)src;
			pd = (mlib_d64 *)dst;

#pragma pipeloop(0)
			for (i = 0; i < xsize; i += 3) {
				pd[i] = ps[i] * a0 + b0;
				pd[i + 1] = ps[i + 1] * a1 + b1;
				pd[i + 2] = ps[i + 2] * a2 + b2;
			}

			src += slb;
			dst += dlb;
		}
	}
}

/* *********************************************************** */
