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

#pragma ident	"@(#)mlib_v_ImageFlipMainDiag.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *      Internal functions for mlib_ImageFlipMainDiag
 *
 * IMPORTANT:
 *       Functions are the same as for mlib_MatrixTranspose_*
 *
 */

#include <vis_proto.h>
#include <mlib_image.h>

/* *********************************************************** */

#define	FNAME_CH1_AL(TT)	mlib_v_ImageFlipMainDiag_##TT##_1
#define	FNAME_CH1_GE(TT)	mlib_v_ImageFlipMainDiag_##TT##_1ge

#define	FNAME_CH2_GE(TT)	mlib_v_ImageFlipMainDiag_##TT##_2

#define	FNAME_CH4_GE(TT)	mlib_v_ImageFlipMainDiag_##TT##_4

/* *********************************************************** */

#define	PARAMS(TYPE)                                                      \
	void *pz, void *px, mlib_s32 sh, mlib_s32 sw, mlib_s32 sstride,   \
	mlib_s32 dstride

/* *********************************************************** */

#define	CALL_PARAMS	(void*)pz, (void*)px, sh, sw, sstride, dstride

/* *********************************************************** */

#define	CHECK_CH1(TT)

/* *********************************************************** */

#define	CHECK_CH2(TT)                                           \
	if (sw < 8 || sh < 8) {                                 \
	    if (sw <= 0 || sh <= 0)                             \
		return (MLIB_FAILURE);                          \
	    STRIP_2(ddata, sdata, sw, sh, mlib_##TT);           \
	    return (MLIB_SUCCESS);                              \
	}

/* *********************************************************** */

#define	CHECK_CH4(TT)                                           \
	if (sw < 8 || sh < 8) {                                 \
	    if (sw <= 0 || sh <= 0)                             \
		return (MLIB_FAILURE);                          \
	    STRIP_4(ddata, sdata, sw, sh, mlib_##TT);           \
	    return (MLIB_SUCCESS);                              \
	}

/* *********************************************************** */

#define	dw	sh
#define	dh	sw

/* *********************************************************** */

#define	PREPAREVARS(data_type, num_ch)                          \
	mlib_s32 xsq, ysq;                                      \
	mlib_s32 s_head, d_head, s_tail, d_tail;                \
	mlib_s32 numhorzsq, numvertsq;                          \
	mlib_s32 ii, jj;                                        \
	data_type *sdata = (data_type *) px;                    \
	data_type *ddata = (data_type *) pz;                    \
	data_type *sl, *dl, *sp, *dp

/* *********************************************************** */

#define	mlib_U8	mlib_u8
#define	mlib_S16	mlib_s16

#define	mlib_S32	mlib_s32

#define	mlib_D64	mlib_d64

/* *********************************************************** */

#define	STRIP_1(pd, ps, w, h, data_type)                        \
	{                                                       \
	    mlib_s32 x, y;                                      \
	    mlib_s32 _w = (w), _h = (h);                        \
	    data_type *_ps = (ps);                              \
	    data_type *_pd = (pd);                              \
	    data_type *spix = _ps;                              \
	    data_type *dpix = _pd;                              \
	                                                        \
	    for (x = 1; x <= _w; ++x) {                         \
		for (y = 0; y < _h; ++y) {                      \
		    dpix[0] = spix[0];                          \
		    spix += sstride;                            \
		    dpix += 1;                                  \
		}                                               \
		spix = _ps + x;                                 \
		dpix = _pd + x * dstride;                       \
	    }                                                   \
	}

/* *********************************************************** */

#define	STRIP_2(pd, ps, w, h, data_type)                        \
	{                                                       \
	    mlib_s32 x, y;                                      \
	    mlib_s32 _w = (w), _h = (h);                        \
	    data_type *_ps = (ps);                              \
	    data_type *_pd = (pd);                              \
	    data_type *spix = _ps;                              \
	    data_type *dpix = _pd;                              \
	                                                        \
	    for (x = 1; x <= _w; ++x) {                         \
		for (y = 0; y < _h; ++y) {                      \
		    data_type c1, c2;                           \
	                                                        \
		    c1 = spix[0];                               \
		    c2 = spix[1];                               \
		    dpix[0] = c1;                               \
		    dpix[1] = c2;                               \
		    spix += sstride;                            \
		    dpix += 2;                                  \
		}                                               \
		spix = _ps + (x << 1);                          \
		dpix = _pd + x * dstride;                       \
	    }                                                   \
	}

/* *********************************************************** */

#define	STRIP_4(pd, ps, w, h, data_type)                        \
	{                                                       \
	    mlib_s32 x, y;                                      \
	    mlib_s32 _w = (w), _h = (h);                        \
	    data_type *_ps = (ps);                              \
	    data_type *_pd = (pd);                              \
	    data_type *spix = _ps;                              \
	    data_type *dpix = _pd;                              \
	                                                        \
	    for (x = 1; x <= _w; ++x) {                         \
		for (y = 0; y < _h; ++y) {                      \
		    data_type c1, c2, c3, c4;                   \
	                                                        \
		    c1 = spix[0];                               \
		    c2 = spix[1];                               \
		    c3 = spix[2];                               \
		    c4 = spix[3];                               \
		    dpix[0] = c1;                               \
		    dpix[1] = c2;                               \
		    dpix[2] = c3;                               \
		    dpix[3] = c4;                               \
		    spix += sstride;                            \
		    dpix += 4;                                  \
		}                                               \
		spix = _ps + (x << 2);                          \
		dpix = _pd + x * dstride;                       \
	    }                                                   \
	}

/* *********************************************************** */

#define	BRAK_OPEN(xpix, ypix, data_type, num_ch)                \
	s_tail = (sw - s_head) & (xpix - 1);                    \
	d_tail = (dw - d_head) & (ypix - 1);                    \
	numhorzsq = (mlib_s32)(sw - s_head - s_tail) / xpix;    \
	numvertsq = (mlib_s32)(dw - d_head - d_tail) / ypix;    \
	sp = sl = sdata + s_head * num_ch + d_head * sstride;   \
	dp = dl = ddata + d_head * num_ch + s_head * dstride

/* *********************************************************** */

#define	BRAK_CLOSE(xpix, ypix, data_type, num_ch)                        \
	if (s_head)                                                      \
	    STRIP_##num_ch(ddata, sdata, s_head, dw, data_type);         \
	if (s_tail)                                                      \
	    STRIP_##num_ch(ddata + dstride * (dh - s_tail),              \
		sdata + num_ch * (sw - s_tail), s_tail, dw, data_type)

/* *********************************************************** */

#define	H_BORDERS_1(xpix)                                            \
	if (d_head) {                                                \
	    for (jj = 0; jj < d_head; jj++) {                        \
		for (ii = 0; ii < xpix; ii++) {                      \
		    dl[-jj - 1 + ii * dstride] =                     \
			sl[ii + (-jj - 1) * sstride];                \
		}                                                    \
	    }                                                        \
	}                                                            \
	if (d_tail) {                                                \
	    for (jj = 0; jj < d_tail; jj++) {                        \
		for (ii = 0; ii < xpix; ii++) {                      \
		    dp[jj + ii * dstride] = sp[ii + jj * sstride];   \
		}                                                    \
	    }                                                        \
	}

/* *********************************************************** */

#define	H_BORDERS_2(xpix)                                       \
	if (d_head) {                                           \
	    for (jj = 0; jj < d_head; jj++) {                   \
		for (ii = 0; ii < xpix; ii++) {                 \
		    dl[2 * (-jj - 1) + ii * dstride] =          \
			sl[2 * ii + (-jj - 1) * sstride];       \
		    dl[2 * (-jj - 1) + ii * dstride + 1] =      \
			sl[2 * ii + (-jj - 1) * sstride + 1];   \
		}                                               \
	    }                                                   \
	}                                                       \
	if (d_tail) {                                           \
	    for (jj = 0; jj < d_tail; jj++) {                   \
		for (ii = 0; ii < xpix; ii++) {                 \
		    dp[2 * jj + ii * dstride] =                 \
			sp[2 * ii + jj * sstride];              \
		    dp[2 * jj + ii * dstride + 1] =             \
			sp[2 * ii + jj * sstride + 1];          \
		}                                               \
	    }                                                   \
	}

/* *********************************************************** */

#define	H_BORDERS_4(xpix)                                       \
	if (d_head) {                                           \
	    for (jj = 0; jj < d_head; jj++) {                   \
		for (ii = 0; ii < xpix; ii++) {                 \
		    dl[4 * (-jj - 1) + ii * dstride] =          \
			sl[4 * ii + (-jj - 1) * sstride];       \
		    dl[4 * (-jj - 1) + ii * dstride + 1] =      \
			sl[4 * ii + (-jj - 1) * sstride + 1];   \
		    dl[4 * (-jj - 1) + ii * dstride + 2] =      \
			sl[4 * ii + (-jj - 1) * sstride + 2];   \
		    dl[4 * (-jj - 1) + ii * dstride + 3] =      \
			sl[4 * ii + (-jj - 1) * sstride + 3];   \
		}                                               \
	    }                                                   \
	}                                                       \
	if (d_tail) {                                           \
	    for (jj = 0; jj < d_tail; jj++) {                   \
		for (ii = 0; ii < xpix; ii++) {                 \
		    dp[4 * jj + ii * dstride] =                 \
			sp[4 * ii + jj * sstride];              \
		    dp[4 * jj + ii * dstride + 1] =             \
			sp[4 * ii + jj * sstride + 1];          \
		    dp[4 * jj + ii * dstride + 2] =             \
			sp[4 * ii + jj * sstride + 2];          \
		    dp[4 * jj + ii * dstride + 3] =             \
			sp[4 * ii + jj * sstride + 3];          \
		}                                               \
	    }                                                   \
	}

/* *********************************************************** */

#define	INCREMENT(xpix, ypix, num_ch)                           \
	H_BORDERS_##num_ch(xpix) sp = sl += xpix * num_ch;      \
	dp = dl += xpix * dstride

/* *********************************************************** */

#define	ALIGN_ADDR(sp)	(void*)((mlib_addr)(sp) &~ 7)

#define	ALIGN_DST(dp)	(void*)((mlib_u8*)(dp) - 8 +            \
				((8 - (mlib_addr)(dp)) & 7))

/* *********************************************************** */

#define	LOAD(N)	s##N = *(mlib_d64*)sp; sp += sstride

/* *********************************************************** */

#define	LOAD_NF(N)	s##N = vis_ld_d64_nf(sp); sp += sstride

/* *********************************************************** */

#define	STORE(N)	*(mlib_d64*)dp = d##N; dp += dstride

/* *********************************************************** */

#define	LOAD_GE(N)                                              \
	pp = ALIGN_ADDR(sp);                                    \
	vis_alignaddr(sp, 0);                                   \
	s##N = vis_faligndata(pp[0], pp[1]);                    \
	sp += sstride

/* *********************************************************** */

#define	LOAD_GE_NF(N)                                           \
	pp = ALIGN_ADDR(sp);                                    \
	vis_alignaddr(sp, 0);                                   \
	s##N = vis_faligndata(vis_ld_d64_nf(pp),                \
		vis_ld_d64_nf(pp + 1));                         \
	sp += sstride

/* *********************************************************** */

#if 1	/* n/a STORE: vis_faligndata() with previous results */

/* *********************************************************** */

#define	INIT_EDGE(N)	mlib_d64 r##N

/* *********************************************************** */

#define	STORE_FIRST(N)                                          \
	if ((mlib_addr)dp & 7) {                                \
	    pp = ALIGN_ADDR(dp);                                \
	    vis_alignaddr((void *)((mlib_addr)dp ^ 7), 1);      \
	    r##N = vis_faligndata(d##N, d##N);                  \
	    emask = vis_edge8(dp, (mlib_u8 *)pp + 7);           \
	    vis_pst_8(r##N, pp, emask);                         \
	}                                                       \
	r##N = d##N;                                            \
	dp += dstride

/* *********************************************************** */

#define	STORE_GE(N)                                             \
	pp = ALIGN_DST(dp);                                     \
	vis_alignaddr((void *)((mlib_addr)dp ^ 7), 1);          \
	pp[0] = vis_faligndata(r##N, d##N);                     \
	r##N = d##N;                                            \
	dp += dstride

/* *********************************************************** */

#define	STORE_EDGE(N)                                           \
	pp = ALIGN_DST(dp);                                     \
	vis_alignaddr((void *)((mlib_addr)dp ^ 7), 1);          \
	r##N = vis_faligndata(r##N, r##N);                      \
	emask = vis_edge8(pp, (mlib_u8 *)dp - 1);               \
	vis_pst_8(r##N, pp, emask);                             \
	dp += dstride

#else /* n/a STORE: two vis_pst_8() instructions */
/* 1 ( n/a STORE: vis_faligndata() with previous results ) */

/* *********************************************************** */

#define	INIT_EDGE(N)                                                        \
	mlib_s32 mask##N                                                    \
/*                                                                          \
 * = vis_edge8(dp + N*dstride, (void *)((mlib_addr)(dp + N*dstride) | 7))   \
 */                                                                         \


/* *********************************************************** */

#define	STORE_GE(N)                                             \
	pp = ALIGN_ADDR(dp);                                    \
	vis_alignaddr((void *)((mlib_addr)dp ^ 7), 1);          \
	d##N = vis_faligndata(d##N, d##N);                      \
	mask##N = vis_edge8(dp, (mlib_u8 *)pp + 7);             \
	vis_pst_8(d##N, pp, mask##N);                           \
	vis_pst_8(d##N, pp + 1, ~mask##N);                      \
	dp += dstride

#define	STORE_FIRST(N)	STORE_GE(N)

#define	STORE_EDGE(N)

#endif /* 1 ( n/a STORE: vis_faligndata() with previous results ) */

/* *********************************************************** */

#define	INIT_EDGE_2	INIT_EDGE(0); INIT_EDGE(1)

/* *********************************************************** */

#define	INIT_EDGE_4	INIT_EDGE_2; INIT_EDGE(2); INIT_EDGE(3)

/* *********************************************************** */

#define	INIT_EDGE_8                                             \
	INIT_EDGE_4;                                            \
	INIT_EDGE(4);                                           \
	INIT_EDGE(5);                                           \
	INIT_EDGE(6);                                           \
	INIT_EDGE(7)

/* *********************************************************** */

#define	STORE_EDGE_2	STORE_EDGE(0); STORE_EDGE(1)

/* *********************************************************** */

#define	STORE_EDGE_4	STORE_EDGE_2; STORE_EDGE(2); STORE_EDGE(3)

/* *********************************************************** */

#define	STORE_EDGE_8                                            \
	STORE_EDGE_4;                                           \
	STORE_EDGE(4);                                          \
	STORE_EDGE(5);                                          \
	STORE_EDGE(6);                                          \
	STORE_EDGE(7)

/* *********************************************************** */

#define	TRANSPOSE_1U8_8x8(LOAD, STORE)                          \
	mlib_d64 s0, s1, s2, s3, s4, s5, s6, s7;                \
	mlib_d64 d0, d1, d2, d3, d4, d5, d6, d7;                \
	mlib_d64 a0, a1, a2, a3, b0, b1, b2, b3;                \
	                                                        \
	LOAD##_NF(0);                                           \
	LOAD(1);                                                \
	LOAD(2);                                                \
	LOAD(3);                                                \
	LOAD(4);                                                \
	LOAD(5);                                                \
	LOAD(6);                                                \
	LOAD##_NF(7);                                           \
	a0 = vis_fpmerge(vis_read_hi(s0), vis_read_hi(s4));     \
	a1 = vis_fpmerge(vis_read_hi(s2), vis_read_hi(s6));     \
	a2 = vis_fpmerge(vis_read_hi(s1), vis_read_hi(s5));     \
	a3 = vis_fpmerge(vis_read_hi(s3), vis_read_hi(s7));     \
	b0 = vis_fpmerge(vis_read_hi(a0), vis_read_hi(a1));     \
	b1 = vis_fpmerge(vis_read_lo(a0), vis_read_lo(a1));     \
	b2 = vis_fpmerge(vis_read_hi(a2), vis_read_hi(a3));     \
	b3 = vis_fpmerge(vis_read_lo(a2), vis_read_lo(a3));     \
	d0 = vis_fpmerge(vis_read_hi(b0), vis_read_hi(b2));     \
	d1 = vis_fpmerge(vis_read_lo(b0), vis_read_lo(b2));     \
	d2 = vis_fpmerge(vis_read_hi(b1), vis_read_hi(b3));     \
	d3 = vis_fpmerge(vis_read_lo(b1), vis_read_lo(b3));     \
	STORE(0);                                               \
	STORE(1);                                               \
	STORE(2);                                               \
	STORE(3);                                               \
	a0 = vis_fpmerge(vis_read_lo(s0), vis_read_lo(s4));     \
	a1 = vis_fpmerge(vis_read_lo(s2), vis_read_lo(s6));     \
	a2 = vis_fpmerge(vis_read_lo(s1), vis_read_lo(s5));     \
	a3 = vis_fpmerge(vis_read_lo(s3), vis_read_lo(s7));     \
	b0 = vis_fpmerge(vis_read_hi(a0), vis_read_hi(a1));     \
	b1 = vis_fpmerge(vis_read_lo(a0), vis_read_lo(a1));     \
	b2 = vis_fpmerge(vis_read_hi(a2), vis_read_hi(a3));     \
	b3 = vis_fpmerge(vis_read_lo(a2), vis_read_lo(a3));     \
	d4 = vis_fpmerge(vis_read_hi(b0), vis_read_hi(b2));     \
	d5 = vis_fpmerge(vis_read_lo(b0), vis_read_lo(b2));     \
	d6 = vis_fpmerge(vis_read_hi(b1), vis_read_hi(b3));     \
	d7 = vis_fpmerge(vis_read_lo(b1), vis_read_lo(b3));     \
	STORE(4);                                               \
	STORE(5);                                               \
	STORE(6);                                               \
	STORE(7);                                               \
	dp -= 8 * dstride

/* *********************************************************** */

#define	TRANSPOSE_1S16_4x4(LOAD, STORE)                         \
	mlib_d64 s0, s1, s2, s3, d0, d1, d2, d3;                \
	mlib_d64 a0, a1, a2, a3, b0, b1, b2, b3;                \
	                                                        \
	LOAD##_NF(0);                                           \
	LOAD(1);                                                \
	LOAD(2);                                                \
	LOAD##_NF(3);                                           \
	a0 = vis_fpmerge(vis_read_hi(s0), vis_read_hi(s2));     \
	a1 = vis_fpmerge(vis_read_lo(s0), vis_read_lo(s2));     \
	a2 = vis_fpmerge(vis_read_hi(s1), vis_read_hi(s3));     \
	a3 = vis_fpmerge(vis_read_lo(s1), vis_read_lo(s3));     \
	b0 = vis_fpmerge(vis_read_hi(a0), vis_read_hi(a2));     \
	b1 = vis_fpmerge(vis_read_lo(a0), vis_read_lo(a2));     \
	b2 = vis_fpmerge(vis_read_hi(a1), vis_read_hi(a3));     \
	b3 = vis_fpmerge(vis_read_lo(a1), vis_read_lo(a3));     \
	d0 = vis_fpmerge(vis_read_hi(b0), vis_read_lo(b0));     \
	d1 = vis_fpmerge(vis_read_hi(b1), vis_read_lo(b1));     \
	d2 = vis_fpmerge(vis_read_hi(b2), vis_read_lo(b2));     \
	d3 = vis_fpmerge(vis_read_hi(b3), vis_read_lo(b3));     \
	STORE(0);                                               \
	STORE(1);                                               \
	STORE(2);                                               \
	STORE(3);                                               \
	dp -= 4 * dstride

/* *********************************************************** */

#define	TRANSPOSE_1S32_2x2(LOAD, STORE)                         \
	mlib_d64 s0, s1, d0, d1;                                \
	                                                        \
	LOAD##_NF(0);                                           \
	LOAD##_NF(1);                                           \
	d0 = vis_freg_pair(vis_read_hi(s0), vis_read_hi(s1));   \
	d1 = vis_freg_pair(vis_read_lo(s0), vis_read_lo(s1));   \
	STORE(0);                                               \
	STORE(1);                                               \
	dp -= 2 * dstride

/* *********************************************************** */

#define	TRANSPOSE_1S32_2x4(LOAD, STORE)                         \
	mlib_d64 s0, s1, s2, s3;                                \
	                                                        \
	LOAD(0);                                                \
	LOAD(1);                                                \
	LOAD(2);                                                \
	LOAD(3);                                                \
	((mlib_d64 *)dp)[0] =                                   \
	vis_freg_pair(vis_read_hi(s0), vis_read_hi(s1));        \
	((mlib_d64 *)dp)[1] =                                   \
	vis_freg_pair(vis_read_hi(s2), vis_read_hi(s3));        \
	dp += dstride;                                          \
	((mlib_d64 *)dp)[0] =                                   \
	vis_freg_pair(vis_read_lo(s0), vis_read_lo(s1));        \
	((mlib_d64 *)dp)[1] =                                   \
	vis_freg_pair(vis_read_lo(s2), vis_read_lo(s3));        \
	dp -= dstride

/* *********************************************************** */

#define	TRANSPOSE_2U8_4x4	TRANSPOSE_1S16_4x4

/* *********************************************************** */

#define	TRANSPOSE_1U8_4x8(LOAD, STORE)                          \
	mlib_f32 s0, s1, s2, s3, s4, s5, s6, s7;                \
	mlib_d64 d0, d1, d2, d3;                                \
	mlib_d64 a0, a1, a2, a3, b0, b1, b2, b3;                \
	                                                        \
	s0 = *(mlib_f32 *)sp;                                   \
	sp += sstride;                                          \
	s1 = *(mlib_f32 *)sp;                                   \
	sp += sstride;                                          \
	s2 = *(mlib_f32 *)sp;                                   \
	sp += sstride;                                          \
	s3 = *(mlib_f32 *)sp;                                   \
	sp += sstride;                                          \
	s4 = *(mlib_f32 *)sp;                                   \
	sp += sstride;                                          \
	s5 = *(mlib_f32 *)sp;                                   \
	sp += sstride;                                          \
	s6 = *(mlib_f32 *)sp;                                   \
	sp += sstride;                                          \
	s7 = *(mlib_f32 *)sp;                                   \
	sp += sstride;                                          \
	a0 = vis_fpmerge(s0, s4);                               \
	a1 = vis_fpmerge(s2, s6);                               \
	a2 = vis_fpmerge(s1, s5);                               \
	a3 = vis_fpmerge(s3, s7);                               \
	b0 = vis_fpmerge(vis_read_hi(a0), vis_read_hi(a1));     \
	b1 = vis_fpmerge(vis_read_lo(a0), vis_read_lo(a1));     \
	b2 = vis_fpmerge(vis_read_hi(a2), vis_read_hi(a3));     \
	b3 = vis_fpmerge(vis_read_lo(a2), vis_read_lo(a3));     \
	d0 = vis_fpmerge(vis_read_hi(b0), vis_read_hi(b2));     \
	d1 = vis_fpmerge(vis_read_lo(b0), vis_read_lo(b2));     \
	d2 = vis_fpmerge(vis_read_hi(b1), vis_read_hi(b3));     \
	d3 = vis_fpmerge(vis_read_lo(b1), vis_read_lo(b3));     \
	*(mlib_d64 *)(dp) = d0;                                 \
	*(mlib_d64 *)(dp + dstride) = d1;                       \
	*(mlib_d64 *)(dp + 2 * dstride) = d2;                   \
	*(mlib_d64 *)(dp + 3 * dstride) = d3

/* *********************************************************** */

#define	TRANSPOSE_1S32_2x4_ge()                                 \
	mlib_d64 s0, s1, s2, s3;                                \
	mlib_d64 *pp;                                           \
	                                                        \
	LOAD_GE(0);                                             \
	LOAD_GE(1);                                             \
	LOAD_GE(2);                                             \
	LOAD_GE(3);                                             \
	((mlib_f32 *)dp)[0] = vis_read_hi(s0);                  \
	((mlib_f32 *)dp)[1] = vis_read_hi(s1);                  \
	((mlib_f32 *)dp)[2] = vis_read_hi(s2);                  \
	((mlib_f32 *)dp)[3] = vis_read_hi(s3);                  \
	dp += dstride;                                          \
	((mlib_f32 *)dp)[0] = vis_read_lo(s0);                  \
	((mlib_f32 *)dp)[1] = vis_read_lo(s1);                  \
	((mlib_f32 *)dp)[2] = vis_read_lo(s2);                  \
	((mlib_f32 *)dp)[3] = vis_read_lo(s3);                  \
	dp -= dstride

/* *********************************************************** */

#define	TRANSPOSE_1D64_8x1_ge()                                 \
	mlib_d64 d0, d1, d2, d3, d4, d5, d6, d7;                \
	                                                        \
	d0 = sp[0];                                             \
	d1 = sp[1];                                             \
	d2 = sp[2];                                             \
	d3 = sp[3];                                             \
	d4 = sp[4];                                             \
	d5 = sp[5];                                             \
	d6 = sp[6];                                             \
	d7 = sp[7];                                             \
	sp += sstride;                                          \
	dp[0] = d0;                                             \
	dp[dstride] = d1;                                       \
	dp[2 * dstride] = d2;                                   \
	dp[3 * dstride] = d3;                                   \
	dp[4 * dstride] = d4;                                   \
	dp[5 * dstride] = d5;                                   \
	dp[6 * dstride] = d6;                                   \
	dp[7 * dstride] = d7

/* *********************************************************** */

#define	TRANSPOSE_1D64_1x4_ge()                                 \
	mlib_d64 s0, s1, s2, s3;                                \
	                                                        \
	LOAD(0);                                                \
	LOAD(1);                                                \
	LOAD(2);                                                \
	LOAD(3);                                                \
	dp[0] = s0;                                             \
	dp[1] = s1;                                             \
	dp[2] = s2;                                             \
	dp[3] = s3

/* *********************************************************** */

#define	TRANSPOSE_2S32_8x1_ge()                                 \
	mlib_d64 d0, d1, d2, d3, d4, d5, d6, d7, d8;            \
	mlib_d64 *pp;                                           \
	mlib_f32 *fp = (mlib_f32 *)dp;                          \
	                                                        \
	pp = ALIGN_ADDR(sp);                                    \
	vis_alignaddr(sp, 0);                                   \
	sp += sstride;                                          \
	d0 = pp[0];                                             \
	d1 = pp[1];                                             \
	d2 = pp[2];                                             \
	d3 = pp[3];                                             \
	d4 = pp[4];                                             \
	d5 = pp[5];                                             \
	d6 = pp[6];                                             \
	d7 = pp[7];                                             \
	d8 = pp[8];                                             \
	d0 = vis_faligndata(d0, d1);                            \
	d1 = vis_faligndata(d1, d2);                            \
	d2 = vis_faligndata(d2, d3);                            \
	d3 = vis_faligndata(d3, d4);                            \
	d4 = vis_faligndata(d4, d5);                            \
	d5 = vis_faligndata(d5, d6);                            \
	d6 = vis_faligndata(d6, d7);                            \
	d7 = vis_faligndata(d7, d8);                            \
	fp[0] = vis_read_hi(d0);                                \
	fp[1] = vis_read_lo(d0);                                \
	fp[dstride] = vis_read_hi(d1);                          \
	fp[dstride + 1] = vis_read_lo(d1);                      \
	fp[2 * dstride] = vis_read_hi(d2);                      \
	fp[2 * dstride + 1] = vis_read_lo(d2);                  \
	fp[3 * dstride] = vis_read_hi(d3);                      \
	fp[3 * dstride + 1] = vis_read_lo(d3);                  \
	fp[4 * dstride] = vis_read_hi(d4);                      \
	fp[4 * dstride + 1] = vis_read_lo(d4);                  \
	fp[5 * dstride] = vis_read_hi(d5);                      \
	fp[5 * dstride + 1] = vis_read_lo(d5);                  \
	fp[6 * dstride] = vis_read_hi(d6);                      \
	fp[6 * dstride + 1] = vis_read_lo(d6);                  \
	fp[7 * dstride] = vis_read_hi(d7);                      \
	fp[7 * dstride + 1] = vis_read_lo(d7)

/* *********************************************************** */

#define	TRANSPOSE_1S32_8x2_ge()                                 \
	mlib_d64 d0, d1, d2, d3, d4, d5, d6, d7, d8;            \
	mlib_d64 *pp, dd;                                       \
	mlib_f32 *fp = (mlib_f32 *)dp;                          \
	                                                        \
	pp = ALIGN_ADDR(sp);                                    \
	vis_alignaddr(sp, 0);                                   \
	sp += sstride;                                          \
	d0 = pp[0];                                             \
	d1 = pp[1];                                             \
	d2 = pp[2];                                             \
	d3 = pp[3];                                             \
	dd = vis_ld_d64_nf(pp + 4);                             \
	d0 = vis_faligndata(d0, d1);                            \
	d1 = vis_faligndata(d1, d2);                            \
	d2 = vis_faligndata(d2, d3);                            \
	d3 = vis_faligndata(d3, dd);                            \
	pp = ALIGN_ADDR(sp);                                    \
	vis_alignaddr(sp, 0);                                   \
	sp += sstride;                                          \
	d4 = pp[0];                                             \
	d5 = pp[1];                                             \
	d6 = pp[2];                                             \
	d7 = pp[3];                                             \
	dd = vis_ld_d64_nf(pp + 4);                             \
	d4 = vis_faligndata(d4, d5);                            \
	d5 = vis_faligndata(d5, d6);                            \
	d6 = vis_faligndata(d6, d7);                            \
	d7 = vis_faligndata(d7, dd);                            \
	fp[0] = vis_read_hi(d0);                                \
	fp[1] = vis_read_hi(d4);                                \
	fp[dstride] = vis_read_lo(d0);                          \
	fp[dstride + 1] = vis_read_lo(d4);                      \
	fp[2 * dstride] = vis_read_hi(d1);                      \
	fp[2 * dstride + 1] = vis_read_hi(d5);                  \
	fp[3 * dstride] = vis_read_lo(d1);                      \
	fp[3 * dstride + 1] = vis_read_lo(d5);                  \
	fp[4 * dstride] = vis_read_hi(d2);                      \
	fp[4 * dstride + 1] = vis_read_hi(d6);                  \
	fp[5 * dstride] = vis_read_lo(d2);                      \
	fp[5 * dstride + 1] = vis_read_lo(d6);                  \
	fp[6 * dstride] = vis_read_hi(d3);                      \
	fp[6 * dstride + 1] = vis_read_hi(d7);                  \
	fp[7 * dstride] = vis_read_lo(d3);                      \
	fp[7 * dstride + 1] = vis_read_lo(d7)

/* *********************************************************** */

#define	FUNC_TRANSPOSE_AL(CH, TT, XN, YN)                             \
	mlib_status FNAME_CH1_GE(TT) (PARAMS(mlib_##TT));             \
	mlib_status FNAME_CH1_AL(TT) (PARAMS(mlib_##TT))              \
	{                                                             \
	    PREPAREVARS(mlib_##TT, CH);                               \
	    if (sw < 8 || sh < 8) {                                   \
		if (sw <= 0 || sh <= 0)                               \
		    return (MLIB_FAILURE);                            \
		STRIP_##CH(ddata, sdata, sw, sh, mlib_##TT);          \
		return (MLIB_SUCCESS);                                \
	    }                                                         \
	    if (((sstride | dstride) * sizeof (mlib_##TT)) & 7) {     \
		return (FNAME_CH1_GE(TT) (CALL_PARAMS));              \
	    }                                                         \
	    s_head =                                                  \
		(XN -                                                 \
		((mlib_s32)sdata / sizeof (mlib_##TT))) & (XN - 1);   \
	    d_head =                                                  \
		(YN -                                                 \
		((mlib_s32)ddata / sizeof (mlib_##TT))) & (YN - 1);   \
	    BRAK_OPEN(XN, YN, mlib_##TT, CH);                         \
	    for (xsq = 0; xsq < numhorzsq; xsq++) {                   \
		for (ysq = 0; ysq < numvertsq; ysq++) {               \
		    TRANSPOSE_##CH##TT##_##XN##x##YN                  \
			(LOAD, STORE);                                \
		    dp += CH * YN;                                    \
		}                                                     \
		INCREMENT(XN, YN, CH);                                \
	    }                                                         \
	    BRAK_CLOSE(XN, YN, mlib_##TT, CH);                        \
	    return (MLIB_SUCCESS);                                    \
	}

/* *********************************************************** */

#define	FUNC_TRANSPOSE_GE(CH, TT, XN, YN)                       \
	mlib_status                                             \
	FNAME_CH##CH##_GE(TT) (PARAMS(mlib_##TT))               \
	{                                                       \
	    mlib_d64 *pp;                                       \
	    mlib_s32 emask;                                     \
	                                                        \
	    PREPAREVARS(mlib_##TT, CH);                         \
	    CHECK_CH##CH(TT);                                   \
	    s_head = 0;                                         \
	    d_head = 0;                                         \
	    BRAK_OPEN(XN, YN, mlib_##TT, CH);                   \
	    if (!numvertsq)                                     \
		numhorzsq = 0;                                  \
	    for (xsq = 0; xsq < numhorzsq; xsq++) {             \
		INIT_EDGE_##XN;                                 \
		TRANSPOSE_##CH##TT##_##XN##x##YN                \
		    (LOAD_GE, STORE_FIRST);                     \
		dp += CH * YN;                                  \
		for (ysq = 1; ysq < numvertsq; ysq++) {         \
		    TRANSPOSE_##CH##TT##_##XN##x##YN            \
			(LOAD_GE, STORE_GE);                    \
		    dp += CH * YN;                              \
		}                                               \
		STORE_EDGE_##XN;                                \
		dp -= XN * dstride;                             \
		INCREMENT(XN, YN, CH);                          \
	    }                                                   \
	    BRAK_CLOSE(XN, YN, mlib_##TT, CH);                  \
	    return (MLIB_SUCCESS);                              \
	}

/* *********************************************************** */

#define	FUNC_TRANSPOSE_G2(CH, TT, XN, YN)                       \
	mlib_status                                             \
	FNAME_CH##CH##_GE(TT) (PARAMS(mlib_##TT))               \
	{                                                       \
	    PREPAREVARS(mlib_##TT, CH);                         \
	    CHECK_CH##CH(TT);                                   \
	    s_head = 0;                                         \
	    d_head = 0;                                         \
	    BRAK_OPEN(XN, YN, mlib_##TT, CH);                   \
	    for (xsq = 0; xsq < numhorzsq; xsq++) {             \
		for (ysq = 0; ysq < numvertsq; ysq++) {         \
		    TRANSPOSE_##CH##TT##_##XN##x##YN##_ge       \
			();                                     \
		    dp += CH * YN;                              \
		}                                               \
		INCREMENT(XN, YN, CH);                          \
	    }                                                   \
	    BRAK_CLOSE(XN, YN, mlib_##TT, CH);                  \
	    return (MLIB_SUCCESS);                              \
	}

/* *********************************************************** */

#if MLIB_VIS < 0x200
    FUNC_TRANSPOSE_AL(1, U8, 4, 8)
#else /* MLIB_VIS < 0x200 */
    FUNC_TRANSPOSE_AL(1, U8, 8, 8)
#endif /* MLIB_VIS < 0x200 */
    FUNC_TRANSPOSE_GE(1, U8, 8, 8)

    FUNC_TRANSPOSE_AL(1, S16, 4, 4)

    FUNC_TRANSPOSE_AL(1, S32, 2, 4)
#if MLIB_VIS < 0x200
    FUNC_TRANSPOSE_G2(1, S32, 8, 2)
#else /* MLIB_VIS < 0x200 */
    FUNC_TRANSPOSE_G2(1, S32, 2, 4)
#endif /* MLIB_VIS < 0x200 */
    FUNC_TRANSPOSE_G2(1, D64, 1, 4)

/* *********************************************************** */

#if 1

#define	TRANSPOSE_4U8_2x2	TRANSPOSE_1S32_2x2

    FUNC_TRANSPOSE_GE(4, U8, 2, 2)

#endif /* 1 */

/* *********************************************************** */

    FUNC_TRANSPOSE_GE(2, U8, 4, 4)

/* *********************************************************** */
/* n/a STORING by two double's */
/* *********************************************************** */

#undef  STORE_FIRST
#define	STORE_FIRST(N)                                          \
	pp = ALIGN_ADDR(dp);                                    \
	if ((mlib_addr)dp & 7) {                                \
	    vis_alignaddr((void *)((mlib_addr)dp ^ 7), 1);      \
	    emask = vis_edge8(dp, (mlib_u8 *)pp + 7);           \
	    vis_pst_8(vis_faligndata(d##N, d##N), pp, emask);   \
	    pp[1] = vis_faligndata(d##N, e##N);                 \
	} else {                                                \
	    pp[0] = d##N;                                       \
	}                                                       \
	                                                        \
	r##N = e##N;                                            \
	dp += dstride

/* *********************************************************** */

#undef  STORE_GE
#define	STORE_GE(N)                                             \
	pp = ALIGN_DST(dp);                                     \
	vis_alignaddr((void *)((mlib_addr)dp ^ 7), 1);          \
	pp[0] = vis_faligndata(r##N, d##N);                     \
	pp[1] = vis_faligndata(d##N, e##N);                     \
	r##N = e##N;                                            \
	dp += dstride

/* *********************************************************** */

#define	TRANSPOSE_2S16_2x4(LOAD, STORE)                         \
	mlib_d64 s0, s1, s2, s3, d0, e0, d1, e1;                \
	                                                        \
	LOAD##_NF(0);                                           \
	LOAD(1);                                                \
	LOAD(2);                                                \
	LOAD##_NF(3);                                           \
	d0 = vis_freg_pair(vis_read_hi(s0), vis_read_hi(s1));   \
	e0 = vis_freg_pair(vis_read_hi(s2), vis_read_hi(s3));   \
	d1 = vis_freg_pair(vis_read_lo(s0), vis_read_lo(s1));   \
	e1 = vis_freg_pair(vis_read_lo(s2), vis_read_lo(s3));   \
	STORE(0);                                               \
	STORE(1);                                               \
	dp -= 2 * dstride

/* *********************************************************** */

#define	TRANSPOSE_1S16_4x8(LOAD, STORE)                         \
	mlib_d64 s0, s1, s2, s3, s4, s5, s6, s7;                \
	mlib_d64 d0, d1, d2, d3, e0, e1, e2, e3;                \
	mlib_d64 a0, a1, a2, a3, b0, b1, b2, b3;                \
	                                                        \
	LOAD##_NF(0);                                           \
	LOAD(1);                                                \
	LOAD(2);                                                \
	LOAD(3);                                                \
	LOAD(4);                                                \
	LOAD(5);                                                \
	LOAD(6);                                                \
	LOAD##_NF(7);                                           \
	a0 = vis_fpmerge(vis_read_hi(s0), vis_read_hi(s2));     \
	a1 = vis_fpmerge(vis_read_lo(s0), vis_read_lo(s2));     \
	a2 = vis_fpmerge(vis_read_hi(s1), vis_read_hi(s3));     \
	a3 = vis_fpmerge(vis_read_lo(s1), vis_read_lo(s3));     \
	b0 = vis_fpmerge(vis_read_hi(a0), vis_read_hi(a2));     \
	b1 = vis_fpmerge(vis_read_lo(a0), vis_read_lo(a2));     \
	b2 = vis_fpmerge(vis_read_hi(a1), vis_read_hi(a3));     \
	b3 = vis_fpmerge(vis_read_lo(a1), vis_read_lo(a3));     \
	d0 = vis_fpmerge(vis_read_hi(b0), vis_read_lo(b0));     \
	d1 = vis_fpmerge(vis_read_hi(b1), vis_read_lo(b1));     \
	d2 = vis_fpmerge(vis_read_hi(b2), vis_read_lo(b2));     \
	d3 = vis_fpmerge(vis_read_hi(b3), vis_read_lo(b3));     \
	a0 = vis_fpmerge(vis_read_hi(s4), vis_read_hi(s6));     \
	a1 = vis_fpmerge(vis_read_lo(s4), vis_read_lo(s6));     \
	a2 = vis_fpmerge(vis_read_hi(s5), vis_read_hi(s7));     \
	a3 = vis_fpmerge(vis_read_lo(s5), vis_read_lo(s7));     \
	b0 = vis_fpmerge(vis_read_hi(a0), vis_read_hi(a2));     \
	b1 = vis_fpmerge(vis_read_lo(a0), vis_read_lo(a2));     \
	b2 = vis_fpmerge(vis_read_hi(a1), vis_read_hi(a3));     \
	b3 = vis_fpmerge(vis_read_lo(a1), vis_read_lo(a3));     \
	e0 = vis_fpmerge(vis_read_hi(b0), vis_read_lo(b0));     \
	e1 = vis_fpmerge(vis_read_hi(b1), vis_read_lo(b1));     \
	e2 = vis_fpmerge(vis_read_hi(b2), vis_read_lo(b2));     \
	e3 = vis_fpmerge(vis_read_hi(b3), vis_read_lo(b3));     \
	STORE(0);                                               \
	STORE(1);                                               \
	STORE(2);                                               \
	STORE(3);                                               \
	dp -= 4 * dstride

/* *********************************************************** */

    FUNC_TRANSPOSE_GE(2, S16, 2, 4)

    FUNC_TRANSPOSE_GE(1, S16, 4, 8)

/* *********************************************************** */

    FUNC_TRANSPOSE_G2(2, S32, 8, 1)

/* *********************************************************** */
