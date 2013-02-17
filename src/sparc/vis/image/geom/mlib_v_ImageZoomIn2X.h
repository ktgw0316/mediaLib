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

#ifndef _MLIB_V_IMAGEZOOMIN2X_H
#define	_MLIB_V_IMAGEZOOMIN2X_H

#pragma ident	"@(#)mlib_v_ImageZoomIn2X.h	9.2	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImageCopy.h>
#include <mlib_v_ImageZoomIn2X_proto.h>

/* *********************************************************** */

#define	COPY(src, dst, n)	mlib_ImageCopy_na((mlib_u8*)(src), \
					(mlib_u8*)(dst), n)
#define	mlib_ImageMerge3ch_U16	mlib_ImageMerge3ch_S16

/* *********************************************************** */

#define	GSR_SCALE_U8	1
#define	GSR_SCALE_S16	15
#define	GSR_SCALE_U16	15

/* *********************************************************** */

#define	READ_HI(x)	vis_read_hi(x)
#define	READ_LO(x)	vis_read_lo(x)

/* *********************************************************** */

#define	MERGE_U8_CH1(s0, s1)                                    \
	s0 = vis_fpmerge(vis_fpack16(s0), vis_fpack16(s1))

/* *********************************************************** */

#define	MERGE_U8_CH2(s0, s1)                                    \
	{                                                       \
	    mlib_d64 t0, t1;                                    \
	                                                        \
	    t0 = vis_faligndata(s0, s0);                        \
	    t1 = vis_faligndata(s1, s1);                        \
	    t0 = vis_faligndata(t0, s1);                        \
	    t1 = vis_faligndata(s0, t1);                        \
	    s0 = vis_fpack16_pair(t0, t1);                      \
	}

/* *********************************************************** */

#define	MERGE_U8_CH4(s0, s1)	s0 = vis_fpack16_pair(s0, s1)

/* *********************************************************** */

#define	SAVE_U8(CH, dp, r0, r1)	MERGE_U8_##CH(r0, r1); (*dp++) = r0

/* *********************************************************** */

#define	SAVE2_U8(CH, dp0, dp1, r0, r1)                          \
	MERGE_U8_##CH(r0, r1);                                  \
	(*dp0++) = vis_read_hi(r0);                             \
	(*dp1++) = vis_read_lo(r0)

/* *********************************************************** */

#define	SAVE_MASK_U8(CH, dp, emask, r0, r1)                     \
	MERGE_U8_##CH(r0, r1);                                  \
	vis_pst_8(r0, dp, emask);                               \
	dp++

/* *********************************************************** */

#define	MERGE_S16_CH1(s0, s1)                                   \
	{                                                       \
	    mlib_d64 d0, d1, d2, d3, e0, e1, e2, e3;            \
	                                                        \
	    d0 = vis_fmuld8ulx16(scale, vis_read_hi(s0));       \
	    d1 = vis_fmuld8ulx16(scale, vis_read_hi(s1));       \
	    d2 = vis_fmuld8ulx16(scale, vis_read_lo(s0));       \
	    d3 = vis_fmuld8ulx16(scale, vis_read_lo(s1));       \
	    e0 = vis_freg_pair(READ_HI(d0), READ_HI(d1));       \
	    e1 = vis_freg_pair(READ_LO(d0), READ_LO(d1));       \
	    e2 = vis_freg_pair(READ_HI(d2), READ_HI(d3));       \
	    e3 = vis_freg_pair(READ_LO(d2), READ_LO(d3));       \
	    s0 = vis_fpackfix_pair(e0, e1);                     \
	    s1 = vis_fpackfix_pair(e2, e3);                     \
	}

/* *********************************************************** */

#define	MERGE_S16_CH2(s0, s1)                                   \
	{                                                       \
	    mlib_d64 d0, d1, d2, d3;                            \
	                                                        \
	    d0 = vis_fmuld8ulx16(scale, vis_read_hi(s0));       \
	    d1 = vis_fmuld8ulx16(scale, vis_read_hi(s1));       \
	    d2 = vis_fmuld8ulx16(scale, vis_read_lo(s0));       \
	    d3 = vis_fmuld8ulx16(scale, vis_read_lo(s1));       \
	    s0 = vis_fpackfix_pair(d0, d1);                     \
	    s1 = vis_fpackfix_pair(d2, d3);                     \
	}

/* *********************************************************** */

#define	MERGE_S16_CH4(s0, s1)                                   \
	{                                                       \
	    mlib_d64 d0, d1, d2, d3;                            \
	                                                        \
	    d0 = vis_fmuld8ulx16(scale, vis_read_hi(s0));       \
	    d1 = vis_fmuld8ulx16(scale, vis_read_lo(s0));       \
	    d2 = vis_fmuld8ulx16(scale, vis_read_hi(s1));       \
	    d3 = vis_fmuld8ulx16(scale, vis_read_lo(s1));       \
	    s0 = vis_fpackfix_pair(d0, d1);                     \
	    s1 = vis_fpackfix_pair(d2, d3);                     \
	}

/* *********************************************************** */

#define	SAVE_S16(CH, dp, r0, r1)                                \
	MERGE_S16_##CH(r0, r1);                                 \
	(*dp++) = r0;                                           \
	(*dp++) = r1

/* *********************************************************** */

#define	SAVE2_S16(CH, dp0, dp1, r0, r1)                         \
	MERGE_S16_##CH(r0, r1);                                 \
	(*dp0++) = r0;                                          \
	(*dp1++) = r1

/* *********************************************************** */

#define	SAVE_MASK_S16(CH, dp, emask, r0, r1)                    \
	MERGE_S16_##CH(r0, r1);                                 \
	if (emask & 0xF0)                                       \
	    vis_pst_16(r0, dp, emask >> 4);                     \
	if (emask & 0x0F)                                       \
	    vis_pst_16(r1, dp + 1, emask);                      \
	dp += 2

/* *********************************************************** */

#define	SAVE_U16(CH, dp, r0, r1)                                \
	MERGE_S16_##CH(r0, r1);                                 \
	(*dp++) = vis_fxor(r0, mask8000);                       \
	(*dp++) = vis_fxor(r1, mask8000)

/* *********************************************************** */

#define	SAVE2_U16(CH, dp0, dp1, r0, r1)                         \
	MERGE_S16_##CH(r0, r1);                                 \
	(*dp0++) = vis_fxor(r0, mask8000);                      \
	(*dp1++) = vis_fxor(r1, mask8000)

/* *********************************************************** */

#define	SAVE_MASK_U16(CH, dp, emask, r0, r1)                      \
	MERGE_S16_##CH(r0, r1);                                   \
	if (emask & 0xF0)                                         \
	    vis_pst_16(vis_fxor(r0, mask8000), dp, emask >> 4);   \
	if (emask & 0x0F)                                         \
	    vis_pst_16(vis_fxor(r1, mask8000), dp + 1, emask);    \
	dp += 2

/* *********************************************************** */

#define	ADD_SLINE(NUM)                                          \
	ind = (ind + 1) & (NUM - 1);                            \
	if ((mlib_addr)sl & (sizeof (FTYPE) - 1)) {             \
	    sp_arr[ind] = buff_arr[ind];                        \
	    COPY((void *)sl, (void *)sp_arr[ind], src_w);       \
	} else {                                                \
	    sp_arr[ind] = (FTYPE *) sl;                         \
	}                                                       \
	                                                        \
	sl += src_stride

/* *********************************************************** */

#define	PROT_ZOOM_FUNC(FLT, TT, CH, H, V)                                  \
	mlib_status                                                        \
	mlib_v_ImageZoomIn2X_##TT##_##CH##FLT##_FL##H##V                   \
	    (void *pdst, void *psrc, mlib_s32 src_width,                   \
	    mlib_s32 src_height, mlib_s32 dst_stride,                      \
	    mlib_s32 src_stride, mlib_s32 src_w_beg, mlib_s32 src_h_beg)

/* *********************************************************** */

PROT_ZOOM_FUNC(BL, U8, 1, 0, 0);
PROT_ZOOM_FUNC(BL, U8, 1, 0, 1);
PROT_ZOOM_FUNC(BL, U8, 1, 1, 0);
PROT_ZOOM_FUNC(BL, U8, 1, 1, 1);
PROT_ZOOM_FUNC(BL, U8, 2, 0, 0);
PROT_ZOOM_FUNC(BL, U8, 2, 0, 1);
PROT_ZOOM_FUNC(BL, U8, 2, 1, 0);
PROT_ZOOM_FUNC(BL, U8, 2, 1, 1);
PROT_ZOOM_FUNC(BL, U8, 3, 0, 0);
PROT_ZOOM_FUNC(BL, U8, 3, 0, 1);
PROT_ZOOM_FUNC(BL, U8, 3, 1, 0);
PROT_ZOOM_FUNC(BL, U8, 3, 1, 1);
PROT_ZOOM_FUNC(BL, U8, 4, 0, 0);
PROT_ZOOM_FUNC(BL, U8, 4, 0, 1);
PROT_ZOOM_FUNC(BL, U8, 4, 1, 0);
PROT_ZOOM_FUNC(BL, U8, 4, 1, 1);
PROT_ZOOM_FUNC(BL, S16, 1, 0, 0);
PROT_ZOOM_FUNC(BL, S16, 1, 0, 1);
PROT_ZOOM_FUNC(BL, S16, 1, 1, 0);
PROT_ZOOM_FUNC(BL, S16, 1, 1, 1);
PROT_ZOOM_FUNC(BL, S16, 2, 0, 0);
PROT_ZOOM_FUNC(BL, S16, 2, 0, 1);
PROT_ZOOM_FUNC(BL, S16, 2, 1, 0);
PROT_ZOOM_FUNC(BL, S16, 2, 1, 1);
PROT_ZOOM_FUNC(BL, S16, 3, 0, 0);
PROT_ZOOM_FUNC(BL, S16, 3, 0, 1);
PROT_ZOOM_FUNC(BL, S16, 3, 1, 0);
PROT_ZOOM_FUNC(BL, S16, 3, 1, 1);
PROT_ZOOM_FUNC(BL, S16, 4, 0, 0);
PROT_ZOOM_FUNC(BL, S16, 4, 0, 1);
PROT_ZOOM_FUNC(BL, S16, 4, 1, 0);
PROT_ZOOM_FUNC(BL, S16, 4, 1, 1);
PROT_ZOOM_FUNC(BL, U16, 1, 0, 0);
PROT_ZOOM_FUNC(BL, U16, 1, 0, 1);
PROT_ZOOM_FUNC(BL, U16, 1, 1, 0);
PROT_ZOOM_FUNC(BL, U16, 1, 1, 1);
PROT_ZOOM_FUNC(BL, U16, 2, 0, 0);
PROT_ZOOM_FUNC(BL, U16, 2, 0, 1);
PROT_ZOOM_FUNC(BL, U16, 2, 1, 0);
PROT_ZOOM_FUNC(BL, U16, 2, 1, 1);
PROT_ZOOM_FUNC(BL, U16, 3, 0, 0);
PROT_ZOOM_FUNC(BL, U16, 3, 0, 1);
PROT_ZOOM_FUNC(BL, U16, 3, 1, 0);
PROT_ZOOM_FUNC(BL, U16, 3, 1, 1);
PROT_ZOOM_FUNC(BL, U16, 4, 0, 0);
PROT_ZOOM_FUNC(BL, U16, 4, 0, 1);
PROT_ZOOM_FUNC(BL, U16, 4, 1, 0);
PROT_ZOOM_FUNC(BL, U16, 4, 1, 1);

/* *********************************************************** */

#define	FUNC_ZOOM_BL(TT, CH, H, V)                                       \
	PROT_ZOOM_FUNC(BL, TT, CH, H, V)                                 \
	{                                                                \
	    DEF_CONST_H##H();                                            \
	    DEF_CONST_V##V();                                            \
	    mlib_s32 channels = CH;                                      \
	    mlib_s32 i, j;                                               \
	    FTYPE *sp_arr[2], *sp0, *sp1;                                \
	    mlib_d64 *dp, *dp_buff;                                      \
	    void *pbuff, *buff_arr[8];                                   \
	    mlib_s32 ind = 0;                                            \
	    mlib_s32 src_w, dst_w, bline, num_b, d_off, bchan;           \
	    mlib_d64 v0, v1, r0, r1;                                     \
	    STYPE *sl = (STYPE *) psrc;                                  \
	    STYPE *dl = (STYPE *) pdst;                                  \
	                                                                 \
	    src_w_beg &= 1;                                              \
	    src_h_beg &= 1;                                              \
	    src_width -= src_w_beg;                                      \
	    d_off = sizeof (STYPE) * ((8 - channels * src_w_beg) & 7);   \
	    bchan = sizeof (STYPE) * channels;                           \
	    src_w = bchan * ((src_width / 2) + 6);                       \
	    dst_w = bchan * (src_w_beg + src_width);                     \
	    bline = (src_w + 7) & ~7;                                    \
	    num_b = (CH == 3) ? 4 : 2;                                   \
	    pbuff = __mlib_malloc(num_b * bline + d_off + dst_w + 32);   \
	    if (pbuff == NULL)                                           \
		return (MLIB_FAILURE);                                   \
	    for (i = 0; i <= num_b; i++) {                               \
		buff_arr[i] = (mlib_u8 *)pbuff + i * bline;              \
	    }                                                            \
	    dp_buff = (mlib_d64 *)buff_arr[num_b];                       \
	    if (src_w_beg) {                                             \
		dl = (void *)((mlib_u8 *)dl - d_off);                    \
		sl -= (4 - channels);                                    \
	    }                                                            \
	    ADD_SLINE(2);                                                \
	    if (src_h_beg) {                                             \
		ADD_SLINE(2);                                            \
	    }                                                            \
	    vis_write_gsr(GSR_SCALE_##TT << 3);                          \
	    for (j = 0; j < src_height; j++) {                           \
		if (((src_h_beg + j) & 1) == 0) {                        \
		    ADD_SLINE(2);                                        \
		    vis_alignaddr((void *)(2 * CH), 0);                  \
		    sp0 = sp_arr[(ind - 1) & 1];                         \
		    sp1 = sp_arr[ind];                                   \
		    ZOOM_LINE_BL_##CH(TT, H, A##V);                      \
		} else {                                                 \
		    vis_alignaddr((void *)(2 * CH), 0);                  \
		    sp0 = sp_arr[(ind - 1) & 1];                         \
		    sp1 = sp_arr[ind];                                   \
		    ZOOM_LINE_BL_##CH(TT, H, B##V);                      \
		}                                                        \
		dl += dst_stride;                                        \
	    }                                                            \
	    __mlib_free(pbuff);                                          \
	    return (MLIB_SUCCESS);                                       \
	}

/* *********************************************************** */

#define	ZOOM_LINE_BL_1(TT, H, V)                                          \
	{                                                                 \
	    mlib_d64 v2;                                                  \
	                                                                  \
	    dp = ((mlib_addr)dl & 7) ? dp_buff : (mlib_d64 *)dl;          \
	    v0 = SUMV_##V(sp0[0], sp1[0]);                                \
	    sp0++;                                                        \
	    sp1++;                                                        \
	    if (d_off) {                                                  \
		mlib_s32 emask = (1 << channels) - 1;                     \
	                                                                  \
		v2 = SUMV_##V(sp0[0], sp1[0]);                            \
		sp0++;                                                    \
		sp1++;                                                    \
		v1 = vis_faligndata(v0, v2);                              \
		r0 = vis_fzero();                                         \
		r1 = SUMH_B##H(v0, v1);                                   \
		SAVE_MASK_##TT(CH1, dp, emask, r0, r1);                   \
		v0 = v2;                                                  \
	    }                                                             \
	    for (i = 0; i < src_width / 8; i++) {                         \
		v2 = SUMV_##V(sp0[0], sp1[0]);                            \
		sp0++;                                                    \
		sp1++;                                                    \
		v1 = vis_faligndata(v0, v2);                              \
		r0 = SUMH_A##H(v0, v1);                                   \
		r1 = SUMH_B##H(v0, v1);                                   \
		SAVE_##TT(CH1, dp, r0, r1);                               \
		v0 = v2;                                                  \
	    }                                                             \
	    if (src_width & 7) {                                          \
		mlib_s32 emask = 0xFF00 >> (src_width & 7);               \
	                                                                  \
		v2 = SUMV_##V(sp0[0], sp1[0]);                            \
		v1 = vis_faligndata(v0, v2);                              \
		r0 = SUMH_A##H(v0, v1);                                   \
		r1 = SUMH_B##H(v0, v1);                                   \
		SAVE_MASK_##TT(CH1, dp, emask, r0, r1);                   \
	    }                                                             \
	    if ((mlib_addr)dl & 7) {                                      \
		COPY((mlib_u8 *)dp_buff + d_off, (mlib_u8 *)dl + d_off,   \
		    dst_w);                                               \
	    }                                                             \
	}

/* *********************************************************** */

#define	ZOOM_LINE_BL_2(TT, H, V)                                          \
	{                                                                 \
	    mlib_d64 v2;                                                  \
	                                                                  \
	    dp = ((mlib_addr)dl & 7) ? dp_buff : (mlib_d64 *)dl;          \
	    v0 = SUMV_##V(sp0[0], sp1[0]);                                \
	    sp0++;                                                        \
	    sp1++;                                                        \
	    if (d_off) {                                                  \
		mlib_s32 emask = (1 << channels) - 1;                     \
	                                                                  \
		v2 = SUMV_##V(sp0[0], sp1[0]);                            \
		sp0++;                                                    \
		sp1++;                                                    \
		v1 = vis_faligndata(v0, v2);                              \
		r0 = vis_fzero();                                         \
		r1 = SUMH_B##H(v0, v1);                                   \
		SAVE_MASK_##TT(CH2, dp, emask, r0, r1);                   \
		v0 = v2;                                                  \
	    }                                                             \
	    for (i = 0; i < src_width / 4; i++) {                         \
		v2 = SUMV_##V(sp0[0], sp1[0]);                            \
		sp0++;                                                    \
		sp1++;                                                    \
		v1 = vis_faligndata(v0, v2);                              \
		r0 = SUMH_A##H(v0, v1);                                   \
		r1 = SUMH_B##H(v0, v1);                                   \
		SAVE_##TT(CH2, dp, r0, r1);                               \
		v0 = v2;                                                  \
	    }                                                             \
	    if (src_width & 7) {                                          \
		mlib_s32 emask = 0xFF00 >> (2 * (src_width & 3));         \
	                                                                  \
		v2 = SUMV_##V(sp0[0], sp1[0]);                            \
		v1 = vis_faligndata(v0, v2);                              \
		r0 = SUMH_A##H(v0, v1);                                   \
		r1 = SUMH_B##H(v0, v1);                                   \
		SAVE_MASK_##TT(CH2, dp, emask, r0, r1);                   \
	    }                                                             \
	    if ((mlib_addr)dl & 7) {                                      \
		COPY((mlib_u8 *)dp_buff + d_off, (mlib_u8 *)dl + d_off,   \
		    dst_w);                                               \
	    }                                                             \
	}

/* *********************************************************** */

#define	ZOOM_LINE_BL_3(TT, H, V)                                         \
	{                                                                \
	    mlib_d64 v2;                                                 \
	                                                                 \
	    dp0 = (void *)buff_arr[2];                                   \
	    dp1 = (void *)buff_arr[3];                                   \
	    dp = dp_buff;                                                \
	    v0 = SUMV_##V(sp0[0], sp1[0]);                               \
	    sp0++;                                                       \
	    sp1++;                                                       \
	    if (d_off) {                                                 \
		v2 = SUMV_##V(sp0[0], sp1[0]);                           \
		sp0++;                                                   \
		sp1++;                                                   \
		v1 = vis_faligndata(v0, v2);                             \
		r1 = SUMH_B##H(v0, v1);                                  \
		SAVE_MASK_##TT(CH4, dp, 0x0F, r1, r1);                   \
		dl[5] = ((STYPE *) dp_buff)[5];                          \
		dl[6] = ((STYPE *) dp_buff)[6];                          \
		dl[7] = ((STYPE *) dp_buff)[7];                          \
		v0 = v2;                                                 \
	    }                                                            \
	    for (i = 0; i < (channels * src_width + 5 + 7) / 8; i++) {   \
		v2 = SUMV_##V(sp0[0], sp1[0]);                           \
		sp0++;                                                   \
		sp1++;                                                   \
		v1 = vis_faligndata(v0, v2);                             \
		r0 = SUMH_A##H(v0, v1);                                  \
		r1 = SUMH_B##H(v0, v1);                                  \
		SAVE2_##TT(CH4, dp0, dp1, r0, r1);                       \
		v0 = v2;                                                 \
	    }                                                            \
	    dp = (src_w_beg) ? (void *)(dl + 8) : (void *)dl;            \
	    if ((mlib_addr)dl & 7) {                                     \
		mlib_ImageMerge3ch_##TT(buff_arr[2], buff_arr[3],        \
		    dp_buff, src_width);                                 \
		COPY((mlib_u8 *)dp_buff, (mlib_u8 *)dp,                  \
		    bchan * src_width);                                  \
	    } else {                                                     \
		mlib_ImageMerge3ch_##TT(buff_arr[2], buff_arr[3],        \
		    dp, src_width);                                      \
	    }                                                            \
	}

/* *********************************************************** */

#define	ZOOM_LINE_BL_4_U8(TT, H, V)                                   \
	dp = ((mlib_addr)dl & 3) ? dp_buff : (mlib_d64 *)dl;          \
	v0 = SUMV_##V(sp0[0], sp1[0]);                                \
	sp0++;                                                        \
	sp1++;                                                        \
	if (d_off) {                                                  \
	    v1 = SUMV_##V(sp0[0], sp1[0]);                            \
	    sp0++;                                                    \
	    sp1++;                                                    \
	    r1 = SUMH_B##H(v0, v1);                                   \
	    ((mlib_f32 *)dp)[1] = vis_fpack16(r1);                    \
	    dp++;                                                     \
	    v0 = v1;                                                  \
	}                                                             \
	for (i = 0; i < (src_width / 2); i++) {                       \
	    v1 = SUMV_##V(sp0[0], sp1[0]);                            \
	    sp0++;                                                    \
	    sp1++;                                                    \
	    r0 = SUMH_A##H(v0, v1);                                   \
	    r1 = SUMH_B##H(v0, v1);                                   \
	    ((mlib_f32 *)dp)[0] = vis_fpack16(r0);                    \
	    ((mlib_f32 *)dp)[1] = vis_fpack16(r1);                    \
	    dp++;                                                     \
	    v0 = v1;                                                  \
	}                                                             \
	                                                              \
	if (src_width & 1) {                                          \
	    v1 = SUMV_##V(sp0[0], sp1[0]);                            \
	    r0 = SUMH_A##H(v0, v1);                                   \
	    ((mlib_f32 *)dp)[0] = vis_fpack16(r0);                    \
	}                                                             \
	if ((mlib_addr)dl & 3) {                                      \
	    COPY((mlib_u8 *)dp_buff + d_off, (mlib_u8 *)dl + d_off,   \
		dst_w);                                               \
	}

/* *********************************************************** */

#define	ZOOM_LINE_BL_4_S16(TT, H, V)                                  \
	dp = ((mlib_addr)dl & 7) ? dp_buff : (mlib_d64 *)dl;          \
	v0 = SUMV_##V(sp0[0], sp1[0]);                                \
	sp0++;                                                        \
	sp1++;                                                        \
	if (d_off) {                                                  \
	    v1 = SUMV_##V(sp0[0], sp1[0]);                            \
	    sp0++;                                                    \
	    sp1++;                                                    \
	    r1 = SUMH_B##H(v0, v1);                                   \
	    SAVE_MASK_##TT(CH4, dp, 0x0F, r1, r1);                    \
	    v0 = v1;                                                  \
	}                                                             \
	for (i = 0; i < (src_width / 2); i++) {                       \
	    v1 = SUMV_##V(sp0[0], sp1[0]);                            \
	    sp0++;                                                    \
	    sp1++;                                                    \
	    r0 = SUMH_A##H(v0, v1);                                   \
	    r1 = SUMH_B##H(v0, v1);                                   \
	    SAVE_##TT(CH4, dp, r0, r1);                               \
	    v0 = v1;                                                  \
	}                                                             \
	                                                              \
	if (src_width & 1) {                                          \
	    v1 = SUMV_##V(sp0[0], sp1[0]);                            \
	    r0 = SUMH_A##H(v0, v1);                                   \
	    SAVE_MASK_##TT(CH4, dp, 0xF0, r0, r0);                    \
	}                                                             \
	if ((mlib_addr)dl & 7) {                                      \
	    COPY((mlib_u8 *)dp_buff + d_off, (mlib_u8 *)dl + d_off,   \
		dst_w);                                               \
	}

/* *********************************************************** */

#define	ZOOM_LINE_BL_4_U16	ZOOM_LINE_BL_4_S16

#define	ZOOM_LINE_BL_4(TT, H, V)	ZOOM_LINE_BL_4_##TT(TT, H, V)

/* *********************************************************** */

PROT_ZOOM_FUNC(BC, U8, 1, 0, 0);
PROT_ZOOM_FUNC(BC, U8, 1, 0, 1);
PROT_ZOOM_FUNC(BC, U8, 1, 1, 0);
PROT_ZOOM_FUNC(BC, U8, 1, 1, 1);
PROT_ZOOM_FUNC(BC, U8, 2, 0, 0);
PROT_ZOOM_FUNC(BC, U8, 2, 0, 1);
PROT_ZOOM_FUNC(BC, U8, 2, 1, 0);
PROT_ZOOM_FUNC(BC, U8, 2, 1, 1);
PROT_ZOOM_FUNC(BC, U8, 3, 0, 0);
PROT_ZOOM_FUNC(BC, U8, 3, 0, 1);
PROT_ZOOM_FUNC(BC, U8, 3, 1, 0);
PROT_ZOOM_FUNC(BC, U8, 3, 1, 1);
PROT_ZOOM_FUNC(BC, U8, 4, 0, 0);
PROT_ZOOM_FUNC(BC, U8, 4, 0, 1);
PROT_ZOOM_FUNC(BC, U8, 4, 1, 0);
PROT_ZOOM_FUNC(BC, U8, 4, 1, 1);
PROT_ZOOM_FUNC(BC, S16, 1, 0, 0);
PROT_ZOOM_FUNC(BC, S16, 1, 0, 1);
PROT_ZOOM_FUNC(BC, S16, 1, 1, 0);
PROT_ZOOM_FUNC(BC, S16, 1, 1, 1);
PROT_ZOOM_FUNC(BC, S16, 2, 0, 0);
PROT_ZOOM_FUNC(BC, S16, 2, 0, 1);
PROT_ZOOM_FUNC(BC, S16, 2, 1, 0);
PROT_ZOOM_FUNC(BC, S16, 2, 1, 1);
PROT_ZOOM_FUNC(BC, S16, 3, 0, 0);
PROT_ZOOM_FUNC(BC, S16, 3, 0, 1);
PROT_ZOOM_FUNC(BC, S16, 3, 1, 0);
PROT_ZOOM_FUNC(BC, S16, 3, 1, 1);
PROT_ZOOM_FUNC(BC, S16, 4, 0, 0);
PROT_ZOOM_FUNC(BC, S16, 4, 0, 1);
PROT_ZOOM_FUNC(BC, S16, 4, 1, 0);
PROT_ZOOM_FUNC(BC, S16, 4, 1, 1);
PROT_ZOOM_FUNC(BC, U16, 1, 0, 0);
PROT_ZOOM_FUNC(BC, U16, 1, 0, 1);
PROT_ZOOM_FUNC(BC, U16, 1, 1, 0);
PROT_ZOOM_FUNC(BC, U16, 1, 1, 1);
PROT_ZOOM_FUNC(BC, U16, 2, 0, 0);
PROT_ZOOM_FUNC(BC, U16, 2, 0, 1);
PROT_ZOOM_FUNC(BC, U16, 2, 1, 0);
PROT_ZOOM_FUNC(BC, U16, 2, 1, 1);
PROT_ZOOM_FUNC(BC, U16, 3, 0, 0);
PROT_ZOOM_FUNC(BC, U16, 3, 0, 1);
PROT_ZOOM_FUNC(BC, U16, 3, 1, 0);
PROT_ZOOM_FUNC(BC, U16, 3, 1, 1);
PROT_ZOOM_FUNC(BC, U16, 4, 0, 0);
PROT_ZOOM_FUNC(BC, U16, 4, 0, 1);
PROT_ZOOM_FUNC(BC, U16, 4, 1, 0);
PROT_ZOOM_FUNC(BC, U16, 4, 1, 1);
PROT_ZOOM_FUNC(BC2, U8, 1, 0, 0);
PROT_ZOOM_FUNC(BC2, U8, 1, 0, 1);
PROT_ZOOM_FUNC(BC2, U8, 1, 1, 0);
PROT_ZOOM_FUNC(BC2, U8, 1, 1, 1);
PROT_ZOOM_FUNC(BC2, U8, 2, 0, 0);
PROT_ZOOM_FUNC(BC2, U8, 2, 0, 1);
PROT_ZOOM_FUNC(BC2, U8, 2, 1, 0);
PROT_ZOOM_FUNC(BC2, U8, 2, 1, 1);
PROT_ZOOM_FUNC(BC2, U8, 3, 0, 0);
PROT_ZOOM_FUNC(BC2, U8, 3, 0, 1);
PROT_ZOOM_FUNC(BC2, U8, 3, 1, 0);
PROT_ZOOM_FUNC(BC2, U8, 3, 1, 1);
PROT_ZOOM_FUNC(BC2, U8, 4, 0, 0);
PROT_ZOOM_FUNC(BC2, U8, 4, 0, 1);
PROT_ZOOM_FUNC(BC2, U8, 4, 1, 0);
PROT_ZOOM_FUNC(BC2, U8, 4, 1, 1);
PROT_ZOOM_FUNC(BC2, S16, 1, 0, 0);
PROT_ZOOM_FUNC(BC2, S16, 1, 0, 1);
PROT_ZOOM_FUNC(BC2, S16, 1, 1, 0);
PROT_ZOOM_FUNC(BC2, S16, 1, 1, 1);
PROT_ZOOM_FUNC(BC2, S16, 2, 0, 0);
PROT_ZOOM_FUNC(BC2, S16, 2, 0, 1);
PROT_ZOOM_FUNC(BC2, S16, 2, 1, 0);
PROT_ZOOM_FUNC(BC2, S16, 2, 1, 1);
PROT_ZOOM_FUNC(BC2, S16, 3, 0, 0);
PROT_ZOOM_FUNC(BC2, S16, 3, 0, 1);
PROT_ZOOM_FUNC(BC2, S16, 3, 1, 0);
PROT_ZOOM_FUNC(BC2, S16, 3, 1, 1);
PROT_ZOOM_FUNC(BC2, S16, 4, 0, 0);
PROT_ZOOM_FUNC(BC2, S16, 4, 0, 1);
PROT_ZOOM_FUNC(BC2, S16, 4, 1, 0);
PROT_ZOOM_FUNC(BC2, S16, 4, 1, 1);
PROT_ZOOM_FUNC(BC2, U16, 1, 0, 0);
PROT_ZOOM_FUNC(BC2, U16, 1, 0, 1);
PROT_ZOOM_FUNC(BC2, U16, 1, 1, 0);
PROT_ZOOM_FUNC(BC2, U16, 1, 1, 1);
PROT_ZOOM_FUNC(BC2, U16, 2, 0, 0);
PROT_ZOOM_FUNC(BC2, U16, 2, 0, 1);
PROT_ZOOM_FUNC(BC2, U16, 2, 1, 0);
PROT_ZOOM_FUNC(BC2, U16, 2, 1, 1);
PROT_ZOOM_FUNC(BC2, U16, 3, 0, 0);
PROT_ZOOM_FUNC(BC2, U16, 3, 0, 1);
PROT_ZOOM_FUNC(BC2, U16, 3, 1, 0);
PROT_ZOOM_FUNC(BC2, U16, 3, 1, 1);
PROT_ZOOM_FUNC(BC2, U16, 4, 0, 0);
PROT_ZOOM_FUNC(BC2, U16, 4, 0, 1);
PROT_ZOOM_FUNC(BC2, U16, 4, 1, 0);
PROT_ZOOM_FUNC(BC2, U16, 4, 1, 1);

/* *********************************************************** */

#define	FUNC_ZOOM_BC(BC, TT, CH, H, V)                                   \
	PROT_ZOOM_FUNC(BC, TT, CH, H, V)                                 \
	{                                                                \
	    DEF_CONST_H##H();                                            \
	    DEF_CONST_V##V();                                            \
	    mlib_s32 channels = CH;                                      \
	    mlib_s32 i, j, mask;                                         \
	    FTYPE *sp_arr[4], *sp0, *sp1, *sp2, *sp3, *dp0, *dp1;        \
	    mlib_d64 *dp, *dp_buff;                                      \
	    void *pbuff, *buff_arr[8];                                   \
	    mlib_s32 ind = 0;                                            \
	    mlib_s32 src_w, dst_w, bline, num_b, d_off, bchan;           \
	    mlib_d64 v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, r0, r1;     \
	    STYPE *sl = (STYPE *) psrc - src_stride - channels;          \
	    STYPE *dl = (STYPE *) pdst;                                  \
	                                                                 \
	    src_w_beg &= 1;                                              \
	    src_h_beg &= 1;                                              \
	    src_width -= src_w_beg;                                      \
	    d_off = sizeof (STYPE) * ((8 - channels * src_w_beg) & 7);   \
	    bchan = sizeof (STYPE) * channels;                           \
	    src_w = bchan * ((src_width / 2) + 5);                       \
	    dst_w = bchan * (src_w_beg + src_width);                     \
	    bline = (src_w + 7) & ~7;                                    \
	    num_b = (CH == 3) ? 6 : 4;                                   \
	    pbuff = __mlib_malloc(num_b * bline + d_off + dst_w + 16);   \
	    if (pbuff == NULL)                                           \
		return (MLIB_FAILURE);                                   \
	    for (i = 0; i <= num_b; i++) {                               \
		buff_arr[i] = (mlib_u8 *)pbuff + i * bline;              \
	    }                                                            \
	    dp_buff = (mlib_d64 *)buff_arr[num_b];                       \
	    if (src_w_beg) {                                             \
		dl = (void *)((mlib_u8 *)dl - d_off);                    \
	    } else {                                                     \
		sl -= channels;                                          \
	    }                                                            \
	    ADD_SLINE(4);                                                \
	    ADD_SLINE(4);                                                \
	    ADD_SLINE(4);                                                \
	    if (src_h_beg) {                                             \
		ADD_SLINE(4);                                            \
	    }                                                            \
	    vis_write_gsr(GSR_SCALE_##TT << 3);                          \
	    for (j = 0; j < src_height; j++) {                           \
		if (((src_h_beg + j) & 1) == 0) {                        \
		    ADD_SLINE(4);                                        \
		    vis_alignaddr((void *)(8 - 2 * CH), 0);              \
		    sp0 = sp_arr[(ind - 3) & 3];                         \
		    sp1 = sp_arr[(ind - 2) & 3];                         \
		    sp2 = sp_arr[(ind - 1) & 3];                         \
		    sp3 = sp_arr[ind];                                   \
		    ZOOM_LINE_BC_##CH(TT, H, A##V);                      \
		} else {                                                 \
		    vis_alignaddr((void *)(8 - 2 * CH), 0);              \
		    sp0 = sp_arr[(ind - 3) & 3];                         \
		    sp1 = sp_arr[(ind - 2) & 3];                         \
		    sp2 = sp_arr[(ind - 1) & 3];                         \
		    sp3 = sp_arr[ind];                                   \
		    ZOOM_LINE_BC_##CH(TT, H, B##V);                      \
		}                                                        \
		dl += dst_stride;                                        \
	    }                                                            \
	    __mlib_free(pbuff);                                          \
	    return (MLIB_SUCCESS);                                       \
	}

/* *********************************************************** */

#define	ZOOM_LINE_BC_1(TT, H, V)                                      \
	dp = ((mlib_addr)dl & 7) ? dp_buff : (mlib_d64 *)dl;          \
	v0 = SUMV_##V(sp0[0], sp1[0], sp2[0], sp3[0]);                \
	v1 = vis_faligndata(v0, v0);                                  \
	v2 = vis_faligndata(v1, v1);                                  \
	sp0++;                                                        \
	sp1++;                                                        \
	sp2++;                                                        \
	sp3++;                                                        \
	if (d_off) {                                                  \
	    mlib_s32 emask = (1 << channels) - 1;                     \
	                                                              \
	    v3 = vis_faligndata(v2, v2);                              \
	    r1 = SUMH_B##H(v3, v2, v1, v0);                           \
	    SAVE_MASK_##TT(CH1, dp, emask, r1, r1);                   \
	}                                                             \
	for (i = 0; i < src_width / 8; i++) {                         \
	    v3 = SUMV_##V(sp0[0], sp1[0], sp2[0], sp3[0]);            \
	    v4 = vis_faligndata(v0, v3);                              \
	    v5 = vis_faligndata(v1, v4);                              \
	    v6 = vis_faligndata(v2, v5);                              \
	    sp0++;                                                    \
	    sp1++;                                                    \
	    sp2++;                                                    \
	    sp3++;                                                    \
	    r0 = SUMH_A##H(v6, v5, v4, v3);                           \
	    r1 = SUMH_B##H(v6, v5, v4, v3);                           \
	    SAVE_##TT(CH1, dp, r0, r1);                               \
	    v0 = v3;                                                  \
	    v1 = v4;                                                  \
	    v2 = v5;                                                  \
	}                                                             \
	                                                              \
	if (src_width & 7) {                                          \
	    mlib_s32 emask = 0xFF00 >> (src_width & 7);               \
	                                                              \
	    v3 = SUMV_##V(sp0[0], sp1[0], sp2[0], sp3[0]);            \
	    v4 = vis_faligndata(v0, v3);                              \
	    v5 = vis_faligndata(v1, v4);                              \
	    v6 = vis_faligndata(v2, v5);                              \
	    r0 = SUMH_A##H(v6, v5, v4, v3);                           \
	    r1 = SUMH_B##H(v6, v5, v4, v3);                           \
	    SAVE_MASK_##TT(CH1, dp, emask, r0, r1);                   \
	}                                                             \
	if ((mlib_addr)dl & 7) {                                      \
	    COPY((mlib_u8 *)dp_buff + d_off, (mlib_u8 *)dl + d_off,   \
		dst_w);                                               \
	}

/* *********************************************************** */

#define	ZOOM_LINE_BC_2(TT, H, V)                                      \
	dp = ((mlib_addr)dl & 7) ? dp_buff : (mlib_d64 *)dl;          \
	v0 = SUMV_##V(sp0[0], sp1[0], sp2[0], sp3[0]);                \
	sp0++;                                                        \
	sp1++;                                                        \
	sp2++;                                                        \
	sp3++;                                                        \
	v2 = SUMV_##V(sp0[0], sp1[0], sp2[0], sp3[0]);                \
	sp0++;                                                        \
	sp1++;                                                        \
	sp2++;                                                        \
	sp3++;                                                        \
	v1 = vis_faligndata(v0, v2);                                  \
	if (d_off) {                                                  \
	    mlib_s32 emask = (1 << channels) - 1;                     \
	                                                              \
	    v3 = vis_faligndata(v0, v0);                              \
	    r1 = SUMH_B##H(v3, v0, v1, v2);                           \
	    SAVE_MASK_##TT(CH2, dp, emask, r1, r1);                   \
	}                                                             \
	for (i = 0; i < src_width / 4; i++) {                         \
	    v4 = SUMV_##V(sp0[0], sp1[0], sp2[0], sp3[0]);            \
	    v3 = vis_faligndata(v2, v4);                              \
	    sp0++;                                                    \
	    sp1++;                                                    \
	    sp2++;                                                    \
	    sp3++;                                                    \
	    r0 = SUMH_A##H(v1, v2, v3, v4);                           \
	    r1 = SUMH_B##H(v1, v2, v3, v4);                           \
	    SAVE_##TT(CH2, dp, r0, r1);                               \
	    v1 = v3;                                                  \
	    v2 = v4;                                                  \
	}                                                             \
	                                                              \
	if (src_width & 3) {                                          \
	    mlib_s32 emask = 0xFF00 >> (2 * (src_width & 3));         \
	                                                              \
	    v4 = SUMV_##V(sp0[0], sp1[0], sp2[0], sp3[0]);            \
	    v3 = vis_faligndata(v2, v4);                              \
	    r0 = SUMH_A##H(v1, v2, v3, v4);                           \
	    r1 = SUMH_B##H(v1, v2, v3, v4);                           \
	    SAVE_MASK_##TT(CH2, dp, emask, r0, r1);                   \
	}                                                             \
	if ((mlib_addr)dl & 7) {                                      \
	    COPY((mlib_u8 *)dp_buff + d_off, (mlib_u8 *)dl + d_off,   \
		dst_w);                                               \
	}

/* *********************************************************** */

#define	ZOOM_LINE_BC_3(TT, H, V)                                          \
	dp0 = (void *)buff_arr[4];                                        \
	dp1 = (void *)buff_arr[5];                                        \
	dp = dp_buff;                                                     \
	v0 = SUMV_##V(sp0[0], sp1[0], sp2[0], sp3[0]);                    \
	sp0++;                                                            \
	sp1++;                                                            \
	sp2++;                                                            \
	sp3++;                                                            \
	v1 = SUMV_##V(sp0[0], sp1[0], sp2[0], sp3[0]);                    \
	sp0++;                                                            \
	sp1++;                                                            \
	sp2++;                                                            \
	sp3++;                                                            \
	v2 = vis_faligndata(v0, v1);                                      \
	v3 = SUMV_##V(sp0[0], sp1[0], sp2[0], sp3[0]);                    \
	sp0++;                                                            \
	sp1++;                                                            \
	sp2++;                                                            \
	sp3++;                                                            \
	v4 = vis_faligndata(v1, v3);                                      \
	v5 = vis_faligndata(v2, v4);                                      \
	if (src_w_beg) {                                                  \
	    v6 = SUMV_##V(sp0[0], sp1[0], sp2[0], sp3[0]);                \
	    v7 = vis_faligndata(v3, v6);                                  \
	    v8 = vis_faligndata(v4, v7);                                  \
	    v9 = vis_faligndata(v5, v8);                                  \
	    r1 = SUMH_B##H(v0, v9, v8, v7);                               \
	    SAVE_MASK_##TT(CH4, dp, 0xF0, r1, r1);                        \
	    dl[5] = ((STYPE *) dp_buff)[0];                               \
	    dl[6] = ((STYPE *) dp_buff)[1];                               \
	    dl[7] = ((STYPE *) dp_buff)[2];                               \
	}                                                                 \
	for (i = 0; i < (channels * src_width + 5 + 7) / 8; i++) {        \
	    v6 = SUMV_##V(sp0[0], sp1[0], sp2[0], LOAD_NF(sp3));          \
	    v7 = vis_faligndata(v3, v6);                                  \
	    v8 = vis_faligndata(v4, v7);                                  \
	    v9 = vis_faligndata(v5, v8);                                  \
	    sp0++;                                                        \
	    sp1++;                                                        \
	    sp2++;                                                        \
	    sp3++;                                                        \
	    r0 = SUMH_A##H(v9, v8, v7, v6);                               \
	    r1 = SUMH_B##H(v9, v8, v7, v6);                               \
	    SAVE2_##TT(CH4, dp0, dp1, r0, r1);                            \
	    v0 = v1;                                                      \
	    v1 = v3;                                                      \
	    v2 = v4;                                                      \
	    v3 = v6;                                                      \
	    v4 = v7;                                                      \
	    v5 = v8;                                                      \
	}                                                                 \
	                                                                  \
	dp = (src_w_beg) ? (void *)(dl + 8) : (void *)dl;                 \
	if ((mlib_addr)dl & 7) {                                          \
	    mlib_ImageMerge3ch_##TT(buff_arr[4], buff_arr[5],             \
		dp_buff, src_width);                                      \
	    COPY((mlib_u8 *)dp_buff, (mlib_u8 *)dp, bchan * src_width);   \
	} else {                                                          \
	    mlib_ImageMerge3ch_##TT(buff_arr[4], buff_arr[5], dp,         \
		src_width);                                               \
	}

/* *********************************************************** */

#define	ZOOM_LINE_BC_4_U8(TT, H, V)                                   \
	dp = ((mlib_addr)dl & 3) ? dp_buff : (mlib_d64 *)dl;          \
	v3 = SUMV_##V(LOAD_NF(sp0), sp1[0], sp2[0], sp3[0]);          \
	sp0++;                                                        \
	sp1++;                                                        \
	sp2++;                                                        \
	sp3++;                                                        \
	v0 = SUMV_##V(sp0[0], sp1[0], sp2[0], sp3[0]);                \
	sp0++;                                                        \
	sp1++;                                                        \
	sp2++;                                                        \
	sp3++;                                                        \
	v1 = SUMV_##V(sp0[0], sp1[0], sp2[0], sp3[0]);                \
	sp0++;                                                        \
	sp1++;                                                        \
	sp2++;                                                        \
	sp3++;                                                        \
	v2 = SUMV_##V(sp0[0], sp1[0], sp2[0], sp3[0]);                \
	sp0++;                                                        \
	sp1++;                                                        \
	sp2++;                                                        \
	sp3++;                                                        \
	if (d_off) {                                                  \
	    r1 = SUMH_B##H(v3, v0, v1, v2);                           \
	    ((mlib_f32 *)dp)[1] = vis_fpack16(r1);                    \
	    dp++;                                                     \
	}                                                             \
	for (i = 0; i < (src_width / 2); i++) {                       \
	    v3 = SUMV_##V(sp0[0], sp1[0], sp2[0], sp3[0]);            \
	    sp0++;                                                    \
	    sp1++;                                                    \
	    sp2++;                                                    \
	    sp3++;                                                    \
	    r0 = SUMH_A##H(v0, v1, v2, v3);                           \
	    r1 = SUMH_B##H(v0, v1, v2, v3);                           \
	    ((mlib_f32 *)dp)[0] = vis_fpack16(r0);                    \
	    ((mlib_f32 *)dp)[1] = vis_fpack16(r1);                    \
	    dp++;                                                     \
	    v0 = v1;                                                  \
	    v1 = v2;                                                  \
	    v2 = v3;                                                  \
	}                                                             \
	                                                              \
	if (src_width & 1) {                                          \
	    v3 = SUMV_##V(sp0[0], sp1[0], sp2[0], sp3[0]);            \
	    r0 = SUMH_A##H(v0, v1, v2, v3);                           \
	    ((mlib_f32 *)dp)[0] = vis_fpack16(r0);                    \
	}                                                             \
	if ((mlib_addr)dl & 3) {                                      \
	    COPY((mlib_u8 *)dp_buff + d_off, (mlib_u8 *)dl + d_off,   \
		dst_w);                                               \
	}

/* *********************************************************** */

#define	ZOOM_LINE_BC_4_S16(TT, H, V)                                  \
	dp = ((mlib_addr)dl & 7) ? dp_buff : (mlib_d64 *)dl;          \
	v3 = SUMV_##V(LOAD_NF(sp0), sp1[0], sp2[0], sp3[0]);          \
	sp0++;                                                        \
	sp1++;                                                        \
	sp2++;                                                        \
	sp3++;                                                        \
	v0 = SUMV_##V(sp0[0], sp1[0], sp2[0], sp3[0]);                \
	sp0++;                                                        \
	sp1++;                                                        \
	sp2++;                                                        \
	sp3++;                                                        \
	v1 = SUMV_##V(sp0[0], sp1[0], sp2[0], sp3[0]);                \
	sp0++;                                                        \
	sp1++;                                                        \
	sp2++;                                                        \
	sp3++;                                                        \
	v2 = SUMV_##V(sp0[0], sp1[0], sp2[0], sp3[0]);                \
	sp0++;                                                        \
	sp1++;                                                        \
	sp2++;                                                        \
	sp3++;                                                        \
	if (d_off) {                                                  \
	    r1 = SUMH_B##H(v3, v0, v1, v2);                           \
	    SAVE_MASK_##TT(CH4, dp, 0x0F, r1, r1);                    \
	}                                                             \
	for (i = 0; i < (src_width / 2); i++) {                       \
	    v3 = SUMV_##V(sp0[0], sp1[0], sp2[0], sp3[0]);            \
	    sp0++;                                                    \
	    sp1++;                                                    \
	    sp2++;                                                    \
	    sp3++;                                                    \
	    r0 = SUMH_A##H(v0, v1, v2, v3);                           \
	    r1 = SUMH_B##H(v0, v1, v2, v3);                           \
	    SAVE_##TT(CH4, dp, r0, r1);                               \
	    v0 = v1;                                                  \
	    v1 = v2;                                                  \
	    v2 = v3;                                                  \
	}                                                             \
	                                                              \
	if (src_width & 1) {                                          \
	    v3 = SUMV_##V(sp0[0], sp1[0], sp2[0], sp3[0]);            \
	    r0 = SUMH_A##H(v0, v1, v2, v3);                           \
	    SAVE_MASK_##TT(CH4, dp, 0xF0, r0, r0);                    \
	}                                                             \
	if ((mlib_addr)dl & 7) {                                      \
	    COPY((mlib_u8 *)dp_buff + d_off, (mlib_u8 *)dl + d_off,   \
		dst_w);                                               \
	}

/* *********************************************************** */

#define	ZOOM_LINE_BC_4_U16	ZOOM_LINE_BC_4_S16

#define	ZOOM_LINE_BC_4(TT, H, V)	ZOOM_LINE_BC_4_##TT(TT, H, V)

/* *********************************************************** */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_V_IMAGEZOOMIN2X_H */
