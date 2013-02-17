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

#pragma ident	"@(#)mlib_v_ImageSqrShift_f.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_v_ImageSqrShift_U8_A8D1X8   - 1-D, Aligned8, dsize 8x
 *      mlib_v_ImageSqrShift_U8_A8D2X8   - 2-D, Aligned8, xsize 8x
 *      mlib_v_ImageSqrShift_U8_D1       - 1-D, not aligned8
 *      mlib_v_ImageSqrShift_U8          - 2-D, not aligned8
 *      mlib_v_ImageSqrShift_S16_A8D1X4  - 1-D, Aligned8, dsize 4x
 *      mlib_v_ImageSqrShift_S16_A8D2X4  - 2-D, Aligned8, xsize 4x
 *      mlib_v_ImageSqrShift_S16_D1      - 1-D, not aligned8
 *      mlib_v_ImageSqrShift_S16         - 2-D, not aligned8
 *      mlib_v_ImageSquare_S16_A8D1X4    - 1-D, Aligned8, dsize 4x
 *      mlib_v_ImageSquare_S16_A8D2X4    - 2-D, Aligned8, xsize 4x
 *      mlib_v_ImageSquare_S16_D1        - 1-D, not aligned8
 *      mlib_v_ImageSquare_S16           - 2-D, not aligned8
 *      mlib_v_ImageSquare_U16_A8D1X4    - 1-D, Aligned8, dsize 4x
 *      mlib_v_ImageSquare_U16_A8D2X4    - 2-D, Aligned8, xsize 4x
 *      mlib_v_ImageSquare_U16_D1        - 1-D, not aligned8
 *      mlib_v_ImageSquare_U16           - 2-D, not aligned8
 *      mlib_v_ImageSqrShift_U16_A8D1X4  - 1-D, Aligned8, dsize 4x
 *      mlib_v_ImageSqrShift_U16_A8D2X4  - 2-D, Aligned8, xsize 4x
 *      mlib_v_ImageSqrShift_U16_D1      - 1-D, not aligned8
 *      mlib_v_ImageSqrShift_U16         - 2-D, not aligned8
 *      mlib_v_ImageSqrSmallShift_U16_A8D1X4  - 1-D, Aligned8, dsize 4x,
 *                                              small shift values
 *      mlib_v_ImageSqrSmallShift_U16_A8D2X4  - 2-D, Aligned8, xsize 4x,
 *                                              small shift values
 *      mlib_v_ImageSqrSmallShift_U16_D1      - 1-D, not aligned8,
 *                                              small shift values
 *      mlib_v_ImageSqrSmallShift_U16         - 2-D, not aligned8,
 *                                              small shift values
 *      mlib_v_ImageSqrShift_S32         - common case
 *
 * SYNOPSIS
 *
 * ARGUMENT
 *      src     pointer to source image data
 *      dst     pointer to destination image data
 *      dsize   data size in pixels
 *      slb     source image line stride in bytes
 *      dlb     destination image line stride in bytes
 *      xsize   image width in pixels
 *      ysize   image height in lines
 *      shift   shift factor
 *
 * DESCRIPTION
 *      dst = (src * src) >> shift
 *      -- VIS2 version low level functions.
 *      These functions are separated from mlib_v_ImageSqrShift.c
 *      for loop unrolling and structure clarity.
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_v_ImageSqrShift_f.h>

/* *********************************************************** */

#define	MLIB_V_IMAGESQRSHIFT_U8(sd, dd)                         \
	sdh = vis_fexpand(vis_read_hi(sd));                     \
	sdl = vis_fexpand(vis_read_lo(sd));                     \
	rdh = vis_fmul8x16(vis_read_hi(sd), sdh);               \
	rdl = vis_fmul8x16(vis_read_lo(sd), sdl);               \
	dd = vis_fpack16_pair(rdh, rdl)

/* *********************************************************** */

#define	MLIB_V_IMAGESQRSHIFT_S16(sd, dd)                            \
	rdhh = vis_fmuld8sux16(vis_read_hi(sd), vis_read_hi(sd));   \
	rdhl = vis_fmuld8ulx16(vis_read_hi(sd), vis_read_hi(sd));   \
	rdh = vis_fpadd32(rdhh, rdhl);                              \
	rdlh = vis_fmuld8sux16(vis_read_lo(sd), vis_read_lo(sd));   \
	rdll = vis_fmuld8ulx16(vis_read_lo(sd), vis_read_lo(sd));   \
	rdl = vis_fpadd32(rdlh, rdll);                              \
	dd = vis_fpackfix_pair(rdh, rdl)

/* *********************************************************** */

#define	MLIB_V_IMAGESQUARE_S16(sd, dd)                          \
	rdh = vis_fmul8sux16(sd, sd);                           \
	rdl = vis_fmul8ulx16(sd, sd);                           \
	dd = vis_fpadd16(rdh, rdl)

/* *********************************************************** */

#define	MLIB_V_IMAGESQUARE_S16_UNROLL(sd, rdh, rdl, dd)         \
	rdh = vis_fmul8sux16(sd, sd);                           \
	rdl = vis_fmul8ulx16(sd, sd);                           \
	dd = vis_fpadd16(rdh, rdl)

/* *********************************************************** */

#define	MLIB_V_IMAGESQRSHIFT_U16(sd, dd)                            \
	sd = vis_fand(sd, mask);                                    \
	sd = vis_fmul8x16(fmin, sd);                                \
	sd = vis_fand(negate, sd);                                  \
	rdhh = vis_fmuld8sux16(vis_read_hi(sd), vis_read_hi(sd));   \
	rdhl = vis_fmuld8ulx16(vis_read_hi(sd), vis_read_hi(sd));   \
	rdh = vis_fpadd32(rdhh, rdhl);                              \
	rdh = vis_fpsub32(rdh, sat_offset);                         \
	rdlh = vis_fmuld8sux16(vis_read_lo(sd), vis_read_lo(sd));   \
	rdll = vis_fmuld8ulx16(vis_read_lo(sd), vis_read_lo(sd));   \
	rdl = vis_fpadd32(rdlh, rdll);                              \
	rdl = vis_fpsub32(rdl, sat_offset);                         \
	dd = vis_fpackfix_pair(rdh, rdl);                           \
	dd = vis_fxor(dd, xor_mask)

/* *********************************************************** */

#define	MLIB_V_IMAGESQUARE_U16(sd, dd)                          \
	sd = vis_fxor(sd, xor_mask);                            \
	rdh = vis_fmul8sux16(sd, sd);                           \
	rdl = vis_fmul8ulx16(sd, sd);                           \
	dd = vis_fpadd16(rdh, rdl);                             \
	dd = vis_fpadd16(dd, sd);                               \
	dd = vis_fpadd16(dd, sat_offset)

/* *********************************************************** */

#define	MLIB_V_IMAGESQRSMALLSHIFT_U16(ud, dd)                           \
	sd = vis_fxor((ud), offset);                                    \
	rdhh = vis_fmuld8sux16(vis_read_hi(sd), vis_read_hi(sd));       \
	rdhl = vis_fmuld8ulx16(vis_read_hi(sd), vis_read_hi(sd));       \
	rdh = vis_fpadd32(rdhh, rdhl);                                  \
	sdad = vis_fmuld8sux16(vis_read_hi(offset), vis_read_hi(sd));   \
	rdh_0 = (mlib_d64)(((mlib_s32 *)(&rdh))[0]);                    \
	rdh_0 -= 2. * (mlib_d64)((mlib_s32 *)(&sdad))[0];               \
	rdh_0 *= dscale;                                                \
	rdh_0 += sat_offset;                                            \
	rdh_1 = (mlib_d64)(((mlib_s32 *)(&rdh))[1]);                    \
	rdh_1 -= 2. * (mlib_d64)((mlib_s32 *)(&sdad))[1];               \
	rdh_1 *= dscale;                                                \
	rdh_1 += sat_offset;                                            \
	rdh = vis_to_double((mlib_s32)(rdh_0), (mlib_s32)(rdh_1));      \
	rdlh = vis_fmuld8sux16(vis_read_lo(sd), vis_read_lo(sd));       \
	rdll = vis_fmuld8ulx16(vis_read_lo(sd), vis_read_lo(sd));       \
	rdl = vis_fpadd32(rdlh, rdll);                                  \
	sdad = vis_fmuld8sux16(vis_read_hi(offset), vis_read_lo(sd));   \
	rdl_0 = (mlib_d64)(((mlib_s32 *)(&rdl))[0]);                    \
	rdl_0 -= 2. * (mlib_d64)((mlib_s32 *)(&sdad))[0];               \
	rdl_0 *= dscale;                                                \
	rdl_0 += sat_offset;                                            \
	rdl_1 = (mlib_d64)(((mlib_s32 *)(&rdl))[1]);                    \
	rdl_1 -= 2. * (mlib_d64)((mlib_s32 *)(&sdad))[1];               \
	rdl_1 *= dscale;                                                \
	rdl_1 += sat_offset;                                            \
	rdl = vis_to_double((mlib_s32)(rdl_0), (mlib_s32)(rdl_1));      \
	dd = vis_fpackfix_pair(rdh, rdl);                               \
	dd = vis_fxor((dd), offset)

/* *********************************************************** */

#define	SCALBN(res, shift)                                      \
	                                                        \
/*                                                              \
 * res = 2 ** (-shift)                                          \
 */                                                             \
	res = 0.;                                               \
	((mlib_s32 *)&res)[0] = (1023 + shift) << 20

/* *********************************************************** */

void
mlib_v_ImageSqrShift_U8_A8D1X8(
    mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize)
{
/* pointer to source image */
	mlib_d64 *sp;

/* pointer to destination image */
	mlib_d64 *dp;

/* source data */
	mlib_d64 sd;

/* destination data */
	mlib_d64 dd;

/* temporaries used in macro */
	mlib_d64 sdh, sdl;

/* temporaries used in macro */
	mlib_d64 rdh, rdl;

/* loop variable */
	mlib_s32 i;

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;

/* 8-pixel loop */
#pragma pipeloop(0)
	for (i = 0; i < (dsize / 8); i++) {
		sd = (*sp++);
		MLIB_V_IMAGESQRSHIFT_U8(sd, dd);
		(*dp++) = dd;
	}
}

/* *********************************************************** */

void
mlib_v_ImageSqrShift_U8_A8D2X8(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
/* pointer to source image */
	mlib_d64 *sp;

/* pointer to a line in source */
	mlib_d64 *sl;

/* pointer to destination image */
	mlib_d64 *dp;

/* pointer to a line in destination */
	mlib_d64 *dl;

/* source data */
	mlib_d64 sd;

/* destination data */
	mlib_d64 dd;

/* temporaries used in macro */
	mlib_d64 sdh, sdl;

/* temporaries used in macro */
	mlib_d64 rdh, rdl;

/* loop variable */
	mlib_s32 i, j;

	sl = sp = (mlib_d64 *)src;
	dl = dp = (mlib_d64 *)dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* 8-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i < (xsize / 8); i++) {
			sd = (*sp++);
			MLIB_V_IMAGESQRSHIFT_U8(sd, dd);
			(*dp++) = dd;
		}

		sl = sp = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dl = dp = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageSqrShift_U8_D1(
    mlib_u8 *src,
    mlib_u8 *dst,
    mlib_s32 dsize)
{
/* pointer to source image */
	mlib_u8 *sa;

/* 8-byte aligned pointer to source image */
	mlib_d64 *sp;

/* pointer to destination image */
	mlib_u8 *da;

/* pointer to end of destination image */
	mlib_u8 *dend;

/* 8-byte aligned pointer to destination image */
	mlib_d64 *dp;

/* offset of address alignment in destination */
	mlib_s32 off;

/* edge masks */
	mlib_s32 emask;

/* source data */
	mlib_d64 s0, s1;

/* source data */
	mlib_d64 sd;

/* destination data */
	mlib_d64 dd;

/* temporaries used in macro */
	mlib_d64 sdh, sdl;

/* temporaries used in macro */
	mlib_d64 rdh, rdl;

/* loop variable */
	mlib_s32 i, n;

	sa = src;
	da = dst;

/* prepare the destination address */
	dp = (mlib_d64 *)((mlib_addr)da & (~7));
	off = (mlib_addr)dp - (mlib_addr)da;
	dend = da + dsize - 1;

/* prepare the source address */
	sp = (mlib_d64 *)vis_alignaddr(sa, off);

/* generate edge mask for the start point */
	emask = vis_edge8(da, dend);

/* first 8 pixels */
	s0 = (*sp++);
	s1 = (*sp++);
	sd = vis_faligndata(s0, s1);
	MLIB_V_IMAGESQRSHIFT_U8(sd, dd);
	vis_pst_8(dd, dp++, emask);

	n = ((mlib_u8 *)(dend + 1) - (mlib_u8 *)dp) / 8;

/* 8-pixel column loop */
#pragma pipeloop(0)
	for (i = 0; i < n; i++) {
		s0 = s1;
		s1 = (*sp++);
		sd = vis_faligndata(s0, s1);
		MLIB_V_IMAGESQRSHIFT_U8(sd, dd);
		(*dp++) = dd;
	}

/* end point handling */

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		emask = vis_edge8(dp, dend);
		s0 = s1;
		s1 = (*sp++);
		sd = vis_faligndata(s0, s1);
		MLIB_V_IMAGESQRSHIFT_U8(sd, dd);
		vis_pst_8(dd, dp++, emask);
	}
}

/* *********************************************************** */

void
mlib_v_ImageSqrShift_U8(
    mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
/* pointer to a line in source image */
	mlib_u8 *sl;

/* 8-byte aligned pointer to source image */
	mlib_d64 *sp;

/* pointer to a line in destination image */
	mlib_u8 *dl;

/* pointer to end of a line in destination image */
	mlib_u8 *dend;

/* 8-byte aligned pointer to destination image */
	mlib_d64 *dp;

/* offset of address alignment in destination */
	mlib_s32 off;

/* edge masks */
	mlib_s32 emask;

/* source data */
	mlib_d64 s0, s1;

/* source data */
	mlib_d64 sd;

/* destination data */
	mlib_d64 dd;

/* temporaries used in macro */
	mlib_d64 sdh, sdl;

/* temporaries used in macro */
	mlib_d64 rdh, rdl;

/* loop variable */
	mlib_s32 i, j, n;

	sl = src;
	dl = dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* prepare the destination address */
		dp = (mlib_d64 *)((mlib_addr)dl & (~7));
		off = (mlib_addr)dp - (mlib_addr)dl;
		dend = dl + xsize - 1;

/* prepare the source address */
		sp = (mlib_d64 *)vis_alignaddr(sl, off);

/* generate edge mask for the start point */
		emask = vis_edge8(dl, dend);

/* first 8 pixels */
		s0 = (*sp++);
		s1 = (*sp++);
		sd = vis_faligndata(s0, s1);
		MLIB_V_IMAGESQRSHIFT_U8(sd, dd);
		vis_pst_8(dd, dp++, emask);

		n = ((mlib_u8 *)(dend + 1) - (mlib_u8 *)dp) / 8;

/* 8-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i < n; i++) {
			s0 = s1;
			s1 = (*sp++);
			sd = vis_faligndata(s0, s1);
			MLIB_V_IMAGESQRSHIFT_U8(sd, dd);
			(*dp++) = dd;
		}

/* end point handling */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			emask = vis_edge8(dp, dend);
			s0 = s1;
			s1 = (*sp++);
			sd = vis_faligndata(s0, s1);
			MLIB_V_IMAGESQRSHIFT_U8(sd, dd);
			vis_pst_8(dd, dp++, emask);
		}

		sl += slb;
		dl += dlb;
	}
}

/* *********************************************************** */

void
mlib_v_ImageSqrShift_S16_A8D1X4(
    mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_s32 shift)
{
/* pointer to source images */
	mlib_d64 *sp;

/* pointer to destination image */
	mlib_d64 *dp;

/* source data */
	mlib_d64 sd;

/* destination data */
	mlib_d64 dd;

/* temporaries used in macro */
	mlib_d64 rdhh, rdhl;

/* temporaries used in macro */
	mlib_d64 rdlh, rdll;

/* temporaries used in macro */
	mlib_d64 rdh, rdl;

/* loop variable */
	mlib_s32 j;

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;

/* 4-pixel loop */
#pragma pipeloop(0)
	for (j = 0; j < (dsize / 4); j++) {
		sd = (*sp++);
		MLIB_V_IMAGESQRSHIFT_S16(sd, dd);
		(*dp++) = dd;
	}
}

/* *********************************************************** */

void
mlib_v_ImageSqrShift_S16_A8D2X4(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 shift)
{
/* pointer to source images */
	mlib_d64 *sp;

/* pointer to a line in source */
	mlib_d64 *sl;

/* pointer to destination image */
	mlib_d64 *dp;

/* pointer to a line in destination */
	mlib_d64 *dl;

/* source data */
	mlib_d64 sd;

/* destination data */
	mlib_d64 dd;

/* temporaries used in macro */
	mlib_d64 rdhh, rdhl;

/* temporaries used in macro */
	mlib_d64 rdlh, rdll;

/* temporaries used in macro */
	mlib_d64 rdh, rdl;

/* loop variable */
	mlib_s32 i, j;

	sl = sp = (mlib_d64 *)src;
	dl = dp = (mlib_d64 *)dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* 4-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i < (xsize / 4); i++) {
			sd = (*sp++);
			MLIB_V_IMAGESQRSHIFT_S16(sd, dd);
			(*dp++) = dd;
		}

		sl = sp = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dl = dp = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageSqrShift_S16_D1(
    mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize,
    mlib_s32 shift)
{
/* aligned pointer to source */
	mlib_d64 *sp;

/* pointer to source */
	mlib_s16 *sa;

/* aligned pointer to destination */
	mlib_d64 *dp;

/* pointer to destination */
	mlib_s16 *da;

/* pointer to end of dst */
	mlib_s16 *dend;

/* offset of address alignment in dst */
	mlib_s32 off;

/* edge masks */
	mlib_s32 emask;

/* source data */
	mlib_d64 s0, s1;

/* source data */
	mlib_d64 sd;

/* destination data */
	mlib_d64 dd;

/* temporaries used in macro */
	mlib_d64 rdhh, rdhl;

/* temporaries used in macro */
	mlib_d64 rdlh, rdll;

/* temporaries used in macro */
	mlib_d64 rdh, rdl;

/* loop variable */
	mlib_s32 i, n;

	sa = src;
	da = dst;

/* prepare the destination address */
	dp = (mlib_d64 *)((mlib_addr)da & (~7));
	off = (mlib_addr)dp - (mlib_addr)da;
	dend = da + dsize - 1;

/* prepare the source address */
	sp = (mlib_d64 *)vis_alignaddr(sa, off);

/* generate edge mask for the start point */
	emask = vis_edge16(da, dend);

/* first 4 pixels */
	s0 = (*sp++);
	s1 = (*sp++);
	sd = vis_faligndata(s0, s1);
	MLIB_V_IMAGESQRSHIFT_S16(sd, dd);
	vis_pst_16(dd, dp++, emask);

	n = ((mlib_u8 *)(dend + 1) - (mlib_u8 *)dp) / 8;

/* 4-pixel column loop */
#pragma pipeloop(0)
	for (i = 0; i < n; i++) {
		s0 = s1;
		s1 = (*sp++);
		sd = vis_faligndata(s0, s1);
		MLIB_V_IMAGESQRSHIFT_S16(sd, dd);
		(*dp++) = dd;
	}

/* end point handling */

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		emask = vis_edge16(dp, dend);
		s0 = s1;
		s1 = (*sp++);
		sd = vis_faligndata(s0, s1);
		MLIB_V_IMAGESQRSHIFT_S16(sd, dd);
		vis_pst_16(dd, dp++, emask);
	}
}

/* *********************************************************** */

void
mlib_v_ImageSqrShift_S16(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 shift)
{
/* aligned pointer to source */
	mlib_d64 *sp;

/* pointer to a line in source */
	mlib_s16 *sl;

/* aligned pointer to destination */
	mlib_d64 *dp;

/* pointer to a line in destination */
	mlib_s16 *dl;

/* pointer to end of a line in dst */
	mlib_s16 *dend;

/* offset of address alignment in dst */
	mlib_s32 off;

/* edge masks */
	mlib_s32 emask;

/* source data */
	mlib_d64 s0, s1;

/* source data */
	mlib_d64 sd;

/* destination data */
	mlib_d64 dd;

/* temporaries used in macro */
	mlib_d64 rdhh, rdhl;

/* temporaries used in macro */
	mlib_d64 rdlh, rdll;

/* temporaries used in macro */
	mlib_d64 rdh, rdl;

/* loop variable */
	mlib_s32 i, j, n;

	sl = src;
	dl = dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* prepare the destination address */
		dp = (mlib_d64 *)((mlib_addr)dl & (~7));
		off = (mlib_addr)dp - (mlib_addr)dl;
		dend = dl + xsize - 1;

/* prepare the source address */
		sp = (mlib_d64 *)vis_alignaddr(sl, off);

/* generate edge mask for the start point */
		emask = vis_edge16(dl, dend);

/* first 4 pixels */
		s0 = (*sp++);
		s1 = (*sp++);
		sd = vis_faligndata(s0, s1);
		MLIB_V_IMAGESQRSHIFT_S16(sd, dd);
		vis_pst_16(dd, dp++, emask);

		n = ((mlib_u8 *)(dend + 1) - (mlib_u8 *)dp) / 8;

/* 4-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i < n; i++) {
			s0 = s1;
			s1 = (*sp++);
			sd = vis_faligndata(s0, s1);
			MLIB_V_IMAGESQRSHIFT_S16(sd, dd);
			(*dp++) = dd;
		}

/* end point handling */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			emask = vis_edge16(dp, dend);
			s0 = s1;
			s1 = (*sp++);
			sd = vis_faligndata(s0, s1);
			MLIB_V_IMAGESQRSHIFT_S16(sd, dd);
			vis_pst_16(dd, dp++, emask);
		}

		sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
		dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageSquare_S16_A8D1X4(
    mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize)
{
/* pointer to source image */
	mlib_d64 *sp;

/* pointer to destination image */
	mlib_d64 *dp;

/* source data */
	mlib_d64 sd, sd0, sd1, sd2;

/* destination data */
	mlib_d64 dd, dd0, dd1, dd2;

/* temporaries used in macro */
	mlib_d64 rdh, rdl, rdh0, rdl0, rdh1, rdl1, rdh2, rdl2;

/* loop variable */
	mlib_s32 j;

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;

/* 4-pixel loop */
#pragma pipeloop(0)
	for (j = 0; j < (dsize / 4) - 3; j += 4) {
		sd = (*sp++);
		MLIB_V_IMAGESQUARE_S16_UNROLL(sd, rdh, rdl, dd);
		(*dp++) = dd;

		sd0 = (*sp++);
		MLIB_V_IMAGESQUARE_S16_UNROLL(sd0, rdh0, rdl0, dd0);
		(*dp++) = dd0;

		sd1 = (*sp++);
		MLIB_V_IMAGESQUARE_S16_UNROLL(sd1, rdh1, rdl1, dd1);
		(*dp++) = dd1;

		sd2 = (*sp++);
		MLIB_V_IMAGESQUARE_S16_UNROLL(sd2, rdh2, rdl2, dd2);
		(*dp++) = dd2;
	}

#pragma pipeloop(0)
	for (; j < (dsize / 4); j++) {
		sd = (*sp++);
		MLIB_V_IMAGESQUARE_S16(sd, dd);
		(*dp++) = dd;
	}
}

/* *********************************************************** */

void
mlib_v_ImageSquare_S16_A8D2X4(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
/* pointer to source image */
	mlib_d64 *sp;

/* pointer to a line in source */
	mlib_d64 *sl;

/* pointer to destination image */
	mlib_d64 *dp;

/* pointer to a line in destination */
	mlib_d64 *dl;

/* source data */
	mlib_d64 sd, sd0, sd1, sd2;

/* destination data */
	mlib_d64 dd, dd0, dd1, dd2;

/* temporaries used in macro */
	mlib_d64 rdh, rdl, rdh0, rdl0, rdh1, rdl1, rdh2, rdl2;

/* loop variable */
	mlib_s32 i, j;

	sl = sp = (mlib_d64 *)src;
	dl = dp = (mlib_d64 *)dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* 4-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i < (xsize / 4) - 3; i += 4) {
			sd = (*sp++);
			MLIB_V_IMAGESQUARE_S16_UNROLL(sd, rdh, rdl, dd);
			(*dp++) = dd;

			sd0 = (*sp++);
			MLIB_V_IMAGESQUARE_S16_UNROLL(sd0, rdh0, rdl0, dd0);
			(*dp++) = dd0;

			sd1 = (*sp++);
			MLIB_V_IMAGESQUARE_S16_UNROLL(sd1, rdh1, rdl1, dd1);
			(*dp++) = dd1;

			sd2 = (*sp++);
			MLIB_V_IMAGESQUARE_S16_UNROLL(sd2, rdh2, rdl2, dd2);
			(*dp++) = dd2;
		}

#pragma pipeloop(0)
		for (; i < (xsize / 4); i++) {
			sd = (*sp++);
			MLIB_V_IMAGESQUARE_S16(sd, dd);
			(*dp++) = dd;
		}

		sl = sp = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dl = dp = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageSquare_S16_D1(
    mlib_s16 *src,
    mlib_s16 *dst,
    mlib_s32 dsize)
{
/* aligned pointer to source */
	mlib_d64 *sp;

/* pointer to source */
	mlib_s16 *sa;

/* aligned pointer to destination */
	mlib_d64 *dp;

/* pointer to destination */
	mlib_s16 *da;

/* pointer to end of dst */
	mlib_s16 *dend;

/* offset of address alignment in dst */
	mlib_s32 off;

/* edge masks */
	mlib_s32 emask;

/* source data */
	mlib_d64 s0, s1, s2;

/* source data */
	mlib_d64 sd, sd0, sd1, sd2;

/* destination data */
	mlib_d64 dd, dd0, dd1, dd2;

/* temporaries used in macro */
	mlib_d64 rdh, rdl, rdh0, rdl0, rdh1, rdl1, rdh2, rdl2;

/* loop variable */
	mlib_s32 i, n;

	sa = src;
	da = dst;

/* prepare the destination address */
	dp = (mlib_d64 *)((mlib_addr)da & (~7));
	off = (mlib_addr)dp - (mlib_addr)da;
	dend = da + dsize - 1;

/* prepare the source address */
	sp = (mlib_d64 *)vis_alignaddr(sa, off);

/* generate edge mask for the start point */
	emask = vis_edge16(da, dend);

/* first 4 pixels */
	s0 = (*sp++);
	s1 = (*sp++);
	sd = vis_faligndata(s0, s1);
	MLIB_V_IMAGESQUARE_S16(sd, dd);
	vis_pst_16(dd, dp++, emask);

	n = ((mlib_u8 *)(dend + 1) - (mlib_u8 *)dp) / 8;

/* 4-pixel column loop */
#pragma pipeloop(0)
	for (i = 0; i < n - 3; i += 4) {
		s2 = (*sp++);
		sd = vis_faligndata(s1, s2);
		MLIB_V_IMAGESQUARE_S16_UNROLL(sd, rdh, rdl, dd);
		(*dp++) = dd;

		s1 = (*sp++);
		sd0 = vis_faligndata(s2, s1);
		MLIB_V_IMAGESQUARE_S16_UNROLL(sd0, rdh0, rdl0, dd0);
		(*dp++) = dd0;

		s2 = (*sp++);
		sd1 = vis_faligndata(s1, s2);
		MLIB_V_IMAGESQUARE_S16_UNROLL(sd1, rdh1, rdl1, dd1);
		(*dp++) = dd1;

		s1 = (*sp++);
		sd2 = vis_faligndata(s2, s1);
		MLIB_V_IMAGESQUARE_S16_UNROLL(sd2, rdh2, rdl2, dd2);
		(*dp++) = dd2;
	}

#pragma pipeloop(0)
	for (; i < n; i++) {
		s0 = s1;
		s1 = (*sp++);
		sd = vis_faligndata(s0, s1);
		MLIB_V_IMAGESQUARE_S16(sd, dd);
		(*dp++) = dd;
	}

/* end point handling */

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		emask = vis_edge16(dp, dend);
		s0 = s1;
		s1 = (*sp++);
		sd = vis_faligndata(s0, s1);
		MLIB_V_IMAGESQUARE_S16(sd, dd);
		vis_pst_16(dd, dp++, emask);
	}
}

/* *********************************************************** */

void
mlib_v_ImageSquare_S16(
    mlib_s16 *src,
    mlib_s32 slb,
    mlib_s16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
/* aligned pointer to source */
	mlib_d64 *sp;

/* pointer to a line in source */
	mlib_s16 *sl;

/* aligned pointer to destination */
	mlib_d64 *dp;

/* pointer to a line in destination */
	mlib_s16 *dl;

/* pointer to end of a line in dst */
	mlib_s16 *dend;

/* offset of address alignment in dst */
	mlib_s32 off;

/* edge masks */
	mlib_s32 emask;

/* source data */
	mlib_d64 s0, s1, s2;

/* source data */
	mlib_d64 sd, sd0, sd1, sd2;

/* destination data */
	mlib_d64 dd, dd0, dd1, dd2;

/* temporaries used in macro */
	mlib_d64 rdh, rdl, rdh0, rdl0, rdh1, rdl1, rdh2, rdl2;

/* loop variable */
	mlib_s32 i, j, n;

	sl = src;
	dl = dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* prepare the destination address */
		dp = (mlib_d64 *)((mlib_addr)dl & (~7));
		off = (mlib_addr)dp - (mlib_addr)dl;
		dend = dl + xsize - 1;

/* prepare the source address */
		sp = (mlib_d64 *)vis_alignaddr(sl, off);

/* generate edge mask for the start point */
		emask = vis_edge16(dl, dend);

/* first 4 pixels */
		s0 = (*sp++);
		s1 = (*sp++);
		sd = vis_faligndata(s0, s1);
		MLIB_V_IMAGESQUARE_S16(sd, dd);
		vis_pst_16(dd, dp++, emask);

		n = ((mlib_u8 *)(dend + 1) - (mlib_u8 *)dp) / 8;

/* 4-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i < n - 3; i += 4) {
			s2 = (*sp++);
			sd = vis_faligndata(s1, s2);
			MLIB_V_IMAGESQUARE_S16_UNROLL(sd, rdh, rdl, dd);
			(*dp++) = dd;

			s1 = (*sp++);
			sd0 = vis_faligndata(s2, s1);
			MLIB_V_IMAGESQUARE_S16_UNROLL(sd0, rdh0, rdl0, dd0);
			(*dp++) = dd0;

			s2 = (*sp++);
			sd1 = vis_faligndata(s1, s2);
			MLIB_V_IMAGESQUARE_S16_UNROLL(sd1, rdh1, rdl1, dd1);
			(*dp++) = dd1;

			s1 = (*sp++);
			sd2 = vis_faligndata(s2, s1);
			MLIB_V_IMAGESQUARE_S16_UNROLL(sd2, rdh2, rdl2, dd2);
			(*dp++) = dd2;
		}

#pragma pipeloop(0)
		for (; i < n; i++) {
			s0 = s1;
			s1 = (*sp++);
			sd = vis_faligndata(s0, s1);
			MLIB_V_IMAGESQUARE_S16(sd, dd);
			(*dp++) = dd;
		}

/* end point handling */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			emask = vis_edge16(dp, dend);
			s0 = s1;
			s1 = (*sp++);
			sd = vis_faligndata(s0, s1);
			MLIB_V_IMAGESQUARE_S16(sd, dd);
			vis_pst_16(dd, dp++, emask);
		}

		sl = (mlib_s16 *)((mlib_u8 *)sl + slb);
		dl = (mlib_s16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageSqrShift_S32(
    mlib_s32 *src,
    mlib_s32 slb,
    mlib_s32 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 shift)
{
	mlib_d64 dscale;
	mlib_s32 i, j;

/* dscale = 2 ** (-shift) */
	SCALBN(dscale, -shift);

/* if src and dst - 1-D vector */
	if (xsize == slb && slb == dlb) {
		xsize *= ysize;
		ysize = 1;
	}

	for (j = 0; j < ysize; j++) {
#pragma pipeloop(0)
		for (i = 0; i < xsize - 3; i += 4) {
			mlib_d64 sat32_tmp0, sat32_tmp1, sat32_tmp2, sat32_tmp3;

			sat32_tmp0 = src[i + 0] * (mlib_d64)src[i + 0] * dscale;
			sat32_tmp1 = src[i + 1] * (mlib_d64)src[i + 1] * dscale;

			if (sat32_tmp0 >= MLIB_S32_MAX)
				sat32_tmp0 = MLIB_S32_MAX;
			dst[i + 0] = (mlib_s32)sat32_tmp0;

			if (sat32_tmp1 >= MLIB_S32_MAX)
				sat32_tmp1 = MLIB_S32_MAX;
			dst[i + 1] = (mlib_s32)sat32_tmp1;

			sat32_tmp2 = src[i + 2] * (mlib_d64)src[i + 2] * dscale;
			sat32_tmp3 = src[i + 3] * (mlib_d64)src[i + 3] * dscale;

			if (sat32_tmp2 >= MLIB_S32_MAX)
				sat32_tmp2 = MLIB_S32_MAX;
			dst[i + 2] = (mlib_s32)sat32_tmp2;

			if (sat32_tmp3 >= MLIB_S32_MAX)
				sat32_tmp3 = MLIB_S32_MAX;
			dst[i + 3] = (mlib_s32)sat32_tmp3;
		}

#pragma pipeloop(0)
		for (; i < xsize; i++) {
			mlib_d64 sat32_tmp = src[i] * (mlib_d64)src[i] * dscale;

			if (sat32_tmp >= MLIB_S32_MAX)
				sat32_tmp = MLIB_S32_MAX;
			dst[i] = (mlib_s32)sat32_tmp;
		}

		src += slb;
		dst += dlb;
	}
}

/* *********************************************************** */

void
mlib_v_ImageSqrShift_U16_A8D1X4(
    mlib_u16 *src,
    mlib_u16 *dst,
    mlib_s32 dsize,
    mlib_s32 shift)
{
/* pointer to source images */
	mlib_d64 *sp;

/* pointer to destination image */
	mlib_d64 *dp;

/* source data */
	mlib_d64 sd;

/* destination data */
	mlib_d64 dd;

/* temporaries used in macro */
	mlib_d64 rdhh, rdhl;

/* temporaries used in macro */
	mlib_d64 rdlh, rdll;

/* temporaries used in macro */
	mlib_d64 rdh, rdl;

/* loop variable */
	mlib_s32 j;

	mlib_d64 mask = vis_to_double_dup(0xfffefffe);
	mlib_f32 fmin = vis_to_float(0x80808080);
	mlib_d64 negate = vis_to_double_dup(0x7FFF7FFF);
	mlib_d64 xor_mask = vis_to_double_dup(0x80008000);
	mlib_d64 sat_offset = vis_to_double_dup(0x20000000 >> (16 - shift));

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;

/* 4-pixel loop */
#pragma pipeloop(0)
	for (j = 0; j < (dsize / 4); j++) {
		sd = (*sp++);
		MLIB_V_IMAGESQRSHIFT_U16(sd, dd);
		(*dp++) = dd;
	}
}

/* *********************************************************** */

void
mlib_v_ImageSqrShift_U16_A8D2X4(
    mlib_u16 *src,
    mlib_s32 slb,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 shift)
{
/* pointer to source images */
	mlib_d64 *sp;

/* pointer to a line in source */
	mlib_d64 *sl;

/* pointer to destination image */
	mlib_d64 *dp;

/* pointer to a line in destination */
	mlib_d64 *dl;

/* source data */
	mlib_d64 sd;

/* destination data */
	mlib_d64 dd;

/* temporaries used in macro */
	mlib_d64 rdhh, rdhl;

/* temporaries used in macro */
	mlib_d64 rdlh, rdll;

/* temporaries used in macro */
	mlib_d64 rdh, rdl;

/* loop variable */
	mlib_s32 i, j;

	mlib_d64 mask = vis_to_double_dup(0xfffefffe);
	mlib_f32 fmin = vis_to_float(0x80808080);
	mlib_d64 negate = vis_to_double_dup(0x7FFF7FFF);
	mlib_d64 xor_mask = vis_to_double_dup(0x80008000);
	mlib_d64 sat_offset = vis_to_double_dup(0x20000000 >> (16 - shift));

	sl = sp = (mlib_d64 *)src;
	dl = dp = (mlib_d64 *)dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* 4-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i < (xsize / 4); i++) {
			sd = (*sp++);
			MLIB_V_IMAGESQRSHIFT_U16(sd, dd);
			(*dp++) = dd;
		}

		sl = sp = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dl = dp = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageSqrShift_U16_D1(
    mlib_u16 *src,
    mlib_u16 *dst,
    mlib_s32 dsize,
    mlib_s32 shift)
{
/* aligned pointer to source */
	mlib_d64 *sp;

/* pointer to source */
	mlib_u16 *sa;

/* aligned pointer to destination */
	mlib_d64 *dp;

/* pointer to destination */
	mlib_u16 *da;

/* pointer to end of dst */
	mlib_u16 *dend;

/* offset of address alignment in dst */
	mlib_s32 off;

/* edge masks */
	mlib_s32 emask;

/* source data */
	mlib_d64 s0, s1;

/* source data */
	mlib_d64 sd;

/* destination data */
	mlib_d64 dd;

/* temporaries used in macro */
	mlib_d64 rdhh, rdhl;

/* temporaries used in macro */
	mlib_d64 rdlh, rdll;

/* temporaries used in macro */
	mlib_d64 rdh, rdl;

/* loop variable */
	mlib_s32 i, n;

	mlib_d64 mask = vis_to_double_dup(0xfffefffe);
	mlib_f32 fmin = vis_to_float(0x80808080);
	mlib_d64 negate = vis_to_double_dup(0x7FFF7FFF);
	mlib_d64 xor_mask = vis_to_double_dup(0x80008000);
	mlib_d64 sat_offset = vis_to_double_dup(0x20000000 >> (16 - shift));

	sa = src;
	da = dst;

/* prepare the destination address */
	dp = (mlib_d64 *)((mlib_addr)da & (~7));
	off = (mlib_addr)dp - (mlib_addr)da;
	dend = da + dsize - 1;

/* prepare the source address */
	sp = (mlib_d64 *)vis_alignaddr(sa, off);

/* generate edge mask for the start point */
	emask = vis_edge16(da, dend);

/* first 4 pixels */
	s0 = (*sp++);
	s1 = (*sp++);
	sd = vis_faligndata(s0, s1);
	MLIB_V_IMAGESQRSHIFT_U16(sd, dd);
	vis_pst_16(dd, dp++, emask);

	n = ((mlib_u8 *)(dend + 1) - (mlib_u8 *)dp) / 8;

/* 4-pixel column loop */
#pragma pipeloop(0)
	for (i = 0; i < n; i++) {
		s0 = s1;
		s1 = (*sp++);
		sd = vis_faligndata(s0, s1);
		MLIB_V_IMAGESQRSHIFT_U16(sd, dd);
		(*dp++) = dd;
	}

/* end point handling */

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		emask = vis_edge16(dp, dend);
		s0 = s1;
		s1 = (*sp++);
		sd = vis_faligndata(s0, s1);
		MLIB_V_IMAGESQRSHIFT_U16(sd, dd);
		vis_pst_16(dd, dp++, emask);
	}
}

/* *********************************************************** */

void
mlib_v_ImageSqrShift_U16(
    mlib_u16 *src,
    mlib_s32 slb,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 shift)
{
/* aligned pointer to source */
	mlib_d64 *sp;

/* pointer to a line in source */
	mlib_u16 *sl;

/* aligned pointer to destination */
	mlib_d64 *dp;

/* pointer to a line in destination */
	mlib_u16 *dl;

/* pointer to end of a line in dst */
	mlib_u16 *dend;

/* offset of address alignment in dst */
	mlib_s32 off;

/* edge masks */
	mlib_s32 emask;

/* source data */
	mlib_d64 s0, s1;

/* source data */
	mlib_d64 sd;

/* destination data */
	mlib_d64 dd;

/* temporaries used in macro */
	mlib_d64 rdhh, rdhl;

/* temporaries used in macro */
	mlib_d64 rdlh, rdll;

/* temporaries used in macro */
	mlib_d64 rdh, rdl;

/* loop variable */
	mlib_s32 i, j, n;

	mlib_d64 mask = vis_to_double_dup(0xfffefffe);
	mlib_f32 fmin = vis_to_float(0x80808080);
	mlib_d64 negate = vis_to_double_dup(0x7FFF7FFF);
	mlib_d64 xor_mask = vis_to_double_dup(0x80008000);
	mlib_d64 sat_offset = vis_to_double_dup(0x20000000 >> (16 - shift));

	sl = src;
	dl = dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* prepare the destination address */
		dp = (mlib_d64 *)((mlib_addr)dl & (~7));
		off = (mlib_addr)dp - (mlib_addr)dl;
		dend = dl + xsize - 1;

/* prepare the source address */
		sp = (mlib_d64 *)vis_alignaddr(sl, off);

/* generate edge mask for the start point */
		emask = vis_edge16(dl, dend);

/* first 4 pixels */
		s0 = (*sp++);
		s1 = (*sp++);
		sd = vis_faligndata(s0, s1);
		MLIB_V_IMAGESQRSHIFT_U16(sd, dd);
		vis_pst_16(dd, dp++, emask);

		n = ((mlib_u8 *)(dend + 1) - (mlib_u8 *)dp) / 8;

/* 4-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i < n; i++) {
			s0 = s1;
			s1 = (*sp++);
			sd = vis_faligndata(s0, s1);
			MLIB_V_IMAGESQRSHIFT_U16(sd, dd);
			(*dp++) = dd;
		}

/* end point handling */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			emask = vis_edge16(dp, dend);
			s0 = s1;
			s1 = (*sp++);
			sd = vis_faligndata(s0, s1);
			MLIB_V_IMAGESQRSHIFT_U16(sd, dd);
			vis_pst_16(dd, dp++, emask);
		}

		sl = (mlib_u16 *)((mlib_u8 *)sl + slb);
		dl = (mlib_u16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageSquare_U16_A8D1X4(
    mlib_u16 *src,
    mlib_u16 *dst,
    mlib_s32 dsize)
{
/* pointer to source image */
	mlib_d64 *sp;

/* pointer to destination image */
	mlib_d64 *dp;

/* source data */
	mlib_d64 sd;

/* destination data */
	mlib_d64 dd;

/* temporaries used in macro */
	mlib_d64 rdh, rdl;

/* loop variable */
	mlib_s32 j;

	mlib_d64 xor_mask = vis_to_double_dup(0x80008000);
	mlib_d64 sat_offset = vis_to_double_dup(0x40004000);

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;

/* 4-pixel loop */
#pragma pipeloop(0)
	for (j = 0; j < (dsize / 4); j++) {
		sd = (*sp++);
		MLIB_V_IMAGESQUARE_U16(sd, dd);
		(*dp++) = dd;
	}
}

/* *********************************************************** */

void
mlib_v_ImageSquare_U16_A8D2X4(
    mlib_u16 *src,
    mlib_s32 slb,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
/* pointer to source image */
	mlib_d64 *sp;

/* pointer to a line in source */
	mlib_d64 *sl;

/* pointer to destination image */
	mlib_d64 *dp;

/* pointer to a line in destination */
	mlib_d64 *dl;

/* source data */
	mlib_d64 sd;

/* destination data */
	mlib_d64 dd;

/* temporaries used in macro */
	mlib_d64 rdh, rdl;

/* loop variable */
	mlib_s32 i, j;

	mlib_d64 xor_mask = vis_to_double_dup(0x80008000);
	mlib_d64 sat_offset = vis_to_double_dup(0x40004000);

	sl = sp = (mlib_d64 *)src;
	dl = dp = (mlib_d64 *)dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* 4-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i < (xsize / 4); i++) {
			sd = (*sp++);
			MLIB_V_IMAGESQUARE_U16(sd, dd);
			(*dp++) = dd;
		}

		sl = sp = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dl = dp = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageSquare_U16_D1(
    mlib_u16 *src,
    mlib_u16 *dst,
    mlib_s32 dsize)
{
/* aligned pointer to source */
	mlib_d64 *sp;

/* pointer to source */
	mlib_u16 *sa;

/* aligned pointer to destination */
	mlib_d64 *dp;

/* pointer to destination */
	mlib_u16 *da;

/* pointer to end of dst */
	mlib_u16 *dend;

/* offset of address alignment in dst */
	mlib_s32 off;

/* edge masks */
	mlib_s32 emask;

/* source data */
	mlib_d64 s0, s1;

/* source data */
	mlib_d64 sd;

/* destination data */
	mlib_d64 dd;

/* temporaries used in macro */
	mlib_d64 rdh, rdl;

/* loop variable */
	mlib_s32 i, n;

	mlib_d64 xor_mask = vis_to_double_dup(0x80008000);
	mlib_d64 sat_offset = vis_to_double_dup(0x40004000);

	sa = src;
	da = dst;

/* prepare the destination address */
	dp = (mlib_d64 *)((mlib_addr)da & (~7));
	off = (mlib_addr)dp - (mlib_addr)da;
	dend = da + dsize - 1;

/* prepare the source address */
	sp = (mlib_d64 *)vis_alignaddr(sa, off);

/* generate edge mask for the start point */
	emask = vis_edge16(da, dend);

/* first 4 pixels */
	s0 = (*sp++);
	s1 = (*sp++);
	sd = vis_faligndata(s0, s1);
	MLIB_V_IMAGESQUARE_U16(sd, dd);
	vis_pst_16(dd, dp++, emask);

	n = ((mlib_u8 *)(dend + 1) - (mlib_u8 *)dp) / 8;

/* 4-pixel column loop */
#pragma pipeloop(0)
	for (i = 0; i < n; i++) {
		s0 = s1;
		s1 = (*sp++);
		sd = vis_faligndata(s0, s1);
		MLIB_V_IMAGESQUARE_U16(sd, dd);
		(*dp++) = dd;
	}

/* end point handling */

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		emask = vis_edge16(dp, dend);
		s0 = s1;
		s1 = (*sp++);
		sd = vis_faligndata(s0, s1);
		MLIB_V_IMAGESQUARE_U16(sd, dd);
		vis_pst_16(dd, dp++, emask);
	}
}

/* *********************************************************** */

void
mlib_v_ImageSquare_U16(
    mlib_u16 *src,
    mlib_s32 slb,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize)
{
/* aligned pointer to source */
	mlib_d64 *sp;

/* pointer to a line in source */
	mlib_u16 *sl;

/* aligned pointer to destination */
	mlib_d64 *dp;

/* pointer to a line in destination */
	mlib_u16 *dl;

/* pointer to end of a line in dst */
	mlib_u16 *dend;

/* offset of address alignment in dst */
	mlib_s32 off;

/* edge masks */
	mlib_s32 emask;

/* source data */
	mlib_d64 s0, s1;

/* source data */
	mlib_d64 sd;

/* destination data */
	mlib_d64 dd;

/* temporaries used in macro */
	mlib_d64 rdh, rdl;

/* loop variable */
	mlib_s32 i, j, n;

	mlib_d64 xor_mask = vis_to_double_dup(0x80008000);
	mlib_d64 sat_offset = vis_to_double_dup(0x40004000);

	sl = src;
	dl = dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* prepare the destination address */
		dp = (mlib_d64 *)((mlib_addr)dl & (~7));
		off = (mlib_addr)dp - (mlib_addr)dl;
		dend = dl + xsize - 1;

/* prepare the source address */
		sp = (mlib_d64 *)vis_alignaddr(sl, off);

/* generate edge mask for the start point */
		emask = vis_edge16(dl, dend);

/* first 4 pixels */
		s0 = (*sp++);
		s1 = (*sp++);
		sd = vis_faligndata(s0, s1);
		MLIB_V_IMAGESQUARE_U16(sd, dd);
		vis_pst_16(dd, dp++, emask);

		n = ((mlib_u8 *)(dend + 1) - (mlib_u8 *)dp) / 8;

/* 4-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i < n; i++) {
			s0 = s1;
			s1 = (*sp++);
			sd = vis_faligndata(s0, s1);
			MLIB_V_IMAGESQUARE_U16(sd, dd);
			(*dp++) = dd;
		}

/* end point handling */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			emask = vis_edge16(dp, dend);
			s0 = s1;
			s1 = (*sp++);
			sd = vis_faligndata(s0, s1);
			MLIB_V_IMAGESQUARE_U16(sd, dd);
			vis_pst_16(dd, dp++, emask);
		}

		sl = (mlib_u16 *)((mlib_u8 *)sl + slb);
		dl = (mlib_u16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageSqrSmallShift_U16_A8D1X4(
    mlib_u16 *src,
    mlib_u16 *dst,
    mlib_s32 dsize,
    mlib_s32 shift)
{
/* pointer to source images */
	mlib_d64 *sp;

/* pointer to destination image */
	mlib_d64 *dp;

/* source data */
	mlib_d64 sd;

/* destination data */
	mlib_d64 dd;

/* temporaries used in macro */
	mlib_d64 rdhh, rdhl;

/* temporaries used in macro */
	mlib_d64 rdlh, rdll;

/* temporaries used in macro */
	mlib_d64 rdh, rdl;

/* loop variable */
	mlib_s32 j;

	mlib_d64 sdad, rdh_0, rdh_1, rdl_0, rdl_1;
	mlib_d64 offset = vis_to_double_dup(0x80008000);
	mlib_d64 dscale = (mlib_d64)(0x10000 >> shift);
	mlib_d64 sat_offset =
	    (mlib_d64)(0x40000000) * dscale - ((mlib_d64)0x80000000);

	sp = (mlib_d64 *)src;
	dp = (mlib_d64 *)dst;

/* 4-pixel loop */
#pragma pipeloop(0)
	for (j = 0; j < (dsize / 4); j++) {
		sd = (*sp++);
		MLIB_V_IMAGESQRSMALLSHIFT_U16(sd, dd);
		(*dp++) = dd;
	}
}

/* *********************************************************** */

void
mlib_v_ImageSqrSmallShift_U16_A8D2X4(
    mlib_u16 *src,
    mlib_s32 slb,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 shift)
{
/* pointer to source images */
	mlib_d64 *sp;

/* pointer to a line in source */
	mlib_d64 *sl;

/* pointer to destination image */
	mlib_d64 *dp;

/* pointer to a line in destination */
	mlib_d64 *dl;

/* source data */
	mlib_d64 sd;

/* destination data */
	mlib_d64 dd;

/* temporaries used in macro */
	mlib_d64 rdhh, rdhl;

/* temporaries used in macro */
	mlib_d64 rdlh, rdll;

/* temporaries used in macro */
	mlib_d64 rdh, rdl;

/* loop variable */
	mlib_s32 i, j;

	mlib_d64 sdad, rdh_0, rdh_1, rdl_0, rdl_1;
	mlib_d64 offset = vis_to_double_dup(0x80008000);
	mlib_d64 dscale = (mlib_d64)(0x10000 >> shift);
	mlib_d64 sat_offset =
	    (mlib_d64)(0x40000000) * dscale - ((mlib_d64)0x80000000);

	sl = sp = (mlib_d64 *)src;
	dl = dp = (mlib_d64 *)dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* 4-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i < (xsize / 4); i++) {
			sd = (*sp++);
			MLIB_V_IMAGESQRSMALLSHIFT_U16(sd, dd);
			(*dp++) = dd;
		}

		sl = sp = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dl = dp = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageSqrSmallShift_U16_D1(
    mlib_u16 *src,
    mlib_u16 *dst,
    mlib_s32 dsize,
    mlib_s32 shift)
{
/* aligned pointer to source */
	mlib_d64 *sp;

/* pointer to source */
	mlib_u16 *sa;

/* aligned pointer to destination */
	mlib_d64 *dp;

/* pointer to destination */
	mlib_u16 *da;

/* pointer to end of dst */
	mlib_u16 *dend;

/* offset of address alignment in dst */
	mlib_s32 off;

/* edge masks */
	mlib_s32 emask;

/* source data */
	mlib_d64 s0, s1;

/* source data */
	mlib_d64 sd;

/* destination data */
	mlib_d64 dd;

/* temporaries used in macro */
	mlib_d64 rdhh, rdhl;

/* temporaries used in macro */
	mlib_d64 rdlh, rdll;

/* temporaries used in macro */
	mlib_d64 rdh, rdl;

/* loop variable */
	mlib_s32 i, n;

	mlib_d64 sdad, rdh_0, rdh_1, rdl_0, rdl_1;
	mlib_d64 offset = vis_to_double_dup(0x80008000);
	mlib_d64 dscale = (mlib_d64)(0x10000 >> shift);
	mlib_d64 sat_offset =
	    (mlib_d64)(0x40000000) * dscale - ((mlib_d64)0x80000000);

	sa = src;
	da = dst;

/* prepare the destination address */
	dp = (mlib_d64 *)((mlib_addr)da & (~7));
	off = (mlib_addr)dp - (mlib_addr)da;
	dend = da + dsize - 1;

/* prepare the source address */
	sp = (mlib_d64 *)vis_alignaddr(sa, off);

/* generate edge mask for the start point */
	emask = vis_edge16(da, dend);

/* first 4 pixels */
	s0 = (*sp++);
	s1 = (*sp++);
	sd = vis_faligndata(s0, s1);
	MLIB_V_IMAGESQRSMALLSHIFT_U16(sd, dd);
	vis_pst_16(dd, dp++, emask);

	n = ((mlib_u8 *)(dend + 1) - (mlib_u8 *)dp) / 8;

/* 4-pixel column loop */
#pragma pipeloop(0)
	for (i = 0; i < n; i++) {
		s0 = s1;
		s1 = (*sp++);
		sd = vis_faligndata(s0, s1);
		MLIB_V_IMAGESQRSMALLSHIFT_U16(sd, dd);
		(*dp++) = dd;
	}

/* end point handling */

	if ((mlib_addr)dp <= (mlib_addr)dend) {
		emask = vis_edge16(dp, dend);
		s0 = s1;
		s1 = (*sp++);
		sd = vis_faligndata(s0, s1);
		MLIB_V_IMAGESQRSMALLSHIFT_U16(sd, dd);
		vis_pst_16(dd, dp++, emask);
	}
}

/* *********************************************************** */

void
mlib_v_ImageSqrSmallShift_U16(
    mlib_u16 *src,
    mlib_s32 slb,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_s32 xsize,
    mlib_s32 ysize,
    mlib_s32 shift)
{
/* aligned pointer to source */
	mlib_d64 *sp;

/* pointer to a line in source */
	mlib_u16 *sl;

/* aligned pointer to destination */
	mlib_d64 *dp;

/* pointer to a line in destination */
	mlib_u16 *dl;

/* pointer to end of a line in dst */
	mlib_u16 *dend;

/* offset of address alignment in dst */
	mlib_s32 off;

/* edge masks */
	mlib_s32 emask;

/* source data */
	mlib_d64 s0, s1;

/* source data */
	mlib_d64 sd;

/* destination data */
	mlib_d64 dd;

/* temporaries used in macro */
	mlib_d64 rdhh, rdhl;

/* temporaries used in macro */
	mlib_d64 rdlh, rdll;

/* temporaries used in macro */
	mlib_d64 rdh, rdl;

/* loop variable */
	mlib_s32 i, j, n;

	mlib_d64 sdad, rdh_0, rdh_1, rdl_0, rdl_1;
	mlib_d64 offset = vis_to_double_dup(0x80008000);
	mlib_d64 dscale = (mlib_d64)(0x10000 >> shift);
	mlib_d64 sat_offset =
	    (mlib_d64)(0x40000000) * dscale - ((mlib_d64)0x80000000);

	sl = src;
	dl = dst;

/* row loop */
	for (j = 0; j < ysize; j++) {

/* prepare the destination address */
		dp = (mlib_d64 *)((mlib_addr)dl & (~7));
		off = (mlib_addr)dp - (mlib_addr)dl;
		dend = dl + xsize - 1;

/* prepare the source address */
		sp = (mlib_d64 *)vis_alignaddr(sl, off);

/* generate edge mask for the start point */
		emask = vis_edge16(dl, dend);

/* first 4 pixels */
		s0 = (*sp++);
		s1 = (*sp++);
		sd = vis_faligndata(s0, s1);
		MLIB_V_IMAGESQRSMALLSHIFT_U16(sd, dd);
		vis_pst_16(dd, dp++, emask);

		n = ((mlib_u8 *)(dend + 1) - (mlib_u8 *)dp) / 8;

/* 4-pixel column loop */
#pragma pipeloop(0)
		for (i = 0; i < n; i++) {
			s0 = s1;
			s1 = (*sp++);
			sd = vis_faligndata(s0, s1);
			MLIB_V_IMAGESQRSMALLSHIFT_U16(sd, dd);
			(*dp++) = dd;
		}

/* end point handling */

		if ((mlib_addr)dp <= (mlib_addr)dend) {
			emask = vis_edge16(dp, dend);
			s0 = s1;
			s1 = (*sp++);
			sd = vis_faligndata(s0, s1);
			MLIB_V_IMAGESQRSMALLSHIFT_U16(sd, dd);
			vis_pst_16(dd, dp++, emask);
		}

		sl = (mlib_u16 *)((mlib_u8 *)sl + slb);
		dl = (mlib_u16 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */
