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

#pragma ident	"@(#)mlib_v_ImageColorErrDiff3x3.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageColorErrorDiffusion3x3 - convert a true color image to an
 *                                         indexed color image by error
 *                                         diffusion dithering
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageColorErrorDiffusion3x3(mlib_image       *dst,
 *                                                   const mlib_image *src,
 *                                                   const mlib_s32   *kernel,
 *                                                   mlib_s32         scale,
 *                                                   const void       *colormap)
 *
 * ARGUMENTS
 *      dst       Pointer to destination image.
 *      src       Pointer to source image.
 *      kernel    Pointer to the error-distribution kernel, in row major order.
 *      scale     The scaling factor for kernel or to convert the input
 *                integer coefficients into floating-point coefficients:
 *                floating-point coefficient = integer coefficient * 2^(-scale)
 *      colormap  Internal data structure for inverse color mapping.
 *
 * DESCRIPTION
 *      Convert a true color image to a pseudo color image with the
 *      method of error diffusion dithering.
 *
 *      The src can be an MLIB_BYTE or MLIB_SHORT image with 3 or 4 channels.
 *      The dst must be a 1-channel MLIB_BYTE or MLIB_SHORT image.
 *
 *   Dithering kernel 3x3:  ...   ...    ...
 *                          ...   ...    koef[0]
 *                          beta  gamma  delta
 *
 * RESTRICTIONS
 *   (abs(koef[0]) + abs(beta) + abs(gamma) + abs(delta)) * 2^(-scale) < 8192
 *    scale >= 0
 */

#include <stdlib.h>
#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_SysMath.h>
#include <mlib_ImageColormap.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageColorErrorDiffusion3x3 = \
	__mlib_ImageColorErrorDiffusion3x3

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageColorErrorDiffusion3x3) mlib_ImageColorErrorDiffusion3x3
    __attribute__((weak, alias("__mlib_ImageColorErrorDiffusion3x3")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static mlib_status mlib_v_ErrorDiffusion3x3_3_8_8(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *kern,
    mlib_s32 scalef_expon,
    const void *colormap);

static mlib_status mlib_v_ErrorDiffusion3x3_4_8_8(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *kern,
    mlib_s32 scalef_expon,
    const void *colormap);

static mlib_status mlib_v_ErrorDiffusion3x3_3_8_16(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *kern,
    mlib_s32 scalef_expon,
    const void *colormap);

static mlib_status mlib_v_ErrorDiffusion3x3_4_8_16(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *kern,
    mlib_s32 scalef_expon,
    const void *colormap);

/* *********************************************************** */

mlib_status
__mlib_ImageColorErrorDiffusion3x3(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kernel,
    mlib_s32 scale,
    const void *colormap)
{
	mlib_status stat;
	mlib_s32 src_nch;
	mlib_s32 scalef_expon, tmp_scale, kernel_u8[9];
	mlib_s32 i, isum, method;
	mlib_d64 div_scalef, sum, dkernel[9], norm;
	mlib_type src_dtype, dst_dtype;

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_SIZE_EQUAL(src, dst);
	MLIB_IMAGE_HAVE_CHAN(dst, 1);
	MLIB_IMAGE_AND_COLORMAP_ARE_COMPAT(src, colormap);

	if (kernel == NULL || colormap == NULL)
		return (MLIB_NULLPOINTER);

	src_dtype = mlib_ImageGetType(src);
	dst_dtype = mlib_ImageGetType(dst);
	src_nch = mlib_ImageGetChannels(src);
	method = mlib_ImageGetMethod(colormap);

	if (src_dtype == MLIB_SHORT || method == LUT_COLOR_DIMENSIONS) {
		return __mlib_ImageColorErrorDiffusionMxN(dst, src, kernel, 3,
		    3, 1, 1, scale, colormap);
	}

	if (src_dtype != MLIB_BYTE) {
		return (MLIB_FAILURE);
	}

	if (scale < 0)
		return (MLIB_OUTOFRANGE);

	div_scalef = 1.0;
	scalef_expon = scale;
	while (scalef_expon > 30) {
		div_scalef /= (1 << 30);
		scalef_expon -= 30;
	}

	div_scalef /= (1 << scalef_expon);

	for (i = 5; i < 9; i++)
		dkernel[i] = kernel[i] * div_scalef;

	sum = 0;

	for (i = 5; i < 9; i++)
		sum += mlib_fabs(dkernel[i]);

	tmp_scale = mlib_ilogb(sum);

	if (tmp_scale > 13)
		return (MLIB_OUTOFRANGE);

	tmp_scale = 14 - tmp_scale;

	if (tmp_scale > 15)
		tmp_scale = 15;

	norm = (32768 >> (15 - tmp_scale));

	for (i = 5; i < 9; i++) {
		if (dkernel[i] > 0.0)
			kernel_u8[i] = (mlib_s32)(dkernel[i] * norm + 0.5);
		else
			kernel_u8[i] = (mlib_s32)(dkernel[i] * norm - 0.5);
	}

	isum = 0;
	for (i = 5; i < 9; i++)
		isum = abs(kernel_u8[i]);

	if (isum > 32767) {
		for (i = 5; i < 9; i++)
			kernel_u8[i] = (mlib_s32)(dkernel[i] * norm);
	}

	switch (dst_dtype) {
	case MLIB_BYTE:
		switch (src_nch) {
		case 3:
			stat =
			    mlib_v_ErrorDiffusion3x3_3_8_8(src, dst, kernel_u8,
			    tmp_scale, colormap);
			break;
		case 4:
			stat =
			    mlib_v_ErrorDiffusion3x3_4_8_8(src, dst, kernel_u8,
			    tmp_scale, colormap);
			break;
		default:
			stat = MLIB_FAILURE;
			break;
		}

		break;
	case MLIB_SHORT:	/* convert wid from pixels to bytes */
		switch (src_nch) {
		case 3:
			stat =
			    mlib_v_ErrorDiffusion3x3_3_8_16(src, dst, kernel_u8,
			    tmp_scale, colormap);
			break;
		case 4:
			stat =
			    mlib_v_ErrorDiffusion3x3_4_8_16(src, dst, kernel_u8,
			    tmp_scale, colormap);
			break;
		default:
			stat = MLIB_FAILURE;
			break;
		}

		break;
	case MLIB_BIT:
	default:
		stat = MLIB_FAILURE;
		break;
	}

	return (stat);
}

/* *********************************************************** */

#define	GET_SRC_DST_PARAMETERS(src_format, dst_format)          \
	sh = mlib_ImageGetHeight(src);                          \
	sw = mlib_ImageGetWidth(src);                           \
	sll = mlib_ImageGetStride(src) / sizeof (src_format);   \
	dll = mlib_ImageGetStride(dst) / sizeof (dst_format);   \
	adr_src = (src_format *) mlib_ImageGetData(src);        \
	adr_dst = (dst_format *) mlib_ImageGetData(dst);        \
	offset = mlib_ImageGetLutOffset(colormap)

/* *********************************************************** */

#define	LOAD_KERNEL_IN_REGS_U8()                                          \
	k[0] = vis_to_double_dup((kern[5] << 16) | (kern[5] & 0xffff));   \
	k[1] = beta =                                                     \
	vis_to_double_dup((kern[6] << 16) | (kern[6] & 0xffff));          \
	k[2] = gamma =                                                    \
	vis_to_double_dup((kern[7] << 16) | (kern[7] & 0xffff));          \
	k[3] = delta =                                                    \
	vis_to_double_dup((kern[8] << 16) | (kern[8] & 0xffff))

/* *********************************************************** */

#define	LOAD_KERNEL_IN_REGS_IS_INVERS_TABLE_U8()                           \
	alpha = vis_to_double_dup((kern[5] << 16) | (kern[5] & 0xffff));   \
	beta  = vis_to_double_dup((kern[6] << 16) | (kern[6] & 0xffff));   \
	gamma = vis_to_double_dup((kern[7] << 16) | (kern[7] & 0xffff));   \
	delta = vis_to_double_dup((kern[8] << 16) | (kern[8] & 0xffff))

/* *********************************************************** */

#define	PREPARE_FOR_COUNT_INDEX_U8()                            \
	shift0 = 8 - bits;                                      \
	shift1 = (shift0 << 1);                                 \
	shift2 = shift0 + shift1;                               \
	tmp_mask0 = (1 << bits) - 1;                            \
	tmp_mask1 = (tmp_mask0 << bits);                        \
	tmp_mask2 = (tmp_mask1 << bits)

/* *********************************************************** */

#define	COUNT_COLOR_U8()                                        \
	resu1 = vis_fmul8sux16(err, k[0]);                      \
	resl1 = vis_fmul8ulx16(err, k[0]);                      \
	res1 = vis_fpadd16(resu1, resl1);                       \
	res0 = vis_fpadd16(res0, res1);                         \
	color = vis_fpack16(res0)

/* *********************************************************** */

#define	COUNT_COLOR_IS_INVERS_TABLE_U8()                        \
	resu1 = vis_fmul8sux16(err, alpha);                     \
	resl1 = vis_fmul8ulx16(err, alpha);                     \
	res1 = vis_fpadd16(resu1, resl1);                       \
	res0 = vis_fpadd16(res0, res1);                         \
	color = vis_fpack16(res0)

/* *********************************************************** */

#define	COUNT_STORE_ERRORS_U8()                                 \
	tmp_ind = (*da++);                                      \
	tmp_color = lut_table[tmp_ind];                         \
	d0 = vis_fmul8x16al(color, scale_fact[1]);              \
	d1 = vis_fmul8x16al(tmp_color, scale_fact[1]);          \
	(*error++) = err = vis_fpsub16(d1, d0)

/* *********************************************************** */

#define	COUNT_STORE_ERRORS_IS_INVERS_TABLE_U8()                 \
	tmp_color = lut_table[tmp_ind];                         \
	d4 = vis_fmul8x16al(color, fsix);                       \
	d5 = vis_fmul8x16al(tmp_color, fsix);                   \
	(*error_sum++) = err = vis_fpsub16(d5, d4)

/* *********************************************************** */

#define	COUNT_ERRORS_FROM_PREV_ROW_U8()                         \
	resu0 = vis_fmul8sux16(d0, delta);                      \
	resl0 = vis_fmul8ulx16(d0, delta);                      \
	res0 = vis_fpadd16(resu0, resl0);                       \
	resu1 = vis_fmul8sux16(d1, gamma);                      \
	resl1 = vis_fmul8ulx16(d1, gamma);                      \
	res1 = vis_fpadd16(resu1, resl1);                       \
	res0 = vis_fpadd16(res0, res1);                         \
	resu2 = vis_fmul8sux16(d2, beta);                       \
	resl2 = vis_fmul8ulx16(d2, beta);                       \
	res2 = vis_fpadd16(resu2, resl2);                       \
	res0 = vis_fpadd16(res0, res2)

/* *********************************************************** */

#define	COUNT_INDEX_IN_INVERS_TABLE_FOR_3_CHANNELS_U8()         \
	t_color = *((mlib_s32 *)&color);                        \
	tmp_in =                                                \
	    ((t_color >> shift0) & tmp_mask0) +                 \
	    ((t_color >> shift1) & tmp_mask1) +                 \
	    ((t_color >> shift2) & tmp_mask2)

/* *********************************************************** */

#define	COUNT_INDEX_IN_INVERS_TABLE_FOR_4_CHANNELS_U8()         \
	t_color = *((mlib_s32 *)&color);                        \
	tmp_in =                                                \
	    ((t_color >> shift0) & tmp_mask0) +                 \
	    ((t_color >> shift1) & tmp_mask1) +                 \
	    ((t_color >> shift2) & tmp_mask2) +                 \
	    ((t_color >> shift3) & tmp_mask3)

/* *********************************************************** */

#define	ERROR_DIFFUSION_3x3_3_8(dst_format, dst_type)                      \
	{                                                                  \
	    mlib_u8 *sa, *adr_src;                                         \
	                                                                   \
/*                                                                         \
 * pointers for pixel and line of source                                   \
 */                                                                        \
	    dst_format *da, *adr_dst;                                      \
	                                                                   \
/*                                                                         \
 * pointers for pixel and line of destination                              \
 */                                                                        \
	    mlib_f32 *lut_table;                                           \
	                                                                   \
/*                                                                         \
 * pointer to the lut table                                                \
 */                                                                        \
	    mlib_s32 tmp_ind;                                              \
	    mlib_s32 offset;                                               \
	                                                                   \
/*                                                                         \
 * offset in the lut table                                                 \
 */                                                                        \
	    mlib_s32 sll,                                                  \
/*                                                                         \
 * line length (in pixels) of source image                                 \
 */                                                                        \
		    dll,                                                   \
/*                                                                         \
 * line length (in pixels) of destination image                            \
 */                                                                        \
		    sw,                                                    \
/*                                                                         \
 * src width in pixels                                                     \
 */                                                                        \
		    sh;                                                    \
	                                                                   \
/*                                                                         \
 * src height in pixels                                                    \
 */                                                                        \
	    mlib_s32 row, i;                                               \
	                                                                   \
/*                                                                         \
 * indices                                                                 \
 */                                                                        \
	    mlib_d64 *error_buf;                                           \
	                                                                   \
/*                                                                         \
 * pointer to error's saving buffer (never incr'd                          \
 */                                                                        \
	    mlib_d64 *error, *error_sum;                                   \
	                                                                   \
/*                                                                         \
 * incremented pointers to error's saving buffer                           \
 */                                                                        \
	    mlib_d64 d0, d1, d2, d3;                                       \
	    mlib_d64 res0, resu0, resl0, res1, resu1, resl1, res2,         \
		resu2, resl2;                                              \
	    mlib_f32 color, tmp_color, scale_fact[2];                      \
	    mlib_d64 beta, gamma, delta, err, k[4];                        \
	    GET_SRC_DST_PARAMETERS(mlib_u8, dst_format);                   \
	                                                                   \
	    lut_table =                                                    \
		(mlib_f32 *)mlib_ImageGetLutNormalTable(colormap) -        \
		offset;                                                    \
	    error_buf =                                                    \
		(mlib_d64 *)__mlib_malloc((sw + 2) * sizeof (mlib_d64));   \
	    if (error_buf == NULL)                                         \
		return (MLIB_FAILURE);                                     \
	    error_buf[0] = error_buf[sw + 1] = 0.0;                        \
	    scale_fact[1] = vis_to_float(0x8000);                          \
	    scale_fact[0] = vis_to_float(1 << (scalef_expon - 1));         \
	    vis_write_gsr(((16 - scalef_expon) << 3) + 7);                 \
	    LOAD_KERNEL_IN_REGS_U8();                                      \
	    error = error_sum = error_buf + 1;                             \
	    err = 0.0;                                                     \
	    sa = adr_src;                                                  \
	    da = adr_dst;                                                  \
	    for (i = 0; i < sw; i++) {                                     \
		d2 = vis_ld_u8(sa + 2);                                    \
		d3 = vis_faligndata(d2, d3);                               \
		d2 = vis_ld_u8(sa + 1);                                    \
		d3 = vis_faligndata(d2, d3);                               \
		d2 = vis_ld_u8(sa);                                        \
		d3 = vis_faligndata(d2, d3);                               \
		d3 = vis_faligndata(d2, d3);                               \
		res0 = vis_fmul8x16al(vis_read_hi(d3), scale_fact[0]);     \
		COUNT_COLOR_U8();                                          \
		mlib_ImageColorTrue2IndexLine_U8_##dst_type##_3(           \
		    (mlib_u8 *)&color + 1, da, 1, colormap);               \
		COUNT_STORE_ERRORS_U8();                                   \
		sa += 3;                                                   \
	    }                                                              \
	    adr_src += sll;                                                \
	    adr_dst += dll;                                                \
	    for (row = 1; row < sh; row++) {                               \
		error = error_buf;                                         \
		error_sum = error_buf + 1;                                 \
		sa = adr_src;                                              \
		da = adr_dst;                                              \
		d0 = (*error++);                                           \
		d1 = (*error++);                                           \
		for (i = 0; i < sw; i++) {                                 \
		    d2 = (*error++);                                       \
		    COUNT_ERRORS_FROM_PREV_ROW_U8();                       \
		    (*error_sum++) = res0;                                 \
		    d0 = d1;                                               \
		    d1 = d2;                                               \
		}                                                          \
		error = error_sum = error_buf + 1;                         \
		err = 0.0;                                                 \
		for (i = 0; i < sw; i++) {                                 \
		    d0 = vis_ld_u8(sa + 2);                                \
		    d3 = vis_faligndata(d0, d3);                           \
		    d1 = vis_ld_u8(sa + 1);                                \
		    d3 = vis_faligndata(d1, d3);                           \
		    d2 = vis_ld_u8(sa);                                    \
		    d3 = vis_faligndata(d2, d3);                           \
		    d3 = vis_faligndata(d2, d3);                           \
		    d0 = (*error_sum++);                                   \
		    d1 = vis_fmul8x16al(vis_read_hi(d3), scale_fact[0]);   \
		    res0 = vis_fpadd16(d0, d1);                            \
		    COUNT_COLOR_U8();                                      \
		    mlib_ImageColorTrue2IndexLine_U8_##dst_type##_3        \
			((mlib_u8 *)&color + 1, da, 1, colormap);          \
		    COUNT_STORE_ERRORS_U8();                               \
		    sa += 3;                                               \
		}                                                          \
		beta  = k[1];                                              \
		gamma = k[2];                                              \
		delta = k[3];                                              \
		adr_src += sll;                                            \
		adr_dst += dll;                                            \
	    }                                                              \
	    __mlib_free(error_buf);                                        \
	}

/* *********************************************************** */

#define	ERROR_DIFFUSION_3x3_3_8_IS_INVERS_TABLE(dst_format,                \
	dst_type, table_format)                                            \
	{                                                                  \
	    mlib_u8 *sa, *adr_src;                                         \
	                                                                   \
/*                                                                         \
 * pointers for pixel and line of source                                   \
 */                                                                        \
	    table_format *table;                                           \
	    dst_format *da, *adr_dst;                                      \
	                                                                   \
/*                                                                         \
 * pointers for pixel and line of destination                              \
 */                                                                        \
	    mlib_f32 *lut_table;                                           \
	                                                                   \
/*                                                                         \
 * pointer to the lut table                                                \
 */                                                                        \
	    mlib_s32 tmp_ind;                                              \
	    mlib_s32 offset;                                               \
	                                                                   \
/*                                                                         \
 * offset in the lut table                                                 \
 */                                                                        \
	    mlib_s32 sll,                                                  \
/*                                                                         \
 * line length (in pixels) of source image                                 \
 */                                                                        \
		    dll,                                                   \
/*                                                                         \
 * line length (in pixels) of destination image                            \
 */                                                                        \
		    sw,                                                    \
/*                                                                         \
 * src width in pixels                                                     \
 */                                                                        \
		    sh;                                                    \
	                                                                   \
/*                                                                         \
 * src height in pixels                                                    \
 */                                                                        \
	    mlib_s32 row, i;                                               \
	                                                                   \
/*                                                                         \
 * indices                                                                 \
 */                                                                        \
	    mlib_d64 *error_buf;                                           \
	                                                                   \
/*                                                                         \
 * pointer to error's saving buffer (never incr'd                          \
 */                                                                        \
	    mlib_d64 *error, *error_sum;                                   \
	                                                                   \
/*                                                                         \
 * incremented pointers to error's saving buffer                           \
 */                                                                        \
	    mlib_d64 d0, d1, d2, d3, d4, d5, d6;                           \
	    mlib_d64 res0, resu0, resl0, res1, resu1, resl1, res2,         \
		resu2, resl2;                                              \
	    mlib_f32 color, tmp_color, fscale, fsix;                       \
	    mlib_d64 alpha, beta, gamma, delta, err;                       \
	    mlib_s32 tmp_in;                                               \
	    mlib_s32 tmp_mask0, tmp_mask1, tmp_mask2;                      \
	    mlib_s32 bits, shift0, shift1, shift2;                         \
	    mlib_s32 t_color;                                              \
	    GET_SRC_DST_PARAMETERS(mlib_u8, dst_format);                   \
	                                                                   \
	    lut_table =                                                    \
		(mlib_f32 *)mlib_ImageGetLutNormalTable(colormap) -        \
		offset;                                                    \
	    table =                                                        \
		(table_format *) mlib_ImageGetLutInversTable(colormap);    \
	    bits = mlib_ImageGetBits(colormap);                            \
	    PREPARE_FOR_COUNT_INDEX_U8();                                  \
	    error_buf =                                                    \
		(mlib_d64 *)__mlib_malloc((sw + 2) * sizeof (mlib_d64));   \
	    if (error_buf == NULL)                                         \
		return (MLIB_FAILURE);                                     \
	    error_buf[0] = error_buf[sw + 1] = 0.0;                        \
	    fsix = vis_to_float(0x8000);                                   \
	    fscale = vis_to_float(1 << (scalef_expon - 1));                \
	    vis_write_gsr(((16 - scalef_expon) << 3) + 7);                 \
	    LOAD_KERNEL_IN_REGS_IS_INVERS_TABLE_U8();                      \
	    error = error_sum = error_buf + 1;                             \
	    err = 0.0;                                                     \
	    sa = adr_src;                                                  \
	    da = adr_dst;                                                  \
	    for (i = 0; i < sw; i++) {                                     \
		d2 = vis_ld_u8(sa + 2);                                    \
		d3 = vis_faligndata(d2, d3);                               \
		d2 = vis_ld_u8(sa + 1);                                    \
		d3 = vis_faligndata(d2, d3);                               \
		d2 = vis_ld_u8(sa);                                        \
		d3 = vis_faligndata(d2, d3);                               \
		d3 = vis_faligndata(d2, d3);                               \
		res0 = vis_fmul8x16al(vis_read_hi(d3), fscale);            \
		COUNT_COLOR_IS_INVERS_TABLE_U8();                          \
		COUNT_INDEX_IN_INVERS_TABLE_FOR_3_CHANNELS_U8();           \
		(*da++) = tmp_ind = table[tmp_in];                         \
		COUNT_STORE_ERRORS_IS_INVERS_TABLE_U8();                   \
		sa += 3;                                                   \
	    }                                                              \
	    adr_src += sll;                                                \
	    adr_dst += dll;                                                \
	    for (row = 1; row < sh; row++) {                               \
		error = error_buf;                                         \
		error_sum = error_buf + 1;                                 \
		sa = adr_src;                                              \
		da = adr_dst;                                              \
		d0 = (*error++);                                           \
		d1 = (*error++);                                           \
		err = 0.0;                                                 \
		for (i = 0; i < sw; i++) {                                 \
		    d2 = (*error++);                                       \
		    COUNT_ERRORS_FROM_PREV_ROW_U8();                       \
		    d3 = vis_ld_u8(sa + 2);                                \
		    d6 = vis_faligndata(d3, d6);                           \
		    d4 = vis_ld_u8(sa + 1);                                \
		    d6 = vis_faligndata(d4, d6);                           \
		    d5 = vis_ld_u8(sa);                                    \
		    d6 = vis_faligndata(d5, d6);                           \
		    d6 = vis_faligndata(d5, d6);                           \
		    d3 = vis_fmul8x16al(vis_read_hi(d6), fscale);          \
		    res0 = vis_fpadd16(res0, d3);                          \
		    COUNT_COLOR_IS_INVERS_TABLE_U8();                      \
		    COUNT_INDEX_IN_INVERS_TABLE_FOR_3_CHANNELS_U8();       \
		    (*da++) = tmp_ind = table[tmp_in];                     \
		    COUNT_STORE_ERRORS_IS_INVERS_TABLE_U8();               \
		    d0 = d1;                                               \
		    d1 = d2;                                               \
		    sa += 3;                                               \
		}                                                          \
		adr_src += sll;                                            \
		adr_dst += dll;                                            \
	    }                                                              \
	    __mlib_free(error_buf);                                        \
	}

/* *********************************************************** */

#define	ERROR_DIFFUSION_3x3_4_8(dst_format, dst_type)                      \
	{                                                                  \
	    mlib_u8 *adr_src;                                              \
	                                                                   \
/*                                                                         \
 * pointer for pixel and line of source                                    \
 */                                                                        \
	    dst_format *da, *adr_dst;                                      \
	                                                                   \
/*                                                                         \
 * pointers for pixel and line of destination                              \
 */                                                                        \
	    mlib_f32 *lut_table;                                           \
	                                                                   \
/*                                                                         \
 * pointer to the lut table                                                \
 */                                                                        \
	    mlib_d64 *sa;                                                  \
	                                                                   \
/*                                                                         \
 * (mlib_d64) pointer for pixel and line of source                         \
 */                                                                        \
	    mlib_s32 tmp_ind;                                              \
	    mlib_s32 offset;                                               \
	                                                                   \
/*                                                                         \
 * offset in the lut table                                                 \
 */                                                                        \
	    mlib_s32 sll,                                                  \
/*                                                                         \
 * line length (in pixels) of source image                                 \
 */                                                                        \
		    dll,                                                   \
/*                                                                         \
 * line length (in pixels) of destination image                            \
 */                                                                        \
		    sw,                                                    \
/*                                                                         \
 * src width in pixels                                                     \
 */                                                                        \
		    sh;                                                    \
	                                                                   \
/*                                                                         \
 * src height in pixels                                                    \
 */                                                                        \
	    mlib_s32 row, i;                                               \
	                                                                   \
/*                                                                         \
 * indices                                                                 \
 */                                                                        \
	    mlib_d64 *error_buf;                                           \
	                                                                   \
/*                                                                         \
 * pointer to error's saving buffer (never incr'd                          \
 */                                                                        \
	    mlib_d64 *error, *error_sum;                                   \
	                                                                   \
/*                                                                         \
 * incremented pointers to error's saving buffer                           \
 */                                                                        \
	    mlib_d64 d0, d1, d2, d3;                                       \
	    mlib_d64 res0, resu0, resl0, res1, resu1, resl1, res2,         \
		resu2, resl2;                                              \
	    mlib_f32 f0, f1, color, tmp_color, scale_fact[2];              \
	    mlib_d64 beta, gamma, delta, err, k[4];                        \
	    GET_SRC_DST_PARAMETERS(mlib_u8, dst_format);                   \
	                                                                   \
	    lut_table =                                                    \
		(mlib_f32 *)mlib_ImageGetLutNormalTable(colormap) -        \
		offset;                                                    \
	    error_buf =                                                    \
		(mlib_d64 *)__mlib_malloc((sw + 2) * sizeof (mlib_d64));   \
	    if (error_buf == NULL)                                         \
		return (MLIB_FAILURE);                                     \
	    error_buf[0] = error_buf[sw + 1] = 0.0;                        \
	    scale_fact[1] = vis_to_float(0x8000);                          \
	    scale_fact[0] = vis_to_float(1 << (scalef_expon - 1));         \
	    vis_write_gsr((16 - scalef_expon) << 3);                       \
	    LOAD_KERNEL_IN_REGS_U8();                                      \
	    error = error_sum = error_buf + 1;                             \
	    err = 0.0;                                                     \
	    sa = (mlib_d64 *)vis_alignaddr(adr_src, 0);                    \
	    da = adr_dst;                                                  \
	    d1 = (*sa++);                                                  \
	    for (i = 0; i < sw - 1; i += 2) {                              \
		d2 = (*sa++);                                              \
		d3 = vis_faligndata(d1, d2);                               \
		f0 = vis_read_hi(d3);                                      \
		f1 = vis_read_lo(d3);                                      \
		res0 = vis_fmul8x16al(f0, scale_fact[0]);                  \
/*                                                                         \
 * U8 -> S16                                                               \
 */                                                                        \
		COUNT_COLOR_U8();                                          \
		mlib_ImageColorTrue2IndexLine_U8_##dst_type##_4(           \
		    (mlib_u8 *)&color, da, 1, colormap);                   \
		COUNT_STORE_ERRORS_U8();                                   \
		res0 = vis_fmul8x16al(f1, scale_fact[0]);                  \
/*                                                                         \
 * U8 -> S16                                                               \
 */                                                                        \
		COUNT_COLOR_U8();                                          \
		mlib_ImageColorTrue2IndexLine_U8_##dst_type##_4(           \
		    (mlib_u8 *)&color, da, 1, colormap);                   \
		COUNT_STORE_ERRORS_U8();                                   \
		d1 = d2;                                                   \
	    }                                                              \
	    if ((sw & 1) != 0) {                                           \
		d2 = (*sa++);                                              \
		d3 = vis_faligndata(d1, d2);                               \
		f0 = vis_read_hi(d3);                                      \
		res0 = vis_fmul8x16al(f0, scale_fact[0]);                  \
/*                                                                         \
 * U8 -> S16                                                               \
 */                                                                        \
		COUNT_COLOR_U8();                                          \
		mlib_ImageColorTrue2IndexLine_U8_##dst_type##_4(           \
		    (mlib_u8 *)&color, da, 1, colormap);                   \
		COUNT_STORE_ERRORS_U8();                                   \
	    }                                                              \
	    adr_src += sll;                                                \
	    adr_dst += dll;                                                \
	    for (row = 1; row < sh; row++) {                               \
		error = error_buf;                                         \
		error_sum = error_buf + 1;                                 \
		sa = (mlib_d64 *)vis_alignaddr(adr_src, 0);                \
		da = adr_dst;                                              \
		d0 = (*error++);                                           \
		d1 = (*error++);                                           \
		for (i = 0; i < sw; i++) {                                 \
		    d2 = (*error++);                                       \
		    COUNT_ERRORS_FROM_PREV_ROW_U8();                       \
		    (*error_sum++) = res0;                                 \
		    d0 = d1;                                               \
		    d1 = d2;                                               \
		}                                                          \
		error = error_sum = error_buf + 1;                         \
		err = 0.0;                                                 \
		d1 = (*sa++);                                              \
		for (i = 0; i < sw - 1; i += 2) {                          \
		    d2 = vis_ld_d64_nf(sa); sa++;                          \
		    d3 = vis_faligndata(d1, d2);                           \
		    f0 = vis_read_hi(d3);                                  \
		    f1 = vis_read_lo(d3);                                  \
		    d0 = (*error_sum++);                                   \
		    d1 = vis_fmul8x16al(f0, scale_fact[0]);                \
/*                                                                         \
 * U8 -> S16                                                               \
 */                                                                        \
		    res0 = vis_fpadd16(d0, d1);                            \
		    COUNT_COLOR_U8();                                      \
		    mlib_ImageColorTrue2IndexLine_U8_##dst_type##_4        \
			((mlib_u8 *)&color, da, 1, colormap);              \
		    COUNT_STORE_ERRORS_U8();                               \
		    d0 = (*error_sum++);                                   \
		    d1 = vis_fmul8x16al(f1, scale_fact[0]);                \
/*                                                                         \
 * U8 -> S16                                                               \
 */                                                                        \
		    res0 = vis_fpadd16(d0, d1);                            \
		    COUNT_COLOR_U8();                                      \
		    mlib_ImageColorTrue2IndexLine_U8_##dst_type##_4        \
			((mlib_u8 *)&color, da, 1, colormap);              \
		    COUNT_STORE_ERRORS_U8();                               \
		    d1 = d2;                                               \
		}                                                          \
		if ((sw & 1) != 0) {                                       \
		    d2 = vis_ld_d64_nf(sa);                                \
		    d3 = vis_faligndata(d1, d2);                           \
		    f0 = vis_read_hi(d3);                                  \
		    d0 = (*error_sum++);                                   \
		    d1 = vis_fmul8x16al(f0, scale_fact[0]);                \
/*                                                                         \
 * U8 -> S16                                                               \
 */                                                                        \
		    res0 = vis_fpadd16(d0, d1);                            \
		    COUNT_COLOR_U8();                                      \
		    mlib_ImageColorTrue2IndexLine_U8_##dst_type##_4        \
			((mlib_u8 *)&color, da, 1, colormap);              \
		    COUNT_STORE_ERRORS_U8();                               \
		}                                                          \
		beta  = k[1];                                              \
		gamma = k[2];                                              \
		delta = k[3];                                              \
		adr_src += sll;                                            \
		adr_dst += dll;                                            \
	    }                                                              \
	    __mlib_free(error_buf);                                        \
	}

/* *********************************************************** */

#define	ERROR_DIFFUSION_3x3_4_8_IS_INVERS_TABLE(dst_format,                \
	dst_type, table_format)                                            \
	{                                                                  \
	    mlib_u8 *sa, *adr_src;                                         \
	                                                                   \
/*                                                                         \
 * pointers for pixel and line of source                                   \
 */                                                                        \
	    table_format *table;                                           \
	    dst_format *da, *adr_dst;                                      \
	                                                                   \
/*                                                                         \
 * pointers for pixel and line of destination                              \
 */                                                                        \
	    mlib_f32 *lut_table;                                           \
	                                                                   \
/*                                                                         \
 * pointer to the lut table                                                \
 */                                                                        \
	    mlib_s32 tmp_ind;                                              \
	    mlib_s32 offset;                                               \
	                                                                   \
/*                                                                         \
 * offset in the lut table                                                 \
 */                                                                        \
	    mlib_s32 sll,                                                  \
/*                                                                         \
 * line length (in pixels) of source image                                 \
 */                                                                        \
		    dll,                                                   \
/*                                                                         \
 * line length (in pixels) of destination image                            \
 */                                                                        \
		    sw,                                                    \
/*                                                                         \
 * src width in pixels                                                     \
 */                                                                        \
		    sh;                                                    \
	                                                                   \
/*                                                                         \
 * src height in pixels                                                    \
 */                                                                        \
	    mlib_s32 row, i;                                               \
	                                                                   \
/*                                                                         \
 * indices                                                                 \
 */                                                                        \
	    mlib_d64 *error_buf;                                           \
	                                                                   \
/*                                                                         \
 * pointer to error's saving buffer (never incr'd                          \
 */                                                                        \
	    mlib_d64 *error, *error_sum;                                   \
	                                                                   \
/*                                                                         \
 * incremented pointers to error's saving buffer                           \
 */                                                                        \
	    mlib_d64 d0, d1, d2, d3, d4, d5, d6;                           \
	    mlib_d64 res0, resu0, resl0, res1, resu1, resl1, res2,         \
		resu2, resl2;                                              \
	    mlib_f32 color, tmp_color, fscale, fsix;                       \
	    mlib_d64 alpha, beta, gamma, delta, err;                       \
	    mlib_s32 tmp_in;                                               \
	    mlib_s32 tmp_mask0, tmp_mask1, tmp_mask2, tmp_mask3;           \
	    mlib_s32 bits, shift0, shift1, shift2, shift3;                 \
	    mlib_s32 t_color;                                              \
	    GET_SRC_DST_PARAMETERS(mlib_u8, dst_format);                   \
	                                                                   \
	    lut_table =                                                    \
		(mlib_f32 *)mlib_ImageGetLutNormalTable(colormap) -        \
		offset;                                                    \
	    table =                                                        \
		(table_format *) mlib_ImageGetLutInversTable(colormap);    \
	    bits = mlib_ImageGetBits(colormap);                            \
	    PREPARE_FOR_COUNT_INDEX_U8();                                  \
	    shift3 = (shift0 << 2);                                        \
	    tmp_mask3 = (tmp_mask2 << bits);                               \
	    error_buf =                                                    \
		(mlib_d64 *)__mlib_malloc((sw + 2) * sizeof (mlib_d64));   \
	    if (error_buf == NULL)                                         \
		return (MLIB_FAILURE);                                     \
	    error_buf[0] = error_buf[sw + 1] = 0.0;                        \
	    fsix = vis_to_float(0x8000);                                   \
	    fscale = vis_to_float(1 << (scalef_expon - 1));                \
	    vis_write_gsr(((16 - scalef_expon) << 3) + 7);                 \
	    LOAD_KERNEL_IN_REGS_IS_INVERS_TABLE_U8();                      \
	    error = error_sum = error_buf + 1;                             \
	    err = 0.0;                                                     \
	    sa = adr_src;                                                  \
	    da = adr_dst;                                                  \
	    for (i = 0; i < sw; i++) {                                     \
		d2 = vis_ld_u8(sa + 3);                                    \
		d3 = vis_faligndata(d2, d3);                               \
		d2 = vis_ld_u8(sa + 2);                                    \
		d3 = vis_faligndata(d2, d3);                               \
		d2 = vis_ld_u8(sa + 1);                                    \
		d3 = vis_faligndata(d2, d3);                               \
		d2 = vis_ld_u8(sa);                                        \
		d3 = vis_faligndata(d2, d3);                               \
		res0 = vis_fmul8x16al(vis_read_hi(d3), fscale);            \
		COUNT_COLOR_IS_INVERS_TABLE_U8();                          \
		COUNT_INDEX_IN_INVERS_TABLE_FOR_4_CHANNELS_U8();           \
		(*da++) = tmp_ind = table[tmp_in];                         \
		COUNT_STORE_ERRORS_IS_INVERS_TABLE_U8();                   \
		sa += 4;                                                   \
	    }                                                              \
	    adr_src += sll;                                                \
	    adr_dst += dll;                                                \
	    for (row = 1; row < sh; row++) {                               \
		error = error_buf;                                         \
		error_sum = error_buf + 1;                                 \
		sa = adr_src;                                              \
		da = adr_dst;                                              \
		d0 = (*error++);                                           \
		d1 = (*error++);                                           \
		err = 0.0;                                                 \
		for (i = 0; i < sw; i++) {                                 \
		    d2 = (*error++);                                       \
		    COUNT_ERRORS_FROM_PREV_ROW_U8();                       \
		    d3 = vis_ld_u8(sa + 3);                                \
		    d6 = vis_faligndata(d3, d6);                           \
		    d4 = vis_ld_u8(sa + 2);                                \
		    d6 = vis_faligndata(d4, d6);                           \
		    d5 = vis_ld_u8(sa + 1);                                \
		    d6 = vis_faligndata(d5, d6);                           \
		    d3 = vis_ld_u8(sa);                                    \
		    d6 = vis_faligndata(d3, d6);                           \
		    d3 = vis_fmul8x16al(vis_read_hi(d6), fscale);          \
		    res0 = vis_fpadd16(res0, d3);                          \
		    COUNT_COLOR_IS_INVERS_TABLE_U8();                      \
		    COUNT_INDEX_IN_INVERS_TABLE_FOR_4_CHANNELS_U8();       \
		    (*da++) = tmp_ind = table[tmp_in];                     \
		    COUNT_STORE_ERRORS_IS_INVERS_TABLE_U8();               \
		    d0 = d1;                                               \
		    d1 = d2;                                               \
		    sa += 4;                                               \
		}                                                          \
		adr_src += sll;                                            \
		adr_dst += dll;                                            \
	    }                                                              \
	    __mlib_free(error_buf);                                        \
	}

/* *********************************************************** */

mlib_status
mlib_v_ErrorDiffusion3x3_3_8_8(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *kern,
    mlib_s32 scalef_expon,
    const void *colormap)
{
	mlib_s32 method = mlib_ImageGetMethod(colormap);

	if (method == 0) {
		ERROR_DIFFUSION_3x3_3_8_IS_INVERS_TABLE(mlib_u8,
		    U8,
		    mlib_u8)
	} else
	{
		ERROR_DIFFUSION_3x3_3_8(mlib_u8,
		    U8)
	}

	return (
	    MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_ErrorDiffusion3x3_4_8_8(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *kern,
    mlib_s32 scalef_expon,
    const void *colormap)
{
	mlib_s32 method = mlib_ImageGetMethod(colormap);

	if (method == 0) {
		ERROR_DIFFUSION_3x3_4_8_IS_INVERS_TABLE(mlib_u8,
		    U8,
		    mlib_u8)
	} else
	{
		ERROR_DIFFUSION_3x3_4_8(mlib_u8,
		    U8)
	}

	return (
	    MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_ErrorDiffusion3x3_3_8_16(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *kern,
    mlib_s32 scalef_expon,
    const void *colormap)
{
	mlib_s32 method = mlib_ImageGetMethod(colormap);
	mlib_s32 indexsize = mlib_ImageGetIndexSize(colormap);

	if (method == 0) {
		if (indexsize == 1) {
			ERROR_DIFFUSION_3x3_3_8_IS_INVERS_TABLE(mlib_s16,
			    S16,
			    mlib_u8)
		} else
		{
			ERROR_DIFFUSION_3x3_3_8_IS_INVERS_TABLE(mlib_s16,
			    S16,
			    mlib_s16)
		}

	} else
	{
		ERROR_DIFFUSION_3x3_3_8(mlib_s16,
		    S16)
	}

	return (
	    MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_v_ErrorDiffusion3x3_4_8_16(
    const mlib_image *src,
    mlib_image *dst,
    const mlib_s32 *kern,
    mlib_s32 scalef_expon,
    const void *colormap)
{
	mlib_s32 method = mlib_ImageGetMethod(colormap);
	mlib_s32 indexsize = mlib_ImageGetIndexSize(colormap);

	if (method == 0) {
		if (indexsize == 1) {
			ERROR_DIFFUSION_3x3_4_8_IS_INVERS_TABLE(mlib_s16,
			    S16,
			    mlib_u8)
		} else
		{
			ERROR_DIFFUSION_3x3_4_8_IS_INVERS_TABLE(mlib_s16,
			    S16,
			    mlib_s16)
		}

	} else
	{
		ERROR_DIFFUSION_3x3_4_8(mlib_s16, S16)
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
