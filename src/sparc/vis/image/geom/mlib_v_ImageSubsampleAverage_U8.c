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

#pragma ident	"@(#)mlib_v_ImageSubsampleAverage_U8.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *      Internal functions for mlib_ImageSubsampleAverage().
 *
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_v_ImageChannelExtract.h>
#include <mlib_v_ImageDataTypeConvert.h>
#include <vis_proto.h>

/* *********************************************************** */

#define	FUNC(SUFF)	mlib_v_ImageSubsampleAverage_U8_##SUFF

/* *********************************************************** */

#define	DTYPE	mlib_u8
#define	YTYPE	mlib_u16

#define	XTYPE	YTYPE

/* *********************************************************** */

#define	USE_VIS

#define	VIS_GSR_SHIFT0	7

#define	USE_SHIFT

#define	FUNC_EXTRACT_43	mlib_v_ImageChannelExtract_U8_43L_D1

#include <mlib_ImageSubsampleAverage.h>

/* *********************************************************** */

#define	ALIGN_ADDR(pp, ptr, off)                                  \
	mlib_d64 *pp =                                            \
	    (void *)((mlib_addr)((mlib_u8 *)(ptr) + off) & ~7);   \
	vis_alignaddr(ptr, off)

/* *********************************************************** */

#define	READ_NA(dst, ptr, off)                                  \
	{                                                       \
	    ALIGN_ADDR(pp, ptr, off);                           \
	    dst = vis_faligndata(pp[0], pp[1]);                 \
	}

/* *********************************************************** */

#define	READ_2NA(dst0, dst1, ptr, off)                          \
	{                                                       \
	    ALIGN_ADDR(pp, ptr, off);                           \
	    dst0 = vis_faligndata(pp[0], pp[1]);                \
	    dst1 = vis_faligndata(pp[1], pp[2]);                \
	}

/* *********************************************************** */

#define	READ_3NA(dst0, dst1, dst2, ptr, off)                    \
	{                                                       \
	    ALIGN_ADDR(pp, ptr, off);                           \
	    dst0 = vis_faligndata(pp[0], pp[1]);                \
	    dst1 = vis_faligndata(pp[1], pp[2]);                \
	    dst2 = vis_faligndata(pp[2], pp[3]);                \
	}

/* *********************************************************** */

#define	READ_4NA(dst0, dst1, dst2, dst3, ptr, off)              \
	{                                                       \
	    ALIGN_ADDR(pp, ptr, off);                           \
	    dst0 = vis_faligndata(pp[0], pp[1]);                \
	    dst1 = vis_faligndata(pp[1], pp[2]);                \
	    dst2 = vis_faligndata(pp[2], pp[3]);                \
	    dst3 = vis_faligndata(pp[3], pp[4]);                \
	}

/* *********************************************************** */

#define	LD_U16(ptr, ind)	vis_ld_u16_i(ptr, ind)

/* *********************************************************** */

#define	LD_2U16(s0, sl, i0, i1)                                 \
	s0 = vis_faligndata(LD_U16(sl, i1), s0);                \
	s0 = vis_faligndata(LD_U16(sl, i0), s0)

/* *********************************************************** */

#define	LD_4U16(s0, sl, i0, i1, i2, i3)                         \
	LD_2U16(s0, sl, i2, i3);                                \
	LD_2U16(s0, sl, i0, i1)

/* *********************************************************** */

#define	FMUL_16x16(x, y)                                          \
	vis_fpadd16(vis_fmul8sux16(x, y), vis_fmul8ulx16(x, y))

/* *********************************************************** */

#define	EXPAND0(x)	vis_fmul8x16al(x, fone)
#define	EXPAND1(x)	vis_fpmerge(fzero, x)

#define	EXPAND0_HI(x)	EXPAND0(vis_read_hi(x))

#define	EXPAND0_LO(x)	EXPAND0(vis_read_lo(x))

#define	EXPAND1_HI(x)	EXPAND1(vis_read_hi(x))

#define	EXPAND1_LO(x)	EXPAND1(vis_read_lo(x))

/* *********************************************************** */

static void FUNC(y1i) (
    YTYPE *buff,
    DTYPE *sp,
    mlib_s32 slb,
    mlib_s32 xsize)
{
	mlib_v_ImageDataTypeConvert_U8_S16_D1((void *)sp, (void *)buff, xsize);
}

/* *********************************************************** */

#define	FUNC_Y(SUFF, ADD)                                                \
	static void FUNC(y2##SUFF) (YTYPE *buff, DTYPE *sp,              \
	    mlib_s32 slb, mlib_s32 xsize)                                \
	{                                                                \
	    mlib_s32 i, off0, off1;                                      \
	    mlib_d64 *sp0, *sp1, *pbuff = (void *)buff;                  \
	    mlib_d64 dhi, dlo, a0, a1, b0, b1;                           \
	    mlib_f32 fone = vis_to_float(0x0100);                        \
	    mlib_f32 fzero = vis_fzeros();                               \
	                                                                 \
	    off0 = (mlib_s32)sp & 7;                                     \
	    off1 = (mlib_s32)(sp + slb) & 7;                             \
	    if (!(off0 | off1)) {                                        \
		sp0 = (void *)(sp);                                      \
		sp1 = (void *)(sp + slb);                                \
		for (i = 0; i < xsize; i += 8) {                         \
		    a0 = ((*sp0++));                                     \
		    b0 = ((*sp1++));                                     \
		    dhi = vis_fpadd16(EXPAND0_HI(a0), EXPAND1_HI(b0));   \
		    dlo = vis_fpadd16(EXPAND0_LO(a0), EXPAND0_LO(b0));   \
		    ADD(pbuff[0], dhi);                                  \
		    ADD(pbuff[1], dlo);                                  \
		    pbuff += 2;                                          \
		}                                                        \
	    } else {                                                     \
		sp0 = (void *)(sp - off0);                               \
		sp1 = (void *)(sp + slb - off1);                         \
		a1 = vis_ld_d64_nf(sp0); sp0++;                          \
		b1 = vis_ld_d64_nf(sp1); sp1++;                          \
		for (i = 0; i < xsize; i += 8) {                         \
		    vis_alignaddr((void *)0, off0);                      \
		    a0 = a1;                                             \
		    a1 = vis_ld_d64_nf(sp0); sp0++;                      \
		    a0 = vis_faligndata(a0, a1);                         \
		    vis_alignaddr((void *)0, off1);                      \
		    b0 = b1;                                             \
		    b1 = vis_ld_d64_nf(sp1); sp1++;                      \
		    b0 = vis_faligndata(b0, b1);                         \
		    dhi = vis_fpadd16(EXPAND0_HI(a0), EXPAND1_HI(b0));   \
		    dlo = vis_fpadd16(EXPAND0_LO(a0), EXPAND0_LO(b0));   \
		    ADD(pbuff[0], dhi);                                  \
		    ADD(pbuff[1], dlo);                                  \
		    pbuff += 2;                                          \
		}                                                        \
	    }                                                            \
	}                                                                \
	                                                                 \
/*                                                                       \
 * ************************************************************          \
 */                                                                      \
	static void FUNC(y3##SUFF) (YTYPE *buff, DTYPE *sp,              \
	    mlib_s32 slb, mlib_s32 xsize)                                \
	{                                                                \
	    mlib_s32 i, off0, off1, off2;                                \
	    mlib_d64 *sp0, *sp1, *sp2, *pbuff = (void *)buff;            \
	    mlib_d64 dhi, dlo, a0, a1, b0, b1, c0, c1;                   \
	    mlib_f32 fone = vis_to_float(0x0100);                        \
	    mlib_f32 fzero = vis_fzeros();                               \
	                                                                 \
	    off0 = (mlib_s32)sp & 7;                                     \
	    off1 = (mlib_s32)(sp + slb) & 7;                             \
	    off2 = (mlib_s32)(sp + 2 * slb) & 7;                         \
	    if (!(off0 | off1)) {                                        \
		sp0 = (void *)(sp);                                      \
		sp1 = (void *)(sp + slb);                                \
		sp2 = (void *)(sp + 2 * slb);                            \
		for (i = 0; i < xsize; i += 8) {                         \
		    a0 = ((*sp0++));                                     \
		    b0 = ((*sp1++));                                     \
		    c0 = ((*sp2++));                                     \
		    dhi = vis_fpadd16(EXPAND0_HI(a0), EXPAND1_HI(b0));   \
		    dlo = vis_fpadd16(EXPAND0_LO(a0), EXPAND0_LO(b0));   \
		    dhi = vis_fpadd16(dhi, EXPAND1_HI(c0));              \
		    dlo = vis_fpadd16(dlo, EXPAND1_LO(c0));              \
		    ADD(pbuff[0], dhi);                                  \
		    ADD(pbuff[1], dlo);                                  \
		    pbuff += 2;                                          \
		}                                                        \
	    } else {                                                     \
		sp0 = (void *)(sp - off0);                               \
		sp1 = (void *)(sp + slb - off1);                         \
		sp2 = (void *)(sp + 2 * slb - off2);                     \
		a1 = vis_ld_d64_nf(sp0); sp0++;                          \
		b1 = vis_ld_d64_nf(sp1); sp1++;                          \
		c1 = vis_ld_d64_nf(sp2); sp2++;                          \
		for (i = 0; i < xsize; i += 8) {                         \
		    vis_alignaddr((void *)0, off0);                      \
		    a0 = a1;                                             \
		    a1 = vis_ld_d64_nf(sp0); sp0++;                      \
		    a0 = vis_faligndata(a0, a1);                         \
		    vis_alignaddr((void *)0, off1);                      \
		    b0 = b1;                                             \
		    b1 = vis_ld_d64_nf(sp1); sp1++;                      \
		    b0 = vis_faligndata(b0, b1);                         \
		    vis_alignaddr((void *)0, off2);                      \
		    c0 = c1;                                             \
		    c1 = vis_ld_d64_nf(sp2); sp2++;                      \
		    c0 = vis_faligndata(c0, c1);                         \
		    dhi = vis_fpadd16(EXPAND0_HI(a0), EXPAND1_HI(b0));   \
		    dlo = vis_fpadd16(EXPAND0_LO(a0), EXPAND0_LO(b0));   \
		    dhi = vis_fpadd16(dhi, EXPAND1_HI(c0));              \
		    dlo = vis_fpadd16(dlo, EXPAND1_LO(c0));              \
		    ADD(pbuff[0], dhi);                                  \
		    ADD(pbuff[1], dlo);                                  \
		    pbuff += 2;                                          \
		}                                                        \
	    }                                                            \
	}                                                                \
	                                                                 \
/*                                                                       \
 * ************************************************************          \
 */                                                                      \
	static void FUNC(y4##SUFF) (YTYPE *buff, DTYPE *sp,              \
	    mlib_s32 slb, mlib_s32 xsize)                                \
	{                                                                \
	    mlib_s32 i, off0, off1, off2, off3;                          \
	    mlib_d64 *sp0, *sp1, *sp2, *sp3, *pbuff = (void *)buff;      \
	    mlib_d64 dhi, dlo, a0, a1, b0, b1, c0, c1, d0, d1;           \
	    mlib_f32 fone = vis_to_float(0x0100);                        \
	    mlib_f32 fzero = vis_fzeros();                               \
	                                                                 \
	    off0 = (mlib_s32)sp & 7;                                     \
	    off1 = (mlib_s32)(sp + slb) & 7;                             \
	    off2 = (mlib_s32)(sp + 2 * slb) & 7;                         \
	    off3 = (mlib_s32)(sp + 3 * slb) & 7;                         \
	    if (!(off0 | off1)) {                                        \
		sp0 = (void *)(sp);                                      \
		sp1 = (void *)(sp + slb);                                \
		sp2 = (void *)(sp + 2 * slb);                            \
		sp3 = (void *)(sp + 3 * slb);                            \
		for (i = 0; i < xsize; i += 8) {                         \
		    a0 = ((*sp0++));                                     \
		    b0 = ((*sp1++));                                     \
		    c0 = ((*sp2++));                                     \
		    d0 = ((*sp3++));                                     \
		    dhi =                                                \
			vis_fpadd16(vis_fpadd16(EXPAND0_HI(a0),          \
			EXPAND0_HI(b0)), vis_fpadd16(EXPAND0_HI(c0),     \
			EXPAND0_HI(d0)));                                \
		    dlo =                                                \
			vis_fpadd16(vis_fpadd16(EXPAND0_LO(a0),          \
			EXPAND1_LO(b0)), vis_fpadd16(EXPAND0_LO(c0),     \
			EXPAND1_LO(d0)));                                \
		    ADD(pbuff[0], dhi);                                  \
		    ADD(pbuff[1], dlo);                                  \
		    pbuff += 2;                                          \
		}                                                        \
	    } else {                                                     \
		sp0 = (void *)(sp - off0);                               \
		sp1 = (void *)(sp + slb - off1);                         \
		sp2 = (void *)(sp + 2 * slb - off2);                     \
		sp3 = (void *)(sp + 3 * slb - off3);                     \
		a1 = vis_ld_d64_nf(sp0); sp0++;                          \
		b1 = vis_ld_d64_nf(sp1); sp1++;                          \
		c1 = vis_ld_d64_nf(sp2); sp2++;                          \
		d1 = vis_ld_d64_nf(sp3); sp3++;                          \
		for (i = 0; i < xsize; i += 8) {                         \
		    vis_alignaddr((void *)0, off0);                      \
		    a0 = a1;                                             \
		    a1 = vis_ld_d64_nf(sp0); sp0++;                      \
		    a0 = vis_faligndata(a0, a1);                         \
		    vis_alignaddr((void *)0, off1);                      \
		    b0 = b1;                                             \
		    b1 = vis_ld_d64_nf(sp1); sp1++;                      \
		    b0 = vis_faligndata(b0, b1);                         \
		    vis_alignaddr((void *)0, off2);                      \
		    c0 = c1;                                             \
		    c1 = vis_ld_d64_nf(sp2); sp2++;                      \
		    c0 = vis_faligndata(c0, c1);                         \
		    vis_alignaddr((void *)0, off3);                      \
		    d0 = d1;                                             \
		    d1 = vis_ld_d64_nf(sp3); sp3++;                      \
		    d0 = vis_faligndata(d0, d1);                         \
		    dhi =                                                \
			vis_fpadd16(vis_fpadd16(EXPAND0_HI(a0),          \
			EXPAND0_HI(b0)), vis_fpadd16(EXPAND0_HI(c0),     \
			EXPAND0_HI(d0)));                                \
		    dlo =                                                \
			vis_fpadd16(vis_fpadd16(EXPAND0_LO(a0),          \
			EXPAND1_LO(b0)), vis_fpadd16(EXPAND0_LO(c0),     \
			EXPAND1_LO(d0)));                                \
		    ADD(pbuff[0], dhi);                                  \
		    ADD(pbuff[1], dlo);                                  \
		    pbuff += 2;                                          \
		}                                                        \
	    }                                                            \
	}

/* *********************************************************** */

#define	STORE(dst, src)	dst = src
#define	ADD(dst, src)	dst = vis_fpadd16(dst, src)

FUNC_Y(i, STORE)
FUNC_Y(a, ADD)

static const func_y func_arr_yi[] = {
	FUNC(y1i),
	FUNC(y2i),
	FUNC(y3i),
	FUNC(y4i)
};

static const func_y func_arr_ya[] = {
	NULL,
	FUNC(y2a),
	FUNC(y3a),
	FUNC(y4a)
};

/* *********************************************************** */

#define	TRANSPOSE_S16_4x4(s0, s1, s2, s3)                         \
	{                                                         \
	    mlib_d64 a0, a1, a2, a3, b0, b1, b2, b3;              \
	                                                          \
	    a0 = vis_fpmerge(vis_read_hi(s0), vis_read_hi(s2));   \
	    a1 = vis_fpmerge(vis_read_lo(s0), vis_read_lo(s2));   \
	    a2 = vis_fpmerge(vis_read_hi(s1), vis_read_hi(s3));   \
	    a3 = vis_fpmerge(vis_read_lo(s1), vis_read_lo(s3));   \
	    b0 = vis_fpmerge(vis_read_hi(a0), vis_read_hi(a2));   \
	    b1 = vis_fpmerge(vis_read_lo(a0), vis_read_lo(a2));   \
	    b2 = vis_fpmerge(vis_read_hi(a1), vis_read_hi(a3));   \
	    b3 = vis_fpmerge(vis_read_lo(a1), vis_read_lo(a3));   \
	    s0 = vis_fpmerge(vis_read_hi(b0), vis_read_lo(b0));   \
	    s1 = vis_fpmerge(vis_read_hi(b1), vis_read_lo(b1));   \
	    s2 = vis_fpmerge(vis_read_hi(b2), vis_read_lo(b2));   \
	    s3 = vis_fpmerge(vis_read_hi(b3), vis_read_lo(b3));   \
	}

/* *********************************************************** */

#define	INIT_ADD                                                \
	mlib_d64 *pbuff = (void *)(ws->buff);                   \
	mlib_s32 *x_ind = ws->x_ind;                            \
	mlib_s32 xsize = ws->xsize;                             \
	mlib_s32 i;                                             \
	mlib_d64 ss

/* *********************************************************** */

#define	INIT_SHF                                                \
	INIT_ADD;                                               \
	mlib_f32 *dp = (void *)(ws->dl);                        \
	mlib_d64 d_ini = ws->d_ini

/* *********************************************************** */

#define	INIT_MUL	INIT_SHF; mlib_d64 dscale = ws->d_mul

/* *********************************************************** */

#define	SAVE_ADD(ss)	((*pbuff++)) = ss

/* *********************************************************** */

#define	SAVE_SHF(ss)	((*dp++)) = vis_fpack16(ss); ((*pbuff++)) = d_ini

/* *********************************************************** */

#define	SAVE_MUL(ss)                                            \
	ss = FMUL_16x16(ss, dscale);                            \
	((*dp++)) = vis_fpack16(ss);                            \
	((*pbuff++)) = d_ini

/* *********************************************************** */

#define	FUNC_X(SUFF, INIT, SAVE)                                        \
	static void FUNC(ch1_x1##SUFF) (mlib_SubsAdap *ws,              \
	    mlib_u8 *sl)                                                \
	{                                                               \
	    mlib_s32 x0, x1, x2, x3;                                    \
	    mlib_d64 s0;                                                \
	                                                                \
	    INIT;                                                       \
	    for (i = 0; i < xsize; i += 4) {                            \
		x0 = x_ind[i];                                          \
		x1 = x_ind[i + 1];                                      \
		x2 = x_ind[i + 2];                                      \
		x3 = x_ind[i + 3];                                      \
		LD_4U16(s0, sl, x0, x1, x2, x3);                        \
		ss = vis_fpadd16(s0, pbuff[0]);                         \
		SAVE(ss);                                               \
	    }                                                           \
	}                                                               \
	                                                                \
/*                                                                      \
 * ************************************************************         \
 */                                                                     \
	static void FUNC(ch1_x2##SUFF) (mlib_SubsAdap *ws,              \
	    mlib_u8 *sl)                                                \
	{                                                               \
	    mlib_s32 x0, x1, x2, x3;                                    \
	    mlib_d64 s0, s1;                                            \
	                                                                \
	    INIT;                                                       \
	    for (i = 0; i < xsize; i += 4) {                            \
		x0 = x_ind[i];                                          \
		x1 = x_ind[i + 1];                                      \
		x2 = x_ind[i + 2];                                      \
		x3 = x_ind[i + 3];                                      \
		LD_4U16(s0, sl, x0, x1, x2, x3);                        \
		LD_4U16(s1, sl, x0 + 2, x1 + 2, x2 + 2, x3 + 2);        \
		ss = vis_fpadd16(s0, s1);                               \
		ss = vis_fpadd16(ss, pbuff[0]);                         \
		SAVE(ss);                                               \
	    }                                                           \
	}                                                               \
	                                                                \
/*                                                                      \
 * ************************************************************         \
 */                                                                     \
	static void FUNC(ch1_x3##SUFF) (mlib_SubsAdap *ws,              \
	    mlib_u8 *sl)                                                \
	{                                                               \
	    mlib_s32 x0, x1, x2, x3;                                    \
	    mlib_d64 s0, s1, s2;                                        \
	                                                                \
	    INIT;                                                       \
	    for (i = 0; i < xsize; i += 4) {                            \
		x0 = x_ind[i];                                          \
		x1 = x_ind[i + 1];                                      \
		x2 = x_ind[i + 2];                                      \
		x3 = x_ind[i + 3];                                      \
		LD_4U16(s0, sl, x0, x1, x2, x3);                        \
		LD_4U16(s1, sl, x0 + 2, x1 + 2, x2 + 2, x3 + 2);        \
		LD_4U16(s2, sl, x0 + 4, x1 + 4, x2 + 4, x3 + 4);        \
		s0 = vis_fpadd16(s0, s1);                               \
		ss = vis_fpadd16(s2, pbuff[0]);                         \
		ss = vis_fpadd16(ss, s0);                               \
		SAVE(ss);                                               \
	    }                                                           \
	}                                                               \
	                                                                \
/*                                                                      \
 * ************************************************************         \
 */                                                                     \
	static void FUNC(ch1_x4##SUFF) (mlib_SubsAdap *ws,              \
	    mlib_u8 *sl)                                                \
	{                                                               \
	    mlib_s32 x0, x1, x2, x3;                                    \
	    mlib_d64 s0, s1, s2, s3;                                    \
	                                                                \
	    INIT;                                                       \
	    for (i = 0; i < xsize; i += 4) {                            \
		x0 = x_ind[i];                                          \
		x1 = x_ind[i + 1];                                      \
		x2 = x_ind[i + 2];                                      \
		x3 = x_ind[i + 3];                                      \
		READ_NA(s0, sl, x0);                                    \
		READ_NA(s1, sl, x1);                                    \
		READ_NA(s2, sl, x2);                                    \
		READ_NA(s3, sl, x3);                                    \
		TRANSPOSE_S16_4x4(s0, s1, s2, s3);                      \
		s0 = vis_fpadd16(s0, s1);                               \
		s2 = vis_fpadd16(s2, s3);                               \
		s2 = vis_fpadd16(s2, pbuff[0]);                         \
		ss = vis_fpadd16(s0, s2);                               \
		SAVE(ss);                                               \
	    }                                                           \
	    vis_alignaddr((void *)0, 6);                                \
/*                                                                      \
 * after READ_NA()                                                      \
 */                                                                     \
	}                                                               \
	                                                                \
/*                                                                      \
 * ************************************************************         \
 */                                                                     \
	static void FUNC(ch2_x1##SUFF) (mlib_SubsAdap *ws,              \
	    mlib_u8 *sl)                                                \
	{                                                               \
	    mlib_s32 x0, x1;                                            \
	    mlib_d64 s0, s1;                                            \
	                                                                \
	    INIT;                                                       \
	    for (i = 0; i < xsize; i += 2) {                            \
		x0 = x_ind[i];                                          \
		x1 = x_ind[i + 1];                                      \
		LD_4U16(s0, sl, x0, x0 + 2, x1, x1 + 2);                \
		ss = vis_fpadd16(s0, pbuff[0]);                         \
		SAVE(ss);                                               \
	    }                                                           \
	}                                                               \
	                                                                \
/*                                                                      \
 * ************************************************************         \
 */                                                                     \
	static void FUNC(ch2_x2##SUFF) (mlib_SubsAdap *ws,              \
	    mlib_u8 *sl)                                                \
	{                                                               \
	    mlib_s32 x0, x1;                                            \
	    mlib_d64 s0, s1;                                            \
	                                                                \
	    INIT;                                                       \
	    for (i = 0; i < xsize; i += 2) {                            \
		x0 = x_ind[i];                                          \
		x1 = x_ind[i + 1];                                      \
		READ_NA(s0, sl, x0);                                    \
		READ_NA(s1, sl, x1);                                    \
		ss = vis_freg_pair(vis_fpadd16s(vis_read_hi(s0),        \
		    vis_read_lo(s0)), vis_fpadd16s(vis_read_hi(s1),     \
		    vis_read_lo(s1)));                                  \
		ss = vis_fpadd16(ss, pbuff[0]);                         \
		SAVE(ss);                                               \
	    }                                                           \
	}                                                               \
	                                                                \
/*                                                                      \
 * ************************************************************         \
 */                                                                     \
	static void FUNC(ch2_x3##SUFF) (mlib_SubsAdap *ws,              \
	    mlib_u8 *sl)                                                \
	{                                                               \
	    mlib_s32 x0, x1;                                            \
	    mlib_d64 s0, s1, s2, s3;                                    \
	                                                                \
	    INIT;                                                       \
	    for (i = 0; i < xsize; i += 2) {                            \
		x0 = x_ind[i];                                          \
		x1 = x_ind[i + 1];                                      \
		READ_2NA(s0, s2, sl, x0);                               \
		READ_2NA(s1, s3, sl, x1);                               \
		s0 = vis_freg_pair(vis_fpadd16s(vis_read_hi(s0),        \
		    vis_read_lo(s0)), vis_fpadd16s(vis_read_hi(s1),     \
		    vis_read_lo(s1)));                                  \
		s2 = vis_freg_pair(vis_read_hi(s2), vis_read_hi(s3));   \
		ss = vis_fpadd16(s0, s2);                               \
		ss = vis_fpadd16(ss, pbuff[0]);                         \
		SAVE(ss);                                               \
	    }                                                           \
	}                                                               \
	                                                                \
/*                                                                      \
 * ************************************************************         \
 */                                                                     \
	static void FUNC(ch2_x4##SUFF) (mlib_SubsAdap *ws,              \
	    mlib_u8 *sl)                                                \
	{                                                               \
	    mlib_s32 x0, x1;                                            \
	    mlib_d64 s0, s1, s2, s3;                                    \
	                                                                \
	    INIT;                                                       \
	    for (i = 0; i < xsize; i += 2) {                            \
		x0 = x_ind[i];                                          \
		x1 = x_ind[i + 1];                                      \
		READ_2NA(s0, s2, sl, x0);                               \
		READ_2NA(s1, s3, sl, x1);                               \
		s0 = vis_freg_pair(vis_fpadd16s(vis_read_hi(s0),        \
		    vis_read_lo(s0)), vis_fpadd16s(vis_read_hi(s1),     \
		    vis_read_lo(s1)));                                  \
		s2 = vis_freg_pair(vis_fpadd16s(vis_read_hi(s2),        \
		    vis_read_lo(s2)), vis_fpadd16s(vis_read_hi(s3),     \
		    vis_read_lo(s3)));                                  \
		ss = vis_fpadd16(s0, s2);                               \
		ss = vis_fpadd16(ss, pbuff[0]);                         \
		SAVE(ss);                                               \
	    }                                                           \
	}                                                               \
	                                                                \
/*                                                                      \
 * ************************************************************         \
 */                                                                     \
	static void FUNC(ch4_x1##SUFF) (mlib_SubsAdap *ws,              \
	    mlib_u8 *sl)                                                \
	{                                                               \
	    mlib_s32 x0;                                                \
	                                                                \
	    INIT;                                                       \
	    for (i = 0; i < xsize; i++) {                               \
		x0 = x_ind[i];                                          \
		READ_NA(ss, sl, x0);                                    \
		SAVE(ss);                                               \
	    }                                                           \
	}                                                               \
	                                                                \
/*                                                                      \
 * ************************************************************         \
 */                                                                     \
	static void FUNC(ch4_x2##SUFF) (mlib_SubsAdap *ws,              \
	    mlib_u8 *sl)                                                \
	{                                                               \
	    mlib_s32 x0;                                                \
	    mlib_d64 s0, s1;                                            \
	                                                                \
	    INIT;                                                       \
	    for (i = 0; i < xsize; i++) {                               \
		x0 = x_ind[i];                                          \
		READ_2NA(s0, s1, sl, x0);                               \
		ss = vis_fpadd16(s0, s1);                               \
		ss = vis_fpadd16(ss, pbuff[0]);                         \
		SAVE(ss);                                               \
	    }                                                           \
	}                                                               \
	                                                                \
/*                                                                      \
 * ************************************************************         \
 */                                                                     \
	static void FUNC(ch4_x3##SUFF) (mlib_SubsAdap *ws,              \
	    mlib_u8 *sl)                                                \
	{                                                               \
	    mlib_s32 x0;                                                \
	    mlib_d64 s0, s1, s2;                                        \
	                                                                \
	    INIT;                                                       \
	    for (i = 0; i < xsize; i++) {                               \
		x0 = x_ind[i];                                          \
		READ_3NA(s0, s1, s2, sl, x0);                           \
		s0 = vis_fpadd16(s0, s1);                               \
		s2 = vis_fpadd16(s2, pbuff[0]);                         \
		ss = vis_fpadd16(s0, s2);                               \
		SAVE(ss);                                               \
	    }                                                           \
	}                                                               \
	                                                                \
/*                                                                      \
 * ************************************************************         \
 */                                                                     \
	static void FUNC(ch4_x4##SUFF) (mlib_SubsAdap *ws,              \
	    mlib_u8 *sl)                                                \
	{                                                               \
	    mlib_s32 x0;                                                \
	    mlib_d64 s0, s1, s2, s3;                                    \
	                                                                \
	    INIT;                                                       \
	    for (i = 0; i < xsize; i++) {                               \
		x0 = x_ind[i];                                          \
		READ_4NA(s0, s1, s2, s3, sl, x0);                       \
		s0 = vis_fpadd16(s0, s1);                               \
		s2 = vis_fpadd16(s2, pbuff[0]);                         \
		s2 = vis_fpadd16(s2, s3);                               \
		ss = vis_fpadd16(s0, s2);                               \
		SAVE(ss);                                               \
	    }                                                           \
	}                                                               \
	                                                                \
/*                                                                      \
 * ************************************************************         \
 */                                                                     \
	static void FUNC(ch3_x2##SUFF) (mlib_SubsAdap *ws,              \
	    mlib_u8 *sl)                                                \
	{                                                               \
	    mlib_s32 x0;                                                \
	    mlib_d64 s0, s1;                                            \
	                                                                \
	    INIT;                                                       \
	    for (i = 0; i < xsize; i++) {                               \
		x0 = x_ind[i];                                          \
		READ_NA(s0, sl, x0);                                    \
		READ_NA(s1, sl, x0 + 6);                                \
		ss = vis_fpadd16(s0, s1);                               \
		ss = vis_fpadd16(ss, pbuff[0]);                         \
		SAVE(ss);                                               \
	    }                                                           \
	}                                                               \
	                                                                \
/*                                                                      \
 * ************************************************************         \
 */                                                                     \
	static void FUNC(ch3_x3##SUFF) (mlib_SubsAdap *ws,              \
	    mlib_u8 *sl)                                                \
	{                                                               \
	    mlib_s32 x0;                                                \
	    mlib_d64 s0, s1, s2;                                        \
	                                                                \
	    INIT;                                                       \
	    for (i = 0; i < xsize; i++) {                               \
		x0 = x_ind[i];                                          \
		READ_NA(s0, sl, x0);                                    \
		READ_NA(s1, sl, x0 + 6);                                \
		READ_NA(s2, sl, x0 + 12);                               \
		s0 = vis_fpadd16(s0, s1);                               \
		s2 = vis_fpadd16(s2, pbuff[0]);                         \
		ss = vis_fpadd16(s0, s2);                               \
		SAVE(ss);                                               \
	    }                                                           \
	}                                                               \
	                                                                \
/*                                                                      \
 * ************************************************************         \
 */                                                                     \
	static void FUNC(ch3_x4##SUFF) (mlib_SubsAdap *ws,              \
	    mlib_u8 *sl)                                                \
	{                                                               \
	    mlib_s32 x0;                                                \
	    mlib_d64 s0, s1, s2, s3;                                    \
	                                                                \
	    INIT;                                                       \
	    for (i = 0; i < xsize; i++) {                               \
		x0 = x_ind[i];                                          \
		READ_NA(s0, sl, x0);                                    \
		READ_NA(s1, sl, x0 + 6);                                \
		READ_NA(s2, sl, x0 + 12);                               \
		READ_NA(s3, sl, x0 + 18);                               \
		s0 = vis_fpadd16(s0, s1);                               \
		s2 = vis_fpadd16(s2, pbuff[0]);                         \
		s2 = vis_fpadd16(s2, s3);                               \
		ss = vis_fpadd16(s0, s2);                               \
		SAVE(ss);                                               \
	    }                                                           \
	}                                                               \
	                                                                \
/*                                                                      \
 * ************************************************************         \
 */                                                                     \
	static const func_x func_arr_x##SUFF[] = {                      \
	    FUNC(ch1_x1##SUFF), FUNC(ch1_x2##SUFF),                     \
	    FUNC(ch1_x3##SUFF), FUNC(ch1_x4##SUFF),                     \
	    FUNC(ch2_x1##SUFF), FUNC(ch2_x2##SUFF),                     \
	    FUNC(ch2_x3##SUFF), FUNC(ch2_x4##SUFF),                     \
	    FUNC(ch4_x1##SUFF), FUNC(ch3_x2##SUFF),                     \
	    FUNC(ch3_x3##SUFF), FUNC(ch3_x4##SUFF),                     \
	    FUNC(ch4_x1##SUFF), FUNC(ch4_x2##SUFF),                     \
	    FUNC(ch4_x3##SUFF), FUNC(ch4_x4##SUFF)                      \
	}

/* *********************************************************** */

FUNC_X(a, INIT_ADD, SAVE_ADD);
FUNC_X(m, INIT_MUL, SAVE_MUL);
FUNC_X(s, INIT_SHF, SAVE_SHF);

/* *********************************************************** */
