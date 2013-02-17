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
 * Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"@(#)mlib_v_ImageMulShiftFuncS16.c	9.3	07/11/05 SMI"

/*
 * VIS2 version of internal functions
 * for ImageMulShift (type S16 and U16).
 *
 *    void mlib_v_ImageMulShift_S16(mlib_s16 *sp1,
 *                                  mlib_s32 stride1,
 *                                  mlib_s16 *sp2,
 *                                  mlib_s32 stride2,
 *                                  mlib_s16 *dp,
 *                                  mlib_s32 strided,
 *                                  mlib_s32 width,
 *                                  mlib_s32 height,
 *                                  mlib_s32 shift);
 *
 *    void mlib_v_ImageMul_S16(mlib_s16 *sp1,
 *                             mlib_s32 stride1,
 *                             mlib_s16 *sp2,
 *                             mlib_s32 stride2,
 *                             mlib_s16 *dp,
 *                             mlib_s32 strided,
 *                             mlib_s32 width,
 *                             mlib_s32 height);
 *
 *    void mlib_v_ImageMulShift_U16(mlib_s16 *sp1,
 *                                  mlib_s32 stride1,
 *                                  mlib_s16 *sp2,
 *                                  mlib_s32 stride2,
 *                                  mlib_s16 *dp,
 *                                  mlib_s32 strided,
 *                                  mlib_s32 width,
 *                                  mlib_s32 height,
 *                                  mlib_s32 shift);
 *
 *    void mlib_v_ImageMulSmallShift_U16(mlib_s16 *sp1,
 *                                       mlib_s32 stride1,
 *                                       mlib_s16 *sp2,
 *                                       mlib_s32 stride2,
 *                                       mlib_s16 *dp,
 *                                       mlib_s32 strided,
 *                                       mlib_s32 width,
 *                                       mlib_s32 height,
 *                                       mlib_s32 shift);
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_v_ImageMulShift.h>

/* *********************************************************** */

#define	MLIB_V_IMAGEMULSHIFT_S16(sd1, sd2, dd)                        \
	rdhh = vis_fmuld8sux16(vis_read_hi(sd1), vis_read_hi(sd2));   \
	rdhl = vis_fmuld8ulx16(vis_read_hi(sd1), vis_read_hi(sd2));   \
	rdh = vis_fpadd32(rdhh, rdhl);                                \
	rdlh = vis_fmuld8sux16(vis_read_lo(sd1), vis_read_lo(sd2));   \
	rdll = vis_fmuld8ulx16(vis_read_lo(sd1), vis_read_lo(sd2));   \
	rdl = vis_fpadd32(rdlh, rdll);                                \
	dd = vis_fpackfix_pair(rdh, rdl)

/* *********************************************************** */

void
mlib_v_ImageMulShift_S16(
    mlib_s16 *sp1,
    mlib_s32 stride1,
    mlib_s16 *sp2,
    mlib_s32 stride2,
    mlib_s16 *dp,
    mlib_s32 strided,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 shift)
{
/* pointers for line of source1 */
	mlib_s16 *sl1;

/* pointers for line of source2 */
	mlib_s16 *sl2;

/* pointers for line of dst */
	mlib_s16 *dl;
	mlib_s32 offdst, offsrc1, offsrc2, emask;
	mlib_d64 *dpp, *spp2, *spp1, *tmp_ptr;
	mlib_d64 dd, dd0, dd1, sd10, sd11, sd20, sd21;
	mlib_s16 *dend;
	mlib_d64 rdhh, rdhl;
	mlib_d64 rdlh, rdll;
	mlib_d64 rdh, rdl;
	mlib_s32 i, j, k;

	if (width == stride1 && width == stride2 && width == strided) {
		width *= height;
		height = 1;
	}

/* initialize GSR scale factor */
	vis_write_gsr(((16 - shift) & 0x1f) << 3);

	sl1 = sp1;
	sl2 = sp2;
	dl = dp;

	offdst = ((mlib_addr)dp) & 7;
	offsrc1 = ((mlib_addr)sp1) & 7;
	offsrc2 = ((mlib_addr)sp2) & 7;

	if ((offdst == offsrc1) && (offdst == offsrc2) &&
	    (((strided ^ stride1) & 3) == 0) &&
	    (((strided ^ stride2) & 3) == 0)) {

		for (j = 0; j < height; j++) {

/* prepare the destination addresses */
			dpp = (mlib_d64 *)vis_alignaddr(dp, 0);
			i = (mlib_s16 *)dpp - dp;

/* prepare the source addresses */
			spp1 = (mlib_d64 *)vis_alignaddr(sp1, 0);
			spp2 = (mlib_d64 *)vis_alignaddr(sp2, 0);

			dend = dp + width - 1;
/* generate edge mask for the start point */
			emask = vis_edge16(dp, dend);

			if (emask != 0xf) {
				sd10 = (*spp1++);
				sd20 = (*spp2++);
				MLIB_V_IMAGEMULSHIFT_S16(sd10, sd20, dd);
				vis_pst_16(dd, dpp++, emask);
				i += 4;
			}
#pragma pipeloop(0)
			for (; i <= width - 4; i += 4) {
				sd10 = (*spp1++);
				sd20 = (*spp2++);
				MLIB_V_IMAGEMULSHIFT_S16(sd10, sd20, dd);
				(*dpp++) = dd;
			}

			if (i < width) {
				emask = vis_edge16(dpp, dend);
				sd10 = (*spp1++);
				sd20 = (*spp2++);
				MLIB_V_IMAGEMULSHIFT_S16(sd10, sd20, dd);
				vis_pst_16(dd, dpp, emask);
			}

			sp1 = sl1 += stride1;
			sp2 = sl2 += stride2;
			dp = dl += strided;
		}
	} else if ((offdst == offsrc1) && (((strided ^ stride1) & 3) == 0)) {

		for (j = 0; j < height; j++) {

/* prepare the destination addresses */
			dpp = (mlib_d64 *)vis_alignaddr(dp, 0);
			i = (mlib_s16 *)dpp - dp;

/* prepare the source addresses */
			spp1 = (mlib_d64 *)vis_alignaddr(sp1, 0);
			spp2 = (mlib_d64 *)vis_alignaddr(sp2, 2 * i);

			dend = dp + width - 1;
/* generate edge mask for the start point */
			emask = vis_edge16(dp, dend);

			sd20 = spp2[0];

			if (emask != 0xf) {
				sd10 = (*spp1++);
				sd21 = spp2[1];
				sd20 = vis_faligndata(sd20, sd21);
				MLIB_V_IMAGEMULSHIFT_S16(sd10, sd20, dd);
				vis_pst_16(dd, dpp++, emask);
				sd20 = sd21;
				spp2++;
				i += 4;
			}
#pragma pipeloop(0)
			for (; i <= width - 4; i += 4) {
				sd10 = (*spp1++);
				sd21 = spp2[1];
				sd20 = vis_faligndata(sd20, sd21);
				MLIB_V_IMAGEMULSHIFT_S16(sd10, sd20, dd);
				(*dpp++) = dd;
				sd20 = sd21;
				spp2++;
			}

			if (i < width) {
				emask = vis_edge16(dpp, dend);
				sd10 = (*spp1++);
				sd20 = vis_faligndata(sd20, spp2[1]);
				MLIB_V_IMAGEMULSHIFT_S16(sd10, sd20, dd);
				vis_pst_16(dd, dpp, emask);
			}

			sp1 = sl1 += stride1;
			sp2 = sl2 += stride2;
			dp = dl += strided;
		}
	} else if ((offdst == offsrc2) && (((strided ^ stride2) & 3) == 0)) {

		for (j = 0; j < height; j++) {

/* prepare the destination addresses */
			dpp = (mlib_d64 *)vis_alignaddr(dp, 0);
			i = (mlib_s16 *)dpp - dp;

/* prepare the source addresses */
			spp2 = (mlib_d64 *)vis_alignaddr(sp2, 0);
			spp1 = (mlib_d64 *)vis_alignaddr(sp1, 2 * i);

			dend = dp + width - 1;
/* generate edge mask for the start point */
			emask = vis_edge16(dp, dend);

			sd10 = spp1[0];

			if (emask != 0xf) {
				sd20 = (*spp2++);
				sd11 = spp1[1];
				sd10 = vis_faligndata(sd10, sd11);
				MLIB_V_IMAGEMULSHIFT_S16(sd10, sd20, dd);
				vis_pst_16(dd, dpp++, emask);
				sd10 = sd11;
				spp1++;
				i += 4;
			}
#pragma pipeloop(0)
			for (; i <= width - 4; i += 4) {
				sd20 = (*spp2++);
				sd11 = spp1[1];
				sd10 = vis_faligndata(sd10, sd11);
				MLIB_V_IMAGEMULSHIFT_S16(sd10, sd20, dd);
				(*dpp++) = dd;
				sd10 = sd11;
				spp1++;
			}

			if (i < width) {
				emask = vis_edge16(dpp, dend);
				sd20 = (*spp2++);
				sd10 = vis_faligndata(sd10, spp1[1]);
				MLIB_V_IMAGEMULSHIFT_S16(sd10, sd20, dd);
				vis_pst_16(dd, dpp, emask);
			}

			sp1 = sl1 += stride1;
			sp2 = sl2 += stride2;
			dp = dl += strided;
		}
	} else if ((offsrc1 == offsrc2) && (((stride1 ^ stride2) & 3) == 0)) {

		for (j = 0; j < height; j++) {

/* prepare the source addresses */
			dpp = (mlib_d64 *)vis_alignaddr(dp, 0);
			i = (mlib_s16 *)dpp - dp;

/* prepare the destination addresses */
			spp1 = (mlib_d64 *)vis_alignaddr(sp1, 2 * i);
			spp2 = (mlib_d64 *)vis_alignaddr(sp2, 2 * i);

			dend = dp + width - 1;
/* generate edge mask for the start point */
			emask = vis_edge16(dp, dend);

			sd10 = (*spp1++);
			sd20 = (*spp2++);
			MLIB_V_IMAGEMULSHIFT_S16(sd10, sd20, dd0);

			if (emask != 0xf) {
				sd10 = (*spp1++);
				sd20 = (*spp2++);
				MLIB_V_IMAGEMULSHIFT_S16(sd10, sd20, dd1);
				dd = vis_faligndata(dd0, dd1);
				vis_pst_16(dd, dpp++, emask);
				dd0 = dd1;
				i += 4;
			}
#pragma pipeloop(0)
			for (; i <= width - 4; i += 4) {
				sd10 = (*spp1++);
				sd20 = (*spp2++);
				MLIB_V_IMAGEMULSHIFT_S16(sd10, sd20, dd1);
				(*dpp++) = vis_faligndata(dd0, dd1);
				dd0 = dd1;
			}

			if (i < width) {
				emask = vis_edge16(dpp, dend);
				sd10 = (*spp1++);
				sd20 = (*spp2++);
				MLIB_V_IMAGEMULSHIFT_S16(sd10, sd20, dd1);
				dd = vis_faligndata(dd0, dd1);
				vis_pst_16(dd, dpp, emask);
			}

			sp1 = sl1 += stride1;
			sp2 = sl2 += stride2;
			dp = dl += strided;
		}
	} else {

		for (j = 0; j < height; j++) {

/* prepare the destination addresses */
			dpp = (mlib_d64 *)vis_alignaddr(dp, 0);
			i = (mlib_s16 *)dpp - dp;

			dend = dp + width - 1;
/* generate edge mask for the start point */
			emask = vis_edge16(dp, dend);

			if (emask != 0xf) {
				spp1 = (mlib_d64 *)vis_alignaddr(sp1, 2 * i);
				sd10 = vis_faligndata(spp1[0], spp1[1]);
				spp2 = (mlib_d64 *)vis_alignaddr(sp2, 2 * i);
				sd20 = vis_faligndata(spp2[0], spp2[1]);
				MLIB_V_IMAGEMULSHIFT_S16(sd10, sd20, dd);
				vis_pst_16(dd, dpp++, emask);
				i += 4;
			}

/* copy src1 to dst */
			spp1 = (mlib_d64 *)vis_alignaddr(sp1, 2 * i);
			sd11 = spp1[0];
			tmp_ptr = dpp;

#pragma pipeloop(0)
			for (k = i; k <= (width - 4); k += 4) {
				sd10 = sd11;
				sd11 = spp1[1];
				(*tmp_ptr++) = vis_faligndata(sd10, sd11);
				spp1++;
			}

			sd11 = vis_faligndata(sd11, spp1[1]);

			spp2 = (mlib_d64 *)vis_alignaddr(sp2, 2 * i);
			sd20 = spp2[0];
			tmp_ptr = dpp;

#pragma pipeloop(0)
			for (; i <= width - 4; i += 4) {
				sd10 = (*tmp_ptr++);
				sd21 = spp2[1];
				sd20 = vis_faligndata(sd20, sd21);
				MLIB_V_IMAGEMULSHIFT_S16(sd10, sd20, dd);
				(*dpp++) = dd;
				sd20 = sd21;
				spp2++;
			}

			if (i < width) {
				emask = vis_edge16(dpp, dend);
				sd20 = vis_faligndata(sd20, spp2[1]);
				MLIB_V_IMAGEMULSHIFT_S16(sd11, sd20, dd);
				vis_pst_16(dd, dpp, emask);
			}

			sp1 = sl1 += stride1;
			sp2 = sl2 += stride2;
			dp = dl += strided;
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageMul_S16(
    mlib_s16 *sp1,
    mlib_s32 stride1,
    mlib_s16 *sp2,
    mlib_s32 stride2,
    mlib_s16 *dp,
    mlib_s32 strided,
    mlib_s32 width,
    mlib_s32 height)
{
	mlib_v_ImageMulShift_S16(sp1, stride1,
	    sp2, stride2, dp, strided, width, height, 0);
}

/* *********************************************************** */

#if 0
/*
 * This macro realizes more accurate u16 multiplication ( then uncommented )
 * but slower one.
 *
 * #define MLIB_V_IMAGEMULSHIFT_U16(ud1, ud2, dd)                          \
 *   sd1  = vis_fxor((ud1), offset);                                      \
 *   sd2  = vis_fxor((ud2), offset);                                      \
 *   rdhh = vis_fmuld8sux16(vis_read_hi(sd1), vis_read_hi(sd2));          \
 *   rdhl = vis_fmuld8ulx16(vis_read_hi(sd1), vis_read_hi(sd2));          \
 *   rdh  = vis_fpadd32(rdhh, rdhl);                                      \
 *   rdh_0 = (mlib_d64)(((mlib_s32*)(&rdh))[0]); rdh_0 *= .5;             \
 *   rdh_1 = (mlib_d64)(((mlib_s32*)(&rdh))[1]); rdh_1 *= .5;             \
 *   rdh = vis_to_double( (mlib_s32)(rdh_0), (mlib_s32)(rdh_1) );         \
 *   sd1ad = vis_fmuld8sux16(vis_read_hi(half_offset), vis_read_hi(sd1)); \
 *   sd2ad = vis_fmuld8sux16(vis_read_hi(half_offset), vis_read_hi(sd2)); \
 *   rdh  = vis_fpadd32(rdh, sd1ad);                                      \
 *   rdh  = vis_fpadd32(rdh, sd2ad);                                      \
 *   rdh  = vis_fpadd32(rdh, const_offset);                               \
 *   rdlh = vis_fmuld8sux16(vis_read_lo(sd1), vis_read_lo(sd2));          \
 *   rdll = vis_fmuld8ulx16(vis_read_lo(sd1), vis_read_lo(sd2));          \
 *   rdl  = vis_fpadd32(rdlh, rdll);                                      \
 *   rdl_0 = (mlib_d64)(((mlib_s32*)(&rdl))[0]); rdl_0 *= .5;             \
 *   rdl_1 = (mlib_d64)(((mlib_s32*)(&rdl))[1]); rdl_1 *= .5;             \
 *   rdl = vis_to_double( (mlib_s32)(rdl_0), (mlib_s32)(rdl_1) );         \
 *   sd1ad = vis_fmuld8sux16(vis_read_hi(half_offset), vis_read_lo(sd1)); \
 *   sd2ad = vis_fmuld8sux16(vis_read_hi(half_offset), vis_read_lo(sd2)); \
 *   rdl  = vis_fpadd32(rdl, sd1ad);                                      \
 *   rdl  = vis_fpadd32(rdl, sd2ad);                                      \
 *   rdl  = vis_fpadd32(rdl, const_offset);                               \
 *   dd   = vis_fpackfix_pair(rdh, rdl);                                  \
 *   dd   = vis_fxor((dd), offset);
 */

#endif

/* *********************************************************** */

#define	MLIB_V_IMAGEMULSHIFT_U16(ud1, ud2, dd)                         \
	sd1 = vis_fxor((ud1), offset);                                 \
	sd2 = vis_fxor((ud2), offset);                                 \
	sd2 = vis_fand(sd2, mask);                                     \
	sd2 = vis_fmul8x16(fmin, sd2);                                 \
	rdhh = vis_fmuld8sux16(vis_read_hi(sd1), vis_read_hi(sd2));    \
	rdhl = vis_fmuld8ulx16(vis_read_hi(sd1), vis_read_hi(sd2));    \
	rdh = vis_fpadd32(rdhh, rdhl);                                 \
	sd1ad =                                                        \
	vis_fmuld8sux16(vis_read_hi(half_offset), vis_read_hi(sd1));   \
	sd2ad =                                                        \
	vis_fmuld8sux16(vis_read_lo(half_offset), vis_read_hi(sd2));   \
	rdh = vis_fpadd32(rdh, sd1ad);                                 \
	rdh = vis_fpsub32(rdh, sd2ad);                                 \
	rdh = vis_fpadd32(rdh, const_offset);                          \
	rdlh = vis_fmuld8sux16(vis_read_lo(sd1), vis_read_lo(sd2));    \
	rdll = vis_fmuld8ulx16(vis_read_lo(sd1), vis_read_lo(sd2));    \
	rdl = vis_fpadd32(rdlh, rdll);                                 \
	sd1ad =                                                        \
	vis_fmuld8sux16(vis_read_hi(half_offset), vis_read_lo(sd1));   \
	sd2ad =                                                        \
	vis_fmuld8sux16(vis_read_lo(half_offset), vis_read_lo(sd2));   \
	rdl = vis_fpadd32(rdl, sd1ad);                                 \
	rdl = vis_fpsub32(rdl, sd2ad);                                 \
	rdl = vis_fpadd32(rdl, const_offset);                          \
	dd = vis_fpackfix_pair(rdh, rdl);                              \
	dd = vis_fxor((dd), offset)

/* *********************************************************** */

void
mlib_v_ImageMulShift_U16(
    mlib_s16 *sp1,
    mlib_s32 stride1,
    mlib_s16 *sp2,
    mlib_s32 stride2,
    mlib_s16 *dp,
    mlib_s32 strided,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 shift)
{
/* pointers for line of source1 */
	mlib_s16 *sl1;

/* pointers for line of source2 */
	mlib_s16 *sl2;

/* pointers for line of dst */
	mlib_s16 *dl;
	mlib_s32 offdst, offsrc1, offsrc2, emask;
	mlib_d64 *dpp, *spp2, *spp1, *tmp_ptr;
	mlib_d64 dd, dd0, dd1, sd10, sd11, sd20, sd21;
	mlib_s16 *dend;
	mlib_d64 rdhh, rdhl;
	mlib_d64 rdlh, rdll;
	mlib_d64 rdh, rdl;
	mlib_s32 i, j, k;

	mlib_d64 sd1, sd2, sd1ad, sd2ad;
	mlib_d64 mask = vis_to_double_dup(0xfffefffe);
	mlib_f32 fmin = vis_to_float(0x80808080);
	mlib_d64 offset = vis_to_double_dup(0x80008000);
	mlib_d64 half_offset = vis_to_double(0x40004000, 0x80008000);
	mlib_d64 const_offset = vis_to_double_dup(0x20000000);

	const_offset =
	    vis_fpsub32(const_offset,
	    vis_to_double_dup(0x40000000 >> (16 - shift)));

	if (width == stride1 && width == stride2 && width == strided) {
		width *= height;
		height = 1;
	}

/* initialize GSR scale factor */
	vis_write_gsr(((16 - (shift - 1)) & 0x1f) << 3);

	sl1 = sp1;
	sl2 = sp2;
	dl = dp;

	offdst = ((mlib_addr)dp) & 7;
	offsrc1 = ((mlib_addr)sp1) & 7;
	offsrc2 = ((mlib_addr)sp2) & 7;

	if ((offdst == offsrc1) && (offdst == offsrc2) &&
	    (((strided ^ stride1) & 3) == 0) &&
	    (((strided ^ stride2) & 3) == 0)) {

		for (j = 0; j < height; j++) {

/* prepare the destination addresses */
			dpp = (mlib_d64 *)vis_alignaddr(dp, 0);
			i = (mlib_s16 *)dpp - dp;

/* prepare the source addresses */
			spp1 = (mlib_d64 *)vis_alignaddr(sp1, 0);
			spp2 = (mlib_d64 *)vis_alignaddr(sp2, 0);

			dend = dp + width - 1;
/* generate edge mask for the start point */
			emask = vis_edge16(dp, dend);

			if (emask != 0xf) {
				sd10 = (*spp1++);
				sd20 = (*spp2++);
				MLIB_V_IMAGEMULSHIFT_U16(sd10, sd20, dd);
				vis_pst_16(dd, dpp++, emask);
				i += 4;
			}
#pragma pipeloop(0)
			for (; i <= width - 4; i += 4) {
				sd10 = (*spp1++);
				sd20 = (*spp2++);
				MLIB_V_IMAGEMULSHIFT_U16(sd10, sd20, dd);
				(*dpp++) = dd;
			}

			if (i < width) {
				emask = vis_edge16(dpp, dend);
				sd10 = (*spp1++);
				sd20 = (*spp2++);
				MLIB_V_IMAGEMULSHIFT_U16(sd10, sd20, dd);
				vis_pst_16(dd, dpp, emask);
			}

			sp1 = sl1 += stride1;
			sp2 = sl2 += stride2;
			dp = dl += strided;
		}
	} else if ((offdst == offsrc1) && (((strided ^ stride1) & 3) == 0)) {

		for (j = 0; j < height; j++) {

/* prepare the destination addresses */
			dpp = (mlib_d64 *)vis_alignaddr(dp, 0);
			i = (mlib_s16 *)dpp - dp;

/* prepare the source addresses */
			spp1 = (mlib_d64 *)vis_alignaddr(sp1, 0);
			spp2 = (mlib_d64 *)vis_alignaddr(sp2, 2 * i);

			dend = dp + width - 1;
/* generate edge mask for the start point */
			emask = vis_edge16(dp, dend);

			sd20 = spp2[0];

			if (emask != 0xf) {
				sd10 = (*spp1++);
				sd21 = spp2[1];
				sd20 = vis_faligndata(sd20, sd21);
				MLIB_V_IMAGEMULSHIFT_U16(sd10, sd20, dd);
				vis_pst_16(dd, dpp++, emask);
				sd20 = sd21;
				spp2++;
				i += 4;
			}
#pragma pipeloop(0)
			for (; i <= width - 4; i += 4) {
				sd10 = (*spp1++);
				sd21 = spp2[1];
				sd20 = vis_faligndata(sd20, sd21);
				MLIB_V_IMAGEMULSHIFT_U16(sd10, sd20, dd);
				(*dpp++) = dd;
				sd20 = sd21;
				spp2++;
			}

			if (i < width) {
				emask = vis_edge16(dpp, dend);
				sd10 = (*spp1++);
				sd20 = vis_faligndata(sd20, spp2[1]);
				MLIB_V_IMAGEMULSHIFT_U16(sd10, sd20, dd);
				vis_pst_16(dd, dpp, emask);
			}

			sp1 = sl1 += stride1;
			sp2 = sl2 += stride2;
			dp = dl += strided;
		}
	} else if ((offdst == offsrc2) && (((strided ^ stride2) & 3) == 0)) {

		for (j = 0; j < height; j++) {

/* prepare the destination addresses */
			dpp = (mlib_d64 *)vis_alignaddr(dp, 0);
			i = (mlib_s16 *)dpp - dp;

/* prepare the source addresses */
			spp2 = (mlib_d64 *)vis_alignaddr(sp2, 0);
			spp1 = (mlib_d64 *)vis_alignaddr(sp1, 2 * i);

			dend = dp + width - 1;
/* generate edge mask for the start point */
			emask = vis_edge16(dp, dend);

			sd10 = spp1[0];

			if (emask != 0xf) {
				sd20 = (*spp2++);
				sd11 = spp1[1];
				sd10 = vis_faligndata(sd10, sd11);
				MLIB_V_IMAGEMULSHIFT_U16(sd10, sd20, dd);
				vis_pst_16(dd, dpp++, emask);
				sd10 = sd11;
				spp1++;
				i += 4;
			}
#pragma pipeloop(0)
			for (; i <= width - 4; i += 4) {
				sd20 = (*spp2++);
				sd11 = spp1[1];
				sd10 = vis_faligndata(sd10, sd11);
				MLIB_V_IMAGEMULSHIFT_U16(sd10, sd20, dd);
				(*dpp++) = dd;
				sd10 = sd11;
				spp1++;
			}

			if (i < width) {
				emask = vis_edge16(dpp, dend);
				sd20 = (*spp2++);
				sd10 = vis_faligndata(sd10, spp1[1]);
				MLIB_V_IMAGEMULSHIFT_U16(sd10, sd20, dd);
				vis_pst_16(dd, dpp, emask);
			}

			sp1 = sl1 += stride1;
			sp2 = sl2 += stride2;
			dp = dl += strided;
		}
	} else if ((offsrc1 == offsrc2) && (((stride1 ^ stride2) & 3) == 0)) {

		for (j = 0; j < height; j++) {

/* prepare the source addresses */
			dpp = (mlib_d64 *)vis_alignaddr(dp, 0);
			i = (mlib_s16 *)dpp - dp;

/* prepare the destination addresses */
			spp1 = (mlib_d64 *)vis_alignaddr(sp1, 2 * i);
			spp2 = (mlib_d64 *)vis_alignaddr(sp2, 2 * i);

			dend = dp + width - 1;
/* generate edge mask for the start point */
			emask = vis_edge16(dp, dend);

			sd10 = (*spp1++);
			sd20 = (*spp2++);
			MLIB_V_IMAGEMULSHIFT_U16(sd10, sd20, dd0);

			if (emask != 0xf) {
				sd10 = (*spp1++);
				sd20 = (*spp2++);
				MLIB_V_IMAGEMULSHIFT_U16(sd10, sd20, dd1);
				dd = vis_faligndata(dd0, dd1);
				vis_pst_16(dd, dpp++, emask);
				dd0 = dd1;
				i += 4;
			}
#pragma pipeloop(0)
			for (; i <= width - 4; i += 4) {
				sd10 = (*spp1++);
				sd20 = (*spp2++);
				MLIB_V_IMAGEMULSHIFT_U16(sd10, sd20, dd1);
				(*dpp++) = vis_faligndata(dd0, dd1);
				dd0 = dd1;
			}

			if (i < width) {
				emask = vis_edge16(dpp, dend);
				sd10 = (*spp1++);
				sd20 = (*spp2++);
				MLIB_V_IMAGEMULSHIFT_U16(sd10, sd20, dd1);
				dd = vis_faligndata(dd0, dd1);
				vis_pst_16(dd, dpp, emask);
			}

			sp1 = sl1 += stride1;
			sp2 = sl2 += stride2;
			dp = dl += strided;
		}
	} else {

		for (j = 0; j < height; j++) {

/* prepare the destination addresses */
			dpp = (mlib_d64 *)vis_alignaddr(dp, 0);
			i = (mlib_s16 *)dpp - dp;

			dend = dp + width - 1;
/* generate edge mask for the start point */
			emask = vis_edge16(dp, dend);

			if (emask != 0xf) {
				spp1 = (mlib_d64 *)vis_alignaddr(sp1, 2 * i);
				sd10 = vis_faligndata(spp1[0], spp1[1]);
				spp2 = (mlib_d64 *)vis_alignaddr(sp2, 2 * i);
				sd20 = vis_faligndata(spp2[0], spp2[1]);
				MLIB_V_IMAGEMULSHIFT_U16(sd10, sd20, dd);
				vis_pst_16(dd, dpp++, emask);
				i += 4;
			}

/* copy src1 to dst */
			spp1 = (mlib_d64 *)vis_alignaddr(sp1, 2 * i);
			sd11 = spp1[0];
			tmp_ptr = dpp;

#pragma pipeloop(0)
			for (k = i; k <= (width - 4); k += 4) {
				sd10 = sd11;
				sd11 = spp1[1];
				(*tmp_ptr++) = vis_faligndata(sd10, sd11);
				spp1++;
			}

			sd11 = vis_faligndata(sd11, spp1[1]);

			spp2 = (mlib_d64 *)vis_alignaddr(sp2, 2 * i);
			sd20 = spp2[0];
			tmp_ptr = dpp;

#pragma pipeloop(0)
			for (; i <= width - 4; i += 4) {
				sd10 = (*tmp_ptr++);
				sd21 = spp2[1];
				sd20 = vis_faligndata(sd20, sd21);
				MLIB_V_IMAGEMULSHIFT_U16(sd10, sd20, dd);
				(*dpp++) = dd;
				sd20 = sd21;
				spp2++;
			}

			if (i < width) {
				emask = vis_edge16(dpp, dend);
				sd20 = vis_faligndata(sd20, spp2[1]);
				MLIB_V_IMAGEMULSHIFT_U16(sd11, sd20, dd);
				vis_pst_16(dd, dpp, emask);
			}

			sp1 = sl1 += stride1;
			sp2 = sl2 += stride2;
			dp = dl += strided;
		}
	}
}

/* *********************************************************** */

#define	MLIB_V_IMAGEMULSHIFTONE_U16(ud1, ud2, dd)                      \
	sd1 = vis_fxor((ud1), offset);                                 \
	sd2 = vis_fxor((ud2), offset);                                 \
	rdhh = vis_fmuld8sux16(vis_read_hi(sd1), vis_read_hi(sd2));    \
	rdhl = vis_fmuld8ulx16(vis_read_hi(sd1), vis_read_hi(sd2));    \
	rdh = vis_fpadd32(rdhh, rdhl);                                 \
	rdh_0 = (mlib_d64)(((mlib_s32 *)(&rdh))[0]);                   \
	rdh_0 *= .5;                                                   \
	rdh_1 = (mlib_d64)(((mlib_s32 *)(&rdh))[1]);                   \
	rdh_1 *= .5;                                                   \
	rdh = vis_to_double((mlib_s32)(rdh_0), (mlib_s32)(rdh_1));     \
	sd1ad =                                                        \
	vis_fmuld8sux16(vis_read_hi(half_offset), vis_read_hi(sd1));   \
	sd2ad =                                                        \
	vis_fmuld8sux16(vis_read_hi(half_offset), vis_read_hi(sd2));   \
	rdh = vis_fpadd32(rdh, sd1ad);                                 \
	rdh = vis_fpadd32(rdh, sd2ad);                                 \
	rdh = vis_fpadd32(rdh, const_offset);                          \
	rdlh = vis_fmuld8sux16(vis_read_lo(sd1), vis_read_lo(sd2));    \
	rdll = vis_fmuld8ulx16(vis_read_lo(sd1), vis_read_lo(sd2));    \
	rdl = vis_fpadd32(rdlh, rdll);                                 \
	rdl_0 = (mlib_d64)(((mlib_s32 *)(&rdl))[0]);                   \
	rdl_0 *= .5;                                                   \
	rdl_1 = (mlib_d64)(((mlib_s32 *)(&rdl))[1]);                   \
	rdl_1 *= .5;                                                   \
	rdl = vis_to_double((mlib_s32)(rdl_0), (mlib_s32)(rdl_1));     \
	sd1ad =                                                        \
	vis_fmuld8sux16(vis_read_hi(half_offset), vis_read_lo(sd1));   \
	sd2ad =                                                        \
	vis_fmuld8sux16(vis_read_hi(half_offset), vis_read_lo(sd2));   \
	rdl = vis_fpadd32(rdl, sd1ad);                                 \
	rdl = vis_fpadd32(rdl, sd2ad);                                 \
	rdl = vis_fpadd32(rdl, const_offset);                          \
	dd = vis_fpackfix_pair(rdh, rdl);                              \
	dd = vis_fxor((dd), offset)

/* *********************************************************** */

void
mlib_v_ImageMulSmallShift_U16(
    mlib_s16 *sp1,
    mlib_s32 stride1,
    mlib_s16 *sp2,
    mlib_s32 stride2,
    mlib_s16 *dp,
    mlib_s32 strided,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 shift)
{
/* pointers for line of source1 */
	mlib_s16 *sl1;

/* pointers for line of source2 */
	mlib_s16 *sl2;

/* pointers for line of dst */
	mlib_s16 *dl;
	mlib_s32 offdst, offsrc1, offsrc2, emask;
	mlib_d64 *dpp, *spp2, *spp1, *tmp_ptr;
	mlib_d64 dd, dd0, dd1, sd10, sd11, sd20, sd21;
	mlib_s16 *dend;
	mlib_d64 rdhh, rdhl;
	mlib_d64 rdlh, rdll;
	mlib_d64 rdh, rdl;
	mlib_s32 i, j, k;

	mlib_d64 sd1, sd2, sd1ad, sd2ad, rdh_0, rdh_1, rdl_0, rdl_1;
	mlib_d64 offset = vis_to_double_dup(0x80008000);
	mlib_d64 half_offset = vis_to_double(0x40004000, 0x80008000);
	mlib_d64 const_offset = vis_to_double_dup(0x20000000);

	const_offset =
	    vis_fpsub32(const_offset,
	    vis_to_double_dup(0x40000000 >> (16 - shift)));

	if (width == stride1 && width == stride2 && width == strided) {
		width *= height;
		height = 1;
	}

/* initialize GSR scale factor */
	vis_write_gsr(((16 - (shift - 1)) & 0x1f) << 3);

	sl1 = sp1;
	sl2 = sp2;
	dl = dp;

	offdst = ((mlib_addr)dp) & 7;
	offsrc1 = ((mlib_addr)sp1) & 7;
	offsrc2 = ((mlib_addr)sp2) & 7;

	if ((offdst == offsrc1) && (offdst == offsrc2) &&
	    (((strided ^ stride1) & 3) == 0) &&
	    (((strided ^ stride2) & 3) == 0)) {

		for (j = 0; j < height; j++) {

/* prepare the destination addresses */
			dpp = (mlib_d64 *)vis_alignaddr(dp, 0);
			i = (mlib_s16 *)dpp - dp;

/* prepare the source addresses */
			spp1 = (mlib_d64 *)vis_alignaddr(sp1, 0);
			spp2 = (mlib_d64 *)vis_alignaddr(sp2, 0);

			dend = dp + width - 1;
/* generate edge mask for the start point */
			emask = vis_edge16(dp, dend);

			if (emask != 0xf) {
				sd10 = (*spp1++);
				sd20 = (*spp2++);
				MLIB_V_IMAGEMULSHIFTONE_U16(sd10, sd20, dd);
				vis_pst_16(dd, dpp++, emask);
				i += 4;
			}
#pragma pipeloop(0)
			for (; i <= width - 4; i += 4) {
				sd10 = (*spp1++);
				sd20 = (*spp2++);
				MLIB_V_IMAGEMULSHIFTONE_U16(sd10, sd20, dd);
				(*dpp++) = dd;
			}

			if (i < width) {
				emask = vis_edge16(dpp, dend);
				sd10 = (*spp1++);
				sd20 = (*spp2++);
				MLIB_V_IMAGEMULSHIFTONE_U16(sd10, sd20, dd);
				vis_pst_16(dd, dpp, emask);
			}

			sp1 = sl1 += stride1;
			sp2 = sl2 += stride2;
			dp = dl += strided;
		}
	} else if ((offdst == offsrc1) && (((strided ^ stride1) & 3) == 0)) {

		for (j = 0; j < height; j++) {

/* prepare the destination addresses */
			dpp = (mlib_d64 *)vis_alignaddr(dp, 0);
			i = (mlib_s16 *)dpp - dp;

/* prepare the source addresses */
			spp1 = (mlib_d64 *)vis_alignaddr(sp1, 0);
			spp2 = (mlib_d64 *)vis_alignaddr(sp2, 2 * i);

			dend = dp + width - 1;
/* generate edge mask for the start point */
			emask = vis_edge16(dp, dend);

			sd20 = spp2[0];

			if (emask != 0xf) {
				sd10 = (*spp1++);
				sd21 = spp2[1];
				sd20 = vis_faligndata(sd20, sd21);
				MLIB_V_IMAGEMULSHIFTONE_U16(sd10, sd20, dd);
				vis_pst_16(dd, dpp++, emask);
				sd20 = sd21;
				spp2++;
				i += 4;
			}
#pragma pipeloop(0)
			for (; i <= width - 4; i += 4) {
				sd10 = (*spp1++);
				sd21 = spp2[1];
				sd20 = vis_faligndata(sd20, sd21);
				MLIB_V_IMAGEMULSHIFTONE_U16(sd10, sd20, dd);
				(*dpp++) = dd;
				sd20 = sd21;
				spp2++;
			}

			if (i < width) {
				emask = vis_edge16(dpp, dend);
				sd10 = (*spp1++);
				sd20 = vis_faligndata(sd20, spp2[1]);
				MLIB_V_IMAGEMULSHIFTONE_U16(sd10, sd20, dd);
				vis_pst_16(dd, dpp, emask);
			}

			sp1 = sl1 += stride1;
			sp2 = sl2 += stride2;
			dp = dl += strided;
		}
	} else if ((offdst == offsrc2) && (((strided ^ stride2) & 3) == 0)) {

		for (j = 0; j < height; j++) {

/* prepare the destination addresses */
			dpp = (mlib_d64 *)vis_alignaddr(dp, 0);
			i = (mlib_s16 *)dpp - dp;

/* prepare the source addresses */
			spp2 = (mlib_d64 *)vis_alignaddr(sp2, 0);
			spp1 = (mlib_d64 *)vis_alignaddr(sp1, 2 * i);

			dend = dp + width - 1;
/* generate edge mask for the start point */
			emask = vis_edge16(dp, dend);

			sd10 = spp1[0];

			if (emask != 0xf) {
				sd20 = (*spp2++);
				sd11 = spp1[1];
				sd10 = vis_faligndata(sd10, sd11);
				MLIB_V_IMAGEMULSHIFTONE_U16(sd10, sd20, dd);
				vis_pst_16(dd, dpp++, emask);
				sd10 = sd11;
				spp1++;
				i += 4;
			}
#pragma pipeloop(0)
			for (; i <= width - 4; i += 4) {
				sd20 = (*spp2++);
				sd11 = spp1[1];
				sd10 = vis_faligndata(sd10, sd11);
				MLIB_V_IMAGEMULSHIFTONE_U16(sd10, sd20, dd);
				(*dpp++) = dd;
				sd10 = sd11;
				spp1++;
			}

			if (i < width) {
				emask = vis_edge16(dpp, dend);
				sd20 = (*spp2++);
				sd10 = vis_faligndata(sd10, spp1[1]);
				MLIB_V_IMAGEMULSHIFTONE_U16(sd10, sd20, dd);
				vis_pst_16(dd, dpp, emask);
			}

			sp1 = sl1 += stride1;
			sp2 = sl2 += stride2;
			dp = dl += strided;
		}
	} else if ((offsrc1 == offsrc2) && (((stride1 ^ stride2) & 3) == 0)) {
/* printf("4:\n"); */
		for (j = 0; j < height; j++) {

/* prepare the source addresses */
			dpp = (mlib_d64 *)vis_alignaddr(dp, 0);
			i = (mlib_s16 *)dpp - dp;

/* prepare the destination addresses */
			spp1 = (mlib_d64 *)vis_alignaddr(sp1, 2 * i);
			spp2 = (mlib_d64 *)vis_alignaddr(sp2, 2 * i);

			dend = dp + width - 1;
/* generate edge mask for the start point */
			emask = vis_edge16(dp, dend);

			sd10 = (*spp1++);
			sd20 = (*spp2++);
			MLIB_V_IMAGEMULSHIFTONE_U16(sd10, sd20, dd0);

			if (emask != 0xf) {
				sd10 = (*spp1++);
				sd20 = (*spp2++);
				MLIB_V_IMAGEMULSHIFTONE_U16(sd10, sd20, dd1);
				dd = vis_faligndata(dd0, dd1);
				vis_pst_16(dd, dpp++, emask);
				dd0 = dd1;
				i += 4;
			}
#pragma pipeloop(0)
			for (; i <= width - 4; i += 4) {
				sd10 = (*spp1++);
				sd20 = (*spp2++);
				MLIB_V_IMAGEMULSHIFTONE_U16(sd10, sd20, dd1);
				(*dpp++) = vis_faligndata(dd0, dd1);
				dd0 = dd1;
			}

			if (i < width) {
				emask = vis_edge16(dpp, dend);
				sd10 = (*spp1++);
				sd20 = (*spp2++);
				MLIB_V_IMAGEMULSHIFTONE_U16(sd10, sd20, dd1);
				dd = vis_faligndata(dd0, dd1);
				vis_pst_16(dd, dpp, emask);
			}

			sp1 = sl1 += stride1;
			sp2 = sl2 += stride2;
			dp = dl += strided;
		}
	} else {

		for (j = 0; j < height; j++) {

/* prepare the destination addresses */
			dpp = (mlib_d64 *)vis_alignaddr(dp, 0);
			i = (mlib_s16 *)dpp - dp;

			dend = dp + width - 1;
/* generate edge mask for the start point */
			emask = vis_edge16(dp, dend);

			if (emask != 0xf) {
				spp1 = (mlib_d64 *)vis_alignaddr(sp1, 2 * i);
				sd10 = vis_faligndata(spp1[0], spp1[1]);
				spp2 = (mlib_d64 *)vis_alignaddr(sp2, 2 * i);
				sd20 = vis_faligndata(spp2[0], spp2[1]);
				MLIB_V_IMAGEMULSHIFTONE_U16(sd10, sd20, dd);
				vis_pst_16(dd, dpp++, emask);
				i += 4;
			}

/* copy src1 to dst */
			spp1 = (mlib_d64 *)vis_alignaddr(sp1, 2 * i);
			sd11 = spp1[0];
			tmp_ptr = dpp;

#pragma pipeloop(0)
			for (k = i; k <= (width - 4); k += 4) {
				sd10 = sd11;
				sd11 = spp1[1];
				(*tmp_ptr++) = vis_faligndata(sd10, sd11);
				spp1++;
			}

			sd11 = vis_faligndata(sd11, spp1[1]);

			spp2 = (mlib_d64 *)vis_alignaddr(sp2, 2 * i);
			sd20 = spp2[0];
			tmp_ptr = dpp;

#pragma pipeloop(0)
			for (; i <= width - 4; i += 4) {
				sd10 = (*tmp_ptr++);
				sd21 = spp2[1];
				sd20 = vis_faligndata(sd20, sd21);
				MLIB_V_IMAGEMULSHIFTONE_U16(sd10, sd20, dd);
				(*dpp++) = dd;
				sd20 = sd21;
				spp2++;
			}

			if (i < width) {
				emask = vis_edge16(dpp, dend);
				sd20 = vis_faligndata(sd20, spp2[1]);
				MLIB_V_IMAGEMULSHIFTONE_U16(sd11, sd20, dd);
				vis_pst_16(dd, dpp, emask);
			}

			sp1 = sl1 += stride1;
			sp2 = sl2 += stride2;
			dp = dl += strided;
		}
	}
}

/* *********************************************************** */
