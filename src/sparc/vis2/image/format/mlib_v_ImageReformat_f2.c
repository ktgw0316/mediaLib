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

#pragma ident	"@(#)mlib_v_ImageReformat_f2.c	9.3	07/11/05 SMI"

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
#include <vis_proto.h>
#include <mlib_ImageReformat.h>

/* *********************************************************** */

typedef union
{
	mlib_d64 value;
	struct
	{
		mlib_s32 int0, int1;
	} twoint;
	struct
	{
		mlib_f32 float0, float1;
	} twofloat;
} type_union_mlib_d64;

/* *********************************************************** */

#define	REFORMAT_U8()                                           \
	sd0.twoint.int0 = (mlib_s32)(sp[0]);                    \
	sp += srcPixelstride;                                   \
	sd1.twoint.int0 = (mlib_s32)(sp[0]);                    \
	sp += srcPixelstride;                                   \
	sd2.twoint.int0 = (mlib_s32)(sp[0]);                    \
	sp += srcPixelstride;                                   \
	sd3.twoint.int0 = (mlib_s32)(sp[0]);                    \
	sp += srcPixelstride;                                   \
	sd0.twoint.int1 = (mlib_s32)(sp[0]);                    \
	sp += srcPixelstride;                                   \
	sd1.twoint.int1 = (mlib_s32)(sp[0]);                    \
	sp += srcPixelstride;                                   \
	sd2.twoint.int1 = (mlib_s32)(sp[0]);                    \
	sp += srcPixelstride;                                   \
	sd3.twoint.int1 = (mlib_s32)(sp[0]);                    \
	sp += srcPixelstride;                                   \
	dd = vis_fpack32(sd0.value, sd0.value);                 \
	dd = vis_fpack32(dd, sd1.value);                        \
	dd = vis_fpack32(dd, sd2.value);                        \
	dd = vis_fpack32(dd, sd3.value);                        \
	dd = vis_faligndata(dd, dd);                            \
	vis_st_u8(dd, dp);                                      \
	dd = vis_faligndata(dd, dd);                            \
	dp += dstPixelstride;                                   \
	vis_st_u8(dd, dp);                                      \
	dd = vis_faligndata(dd, dd);                            \
	dp += dstPixelstride;                                   \
	vis_st_u8(dd, dp);                                      \
	dd = vis_faligndata(dd, dd);                            \
	dp += dstPixelstride;                                   \
	vis_st_u8(dd, dp);                                      \
	dd = vis_faligndata(dd, dd);                            \
	dp += dstPixelstride;                                   \
	vis_st_u8(dd, dp);                                      \
	dd = vis_faligndata(dd, dd);                            \
	dp += dstPixelstride;                                   \
	vis_st_u8(dd, dp);                                      \
	dd = vis_faligndata(dd, dd);                            \
	dp += dstPixelstride;                                   \
	vis_st_u8(dd, dp);                                      \
	dd = vis_faligndata(dd, dd);                            \
	dp += dstPixelstride;                                   \
	vis_st_u8(dd, dp);                                      \
	dp += dstPixelstride

/* *********************************************************** */

#define	REFORMAT_U8_S32()                                       \
	sd0.twofloat.float0 = sp[0];                            \
	sp += srcPixelstride;                                   \
	sd1.twofloat.float0 = sp[0];                            \
	sp += srcPixelstride;                                   \
	sd2.twofloat.float0 = sp[0];                            \
	sp += srcPixelstride;                                   \
	sd3.twofloat.float0 = sp[0];                            \
	sp += srcPixelstride;                                   \
	sd0.twofloat.float1 = sp[0];                            \
	sp += srcPixelstride;                                   \
	sd1.twofloat.float1 = sp[0];                            \
	sp += srcPixelstride;                                   \
	sd2.twofloat.float1 = sp[0];                            \
	sp += srcPixelstride;                                   \
	sd3.twofloat.float1 = sp[0];                            \
	sp += srcPixelstride;                                   \
	dd = vis_fpack32(sd0.value, sd0.value);                 \
	dd = vis_fpack32(dd, sd1.value);                        \
	dd = vis_fpack32(dd, sd2.value);                        \
	dd = vis_fpack32(dd, sd3.value);                        \
	dd = vis_faligndata(dd, dd);                            \
	vis_st_u8(dd, dp);                                      \
	dd = vis_faligndata(dd, dd);                            \
	dp += dstPixelstride;                                   \
	vis_st_u8(dd, dp);                                      \
	dd = vis_faligndata(dd, dd);                            \
	dp += dstPixelstride;                                   \
	vis_st_u8(dd, dp);                                      \
	dd = vis_faligndata(dd, dd);                            \
	dp += dstPixelstride;                                   \
	vis_st_u8(dd, dp);                                      \
	dd = vis_faligndata(dd, dd);                            \
	dp += dstPixelstride;                                   \
	vis_st_u8(dd, dp);                                      \
	dd = vis_faligndata(dd, dd);                            \
	dp += dstPixelstride;                                   \
	vis_st_u8(dd, dp);                                      \
	dd = vis_faligndata(dd, dd);                            \
	dp += dstPixelstride;                                   \
	vis_st_u8(dd, dp);                                      \
	dd = vis_faligndata(dd, dd);                            \
	dp += dstPixelstride;                                   \
	vis_st_u8(dd, dp);                                      \
	dp += dstPixelstride

/* *********************************************************** */

#define	REFORMAT_U8_S16()                                       \
	d0 = vis_ld_u16(sp);                                    \
	sp += srcPixelstride;                                   \
	d1 = vis_ld_u16(sp);                                    \
	sp += srcPixelstride;                                   \
	d2 = vis_fpmerge(vis_read_lo(d0), vis_read_lo(d1));     \
	d3 = vis_ld_u16(sp);                                    \
	sp += srcPixelstride;                                   \
	d4 = vis_ld_u16(sp);                                    \
	sp += srcPixelstride;                                   \
	d5 = vis_fpmerge(vis_read_lo(d3), vis_read_lo(d4));     \
	dd0 = vis_bshuffle(d2, d5);                             \
	d0 = vis_ld_u16(sp);                                    \
	sp += srcPixelstride;                                   \
	d1 = vis_ld_u16(sp);                                    \
	sp += srcPixelstride;                                   \
	d2 = vis_fpmerge(vis_read_lo(d0), vis_read_lo(d1));     \
	d3 = vis_ld_u16(sp);                                    \
	sp += srcPixelstride;                                   \
	d4 = vis_ld_u16(sp);                                    \
	sp += srcPixelstride;                                   \
	d5 = vis_fpmerge(vis_read_lo(d3), vis_read_lo(d4));     \
	dd1 = vis_bshuffle(d2, d5);                             \
	dd = vis_fpack16_pair(dd0, dd1);                        \
	dd = vis_faligndata(dd, dd);                            \
	vis_st_u8(dd, dp);                                      \
	dd = vis_faligndata(dd, dd);                            \
	dp += dstPixelstride;                                   \
	vis_st_u8(dd, dp);                                      \
	dd = vis_faligndata(dd, dd);                            \
	dp += dstPixelstride;                                   \
	vis_st_u8(dd, dp);                                      \
	dd = vis_faligndata(dd, dd);                            \
	dp += dstPixelstride;                                   \
	vis_st_u8(dd, dp);                                      \
	dd = vis_faligndata(dd, dd);                            \
	dp += dstPixelstride;                                   \
	vis_st_u8(dd, dp);                                      \
	dd = vis_faligndata(dd, dd);                            \
	dp += dstPixelstride;                                   \
	vis_st_u8(dd, dp);                                      \
	dd = vis_faligndata(dd, dd);                            \
	dp += dstPixelstride;                                   \
	vis_st_u8(dd, dp);                                      \
	dd = vis_faligndata(dd, dd);                            \
	dp += dstPixelstride;                                   \
	vis_st_u8(dd, dp);                                      \
	dp += dstPixelstride

/* *********************************************************** */

#define	REFORMAT_S16()                                          \
	sd0.twoint.int0 = (mlib_s32)(sp[0]);                    \
	sp += srcPixelstride;                                   \
	sd0.twoint.int1 = (mlib_s32)(sp[0]);                    \
	sp += srcPixelstride;                                   \
	sd1.twoint.int0 = (mlib_s32)(sp[0]);                    \
	sp += srcPixelstride;                                   \
	sd1.twoint.int1 = (mlib_s32)(sp[0]);                    \
	sp += srcPixelstride;                                   \
	dd = vis_fpackfix_pair(sd0.value, sd1.value);           \
	dd = vis_faligndata(dd, dd);                            \
	vis_st_u16(dd, dp);                                     \
	dd = vis_faligndata(dd, dd);                            \
	dp += dstPixelstride;                                   \
	vis_st_u16(dd, dp);                                     \
	dd = vis_faligndata(dd, dd);                            \
	dp += dstPixelstride;                                   \
	vis_st_u16(dd, dp);                                     \
	dd = vis_faligndata(dd, dd);                            \
	dp += dstPixelstride;                                   \
	vis_st_u16(dd, dp);                                     \
	dp += dstPixelstride

/* *********************************************************** */

#define	REFORMAT_S16_S32()                                      \
	sd0.twofloat.float0 = sp[0];                            \
	sp += srcPixelstride;                                   \
	sd0.twofloat.float1 = sp[0];                            \
	sp += srcPixelstride;                                   \
	sd1.twofloat.float0 = sp[0];                            \
	sp += srcPixelstride;                                   \
	sd1.twofloat.float1 = sp[0];                            \
	sp += srcPixelstride;                                   \
	dd = vis_fpackfix_pair(sd0.value, sd1.value);           \
	dd = vis_faligndata(dd, dd);                            \
	vis_st_u16(dd, dp);                                     \
	dd = vis_faligndata(dd, dd);                            \
	dp += dstPixelstride;                                   \
	vis_st_u16(dd, dp);                                     \
	dd = vis_faligndata(dd, dd);                            \
	dp += dstPixelstride;                                   \
	vis_st_u16(dd, dp);                                     \
	dd = vis_faligndata(dd, dd);                            \
	dp += dstPixelstride;                                   \
	vis_st_u16(dd, dp);                                     \
	dp += dstPixelstride

/* *********************************************************** */

#define	REFORMAT_U16()                                          \
	sd0.twoint.int0 = (mlib_s32)(sp[0] - soff);             \
	sp += srcPixelstride;                                   \
	sd0.twoint.int1 = (mlib_s32)(sp[0] - soff);             \
	sp += srcPixelstride;                                   \
	sd1.twoint.int0 = (mlib_s32)(sp[0] - soff);             \
	sp += srcPixelstride;                                   \
	sd1.twoint.int1 = (mlib_s32)(sp[0] - soff);             \
	sp += srcPixelstride;                                   \
	dd = vis_fpackfix_pair(sd0.value, sd1.value);           \
	dd = vis_fxor(dd, mask8000);                            \
	dd = vis_faligndata(dd, dd);                            \
	vis_st_u16(dd, dp);                                     \
	dd = vis_faligndata(dd, dd);                            \
	dp += dstPixelstride;                                   \
	vis_st_u16(dd, dp);                                     \
	dd = vis_faligndata(dd, dd);                            \
	dp += dstPixelstride;                                   \
	vis_st_u16(dd, dp);                                     \
	dd = vis_faligndata(dd, dd);                            \
	dp += dstPixelstride;                                   \
	vis_st_u16(dd, dp);                                     \
	dp += dstPixelstride

/* *********************************************************** */

#define	REFORMAT_U16_S32()                                      \
	sd0.twofloat.float0 = sp[0];                            \
	sp += srcPixelstride;                                   \
	sd0.twofloat.float1 = sp[0];                            \
	sp += srcPixelstride;                                   \
	sd1.twofloat.float0 = sp[0];                            \
	sp += srcPixelstride;                                   \
	sd1.twofloat.float1 = sp[0];                            \
	sp += srcPixelstride;                                   \
	dd0 = vis_fand(sd0.value, mask);                        \
	dd1 = vis_fand(sd1.value, mask);                        \
	dd2 = vis_fpsub32(sd0.value, shift);                    \
	dd3 = vis_fpsub32(sd1.value, shift);                    \
	dd0 = vis_for(dd2, dd0);                                \
	dd1 = vis_for(dd3, dd1);                                \
	dd = vis_fpackfix_pair(dd0, dd1);                       \
	dd = vis_fxor(dd, mask8000);                            \
	dd = vis_faligndata(dd, dd);                            \
	vis_st_u16(dd, dp);                                     \
	dd = vis_faligndata(dd, dd);                            \
	dp += dstPixelstride;                                   \
	vis_st_u16(dd, dp);                                     \
	dd = vis_faligndata(dd, dd);                            \
	dp += dstPixelstride;                                   \
	vis_st_u16(dd, dp);                                     \
	dd = vis_faligndata(dd, dd);                            \
	dp += dstPixelstride;                                   \
	vis_st_u16(dd, dp);                                     \
	dp += dstPixelstride

/* *********************************************************** */

#define	REFORMAT_S32()                                          \
	buf = *sp;                                              \
	if (buf >= MLIB_S32_MAX)                                \
	    buf = MLIB_S32_MAX;                                 \
	if (buf <= MLIB_S32_MIN)                                \
	    buf = MLIB_S32_MIN;                                 \
	*dp = (mlib_s32)buf;                                    \
	sp += srcPixelstride;                                   \
	dp += dstPixelstride

/* *********************************************************** */

/* Reformat an MLIB_INT image into an MLIB_BYTE image. */

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
	mlib_d64 d0, d1, d2, d3, d4, d5;
	mlib_d64 dd0, dd1;
	mlib_d64 dd;

	vis_write_gsr((7 << 3) | 1);
	vis_write_bmask(0x4657CEDF, 0);

	for (k = 0; k < numBands; k++) {
		sl = sp = (mlib_s16 *)srcData[k] + srcBandoffsets[k];
		dl = dp = (mlib_u8 *)dstData[k] + dstBandoffsets[k];
		for (j = 0; j < ySize; j++) {

#pragma pipeloop(0)
			for (i = 0; i <= (xSize - 8); i += 8) {
				REFORMAT_U8_S16();
			}

#pragma pipeloop(0)
			for (; i < xSize; i++) {
				mlib_s32 v = *sp, mask = (v - 0xff) >> 31;

				*dp = (v | ~mask) & ~(v >> 31);
				sp += srcPixelstride;
				dp += dstPixelstride;
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
	mlib_f32 *sp;

/* pointer for pixels in destination */
	mlib_u8 *dp;

/* pointer for lines in source */
	mlib_f32 *sl;

/* pointer for lines in destination */
	mlib_u8 *dl;
	mlib_s32 i, j, k;
	type_union_mlib_d64 sd0, sd1, sd2, sd3;
	mlib_d64 dd;

	vis_write_gsr((23 << 3) | 1);

	for (k = 0; k < numBands; k++) {
		sl = sp = (mlib_f32 *)srcData[k] + srcBandoffsets[k];
		dl = dp = (mlib_u8 *)dstData[k] + dstBandoffsets[k];
		for (j = 0; j < ySize; j++) {

#pragma pipeloop(0)
			for (i = 0; i <= (xSize - 8); i += 8) {
				REFORMAT_U8_S32();
			}

#pragma pipeloop(0)
			for (; i < xSize; i++) {
				mlib_s32 v = *(mlib_s32 *)sp, mask =
				    (v - 0xff) >> 31;
				*dp = (v | ~mask) & ~(v >> 31);
				sp += srcPixelstride;
				dp += dstPixelstride;
			}

			sp = sl += srcScanlinestride;
			dp = dl += dstScanlinestride;
		}
	}
}

/* *********************************************************** */

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
	type_union_mlib_d64 sd0, sd1, sd2, sd3;
	mlib_d64 dd;

	vis_write_gsr((23 << 3) | 1);

	for (k = 0; k < numBands; k++) {
		sl = sp = (mlib_f32 *)srcData[k] + srcBandoffsets[k];
		dl = dp = (mlib_u8 *)dstData[k] + dstBandoffsets[k];
		for (j = 0; j < ySize; j++) {

#pragma pipeloop(0)
			for (i = 0; i <= (xSize - 8); i += 8) {
				REFORMAT_U8();
			}

#pragma pipeloop(0)
			for (; i < xSize; i++) {
				mlib_f32 buf;

				buf = *sp;

				if (buf >= MLIB_U8_MAX)
					buf = MLIB_U8_MAX;

				if (buf <= MLIB_U8_MIN)
					buf = MLIB_U8_MIN;
				*dp = (mlib_u8)buf;
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
	type_union_mlib_d64 sd0, sd1, sd2, sd3;
	mlib_d64 dd;

	vis_write_gsr((23 << 3) | 1);

	for (k = 0; k < numBands; k++) {
		sl = sp = (mlib_d64 *)srcData[k] + srcBandoffsets[k];
		dl = dp = (mlib_u8 *)dstData[k] + dstBandoffsets[k];
		for (j = 0; j < ySize; j++) {

#pragma pipeloop(0)
			for (i = 0; i <= (xSize - 8); i += 8) {
				REFORMAT_U8();
			}

#pragma pipeloop(0)
			for (; i < xSize; i++) {
				mlib_d64 buf;

				buf = *sp;

				if (buf >= MLIB_U8_MAX)
					buf = MLIB_U8_MAX;

				if (buf <= MLIB_U8_MIN)
					buf = MLIB_U8_MIN;
				*dp = (mlib_u8)buf;
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
	mlib_f32 *sp;

/* pointer for pixels in destination */
	mlib_s16 *dp;

/* pointer for lines in source */
	mlib_f32 *sl;

/* pointer for lines in destination */
	mlib_s16 *dl;
	mlib_s32 i, j, k;
	type_union_mlib_d64 sd0, sd1;
	mlib_d64 dd;

	vis_write_gsr((16 << 3) | 2);

	for (k = 0; k < numBands; k++) {
		sl = sp = (mlib_f32 *)srcData[k] + srcBandoffsets[k];
		dl = dp = (mlib_s16 *)dstData[k] + dstBandoffsets[k];
		for (j = 0; j < ySize; j++) {

#pragma pipeloop(0)
			for (i = 0; i <= (xSize - 4); i += 4) {
				REFORMAT_S16_S32();
			}

#pragma pipeloop(0)
			for (; i < xSize; i++) {
				mlib_s32 s = *(mlib_s32 *)sp;

				*dp = ((s | ((32767 - s) >> 31)) & 0x7FFF)
				    & (~(((s +
				    32767) & s) >> 31)) | ((s >> 31) & 0x8000);

				sp += srcPixelstride;
				dp += dstPixelstride;
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
	type_union_mlib_d64 sd0, sd1;
	mlib_d64 dd;

	vis_write_gsr((16 << 3) | 2);

	for (k = 0; k < numBands; k++) {
		sl = sp = (mlib_f32 *)srcData[k] + srcBandoffsets[k];
		dl = dp = (mlib_s16 *)dstData[k] + dstBandoffsets[k];
		for (j = 0; j < ySize; j++) {

#pragma pipeloop(0)
			for (i = 0; i <= (xSize - 4); i += 4) {
				REFORMAT_S16();
			}

#pragma pipeloop(0)
			for (; i < xSize; i++) {
				mlib_f32 buf;

				buf = *sp;

				if (buf >= MLIB_S16_MAX)
					buf = MLIB_S16_MAX;

				if (buf <= MLIB_S16_MIN)
					buf = MLIB_S16_MIN;
				*dp = (mlib_s16)buf;
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
	type_union_mlib_d64 sd0, sd1;
	mlib_d64 dd;

	vis_write_gsr((16 << 3) | 2);

	for (k = 0; k < numBands; k++) {
		sl = sp = (mlib_d64 *)srcData[k] + srcBandoffsets[k];
		dl = dp = (mlib_s16 *)dstData[k] + dstBandoffsets[k];
		for (j = 0; j < ySize; j++) {

#pragma pipeloop(0)
			for (i = 0; i <= (xSize - 4); i += 4) {
				REFORMAT_S16();
			}

#pragma pipeloop(0)
			for (; i < xSize; i++) {
				mlib_d64 buf;

				buf = *sp;

				if (buf >= MLIB_S16_MAX)
					buf = MLIB_S16_MAX;

				if (buf <= MLIB_S16_MIN)
					buf = MLIB_S16_MIN;
				*dp = (mlib_s32)buf;
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
	mlib_f32 *sp;

/* pointer for pixels in destination */
	mlib_s16 *dp;

/* pointer for lines in source */
	mlib_f32 *sl;

/* pointer for lines in destination */
	mlib_s16 *dl;
	mlib_s32 i, j, k;
	mlib_d64 dd0, dd1, dd2, dd3;
	mlib_d64 mask8000 = vis_to_double_dup(0x80008000);
	mlib_d64 mask = vis_to_double_dup(0x80000000);
	mlib_d64 shift = vis_to_double_dup(0x8000);
	type_union_mlib_d64 sd0, sd1;
	mlib_d64 dd;

	vis_write_gsr((16 << 3) | 2);

	for (k = 0; k < numBands; k++) {
		sl = sp = (mlib_f32 *)srcData[k] + srcBandoffsets[k];
		dl = dp = (mlib_s16 *)dstData[k] + dstBandoffsets[k];
		for (j = 0; j < ySize; j++) {

#pragma pipeloop(0)
			for (i = 0; i <= (xSize - 4); i += 4) {
				REFORMAT_U16_S32();
			}

#pragma pipeloop(0)
			for (; i < xSize; i++) {
				mlib_s32 s = *(mlib_s32 *)sp;

				*dp = (s | ((65535 - s) >> 31)) & ~(s >> 31);

				sp += srcPixelstride;
				dp += dstPixelstride;
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
	mlib_s16 *dp;

/* pointer for lines in source */
	mlib_f32 *sl;

/* pointer for lines in destination */
	mlib_s16 *dl;
	mlib_s32 i, j, k;
	mlib_d64 soff = (mlib_d64)(1 << 15);
	mlib_d64 mask8000 = vis_to_double_dup(0x80008000);
	type_union_mlib_d64 sd0, sd1;
	mlib_d64 dd;

	vis_write_gsr((16 << 3) | 2);

	for (k = 0; k < numBands; k++) {
		sl = sp = (mlib_f32 *)srcData[k] + srcBandoffsets[k];
		dl = dp = (mlib_s16 *)dstData[k] + dstBandoffsets[k];
		for (j = 0; j < ySize; j++) {

#pragma pipeloop(0)
			for (i = 0; i <= (xSize - 4); i += 4) {
				REFORMAT_U16();
			}

#pragma pipeloop(0)
			for (; i < xSize; i++) {
				mlib_f32 s0 = *sp;

				if (s0 > (mlib_f32)MLIB_U16_MAX)
					s0 = (mlib_f32)MLIB_U16_MAX;

				if (s0 < (mlib_f32)MLIB_U16_MIN)
					s0 = (mlib_f32)MLIB_U16_MIN;
				*dp = (mlib_s32)s0;
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
	mlib_s16 *dp;

/* pointer for lines in source */
	mlib_d64 *sl;

/* pointer for lines in destination */
	mlib_s16 *dl;
	mlib_s32 i, j, k;
	mlib_d64 soff = (mlib_d64)(1 << 15);
	mlib_d64 mask8000 = vis_to_double_dup(0x80008000);
	type_union_mlib_d64 sd0, sd1;
	mlib_d64 dd;

	vis_write_gsr((16 << 3) | 2);

	for (k = 0; k < numBands; k++) {
		sl = sp = (mlib_d64 *)srcData[k] + srcBandoffsets[k];
		dl = dp = (mlib_s16 *)dstData[k] + dstBandoffsets[k];
		for (j = 0; j < ySize; j++) {

#pragma pipeloop(0)
			for (i = 0; i <= (xSize - 4); i += 4) {
				REFORMAT_U16();
			}

#pragma pipeloop(0)
			for (; i < xSize; i++) {
				mlib_d64 s0 = *sp;

				if (s0 > (mlib_d64)MLIB_U16_MAX)
					s0 = (mlib_d64)MLIB_U16_MAX;

				if (s0 < (mlib_d64)MLIB_U16_MIN)
					s0 = (mlib_d64)MLIB_U16_MIN;
				*dp = (mlib_s32)s0;
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

#pragma pipeloop(0)
			for (i = 0; i <= (xSize - 8); i += 8) {
				mlib_d64 buf;

				REFORMAT_S32();
				REFORMAT_S32();
				REFORMAT_S32();
				REFORMAT_S32();
				REFORMAT_S32();
				REFORMAT_S32();
				REFORMAT_S32();
				REFORMAT_S32();
			}

#pragma pipeloop(0)
			for (; i < xSize; i++) {
				mlib_d64 buf;

				REFORMAT_S32();
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

#pragma pipeloop(0)
			for (i = 0; i <= (xSize - 8); i += 8) {
				mlib_d64 buf;

				REFORMAT_S32();
				REFORMAT_S32();
				REFORMAT_S32();
				REFORMAT_S32();
				REFORMAT_S32();
				REFORMAT_S32();
				REFORMAT_S32();
				REFORMAT_S32();
			}

#pragma pipeloop(0)
			for (; i < xSize; i++) {
				mlib_d64 buf;

				REFORMAT_S32();
			}

			sp = sl += srcScanlinestride;
			dp = dl += dstScanlinestride;
		}
	}
}

/* *********************************************************** */
