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

#pragma ident	"@(#)mlib_v_ImageChannelCopy_f.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *  mlib_v_ImageChannelCopy_U8_2_A8D1X4
 *  mlib_v_ImageChannelCopy_U8_2_A8D2X4
 *  mlib_v_ImageChannelCopy_U8_2
 *  mlib_v_ImageChannelCopy_U8_3_A8D1
 *  mlib_v_ImageChannelCopy_U8_3_A8D2
 *  mlib_v_ImageChannelCopy_U8_3
 *  mlib_v_ImageChannelCopy_U8_4_A8D1X2
 *  mlib_v_ImageChannelCopy_U8_4_A8D2X2
 *  mlib_v_ImageChannelCopy_U8_4
 *  mlib_v_ImageChannelCopy_S16_2_A8D1X2
 *  mlib_v_ImageChannelCopy_S16_2_A8D2X2
 *  mlib_v_ImageChannelCopy_S16_2
 *  mlib_v_ImageChannelCopy_S16_3_A8D1
 *  mlib_v_ImageChannelCopy_S16_3_A8D2
 *  mlib_v_ImageChannelCopy_S16_3
 *  mlib_v_ImageChannelCopy_S16_4_A8D1
 *  mlib_v_ImageChannelCopy_S16_4_A8D2
 *  mlib_v_ImageChannelCopy_S16_4
 *  mlib_v_ImageChannelCopy_S32_2_A8D1
 *  mlib_v_ImageChannelCopy_S32_2_A8D2
 *  mlib_v_ImageChannelCopy_S32_2
 *  mlib_v_ImageChannelCopy_S32_3_A8D1
 *  mlib_v_ImageChannelCopy_S32_3_A8D2
 *  mlib_v_ImageChannelCopy_S32_3
 *  mlib_v_ImageChannelCopy_S32_4_A8D1
 *  mlib_v_ImageChannelCopy_S32_4_A8D2
 *  mlib_v_ImageChannelCopy_S32_4
 *  mlib_v_ImageChannelCopy_s32
 *  mlib_v_ImageChannelCopy_D64
 *
 * DESCRIPTION
 *  Copy the selected channels of the source image into the
 *  destination image.
 *
 * NOTE
 *  These functions are separated from mlib_v_ImageChannelCopy.c
 *  for loop unrolling and structure clarity.
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_v_ImageChannelCopy.h>

/* *********************************************************** */

#define	LOAD_SOURCE_8_FIRST                                     \
	                                                        \
/*                                                              \
 * first 8-byte                                                 \
 */                                                             \
	s0 = vis_ld_d64_nf(sp);                                 \
	s1 = vis_ld_d64_nf(sp + 1);                             \
	sd = vis_faligndata(s0, s1)

/* *********************************************************** */

#define	LOAD_SOURCE_8_NEXT                                      \
	s0 = s1;                                                \
	s1 = sp[1];                                             \
	sd = vis_faligndata(s0, s1)

/* *********************************************************** */

#define	LOAD_SOURCE_8_NEXT_NF                                   \
	s0 = s1;                                                \
	s1 = vis_ld_d64_nf(sp + 1);                             \
	sd = vis_faligndata(s0, s1)

/* *********************************************************** */

#define	MLIB_V_IMAGECHANNELCOPY_U8(sd, chanmask, emask)         \
	vis_pst_8(sd, dp, (chanmask & emask));                  \
	sp++;                                                   \
	dp++;

/* *********************************************************** */

#define	MLIB_V_IMAGECHANNELCOPY_U8_A8(sd, chanmask)             \
	vis_pst_8(sd, dp, chanmask);                            \
	sp++;                                                   \
	dp++

/* *********************************************************** */

void
mlib_v_ImageChannelCopy_U8_2_A8D1X4(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
/* aligned src address */
	mlib_d64 *sp;

/* aligned dst address */
	mlib_d64 *dp;

/* channel masks */
	mlib_s32 cm0, chanmask;

/* loop variables */
	mlib_s32 i;

/* prepare the channel mask */
	cm0 = cmask & 0x03;
	chanmask = (cm0 << 6) | (cm0 << 4) | (cm0 << 2) | cm0;

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;

/* 4-pixel loop */
#pragma pipeloop(0)
	for (i = 0; i < dsize / 4; i++) {
		vis_pst_8((*sp++), dp++, chanmask);
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelCopy_U8_2_A8D2X4(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
/* aligned src address */
	mlib_d64 *sp, *sl;

/* aligned dst address */
	mlib_d64 *dp, *dl;

/* channel masks */
	mlib_s32 cm0, chanmask;

/* loop variables */
	mlib_s32 i, j;

/* prepare the channel mask */
	cm0 = cmask & 0x03;
	chanmask = (cm0 << 6) | (cm0 << 4) | (cm0 << 2) | cm0;

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_d64 *)dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* 4-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i < xsize / 4; i++) {
			vis_pst_8((*sp++), dp++, chanmask);
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelCopy_U8_2(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
/* aligned src address */
	mlib_d64 *sp;

/* aligned dst address */
	mlib_d64 *dp;

/* pointer to begin of a src line */
	mlib_u8 *sl;

/* pointer to begin of a dst line */
	mlib_u8 *dl;

/* pointer to end of a dst line */
	mlib_u8 *dend, *dend2;

/* source data */
	mlib_d64 sd, s0, s1;

/* channel masks */
	mlib_s32 cm0, c[2], chanmask;

/* edge mask */
	mlib_s32 emask;

/* offset in dst address */
	mlib_s32 off;

/* loop variables */
	mlib_s32 i, j, n;

/* prepare the channel mask */
	cm0 = cmask & 0x03;
	c[0] = (cm0 << 6) | (cm0 << 4) | (cm0 << 2) | cm0;
	c[1] = ((c[0] << 1) | (cm0 >> 1)) & 0xff;

	sl = (void *)src;
	dl = dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* prepare the destination address */
		dp = (mlib_d64 *)((mlib_addr)dl & (~7));
		off = (mlib_addr)dp - (mlib_addr)dl;
		dend = dl + 2 * xsize - 1;
		dend2 = dend - 7;

/* prepare the source address */
		sp = (mlib_d64 *)vis_alignaddr(sl, off);

/* prepare the channel mask */
		chanmask = c[(8 + off) % 2];

/* generate edge mask for the start point */
		emask = vis_edge8(dl, dend);

/* first-8-byte (4 pixels) */
		LOAD_SOURCE_8_FIRST;
		MLIB_V_IMAGECHANNELCOPY_U8(sd, chanmask, emask);

		if ((mlib_addr)dp <= (mlib_addr)dend2) {
			n = ((mlib_u8 *)dend2 - (mlib_u8 *)dp) / 8;

/* 4-pixel column loop, emask not needed */
#pragma pipeloop(0)
			for (i = 0; i < n; i++) {
				LOAD_SOURCE_8_NEXT;
				MLIB_V_IMAGECHANNELCOPY_U8_A8(sd, chanmask);
			}

			LOAD_SOURCE_8_NEXT_NF;
			MLIB_V_IMAGECHANNELCOPY_U8_A8(sd, chanmask);
		}

/* end point handling */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			emask = vis_edge8(dp, dend);
			LOAD_SOURCE_8_NEXT_NF;
			MLIB_V_IMAGECHANNELCOPY_U8(sd, chanmask, emask);
		}

		sl += slb;
		dl += dlb;
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelCopy_U8_3_A8D1(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
/* aligned src address */
	mlib_d64 *sp;

/* aligned dst address */
	mlib_d64 *dp;

/* channel mask */
	mlib_s32 cm0;

/* channel masks */
	mlib_s32 cmask0, cmask1, cmask2;

/* loop variables */
	mlib_s32 i;

/* prepare the channel masks */
	cm0 = cmask & 0x07;
	cmask0 = ((cm0 << 5) | (cm0 << 2) | (cm0 >> 1)) & 0xff;
	cmask1 = ((cm0 << 7) | (cm0 << 4) | (cm0 << 1) | (cm0 >> 2)) & 0xff;
	cmask2 = ((cm0 << 6) | (cm0 << 3) | cm0) & 0xff;

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;

/* 8-pixel column loop */
#pragma pipeloop(0)
	for (i = 0; i < dsize / 8; i++) {
		vis_pst_8((*sp++), dp++, cmask0);
		vis_pst_8((*sp++), dp++, cmask1);
		vis_pst_8((*sp++), dp++, cmask2);
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelCopy_U8_3_A8D2(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
/* aligned src address */
	mlib_d64 *sp, *sl;

/* aligned dst address */
	mlib_d64 *dp, *dl;

/* channel mask */
	mlib_s32 cm0;

/* channel masks */
	mlib_s32 cmask0, cmask1, cmask2;

/* loop variables */
	mlib_s32 i, j;

/* prepare the channel masks */
	cm0 = cmask & 0x07;
	cmask0 = ((cm0 << 5) | (cm0 << 2) | (cm0 >> 1)) & 0xff;
	cmask1 = ((cm0 << 7) | (cm0 << 4) | (cm0 << 1) | (cm0 >> 2)) & 0xff;
	cmask2 = ((cm0 << 6) | (cm0 << 3) | cm0) & 0xff;

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_d64 *)dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* 8-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i < xsize / 8; i++) {
			vis_pst_8((*sp++), dp++, cmask0);
			vis_pst_8((*sp++), dp++, cmask1);
			vis_pst_8((*sp++), dp++, cmask2);
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelCopy_U8_3(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
/* aligned src address */
	mlib_d64 *sp;

/* aligned dst address */
	mlib_d64 *dp;

/* pointer to src line */
	mlib_u8 *sl;

/* pointer to dst line */
	mlib_u8 *dl;

/* pointer to end of dst */
	mlib_u8 *dend, *dend2;

/* source data */
	mlib_d64 sd, s0, s1;

/* channel masks */
	mlib_s32 cm0, c[3];

/* channel masks */
	mlib_s32 cmask0, cmask1, cmask2;

/* edge masks */
	mlib_s32 emask;

/* offset of address in dst */
	mlib_s32 off;

/* loop variables */
	mlib_s32 i, j, n;

/* prepare the channel masks */
	cm0 = cmask & 0x07;
	c[0] = ((cm0 << 5) | (cm0 << 2) | (cm0 >> 1)) & 0xff;
	c[1] = ((cm0 << 7) | (cm0 << 4) | (cm0 << 1) | (cm0 >> 2)) & 0xff;
	c[2] = ((cm0 << 6) | (cm0 << 3) | cm0) & 0xff;

	sl = (void *)src;
	dl = dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* prepare the destination address */
		dp = (mlib_d64 *)((mlib_addr)dl & (~7));
		off = (mlib_addr)dp - (mlib_addr)dl;
		dend = dl + 3 * xsize - 1;
		dend2 = dend - 23;

/* prepare the source address */
		sp = (mlib_d64 *)vis_alignaddr(sl, off);

/* prepare the channel masks */
		cmask0 = c[(9 - off) % 3];
		cmask1 = c[(10 - off) % 3];
		cmask2 = c[(11 - off) % 3];

/* generate edge mask for the start point */
		emask = vis_edge8(dl, dend);

/* first-8-byte */
		LOAD_SOURCE_8_FIRST;
		MLIB_V_IMAGECHANNELCOPY_U8(sd, cmask0, emask);

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			emask = vis_edge8(dp, dend);
			LOAD_SOURCE_8_NEXT_NF;
			MLIB_V_IMAGECHANNELCOPY_U8(sd, cmask1, emask);

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge8(dp, dend);
				LOAD_SOURCE_8_NEXT_NF;
				MLIB_V_IMAGECHANNELCOPY_U8(sd, cmask2, emask);
			}
		}

		if ((mlib_addr)dp <= (mlib_addr)dend2) {
/* calculate the number of loops */
			n = ((mlib_u8 *)dend2 - (mlib_u8 *)dp) / 24;

/* 8-pixel column loop */
#pragma pipeloop(0)
			for (i = 0; i < n; i++) {
				LOAD_SOURCE_8_NEXT;
				MLIB_V_IMAGECHANNELCOPY_U8_A8(sd, cmask0);

				LOAD_SOURCE_8_NEXT;
				MLIB_V_IMAGECHANNELCOPY_U8_A8(sd, cmask1);

				LOAD_SOURCE_8_NEXT;
				MLIB_V_IMAGECHANNELCOPY_U8_A8(sd, cmask2);
			}

			LOAD_SOURCE_8_NEXT;
			MLIB_V_IMAGECHANNELCOPY_U8_A8(sd, cmask0);

			LOAD_SOURCE_8_NEXT;
			MLIB_V_IMAGECHANNELCOPY_U8_A8(sd, cmask1);

			LOAD_SOURCE_8_NEXT_NF;
			MLIB_V_IMAGECHANNELCOPY_U8_A8(sd, cmask2);
		}

/* end point handling */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			emask = vis_edge8(dp, dend);
			LOAD_SOURCE_8_NEXT_NF;
			MLIB_V_IMAGECHANNELCOPY_U8(sd, cmask0, emask);

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge8(dp, dend);
				LOAD_SOURCE_8_NEXT_NF;
				MLIB_V_IMAGECHANNELCOPY_U8(sd, cmask1, emask);

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					emask = vis_edge8(dp, dend);
					LOAD_SOURCE_8_NEXT_NF;
					MLIB_V_IMAGECHANNELCOPY_U8(sd, cmask2,
					    emask);
				}
			}
		}

		sl += slb;
		dl += dlb;
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelCopy_U8_4_A8D1X2(
    const mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
/* aligned src address */
	mlib_d64 *sp;

/* aligned dst address */
	mlib_d64 *dp;

/* channel masks */
	mlib_s32 cm0, chanmask;

/* loop variables */
	mlib_s32 i;

/* prepare the channel mask */
	cm0 = cmask & 0x0f;
	chanmask = (cm0 << 4) | cm0;

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;

/* 2-pixel loop */
#pragma pipeloop(0)
	for (i = 0; i < dsize / 2; i++) {
		vis_pst_8((*sp++), dp++, chanmask);
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelCopy_U8_4_A8D2X2(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
/* aligned src address */
	mlib_d64 *sp, *sl;

/* aligned dst address */
	mlib_d64 *dp, *dl;

/* channel masks */
	mlib_s32 cm0, chanmask;

/* loop variables */
	mlib_s32 i, j;

/* prepare the channel mask */
	cm0 = cmask & 0x0f;
	chanmask = (cm0 << 4) | cm0;

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_d64 *)dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* 2-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i < xsize / 2; i++) {
			vis_pst_8((*sp++), dp++, chanmask);
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelCopy_U8_4(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
/* aligned src address */
	mlib_d64 *sp;

/* aligned dst address */
	mlib_d64 *dp;

/* pointer to begin of a src line */
	mlib_u8 *sl;

/* pointer to begin of a dst line */
	mlib_u8 *dl;

/* pointer to end of a dst line */
	mlib_u8 *dend, *dend2;

/* source data */
	mlib_d64 sd, s0, s1;

/* channel masks */
	mlib_s32 cm0, c[4], chanmask;

/* edge mask */
	mlib_s32 emask;

/* offset in dst address */
	mlib_s32 off;

/* loop variables */
	mlib_s32 i, j, n;

/* prepare the channel mask */
	cm0 = cmask & 0x0f;
	c[0] = (cm0 << 4) | cm0;
	c[1] = ((cm0 << 5) | (cm0 << 1) | (cm0 >> 3)) & 0xff;
	c[2] = ((cm0 << 6) | (cm0 << 2) | (cm0 >> 2)) & 0xff;
	c[3] = ((cm0 << 7) | (cm0 << 3) | (cm0 >> 1)) & 0xff;

	sl = (void *)src;
	dl = dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* prepare the destination address */
		dp = (mlib_d64 *)((mlib_addr)dl & (~7));
		off = (mlib_addr)dp - (mlib_addr)dl;
		dend = dl + 4 * xsize - 1;
		dend2 = dend - 7;

/* prepare the source address */
		sp = (mlib_d64 *)vis_alignaddr(sl, off);

/* prepare the channel mask */
		chanmask = c[(8 + off) % 4];

/* generate edge mask for the start point */
		emask = vis_edge8(dl, dend);

/* first-8-byte (2 pixels) */
		LOAD_SOURCE_8_FIRST;
		MLIB_V_IMAGECHANNELCOPY_U8(sd, chanmask, emask);

		if ((mlib_addr)dp <= (mlib_addr)dend2) {
			n = ((mlib_u8 *)dend2 - (mlib_u8 *)dp) / 8;

/* 2-pixel column loop, no emask needed */
#pragma pipeloop(0)
			for (i = 0; i < n; i++) {
				LOAD_SOURCE_8_NEXT;
				MLIB_V_IMAGECHANNELCOPY_U8_A8(sd, chanmask);
			}

			LOAD_SOURCE_8_NEXT_NF;
			MLIB_V_IMAGECHANNELCOPY_U8_A8(sd, chanmask);
		}

/* end point handling */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			emask = vis_edge8(dp, dend);
			LOAD_SOURCE_8_NEXT_NF;
			MLIB_V_IMAGECHANNELCOPY_U8(sd, chanmask, emask);
		}

		sl += slb;
		dl += dlb;
	}
}

/* *********************************************************** */

#define	MLIB_V_IMAGECHANNELCOPY_S16(sd, chanmask, emask)        \
	vis_pst_16(sd, dp, (chanmask & emask));                 \
	sp++;                                                   \
	dp++

/* *********************************************************** */

#define	MLIB_V_IMAGECHANNELCOPY_S16_A8(sd, chanmask)            \
	vis_pst_16(sd, dp, chanmask);                           \
	sp++;                                                   \
	dp++

/* *********************************************************** */

void
mlib_v_ImageChannelCopy_S16_2_A8D1X2(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
/* aligned src address */
	mlib_d64 *sp;

/* aligned dst address */
	mlib_d64 *dp;

/* channel masks */
	mlib_s32 cm0, chanmask;

/* loop variables */
	mlib_s32 i;

/* prepare the channel mask */
	cm0 = cmask & 0x03;
	chanmask = (cm0 << 2) | cm0;

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;

/* 2-pixel loop */
#pragma pipeloop(0)
	for (i = 0; i < dsize / 2; i++) {
		vis_pst_16((*sp++), dp++, chanmask);
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelCopy_S16_2_A8D2X2(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
/* aligned src address */
	mlib_d64 *sp, *sl;

/* aligned dst address */
	mlib_d64 *dp, *dl;

/* channel masks */
	mlib_s32 cm0, chanmask;

/* loop variables */
	mlib_s32 i, j;

/* prepare the channel mask */
	cm0 = cmask & 0x03;
	chanmask = (cm0 << 2) | cm0;

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_d64 *)dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* 2-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i < xsize / 2; i++) {
			vis_pst_16((*sp++), dp++, chanmask);
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelCopy_S16_2(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
/* aligned src address */
	mlib_d64 *sp;

/* aligned dst address */
	mlib_d64 *dp;

/* pointer to begin of a src line */
	mlib_s16 *sl;

/* pointer to begin of a dst line */
	mlib_s16 *dl;

/* pointer to end of a dst line */
	mlib_s16 *dend, *dend2;

/* source data */
	mlib_d64 sd, s0, s1;

/* channel masks */
	mlib_s32 cm0, c[2], chanmask;

/* edge mask */
	mlib_s32 emask;

/* offset in dst address */
	mlib_s32 off, off2;

/* loop variables */
	mlib_s32 i, j, n;

/* prepare the channel mask */
	cm0 = cmask & 0x03;
	c[0] = (cm0 << 2) | cm0;
	c[1] = ((cm0 << 3) | (cm0 << 1) | (cm0 >> 1)) & 0x0f;

	sl = (void *)src;
	dl = dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* prepare the destination address */
		dp = (mlib_d64 *)((mlib_addr)dl & (~7));
		off = (mlib_addr)dp - (mlib_addr)dl;
		off2 = off / 2;
		dend = dl + 2 * xsize - 1;
		dend2 = dend - 3;

/* prepare the source address */
		sp = (mlib_d64 *)vis_alignaddr(sl, off);

/* prepare the channel mask */
		chanmask = c[(8 + off2) % 2];

/* generate edge mask for the start point */
		emask = vis_edge16(dl, dend);

/* first-8-byte (2 pixels) */
		LOAD_SOURCE_8_FIRST;
		MLIB_V_IMAGECHANNELCOPY_S16(sd, chanmask, emask);

		if ((mlib_addr)dp <= (mlib_addr)dend2) {
			n = ((mlib_u8 *)dend2 - (mlib_u8 *)dp) / 8;

/* 2-pixel column loop */
#pragma pipeloop(0)
			for (i = 0; i < n; i++) {
				LOAD_SOURCE_8_NEXT;
				MLIB_V_IMAGECHANNELCOPY_S16_A8(sd, chanmask);
			}

			LOAD_SOURCE_8_NEXT_NF;
			MLIB_V_IMAGECHANNELCOPY_S16_A8(sd, chanmask);
		}

/* end point handling */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			emask = vis_edge16(dp, dend);
			LOAD_SOURCE_8_NEXT_NF;
			MLIB_V_IMAGECHANNELCOPY_S16(sd, chanmask, emask);
		}

		sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
		dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelCopy_S16_3_A8D1(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
/* aligned src address */
	mlib_d64 *sp;

/* aligned dst address */
	mlib_d64 *dp;

/* channel mask */
	mlib_s32 cm0;

/* channel masks */
	mlib_s32 cmask0, cmask1, cmask2;

/* loop variables */
	mlib_s32 i;

/* prepare the channel masks */
	cm0 = cmask & 0x07;
	cmask0 = ((cm0 << 1) | (cm0 >> 2)) & 0x0f;
	cmask1 = ((cm0 << 2) | (cm0 >> 1)) & 0x0f;
	cmask2 = ((cm0 << 3) | cm0) & 0x0f;

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;

/* 4-pixel column loop */
#pragma pipeloop(0)
	for (i = 0; i < dsize / 4; i++) {
		vis_pst_16((*sp++), dp++, cmask0);
		vis_pst_16((*sp++), dp++, cmask1);
		vis_pst_16((*sp++), dp++, cmask2);
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelCopy_S16_3_A8D2(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
/* aligned src address */
	mlib_d64 *sp, *sl;

/* aligned dst address */
	mlib_d64 *dp, *dl;

/* channel mask */
	mlib_s32 cm0;

/* channel masks */
	mlib_s32 cmask0, cmask1, cmask2;

/* loop variables */
	mlib_s32 i, j;

/* prepare the channel masks */
	cm0 = cmask & 0x07;
	cmask0 = ((cm0 << 1) | (cm0 >> 2)) & 0x0f;
	cmask1 = ((cm0 << 2) | (cm0 >> 1)) & 0x0f;
	cmask2 = ((cm0 << 3) | cm0) & 0x0f;

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_d64 *)dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* 4-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i < xsize / 4; i++) {
			vis_pst_16((*sp++), dp++, cmask0);
			vis_pst_16((*sp++), dp++, cmask1);
			vis_pst_16((*sp++), dp++, cmask2);
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelCopy_S16_3(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
/* aligned src address */
	mlib_d64 *sp;

/* aligned dst address */
	mlib_d64 *dp;

/* pointer to src line */
	mlib_s16 *sl;

/* pointer to dst line */
	mlib_s16 *dl;

/* pointer to end of dst */
	mlib_s16 *dend, *dend2;

/* source data */
	mlib_d64 sd, sd0, sd1, sd2;

/* temporaries */
	mlib_d64 s0, s1, s2, s3;

/* channel mask */
	mlib_s32 cm0, c[3];

/* channel masks */
	mlib_s32 cmask0, cmask1, cmask2;

/* edge masks */
	mlib_s32 emask;

/* offset of address in dst */
	mlib_s32 off, off2;

/* loop variables */
	mlib_s32 i, j, n;

/* prepare the channel masks */
	cm0 = cmask & 0x07;
	c[0] = ((cm0 << 1) | (cm0 >> 2)) & 0x0f;
	c[1] = ((cm0 << 2) | (cm0 >> 1)) & 0x0f;
	c[2] = ((cm0 << 3) | cm0) & 0x0f;

	sl = (void *)src;
	dl = dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* prepare the destination address */
		dp = (mlib_d64 *)((mlib_addr)dl & (~7));
		off = (mlib_addr)dp - (mlib_addr)dl;
		off2 = off / 2;
		dend = dl + 3 * xsize - 1;
		dend2 = dend - 11;

/* prepare the channel masks */
		cmask0 = c[(9 + off2) % 3];
		cmask1 = c[(10 + off2) % 3];
		cmask2 = c[(11 + off2) % 3];

/* prepare the source address */
		sp = (mlib_d64 *)vis_alignaddr(sl, off);

/* generate edge mask for the start point */
		emask = vis_edge16(dl, dend);

/* first-8-byte */
		LOAD_SOURCE_8_FIRST;
		MLIB_V_IMAGECHANNELCOPY_S16(sd, cmask0, emask);

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			emask = vis_edge16(dp, dend);
			LOAD_SOURCE_8_NEXT_NF;
			MLIB_V_IMAGECHANNELCOPY_S16(sd, cmask1, emask);

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge16(dp, dend);
				LOAD_SOURCE_8_NEXT_NF;
				MLIB_V_IMAGECHANNELCOPY_S16(sd, cmask2, emask);
			}
		}

		if ((mlib_addr)dp <= (mlib_addr)dend2) {
			n = ((mlib_u8 *)dend2 - (mlib_u8 *)dp) / 24 + 1;

/* 4-pixel column loop, no emask needed */
#pragma pipeloop(0)
			for (i = 0; i < n; i++) {
				s0 = s1;
				s3 = sp[1];
				s2 = sp[2];
				s1 = vis_ld_d64_nf(sp+3);

				sd0 = vis_faligndata(s0, s3);
				sd1 = vis_faligndata(s3, s2);
				sd2 = vis_faligndata(s2, s1);

				vis_pst_16(sd0, dp, cmask0);
				vis_pst_16(sd1, dp + 1, cmask1);
				vis_pst_16(sd2, dp + 2, cmask2);

				sp += 3;
				dp += 3;
			}
		}

/* end point handling */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			emask = vis_edge16(dp, dend);
			LOAD_SOURCE_8_NEXT_NF;
			MLIB_V_IMAGECHANNELCOPY_S16(sd, cmask0, emask);

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge16(dp, dend);
				LOAD_SOURCE_8_NEXT_NF;
				MLIB_V_IMAGECHANNELCOPY_S16(sd, cmask1, emask);

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					emask = vis_edge16(dp, dend);
					LOAD_SOURCE_8_NEXT_NF;
					MLIB_V_IMAGECHANNELCOPY_S16(sd, cmask2,
					    emask);
				}
			}
		}

		sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
		dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelCopy_S16_4_A8D1(
    const mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
/* aligned src address */
	mlib_d64 *sp;

/* aligned dst address */
	mlib_d64 *dp;

/* channel masks */
	mlib_s32 chanmask;

/* loop variables */
	mlib_s32 i;

/* prepare the channel mask */
	chanmask = cmask & 0x0f;

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;

/* 1-pixel loop */
#pragma pipeloop(0)
	for (i = 0; i < dsize; i++) {
		vis_pst_16((*sp++), dp++, chanmask);
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelCopy_S16_4_A8D2(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
/* aligned src address */
	mlib_d64 *sp, *sl;

/* aligned dst address */
	mlib_d64 *dp, *dl;

/* channel masks */
	mlib_s32 chanmask;

/* loop variables */
	mlib_s32 i, j;

/* prepare the channel mask */
	chanmask = cmask & 0x0f;

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_d64 *)dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* 1-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i < xsize; i++) {
			vis_pst_16((*sp++), dp++, chanmask);
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelCopy_S16_4(
    const mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
/* aligned src address */
	mlib_d64 *sp;

/* aligned dst address */
	mlib_d64 *dp;

/* pointer to begin of a src line */
	mlib_s16 *sl;

/* pointer to begin of a dst line */
	mlib_s16 *dl;

/* pointer to end of a dst line */
	mlib_s16 *dend, *dend2;

/* source data */
	mlib_d64 sd, s0, s1;

/* channel masks */
	mlib_s32 cm0, c[4], chanmask;

/* edge mask */
	mlib_s32 emask;

/* offset in dst address */
	mlib_s32 off, off2;

/* loop variables */
	mlib_s32 i, j, n;

/* prepare the channel mask */
	cm0 = cmask & 0x0f;
	c[0] = cm0;
	c[1] = ((cm0 << 1) | (cm0 >> 3)) & 0x0f;
	c[2] = ((cm0 << 2) | (cm0 >> 2)) & 0x0f;
	c[3] = ((cm0 << 3) | (cm0 >> 1)) & 0x0f;

	sl = (void *)src;
	dl = dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* prepare the destination address */
		dp = (mlib_d64 *)((mlib_addr)dl & (~7));
		off = (mlib_addr)dp - (mlib_addr)dl;
		off2 = off / 2;
		dend = dl + 4 * xsize - 1;
		dend2 = dend - 3;

/* prepare the source address */
		sp = (mlib_d64 *)vis_alignaddr(sl, off);

/* prepare the channel mask */
		chanmask = c[(8 + off2) % 4];

/* generate edge mask for the start point */
		emask = vis_edge16(dl, dend);

/* first-8-byte (1 pixel) */
		LOAD_SOURCE_8_FIRST;
		MLIB_V_IMAGECHANNELCOPY_S16(sd, chanmask, emask);

		if ((mlib_addr)dp <= (mlib_addr)dend2) {
			n = ((mlib_u8 *)dend2 - (mlib_u8 *)dp) / 8;

/* 1-pixel column loop, no emask needed */
#pragma pipeloop(0)
			for (i = 0; i < n; i++) {
				LOAD_SOURCE_8_NEXT;
				MLIB_V_IMAGECHANNELCOPY_S16_A8(sd, chanmask);
			}

			LOAD_SOURCE_8_NEXT_NF;
			MLIB_V_IMAGECHANNELCOPY_S16_A8(sd, chanmask);
		}

/* end point handling */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			emask = vis_edge16(dp, dend);
			LOAD_SOURCE_8_NEXT_NF;
			MLIB_V_IMAGECHANNELCOPY_S16(sd, chanmask, emask);
		}

		sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
		dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

#define	MLIB_V_IMAGECHANNELCOPY_S32(sd, chanmask, emask)        \
	vis_pst_32(sd, dp, (chanmask & emask));                 \
	sp++;                                                   \
	dp++

/* *********************************************************** */

#define	MLIB_V_IMAGECHANNELCOPY_S32_A8(sd, chanmask)            \
	vis_pst_32(sd, dp, chanmask);                           \
	sp++;                                                   \
	dp++;

/* *********************************************************** */

#define	MLIB_V_IMAGECHANNELCOPY_S32_LAST(sd, chanmask)          \
	vis_pst_32(sd, dp, chanmask)

/* *********************************************************** */

void
mlib_v_ImageChannelCopy_S32_2_A8D1(
    const mlib_s32 *src,
    mlib_s32 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
/* aligned src address */
	mlib_d64 *sp;

/* aligned dst address */
	mlib_d64 *dp;

/* channel masks */
	mlib_s32 cm0, chanmask;

/* loop variables */
	mlib_s32 i;

/* prepare the channel mask */
	cm0 = cmask & 0x03;
	chanmask = cm0;

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;

/* 2-pixel loop */
#pragma pipeloop(0)
	for (i = 0; i < dsize; i++) {
		vis_pst_32((*sp++), dp++, chanmask);
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelCopy_S32_2_A8D2(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
/* aligned src address */
	mlib_d64 *sp, *sl;

/* aligned dst address */
	mlib_d64 *dp, *dl;

/* channel masks */
	mlib_s32 cm0, chanmask;

/* loop variables */
	mlib_s32 i, j;

/* prepare the channel mask */
	cm0 = cmask & 0x03;
	chanmask = cm0;

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_d64 *)dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* 2-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i < xsize; i++) {
			vis_pst_32((*sp++), dp++, chanmask);
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelCopy_S32_2(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
/* aligned src address */
	mlib_d64 *sp;

/* aligned dst address */
	mlib_d64 *dp;

/* pointer to begin of a src line */
	mlib_s32 *sl;

/* pointer to begin of a dst line */
	mlib_s32 *dl;

/* pointer to end of a dst line */
	mlib_s32 *dend, *dend2;

/* source data */
	mlib_d64 sd, s0, s1;

/* channel masks */
	mlib_s32 cm0, c[2], chanmask;

/* edge mask */
	mlib_s32 emask;

/* offset in dst address */
	mlib_s32 off, off2;

/* loop variables */
	mlib_s32 i, j, n;

/* prepare the channel mask */
	cm0 = cmask & 0x03;
	c[0] = cm0;
	c[1] = ((cm0 << 1) | (cm0 >> 1)) & 0x0f;

	sl = (void *)src;
	dl = dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* prepare the destination address */
		dp = (mlib_d64 *)((mlib_addr)dl & (~7));
		off = (mlib_addr)dp - (mlib_addr)dl;
		off2 = off / 4;
		dend = dl + 2 * xsize - 1;
		dend2 = dend - 1;

/* prepare the channel mask */
		chanmask = c[(8 + off2) % 2];

/* prepare the source address */
		sp = (mlib_d64 *)vis_alignaddr(sl, off);

/* generate edge mask for the start point */
		emask = vis_edge32(dl, dend);

/* first-8-byte (2 pixels) */
		LOAD_SOURCE_8_FIRST;
		MLIB_V_IMAGECHANNELCOPY_S32(sd, chanmask, emask);

		if ((mlib_addr)dp <= (mlib_addr)dend2) {
			n = ((mlib_u8 *)dend2 - (mlib_u8 *)dp) / 8;

/* 2-pixel column loop */
#pragma pipeloop(0)
			for (i = 0; i < n; i++) {
				LOAD_SOURCE_8_NEXT;
				MLIB_V_IMAGECHANNELCOPY_S32_A8(sd, chanmask);
			}

			LOAD_SOURCE_8_NEXT_NF;
			MLIB_V_IMAGECHANNELCOPY_S32_A8(sd, chanmask);
		}

/* end point handling */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			emask = vis_edge32(dp, dend);
			LOAD_SOURCE_8_NEXT_NF;
			MLIB_V_IMAGECHANNELCOPY_S32(sd, chanmask, emask);
		}

		sl = (mlib_s32 *)((mlib_u8 *)sl + slb);
		dl = (mlib_s32 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelCopy_S32_3_A8D1(
    const mlib_s32 *src,
    mlib_s32 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
/* aligned src address */
	mlib_d64 *sp;

/* aligned dst address */
	mlib_d64 *dp;

/* channel mask */
	mlib_s32 cm0;

/* channel masks */
	mlib_s32 cmask0, cmask1, cmask2;

/* loop variables */
	mlib_s32 i;

/* prepare the channel masks */
	cm0 = cmask & 0x07;
	cmask0 = cm0 >> 1;
	cmask1 = (cm0 << 1) | (cm0 >> 2);
	cmask2 = cm0;

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;

/* 4-pixel column loop */
#pragma pipeloop(0)
	for (i = 0; i < dsize / 2; i++) {
		vis_pst_32((*sp++), dp++, cmask0);
		vis_pst_32((*sp++), dp++, cmask1);
		vis_pst_32((*sp++), dp++, cmask2);
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelCopy_S32_3_A8D2(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
/* aligned src address */
	mlib_d64 *sp, *sl;

/* aligned dst address */
	mlib_d64 *dp, *dl;

/* channel mask */
	mlib_s32 cm0;

/* channel masks */
	mlib_s32 cmask0, cmask1, cmask2;

/* loop variables */
	mlib_s32 i, j;

/* prepare the channel masks */
	cm0 = cmask & 0x07;
	cmask0 = cm0 >> 1;
	cmask1 = (cm0 << 1) | (cm0 >> 2);
	cmask2 = cm0;

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_d64 *)dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* 4-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i < xsize / 2; i++) {
			vis_pst_32((*sp++), dp++, cmask0);
			vis_pst_32((*sp++), dp++, cmask1);
			vis_pst_32((*sp++), dp++, cmask2);
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelCopy_S32_3(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
/* aligned src address */
	mlib_d64 *sp;

/* aligned dst address */
	mlib_d64 *dp;

/* pointer to src line */
	mlib_s32 *sl;

/* pointer to dst line */
	mlib_s32 *dl;

/* pointer to end of dst */
	mlib_s32 *dend, *dend2;

/* source data */
	mlib_d64 sd, sd0, sd1, sd2;

/* temporaries */
	mlib_d64 s0, s1, s2, s3;

/* channel mask */
	mlib_s32 cm0, c[3];

/* channel masks */
	mlib_s32 cmask0, cmask1, cmask2;

/* edge masks */
	mlib_s32 emask;

/* offset of address in dst */
	mlib_s32 off, off2;

/* loop variables */
	mlib_s32 i, j, n;

/* prepare the channel masks */
	cm0 = cmask & 0x07;
	c[0] = cm0 >> 1;
	c[1] = (cm0 << 1) | (cm0 >> 2);
	c[2] = cm0;

	sl = (void *)src;
	dl = dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* prepare the destination address */
		dp = (mlib_d64 *)((mlib_addr)dl & (~7));
		off = (mlib_addr)dp - (mlib_addr)dl;
		off2 = off / 2;
		dend = dl + 3 * xsize - 1;
		dend2 = dend - 6;

/* prepare the channel masks */
		cmask0 = c[(9 + off2) % 3];
		cmask1 = c[(10 + off2) % 3];
		cmask2 = c[(11 + off2) % 3];

/* prepare the source address */
		sp = (mlib_d64 *)vis_alignaddr(sl, off);

/* generate edge mask for the start point */
		emask = vis_edge32(dl, dend);

/* first-8-byte */
		LOAD_SOURCE_8_FIRST;
		MLIB_V_IMAGECHANNELCOPY_S32(sd, cmask0, emask);

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			emask = vis_edge32(dp, dend);
			LOAD_SOURCE_8_NEXT_NF;
			MLIB_V_IMAGECHANNELCOPY_S32(sd, cmask1, emask);

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge32(dp, dend);
				LOAD_SOURCE_8_NEXT_NF;
				MLIB_V_IMAGECHANNELCOPY_S32(sd, cmask2, emask);
			}
		}

		if ((mlib_addr)dp <= (mlib_addr)dend2) {
			n = ((mlib_u8 *)dend2 - (mlib_u8 *)dp) / 24 + 1;

/* 4-pixel column loop, no emask needed */
#pragma pipeloop(0)
			for (i = 0; i < n; i++) {
				s0 = s1;
				s3 = sp[1];
				s2 = sp[2];
				s1 = sp[3];

				sd0 = vis_faligndata(s0, s3);
				sd1 = vis_faligndata(s3, s2);
				sd2 = vis_faligndata(s2, s1);

				vis_pst_32(sd0, dp, cmask0);
				vis_pst_32(sd1, dp + 1, cmask1);
				vis_pst_32(sd2, dp + 2, cmask2);

				sp += 3;
				dp += 3;
			}
		}

/* end point handling */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			emask = vis_edge32(dp, dend);
			LOAD_SOURCE_8_NEXT_NF;
			MLIB_V_IMAGECHANNELCOPY_S32(sd, cmask0, emask);

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge32(dp, dend);
				LOAD_SOURCE_8_NEXT_NF;
				MLIB_V_IMAGECHANNELCOPY_S32(sd, cmask1, emask);

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					emask = vis_edge32(dp, dend);
					LOAD_SOURCE_8_NEXT_NF;
					MLIB_V_IMAGECHANNELCOPY_S32(sd, cmask2,
					    emask);
				}
			}
		}

		sl = (mlib_s32 *)((mlib_u8 *)sl + slb);
		dl = (mlib_s32 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelCopy_S32_4_A8D1(
    const mlib_s32 *src,
    mlib_s32 *dst,
    mlib_s32 dsize,
    mlib_s32 cmask)
{
/* aligned src address */
	mlib_d64 *sp;

/* aligned dst address */
	mlib_d64 *dp;

/* channel masks */
	mlib_s32 cmask1, cmask2;

/* loop variables */
	mlib_s32 i;

/* prepare the channel mask */
	cmask1 = (cmask & 0x0f) >> 2;
	cmask2 = cmask & 0x0f;

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;

/* 1-pixel loop */
#pragma pipeloop(0)
	for (i = 0; i < dsize; i++) {
		vis_pst_32((*sp++), dp++, cmask1);
		vis_pst_32((*sp++), dp++, cmask2);
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelCopy_S32_4_A8D2(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
/* aligned src address */
	mlib_d64 *sp, *sl;

/* aligned dst address */
	mlib_d64 *dp, *dl;

/* channel masks */
	mlib_s32 cmask1, cmask2;

/* loop variables */
	mlib_s32 i, j;

/* prepare the channel mask */
	cmask1 = (cmask & 0x0f) >> 2;
	cmask2 = cmask & 0x0f;

	sp = sl = (mlib_d64 *)src;
	dp = dl = (mlib_d64 *)dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* 1-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i < xsize; i++) {
			vis_pst_32((*sp++), dp++, cmask1);
			vis_pst_32((*sp++), dp++, cmask2);
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelCopy_S32_4(
    const mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 cmask)
{
/* aligned src address */
	mlib_d64 *sp;

/* aligned dst address */
	mlib_d64 *dp;

/* pointer to begin of a src line */
	mlib_s32 *sl;

/* pointer to begin of a dst line */
	mlib_s32 *dl;

/* source data */
	mlib_d64 sd0, sd1, sd2, s0, s1, s2, s3;

/* channel masks */
	mlib_s32 cmask0, cmask1, cmask2;

/* offset in dst address */
	mlib_s32 off, off2;

/* loop variables */
	mlib_s32 i, j;

	sl = (void *)src;
	dl = dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* prepare the destination address */
		dp = (mlib_d64 *)((mlib_addr)dl & (~7));
		off = (mlib_addr)dp - (mlib_addr)dl;
		off2 = off / 4;

/* prepare the source address */
		sp = (mlib_d64 *)vis_alignaddr(sl, off);

/* prepare the channel mask */
		cmask0 = cmask >> (2 - off2);
		cmask1 = cmask >> (-off2);
		cmask2 = (cmask & off2) << 1;

		s2 = vis_ld_d64_nf(sp);
		s3 = vis_ld_d64_nf(sp + 1);
/* 1-pixel column loop, no emask needed */
#pragma pipeloop(0)
		for (i = 0; i < xsize - 1; i++) {
			s0 = s2;
			s1 = s3;
			s2 = sp[2];
			s3 = sp[3];
			sd0 = vis_faligndata(s0, s1);
			sd1 = vis_faligndata(s1, s2);
			sd2 = vis_faligndata(s2, s3);
			vis_pst_32(sd0, dp, cmask0);
			vis_pst_32(sd1, dp + 1, cmask1);
			vis_pst_32(sd2, dp + 2, cmask2);
			dp += 2;
			sp += 2;
		}

		if (i < xsize) {
			s0 = s2;
			s1 = s3;
			s2 = vis_ld_d64_nf(sp + 2);
			s3 = vis_ld_d64_nf(sp + 3);
			sd0 = vis_faligndata(s0, s1);
			sd1 = vis_faligndata(s1, s2);
			sd2 = vis_faligndata(s2, s3);
			vis_pst_32(sd0, dp, cmask0);
			vis_pst_32(sd1, dp + 1, cmask1);
			if (cmask2)
				vis_pst_32(sd2, dp + 2, cmask2);
		}

		sl = (mlib_s32 *)((mlib_u8 *)sl + slb);
		dl = (mlib_s32 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelCopy_s32(
    const mlib_s32 *src,
    mlib_s32 src_stride,
    mlib_s32 *dst,
    mlib_s32 dst_stride,
    mlib_s32 nchannels,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 cmask)
{
	mlib_u8 *srcRow, *dstRow;
	mlib_s32 *pSrc, *pDst;
	mlib_s32 mask = cmask;
	mlib_s32 num2copy = 0;
	mlib_s32 channels[4];
	mlib_s32 i, x, y, cc;

	for (i = (nchannels - 1); i >= 0; i--) {
		if (mask & 1) {
			channels[num2copy] = i * 4;
			num2copy++;
		}

		mask >>= 1;
	}

	for (i = 0; i < num2copy; i++) {
		srcRow = (mlib_u8 *)src;
		dstRow = (mlib_u8 *)dst;

		cc = channels[i];

		for (y = 0; y < src_height; y++) {
			pSrc = (mlib_s32 *)(srcRow + cc);
			pDst = (mlib_s32 *)(dstRow + cc);

#pragma pipeloop(0)
			for (x = 0; x < src_width; x++) {
				*pDst = *pSrc;
				pSrc += nchannels;
				pDst += nchannels;
			}

			srcRow += src_stride;
			dstRow += dst_stride;
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelCopy_D64(
    const mlib_d64 *src,
    mlib_s32 src_stride,
    mlib_d64 *dst,
    mlib_s32 dst_stride,
    mlib_s32 nchannels,
    mlib_s32 src_width,
    mlib_s32 src_height,
    mlib_s32 cmask)
{
	mlib_u8 *srcRow, *dstRow;
	mlib_d64 *pSrc, *pDst;
	mlib_s32 mask = cmask;
	mlib_s32 num2copy = 0;
	mlib_s32 channels[4];
	mlib_s32 i, x, y, cc;

	for (i = (nchannels - 1); i >= 0; i--) {
		if (mask & 1) {
			channels[num2copy] = i * 8;
			num2copy++;
		}

		mask >>= 1;
	}

	for (i = 0; i < num2copy; i++) {
		srcRow = (mlib_u8 *)src;
		dstRow = (mlib_u8 *)dst;

		cc = channels[i];

		for (y = 0; y < src_height; y++) {
			pSrc = (mlib_d64 *)(srcRow + cc);
			pDst = (mlib_d64 *)(dstRow + cc);

#pragma pipeloop(0)
			for (x = 0; x < src_width; x++) {
				*pDst = *pSrc;
				pSrc += nchannels;
				pDst += nchannels;
			}

			srcRow += src_stride;
			dstRow += dst_stride;
		}
	}
}

/* *********************************************************** */
