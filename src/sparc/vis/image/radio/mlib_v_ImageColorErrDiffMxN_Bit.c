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

#pragma ident	"@(#)mlib_v_ImageColorErrDiffMxN_Bit.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *   Internal functions for mlib_ImageColorErrorDiffusionMxN()
 */

#include <stdlib.h>
#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_SysMath.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageColormap.h>

/* *********************************************************** */

#define	FUNC(SUFF)	mlib_ImageColorErrorDiffusionBit_##SUFF

/* *********************************************************** */

/* #define USE_COLOR2INDEXLINE */

/* *********************************************************** */

#ifdef USE_COLOR2INDEXLINE
/*
 * void mlib_ImageColorTrue2IndexLine_U8_BIT_1(const mlib_u8 *src,
 *                                             mlib_u8       *dst,
 *                                             mlib_s32      bit_offset,
 *                                             mlib_s32      length,
 *                                             const void    *state);
 */
#define	EXTRA_BUFF	sw + 8

#else /* USE_COLOR2INDEXLINE */

#define	EXTRA_BUFF	8

#endif /* USE_COLOR2INDEXLINE */

/* *********************************************************** */

#define	FUNC_M_ARG                                                         \
	mlib_d64 *buffd, mlib_u16 *perror1, mlib_d64 *pkern, mlib_s32 sw

/* *********************************************************** */

#define	FUNC_DM_ARG                                                        \
	mlib_u8 *dl, mlib_u16 *buffd, mlib_u16 *perror, mlib_d64 *pkern,   \
	    const void *colormap, mlib_u8 *lut_table, mlib_s32 sw,         \
	    mlib_s32 dm, mlib_s32 bitoff

/* *********************************************************** */

#define	NCHAN	1

/* *********************************************************** */

#define	FMUL_16x16(x, y)                                          \
	vis_fpadd16(vis_fmul8sux16(x, y), vis_fmul8ulx16(x, y))

/* *********************************************************** */

static void FUNC(
    m1) (
    FUNC_M_ARG)
{
	mlib_s32 i;
	mlib_d64 k0 = pkern[0];
	mlib_d64 a0, e0, e1;
	mlib_d64 *perror = vis_alignaddr(perror1, 0);

	e0 = (*perror++);

	for (i = 0; i < (sw + 3) / 4; i++) {
		e1 = (*perror++);
		a0 = vis_faligndata(e0, e1);
		buffd[i] = vis_fpadd16(buffd[i], FMUL_16x16(k0, a0));
		e0 = e1;
	}
}

/* *********************************************************** */

static void FUNC(
    m2) (
    FUNC_M_ARG)
{
	mlib_s32 i;
	mlib_d64 k0 = pkern[0];
	mlib_d64 k1 = pkern[1];
	mlib_d64 a0, a1, aa, sum;
	mlib_d64 *perror = vis_alignaddr(perror1, 0);

	a0 = (*perror++);

	for (i = 0; i < (sw + 3) / 4; i++) {
		aa = (*perror++);
		a1 = vis_faligndata(a0, aa);
		sum = vis_fpadd16(buffd[i], FMUL_16x16(k0, a0));
		buffd[i] = vis_fpadd16(sum, FMUL_16x16(k1, a1));
		a0 = aa;
	}
}

/* *********************************************************** */

static void FUNC(
    m3) (
    FUNC_M_ARG)
{
	mlib_s32 i;
	mlib_d64 k0 = pkern[0];
	mlib_d64 k1 = pkern[1];
	mlib_d64 k2 = pkern[2];
	mlib_d64 a0, a1, a2, aa, sum;
	mlib_d64 *perror = vis_alignaddr(perror1, 0);

	a0 = (*perror++);

	for (i = 0; i < sw; i++) {
		aa = (*perror++);
		a1 = vis_faligndata(a0, aa);
		a2 = vis_faligndata(a1, vis_faligndata(aa, aa));
		sum = vis_fpadd16(buffd[i], FMUL_16x16(k0, a0));
		sum = vis_fpadd16(sum, FMUL_16x16(k1, a1));
		buffd[i] = vis_fpadd16(sum, FMUL_16x16(k2, a2));
		a0 = aa;
	}
}

/* *********************************************************** */

static void FUNC(
    m4) (
    FUNC_M_ARG)
{
	mlib_s32 i;
	mlib_d64 k0 = pkern[0];
	mlib_d64 k1 = pkern[1];
	mlib_d64 k2 = pkern[2];
	mlib_d64 k3 = pkern[3];
	mlib_d64 a0, a1, a2, a3, sum;
	mlib_d64 *perror = vis_alignaddr(perror1, 0);

	a0 = (*perror++);
	a1 = (*perror++);
	a2 = (*perror++);

	for (i = 0; i < sw; i++) {
		a3 = (*perror++);
		sum = vis_fpadd16(buffd[i], FMUL_16x16(k0, a0));
		sum = vis_fpadd16(sum, FMUL_16x16(k1, a1));
		sum = vis_fpadd16(sum, FMUL_16x16(k2, a2));
		buffd[i] = vis_fpadd16(sum, FMUL_16x16(k3, a3));
		a0 = a1;
		a1 = a2;
		a2 = a3;
	}
}

/* *********************************************************** */

#define	LD_U8(pp, i)	vis_ld_u8((mlib_u8*)pp + (i))

#define	LD_U16(pp, i)	vis_ld_u16((mlib_u16*)pp + (i))

#define	ST_U16(pp, i, val)	vis_st_u16(val, (mlib_u16*)pp + (i))

/* *********************************************************** */

#define	GET_COLOR_ERROR(ae)                                     \
	cc = LD_U8(lut_table, col_ind);                         \
	lut_color = vis_read_lo(cc);                            \
	d0 = vis_fmul8x16al(dst_color, minus_half);             \
	d1 = vis_fmul8x16al(lut_color, minus_half);             \
	ae = vis_fpsub16(d1, d0)

/* *********************************************************** */

#define	DEF_VARS                                                \
	DEF_EXTRA_VARS mlib_s32 i;                              \
	mlib_s32 col_ind;                                       \
	mlib_f32 dst_color, lut_color;                          \
	mlib_f32 minus_half = vis_to_float(0x8000);             \
	mlib_d64 a, ae, d0, d1, cc

/* *********************************************************** */

#define	FUNC_DM(SUFF)                                            \
	static void FUNC(t##SUFF##_dm0) (FUNC_DM_ARG)            \
	{                                                        \
	    DEF_VARS;                                            \
	    for (i = 0; i < sw; i++) {                           \
		a = LD_U16(buffd, i);                            \
		GET_COLOR_INDEX(dl, a);                          \
		GET_COLOR_ERROR(ae);                             \
		ST_U16(perror, i, ae);                           \
	    }                                                    \
	}                                                        \
	                                                         \
/*                                                               \
 * ************************************************************  \
 */                                                              \
	static void FUNC(t##SUFF##_dm1) (FUNC_DM_ARG)            \
	{                                                        \
	    DEF_VARS;                                            \
	    mlib_d64 k0 = pkern[0];                              \
	                                                         \
	    ae = 0;                                              \
	    for (i = 0; i < sw; i++) {                           \
		a = LD_U16(buffd, i);                            \
		a = vis_fpadd16(a, FMUL_16x16(k0, ae));          \
		GET_COLOR_INDEX(dl, a);                          \
		GET_COLOR_ERROR(ae);                             \
		ST_U16(perror, i, ae);                           \
	    }                                                    \
	}                                                        \
	                                                         \
/*                                                               \
 * ************************************************************  \
 */                                                              \
	static void FUNC(t##SUFF##_dm) (FUNC_DM_ARG)             \
	{                                                        \
	    DEF_VARS;                                            \
	    mlib_d64 k0, kk, dd;                                 \
	    mlib_s32 j;                                          \
	                                                         \
	    pkern += dm - 1;                                     \
	    k0 = pkern[0];                                       \
	    a = LD_U16(buffd, 0);                                \
	    buffd++;                                             \
	    for (i = 0; i < sw; i++) {                           \
		GET_COLOR_INDEX(dl, a);                          \
		GET_COLOR_ERROR(ae);                             \
		ST_U16(perror, i, ae);                           \
/*                                                               \
 * j = 0                                                         \
 */                                                              \
		a = LD_U16(buffd, 0);                            \
		a = vis_fpadd16(a, FMUL_16x16(k0, ae));          \
		for (j = 1; j < dm; j++) {                       \
		    kk = pkern[-j];                              \
		    dd = LD_U16(buffd, j);                       \
		    dd = vis_fpadd16(dd, FMUL_16x16(kk, ae));    \
		    ST_U16(buffd, j, dd);                        \
		}                                                \
		buffd++;                                         \
	    }                                                    \
	}

/* *********************************************************** */

#ifdef USE_COLOR2INDEXLINE

/* *********************************************************** */

#undef  DEF_EXTRA_VARS
#define	DEF_EXTRA_VARS                                          \
	mlib_s32 mask = (lut_table[1] - lut_table[0]) >> 31;    \
	mlib_s32 col_med = (mlib_s32)lut_table[2];              \
	mlib_s32 col0;

/* *********************************************************** */

#undef  GET_COLOR_INDEX
#define	GET_COLOR_INDEX(dp, a)                                  \
	dst_color = vis_fpack16(a);                             \
	col0 = ((mlib_u8 *)&dst_color)[3];                      \
	col_ind = ((col_med - col0) >> 31) ^ mask;              \
	col_ind &= 1;                                           \
	(*dp++) = col0

#else /* USE_COLOR2INDEXLINE */

/* *********************************************************** */

#undef  DEF_EXTRA_VARS
#define	DEF_EXTRA_VARS                                          \
	mlib_s32 mask = (lut_table[1] - lut_table[0]) >> 31;    \
	mlib_s32 col_med = (mlib_s32)lut_table[2];              \
	mlib_s32 col0, ii, dval, bit_mask;

/* *********************************************************** */

#undef  GET_COLOR_INDEX
#define	GET_COLOR_INDEX(dp, a)                                            \
	dst_color = vis_fpack16(a);                                       \
	col0 = ((mlib_u8 *)&dst_color)[3];                                \
	col_ind = ((col_med - col0) >> 31) ^ mask;                        \
	ii = i + bitoff;                                                  \
	bit_mask = 1 << (7 - (ii & 7));                                   \
	dp[ii >> 3] = (dp[ii >> 3] & ~bit_mask) | (col_ind & bit_mask);   \
	col_ind &= 1

#endif /* USE_COLOR2INDEXLINE */

FUNC_DM(0)

/* *********************************************************** */

typedef void (*func_m_type) (FUNC_M_ARG);
typedef void (*func_dm_type) (FUNC_DM_ARG);

static const func_m_type func_m_arr[] = {
	NULL,
	FUNC(m1),
	FUNC(m2),
	FUNC(m3),
	FUNC(m4),
};

static const func_dm_type func_dm_arr[] = {
	FUNC(t0_dm0),
	FUNC(t0_dm1),
	FUNC(t0_dm)
};

/* *********************************************************** */

#define	KW_MAX	1
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
	mlib_u16 *pbuff, *buff_lcl[2 * MAX_N], **buff_arr = buff_lcl, **buff;
	mlib_d64 *buffd;
	mlib_d64 kern_lcl[MAX_N * MAX_M], *kern = kern_lcl, *pkern;
	mlib_d64 dscale;
	func_dm_type func_dm;

	mlib_s32 vis_scale, kern_i;
	mlib_s32 kern_size, isum;
	mlib_d64 sum, norm;
	mlib_f32 fscale;
	mlib_s32 bit_offset;
	mlib_u8 *buff_dst;

	MLIB_IMAGE_GET_ALL_PARAMS(dst, dtype, dchan, dw, dh, dll, dl);
	MLIB_IMAGE_GET_ALL_PARAMS(src, stype, schan, sw, sh, sll, sl);
	bit_offset = mlib_ImageGetBitOffset(dst);

	if (!(stype == MLIB_BYTE && schan == 1)) {
		return (MLIB_FAILURE);
	}
#if 0
	for (i = 0; i <= m * dn + dm; i++) {
		if (kernel[i])
			return (MLIB_FAILURE);
	}

#endif /* 0 */

	dn = n - 1 - dn;
	dm = m - 1 - dm;
	kern_size = m * dn + dm;

	if (n > MAX_N || m > MAX_M) {
		kern =
		    __mlib_malloc(n * m * sizeof (mlib_d64) +
		    2 * n * sizeof (mlib_u16 *));

		if (kern == NULL)
			return (MLIB_FAILURE);
		buff_arr = (mlib_u16 **)(kern + n * m);
	}

	dscale = 1.0;
	while (scale > 30) {
		dscale *= 1.0 / (1 << 30);
		scale -= 30;
	}

	dscale /= (1 << scale);

/* load kernel */
	kernel += m * n - 1;
	sum = 0;
	for (i = 0; i < kern_size; i++) {
		kern[i] = dscale * kernel[-i];
		sum += mlib_fabs(kern[i]);
	}

	vis_scale = mlib_ilogb(sum);

	if (vis_scale > 13)
		return (MLIB_OUTOFRANGE);
	vis_scale = 14 - vis_scale;

	if (vis_scale > 15)
		vis_scale = 15;
	norm = 32768 >> (15 - vis_scale);
	isum = 0;
	for (i = 0; i < kern_size; i++) {
		if (kern[i] > 0.0) {
			kern_i = (mlib_s32)(kern[i] * norm + 0.5);
		} else {
			kern_i = (mlib_s32)(kern[i] * norm - 0.5);
		}

		isum += abs(kern_i);
		kern[i] = vis_to_double_dup((kern_i << 16) | (kern_i & 0xffff));
	}

/* recalc without rounding */
	if (isum > 32767) {
		dscale *= norm;
		for (i = 0; i < kern_size; i++) {
			kern_i = (mlib_s32)(dscale * kernel[-i]);
			kern[i] =
			    vis_to_double_dup((kern_i << 16) | (kern_i &
			    0xffff));
		}
	}

	fscale = vis_to_float(1 << (vis_scale - 1));
	vis_write_gsr(((16 - vis_scale) << 3) + 2);

	offset = mlib_ImageGetLutOffset(colormap);
	lut_table = (mlib_u8 *)mlib_ImageGetLutInversTable(colormap);

	bsize = (sw + m) * NCHAN;
	bsize = (bsize + 7) & ~7;
	dn1 = (dn) ? dn : 1;
	pbuff =
	    __mlib_malloc((dn1 + 1) * bsize * sizeof (mlib_u16) + EXTRA_BUFF);

	if (pbuff == NULL) {
		if (kern != kern_lcl)
			__mlib_free(kern);
		return (MLIB_FAILURE);
	}

	for (j = 0; j < dn1; j++) {
		buff_arr[dn1 + j] = buff_arr[j] = pbuff + j * bsize;
	}

	buff_ind = 0;
	buffd = (mlib_d64 *)(pbuff + dn1 * bsize);
	buff_dst = (mlib_u8 *)((mlib_u16 *)buffd + bsize);

/* clear buffer */
	for (i = 0; i < dn * (bsize / 4); i++) {
		((mlib_d64 *)pbuff)[i] = 0;
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
		mlib_u8 *sp = sl;

		buff = buff_arr + buff_ind;

/* convert source line */
		for (i = 0; i < sw; i++) {
			mlib_d64 ss;

			ss = LD_U8(sp, i);
			ss = vis_fmul8x16al(vis_read_lo(ss), fscale);
			ST_U16(buffd, i, ss);
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

#ifdef USE_COLOR2INDEXLINE
		func_dm(buff_dst, (void *)buffd, buff[dn] + dm * NCHAN, pkern,
		    colormap, lut_table, sw, dm, 0);
/*
 * mlib_ImageColorTrue2IndexLine_U8_BIT_1
 * (buff_dst, dl, bit_offset, sw, colormap);
 */
#else /* USE_COLOR2INDEXLINE */
		func_dm(dl, (void *)buffd, buff[dn] + dm * NCHAN, pkern,
		    colormap, lut_table, sw, dm, bit_offset);
#endif /* USE_COLOR2INDEXLINE */

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
