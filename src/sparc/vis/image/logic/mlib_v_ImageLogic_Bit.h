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

#ifndef _MLIB_V_IMAGELOGIC_BIT_H
#define	_MLIB_V_IMAGELOGIC_BIT_H

#pragma ident	"@(#)mlib_v_ImageLogic_Bit.h	9.3	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#include <mlib_image.h>
#include <vis_proto.h>
#include <mlib_ImageCheck.h>
#include <mlib_v_ImageLogic_proto.h>

/*
 * Functions for MLIB_VIS version bit image logical functions.
 */

static void mlib_v_aligned_dst_src1(
    mlib_u8 *dp,
    mlib_u8 *sp1,
    mlib_u8 *sp2,
    mlib_s32 amount);

static void mlib_v_aligned_dst_src2(
    mlib_u8 *dp,
    mlib_u8 *sp1,
    mlib_u8 *sp2,
    mlib_s32 amount);

static void mlib_v_aligned_src1_src2(
    mlib_u8 *dp,
    mlib_u8 *sp1,
    mlib_u8 *sp2,
    mlib_s32 amount);

static void mlib_v_nonaligned(
    mlib_u8 *dp,
    mlib_u8 *sp1,
    mlib_u8 *sp2,
    mlib_s32 amount);

/* *********************************************************** */

#define	VALIDATE()                                              \
	mlib_u8 *sp1, *sl1;                                     \
	                                                        \
/*                                                              \
 * pointers for pixel and line of source                        \
 */                                                             \
	mlib_u8 *sp2, *sl2;                                     \
	                                                        \
/*                                                              \
 * pointers for pixel and line of source                        \
 */                                                             \
	mlib_u8 *dp, *dl;                                       \
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
	mlib_s32 bitoffsetd;                                    \
	                                                        \
/*                                                              \
 * for dst                                                      \
 */                                                             \
	MLIB_IMAGE_SIZE_EQUAL(dst, src1);                       \
	MLIB_IMAGE_TYPE_EQUAL(dst, src1);                       \
	MLIB_IMAGE_CHAN_EQUAL(dst, src1);                       \
	MLIB_IMAGE_SIZE_EQUAL(dst, src2);                       \
	MLIB_IMAGE_TYPE_EQUAL(dst, src2);                       \
	MLIB_IMAGE_CHAN_EQUAL(dst, src2);                       \
	dp = (mlib_u8 *)mlib_ImageGetData(dst);                 \
	sp1 = (mlib_u8 *)mlib_ImageGetData(src1);               \
	sp2 = (mlib_u8 *)mlib_ImageGetData(src2);               \
	height = mlib_ImageGetHeight(dst);                      \
	width = mlib_ImageGetWidth(dst);                        \
	stride1 = mlib_ImageGetStride(src1);                    \
	stride2 = mlib_ImageGetStride(src2);                    \
	strided = mlib_ImageGetStride(dst);                     \
	channels = mlib_ImageGetChannels(dst);                  \
	bitoffsetd = mlib_ImageGetBitOffset(dst);               \
	width *= channels;                                      \
	if (width > stride1 * 8 || width > stride2 * 8 ||       \
	    width > strided * 8)                                \
	    return MLIB_FAILURE

/* *********************************************************** */

LOGIC_BITS {
	mlib_s32 i, j;
	mlib_s32 offdst, offsrc1, offsrc2, mask, emask;
	mlib_s32 amount, dend_offset;
	mlib_d64 *dpp, *spp2, *spp1;
	mlib_d64 dd, sd10, sd20;
	mlib_u8 *dend;
	mlib_u8 tmp, mask1;

	VALIDATE();

	if ((width == stride1 * 8) && (width == stride2 * 8) &&
	    (width == strided * 8)) {

		amount = height * width;
		dend_offset = (bitoffsetd + amount) & 7;

		if (bitoffsetd != 0) {
			mask1 = (0xFF >> bitoffsetd);

			if (amount < 8 - bitoffsetd) {
				mask1 =
				    mask1 & (0xFF << (8 - bitoffsetd - amount));
				tmp = C_LOGIC(*sp1, *sp2);
				*dp = (((*dp) & (~mask1)) | (tmp & mask1));
				return (MLIB_SUCCESS);
			}

			tmp = C_LOGIC(*sp1, *sp2);
			*dp = (((*dp) & (~mask1)) | (tmp & mask1));
			sp1++;
			sp2++;
			dp++;
			amount -= (8 - bitoffsetd);
		}

		if (amount >= 8) {
			amount >>= 3;
			dend = dp + amount - 1;

			offdst = ((mlib_addr)dp) & 7;
			offsrc1 = ((mlib_addr)sp1) & 7;
			offsrc2 = ((mlib_addr)sp2) & 7;
			mask = ((offsrc1 ^ offsrc2) << 8) |
			    ((offdst ^ offsrc2) << 4) | (offdst ^ offsrc1);

/* offdst = offsrc1 = offsrc2 */
			if (mask == 0) {

/* prepare the destination addresses */
				dpp = (mlib_d64 *)vis_alignaddr(dp, 0);
				i = (mlib_u8 *)dpp - dp;

/* prepare the source addresses */
				spp1 = (mlib_d64 *)vis_alignaddr(sp1, 0);
				spp2 = (mlib_d64 *)vis_alignaddr(sp2, 0);

/* generate edge mask for the start point */
				emask = vis_edge8(dp, dend);

				if (emask != 0xff) {
					sd10 = vis_ld_d64_nf(spp1);
					spp1++;
					sd20 = vis_ld_d64_nf(spp2);
					spp2++;
					dd = VIS_LOGIC(sd20, sd10);
					vis_pst_8(dd, dpp++, emask);
					i += 8;
				}
#pragma pipeloop(0)
				for (; i <= amount - 8; i += 8) {
					sd10 = (*spp1++);
					sd20 = (*spp2++);
					(*dpp++) = VIS_LOGIC(sd20, sd10);
				}

				if (i < amount) {
					emask = vis_edge8(dpp, dend);
					sd10 = vis_ld_d64_nf(spp1);
					sd20 = vis_ld_d64_nf(spp2);
					dd = VIS_LOGIC(sd20, sd10);
					vis_pst_8(dd, dpp, emask);
				}

			} else if ((mask & 0xF) == 0) {
/* offdst = offsrc1 != offsrc2 */

				mlib_v_aligned_dst_src1(dp, sp1, sp2, amount);

			} else if ((mask & 0xF0) == 0) {
/* offdst = offsrc2 != offsrc1 */

				mlib_v_aligned_dst_src2(dp, sp1, sp2, amount);

			} else if ((mask & 0xF00) == 0) {
/* offsrc1 = offsrc2 != offdst */

				mlib_v_aligned_src1_src2(dp, sp1, sp2, amount);

			} else {
/* offdst != offsrc1 != offsrc2 */

				mlib_v_nonaligned(dp, sp1, sp2, amount);
			}

			sp1 += amount;
			sp2 += amount;
			dp = dend + 1;
		}

		if (dend_offset != 0) {
			tmp = C_LOGIC(*sp1, *sp2);
			mask1 = (0xFF << (8 - dend_offset));
			*dp = (((*dp) & (~mask1)) | (tmp & mask1));
		}
	} else {

		sl1 = sp1;
		sl2 = sp2;
		dl = dp;

		dend_offset = (bitoffsetd + width) & 7;

		for (j = 0; j < height; j++) {

			amount = width;

			if (bitoffsetd != 0) {
				mask1 = (0xFF >> bitoffsetd);

				if (amount < 8 - bitoffsetd) {
					mask1 =
					    mask1 & (0xFF << (8 - bitoffsetd -
					    amount));
					tmp = C_LOGIC(*sp1, *sp2);
					*dp =
					    (((*dp) & (~mask1)) | (tmp &
					    mask1));
					sp1 = sl1 += stride1;
					sp2 = sl2 += stride2;
					dp = dl += strided;
					continue;
				}

				tmp = C_LOGIC(*sp1, *sp2);
				*dp = (((*dp) & (~mask1)) | (tmp & mask1));
				sp1++;
				sp2++;
				dp++;
				amount -= (8 - bitoffsetd);
			}

			if (amount >= 8) {
				amount >>= 3;
				dend = dp + amount - 1;

				offdst = ((mlib_addr)dp) & 7;
				offsrc1 = ((mlib_addr)sp1) & 7;
				offsrc2 = ((mlib_addr)sp2) & 7;
				mask = ((offsrc1 ^ offsrc2) << 8) |
				    ((offdst ^ offsrc2) << 4) | (offdst ^
				    offsrc1);

/* offdst = offsrc1 = offsrc2 */
				if (mask == 0) {

/* prepare the destination addresses */
					dpp = (mlib_d64 *)vis_alignaddr(dp, 0);
					i = (mlib_u8 *)dpp - dp;

/* prepare the source addresses */
					spp1 =
					    (mlib_d64 *)vis_alignaddr(sp1, 0);
					spp2 =
					    (mlib_d64 *)vis_alignaddr(sp2, 0);

/* generate edge mask for the start point */
					emask = vis_edge8(dp, dend);

					if (emask != 0xff) {
						sd10 = vis_ld_d64_nf(spp1);
						spp1++;
						sd20 = vis_ld_d64_nf(spp2);
						spp2++;
						dd = VIS_LOGIC(sd20, sd10);
						vis_pst_8(dd, dpp++, emask);
						i += 8;
					}
#pragma pipeloop(0)
					for (; i <= amount - 8; i += 8) {
						sd10 = (*spp1++);
						sd20 = (*spp2++);
						(*dpp++) =
						    VIS_LOGIC(sd20, sd10);
					}

					if (i < amount) {
						emask = vis_edge8(dpp, dend);
						sd10 = vis_ld_d64_nf(spp1);
						sd20 = vis_ld_d64_nf(spp2);
						dd = VIS_LOGIC(sd20, sd10);
						vis_pst_8(dd, dpp, emask);
					}

				} else if ((mask & 0xF) == 0) {
/* offdst = offsrc1 != offsrc2 */

					mlib_v_aligned_dst_src1(dp, sp1, sp2,
					    amount);

				} else if ((mask & 0xF0) == 0) {
/* offdst = offsrc2 != offsrc1 */

					mlib_v_aligned_dst_src2(dp, sp1, sp2,
					    amount);

				} else if ((mask & 0xF00) == 0) {
/* offsrc1 = offsrc2 != offdst */

					mlib_v_aligned_src1_src2(dp, sp1, sp2,
					    amount);

				} else {
/* offdst != offsrc1 != offsrc2 */

					mlib_v_nonaligned(dp, sp1, sp2,
					    amount);
				}

				sp1 += amount;
				sp2 += amount;
				dp = dend + 1;
			}

			if (dend_offset != 0) {
				tmp = C_LOGIC(*sp1, *sp2);
				mask1 = (0xFF << (8 - dend_offset));
				*dp = (((*dp) & (~mask1)) | (tmp & mask1));
			}

			sp1 = sl1 += stride1;
			sp2 = sl2 += stride2;
			dp = dl += strided;
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

static void
mlib_v_aligned_dst_src1(
    mlib_u8 *dp,
    mlib_u8 *sp1,
    mlib_u8 *sp2,
    mlib_s32 amount)
{
	mlib_s32 i;
	mlib_s32 emask;
	mlib_d64 *dpp, *spp2, *spp1;
	mlib_d64 dd, sd10, sd20, sd21;
	mlib_u8 *dend;

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
		sd10 = vis_ld_d64_nf(spp1);
		spp1++;
		sd21 = vis_ld_d64_nf(spp2 + 1);
		sd20 = vis_faligndata(sd20, sd21);
		dd = VIS_LOGIC(sd20, sd10);
		vis_pst_8(dd, dpp++, emask);
		sd20 = sd21;
		spp2++;
		i += 8;
	}
#pragma pipeloop(0)
	for (; i <= amount - 8; i += 8) {
		sd10 = (*spp1++);
		sd21 = spp2[1];
		sd20 = vis_faligndata(sd20, sd21);
		(*dpp++) = VIS_LOGIC(sd20, sd10);
		sd20 = sd21;
		spp2++;
	}

	if (i < amount) {
		emask = vis_edge8(dpp, dend);
		sd10 = vis_ld_d64_nf(spp1);
		sd20 = vis_faligndata(sd20, vis_ld_d64_nf(spp2 + 1));
		dd = VIS_LOGIC(sd20, sd10);
		vis_pst_8(dd, dpp, emask);
	}
}

/* *********************************************************** */

static void
mlib_v_aligned_dst_src2(
    mlib_u8 *dp,
    mlib_u8 *sp1,
    mlib_u8 *sp2,
    mlib_s32 amount)
{
	mlib_s32 i;
	mlib_s32 emask;
	mlib_d64 *dpp, *spp2, *spp1;
	mlib_d64 dd, sd10, sd11, sd20;
	mlib_u8 *dend;

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
		sd20 = vis_ld_d64_nf(spp2);
		spp2++;
		sd11 = vis_ld_d64_nf(spp1 + 1);
		sd10 = vis_faligndata(sd10, sd11);
		dd = VIS_LOGIC(sd20, sd10);
		vis_pst_8(dd, dpp++, emask);
		sd10 = sd11;
		spp1++;
		i += 8;
	}
#pragma pipeloop(0)
	for (; i <= amount - 8; i += 8) {
		sd20 = (*spp2++);
		sd11 = spp1[1];
		sd10 = vis_faligndata(sd10, sd11);
		(*dpp++) = VIS_LOGIC(sd20, sd10);
		sd10 = sd11;
		spp1++;
	}

	if (i < amount) {
		emask = vis_edge8(dpp, dend);
		sd20 = vis_ld_d64_nf(spp2);
		sd10 = vis_faligndata(sd10, vis_ld_d64_nf(spp1 + 1));
		dd = VIS_LOGIC(sd20, sd10);
		vis_pst_8(dd, dpp, emask);
	}
}

/* *********************************************************** */

static void
mlib_v_aligned_src1_src2(
    mlib_u8 *dp,
    mlib_u8 *sp1,
    mlib_u8 *sp2,
    mlib_s32 amount)
{
	mlib_s32 i;
	mlib_s32 emask;
	mlib_d64 *dpp, *spp2, *spp1;
	mlib_d64 dd, sd10, dd0, sd20, dd1;
	mlib_u8 *dend;

/* prepare the source addresses */
	dpp = (mlib_d64 *)vis_alignaddr(dp, 0);
	i = (mlib_u8 *)dpp - dp;

/* prepare the destination addresses */
	spp1 = (mlib_d64 *)vis_alignaddr(sp1, i);
	spp2 = (mlib_d64 *)vis_alignaddr(sp2, i);

	dend = dp + amount - 1;
/* generate edge mask for the start point */
	emask = vis_edge8(dp, dend);

	sd10 = vis_ld_d64_nf(spp1);
	spp1++;
	sd20 = vis_ld_d64_nf(spp2);
	spp2++;
	dd0 = VIS_LOGIC(sd20, sd10);

	if (emask != 0xff) {
		sd10 = vis_ld_d64_nf(spp1);
		spp1++;
		sd20 = vis_ld_d64_nf(spp2);
		spp2++;
		dd1 = VIS_LOGIC(sd20, sd10);
		dd = vis_faligndata(dd0, dd1);
		vis_pst_8(dd, dpp++, emask);
		dd0 = dd1;
		i += 8;
	}
#pragma pipeloop(0)
	for (; i <= amount - 8; i += 8) {
		sd10 = (*spp1++);
		sd20 = (*spp2++);
		dd1 = VIS_LOGIC(sd20, sd10);
		(*dpp++) = vis_faligndata(dd0, dd1);
		dd0 = dd1;
	}

	if (i < amount) {
		emask = vis_edge8(dpp, dend);
		sd10 = vis_ld_d64_nf(spp1);
		sd20 = vis_ld_d64_nf(spp2);
		dd1 = VIS_LOGIC(sd20, sd10);
		dd = vis_faligndata(dd0, dd1);
		vis_pst_8(dd, dpp, emask);
	}
}

/* *********************************************************** */

static void
mlib_v_nonaligned(
    mlib_u8 *dp,
    mlib_u8 *sp1,
    mlib_u8 *sp2,
    mlib_s32 amount)
{
	mlib_s32 i, k;
	mlib_s32 emask;
	mlib_d64 *dpp, *spp2, *spp1, *tmp_ptr;
	mlib_d64 dd, sd10, sd11, sd20, sd21;
	mlib_u8 *dend;

/* prepare the destination addresses */
	dpp = (mlib_d64 *)vis_alignaddr(dp, 0);
	i = (mlib_u8 *)dpp - dp;

	dend = dp + amount - 1;
/* generate edge mask for the start point */
	emask = vis_edge8(dp, dend);

	if (emask != 0xff) {
		spp1 = (mlib_d64 *)vis_alignaddr(sp1, i);
		sd10 =
		    vis_faligndata(vis_ld_d64_nf(spp1),
		    vis_ld_d64_nf(spp1 + 1));
		spp2 = (mlib_d64 *)vis_alignaddr(sp2, i);
		sd20 =
		    vis_faligndata(vis_ld_d64_nf(spp2),
		    vis_ld_d64_nf(spp2 + 1));
		dd = VIS_LOGIC(sd20, sd10);
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
		sd11 = spp1[1];
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
		sd21 = spp2[1];
		sd20 = vis_faligndata(sd20, sd21);
		(*dpp++) = VIS_LOGIC(sd20, sd10);
		sd20 = sd21;
		spp2++;
	}

	if (i < amount) {
		emask = vis_edge8(dpp, dend);
		sd20 = vis_faligndata(sd20, vis_ld_d64_nf(spp2 + 1));
		dd = VIS_LOGIC(sd20, sd11);
		vis_pst_8(dd, dpp, emask);
	}
}

/* *********************************************************** */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_V_IMAGELOGIC_BIT_H */
