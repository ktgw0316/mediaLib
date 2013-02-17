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

#pragma ident	"@(#)mlib_ImageReformat_f2.c	9.2	07/10/09 SMI"

/*
 * FUNCTIONS
 *      mlib_ImageReformat_U8_S16
 *      mlib_ImageReformat_U8_S32
 *      mlib_ImageReformat_U8_F32
 *      mlib_ImageReformat_U8_D64
 *
 *      mlib_ImageReformat_S16_S32
 *      mlib_ImageReformat_S16_F32
 *      mlib_ImageReformat_S16_D64
 *
 *      mlib_ImageReformat_U16_S32
 *      mlib_ImageReformat_U16_F32
 *      mlib_ImageReformat_U16_D64
 *
 *      mlib_ImageReformat_S32_F32
 *      mlib_ImageReformat_S32_D64
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

#define	MLIB_INT_CLAMP_U8(dst, src)                             \
	{                                                       \
	    mlib_s32 v = src, mask = (v - 0xff) >> 31;          \
	                                                        \
	    dst = (v | ~mask) & ~(v >> 31);                     \
	}

/* *********************************************************** */

#define	MLIB_INT_CLAMP_S16(dst, src)                             \
	{                                                        \
	    mlib_s32 s = src;                                    \
	                                                         \
	    dst =                                                \
		((s | ((32767 - s) >> 31)) & 0x7FFF) & ~(((s +   \
		32767) & s) >> 31) | ((s >> 31) & 0x8000);       \
	}

/* *********************************************************** */

#define	MLIB_INT_CLAMP_U16(dst, src)                            \
	{                                                       \
	    mlib_s32 s = src;                                   \
	                                                        \
	    dst = (s | ((65535 - s) >> 31)) & ~(s >> 31);       \
	}

/* *********************************************************** */

#ifdef MLIB_USE_FTOI_CLAMPING

#define	MLIB_CLAMP_U8(DST, SRC)	DST = (mlib_u8) SRC;
#define	MLIB_CLAMP_S16(DST, SRC)	DST = (mlib_s16) SRC;

#define	MLIB_CLAMP_U16(DST, SRC)	DST = (mlib_u16) SRC;

#define	MLIB_CLAMP_S32(DST, SRC)	DST = (mlib_s32) SRC

#else /* MLIB_USE_FTOI_CLAMPING */

#define	MLIB_CLAMP_U8(DST, SRC)                                 \
	if (SRC >= MLIB_U8_MAX)                                 \
	    DST = MLIB_U8_MAX;                                  \
	else                                                    \
	    if (SRC <= MLIB_U8_MIN)                             \
		DST = MLIB_U8_MIN;                              \
	    else                                                \
		DST = (mlib_u8)SRC

#define	MLIB_CLAMP_S16(DST, SRC)                                \
	if (SRC >= MLIB_S16_MAX)                                \
	    DST = MLIB_S16_MAX;                                 \
	else                                                    \
	    if (SRC <= MLIB_S16_MIN)                            \
		DST = MLIB_S16_MIN;                             \
	    else                                                \
		DST = (mlib_s16)SRC

#define	MLIB_CLAMP_U16(DST, SRC)                                \
	if (SRC >= MLIB_U16_MAX)                                \
	    DST = MLIB_U16_MAX;                                 \
	else                                                    \
	    if (SRC <= MLIB_U16_MIN)                            \
		DST = MLIB_U16_MIN;                             \
	    else                                                \
		DST = (mlib_u16)SRC

#define	MLIB_CLAMP_S32(DST, SRC)                                \
	if (SRC >= MLIB_S32_MAX)                                \
	    DST = MLIB_S32_MAX;                                 \
	else                                                    \
	    if (SRC <= MLIB_S32_MIN)                            \
		DST = MLIB_S32_MIN;                             \
	    else                                                \
	    DST = (mlib_s32)SRC

#endif /* MLIB_USE_FTOI_CLAMPING */

/* *********************************************************** */

/* Reformat an MLIB_SHORT image into an MLIB_BYTE image. */

void
mlib_ImageReformat_U8_S16(
    REFORMAT_PARAMS1)
{
/* pointer for pixels in source */
	mlib_s16 *sp;

/* pointer for pixels in destination */
	mlib_u8 *dp;

/* pointer for lines in source */
	mlib_s16 *sl;

/* pointer for lines in destination */
	mlib_u8 *dl;
	mlib_s32 i, j, k;

	for (k = 0; k < numBands; k++) {
		sl = sp = (mlib_s16 *)srcData[k] + srcBandoffsets[k];
		dl = dp = (mlib_u8 *)dstData[k] + dstBandoffsets[k];
		for (j = 0; j < ySize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < xSize; i++) {
				MLIB_INT_CLAMP_U8(dp[i * dstPixelstride],
				    sp[i * srcPixelstride]);
			}

			sp = sl += srcScanlinestride;
			dp = dl += dstScanlinestride;
		}
	}
}

/* *********************************************************** */

/* Reformat an MLIB_INT image into an MLIB_BYTE image. */

void
mlib_ImageReformat_U8_S32(
    REFORMAT_PARAMS1)
{
/* pointer for pixels in source */
	mlib_s32 *sp;

/* pointer for pixels in destination */
	mlib_u8 *dp;

/* pointer for lines in source */
	mlib_s32 *sl;

/* pointer for lines in destination */
	mlib_u8 *dl;
	mlib_s32 i, j, k;

	for (k = 0; k < numBands; k++) {
		sl = sp = (mlib_s32 *)srcData[k] + srcBandoffsets[k];
		dl = dp = (mlib_u8 *)dstData[k] + dstBandoffsets[k];
		for (j = 0; j < ySize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < xSize; i++) {
				MLIB_INT_CLAMP_U8(dp[i * dstPixelstride],
				    sp[i * srcPixelstride]);
			}

			sp = sl += srcScanlinestride;
			dp = dl += dstScanlinestride;
		}
	}
}

/* *********************************************************** */

/* Reformat an MLIB_FLOAT image into an MLIB_BYTE image. */

void
mlib_ImageReformat_U8_F32(
    REFORMAT_PARAMS1)
{
/* pointer for pixels in source */
	mlib_f32 *sp;

/* pointer for pixels in destination */
	mlib_u8 *dp;

/* pointer for lines in source */
	mlib_f32 *sl;

/* pointer for lines in destination */
	mlib_u8 *dl;
	mlib_s32 i, j, k;

	for (k = 0; k < numBands; k++) {
		sl = sp = (mlib_f32 *)srcData[k] + srcBandoffsets[k];
		dl = dp = (mlib_u8 *)dstData[k] + dstBandoffsets[k];
		for (j = 0; j < ySize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < xSize; i++) {
#ifdef MLIB_USE_FTOI_CLAMPING
				mlib_s32 buf;

				buf = (mlib_s32)((*sp - 128.0f) * 16777216.0f);
				*dp = (mlib_u8)((buf >> 24) + 128);
#else /* MLIB_USE_FTOI_CLAMPING */
				MLIB_CLAMP_U8(*dp, *sp);
#endif /* MLIB_USE_FTOI_CLAMPING */
				sp += srcPixelstride;
				dp += dstPixelstride;
			}

			sp = sl += srcScanlinestride;
			dp = dl += dstScanlinestride;
		}
	}
}

/* *********************************************************** */

/* Reformat an MLIB_DOUBLE image into an MLIB_BYTE image. */

void
mlib_ImageReformat_U8_D64(
    REFORMAT_PARAMS1)
{
/* pointer for pixels in source */
	mlib_d64 *sp;

/* pointer for pixels in destination */
	mlib_u8 *dp;

/* pointer for lines in source */
	mlib_d64 *sl;

/* pointer for lines in destination */
	mlib_u8 *dl;
	mlib_s32 i, j, k;

	for (k = 0; k < numBands; k++) {
		sl = sp = (mlib_d64 *)srcData[k] + srcBandoffsets[k];
		dl = dp = (mlib_u8 *)dstData[k] + dstBandoffsets[k];
		for (j = 0; j < ySize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < xSize; i++) {
#ifdef MLIB_USE_FTOI_CLAMPING
				mlib_s32 buf;

				buf = (mlib_s32)((*sp - 128.0) * 16777216.0);
				*dp = (mlib_u8)((buf >> 24) + 128);
#else /* MLIB_USE_FTOI_CLAMPING */
				MLIB_CLAMP_U8(*dp, *sp);
#endif /* MLIB_USE_FTOI_CLAMPING */
				sp += srcPixelstride;
				dp += dstPixelstride;
			}

			sp = sl += srcScanlinestride;
			dp = dl += dstScanlinestride;
		}
	}
}

/* *********************************************************** */

/* Reformat an MLIB_INT image into an MLIB_SHORT image. */

void
mlib_ImageReformat_S16_S32(
    REFORMAT_PARAMS1)
{
/* pointer for pixels in source */
	mlib_s32 *sp;

/* pointer for pixels in destination */
	mlib_s16 *dp;

/* pointer for lines in source */
	mlib_s32 *sl;

/* pointer for lines in destination */
	mlib_s16 *dl;
	mlib_s32 i, j, k;

	for (k = 0; k < numBands; k++) {
		sl = sp = (mlib_s32 *)srcData[k] + srcBandoffsets[k];
		dl = dp = (mlib_s16 *)dstData[k] + dstBandoffsets[k];
		for (j = 0; j < ySize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < xSize; i++) {
				MLIB_INT_CLAMP_S16(dp[i * dstPixelstride],
				    sp[i * srcPixelstride]);
			}

			sp = sl += srcScanlinestride;
			dp = dl += dstScanlinestride;
		}
	}
}

/* *********************************************************** */

/* Reformat an MLIB_FLOAT image into an MLIB_SHORT image. */

void
mlib_ImageReformat_S16_F32(
    REFORMAT_PARAMS1)
{
/* pointer for pixels in source */
	mlib_f32 *sp;

/* pointer for pixels in destination */
	mlib_s16 *dp;

/* pointer for lines in source */
	mlib_f32 *sl;

/* pointer for lines in destination */
	mlib_s16 *dl;
	mlib_s32 i, j, k;

	for (k = 0; k < numBands; k++) {
		sl = sp = (mlib_f32 *)srcData[k] + srcBandoffsets[k];
		dl = dp = (mlib_s16 *)dstData[k] + dstBandoffsets[k];
		for (j = 0; j < ySize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < xSize; i++) {
#ifdef MLIB_USE_FTOI_CLAMPING
				mlib_s32 buf;

				buf = (mlib_s32)((*sp) * 65536.0f);
				*dp = (mlib_s16)(buf >> 16);
#else /* MLIB_USE_FTOI_CLAMPING */
				MLIB_CLAMP_S16(*dp, *sp);
#endif /* MLIB_USE_FTOI_CLAMPING */
				sp += srcPixelstride;
				dp += dstPixelstride;
			}

			sp = sl += srcScanlinestride;
			dp = dl += dstScanlinestride;
		}
	}
}

/* *********************************************************** */

/* Reformat an MLIB_DOUBLE image into an MLIB_SHORT image. */

void
mlib_ImageReformat_S16_D64(
    REFORMAT_PARAMS1)
{
/* pointer for pixels in source */
	mlib_d64 *sp;

/* pointer for pixels in destination */
	mlib_s16 *dp;

/* pointer for lines in source */
	mlib_d64 *sl;

/* pointer for lines in destination */
	mlib_s16 *dl;
	mlib_s32 i, j, k;

	for (k = 0; k < numBands; k++) {
		sl = sp = (mlib_d64 *)srcData[k] + srcBandoffsets[k];
		dl = dp = (mlib_s16 *)dstData[k] + dstBandoffsets[k];
		for (j = 0; j < ySize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < xSize; i++) {
#ifdef MLIB_USE_FTOI_CLAMPING
				mlib_s32 buf;

				buf = (mlib_s32)((*sp) * 65536.0);
				*dp = (mlib_s16)(buf >> 16);
#else /* MLIB_USE_FTOI_CLAMPING */
				MLIB_CLAMP_S16(*dp, *sp);
#endif /* MLIB_USE_FTOI_CLAMPING */
				sp += srcPixelstride;
				dp += dstPixelstride;
			}

			sp = sl += srcScanlinestride;
			dp = dl += dstScanlinestride;
		}
	}
}

/* *********************************************************** */

/* Reformat an MLIB_INT image into an MLIB_USHORT image. */

void
mlib_ImageReformat_U16_S32(
    REFORMAT_PARAMS1)
{
/* pointer for pixels in source */
	mlib_s32 *sp;

/* pointer for pixels in destination */
	mlib_s16 *dp;

/* pointer for lines in source */
	mlib_s32 *sl;

/* pointer for lines in destination */
	mlib_s16 *dl;
	mlib_s32 i, j, k;

	for (k = 0; k < numBands; k++) {
		sl = sp = (mlib_s32 *)srcData[k] + srcBandoffsets[k];
		dl = dp = (mlib_s16 *)dstData[k] + dstBandoffsets[k];
		for (j = 0; j < ySize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < xSize; i++) {
				MLIB_INT_CLAMP_U16(dp[i * dstPixelstride],
				    sp[i * srcPixelstride]);
			}

			sp = sl += srcScanlinestride;
			dp = dl += dstScanlinestride;
		}
	}
}

/* *********************************************************** */

/* Reformat an MLIB_FLOAT image into an MLIB_USHORT image. */

void
mlib_ImageReformat_U16_F32(
    REFORMAT_PARAMS1)
{
/* pointer for pixels in source */
	mlib_f32 *sp;

/* pointer for pixels in destination */
	mlib_u16 *dp;

/* pointer for lines in source */
	mlib_f32 *sl;

/* pointer for lines in destination */
	mlib_u16 *dl;
	mlib_s32 i, j, k;

	for (k = 0; k < numBands; k++) {
		sl = sp = (mlib_f32 *)srcData[k] + srcBandoffsets[k];
		dl = dp = (mlib_u16 *)dstData[k] + dstBandoffsets[k];
		for (j = 0; j < ySize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < xSize; i++) {
#ifdef MLIB_USE_FTOI_CLAMPING
				mlib_s32 buf;

				buf = (mlib_s32)(((*sp) - 32768.0f) * 65536.0f);
				*dp = (mlib_u16)((buf >> 16) + 32768);
#else /* MLIB_USE_FTOI_CLAMPING */
				MLIB_CLAMP_U16(*dp, *sp);
#endif /* MLIB_USE_FTOI_CLAMPING */
				sp += srcPixelstride;
				dp += dstPixelstride;
			}

			sp = sl += srcScanlinestride;
			dp = dl += dstScanlinestride;
		}
	}
}

/* *********************************************************** */

/* Reformat an MLIB_DOUBLE image into an MLIB_USHORT image. */

void
mlib_ImageReformat_U16_D64(
    REFORMAT_PARAMS1)
{
/* pointer for pixels in source */
	mlib_d64 *sp;

/* pointer for pixels in destination */
	mlib_u16 *dp;

/* pointer for lines in source */
	mlib_d64 *sl;

/* pointer for lines in destination */
	mlib_u16 *dl;
	mlib_s32 i, j, k;

	for (k = 0; k < numBands; k++) {
		sl = sp = (mlib_d64 *)srcData[k] + srcBandoffsets[k];
		dl = dp = (mlib_u16 *)dstData[k] + dstBandoffsets[k];
		for (j = 0; j < ySize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < xSize; i++) {
#ifdef MLIB_USE_FTOI_CLAMPING
				mlib_s32 buf;

				buf = (mlib_s32)(((*sp) - 32768.0) * 65536.0);
				*dp = (mlib_u16)((buf >> 16) + 32768);
#else /* MLIB_USE_FTOI_CLAMPING */
				MLIB_CLAMP_U16(*dp, *sp);
#endif /* MLIB_USE_FTOI_CLAMPING */
				sp += srcPixelstride;
				dp += dstPixelstride;
			}

			sp = sl += srcScanlinestride;
			dp = dl += dstScanlinestride;
		}
	}
}

/* *********************************************************** */

/* Reformat an MLIB_FLOAT image into an MLIB_INT image. */

void
mlib_ImageReformat_S32_F32(
    REFORMAT_PARAMS1)
{
/* pointer for pixels in source */
	mlib_f32 *sp;

/* pointer for pixels in destination */
	mlib_s32 *dp;

/* pointer for lines in source */
	mlib_f32 *sl;

/* pointer for lines in destination */
	mlib_s32 *dl;
	mlib_s32 i, j, k;

	for (k = 0; k < numBands; k++) {
		sl = sp = (mlib_f32 *)srcData[k] + srcBandoffsets[k];
		dl = dp = (mlib_s32 *)dstData[k] + dstBandoffsets[k];
		for (j = 0; j < ySize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < xSize; i++) {
				MLIB_CLAMP_S32(dp[i * dstPixelstride],
				    sp[i * srcPixelstride]);
			}

			sp = sl += srcScanlinestride;
			dp = dl += dstScanlinestride;
		}
	}
}

/* *********************************************************** */

/* Reformat an MLIB_DOUBLE image into an MLIB_INT image. */

void
mlib_ImageReformat_S32_D64(
    REFORMAT_PARAMS1)
{
/* pointer for pixels in source */
	mlib_d64 *sp;

/* pointer for pixels in destination */
	mlib_s32 *dp;

/* pointer for lines in source */
	mlib_d64 *sl;

/* pointer for lines in destination */
	mlib_s32 *dl;
	mlib_s32 i, j, k;

	for (k = 0; k < numBands; k++) {
		sl = sp = (mlib_d64 *)srcData[k] + srcBandoffsets[k];
		dl = dp = (mlib_s32 *)dstData[k] + dstBandoffsets[k];
		for (j = 0; j < ySize; j++) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (i = 0; i < xSize; i++) {
				MLIB_CLAMP_S32(dp[i * dstPixelstride],
				    sp[i * srcPixelstride]);
			}

			sp = sl += srcScanlinestride;
			dp = dl += dstScanlinestride;
		}
	}
}

/* *********************************************************** */
