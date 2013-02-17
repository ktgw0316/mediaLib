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

#pragma ident	"@(#)mlib_v_ImageAveFunc.c	9.4	07/11/05 SMI"

/*
 * Internal function
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_v_ImageAveFunc.h>

/* *********************************************************** */

#define	VALIDATE(FORMAT)                                        \
	FORMAT *sp1;                                            \
	                                                        \
/*                                                              \
 * pointer for pixel of source                                  \
 */                                                             \
	FORMAT *sp2;                                            \
	                                                        \
/*                                                              \
 * pointer for pixel of source                                  \
 */                                                             \
	FORMAT *dp;                                             \
	                                                        \
/*                                                              \
 * pointer for pixel of dst                                     \
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

#define	MLIB_V_AVEIMAGE_U8(sd1, sd2, dd)                    \
	sd1h = vis_fpmerge(nul, vis_read_hi(sd1));              \
	sd2h = vis_fpmerge(nul, vis_read_hi(sd2));              \
	sd1l = vis_fmul8x16al(vis_read_lo(sd1), fone);          \
	sd2l = vis_fmul8x16al(vis_read_lo(sd2), fone);          \
	rdh = vis_fpadd16(sd1h, sd2h);                          \
	rdl = vis_fpadd16(sd1l, sd2l);                          \
	rzh = vis_fpadd16(rdh, inc);                            \
	rzl = vis_fpadd16(rdl, inc);                            \
	dd = vis_fpack16_pair(rzh, rzl)

/* *********************************************************** */

mlib_status
mlib_v_ImageAve_U8(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2)
{

	mlib_s32 i, j, k;
	mlib_s32 offdst, offsrc1, offsrc2, emask;
	mlib_s32 amount;
	mlib_d64 *dpp, *spp2, *spp1, *tmp_ptr;
	mlib_d64 dd, dd0, dd1, sd10, sd11, sd20, sd21;
	mlib_d64 sd1h, sd2h, sd1l, sd2l, rdh, rdl, rzh, rzl;
	mlib_u8 *dend;
	mlib_f32 nul = vis_to_float(0), fone = vis_to_float(0x100);
	mlib_d64 inc = vis_to_double_dup(0x00010001);

/* pointer for line of source */
	mlib_u8 *sl1;

/* pointer for line of source */
	mlib_u8 *sl2;

/* pointer for line of dst */
	mlib_u8 *dl;

	VALIDATE(mlib_u8);

/* initialize GSR scale factor */
	vis_write_gsr(6 << 3);

	sl1 = sp1;
	sl2 = sp2;
	dl = dp;

	amount = width * channels;

	offdst = ((mlib_addr)dp) & 7;
	offsrc1 = ((mlib_addr)sp1) & 7;
	offsrc2 = ((mlib_addr)sp2) & 7;

	if ((offdst == offsrc1) && (offdst == offsrc2) &&
	    (((strided ^ stride1) & 7) == 0) &&
	    (((strided ^ stride2) & 7) == 0)) {

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
				MLIB_V_AVEIMAGE_U8(sd10, sd20, dd);
				vis_pst_8(dd, dpp++, emask);
				i += 8;
			}
#pragma pipeloop(0)
			for (; i <= amount - 8; i += 8) {
				sd10 = (*spp1++);
				sd20 = (*spp2++);
				MLIB_V_AVEIMAGE_U8(sd10, sd20, dd);
				(*dpp++) = dd;
			}

			if (i < amount) {
				emask = vis_edge8(dpp, dend);
				sd10 = (*spp1++);
				sd20 = (*spp2++);
				MLIB_V_AVEIMAGE_U8(sd10, sd20, dd);
				vis_pst_8(dd, dpp, emask);
			}

			sp1 = sl1 += stride1;
			sp2 = sl2 += stride2;
			dp = dl += strided;
		}
	} else if ((offdst == offsrc1) && (((strided ^ stride1) & 7) == 0)) {

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
				MLIB_V_AVEIMAGE_U8(sd10, sd20, dd);
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
				MLIB_V_AVEIMAGE_U8(sd10, sd20, dd);
				(*dpp++) = dd;
				sd20 = sd21;
				spp2++;
			}

			if (i < amount) {
				emask = vis_edge8(dpp, dend);
				sd10 = (*spp1++);
				sd20 = vis_faligndata(sd20,
					vis_ld_d64_nf(spp2 + 1));
				MLIB_V_AVEIMAGE_U8(sd10, sd20, dd);
				vis_pst_8(dd, dpp, emask);
			}

			sp1 = sl1 += stride1;
			sp2 = sl2 += stride2;
			dp = dl += strided;
		}
	} else if ((offdst == offsrc2) && (((strided ^ stride2) & 7) == 0)) {

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
				MLIB_V_AVEIMAGE_U8(sd10, sd20, dd);
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
				MLIB_V_AVEIMAGE_U8(sd10, sd20, dd);
				(*dpp++) = dd;
				sd10 = sd11;
				spp1++;
			}

			if (i < amount) {
				emask = vis_edge8(dpp, dend);
				sd20 = (*spp2++);
				sd10 = vis_faligndata(sd10,
					vis_ld_d64_nf(spp1 + 1));
				MLIB_V_AVEIMAGE_U8(sd10, sd20, dd);
				vis_pst_8(dd, dpp, emask);
			}

			sp1 = sl1 += stride1;
			sp2 = sl2 += stride2;
			dp = dl += strided;
		}
	} else if ((offsrc1 == offsrc2) && (((stride1 ^ stride2) & 7) == 0)) {

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
			MLIB_V_AVEIMAGE_U8(sd10, sd20, dd0);

			if (emask != 0xff) {
				sd10 = vis_ld_d64_nf(spp1); spp1++;
				sd20 = vis_ld_d64_nf(spp2); spp2++;
				MLIB_V_AVEIMAGE_U8(sd10, sd20, dd1);
				dd = vis_faligndata(dd0, dd1);
				vis_pst_8(dd, dpp++, emask);
				dd0 = dd1;
				i += 8;
			}
#pragma pipeloop(0)
			for (; i <= amount - 8; i += 8) {
				sd10 = (*spp1++);
				sd20 = (*spp2++);
				MLIB_V_AVEIMAGE_U8(sd10, sd20, dd1);
				(*dpp++) = vis_faligndata(dd0, dd1);
				dd0 = dd1;
			}

			if (i < amount) {
				emask = vis_edge8(dpp, dend);
				sd10 = vis_ld_d64_nf(spp1); spp1++;
				sd20 = vis_ld_d64_nf(spp2); spp2++;
				MLIB_V_AVEIMAGE_U8(sd10, sd20, dd1);
				dd = vis_faligndata(dd0, dd1);
				vis_pst_8(dd, dpp, emask);
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
				MLIB_V_AVEIMAGE_U8(sd10, sd20, dd);
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
				MLIB_V_AVEIMAGE_U8(sd10, sd20, dd);
				(*dpp++) = dd;
				sd20 = sd21;
				spp2++;
			}

			if (i < amount) {
				emask = vis_edge8(dpp, dend);
				sd20 = vis_faligndata(sd20,
					vis_ld_d64_nf(spp2 + 1));
				MLIB_V_AVEIMAGE_U8(sd11, sd20, dd);
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

#if MLIB_VIS < 0x300

#define	MLIB_V_AVEIMAGE_S16(sd1, sd2, dd)	\
	inc = vis_fand(sd1, sd2);	\
	inc = vis_fand(inc, roundmask);	\
	dz =                                                               \
	vis_fpadd16(vis_fmul8x16(scale, sd1),	\
			vis_fmul8x16(scale, sd2));	\
	dd = vis_fpsub16(dz, inc);

#else /* MLIB_VIS < 0x300 */

#define	MLIB_V_AVEIMAGE_S16(sd1, sd2, dd)                                  \
	dd = vis_fmean16(sd1, sd2)

#endif /* MLIB_VIS < 0x300 */

/* *********************************************************** */

mlib_status
mlib_v_ImageAve_S16(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2)
{
	mlib_s32 i, j, k;
	mlib_s32 offdst, offsrc1, offsrc2, emask;
	mlib_s32 amount;
	mlib_d64 *dpp, *spp2, *spp1, *tmp_ptr;
	mlib_d64 dd, dd0, dd1, sd10, sd11, sd20, sd21, sdd1, inc, dz;
	mlib_s16 *dend;

#if MLIB_VIS < 0x300
	mlib_f32 scale = vis_to_float(0x80808080);
	mlib_d64 roundmask = vis_to_double_dup(0x00010001);
#endif /* MLIB_VIS < 0x300 */

/* pointer for line of source */
	mlib_s16 *sl1;

/* pointer for line of source */
	mlib_s16 *sl2;

/* pointer for line of dst */
	mlib_s16 *dl;

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
				MLIB_V_AVEIMAGE_S16(sd10, sd20, dd);
				vis_pst_16(dd, dpp++, emask);
				i += 4;
			}
#pragma pipeloop(0)
			for (; i <= amount - 4; i += 4) {
				sd10 = (*spp1++);
				sd20 = (*spp2++);
				MLIB_V_AVEIMAGE_S16(sd10, sd20, dd);
				(*dpp++) = dd;
			}

			if (i < amount) {
				emask = vis_edge16(dpp, dend);
				sd10 = (*spp1++);
				sd20 = (*spp2++);
				MLIB_V_AVEIMAGE_S16(sd10, sd20, dd);
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
				MLIB_V_AVEIMAGE_S16(sd10, sd20, dd);
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
				MLIB_V_AVEIMAGE_S16(sd10, sd20, dd);
				(*dpp++) = dd;
				sd20 = sd21;
				spp2++;
			}

			if (i < amount) {
				emask = vis_edge16(dpp, dend);
				sd10 = (*spp1++);
				sd20 = vis_faligndata(sd20,
					vis_ld_d64_nf(spp2 + 1));
				MLIB_V_AVEIMAGE_S16(sd10, sd20, dd);
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
				MLIB_V_AVEIMAGE_S16(sd10, sd20, dd);
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
				MLIB_V_AVEIMAGE_S16(sd10, sd20, dd);
				(*dpp++) = dd;
				sd10 = sd11;
				spp1++;
			}

			if (i < amount) {
				emask = vis_edge16(dpp, dend);
				sd20 = (*spp2++);
				sd10 = vis_faligndata(sd10,
					vis_ld_d64_nf(spp1 + 1));
				MLIB_V_AVEIMAGE_S16(sd10, sd20, dd);
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
			MLIB_V_AVEIMAGE_S16(sd10, sd20, dd0);

			if (emask != 0xf) {
				sd10 = vis_ld_d64_nf(spp1); spp1++;
				sd20 = vis_ld_d64_nf(spp2); spp2++;
				MLIB_V_AVEIMAGE_S16(sd10, sd20, dd1);
				dd = vis_faligndata(dd0, dd1);
				vis_pst_16(dd, dpp++, emask);
				dd0 = dd1;
				i += 4;
			}
#pragma pipeloop(0)
			for (; i <= amount - 4; i += 4) {
				sd10 = vis_ld_d64_nf(spp1); spp1++;
				sd20 = vis_ld_d64_nf(spp2); spp2++;
				MLIB_V_AVEIMAGE_S16(sd10, sd20, dd1);
				(*dpp++) = vis_faligndata(dd0, dd1);
				dd0 = dd1;
			}

			if (i < amount) {
				emask = vis_edge16(dpp, dend);
				sd10 = vis_ld_d64_nf(spp1); spp1++;
				sd20 = vis_ld_d64_nf(spp2); spp2++;
				MLIB_V_AVEIMAGE_S16(sd10, sd20, dd1);
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
				MLIB_V_AVEIMAGE_S16(sd10, sd20, dd);
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
				sd11 = spp1[1];
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
				MLIB_V_AVEIMAGE_S16(sd10, sd20, dd);
				(*dpp++) = dd;
				sd20 = sd21;
				spp2++;
			}

			if (i < amount) {
				emask = vis_edge16(dpp, dend);
				sd20 = vis_faligndata(sd20,
					vis_ld_d64_nf(spp2 + 1));
				MLIB_V_AVEIMAGE_S16(sd11, sd20, dd);
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

#if MLIB_VIS < 0x300

#define	MLIB_V_AVEIMAGE_U16(sd1, sd2, dd)                            \
	sdd1 = vis_fandnot(anti_rounding_mask, sd1);                 \
	sdd1 = vis_fxor(sdd1, half_offset);                          \
	sd2 = vis_fxor(sd2, half_offset);                            \
	dd =                                                         \
	vis_fpadd16(vis_fmul8x16(scale, sdd1), vis_fmul8x16(scale,   \
	    sd2));                                                   \
	dd = vis_fxor(dd, half_offset)

#else /* MLIB_VIS < 0x300 */

#define	MLIB_V_AVEIMAGE_U16(sd1, sd2, dd)                            \
	sdd1 = vis_fxor(sd1, half_offset);                           \
	sd2 = vis_fxor(sd2, half_offset);                            \
	dd = vis_fmean16(sdd1, sd2);                                 \
	dd = vis_fxor(dd, half_offset)

#endif /* MLIB_VIS < 0x300 */

/* *********************************************************** */

mlib_status
mlib_v_ImageAve_U16(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2)
{
	mlib_s32 i, j, k;
	mlib_s32 offdst, offsrc1, offsrc2, emask;
	mlib_s32 amount;
	mlib_d64 *dpp, *spp2, *spp1, *tmp_ptr;
	mlib_d64 dd, dd0, dd1, sd10, sd11, sd20, sd21, sdd1;
	mlib_u16 *dend;

#if MLIB_VIS < 0x300
	mlib_f32 scale = vis_to_float(0x80808080);
	mlib_d64 anti_rounding_mask = vis_to_double_dup(0x10001);
#endif /* MLIB_VIS < 0x300 */

	mlib_d64 half_offset = vis_to_double_dup(0x80008000);

/* pointer for line of source */
	mlib_u16 *sl1;

/* pointer for line of source */
	mlib_u16 *sl2;

/* pointer for line of dst */
	mlib_u16 *dl;

	VALIDATE(mlib_u16);

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
				MLIB_V_AVEIMAGE_U16(sd10, sd20, dd);
				vis_pst_16(dd, dpp++, emask);
				i += 4;
			}
#pragma pipeloop(0)
			for (; i <= amount - 4; i += 4) {
				sd10 = (*spp1++);
				sd20 = (*spp2++);
				MLIB_V_AVEIMAGE_U16(sd10, sd20, dd);
				(*dpp++) = dd;
			}

			if (i < amount) {
				emask = vis_edge16(dpp, dend);
				sd10 = (*spp1++);
				sd20 = (*spp2++);
				MLIB_V_AVEIMAGE_U16(sd10, sd20, dd);
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
				MLIB_V_AVEIMAGE_U16(sd10, sd20, dd);
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
				MLIB_V_AVEIMAGE_U16(sd10, sd20, dd);
				(*dpp++) = dd;
				sd20 = sd21;
				spp2++;
			}

			if (i < amount) {
				emask = vis_edge16(dpp, dend);
				sd10 = (*spp1++);
				sd20 = vis_faligndata(sd20,
					vis_ld_d64_nf(spp2 + 1));
				MLIB_V_AVEIMAGE_U16(sd10, sd20, dd);
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
				MLIB_V_AVEIMAGE_U16(sd10, sd20, dd);
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
				MLIB_V_AVEIMAGE_U16(sd10, sd20, dd);
				(*dpp++) = dd;
				sd10 = sd11;
				spp1++;
			}

			if (i < amount) {
				emask = vis_edge16(dpp, dend);
				sd20 = (*spp2++);
				sd10 = vis_faligndata(sd10,
					vis_ld_d64_nf(spp1 + 1));
				MLIB_V_AVEIMAGE_U16(sd10, sd20, dd);
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
			i = (mlib_u16 *)dpp - dp;

/* prepare the destination addresses */
			spp1 = (mlib_d64 *)vis_alignaddr(sp1, 2 * i);
			spp2 = (mlib_d64 *)vis_alignaddr(sp2, 2 * i);

			dend = dp + amount - 1;
/* generate edge mask for the start point */
			emask = vis_edge16(dp, dend);

			sd10 = vis_ld_d64_nf(spp1); spp1++;
			sd20 = vis_ld_d64_nf(spp2); spp2++;
			MLIB_V_AVEIMAGE_U16(sd10, sd20, dd0);

			if (emask != 0xf) {
				sd10 = vis_ld_d64_nf(spp1); spp1++;
				sd20 = vis_ld_d64_nf(spp2); spp2++;
				MLIB_V_AVEIMAGE_U16(sd10, sd20, dd1);
				dd = vis_faligndata(dd0, dd1);
				vis_pst_16(dd, dpp++, emask);
				dd0 = dd1;
				i += 4;
			}
#pragma pipeloop(0)
			for (; i <= amount - 4; i += 4) {
				sd10 = vis_ld_d64_nf(spp1); spp1++;
				sd20 = vis_ld_d64_nf(spp2); spp2++;
				MLIB_V_AVEIMAGE_U16(sd10, sd20, dd1);
				(*dpp++) = vis_faligndata(dd0, dd1);
				dd0 = dd1;
			}

			if (i < amount) {
				emask = vis_edge16(dpp, dend);
				sd10 = vis_ld_d64_nf(spp1); spp1++;
				sd20 = vis_ld_d64_nf(spp2); spp2++;
				MLIB_V_AVEIMAGE_U16(sd10, sd20, dd1);
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
				MLIB_V_AVEIMAGE_U16(sd10, sd20, dd);
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
				MLIB_V_AVEIMAGE_U16(sd10, sd20, dd);
				(*dpp++) = dd;
				sd20 = sd21;
				spp2++;
			}

			if (i < amount) {
				emask = vis_edge16(dpp, dend);
				sd20 = vis_faligndata(sd20,
					vis_ld_d64_nf(spp2 + 1));
				MLIB_V_AVEIMAGE_U16(sd11, sd20, dd);
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

mlib_status
mlib_v_ImageAve_S32(
    mlib_image *dst,
    const mlib_image *src1,
    const mlib_image *src2)
{
	mlib_s32 i, j, amount;

	VALIDATE(mlib_s32);

	amount = width * channels;

	for (j = 0; j < height; j++) {
#pragma pipeloop(0)
		for (i = 0; i < amount; i++) {
			((mlib_s32 *)dp)[i] =
			    ((((mlib_s32 *)sp1)[i]) >> 1) +
			    ((((mlib_s32 *)sp2)[i]) >> 1) +
			    (((((mlib_s32 *)sp1)[i]) & 0x1) |
			    ((((mlib_s32 *)sp2)[i]) & 0x1));
		}

		sp1 += stride1;
		sp2 += stride2;
		dp += strided;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
