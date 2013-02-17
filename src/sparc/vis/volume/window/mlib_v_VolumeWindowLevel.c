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

#pragma ident	"@(#)mlib_v_VolumeWindowLevel.c	9.2	07/11/05 SMI"

/*
 *  FUNCTION
 *    mlib_VolumeWindowLevel
 *
 *  SYNOPSIS
 *    mlib_status mlib_VolumeWindowLevel(mlib_u8        *dst,
 *                                       const mlib_s16 *src,
 *                                       mlib_s32       window,
 *                                       mlib_s32       level,
 *                                       mlib_s32       gmax,
 *                                       mlib_s32       gmin,
 *                                       mlib_s32       len)
 *
 *  ARGUMENTS
 *    dst       Pointer to output or destination array.
 *    src       Pointer to input or source array.
 *    window    Width of window.
 *    level     Center of window.
 *    gmax      Maximum grayscale in the destination array.
 *    gmin      Minimum grayscale in the destination array.
 *    len       Length of data array.
 *
 *  DESCRIPTION
 *
 */

#include <stdlib.h>
#include <mlib_volume.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VolumeWindowLevel = __mlib_VolumeWindowLevel

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_VolumeWindowLevel) mlib_VolumeWindowLevel
	__attribute__((weak, alias("__mlib_VolumeWindowLevel")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	INIT_VARS                                                          \
	mlib_s32 scale = 0;                                                \
	mlib_d64 a = (mlib_d64)(gmax - gmin) / window;                     \
	mlib_s32 i, ia;                                                    \
	mlib_s32 hmask, lmask, emask, off;                                 \
	mlib_s32 win = (2 * level - window + 1) >> 1;                      \
	mlib_s32 ileft = ((2 * level - window) >> 1) + 1;                  \
	mlib_s32 iright = ((2 * level + window) >> 1) - 1;                 \
	mlib_u8 *edst = dst + len - 1;                                     \
	mlib_d64 *dsp, *ddp, bf_0, bf_1, bf_2;                             \
	mlib_d64 dx_0, dx_1, d0_0, d0_1, d1_0, d1_1, d2_0, d2_1, rd;       \
	mlib_d64 dwin = vis_to_double_dup((win << 16) | (win & 0xffff));   \
	mlib_d64 GMIN =                                                    \
	vis_to_double_dup((gmin << 24) | (gmin << 16) | (gmin << 8) |      \
		gmin);                                                     \
	mlib_d64 GMAX =                                                    \
	vis_to_double_dup((gmax << 24) | (gmax << 16) | (gmax << 8) |      \
		gmax);                                                     \
	mlib_d64 dmin =                                                    \
	vis_to_double_dup((ileft << 16) | (ileft & 0xffff));               \
	mlib_d64 dmax =                                                    \
	vis_to_double_dup((iright << 16) | (iright & 0xffff));             \
	mlib_d64 dgmin

/* *********************************************************** */

#define	MLIB_CALC1                                                     \
	lmask = (vis_fcmplt16(dx_0, dmin) << 4) | vis_fcmplt16(dx_1,   \
		dmin);                                                 \
	hmask = (vis_fcmpgt16(dx_0, dmax) << 4) | vis_fcmpgt16(dx_1,   \
		dmax);                                                 \
	d0_0 = vis_fpsub16(dx_0, dwin);                                \
	d0_1 = vis_fpsub16(dx_1, dwin);                                \
	d1_0 = vis_fmul8x16(A, d0_0);                                  \
	d1_1 = vis_fmul8x16(A, d0_1);                                  \
	d2_0 = vis_fpadd16(d1_0, dgmin);                               \
	d2_1 = vis_fpadd16(d1_1, dgmin);                               \
	rd = vis_fpack16_pair(d2_0, d2_1)

/* *********************************************************** */

#if defined(MLIB_VIS) && MLIB_VIS >= 0x200

#define	WRITE_BMASK(mask)                                       \
	vis_write_bmask(mask, 0)

#define	UNMERGE_FMUL8X16                                        \
	d0_0 = vis_bshuffle(d0_0, d0_1);                        \
	d1_0 = vis_fmul8x16(vis_read_hi(d0_0), A);              \
	d1_1 = vis_fmul8x16(vis_read_lo(d0_0), A)

#else /* defined(MLIB_VIS) && MLIB_VIS >= 0x200 */

#define	WRITE_BMASK(mask)

#define	UNMERGE_FMUL8X16                                            \
	d0_0 = vis_fpmerge(vis_read_hi(d0_0), vis_read_lo(d0_0));   \
	d0_0 = vis_fpmerge(vis_read_hi(d0_0), vis_read_lo(d0_0));   \
	d0_1 = vis_fpmerge(vis_read_hi(d0_1), vis_read_lo(d0_1));   \
	d0_1 = vis_fpmerge(vis_read_hi(d0_1), vis_read_lo(d0_1));   \
	d1_0 = vis_fmul8x16(vis_read_lo(d0_0), A);                  \
	d1_1 = vis_fmul8x16(vis_read_lo(d0_1), A)

#endif /* defined(MLIB_VIS) && MLIB_VIS >= 0x200 */

/* *********************************************************** */

#define	MLIB_CALC2                                                     \
	lmask = (vis_fcmplt16(dx_0, dmin) << 4) | vis_fcmplt16(dx_1,   \
		dmin);                                                 \
	hmask = (vis_fcmpgt16(dx_0, dmax) << 4) | vis_fcmpgt16(dx_1,   \
		dmax);                                                 \
	d0_0 = vis_fpsub16(dx_0, dwin);                                \
	d0_1 = vis_fpsub16(dx_1, dwin);                                \
	UNMERGE_FMUL8X16;                                              \
	d2_0 = vis_fpadd16(d1_0, dgmin);                               \
	d2_1 = vis_fpadd16(d1_1, dgmin);                               \
	rd = vis_fpack16_pair(d2_0, d2_1)

/* *********************************************************** */

#define	PRE_LOOP(MLIB_CALC)                                       \
	ddp = (mlib_d64 *)((mlib_addr)dst & ~7);                  \
	off = -((mlib_addr)dst & 7);                              \
	dsp = (mlib_d64 *)vis_alignaddr((void *)src, off << 1);   \
	bf_0 = vis_ld_d64_nf(dsp);                                \
	if (off) {                                                \
	    bf_1 = vis_ld_d64_nf(dsp + 1);                        \
	    bf_2 = vis_ld_d64_nf(dsp + 2);                        \
	    emask = vis_edge8(dst, edst);                         \
	    len -= 8 + off;                                       \
	    dx_0 = vis_faligndata(bf_0, bf_1);                    \
	    dx_1 = vis_faligndata(bf_1, bf_2);                    \
	    dsp += 2;                                             \
	    MLIB_CALC;                                            \
	    bf_0 = bf_2;                                          \
	    vis_pst_8(rd, ddp, emask);                            \
	    vis_pst_8(GMIN, ddp, lmask & emask);                  \
	    vis_pst_8(GMAX, ddp, hmask & emask);                  \
	    ddp++;                                                \
	}

/* *********************************************************** */

#define	MAIN_LOOP(MLIB_CALC)                                    \
	for (i = 0; i < (len >> 3); i++) {                      \
	    bf_1 = dsp[1];                                      \
	    bf_2 = vis_ld_d64_nf(dsp + 2);                      \
	    dsp += 2;                                           \
	    dx_0 = vis_faligndata(bf_0, bf_1);                  \
	    dx_1 = vis_faligndata(bf_1, bf_2);                  \
	    MLIB_CALC;                                          \
	    bf_0 = bf_2;                                        \
	    *ddp = rd;                                          \
	    vis_pst_8(GMIN, ddp, lmask);                        \
	    vis_pst_8(GMAX, ddp, hmask);                        \
	    ddp++;                                              \
	}

/* *********************************************************** */

#define	END_LOOP(MLIB_CALC)                                     \
	if ((mlib_addr)ddp <= (mlib_addr)edst) {                \
	    bf_1 = vis_ld_d64_nf(dsp + 1);                      \
	    bf_2 = vis_ld_d64_nf(dsp + 2);                      \
	    emask = vis_edge8(ddp, edst);                       \
	    dx_0 = vis_faligndata(bf_0, bf_1);                  \
	    dx_1 = vis_faligndata(bf_1, bf_2);                  \
	    MLIB_CALC;                                          \
	    vis_pst_8(rd, ddp, emask);                          \
	    vis_pst_8(GMIN, ddp, lmask & emask);                \
	    vis_pst_8(GMAX, ddp, hmask & emask);                \
	}

/* *********************************************************** */

static void
mlib_VolumeWindowLevel1(
	mlib_u8 *dst,
	const mlib_s16 *src,
	mlib_s32 window,
	mlib_s32 level,
	mlib_s32 gmax,
	mlib_s32 gmin,
	mlib_s32 len)
{
	INIT_VARS;
	mlib_s32 ia4[1];
	mlib_f32 A;

	while (2 * a < 1 && scale < 7) {
		a *= 2;
		scale++;
	}

	vis_write_gsr((7 - scale) << 3);

	ia = a * 256.0 + 0.5;

	if (ia > MLIB_U8_MAX)
		ia = MLIB_U8_MAX;
	ia4[0] = (ia << 24) | (ia << 16) | (ia << 8) | ia;
	A = *(mlib_f32 *)ia4;

	dgmin = vis_to_double_dup(((gmin << 16) | gmin) << scale);

	if (window >= (1 << 15)) {
		dwin = 0;
		ia = ((gmax + gmin) << scale) * 0.5 - level * a;
		dgmin = vis_to_double_dup((ia << 16) | (ia & 0xFFFF));
	}

	PRE_LOOP(MLIB_CALC1);

#pragma pipeloop(0)
	MAIN_LOOP(MLIB_CALC1);

	END_LOOP(MLIB_CALC1);
}

/* *********************************************************** */

static void
mlib_VolumeWindowLevel2(
	mlib_u8 *dst,
	const mlib_s16 *src,
	mlib_s32 window,
	mlib_s32 level,
	mlib_s32 gmax,
	mlib_s32 gmin,
	mlib_s32 len)
{
	INIT_VARS;
	mlib_d64 A;

	while (2 * a < (1 << 7) && scale < 7) {
		a *= 2;
		scale++;
	}

	vis_write_gsr((7 - scale) << 3);
	WRITE_BMASK(0x13579BDF);

	ia = a * 256.0 + 0.5;

	if (ia > MLIB_S16_MAX)
		ia = MLIB_S16_MAX;
	A = vis_to_double_dup((ia << 16) | (ia & 0xFFFF));

	dgmin = vis_to_double_dup(((gmin << 16) | gmin) << scale);

	PRE_LOOP(MLIB_CALC2);

#pragma pipeloop(0)
	MAIN_LOOP(MLIB_CALC2);

	END_LOOP(MLIB_CALC2);
}

/* *********************************************************** */

mlib_status
__mlib_VolumeWindowLevel(
	mlib_u8 *dst,
	const mlib_s16 *src,
	mlib_s32 window,
	mlib_s32 level,
	mlib_s32 gmax,
	mlib_s32 gmin,
	mlib_s32 len)
{
	mlib_s32 left = ((2 * level - window) >> 1) + 1;
	mlib_s32 right = ((2 * level + window) >> 1) - 1;

	if (dst == NULL || src == NULL)
		return (MLIB_FAILURE);

	if (window <= 0 || gmin > gmax || gmax > MLIB_U8_MAX ||
		gmin < MLIB_U8_MIN)
		return (MLIB_FAILURE);

	if (left < MLIB_S16_MIN || right > MLIB_S16_MAX || left > right)
		return (MLIB_FAILURE);

	if (len <= 0)
		return (MLIB_FAILURE);

	if (window < (gmax - gmin)) {
		mlib_VolumeWindowLevel2(dst, src, window, level, gmax, gmin,
			len);
	} else {
		mlib_VolumeWindowLevel1(dst, src, window, level, gmax, gmin,
			len);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
