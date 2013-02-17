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

#pragma ident	"@(#)mlib_v_ImageColorErrDiffMxN_U8.c	9.2	07/11/05 SMI"

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

#define	FUNC(SUFF)	mlib_ImageColorErrorDiffusionU8_##SUFF

/* *********************************************************** */

#define	FUNC_M_ARG                                                        \
	mlib_d64 *buffd, mlib_d64 *perror, mlib_d64 *pkern, mlib_s32 sw

/* *********************************************************** */

#define	FUNC_DM_ARG                                                     \
	void *dl, mlib_d64 *buffd, mlib_d64 *perror, mlib_d64 *pkern,   \
	const void *colormap, mlib_f32 *lut_table, mlib_s32 sw,         \
	mlib_s32 dm

/* *********************************************************** */

#define	NCHAN	1

/* *********************************************************** */

#define	FMUL_16x16(x, y)                                          \
	vis_fpadd16(vis_fmul8sux16(x, y), vis_fmul8ulx16(x, y))

/* *********************************************************** */

static void FUNC(m1) (
    FUNC_M_ARG)
{
	mlib_s32 i;
	mlib_d64 k0 = pkern[0];
	mlib_d64 a0;

	for (i = 0; i < sw; i++) {
		a0 = perror[i];
		buffd[i] = vis_fpadd16(buffd[i], FMUL_16x16(k0, a0));
	}
}

/* *********************************************************** */

static void FUNC(m2) (
    FUNC_M_ARG)
{
	mlib_s32 i;
	mlib_d64 k0 = pkern[0];
	mlib_d64 k1 = pkern[1];
	mlib_d64 a0, a1, sum;

	a0 = (*perror++);

	for (i = 0; i < sw; i++) {
		a1 = (*perror++);
		sum = vis_fpadd16(buffd[i], FMUL_16x16(k0, a0));
		buffd[i] = vis_fpadd16(sum, FMUL_16x16(k1, a1));
		a0 = a1;
	}
}

/* *********************************************************** */

static void FUNC(m3) (
    FUNC_M_ARG)
{
	mlib_s32 i;
	mlib_d64 k0 = pkern[0];
	mlib_d64 k1 = pkern[1];
	mlib_d64 k2 = pkern[2];
	mlib_d64 a0, a1, a2, sum;

	a0 = (*perror++);
	a1 = (*perror++);

	for (i = 0; i < sw; i++) {
		a2 = (*perror++);
		sum = vis_fpadd16(buffd[i], FMUL_16x16(k0, a0));
		sum = vis_fpadd16(sum, FMUL_16x16(k1, a1));
		buffd[i] = vis_fpadd16(sum, FMUL_16x16(k2, a2));
		a0 = a1;
		a1 = a2;
	}
}

/* *********************************************************** */

static void FUNC(m4) (
    FUNC_M_ARG)
{
	mlib_s32 i;
	mlib_d64 k0 = pkern[0];
	mlib_d64 k1 = pkern[1];
	mlib_d64 k2 = pkern[2];
	mlib_d64 k3 = pkern[3];
	mlib_d64 a0, a1, a2, a3, sum;

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

typedef void (*func_m_type) (FUNC_M_ARG);
typedef void (*func_dm_type) (FUNC_DM_ARG);

static const func_m_type func_m_arr[] = {
	FUNC(m1),
	FUNC(m2),
	FUNC(m3),
	FUNC(m4),
	FUNC(m1),
	FUNC(m2),
	FUNC(m3),
	FUNC(m4),
};

/* *********************************************************** */

#define	DEF_ARR_3(SUFF, STYPE, DTYPE)

/* *********************************************************** */

#define	DEF_ARR_4(SUFF, STYPE, DTYPE)                     \
	static const func_dm_type                         \
	    func_arr_##STYPE##_##DTYPE##_t##SUFF[] =      \
	{                                                 \
	    FUNC(ch3_##STYPE##_##DTYPE##_t##SUFF##_dm0),  \
	    FUNC(ch4_##STYPE##_##DTYPE##_t##SUFF##_dm0),  \
	    FUNC(ch3_##STYPE##_##DTYPE##_t##SUFF##_dm1),  \
	    FUNC(ch4_##STYPE##_##DTYPE##_t##SUFF##_dm1),  \
	    FUNC(ch3_##STYPE##_##DTYPE##_t##SUFF##_dm),   \
	    FUNC(ch4_##STYPE##_##DTYPE##_t##SUFF##_dm),   \
	    FUNC(ch3_##STYPE##_##DTYPE##_t##SUFF##_3x3),  \
	    FUNC(ch4_##STYPE##_##DTYPE##_t##SUFF##_3x3),  \
	    FUNC(ch3_##STYPE##_##DTYPE##_t##SUFF##_3x3),  \
	    FUNC(ch4_##STYPE##_##DTYPE##_t##SUFF##_3x3)   \
	};

/* *********************************************************** */

#define	GET_COLOR_ERROR(ae)                                     \
	lut_color = lut_table[col_ind];                         \
	d0 = vis_fmul8x16al(dst_color, minus_half);             \
	d1 = vis_fmul8x16al(lut_color, minus_half);             \
	ae = vis_fpsub16(d1, d0)

/* *********************************************************** */

#define	DEF_EXTRA_VARS_CH3(STYPE, DTYPE)	DEF_EXTRA_VARS(STYPE, DTYPE)

/* *********************************************************** */

#define	DEF_EXTRA_VARS_CH4(STYPE, DTYPE)                             \
	DEF_EXTRA_VARS(STYPE, DTYPE) DEF_EXTRA_VARS4(STYPE, DTYPE)

/* *********************************************************** */

#define	DEF_VARS(STYPE, DTYPE, CH)                          \
	DEF_EXTRA_VARS_CH##CH(STYPE, DTYPE)                 \
	mlib_##DTYPE *dp = (void *)dl;                      \
	mlib_s32 i;                                         \
	mlib_s32 col_ind;                                   \
	mlib_f32 dst_color, lut_color;                      \
	mlib_f32 minus_half = vis_to_float(0x8000);         \
	mlib_d64 a, ae, d0, d1

/* *********************************************************** */

#define	FUNC_DM(SUFF, STYPE, DTYPE, CH)                                  \
	static void                                                      \
	FUNC(ch##CH##_##STYPE##_##DTYPE##_t##SUFF##_dm0) (FUNC_DM_ARG)   \
	{                                                                \
	    DEF_VARS(STYPE, DTYPE, CH);                                  \
	    for (i = 0; i < sw; i++) {                                   \
		a = buffd[i];                                            \
		GET_COLOR_INDEX##CH(STYPE, DTYPE);                       \
		GET_COLOR_ERROR(ae);                                     \
		dp++;                                                    \
		perror[i] = ae;                                          \
	    }                                                            \
	}                                                                \
	                                                                 \
/*                                                                       \
 * ************************************************************          \
 */                                                                      \
	static void                                                      \
	FUNC(ch##CH##_##STYPE##_##DTYPE##_t##SUFF##_dm1) (FUNC_DM_ARG)   \
	{                                                                \
	    DEF_VARS(STYPE, DTYPE, CH);                                  \
	    mlib_d64 k0 = pkern[0];                                      \
	                                                                 \
	    ae = 0;                                                      \
	    for (i = 0; i < sw; i++) {                                   \
		a = vis_fpadd16(buffd[i], FMUL_16x16(k0, ae));           \
		GET_COLOR_INDEX##CH(STYPE, DTYPE);                       \
		GET_COLOR_ERROR(ae);                                     \
		dp++;                                                    \
		perror[i] = ae;                                          \
	    }                                                            \
	}                                                                \
	                                                                 \
/*                                                                       \
 * ************************************************************          \
 */                                                                      \
	static void                                                      \
	FUNC(ch##CH##_##STYPE##_##DTYPE##_t##SUFF##_dm2) (FUNC_DM_ARG)   \
	{                                                                \
	    DEF_VARS(STYPE, DTYPE, CH);                                  \
	    mlib_d64 k0 = pkern[0];                                      \
	    mlib_d64 k1 = pkern[1];                                      \
	    mlib_d64 e0, e1;                                             \
	                                                                 \
	    ae = 0;                                                      \
	    e0 = e1 = ae;                                                \
	    for (i = 0; i < sw; i++) {                                   \
		a = buffd[i];                                            \
		a = vis_fpadd16(a, FMUL_16x16(k0, e0));                  \
		a = vis_fpadd16(a, FMUL_16x16(k1, e1));                  \
		e0 = e1;                                                 \
		GET_COLOR_INDEX##CH(STYPE, DTYPE);                       \
		dp++;                                                    \
		GET_COLOR_ERROR(e1);                                     \
		perror[i] = e1;                                          \
	    }                                                            \
	}                                                                \
	                                                                 \
/*                                                                       \
 * ************************************************************          \
 */                                                                      \
	static void                                                      \
	FUNC(ch##CH##_##STYPE##_##DTYPE##_t##SUFF##_dm) (FUNC_DM_ARG)    \
	{                                                                \
	    DEF_VARS(STYPE, DTYPE, CH);                                  \
	    mlib_d64 k0, kk;                                             \
	    mlib_s32 j;                                                  \
	                                                                 \
	    pkern += dm - 1;                                             \
	    k0 = pkern[0];                                               \
	    a = (*buffd++);                                              \
	    for (i = 0; i < sw; i++) {                                   \
		GET_COLOR_INDEX##CH(STYPE, DTYPE);                       \
		GET_COLOR_ERROR(ae);                                     \
		dp++;                                                    \
		perror[i] = ae;                                          \
/*                                                                       \
 * j = 0                                                                 \
 */                                                                      \
		a = vis_fpadd16(buffd[0], FMUL_16x16(k0, ae));           \
		for (j = 1; j < dm; j++) {                               \
		    kk = pkern[-j];                                      \
		    buffd[j] =                                           \
			vis_fpadd16(buffd[j], FMUL_16x16(kk, ae));       \
		}                                                        \
		buffd += 1;                                              \
	    }                                                            \
	}                                                                \
	                                                                 \
/*                                                                       \
 * ************************************************************          \
 */                                                                      \
	static void                                                      \
	FUNC(ch##CH##_##STYPE##_##DTYPE##_t##SUFF##_3x3) (FUNC_DM_ARG)   \
	{                                                                \
	    DEF_VARS(STYPE, DTYPE, CH);                                  \
	    mlib_d64 k0 = pkern[0];                                      \
	    mlib_d64 k1 = pkern[1];                                      \
	    mlib_d64 k2 = pkern[2];                                      \
	    mlib_d64 kk = pkern[3];                                      \
	    mlib_d64 a0, a1, a2, r1, r2;                                 \
	    mlib_u8 *sp = (void *)buffd;                                 \
	    mlib_f32 fscale = *(mlib_f32 *)(pkern + 4);                  \
	                                                                 \
	    ae = 0;                                                      \
	    a0 = 0;                                                      \
	    a1 = perror[0];                                              \
	    for (i = 0; i < sw; i++) {                                   \
		a2 = perror[1];                                          \
		GET_SRC##CH(a);                                          \
		r1 = FMUL_16x16(k0, a0);                                 \
		r2 = FMUL_16x16(k1, a1);                                 \
		r1 = vis_fpadd16(r1, FMUL_16x16(k2, a2));                \
		r2 = vis_fpadd16(r2, FMUL_16x16(kk, ae));                \
		a = vis_fpadd16(a, vis_fpadd16(r1, r2));                 \
		a0 = a1;                                                 \
		a1 = a2;                                                 \
		GET_COLOR_INDEX##CH(STYPE, DTYPE);                       \
		GET_COLOR_ERROR(ae);                                     \
		dp++;                                                    \
		(*perror++) = ae;                                        \
	    }                                                            \
	}                                                                \
	                                                                 \
/*                                                                       \
 * ************************************************************          \
 */                                                                      \
	DEF_ARR_##CH(SUFF, STYPE, DTYPE)

/* *********************************************************** */

#define	mlib_ImageColorTrue2IndexLine_u8_u8_3           \
		mlib_ImageColorTrue2IndexLine_U8_U8_3
#define	mlib_ImageColorTrue2IndexLine_u8_u8_4           \
		mlib_ImageColorTrue2IndexLine_U8_U8_4

#define	mlib_ImageColorTrue2IndexLine_u8_s16_3          \
		mlib_ImageColorTrue2IndexLine_U8_S16_3

#define	mlib_ImageColorTrue2IndexLine_u8_s16_4          \
		mlib_ImageColorTrue2IndexLine_U8_S16_4

#define	mlib_ImageColorTrue2IndexLine_s16_u8_3          \
		mlib_ImageColorTrue2IndexLine_S16_U8_3

#define	mlib_ImageColorTrue2IndexLine_s16_u8_4          \
		mlib_ImageColorTrue2IndexLine_S16_U8_4

#define	mlib_ImageColorTrue2IndexLine_s16_s16_3         \
		mlib_ImageColorTrue2IndexLine_S16_S16_3

#define	mlib_ImageColorTrue2IndexLine_s16_s16_4         \
		mlib_ImageColorTrue2IndexLine_S16_S16_4

/* *********************************************************** */

#define	GET_SRC3(ss)                                            \
	ss = vis_faligndata(vis_ld_u8(sp + 2), ss);             \
	ss = vis_faligndata(vis_ld_u8(sp + 1), ss);             \
	ss = vis_faligndata(vis_ld_u8(sp), ss);                 \
	ss = vis_faligndata(ss, ss);                            \
	sp += 3;                                                \
	ss = vis_fmul8x16al(vis_read_hi(ss), fscale)

/* *********************************************************** */

#define	GET_SRC4(ss)                                            \
	ss = vis_faligndata(vis_ld_u8(sp + 3), ss);             \
	ss = vis_faligndata(vis_ld_u8(sp + 2), ss);             \
	ss = vis_faligndata(vis_ld_u8(sp + 1), ss);             \
	ss = vis_faligndata(vis_ld_u8(sp), ss);                 \
	sp += 4;                                                \
	ss = vis_fmul8x16al(vis_read_hi(ss), fscale)

/* *********************************************************** */

#define	DEF_EXTRA_VARS(STYPE, DTYPE)

#define	DEF_EXTRA_VARS4(STYPE, DTYPE)

/* *********************************************************** */

#define	GET_COLOR_INDEX3(STYPE, DTYPE)                             \
	dst_color = vis_fpack16(a);                                \
	mlib_ImageColorTrue2IndexLine_##STYPE##_##DTYPE##_3        \
	    ((mlib_##STYPE *) & dst_color + 1, dp, 1, colormap);   \
	col_ind = *dp

/* *********************************************************** */

#define	GET_COLOR_INDEX4(STYPE, DTYPE)                          \
	dst_color = vis_fpack16(a);                             \
	mlib_ImageColorTrue2IndexLine_##STYPE##_##DTYPE##_4     \
	    ((mlib_##STYPE *) & dst_color, dp, 1, colormap);    \
	col_ind = *dp

/* *********************************************************** */

FUNC_DM(0, u8, u8, 3)
FUNC_DM(0, u8, u8, 4)
FUNC_DM(0, u8, s16, 3)
FUNC_DM(0, u8, s16, 4)

/* *********************************************************** */

#undef  DEF_EXTRA_VARS
#define	DEF_EXTRA_VARS(STYPE, DTYPE)                                  \
	mlib_s32 i_color;                                             \
	mlib_##DTYPE *inv_table =                                     \
	    (mlib_##DTYPE *) mlib_ImageGetLutInversTable(colormap);   \
	mlib_s32 bits = mlib_ImageGetBits(colormap);                  \
	mlib_s32 shift0 = 8 - bits;                                   \
	mlib_s32 shift1 = 2 * shift0;                                 \
	mlib_s32 shift2 = shift0 + shift1;                            \
	mlib_s32 mask0 = (1 << bits) - 1;                             \
	mlib_s32 mask1 = (mask0 << bits);                             \
	mlib_s32 mask2 = (mask1 << bits);

/* *********************************************************** */

#undef  DEF_EXTRA_VARS4
#define	DEF_EXTRA_VARS4(STYPE, DTYPE)                           \
	mlib_s32 shift3 = 2 * shift1;                           \
	mlib_s32 mask3 = (mask2 << bits);

/* *********************************************************** */

#undef  GET_COLOR_INDEX3
#define	GET_COLOR_INDEX3(STYPE, DTYPE)                          \
	dst_color = vis_fpack16(a);                             \
	i_color = *(mlib_s32 *)&dst_color;                      \
	col_ind =                                               \
	    ((i_color >> shift0) & mask0) +                     \
	    ((i_color >> shift1) & mask1) +                     \
	    ((i_color >> shift2) & mask2);                      \
	col_ind = inv_table[col_ind];                           \
	*dp = col_ind

/* *********************************************************** */

#undef  GET_COLOR_INDEX4
#define	GET_COLOR_INDEX4(STYPE, DTYPE)                          \
	dst_color = vis_fpack16(a);                             \
	i_color = *(mlib_s32 *)&dst_color;                      \
	col_ind =                                               \
	    ((i_color >> shift0) & mask0) +                     \
	    ((i_color >> shift1) & mask1) +                     \
	    ((i_color >> shift2) & mask2) +                     \
	    ((i_color >> shift3) & mask3);                      \
	col_ind = inv_table[col_ind];                           \
	*dp = col_ind

/* *********************************************************** */

FUNC_DM(1, u8, u8, 3)
FUNC_DM(1, u8, u8, 4)
FUNC_DM(1, u8, s16, 3)
FUNC_DM(1, u8, s16, 4)

/* *********************************************************** */
#undef  DEF_EXTRA_VARS
#define	DEF_EXTRA_VARS(STYPE, DTYPE)                            \
	mlib_s32 i_color;                                       \
	mlib_u8 *inv_table =                                    \
	    (mlib_u8 *)mlib_ImageGetLutInversTable(colormap);   \
	mlib_s32 bits = mlib_ImageGetBits(colormap);            \
	mlib_s32 shift0 = 8 - bits;                             \
	mlib_s32 shift1 = 2 * shift0;                           \
	mlib_s32 shift2 = shift0 + shift1;                      \
	mlib_s32 mask0 = (1 << bits) - 1;                       \
	mlib_s32 mask1 = (mask0 << bits);                       \
	mlib_s32 mask2 = (mask1 << bits);

/* *********************************************************** */

FUNC_DM(1s, u8, s16, 3)
FUNC_DM(1s, u8, s16, 4)

/* *********************************************************** */

#define	CONV_u8(x)	(x)
#define	CONV_s16(x)	((x >> 8) & 0xFF)

/* *********************************************************** */

#undef  DEF_EXTRA_VARS
#define	DEF_EXTRA_VARS(STYPE, DTYPE)                            \
	mlib_##DTYPE *tab0 =                                    \
	    (void *)mlib_ImageGetLutInversTable(colormap);      \
	mlib_##DTYPE *tab1 = tab0 + 256;                        \
	mlib_##DTYPE *tab2 = tab1 + 256;                        \
	mlib_s32 col0, col1, col2;

/* *********************************************************** */

#undef  DEF_EXTRA_VARS4
#define	DEF_EXTRA_VARS4(STYPE, DTYPE)                           \
	mlib_##DTYPE *tab3 = tab2 + 256;                        \
	mlib_s32 col3;

/* *********************************************************** */

#undef  GET_COLOR_INDEX3
#define	GET_COLOR_INDEX3(STYPE, DTYPE)                             \
	dst_color = vis_fpack16(a);                                \
	col0 = ((mlib_u8 *)&dst_color)[1];                         \
	col1 = ((mlib_u8 *)&dst_color)[2];                         \
	col2 = ((mlib_u8 *)&dst_color)[3];                         \
	col_ind =                                                  \
	    tab0[CONV_##STYPE(col0)] +                             \
	    tab1[CONV_##STYPE(col1)] + tab2[CONV_##STYPE(col2)];   \
	*dp = col_ind

/* *********************************************************** */

#undef  GET_COLOR_INDEX4
#define	GET_COLOR_INDEX4(STYPE, DTYPE)                             \
	dst_color = vis_fpack16(a);                                \
	col0 = ((mlib_u8 *)&dst_color)[0];                         \
	col1 = ((mlib_u8 *)&dst_color)[1];                         \
	col2 = ((mlib_u8 *)&dst_color)[2];                         \
	col3 = ((mlib_u8 *)&dst_color)[3];                         \
	col_ind =                                                  \
	    tab0[CONV_##STYPE(col0)] +                             \
	    tab1[CONV_##STYPE(col1)] +                             \
	    tab2[CONV_##STYPE(col2)] + tab3[CONV_##STYPE(col3)];   \
	*dp = col_ind

/* *********************************************************** */

FUNC_DM(2, u8, u8, 3)
FUNC_DM(2, u8, u8, 4)
FUNC_DM(2, u8, s16, 3)
FUNC_DM(2, u8, s16, 4)

/* *********************************************************** */

static const func_dm_type *func_arr[] = {
	func_arr_u8_u8_t0,
	func_arr_u8_s16_t0,
	NULL,
	NULL,

	func_arr_u8_u8_t1,
	func_arr_u8_s16_t1,
	NULL,
	NULL,

	func_arr_u8_u8_t2,
	func_arr_u8_s16_t2,
	NULL,
	NULL,

	NULL,
	func_arr_u8_s16_t1s,
	NULL,
	NULL
};

/* *********************************************************** */

static mlib_status FUNC(
    Bit_MxN) (
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kernel,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 dm,
    mlib_s32 dn,
    mlib_s32 scale,
    const void *colormap);

/* *********************************************************** */

#define	KW_MAX	4
#define	KH_MAX	2

/* *********************************************************** */

#define	MAX_N	8
#define	MAX_M	8

/* *********************************************************** */

#define	LOAD_KERNEL(kern, kernel)                                      \
	{                                                              \
	    mlib_s32 vis_scale, kern_i;                                \
	    mlib_s32 isum;                                             \
	    mlib_d64 sum, norm;                                        \
	    mlib_s32 kern_size = m * dn + dm;                          \
	                                                               \
	    kernel += m * n - 1;                                       \
	    sum = 0;                                                   \
	    for (i = 0; i < kern_size; i++) {                          \
		kern[i] = dscale * kernel[-i];                         \
		sum += mlib_fabs(kern[i]);                             \
	    }                                                          \
	    vis_scale = mlib_ilogb(sum);                               \
	    if (vis_scale > 13)                                        \
		return (MLIB_OUTOFRANGE);                              \
	    vis_scale = 14 - vis_scale;                                \
	    if (vis_scale > 15)                                        \
		vis_scale = 15;                                        \
	    norm = 32768 >> (15 - vis_scale);                          \
	    isum = 0;                                                  \
	    for (i = 0; i < kern_size; i++) {                          \
		if (kern[i] > 0.0) {                                   \
		    kern_i = (mlib_s32)(kern[i] * norm + 0.5);         \
		} else {                                               \
		    kern_i = (mlib_s32)(kern[i] * norm - 0.5);         \
		}                                                      \
		isum += abs(kern_i);                                   \
		kern[i] =                                              \
		    vis_to_double_dup((kern_i << 16) | (kern_i &       \
		    0xffff));                                          \
	    }                                                          \
	    if (isum > 32767) {                                        \
	                                                               \
/*                                                                     \
 * recalc without rounding                                             \
 */                                                                    \
		dscale *= norm;                                        \
		for (i = 0; i < kern_size; i++) {                      \
		    kern_i = (mlib_s32)(dscale * kernel[-i]);          \
		    kern[i] =                                          \
			vis_to_double_dup((kern_i << 16) | (kern_i &   \
			0xffff));                                      \
		}                                                      \
	    }                                                          \
	    vis_write_gsr(((16 - vis_scale) << 3) + 7);                \
	    fscale = vis_to_float(1 << (vis_scale - 1));               \
	    if (flag3x3 == 0)                                          \
		*(mlib_f32 *)(kern + 4) = fscale;                      \
	}

/* *********************************************************** */

#define	LOAD_SRC_LINE(buffd, sl)                                \
	{                                                       \
	    mlib_u8 *sp = sl;                                   \
	    mlib_d64 ss;                                        \
	                                                        \
/*                                                              \
 * convert source line                                          \
 */                                                             \
	    if (nchan == 3) {                                   \
		for (i = 0; i < sw; i++) {                      \
		    GET_SRC3(ss);                               \
		    buffd[i] = ss;                              \
		}                                               \
	    } else {                                            \
		for (i = 0; i < sw; i++) {                      \
		    GET_SRC4(ss);                               \
		    buffd[i] = ss;                              \
		}                                               \
	    }                                                   \
	}

/* *********************************************************** */

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
	void *lut_table;
	mlib_s32 offset, off, kw, dn1;
	mlib_s32 nchan, sll, dll, sw, sh, flag3x3;
	mlib_s32 row, i, j, bsize, buff_ind = 0, func_ind1, func_ind2,
	    method, indexsize;
	mlib_d64 *pbuff, *buffd, *buff_lcl[2 * MAX_N], **buff_arr =
	    buff_lcl, **buff;
	mlib_d64 kern_lcl[MAX_N * MAX_M], *kern = kern_lcl, *pkern;
	mlib_d64 dscale;
	func_dm_type func_dm;
	const func_m_type *func_m_ptr;

	mlib_f32 fscale;

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_SIZE_EQUAL(src, dst);
	MLIB_IMAGE_HAVE_CHAN(dst, 1);

	if (kernel == NULL || colormap == NULL) {
		return (MLIB_NULLPOINTER);
	}

	MLIB_IMAGE_AND_COLORMAP_ARE_COMPAT(src, colormap);

	if (!(dm >= 0 && dm < m && dn >= 0 && dn < n)) {
		return (MLIB_FAILURE);
	}

	flag3x3 = (m - 3) | (n - 3) | (dm - 1) | (dn - 1);
	method = mlib_ImageGetMethod(colormap);

	if (flag3x3 == 0 && mlib_ImageGetType(src) == MLIB_BYTE &&
	    method != LUT_COLOR_DIMENSIONS) {
		return __mlib_ImageColorErrorDiffusion3x3(dst, src, kernel,
		    scale, colormap);
	}

	flag3x3 = -1;

	MLIB_IMAGE_GET_ALL_PARAMS(dst, dtype, nchan, sw, sh, dll, dl);
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
	LOAD_KERNEL(kern, kernel);

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
/* special case */
	if (flag3x3 == 0) {
		func_ind2 = KH_MAX + 1;
		dn = 0;
	}

	func_ind2 = 2 * func_ind2 + (nchan - 3);

	func_dm = func_arr[func_ind1][func_ind2];
	func_m_ptr = func_m_arr + (nchan - 3) * KW_MAX - 1;

	offset = mlib_ImageGetLutOffset(colormap);

	if (stype == MLIB_SHORT)
		offset *= 2;
	lut_table =
	    (mlib_u8 *)mlib_ImageGetLutNormalTable(colormap) -
	    4 * offset;

	for (row = 0; row < sh; row++) {
		buff = buff_arr + buff_ind;

		if (flag3x3 == 0) {
			buffd = (void *)sl;
		} else {
			LOAD_SRC_LINE(buffd, sl);
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
