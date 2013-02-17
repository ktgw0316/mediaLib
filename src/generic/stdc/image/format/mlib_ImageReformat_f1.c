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

#pragma ident	"@(#)mlib_ImageReformat_f1.c	9.2	07/10/09 SMI"

/*
 * FILENAME: mlib_ImageReformat_f1.c
 *
 * FUNCTIONS
 *      mlib_ImageReformat_U8_U8
 *      mlib_ImageReformat_U8_U16
 *
 *      mlib_ImageReformat_S16_U8
 *      mlib_ImageReformat_S16_S16
 *      mlib_ImageReformat_S16_U16
 *
 *      mlib_ImageReformat_U16_U8
 *      mlib_ImageReformat_U16_S16
 *      mlib_ImageReformat_U16_U16
 *
 *      mlib_ImageReformat_S32_U8
 *      mlib_ImageReformat_S32_S16
 *      mlib_ImageReformat_S32_U16
 *      mlib_ImageReformat_S32_S32
 *
 * ARGUMENTS
 *      dstData            The pointer to the destination image data buffer.
 *      srcData            The pointer to the source image data buffer.
 *      numBands           The number of channels of the image data buffers.
 *      xSize              The width of the image.
 *      ySize              The height of the image.
 *      dstBandoffsets     The initial pixel's offsets in the dstData buffer
 *                              in terms of destination data buffer elements.
 *      dstScanlinestride  The scanline stride of the dstData buffer in terms
 *                              of destination data buffer elements.
 *      dstPixelstride     The pixel stride of the dstData buffer in terms
 *                              of destination data buffer elements.
 *      srcBandoffsets     The initial pixel's offsets in the srcData buffer
 *                              in terms of source data buffer elements.
 *      srcScanlinestride  The scanline stride of the srcData buffer in terms
 *                              of source data buffer elements.
 *      srcPixelstride     The pixel stride of the srcData buffer in terms
 *                              of source data buffer elements.
 *
 * DESCRIPTION
 *      Image data buffer reformat.
 *      These functions are separated from mlib_ImageReformat.c
 *      for loop unrolling and structure clarity.
 */

#include <mlib_image.h>
#include <mlib_ImageReformat.h>

/* *********************************************************** */

#define	MLIB_INT_CLAMP_U8_U16(dst, src)                         \
	{                                                       \
	    dst = ((con - src) >> 31) | src;                    \
	}

/* *********************************************************** */

#define	MLIB_INT_CLAMP_S16_U16(dst, src)                        \
	{                                                       \
	    dst = (((0x7FFF - src) >> 31) | src) & 0x7FFF;      \
	}

/* *********************************************************** */

#define	MLIB_INT_CLAMP_U16_S16(dst, src)                        \
	{                                                       \
	    dst = ((~src) >> 31) & src;                         \
	}

/* *********************************************************** */

/* Reformat an MLIB_BYTE image into another MLIB_BYTE image. */

void
mlib_ImageReformat_U8_U8(
    REFORMAT_PARAMS1)
{
/* pointer for pixels in source */
	mlib_u8 *sp;

/* pointer for pixels in destination */
	mlib_u8 *dp;

/* pointer for lines in source */
	mlib_u8 *sl;

/* pointer for lines in destination */
	mlib_u8 *dl;
	mlib_s32 i, j, k;

	for (k = 0; k < numBands; k++) {
		sl = sp = (mlib_u8 *)srcData[k] + srcBandoffsets[k];
		dl = dp = (mlib_u8 *)dstData[k] + dstBandoffsets[k];
		for (j = 0; j < ySize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < xSize; i++) {
				*dp = *sp;
				sp += srcPixelstride;
				dp += dstPixelstride;
			}

			sp = sl += srcScanlinestride;
			dp = dl += dstScanlinestride;
		}
	}
}

/* *********************************************************** */

/* Reformat an MLIB_USHORT image into an MLIB_BYTE image. */

void
mlib_ImageReformat_U8_U16(
    REFORMAT_PARAMS1)
{
/* pointer for pixels in source */
	mlib_u16 *sp;

/* pointer for pixels in destination */
	mlib_u8 *dp;

/* pointer for lines in source */
	mlib_u16 *sl;

/* pointer for lines in destination */
	mlib_u8 *dl;
	mlib_s32 i, j, k;
	mlib_s32 con = 255;

	for (k = 0; k < numBands; k++) {
		sl = sp = (mlib_u16 *)srcData[k] + srcBandoffsets[k];
		dl = dp = (mlib_u8 *)dstData[k] + dstBandoffsets[k];
		for (j = 0; j < ySize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (xSize - 8); i += 8) {
				MLIB_INT_CLAMP_U8_U16(*dp, *sp);
				sp += srcPixelstride;
				dp += dstPixelstride;
				MLIB_INT_CLAMP_U8_U16(*dp, *sp);
				sp += srcPixelstride;
				dp += dstPixelstride;
				MLIB_INT_CLAMP_U8_U16(*dp, *sp);
				sp += srcPixelstride;
				dp += dstPixelstride;
				MLIB_INT_CLAMP_U8_U16(*dp, *sp);
				sp += srcPixelstride;
				dp += dstPixelstride;
				MLIB_INT_CLAMP_U8_U16(*dp, *sp);
				sp += srcPixelstride;
				dp += dstPixelstride;
				MLIB_INT_CLAMP_U8_U16(*dp, *sp);
				sp += srcPixelstride;
				dp += dstPixelstride;
				MLIB_INT_CLAMP_U8_U16(*dp, *sp);
				sp += srcPixelstride;
				dp += dstPixelstride;
				MLIB_INT_CLAMP_U8_U16(*dp, *sp);
				sp += srcPixelstride;
				dp += dstPixelstride;
			}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i < xSize; i++) {
				MLIB_INT_CLAMP_U8_U16(*dp, *sp);
				sp += srcPixelstride;
				dp += dstPixelstride;
			}

			sp = sl += srcScanlinestride;
			dp = dl += dstScanlinestride;
		}
	}
}

/* *********************************************************** */

/* Reformat an MLIB_BYTE image into an MLIB_SHORT image. */

void
mlib_ImageReformat_S16_U8(
    REFORMAT_PARAMS1)
{
/* pointer for pixels in source */
	mlib_u8 *sp;

/* pointer for pixels in destination */
	mlib_s16 *dp;

/* pointer for lines in source */
	mlib_u8 *sl;

/* pointer for lines in destination */
	mlib_s16 *dl;
	mlib_s32 i, j, k;

	for (k = 0; k < numBands; k++) {
		sl = sp = (mlib_u8 *)srcData[k] + srcBandoffsets[k];
		dl = dp = (mlib_s16 *)dstData[k] + dstBandoffsets[k];
		for (j = 0; j < ySize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < xSize; i++) {
				*dp = (mlib_s16)(*sp);
				sp += srcPixelstride;
				dp += dstPixelstride;
			}

			sp = sl += srcScanlinestride;
			dp = dl += dstScanlinestride;
		}
	}
}

/* *********************************************************** */

/* Reformat an MLIB_SHORT image into another MLIB_SHORT image. */

void
mlib_ImageReformat_S16_S16(
    REFORMAT_PARAMS1)
{
/* pointer for pixels in source */
	mlib_u16 *sp;

/* pointer for pixels in destination */
	mlib_u16 *dp;

/* pointer for lines in source */
	mlib_u16 *sl;

/* pointer for lines in destination */
	mlib_u16 *dl;
	mlib_s32 i, j, k;

	for (k = 0; k < numBands; k++) {
		sl = sp = (mlib_u16 *)srcData[k] + srcBandoffsets[k];
		dl = dp = (mlib_u16 *)dstData[k] + dstBandoffsets[k];
		for (j = 0; j < ySize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < xSize; i++) {
				dp[i * dstPixelstride] = sp[i * srcPixelstride];
			}

			sp = sl += srcScanlinestride;
			dp = dl += dstScanlinestride;
		}
	}
}

/* *********************************************************** */

/* Reformat an MLIB_USHORT image into another MLIB_SHORT image. */

void
mlib_ImageReformat_S16_U16(
    REFORMAT_PARAMS1)
{
/* pointer for pixels in source */
	mlib_u16 *sp;

/* pointer for pixels in destination */
	mlib_s16 *dp;

/* pointer for lines in source */
	mlib_u16 *sl;

/* pointer for lines in destination */
	mlib_s16 *dl;
	mlib_s32 i, j, k;

	for (k = 0; k < numBands; k++) {
		sl = sp = (mlib_u16 *)srcData[k] + srcBandoffsets[k];
		dl = dp = (mlib_s16 *)dstData[k] + dstBandoffsets[k];
		for (j = 0; j < ySize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (xSize - 4); i += 4) {
				MLIB_INT_CLAMP_S16_U16(*dp, *sp);
				sp += srcPixelstride;
				dp += dstPixelstride;
				MLIB_INT_CLAMP_S16_U16(*dp, *sp);
				sp += srcPixelstride;
				dp += dstPixelstride;
				MLIB_INT_CLAMP_S16_U16(*dp, *sp);
				sp += srcPixelstride;
				dp += dstPixelstride;
				MLIB_INT_CLAMP_S16_U16(*dp, *sp);
				sp += srcPixelstride;
				dp += dstPixelstride;
			}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i < xSize; i++) {
				MLIB_INT_CLAMP_S16_U16(*dp, *sp);
				sp += srcPixelstride;
				dp += dstPixelstride;
			}

			sp = sl += srcScanlinestride;
			dp = dl += dstScanlinestride;
		}
	}
}

/* *********************************************************** */

/* Reformat an MLIB_BYTE image into an MLIB_SHORT image. */

void
mlib_ImageReformat_U16_U8(
    REFORMAT_PARAMS1)
{
/* pointer for pixels in source */
	mlib_u8 *sp;

/* pointer for pixels in destination */
	mlib_u16 *dp;

/* pointer for lines in source */
	mlib_u8 *sl;

/* pointer for lines in destination */
	mlib_u16 *dl;
	mlib_s32 i, j, k;

	for (k = 0; k < numBands; k++) {
		sl = sp = (mlib_u8 *)srcData[k] + srcBandoffsets[k];
		dl = dp = (mlib_u16 *)dstData[k] + dstBandoffsets[k];
		for (j = 0; j < ySize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < xSize; i++) {
				*dp = (mlib_u16)(*sp);
				sp += srcPixelstride;
				dp += dstPixelstride;
			}

			sp = sl += srcScanlinestride;
			dp = dl += dstScanlinestride;
		}
	}
}

/* *********************************************************** */

/* Reformat an MLIB_SHORT image into another MLIB_USHORT image. */

void
mlib_ImageReformat_U16_S16(
    REFORMAT_PARAMS1)
{
/* pointer for pixels in source */
	mlib_s16 *sp;

/* pointer for pixels in destination */
	mlib_u16 *dp;

/* pointer for lines in source */
	mlib_s16 *sl;

/* pointer for lines in destination */
	mlib_u16 *dl;
	mlib_s32 i, j, k;

	for (k = 0; k < numBands; k++) {
		sl = sp = (mlib_s16 *)srcData[k] + srcBandoffsets[k];
		dl = dp = (mlib_u16 *)dstData[k] + dstBandoffsets[k];
		for (j = 0; j < ySize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i <= (xSize - 4); i += 4) {
				MLIB_INT_CLAMP_U16_S16(*dp, *sp);
				sp += srcPixelstride;
				dp += dstPixelstride;
				MLIB_INT_CLAMP_U16_S16(*dp, *sp);
				sp += srcPixelstride;
				dp += dstPixelstride;
				MLIB_INT_CLAMP_U16_S16(*dp, *sp);
				sp += srcPixelstride;
				dp += dstPixelstride;
				MLIB_INT_CLAMP_U16_S16(*dp, *sp);
				sp += srcPixelstride;
				dp += dstPixelstride;
			}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; i < xSize; i++) {
				MLIB_INT_CLAMP_U16_S16(*dp, *sp);
				sp += srcPixelstride;
				dp += dstPixelstride;
			}

			sp = sl += srcScanlinestride;
			dp = dl += dstScanlinestride;
		}
	}
}

/* *********************************************************** */

/* Reformat an MLIB_USHORT image into another MLIB_USHORT image. */

void
mlib_ImageReformat_U16_U16(
    REFORMAT_PARAMS1)
{
/* pointer for pixels in source */
	mlib_u16 *sp;

/* pointer for pixels in destination */
	mlib_u16 *dp;

/* pointer for lines in source */
	mlib_u16 *sl;

/* pointer for lines in destination */
	mlib_u16 *dl;
	mlib_s32 i, j, k;

	for (k = 0; k < numBands; k++) {
		sl = sp = (mlib_u16 *)srcData[k] + srcBandoffsets[k];
		dl = dp = (mlib_u16 *)dstData[k] + dstBandoffsets[k];
		for (j = 0; j < ySize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < xSize; i++) {
				dp[i * dstPixelstride] = sp[i * srcPixelstride];
			}

			sp = sl += srcScanlinestride;
			dp = dl += dstScanlinestride;
		}
	}
}

/* *********************************************************** */

/* Reformat an MLIB_BYTE image into an MLIB_INT image. */

void
mlib_ImageReformat_S32_U8(
    REFORMAT_PARAMS1)
{
/* pointer for pixels in source */
	mlib_u8 *sp;

/* pointer for pixels in destination */
	mlib_s32 *dp;

/* pointer for lines in source */
	mlib_u8 *sl;

/* pointer for lines in destination */
	mlib_s32 *dl;
	mlib_s32 i, j, k;

	for (k = 0; k < numBands; k++) {
		sl = sp = (mlib_u8 *)srcData[k] + srcBandoffsets[k];
		dl = dp = (mlib_s32 *)dstData[k] + dstBandoffsets[k];
		for (j = 0; j < ySize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < xSize; i++) {
				dp[i * dstPixelstride] = sp[i * srcPixelstride];
			}

			sp = sl += srcScanlinestride;
			dp = dl += dstScanlinestride;
		}
	}
}

/* *********************************************************** */

/* Reformat an MLIB_SHORT image into an MLIB_INT image. */

void
mlib_ImageReformat_S32_S16(
    REFORMAT_PARAMS1)
{
/* pointer for pixels in source */
	mlib_s16 *sp;

/* pointer for pixels in destination */
	mlib_s32 *dp;

/* pointer for lines in source */
	mlib_s16 *sl;

/* pointer for lines in destination */
	mlib_s32 *dl;
	mlib_s32 i, j, k;

	for (k = 0; k < numBands; k++) {
		sl = sp = (mlib_s16 *)srcData[k] + srcBandoffsets[k];
		dl = dp = (mlib_s32 *)dstData[k] + dstBandoffsets[k];
		for (j = 0; j < ySize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < xSize; i++) {
				dp[i * dstPixelstride] = sp[i * srcPixelstride];
			}

			sp = sl += srcScanlinestride;
			dp = dl += dstScanlinestride;
		}
	}
}

/* *********************************************************** */

/* Reformat an MLIB_USHORT image into an MLIB_INT image. */

void
mlib_ImageReformat_S32_U16(
    REFORMAT_PARAMS1)
{
/* pointer for pixels in source */
	mlib_u16 *sp;

/* pointer for pixels in destination */
	mlib_s32 *dp;

/* pointer for lines in source */
	mlib_u16 *sl;

/* pointer for lines in destination */
	mlib_s32 *dl;
	mlib_s32 i, j, k;

	for (k = 0; k < numBands; k++) {
		sl = sp = (mlib_u16 *)srcData[k] + srcBandoffsets[k];
		dl = dp = (mlib_s32 *)dstData[k] + dstBandoffsets[k];
		for (j = 0; j < ySize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < xSize; i++) {
				dp[i * dstPixelstride] = sp[i * srcPixelstride];
			}

			sp = sl += srcScanlinestride;
			dp = dl += dstScanlinestride;
		}
	}
}

/* *********************************************************** */

/* Reformat an MLIB_INT image into another MLIB_INT image. */

void
mlib_ImageReformat_S32_S32(
    REFORMAT_PARAMS1)
{
/* pointer for pixels in source */
	mlib_s32 *sp;

/* pointer for pixels in destination */
	mlib_s32 *dp;

/* pointer for lines in source */
	mlib_s32 *sl;

/* pointer for lines in destination */
	mlib_s32 *dl;
	mlib_s32 i, j, k;

	for (k = 0; k < numBands; k++) {
		sl = sp = (mlib_s32 *)srcData[k] + srcBandoffsets[k];
		dl = dp = (mlib_s32 *)dstData[k] + dstBandoffsets[k];
		for (j = 0; j < ySize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < xSize; i++) {
				dp[i * dstPixelstride] = sp[i * srcPixelstride];
			}

			sp = sl += srcScanlinestride;
			dp = dl += dstScanlinestride;
		}
	}
}

/* *********************************************************** */
