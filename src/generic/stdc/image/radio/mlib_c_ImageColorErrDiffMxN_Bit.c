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

#pragma ident	"@(#)mlib_c_ImageColorErrDiffMxN_Bit.c	9.3	07/10/09 SMI"

/*
 * FUNCTION
 *   Internal functions for mlib_ImageColorErrorDiffusionMxN()
 */

#include <mlib_image.h>
#include <mlib_SysMath.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageColormap.h>

/* *********************************************************** */

#define	FUNC(SUFF)	mlib_ImageColorErrorDiffusionBit_##SUFF

/* *********************************************************** */

#define	FUNC_M_ARG                                                        \
	mlib_d64 *buffd, mlib_d64 *perror, mlib_d64 *pkern, mlib_s32 sw

/* *********************************************************** */

#define	FUNC_DM_ARG                                                        \
	mlib_u8 *dl, mlib_d64 *buffd, mlib_d64 *perror, mlib_d64 *pkern,   \
	    const void *colormap, mlib_u8 *lut_table, mlib_s32 sw,         \
	    mlib_s32 dm, mlib_s32 bitoff

/* *********************************************************** */

#define	NCHAN	1

/* *********************************************************** */

void FUNC(
    m1) (
    FUNC_M_ARG)
{
	mlib_s32 i;
	mlib_d64 k0 = pkern[0];
	mlib_d64 a0;

	for (i = 0; i < sw; i++) {
		a0 = (*perror++);
		buffd[0] = buffd[0] + k0 * a0;
		buffd += NCHAN;
	}
}

/* *********************************************************** */

void FUNC(
    m2) (
    FUNC_M_ARG)
{
	mlib_s32 i;
	mlib_d64 k0 = pkern[0];
	mlib_d64 k1 = pkern[1];
	mlib_d64 a0, a1;

	a0 = (*perror++);

	for (i = 0; i < sw; i++) {
		a1 = (*perror++);
		buffd[0] = buffd[0] + k0 * a0 + k1 * a1;
		buffd += NCHAN;
		a0 = a1;
	}
}

/* *********************************************************** */

void FUNC(
    m3) (
    FUNC_M_ARG)
{
	mlib_s32 i;
	mlib_d64 k0 = pkern[0];
	mlib_d64 k1 = pkern[1];
	mlib_d64 k2 = pkern[2];
	mlib_d64 a0, a1, a2;

	a0 = (*perror++);
	a1 = (*perror++);

	for (i = 0; i < sw; i++) {
		a2 = (*perror++);
		buffd[0] = buffd[0] + k0 * a0 + k1 * a1 + k2 * a2;
		buffd += NCHAN;
		a0 = a1;
		a1 = a2;
	}
}

/* *********************************************************** */

void FUNC(
    m4) (
    FUNC_M_ARG)
{
	mlib_s32 i;
	mlib_d64 k0 = pkern[0];
	mlib_d64 k1 = pkern[1];
	mlib_d64 k2 = pkern[2];
	mlib_d64 k3 = pkern[3];
	mlib_d64 a0, a1, a2, a3;

	a0 = (*perror++);
	a1 = (*perror++);
	a2 = (*perror++);

	for (i = 0; i < sw; i++) {
		a3 = (*perror++);
		buffd[0] = buffd[0] + k0 * a0 + k1 * a1 + k2 * a2 + k3 * a3;
		buffd += NCHAN;
		a0 = a1;
		a1 = a2;
		a2 = a3;
	}
}

/* *********************************************************** */

#ifndef MLIB_USE_FTOI_CLAMPING

/* *********************************************************** */

#define	D64_T0_U8(src)                                          \
	(((src) < MLIB_U8_MIN) ? MLIB_U8_MIN : (((src) >        \
	    MLIB_U8_MAX) ? MLIB_U8_MAX : (mlib_s32)(src)))

#else /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#define	D64_T0_U8(src)	(((mlib_s32)((src)*scalef - sat)) >> 24) + 128

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#define	GET_COLOR_ERROR(ae)	ae = col0 - (mlib_d64)lut_table[col_ind]

/* *********************************************************** */

#define	DEF_VARS                                                \
	DEF_EXTRA_VARS;                                         \
	mlib_s32 i;                                             \
	mlib_s32 col_ind;                                       \
	mlib_d64 sat = 2147483648.0;                            \
	mlib_d64 scalef = (1 << 24);                            \
	mlib_d64 a;                                             \
	mlib_d64 ae

/* *********************************************************** */

#define	FUNC_DM(SUFF)                                            \
	void FUNC(t##SUFF##_dm0) (FUNC_DM_ARG)                   \
	{                                                        \
	    DEF_VARS;                                            \
	    for (i = 0; i < sw; i++) {                           \
		a = buffd[0];                                    \
		buffd += NCHAN;                                  \
		GET_COLOR_INDEX(dl, a);                          \
		GET_COLOR_ERROR(ae);                             \
		perror[0] = ae;                                  \
		perror += NCHAN;                                 \
	    }                                                    \
	}                                                        \
	                                                         \
/*                                                               \
 * ************************************************************  \
 */                                                              \
	void FUNC(t##SUFF##_dm1) (FUNC_DM_ARG)                   \
	{                                                        \
	    DEF_VARS;                                            \
	    mlib_d64 k0 = pkern[0];                              \
	                                                         \
	    ae = 0;                                              \
	    for (i = 0; i < sw; i++) {                           \
		a = buffd[0] + k0 * ae;                          \
		buffd += NCHAN;                                  \
		GET_COLOR_INDEX(dl, a);                          \
		GET_COLOR_ERROR(ae);                             \
		perror[0] = ae;                                  \
		perror += NCHAN;                                 \
	    }                                                    \
	}                                                        \
	                                                         \
/*                                                               \
 * ************************************************************  \
 */                                                              \
	void FUNC(t##SUFF##_dm) (FUNC_DM_ARG)                    \
	{                                                        \
	    DEF_VARS;                                            \
	    mlib_d64 k0, kk;                                     \
	    mlib_s32 j;                                          \
	                                                         \
	    pkern += dm - 1;                                     \
	    k0 = pkern[0];                                       \
	    a = buffd[0];                                        \
	    buffd += NCHAN;                                      \
	    for (i = 0; i < sw; i++) {                           \
		GET_COLOR_INDEX(dl, a);                          \
		GET_COLOR_ERROR(ae);                             \
		perror[0] = ae;                                  \
		perror += NCHAN;                                 \
/*                                                               \
 * j = 0                                                         \
 */                                                              \
		a = buffd[0] + k0 * ae;                          \
		for (j = 1; j < dm; j++) {                       \
		    kk = pkern[-j];                              \
		    buffd[j] += kk * ae;                         \
		}                                                \
		buffd += NCHAN;                                  \
	    }                                                    \
	}

/* *********************************************************** */

#undef  DEF_EXTRA_VARS
#define	DEF_EXTRA_VARS                                             \
	mlib_s32 mask = (lut_table[0] <= lut_table[1]) ? 0 : -1;   \
	mlib_s32 col_med = (mlib_s32)lut_table[2];                 \
	mlib_s32 col0, ii, bit_mask

/* *********************************************************** */

#undef  GET_COLOR_INDEX
#define	GET_COLOR_INDEX(dp, a)                                            \
	col0 = D64_T0_U8(a);                                              \
	col_ind = ((col_med - col0) >> 31) ^ mask;                        \
	ii = i + bitoff;                                                  \
	bit_mask = 1 << (7 - (ii & 7));                                   \
	dp[ii >> 3] = (dp[ii >> 3] & ~bit_mask) | (col_ind & bit_mask);   \
	col_ind &= 1

FUNC_DM(0)

/* *********************************************************** */

typedef void (*func_m_type) (FUNC_M_ARG);
typedef void (*func_dm_type) (FUNC_DM_ARG);

static func_m_type func_m_arr[] = {
	NULL,
	FUNC(m1),
	FUNC(m2),
	FUNC(m3),
	FUNC(m4),
};

static func_dm_type func_dm_arr[] = {
	FUNC(t0_dm0),
	FUNC(t0_dm1),
	FUNC(t0_dm)
};

/* *********************************************************** */

#define	KW_MAX	4
#define	KH_MAX	2

/* *********************************************************** */

#define	MAX_N	8
#define	MAX_M	8

mlib_status FUNC(
    MxN) (
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
	mlib_u8 *lut_table;
	mlib_s32 offset, off, kw, dn1;
	mlib_s32 schan, dchan, sll, dll, sw, sh, dw, dh;
	mlib_s32 row, i, j, bsize, buff_ind = 0, func_ind, method;
	mlib_d64 *pbuff, *buffd, *buff_lcl[2 * MAX_N], **buff_arr =
	    buff_lcl, **buff;
	mlib_d64 kern_lcl[MAX_N * MAX_M], *kern = kern_lcl, *pkern;
	mlib_d64 dscale;
	func_dm_type func_dm;
	mlib_s32 bit_offset;

	MLIB_IMAGE_GET_ALL_PARAMS(dst, dtype, dchan, dw, dh, dll, dl);
	MLIB_IMAGE_GET_ALL_PARAMS(src, stype, schan, sw, sh, sll, sl);
	bit_offset = mlib_ImageGetBitOffset(dst);

	if (!(stype == MLIB_BYTE && schan == 1)) {
		return (MLIB_FAILURE);
	}

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

	dscale = 1.0;
	while (scale > 30) {
		dscale *= 1.0 / (1 << 30);
		scale -= 30;
	}

	dscale /= (1 << scale);

	kernel += m * n - 1;
	for (i = 0; i < (m * dn + dm); i++) {
		kern[i] = dscale * kernel[-i];
	}

	offset = mlib_ImageGetLutOffset(colormap);
	lut_table = (mlib_u8 *)mlib_ImageGetLutInversTable(colormap);

	bsize = (sw + m) * NCHAN;
	dn1 = (dn) ? dn : 1;
	pbuff = __mlib_malloc((dn1 + 1) * bsize * sizeof (mlib_d64));

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

	func_ind = dm;

	if (func_ind > KH_MAX)
		func_ind = KH_MAX;
	method = mlib_ImageGetMethod(colormap);

	if (method == LUT_COLOR_CUBE_SEARCH)
		func_ind += KH_MAX + 1;
	else if (method == LUT_COLOR_DIMENSIONS)
		func_ind += 2 * (KH_MAX + 1);
	func_dm = func_dm_arr[func_ind];

	for (row = 0; row < sh; row++) {
		buff = buff_arr + buff_ind;

/* convert source line to mlib_d64 */
		for (i = 0; i < sw * NCHAN; i++) {
			buffd[i] = sl[i];
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

				func_m_arr[kw] (buffd, buff[j] + off * NCHAN,
				    pkern + off, sw);
			}

			pkern += m;
		}

		func_dm(dl, buffd, buff[dn] + dm * NCHAN, pkern, colormap,
		    lut_table, sw, dm, bit_offset);

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
