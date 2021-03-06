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

#pragma ident	"@(#)mlib_c_ImageThresh1_S32.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageThresh1 - thresholding
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageThresh1(mlib_image       *dst,
 *                                    const mlib_image *src,
 *                                    const mlib_s32   *thresh,
 *                                    const mlib_s32   *ghigh,
 *                                    const mlib_s32   *glow);
 *
 * ARGUMENT
 *      dst     pointer to output image
 *      src     pointer to input image
 *      thresh  array of thresholds
 *      ghigh   array of values above thresholds
 *      glow    array of values below thresholds
 *
 * RESTRICTION
 *      The images must have the same size, and the same number
 *      of channels.
 *      The images can have 1, 2, 3, or 4 channels.
 *      The images can be in MLIB_BYTE, MLIB_SHORT or MLIB_INT data type.
 *      The type of the output image can be MLIB_BIT, or the same as the
 *      type of the input image.
 *
 * DESCRIPTION
 *      If the pixel band value is above the threshold for that channel,
 *      set the destination to the ghigh value for that channel.
 *      Otherwise, set the destination to the glow value for that channel.
 *
 *                      +- glow[c]   src[x][y][c] <= thresh[c]
 *      dst[x][y][c]  = |
 *                      +- ghigh[c]  src[x][y][c] >  thresh[c]
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_c_ImageThresh1.h>

/* *********************************************************** */

static void mlib_c_ImageThresh1_S321_1B_LT0(
    PARAMS,
    mlib_s32 dbit_off);

/* *********************************************************** */

#define	STYPE	mlib_s32

#ifdef _NO_LONGLONG

#define	TTYPE	mlib_s32

#define	DO_THRESH(s0, th, gl, gh)	((s0 > th) ? (gh) : (gl))

/* *********************************************************** */

#define	THRESH1_CMP_SHIFT(s0, th, sh)                           \
	(((((th) - (s0)) ^ (((th) ^ (s0)) & ~((s0) ^ ((th) -    \
	    (s0))))) >> (31 - (sh))) & (1 << (sh)))

#else /* _NO_LONGLONG */

/* *********************************************************** */

#define	TTYPE	mlib_s64
#define	T_SHIFT	63

/* *********************************************************** */

#define	DO_THRESH(s0, th, gl, gh)                                          \
	(((gh) & (((th) - (TTYPE) (s0)) >> T_SHIFT)) | ((gl) & ~(((th) -   \
	    (TTYPE) (s0)) >> T_SHIFT)))

/* *********************************************************** */

#define	THRESH1_CMP_SHIFT(s0, th, sh)                           \
	((((TTYPE) (th) -                                       \
	    (TTYPE) (s0)) >> T_SHIFT) & ((TTYPE) 1 << (sh)))

#endif /* _NO_LONGLONG */

/* *********************************************************** */

#define	STRIP(pd, ps, w, h, ch, th, gh, gl)                             \
	{                                                               \
	    STYPE s0;                                                   \
	                                                                \
	    for (i = 0; i < h; i++) {                                   \
		for (j = 0; j < w; j++) {                               \
		    for (k = 0; k < ch; k++) {                          \
			s0 = ((STYPE *) ps)[i * src_stride + j * ch +   \
			    k];                                         \
			((STYPE *) pd)[i * dst_stride + j * ch + k] =   \
			    (s0 <= th[k]) ? gl[k] : gh[k];              \
		    }                                                   \
		}                                                       \
	    }                                                           \
	}

/* *********************************************************** */

#define	INIT_THRESH0(n)                                         \
	thresh0 = thresh[n];                                    \
	ghigh0 = ghigh[n];                                      \
	glow0 = glow[n]

/* *********************************************************** */

#define	INIT_THRESH1(n)                                         \
	thresh1 = thresh[n];                                    \
	ghigh1 = ghigh[n];                                      \
	glow1 = glow[n]

/* *********************************************************** */

#define	INIT_THRESH2(n)                                         \
	thresh2 = thresh[n];                                    \
	ghigh2 = ghigh[n];                                      \
	glow2 = glow[n]

/* *********************************************************** */

#define	INIT_THRESH3(n)                                         \
	thresh3 = thresh[n];                                    \
	ghigh3 = ghigh[n];                                      \
	glow3 = glow[n]

/* *********************************************************** */

#define	THRESH0(s0)	DO_THRESH(s0, thresh0, glow0, ghigh0)
#define	THRESH1(s0)	DO_THRESH(s0, thresh1, glow1, ghigh1)

#define	THRESH2(s0)	DO_THRESH(s0, thresh2, glow2, ghigh2)

#define	THRESH3(s0)	DO_THRESH(s0, thresh3, glow3, ghigh3)

/* *********************************************************** */

#define	THRESH_S32_LT0(s0)                                      \
	(((ghigh0) & (((thresh0 -                               \
	    (s0)) | ~(s0)) >> 31)) | ((glow0) & ~(((thresh0 -   \
	    (s0)) | ~(s0)) >> 31)))

/* *********************************************************** */

#define	THRESH_S32_GE0(s0)                                      \
	(((ghigh0) & (((thresh0 -                               \
	    (s0)) & ~(s0)) >> 31)) | ((glow0) & ~(((thresh0 -   \
	    (s0)) & ~(s0)) >> 31)))

/* *********************************************************** */

void
mlib_c_ImageThresh1_S321(
    PARAMS)
{
	mlib_s32 *thresh = (void *)__thresh;
	mlib_s32 *ghigh = (void *)__ghigh;
	mlib_s32 *glow = (void *)__glow;
	mlib_s32 i, j, k;
	mlib_s32 thresh0;
	mlib_s32 ghigh0, glow0;

	if (width < 16) {
		STRIP(pdst, psrc, width, height, 1, thresh, ghigh, glow);
		return;
	}

	INIT_THRESH0(0);

	if (thresh0 < 0) {
		for (i = 0; i < height; i++) {
			mlib_s32 *psrc_row = (STYPE *) psrc + i * src_stride,
			    *pdst_row = (STYPE *) pdst + i * dst_stride;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j <= (width - 8); j += 8) {
				pdst_row[j] = THRESH_S32_LT0(psrc_row[j]);
				pdst_row[j + 1] =
				    THRESH_S32_LT0(psrc_row[j + 1]);
				pdst_row[j + 2] =
				    THRESH_S32_LT0(psrc_row[j + 2]);
				pdst_row[j + 3] =
				    THRESH_S32_LT0(psrc_row[j + 3]);
				pdst_row[j + 4] =
				    THRESH_S32_LT0(psrc_row[j + 4]);
				pdst_row[j + 5] =
				    THRESH_S32_LT0(psrc_row[j + 5]);
				pdst_row[j + 6] =
				    THRESH_S32_LT0(psrc_row[j + 6]);
				pdst_row[j + 7] =
				    THRESH_S32_LT0(psrc_row[j + 7]);
			}

			for (; j < width; j++) {
				pdst_row[j] = THRESH_S32_LT0(psrc_row[j]);
			}
		}
	} else {
		for (i = 0; i < height; i++) {
			mlib_s32 *psrc_row = (STYPE *) psrc + i * src_stride,
			    *pdst_row = (STYPE *) pdst + i * dst_stride;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j <= (width - 8); j += 8) {
				pdst_row[j] = THRESH_S32_GE0(psrc_row[j]);
				pdst_row[j + 1] =
				    THRESH_S32_GE0(psrc_row[j + 1]);
				pdst_row[j + 2] =
				    THRESH_S32_GE0(psrc_row[j + 2]);
				pdst_row[j + 3] =
				    THRESH_S32_GE0(psrc_row[j + 3]);
				pdst_row[j + 4] =
				    THRESH_S32_GE0(psrc_row[j + 4]);
				pdst_row[j + 5] =
				    THRESH_S32_GE0(psrc_row[j + 5]);
				pdst_row[j + 6] =
				    THRESH_S32_GE0(psrc_row[j + 6]);
				pdst_row[j + 7] =
				    THRESH_S32_GE0(psrc_row[j + 7]);
			}

			for (; j < width; j++) {
				pdst_row[j] = THRESH_S32_GE0(psrc_row[j]);
			}
		}
	}
}

/* *********************************************************** */

void
mlib_c_ImageThresh1_S322(
    PARAMS)
{
	mlib_s32 *thresh = (void *)__thresh;
	mlib_s32 *ghigh = (void *)__ghigh;
	mlib_s32 *glow = (void *)__glow;
	mlib_s32 i, j, k;
	mlib_s32 thresh0 = thresh[0], thresh1 = thresh[1];
	mlib_s32 high0 = ghigh[0], low0 = glow[0];
	mlib_s32 high1 = ghigh[1], low1 = glow[1];
	mlib_s32 src0, src1, src2, src3, src4, src5, src6, src7;

	if (width < 16) {
		STRIP(pdst, psrc, width, height, 2, thresh, ghigh, glow);
		return;
	}

	width <<= 1;
	for (i = 0; i < height; i++) {
		mlib_s32 *psrc_row = (STYPE *) psrc + i * src_stride,
		    *pdst_row = (STYPE *) pdst + i * dst_stride;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j <= (width - 8); j += 8) {
			src0 = psrc_row[j];
			src1 = psrc_row[j + 1];
			src2 = psrc_row[j + 2];
			src3 = psrc_row[j + 3];
			src4 = psrc_row[j + 4];
			src5 = psrc_row[j + 5];
			src6 = psrc_row[j + 6];
			src7 = psrc_row[j + 7];
			pdst_row[j] = (src0 <= thresh0) ? low0 : high0;
			pdst_row[j + 1] = (src1 <= thresh1) ? low1 : high1;
			pdst_row[j + 2] = (src2 <= thresh0) ? low0 : high0;
			pdst_row[j + 3] = (src3 <= thresh1) ? low1 : high1;
			pdst_row[j + 4] = (src4 <= thresh0) ? low0 : high0;
			pdst_row[j + 5] = (src5 <= thresh1) ? low1 : high1;
			pdst_row[j + 6] = (src6 <= thresh0) ? low0 : high0;
			pdst_row[j + 7] = (src7 <= thresh1) ? low1 : high1;
		}

		for (; j < width; j += 2) {
			pdst_row[j] = (psrc_row[j] <= thresh0) ? low0 : high0;
			pdst_row[j + 1] =
			    (psrc_row[j + 1] <= thresh1) ? low1 : high1;
		}
	}
}

/* *********************************************************** */

void
mlib_c_ImageThresh1_S323(
    PARAMS)
{
	mlib_s32 *thresh = (void *)__thresh;
	mlib_s32 *ghigh = (void *)__ghigh;
	mlib_s32 *glow = (void *)__glow;
	mlib_s32 i, j, k;
	mlib_s32 thresh0 = thresh[0], thresh1 = thresh[1];
	mlib_s32 thresh2 = thresh[2];
	mlib_s32 high0 = ghigh[0], low0 = glow[0];
	mlib_s32 high1 = ghigh[1], low1 = glow[1];
	mlib_s32 high2 = ghigh[2], low2 = glow[2];
	mlib_s32 src0, src1, src2, src3, src4, src5, src6, src7, src8, src9,
	    src10, src11;

	if (width < 16) {
		STRIP(pdst, psrc, width, height, 3, thresh, ghigh, glow);
		return;
	}

	width = width * 3;
	for (i = 0; i < height; i++) {
		mlib_s32 *psrc_row = (STYPE *) psrc + i * src_stride,
		    *pdst_row = (STYPE *) pdst + i * dst_stride;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j <= (width - 12); j += 12) {
			src0 = psrc_row[j];
			src1 = psrc_row[j + 1];
			src2 = psrc_row[j + 2];
			src3 = psrc_row[j + 3];
			src4 = psrc_row[j + 4];
			src5 = psrc_row[j + 5];
			src6 = psrc_row[j + 6];
			src7 = psrc_row[j + 7];
			src8 = psrc_row[j + 8];
			src9 = psrc_row[j + 9];
			src10 = psrc_row[j + 10];
			src11 = psrc_row[j + 11];
			pdst_row[j] = (src0 <= thresh0) ? low0 : high0;
			pdst_row[j + 1] = (src1 <= thresh1) ? low1 : high1;
			pdst_row[j + 2] = (src2 <= thresh2) ? low2 : high2;
			pdst_row[j + 3] = (src3 <= thresh0) ? low0 : high0;
			pdst_row[j + 4] = (src4 <= thresh1) ? low1 : high1;
			pdst_row[j + 5] = (src5 <= thresh2) ? low2 : high2;
			pdst_row[j + 6] = (src6 <= thresh0) ? low0 : high0;
			pdst_row[j + 7] = (src7 <= thresh1) ? low1 : high1;
			pdst_row[j + 8] = (src8 <= thresh2) ? low2 : high2;
			pdst_row[j + 9] = (src9 <= thresh0) ? low0 : high0;
			pdst_row[j + 10] = (src10 <= thresh1) ? low1 : high1;
			pdst_row[j + 11] = (src11 <= thresh2) ? low2 : high2;
		}

		for (; j < width; j += 3) {
			pdst_row[j] = (psrc_row[j] <= thresh0) ? low0 : high0;
			pdst_row[j + 1] =
			    (psrc_row[j + 1] <= thresh1) ? low1 : high1;
			pdst_row[j + 2] =
			    (psrc_row[j + 2] <= thresh2) ? low2 : high2;
		}
	}
}

/* *********************************************************** */

void
mlib_c_ImageThresh1_S324(
    PARAMS)
{
	mlib_s32 *thresh = (void *)__thresh;
	mlib_s32 *ghigh = (void *)__ghigh;
	mlib_s32 *glow = (void *)__glow;
	mlib_s32 i, j, k;
	mlib_s32 thresh0 = thresh[0], thresh1 = thresh[1];
	mlib_s32 thresh2 = thresh[2], thresh3 = thresh[3];
	mlib_s32 high0 = ghigh[0], low0 = glow[0];
	mlib_s32 high1 = ghigh[1], low1 = glow[1];
	mlib_s32 high2 = ghigh[2], low2 = glow[2];
	mlib_s32 high3 = ghigh[3], low3 = glow[3];
	mlib_s32 src0, src1, src2, src3, src4, src5, src6, src7;

	if (width < 16) {
		STRIP(pdst, psrc, width, height, 4, thresh, ghigh, glow);
		return;
	}

	width <<= 2;
	for (i = 0; i < height; i++) {
		mlib_s32 *psrc_row = (STYPE *) psrc + i * src_stride,
		    *pdst_row = (STYPE *) pdst + i * dst_stride;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j <= (width - 8); j += 8) {
			src0 = psrc_row[j];
			src1 = psrc_row[j + 1];
			src2 = psrc_row[j + 2];
			src3 = psrc_row[j + 3];
			src4 = psrc_row[j + 4];
			src5 = psrc_row[j + 5];
			src6 = psrc_row[j + 6];
			src7 = psrc_row[j + 7];
			pdst_row[j] = (src0 <= thresh0) ? low0 : high0;
			pdst_row[j + 1] = (src1 <= thresh1) ? low1 : high1;
			pdst_row[j + 2] = (src2 <= thresh2) ? low2 : high2;
			pdst_row[j + 3] = (src3 <= thresh3) ? low3 : high3;
			pdst_row[j + 4] = (src4 <= thresh0) ? low0 : high0;
			pdst_row[j + 5] = (src5 <= thresh1) ? low1 : high1;
			pdst_row[j + 6] = (src6 <= thresh2) ? low2 : high2;
			pdst_row[j + 7] = (src7 <= thresh3) ? low3 : high3;
		}

		for (; j < width; j += 4) {
			pdst_row[j] = (psrc_row[j] <= thresh0) ? low0 : high0;
			pdst_row[j + 1] =
			    (psrc_row[j + 1] <= thresh1) ? low1 : high1;
			pdst_row[j + 2] =
			    (psrc_row[j + 2] <= thresh2) ? low2 : high2;
			pdst_row[j + 3] =
			    (psrc_row[j + 3] <= thresh3) ? low3 : high3;
		}
	}
}

/* *********************************************************** */

#define	THRESH1_S32_CMP_SHIFT_LT0(s0, th, sh)                             \
		(((((th) - (s0)) | ~(s0)) >> (31 - (sh))) & (1 << (sh)))

#define	THRESH1_S32_CMP_SHIFT_GE0(s0, th, sh)                             \
		(((((th) - (s0)) & ~(s0)) >> (31 - (sh))) & (1 << (sh)))

/* *********************************************************** */

void
mlib_c_ImageThresh1_S321_1B_LT0(
    PARAMS,
    mlib_s32 dbit_off)
{
	mlib_s32 *thresh = (void *)__thresh;
	mlib_s32 *ghigh = (void *)__ghigh;
	mlib_s32 *glow = (void *)__glow;
	STYPE *psrc_row = psrc;
	mlib_u8 *pdst_row = pdst;
	mlib_s32 thresh0 = thresh[0];
	mlib_s32 mhigh, mlow, emask, dst0;
	mlib_s32 i, j, jbit, l;

	mhigh = (ghigh[0] > 0) ? 0xff : 0;
	mlow = (glow[0] > 0) ? 0xff : 0;

	for (i = 0; i < height; i++) {
		j = 0;
		jbit = 0;

		if (dbit_off) {
			mlib_s32 nume = 8 - dbit_off;

			if (nume > width)
				nume = width;
			dst0 = 0;
			emask = 0;

			for (; j < nume; j++) {
				emask |= (1 << (7 - (dbit_off + j)));
				dst0 |=
				    THRESH1_S32_CMP_SHIFT_LT0(psrc_row[j],
				    thresh0, 7 - (dbit_off + j));
			}

			dst0 = (mhigh & dst0) | (mlow & ~dst0);
			pdst_row[0] = (dst0 & emask) | (pdst_row[0] & ~emask);
			jbit++;
		}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; j <= (width - 16); j += 16) {
			dst0 =
			    THRESH1_S32_CMP_SHIFT_LT0(psrc_row[j], thresh0,
			    7) | THRESH1_S32_CMP_SHIFT_LT0(psrc_row[j + 1],
			    thresh0,
			    6) | THRESH1_S32_CMP_SHIFT_LT0(psrc_row[j + 2],
			    thresh0,
			    5) | THRESH1_S32_CMP_SHIFT_LT0(psrc_row[j + 3],
			    thresh0,
			    4) | THRESH1_S32_CMP_SHIFT_LT0(psrc_row[j + 4],
			    thresh0,
			    3) | THRESH1_S32_CMP_SHIFT_LT0(psrc_row[j + 5],
			    thresh0,
			    2) | THRESH1_S32_CMP_SHIFT_LT0(psrc_row[j + 6],
			    thresh0,
			    1) | THRESH1_S32_CMP_SHIFT_LT0(psrc_row[j + 7],
			    thresh0, 0);
			dst0 = (mhigh & dst0) | (mlow & ~dst0);
			*(pdst_row + jbit) = (mlib_u8)dst0;
			jbit++;
			dst0 =
			    THRESH1_S32_CMP_SHIFT_LT0(psrc_row[j + 8], thresh0,
			    7) | THRESH1_S32_CMP_SHIFT_LT0(psrc_row[j + 9],
			    thresh0,
			    6) | THRESH1_S32_CMP_SHIFT_LT0(psrc_row[j + 10],
			    thresh0,
			    5) | THRESH1_S32_CMP_SHIFT_LT0(psrc_row[j + 11],
			    thresh0,
			    4) | THRESH1_S32_CMP_SHIFT_LT0(psrc_row[j + 12],
			    thresh0,
			    3) | THRESH1_S32_CMP_SHIFT_LT0(psrc_row[j + 13],
			    thresh0,
			    2) | THRESH1_S32_CMP_SHIFT_LT0(psrc_row[j + 14],
			    thresh0,
			    1) | THRESH1_S32_CMP_SHIFT_LT0(psrc_row[j + 15],
			    thresh0, 0);
			dst0 = (mhigh & dst0) | (mlow & ~dst0);
			*(pdst_row + jbit) = (mlib_u8)dst0;
			jbit++;
		}

		if (width - j >= 8) {
			dst0 =
			    THRESH1_S32_CMP_SHIFT_LT0(psrc_row[j], thresh0,
			    7) | THRESH1_S32_CMP_SHIFT_LT0(psrc_row[j + 1],
			    thresh0,
			    6) | THRESH1_S32_CMP_SHIFT_LT0(psrc_row[j + 2],
			    thresh0,
			    5) | THRESH1_S32_CMP_SHIFT_LT0(psrc_row[j + 3],
			    thresh0,
			    4) | THRESH1_S32_CMP_SHIFT_LT0(psrc_row[j + 4],
			    thresh0,
			    3) | THRESH1_S32_CMP_SHIFT_LT0(psrc_row[j + 5],
			    thresh0,
			    2) | THRESH1_S32_CMP_SHIFT_LT0(psrc_row[j + 6],
			    thresh0,
			    1) | THRESH1_S32_CMP_SHIFT_LT0(psrc_row[j + 7],
			    thresh0, 0);
			dst0 = (mhigh & dst0) | (mlow & ~dst0);
			*(pdst_row + jbit) = (mlib_u8)dst0;
			jbit++;
			j += 8;
		}

		if (j < width) {
			dst0 = 0;
			l = 7;
			for (; j < width; j++) {
				dst0 |=
				    THRESH1_S32_CMP_SHIFT_LT0(psrc_row[j],
				    thresh0, l);
				l--;
			}

			emask = (0xFF << (l + 1));
			dst0 = (mhigh & dst0) | (mlow & ~dst0);
			pdst_row[jbit] =
			    (dst0 & emask) | (pdst_row[jbit] & ~emask);
		}

		psrc_row += src_stride;
		pdst_row += dst_stride;
	}
}

/* *********************************************************** */

void
mlib_c_ImageThresh1_S321_1B(
    PARAMS,
    mlib_s32 dbit_off)
{
	mlib_s32 *thresh = (void *)__thresh;
	mlib_s32 *ghigh = (void *)__ghigh;
	mlib_s32 *glow = (void *)__glow;
	STYPE *psrc_row = psrc;
	mlib_u8 *pdst_row = pdst;
	mlib_s32 thresh0 = thresh[0];
	mlib_s32 mhigh, mlow, emask, dst0;
	mlib_s32 i, j, jbit, l;

	if (thresh0 < 0) {
		mlib_c_ImageThresh1_S321_1B_LT0(psrc, pdst, src_stride,
		    dst_stride, width, height, thresh, ghigh, glow, dbit_off);
		return;
	}

	mhigh = (ghigh[0] > 0) ? 0xff : 0;
	mlow = (glow[0] > 0) ? 0xff : 0;

	for (i = 0; i < height; i++) {
		j = 0;
		jbit = 0;

		if (dbit_off) {
			mlib_s32 nume = 8 - dbit_off;

			if (nume > width)
				nume = width;
			dst0 = 0;
			emask = 0;

			for (; j < nume; j++) {
				emask |= (1 << (7 - (dbit_off + j)));
				dst0 |=
				    THRESH1_S32_CMP_SHIFT_GE0(psrc_row[j],
				    thresh0, 7 - (dbit_off + j));
			}

			dst0 = (mhigh & dst0) | (mlow & ~dst0);
			pdst_row[0] = (dst0 & emask) | (pdst_row[0] & ~emask);
			jbit++;
		}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; j <= (width - 16); j += 16) {
			dst0 =
			    THRESH1_S32_CMP_SHIFT_GE0(psrc_row[j], thresh0,
			    7) | THRESH1_S32_CMP_SHIFT_GE0(psrc_row[j + 1],
			    thresh0,
			    6) | THRESH1_S32_CMP_SHIFT_GE0(psrc_row[j + 2],
			    thresh0,
			    5) | THRESH1_S32_CMP_SHIFT_GE0(psrc_row[j + 3],
			    thresh0,
			    4) | THRESH1_S32_CMP_SHIFT_GE0(psrc_row[j + 4],
			    thresh0,
			    3) | THRESH1_S32_CMP_SHIFT_GE0(psrc_row[j + 5],
			    thresh0,
			    2) | THRESH1_S32_CMP_SHIFT_GE0(psrc_row[j + 6],
			    thresh0,
			    1) | THRESH1_S32_CMP_SHIFT_GE0(psrc_row[j + 7],
			    thresh0, 0);
			dst0 = (mhigh & dst0) | (mlow & ~dst0);
			*(pdst_row + jbit) = (mlib_u8)dst0;
			jbit++;
			dst0 =
			    THRESH1_S32_CMP_SHIFT_GE0(psrc_row[j + 8], thresh0,
			    7) | THRESH1_S32_CMP_SHIFT_GE0(psrc_row[j + 9],
			    thresh0,
			    6) | THRESH1_S32_CMP_SHIFT_GE0(psrc_row[j + 10],
			    thresh0,
			    5) | THRESH1_S32_CMP_SHIFT_GE0(psrc_row[j + 11],
			    thresh0,
			    4) | THRESH1_S32_CMP_SHIFT_GE0(psrc_row[j + 12],
			    thresh0,
			    3) | THRESH1_S32_CMP_SHIFT_GE0(psrc_row[j + 13],
			    thresh0,
			    2) | THRESH1_S32_CMP_SHIFT_GE0(psrc_row[j + 14],
			    thresh0,
			    1) | THRESH1_S32_CMP_SHIFT_GE0(psrc_row[j + 15],
			    thresh0, 0);
			dst0 = (mhigh & dst0) | (mlow & ~dst0);
			*(pdst_row + jbit) = (mlib_u8)dst0;
			jbit++;
		}

		if (width - j >= 8) {
			dst0 =
			    THRESH1_S32_CMP_SHIFT_GE0(psrc_row[j], thresh0,
			    7) | THRESH1_S32_CMP_SHIFT_GE0(psrc_row[j + 1],
			    thresh0,
			    6) | THRESH1_S32_CMP_SHIFT_GE0(psrc_row[j + 2],
			    thresh0,
			    5) | THRESH1_S32_CMP_SHIFT_GE0(psrc_row[j + 3],
			    thresh0,
			    4) | THRESH1_S32_CMP_SHIFT_GE0(psrc_row[j + 4],
			    thresh0,
			    3) | THRESH1_S32_CMP_SHIFT_GE0(psrc_row[j + 5],
			    thresh0,
			    2) | THRESH1_S32_CMP_SHIFT_GE0(psrc_row[j + 6],
			    thresh0,
			    1) | THRESH1_S32_CMP_SHIFT_GE0(psrc_row[j + 7],
			    thresh0, 0);
			dst0 = (mhigh & dst0) | (mlow & ~dst0);
			*(pdst_row + jbit) = (mlib_u8)dst0;
			jbit++;
			j += 8;
		}

		if (j < width) {
			dst0 = 0;
			l = 7;
			for (; j < width; j++) {
				dst0 |=
				    THRESH1_S32_CMP_SHIFT_GE0(psrc_row[j],
				    thresh0, l);
				l--;
			}

			emask = (0xFF << (l + 1));
			dst0 = (mhigh & dst0) | (mlow & ~dst0);
			pdst_row[jbit] =
			    (dst0 & emask) | (pdst_row[jbit] & ~emask);
		}

		psrc_row += src_stride;
		pdst_row += dst_stride;
	}
}

/* *********************************************************** */

void
mlib_c_ImageThresh1_S322_1B(
    PARAMS,
    mlib_s32 dbit_off)
{
	mlib_s32 *thresh = (void *)__thresh;
	mlib_s32 *ghigh = (void *)__ghigh;
	mlib_s32 *glow = (void *)__glow;
	STYPE *psrc_row = psrc;
	mlib_u8 *pdst_row = pdst;
	TTYPE thresh0 = thresh[0], thresh1 = thresh[1];
	mlib_s32 mhigh0, mlow0, mhigh, mlow, emask, dst0;
	mlib_s32 i, j, jbit, l;

	mhigh0 = (ghigh[0] > 0) ? 0xaaa : 0;
	mhigh0 |= (ghigh[1] > 0) ? 0x555 : 0;
	mlow0 = (glow[0] > 0) ? 0xaaa : 0;
	mlow0 |= (glow[1] > 0) ? 0x555 : 0;

	width *= 2;

	for (i = 0; i < height; i++) {
		thresh0 = thresh[0];
		thresh1 = thresh[1];

		j = 0;
		jbit = 0;
		mhigh = mhigh0 >> (dbit_off & 1);
		mlow = mlow0 >> (dbit_off & 1);

		if (dbit_off) {
			mlib_s32 nume = 8 - dbit_off;

			if (nume > width)
				nume = width;
			dst0 = 0;
			emask = 0;

			for (; j <= (nume - 2); j += 2) {
				emask |= (3 << (6 - (dbit_off + j)));
				dst0 |=
				    THRESH1_CMP_SHIFT(psrc_row[j], thresh0,
				    7 - (dbit_off + j));
				dst0 |=
				    THRESH1_CMP_SHIFT(psrc_row[j + 1], thresh1,
				    6 - (dbit_off + j));
			}

			if (j < nume) {
				emask |= (1 << (7 - (dbit_off + j)));
				dst0 |=
				    THRESH1_CMP_SHIFT(psrc_row[j], thresh0,
				    7 - (dbit_off + j));
/* swap threshes */
				thresh0 = thresh[1];
				thresh1 = thresh[0];
				j++;
			}

			dst0 = (mhigh & dst0) | (mlow & ~dst0);
			pdst_row[0] = (dst0 & emask) | (pdst_row[0] & ~emask);
			jbit++;
		}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; j <= (width - 16); j += 16) {
			dst0 = THRESH1_CMP_SHIFT(psrc_row[j], thresh0, 7) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 1], thresh1, 6) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 2], thresh0, 5) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 3], thresh1, 4) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 4], thresh0, 3) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 5], thresh1, 2) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 6], thresh0, 1) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 7], thresh1, 0);
			dst0 = (mhigh & dst0) | (mlow & ~dst0);
			*(pdst_row + jbit) = (mlib_u8)dst0;
			jbit++;
			dst0 = THRESH1_CMP_SHIFT(psrc_row[j + 8], thresh0, 7) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 9], thresh1, 6) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 10], thresh0, 5) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 11], thresh1, 4) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 12], thresh0, 3) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 13], thresh1, 2) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 14], thresh0, 1) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 15], thresh1, 0);
			dst0 = (mhigh & dst0) | (mlow & ~dst0);
			*(pdst_row + jbit) = (mlib_u8)dst0;
			jbit++;
		}

		if (width - j >= 8) {
			dst0 = THRESH1_CMP_SHIFT(psrc_row[j], thresh0, 7) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 1], thresh1, 6) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 2], thresh0, 5) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 3], thresh1, 4) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 4], thresh0, 3) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 5], thresh1, 2) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 6], thresh0, 1) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 7], thresh1, 0);
			dst0 = (mhigh & dst0) | (mlow & ~dst0);
			*(pdst_row + jbit) = (mlib_u8)dst0;
			jbit++;
			j += 8;
		}

		if (j < width) {
			dst0 = 0;
			l = 7;
			for (; j <= (width - 2); j += 2) {
				dst0 |=
				    THRESH1_CMP_SHIFT(psrc_row[j], thresh0, l);
				dst0 |=
				    THRESH1_CMP_SHIFT(psrc_row[j + 1], thresh1,
				    l - 1);
				l -= 2;
			}

			if (j < width) {
				dst0 |=
				    THRESH1_CMP_SHIFT(psrc_row[j], thresh0, l);
				l--;
			}

			emask = (0xFF << (l + 1));
			dst0 = (mhigh & dst0) | (mlow & ~dst0);
			pdst_row[jbit] =
			    (dst0 & emask) | (pdst_row[jbit] & ~emask);
		}

		psrc_row += src_stride;
		pdst_row += dst_stride;
	}
}

/* *********************************************************** */

void
mlib_c_ImageThresh1_S323_1B(
    PARAMS,
    mlib_s32 dbit_off)
{
	mlib_s32 *thresh = (void *)__thresh;
	mlib_s32 *ghigh = (void *)__ghigh;
	mlib_s32 *glow = (void *)__glow;
	STYPE *psrc_row = psrc;
	mlib_u8 *pdst_row = pdst;
	TTYPE thresh0, thresh1, thresh2, threshT;
	mlib_s32 mhigh = 0, mlow =
	    0, mhigh0, mlow0, mhigh1, mlow1, mhigh2, mlow2, emask, dst0, dst1;
	mlib_s32 i, j, jbit, k, l;

	if (ghigh[0] > 0)
		mhigh = 0x492492;

	if (ghigh[1] > 0)
		mhigh |= 0x249249;

	if (ghigh[2] > 0)
		mhigh |= 0x924924;

	if (glow[0] > 0)
		mlow = 0x492492;

	if (glow[1] > 0)
		mlow |= 0x249249;

	if (glow[2] > 0)
		mlow |= 0x924924;

	width = 3 * width;

	for (i = 0; i < height; i++) {
		thresh0 = thresh[0];
		thresh1 = thresh[1];
		thresh2 = thresh[2];

		j = 0;
		jbit = 0;
		mhigh0 = mhigh >> (dbit_off & 7);
		mlow0 = mlow >> (dbit_off & 7);
		mhigh1 = mhigh0 >> 1;
		mlow1 = mlow0 >> 1;
		mhigh2 = mhigh0 >> 2;
		mlow2 = mlow0 >> 2;

		if (dbit_off) {
			mlib_s32 nume = 8 - dbit_off;

			if (nume > width)
				nume = width;
			dst0 = 0;
			emask = 0;

			for (; j <= (nume - 3); j += 3) {
				emask |= (7 << (5 - (dbit_off + j)));
				dst0 |=
				    THRESH1_CMP_SHIFT(psrc_row[j], thresh0,
				    7 - (dbit_off + j));
				dst0 |=
				    THRESH1_CMP_SHIFT(psrc_row[j + 1], thresh1,
				    6 - (dbit_off + j));
				dst0 |=
				    THRESH1_CMP_SHIFT(psrc_row[j + 2], thresh2,
				    5 - (dbit_off + j));
			}

			for (; j < nume; j++) {
				emask |= (1 << (7 - (dbit_off + j)));
				dst0 |=
				    THRESH1_CMP_SHIFT(psrc_row[j], thresh0,
				    7 - (dbit_off + j));
/* swap threshes */
				threshT = thresh0;
				thresh0 = thresh1;
				thresh1 = thresh2;
				thresh2 = threshT;
			}

			dst0 = (mhigh0 & dst0) | (mlow0 & ~dst0);
			pdst_row[0] = (dst0 & emask) | (pdst_row[0] & ~emask);
			jbit++;

			mhigh0 = mhigh >> (9 - nume);
			mlow0 = mlow >> (9 - nume);
			mhigh1 = mhigh0 >> 1;
			mlow1 = mlow0 >> 1;
			mhigh2 = mhigh0 >> 2;
			mlow2 = mlow0 >> 2;
		}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; j <= (width - 24); j += 24) {
			dst0 = (THRESH1_CMP_SHIFT(psrc_row[j], thresh0, 7) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 1], thresh1, 6) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 2], thresh2, 5) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 3], thresh0, 4) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 4], thresh1, 3) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 5], thresh2, 2) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 6], thresh0, 1) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 7], thresh1, 0));
			dst0 = (mhigh0 & dst0) | (mlow0 & ~dst0);
			*(pdst_row + jbit) = dst0;
			jbit++;
			dst0 = (THRESH1_CMP_SHIFT(psrc_row[j + 8], thresh2, 7) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 9], thresh0, 6) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 10], thresh1, 5) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 11], thresh2, 4) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 12], thresh0, 3) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 13], thresh1, 2) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 14], thresh2, 1) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 15], thresh0, 0));
			dst0 = (mhigh1 & dst0) | (mlow1 & ~dst0);
			*(pdst_row + jbit) = dst0;
			jbit++;
			dst0 =
			    (THRESH1_CMP_SHIFT(psrc_row[j + 16], thresh1,
			    7) | THRESH1_CMP_SHIFT(psrc_row[j + 17], thresh2,
			    6) | THRESH1_CMP_SHIFT(psrc_row[j + 18], thresh0,
			    5) | THRESH1_CMP_SHIFT(psrc_row[j + 19], thresh1,
			    4) | THRESH1_CMP_SHIFT(psrc_row[j + 20], thresh2,
			    3) | THRESH1_CMP_SHIFT(psrc_row[j + 21], thresh0,
			    2) | THRESH1_CMP_SHIFT(psrc_row[j + 22], thresh1,
			    1) | THRESH1_CMP_SHIFT(psrc_row[j + 23], thresh2,
			    0));
			dst0 = (mhigh2 & dst0) | (mlow2 & ~dst0);
			*(pdst_row + jbit) = dst0;
			jbit++;
		}

		if (j < width) {
			k = width - j;
			dst0 = 0;
			l = 31;
			for (; j < width - 3; j += 3) {
				dst0 |=
				    (THRESH1_CMP_SHIFT(psrc_row[j], thresh0,
				    l) | THRESH1_CMP_SHIFT(psrc_row[j + 1],
				    thresh1,
				    l - 1) | THRESH1_CMP_SHIFT(psrc_row[j + 2],
				    thresh2, l - 2));
				l -= 3;
			}

			if (j < width) {
				dst0 |= THRESH1_CMP_SHIFT(
					psrc_row[j],
					thresh0, l);
				if ((j + 1) < width) {
					dst0 |= THRESH1_CMP_SHIFT(
						psrc_row[j + 1],
						thresh1, l - 1);
					if ((j + 2) < width) {
						dst0 |= THRESH1_CMP_SHIFT(
							psrc_row[j + 2],
							thresh2, l - 2);
					}
				}
			}

			l = (k + 7) >> 3;
			k = (l << 3) - k;
			emask = (0xFF << k);

			if (l == 3) {
				dst1 = dst0 >> 24;
				dst1 = (mhigh0 & dst1) | (mlow0 & ~dst1);
				pdst_row[jbit] = dst1;
				dst1 = (dst0 >> 16);
				dst1 = (mhigh1 & dst1) | (mlow1 & ~dst1);
				pdst_row[jbit + 1] = dst1;
				dst1 = (dst0 >> 8);
				dst1 = (mhigh2 & dst1) | (mlow2 & ~dst1);
				pdst_row[jbit + 2] =
				    (dst1 & emask) | (pdst_row[jbit +
				    2] & ~emask);
			} else if (l == 2) {
				dst1 = dst0 >> 24;
				dst1 = (mhigh0 & dst1) | (mlow0 & ~dst1);
				pdst_row[jbit] = dst1;
				dst1 = (dst0 >> 16);
				dst1 = (mhigh1 & dst1) | (mlow1 & ~dst1);
				pdst_row[jbit + 1] =
				    (dst1 & emask) | (pdst_row[jbit +
				    1] & ~emask);
			} else {
				dst1 = dst0 >> 24;
				dst1 = (mhigh0 & dst1) | (mlow0 & ~dst1);
				pdst_row[jbit] =
				    (dst1 & emask) | (pdst_row[jbit] & ~emask);
			}
		}

		psrc_row += src_stride;
		pdst_row += dst_stride;
	}
}

/* *********************************************************** */

void
mlib_c_ImageThresh1_S324_1B(
    PARAMS,
    mlib_s32 dbit_off)
{
	mlib_s32 *thresh = (void *)__thresh;
	mlib_s32 *ghigh = (void *)__ghigh;
	mlib_s32 *glow = (void *)__glow;
	STYPE *psrc_row = psrc;
	mlib_u8 *pdst_row = pdst;
	TTYPE thresh0, thresh1, thresh2, thresh3, threshT;
	mlib_s32 mhigh0, mlow0, mhigh, mlow, emask, dst0;
	mlib_s32 i, j, jbit;

	mhigh0 = (ghigh[0] > 0) ? 0x8888 : 0;
	mhigh0 |= (ghigh[1] > 0) ? 0x4444 : 0;
	mhigh0 |= (ghigh[2] > 0) ? 0x2222 : 0;
	mhigh0 |= (ghigh[3] > 0) ? 0x1111 : 0;

	mlow0 = (glow[0] > 0) ? 0x8888 : 0;
	mlow0 |= (glow[1] > 0) ? 0x4444 : 0;
	mlow0 |= (glow[2] > 0) ? 0x2222 : 0;
	mlow0 |= (glow[3] > 0) ? 0x1111 : 0;

	width *= 4;

	for (i = 0; i < height; i++) {
		thresh0 = thresh[0];
		thresh1 = thresh[1];
		thresh2 = thresh[2];
		thresh3 = thresh[3];

		j = 0;
		jbit = 0;
		mhigh = mhigh0 >> dbit_off;
		mlow = mlow0 >> dbit_off;

		if (dbit_off) {
			mlib_s32 nume = 8 - dbit_off;

			if (nume > width)
				nume = width;
			dst0 = 0;
			emask = 0;

			for (; j <= (nume - 4); j += 4) {
				emask |= (0xf << (4 - (dbit_off + j)));
				dst0 |=
				    THRESH1_CMP_SHIFT(psrc_row[j], thresh0,
				    7 - (dbit_off + j));
				dst0 |=
				    THRESH1_CMP_SHIFT(psrc_row[j + 1], thresh1,
				    6 - (dbit_off + j));
				dst0 |=
				    THRESH1_CMP_SHIFT(psrc_row[j + 2], thresh2,
				    5 - (dbit_off + j));
				dst0 |=
				    THRESH1_CMP_SHIFT(psrc_row[j + 3], thresh3,
				    4 - (dbit_off + j));
			}

			for (; j < nume; j++) {
				emask |= (1 << (7 - (dbit_off + j)));
				dst0 |=
				    THRESH1_CMP_SHIFT(psrc_row[j], thresh0,
				    7 - (dbit_off + j));
/* swap threshes */
				threshT = thresh0;
				thresh0 = thresh1;
				thresh1 = thresh2;
				thresh2 = thresh3;
				thresh3 = threshT;
			}

			dst0 = (mhigh & dst0) | (mlow & ~dst0);
			pdst_row[0] = (dst0 & emask) | (pdst_row[0] & ~emask);
			jbit++;
		}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; j <= (width - 16); j += 16) {
			dst0 = (THRESH1_CMP_SHIFT(psrc_row[j], thresh0, 7) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 1], thresh1, 6) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 2], thresh2, 5) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 3], thresh3, 4) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 4], thresh0, 3) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 5], thresh1, 2) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 6], thresh2, 1) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 7], thresh3, 0));
			dst0 = (mhigh & dst0) | (mlow & ~dst0);
			pdst_row[jbit] = dst0;
			jbit++;
			dst0 = (THRESH1_CMP_SHIFT(psrc_row[j + 8], thresh0, 7) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 9], thresh1, 6) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 10], thresh2, 5) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 11], thresh3, 4) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 12], thresh0, 3) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 13], thresh1, 2) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 14], thresh2, 1) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 15], thresh3, 0));
			dst0 = (mhigh & dst0) | (mlow & ~dst0);
			pdst_row[jbit] = dst0;
			jbit++;
		}

		if (j <= width - 8) {
			dst0 = (THRESH1_CMP_SHIFT(psrc_row[j], thresh0, 7) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 1], thresh1, 6) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 2], thresh2, 5) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 3], thresh3, 4) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 4], thresh0, 3) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 5], thresh1, 2) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 6], thresh2, 1) |
			    THRESH1_CMP_SHIFT(psrc_row[j + 7], thresh3, 0));
			dst0 = (mhigh & dst0) | (mlow & ~dst0);
			pdst_row[jbit] = dst0;
			jbit++;
			j += 8;
		}

		if (j < width) {
		    dst0 = THRESH1_CMP_SHIFT(psrc_row[j], thresh0, 7);
		    if ((j + 1) < width) {
			dst0 |= THRESH1_CMP_SHIFT(psrc_row[j + 1], thresh1, 6);
			if ((j + 2) < width) {
			    dst0 |= THRESH1_CMP_SHIFT(
				psrc_row[j + 2], thresh2, 5);
			    if ((j + 3) < width) {
				dst0 |= THRESH1_CMP_SHIFT(
					psrc_row[j + 3], thresh3, 4);
				if ((j + 4) < width) {
				    dst0 |= THRESH1_CMP_SHIFT(
					psrc_row[j + 4], thresh0, 3);
				    if ((j + 5) < width) {
					dst0 |= THRESH1_CMP_SHIFT(
						psrc_row[j + 5], thresh1, 2);
					if ((j + 6) < width) {
					    dst0 |= THRESH1_CMP_SHIFT(
						psrc_row[j + 6], thresh2, 1);
					}
				    }
				}
			    }
			}
		    }

		    emask = (0xFF << (8 - (width - j)));
		    dst0 = (mhigh & dst0) | (mlow & ~dst0);
		    pdst_row[jbit] =
			(dst0 & emask) | (pdst_row[jbit] & ~emask);
		}

		psrc_row += src_stride;
		pdst_row += dst_stride;
	}
}

/* *********************************************************** */
