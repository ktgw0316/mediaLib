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

#pragma ident	"@(#)mlib_v_ImageChannelExtract_f.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_v_ImageChannelExtract_U8_2_1
 *      mlib_v_ImageChannelExtract_U8_3_2
 *      mlib_v_ImageChannelExtract_U8_4_2
 *      mlib_v_ImageChannelExtract_32_2_1
 *      mlib_v_ImageChannelExtract_32_3_1
 *      mlib_v_ImageChannelExtract_32_3_2
 *      mlib_v_ImageChannelExtract_32_4_1
 *      mlib_v_ImageChannelExtract_32_4_2
 *      mlib_v_ImageChannelExtract_32_4_3
 *      mlib_v_ImageChannelExtract_U8
 *      mlib_v_ImageChannelExtract_S16
 *      mlib_v_ImageChannelExtract_D64
 *
 * DESCRIPTION
 *      Internal functions.
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_v_ImageChannelExtract.h>

/* *********************************************************** */

typedef union
{
	mlib_d64 d64;
	struct
	{
		mlib_f32 f0;
		mlib_f32 f1;
	} f32s;
} d64_2_f32;

/* *********************************************************** */

void
mlib_v_ImageChannelExtract_U8_2_1(
    const mlib_u8 *sl,
    mlib_s32 slb,
    mlib_u8 *dl,
    mlib_s32 dlb,
    mlib_s32 width,
    mlib_s32 height)
{
	mlib_u8 *sp = (void *)sl;
	mlib_u8 *dp = dl;
	mlib_s32 j;

	for (j = 0; j < height; j++) {
		mlib_u8 *dend = dl + width;
		mlib_u32 *sp2;

		while (((mlib_addr)sp & 7) > 1) {
			(*dp++) = *sp;
			sp += 2;

			if (dp >= dend)
				break;
		}

		if ((mlib_addr)sp & 7) {
			sp2 = (mlib_u32 *)(sp - 1);

#pragma pipeloop(0)
			for (; dp <= (dend - 2); dp += 2) {
				mlib_u32 s0;

				s0 = (*sp2++);
				dp[0] = s0 >> 16;
				dp[1] = s0;
			}

			if (dp < dend) {
				dp[0] = sp2[0] >> 16;
			}
		} else {
			sp2 = (mlib_u32 *)sp;

#pragma pipeloop(0)
			for (; dp <= (dend - 2); dp += 2) {
				mlib_u32 s0;

				s0 = (*sp2++);
				dp[0] = s0 >> 24;
				dp[1] = s0 >> 8;
			}

			if (dp < dend) {
				dp[0] = sp2[0] >> 24;
			}
		}

		sp = (void *)(sl += slb);
		dp = dl += dlb;
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelExtract_U8_3_2(
    const mlib_u8 *sl,
    mlib_s32 slb,
    mlib_u8 *dl,
    mlib_s32 dlb,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 count1)
{
	mlib_u8 *sp = (void *)sl;
	mlib_u8 *dp = dl;
	mlib_u32 *sp2;
	mlib_u16 *dp2;
	mlib_u16 *d2end;
	mlib_u32 s0, s1, s2, s3;
	mlib_s32 j, off;

	for (j = 0; j < height; j++) {
		mlib_u8 *dend = dl + 2 * width;

		if (count1 == 1) {
			if (dp < dend)
				(*dp++) = sp[0];
			sp += 2;
		}

		if ((mlib_addr)dp & 1) {

#pragma pipeloop(0)
			for (; dp <= (dend - 2); dp += 2) {
				dp[0] = sp[0];
				dp[1] = sp[1];
				sp += 3;
			}

			if (dp < dend) {
				dp[0] = sp[0];
			}

			sp = (void *)(sl += slb);
			dp = dl += dlb;
			continue;
		}

		dp2 = (mlib_u16 *)dp;
		d2end = (mlib_u16 *)((mlib_addr)dend & ~1);
		off = (mlib_addr)sp & 3;
		sp2 = (mlib_u32 *)(sp - off);

		switch (off) {

		case 0:

#pragma pipeloop(0)
			for (; dp2 <= (d2end - 4); dp2 += 4) {
				s0 = sp2[0];
				s1 = sp2[1];
				s2 = sp2[2];
				dp2[0] = s0 >> 16;
				dp2[1] = (s0 << 8) | (s1 >> 24);
				dp2[2] = s1;
				dp2[3] = s2 >> 8;
				sp2 += 3;
			}

			break;

		case 1:

#pragma pipeloop(0)
			for (; dp2 <= (d2end - 4); dp2 += 4) {
				s0 = sp2[0];
				s1 = sp2[1];
				s2 = sp2[2];
				dp2[0] = s0 >> 8;
				dp2[1] = s1 >> 16;
				dp2[2] = (s1 << 8) | (s2 >> 24);
				dp2[3] = s2;
				sp2 += 3;
			}

			break;

		case 2:
			s3 = sp2[0];

#pragma pipeloop(0)
			for (; dp2 <= (d2end - 4); dp2 += 4) {
				s0 = s3;
				s1 = sp2[1];
				s2 = sp2[2];
				s3 = sp2[3];
				dp2[0] = s0;
				dp2[1] = s1 >> 8;
				dp2[2] = s2 >> 16;
				dp2[3] = (s2 << 8) | (s3 >> 24);
				sp2 += 3;
			}

			break;

		case 3:
			s3 = sp2[0];

#pragma pipeloop(0)
			for (; dp2 <= (d2end - 4); dp2 += 4) {
				s0 = s3;
				s1 = sp2[1];
				s2 = sp2[2];
				s3 = sp2[3];
				dp2[0] = (s0 << 8) | (s1 >> 24);
				dp2[1] = s1;
				dp2[2] = s2 >> 8;
				dp2[3] = s3 >> 16;
				sp2 += 3;
			}
		}

		sp = (mlib_u8 *)sp2 + off;
		dp = (mlib_u8 *)dp2;
		while (dp < dend) {
			(*dp++) = sp[0];

			if (dp < dend)
				(*dp++) = sp[1];
			sp += 3;
		}

		sp = (void *)(sl += slb);
		dp = dl += dlb;
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelExtract_U8_4_2(
    const mlib_u8 *sl,
    mlib_s32 slb,
    mlib_u8 *dl,
    mlib_s32 dlb,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 count1)
{
	mlib_u8 *sp = (void *)sl;
	mlib_u8 *dp = dl;
	mlib_u32 *sp2;
	mlib_u16 *dp2;
	mlib_u16 *d2end;
	mlib_u32 s0;
	mlib_s32 j, off;

	for (j = 0; j < height; j++) {
		mlib_u8 *dend = dl + 2 * width;

		if (count1 == 1) {
			if (dp < dend)
				(*dp++) = sp[0];
			sp += 3;
		}

		off = (mlib_addr)sp & 3;

		if (((mlib_addr)dp & 1) || (off == 3)) {

#pragma pipeloop(0)
			for (; dp <= (dend - 2); dp += 2) {
				dp[0] = sp[0];
				dp[1] = sp[1];
				sp += 4;
			}

			if (dp < dend) {
				dp[0] = sp[0];
			}

			sp = (void *)(sl += slb);
			dp = dl += dlb;
			continue;
		}

		dp2 = (mlib_u16 *)dp;
		d2end = (mlib_u16 *)((mlib_addr)dend & ~1);
		sp2 = (mlib_u32 *)(sp - off);

		switch (off) {

		case 0:

#pragma pipeloop(0)
			for (; dp2 < d2end; dp2++) {
				s0 = sp2[0];
				dp2[0] = s0 >> 16;
				sp2++;
			}

			break;

		case 1:

#pragma pipeloop(0)
			for (; dp2 < d2end; dp2++) {
				s0 = sp2[0];
				dp2[0] = s0 >> 8;
				sp2++;
			}

			break;

		case 2:

#pragma pipeloop(0)
			for (; dp2 < d2end; dp2++) {
				s0 = sp2[0];
				dp2[0] = s0;
				sp2++;
			}

			break;
		}

		sp = (mlib_u8 *)sp2 + off;
		dp = (mlib_u8 *)dp2;

		if (dp < dend) {
			(*dp++) = sp[0];
		}

		sp = (void *)(sl += slb);
		dp = dl += dlb;
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelExtract_32_2_1(
    const mlib_f32 *sp,
    mlib_s32 slb,
    mlib_f32 *dp,
    mlib_s32 dlb,
    mlib_s32 width,
    mlib_s32 height)
{
	mlib_d64 *sp2;
	mlib_s32 i, j;

	for (j = 0; j < height; j++) {

		if (((mlib_addr)sp & 7) == 0) {
			sp2 = (mlib_d64 *)sp;

#pragma pipeloop(0)
			for (i = 0; i < width; i++) {
				d64_2_f32 d;

				d.d64 = sp2[i];
				dp[i] = d.f32s.f0;
			}
		} else {
			sp2 = (mlib_d64 *)(sp - 1);

#pragma pipeloop(0)
			for (i = 0; i < width; i++) {
				d64_2_f32 d;

				d.d64 = sp2[i];
				dp[i] = d.f32s.f1;
			}
		}

		sp += slb;
		dp += dlb;
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelExtract_32_3_1(
    const mlib_f32 *sl,
    mlib_s32 slb,
    mlib_f32 *dl,
    mlib_s32 dlb,
    mlib_s32 width,
    mlib_s32 height)
{
	mlib_f32 *sp = (void *)sl;
	mlib_f32 *dp = dl;
	mlib_d64 *sp2;
	d64_2_f32 d0;
	mlib_s32 j;

	for (j = 0; j < height; j++) {
		mlib_f32 *dend = dl + width;

		if ((mlib_addr)sp & 7) {
			dp[0] = sp[0];
			sp += 3;
			dp++;
		}

		sp2 = (mlib_d64 *)sp;

#pragma pipeloop(0)
		for (; dp <= (dend - 2); dp += 2) {
			d64_2_f32 d0, d1;

			d0.d64 = sp2[0];
			d1.d64 = sp2[1];
			dp[0] = d0.f32s.f0;
			dp[1] = d1.f32s.f1;
			sp2 += 3;
		}

		if (dp < dend) {
			d0.d64 = sp2[0];
			dp[0] = d0.f32s.f0;
		}

		sp = (void *)(sl += slb);
		dp = dl += dlb;
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelExtract_32_3_2(
    const mlib_f32 *sl,
    mlib_s32 slb,
    mlib_f32 *dl,
    mlib_s32 dlb,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 count1)
{
	mlib_f32 *sp = (void *)sl;
	mlib_f32 *dp = dl;
	mlib_d64 *sp2;
	mlib_s32 j;

	for (j = 0; j < height; j++) {
		mlib_f32 *dend = dl + 2 * width;

		if (count1 == 1) {
			if (dp < dend)
				(*dp++) = sp[0];
			sp += 2;
		}

		if ((mlib_addr)sp & 7) {
			if (dp < dend)
				(*dp++) = sp[0];

			if (dp < dend)
				(*dp++) = sp[1];
			sp += 3;
		}

		sp2 = (mlib_d64 *)sp;

#pragma pipeloop(0)
		for (; dp <= (dend - 4); dp += 4) {
			d64_2_f32 d0, d1, d2;

			d0.d64 = sp2[0];
			d1.d64 = sp2[1];
			d2.d64 = sp2[2];
			dp[0] = d0.f32s.f0;
			dp[1] = d0.f32s.f1;
			dp[2] = d1.f32s.f1;
			dp[3] = d2.f32s.f0;
			sp2 += 3;
		}

		if (dp < dend) {
			sp = (mlib_f32 *)sp2;
			(*dp++) = sp[0];

			if (dp < dend)
				(*dp++) = sp[1];

			if (dp < dend)
				(*dp++) = sp[3];
		}

		sp = (void *)(sl += slb);
		dp = dl += dlb;
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelExtract_32_4_1(
    const mlib_f32 *sp,
    mlib_s32 slb,
    mlib_f32 *dp,
    mlib_s32 dlb,
    mlib_s32 width,
    mlib_s32 height)
{
	mlib_d64 *sp2;
	mlib_s32 i, j;

	for (j = 0; j < height; j++) {

		if (((mlib_addr)sp & 7) == 0) {
			sp2 = (mlib_d64 *)sp;

#pragma pipeloop(0)
			for (i = 0; i < width; i++) {
				d64_2_f32 d;

				d.d64 = sp2[2 * i];
				dp[i] = d.f32s.f0;
			}
		} else {
			sp2 = (mlib_d64 *)(sp - 1);

#pragma pipeloop(0)
			for (i = 0; i < width; i++) {
				d64_2_f32 d;

				d.d64 = sp2[2 * i];
				dp[i] = d.f32s.f1;
			}
		}

		sp += slb;
		dp += dlb;
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelExtract_32_4_2(
    const mlib_f32 *sl,
    mlib_s32 slb,
    mlib_f32 *dl,
    mlib_s32 dlb,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 count1)
{
	mlib_f32 *sp = (void *)sl;
	mlib_f32 *dp = dl;
	mlib_d64 *sp2;
	mlib_s32 j;
	d64_2_f32 d0;

	for (j = 0; j < height; j++) {
		mlib_f32 *dend = dl + 2 * width;

		if (count1 == 1) {
			dp[0] = sp[0];
			sp += 3;
			dp++;
		}

		if (((mlib_addr)sp & 7) == 0) {
			sp2 = (mlib_d64 *)sp;

#pragma pipeloop(0)
			for (; dp <= (dend - 2); dp += 2) {
				d64_2_f32 d;

				d.d64 = sp2[0];
				dp[0] = d.f32s.f0;
				dp[1] = d.f32s.f1;
				sp2 += 2;
			}

			if (dp < dend) {
				d0.d64 = sp2[0];
				dp[0] = d0.f32s.f0;
			}
		} else {
			sp2 = (mlib_d64 *)(sp - 1);

#pragma pipeloop(0)
			for (; dp <= (dend - 2); dp += 2) {
				d64_2_f32 d0, d1;

				d0.d64 = sp2[0];
				d1.d64 = sp2[1];
				dp[0] = d0.f32s.f1;
				dp[1] = d1.f32s.f0;
				sp2 += 2;
			}

			if (dp < dend) {
				d0.d64 = sp2[0];
				dp[0] = d0.f32s.f1;
			}
		}

		sp = (void *)(sl += slb);
		dp = dl += dlb;
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelExtract_32_4_3(
    const mlib_f32 *sl,
    mlib_s32 slb,
    mlib_f32 *dl,
    mlib_s32 dlb,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 count1)
{
	mlib_f32 *sp = (void *)sl;
	mlib_f32 *dp = dl;
	mlib_d64 *sp2;
	mlib_s32 j, k;
	d64_2_f32 d0, d1;

	for (j = 0; j < height; j++) {
		mlib_f32 *dend = dl + 3 * width;

		for (k = 0; k < count1; k++) {
			if (dp < dend)
				(*dp++) = (*sp++);
		}

		sp++;

		if (((mlib_addr)sp & 7) == 0) {
			sp2 = (mlib_d64 *)sp;

#pragma pipeloop(0)
			for (; dp <= (dend - 3); dp += 3) {
				d64_2_f32 d0, d1;

				d0.d64 = sp2[0];
				d1.d64 = sp2[1];
				dp[0] = d0.f32s.f0;
				dp[1] = d0.f32s.f1;
				dp[2] = d1.f32s.f0;
				sp2 += 2;
			}

			if (dp < dend) {
				d0.d64 = sp2[0];
				(*dp++) = d0.f32s.f0;

				if (dp < dend)
					(*dp++) = d0.f32s.f1;
			}
		} else {
			sp2 = (mlib_d64 *)(sp - 1);

#pragma pipeloop(0)
			for (; dp <= (dend - 3); dp += 3) {
				d64_2_f32 d0, d1;

				d0.d64 = sp2[0];
				d1.d64 = sp2[1];
				dp[0] = d0.f32s.f1;
				dp[1] = d1.f32s.f0;
				dp[2] = d1.f32s.f1;
				sp2 += 2;
			}

			if (dp < dend) {
				d0.d64 = sp2[0];
				d1.d64 = vis_ld_d64_nf(sp2 + 1);
				(*dp++) = d0.f32s.f1;

				if (dp < dend)
					(*dp++) = d1.f32s.f0;
			}
		}

		sp = (void *)(sl += slb);
		dp = dl += dlb;
	}
}

/* *********************************************************** */

/* general channel extraction: slower due to the inner loop */

void
mlib_v_ImageChannelExtract_U8(
    const mlib_u8 *src,
    mlib_s32 slb,
    mlib_u8 *dst,
    mlib_s32 dlb,
    mlib_s32 channels,
    mlib_s32 channeld,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 cmask)
{
/* pointer for pixel in src */
	mlib_u8 *sp;

/* pointer for line in src  */
	mlib_u8 *sl;

/* pointer for pixel in dst */
	mlib_u8 *dp;

/* pointer for line in dst  */
	mlib_u8 *dl;

/* indices for x, y, channel */
	mlib_s32 i, j, k;
	mlib_s32 deltac[5] = { 0, 1, 1, 1, 1 };
	mlib_s32 inc0, inc1, inc2;
	mlib_u8 s0, s1, s2;

	deltac[channeld] = 1;
	for (i = (channels - 1), k = 0; i >= 0; i--) {
		if ((cmask & (1 << i)) == 0)
			deltac[k]++;
		else
			k++;
	}

	deltac[channeld] = channels;
	for (i = 1; i < channeld; i++) {
		deltac[channeld] -= deltac[i];
	}

	sp = sl = (void *)(src + deltac[0]);
	dp = dl = dst;

/* Only THREE CHANNEL CASE could be executed here!!! */

	inc0 = deltac[1];
	inc1 = deltac[2] + inc0;
	inc2 = deltac[3] + inc1;
	for (j = 0; j < height; j++) {

#pragma pipeloop(0)
		for (i = 0; i < width; i++) {
			s0 = sp[0];
			s1 = sp[inc0];
			s2 = sp[inc1];
			dp[0] = s0;
			dp[1] = s1;
			dp[2] = s2;
			sp += inc2;
			dp += 3;
		}

		sp = (void *)(sl += slb);
		dp = dl += dlb;
	}
}

/* *********************************************************** */

/* general channel extraction: slower due to the inner loop */

void
mlib_v_ImageChannelExtract_S16(
    const mlib_u16 *src,
    mlib_s32 slb,
    mlib_u16 *dst,
    mlib_s32 dlb,
    mlib_s32 channels,
    mlib_s32 channeld,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 cmask)
{
/* pointer for pixel in src */
	mlib_u16 *sp;

/* pointer for line in src  */
	mlib_u16 *sl;

/* pointer for pixel in dst */
	mlib_u16 *dp;

/* pointer for line in dst  */
	mlib_u16 *dl;

/* indices for x, y, channel */
	mlib_s32 i, j, k;
	mlib_s32 deltac[5] = { 0, 1, 1, 1, 1 };
	mlib_s32 inc0, inc1, inc2;
	mlib_u16 s0, s1, s2;

	slb >>= 1;
	dlb >>= 1;

	deltac[channeld] = 1;
	for (i = (channels - 1), k = 0; i >= 0; i--) {
		if ((cmask & (1 << i)) == 0)
			deltac[k]++;
		else
			k++;
	}

	deltac[channeld] = channels;
	for (i = 1; i < channeld; i++) {
		deltac[channeld] -= deltac[i];
	}

	sp = sl = (void *)(src + deltac[0]);
	dp = dl = dst;

	if (channeld == 2) {
		inc0 = deltac[1];
		inc1 = deltac[2] + inc0;
		for (j = 0; j < height; j++) {

#pragma pipeloop(0)
			for (i = 0; i < width; i++) {
				s0 = sp[0];
				s1 = sp[inc0];
				dp[0] = s0;
				dp[1] = s1;
				sp += inc1;
				dp += 2;
			}

			sp = sl = sl + slb;
			dp = dl = dl + dlb;
		}
	} else if (channeld == 3) {
		inc0 = deltac[1];
		inc1 = deltac[2] + inc0;
		inc2 = deltac[3] + inc1;
		for (j = 0; j < height; j++) {

#pragma pipeloop(0)
			for (i = 0; i < width; i++) {
				s0 = sp[0];
				s1 = sp[inc0];
				s2 = sp[inc1];
				dp[0] = s0;
				dp[1] = s1;
				dp[2] = s2;
				sp += inc2;
				dp += 3;
			}

			sp = sl = sl + slb;
			dp = dl = dl + dlb;
		}
	}
}

/* *********************************************************** */

/* general channel extraction: slower due to the inner loop */

void
mlib_v_ImageChannelExtract_D64(
    const mlib_d64 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 channels,
    mlib_s32 channeld,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 cmask)
{
/* pointer for pixel in src */
	mlib_d64 *sp;

/* pointer for line in src  */
	mlib_d64 *sl;

/* pointer for pixel in dst */
	mlib_d64 *dp;

/* pointer for line in dst  */
	mlib_d64 *dl;

/* indices for x, y, channel */
	mlib_s32 i, j, k;
	mlib_s32 deltac[5] = { 0, 1, 1, 1, 1 };
	mlib_s32 inc0, inc1, inc2;
	mlib_d64 s0, s1, s2;

	deltac[channeld] = 1;
	for (i = (channels - 1), k = 0; i >= 0; i--) {
		if ((cmask & (1 << i)) == 0)
			deltac[k]++;
		else
			k++;
	}

	deltac[channeld] = channels;
	for (i = 1; i < channeld; i++) {
		deltac[channeld] -= deltac[i];
	}

	sp = sl = (void *)(src + deltac[0]);
	dp = dl = dst;

	if (channeld == 1) {
		for (j = 0; j < height; j++) {

#pragma pipeloop(0)
			for (i = 0; i < width; i++) {
				s0 = sp[0];
				dp[i] = s0;
				sp += channels;
			}

			sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
			dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
		}
	} else if (channeld == 2) {
		inc0 = deltac[1];
		inc1 = deltac[2] + inc0;
		for (j = 0; j < height; j++) {

#pragma pipeloop(0)
			for (i = 0; i < width; i++) {
				s0 = sp[0];
				s1 = sp[inc0];
				dp[0] = s0;
				dp[1] = s1;
				sp += inc1;
				dp += 2;
			}

			sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
			dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
		}
	} else if (channeld == 3) {
		inc0 = deltac[1];
		inc1 = deltac[2] + inc0;
		inc2 = deltac[3] + inc1;
		for (j = 0; j < height; j++) {

#pragma pipeloop(0)
			for (i = 0; i < width; i++) {
				s0 = sp[0];
				s1 = sp[inc0];
				s2 = sp[inc1];
				dp[0] = s0;
				dp[1] = s1;
				dp[2] = s2;
				sp += inc2;
				dp += 3;
			}

			sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
			dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
		}
	}
}

/* *********************************************************** */
