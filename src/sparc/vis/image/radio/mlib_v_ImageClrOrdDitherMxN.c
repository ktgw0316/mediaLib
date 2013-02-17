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

#pragma ident	"@(#)mlib_v_ImageClrOrdDitherMxN.c	9.2	07/11/05 SMI"

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

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_SysMath.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageColormap.h>
#include <mlib_ImageColorOrderedDitherMxN.h>

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

#define	LOAD_NA(ss, sp)                                         \
	{                                                       \
	    mlib_d64 *pp = vis_alignaddr(sp, 0);                \
	                                                        \
	    ss = vis_faligndata(pp[0], pp[1]);                  \
	}

/* *********************************************************** */

#define	LOAD_NA_NF(ss, sp)                                      \
	{                                                       \
	    mlib_d64 *pp = vis_alignaddr(sp, 0);                \
	                                                        \
	    ss = vis_faligndata(vis_ld_d64_nf(pp),              \
		vis_ld_d64_nf(pp + 1));                         \
	}

/* *********************************************************** */

#define	EXPAND8(x)	vis_fpmerge(fzeros, x)
#define	EXPAND16(x)	vis_fmuld8ulx16(f_two, x)

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

#define	GET_STEPS                                                        \
	switch (mlib_ImageGetMethod(colormap)) {                         \
	case LUT_COLOR_CUBE_SEARCH:                                      \
	    off = (stype == MLIB_BYTE) ? 8 : 16;                         \
	    step0 = step1 = step2 = step3 =                              \
		1 << (off - mlib_ImageGetBits(colormap));                \
	    break;                                                       \
	case LUT_COLOR_DIMENSIONS:                                       \
	    p_dim = (mlib_s32 *)mlib_ImageGetLutInversTable(colormap);   \
	    off = nchan * (256 / sizeof (mlib_s32));                     \
	    if (dtype == MLIB_SHORT)                                     \
		off *= 2;                                                \
	    if (stype == MLIB_SHORT)                                     \
		off *= 4;                                                \
	    p_dim += off;                                                \
	    srange = (stype == MLIB_BYTE) ? 255 : ((1 << 16) - 1);       \
	    step0 = (p_dim[0] > 1) ? srange / (p_dim[0] - 1) : 0;        \
	    step1 = (p_dim[1] > 1) ? srange / (p_dim[1] - 1) : 0;        \
	    step2 = (p_dim[2] > 1) ? srange / (p_dim[2] - 1) : 0;        \
	    if (nchan == 4) {                                            \
		step3 = (p_dim[3] > 1) ? srange / (p_dim[3] - 1) : 0;    \
	    }                                                            \
	    break;                                                       \
	default:                                                         \
	    return (MLIB_FAILURE);                                       \
	}                                                                \
	                                                                 \
	dscale = 1.0;                                                    \
	while (scale > 30) {                                             \
	    dscale *= 1.0 / (1 << 30);                                   \
	    scale -= 30;                                                 \
	}                                                                \
	                                                                 \
	dscale /= (1 << scale);                                          \
	dscale0 = dscale * step0;                                        \
	dscale1 = dscale * step1;                                        \
	dscale2 = dscale * step2;                                        \
	half_step0 = (step0 - 1) >> 1;                                   \
	half_step1 = (step1 - 1) >> 1;                                   \
	half_step2 = (step2 - 1) >> 1;                                   \
	dmask0 = dmask[0];                                               \
	dmask1 = dmask[1];                                               \
	dmask2 = dmask[2];                                               \
	if (nchan == 4) {                                                \
	    dmask3 = dmask[3];                                           \
	    dscale3 = dscale * step3;                                    \
	    half_step3 = (step3 - 1) >> 1;                               \
	}

/* *********************************************************** */

#define	MAX_KERN	64

/* *********************************************************** */

#define	FILL_KERN(DTYPE, SCALE)                                         \
	{                                                               \
	    DTYPE *pkern;                                               \
	                                                                \
	    line_size = (mstep * num_blk * sizeof (DTYPE) + 7) & ~7;    \
	    kern_size = n * line_size;                                  \
	    kern = __mlib_malloc(kern_size + 16);                       \
	    if (kern == NULL)                                           \
		return (MLIB_FAILURE);                                  \
	    for (j = 0; j < n; j++) {                                   \
		for (i = 0; i < m; i++) {                               \
		    pkern =                                             \
			(DTYPE *) (kern + j * line_size) + nchan * i;   \
		    if (nchan == 3) {                                   \
			v0 = half_step0 - (mlib_s32)(dmask0[j * m +     \
			    i] * dscale0);                              \
			v1 = half_step1 - (mlib_s32)(dmask1[j * m +     \
			    i] * dscale1);                              \
			v2 = half_step2 - (mlib_s32)(dmask2[j * m +     \
			    i] * dscale2);                              \
			v0 *= SCALE;                                    \
			v1 *= SCALE;                                    \
			v2 *= SCALE;                                    \
			for (k = 0; k < num_blk; k++) {                 \
			    pkern[k * mstep] = v0;                      \
			    pkern[k * mstep + 1] = v1;                  \
			    pkern[k * mstep + 2] = v2;                  \
			}                                               \
		    } else {                                            \
			v0 = half_step0 - (mlib_s32)(dmask0[j * m +     \
			    i] * dscale0);                              \
			v1 = half_step1 - (mlib_s32)(dmask1[j * m +     \
			    i] * dscale1);                              \
			v2 = half_step2 - (mlib_s32)(dmask2[j * m +     \
			    i] * dscale2);                              \
			v3 = half_step3 - (mlib_s32)(dmask3[j * m +     \
			    i] * dscale3);                              \
			v0 *= SCALE;                                    \
			v1 *= SCALE;                                    \
			v2 *= SCALE;                                    \
			v3 *= SCALE;                                    \
			for (k = 0; k < num_blk; k++) {                 \
			    pkern[k * mstep] = v0;                      \
			    pkern[k * mstep + 1] = v1;                  \
			    pkern[k * mstep + 2] = v2;                  \
			    pkern[k * mstep + 3] = v3;                  \
			}                                               \
		    }                                                   \
		}                                                       \
	    }                                                           \
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
	mlib_d64 *pbuff;
	mlib_s32 *p_dim;
	mlib_u8 *kern, *pkern;
	mlib_d64 *dkern;
	mlib_d64 ss, d0, d1;
	mlib_s32 step0, step1, step2, step3;
	mlib_d64 srange, dscale, dscale0, dscale1, dscale2, dscale3;
	mlib_s32 half_step0, half_step1, half_step2, half_step3;
	mlib_s32 v0, v1, v2, v3;
	mlib_f32 fzeros = vis_fzeros();
	mlib_f32 f_two = vis_to_float(0x20002);
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
		FILL_KERN(mlib_s16,
		    1);

		dsize = (nchan * sw + 7) / 8;
		vis_write_gsr(7 << 3);
	} else {
		FILL_KERN(mlib_s32,
		    2);

		dsize = (nchan * sw + 3) / 4;
		vis_write_gsr(15 << 3);
	}

	pbuff = __mlib_malloc(dsize * sizeof (mlib_d64));

	if (pbuff == NULL) {
		__mlib_free(kern);
		return (MLIB_FAILURE);
	}

	pkern = kern;

	for (j = 0; j < sh; j++) {
		dkern = (mlib_d64 *)pkern;

		if ((mlib_s32)sl & 7) {
			mlib_u8 *sp = sl;

			if (stype == MLIB_BYTE) {
#pragma pipeloop(0)
				for (i = 0; i < dsize; i++) {
					LOAD_NA_NF(ss, sp);
					d0 = vis_fpadd16(EXPAND8(vis_read_hi
					    (ss)), dkern[2 * i]);
					d1 = vis_fpadd16(EXPAND8(vis_read_lo
					    (ss)), dkern[2 * i + 1]);
					pbuff[i] = vis_fpack16_pair(d0, d1);
					sp += 8;
				}

			} else {
#pragma pipeloop(0)
				for (i = 0; i < dsize; i++) {
					LOAD_NA_NF(ss, sp);
					d0 = vis_fpadd32(EXPAND16(vis_read_hi
					    (ss)), dkern[2 * i]);
					d1 = vis_fpadd32(EXPAND16(vis_read_lo
					    (ss)), dkern[2 * i + 1]);
					pbuff[i] = vis_fpackfix_pair(d0, d1);
					sp += 8;
				}
			}

		} else {
			mlib_d64 *sp = (mlib_d64 *)sl;

			if (stype == MLIB_BYTE) {
#pragma pipeloop(0)
				for (i = 0; i < dsize; i++) {
					ss = sp[i];
					d0 = vis_fpadd16(EXPAND8(vis_read_hi
					    (ss)), dkern[2 * i]);
					d1 = vis_fpadd16(EXPAND8(vis_read_lo
					    (ss)), dkern[2 * i + 1]);
					pbuff[i] = vis_fpack16_pair(d0, d1);
				}

			} else {
#pragma pipeloop(0)
				for (i = 0; i < dsize; i++) {
					ss = sp[i];
					d0 = vis_fpadd32(EXPAND16(vis_read_hi
					    (ss)), dkern[2 * i]);
					d1 = vis_fpadd32(EXPAND16(vis_read_lo
					    (ss)), dkern[2 * i + 1]);
					pbuff[i] = vis_fpackfix_pair(d0, d1);
				}
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
