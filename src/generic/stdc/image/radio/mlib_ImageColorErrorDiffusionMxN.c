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

#pragma ident	"@(#)mlib_ImageColorErrorDiffusionMxN.c	9.3	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageColorErrorDiffusionMxN - convert a true-color/grayscale image
 *                                         to an indexed-color/black-white
 *                                         image by error diffusion
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageColorErrorDiffusionMxN(mlib_image       *dst,
 *                                                   const mlib_image *src,
 *                                                   const mlib_s32   *kernel,
 *                                                   mlib_s32         m,
 *                                                   mlib_s32         n,
 *                                                   mlib_s32         dm,
 *                                                   mlib_s32         dn,
 *                                                   mlib_s32         scale,
 *                                                   const void       *colormap)
 *
 * ARGUMENTS
 *      dst       Pointer to destination image.
 *      src       Pointer to source image.
 *      kernel    Pointer to the error-distribution kernel, in row major order.
 *      m         Kernel width (m > 1).
 *      n         Kernel height (n > 1).
 *      dm, dn    Position of key element in the kernel.
 *      scale     The scaling factor for kernel to convert the input
 *                integer coefficients into floating-point coefficients:
 *                floating-point coefficient = integer coefficient * 2^(-scale)
 *      colormap  Internal data structure for image dithering.
 *
 * DESCRIPTION
 *      Convert a 3 or 4-channel image to a 1-channel indexed image, or convert
 *      a 1-channel grayscale image to a 1-channel MLIB_BIT image, with the
 *      method of error diffusion.
 *
 * RESTRICTIONS
 *      If src is a 1-channel MLIB_BYTE image
 *      then dst must be a 1-channel MLIB_BIT
 *      image. In other cases src must be an MLIB_BYTE or MLIB_SHORT image
 *      with 3 or 4 channels and dst must be a 1-channel MLIB_BYTE or
 *      MLIB_SHORT image.
 *
 *      The colormap must be created by mlib_ImageColorTrue2IndexInit() or by
 *      mlib_ImageColorDitherInit().
 *      It may or may not have a colorcube included.
 *      If it does, the colorcube is used. Otherwise, the general lookup table
 *      included in colormap is used.
 *
 *      kernel[0] = kernel[1] = ... = kernel[m*dn+dm] = 0;
 *      kernel[m*dn+dm+1] + ... + kernel[m*n-1] = 2^scale;
 *      scale >= 0
 */

#include <mlib_image.h>
#include <mlib_SysMath.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageColormap.h>
#include <mlib_ImageColorErrorDiffusionMxN.h>

#ifdef MLIB_VIS
#include <vis_proto.h>
#endif /* MLIB_VIS */

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageColorErrorDiffusionMxN = \
	__mlib_ImageColorErrorDiffusionMxN

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageColorErrorDiffusionMxN) mlib_ImageColorErrorDiffusionMxN
    __attribute__((weak, alias("__mlib_ImageColorErrorDiffusionMxN")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	FUNC(SUFF)	mlib_ImageColorErrorDiffusion##SUFF

/* *********************************************************** */

#define	FUNC_M_ARG                                                        \
	mlib_d64 *buffd, mlib_d64 *perror, mlib_d64 *pkern, mlib_s32 sw

/* *********************************************************** */

#define	FUNC_DM_ARG                                                     \
	void *dl, mlib_d64 *buffd, mlib_d64 *perror, mlib_d64 *pkern,   \
	const void *colormap, mlib_d64 *lut_table, mlib_s32 sw,         \
	mlib_s32 dm

/* *********************************************************** */

void FUNC(
    ch3_m1) (
    FUNC_M_ARG)
{
	mlib_s32 i;
	mlib_d64 k0 = pkern[0];
	mlib_d64 a0, b0, c0;

	for (i = 0; i < sw; i++) {
		a0 = (*perror++);
		b0 = (*perror++);
		c0 = (*perror++);
		buffd[0] = buffd[0] + k0 * a0;
		buffd[1] = buffd[1] + k0 * b0;
		buffd[2] = buffd[2] + k0 * c0;
		buffd += 3;
	}
}

/* *********************************************************** */

void FUNC(
    ch3_m2) (
    FUNC_M_ARG)
{
	mlib_s32 i;
	mlib_d64 k0 = pkern[0];
	mlib_d64 k1 = pkern[1];
	mlib_d64 a0, b0, c0;
	mlib_d64 a1, b1, c1;

	a0 = (*perror++);
	b0 = (*perror++);
	c0 = (*perror++);

	for (i = 0; i < sw; i++) {
		a1 = (*perror++);
		b1 = (*perror++);
		c1 = (*perror++);
		buffd[0] = buffd[0] + k0 * a0 + k1 * a1;
		buffd[1] = buffd[1] + k0 * b0 + k1 * b1;
		buffd[2] = buffd[2] + k0 * c0 + k1 * c1;
		buffd += 3;
		a0 = a1;
		b0 = b1;
		c0 = c1;
	}
}

/* *********************************************************** */

void FUNC(
    ch3_m3) (
    FUNC_M_ARG)
{
	mlib_s32 i;
	mlib_d64 k0 = pkern[0];
	mlib_d64 k1 = pkern[1];
	mlib_d64 k2 = pkern[2];
	mlib_d64 a0, b0, c0;
	mlib_d64 a1, b1, c1;
	mlib_d64 a2, b2, c2;

	a0 = (*perror++);
	b0 = (*perror++);
	c0 = (*perror++);
	a1 = (*perror++);
	b1 = (*perror++);
	c1 = (*perror++);

	for (i = 0; i < sw; i++) {
		a2 = (*perror++);
		b2 = (*perror++);
		c2 = (*perror++);
		buffd[0] = buffd[0] + k0 * a0 + k1 * a1 + k2 * a2;
		buffd[1] = buffd[1] + k0 * b0 + k1 * b1 + k2 * b2;
		buffd[2] = buffd[2] + k0 * c0 + k1 * c1 + k2 * c2;
		buffd += 3;
		a0 = a1;
		a1 = a2;
		b0 = b1;
		b1 = b2;
		c0 = c1;
		c1 = c2;
	}
}

/* *********************************************************** */

void FUNC(
    ch3_m4) (
    FUNC_M_ARG)
{
	mlib_s32 i;
	mlib_d64 k0 = pkern[0];
	mlib_d64 k1 = pkern[1];
	mlib_d64 k2 = pkern[2];
	mlib_d64 k3 = pkern[3];
	mlib_d64 a0, b0, c0;
	mlib_d64 a1, b1, c1;
	mlib_d64 a2, b2, c2;
	mlib_d64 a3, b3, c3;

	a0 = (*perror++);
	b0 = (*perror++);
	c0 = (*perror++);
	a1 = (*perror++);
	b1 = (*perror++);
	c1 = (*perror++);
	a2 = (*perror++);
	b2 = (*perror++);
	c2 = (*perror++);

	for (i = 0; i < sw; i++) {
		a3 = (*perror++);
		b3 = (*perror++);
		c3 = (*perror++);
		buffd[0] = buffd[0] + k0 * a0 + k1 * a1 + k2 * a2 + k3 * a3;
		buffd[1] = buffd[1] + k0 * b0 + k1 * b1 + k2 * b2 + k3 * b3;
		buffd[2] = buffd[2] + k0 * c0 + k1 * c1 + k2 * c2 + k3 * c3;
		buffd += 3;
		a0 = a1;
		a1 = a2;
		a2 = a3;
		b0 = b1;
		b1 = b2;
		b2 = b3;
		c0 = c1;
		c1 = c2;
		c2 = c3;
	}
}

/* *********************************************************** */

void FUNC(
    ch4_m1) (
    FUNC_M_ARG)
{
	mlib_s32 i;
	mlib_d64 k0 = pkern[0];
	mlib_d64 a0, b0, c0, d0;

	for (i = 0; i < sw; i++) {
		a0 = (*perror++);
		b0 = (*perror++);
		c0 = (*perror++);
		d0 = (*perror++);
		buffd[0] = buffd[0] + k0 * a0;
		buffd[1] = buffd[1] + k0 * b0;
		buffd[2] = buffd[2] + k0 * c0;
		buffd[3] = buffd[3] + k0 * d0;
		buffd += 4;
	}
}

/* *********************************************************** */

void FUNC(
    ch4_m2) (
    FUNC_M_ARG)
{
	mlib_s32 i;
	mlib_d64 k0 = pkern[0];
	mlib_d64 k1 = pkern[1];
	mlib_d64 a0, b0, c0, d0;
	mlib_d64 a1, b1, c1, d1;

	a0 = (*perror++);
	b0 = (*perror++);
	c0 = (*perror++);
	d0 = (*perror++);

	for (i = 0; i < sw; i++) {
		a1 = (*perror++);
		b1 = (*perror++);
		c1 = (*perror++);
		d1 = (*perror++);
		buffd[0] = buffd[0] + k0 * a0 + k1 * a1;
		buffd[1] = buffd[1] + k0 * b0 + k1 * b1;
		buffd[2] = buffd[2] + k0 * c0 + k1 * c1;
		buffd[3] = buffd[3] + k0 * d0 + k1 * d1;
		buffd += 4;
		a0 = a1;
		b0 = b1;
		c0 = c1;
		d0 = d1;
	}
}

/* *********************************************************** */

void FUNC(
    ch4_m3) (
    FUNC_M_ARG)
{
	mlib_s32 i;
	mlib_d64 k0 = pkern[0];
	mlib_d64 k1 = pkern[1];
	mlib_d64 k2 = pkern[2];
	mlib_d64 a0, b0, c0, d0;
	mlib_d64 a1, b1, c1, d1;
	mlib_d64 a2, b2, c2, d2;

	a0 = (*perror++);
	b0 = (*perror++);
	c0 = (*perror++);
	d0 = (*perror++);
	a1 = (*perror++);
	b1 = (*perror++);
	c1 = (*perror++);
	d1 = (*perror++);

	for (i = 0; i < sw; i++) {
		a2 = (*perror++);
		b2 = (*perror++);
		c2 = (*perror++);
		d2 = (*perror++);
		buffd[0] = buffd[0] + k0 * a0 + k1 * a1 + k2 * a2;
		buffd[1] = buffd[1] + k0 * b0 + k1 * b1 + k2 * b2;
		buffd[2] = buffd[2] + k0 * c0 + k1 * c1 + k2 * c2;
		buffd[3] = buffd[3] + k0 * d0 + k1 * d1 + k2 * d2;
		buffd += 4;
		a0 = a1;
		a1 = a2;
		b0 = b1;
		b1 = b2;
		c0 = c1;
		c1 = c2;
		d0 = d1;
		d1 = d2;
	}
}

/* *********************************************************** */

void FUNC(
    ch4_m4) (
    FUNC_M_ARG)
{
	mlib_s32 i;
	mlib_d64 k0 = pkern[0];
	mlib_d64 k1 = pkern[1];
	mlib_d64 k2 = pkern[2];
	mlib_d64 k3 = pkern[3];
	mlib_d64 a0, b0, c0, d0;
	mlib_d64 a1, b1, c1, d1;
	mlib_d64 a2, b2, c2, d2;
	mlib_d64 a3, b3, c3, d3;

	a0 = (*perror++);
	b0 = (*perror++);
	c0 = (*perror++);
	d0 = (*perror++);
	a1 = (*perror++);
	b1 = (*perror++);
	c1 = (*perror++);
	d1 = (*perror++);
	a2 = (*perror++);
	b2 = (*perror++);
	c2 = (*perror++);
	d2 = (*perror++);

	for (i = 0; i < sw; i++) {
		a3 = (*perror++);
		b3 = (*perror++);
		c3 = (*perror++);
		d3 = (*perror++);
		buffd[0] = buffd[0] + k0 * a0 + k1 * a1 + k2 * a2 + k3 * a3;
		buffd[1] = buffd[1] + k0 * b0 + k1 * b1 + k2 * b2 + k3 * b3;
		buffd[2] = buffd[2] + k0 * c0 + k1 * c1 + k2 * c2 + k3 * c3;
		buffd[3] = buffd[3] + k0 * d0 + k1 * d1 + k2 * d2 + k3 * d3;
		buffd += 4;
		a0 = a1;
		a1 = a2;
		a2 = a3;
		b0 = b1;
		b1 = b2;
		b2 = b3;
		c0 = c1;
		c1 = c2;
		c2 = c3;
		d0 = d1;
		d1 = d2;
		d2 = d3;
	}
}

/* *********************************************************** */

typedef void (*func_m_type) (
    FUNC_M_ARG);

static const func_m_type func_m_arr[] = {
	FUNC(ch3_m1),
	FUNC(ch3_m2),
	FUNC(ch3_m3),
	FUNC(ch3_m4),
	FUNC(ch4_m1),
	FUNC(ch4_m2),
	FUNC(ch4_m3),
	FUNC(ch4_m4),
};

typedef void (*func_dm_type) (
    FUNC_DM_ARG);

/* *********************************************************** */

#define	DEF_VAR_u8                                              \
	mlib_d64 sat = 2147483648.0;                            \
	mlib_d64 scalef = (1 << 24)

/* *********************************************************** */

#define	DEF_VAR_s16	mlib_d64 scalef = (1 << 16)

/* *********************************************************** */

#define	DEF_VARS(STYPE, DTYPE, ITYPE)                           \
	DEF_VAR_##STYPE;                                        \
	mlib_##DTYPE *dp = dl;                                  \
	mlib_##ITYPE a, b, c;                                   \
	mlib_##ITYPE ae, be, ce;                                \
	mlib_s32 i, col_ind;                                    \
	mlib_s32 col0, col1, col2

/* *********************************************************** */

#define	FUNC_DM(SUFF, STYPE, DTYPE)                                      \
	static void                                                      \
	FUNC(ch3_##STYPE##_##DTYPE##_t##SUFF##_dm0) (FUNC_DM_ARG)        \
	{                                                                \
	    DEF_VARS(STYPE, DTYPE, d64);                                 \
	    DEF_EXTRA_VARS(STYPE, DTYPE, d64)                            \
	    for (i = 0; i < sw; i++) {                                   \
		a = buffd[0];                                            \
		b = buffd[1];                                            \
		c = buffd[2];                                            \
		buffd += 3;                                              \
		GET_COLOR_INDEX3(STYPE, DTYPE, d64);                     \
		GET_COLOR_ERROR3(d64);                                   \
		dp++;                                                    \
		perror[0] = ae;                                          \
		perror[1] = be;                                          \
		perror[2] = ce;                                          \
		perror += 3;                                             \
	    }                                                            \
	}                                                                \
/*                                                                       \
 * ************************************************************          \
 */                                                                      \
	static void                                                      \
	FUNC(ch4_##STYPE##_##DTYPE##_t##SUFF##_dm0) (FUNC_DM_ARG)        \
	{                                                                \
	    DEF_VARS(STYPE, DTYPE, d64);                                 \
	    DEF_EXTRA_VARS(STYPE, DTYPE, d64)                            \
	    DEF_EXTRA_VARS4(STYPE, DTYPE, d64)                           \
	    for (i = 0; i < sw; i++) {                                   \
		a = buffd[0];                                            \
		b = buffd[1];                                            \
		c = buffd[2];                                            \
		d = buffd[3];                                            \
		buffd += 4;                                              \
		GET_COLOR_INDEX4(STYPE, DTYPE, d64);                     \
		GET_COLOR_ERROR4(d64);                                   \
		dp++;                                                    \
		perror[0] = ae;                                          \
		perror[1] = be;                                          \
		perror[2] = ce;                                          \
		perror[3] = de;                                          \
		perror += 4;                                             \
	    }                                                            \
	}                                                                \
	                                                                 \
/*                                                                       \
 * ************************************************************          \
 */                                                                      \
	static void                                                      \
	FUNC(ch3_##STYPE##_##DTYPE##_t##SUFF##_dm1) (FUNC_DM_ARG)        \
	{                                                                \
	    DEF_VARS(STYPE, DTYPE, d64);                                 \
	    DEF_EXTRA_VARS(STYPE, DTYPE, d64)                            \
	    mlib_d64 k0 = pkern[0];                                      \
	                                                                 \
	    ae = be = ce = 0;                                            \
	    for (i = 0; i < sw; i++) {                                   \
		a = buffd[0] + k0 * ae;                                  \
		b = buffd[1] + k0 * be;                                  \
		c = buffd[2] + k0 * ce;                                  \
		buffd += 3;                                              \
		GET_COLOR_INDEX3(STYPE, DTYPE, d64);                     \
		GET_COLOR_ERROR3(d64);                                   \
		dp++;                                                    \
		perror[0] = ae;                                          \
		perror[1] = be;                                          \
		perror[2] = ce;                                          \
		perror += 3;                                             \
	    }                                                            \
	}                                                                \
/*                                                                       \
 * ************************************************************          \
 */                                                                      \
	static void                                                      \
	FUNC(ch4_##STYPE##_##DTYPE##_t##SUFF##_dm1) (FUNC_DM_ARG)        \
	{                                                                \
	    DEF_VARS(STYPE, DTYPE, d64);                                 \
	    DEF_EXTRA_VARS(STYPE, DTYPE, d64)                            \
	    DEF_EXTRA_VARS4(STYPE,                                       \
		DTYPE, d64)                                              \
	    mlib_d64 k0 = pkern[0];                                      \
	    ae = be = ce = de = 0;                                       \
	    for (i = 0; i < sw; i++) {                                   \
		a = buffd[0] + k0 * ae;                                  \
		b = buffd[1] + k0 * be;                                  \
		c = buffd[2] + k0 * ce;                                  \
		d = buffd[3] + k0 * de;                                  \
		buffd += 4;                                              \
		GET_COLOR_INDEX4(STYPE, DTYPE, d64);                     \
		GET_COLOR_ERROR4(d64);                                   \
		dp++;                                                    \
		perror[0] = ae;                                          \
		perror[1] = be;                                          \
		perror[2] = ce;                                          \
		perror[3] = de;                                          \
		perror += 4;                                             \
	    }                                                            \
	}                                                                \
/*                                                                       \
 * ************************************************************          \
 */                                                                      \
	static void                                                      \
	FUNC(ch3_##STYPE##_##DTYPE##_t##SUFF##_dm) (FUNC_DM_ARG)         \
	{                                                                \
	    DEF_VARS(STYPE, DTYPE, d64);                                 \
	    DEF_EXTRA_VARS(STYPE, DTYPE, d64)                            \
	    mlib_d64 k0, kk;                                             \
	    mlib_s32 j;                                                  \
	                                                                 \
	    pkern += dm - 1;                                             \
	    k0 = pkern[0];                                               \
	    a = buffd[0];                                                \
	    b = buffd[1];                                                \
	    c = buffd[2];                                                \
	    buffd += 3;                                                  \
	    for (i = 0; i < sw; i++) {                                   \
		GET_COLOR_INDEX3(STYPE, DTYPE, d64);                     \
		GET_COLOR_ERROR3(d64);                                   \
		dp++;                                                    \
		perror[0] = ae;                                          \
		perror[1] = be;                                          \
		perror[2] = ce;                                          \
		perror += 3;                                             \
/*                                                                       \
 * j = 0                                                                 \
 */                                                                      \
		a = buffd[0] + k0 * ae;                                  \
		b = buffd[1] + k0 * be;                                  \
		c = buffd[2] + k0 * ce;                                  \
		for (j = 1; j < dm; j++) {                               \
		    kk = pkern[-j];                                      \
		    buffd[3 * j] += kk * ae;                             \
		    buffd[3 * j + 1] += kk * be;                         \
		    buffd[3 * j + 2] += kk * ce;                         \
		}                                                        \
		buffd += 3;                                              \
	    }                                                            \
	}                                                                \
	                                                                 \
/*                                                                       \
 * ************************************************************          \
 */                                                                      \
	static void                                                      \
	FUNC(ch4_##STYPE##_##DTYPE##_t##SUFF##_dm) (FUNC_DM_ARG)         \
	{                                                                \
	    DEF_VARS(STYPE, DTYPE, d64);                                 \
	    DEF_EXTRA_VARS(STYPE, DTYPE, d64)                            \
	    DEF_EXTRA_VARS4(STYPE, DTYPE, d64)                           \
	    mlib_d64 k0, kk;                                             \
	    mlib_s32 j;                                                  \
	                                                                 \
	    pkern += dm - 1;                                             \
	    k0 = pkern[0];                                               \
	    a = buffd[0];                                                \
	    b = buffd[1];                                                \
	    c = buffd[2];                                                \
	    d = buffd[3];                                                \
	    buffd += 4;                                                  \
	    for (i = 0; i < sw; i++) {                                   \
		GET_COLOR_INDEX4(STYPE, DTYPE, d64);                     \
		GET_COLOR_ERROR4(d64);                                   \
		dp++;                                                    \
		perror[0] = ae;                                          \
		perror[1] = be;                                          \
		perror[2] = ce;                                          \
		perror[3] = de;                                          \
		perror += 4;                                             \
/*                                                                       \
 * j = 0                                                                 \
 */                                                                      \
		a = buffd[0] + k0 * ae;                                  \
		b = buffd[1] + k0 * be;                                  \
		c = buffd[2] + k0 * ce;                                  \
		d = buffd[3] + k0 * de;                                  \
		for (j = 1; j < dm; j++) {                               \
		    kk = pkern[-j];                                      \
		    buffd[4 * j] += kk * ae;                             \
		    buffd[4 * j + 1] += kk * be;                         \
		    buffd[4 * j + 2] += kk * ce;                         \
		    buffd[4 * j + 3] += kk * de;                         \
		}                                                        \
		buffd += 4;                                              \
	    }                                                            \
	}                                                                \
	                                                                 \
/*                                                                       \
 * ************************************************************          \
 */                                                                      \
	static void                                                      \
	FUNC(ch4_##STYPE##_##DTYPE##_t##SUFF##_3x3) (FUNC_DM_ARG)        \
	{                                                                \
	    DEF_VARS(STYPE, DTYPE, d64);                                 \
	    DEF_EXTRA_VARS(STYPE, DTYPE, d64)                            \
	    DEF_EXTRA_VARS4(STYPE, DTYPE, d64)                           \
	    mlib_d64 k0 = pkern[0];                                      \
	    mlib_d64 k1 = pkern[1];                                      \
	    mlib_d64 k2 = pkern[2];                                      \
	    mlib_d64 kk = pkern[3];                                      \
	    mlib_d64 a0, b0, c0, d0;                                     \
	    mlib_d64 a1, b1, c1, d1;                                     \
	    mlib_d64 a2, b2, c2, d2;                                     \
	    mlib_##STYPE *sl = (void *)buffd;                            \
	                                                                 \
	    ae = be = ce = de = 0;                                       \
	    a0 = b0 = c0 = d0 = 0;                                       \
	    a1 = perror[0];                                              \
	    b1 = perror[1];                                              \
	    c1 = perror[2];                                              \
	    d1 = perror[3];                                              \
	    for (i = 0; i < sw; i++) {                                   \
		a2 = perror[4];                                          \
		b2 = perror[5];                                          \
		c2 = perror[6];                                          \
		d2 = perror[7];                                          \
		GET_3x3_SRC4(a, b, c, d);                                \
		a = a + k0 * a0 + k1 * a1 + k2 * a2 + kk * ae;           \
		b = b + k0 * b0 + k1 * b1 + k2 * b2 + kk * be;           \
		c = c + k0 * c0 + k1 * c1 + k2 * c2 + kk * ce;           \
		d = d + k0 * d0 + k1 * d1 + k2 * d2 + kk * de;           \
		sl += 4;                                                 \
		a0 = a1;                                                 \
		a1 = a2;                                                 \
		b0 = b1;                                                 \
		b1 = b2;                                                 \
		c0 = c1;                                                 \
		c1 = c2;                                                 \
		d0 = d1;                                                 \
		d1 = d2;                                                 \
		GET_COLOR_INDEX4(STYPE, DTYPE, d64);                     \
		GET_COLOR_ERROR4(d64);                                   \
		dp++;                                                    \
		perror[0] = ae;                                          \
		perror[1] = be;                                          \
		perror[2] = ce;                                          \
		perror[3] = de;                                          \
		perror += 4;                                             \
	    }                                                            \
	}                                                                \
	                                                                 \
/*                                                                       \
 * ************************************************************          \
 */                                                                      \
	                                                                 \
/*                                                                       \
 * FLoyd-Steinberg kernel 3x3:	...  ...  ...                            \
 *				...  ...  7/16                           \
 *				3/16 5/16 1/16                           \
 */                                                                      \
	static void                                                      \
	FUNC(ch4_##STYPE##_##DTYPE##_t##SUFF##_3x3fs) (FUNC_DM_ARG)      \
	{                                                                \
	    DEF_VARS(STYPE, DTYPE, s32);                                 \
	    DEF_EXTRA_VARS(STYPE, DTYPE, s32)                            \
	    DEF_EXTRA_VARS4(STYPE, DTYPE, s32)                           \
	    FS_VARS_##STYPE;                                             \
	    mlib_s32 a0, b0, c0, d0;                                     \
	    mlib_s32 a1, b1, c1, d1;                                     \
	    mlib_s32 a2, b2, c2, d2;                                     \
	    mlib_s32 *ierror = (mlib_s32 *)perror;                       \
	    mlib_##STYPE *sl = (void *)buffd;                            \
	                                                                 \
	    ae = be = ce = de = 0;                                       \
	    a0 = b0 = c0 = d0 = 0;                                       \
	    a1 = ierror[0];                                              \
	    b1 = ierror[1];                                              \
	    c1 = ierror[2];                                              \
	    d1 = ierror[3];                                              \
	    for (i = 0; i < sw; i++) {                                   \
		a2 = ierror[4];                                          \
		b2 = ierror[5];                                          \
		c2 = ierror[6];                                          \
		d2 = ierror[7];                                          \
		a = (((mlib_s32)sl[0] << 4) + a0 + 5 * a1 + 3 * a2 +     \
		    7 * ae) >> 4;                                        \
		b = (((mlib_s32)sl[1] << 4) + b0 + 5 * b1 + 3 * b2 +     \
		    7 * be) >> 4;                                        \
		c = (((mlib_s32)sl[2] << 4) + c0 + 5 * c1 + 3 * c2 +     \
		    7 * ce) >> 4;                                        \
		d = (((mlib_s32)sl[3] << 4) + d0 + 5 * d1 + 3 * d2 +     \
		    7 * de) >> 4;                                        \
		sl += 4;                                                 \
		a0 = a1;                                                 \
		a1 = a2;                                                 \
		b0 = b1;                                                 \
		b1 = b2;                                                 \
		c0 = c1;                                                 \
		c1 = c2;                                                 \
		d0 = d1;                                                 \
		d1 = d2;                                                 \
		GET_COLOR_INDEX4(STYPE, DTYPE, s32);                     \
		GET_COLOR_ERROR4(STYPE);                                 \
		dp++;                                                    \
		ierror[0] = ae;                                          \
		ierror[1] = be;                                          \
		ierror[2] = ce;                                          \
		ierror[3] = de;                                          \
		ierror += 4;                                             \
	    }                                                            \
	}                                                                \
	                                                                 \
/*                                                                       \
 * ************************************************************          \
 */                                                                      \
	static func_dm_type                                              \
	func_arr_##STYPE##_##DTYPE##_t##SUFF[] = {                       \
	    FUNC(ch3_##STYPE##_##DTYPE##_t##SUFF##_dm0),                 \
	    FUNC(ch4_##STYPE##_##DTYPE##_t##SUFF##_dm0),                 \
	    FUNC(ch3_##STYPE##_##DTYPE##_t##SUFF##_dm1),                 \
	    FUNC(ch4_##STYPE##_##DTYPE##_t##SUFF##_dm1),                 \
	    FUNC(ch3_##STYPE##_##DTYPE##_t##SUFF##_dm),                  \
	    FUNC(ch4_##STYPE##_##DTYPE##_t##SUFF##_dm),                  \
	    FUNC(ch3_##STYPE##_##DTYPE##_t##SUFF##_3x3),                 \
	    FUNC(ch4_##STYPE##_##DTYPE##_t##SUFF##_3x3),                 \
	    FUNC(ch3_##STYPE##_##DTYPE##_t##SUFF##_3x3fs),               \
	    FUNC(ch4_##STYPE##_##DTYPE##_t##SUFF##_3x3fs)                \
	};

/* *********************************************************** */

/* common for C and VIS: 3x3 & 3x3fs, 3 channels */

/* *********************************************************** */

#define	FUNC_DM_COMMON(SUFF, STYPE, DTYPE)                              \
	static void                                                     \
	FUNC(ch3_##STYPE##_##DTYPE##_t##SUFF##_3x3) (FUNC_DM_ARG)       \
	{                                                               \
	    DEF_VARS(STYPE, DTYPE, d64);                                \
	    DEF_EXTRA_VARS(STYPE, DTYPE, d64)                           \
	    mlib_d64 k0 = pkern[0];                                     \
	    mlib_d64 k1 = pkern[1];                                     \
	    mlib_d64 k2 = pkern[2];                                     \
	    mlib_d64 kk = pkern[3];                                     \
	    mlib_d64 a0, b0, c0;                                        \
	    mlib_d64 a1, b1, c1;                                        \
	    mlib_d64 a2, b2, c2;                                        \
	    mlib_##STYPE *sl = (void *)buffd;                           \
	                                                                \
	    ae = be = ce = 0;                                           \
	    a0 = b0 = c0 = 0;                                           \
	    a1 = perror[0];                                             \
	    b1 = perror[1];                                             \
	    c1 = perror[2];                                             \
	    for (i = 0; i < sw; i++) {                                  \
		a2 = perror[3];                                         \
		b2 = perror[4];                                         \
		c2 = perror[5];                                         \
		GET_3x3_SRC3(a, b, c);                                  \
		a = a + k0 * a0 + k1 * a1 + k2 * a2 + kk * ae;          \
		b = b + k0 * b0 + k1 * b1 + k2 * b2 + kk * be;          \
		c = c + k0 * c0 + k1 * c1 + k2 * c2 + kk * ce;          \
		sl += 3;                                                \
		a0 = a1;                                                \
		a1 = a2;                                                \
		b0 = b1;                                                \
		b1 = b2;                                                \
		c0 = c1;                                                \
		c1 = c2;                                                \
		GET_COLOR_INDEX3(STYPE, DTYPE, d64);                    \
		GET_COLOR_ERROR3(d64);                                  \
		dp++;                                                   \
		perror[0] = ae;                                         \
		perror[1] = be;                                         \
		perror[2] = ce;                                         \
		perror += 3;                                            \
	    }                                                           \
	}                                                               \
	                                                                \
/*                                                                      \
 * ************************************************************         \
 */                                                                     \
	static void                                                     \
	FUNC(ch3_##STYPE##_##DTYPE##_t##SUFF##_3x3fs) (FUNC_DM_ARG)     \
	{                                                               \
	    DEF_VARS(STYPE, DTYPE, s32);                                \
	    DEF_EXTRA_VARS(STYPE, DTYPE, s32)                           \
	    FS_VARS_##STYPE;                                            \
	    mlib_s32 a0, b0, c0;                                        \
	    mlib_s32 a1, b1, c1;                                        \
	    mlib_s32 a2, b2, c2;                                        \
	    mlib_s32 *ierror = (mlib_s32 *)perror;                      \
	    mlib_##STYPE *sl = (void *)buffd;                           \
	                                                                \
	    ae = be = ce = 0;                                           \
	    a0 = b0 = c0 = 0;                                           \
	    a1 = ierror[0];                                             \
	    b1 = ierror[1];                                             \
	    c1 = ierror[2];                                             \
	    for (i = 0; i < sw; i++) {                                  \
		a2 = ierror[3];                                         \
		b2 = ierror[4];                                         \
		c2 = ierror[5];                                         \
		a = (((mlib_s32)sl[0] << 4) + a0 + 5 * a1 + 3 * a2 +    \
		    7 * ae) >> 4;                                       \
		b = (((mlib_s32)sl[1] << 4) + b0 + 5 * b1 + 3 * b2 +    \
		    7 * be) >> 4;                                       \
		c = (((mlib_s32)sl[2] << 4) + c0 + 5 * c1 + 3 * c2 +    \
		    7 * ce) >> 4;                                       \
		sl += 3;                                                \
		a0 = a1;                                                \
		a1 = a2;                                                \
		b0 = b1;                                                \
		b1 = b2;                                                \
		c0 = c1;                                                \
		c1 = c2;                                                \
		GET_COLOR_INDEX3(STYPE, DTYPE, s32);                    \
		GET_COLOR_ERROR3(STYPE);                                \
		dp++;                                                   \
		ierror[0] = ae;                                         \
		ierror[1] = be;                                         \
		ierror[2] = ce;                                         \
		ierror += 3;                                            \
	    }                                                           \
	}

/* *********************************************************** */

#ifndef MLIB_VIS

#define	DEF_FUNC(SUFF, STYPE, DTYPE)                                     \
	FUNC_DM_COMMON(SUFF, STYPE, DTYPE) FUNC_DM(SUFF, STYPE, DTYPE)

#else /* MLIB_VIS */

#define	DEF_FUNC(SUFF, STYPE, DTYPE)                            \
	FUNC_DM_COMMON(SUFF, STYPE, DTYPE)

#endif /* MLIB_VIS */

/* *********************************************************** */

#define	mlib_ImageColorTrue2IndexLine_u8_u8_3\
		mlib_ImageColorTrue2IndexLine_U8_U8_3
#define	mlib_ImageColorTrue2IndexLine_u8_u8_4\
		mlib_ImageColorTrue2IndexLine_U8_U8_4

#define	mlib_ImageColorTrue2IndexLine_u8_s16_3\
		mlib_ImageColorTrue2IndexLine_U8_S16_3

#define	mlib_ImageColorTrue2IndexLine_u8_s16_4\
		mlib_ImageColorTrue2IndexLine_U8_S16_4

#define	mlib_ImageColorTrue2IndexLine_s16_u8_3\
		mlib_ImageColorTrue2IndexLine_S16_U8_3

#define	mlib_ImageColorTrue2IndexLine_s16_u8_4\
		mlib_ImageColorTrue2IndexLine_S16_U8_4

#define	mlib_ImageColorTrue2IndexLine_s16_s16_3\
		mlib_ImageColorTrue2IndexLine_S16_S16_3

#define	mlib_ImageColorTrue2IndexLine_s16_s16_4\
		mlib_ImageColorTrue2IndexLine_S16_S16_4

/* *********************************************************** */

#ifndef MLIB_USE_FTOI_CLAMPING

/* *********************************************************** */

#define	d64_T0_u8(DST, SRC)                                     \
	DST =                                                   \
	    (((SRC) < MLIB_U8_MIN) ? MLIB_U8_MIN : (((SRC) >    \
	    MLIB_U8_MAX) ? MLIB_U8_MAX : (mlib_s32)(SRC)))

/* *********************************************************** */

#define	d64_T0_s16(DST, SRC)                                     \
	DST =                                                    \
	    (((SRC) < MLIB_S16_MIN) ? MLIB_S16_MIN : (((SRC) >   \
	    MLIB_S16_MAX) ? MLIB_S16_MAX : (mlib_s32)(SRC)))

#else /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#define	d64_T0_u8(DST, SRC)                                     \
	DST = ((mlib_s32)((SRC) * scalef - sat) >> 24) + 128

/* *********************************************************** */

#define	d64_T0_s16(DST, SRC)	DST = ((mlib_s32)((SRC)*scalef) >> 16)

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#define	FS_VARS_u8	mlib_s32 imask, mask1 = 0x00FF, mask2 = 0xFF00

/* *********************************************************** */

#define	s32_T0_u8(DST, SRC)                                     \
	imask = ((SRC) >> 5) & 0x18;                            \
	DST = ((SRC) & (mask1 >> imask)) | (mask2 >> imask);    \
	DST &= mask1

/* *********************************************************** */

#define	FS_VARS_s16                                             \
	mlib_u32 mask32[] = {                                   \
	    0x0000FFFF, 0x7FFF0000, 0x80000000, 0x0000FFFF      \
	}                                                       \
	                                                        \
	;                                                       \
	mlib_u32 smask;                                         \
	mlib_s16 res_s16

/* *********************************************************** */

#define	s32_T0_s16(DST, SRC)                                            \
	smask =                                                         \
	    *(mlib_u32 *)((mlib_u8 *)mask32 + (((SRC) >> 13) & 0xC));   \
	res_s16 = ((SRC) & smask) | (smask >> 16);                      \
	DST = (mlib_s32)res_s16

/* *********************************************************** */

#define	GET_3x3_SRC3(a, b, c)                                   \
	a = (mlib_d64)sl[0];                                    \
	b = (mlib_d64)sl[1];                                    \
	c = (mlib_d64)sl[2]

/* *********************************************************** */

#define	GET_3x3_SRC4(a, b, c, d)                                \
	a = (mlib_d64)sl[0];                                    \
	b = (mlib_d64)sl[1];                                    \
	c = (mlib_d64)sl[2];                                    \
	d = (mlib_d64)sl[3]

/* *********************************************************** */

#define	GET_COLOR3(STYPE, ITYPE)                                \
	ITYPE##_T0_##STYPE(col0, a);                            \
	ITYPE##_T0_##STYPE(col1, b);                            \
	ITYPE##_T0_##STYPE(col2, c)

/* *********************************************************** */

#define	GET_COLOR4(STYPE, ITYPE)                                \
	ITYPE##_T0_##STYPE(col0, a);                            \
	ITYPE##_T0_##STYPE(col1, b);                            \
	ITYPE##_T0_##STYPE(col2, c);                            \
	ITYPE##_T0_##STYPE(col3, d)

/* *********************************************************** */

#define	LUT_IND3_u8(ind)	4*ind + 1
#define	LUT_IND3_s16(ind)	4*ind + 1
#define	LUT_IND3_d64(ind)	3*ind

/* *********************************************************** */

#define	GET_COLOR_ERROR3(LUT_TYPE)                                  \
	col_ind = LUT_IND3_##LUT_TYPE(col_ind);                     \
	ae = col0 - ((mlib_##LUT_TYPE *) lut_table)[col_ind];       \
	be = col1 - ((mlib_##LUT_TYPE *) lut_table)[col_ind + 1];   \
	ce = col2 - ((mlib_##LUT_TYPE *) lut_table)[col_ind + 2]

/* *********************************************************** */

#define	GET_COLOR_ERROR4(LUT_TYPE)                                  \
	col_ind *= 4;                                               \
	ae = col0 - ((mlib_##LUT_TYPE *) lut_table)[col_ind];       \
	be = col1 - ((mlib_##LUT_TYPE *) lut_table)[col_ind + 1];   \
	ce = col2 - ((mlib_##LUT_TYPE *) lut_table)[col_ind + 2];   \
	de = col3 - ((mlib_##LUT_TYPE *) lut_table)[col_ind + 3]

/* *********************************************************** */

#define	DEF_EXTRA_VARS(STYPE, DTYPE, ITYPE)                     \
	mlib_##STYPE tmp_color[4];

/* *********************************************************** */

#define	DEF_EXTRA_VARS4(STYPE, DTYPE, ITYPE)                    \
	mlib_##ITYPE d, de;                                     \
	mlib_s32 col3;

/* *********************************************************** */

#define	GET_COLOR_INDEX3(STYPE, DTYPE, ITYPE)                   \
	GET_COLOR3(STYPE, ITYPE);                               \
	tmp_color[0] = col0;                                    \
	tmp_color[1] = col1;                                    \
	tmp_color[2] = col2;                                    \
	mlib_ImageColorTrue2IndexLine_##STYPE##_##DTYPE##_3     \
	    (tmp_color, dp, 1, colormap);                       \
	col_ind = *dp

/* *********************************************************** */

#define	GET_COLOR_INDEX4(STYPE, DTYPE, ITYPE)                   \
	GET_COLOR4(STYPE, ITYPE);                               \
	tmp_color[0] = col0;                                    \
	tmp_color[1] = col1;                                    \
	tmp_color[2] = col2;                                    \
	tmp_color[3] = col3;                                    \
	mlib_ImageColorTrue2IndexLine_##STYPE##_##DTYPE##_4     \
	    (tmp_color, dp, 1, colormap);                       \
	col_ind = *dp

#ifndef MLIB_VIS

DEF_FUNC(0, u8, u8)
DEF_FUNC(0, u8, s16)

#endif /* MLIB_VIS */

DEF_FUNC(0, s16, u8)
DEF_FUNC(0, s16, s16)

/* *********************************************************** */

#define	BITS_IN_u8	8
#define	BITS_IN_s16	16

/* *********************************************************** */

#define	COL_MSK_u8(x)	((x) & mask)
#define	COL_MSK_s16(x)	(((x) - MLIB_S16_MIN) & mask)
#define	TAB_TYPE(DTYPE)	mlib_##DTYPE

/* *********************************************************** */

#undef  DEF_EXTRA_VARS
#define	DEF_EXTRA_VARS(STYPE, DTYPE, ITYPE)                     \
	TAB_TYPE(DTYPE) *inv_table =                            \
	    (void *)mlib_ImageGetLutInversTable(colormap);      \
	mlib_s32 bits = mlib_ImageGetBits(colormap);            \
	mlib_s32 minus_bits = BITS_IN_##STYPE - bits;           \
	mlib_s32 two_bits = 2 * bits;                           \
	mlib_s32 mask = ~((1 << minus_bits) - 1);

/* *********************************************************** */

#undef  DEF_EXTRA_VARS4
#define	DEF_EXTRA_VARS4(STYPE, DTYPE, ITYPE)                    \
	mlib_##ITYPE d, de;                                     \
	mlib_s32 col3;

/* *********************************************************** */

#undef  GET_COLOR_INDEX3
#define	GET_COLOR_INDEX3(STYPE, DTYPE, ITYPE)                         \
	GET_COLOR3(STYPE, ITYPE);                                     \
	col_ind =                                                     \
	    (((COL_MSK_##STYPE(col0)) >> minus_bits) << two_bits) +   \
	    (((COL_MSK_##STYPE(col1) << bits) +                       \
	    COL_MSK_##STYPE(col2)) >> minus_bits);                    \
	col_ind = inv_table[col_ind];                                 \
	*dp = col_ind

/* *********************************************************** */

#undef  GET_COLOR_INDEX4
#define	GET_COLOR_INDEX4(STYPE, DTYPE, ITYPE)                      \
	GET_COLOR4(STYPE, ITYPE);                                  \
	col_ind =                                                  \
	    ((((COL_MSK_##STYPE(col0) << bits) +                   \
	    COL_MSK_##STYPE(col1)) >> minus_bits) << two_bits) +   \
	    (((COL_MSK_##STYPE(col2) << bits) +                    \
	    COL_MSK_##STYPE(col3)) >> minus_bits);                 \
	col_ind = inv_table[col_ind];                              \
	*dp = col_ind

#ifndef MLIB_VIS

DEF_FUNC(1, u8, u8)
DEF_FUNC(1, u8, s16)

#endif /* MLIB_VIS */

DEF_FUNC(1, s16, u8)
DEF_FUNC(1, s16, s16)

/* *********************************************************** */

#undef  TAB_TYPE
#define	TAB_TYPE(DTYPE)	mlib_u8

#ifndef MLIB_VIS

DEF_FUNC(1s, u8, s16)

#endif /* MLIB_VIS */

DEF_FUNC(1s, s16, s16)

/* *********************************************************** */

#define	TAB_SIZE_u8	256

#define	TAB_SIZE_s16	1024

/* *********************************************************** */

#define	CONV_u8(x)	(x)
/* #define CONV_s16(x) ((x >> 6) & 0x3FF) */
#define	CONV_s16(x)	(((x) & 0xFFC0) >> 6)

/* *********************************************************** */

#undef  DEF_EXTRA_VARS
#define	DEF_EXTRA_VARS(STYPE, DTYPE, ITYPE)                     \
	mlib_##DTYPE *tab0 =                                    \
	    (void *)mlib_ImageGetLutInversTable(colormap);      \
	mlib_##DTYPE *tab1 = tab0 + TAB_SIZE_##STYPE;           \
	mlib_##DTYPE *tab2 = tab1 + TAB_SIZE_##STYPE;

/* *********************************************************** */

#undef  DEF_EXTRA_VARS4
#define	DEF_EXTRA_VARS4(STYPE, DTYPE, ITYPE)                    \
	mlib_##DTYPE *tab3 = tab2 + TAB_SIZE_##STYPE;           \
	mlib_##ITYPE d, de;                                     \
	mlib_s32 col3;

/* *********************************************************** */

#undef  GET_COLOR_INDEX3
#define	GET_COLOR_INDEX3(STYPE, DTYPE, ITYPE)                      \
	GET_COLOR3(STYPE, ITYPE);                                  \
	col_ind =                                                  \
	    tab0[CONV_##STYPE(col0)] +                             \
	    tab1[CONV_##STYPE(col1)] + tab2[CONV_##STYPE(col2)];   \
	*dp = col_ind

/* *********************************************************** */

#undef  GET_COLOR_INDEX4
#define	GET_COLOR_INDEX4(STYPE, DTYPE, ITYPE)                      \
	GET_COLOR4(STYPE, ITYPE);                                  \
	col_ind =                                                  \
	    tab0[CONV_##STYPE(col0)] +                             \
	    tab1[CONV_##STYPE(col1)] +                             \
	    tab2[CONV_##STYPE(col2)] + tab3[CONV_##STYPE(col3)];   \
	*dp = col_ind

#ifndef MLIB_VIS

DEF_FUNC(2, u8, u8)
DEF_FUNC(2, u8, s16)

#endif /* MLIB_VIS */

DEF_FUNC(2, s16, u8)
DEF_FUNC(2, s16, s16)

/* *********************************************************** */

#ifndef MLIB_VIS

static const func_dm_type *func_arr[] = {
	func_arr_u8_u8_t0,
	func_arr_u8_s16_t0,
	func_arr_s16_u8_t0,
	func_arr_s16_s16_t0,

	func_arr_u8_u8_t1,
	func_arr_u8_s16_t1,
	func_arr_s16_u8_t1,
	func_arr_s16_s16_t1,

	func_arr_u8_u8_t2,
	func_arr_u8_s16_t2,
	func_arr_s16_u8_t2,
	func_arr_s16_s16_t2,

	NULL,
	func_arr_u8_s16_t1s,
	NULL,
	func_arr_s16_s16_t1s
};

#else /* MLIB_VIS */

/* *********************************************************** */

#undef  DEF_FUNC
#define	DEF_FUNC(SUFF, STYPE, DTYPE)	FUNC_DM(SUFF, STYPE, DTYPE)

/* *********************************************************** */

typedef union
{
	mlib_d64 d64;
	struct
	{
		mlib_s32 f0;
		mlib_s32 f1;
	} i32s;
} d64_2x32;

/* *********************************************************** */

#undef  DEF_VAR_s16
#define	DEF_VAR_s16                                             \
	d64_2x32 col01, col23;                                  \
	mlib_f32 fscale = vis_to_float(0x40004000);             \
	mlib_d64 dst_color, lut_color

/* *********************************************************** */

#undef  GET_3x3_SRC3
#define	GET_3x3_SRC3(a, b, c)                                   \
	a = vis_faligndata(vis_ld_u16(sl + 1), a);              \
	a = vis_faligndata(vis_ld_u16(sl), a);                  \
	c = vis_ld_u16(sl + 2);                                 \
	col01.d64 = vis_fmuld8sux16(fscale, vis_read_hi(a));    \
	col23.d64 = vis_fmuld8sux16(fscale, vis_read_lo(c));    \
	a = (mlib_d64)col01.i32s.f0;                            \
	b = (mlib_d64)col01.i32s.f1;                            \
	c = (mlib_d64)col23.i32s.f1

/* *********************************************************** */

#undef  GET_3x3_SRC4
#define	GET_3x3_SRC4(a, b, c, d)                                \
	a = vis_faligndata(vis_ld_u16(sl + 1), a);              \
	a = vis_faligndata(vis_ld_u16(sl), a);                  \
	c = vis_faligndata(vis_ld_u16(sl + 3), c);              \
	c = vis_faligndata(vis_ld_u16(sl + 2), c);              \
	col01.d64 = vis_fmuld8sux16(fscale, vis_read_hi(a));    \
	col23.d64 = vis_fmuld8sux16(fscale, vis_read_hi(c));    \
	a = (mlib_d64)col01.i32s.f0;                            \
	b = (mlib_d64)col01.i32s.f1;                            \
	c = (mlib_d64)col23.i32s.f0;                            \
	d = (mlib_d64)col23.i32s.f1

/* *********************************************************** */

#undef  GET_COLOR_ERROR3
#define	GET_COLOR_ERROR3(LUT_TYPE)                                     \
	lut_color = lut_table[col_ind];                                \
	col01.d64 =                                                    \
	vis_fpsub32(vis_fmuld8sux16(fscale, vis_read_hi(dst_color)),   \
	    vis_fmuld8sux16(fscale, vis_read_hi(lut_color)));          \
	col23.d64 =                                                    \
	vis_fpsub32(vis_fmuld8sux16(fscale, vis_read_lo(dst_color)),   \
	    vis_fmuld8sux16(fscale, vis_read_lo(lut_color)));          \
	ae = (mlib_d64)col01.i32s.f1;                                  \
	be = (mlib_d64)col23.i32s.f0;                                  \
	ce = (mlib_d64)col23.i32s.f1

/* *********************************************************** */

#undef  GET_COLOR_ERROR4
#define	GET_COLOR_ERROR4(LUT_TYPE)                                     \
	lut_color = lut_table[col_ind];                                \
	col01.d64 =                                                    \
	vis_fpsub32(vis_fmuld8sux16(fscale, vis_read_hi(dst_color)),   \
	    vis_fmuld8sux16(fscale, vis_read_hi(lut_color)));          \
	col23.d64 =                                                    \
	vis_fpsub32(vis_fmuld8sux16(fscale, vis_read_lo(dst_color)),   \
	    vis_fmuld8sux16(fscale, vis_read_lo(lut_color)));          \
	ae = (mlib_d64)col01.i32s.f0;                                  \
	be = (mlib_d64)col01.i32s.f1;                                  \
	ce = (mlib_d64)col23.i32s.f0;                                  \
	de = (mlib_d64)col23.i32s.f1

/* *********************************************************** */

#undef  GET_COLOR3
#define	GET_COLOR3(STYPE, ITYPE)                                \
	col01.i32s.f1 = (mlib_s32)a;                            \
	col23.i32s.f0 = (mlib_s32)b;                            \
	col23.i32s.f1 = (mlib_s32)c;                            \
	dst_color = vis_fpackfix_pair(col01.d64, col23.d64)

/* *********************************************************** */

#undef  GET_COLOR4
#define	GET_COLOR4(STYPE, ITYPE)                                \
	col01.i32s.f0 = (mlib_s32)a;                            \
	col01.i32s.f1 = (mlib_s32)b;                            \
	col23.i32s.f0 = (mlib_s32)c;                            \
	col23.i32s.f1 = (mlib_s32)d;                            \
	dst_color = vis_fpackfix_pair(col01.d64, col23.d64)

/* *********************************************************** */

#undef  DEF_EXTRA_VARS
#define	DEF_EXTRA_VARS(STYPE, DTYPE, ITYPE)	mlib_s32 ii;

/* *********************************************************** */

#undef  DEF_EXTRA_VARS4
#define	DEF_EXTRA_VARS4(STYPE, DTYPE, ITYPE)                    \
	mlib_s32 col3;                                          \
	mlib_##ITYPE d, de;

/* *********************************************************** */

#undef  GET_COLOR_INDEX3
#define	GET_COLOR_INDEX3(STYPE, DTYPE, ITYPE)                   \
	GET_COLOR3(STYPE, ITYPE);                               \
	mlib_ImageColorTrue2IndexLine_##STYPE##_##DTYPE##_3     \
	    ((mlib_s16 *)&dst_color + 1, dp, 1, colormap);      \
	col_ind = *dp

/* *********************************************************** */

#undef  GET_COLOR_INDEX4
#define	GET_COLOR_INDEX4(STYPE, DTYPE, ITYPE)                   \
	GET_COLOR4(STYPE, ITYPE);                               \
	mlib_ImageColorTrue2IndexLine_##STYPE##_##DTYPE##_4     \
	    ((mlib_s16 *)&dst_color, dp, 1, colormap);          \
	col_ind = *dp

DEF_FUNC(0, s16, u8)
DEF_FUNC(0, s16, s16)

/* *********************************************************** */

#undef  TAB_TYPE
#define	TAB_TYPE(DTYPE)	mlib_##DTYPE

/* *********************************************************** */

#undef  DEF_EXTRA_VARS
#define	DEF_EXTRA_VARS(STYPE, DTYPE, ITYPE)                     \
	mlib_s64 i_color;                                       \
	                                                        \
	TAB_TYPE(DTYPE) *inv_table =                            \
	    mlib_ImageGetLutInversTable(colormap);              \
	mlib_d64 dmask = vis_to_double_dup(0x80008000);         \
	mlib_d64 dst_color1;                                    \
	mlib_s32 bits = mlib_ImageGetBits(colormap);            \
	mlib_s32 shift0 = 16 - bits;                            \
	mlib_s32 shift1 = 2 * shift0;                           \
	mlib_s32 shift2 = shift0 + shift1;                      \
	mlib_s32 mask0 = (1 << bits) - 1;                       \
	mlib_s32 mask1 = (mask0 << bits);                       \
	mlib_s32 mask2 = (mask1 << bits);

/* *********************************************************** */

#undef  DEF_EXTRA_VARS4
#define	DEF_EXTRA_VARS4(STYPE, DTYPE, ITYPE)                    \
	mlib_s32 shift3 = 2 * shift1;                           \
	mlib_s32 mask3 = (mask2 << bits);                       \
	mlib_s32 col3;                                          \
	mlib_##ITYPE d, de;

/* *********************************************************** */

#undef  GET_COLOR_INDEX3
#define	GET_COLOR_INDEX3(STYPE, DTYPE, ITYPE)                   \
	GET_COLOR3(STYPE, ITYPE);                               \
	dst_color1 = vis_fxor(dst_color, dmask);                \
	i_color = *(mlib_s64 *)&dst_color1;                     \
	col_ind =                                               \
	    ((i_color >> shift0) & mask0) +                     \
	    ((i_color >> shift1) & mask1) +                     \
	    ((i_color >> shift2) & mask2);                      \
	col_ind = inv_table[col_ind];                           \
	*dp = col_ind

/* *********************************************************** */

#undef  GET_COLOR_INDEX4
#define	GET_COLOR_INDEX4(STYPE, DTYPE, ITYPE)                   \
	GET_COLOR4(STYPE, ITYPE);                               \
	dst_color1 = vis_fxor(dst_color, dmask);                \
	i_color = *(mlib_s64 *)&dst_color1;                     \
	col_ind =                                               \
	    ((i_color >> shift0) & mask0) +                     \
	    ((i_color >> shift1) & mask1) +                     \
	    ((i_color >> shift2) & mask2) +                     \
	    ((i_color >> shift3) & mask3);                      \
	col_ind = inv_table[col_ind];                           \
	*dp = col_ind

DEF_FUNC(1, s16, u8)
DEF_FUNC(1, s16, s16)

/* *********************************************************** */

#undef  TAB_TYPE
#define	TAB_TYPE(DTYPE)	mlib_u8

DEF_FUNC(1s, s16, s16)

/* *********************************************************** */

#undef  CONV_s16
#define	CONV_s16(x)	(x >> 6)

/* *********************************************************** */

#undef  DEF_EXTRA_VARS
#define	DEF_EXTRA_VARS(STYPE, DTYPE, ITYPE)                     \
	mlib_##DTYPE *tab0 =                                    \
	    (void *)mlib_ImageGetLutInversTable(colormap);      \
	mlib_##DTYPE *tab1 = tab0 + TAB_SIZE_##STYPE;           \
	mlib_##DTYPE *tab2 = tab1 + TAB_SIZE_##STYPE;

/* *********************************************************** */

#undef  DEF_EXTRA_VARS4
#define	DEF_EXTRA_VARS4(STYPE, DTYPE, ITYPE)                    \
	mlib_##DTYPE *tab3 = tab2 + TAB_SIZE_##STYPE;           \
	mlib_s32 col3;                                          \
	mlib_##ITYPE d, de;

/* *********************************************************** */

#undef  GET_COLOR_INDEX3
#define	GET_COLOR_INDEX3(STYPE, DTYPE, ITYPE)                      \
	GET_COLOR3(STYPE, ITYPE);                                  \
	col0 = ((mlib_u16 *)&dst_color)[1];                        \
	col1 = ((mlib_u16 *)&dst_color)[2];                        \
	col2 = ((mlib_u16 *)&dst_color)[3];                        \
	col_ind =                                                  \
	    tab0[CONV_##STYPE(col0)] +                             \
	    tab1[CONV_##STYPE(col1)] + tab2[CONV_##STYPE(col2)];   \
	*dp = col_ind

/* *********************************************************** */

#undef  GET_COLOR_INDEX4
#define	GET_COLOR_INDEX4(STYPE, DTYPE, ITYPE)                      \
	GET_COLOR4(STYPE, ITYPE);                                  \
	col0 = ((mlib_u16 *)&dst_color)[0];                        \
	col1 = ((mlib_u16 *)&dst_color)[1];                        \
	col2 = ((mlib_u16 *)&dst_color)[2];                        \
	col3 = ((mlib_u16 *)&dst_color)[3];                        \
	col_ind =                                                  \
	    tab0[CONV_##STYPE(col0)] +                             \
	    tab1[CONV_##STYPE(col1)] +                             \
	    tab2[CONV_##STYPE(col2)] + tab3[CONV_##STYPE(col3)];   \
	*dp = col_ind

DEF_FUNC(2, s16, u8)
DEF_FUNC(2, s16, s16)

/* *********************************************************** */

static const func_dm_type *func_arr[] = {
	NULL,
	NULL,
	func_arr_s16_u8_t0,
	func_arr_s16_s16_t0,

	NULL,
	NULL,
	func_arr_s16_u8_t1,
	func_arr_s16_s16_t1,

	NULL,
	NULL,
	func_arr_s16_u8_t2,
	func_arr_s16_s16_t2,

	NULL,
	NULL,
	NULL,
	func_arr_s16_s16_t1s
};

/* *********************************************************** */

#endif /* MLIB_VIS */

/* *********************************************************** */

#define	KW_MAX	4
#define	KH_MAX	2

/* *********************************************************** */

#define	MAX_N	8
#define	MAX_M	8

/* *********************************************************** */

#ifdef MLIB_VIS
#define	WRITE_GSR(gsr)	vis_write_gsr(gsr);
#else /* MLIB_VIS */

/* *********************************************************** */

#define	WRITE_GSR(gsr)

#endif /* MLIB_VIS */

/* *********************************************************** */

#define	NCHAN	nchan

/* *********************************************************** */

#define	LOAD_KERNEL(kern, kernel)                               \
	kernel += m * n - 1;                                    \
	for (i = 0; i < (m * dn + dm); i++) {                   \
	    kern[i] = dscale * kernel[-i];                      \
	}                                                       \
	                                                        \
	WRITE_GSR((2 << 3) + 6)

/* *********************************************************** */

#ifndef MLIB_VIS

/* *********************************************************** */

#define	LOAD_SRC_LINE(buffd, sl)                                \
	if (stype == MLIB_BYTE) {                               \
	    for (i = 0; i < sw * nchan; i++) {                  \
		buffd[i] = sl[i];                               \
	    }                                                   \
	} else {                                                \
	    for (i = 0; i < sw * nchan; i++) {                  \
		buffd[i] = ((mlib_s16 *)sl)[i];                 \
	    }                                                   \
	}

#else /* MLIB_VIS */

/* *********************************************************** */

#define	LOAD_SRC_LINE(buffd, sl)                                 \
	mlib_f32 fscale = vis_to_float(0x40004000);              \
	mlib_s16 *sp = (void *)sl;                               \
	mlib_d64 ss;                                             \
	d64_2x32 dd;                                             \
	                                                         \
	for (i = 0; i < sw * nchan; i += 2) {                    \
	    ss = vis_faligndata(vis_ld_u16(sp + 1), ss);         \
	    ss = vis_faligndata(vis_ld_u16(sp), ss);             \
	    dd.d64 = vis_fmuld8sux16(fscale, vis_read_hi(ss));   \
	    buffd[i] = (mlib_d64)dd.i32s.f0;                     \
	    buffd[i + 1] = (mlib_d64)dd.i32s.f1;                 \
	    sp += 2;                                             \
	}

#endif /* MLIB_VIS */

/* *********************************************************** */

mlib_status
__mlib_ImageColorErrorDiffusionMxN(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kernel,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 dm,
    mlib_s32 dn,
    mlib_s32 scale,
    const void *colormap)
{
	mlib_type stype, dtype;
	mlib_u8 *sl, *dl;
	void *lut_table;
	mlib_s32 offset, off, kw, dn1;
	mlib_s32 nchan, dchan, sll, dll, sw, sh, dw, dh, flag3x3, flag3x3fs = 0;
	mlib_s32 row, i, j, bsize, buff_ind =
	    0, func_ind1, func_ind2, method, indexsize;
	mlib_d64 *pbuff, *buffd, *buff_lcl[2 * MAX_N], **buff_arr =
	    buff_lcl, **buff;
	mlib_d64 kern_lcl[MAX_N * MAX_M], *kern = kern_lcl, *pkern;
	mlib_d64 dscale;
	func_dm_type func_dm;
	const func_m_type *func_m_ptr;

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_SIZE_EQUAL(src, dst);
	MLIB_IMAGE_HAVE_CHAN(dst, 1);
	MLIB_IMAGE_AND_COLORMAP_ARE_COMPAT(src, colormap);

	if (!(dm >= 0 && dm < m && dn >= 0 && dn < n)) {
		return (MLIB_FAILURE);
	}

	if (mlib_ImageGetType(dst) == MLIB_BIT) {
		return FUNC(Bit_MxN) (dst, src, kernel, m, n, dm, dn, scale,
		    colormap);
	}
#ifdef MLIB_VIS

	if (mlib_ImageGetType(src) == MLIB_BYTE) {
		return FUNC(U8_MxN) (dst, src, kernel, m, n, dm, dn, scale,
		    colormap);
	}
#endif /* MLIB_VIS */

	MLIB_IMAGE_GET_ALL_PARAMS(dst, dtype, dchan, dw, dh, dll, dl);
	MLIB_IMAGE_GET_ALL_PARAMS(src, stype, nchan, sw, sh, sll, sl);

	if (!(stype == MLIB_BYTE || stype == MLIB_SHORT)) {
		return (MLIB_FAILURE);
	}

	if (!(dtype == MLIB_BYTE || dtype == MLIB_SHORT)) {
		return (MLIB_FAILURE);
	}

	if (!(nchan >= 3 && nchan <= 4)) {
		return (MLIB_FAILURE);
	}

	if (kernel == NULL || colormap == NULL) {
		return (MLIB_NULLPOINTER);
	}

	if (scale < 0) {
		return (MLIB_OUTOFRANGE);
	}

	dscale = 1.0;
	while (scale > 30) {
		dscale *= 1.0 / (1 << 30);
		scale -= 30;
	}

	dscale /= (1 << scale);

#if 0
	for (i = 0; i <= (m * dn + dm); i++) {
		if (kernel[i])
			return (MLIB_FAILURE);
	}

#endif /* 0 */
#if 0
	{
		mlib_d64 sum = 0;

		for (i = m * dn + dm + 1; i < m * n; i++) {
			sum += kernel[i];
		}

		sum *= dscale;

		if (sum < 0.5 || sum > 2) {
			if (dm != m - 1 || dn != n - 1)
				return (MLIB_FAILURE);
		}
	}

#endif /* 0 */

	dn = n - 1 - dn;
	dm = m - 1 - dm;

	if (n > MAX_N || m > MAX_M) {
		kern =
		    __mlib_malloc(n * m * sizeof (mlib_d64) +
		    2 * n * sizeof (mlib_d64 *));

		if (kern == NULL)
			return (MLIB_FAILURE);
		buff_arr = (mlib_d64 **)(kern + n * m);
	}

/* load kernel */
	LOAD_KERNEL(kern, kernel)

/* malloc buffers */
	    bsize = (sw + m) * NCHAN;
	dn1 = (dn) ? dn : 1;
	pbuff = __mlib_malloc((dn1 + 1) * bsize * sizeof (mlib_d64) + 32);

	if (pbuff == NULL) {
		if (kern != kern_lcl)
			__mlib_free(kern);
		return (MLIB_FAILURE);
	}

	for (j = 0; j < dn1; j++) {
		buff_arr[dn1 + j] = buff_arr[j] = pbuff + j * bsize;
	}

	buff_ind = 0;
	buffd = pbuff + dn1 * bsize;

/* clear buffer */
	for (i = 0; i < dn * bsize; i++) {
		pbuff[i] = 0;
	}

/* func_ind1 */
	func_ind1 = 0;
	method = mlib_ImageGetMethod(colormap);

	if (method == LUT_COLOR_CUBE_SEARCH) {
		func_ind1 = 1;
		indexsize = mlib_ImageGetIndexSize(colormap);

		if (dtype == MLIB_SHORT && indexsize == 1)
			func_ind1 = 3;
	} else
	if (method == LUT_COLOR_DIMENSIONS) {
		func_ind1 = 2;
	}

	func_ind1 *= 4;

	if (dtype == MLIB_SHORT)
		func_ind1 += 1;
	if (stype == MLIB_SHORT)
		func_ind1 += 2;

/* func_ind2 */
	func_ind2 = dm;

	if (func_ind2 > KH_MAX)
		func_ind2 = KH_MAX;
	flag3x3 = ((m - 3) | (n - 3) | (dm - 1) | (dn - 1)) == 0;

/* special case */
	if (flag3x3) {
#ifdef MLIB_VIS
		if (kern[0] == 1.0 / 16 && kern[1] == 5.0 / 16 &&
		    kern[2] == 3.0 / 16 && kern[3] == 7.0 / 16 &&
		    nchan == 3) {
#else
		if (kern[0] == 1.0 / 16 && kern[1] == 5.0 / 16 &&
		    kern[2] == 3.0 / 16 && kern[3] == 7.0 / 16) {
#endif /* MLIB_VIS */
/* FLoyd-Steinberg kernel */
			flag3x3fs = 1;
			func_ind2 = KH_MAX + 2;
		} else {
			func_ind2 = KH_MAX + 1;
		}

		dn = 0;
	}

	func_ind2 = 2 * func_ind2 + (nchan - 3);

	func_dm = func_arr[func_ind1][func_ind2];
	func_m_ptr = func_m_arr + (nchan - 3) * KW_MAX - 1;

	offset = mlib_ImageGetLutOffset(colormap);
#ifndef MLIB_VIS

	if (flag3x3fs) {
		if (stype == MLIB_SHORT)
			offset *= 2;
		lut_table =
		    (mlib_u8 *)mlib_ImageGetLutNormalTable(colormap) -
		    4 * offset;
	} else {
		lut_table =
		    (mlib_d64 *)mlib_ImageGetLutDoubleData(colormap) -
		    nchan * offset;
	}

#else /* MLIB_VIS */

	if (nchan == 3 && flag3x3 && !flag3x3fs) {
		lut_table =
		    (mlib_d64 *)mlib_ImageGetLutDoubleData(colormap) -
		    nchan * offset;
	} else {

		if (stype == MLIB_SHORT)
			offset *= 2;
		lut_table =
		    (mlib_u8 *)mlib_ImageGetLutNormalTable(colormap) -
		    4 * offset;
	}

#endif /* MLIB_VIS */

	for (row = 0; row < sh; row++) {
		buff = buff_arr + buff_ind;

		if (flag3x3) {
			buffd = (void *)sl;
		} else {
			LOAD_SRC_LINE(buffd, sl)
		}

		pkern = kern;
		for (j = 0; j < dn; j++) {
			for (off = 0; off < m; off += kw) {
				kw = m - off;

				if (kw > KW_MAX) {
					if (kw > 2 * KW_MAX)
						kw = KW_MAX;
					else
						kw = kw / 2;
				}

				func_m_ptr[kw] (buffd, buff[j] + off * NCHAN,
				    pkern + off, sw);
			}

			pkern += m;
		}

		func_dm(dl, buffd, buff[dn] + dm * NCHAN, pkern, colormap,
		    lut_table, sw, dm);

		buff_ind++;

		if (buff_ind >= dn1)
			buff_ind -= dn1;

		sl += sll;
		dl += dll;
	}

	__mlib_free(pbuff);

	if (kern != kern_lcl)
		__mlib_free(kern);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
