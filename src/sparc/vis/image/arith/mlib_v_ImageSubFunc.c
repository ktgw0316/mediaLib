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

#pragma ident	"@(#)mlib_v_ImageSubFunc.c	9.2	07/11/05 SMI"

/*
 * Internal function.
 *
 *      mlib_status mlib_v_ImageSub_S16(mlib_image       *dst,
 *                                      const mlib_image *src1,
 *                                      const mlib_image *src2);
 *
 *      mlib_status mlib_v_ImageSub_S32(mlib_image       *dst,
 *                                      const mlib_image *src1,
 *                                      const mlib_image *src2);
 *
 *      mlib_status mlib_v_ImageSub_U16(mlib_image       *dst,
 *                                      const mlib_image *src1,
 *                                      const mlib_image *src2);
 *
 *      mlib_status mlib_v_ImageSub_U8(mlib_image       *dst,
 *                                     const mlib_image *src1,
 *                                     const mlib_image *src2);
 *
 * ARGUMENT
 *         dst     Pointer to output image.
 *         src1    Pointer to first input image.
 *         src2    Pointer to second input image.
 *
 *         src1, src2 and dst must be the same type, the same size and the same
 *         number of channels. They can have 1, 2, 3 or 4 channels.
 *         They can be in MLIB_BYTE, MLIB_SHORT,
 *         MLIB_USHORT or MLIB_INT data type.
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_v_ImageSubFunc.h>

/* *********************************************************** */

#define	VALIDATE(FORMAT)                                        \
	FORMAT *sp1, *sl1;                                      \
	                                                        \
/*                                                              \
 * pointers for pixel and line of source                        \
 */                                                             \
	FORMAT *sp2, *sl2;                                      \
	                                                        \
/*                                                              \
 * pointers for pixel and line of source                        \
 */                                                             \
	FORMAT *dp, *dl;                                        \
	                                                        \
/*                                                              \
 * pointers for pixel and line of dst                           \
 */                                                             \
	mlib_s32 width, height, channels;                       \
	mlib_s32 stride1;                                       \
	                                                        \
/*                                                              \
 * for src1                                                     \
 */                                                             \
	mlib_s32 stride2;                                       \
	                                                        \
/*                                                              \
 * for src2                                                     \
 */                                                             \
	mlib_s32 strided;                                       \
	                                                        \
/*                                                              \
 * for dst                                                      \
 */                                                             \
	mlib_s32 is_inplace2 = !src2;                           \
	                                                        \
/*                                                              \
 * need it for Inp_U16 @ UIII                                   \
 */                                                             \
	src2 = is_inplace2 ? dst : src2;                        \
	channels = mlib_ImageGetChannels(dst);                  \
	width = mlib_ImageGetWidth(dst);                        \
	height = mlib_ImageGetHeight(dst);                      \
	strided = mlib_ImageGetStride(dst);                     \
	dp = (void *)mlib_ImageGetData(dst);                    \
	stride1 = mlib_ImageGetStride(src1);                    \
	sp1 = mlib_ImageGetData(src1);                          \
	stride2 = mlib_ImageGetStride(src2);                    \
	sp2 = mlib_ImageGetData(src2);                          \
	stride1 /= sizeof (FORMAT);                             \
	stride2 /= sizeof (FORMAT);                             \
	strided /= sizeof (FORMAT);                             \
	if ((width * channels) == stride1 &&                    \
	    (width * channels) == stride2 &&                    \
	    (width * channels) == strided) {                    \
	    width = width * height;                             \
	    height = 1;                                         \
	}

/* *********************************************************** */

#define	MLIB_V_SUBIMAGE_U8(sd1, sd2, dd)                        \
	sd1h = vis_fpmerge(nul, vis_read_hi(sd1));              \
	sd2h = vis_fpmerge(nul, vis_read_hi(sd2));              \
	sd1l = vis_fmul8x16al(vis_read_lo(sd1), fone);          \
	sd2l = vis_fmul8x16al(vis_read_lo(sd2), fone);          \
	rdh = vis_fpsub16(sd1h, sd2h);                          \
	rdl = vis_fpsub16(sd1l, sd2l);                          \
	dd = vis_fpack16_to_hi(dd, rdh);                        \
	dd = vis_fpack16_pair(rdh, rdl)

/* *********************************************************** */

mlib_status
mlib_v_ImageSub_U8(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2)
{
	mlib_s32 i, j, k;
	mlib_s32 offdst, offsrc1, offsrc2, emask;
	mlib_s32 amount;
	mlib_d64 *dpp, *spp2, *spp1, *tmp_ptr;
	mlib_d64 dd, dd0, dd1, sd10, sd11, sd20, sd21;
	mlib_d64 sd1h, sd2h, sd1l, sd2l, rdh, rdl;
	mlib_u8 *dend;
	mlib_f32 nul = vis_to_float(0), fone = vis_to_float(0x100);

	VALIDATE(mlib_u8);

/* initialize GSR scale factor */
	vis_write_gsr(7 << 3);

	sl1 = sp1;
	sl2 = sp2;
	dl = dp;

	amount = width * channels;

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

			dend = dp + amount - 1;
/* generate edge mask for the start point */
			emask = vis_edge8(dp, dend);

			if (emask != 0xff) {
				sd10 = (*spp1++);
				sd20 = (*spp2++);
				MLIB_V_SUBIMAGE_U8(sd10, sd20, dd);
				vis_pst_8(dd, dpp++, emask);
				i += 8;
			}
#pragma pipeloop(0)
			for (; i <= amount - 8; i += 8) {
				sd10 = (*spp1++);
				sd20 = (*spp2++);
				MLIB_V_SUBIMAGE_U8(sd10, sd20, dd);
				(*dpp++) = dd;
			}

			if (i < amount) {
				emask = vis_edge8(dpp, dend);
				sd10 = (*spp1++);
				sd20 = (*spp2++);
				MLIB_V_SUBIMAGE_U8(sd10, sd20, dd);
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

			dend = dp + amount - 1;
/* generate edge mask for the start point */
			emask = vis_edge8(dp, dend);

			sd20 = vis_ld_d64_nf(spp2);

			if (emask != 0xff) {
				sd10 = (*spp1++);
				sd21 = vis_ld_d64_nf(spp2 + 1);
				sd20 = vis_faligndata(sd20, sd21);
				MLIB_V_SUBIMAGE_U8(sd10, sd20, dd);
				vis_pst_8(dd, dpp++, emask);
				sd20 = sd21;
				spp2++;
				i += 8;
			}
#pragma pipeloop(0)
			for (; i <= amount - 8; i += 8) {
				sd10 = (*spp1++);
				sd21 = vis_ld_d64_nf(spp2 + 1);
				sd20 = vis_faligndata(sd20, sd21);
				MLIB_V_SUBIMAGE_U8(sd10, sd20, dd);
				(*dpp++) = dd;
				sd20 = sd21;
				spp2++;
			}

			if (i < amount) {
				emask = vis_edge8(dpp, dend);
				sd10 = (*spp1++);
				sd20 = vis_faligndata(sd20,
					vis_ld_d64_nf(spp2 + 1));
				MLIB_V_SUBIMAGE_U8(sd10, sd20, dd);
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

			dend = dp + amount - 1;
/* generate edge mask for the start point */
			emask = vis_edge8(dp, dend);

			sd10 = vis_ld_d64_nf(spp1);

			if (emask != 0xff) {
				sd20 = (*spp2++);
				sd11 = vis_ld_d64_nf(spp1 + 1);
				sd10 = vis_faligndata(sd10, sd11);
				MLIB_V_SUBIMAGE_U8(sd10, sd20, dd);
				vis_pst_8(dd, dpp++, emask);
				sd10 = sd11;
				spp1++;
				i += 8;
			}
#pragma pipeloop(0)
			for (; i <= amount - 8; i += 8) {
				sd20 = (*spp2++);
				sd11 = vis_ld_d64_nf(spp1 + 1);
				sd10 = vis_faligndata(sd10, sd11);
				MLIB_V_SUBIMAGE_U8(sd10, sd20, dd);
				(*dpp++) = dd;
				sd10 = sd11;
				spp1++;
			}

			if (i < amount) {
				emask = vis_edge8(dpp, dend);
				sd20 = (*spp2++);
				sd10 = vis_faligndata(sd10,
					vis_ld_d64_nf(spp1 + 1));
				MLIB_V_SUBIMAGE_U8(sd10, sd20, dd);
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

			dend = dp + amount - 1;
/* generate edge mask for the start point */
			emask = vis_edge8(dp, dend);

			sd10 = vis_ld_d64_nf(spp1); spp1++;
			sd20 = vis_ld_d64_nf(spp2); spp2++;
			MLIB_V_SUBIMAGE_U8(sd10, sd20, dd0);

			if (emask != 0xff) {
				sd10 = vis_ld_d64_nf(spp1); spp1++;
				sd20 = vis_ld_d64_nf(spp2); spp2++;
				MLIB_V_SUBIMAGE_U8(sd10, sd20, dd1);
				dd = vis_faligndata(dd0, dd1);
				vis_pst_8(dd, dpp++, emask);
				dd0 = dd1;
				i += 8;
			}
#pragma pipeloop(0)
			for (; i <= amount - 8; i += 8) {
				sd10 = vis_ld_d64_nf(spp1); spp1++;
				sd20 = vis_ld_d64_nf(spp2); spp2++;
				MLIB_V_SUBIMAGE_U8(sd10, sd20, dd1);
				(*dpp++) = vis_faligndata(dd0, dd1);
				dd0 = dd1;
			}

			if (i < amount) {
				emask = vis_edge8(dpp, dend);
				sd10 = vis_ld_d64_nf(spp1); spp1++;
				sd20 = vis_ld_d64_nf(spp2); spp2++;
				MLIB_V_SUBIMAGE_U8(sd10, sd20, dd1);
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

			dend = dp + amount - 1;
/* generate edge mask for the start point */
			emask = vis_edge8(dp, dend);

			if (emask != 0xff) {
				spp1 = (mlib_d64 *)vis_alignaddr(sp1, i);
				sd10 = vis_faligndata(vis_ld_d64_nf(spp1),
					vis_ld_d64_nf(spp1 + 1));
				spp2 = (mlib_d64 *)vis_alignaddr(sp2, i);
				sd20 = vis_faligndata(vis_ld_d64_nf(spp2),
					vis_ld_d64_nf(spp2 + 1));
				MLIB_V_SUBIMAGE_U8(sd10, sd20, dd);
				vis_pst_8(dd, dpp++, emask);
				i += 8;
			}

/* copy src1 to dst */
			spp1 = (mlib_d64 *)vis_alignaddr(sp1, i);
			sd11 = vis_ld_d64_nf(spp1);
			tmp_ptr = dpp;

#pragma pipeloop(0)
			for (k = i; k <= (amount - 8); k += 8) {
				sd10 = sd11;
				sd11 = vis_ld_d64_nf(spp1 + 1);
				(*tmp_ptr++) = vis_faligndata(sd10, sd11);
				spp1++;
			}

			sd11 = vis_faligndata(sd11, vis_ld_d64_nf(spp1 + 1));

			spp2 = (mlib_d64 *)vis_alignaddr(sp2, i);
			sd20 = vis_ld_d64_nf(spp2);
			tmp_ptr = dpp;

#pragma pipeloop(0)
			for (; i <= amount - 8; i += 8) {
				sd10 = (*tmp_ptr++);
				sd21 = vis_ld_d64_nf(spp2 + 1);
				sd20 = vis_faligndata(sd20, sd21);
				MLIB_V_SUBIMAGE_U8(sd10, sd20, dd);
				(*dpp++) = dd;
				sd20 = sd21;
				spp2++;
			}

			if (i < amount) {
				emask = vis_edge8(dpp, dend);
				sd20 = vis_faligndata(sd20,
					vis_ld_d64_nf(spp2 + 1));
				MLIB_V_SUBIMAGE_U8(sd11, sd20, dd);
				vis_pst_8(dd, dpp, emask);
			}

			sp1 = sl1 += stride1;
			sp2 = sl2 += stride2;
			dp = dl += strided;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MLIB_V_SUBIMAGE_S16(sd1, sd2, dd)	dd = vis_fpsub16(sd1, sd2)

/* *********************************************************** */

mlib_status
mlib_v_ImageSub_S16(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2)
{
	mlib_s32 i, j, k;
	mlib_s32 offdst, offsrc1, offsrc2, emask;
	mlib_s32 amount;
	mlib_d64 *dpp, *spp2, *spp1, *tmp_ptr;
	mlib_d64 dd, dd0, dd1, sd10, sd11, sd20, sd21;
	mlib_s16 *dend;

	VALIDATE(mlib_s16);

	sl1 = sp1;
	sl2 = sp2;
	dl = dp;

	amount = width * channels;

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

			dend = dp + amount - 1;
/* generate edge mask for the start point */
			emask = vis_edge16(dp, dend);

			if (emask != 0xf) {
				sd10 = (*spp1++);
				sd20 = (*spp2++);
				MLIB_V_SUBIMAGE_S16(sd10, sd20, dd);
				vis_pst_16(dd, dpp++, emask);
				i += 4;
			}
#pragma pipeloop(0)
			for (; i <= amount - 4; i += 4) {
				sd10 = (*spp1++);
				sd20 = (*spp2++);
				MLIB_V_SUBIMAGE_S16(sd10, sd20, dd);
				(*dpp++) = dd;
			}

			if (i < amount) {
				emask = vis_edge16(dpp, dend);
				sd10 = (*spp1++);
				sd20 = (*spp2++);
				MLIB_V_SUBIMAGE_S16(sd10, sd20, dd);
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

			dend = dp + amount - 1;
/* generate edge mask for the start point */
			emask = vis_edge16(dp, dend);

			sd20 = vis_ld_d64_nf(spp2);

			if (emask != 0xf) {
				sd10 = (*spp1++);
				sd21 = vis_ld_d64_nf(spp2 + 1);
				sd20 = vis_faligndata(sd20, sd21);
				MLIB_V_SUBIMAGE_S16(sd10, sd20, dd);
				vis_pst_16(dd, dpp++, emask);
				sd20 = sd21;
				spp2++;
				i += 4;
			}
#pragma pipeloop(0)
			for (; i <= amount - 4; i += 4) {
				sd10 = (*spp1++);
				sd21 = vis_ld_d64_nf(spp2 + 1);
				sd20 = vis_faligndata(sd20, sd21);
				MLIB_V_SUBIMAGE_S16(sd10, sd20, dd);
				(*dpp++) = dd;
				sd20 = sd21;
				spp2++;
			}

			if (i < amount) {
				emask = vis_edge16(dpp, dend);
				sd10 = (*spp1++);
				sd20 = vis_faligndata(sd20,
					vis_ld_d64_nf(spp2 + 1));
				MLIB_V_SUBIMAGE_S16(sd10, sd20, dd);
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

			dend = dp + amount - 1;
/* generate edge mask for the start point */
			emask = vis_edge16(dp, dend);

			sd10 = vis_ld_d64_nf(spp1);

			if (emask != 0xf) {
				sd20 = (*spp2++);
				sd11 = vis_ld_d64_nf(spp1 + 1);
				sd10 = vis_faligndata(sd10, sd11);
				MLIB_V_SUBIMAGE_S16(sd10, sd20, dd);
				vis_pst_16(dd, dpp++, emask);
				sd10 = sd11;
				spp1++;
				i += 4;
			}
#pragma pipeloop(0)
			for (; i <= amount - 4; i += 4) {
				sd20 = (*spp2++);
				sd11 = vis_ld_d64_nf(spp1 + 1);
				sd10 = vis_faligndata(sd10, sd11);
				MLIB_V_SUBIMAGE_S16(sd10, sd20, dd);
				(*dpp++) = dd;
				sd10 = sd11;
				spp1++;
			}

			if (i < amount) {
				emask = vis_edge16(dpp, dend);
				sd20 = (*spp2++);
				sd10 = vis_faligndata(sd10,
					vis_ld_d64_nf(spp1 + 1));
				MLIB_V_SUBIMAGE_S16(sd10, sd20, dd);
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

			dend = dp + amount - 1;
/* generate edge mask for the start point */
			emask = vis_edge16(dp, dend);

			sd10 = vis_ld_d64_nf(spp1); spp1++;
			sd20 = vis_ld_d64_nf(spp2); spp2++;
			MLIB_V_SUBIMAGE_S16(sd10, sd20, dd0);

			if (emask != 0xf) {
				sd10 = vis_ld_d64_nf(spp1); spp1++;
				sd20 = vis_ld_d64_nf(spp2); spp2++;
				MLIB_V_SUBIMAGE_S16(sd10, sd20, dd1);
				dd = vis_faligndata(dd0, dd1);
				vis_pst_16(dd, dpp++, emask);
				dd0 = dd1;
				i += 4;
			}
#pragma pipeloop(0)
			for (; i <= amount - 4; i += 4) {
				sd10 = vis_ld_d64_nf(spp1); spp1++;
				sd20 = vis_ld_d64_nf(spp2); spp2++;
				MLIB_V_SUBIMAGE_S16(sd10, sd20, dd1);
				(*dpp++) = vis_faligndata(dd0, dd1);
				dd0 = dd1;
			}

			if (i < amount) {
				emask = vis_edge16(dpp, dend);
				sd10 = vis_ld_d64_nf(spp1); spp1++;
				sd20 = vis_ld_d64_nf(spp2); spp2++;
				MLIB_V_SUBIMAGE_S16(sd10, sd20, dd1);
				dd = vis_faligndata(dd0, dd1);
				vis_pst_16(dd, dpp, emask);
			}

			sp1 = sl1 += stride1;
			sp2 = sl2 += stride2;
			dp = dl += strided;
		}
	} else {
/* common case */

		for (j = 0; j < height; j++) {

/* prepare the destination addresses */
			dpp = (mlib_d64 *)vis_alignaddr(dp, 0);
			i = (mlib_s16 *)dpp - dp;

			dend = dp + amount - 1;
/* generate edge mask for the start point */
			emask = vis_edge16(dp, dend);

			if (emask != 0xf) {
				spp1 = (mlib_d64 *)vis_alignaddr(sp1, 2 * i);
				sd10 = vis_faligndata(vis_ld_d64_nf(spp1),
					vis_ld_d64_nf(spp1 + 1));
				spp2 = (mlib_d64 *)vis_alignaddr(sp2, 2 * i);
				sd20 = vis_faligndata(vis_ld_d64_nf(spp2),
					vis_ld_d64_nf(spp2 + 1));
				MLIB_V_SUBIMAGE_S16(sd10, sd20, dd);
				vis_pst_16(dd, dpp++, emask);
				i += 4;
			}

/* copy src1 to dst */
			spp1 = (mlib_d64 *)vis_alignaddr(sp1, 2 * i);
			sd11 = vis_ld_d64_nf(spp1);
			tmp_ptr = dpp;

#pragma pipeloop(0)
			for (k = i; k <= (amount - 4); k += 4) {
				sd10 = sd11;
				sd11 = vis_ld_d64_nf(spp1 + 1);
				(*tmp_ptr++) = vis_faligndata(sd10, sd11);
				spp1++;
			}

			sd11 = vis_faligndata(sd11, vis_ld_d64_nf(spp1 + 1));

			spp2 = (mlib_d64 *)vis_alignaddr(sp2, 2 * i);
			sd20 = vis_ld_d64_nf(spp2);
			tmp_ptr = dpp;

#pragma pipeloop(0)
			for (; i <= amount - 4; i += 4) {
				sd10 = (*tmp_ptr++);
				sd21 = vis_ld_d64_nf(spp2 + 1);
				sd20 = vis_faligndata(sd20, sd21);
				MLIB_V_SUBIMAGE_S16(sd10, sd20, dd);
				(*dpp++) = dd;
				sd20 = sd21;
				spp2++;
			}

			if (i < amount) {
				emask = vis_edge16(dpp, dend);
				sd20 = vis_faligndata(sd20,
					vis_ld_d64_nf(spp2 + 1));
				MLIB_V_SUBIMAGE_S16(sd11, sd20, dd);
				vis_pst_16(dd, dpp, emask);
			}

			sp1 = sl1 += stride1;
			sp2 = sl2 += stride2;
			dp = dl += strided;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MLIB_V_SUBIMAGE_U16(sd1, sd2, dpp)                      \
	{                                                       \
	    tmp = vis_fpsub16(sd1, sd2);                        \
	    sd1 = vis_fxor(ones, sd1);                          \
	    sd2 = vis_fxor(ones, sd2);                          \
	    mask = vis_fcmpge16(sd1, sd2);                      \
	    vis_pst_16(tmp, dpp, mask);                         \
	    vis_pst_16(max_u16, dpp, ~mask);                    \
	    dpp++;                                              \
	}

/* *********************************************************** */

#define	MLIB_V_SUBIMAGE_U16_emask(sd1, sd2, dpp, emask)         \
	{                                                       \
	    tmp = vis_fpsub16(sd1, sd2);                        \
	    sd1 = vis_fxor(ones, sd1);                          \
	    sd2 = vis_fxor(ones, sd2);                          \
	    mask = vis_fcmpge16(sd1, sd2);                      \
	    vis_pst_16(tmp, dpp, mask & emask);                 \
	    vis_pst_16(max_u16, dpp, ~mask & emask);            \
	    dpp++;                                              \
	}

/* *********************************************************** */

mlib_status
mlib_v_ImageSub_U16(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2)
{
	mlib_s32 i, j, k;
	mlib_s32 offdst, offsrc1, offsrc2, emask, mask;
	mlib_s32 amount;
	mlib_d64 *dpp, *spp2, *spp1, *tmp_ptr, tmp;
	mlib_d64 sd10, sd11, sd20, sd21;
	mlib_d64 ones = vis_to_double_dup(0x80008000);
	mlib_d64 max_u16 = vis_to_double_dup(0);
	mlib_u16 *dend;
	mlib_s32 skipit = 0;

	VALIDATE(mlib_u16);

/* initialize GSR scale factor */
	vis_write_gsr(15 << 3);

	sl1 = sp1;
	sl2 = sp2;
	dl = dp;

	amount = width * channels;

	offdst = ((mlib_addr)dp) & 7;
	offsrc1 = ((mlib_addr)sp1) & 7;
	offsrc2 = ((mlib_addr)sp2) & 7;

/*  @ UIII generic code works in this case MUCH faster  */

	if (is_inplace2 && (offsrc1 == 0) && (offdst == 0) &&
	    (((strided ^ stride1) & 3) == 0))
		skipit = 1;

	if (!skipit && (offdst == offsrc1) && (offdst == offsrc2) &&
	    (((strided ^ stride1) & 3) == 0) &&
	    (((strided ^ stride2) & 3) == 0)) {

		for (j = 0; j < height; j++) {

/* prepare the destination addresses */
			dpp = (mlib_d64 *)vis_alignaddr(dp, 0);
			i = (mlib_u16 *)dpp - dp;

/* prepare the source addresses */
			spp1 = (mlib_d64 *)vis_alignaddr(sp1, 0);
			spp2 = (mlib_d64 *)vis_alignaddr(sp2, 0);

			dend = dp + amount - 1;
/* generate edge mask for the start point */
			emask = vis_edge16(dp, dend);

			if (emask != 0xf) {
				sd10 = (*spp1++);
				sd20 = (*spp2++);
				MLIB_V_SUBIMAGE_U16_emask(sd10, sd20, dpp,
				    emask)
				    i += 4;
			}
#pragma pipeloop(0)
			for (; i <= amount - 4; i += 4) {
				sd10 = (*spp1++);
				sd20 = (*spp2++);
				MLIB_V_SUBIMAGE_U16(sd10, sd20, dpp)
			}

			if (i < amount) {
				emask = vis_edge16(dpp, dend);
				sd10 = (*spp1++);
				sd20 = (*spp2++);
				MLIB_V_SUBIMAGE_U16_emask(sd10, sd20, dpp,
				    emask)
			}

			sp1 = sl1 += stride1;
			sp2 = sl2 += stride2;
			dp = dl += strided;
		}
	} else if ((offdst == offsrc1) && (((strided ^ stride1) & 3) == 0)) {

		for (j = 0; j < height; j++) {

/* prepare the destination addresses */
			dpp = (mlib_d64 *)vis_alignaddr(dp, 0);
			i = (mlib_u16 *)dpp - dp;

/* prepare the source addresses */
			spp1 = (mlib_d64 *)vis_alignaddr(sp1, 0);
			spp2 = (mlib_d64 *)vis_alignaddr(sp2, 2 * i);

			dend = dp + amount - 1;
/* generate edge mask for the start point */
			emask = vis_edge16(dp, dend);

			sd20 = vis_ld_d64_nf(spp2);

			if (emask != 0xf) {
				sd10 = (*spp1++);
				sd21 = vis_ld_d64_nf(spp2 + 1);
				sd20 = vis_faligndata(sd20, sd21);
				MLIB_V_SUBIMAGE_U16_emask(sd10, sd20, dpp,
				    emask)
				    sd20 = sd21;
				spp2++;
				i += 4;
			}
#pragma pipeloop(0)
			for (; i <= amount - 4; i += 4) {
				sd10 = (*spp1++);
				sd21 = vis_ld_d64_nf(spp2 + 1);
				sd20 = vis_faligndata(sd20, sd21);
				MLIB_V_SUBIMAGE_U16(sd10, sd20, dpp)
				    sd20 = sd21;
				spp2++;
			}

			if (i < amount) {
				emask = vis_edge16(dpp, dend);
				sd10 = (*spp1++);
				sd20 = vis_faligndata(sd20,
					vis_ld_d64_nf(spp2 + 1));
				MLIB_V_SUBIMAGE_U16_emask(sd10, sd20, dpp,
				    emask)
			}

			sp1 = sl1 += stride1;
			sp2 = sl2 += stride2;
			dp = dl += strided;
		}
	} else if ((offdst == offsrc2) && (((strided ^ stride2) & 3) == 0)) {

		for (j = 0; j < height; j++) {

/* prepare the destination addresses */
			dpp = (mlib_d64 *)vis_alignaddr(dp, 0);
			i = (mlib_u16 *)dpp - dp;

/* prepare the source addresses */
			spp2 = (mlib_d64 *)vis_alignaddr(sp2, 0);
			spp1 = (mlib_d64 *)vis_alignaddr(sp1, 2 * i);

			dend = dp + amount - 1;
/* generate edge mask for the start point */
			emask = vis_edge16(dp, dend);

			sd10 = vis_ld_d64_nf(spp1);

			if (emask != 0xf) {
				sd20 = (*spp2++);
				sd11 = vis_ld_d64_nf(spp1 + 1);
				sd10 = vis_faligndata(sd10, sd11);
				MLIB_V_SUBIMAGE_U16_emask(sd10, sd20, dpp,
				    emask)
				    sd10 = sd11;
				spp1++;
				i += 4;
			}
#pragma pipeloop(0)
			for (; i <= amount - 4; i += 4) {
				sd20 = (*spp2++);
				sd11 = vis_ld_d64_nf(spp1 + 1);
				sd10 = vis_faligndata(sd10, sd11);
				MLIB_V_SUBIMAGE_U16(sd10, sd20, dpp)
				    sd10 = sd11;
				spp1++;
			}

			if (i < amount) {
				emask = vis_edge16(dpp, dend);
				sd20 = (*spp2++);
				sd10 = vis_faligndata(sd10,
					vis_ld_d64_nf(spp1 + 1));
				MLIB_V_SUBIMAGE_U16_emask(sd10, sd20, dpp,
				    emask)
			}

			sp1 = sl1 += stride1;
			sp2 = sl2 += stride2;
			dp = dl += strided;
		}
	} else {
/* common case */

		for (j = 0; j < height; j++) {

/* prepare the destination addresses */
			dpp = (mlib_d64 *)vis_alignaddr(dp, 0);
			i = (mlib_u16 *)dpp - dp;

			dend = dp + amount - 1;
/* generate edge mask for the start point */
			emask = vis_edge16(dp, dend);

			if (emask != 0xf) {
				spp1 = (mlib_d64 *)vis_alignaddr(sp1, 2 * i);
				sd10 = vis_faligndata(vis_ld_d64_nf(spp1),
					vis_ld_d64_nf(spp1 + 1));
				spp2 = (mlib_d64 *)vis_alignaddr(sp2, 2 * i);
				sd20 = vis_faligndata(vis_ld_d64_nf(spp2),
					vis_ld_d64_nf(spp2 + 1));
				MLIB_V_SUBIMAGE_U16_emask(sd10, sd20, dpp,
				    emask)
				    i += 4;
			}

/* copy src1 to dst */
			spp1 = (mlib_d64 *)vis_alignaddr(sp1, 2 * i);
			sd11 = vis_ld_d64_nf(spp1);
			tmp_ptr = dpp;

#pragma pipeloop(0)
			for (k = i; k <= (amount - 4); k += 4) {
				sd10 = sd11;
				sd11 = vis_ld_d64_nf(spp1 + 1);
				(*tmp_ptr++) = vis_faligndata(sd10, sd11);
				spp1++;
			}

			sd11 = vis_faligndata(sd11, vis_ld_d64_nf(spp1 + 1));

			spp2 = (mlib_d64 *)vis_alignaddr(sp2, 2 * i);
			sd20 = vis_ld_d64_nf(spp2);
			tmp_ptr = dpp;

#pragma pipeloop(0)
			for (; i <= amount - 4; i += 4) {
				sd10 = (*tmp_ptr++);
				sd21 = vis_ld_d64_nf(spp2 + 1);
				sd20 = vis_faligndata(sd20, sd21);
				MLIB_V_SUBIMAGE_U16(sd10, sd20, dpp)
				    sd20 = sd21;
				spp2++;
			}

			if (i < amount) {
				emask = vis_edge16(dpp, dend);
				sd20 = vis_faligndata(sd20,
					vis_ld_d64_nf(spp2 + 1));
				MLIB_V_SUBIMAGE_U16_emask(sd11, sd20, dpp,
				    emask)
			}

			sp1 = sl1 += stride1;
			sp2 = sl2 += stride2;
			dp = dl += strided;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MLIB_V_SUBIMAGE_S32(sd1, sd2, dd)	dd = vis_fpsub32(sd1, sd2)

/* *********************************************************** */

mlib_status
mlib_v_ImageSub_S32(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2)
{
	mlib_s32 i, j, k;
	mlib_s32 offdst, offsrc1, offsrc2, emask;
	mlib_s32 amount;
	mlib_d64 *dpp, *spp2, *spp1, *tmp_ptr;
	mlib_d64 dd, dd0, dd1, sd10, sd11, sd20, sd21;
	mlib_s32 *dend;

	VALIDATE(mlib_s32);

	sl1 = sp1;
	sl2 = sp2;
	dl = dp;

	amount = width * channels;

	offdst = ((mlib_addr)dp) & 7;
	offsrc1 = ((mlib_addr)sp1) & 7;
	offsrc2 = ((mlib_addr)sp2) & 7;

	if ((offdst == offsrc1) && (offdst == offsrc2) &&
	    (((strided ^ stride1) & 1) == 0) &&
	    (((strided ^ stride2) & 1) == 0)) {

		for (j = 0; j < height; j++) {

/* prepare the destination addresses */
			dpp = (mlib_d64 *)vis_alignaddr(dp, 0);
			i = (mlib_s32 *)dpp - dp;

/* prepare the source addresses */
			spp1 = (mlib_d64 *)vis_alignaddr(sp1, 0);
			spp2 = (mlib_d64 *)vis_alignaddr(sp2, 0);

			dend = dp + amount - 1;
/* generate edge mask for the start point */
			emask = vis_edge32(dp, dend);

			if (emask != 3) {
				sd10 = (*spp1++);
				sd20 = (*spp2++);
				MLIB_V_SUBIMAGE_S32(sd10, sd20, dd);
				vis_pst_32(dd, dpp++, emask);
				i += 2;
			}
#pragma pipeloop(0)
			for (; i <= amount - 2; i += 2) {
				sd10 = (*spp1++);
				sd20 = (*spp2++);
				MLIB_V_SUBIMAGE_S32(sd10, sd20, dd);
				(*dpp++) = dd;
			}

			if (i < amount) {
				emask = vis_edge32(dpp, dend);
				sd10 = (*spp1++);
				sd20 = (*spp2++);
				MLIB_V_SUBIMAGE_S32(sd10, sd20, dd);
				vis_pst_32(dd, dpp, emask);
			}

			sp1 = sl1 += stride1;
			sp2 = sl2 += stride2;
			dp = dl += strided;
		}
	} else if ((offdst == offsrc1) && (((strided ^ stride1) & 1) == 0)) {

		for (j = 0; j < height; j++) {

/* prepare the destination addresses */
			dpp = (mlib_d64 *)vis_alignaddr(dp, 0);
			i = (mlib_s32 *)dpp - dp;

/* prepare the source addresses */
			spp1 = (mlib_d64 *)vis_alignaddr(sp1, 0);
			spp2 = (mlib_d64 *)vis_alignaddr(sp2, 4 * i);

			dend = dp + amount - 1;
/* generate edge mask for the start point */
			emask = vis_edge32(dp, dend);

			sd20 = vis_ld_d64_nf(spp2);

			if (emask != 3) {
				sd10 = (*spp1++);
				sd21 = vis_ld_d64_nf(spp2 + 1);
				sd20 = vis_faligndata(sd20, sd21);
				MLIB_V_SUBIMAGE_S32(sd10, sd20, dd);
				vis_pst_32(dd, dpp++, emask);
				sd20 = sd21;
				spp2++;
				i += 2;
			}
#pragma pipeloop(0)
			for (; i <= amount - 2; i += 2) {
				sd10 = (*spp1++);
				sd21 = vis_ld_d64_nf(spp2 + 1);
				sd20 = vis_faligndata(sd20, sd21);
				MLIB_V_SUBIMAGE_S32(sd10, sd20, dd);
				(*dpp++) = dd;
				sd20 = sd21;
				spp2++;
			}

			if (i < amount) {
				emask = vis_edge32(dpp, dend);
				sd10 = (*spp1++);
				sd20 = vis_faligndata(sd20,
					vis_ld_d64_nf(spp2 + 1));
				MLIB_V_SUBIMAGE_S32(sd10, sd20, dd);
				vis_pst_32(dd, dpp, emask);
			}

			sp1 = sl1 += stride1;
			sp2 = sl2 += stride2;
			dp = dl += strided;
		}
	} else if ((offdst == offsrc2) && (((strided ^ stride2) & 1) == 0)) {

		for (j = 0; j < height; j++) {

/* prepare the destination addresses */
			dpp = (mlib_d64 *)vis_alignaddr(dp, 0);
			i = (mlib_s32 *)dpp - dp;

/* prepare the source addresses */
			spp2 = (mlib_d64 *)vis_alignaddr(sp2, 0);
			spp1 = (mlib_d64 *)vis_alignaddr(sp1, 4 * i);

			dend = dp + amount - 1;
/* generate edge mask for the start point */
			emask = vis_edge32(dp, dend);

			sd10 = vis_ld_d64_nf(spp1);

			if (emask != 3) {
				sd20 = (*spp2++);
				sd11 = vis_ld_d64_nf(spp1 + 1);
				sd10 = vis_faligndata(sd10, sd11);
				MLIB_V_SUBIMAGE_S32(sd10, sd20, dd);
				vis_pst_32(dd, dpp++, emask);
				sd10 = sd11;
				spp1++;
				i += 2;
			}
#pragma pipeloop(0)
			for (; i <= amount - 2; i += 2) {
				sd20 = (*spp2++);
				sd11 = vis_ld_d64_nf(spp1 + 1);
				sd10 = vis_faligndata(sd10, sd11);
				MLIB_V_SUBIMAGE_S32(sd10, sd20, dd);
				(*dpp++) = dd;
				sd10 = sd11;
				spp1++;
			}

			if (i < amount) {
				emask = vis_edge32(dpp, dend);
				sd20 = (*spp2++);
				sd10 = vis_faligndata(sd10,
					vis_ld_d64_nf(spp1 + 1));
				MLIB_V_SUBIMAGE_S32(sd10, sd20, dd);
				vis_pst_32(dd, dpp, emask);
			}

			sp1 = sl1 += stride1;
			sp2 = sl2 += stride2;
			dp = dl += strided;
		}
	} else if ((offsrc1 == offsrc2) && (((stride1 ^ stride2) & 1) == 0)) {

		for (j = 0; j < height; j++) {

/* prepare the source addresses */
			dpp = (mlib_d64 *)vis_alignaddr(dp, 0);
			i = (mlib_s32 *)dpp - dp;

/* prepare the destination addresses */
			spp1 = (mlib_d64 *)vis_alignaddr(sp1, 4 * i);
			spp2 = (mlib_d64 *)vis_alignaddr(sp2, 4 * i);

			dend = dp + amount - 1;
/* generate edge mask for the start point */
			emask = vis_edge32(dp, dend);

			sd10 = vis_ld_d64_nf(spp1); spp1++;
			sd20 = vis_ld_d64_nf(spp2); spp2++;
			MLIB_V_SUBIMAGE_S32(sd10, sd20, dd0);

			if (emask != 3) {
				sd10 = vis_ld_d64_nf(spp1); spp1++;
				sd20 = vis_ld_d64_nf(spp2); spp2++;
				MLIB_V_SUBIMAGE_S32(sd10, sd20, dd1);
				dd = vis_faligndata(dd0, dd1);
				vis_pst_32(dd, dpp++, emask);
				dd0 = dd1;
				i += 2;
			}
#pragma pipeloop(0)
			for (; i <= amount - 2; i += 2) {
				sd10 = vis_ld_d64_nf(spp1); spp1++;
				sd20 = vis_ld_d64_nf(spp2); spp2++;
				MLIB_V_SUBIMAGE_S32(sd10, sd20, dd1);
				(*dpp++) = vis_faligndata(dd0, dd1);
				dd0 = dd1;
			}

			if (i < amount) {
				emask = vis_edge32(dpp, dend);
				sd10 = vis_ld_d64_nf(spp1); spp1++;
				sd20 = vis_ld_d64_nf(spp2); spp2++;
				MLIB_V_SUBIMAGE_S32(sd10, sd20, dd1);
				dd = vis_faligndata(dd0, dd1);
				vis_pst_32(dd, dpp, emask);
			}

			sp1 = sl1 += stride1;
			sp2 = sl2 += stride2;
			dp = dl += strided;
		}
	} else {
/* common case */

		for (j = 0; j < height; j++) {

/* prepare the destination addresses */
			dpp = (mlib_d64 *)vis_alignaddr(dp, 0);
			i = (mlib_s32 *)dpp - dp;

			dend = dp + amount - 1;
/* generate edge mask for the start point */
			emask = vis_edge32(dp, dend);

			if (emask != 3) {
				spp1 = (mlib_d64 *)vis_alignaddr(sp1, 4 * i);
				sd10 = vis_faligndata(vis_ld_d64_nf(spp1),
					vis_ld_d64_nf(spp1 + 1));
				spp2 = (mlib_d64 *)vis_alignaddr(sp2, 4 * i);
				sd20 = vis_faligndata(vis_ld_d64_nf(spp2),
					vis_ld_d64_nf(spp2 + 1));
				MLIB_V_SUBIMAGE_S32(sd10, sd20, dd);
				vis_pst_32(dd, dpp++, emask);
				i += 2;
			}

/* copy src1 to dst */
			spp1 = (mlib_d64 *)vis_alignaddr(sp1, 4 * i);
			sd11 = vis_ld_d64_nf(spp1);
			tmp_ptr = dpp;

#pragma pipeloop(0)
			for (k = i; k <= (amount - 2); k += 2) {
				sd10 = sd11;
				sd11 = vis_ld_d64_nf(spp1 + 1);
				(*tmp_ptr++) = vis_faligndata(sd10, sd11);
				spp1++;
			}

			sd11 = vis_faligndata(sd11, vis_ld_d64_nf(spp1 + 1));

			spp2 = (mlib_d64 *)vis_alignaddr(sp2, 4 * i);
			sd20 = vis_ld_d64_nf(spp2);
			tmp_ptr = dpp;

#pragma pipeloop(0)
			for (; i <= amount - 2; i += 2) {
				sd10 = (*tmp_ptr++);
				sd21 = vis_ld_d64_nf(spp2 + 1);
				sd20 = vis_faligndata(sd20, sd21);
				MLIB_V_SUBIMAGE_S32(sd10, sd20, dd);
				(*dpp++) = dd;
				sd20 = sd21;
				spp2++;
			}

			if (i < amount) {
				emask = vis_edge32(dpp, dend);
				sd20 = vis_faligndata(sd20,
					vis_ld_d64_nf(spp2 + 1));
				MLIB_V_SUBIMAGE_S32(sd11, sd20, dd);
				vis_pst_32(dd, dpp, emask);
			}

			sp1 = sl1 += stride1;
			sp2 = sl2 += stride2;
			dp = dl += strided;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
