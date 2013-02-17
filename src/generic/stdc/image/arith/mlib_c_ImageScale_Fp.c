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

#pragma ident	"@(#)mlib_c_ImageScale_Fp.c	9.2	07/10/09 SMI"

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

#define	ARITH_FUN(x, a, b)	((x)*(a)+(b))

/* *********************************************************** */

typedef union
{
	mlib_d64 d64;
	struct
	{
		mlib_f32 f0;
		mlib_f32 f1;
	} f32s;
	struct
	{
		mlib_s32 f0;
		mlib_s32 f1;
	} i32s;
} d64_2x32;

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

#define	FLOAT2INT_CLAMP(X)	((mlib_s32)(X))

#else /* MLIB_USE_FTOI_CLAMPING */

static mlib_s32
mlib_c_clamp_d64_s32(
    mlib_d64 x)
{
	if (x >= MLIB_S32_MAX)
		return (MLIB_S32_MAX);

	if (x <= MLIB_S32_MIN)
		return (MLIB_S32_MIN);
	return ((mlib_s32)x);
}

/* *********************************************************** */

#define	FLOAT2INT_CLAMP(X)	mlib_c_clamp_d64_s32((mlib_d64) (X))
#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#define	PREPARE_AB_u8                                           \
	for (k = 0; k < nchan; k++) {                           \
	    da[k] = alpha[k] * (mlib_d64)(1 << 24);             \
	    db[k] = (beta[k] - 128.0) * (mlib_d64)(1 << 24);    \
	}                                                       \
	                                                        \
	alpha = da;                                             \
	beta = db

/* *********************************************************** */

#define	LAST_CONV_u8(x)	((FLOAT2INT_CLAMP(x) >> 24) + 128)

/* *********************************************************** */

#define	PREPARE_AB_s16                                          \
	for (k = 0; k < nchan; k++) {                           \
	    da[k] = alpha[k] * 65536.0;                         \
	    db[k] = beta[k] * 65536.0;                          \
	}                                                       \
	                                                        \
	alpha = da;                                             \
	beta = db

/* *********************************************************** */

#define	LAST_CONV_s16(x)	(FLOAT2INT_CLAMP(x) >> 16)

/* *********************************************************** */

#define	PREPARE_AB_u16                                          \
	for (k = 0; k < nchan; k++) {                           \
	    da[k] = alpha[k] * 65536.0;                         \
	    db[k] = (beta[k] - 32768) * 65536.0;                \
	}                                                       \
	                                                        \
	alpha = da;                                             \
	beta = db

/* *********************************************************** */

#define	LAST_CONV_u16(x)	((FLOAT2INT_CLAMP(x) >> 16) ^ 0x8000)

/* *********************************************************** */

#define	PREPARE_AB_s32

#define	LAST_CONV_s32(x)	(FLOAT2INT_CLAMP(x))

#define	PREPARE_AB_f32

#define	LAST_CONV_f32(x)	((mlib_f32)(x))

#define	PREPARE_AB_d64

#define	LAST_CONV_d64(x)	((mlib_d64)(x))

/* *********************************************************** */

#define	SCALEHEAD(srctype, dsttype)                             \
	PREPARE_AB_##dsttype;                                   \
	for (j = 0; j < ysize; j++) {                           \
	    for (k = 0; k < nchan; k++) {                       \
		mlib_d64 alp = alpha[k];                        \
		mlib_d64 bet = beta[k]

/* *********************************************************** */

#define	SCALE(srctype, dsttype)                                         \
		for (i = 0; i < xsize; i++) {                           \
			v = ((mlib_##srctype *) psrc)[i * nchan + k];   \
			v = v * alp + bet;                              \
			((mlib_##dsttype *) pdst)[i * nchan + k] =      \
				LAST_CONV_##dsttype(v);                 \
		}                                                       \
	    }                                                           \
	    psrc += slb;                                                \
	    pdst += dlb;                                                \
	}                                                               \
	                                                                \
	return MLIB_SUCCESS

/* *********************************************************** */

static void mlib_ImageScale_Fp_F32_F32(
    mlib_u8 *dst,
    mlib_u8 *src,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 nchan,
    mlib_s32 slb,
    mlib_s32 dlb,
    const mlib_d64 *alpha,
    const mlib_d64 *beta);

/* *********************************************************** */

mlib_status
__mlib_ImageScale_Fp(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_d64 *alpha,
    const mlib_d64 *beta)
{
	mlib_type stype, dtype;
	mlib_s32 slb, dlb, xsize, ysize, nchan;
	mlib_u8 *psrc, *pdst;
	mlib_d64 v;
	mlib_d64 da[4], db[4];
	mlib_s32 i, j, k;

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_SIZE_EQUAL(dst, src);
	MLIB_IMAGE_CHAN_EQUAL(dst, src);

	MLIB_IMAGE_GET_ALL_PARAMS(dst, dtype, nchan, xsize, ysize, dlb, pdst);
	stype = mlib_ImageGetType(src);
	slb = mlib_ImageGetStride(src);
	psrc = mlib_ImageGetData(src);

	if (stype == MLIB_FLOAT) {

		if (dtype == MLIB_BYTE) {

			SCALEHEAD(f32, u8);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			SCALE(f32, u8);
		} else if (dtype == MLIB_SHORT) {

			SCALEHEAD(f32, s16);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			SCALE(f32, s16);
		} else if (dtype == MLIB_USHORT) {

			SCALEHEAD(f32, u16);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			SCALE(f32, u16);
		} else if (dtype == MLIB_INT) {

			SCALEHEAD(f32, s32);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			SCALE(f32, s32);
		} else if (dtype == MLIB_FLOAT) {

			mlib_ImageScale_Fp_F32_F32(pdst, psrc,
			    xsize, ysize, nchan, slb, dlb, alpha, beta);
			return (MLIB_SUCCESS);
		} else if (dtype == MLIB_DOUBLE) {

			SCALEHEAD(f32, d64);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			SCALE(f32, d64);
		}
	} else if (stype == MLIB_DOUBLE) {

		if (dtype == MLIB_BYTE) {

			SCALEHEAD(d64, u8);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			SCALE(d64, u8);
		} else if (dtype == MLIB_SHORT) {

			SCALEHEAD(d64, s16);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			SCALE(d64, s16);
		} else if (dtype == MLIB_USHORT) {

			SCALEHEAD(d64, u16);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			SCALE(d64, u16);
		} else if (dtype == MLIB_INT) {

			SCALEHEAD(d64, s32);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			SCALE(d64, s32);
		} else if (dtype == MLIB_FLOAT) {

			SCALEHEAD(d64, f32);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			SCALE(d64, f32);
		} else if (dtype == MLIB_DOUBLE) {

			SCALEHEAD(d64, d64);
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			SCALE(d64, d64);
		}
	}

	return (MLIB_FAILURE);
}

/* *********************************************************** */

void
mlib_ImageScale_Fp_F32_F32(
    mlib_u8 *dst,
    mlib_u8 *src,
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
		alpha2[k] = (mlib_f32)alpha[k % nchan];
		beta2[k] = (mlib_f32)beta[k % nchan];
	}

	for (j = 0; j < ysize; j++) {
		ps = (mlib_f32 *)src;
		pd = (mlib_f32 *)dst;
		pend = pd + xsize * nchan;

		if ((mlib_addr)pd & 7) {
			(*pd++) = ARITH_FUN((*ps++), alpha2[0], beta2[0]);
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
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
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
					(*pd++) = ARITH_FUN((*ps++), a0, b0);

					if (pd < pend) {
						(*pd++) =
						    ARITH_FUN((*ps++), a1, b1);

						if (pd < pend) {
							(*pd++) =
							    ARITH_FUN((*ps++),
							    a2, b2);

							if (pd < pend) {
							    (*pd++) =
								ARITH_FUN(
								(*ps++), a3,
								b3);

							    if (pd < pend)
								(*pd++) =
								    ARITH_FUN
								    (
								    (*ps++),
								    a4,
								    b4);
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
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
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
					(*pd++) = ARITH_FUN((*ps++), a0, b0);

					if (pd < pend) {
						(*pd++) =
						    ARITH_FUN((*ps++), a1, b1);

						if (pd < pend)
							(*pd++) =
							    ARITH_FUN((*ps++),
							    a2, b2);
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
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
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
					(*pd++) = ARITH_FUN((*ps++), a0, b0);

					if (pd < pend) {
						(*pd++) =
						    ARITH_FUN((*ps++), a1, b1);

						if (pd < pend)
							(*pd++) =
							    ARITH_FUN((*ps++),
							    a2, b2);
					}
				}
			}
		}

		src += slb;
		dst += dlb;
	}
}

/* *********************************************************** */
