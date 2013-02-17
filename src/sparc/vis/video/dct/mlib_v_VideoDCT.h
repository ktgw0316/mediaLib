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

#ifndef _MLIB_V_VIDEODCT_H
#define	_MLIB_V_VIDEODCT_H

#pragma ident	"@(#)mlib_v_VideoDCT.h	9.4	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

/* *********************************************************** */

#ifdef __ENABLE_DECLARE_ALIGN

#pragma align 128(mlib_ccf16, mlib_ccf16_12, mlib_ccf6)
#pragma align 128(mlib_cTable)

#else		   /* __ENABLE_DECLARE_ALIGN */

extern const mlib_u32 mlib_ccf16[];
extern const mlib_u32 mlib_ccf16_12[];
extern const mlib_u32 mlib_ccf6[];
extern const mlib_u16 mlib_cTable[];

#endif /* __ENABLE_DECLARE_ALIGN */

/* *********************************************************** */

/*
 * Definition
 */

/*
 * Common
 */

#define	TRANSPOSE(p0, p1, p2, p3, r0, r1, r2, r3)                       \
	{                                                               \
	    mlib_d64 m02, m13, m0213;                                   \
	                                                                \
	    m02 = vis_fpmerge(vis_read_hi(p0), vis_read_hi(p2));        \
	    m13 = vis_fpmerge(vis_read_hi(p1), vis_read_hi(p3));        \
	    m0213 = vis_fpmerge(vis_read_hi(m02), vis_read_hi(m13));    \
	    r0 = vis_fpmerge(vis_read_hi(m0213), vis_read_lo(m0213));   \
	    m0213 = vis_fpmerge(vis_read_lo(m02), vis_read_lo(m13));    \
	    r1 = vis_fpmerge(vis_read_hi(m0213), vis_read_lo(m0213));   \
	    m02 = vis_fpmerge(vis_read_lo(p0), vis_read_lo(p2));        \
	    m13 = vis_fpmerge(vis_read_lo(p1), vis_read_lo(p3));        \
	    m0213 = vis_fpmerge(vis_read_hi(m02), vis_read_hi(m13));    \
	    r2 = vis_fpmerge(vis_read_hi(m0213), vis_read_lo(m0213));   \
	    m0213 = vis_fpmerge(vis_read_lo(m02), vis_read_lo(m13));    \
	    r3 = vis_fpmerge(vis_read_hi(m0213), vis_read_lo(m0213));   \
	}

/* *********************************************************** */

/*
 * For DCT
 */

#define	MUL(x, y)	vis_fpadd16(vis_fmul8sux16(x, y), vis_fmul8ulx16(x, y))

/* *********************************************************** */

#define	LOADCONSTS5                                             \
	FONE = ((mlib_f32 *)mlib_ccf6)[4];                      \
	FCOS = ((mlib_f32 *)mlib_ccf6)[0];                      \
	c17 = ((mlib_f32 *)mlib_ccf6)[1];                       \
	c26 = ((mlib_f32 *)mlib_ccf6)[2];                       \
	c35 = ((mlib_f32 *)mlib_ccf6)[3];

/* *********************************************************** */

#define	LOADCONSTS4                                             \
	FCOS = ((mlib_f32 *)mlib_ccf6)[0];                      \
	c17 = ((mlib_f32 *)mlib_ccf6)[1];                       \
	c26 = ((mlib_f32 *)mlib_ccf6)[2];                       \
	c35 = ((mlib_f32 *)mlib_ccf6)[3];

/* *********************************************************** */

#define	LOADCONSTS4_12                                          \
	FCOS = ((mlib_f32 *)mlib_ccf6)[0];                      \
	c17 = ((mlib_f32 *)mlib_ccf6)[1];                       \
	c26 = ((mlib_f32 *)mlib_ccf6)[2];                       \
	c35 = ((mlib_f32 *)mlib_ccf6)[3];                       \
	c_4 = ((mlib_f32 *)mlib_ccf6)[5];

/* *********************************************************** */

#define	TRANSPOSE_8X8(i0, i1, i2, i3, i4, i5, i6, i7, o0, o1,           \
	o2, o3, o4, o5, o6, o7)                                         \
	{                                                               \
	    mlib_d64 m04, m26, m15, m37, m0426, m1537;                  \
	                                                                \
	    m04 = vis_fpmerge(vis_read_hi(i0), vis_read_hi(i4));        \
	    m26 = vis_fpmerge(vis_read_hi(i2), vis_read_hi(i6));        \
	    m15 = vis_fpmerge(vis_read_hi(i1), vis_read_hi(i5));        \
	    m37 = vis_fpmerge(vis_read_hi(i3), vis_read_hi(i7));        \
	    m0426 = vis_fpmerge(vis_read_hi(m04), vis_read_hi(m26));    \
	    m1537 = vis_fpmerge(vis_read_hi(m15), vis_read_hi(m37));    \
	    o0 = vis_fpmerge(vis_read_hi(m0426), vis_read_hi(m1537));   \
	    o1 = vis_fpmerge(vis_read_lo(m0426), vis_read_lo(m1537));   \
	    m0426 = vis_fpmerge(vis_read_lo(m04), vis_read_lo(m26));    \
	    m1537 = vis_fpmerge(vis_read_lo(m15), vis_read_lo(m37));    \
	    o2 = vis_fpmerge(vis_read_hi(m0426), vis_read_hi(m1537));   \
	    o3 = vis_fpmerge(vis_read_lo(m0426), vis_read_lo(m1537));   \
	    m04 = vis_fpmerge(vis_read_lo(i0), vis_read_lo(i4));        \
	    m26 = vis_fpmerge(vis_read_lo(i2), vis_read_lo(i6));        \
	    m15 = vis_fpmerge(vis_read_lo(i1), vis_read_lo(i5));        \
	    m37 = vis_fpmerge(vis_read_lo(i3), vis_read_lo(i7));        \
	    m0426 = vis_fpmerge(vis_read_hi(m04), vis_read_hi(m26));    \
	    m1537 = vis_fpmerge(vis_read_hi(m15), vis_read_hi(m37));    \
	    o4 = vis_fpmerge(vis_read_hi(m0426), vis_read_hi(m1537));   \
	    o5 = vis_fpmerge(vis_read_lo(m0426), vis_read_lo(m1537));   \
	    m0426 = vis_fpmerge(vis_read_lo(m04), vis_read_lo(m26));    \
	    m1537 = vis_fpmerge(vis_read_lo(m15), vis_read_lo(m37));    \
	    o6 = vis_fpmerge(vis_read_hi(m0426), vis_read_hi(m1537));   \
	    o7 = vis_fpmerge(vis_read_lo(m0426), vis_read_lo(m1537));   \
	}

/* *********************************************************** */

#define	STORE_DATA_GE(out)                                      \
	dpdst = (mlib_d64 *)((mlib_addr)pdst & ~7);             \
	emask = 0xff >> ((mlib_addr)pdst & 7);                  \
	vis_alignaddrl((void *)pdst, 0);                        \
	out = vis_faligndata(out, out);                         \
	vis_pst_8(out, dpdst, emask);                           \
	vis_pst_8(out, dpdst + 1, ~emask);

/* *********************************************************** */

#define	LOAD_DATA_AA_INTRA                                      \
	d00 = *(mlib_d64 *)sp;                                  \
	sp += stride;                                           \
	d10 = *(mlib_d64 *)sp;                                  \
	sp += stride;                                           \
	d20 = *(mlib_d64 *)sp;                                  \
	sp += stride;                                           \
	d30 = *(mlib_d64 *)sp;                                  \
	sp += stride;                                           \
	d40 = *(mlib_d64 *)sp;                                  \
	sp += stride;                                           \
	d50 = *(mlib_d64 *)sp;                                  \
	sp += stride;                                           \
	d60 = *(mlib_d64 *)sp;                                  \
	sp += stride;                                           \
	d70 = *(mlib_d64 *)sp;

/* *********************************************************** */

#define	LOAD_DATA_GE_INTRA                                      \
	dp = (mlib_d64 *)vis_alignaddr(sp, 0);                  \
	d00 = dp[0];                                            \
	d01 = dp[1];                                            \
	sp += stride;                                           \
	d00 = vis_faligndata(d00, d01);                         \
	dp = (mlib_d64 *)vis_alignaddr(sp, 0);                  \
	d10 = dp[0];                                            \
	d11 = dp[1];                                            \
	sp += stride;                                           \
	d10 = vis_faligndata(d10, d11);                         \
	dp = (mlib_d64 *)vis_alignaddr(sp, 0);                  \
	d20 = dp[0];                                            \
	d21 = dp[1];                                            \
	sp += stride;                                           \
	d20 = vis_faligndata(d20, d21);                         \
	dp = (mlib_d64 *)vis_alignaddr(sp, 0);                  \
	d30 = dp[0];                                            \
	d31 = dp[1];                                            \
	sp += stride;                                           \
	d30 = vis_faligndata(d30, d31);                         \
	dp = (mlib_d64 *)vis_alignaddr(sp, 0);                  \
	d40 = dp[0];                                            \
	d41 = dp[1];                                            \
	sp += stride;                                           \
	d40 = vis_faligndata(d40, d41);                         \
	dp = (mlib_d64 *)vis_alignaddr(sp, 0);                  \
	d50 = dp[0];                                            \
	d51 = dp[1];                                            \
	sp += stride;                                           \
	d50 = vis_faligndata(d50, d51);                         \
	dp = (mlib_d64 *)vis_alignaddr(sp, 0);                  \
	d60 = dp[0];                                            \
	d61 = dp[1];                                            \
	sp += stride;                                           \
	d60 = vis_faligndata(d60, d61);                         \
	dp = (mlib_d64 *)vis_alignaddr(sp, 0);                  \
	d70 = dp[0];                                            \
	d71 = vis_ld_d64_nf(dp + 1);                            \
	d70 = vis_faligndata(d70, d71);

/* *********************************************************** */

#define	PREPARE_DATA_INTRA(hl, n)                               \
	r00 = vis_fmul8x16al(vis_read_##hl(d01), FONE);         \
	r70 = vis_fmul8x16al(vis_read_##hl(d71), FONE);         \
	t0##n = vis_fpadd16(r00, r70);                          \
	t7##n = vis_fpsub16(r00, r70);                          \
	r10 = vis_fmul8x16al(vis_read_##hl(d11), FONE);         \
	r60 = vis_fmul8x16al(vis_read_##hl(d61), FONE);         \
	t1##n = vis_fpadd16(r10, r60);                          \
	t6##n = vis_fpsub16(r10, r60);                          \
	r20 = vis_fmul8x16al(vis_read_##hl(d21), FONE);         \
	r50 = vis_fmul8x16al(vis_read_##hl(d51), FONE);         \
	t2##n = vis_fpadd16(r20, r50);                          \
	t5##n = vis_fpsub16(r20, r50);                          \
	r30 = vis_fmul8x16al(vis_read_##hl(d31), FONE);         \
	r40 = vis_fmul8x16al(vis_read_##hl(d41), FONE);         \
	t3##n = vis_fpadd16(r30, r40);                          \
	t4##n = vis_fpsub16(r30, r40);

/* *********************************************************** */

#define	LOAD_DATA_AA_INTER1                                     \
	d00 = sp[0];                                            \
	d10 = sp[1];                                            \
	d20 = sp[2];                                            \
	d30 = sp[3];                                            \
	d40 = sp[4];                                            \
	d50 = sp[5];                                            \
	d60 = sp[6];                                            \
	d70 = sp[7];

/* *********************************************************** */

#define	LOAD_DATA_AA_INTER2                                     \
	d01 = sp[8];                                            \
	d11 = sp[9];                                            \
	d21 = sp[10];                                           \
	d31 = sp[11];                                           \
	d41 = sp[12];                                           \
	d51 = sp[13];                                           \
	d61 = sp[14];                                           \
	d71 = sp[15];

/* *********************************************************** */

#define	LOAD_DATA_GE_INTER1                                     \
	dp = (mlib_d64 *)vis_alignaddr(sp, 0);                  \
	d00 = dp[0];                                            \
	d10 = dp[1];                                            \
	d20 = dp[2];                                            \
	d00 = vis_faligndata(d00, d10);                         \
	d30 = dp[3];                                            \
	d10 = vis_faligndata(d10, d20);                         \
	d40 = dp[4];                                            \
	d20 = vis_faligndata(d20, d30);                         \
	d50 = dp[5];                                            \
	d30 = vis_faligndata(d30, d40);                         \
	d60 = dp[6];                                            \
	d40 = vis_faligndata(d40, d50);                         \
	d70 = dp[7];                                            \
	d50 = vis_faligndata(d50, d60);                         \
	d60 = vis_faligndata(d60, d70);                         \
	d70 = vis_faligndata(d70, dp[8]);

/* *********************************************************** */

#define	LOAD_DATA_GE_INTER2                                     \
	d01 = dp[8];                                            \
	d11 = dp[9];                                            \
	d21 = dp[10];                                           \
	d01 = vis_faligndata(d01, d11);                         \
	d31 = dp[11];                                           \
	d11 = vis_faligndata(d11, d21);                         \
	d41 = dp[12];                                           \
	d21 = vis_faligndata(d21, d31);                         \
	d51 = dp[13];                                           \
	d31 = vis_faligndata(d31, d41);                         \
	d61 = dp[14];                                           \
	d41 = vis_faligndata(d41, d51);                         \
	d71 = dp[15];                                           \
	d51 = vis_faligndata(d51, d61);                         \
	d61 = vis_faligndata(d61, d71);                         \
	d71 = vis_faligndata(d71, vis_ld_d64_nf(dp + 16));

/* *********************************************************** */

#define	PREPARE_DATA_INTER(n)                                   \
	t0##n = vis_fpadd16(r0##n, r7##n);                      \
	t7##n = vis_fpsub16(r0##n, r7##n);                      \
	t1##n = vis_fpadd16(r1##n, r6##n);                      \
	t6##n = vis_fpsub16(r1##n, r6##n);                      \
	t2##n = vis_fpadd16(r2##n, r5##n);                      \
	t5##n = vis_fpsub16(r2##n, r5##n);                      \
	t3##n = vis_fpadd16(r3##n, r4##n);                      \
	t4##n = vis_fpsub16(r3##n, r4##n);

/* *********************************************************** */

#define	COMPUTING_DATA(n)                                       \
	t8##n =                                                 \
	vis_fmul8x16(FCOS, vis_fpadd16(t6##n, t5##n));          \
	t9##n =                                                 \
	vis_fmul8x16(FCOS, vis_fpsub16(t6##n, t5##n));          \
	t5##n = vis_fpadd16(t3##n, t0##n);                      \
	t6##n = vis_fpadd16(t2##n, t1##n);                      \
	t3##n = vis_fpsub16(t0##n, t3##n);                      \
	t2##n = vis_fpsub16(t1##n, t2##n);                      \
	t0##n = vis_fpsub16(t7##n, t8##n);                      \
	t1##n = vis_fpadd16(t4##n, t9##n);                      \
	t7##n = vis_fpadd16(t7##n, t8##n);                      \
	t4##n = vis_fpsub16(t4##n, t9##n);                      \
	d0##n = vis_fpadd16(t5##n, t6##n);                      \
	d1##n =                                                 \
	vis_fpadd16(t7##n, vis_fmul8x16(c17, t1##n));           \
	d2##n =                                                 \
	vis_fpadd16(t3##n, vis_fmul8x16(c26, t2##n));           \
	d3##n =                                                 \
	vis_fpsub16(t0##n, vis_fmul8x16(c35, t4##n));           \
	d4##n = vis_fpsub16(t5##n, t6##n);                      \
	d5##n =                                                 \
	vis_fpadd16(vis_fmul8x16(c35, t0##n), t4##n);           \
	d6##n =                                                 \
	vis_fpsub16(vis_fmul8x16(c26, t3##n), t2##n);           \
	d7##n =                                                 \
	vis_fpsub16(vis_fmul8x16(c17, t7##n), t1##n);

/* *********************************************************** */

#define	COMPUTING_DATA_12(n)                                    \
	t0##n = vis_fmul8x16(c_4, t0##n);                       \
	t1##n = vis_fmul8x16(c_4, t1##n);                       \
	t2##n = vis_fmul8x16(c_4, t2##n);                       \
	t3##n = vis_fmul8x16(c_4, t3##n);                       \
	t4##n = vis_fmul8x16(c_4, t4##n);                       \
	t5##n = vis_fmul8x16(c_4, t5##n);                       \
	t6##n = vis_fmul8x16(c_4, t6##n);                       \
	t7##n = vis_fmul8x16(c_4, t7##n);                       \
	t8##n =                                                 \
	vis_fmul8x16(FCOS, vis_fpadd16(t6##n, t5##n));          \
	t9##n =                                                 \
	vis_fmul8x16(FCOS, vis_fpsub16(t6##n, t5##n));          \
	t5##n = vis_fpadd16(t3##n, t0##n);                      \
	t6##n = vis_fpadd16(t2##n, t1##n);                      \
	t3##n = vis_fpsub16(t0##n, t3##n);                      \
	t2##n = vis_fpsub16(t1##n, t2##n);                      \
	t0##n = vis_fpsub16(t7##n, t8##n);                      \
	t1##n = vis_fpadd16(t4##n, t9##n);                      \
	t7##n = vis_fpadd16(t7##n, t8##n);                      \
	t4##n = vis_fpsub16(t4##n, t9##n);                      \
	d0##n = vis_fpadd16(t5##n, t6##n);                      \
	d1##n =                                                 \
	vis_fpadd16(t7##n, vis_fmul8x16(c17, t1##n));           \
	d2##n =                                                 \
	vis_fpadd16(t3##n, vis_fmul8x16(c26, t2##n));           \
	d3##n =                                                 \
	vis_fpsub16(t0##n, vis_fmul8x16(c35, t4##n));           \
	d4##n = vis_fpsub16(t5##n, t6##n);                      \
	d5##n =                                                 \
	vis_fpadd16(vis_fmul8x16(c35, t0##n), t4##n);           \
	d6##n =                                                 \
	vis_fpsub16(vis_fmul8x16(c26, t3##n), t2##n);           \
	d7##n =                                                 \
	vis_fpsub16(vis_fmul8x16(c17, t7##n), t1##n);

/* *********************************************************** */

#define	ENDSCALE(n)                                             \
	{                                                       \
	    mlib_d64 sc0 = ((mlib_d64 *)mlib_ccf16)[0 + n];     \
	    mlib_d64 sc1 = ((mlib_d64 *)mlib_ccf16)[2 + n];     \
	    mlib_d64 sc2 = ((mlib_d64 *)mlib_ccf16)[4 + n];     \
	    mlib_d64 sc3 = ((mlib_d64 *)mlib_ccf16)[6 + n];     \
	                                                        \
	    d0##n = MUL(d0##n, sc0);                            \
	    d1##n = MUL(d1##n, sc1);                            \
	    d2##n = MUL(d2##n, sc2);                            \
	    d3##n = MUL(d3##n, sc3);                            \
	    d4##n = MUL(d4##n, sc0);                            \
	    d5##n = MUL(d5##n, sc3);                            \
	    d6##n = MUL(d6##n, sc2);                            \
	    d7##n = MUL(d7##n, sc1);                            \
	}

/* *********************************************************** */


#define	ENDSCALE_12(n)                                           \
	{                                                        \
	    mlib_d64 sc0 = ((mlib_d64 *)mlib_ccf16_12)[0 + n];   \
	    mlib_d64 sc1 = ((mlib_d64 *)mlib_ccf16_12)[2 + n];   \
	    mlib_d64 sc2 = ((mlib_d64 *)mlib_ccf16_12)[4 + n];   \
	    mlib_d64 sc3 = ((mlib_d64 *)mlib_ccf16_12)[6 + n];   \
	                                                         \
	    d0##n = MUL(d0##n, sc0);                             \
	    d1##n = MUL(d1##n, sc1);                             \
	    d2##n = MUL(d2##n, sc2);                             \
	    d3##n = MUL(d3##n, sc3);                             \
	    d4##n = MUL(d4##n, sc0);                             \
	    d5##n = MUL(d5##n, sc3);                             \
	    d6##n = MUL(d6##n, sc2);                             \
	    d7##n = MUL(d7##n, sc1);                             \
	    d0##n = vis_fpadd16(d0##n, d0##n);                   \
	    d1##n = vis_fpadd16(d1##n, d1##n);                   \
	    d2##n = vis_fpadd16(d2##n, d2##n);                   \
	    d3##n = vis_fpadd16(d3##n, d3##n);                   \
	    d4##n = vis_fpadd16(d4##n, d4##n);                   \
	    d5##n = vis_fpadd16(d5##n, d5##n);                   \
	    d6##n = vis_fpadd16(d6##n, d6##n);                   \
	    d7##n = vis_fpadd16(d7##n, d7##n);                   \
	}

/* *********************************************************** */
/*
 * For IDCT
 *
 */

#define	MUL_S16_S32A(dx, dy)                                           \
	{                                                              \
	    mlib_d64 dr, dr1, dr2, dr3;                                \
	                                                               \
	    dr = vis_fmuld8sux16(vis_read_hi(dx), vis_read_hi(dy));    \
	    dr1 = vis_fmuld8ulx16(vis_read_hi(dx), vis_read_hi(dy));   \
	    dr = vis_fpadd32(dr, dr1);                                 \
	    dr2 = vis_fmuld8sux16(vis_read_lo(dx), vis_read_lo(dy));   \
	    dr3 = vis_fmuld8ulx16(vis_read_lo(dx), vis_read_lo(dy));   \
	    dr1 = vis_fpadd32(dr2, dr3);                               \
	    dx = vis_fpackfix_pair(dr, dr1);			       \
	}

#define	LOAD_AA1_DEQUANT					\
	LOAD_DATA_AA1						\
	d0 = dPtr1[0];						\
	MUL_S16_S32A(p00, d0);					\
	d1 = dPtr1[1];						\
	MUL_S16_S32A(p01, d1);					\
	d2 = dPtr1[2];						\
	MUL_S16_S32A(p10, d2);					\
	d3 = dPtr1[3];						\
	MUL_S16_S32A(p11, d3);					\
	d4 = dPtr1[4];						\
	MUL_S16_S32A(p20, d4);					\
	d5 = dPtr1[5];						\
	MUL_S16_S32A(p21, d5);					\
	d6 = dPtr1[6];						\
	MUL_S16_S32A(p30, d6);					\
	d7 = dPtr1[7];						\
	MUL_S16_S32A(p31, d7);

#define	LOAD_DATA_AA1                                           \
	p00 = dPtr[0];                                          \
	p01 = dPtr[1];                                          \
	p10 = dPtr[2];                                          \
	p11 = dPtr[3];                                          \
	p20 = dPtr[4];                                          \
	p21 = dPtr[5];                                          \
	p30 = dPtr[6];                                          \
	p31 = dPtr[7];

/* *********************************************************** */

#define	LOAD_AA2_DEQUANT					\
	LOAD_DATA_AA2						\
	d0 = dPtr1[8];						\
	MUL_S16_S32A(p40, d0);					\
	d1 = dPtr1[9];						\
	MUL_S16_S32A(p41, d1);					\
	d2 = dPtr1[10];						\
	MUL_S16_S32A(p50, d2);					\
	d3 = dPtr1[11];						\
	MUL_S16_S32A(p51, d3);					\
	d4 = dPtr1[12];						\
	MUL_S16_S32A(p60, d4);					\
	d5 = dPtr1[13];						\
	MUL_S16_S32A(p61, d5);					\
	d6 = dPtr1[14];						\
	MUL_S16_S32A(p70, d6);					\
	d7 = dPtr1[15];						\
	MUL_S16_S32A(p71, d7);

#define	LOAD_DATA_AA2                                           \
	p40 = dPtr[8];                                          \
	p41 = dPtr[9];                                          \
	p50 = dPtr[10];                                         \
	p51 = dPtr[11];                                         \
	p60 = dPtr[12];                                         \
	p61 = dPtr[13];                                         \
	p70 = dPtr[14];                                         \
	p71 = dPtr[15];

/* *********************************************************** */

#define	LOAD_DEQUANT_GE1					\
	d0 = vis_faligndata(dPtr1[0], dPtr1[1]);		\
	MUL_S16_S32A(p00, d0);					\
	d1 = vis_faligndata(dPtr1[1], dPtr1[2]);		\
	MUL_S16_S32A(p01, d1);					\
	d2 = vis_faligndata(dPtr1[2], dPtr1[3]);		\
	MUL_S16_S32A(p10, d2);					\
	d3 = vis_faligndata(dPtr1[3], dPtr1[4]);		\
	MUL_S16_S32A(p11, d3);					\
	d4 = vis_faligndata(dPtr1[4], dPtr1[5]);		\
	MUL_S16_S32A(p20, d4);					\
	d5 = vis_faligndata(dPtr1[5], dPtr1[6]);		\
	MUL_S16_S32A(p21, d5);					\
	d6 = vis_faligndata(dPtr1[6], dPtr1[7]);		\
	MUL_S16_S32A(p30, d6);					\
	d7 = vis_faligndata(dPtr1[7], dPtr1[8]);		\
	MUL_S16_S32A(p31, d7);

#define	LOAD_DATA_GE1                                           \
	p00 = vis_faligndata(dPtr[0], dPtr[1]);                 \
	p01 = vis_faligndata(dPtr[1], dPtr[2]);                 \
	p10 = vis_faligndata(dPtr[2], dPtr[3]);                 \
	p11 = vis_faligndata(dPtr[3], dPtr[4]);                 \
	p20 = vis_faligndata(dPtr[4], dPtr[5]);                 \
	p21 = vis_faligndata(dPtr[5], dPtr[6]);                 \
	p30 = vis_faligndata(dPtr[6], dPtr[7]);                 \
	p31 = vis_faligndata(dPtr[7], dPtr[8]);

/* *********************************************************** */

#define	LOAD_DEQUANT_GE2					\
	d0 = vis_faligndata(dPtr1[8], dPtr1[9]);		\
	MUL_S16_S32A(p40, d0);					\
	d1 = vis_faligndata(dPtr1[9], dPtr1[10]);		\
	MUL_S16_S32A(p41, d1);					\
	d2 = vis_faligndata(dPtr1[10], dPtr1[11]);		\
	MUL_S16_S32A(p50, d2);					\
	d3 = vis_faligndata(dPtr1[11], dPtr1[12]);		\
	MUL_S16_S32A(p51, d3);					\
	d4 = vis_faligndata(dPtr1[12], dPtr1[13]);		\
	MUL_S16_S32A(p60, d4);					\
	d5 = vis_faligndata(dPtr1[13], dPtr1[14]);		\
	MUL_S16_S32A(p61, d5);					\
	d6 = vis_faligndata(dPtr1[14], dPtr1[15]);		\
	MUL_S16_S32A(p70, d6);					\
	d7 = vis_faligndata(dPtr1[15], vis_ld_d64_nf(dPtr1 + 16)); \
	MUL_S16_S32A(p71, d7);

#define	LOAD_DATA_GE2                                           \
	p40 = vis_faligndata(dPtr[8], dPtr[9]);                 \
	p41 = vis_faligndata(dPtr[9], dPtr[10]);                \
	p50 = vis_faligndata(dPtr[10], dPtr[11]);               \
	p51 = vis_faligndata(dPtr[11], dPtr[12]);               \
	p60 = vis_faligndata(dPtr[12], dPtr[13]);               \
	p61 = vis_faligndata(dPtr[13], dPtr[14]);               \
	p70 = vis_faligndata(dPtr[14], dPtr[15]);               \
	p71 = vis_faligndata(dPtr[15], vis_ld_d64_nf(dPtr + 16));

/* *********************************************************** */

#define	MULf(m0, m1)	vis_fmul8x16(m1, m0)

/* *********************************************************** */

#define	IDCT(d0, d1, d2, d3, d4, d5, d6, d7)                           \
	dx2 = vis_fmul8x16(COS_4_16, vis_fpadd16(d3, d5));             \
	dx4 = vis_fmul8x16(COS_4_16, vis_fpsub16(d3, d5));             \
	dx1 = vis_fmul8x16(COS_4_16, vis_fpadd16(d1, d7));             \
	dx6 = vis_fmul8x16(COS_4_16, vis_fpsub16(d1, d7));             \
	dx7 = vis_fpadd16(d1, dx2);                                    \
	dx8 = vis_fpadd16(d7, dx4);                                    \
	dx3 = vis_fpsub16(dx1, d3);                                    \
	dx0 = vis_fpsub16(dx6, d5);                                    \
	dx1 = vis_fpadd16(MULf(dx7, COS_1_16), MULf(dx8, COS_7_16));   \
	dx4 = vis_fpsub16(MULf(dx3, COS_1_16), MULf(dx0, COS_7_16));   \
	dx2 = vis_fpadd16(MULf(dx0, COS_1_16), MULf(dx3, COS_7_16));   \
	dx6 = vis_fpsub16(MULf(dx7, COS_7_16), MULf(dx8, COS_1_16));   \
	t0 = vis_fpadd16(MULf(d2, COS_2_16), MULf(d6, COS_6_16));      \
	t1 = vis_fpsub16(MULf(d2, COS_6_16), MULf(d6, COS_2_16));      \
	dx3 = vis_fmul8x16(COS_4_16, vis_fpsub16(d0, d4));             \
	dx8 = vis_fmul8x16(COS_4_16, vis_fpadd16(d0, d4));             \
	dx7 = vis_fpadd16(dx8, t0);                                    \
	dx0 = vis_fpsub16(dx3, t1);                                    \
	dx3 = vis_fpadd16(dx3, t1);                                    \
	dx8 = vis_fpsub16(dx8, t0);

/* *********************************************************** */

#define	TRANSPACK_8X4(i0, i1, i2, i3, i4, i5, i6, i7, o0, o1, o2, o3)   \
	{                                                               \
	    mlib_d64 m04, m26, m15, m37, m0426, m1537;                  \
	                                                                \
	    m04 = vis_fpmerge(vis_fpack16(i0), vis_fpack16(i4));        \
	    m26 = vis_fpmerge(vis_fpack16(i2), vis_fpack16(i6));        \
	    m15 = vis_fpmerge(vis_fpack16(i1), vis_fpack16(i5));        \
	    m37 = vis_fpmerge(vis_fpack16(i3), vis_fpack16(i7));        \
	    m0426 = vis_fpmerge(vis_read_hi(m04), vis_read_hi(m26));    \
	    m1537 = vis_fpmerge(vis_read_hi(m15), vis_read_hi(m37));    \
	    o0 = vis_fpmerge(vis_read_hi(m0426), vis_read_hi(m1537));   \
	    o1 = vis_fpmerge(vis_read_lo(m0426), vis_read_lo(m1537));   \
	    m0426 = vis_fpmerge(vis_read_lo(m04), vis_read_lo(m26));    \
	    m1537 = vis_fpmerge(vis_read_lo(m15), vis_read_lo(m37));    \
	    o2 = vis_fpmerge(vis_read_hi(m0426), vis_read_hi(m1537));   \
	    o3 = vis_fpmerge(vis_read_lo(m0426), vis_read_lo(m1537));   \
	}

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_V_VIDEODCT_H */
