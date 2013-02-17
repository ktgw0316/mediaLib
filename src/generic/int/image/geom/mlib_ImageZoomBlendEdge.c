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

#pragma ident	"@(#)mlib_ImageZoomBlendEdge.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *      Edge processing for mlib_ImageZoomTranslateBlend().
 *
 */

#ifndef	_NO_LONGLONG

#include <mlib_image.h>
#include <mlib_ImageFilters.h>
#include <mlib_ImageZoom.h>
#include <mlib_ImageDivTables.h>
#include <mlib_Utils.h>
#include <mlib_GeomBasicIntOp.h>

/* *********************************************************** */

#define	SATUR_U8_INT(DST, SRC)                                  \
	{                                                       \
		FP_TYPE	dsrc = (FP_TYPE) (SRC);                 \
		if (dsrc > 255) {                               \
			dsrc = 255;                             \
		}                                               \
		if (dsrc < 0) {                                 \
			dsrc = 0;                               \
		}                                               \
		(DST) = dsrc;                                   \
	}

/* *********************************************************** */

#define	STORE(dst, fres)                                        \
	dst = fres

/* *********************************************************** */

#define	BLEND33(dst, fres)                                      \
{                                                               \
	mlib_s64 out, in;                                       \
	in = a0 * (fres - dst);                                 \
	DIV255(out, in);                                        \
	dst = (mlib_s32)(out) + dst;                            \
}

/* *********************************************************** */

#define	BLEND33_dalpha(dst, fres)                               \
{                                                               \
	mlib_s64 out, in;                                       \
	in = a0 * (fres - dst);                                 \
	DIV65025(out, in);                                      \
	dst = (mlib_s32)(out) + dst;                            \
}

/* *********************************************************** */

#define	BLEND34(dst, fres)                                      \
	dst += (mlib_s32)(MUL255(fres - dst) / w)

#define	BLEND34_OVER2(dst, fres)                                \
	dst += (mlib_s32)((fres - dst) * w0 / w)

#define	BLEND34z(dst, fres)                                     \
	dst = (mlib_u8)((w0 * fres + w1 * dst) / w)

/* *********************************************************** */

#define	BLEND43(dst, fres)                                      \
{                                                               \
	mlib_s64 out, in;                                       \
	in = (MUL255(a0)) * fres + a1 * dst;                    \
	DIV65025(out, in);                                      \
	SATUR_U8_INT(dst, out);                                 \
}

/* equivalent to above macro */
#if 0
#define	BLEND43(dst, fres)                                      \
	SATUR_U8_INT(dst, ((a0 * fres + (a1 * dst / 255)) / 255))
#endif

/* *********************************************************** */

#define	BLEND44(dst, fres)                                        \
	SATUR_U8_INT(dst, dst + ((dalpha0 * (255*255) * fres - w0 * dst) / w))

#define	BLEND44_A(dst)                                            \
	SATUR_U8_INT(dst, dst)

#define	BLEND44z(dst, fres)                                       \
	SATUR_U8_INT(dst, (dalpha0 * (255*255) * fres + w1 * dst) / w)

/* *********************************************************** */

#define	BLEND_GTK_SRC(dst, fres)                                \
	SATUR_U8_INT(dst, MUL255(fres) / w)

/* *********************************************************** */

#define	FP_TYPE	mlib_s64

/* *********************************************************** */

mlib_status
mlib_ZoomBlendEdge(
	mlib_image *dst,
	const mlib_image *src,
	mlib_work_image * param,
	mlib_filter filter,
	mlib_edge edge,
	mlib_s32 alp_ind)
{
	mlib_s32 schan, dchan, t_ind, bsize, i, j, k;
	mlib_blend blend = param->blend;
	mlib_edge_box *edges = param->edges;
	mlib_u8 *buff, *pbuff;

	schan = mlib_ImageGetChannels(src);
	dchan = mlib_ImageGetChannels(dst);
	t_ind = (schan - 3) + 2 * (dchan - 3);

	bsize = 0;
	for (i = 0; i < 4; i++) {
		if (edges[i].w < 0) edges[i].w = 0;
		if (edges[i].h < 0) edges[i].h = 0;
		bsize += edges[i].w * edges[i].h;
	}

	if (!bsize)
		return (MLIB_SUCCESS);

	bsize *= schan;

	mlib_ImageCopy_na((void *)edges, (void *)&(param->edges_blend),
		sizeof (param->edges_blend));

	pbuff = buff = __mlib_malloc(bsize);

	if (buff == NULL)
		return (MLIB_FAILURE);

	for (i = 0; i < 4; i++) {
		mlib_s32 dlb = schan * edges[i].w;

		edges[i].dp = pbuff;
		edges[i].dlb = dlb;
		pbuff += dlb * edges[i].h;
	}

	MLIB_EDGE_RULES;

	for (k = 0; k < 4; k++) {
		mlib_s32 ww = edges[k].w;
		mlib_s32 hh = edges[k].h;
		mlib_s32 slb = edges[k].dlb;
		mlib_s32 dlb = param->edges_blend[k].dlb;
		mlib_u8 *sp = edges[k].dp;
		mlib_u8 *dp = param->edges_blend[k].dp;

		if (ww == 0 || hh == 0)
			continue;

		if (alp_ind == -1) {
			if (schan == 4)
				sp++;
			if (dchan == 4)
				dp++;
		}

/* 33 */
		if (t_ind == 0) {
			FP_TYPE a0 = (param->alpha);

			for (j = 0; j < hh; j++) {
				for (i = 0; i < 3 * ww; i++) {
					BLEND33(dp[i], sp[i]);
				}
				sp += slb;
				dp += dlb;
			}
		}

		else if (t_ind == 1) { /* 43 */
			FP_TYPE a0 = (param->alpha), dalpha = a0;

			if (edge == MLIB_EDGE_SRC_EXTEND ||
				edge == MLIB_EDGE_SRC_EXTEND_INDEF) {
				FP_TYPE a1;

				if (blend == MLIB_BLEND_GTK_SRC) {
					for (j = 0; j < hh; j++) {
						for (i = 0; i < ww; i++) {
							STORE(dp[3 * i],
								sp[4 * i]);
							STORE(dp[3 * i + 1],
								sp[4 * i + 1]);
							STORE(dp[3 * i + 2],
								sp[4 * i + 2]);
						}
						sp += slb;
						dp += dlb;
					}
				} else {
					for (j = 0; j < hh; j++) {
						for (i = 0; i < ww; i++) {
							a1 = (255*255) -
							    dalpha * sp[4 * i +
							    alp_ind];
							BLEND43(dp[3 * i],
								sp[4 * i]);
							BLEND43(dp[3 * i + 1],
								sp[4 * i + 1]);
							BLEND43(dp[3 * i + 2],
								sp[4 * i + 2]);
						}
						sp += slb;
						dp += dlb;
					}
				}
			} else {
				if (blend == MLIB_BLEND_GTK_SRC) {
					for (j = 0; j < hh; j++) {
						for (i = 0; i < ww; i++) {
							mlib_s64 out, in;
							a0 = sp[4 * i +
								alp_ind];
							in = a0 * sp[4 * i];
							DIV255(out, in);
							dp[3 * i] =
								(mlib_u8)(out);
							in = a0 * sp[4 * i + 1];
							DIV255(out, in);
							dp[3 * i + 1] =
								(mlib_u8)(out);
							in = a0 * sp[4 * i + 2];
							DIV255(out, in);
							dp[3 * i + 2] =
								(mlib_u8)(out);
						}
						sp += slb;
						dp += dlb;
					}
				} else {
					for (j = 0; j < hh; j++) {
						for (i = 0; i < ww; i++) {
							a0 = dalpha * sp[4 * i +
								alp_ind];
							BLEND33_dalpha(
								dp[3 * i],
								sp[4 * i]);
							BLEND33_dalpha(
								dp[3 * i + 1],
								sp[4 * i + 1]);
							BLEND33_dalpha(
								dp[3 * i + 2],
								sp[4 * i + 2]);
						}
						sp += slb;
						dp += dlb;
					}
				}
			}
		} else if (t_ind == 2) {
/* 34 */
			FP_TYPE w0 = (FP_TYPE) param->alpha;
			FP_TYPE w1s = 255 - w0;
			w0 = MUL255(w0);
			FP_TYPE w1, w;

			if (blend == MLIB_BLEND_GTK_SRC_OVER2) {
				for (j = 0; j < hh; j++) {
					for (i = 0; i < ww; i++) {
						w1 = w1s * dp[4 * i + alp_ind];
						w = w0 + w1;

						if (w < 255) {
							dp[4 * i + alp_ind] = 0;
						} else {
							BLEND34_OVER2(dp[4 * i],
							    sp[3 * i]);
							BLEND34_OVER2(
							    dp[4 * i + 1],
							    sp[3 * i + 1]);
							BLEND34_OVER2(
							    dp[4 * i + 2],
							    sp[3 * i + 2]);
							mlib_s64 out;
							DIV255(out, w);
							dp[4 * i + alp_ind] =
							    (mlib_u8)(out);
						}
					}
					sp += slb;
					dp += dlb;
				}
			} else {
				for (j = 0; j < hh; j++) {
					for (i = 0; i < ww; i++) {
						w1 = w1s * dp[4 * i + alp_ind];
						w = w0 + w1;

						if (w < 255) {
							dp[4 * i] = 0;
							dp[4 * i + 1] = 0;
							dp[4 * i + 2] = 0;
							dp[4 * i + alp_ind] = 0;
						} else {
							BLEND34z(dp[4 * i],
							    sp[3 * i]);
							BLEND34z(dp[4 * i + 1],
							    sp[3 * i + 1]);
							BLEND34z(dp[4 * i + 2],
							    sp[3 * i + 2]);
							mlib_s64 out;
							DIV255(out, w);
							dp[4 * i + alp_ind] =
							    (mlib_u8)(out);
						}
					}
					sp += slb;
					dp += dlb;
				}
			}
		} else if (t_ind == 3) {
/* 44 */
			FP_TYPE dalpha0 = (FP_TYPE) param->alpha,
				dalpha = dalpha0;
			FP_TYPE w0, w1, w;

			if (edge == MLIB_EDGE_SRC_EXTEND ||
				edge == MLIB_EDGE_SRC_EXTEND_INDEF) {
				for (j = 0; j < hh; j++) {
					if (blend == MLIB_BLEND_GTK_SRC) {
						for (i = 0; i < ww; i++) {
							w = sp[4 * i + alp_ind];

							if (w == 0) {
								dp[4 * i] = 0;
								dp[4 * i + 1] =
								    0;
								dp[4 * i + 2] =
								    0;
								dp[4 * i +
								    alp_ind] =
								    0;
							} else {
								BLEND_GTK_SRC(
								    dp[4 * i],
								    sp[4 * i]);
								BLEND_GTK_SRC(
								    dp[4 * i +
								    1],
								    sp[4 * i +
								    1]);
								BLEND_GTK_SRC(
								    dp[4 * i +
								    2],
								    sp[4 * i +
								    2]);
								dp[4 * i +
								    alp_ind] =
								    sp[4 * i +
								    alp_ind];
							}
						}
					} else if (blend ==
						MLIB_BLEND_GTK_SRC_OVER2) {
						for (i = 0; i < ww; i++) {
							w0 = dalpha * sp[4 * i +
								alp_ind];
							w1 = ((255*255) - w0) *
								dp[4 * i +
								alp_ind];
							w0 = MUL255(w0);
							w = w0 + w1;

							if (w < (255*255)) {
								BLEND44_A(
								    dp[4 * i]);
								BLEND44_A(
								    dp[4 * i +
								    1]);
								BLEND44_A(
								    dp[4 * i +
								    2]);
								dp[4 * i +
								    alp_ind] =
								    0;
							} else {
								BLEND44(
								    dp[4 * i],
								    sp[4 * i]);
								BLEND44(
								    dp[4 * i +
								    1],
								    sp[4 * i +
								    1]);
								BLEND44(
								    dp[4 * i +
								    2],
								    sp[4 * i +
								    2]);
								mlib_s64 out;
								DIV65025(out,
								    w);
								dp[4 * i +
								    alp_ind] =
								    (mlib_u8)
								    (out);
							}
						}
					} else {
						for (i = 0; i < ww; i++) {
							w0 = dalpha * sp[4 * i +
								alp_ind];
							w1 = ((255*255) - w0) *
								dp[4 * i +
								alp_ind];
							w0 = MUL255(w0);
							w = w0 + w1;

							if (w < (255*255)) {
								dp[4 * i] = 0;
								dp[4 * i + 1] =
								    0;
								dp[4 * i + 2] =
								    0;
								dp[4 * i +
								    alp_ind] =
								    0;
							} else {
								BLEND44z(
								    dp[4 * i],
								    sp[4 * i]);
								BLEND44z(
								    dp[4 * i +
								    1],
								    sp[4 * i +
								    1]);
								BLEND44z(
								    dp[4 * i +
								    2],
								    sp[4 * i +
								    2]);
								mlib_s64 out;
								DIV65025(out,
								    w);
								dp[4 * i +
								    alp_ind] =
								    (mlib_u8)
								    (out);
							}
						}
					}
					sp += slb;
					dp += dlb;
				}
			} else {
				for (j = 0; j < hh; j++) {
					if (blend == MLIB_BLEND_GTK_SRC) {
						for (i = 0; i < ww; i++) {
							dp[4 * i] = sp[4 * i];
							dp[4 * i + 1] =
								sp[4 * i + 1];
							dp[4 * i + 2] =
								sp[4 * i + 2];
							dp[4 * i + alp_ind] =
								sp[4 * i +
								alp_ind];
						}
					} else if (blend ==
						MLIB_BLEND_GTK_SRC_OVER2) {
						for (i = 0; i < ww; i++) {
							w0 = dalpha * sp[4 * i +
								alp_ind];
							w1 = ((255*255) - w0) *
								dp[4 * i +
								alp_ind];
							w0 = MUL255(w0);
							w = w0 + w1;

							if (w < (255*255)) {
								dp[4 * i +
								    alp_ind] =
								    0;
							} else {
								BLEND34_OVER2(
								    dp[4 * i],
								    sp[4 * i]);
								BLEND34_OVER2(
								    dp[4 * i +
								    1],
								    sp[4 * i +
								    1]);
								BLEND34_OVER2(
								    dp[4 * i +
								    2],
								    sp[4 * i +
								    2]);
								mlib_s64 out;
								DIV65025(out,
								    w);
								dp[4 * i +
								    alp_ind] =
								    (mlib_u8)
								    (out);
							}
						}
					} else {
						for (i = 0; i < ww; i++) {
							w0 = dalpha * sp[4 * i +
								alp_ind];
							w1 = ((255*255) - w0) *
								dp[4 * i +
								alp_ind];
							w0 = MUL255(w0);
							w = w0 + w1;

							if (w < (255*255)) {
								dp[4 * i] = 0;
								dp[4 * i + 1] =
								    0;
								dp[4 * i + 2] =
								    0;
								dp[4 * i +
								    alp_ind] =
								    0;
							} else {
								BLEND34z(
								    dp[4 * i],
								    sp[4 * i]);
								BLEND34z(
								    dp[4 * i +
								    1],
								    sp[4 * i +
								    1]);
								BLEND34z(
								    dp[4 * i +
								    2],
								    sp[4 * i +
								    2]);
								mlib_s64 out;
								DIV65025(out,
								    w);
								dp[4 * i +
								    alp_ind] =
								    (mlib_u8)
								    (out);
							}
						}
					}
					sp += slb;
					dp += dlb;
				}
			}
		}
	}

	__mlib_free(buff);

	return (MLIB_SUCCESS);
}

#else /* _NO_LONGLONG */

#include <mlib_image.h>
#include <mlib_ImageFilters.h>
#include <mlib_ImageZoom.h>
#include <mlib_ImageDivTables.h>
#include <mlib_Utils.h>

/* *********************************************************** */

#define	STORE(dst, fres)                                        \
	dst = fres

/* *********************************************************** */

#define	BLEND33(dst, fres)                                      \
	dst = (mlib_s32)(a0 * (fres - dst)) + dst

/* *********************************************************** */

#define	BLEND34(dst, fres)                                      \
	dst += (mlib_s32)((fres - dst) * rw)

#define	BLEND34z(dst, fres)                                     \
	dst = (mlib_u8)((w0 * fres + w1 * dst) * rw)

/* *********************************************************** */

#define	BLEND43(dst, fres)                                      \
	SATUR_U8(dst, a0 * fres + a1 * dst)

/* *********************************************************** */

#define	BLEND44(dst, fres)                                        \
	SATUR_U8(dst, dst + ((dalpha0 * fres - w0 * dst) * rw))

#define	BLEND44z(dst, fres)                                     \
	SATUR_U8(dst, (dalpha0 * fres + w1 * dst) * rw)

/* *********************************************************** */

#define	BLEND_GTK_SRC(dst, fres)                                \
	SATUR_U8(dst, fres * rw)

/* *********************************************************** */

#define	FP_TYPE	mlib_f32
#define	F_ONE	((FP_TYPE)1)

/* *********************************************************** */

mlib_status
mlib_ZoomBlendEdge(
	mlib_image *dst,
	const mlib_image *src,
	mlib_work_image * param,
	mlib_filter filter,
	mlib_edge edge,
	mlib_s32 alp_ind)
{
	mlib_s32 schan, dchan, t_ind, bsize, i, j, k;
	mlib_blend blend = param->blend;
	mlib_edge_box *edges = param->edges;
	mlib_u8 *buff, *pbuff;

	schan = mlib_ImageGetChannels(src);
	dchan = mlib_ImageGetChannels(dst);
	t_ind = (schan - 3) + 2 * (dchan - 3);

	bsize = 0;
	for (i = 0; i < 4; i++) {
		if (edges[i].w < 0) edges[i].w = 0;
		if (edges[i].h < 0) edges[i].h = 0;
		bsize += edges[i].w * edges[i].h;
	}

	if (!bsize)
		return (MLIB_SUCCESS);

	bsize *= schan;

	mlib_ImageCopy_na((void *)edges, (void *)&(param->edges_blend),
		sizeof (param->edges_blend));

	pbuff = buff = __mlib_malloc(bsize);

	if (buff == NULL)
		return (MLIB_FAILURE);

	for (i = 0; i < 4; i++) {
		mlib_s32 dlb = schan * edges[i].w;

		edges[i].dp = pbuff;
		edges[i].dlb = dlb;
		pbuff += dlb * edges[i].h;
	}

	MLIB_EDGE_RULES;

	for (k = 0; k < 4; k++) {
		mlib_s32 ww = edges[k].w;
		mlib_s32 hh = edges[k].h;
		mlib_s32 slb = edges[k].dlb;
		mlib_s32 dlb = param->edges_blend[k].dlb;
		mlib_u8 *sp = edges[k].dp;
		mlib_u8 *dp = param->edges_blend[k].dp;

		if (ww == 0 || hh == 0)
			continue;

		if (alp_ind == -1) {
			if (schan == 4)
				sp++;
			if (dchan == 4)
				dp++;
		}

/* 33 */
		if (t_ind == 0) {
			FP_TYPE a0 = (param->alpha) * (F_ONE / 255);

			for (j = 0; j < hh; j++) {
				for (i = 0; i < 3 * ww; i++) {
					BLEND33(dp[i], sp[i]);
				}
				sp += slb;
				dp += dlb;
			}
		}

		else if (t_ind == 1) { /* 43 */
			FP_TYPE a0 = (param->alpha) * (F_ONE / 255), dalpha =
				a0 * (F_ONE / 255);

			if (edge == MLIB_EDGE_SRC_EXTEND ||
				edge == MLIB_EDGE_SRC_EXTEND_INDEF) {
				FP_TYPE a1;

				if (blend == MLIB_BLEND_GTK_SRC) {
					for (j = 0; j < hh; j++) {
						for (i = 0; i < ww; i++) {
							STORE(dp[3 * i],
								sp[4 * i]);
							STORE(dp[3 * i + 1],
								sp[4 * i + 1]);
							STORE(dp[3 * i + 2],
								sp[4 * i + 2]);
						}
						sp += slb;
						dp += dlb;
					}
				} else {
					for (j = 0; j < hh; j++) {
						for (i = 0; i < ww; i++) {
							a1 = 1 - dalpha * sp[4 *
								i + alp_ind];
							BLEND43(dp[3 * i],
								sp[4 * i]);
							BLEND43(dp[3 * i + 1],
								sp[4 * i + 1]);
							BLEND43(dp[3 * i + 2],
								sp[4 * i + 2]);
						}
						sp += slb;
						dp += dlb;
					}
				}
			} else {
				if (blend == MLIB_BLEND_GTK_SRC) {
					for (j = 0; j < hh; j++) {
						for (i = 0; i < ww; i++) {
							a0 = sp[4 * i +
								alp_ind] *
								(F_ONE / 255);
							dp[3 * i] =
								(mlib_u8)(a0 *
								sp[4 * i]);
							dp[3 * i + 1] =
								(mlib_u8)(a0 *
								sp[4 * i + 1]);
							dp[3 * i + 2] =
								(mlib_u8)(a0 *
								sp[4 * i + 2]);
						}
						sp += slb;
						dp += dlb;
					}
				} else {
					for (j = 0; j < hh; j++) {
						for (i = 0; i < ww; i++) {
							a0 = dalpha * sp[4 * i +
								alp_ind];
							BLEND33(dp[3 * i],
								sp[4 * i]);
							BLEND33(dp[3 * i + 1],
								sp[4 * i + 1]);
							BLEND33(dp[3 * i + 2],
								sp[4 * i + 2]);
						}
						sp += slb;
						dp += dlb;
					}
				}
			}
		} else if (t_ind == 2) {
/* 34 */
			FP_TYPE w0 = (FP_TYPE) param->alpha;
			FP_TYPE w1s = F_ONE - w0 * (F_ONE / 255);
			FP_TYPE w1, w, rw;

			if (blend == MLIB_BLEND_GTK_SRC_OVER2) {
				for (j = 0; j < hh; j++) {
					for (i = 0; i < ww; i++) {
						w1 = w1s * dp[4 * i + alp_ind];
						w = w0 + w1;
						FP_DIV(rw, w0, w);

						BLEND34(dp[4 * i], sp[3 * i]);
						BLEND34(dp[4 * i + 1],
							sp[3 * i + 1]);
						BLEND34(dp[4 * i + 2],
							sp[3 * i + 2]);
						dp[4 * i + alp_ind] =
							(mlib_u8)w;
					}
					sp += slb;
					dp += dlb;
				}
			} else {
				for (j = 0; j < hh; j++) {
					for (i = 0; i < ww; i++) {
						w1 = w1s * dp[4 * i + alp_ind];
						w = w0 + w1;
						FP_INV(rw, w);

						BLEND34z(dp[4 * i], sp[3 * i]);
						BLEND34z(dp[4 * i + 1],
							sp[3 * i + 1]);
						BLEND34z(dp[4 * i + 2],
							sp[3 * i + 2]);
						dp[4 * i + alp_ind] =
							(mlib_u8)w;
					}
					sp += slb;
					dp += dlb;
				}
			}
		} else if (t_ind == 3) {
/* 44 */
			FP_TYPE dalpha0 = (FP_TYPE) param->alpha,
				dalpha = dalpha0 * (F_ONE / 255);
			FP_TYPE w0, w1, w, rw;

			if (edge == MLIB_EDGE_SRC_EXTEND ||
				edge == MLIB_EDGE_SRC_EXTEND_INDEF) {
				for (j = 0; j < hh; j++) {
					if (blend == MLIB_BLEND_GTK_SRC) {
						for (i = 0; i < ww; i++) {
							w = sp[4 * i + alp_ind];
							FP_DIV(rw, 255, w);

							BLEND_GTK_SRC(dp[4 * i],
								sp[4 * i]);
							BLEND_GTK_SRC(dp[4 * i +
								1],
								sp[4 * i + 1]);
							BLEND_GTK_SRC(dp[4 * i +
								2],
								sp[4 * i + 2]);
							dp[4 * i + alp_ind] =
								sp[4 * i +
								alp_ind];
						}
					} else if (blend ==
						MLIB_BLEND_GTK_SRC_OVER2) {
						for (i = 0; i < ww; i++) {
							w0 = dalpha * sp[4 * i +
								alp_ind];
							w1 = (F_ONE -
								w0 * (F_ONE /
								255)) * dp[4 *
								i + alp_ind];
							w = w0 + w1;
							FP_INV(rw, w);

							BLEND44(dp[4 * i],
								sp[4 * i]);
							BLEND44(dp[4 * i + 1],
								sp[4 * i + 1]);
							BLEND44(dp[4 * i + 2],
								sp[4 * i + 2]);
							dp[4 * i + alp_ind] =
								(mlib_u8)w;
						}
					} else {
						for (i = 0; i < ww; i++) {
							w0 = dalpha * sp[4 * i +
								alp_ind];
							w1 = (F_ONE -
								w0 * (F_ONE /
								255)) * dp[4 *
								i + alp_ind];
							w = w0 + w1;
							FP_INV(rw, w);

							BLEND44z(dp[4 * i],
								sp[4 * i]);
							BLEND44z(dp[4 * i + 1],
								sp[4 * i + 1]);
							BLEND44z(dp[4 * i + 2],
								sp[4 * i + 2]);
							dp[4 * i + alp_ind] =
								(mlib_u8)w;
						}
					}
					sp += slb;
					dp += dlb;
				}
			} else {
				for (j = 0; j < hh; j++) {
					if (blend == MLIB_BLEND_GTK_SRC) {
						for (i = 0; i < ww; i++) {
							dp[4 * i] = sp[4 * i];
							dp[4 * i + 1] =
								sp[4 * i + 1];
							dp[4 * i + 2] =
								sp[4 * i + 2];
							dp[4 * i + alp_ind] =
								sp[4 * i +
								alp_ind];
						}
					} else if (blend ==
						MLIB_BLEND_GTK_SRC_OVER2) {
						for (i = 0; i < ww; i++) {
							w0 = dalpha * sp[4 * i +
								alp_ind];
							w1 = (F_ONE -
								w0 * (F_ONE /
								255)) * dp[4 *
								i + alp_ind];
							w = w0 + w1;
							FP_DIV(rw, w0, w);

							BLEND34(dp[4 * i],
								sp[4 * i]);
							BLEND34(dp[4 * i + 1],
								sp[4 * i + 1]);
							BLEND34(dp[4 * i + 2],
								sp[4 * i + 2]);
							dp[4 * i + alp_ind] =
								(mlib_u8)w;
						}
					} else {
						for (i = 0; i < ww; i++) {
							w0 = dalpha * sp[4 * i +
								alp_ind];
							w1 = (F_ONE -
								w0 * (F_ONE /
								255)) * dp[4 *
								i + alp_ind];
							w = w0 + w1;
							FP_INV(rw, w);

							BLEND34z(dp[4 * i],
								sp[4 * i]);
							BLEND34z(dp[4 * i + 1],
								sp[4 * i + 1]);
							BLEND34z(dp[4 * i + 2],
								sp[4 * i + 2]);
							dp[4 * i + alp_ind] =
								(mlib_u8)w;
						}
					}
					sp += slb;
					dp += dlb;
				}
			}
		}
	}

	__mlib_free(buff);

	return (MLIB_SUCCESS);
}

#endif /* _NO_LONGLONG */

/* *********************************************************** */
