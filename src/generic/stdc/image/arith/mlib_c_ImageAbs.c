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

#pragma ident	"@(#)mlib_c_ImageAbs.c	9.3	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageAbs_S16 - absolute value of each 16-bit pixel
 *      mlib_ImageAbs_S32 - absolute value of each 32-bit pixel
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageAbs_S16 (mlib_s16  *dst,
 *                                      mlib_s16 *src,
 *                                     mlib_s32  dlb,
 *                                     mlib_s32  slb,
 *                                     mlib_s32  wid,
 *                                     mlib_s32  hgt);
 *      mlib_status mlib_ImageAbs_S32 (mlib_s32  *dst,
 *                                      mlib_s32 *src,
 *                                     mlib_s32  dlb,
 *                                     mlib_s32  slb,
 *                                     mlib_s32  wid,
 *                                     mlib_s32  hgt);
 *
 * ARGUMENT
 *      dst     pointer to destination image
 *      src     pointer to source image
 *      dlb     dst image line bytes (bytes / line)
 *      slb     src image line bytes (bytes / line)
 *      wid     src image width in pixels
 *      hgt     src image height in pixels
 *
 * DESCRIPTION
 *      dst(i, j) = |src(i, j)|
 */

#include <mlib_image.h>
#include <mlib_ImageAbs.h>

/* *********************************************************** */

#define	BLOCKSIZE	8

#define	ABS_VALUE(x)	(((x) ^ ((x) >> 31)) - ((x) >> 31))

/* *********************************************************** */

#define	GET_BLOCK_OF_PXLS                                       \
	pxl0 = (mlib_s32)sa[0];                                 \
	pxl1 = (mlib_s32)sa[1];                                 \
	pxl2 = (mlib_s32)sa[2];                                 \
	pxl3 = (mlib_s32)sa[3];                                 \
	pxl4 = (mlib_s32)sa[4];                                 \
	pxl5 = (mlib_s32)sa[5];                                 \
	pxl6 = (mlib_s32)sa[6];                                 \
	pxl7 = (mlib_s32)sa[7]

/* *********************************************************** */

#define	FIND_ABS_VAL                                            \
	absv0 = ABS_VALUE(pxl0);                                \
	absv1 = ABS_VALUE(pxl1);                                \
	absv2 = ABS_VALUE(pxl2);                                \
	absv3 = ABS_VALUE(pxl3);                                \
	absv4 = ABS_VALUE(pxl4);                                \
	absv5 = ABS_VALUE(pxl5);                                \
	absv6 = ABS_VALUE(pxl6);                                \
	absv7 = ABS_VALUE(pxl7)

/* *********************************************************** */

#define	STORE_BLOCK_OF_PXLS(type)                               \
	da[0] = (type) absv0;                                   \
	da[1] = (type) absv1;                                   \
	da[2] = (type) absv2;                                   \
	da[3] = (type) absv3;                                   \
	da[4] = (type) absv4;                                   \
	da[5] = (type) absv5;                                   \
	da[6] = (type) absv6;                                   \
	da[7] = (type) absv7

/* *********************************************************** */

mlib_status
mlib_ImageAbs_S16(
    mlib_s16 *dst,
    mlib_s16 *src,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 w,
    mlib_s32 h)
{
	mlib_s32 row, block, col, w1;

/* pointers for pixel and line of source */
	mlib_s16 *sa, *sl;

/* pointers for pixel and line of destination */
	mlib_s16 *da, *dl;

/* pxl count of source, dst lines */
	mlib_s32 slpxl, dlpxl;

/* orig pxls */
	mlib_s32 pxl0, pxl1, pxl2, pxl3, pxl4, pxl5, pxl6, pxl7;

/* abs values */
	mlib_s32 absv0, absv1, absv2, absv3, absv4, absv5, absv6, absv7;

/* convert source line bytes to pixels */
	slpxl = slb >> 1;
/* convert dst line bytes to pixels */
	dlpxl = dlb >> 1;
	sl = sa = src;
	dl = da = dst;
/* N - pxl blocks */
	w1 = w / BLOCKSIZE;

/* startup uses bufs 1 - 2, not 0 - 1 */
	for (row = 0; row < h; row++) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (block = 0; block < w1; block++) {
/* get new pxl0 - pxl7 */
			GET_BLOCK_OF_PXLS;
/* use absv0 - absv7 */
			FIND_ABS_VAL;
/* store in da[0] - da[7] */
			STORE_BLOCK_OF_PXLS(mlib_s16);

			sa += BLOCKSIZE;
			da += BLOCKSIZE;
		}

/* edge handling */
		for (col = w1 * BLOCKSIZE; col < w; col++) {
			pxl0 = (mlib_s32)(*sa++);
			(*da++) = (mlib_s16)ABS_VALUE(pxl0);
		}

/* update src ptrs */
		sa = (sl += slpxl);
/* update dst ptrs */
		da = (dl += dlpxl);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageAbs_S32(
    mlib_s32 *dst,
    mlib_s32 *src,
    mlib_s32 dlb,
    mlib_s32 slb,
    mlib_s32 w,
    mlib_s32 h)
{
	mlib_s32 row, block, col, w1;

/* pointers for pixel and line of source */
	mlib_s32 *sa, *sl;

/* pointers for pixel and line of destination */
	mlib_s32 *da, *dl;

/* pxl count of source, dst lines */
	mlib_s32 slpxl, dlpxl;

/* orig pxls */
	mlib_s32 pxl0, pxl1, pxl2, pxl3, pxl4, pxl5, pxl6, pxl7;

/* abs values */
	mlib_s32 absv0, absv1, absv2, absv3, absv4, absv5, absv6, absv7;

/* convert source line bytes to pixels */
	slpxl = slb >> 2;
/* convert dst line bytes to pixels */
	dlpxl = dlb >> 2;
	sl = sa = src;
	dl = da = dst;
/* N - pxl blocks */
	w1 = w / BLOCKSIZE;

/* startup uses bufs 1 - 2, not 0 - 1 */
	for (row = 0; row < h; row++) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (block = 0; block < w1; block++) {
/* get new pxl0 - pxl7 */
			GET_BLOCK_OF_PXLS;
/* use absv0 - absv7 */
			FIND_ABS_VAL;
/* store in da[0] - da[7] */
			STORE_BLOCK_OF_PXLS(mlib_s32);

			sa += BLOCKSIZE;
			da += BLOCKSIZE;
		}

/* edge handling */
		for (col = w1 * BLOCKSIZE; col < w; col++) {
			pxl0 = (mlib_s32)(*sa++);
			(*da++) = (mlib_s32)ABS_VALUE(pxl0);
		}

/* update src ptrs */
		sa = (sl += slpxl);
/* update dst ptrs */
		da = (dl += dlpxl);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
