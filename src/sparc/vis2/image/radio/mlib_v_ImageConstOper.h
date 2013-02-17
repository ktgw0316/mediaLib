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

#ifndef _MLIB_V_IMAGECONSTOPER_H
#define	_MLIB_V_IMAGECONSTOPER_H

#pragma ident	"@(#)mlib_v_ImageConstOper.h	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *      Internal function for any image
 *      operation with array of mlib_d64 parameters.
 *
 *   File contains macro function implementation for any type:
 *     FUNC_NAME     - name of internal function.
 *     DTYPE         - data type of image.
 *     LOAD_CONST(A) - load from cnst array according to
 *                     dst line alignment equal A
 *     PROCESS_CH(N) - macro to process source s##N of mlib_d64 type and
 *                     store result by dp pointer
 *     VIS_EDGE      - vis_edge8 or vis_edge16 depending of data type
 */

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(MLIB_VIS) || MLIB_VIS < 0x200
#error this include file can be used with MLIB_VIS >= 2.0
#endif /* !defined(MLIB_VIS) || MLIB_VIS < 0x200 */

#include <mlib_types.h>
#include <mlib_status.h>

extern const mlib_u8 *mlib_mod_nchan[5];
extern const mlib_u32 mlib_bmask_cmp16[16];
extern const mlib_u32 mlib_bmask_cmp32[4];

/* *********************************************************** */

#define	VIS_ALIGNADDR(X, Y)	vis_alignaddr((void *)(X), (Y))

/* *********************************************************** */

#define	PST_8(dp, cmask, aa, bb)                                \
	vis_pst_8(aa, dp, emask & ~cmask);                      \
	vis_pst_8(bb, dp, emask & cmask);                       \
	dp++

/* *********************************************************** */

#define	PST_16(dp, cmask, aa, bb)                               \
	vis_write_bmask(mlib_bmask_cmp16[cmask], 0);            \
	vis_pst_16(vis_bshuffle(aa, bb), dp, emask);            \
	dp++

/* *********************************************************** */

#define	PST_32(dp, cmask, aa, bb)                               \
	vis_write_bmask(mlib_bmask_cmp32[cmask], 0);            \
	vis_pst_32(vis_bshuffle(aa, bb), dp, emask);            \
	dp++

/* *********************************************************** */

#define	CLAMP_U8(x)                                             \
	(((x) > MLIB_U8_MAX) ? MLIB_U8_MAX : (((x) <            \
	    MLIB_U8_MIN) ? MLIB_U8_MIN : (x)))

/* *********************************************************** */

#define	CLAMP_S16(x)                                            \
	(((x) > MLIB_S16_MAX) ? MLIB_S16_MAX : (((x) <          \
	    MLIB_S16_MIN) ? MLIB_S16_MIN : (x)))

/* *********************************************************** */

#define	CLAMP_U16(x)                                            \
	(((x) > MLIB_U16_MAX) ? MLIB_U16_MAX : (((x) <          \
	    MLIB_U16_MIN) ? MLIB_U16_MIN : (x)))

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_V_IMAGECONSTOPER_H */

/* *********************************************************** */

#ifdef FUNC_NAME
#ifdef FUNC_A
static mlib_status FUNC_NAME(
    _A) (
    const DTYPE * sl,
    DTYPE * dl,
    mlib_s32 sstride,
    mlib_s32 dstride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 nchan,
    const mlib_d64 *cnst)
{
	mlib_d64 s0;
	mlib_s32 cmask, emask = 0xFF;
	mlib_s32 i, j, off;

	LOAD_CONST(0);

	width = nchan * width * sizeof (DTYPE);

	for (i = 0; i < height; i++) {
		DTYPE *dend = (DTYPE *) ((mlib_u8 *)dl + width) - 1;
		mlib_d64 *sp = (mlib_d64 *)sl;
		mlib_d64 *dp = (mlib_d64 *)dl;

		emask = 0xFF;

#pragma pipeloop(0)
		for (j = 0; j <= (width - 8); j += 8) {
			s0 = sp[0];
			PROCESS_CH(0);
			sp++;
		}

		if ((DTYPE *) dp <= dend) {
			emask = VIS_EDGE(dp, dend);
			s0 = sp[0];
			PROCESS_CH(0);
		}

		sl += sstride;
		dl += dstride;
	}

	return (MLIB_SUCCESS);
}

#endif /* FUNC_A */

/* *********************************************************** */

static mlib_status FUNC_NAME(
    _3) (
    const DTYPE * sl,
    DTYPE * dl,
    mlib_s32 sstride,
    mlib_s32 dstride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 nchan,
    const mlib_d64 *cnst)
{
	mlib_d64 s0, s1, s2, sx;
	mlib_s32 cmask, emask;
	mlib_s32 i, off;

	width = nchan * width * sizeof (DTYPE);

	for (i = 0; i < height; i++) {
		DTYPE *dend = (DTYPE *) ((mlib_u8 *)dl + width) - 1;
		mlib_d64 *sp;
		mlib_d64 *dp = (mlib_d64 *)((mlib_addr)dl & ~7);
		mlib_s32 j = (DTYPE *) dp - (DTYPE *) dl;

		LOAD_CONST(j);

		j *= sizeof (DTYPE);

		if (((mlib_addr)sl | (mlib_addr)dl) & 7) {
			sp = (mlib_d64 *)VIS_ALIGNADDR(sl, j);
			emask = VIS_EDGE(dl, dend);

			s0 = vis_faligndata(sp[0], sp[1]);
			PROCESS_CH(0);
			sp++;
			j += 8;
			emask = 0xFF;
			sx = *sp;
#pragma pipeloop(0)
			for (; j <= (width - 24); j += 24) {
				s1 = sx;
				s2 = sp[1];
				s0 = sp[2];
				sx = sp[3];
				s1 = vis_faligndata(s1, s2);
				s2 = vis_faligndata(s2, s0);
				s0 = vis_faligndata(s0, sx);
				PROCESS_CH(1);
				PROCESS_CH(2);
				PROCESS_CH(0);
				sp += 3;
			}

			s1 = sx;
			s2 = sp[1];
			s0 = sp[2];
			sx = sp[3];
			s1 = vis_faligndata(s1, s2);
			s2 = vis_faligndata(s2, s0);
			s0 = vis_faligndata(s0, sx);

			if ((DTYPE *) dp <= dend) {
				emask = VIS_EDGE(dp, dend);
				PROCESS_CH(1);

				if ((DTYPE *) dp <= dend) {
					emask = VIS_EDGE(dp, dend);
					PROCESS_CH(2);

					if ((DTYPE *) dp <= dend) {
						emask = VIS_EDGE(dp, dend);
						PROCESS_CH(0);
					}
				}
			}
		} else {
			sp = (mlib_d64 *)sl;
			dp = (mlib_d64 *)dl;
			emask = 0xFF;
#pragma pipeloop(0)
			for (j = 0; j <= (width - 24); j += 24) {
				s0 = sp[0];
				s1 = sp[1];
				s2 = sp[2];
				PROCESS_CH(0);
				PROCESS_CH(1);
				PROCESS_CH(2);
				sp += 3;
			}

			s0 = sp[0];
			s1 = sp[1];
			s2 = sp[2];

			if ((DTYPE *) dp <= dend) {
				emask = VIS_EDGE(dp, dend);
				PROCESS_CH(0);

				if ((DTYPE *) dp <= dend) {
					emask = VIS_EDGE(dp, dend);
					PROCESS_CH(1);

					if ((DTYPE *) dp <= dend) {
						emask = VIS_EDGE(dp, dend);
						PROCESS_CH(2);
					}
				}
			}
		}

		sl += sstride;
		dl += dstride;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#ifdef FUNC_2
static mlib_status FUNC_NAME(
    _2) (
    const DTYPE * sl,
    DTYPE * dl,
    mlib_s32 sstride,
    mlib_s32 dstride,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 nchan,
    const mlib_d64 *cnst)
{
	mlib_d64 s0, s1, s2, sx;
	mlib_s32 cmask, emask;
	mlib_s32 i, off;

	width = nchan * width * sizeof (DTYPE);

	for (i = 0; i < height; i++) {
		DTYPE *dend = (DTYPE *) ((mlib_u8 *)dl + width) - 1;
		mlib_d64 *sp;
		mlib_d64 *dp = (mlib_d64 *)((mlib_addr)dl & ~7);
		mlib_s32 j = (DTYPE *) dp - (DTYPE *) dl;

		LOAD_CONST(j);

		j *= sizeof (DTYPE);

		if (((mlib_addr)sl | (mlib_addr)dl) & 7) {
			sp = (mlib_d64 *)VIS_ALIGNADDR(sl, j);
			emask = VIS_EDGE(dl, dend);

			s0 = vis_faligndata(sp[0], sp[1]);
			PROCESS_CH(0);
			sp++;
			j += 8;
			emask = 0xFF;
			sx = *sp;
#pragma pipeloop(0)
			for (; j <= (width - 16); j += 16) {
				s1 = sx;
				s0 = sp[1];
				sx = sp[2];
				s1 = vis_faligndata(s1, s0);
				s0 = vis_faligndata(s0, sx);
				PROCESS_CH(1);
				PROCESS_CH(0);
				sp += 2;
			}

			s1 = sx;
			s0 = sp[1];
			sx = sp[2];
			s1 = vis_faligndata(s1, s0);
			s0 = vis_faligndata(s0, sx);

			if ((DTYPE *) dp <= dend) {
				emask = VIS_EDGE(dp, dend);
				PROCESS_CH(1);

				if ((DTYPE *) dp <= dend) {
					emask = VIS_EDGE(dp, dend);
					PROCESS_CH(0);
				}
			}
		} else {
			sp = (mlib_d64 *)sl;
			dp = (mlib_d64 *)dl;
			emask = 0xFF;
#pragma pipeloop(0)
			for (j = 0; j <= (width - 16); j += 16) {
				s0 = sp[0];
				s1 = sp[1];
				PROCESS_CH(0);
				PROCESS_CH(1);
				sp += 2;
			}

			s0 = sp[0];
			s1 = sp[1];
			s2 = sp[2];

			if ((DTYPE *) dp <= dend) {
				emask = VIS_EDGE(dp, dend);
				PROCESS_CH(0);

				if ((DTYPE *) dp <= dend) {
					emask = VIS_EDGE(dp, dend);
					PROCESS_CH(1);
				}
			}
		}

		sl += sstride;
		dl += dstride;
	}

	return (MLIB_SUCCESS);
}

#endif /* FUNC_2 */
#endif /* FUNC_NAME */

/* *********************************************************** */

#undef  FUNC_NAME
#undef  FUNC_A
#undef  FUNC_2
#undef  DTYPE
#undef  VIS_EDGE
#undef  LOAD_CONST
#undef  PROCESS_CH
