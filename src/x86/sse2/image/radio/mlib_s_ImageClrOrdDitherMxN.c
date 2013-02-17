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
 * Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident   "@(#)mlib_s_ImageClrOrdDitherMxN.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageColorOrderedDitherMxN - convert a true-color/grayscale image
 *                                        to an indexed-color/black-white image
 *                                        by ordered dithering
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageColorOrderedDitherMxN(mlib_image       *dst,
 *                                                  const mlib_image *src,
 *                                                  const mlib_s32   **dmask,
 *                                                  mlib_s32         m,
 *                                                  mlib_s32         n,
 *                                                  mlib_s32         scale,
 *                                                  const void       *colormap)
 *
 * ARGUMENTS
 *      dst       Pointer to destination image.
 *      src       Pointer to source image.
 *      dmask     Pointer to the dither masks, one per channel, in row major
 *                order.
 *      m         Mask width (m > 1).
 *      n         Mask height (n > 1).
 *      scale     The scaling factor for dmask to convert the input
 *                integer coefficients into floating-point coefficients:
 *                floating-point coefficient = integer coefficient * 2^(-scale)
 *      colormap  Internal data structure for image dithering.
 *
 * DESCRIPTION
 *      Convert a 3 or 4-channel image to a 1-channel indexed image, or convert
 *      a 1-channel grayscale image to a 1-channel MLIB_BIT image, with the
 *      method of ordered dithering.
 *
 * RESTRICTIONS
 *      If src is a 1-channel MLIB_BYTE image
 *      then dst must be a 1-channel MLIB_BIT
 *      image. In other cases src must be an MLIB_BYTE or MLIB_SHORT image
 *      with 3 or 4 channels and dst must be a 1-channel MLIB_BYTE or
 *      MLIB_SHORT image.
 *
 *      The colormap must be created by
 *      mlib_ImageColorTrue2IndexInit with colorcube
 *      included (bits != 0) or by mlib_ImageColorDitherInit()
 *      with dimensions != NULL.
 *
 *      0 <= dmask[i][j] < 2^scale;
 *      scale > 0
 */

#include <mlib_image.h>
#include <mlib_SysMath.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageColormap.h>
#include <mlib_ImageColorOrderedDitherMxN.h>
#ifdef  __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif
/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageColorOrderedDitherMxN = \
	__mlib_ImageColorOrderedDitherMxN

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageColorOrderedDitherMxN) mlib_ImageColorOrderedDitherMxN
    __attribute__((weak, alias("__mlib_ImageColorOrderedDitherMxN")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	FUNC(SUFF)	mlib_ImageColorOrderedDither##SUFF

/* *********************************************************** */

typedef void (*line_func_type) (
    void *src,
    void *dst,
    mlib_s32 length,
    const void *state);

static line_func_type line_func_arr[] = {
	(line_func_type) mlib_ImageColorTrue2IndexLine_U8_U8_3,
	(line_func_type) mlib_ImageColorTrue2IndexLine_U8_U8_4,
	(line_func_type) mlib_ImageColorTrue2IndexLine_U8_S16_3,
	(line_func_type) mlib_ImageColorTrue2IndexLine_U8_S16_4,
	(line_func_type) mlib_ImageColorTrue2IndexLine_S16_U8_3,
	(line_func_type) mlib_ImageColorTrue2IndexLine_S16_U8_4,
	(line_func_type) mlib_ImageColorTrue2IndexLine_S16_S16_3,
	(line_func_type) mlib_ImageColorTrue2IndexLine_S16_S16_4
};

/* *********************************************************** */

#define	GET_STEPS                                     \
	switch (mlib_ImageGetMethod(colormap)) {          \
	case LUT_COLOR_CUBE_SEARCH:                       \
	    off = (stype == MLIB_BYTE) ? 8 : 16;          \
	    step0 = step1 = step2 = step3 =               \
		1 << (off - mlib_ImageGetBits(colormap));     \
	    break;                                        \
	case LUT_COLOR_DIMENSIONS:                        \
	    p_dim =                                       \
			(mlib_s32 *)mlib_ImageGetLutInversTable(  \
			colormap);                                \
	    off = nchan * (256 / sizeof (mlib_s32));      \
	    if (dtype == MLIB_SHORT)                      \
		off *= 2;                                     \
	    if (stype == MLIB_SHORT)                      \
		off *= 4;                                     \
	    p_dim += off;                                 \
	    srange = (stype == MLIB_BYTE) ?               \
				255 : ((1 << 16) - 1);                \
	    step0 = (p_dim[0] > 1) ?                      \
				srange / (p_dim[0] - 1) : 0;          \
	    step1 = (p_dim[1] > 1) ?                      \
				srange / (p_dim[1] - 1) : 0;          \
	    step2 = (p_dim[2] > 1) ?                      \
				srange / (p_dim[2] - 1) : 0;          \
	    if (nchan == 4) {                             \
		step3 = (p_dim[3] > 1) ?                      \
				srange / (p_dim[3] - 1) : 0;          \
	    }                                             \
	    break;                                        \
	default:                                          \
	    return (MLIB_FAILURE);                        \
	}                                                 \
	                                                  \
	dscale = 1.0;                                     \
	while (scale > 30) {                              \
	    dscale *= 1.0 / (1 << 30);                    \
	    scale -= 30;                                  \
	}                                                 \
	                                                  \
	dscale /= (1 << scale);                           \
	dscale0 = dscale * step0;                         \
	dscale1 = dscale * step1;                         \
	dscale2 = dscale * step2;                         \
	half_step0 = (step0 - 1) >> 1;                    \
	half_step1 = (step1 - 1) >> 1;                    \
	half_step2 = (step2 - 1) >> 1;                    \
	dmask0 = dmask[0];                                \
	dmask1 = dmask[1];                                \
	dmask2 = dmask[2];                                \
	if (nchan == 4) {                                 \
	    dmask3 = dmask[3];                            \
	    dscale3 = dscale * step3;                     \
	    half_step3 = (step3 - 1) >> 1;                \
	}

/* *********************************************************** */

#define	MAX_KERN	64

/* *********************************************************** */

#define	FILL_KERN(DTYPE)                             \
	{                                                \
	    DTYPE *pkern;                                \
	                                                 \
	    line_size = (mstep * num_blk *               \
					sizeof (DTYPE) + 7) & ~7;        \
	    kern_size = n * line_size;                   \
	    kern = __mlib_malloc(kern_size + 16);        \
	    if (kern == NULL)                            \
			return (MLIB_FAILURE);                   \
		for (j = 0; j < n; j++) {                    \
			for (i = 0; i < m; i++) {                \
				pkern =                              \
					(DTYPE *)(kern +                 \
					j * line_size) + nchan * i;      \
				if (nchan == 3) {                    \
					v0 = half_step0 -                \
						(mlib_s32)(dmask0[j * m +    \
						i] * dscale0);               \
					v1 = half_step1 -                \
						(mlib_s32)(dmask1[j * m +    \
						i] * dscale1);               \
					v2 = half_step2 -                \
						(mlib_s32)(dmask2[j * m +    \
						i] * dscale2);               \
					for (k = 0; k < num_blk; k++) {  \
						pkern[k * mstep] = v0;       \
						pkern[k * mstep + 1] = v1;   \
						pkern[k * mstep + 2] = v2;   \
					}                                \
				} else {                             \
					v0 = half_step0 -                \
						(mlib_s32)(dmask0[j * m +    \
						i] * dscale0);               \
					v1 = half_step1 -                \
						(mlib_s32)(dmask1[j * m +    \
						i] * dscale1);               \
					v2 = half_step2 -                \
						(mlib_s32)(dmask2[j * m +    \
						i] * dscale2);               \
					v3 = half_step3 -                \
						(mlib_s32)(dmask3[j * m +    \
						i] * dscale3);               \
					for (k = 0; k < num_blk; k++) {  \
						pkern[k * mstep] = v0;       \
						pkern[k * mstep + 1] = v1;   \
						pkern[k * mstep + 2] = v2;   \
						pkern[k * mstep + 3] = v3;   \
					}                                \
				}                                    \
			}                                        \
		}                                            \
	}

/* *********************************************************** */

mlib_status
__mlib_ImageColorOrderedDitherMxN(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 **dmask,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 scale,
    const void *colormap)
{
	mlib_type stype, dtype;
	const mlib_s32 *dmask0, *dmask1, *dmask2, *dmask3;
	mlib_u8 *sl, *dl;
	mlib_s32 nchan, dchan, sll, dll, sw, sh, dw, dh, num_blk;
	mlib_s32 off, mstep, line_size, kern_size, dsize, i, j, k, fun_ind;
	__m128i *pbuff, *pb;
	mlib_s32 *p_dim;
	mlib_u8 *kern, *pkern;
	__m128i *dkern;
	__m128i ss, d0, d1;
	__m128i k0, k1;
	mlib_s32 step0, step1, step2, step3;
	mlib_d64 srange, dscale, dscale0, dscale1, dscale2, dscale3;
	mlib_s32 half_step0, half_step1, half_step2, half_step3;
	mlib_s32 v0, v1, v2, v3;
	__m128i _s_zero = _mm_xor_si128(_s_zero, _s_zero);
	line_func_type line_func;

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_SIZE_EQUAL(src, dst);
	MLIB_IMAGE_HAVE_CHAN(dst, 1);
	MLIB_IMAGE_AND_COLORMAP_ARE_COMPAT(src, colormap);

	MLIB_IMAGE_GET_ALL_PARAMS(dst, dtype, dchan, dw, dh, dll, dl);
	MLIB_IMAGE_GET_ALL_PARAMS(src, stype, nchan, sw, sh, sll, sl);

	if (stype == MLIB_BYTE && nchan == 1 && dtype == MLIB_BIT) {
		return mlib_ImageColorOrderedDitherBit_MxN(dst, src, dmask, m,
		    n, scale, colormap);
	}

	if (!(stype == MLIB_BYTE || stype == MLIB_SHORT)) {
		return (MLIB_FAILURE);
	}

	if (!(dtype == MLIB_BYTE || dtype == MLIB_SHORT)) {
		return (MLIB_FAILURE);
	}

	if (!(nchan >= 3 && nchan <= 4)) {
		return (MLIB_FAILURE);
	}

	if (dmask == NULL || colormap == NULL) {
		return (MLIB_NULLPOINTER);
	}

	if (scale <= 0) {
		return (MLIB_OUTOFRANGE);
	}

	fun_ind = nchan - 3;

	if (dtype == MLIB_SHORT)
		fun_ind += 2;
	if (stype == MLIB_SHORT)
		fun_ind += 4;
	line_func = line_func_arr[fun_ind];

	num_blk = (sw + (m - 1)) / m;
	mstep = m * nchan;

	GET_STEPS;

	if (stype == MLIB_BYTE) {
		FILL_KERN(mlib_s16);
		dsize = (nchan * sw + 15) / 16;
	} else {
		FILL_KERN(mlib_s32);
		dsize = (nchan * sw + 7) / 8;
	}

	pbuff = __mlib_malloc(dsize * sizeof (__m128i));

	if (pbuff == NULL) {
		__mlib_free(kern);
		return (MLIB_FAILURE);
	}

	pkern = kern;

#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
	for (j = 0; j < sh; j++) {
		dkern = (__m128i *)pkern;

		__m128i *sp = (__m128i *)sl;
		pb = pbuff;

		if (stype == MLIB_BYTE) {
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
			for (i = 0; i < dsize; i++) {
				ss = _mm_loadu_si128(sp);
				sp++;
				k0 = _mm_loadu_si128(dkern);
				dkern++;
				k1 = _mm_loadu_si128(dkern);
				dkern++;
				d0 = _mm_unpacklo_epi8(ss, _s_zero);
				d1 = _mm_unpackhi_epi8(ss, _s_zero);
				d0 = _mm_add_epi16(d0, k0);
				d1 = _mm_add_epi16(d1, k1);
				d1 = _mm_packus_epi16(d0, d1);
				_mm_storeu_si128(pb, d1);
				pb++;
			}

		} else {
#ifdef  __SUNPRO_C
#pragma pipeloop(0)
#endif
			for (i = 0; i < dsize; i++) {
				ss = _mm_loadu_si128(sp);
				sp++;
				k0 = _mm_loadu_si128(dkern);
				dkern++;
				k1 = _mm_loadu_si128(dkern);
				dkern++;
				d0 = _mm_srai_epi32(
					_mm_unpacklo_epi16(_s_zero, ss), 16);
				d1 = _mm_srai_epi32(
					_mm_unpackhi_epi16(_s_zero, ss), 16);
				d0 = _mm_add_epi32(d0, k0);
				d1 = _mm_add_epi32(d1, k1);
				d1 = _mm_packs_epi32(d0, d1);
				_mm_storeu_si128(pb, d1);
				pb++;
			}
		}

		pkern += line_size;

		if (pkern >= kern + kern_size)
			pkern = kern;

		line_func(pbuff, dl, sw, colormap);

		sl += sll;
		dl += dll;
	}

	__mlib_free(pbuff);
	__mlib_free(kern);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
