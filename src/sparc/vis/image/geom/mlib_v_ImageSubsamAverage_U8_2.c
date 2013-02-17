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

#pragma ident	"@(#)mlib_v_ImageSubsamAverage_U8_2.c	9.2	07/11/05 SMI"

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

/* #define TYPE_USHORT */

/* *********************************************************** */

#define	FUNC(SUFF)	mlib_v_ImageSubsampleAverage_U8e_##SUFF
#define	DTYPE	mlib_u8

/* *********************************************************** */

#define	YTYPE	mlib_s32
#define	XTYPE	YTYPE

/* *********************************************************** */

#define	USE_VIS

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

#define	READ_5NA(dst0, dst1, dst2, dst3, dst4, ptr, off)        \
	{                                                       \
	    mlib_d64 src0, src1, src2, src3, src4, src5;        \
	                                                        \
	    ALIGN_ADDR(pp, ptr, off);                           \
	    src0 = pp[0];                                       \
	    src1 = pp[1];                                       \
	    src2 = pp[2];                                       \
	    src3 = pp[3];                                       \
	    src4 = pp[4];                                       \
	    src5 = pp[5];                                       \
	    dst0 = vis_faligndata(src0, src1);                  \
	    dst1 = vis_faligndata(src1, src2);                  \
	    dst2 = vis_faligndata(src2, src3);                  \
	    dst3 = vis_faligndata(src3, src4);                  \
	    dst4 = vis_faligndata(src4, src5);                  \
	}

/* *********************************************************** */

#define	READ_6NA(dst0, dst1, dst2, dst3, dst4, dst5, ptr, off)   \
	{                                                        \
	    mlib_d64 src0, src1, src2, src3, src4, src5, src6;   \
	                                                         \
	    ALIGN_ADDR(pp, ptr, off);                            \
	    src0 = pp[0];                                        \
	    src1 = pp[1];                                        \
	    src2 = pp[2];                                        \
	    src3 = pp[3];                                        \
	    src4 = pp[4];                                        \
	    src5 = pp[5];                                        \
	    src6 = pp[6];                                        \
	    dst0 = vis_faligndata(src0, src1);                   \
	    dst1 = vis_faligndata(src1, src2);                   \
	    dst2 = vis_faligndata(src2, src3);                   \
	    dst3 = vis_faligndata(src3, src4);                   \
	    dst4 = vis_faligndata(src4, src5);                   \
	    dst5 = vis_faligndata(src5, src6);                   \
	}

/* *********************************************************** */

#define	READ_8NA(dst0, dst1, dst2, dst3, dst4, dst5, dst6,             \
	dst7, ptr, off)                                                \
	{                                                              \
	    mlib_d64 src0, src1, src2, src3, src4, src5, src6, src7,   \
		src8;                                                  \
	    ALIGN_ADDR(pp, ptr, off);                                  \
	    src0 = pp[0];                                              \
	    src1 = pp[1];                                              \
	    src2 = pp[2];                                              \
	    src3 = pp[3];                                              \
	    src4 = pp[4];                                              \
	    src5 = pp[5];                                              \
	    src6 = pp[6];                                              \
	    src7 = pp[7];                                              \
	    src8 = pp[8];                                              \
	    dst0 = vis_faligndata(src0, src1);                         \
	    dst1 = vis_faligndata(src1, src2);                         \
	    dst2 = vis_faligndata(src2, src3);                         \
	    dst3 = vis_faligndata(src3, src4);                         \
	    dst4 = vis_faligndata(src4, src5);                         \
	    dst5 = vis_faligndata(src5, src6);                         \
	    dst6 = vis_faligndata(src6, src7);                         \
	    dst7 = vis_faligndata(src7, src8);                         \
	}

/* *********************************************************** */

#define	LD_2S32(s0, sl, i0, i1)                                         \
	s0 =                                                            \
	vis_freg_pair(*(mlib_f32 *)(sl + i0), *(mlib_f32 *)(sl + i1))

/* *********************************************************** */

#define	DEF_CONST                                               \
	mlib_f32 fone = vis_to_float(0x0100);                   \
	mlib_f32 fzero = vis_fzeros();                          \
	mlib_f32 fone16 = vis_to_float(0x10001)

/* *********************************************************** */

#define	EXPAND0(x)	vis_fmul8x16al(x, fone)
#define	EXPAND1(x)	vis_fpmerge(fzero, x)

#define	EXPAND0_HI(x)	EXPAND0(vis_read_hi(x))

#define	EXPAND0_LO(x)	EXPAND0(vis_read_lo(x))

#define	EXPAND1_HI(x)	EXPAND1(vis_read_hi(x))

#define	EXPAND1_LO(x)	EXPAND1(vis_read_lo(x))

/* *********************************************************** */

#define	EXPAND_S16_HI(x)	vis_fmuld8ulx16(fone16, vis_read_hi(x))
#define	EXPAND_S16_LO(x)	vis_fmuld8ulx16(fone16, vis_read_lo(x))

/* *********************************************************** */

void mlib_v_ImageDataTypeConvert_U8_S32_D1(
    const mlib_u8 *src,
    mlib_s32 *dst,
    mlib_s32 dsize);

/* *********************************************************** */

static void FUNC(y1i) (
    YTYPE *buff,
    DTYPE *sp,
    mlib_s32 slb,
    mlib_s32 xsize)
{
	mlib_v_ImageDataTypeConvert_U8_S32_D1((void *)sp, (void *)buff, xsize);
}

/* *********************************************************** */

#define	FUNC_Y(SUFF, ADD)                                                \
	static void FUNC(y2##SUFF) (YTYPE *buff, DTYPE *sp,              \
	    mlib_s32 slb, mlib_s32 xsize)                                \
	{                                                                \
	    mlib_s32 i, off0, off1;                                      \
	    mlib_d64 *sp0, *sp1, *pbuff = (void *)buff;                  \
	    mlib_d64 dhi, dlo, a0, a1, b0, b1;                           \
	                                                                 \
	    DEF_CONST;                                                   \
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
		    ADD(pbuff[0], EXPAND_S16_HI(dhi));                   \
		    ADD(pbuff[1], EXPAND_S16_LO(dhi));                   \
		    ADD(pbuff[2], EXPAND_S16_HI(dlo));                   \
		    ADD(pbuff[3], EXPAND_S16_LO(dlo));                   \
		    pbuff += 4;                                          \
		}                                                        \
	    } else {                                                     \
		sp0 = (void *)(sp - off0);                               \
		sp1 = (void *)(sp + slb - off1);                         \
		a1 = ((*sp0++));                                         \
		b1 = ((*sp1++));                                         \
		for (i = 0; i < xsize; i += 8) {                         \
		    vis_alignaddr((void *)0, off0);                      \
		    a0 = a1;                                             \
		    a1 = ((*sp0++));                                     \
		    a0 = vis_faligndata(a0, a1);                         \
		    vis_alignaddr((void *)0, off1);                      \
		    b0 = b1;                                             \
		    b1 = ((*sp1++));                                     \
		    b0 = vis_faligndata(b0, b1);                         \
		    dhi = vis_fpadd16(EXPAND0_HI(a0), EXPAND1_HI(b0));   \
		    dlo = vis_fpadd16(EXPAND0_LO(a0), EXPAND0_LO(b0));   \
		    ADD(pbuff[0], EXPAND_S16_HI(dhi));                   \
		    ADD(pbuff[1], EXPAND_S16_LO(dhi));                   \
		    ADD(pbuff[2], EXPAND_S16_HI(dlo));                   \
		    ADD(pbuff[3], EXPAND_S16_LO(dlo));                   \
		    pbuff += 4;                                          \
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
	                                                                 \
	    DEF_CONST;                                                   \
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
		    ADD(pbuff[0], EXPAND_S16_HI(dhi));                   \
		    ADD(pbuff[1], EXPAND_S16_LO(dhi));                   \
		    ADD(pbuff[2], EXPAND_S16_HI(dlo));                   \
		    ADD(pbuff[3], EXPAND_S16_LO(dlo));                   \
		    pbuff += 4;                                          \
		}                                                        \
	    } else {                                                     \
		sp0 = (void *)(sp - off0);                               \
		sp1 = (void *)(sp + slb - off1);                         \
		sp2 = (void *)(sp + 2 * slb - off2);                     \
		a1 = ((*sp0++));                                         \
		b1 = ((*sp1++));                                         \
		c1 = ((*sp2++));                                         \
		for (i = 0; i < xsize; i += 8) {                         \
		    vis_alignaddr((void *)0, off0);                      \
		    a0 = a1;                                             \
		    a1 = ((*sp0++));                                     \
		    a0 = vis_faligndata(a0, a1);                         \
		    vis_alignaddr((void *)0, off1);                      \
		    b0 = b1;                                             \
		    b1 = ((*sp1++));                                     \
		    b0 = vis_faligndata(b0, b1);                         \
		    vis_alignaddr((void *)0, off2);                      \
		    c0 = c1;                                             \
		    c1 = ((*sp2++));                                     \
		    c0 = vis_faligndata(c0, c1);                         \
		    dhi = vis_fpadd16(EXPAND0_HI(a0), EXPAND1_HI(b0));   \
		    dlo = vis_fpadd16(EXPAND0_LO(a0), EXPAND0_LO(b0));   \
		    dhi = vis_fpadd16(dhi, EXPAND1_HI(c0));              \
		    dlo = vis_fpadd16(dlo, EXPAND1_LO(c0));              \
		    ADD(pbuff[0], EXPAND_S16_HI(dhi));                   \
		    ADD(pbuff[1], EXPAND_S16_LO(dhi));                   \
		    ADD(pbuff[2], EXPAND_S16_HI(dlo));                   \
		    ADD(pbuff[3], EXPAND_S16_LO(dlo));                   \
		    pbuff += 4;                                          \
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
	                                                                 \
	    DEF_CONST;                                                   \
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
		    ADD(pbuff[0], EXPAND_S16_HI(dhi));                   \
		    ADD(pbuff[1], EXPAND_S16_LO(dhi));                   \
		    ADD(pbuff[2], EXPAND_S16_HI(dlo));                   \
		    ADD(pbuff[3], EXPAND_S16_LO(dlo));                   \
		    pbuff += 4;                                          \
		}                                                        \
	    } else {                                                     \
		sp0 = (void *)(sp - off0);                               \
		sp1 = (void *)(sp + slb - off1);                         \
		sp2 = (void *)(sp + 2 * slb - off2);                     \
		sp3 = (void *)(sp + 3 * slb - off3);                     \
		a1 = ((*sp0++));                                         \
		b1 = ((*sp1++));                                         \
		c1 = ((*sp2++));                                         \
		d1 = ((*sp3++));                                         \
		for (i = 0; i < xsize; i += 8) {                         \
		    vis_alignaddr((void *)0, off0);                      \
		    a0 = a1;                                             \
		    a1 = ((*sp0++));                                     \
		    a0 = vis_faligndata(a0, a1);                         \
		    vis_alignaddr((void *)0, off1);                      \
		    b0 = b1;                                             \
		    b1 = ((*sp1++));                                     \
		    b0 = vis_faligndata(b0, b1);                         \
		    vis_alignaddr((void *)0, off2);                      \
		    c0 = c1;                                             \
		    c1 = ((*sp2++));                                     \
		    c0 = vis_faligndata(c0, c1);                         \
		    vis_alignaddr((void *)0, off3);                      \
		    d0 = d1;                                             \
		    d1 = ((*sp3++));                                     \
		    d0 = vis_faligndata(d0, d1);                         \
		    dhi =                                                \
			vis_fpadd16(vis_fpadd16(EXPAND0_HI(a0),          \
			EXPAND0_HI(b0)), vis_fpadd16(EXPAND0_HI(c0),     \
			EXPAND0_HI(d0)));                                \
		    dlo =                                                \
			vis_fpadd16(vis_fpadd16(EXPAND0_LO(a0),          \
			EXPAND1_LO(b0)), vis_fpadd16(EXPAND0_LO(c0),     \
			EXPAND1_LO(d0)));                                \
		    ADD(pbuff[0], EXPAND_S16_HI(dhi));                   \
		    ADD(pbuff[1], EXPAND_S16_LO(dhi));                   \
		    ADD(pbuff[2], EXPAND_S16_HI(dlo));                   \
		    ADD(pbuff[3], EXPAND_S16_LO(dlo));                   \
		    pbuff += 4;                                          \
		}                                                        \
	    }                                                            \
	}

/* *********************************************************** */

#define	STORE(dst, src)	dst = src
#define	ADD(dst, src)	dst = vis_fpadd32(dst, src)

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

typedef union
{
	mlib_d64 d64;
	struct
	{
		mlib_s32 i0;
		mlib_s32 i1;
	} i32s;
} d64_2x32;

/* *********************************************************** */

#define	INIT_ADD                                                \
	mlib_d64 *pbuff = (void *)(ws->buff);                   \
	mlib_s32 *x_ind = ws->x_ind;                            \
	mlib_s32 xsize = ws->xsize;                             \
	mlib_s32 i;                                             \
	mlib_d64 ss

/* *********************************************************** */

#define	INIT_MUL                                                \
	INIT_ADD;                                               \
	DTYPE *dp = ws->dl;                                     \
	mlib_d64 d_ini = ws->d_ini;                             \
	mlib_d64 dscale = ws->d_mul

/* *********************************************************** */

#define	SAVE_ADD(ss)	((*pbuff++)) = ss

/* *********************************************************** */

#define	SAVE_MUL(ss)                                            \
	{                                                       \
	    d64_2x32 res, res0, res1;                           \
	                                                        \
	    res.d64 = ss;                                       \
	    res0.i32s.i1 = res.i32s.i0 * dscale;                \
	    res1.i32s.i1 = res.i32s.i1 * dscale;                \
	    vis_st_u8(res0.d64, dp);                            \
	    vis_st_u8(res1.d64, dp + 1);                        \
	    dp += 2;                                            \
	    ((*pbuff++)) = d_ini;                               \
	}

/* *********************************************************** */

#define	X_ADD32(s0, s1)                                                 \
	vis_freg_pair(vis_fpadd32s(vis_read_hi(s0), vis_read_lo(s0)),   \
	    vis_fpadd32s(vis_read_hi(s1), vis_read_lo(s1)))

/* *********************************************************** */

#define	FUNC_X(SUFF, INIT, SAVE)                                      \
	static void FUNC(ch1_x1##SUFF) (mlib_SubsAdap *ws,            \
	    mlib_u8 *sl)                                              \
	{                                                             \
	    mlib_s32 x0, x1;                                          \
	    mlib_d64 s0;                                              \
	                                                              \
	    INIT;                                                     \
	    for (i = 0; i < xsize; i += 2) {                          \
		x0 = x_ind[i];                                        \
		x1 = x_ind[i + 1];                                    \
		LD_2S32(s0, sl, x0, x1);                              \
		ss = vis_fpadd32(s0, pbuff[0]);                       \
		SAVE(ss);                                             \
	    }                                                         \
	}                                                             \
	                                                              \
/*                                                                    \
 * ************************************************************       \
 */                                                                   \
	static void FUNC(ch1_x2##SUFF) (mlib_SubsAdap *ws,            \
	    mlib_u8 *sl)                                              \
	{                                                             \
	    mlib_s32 x0, x1;                                          \
	    mlib_d64 s0, s1;                                          \
	                                                              \
	    INIT;                                                     \
	    for (i = 0; i < xsize; i += 2) {                          \
		x0 = x_ind[i];                                        \
		x1 = x_ind[i + 1];                                    \
		LD_2S32(s0, sl, x0, x1);                              \
		LD_2S32(s1, sl, x0 + 4, x1 + 4);                      \
		ss = vis_fpadd32(s0, s1);                             \
		ss = vis_fpadd32(ss, pbuff[0]);                       \
		SAVE(ss);                                             \
	    }                                                         \
	}                                                             \
	                                                              \
/*                                                                    \
 * ************************************************************       \
 */                                                                   \
	static void FUNC(ch1_x3##SUFF) (mlib_SubsAdap *ws,            \
	    mlib_u8 *sl)                                              \
	{                                                             \
	    mlib_s32 x0, x1;                                          \
	    mlib_d64 s0, s1, s2;                                      \
	                                                              \
	    INIT;                                                     \
	    for (i = 0; i < xsize; i += 2) {                          \
		x0 = x_ind[i];                                        \
		x1 = x_ind[i + 1];                                    \
		LD_2S32(s0, sl, x0, x1);                              \
		LD_2S32(s1, sl, x0 + 4, x1 + 4);                      \
		LD_2S32(s2, sl, x0 + 8, x1 + 8);                      \
		s0 = vis_fpadd32(s0, s1);                             \
		ss = vis_fpadd32(s2, pbuff[0]);                       \
		ss = vis_fpadd32(ss, s0);                             \
		SAVE(ss);                                             \
	    }                                                         \
	}                                                             \
	                                                              \
/*                                                                    \
 * ************************************************************       \
 */                                                                   \
	static void FUNC(ch1_x4##SUFF) (mlib_SubsAdap *ws,            \
	    mlib_u8 *sl)                                              \
	{                                                             \
	    mlib_s32 x0, x1;                                          \
	    mlib_d64 s0, s1, s2, s3;                                  \
	                                                              \
	    INIT;                                                     \
	    for (i = 0; i < xsize; i += 2) {                          \
		x0 = x_ind[i];                                        \
		x1 = x_ind[i + 1];                                    \
		READ_2NA(s0, s1, sl, x0);                             \
		READ_2NA(s2, s3, sl, x1);                             \
		s0 = vis_fpadd32(s0, s1);                             \
		s2 = vis_fpadd32(s2, s3);                             \
		ss = X_ADD32(s0, s2);                                 \
		ss = vis_fpadd32(ss, pbuff[0]);                       \
		SAVE(ss);                                             \
	    }                                                         \
	}                                                             \
	                                                              \
/*                                                                    \
 * ************************************************************       \
 */                                                                   \
	static void FUNC(ch2_x1##SUFF) (mlib_SubsAdap *ws,            \
	    mlib_u8 *sl)                                              \
	{                                                             \
	    mlib_s32 x0;                                              \
	                                                              \
	    INIT;                                                     \
	    for (i = 0; i < xsize; i++) {                             \
		x0 = x_ind[i];                                        \
		READ_NA(ss, sl, x0);                                  \
		ss = vis_fpadd32(ss, pbuff[0]);                       \
		SAVE(ss);                                             \
	    }                                                         \
	}                                                             \
	                                                              \
/*                                                                    \
 * ************************************************************       \
 */                                                                   \
	static void FUNC(ch2_x2##SUFF) (mlib_SubsAdap *ws,            \
	    mlib_u8 *sl)                                              \
	{                                                             \
	    mlib_s32 x0;                                              \
	    mlib_d64 s0, s1;                                          \
	                                                              \
	    INIT;                                                     \
	    for (i = 0; i < xsize; i++) {                             \
		x0 = x_ind[i];                                        \
		READ_2NA(s0, s1, sl, x0);                             \
		ss = vis_fpadd32(s0, s1);                             \
		ss = vis_fpadd32(ss, pbuff[0]);                       \
		SAVE(ss);                                             \
	    }                                                         \
	}                                                             \
	                                                              \
/*                                                                    \
 * ************************************************************       \
 */                                                                   \
	static void FUNC(ch2_x3##SUFF) (mlib_SubsAdap *ws,            \
	    mlib_u8 *sl)                                              \
	{                                                             \
	    mlib_s32 x0;                                              \
	    mlib_d64 s0, s1, s2;                                      \
	                                                              \
	    INIT;                                                     \
	    for (i = 0; i < xsize; i++) {                             \
		x0 = x_ind[i];                                        \
		READ_3NA(s0, s1, s2, sl, x0);                         \
		s0 = vis_fpadd32(s0, s1);                             \
		s2 = vis_fpadd32(s2, pbuff[0]);                       \
		ss = vis_fpadd32(s0, s2);                             \
		SAVE(ss);                                             \
	    }                                                         \
	}                                                             \
	                                                              \
/*                                                                    \
 * ************************************************************       \
 */                                                                   \
	static void FUNC(ch2_x4##SUFF) (mlib_SubsAdap *ws,            \
	    mlib_u8 *sl)                                              \
	{                                                             \
	    mlib_s32 x0;                                              \
	    mlib_d64 s0, s1, s2, s3;                                  \
	                                                              \
	    INIT;                                                     \
	    for (i = 0; i < xsize; i++) {                             \
		x0 = x_ind[i];                                        \
		READ_4NA(s0, s1, s2, s3, sl, x0);                     \
		s0 = vis_fpadd32(s0, s1);                             \
		s2 = vis_fpadd32(s2, pbuff[0]);                       \
		ss = vis_fpadd32(s0, s2);                             \
		ss = vis_fpadd32(ss, s3);                             \
		SAVE(ss);                                             \
	    }                                                         \
	}                                                             \
	                                                              \
/*                                                                    \
 * ************************************************************       \
 */                                                                   \
	static void FUNC(ch4_x1##SUFF) (mlib_SubsAdap *ws,            \
	    mlib_u8 *sl)                                              \
	{                                                             \
	    mlib_s32 x0;                                              \
	    mlib_d64 s0, s1;                                          \
	                                                              \
	    INIT;                                                     \
	    for (i = 0; i < xsize; i++) {                             \
		x0 = x_ind[i];                                        \
		READ_2NA(s0, s1, sl, x0);                             \
		s0 = vis_fpadd32(s0, pbuff[0]);                       \
		s1 = vis_fpadd32(s1, pbuff[1]);                       \
		SAVE(s0);                                             \
		SAVE(s1);                                             \
	    }                                                         \
	}                                                             \
	                                                              \
/*                                                                    \
 * ************************************************************       \
 */                                                                   \
	static void FUNC(ch4_x2##SUFF) (mlib_SubsAdap *ws,            \
	    mlib_u8 *sl)                                              \
	{                                                             \
	    mlib_s32 x0;                                              \
	    mlib_d64 s0, s1, s2, s3;                                  \
	                                                              \
	    INIT;                                                     \
	    for (i = 0; i < xsize; i++) {                             \
		x0 = x_ind[i];                                        \
		READ_4NA(s0, s1, s2, s3, sl, x0);                     \
		s0 = vis_fpadd32(s0, pbuff[0]);                       \
		s1 = vis_fpadd32(s1, pbuff[1]);                       \
		s0 = vis_fpadd32(s0, s2);                             \
		s1 = vis_fpadd32(s1, s3);                             \
		SAVE(s0);                                             \
		SAVE(s1);                                             \
	    }                                                         \
	}                                                             \
	                                                              \
/*                                                                    \
 * ************************************************************       \
 */                                                                   \
	static void FUNC(ch4_x3##SUFF) (mlib_SubsAdap *ws,            \
	    mlib_u8 *sl)                                              \
	{                                                             \
	    mlib_s32 x0;                                              \
	    mlib_d64 s0, s1, s2, s3, s4, s5;                          \
	                                                              \
	    INIT;                                                     \
	    for (i = 0; i < xsize; i++) {                             \
		x0 = x_ind[i];                                        \
		READ_6NA(s0, s1, s2, s3, s4, s5, sl, x0);             \
		s0 = vis_fpadd32(s0, pbuff[0]);                       \
		s1 = vis_fpadd32(s1, pbuff[1]);                       \
		s2 = vis_fpadd32(s2, s4);                             \
		s3 = vis_fpadd32(s3, s5);                             \
		s0 = vis_fpadd32(s0, s2);                             \
		s1 = vis_fpadd32(s1, s3);                             \
		SAVE(s0);                                             \
		SAVE(s1);                                             \
	    }                                                         \
	}                                                             \
	                                                              \
/*                                                                    \
 * ************************************************************       \
 */                                                                   \
	static void FUNC(ch4_x4##SUFF) (mlib_SubsAdap *ws,            \
	    mlib_u8 *sl)                                              \
	{                                                             \
	    mlib_s32 x0;                                              \
	    mlib_d64 s0, s1, s2, s3, s4, s5, s6, s7;                  \
	                                                              \
	    INIT;                                                     \
	    for (i = 0; i < xsize; i++) {                             \
		x0 = x_ind[i];                                        \
		READ_8NA(s0, s1, s2, s3, s4, s5, s6, s7, sl, x0);     \
		s0 = vis_fpadd32(s0, s2);                             \
		s1 = vis_fpadd32(s1, s3);                             \
		s4 = vis_fpadd32(s4, s6);                             \
		s5 = vis_fpadd32(s5, s7);                             \
		s0 = vis_fpadd32(s0, pbuff[0]);                       \
		s1 = vis_fpadd32(s1, pbuff[1]);                       \
		s0 = vis_fpadd32(s0, s4);                             \
		s1 = vis_fpadd32(s1, s5);                             \
		SAVE(s0);                                             \
		SAVE(s1);                                             \
	    }                                                         \
	}                                                             \
	                                                              \
/*                                                                    \
 * ************************************************************       \
 */                                                                   \
	static void FUNC(ch3_x2##SUFF) (mlib_SubsAdap *ws,            \
	    mlib_u8 *sl)                                              \
	{                                                             \
	    mlib_s32 x0;                                              \
	    mlib_d64 s0, s1, s2;                                      \
	                                                              \
	    INIT;                                                     \
	    for (i = 0; i < xsize; i++) {                             \
		x0 = x_ind[i];                                        \
		READ_3NA(s0, s1, s2, sl, x0);                         \
		s0 = vis_freg_pair(vis_fpadd32s(vis_read_hi(s0),      \
		    vis_read_lo(s1)), vis_fpadd32s(vis_read_lo(s0),   \
		    vis_read_hi(s2)));                                \
		s1 = vis_freg_pair(vis_fpadd32s(vis_read_hi(s1),      \
		    vis_read_lo(s2)), vis_read_lo(s1));               \
		s0 = vis_fpadd32(s0, pbuff[0]);                       \
		s1 = vis_fpadd32(s1, pbuff[1]);                       \
		SAVE(s0);                                             \
		SAVE(s1);                                             \
	    }                                                         \
	}                                                             \
	                                                              \
/*                                                                    \
 * ************************************************************       \
 */                                                                   \
	static void FUNC(ch3_x3##SUFF) (mlib_SubsAdap *ws,            \
	    mlib_u8 *sl)                                              \
	{                                                             \
	    mlib_s32 x0;                                              \
	    mlib_d64 s0, s1, s2, s3, s4;                              \
	                                                              \
	    INIT;                                                     \
	    for (i = 0; i < xsize; i++) {                             \
		x0 = x_ind[i];                                        \
		READ_5NA(s0, s1, s2, s3, s4, sl, x0);                 \
		s0 = vis_fpadd32(s0, s3);                             \
		s0 = vis_freg_pair(vis_fpadd32s(vis_read_hi(s0),      \
		    vis_read_lo(s1)), vis_fpadd32s(vis_read_lo(s0),   \
		    vis_read_hi(s2)));                                \
		s1 = vis_freg_pair(vis_fpadd32s(vis_read_hi(s1),      \
		    vis_read_lo(s2)), vis_read_lo(s1));               \
		s1 = vis_fpadd32(s1, s4);                             \
		s0 = vis_fpadd32(s0, pbuff[0]);                       \
		s1 = vis_fpadd32(s1, pbuff[1]);                       \
		SAVE(s0);                                             \
		SAVE(s1);                                             \
	    }                                                         \
	}                                                             \
	                                                              \
/*                                                                    \
 * ************************************************************       \
 */                                                                   \
	static void FUNC(ch3_x4##SUFF) (mlib_SubsAdap *ws,            \
	    mlib_u8 *sl)                                              \
	{                                                             \
	    mlib_s32 x0;                                              \
	    mlib_d64 s0, s1, s2, s3, s4, s5;                          \
	                                                              \
	    INIT;                                                     \
	    for (i = 0; i < xsize; i++) {                             \
		x0 = x_ind[i];                                        \
		READ_6NA(s0, s1, s2, s3, s4, s5, sl, x0);             \
		s0 = vis_fpadd32(s0, s3);                             \
		s1 = vis_fpadd32(s1, s4);                             \
		s2 = vis_fpadd32(s2, s5);                             \
		s0 = vis_freg_pair(vis_fpadd32s(vis_read_hi(s0),      \
		    vis_read_lo(s1)), vis_fpadd32s(vis_read_lo(s0),   \
		    vis_read_hi(s2)));                                \
		s1 = vis_freg_pair(vis_fpadd32s(vis_read_hi(s1),      \
		    vis_read_lo(s2)), vis_read_lo(s1));               \
		s0 = vis_fpadd32(s0, pbuff[0]);                       \
		s1 = vis_fpadd32(s1, pbuff[1]);                       \
		SAVE(s0);                                             \
		SAVE(s1);                                             \
	    }                                                         \
	}                                                             \
	                                                              \
/*                                                                    \
 * ************************************************************       \
 */                                                                   \
	static const func_x func_arr_x##SUFF[] = {                    \
	    FUNC(ch1_x1##SUFF), FUNC(ch1_x2##SUFF),                   \
	    FUNC(ch1_x3##SUFF), FUNC(ch1_x4##SUFF),                   \
	    FUNC(ch2_x1##SUFF), FUNC(ch2_x2##SUFF),                   \
	    FUNC(ch2_x3##SUFF), FUNC(ch2_x4##SUFF),                   \
	    FUNC(ch4_x1##SUFF), FUNC(ch3_x2##SUFF),                   \
	    FUNC(ch3_x3##SUFF), FUNC(ch3_x4##SUFF),                   \
	    FUNC(ch4_x1##SUFF), FUNC(ch4_x2##SUFF),                   \
	    FUNC(ch4_x3##SUFF), FUNC(ch4_x4##SUFF)                    \
	}

/* *********************************************************** */

FUNC_X(a, INIT_ADD, SAVE_ADD);
FUNC_X(m, INIT_MUL, SAVE_MUL);

/* *********************************************************** */
