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

#pragma ident	"@(#)mlib_v_ImageMulShiftFunc.c	9.2	07/11/05 SMI"

/*
 * Internal functions.
 *
 *     void mlib_v_ImageMul_U8(mlib_u8 *sp1,
 *                             mlib_s32 stride1,
 *                             mlib_u8 *sp2,
 *                             mlib_s32 stride2,
 *                             mlib_u8 *dp,
 *                             mlib_s32 strided,
 *                             mlib_s32 width,
 *                             mlib_s32 height);
 *
 *     void mlib_v_ImageMulShift_U8(mlib_u8 *sp1,
 *                                  mlib_s32 stride1,
 *                                  mlib_u8 *sp2,
 *                                  mlib_s32 stride2,
 *                                  mlib_u8 *dp,
 *                                  mlib_s32 strided,
 *                                  mlib_s32 width,
 *                                  mlib_s32 height,
 *                                  mlib_s32 shift);
 *
 *     void mlib_v_ImageMulShift123_U8(mlib_u8 *sp1,
 *                                     mlib_s32 stride1,
 *                                     mlib_u8 *sp2,
 *                                     mlib_s32 stride2,
 *                                     mlib_u8 *dp,
 *                                     mlib_s32 strided,
 *                                     mlib_s32 width,
 *                                     mlib_s32 height,
 *                                     mlib_s32 shift);
 *
 *     void mlib_v_ImageMulShift_S32(mlib_s32 *sp1,
 *                                   mlib_s32 stride1,
 *                                   mlib_s32 *sp2,
 *                                   mlib_s32 stride2,
 *                                   mlib_s32 *dp,
 *                                   mlib_s32 strided,
 *                                   mlib_s32 width,
 *                                   mlib_s32 height,
 *                                   mlib_s32 shift);
 *
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_v_ImageMulShift.h>

/* *********************************************************** */

#ifndef MLIB_USE_FTOI_CLAMPING

/* *********************************************************** */

#define	FLOAT2INT_CLAMP(DST, SRC)                               \
	{                                                       \
	    mlib_d64 dsrc = (mlib_d64)(SRC);                    \
	                                                        \
	    if (dsrc > (mlib_d64)MLIB_S32_MAX)                  \
		dsrc = (mlib_d64)MLIB_S32_MAX;                  \
	    if (dsrc < (mlib_d64)MLIB_S32_MIN)                  \
		dsrc = (mlib_d64)MLIB_S32_MIN;                  \
	    DST = (mlib_s32)dsrc;                               \
	}

#else /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#define	FLOAT2INT_CLAMP(DST, SRC)	DST = ((mlib_s32)(SRC))
#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

#define	SCALBN(res, shift)                                      \
	                                                        \
/*                                                              \
 * res = 2 ** (-shift)                                          \
 */                                                             \
	res = 0.;                                               \
	((mlib_s32 *)&res)[0] = (1023 + shift) << 20

/* *********************************************************** */

#define	MLIB_V_IMAGEMULSHIFT_U8(sd1, sd2, dd)                   \
	sd2h = vis_fexpand_hi(sd2);                             \
	sd2l = vis_fexpand_lo(sd2);                             \
	rdh = vis_fmul8x16(vis_read_hi(sd1), sd2h);             \
	rdl = vis_fmul8x16(vis_read_lo(sd1), sd2l);             \
	dd = vis_fpack16_pair(rdh, rdl)

/* *********************************************************** */

#define	MLIB_V_IMAGEMULSHIFT123_U8(sd1, sd2, dd)                \
	sd2h = vis_fmul8x16al(vis_read_hi(sd2), f_2pow14);      \
	sd2l = vis_fmul8x16al(vis_read_lo(sd2), f_2pow14);      \
	sd2h = vis_fpadd16(sd2h, sd2h);                         \
	sd2l = vis_fpadd16(sd2l, sd2l);                         \
	rdh = vis_fmul8x16(vis_read_hi(sd1), sd2h);             \
	rdl = vis_fmul8x16(vis_read_lo(sd1), sd2l);             \
	dd = vis_fpack16_pair(rdh, rdl)

/* *********************************************************** */

#define	MLIB_V_IMAGEMUL_U8_U16(dx, dy, ddh, ddl)                \
	dr2 = vis_fxnor(restore, dy);                           \
	dr4 = vis_fpmerge(vis_read_hi(dr2), vis_fzeros());      \
	dr4 = vis_fmul8x16(vis_read_hi(dx), dr4);               \
	dr3 = vis_fmul8x16al(vis_read_hi(dx), f127);            \
	ddh = vis_fpsub16(dr3, dr4);                            \
	dr2 = vis_fpmerge(vis_read_lo(dr2), vis_fzeros());      \
	dr2 = vis_fmul8x16(vis_read_lo(dx), dr2);               \
	dr5 = vis_fmul8x16al(vis_read_lo(dx), f127);            \
	ddl = vis_fpsub16(dr5, dr2)

/* *********************************************************** */

#define	MLIB_V_IMAGEMUL_U8(sd1, sd2, dd)                        \
	{                                                       \
	    mlib_d64 dr2, dr3, dr4, dr5;                        \
	    mlib_d64 ddh, ddl, ddh2, ddl2;                      \
	                                                        \
	    MLIB_V_IMAGEMUL_U8_U16(sd1, sd2, ddh, ddl);         \
	    ddh2 = vis_fmul8sux16(ddh, d_s16_2pow14);           \
	    ddl2 = vis_fmul8sux16(ddl, d_s16_2pow14);           \
	    ddh = vis_fxor(ddh, ddh2);                          \
	    ddl = vis_fxor(ddl, ddl2);                          \
	    dd = vis_fpack16_pair(ddh, ddl);                    \
	}

/* *********************************************************** */

void
mlib_v_ImageMul_U8(
    mlib_u8 *sp1,
    mlib_s32 stride1,
    mlib_u8 *sp2,
    mlib_s32 stride2,
    mlib_u8 *dp,
    mlib_s32 strided,
    mlib_s32 width,
    mlib_s32 height)
{
/* pointers for line of source1 */
	mlib_u8 *sl1;

/* pointers for line of source2 */
	mlib_u8 *sl2;

/* pointers for line of dst */
	mlib_u8 *dl;
	mlib_s32 offdst, offsrc1, offsrc2, emask;
	mlib_d64 *dpp, *spp2, *spp1, *tmp_ptr;
	mlib_d64 dd, dd0, dd1, sd10, sd11, sd20, sd21;
	mlib_u8 *dend;
	mlib_d64 restore = vis_to_double_dup(0x80808080);
	mlib_d64 d_s16_2pow14 = vis_to_double_dup(0x40004000);
	mlib_f32 f127 = vis_to_float(0x7f00);
	mlib_s32 i, j, k;

	if (width == stride1 && width == stride2 && width == strided) {
		width *= height;
		height = 1;
	}

/* initialize GSR scale factor */
	vis_write_gsr(7 << 3);

	sl1 = sp1;
	sl2 = sp2;
	dl = dp;

	offdst = ((mlib_addr)dp) & 7;
	offsrc1 = ((mlib_addr)sp1) & 7;
	offsrc2 = ((mlib_addr)sp2) & 7;

	if ((offdst == offsrc1) && (offdst == offsrc2) &&
	    ((strided & 7) == (stride1 & 7)) &&
	    ((strided & 7) == (stride2 & 7))) {
/* dst, src1, src2 are identically aligned */

		for (j = 0; j < height; j++) {

/* prepare the destination addresses */
			dpp = (mlib_d64 *)vis_alignaddr(dp, 0);
			i = (mlib_u8 *)dpp - dp;

/* prepare the source addresses */
			spp1 = (mlib_d64 *)vis_alignaddr(sp1, 0);
			spp2 = (mlib_d64 *)vis_alignaddr(sp2, 0);

			dend = dp + width - 1;
/* generate edge mask for the start point */
			emask = vis_edge8(dp, dend);

			if (emask != 0xff) {
				sd10 = (*spp1++);
				sd20 = (*spp2++);
				MLIB_V_IMAGEMUL_U8(sd10, sd20, dd);
				vis_pst_8(dd, dpp++, emask);
				i += 8;
			}
#pragma pipeloop(0)
			for (; i <= width - 8; i += 8) {
				sd10 = (*spp1++);
				sd20 = (*spp2++);
				MLIB_V_IMAGEMUL_U8(sd10, sd20, dd);
				(*dpp++) = dd;
			}

			if (i < width) {
				emask = vis_edge8(dpp, dend);
				sd10 = vis_ld_d64_nf(spp1); spp1++;
				sd20 = vis_ld_d64_nf(spp2); spp2++;
				MLIB_V_IMAGEMUL_U8(sd10, sd20, dd);
				vis_pst_8(dd, dpp, emask);
			}

			sp1 = sl1 += stride1;
			sp2 = sl2 += stride2;
			dp = dl += strided;
		}
	} else if ((offdst == offsrc1) && ((strided & 7) == (stride1 & 7))) {
/* dst, src1 are identically aligned */

		for (j = 0; j < height; j++) {

/* prepare the destination addresses */
			dpp = (mlib_d64 *)vis_alignaddr(dp, 0);
			i = (mlib_u8 *)dpp - dp;

/* prepare the source addresses */
			spp1 = (mlib_d64 *)vis_alignaddr(sp1, 0);
			spp2 = (mlib_d64 *)vis_alignaddr(sp2, i);

			dend = dp + width - 1;
/* generate edge mask for the start point */
			emask = vis_edge8(dp, dend);

			sd20 = vis_ld_d64_nf(spp2);

			if (emask != 0xff) {
				sd10 = (*spp1++);
				sd21 = vis_ld_d64_nf(spp2 + 1);
				sd20 = vis_faligndata(sd20, sd21);
				MLIB_V_IMAGEMUL_U8(sd10, sd20, dd);
				vis_pst_8(dd, dpp++, emask);
				sd20 = sd21;
				spp2++;
				i += 8;
			}
#pragma pipeloop(0)
			for (; i <= width - 8; i += 8) {
				sd10 = (*spp1++);
				sd21 = vis_ld_d64_nf(spp2 + 1);
				sd20 = vis_faligndata(sd20, sd21);
				MLIB_V_IMAGEMUL_U8(sd10, sd20, dd);
				(*dpp++) = dd;
				sd20 = sd21;
				spp2++;
			}

			if (i < width) {
				emask = vis_edge8(dpp, dend);
				sd10 = (*spp1++);
				sd20 = vis_faligndata(sd20,
					vis_ld_d64_nf(spp2 + 1));
				MLIB_V_IMAGEMUL_U8(sd10, sd20, dd);
				vis_pst_8(dd, dpp, emask);
			}

			sp1 = sl1 += stride1;
			sp2 = sl2 += stride2;
			dp = dl += strided;
		}
	} else if ((offdst == offsrc2) && ((strided & 7) == (stride2 & 7))) {
/* dst, src2 are identically aligned */

		for (j = 0; j < height; j++) {

/* prepare the destination addresses */
			dpp = (mlib_d64 *)vis_alignaddr(dp, 0);
			i = (mlib_u8 *)dpp - dp;

/* prepare the source addresses */
			spp2 = (mlib_d64 *)vis_alignaddr(sp2, 0);
			spp1 = (mlib_d64 *)vis_alignaddr(sp1, i);

			dend = dp + width - 1;
/* generate edge mask for the start point */
			emask = vis_edge8(dp, dend);

			sd10 = vis_ld_d64_nf(spp1);

			if (emask != 0xff) {
				sd20 = (*spp2++);
				sd11 = vis_ld_d64_nf(spp1 + 1);
				sd10 = vis_faligndata(sd10, sd11);
				MLIB_V_IMAGEMUL_U8(sd10, sd20, dd);
				vis_pst_8(dd, dpp++, emask);
				sd10 = sd11;
				spp1++;
				i += 8;
			}
#pragma pipeloop(0)
			for (; i <= width - 8; i += 8) {
				sd20 = (*spp2++);
				sd11 = vis_ld_d64_nf(spp1 + 1);
				sd10 = vis_faligndata(sd10, sd11);
				MLIB_V_IMAGEMUL_U8(sd10, sd20, dd);
				(*dpp++) = dd;
				sd10 = sd11;
				spp1++;
			}

			if (i < width) {
				emask = vis_edge8(dpp, dend);
				sd20 = (*spp2++);
				sd10 = vis_faligndata(sd10,
					vis_ld_d64_nf(spp1 + 1));
				MLIB_V_IMAGEMUL_U8(sd10, sd20, dd);
				vis_pst_8(dd, dpp, emask);
			}

			sp1 = sl1 += stride1;
			sp2 = sl2 += stride2;
			dp = dl += strided;
		}
	} else if ((offsrc1 == offsrc2) && ((stride1 & 7) == (stride2 & 7))) {
/* src1, src2 are identically aligned */

		for (j = 0; j < height; j++) {

/* prepare the source addresses */
			dpp = (mlib_d64 *)vis_alignaddr(dp, 0);
			i = (mlib_u8 *)dpp - dp;

/* prepare the destination addresses */
			spp1 = (mlib_d64 *)vis_alignaddr(sp1, i);
			spp2 = (mlib_d64 *)vis_alignaddr(sp2, i);

			dend = dp + width - 1;
/* generate edge mask for the start point */
			emask = vis_edge8(dp, dend);

			sd10 = vis_ld_d64_nf(spp1); spp1++;
			sd20 = vis_ld_d64_nf(spp2); spp2++;
			MLIB_V_IMAGEMUL_U8(sd10, sd20, dd0);

			if (emask != 0xff) {
				sd10 = vis_ld_d64_nf(spp1); spp1++;
				sd20 = vis_ld_d64_nf(spp2); spp2++;
				MLIB_V_IMAGEMUL_U8(sd10, sd20, dd1);
				dd = vis_faligndata(dd0, dd1);
				vis_pst_8(dd, dpp++, emask);
				dd0 = dd1;
				i += 8;
			}
#pragma pipeloop(0)
			for (; i <= width - 8; i += 8) {
				sd10 = vis_ld_d64_nf(spp1); spp1++;
				sd20 = vis_ld_d64_nf(spp2); spp2++;
				MLIB_V_IMAGEMUL_U8(sd10, sd20, dd1);
				(*dpp++) = vis_faligndata(dd0, dd1);
				dd0 = dd1;
			}

			if (i < width) {
				emask = vis_edge8(dpp, dend);
				sd10 = vis_ld_d64_nf(spp1); spp1++;
				sd20 = vis_ld_d64_nf(spp2); spp2++;
				MLIB_V_IMAGEMUL_U8(sd10, sd20, dd1);
				dd = vis_faligndata(dd0, dd1);
				vis_pst_8(dd, dpp, emask);
			}

			sp1 = sl1 += stride1;
			sp2 = sl2 += stride2;
			dp = dl += strided;
		}
	} else {
/* general case */

		for (j = 0; j < height; j++) {

/* prepare the destination addresses */
			dpp = (mlib_d64 *)vis_alignaddr(dp, 0);
			i = (mlib_u8 *)dpp - dp;

			dend = dp + width - 1;
/* generate edge mask for the start point */
			emask = vis_edge8(dp, dend);

			if (emask != 0xff) {
				spp1 = (mlib_d64 *)vis_alignaddr(sp1, i);
				sd10 = vis_faligndata(vis_ld_d64_nf(spp1),
					vis_ld_d64_nf(spp1 + 1));
				spp2 = (mlib_d64 *)vis_alignaddr(sp2, i);
				sd20 = vis_faligndata(vis_ld_d64_nf(spp2),
					vis_ld_d64_nf(spp2 + 1));
				MLIB_V_IMAGEMUL_U8(sd10, sd20, dd);
				vis_pst_8(dd, dpp++, emask);
				i += 8;
			}

/* copy src1 to dst */
			spp1 = (mlib_d64 *)vis_alignaddr(sp1, i);
			sd11 = vis_ld_d64_nf(spp1);
			tmp_ptr = dpp;

#pragma pipeloop(0)
			for (k = i; k <= (width - 8); k += 8) {
				sd10 = sd11;
				sd11 = vis_ld_d64_nf(spp1 + 1);
				(*tmp_ptr++) = vis_faligndata(sd10, sd11);
				spp1++;
			}

			sd11 = vis_faligndata(sd11,
				vis_ld_d64_nf(spp1 + 1));

			spp2 = (mlib_d64 *)vis_alignaddr(sp2, i);
			sd20 = vis_ld_d64_nf(spp2);
			tmp_ptr = dpp;

#pragma pipeloop(0)
			for (; i <= width - 8; i += 8) {
				sd10 = (*tmp_ptr++);
				sd21 = vis_ld_d64_nf(spp2 + 1);
				sd20 = vis_faligndata(sd20, sd21);
				MLIB_V_IMAGEMUL_U8(sd10, sd20, dd);
				(*dpp++) = dd;
				sd20 = sd21;
				spp2++;
			}

			if (i < width) {
				emask = vis_edge8(dpp, dend);
				sd20 = vis_faligndata(sd20,
					vis_ld_d64_nf(spp2 + 1));
				MLIB_V_IMAGEMUL_U8(sd11, sd20, dd);
				vis_pst_8(dd, dpp, emask);
			}

			sp1 = sl1 += stride1;
			sp2 = sl2 += stride2;
			dp = dl += strided;
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageMulShift_U8(
    mlib_u8 *sp1,
    mlib_s32 stride1,
    mlib_u8 *sp2,
    mlib_s32 stride2,
    mlib_u8 *dp,
    mlib_s32 strided,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 shift)
{
/* pointers for line of source1 */
	mlib_u8 *sl1;

/* pointers for line of source2 */
	mlib_u8 *sl2;

/* pointers for line of dst */
	mlib_u8 *dl;
	mlib_s32 offdst, offsrc1, offsrc2, emask;
	mlib_d64 *dpp, *spp2, *spp1, *tmp_ptr;
	mlib_d64 dd, dd0, dd1, sd10, sd11, sd20, sd21;
	mlib_d64 sd2h, sd2l, rdh, rdl;
	mlib_u8 *dend;
	mlib_s32 i, j, k;

	if (width == stride1 && width == stride2 && width == strided) {
		width *= height;
		height = 1;
	}

/* initialize GSR scale factor */
	vis_write_gsr(((11 - shift) & 0xF) << 3);

	sl1 = sp1;
	sl2 = sp2;
	dl = dp;

	offdst = ((mlib_addr)dp) & 7;
	offsrc1 = ((mlib_addr)sp1) & 7;
	offsrc2 = ((mlib_addr)sp2) & 7;

	if ((offdst == offsrc1) && (offdst == offsrc2) &&
	    ((strided & 7) == (stride1 & 7)) &&
	    ((strided & 7) == (stride2 & 7))) {

		for (j = 0; j < height; j++) {

/* prepare the destination addresses */
			dpp = (mlib_d64 *)vis_alignaddr(dp, 0);
			i = (mlib_u8 *)dpp - dp;

/* prepare the source addresses */
			spp1 = (mlib_d64 *)vis_alignaddr(sp1, 0);
			spp2 = (mlib_d64 *)vis_alignaddr(sp2, 0);

			dend = dp + width - 1;
/* generate edge mask for the start point */
			emask = vis_edge8(dp, dend);

			if (emask != 0xff) {
				sd10 = (*spp1++);
				sd20 = (*spp2++);
				MLIB_V_IMAGEMULSHIFT_U8(sd10, sd20, dd);
				vis_pst_8(dd, dpp++, emask);
				i += 8;
			}
#pragma pipeloop(0)
			for (; i <= width - 8; i += 8) {
				sd10 = (*spp1++);
				sd20 = (*spp2++);
				MLIB_V_IMAGEMULSHIFT_U8(sd10, sd20, dd);
				(*dpp++) = dd;
			}

			if (i < width) {
				emask = vis_edge8(dpp, dend);
				sd10 = (*spp1++);
				sd20 = (*spp2++);
				MLIB_V_IMAGEMULSHIFT_U8(sd10, sd20, dd);
				vis_pst_8(dd, dpp, emask);
			}

			sp1 = sl1 += stride1;
			sp2 = sl2 += stride2;
			dp = dl += strided;
		}
	} else if ((offdst == offsrc1) && ((strided & 7) == (stride1 & 7))) {

		for (j = 0; j < height; j++) {

/* prepare the destination addresses */
			dpp = (mlib_d64 *)vis_alignaddr(dp, 0);
			i = (mlib_u8 *)dpp - dp;

/* prepare the source addresses */
			spp1 = (mlib_d64 *)vis_alignaddr(sp1, 0);
			spp2 = (mlib_d64 *)vis_alignaddr(sp2, i);

			dend = dp + width - 1;
/* generate edge mask for the start point */
			emask = vis_edge8(dp, dend);

			sd20 = vis_ld_d64_nf(spp2);

			if (emask != 0xff) {
				sd10 = (*spp1++);
				sd21 = vis_ld_d64_nf(spp2 + 1);
				sd20 = vis_faligndata(sd20, sd21);
				MLIB_V_IMAGEMULSHIFT_U8(sd10, sd20, dd);
				vis_pst_8(dd, dpp++, emask);
				sd20 = sd21;
				spp2++;
				i += 8;
			}
#pragma pipeloop(0)
			for (; i <= width - 8; i += 8) {
				sd10 = (*spp1++);
				sd21 = vis_ld_d64_nf(spp2 + 1);
				sd20 = vis_faligndata(sd20, sd21);
				MLIB_V_IMAGEMULSHIFT_U8(sd10, sd20, dd);
				(*dpp++) = dd;
				sd20 = sd21;
				spp2++;
			}

			if (i < width) {
				emask = vis_edge8(dpp, dend);
				sd10 = (*spp1++);
				sd20 = vis_faligndata(sd20,
					vis_ld_d64_nf(spp2 + 1));
				MLIB_V_IMAGEMULSHIFT_U8(sd10, sd20, dd);
				vis_pst_8(dd, dpp, emask);
			}

			sp1 = sl1 += stride1;
			sp2 = sl2 += stride2;
			dp = dl += strided;
		}
	} else if ((offdst == offsrc2) && ((strided & 7) == (stride2 & 7))) {

		for (j = 0; j < height; j++) {

/* prepare the destination addresses */
			dpp = (mlib_d64 *)vis_alignaddr(dp, 0);
			i = (mlib_u8 *)dpp - dp;

/* prepare the source addresses */
			spp2 = (mlib_d64 *)vis_alignaddr(sp2, 0);
			spp1 = (mlib_d64 *)vis_alignaddr(sp1, i);

			dend = dp + width - 1;
/* generate edge mask for the start point */
			emask = vis_edge8(dp, dend);

			sd10 = vis_ld_d64_nf(spp1);

			if (emask != 0xff) {
				sd20 = (*spp2++);
				sd11 = vis_ld_d64_nf(spp1 + 1);
				sd10 = vis_faligndata(sd10, sd11);
				MLIB_V_IMAGEMULSHIFT_U8(sd10, sd20, dd);
				vis_pst_8(dd, dpp++, emask);
				sd10 = sd11;
				spp1++;
				i += 8;
			}
#pragma pipeloop(0)
			for (; i <= width - 8; i += 8) {
				sd20 = (*spp2++);
				sd11 = vis_ld_d64_nf(spp1 + 1);
				sd10 = vis_faligndata(sd10, sd11);
				MLIB_V_IMAGEMULSHIFT_U8(sd10, sd20, dd);
				(*dpp++) = dd;
				sd10 = sd11;
				spp1++;
			}

			if (i < width) {
				emask = vis_edge8(dpp, dend);
				sd20 = (*spp2++);
				sd10 = vis_faligndata(sd10,
					vis_ld_d64_nf(spp1 + 1));
				MLIB_V_IMAGEMULSHIFT_U8(sd10, sd20, dd);
				vis_pst_8(dd, dpp, emask);
			}

			sp1 = sl1 += stride1;
			sp2 = sl2 += stride2;
			dp = dl += strided;
		}
	} else if ((offsrc1 == offsrc2) && ((stride1 & 7) == (stride2 & 7))) {

		for (j = 0; j < height; j++) {

/* prepare the source addresses */
			dpp = (mlib_d64 *)vis_alignaddr(dp, 0);
			i = (mlib_u8 *)dpp - dp;

/* prepare the destination addresses */
			spp1 = (mlib_d64 *)vis_alignaddr(sp1, i);
			spp2 = (mlib_d64 *)vis_alignaddr(sp2, i);

			dend = dp + width - 1;
/* generate edge mask for the start point */
			emask = vis_edge8(dp, dend);

			sd10 = vis_ld_d64_nf(spp1); spp1++;
			sd20 = vis_ld_d64_nf(spp2); spp2++;
			MLIB_V_IMAGEMULSHIFT_U8(sd10, sd20, dd0);

			if (emask != 0xff) {
				sd10 = vis_ld_d64_nf(spp1); spp1++;
				sd20 = vis_ld_d64_nf(spp2); spp2++;
				MLIB_V_IMAGEMULSHIFT_U8(sd10, sd20, dd1);
				dd = vis_faligndata(dd0, dd1);
				vis_pst_8(dd, dpp++, emask);
				dd0 = dd1;
				i += 8;
			}
#pragma pipeloop(0)
			for (; i <= width - 8; i += 8) {
				sd10 = vis_ld_d64_nf(spp1); spp1++;
				sd20 = vis_ld_d64_nf(spp2); spp2++;
				MLIB_V_IMAGEMULSHIFT_U8(sd10, sd20, dd1);
				(*dpp++) = vis_faligndata(dd0, dd1);
				dd0 = dd1;
			}

			if (i < width) {
				emask = vis_edge8(dpp, dend);
				sd10 = vis_ld_d64_nf(spp1); spp1++;
				sd20 = vis_ld_d64_nf(spp2); spp2++;
				MLIB_V_IMAGEMULSHIFT_U8(sd10, sd20, dd1);
				dd = vis_faligndata(dd0, dd1);
				vis_pst_8(dd, dpp, emask);
			}

			sp1 = sl1 += stride1;
			sp2 = sl2 += stride2;
			dp = dl += strided;
		}
	} else {

		for (j = 0; j < height; j++) {

/* prepare the destination addresses */
			dpp = (mlib_d64 *)vis_alignaddr(dp, 0);
			i = (mlib_u8 *)dpp - dp;

			dend = dp + width - 1;
/* generate edge mask for the start point */
			emask = vis_edge8(dp, dend);

			if (emask != 0xff) {
				spp1 = (mlib_d64 *)vis_alignaddr(sp1, i);
				sd10 = vis_faligndata(vis_ld_d64_nf(spp1),
					vis_ld_d64_nf(spp1 + 1));
				spp2 = (mlib_d64 *)vis_alignaddr(sp2, i);
				sd20 = vis_faligndata(vis_ld_d64_nf(spp2),
					vis_ld_d64_nf(spp2 + 1));
				MLIB_V_IMAGEMULSHIFT_U8(sd10, sd20, dd);
				vis_pst_8(dd, dpp++, emask);
				i += 8;
			}

/* copy src1 to dst */
			spp1 = (mlib_d64 *)vis_alignaddr(sp1, i);
			sd11 = vis_ld_d64_nf(spp1);
			tmp_ptr = dpp;

#pragma pipeloop(0)
			for (k = i; k <= (width - 8); k += 8) {
				sd10 = sd11;
				sd11 = vis_ld_d64_nf(spp1 + 1);
				(*tmp_ptr++) = vis_faligndata(sd10, sd11);
				spp1++;
			}

			sd11 = vis_faligndata(sd11,
				vis_ld_d64_nf(spp1 + 1));

			spp2 = (mlib_d64 *)vis_alignaddr(sp2, i);
			sd20 = vis_ld_d64_nf(spp2);
			tmp_ptr = dpp;

#pragma pipeloop(0)
			for (; i <= width - 8; i += 8) {
				sd10 = (*tmp_ptr++);
				sd21 = vis_ld_d64_nf(spp2 + 1);
				sd20 = vis_faligndata(sd20, sd21);
				MLIB_V_IMAGEMULSHIFT_U8(sd10, sd20, dd);
				(*dpp++) = dd;
				sd20 = sd21;
				spp2++;
			}

			if (i < width) {
				emask = vis_edge8(dpp, dend);
				sd20 = vis_faligndata(sd20,
					vis_ld_d64_nf(spp2 + 1));
				MLIB_V_IMAGEMULSHIFT_U8(sd11, sd20, dd);
				vis_pst_8(dd, dpp, emask);
			}

			sp1 = sl1 += stride1;
			sp2 = sl2 += stride2;
			dp = dl += strided;
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageMulShift123_U8(
    mlib_u8 *sp1,
    mlib_s32 stride1,
    mlib_u8 *sp2,
    mlib_s32 stride2,
    mlib_u8 *dp,
    mlib_s32 strided,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 shift)
{
/* pointers for line of source1 */
	mlib_u8 *sl1;

/* pointers for line of source2 */
	mlib_u8 *sl2;

/* pointers for line of dst */
	mlib_u8 *dl;
	mlib_s32 offdst, offsrc1, offsrc2, emask;
	mlib_d64 *dpp, *spp2, *spp1, *tmp_ptr;
	mlib_d64 dd, dd0, dd1, sd10, sd11, sd20, sd21;
	mlib_d64 sd2h, sd2l, rdh, rdl;
	mlib_u8 *dend;
	mlib_f32 f_2pow14 = vis_to_float(0x4000);
	mlib_s32 i, j, k;

	if (width == stride1 && width == stride2 && width == strided) {
		width *= height;
		height = 1;
	}

/* initialize GSR scale factor */
	vis_write_gsr(((8 - shift) & 0xF) << 3);

	sl1 = sp1;
	sl2 = sp2;
	dl = dp;

	offdst = ((mlib_addr)dp) & 7;
	offsrc1 = ((mlib_addr)sp1) & 7;
	offsrc2 = ((mlib_addr)sp2) & 7;

	if ((offdst == offsrc1) && (offdst == offsrc2) &&
	    ((strided & 7) == (stride1 & 7)) &&
	    ((strided & 7) == (stride2 & 7))) {

		for (j = 0; j < height; j++) {

/* prepare the destination addresses */
			dpp = (mlib_d64 *)vis_alignaddr(dp, 0);
			i = (mlib_u8 *)dpp - dp;

/* prepare the source addresses */
			spp1 = (mlib_d64 *)vis_alignaddr(sp1, 0);
			spp2 = (mlib_d64 *)vis_alignaddr(sp2, 0);

			dend = dp + width - 1;
/* generate edge mask for the start point */
			emask = vis_edge8(dp, dend);

			if (emask != 0xff) {
				sd10 = (*spp1++);
				sd20 = (*spp2++);
				MLIB_V_IMAGEMULSHIFT123_U8(sd10, sd20, dd);
				vis_pst_8(dd, dpp++, emask);
				i += 8;
			}
#pragma pipeloop(0)
			for (; i <= width - 8; i += 8) {
				sd10 = (*spp1++);
				sd20 = (*spp2++);
				MLIB_V_IMAGEMULSHIFT123_U8(sd10, sd20, dd);
				(*dpp++) = dd;
			}

			if (i < width) {
				emask = vis_edge8(dpp, dend);
				sd10 = (*spp1++);
				sd20 = (*spp2++);
				MLIB_V_IMAGEMULSHIFT123_U8(sd10, sd20, dd);
				vis_pst_8(dd, dpp, emask);
			}

			sp1 = sl1 += stride1;
			sp2 = sl2 += stride2;
			dp = dl += strided;
		}
	} else if ((offdst == offsrc1) && ((strided & 7) == (stride1 & 7))) {

		for (j = 0; j < height; j++) {

/* prepare the destination addresses */
			dpp = (mlib_d64 *)vis_alignaddr(dp, 0);
			i = (mlib_u8 *)dpp - dp;

/* prepare the source addresses */
			spp1 = (mlib_d64 *)vis_alignaddr(sp1, 0);
			spp2 = (mlib_d64 *)vis_alignaddr(sp2, i);

			dend = dp + width - 1;
/* generate edge mask for the start point */
			emask = vis_edge8(dp, dend);

			sd20 = vis_ld_d64_nf(spp2);

			if (emask != 0xff) {
				sd10 = (*spp1++);
				sd21 = vis_ld_d64_nf(spp2 + 1);
				sd20 = vis_faligndata(sd20, sd21);
				MLIB_V_IMAGEMULSHIFT123_U8(sd10, sd20, dd);
				vis_pst_8(dd, dpp++, emask);
				sd20 = sd21;
				spp2++;
				i += 8;
			}
#pragma pipeloop(0)
			for (; i <= width - 8; i += 8) {
				sd10 = (*spp1++);
				sd21 = vis_ld_d64_nf(spp2 + 1);
				sd20 = vis_faligndata(sd20, sd21);
				MLIB_V_IMAGEMULSHIFT123_U8(sd10, sd20, dd);
				(*dpp++) = dd;
				sd20 = sd21;
				spp2++;
			}

			if (i < width) {
				emask = vis_edge8(dpp, dend);
				sd10 = (*spp1++);
				sd20 = vis_faligndata(sd20,
					vis_ld_d64_nf(spp2 + 1));
				MLIB_V_IMAGEMULSHIFT123_U8(sd10, sd20, dd);
				vis_pst_8(dd, dpp, emask);
			}

			sp1 = sl1 += stride1;
			sp2 = sl2 += stride2;
			dp = dl += strided;
		}
	} else if ((offdst == offsrc2) && ((strided & 7) == (stride2 & 7))) {

		for (j = 0; j < height; j++) {

/* prepare the destination addresses */
			dpp = (mlib_d64 *)vis_alignaddr(dp, 0);
			i = (mlib_u8 *)dpp - dp;

/* prepare the source addresses */
			spp2 = (mlib_d64 *)vis_alignaddr(sp2, 0);
			spp1 = (mlib_d64 *)vis_alignaddr(sp1, i);

			dend = dp + width - 1;
/* generate edge mask for the start point */
			emask = vis_edge8(dp, dend);

			sd10 = vis_ld_d64_nf(spp1);

			if (emask != 0xff) {
				sd20 = (*spp2++);
				sd11 = vis_ld_d64_nf(spp1 + 1);
				sd10 = vis_faligndata(sd10, sd11);
				MLIB_V_IMAGEMULSHIFT123_U8(sd10, sd20, dd);
				vis_pst_8(dd, dpp++, emask);
				sd10 = sd11;
				spp1++;
				i += 8;
			}
#pragma pipeloop(0)
			for (; i <= width - 8; i += 8) {
				sd20 = (*spp2++);
				sd11 = vis_ld_d64_nf(spp1 + 1);
				sd10 = vis_faligndata(sd10, sd11);
				MLIB_V_IMAGEMULSHIFT123_U8(sd10, sd20, dd);
				(*dpp++) = dd;
				sd10 = sd11;
				spp1++;
			}

			if (i < width) {
				emask = vis_edge8(dpp, dend);
				sd20 = (*spp2++);
				sd10 = vis_faligndata(sd10,
					vis_ld_d64_nf(spp1 + 1));
				MLIB_V_IMAGEMULSHIFT123_U8(sd10, sd20, dd);
				vis_pst_8(dd, dpp, emask);
			}

			sp1 = sl1 += stride1;
			sp2 = sl2 += stride2;
			dp = dl += strided;
		}
	} else if ((offsrc1 == offsrc2) && ((stride1 & 7) == (stride2 & 7))) {

		for (j = 0; j < height; j++) {

/* prepare the source addresses */
			dpp = (mlib_d64 *)vis_alignaddr(dp, 0);
			i = (mlib_u8 *)dpp - dp;

/* prepare the destination addresses */
			spp1 = (mlib_d64 *)vis_alignaddr(sp1, i);
			spp2 = (mlib_d64 *)vis_alignaddr(sp2, i);

			dend = dp + width - 1;
/* generate edge mask for the start point */
			emask = vis_edge8(dp, dend);

			sd10 = vis_ld_d64_nf(spp1); spp1++;
			sd20 = vis_ld_d64_nf(spp2); spp2++;
			MLIB_V_IMAGEMULSHIFT123_U8(sd10, sd20, dd0);

			if (emask != 0xff) {
				sd10 = vis_ld_d64_nf(spp1); spp1++;
				sd20 = vis_ld_d64_nf(spp2); spp2++;
				MLIB_V_IMAGEMULSHIFT123_U8(sd10, sd20, dd1);
				dd = vis_faligndata(dd0, dd1);
				vis_pst_8(dd, dpp++, emask);
				dd0 = dd1;
				i += 8;
			}
#pragma pipeloop(0)
			for (; i <= width - 8; i += 8) {
				sd10 = vis_ld_d64_nf(spp1); spp1++;
				sd20 = vis_ld_d64_nf(spp2); spp2++;
				MLIB_V_IMAGEMULSHIFT123_U8(sd10, sd20, dd1);
				(*dpp++) = vis_faligndata(dd0, dd1);
				dd0 = dd1;
			}

			if (i < width) {
				emask = vis_edge8(dpp, dend);
				sd10 = vis_ld_d64_nf(spp1); spp1++;
				sd20 = vis_ld_d64_nf(spp2); spp2++;
				MLIB_V_IMAGEMULSHIFT123_U8(sd10, sd20, dd1);
				dd = vis_faligndata(dd0, dd1);
				vis_pst_8(dd, dpp, emask);
			}

			sp1 = sl1 += stride1;
			sp2 = sl2 += stride2;
			dp = dl += strided;
		}
	} else {

		for (j = 0; j < height; j++) {

/* prepare the destination addresses */
			dpp = (mlib_d64 *)vis_alignaddr(dp, 0);
			i = (mlib_u8 *)dpp - dp;

			dend = dp + width - 1;
/* generate edge mask for the start point */
			emask = vis_edge8(dp, dend);

			if (emask != 0xff) {
				spp1 = (mlib_d64 *)vis_alignaddr(sp1, i);
				sd10 = vis_faligndata(vis_ld_d64_nf(spp1),
					vis_ld_d64_nf(spp1 + 1));
				spp2 = (mlib_d64 *)vis_alignaddr(sp2, i);
				sd20 = vis_faligndata(vis_ld_d64_nf(spp2),
					vis_ld_d64_nf(spp2 + 1));
				MLIB_V_IMAGEMULSHIFT123_U8(sd10, sd20, dd);
				vis_pst_8(dd, dpp++, emask);
				i += 8;
			}

/* copy src1 to dst */
			spp1 = (mlib_d64 *)vis_alignaddr(sp1, i);
			sd11 = vis_ld_d64_nf(spp1);
			tmp_ptr = dpp;

#pragma pipeloop(0)
			for (k = i; k <= (width - 8); k += 8) {
				sd10 = sd11;
				sd11 = vis_ld_d64_nf(spp1 + 1);
				(*tmp_ptr++) = vis_faligndata(sd10, sd11);
				spp1++;
			}

			sd11 = vis_faligndata(sd11,
				vis_ld_d64_nf(spp1 + 1));

			spp2 = (mlib_d64 *)vis_alignaddr(sp2, i);
			sd20 = vis_ld_d64_nf(spp2);
			tmp_ptr = dpp;

#pragma pipeloop(0)
			for (; i <= width - 8; i += 8) {
				sd10 = (*tmp_ptr++);
				sd21 = vis_ld_d64_nf(spp2 + 1);
				sd20 = vis_faligndata(sd20, sd21);
				MLIB_V_IMAGEMULSHIFT123_U8(sd10, sd20, dd);
				(*dpp++) = dd;
				sd20 = sd21;
				spp2++;
			}

			if (i < width) {
				emask = vis_edge8(dpp, dend);
				sd20 = vis_faligndata(sd20,
					vis_ld_d64_nf(spp2 + 1));
				MLIB_V_IMAGEMULSHIFT123_U8(sd11, sd20, dd);
				vis_pst_8(dd, dpp, emask);
			}

			sp1 = sl1 += stride1;
			sp2 = sl2 += stride2;
			dp = dl += strided;
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageMulShift_S32(
    mlib_s32 *sp1,
    mlib_s32 stride1,
    mlib_s32 *sp2,
    mlib_s32 stride2,
    mlib_s32 *dp,
    mlib_s32 strided,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 shift)
{
	mlib_s32 i, j;
	mlib_d64 dscale;

	if (width == stride1 && width == stride2 && width == strided) {
		width *= height;
		height = 1;
	}

/* dscale = 2 ** (-shift) */
	SCALBN(dscale, -shift);

	for (j = 0; j < height; j++) {

#pragma pipeloop(0)
		for (i = 0; i < width; i++) {
			FLOAT2INT_CLAMP(dp[i],
			    (mlib_d64)sp1[i] * dscale * (mlib_d64)sp2[i]);
		}

		sp1 += stride1;
		sp2 += stride2;
		dp += strided;
	}
}

/* *********************************************************** */
