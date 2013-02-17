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

#pragma ident	"@(#)mlib_ImageZoomOut2XEdge.c	9.2	07/10/09 SMI"

#include <mlib_image.h>
#include <mlib_ImageZoomOut2X.h>
#include <mlib_ImageZoom2X.h>

/* *********************************************************** */

#define	CASE_EDGE_FILL_ZERO(NuLL)                                          \
	if (top)                                                           \
	    for (j = 0; j < dst_width * chan; j++)                         \
		p_dst[j] = NuLL;                                           \
	if (bottom)                                                        \
	    for (j = 0; j < dst_width * chan; j++)                         \
		p_dst[(dst_height - 1) * dst_stride + j] = NuLL;           \
	for (l = 0; l < chan; l++) {                                       \
	    if (left) {                                                    \
		for (i = 0; i < dst_height; i++)                           \
		    p_dst[i * dst_stride + l] = NuLL;                      \
	    }                                                              \
	    if (right) {                                                   \
		for (i = 0; i < dst_height; i++)                           \
		    p_dst[i * dst_stride + (dst_width - 1) * chan + l] =   \
			NuLL;                                              \
	    }                                                              \
	}

/* *********************************************************** */

#define	CASE_EDGE_NEAREST                                                  \
	for (l = 0; l < chan; l++) {                                       \
	    if (top)                                                       \
		for (j = 0; j < dst_width; j++)                            \
		    p_dst[j * chan + l] = p_src[2 * j * chan + l];         \
	    if (bottom)                                                    \
		for (j = 0; j < dst_width; j++)                            \
		    p_dst[(dst_height - 1) * dst_stride + j * chan +       \
			l] =                                               \
			p_src[2 * (dst_height - 1) * src_stride +          \
			2 * j * chan + l];                                 \
	    if (left)                                                      \
		for (i = 0; i < dst_height; i++)                           \
		    p_dst[i * dst_stride + l] =                            \
			p_src[2 * i * src_stride + l];                     \
	    if (right)                                                     \
		for (i = 0; i < dst_height; i++)                           \
		    p_dst[i * dst_stride + (dst_width - 1) * chan + l] =   \
			p_src[2 * i * src_stride + 2 * (dst_width -        \
			1) * chan + l];                                    \
	}

/* *********************************************************** */

#define	CASE_EDGE_BILINEAR(DIF1, DIF2)                                     \
	for (l = 0; l < chan; l++) {                                       \
	    if (top) {                                                     \
		if (dw) {                                                  \
		    for (j = 0; j < dst_width; j++) {                      \
			p_dst[j * chan + l] = p_src[2 * j * chan + l];     \
		    }                                                      \
		} else {                                                   \
		    if (left) {                                            \
			p_dst[l] = p_src[l];                               \
		    }                                                      \
		    for (j = left; j < (dst_width - right); j++) {         \
			p_dst[j * chan + l] =                              \
			    ((p_src[(2 * j - 1) * chan + l] DIF1) +        \
			    (p_src[2 * j * chan + l] DIF1)) DIF2;          \
		    }                                                      \
		}                                                          \
	    }                                                              \
	    if (bottom) {                                                  \
		if (dw) {                                                  \
		    for (j = 0; j < dst_width; j++) {                      \
			p_dst[(dst_height - 1) * dst_stride + j * chan +   \
			    l] =                                           \
			    p_src[2 * (dst_height - 1) * src_stride +      \
			    2 * j * chan + l];                             \
		    }                                                      \
		} else {                                                   \
		    if (left) {                                            \
			p_dst[(dst_height - 1) * dst_stride + l] =         \
			    p_src[2 * (dst_height - 1) * src_stride +      \
			    l];                                            \
		    }                                                      \
		    for (j = left; j < (dst_width - right); j++) {         \
			p_dst[(dst_height - 1) * dst_stride + j * chan +   \
			    l] =                                           \
			    ((p_src[2 * (dst_height - 1) * src_stride +    \
			    (2 * j - 1) * chan + l] DIF1) +                \
			    (p_src[2 * (dst_height - 1) * src_stride +     \
			    2 * j * chan + l] DIF1)) DIF2;                 \
		    }                                                      \
		}                                                          \
	    }                                                              \
	    if (left) {                                                    \
		if (dh) {                                                  \
		    for (i = 0; i < dst_height; i++) {                     \
			p_dst[i * dst_stride + l] =                        \
			    p_src[2 * i * src_stride + l];                 \
		    }                                                      \
		} else {                                                   \
		    for (i = top; i < (dst_height - bottom); i++) {        \
			p_dst[i * dst_stride + l] =                        \
			    ((p_src[(2 * i - 1) * src_stride +             \
			    l] DIF1) + (p_src[2 * i * src_stride +         \
			    l] DIF1)) DIF2;                                \
		    }                                                      \
		}                                                          \
	    }                                                              \
	    if (right) {                                                   \
		if (dh) {                                                  \
		    for (i = 0; i < dst_height; i++) {                     \
			p_dst[i * dst_stride + (dst_width - 1) * chan +    \
			    l] =                                           \
			    p_src[2 * i * src_stride + 2 * (dst_width -    \
			    1) * chan + l];                                \
		    }                                                      \
		} else {                                                   \
		    for (i = top; i < (dst_height - bottom); i++) {        \
			p_dst[i * dst_stride + (dst_width - 1) * chan +    \
			    l] =                                           \
			    ((p_src[(2 * i - 1) * src_stride +             \
			    2 * (dst_width - 1) * chan + l] DIF1) +        \
			    (p_src[2 * i * src_stride + 2 * (dst_width -   \
			    1) * chan + l] DIF1)) DIF2;                    \
		    }                                                      \
		}                                                          \
	    }                                                              \
	}

/* *********************************************************** */

#define	PDT	p_dst[j * chan + l]
#define	PDB	p_dst[(dst_height - 1) * dst_stride + j * chan + l]

#define	PDL	p_dst[i * dst_stride + l]

#define	PDR	p_dst[i * dst_stride + (dst_width - 1) * chan + l]

/* *********************************************************** */

#define	PST(ac, ad)	p_src[ad + (j2 + ac) * chan + l]
#define	PSB(ac, av)	p_src[(2 * (dst_height-1)+av)*src_stride + \
				(2*j + ac) * chan + l]

#define	PSL(ay, ax)	p_src[(2 * ay) * src_stride   ax + l]

#define	PSR(ay, ax)	p_src[(2 * ay) * src_stride + \
				(2 * (dst_width - 1) ax) * chan + l]

/* *********************************************************** */

#define	CASE_EDGE_BICUBIC(SATUR, TYPED, CONST_BC, CONST_BD,                  \
	NORMAL, NORMAL2, TYPE)                                               \
	for (l = 0; l < chan; l++) {                                         \
	    if (top) {                                                       \
		mlib_s32 j2;                                                 \
	                                                                     \
		if (dst_height == 1 && dh == 0) {                            \
	                                                                     \
/*                                                                           \
 * 1-3 size addition                                                         \
 */                                                                          \
		    if ((dw & 1)) {                                          \
	                                                                     \
/*                                                                           \
 * y-case                                                                    \
 */                                                                          \
			mlib_s32 pred_index = src_stride * (dh == 0);        \
	                                                                     \
			for (j = 0, j2 = 0; j < dst_width; j++, j2 += 2) {   \
			    res =                                            \
				(CONST_BD * ((TYPE) PST(0,                   \
				0) + (TYPE) PST(0,                           \
				-pred_index))) NORMAL;                       \
			    PDT = SATUR(res);                                \
			}                                                    \
		    } else {                                                 \
	                                                                     \
/*                                                                           \
 * xy-case                                                                   \
 */                                                                          \
			mlib_s32 pred_index_h = src_stride;                  \
			mlib_s32 pred_index_w = exist_w | (dw == 0);         \
			TYPE st0 =                                           \
			    CONST_BD * ((TYPE) p_src[-(exist_w +             \
			    pred_index_w) * chan + l] +                      \
			    (TYPE) p_src[-pred_index_h - (exist_w +          \
			    pred_index_w) * chan + l]), st1 =                \
			    CONST_BD * ((TYPE) p_src[-pred_index_w *         \
			    chan + l] + (TYPE) p_src[-pred_index_h -         \
			    pred_index_w * chan + l]), st2, st3;             \
			for (j = 0, j2 = 0; j < (dst_width - 1);             \
			    j++, j2 += 2) {                                  \
			    st2 =                                            \
				CONST_BD * ((TYPE) PST(0,                    \
				0) + (TYPE) PST(0, -pred_index_h));          \
			    st3 =                                            \
				CONST_BD * ((TYPE) PST(1,                    \
				0) + (TYPE) PST(1, -pred_index_h));          \
			    res =                                            \
				(CONST_BC * (st1 + st2) - st0 -              \
				st3) NORMAL2;                                \
			    st0 = st2;                                       \
			    st1 = st3;                                       \
			    PDT = SATUR(res);                                \
			}                                                    \
			st2 =                                                \
			    CONST_BD * ((TYPE) PST(0, 0) + (TYPE) PST(0,     \
			    -pred_index_h));                                 \
			st3 =                                                \
			    CONST_BD * ((TYPE) p_src[(j2 +                   \
			    (exist_w | (dw ==                                \
			    2))) * chan + l] +                               \
			    (TYPE) p_src[-pred_index_h + (j2 +               \
			    (exist_w | (dw == 2))) * chan + l]);             \
			res =                                                \
			    (CONST_BC * (st1 + st2) - st0 -                  \
			    st3) NORMAL2;                                    \
			PDT = SATUR(res);                                    \
		    }                                                        \
		    bottom = 0;                                              \
		    left = 0;                                                \
		    right = 0;                                               \
		}                                                            \
	                                                                     \
/*                                                                           \
 * end addition                                                              \
 */                                                                          \
		else {                                                       \
		    if ((dw & 1) == 0) {                                     \
	                                                                     \
/*                                                                           \
 * dw == 0 or -2                                                             \
 */                                                                          \
			mlib_s32 pred_index_w = exist_w | (dw == 0);         \
	                                                                     \
			if (dh & 1) {                                        \
	                                                                     \
/*                                                                           \
 * dh == -3 or -1 y-case                                                     \
 */                                                                          \
			    TYPE a0 =                                        \
				p_src[-(exist_w + pred_index_w) * chan +     \
				l], a1 =                                     \
				p_src[-pred_index_w * chan + l];             \
			    for (j = 0, j2 = 0; j < (dst_width - 1);         \
				j++, j2 += 2) {                              \
				res =                                        \
				    (CONST_BC * ((TYPE) PST(0,               \
				    0) + a1) - a0 - PST(1, 0)) NORMAL;       \
				a0 = PST(0, 0);                              \
				a1 = PST(1, 0);                              \
				PDT = SATUR(res);                            \
			    }                                                \
			    res =                                            \
				(CONST_BC * ((TYPE) PST(0,                   \
				0) + a1) - a0 - (TYPE) p_src[(j2 +           \
				(exist_w | (dw ==                            \
				2))) * chan + l]) NORMAL;                    \
			    PDT = SATUR(res);                                \
			} else {                                             \
	                                                                     \
/*                                                                           \
 * dh == 0 or -2 xy-case                                                     \
 */                                                                          \
			    mlib_s32 pred_index_h =                          \
				src_stride * (dh == 0);                      \
			    TYPE st0 =                                       \
				CONST_BC * (TYPE) p_src[-(exist_w +          \
				pred_index_w) * chan + l] +                  \
				CONST_BD * (TYPE) p_src[-pred_index_h -      \
				(exist_w + pred_index_w) * chan + l] -       \
				(TYPE) p_src[src_stride - (exist_w +         \
				pred_index_w) * chan + l], st1 =             \
				CONST_BC * (TYPE) p_src[-pred_index_w *      \
				chan + l] +                                  \
				CONST_BD * (TYPE) p_src[-pred_index_h -      \
				pred_index_w * chan + l] -                   \
				(TYPE) p_src[src_stride -                    \
				pred_index_w * chan + l], st2, st3;          \
			    for (j = 0, j2 = 0; j < (dst_width - 1);         \
				j++, j2 += 2) {                              \
				st2 =                                        \
				    CONST_BC * (TYPE) PST(0,                 \
				    0) + CONST_BD * (TYPE) PST(0,            \
				    -pred_index_h) - (TYPE) PST(0,           \
				    src_stride);                             \
				st3 =                                        \
				    CONST_BC * (TYPE) PST(1,                 \
				    0) + CONST_BD * (TYPE) PST(1,            \
				    -pred_index_h) - (TYPE) PST(1,           \
				    src_stride);                             \
				res =                                        \
				    (CONST_BC * (st1 + st2) - st0 -          \
				    st3) NORMAL2;                            \
				st0 = st2;                                   \
				st1 = st3;                                   \
				PDT = SATUR(res);                            \
			    }                                                \
			    st2 =                                            \
				CONST_BC * (TYPE) PST(0,                     \
				0) + CONST_BD * (TYPE) PST(0,                \
				-pred_index_h) - (TYPE) PST(0,               \
				src_stride);                                 \
			    st3 =                                            \
				CONST_BC * (TYPE) p_src[(j2 +                \
				(exist_w | (dw ==                            \
				2))) * chan + l] +                           \
				CONST_BD * (TYPE) p_src[-pred_index_h +      \
				(j2 + (exist_w | (dw ==                      \
				2))) * chan + l] -                           \
				(TYPE) p_src[src_stride + (j2 +              \
				(exist_w | (dw == 2))) * chan + l];          \
			    res =                                            \
				(CONST_BC * (st1 + st2) - st0 -              \
				st3) NORMAL2;                                \
			    PDT = SATUR(res);                                \
			}                                                    \
		    } else {                                                 \
	                                                                     \
/*                                                                           \
 * dw == -3 or -1                                                            \
 */                                                                          \
			if (dh & 1) {                                        \
	                                                                     \
/*                                                                           \
 * dh == -3 or -1 nearest                                                    \
 */                                                                          \
			    for (j = 0, j2 = 0; j < dst_width;               \
				j++, j2 += 2)                                \
				PDT = PST(0, 0);                             \
			} else {                                             \
	                                                                     \
/*                                                                           \
 * dh == -2 or 0 x-case                                                      \
 */                                                                          \
			    mlib_s32 pred_index =                            \
				src_stride * (dh == 0);                      \
			    for (j = 0, j2 = 0; j < dst_width;               \
				j++, j2 += 2) {                              \
				res =                                        \
				    (CONST_BC * (TYPE) PST(0,                \
				    0) + CONST_BD * (TYPE) PST(0,            \
				    -pred_index) - (TYPE) PST(0,             \
				    src_stride)) NORMAL;                     \
				PDT = SATUR(res);                            \
			    }                                                \
			}                                                    \
		    }                                                        \
		}                                                            \
	    }                                                                \
	                                                                     \
/*                                                                           \
 * bottom edge fill                                                          \
 */                                                                          \
	    if (bottom) {                                                    \
		if ((dw & 1) == 0) {                                         \
	                                                                     \
/*                                                                           \
 * dw == 0 or -2                                                             \
 */                                                                          \
		    mlib_s32 pred_index_w = exist_w | (dw == 0);             \
	                                                                     \
		    if (dh & 1) {                                            \
	                                                                     \
/*                                                                           \
 * dh == -1 or -3                                                            \
 */                                                                          \
			TYPE a0 =                                            \
			    p_src[2 * (dst_height - 1) * src_stride -        \
			    (exist_w + pred_index_w) * chan + l], a1 =       \
			    p_src[2 * (dst_height - 1) * src_stride -        \
			    pred_index_w * chan + l];                        \
			for (j = 0; j < (dst_width - 1); j++) {              \
			    res =                                            \
				(CONST_BC * ((TYPE) PSB(0,                   \
				0) + a1) - a0 - PSB(1, 0)) NORMAL;           \
			    a0 = PSB(0, 0);                                  \
			    a1 = PSB(1, 0);                                  \
			    PDB = SATUR(res);                                \
			}                                                    \
			res =                                                \
			    (CONST_BC * ((TYPE) PSB(0,                       \
			    0) + a1) - a0 -                                  \
			    (TYPE) p_src[2 * (dst_height -                   \
			    1) * src_stride + (2 * j + (exist_w | (dw ==     \
			    2))) * chan + l]) NORMAL;                        \
			PDB = SATUR(res);                                    \
		    } else {                                                 \
	                                                                     \
/*                                                                           \
 * dh == 0 or -2                                                             \
 */                                                                          \
			TYPE st0 =                                           \
			    CONST_BD * (TYPE) p_src[2 * (dst_height -        \
			    1) * src_stride - (exist_w +                     \
			    pred_index_w) * chan + l] +                      \
			    CONST_BC * (TYPE) p_src[(2 * (dst_height -       \
			    1) - 1) * src_stride - (exist_w +                \
			    pred_index_w) * chan + l] -                      \
			    (TYPE) p_src[(2 * (dst_height - 1) -             \
			    2) * src_stride - (exist_w +                     \
			    pred_index_w) * chan + l], st1 =                 \
			    CONST_BD * (TYPE) p_src[2 * (dst_height -        \
			    1) * src_stride - pred_index_w * chan + l] +     \
			    CONST_BC * (TYPE) p_src[(2 * (dst_height -       \
			    1) - 1) * src_stride - pred_index_w * chan +     \
			    l] - (TYPE) p_src[(2 * (dst_height - 1) -        \
			    2) * src_stride - pred_index_w * chan + l],      \
			    st2, st3;                                        \
			for (j = 0; j < (dst_width - 1); j++) {              \
			    st2 =                                            \
				CONST_BD * (TYPE) PSB(0,                     \
				0) + CONST_BC * (TYPE) PSB(0,                \
				-1) - (TYPE) PSB(0, -2);                     \
			    st3 =                                            \
				CONST_BD * (TYPE) PSB(1,                     \
				0) + CONST_BC * (TYPE) PSB(1,                \
				-1) - (TYPE) PSB(1, -2);                     \
			    res =                                            \
				(CONST_BC * (st1 + st2) - st0 -              \
				st3) NORMAL2;                                \
			    st0 = st2;                                       \
			    st1 = st3;                                       \
			    PDB = SATUR(res);                                \
			}                                                    \
			st2 =                                                \
			    CONST_BD * (TYPE) PSB(0,                         \
			    0) + CONST_BC * (TYPE) PSB(0,                    \
			    -1) - (TYPE) PSB(0, -2);                         \
			st3 =                                                \
			    CONST_BD * (TYPE) p_src[2 * (dst_height -        \
			    1) * src_stride + (2 * j + (exist_w | (dw ==     \
			    2))) * chan + l] +                               \
			    CONST_BC * (TYPE) p_src[(2 * (dst_height -       \
			    1) - 1) * src_stride + (2 * j +                  \
			    (exist_w | (dw ==                                \
			    2))) * chan + l] -                               \
			    (TYPE) p_src[(2 * (dst_height - 1) -             \
			    2) * src_stride + (2 * j + (exist_w | (dw ==     \
			    2))) * chan + l];                                \
			res =                                                \
			    (CONST_BC * (st1 + st2) - st0 -                  \
			    st3) NORMAL2;                                    \
			PDB = SATUR(res);                                    \
		    }                                                        \
		} else {                                                     \
	                                                                     \
/*                                                                           \
 * dw == -3 or -1                                                            \
 */                                                                          \
		    if (dh & 1) {                                            \
	                                                                     \
/*                                                                           \
 * dh == -3 or -1                                                            \
 */                                                                          \
			for (j = 0; j < dst_width; j++)                      \
			    PDB = PSB(0, 0);                                 \
		    } else {                                                 \
			for (j = 0; j < dst_width; j++) {                    \
			    res =                                            \
				(CONST_BD * (TYPE) PSB(0,                    \
				0) + CONST_BC * (TYPE) PSB(0,                \
				-1) - (TYPE) PSB(0, -2)) NORMAL;             \
			    PDB = SATUR(res);                                \
			}                                                    \
		    }                                                        \
		}                                                            \
	    }                                                                \
	                                                                     \
/*                                                                           \
 * left edge fill                                                            \
 */                                                                          \
	    if (left) {                                                      \
		if (dst_width == 1 && dw == 0) {                             \
	                                                                     \
/*                                                                           \
 * 1-3 size addition                                                         \
 */                                                                          \
		    mlib_s32 pred_index = chan;                              \
	                                                                     \
		    if (dh & 1) {                                            \
	                                                                     \
/*                                                                           \
 * dh == -1 or -3 x case                                                     \
 */                                                                          \
			for (i = top; i < (dst_height - bottom); i++) {      \
			    res =                                            \
				(CONST_BD * ((TYPE) PSL(i,                   \
				-pred_index) + (TYPE) PSL(i,                 \
				+0))) NORMAL;                                \
			    PDL = SATUR(res);                                \
			}                                                    \
		    } else {                                                 \
	                                                                     \
/*                                                                           \
 * dh == 0 or -2 xy case                                                     \
 */                                                                          \
			TYPE st0 =                                           \
			    CONST_BD * ((TYPE) PSL(top - 2,                  \
			    -pred_index) + (TYPE) PSL(top - 2, +0)),         \
			    st1 =                                            \
			    CONST_BD * ((TYPE) PSL(top - 1,                  \
			    -pred_index) + (TYPE) PSL(top - 1, +0)),         \
			    st2, st3;                                        \
			for (i = top; i < (dst_height - bottom); i++) {      \
			    st2 =                                            \
				CONST_BD * ((TYPE) PSL(i,                    \
				-pred_index) + (TYPE) PSL(i, +0));           \
			    st3 =                                            \
				CONST_BD * ((TYPE) PSL(i + 1,                \
				-pred_index) + (TYPE) PSL(i + 1, +0));       \
			    res =                                            \
				(CONST_BC * (st1 + st2) - st0 -              \
				st3) NORMAL2;                                \
			    st0 = st2;                                       \
			    st1 = st3;                                       \
			    PDL = SATUR(res);                                \
			}                                                    \
		    }                                                        \
		    right = 0;                                               \
/*                                                                           \
 * end addition                                                              \
 */                                                                          \
		} else {                                                     \
		    if ((dw & 1) == 0) {                                     \
	                                                                     \
/*                                                                           \
 * dw == 0 or -2                                                             \
 */                                                                          \
			mlib_s32 pred_index = chan * (dw == 0);              \
	                                                                     \
			if (dh & 1) {                                        \
	                                                                     \
/*                                                                           \
 * dh == -1 or -3 x case                                                     \
 */                                                                          \
			    for (i = top; i < (dst_height - bottom);         \
				i++) {                                       \
				res =                                        \
				    (CONST_BD * (TYPE) PSL(i,                \
				    -pred_index) +                           \
				    CONST_BC * (TYPE) PSL(i,                 \
				    +0) - (TYPE) PSL(i, +chan)) NORMAL;      \
				PDL = SATUR(res);                            \
			    }                                                \
			} else {                                             \
	                                                                     \
/*                                                                           \
 * dh == 0 or -2 xy case                                                     \
 */                                                                          \
			    TYPE st0 =                                       \
				CONST_BD * (TYPE) PSL(top - 2,               \
				-pred_index) +                               \
				CONST_BC * (TYPE) PSL(top - 2,               \
				+0) - (TYPE) PSL(top - 2, +chan), st1 =      \
				CONST_BD * (TYPE) PSL(top - 1,               \
				-pred_index) +                               \
				CONST_BC * (TYPE) PSL(top - 1,               \
				+0) - (TYPE) PSL(top - 1, +chan), st2,       \
				st3;                                         \
			    for (i = top; i < (dst_height - bottom);         \
				i++) {                                       \
				st2 =                                        \
				    CONST_BD * (TYPE) PSL(i,                 \
				    -pred_index) +                           \
				    CONST_BC * (TYPE) PSL(i,                 \
				    +0) - (TYPE) PSL(i, +chan);              \
				st3 =                                        \
				    CONST_BD * (TYPE) PSL(i + 1,             \
				    -pred_index) +                           \
				    CONST_BC * (TYPE) PSL(i + 1,             \
				    +0) - (TYPE) PSL(i + 1, +chan);          \
				res =                                        \
				    (CONST_BC * (st1 + st2) - st0 -          \
				    st3) NORMAL2;                            \
				st0 = st2;                                   \
				st1 = st3;                                   \
				PDL = SATUR(res);                            \
			    }                                                \
			}                                                    \
		    } else {                                                 \
	                                                                     \
/*                                                                           \
 * dw == -1 or -3                                                            \
 */                                                                          \
			if (dh & 1) {                                        \
	                                                                     \
/*                                                                           \
 * dh == -1 or -3 nearest                                                    \
 */                                                                          \
			    for (i = top; i < (dst_height - bottom);         \
				i++) {                                       \
				PDL = PSL(i, +0);                            \
			    }                                                \
			} else {                                             \
	                                                                     \
/*                                                                           \
 * dh == 0 or -2 y case                                                      \
 */                                                                          \
			    for (i = top; i < (dst_height - bottom);         \
				i++) {                                       \
				res =                                        \
				    (CONST_BC * ((TYPE) PSL(i,               \
				    +0) + (TYPE) PSL(i - 1,                  \
				    +0)) - (TYPE) PSL(i - 2,                 \
				    +0) - (TYPE) PSL(i + 1, +0)) NORMAL;     \
				PDL = SATUR(res);                            \
			    }                                                \
			}                                                    \
		    }                                                        \
		}                                                            \
	    }                                                                \
	                                                                     \
/*                                                                           \
 * right edge fill                                                           \
 */                                                                          \
	    if (right) {                                                     \
		if ((dw & 1) == 0) {                                         \
	                                                                     \
/*                                                                           \
 * dw == 0 or -2                                                             \
 */                                                                          \
		    if (dh & 1) {                                            \
	                                                                     \
/*                                                                           \
 * dh == -1 or -3                                                            \
 */                                                                          \
			for (i = top; i < (dst_height - bottom); i++) {      \
			    res =                                            \
				(CONST_BC * (TYPE) PSR(i,                    \
				-1) + CONST_BD * (TYPE) PSR(i,               \
				+0) - (TYPE) PSR(i, -2)) NORMAL;             \
			    PDR = SATUR(res);                                \
			}                                                    \
		    } else {                                                 \
	                                                                     \
/*                                                                           \
 * dh == 0 or -2                                                             \
 */                                                                          \
			TYPE st0 =                                           \
			    CONST_BC * (TYPE) PSR(top - 2,                   \
			    -1) + CONST_BD * (TYPE) PSR(top - 2,             \
			    +0) - (TYPE) PSR(top - 2, -2), st1 =             \
			    CONST_BC * (TYPE) PSR(top - 1,                   \
			    -1) + CONST_BD * (TYPE) PSR(top - 1,             \
			    +0) - (TYPE) PSR(top - 1, -2), st2, st3;         \
			for (i = top; i < (dst_height - bottom); i++) {      \
			    st2 =                                            \
				CONST_BC * (TYPE) PSR(i,                     \
				-1) + CONST_BD * (TYPE) PSR(i,               \
				+0) - (TYPE) PSR(i, -2);                     \
			    st3 =                                            \
				CONST_BC * (TYPE) PSR(i + 1,                 \
				-1) + CONST_BD * (TYPE) PSR(i + 1,           \
				+0) - (TYPE) PSR(i + 1, -2);                 \
			    res =                                            \
				(CONST_BC * (st1 + st2) - st0 -              \
				st3) NORMAL2;                                \
			    st0 = st2;                                       \
			    st1 = st3;                                       \
			    PDR = SATUR(res);                                \
			}                                                    \
		    }                                                        \
		} else {                                                     \
	                                                                     \
/*                                                                           \
 * dw == -1 or -3                                                            \
 */                                                                          \
		    if (dh & 1) {                                            \
	                                                                     \
/*                                                                           \
 * dh == -1 or -3                                                            \
 */                                                                          \
			for (i = top; i < (dst_height - bottom); i++)        \
			    PDR = PSR(i, +0);                                \
		    } else {                                                 \
	                                                                     \
/*                                                                           \
 * dh == 0 or -2                                                             \
 */                                                                          \
			for (i = top; i < (dst_height - bottom); i++) {      \
			    res =                                            \
				(CONST_BC * ((TYPE) PSR(i,                   \
				+0) + (TYPE) PSR(i - 1,                      \
				+0)) - (TYPE) PSR(i - 2,                     \
				+0) - (TYPE) PSR(i + 1, +0)) NORMAL;         \
			    PDR = SATUR(res);                                \
			}                                                    \
		    }                                                        \
		}                                                            \
	    }                                                                \
	}

/* *********************************************************** */

/*			bilinear edges				*/

mlib_status
mlib_ImageZoomOut2X_F32_Bilinear_Edge(
    EDGE_PARAMETERS)
{
	EDGE_BILINEAR(mlib_f32);

	switch (edge) {
	case MLIB_EDGE_DST_FILL_ZERO:
		CASE_EDGE_FILL_ZERO(0.0f) break;
	case MLIB_EDGE_OP_NEAREST:
		CASE_EDGE_NEAREST break;
	case MLIB_EDGE_SRC_EXTEND:
		CASE_EDGE_BILINEAR(+0, *0.5f) break;
	default:
		return (MLIB_FAILURE);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_ImageZoomOut2X_D64_Bilinear_Edge(
    EDGE_PARAMETERS)
{
	EDGE_BILINEAR(mlib_d64);

	switch (edge) {
	case MLIB_EDGE_DST_FILL_ZERO:
		CASE_EDGE_FILL_ZERO(0.0f) break;
	case MLIB_EDGE_OP_NEAREST:
		CASE_EDGE_NEAREST break;
	case MLIB_EDGE_SRC_EXTEND:
		CASE_EDGE_BILINEAR(+0, *0.5f) break;
	default:
		return (MLIB_FAILURE);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

/*			bicubic edges				*/

/* *********************************************************** */

#define	BC_CF	9
#define	BC_CD	8

#define	BC_NRML	>>4

#define	BC_NRM2	>>8

/*  for s32 data only  */
#define	BC_SHIFT	4

#define	BC_SHIFT2	3

/* *********************************************************** */

/*	data type	*/
#define	D_TYPE	mlib_u8
/*  operate type  */
#define	O_TYPE	mlib_s32

#define	SATUR	SAT_U8

mlib_status
mlib_ImageZoomOut2X_U8_Bicubic_Edge(
    EDGE_PARAMETERS)
{
	O_TYPE res;

	MASK_U8_INIT EDGE_BICUBIC(D_TYPE);
	switch (edge) {
	case MLIB_EDGE_DST_NO_WRITE:
		break;
	case MLIB_EDGE_DST_FILL_ZERO:
		CASE_EDGE_FILL_ZERO(0) break;
	case MLIB_EDGE_OP_NEAREST:
		CASE_EDGE_NEAREST break;
	case MLIB_EDGE_SRC_EXTEND:
		CASE_EDGE_BICUBIC(SATUR, D_TYPE, BC_CF, BC_CD, BC_NRML, BC_NRM2,
		    O_TYPE);
		break;
	default:
		return (MLIB_FAILURE);
	}

	return (MLIB_SUCCESS);
}

#undef  D_TYPE
#undef  O_TYPE
#undef  SATUR

/* *********************************************************** */

#define	D_TYPE	mlib_s16
#define	O_TYPE	mlib_s32

#define	SATUR	SAT_S16

mlib_status
mlib_ImageZoomOut2X_S16_Bicubic_Edge(
    EDGE_PARAMETERS)
{
	O_TYPE res;

	MASK_S16_INIT;
	EDGE_BICUBIC(D_TYPE);
	switch (edge) {
	case MLIB_EDGE_DST_NO_WRITE:
		break;
	case MLIB_EDGE_DST_FILL_ZERO:
		CASE_EDGE_FILL_ZERO(0) break;
	case MLIB_EDGE_OP_NEAREST:
		CASE_EDGE_NEAREST break;
	case MLIB_EDGE_SRC_EXTEND:
		CASE_EDGE_BICUBIC(SATUR, D_TYPE, BC_CF, BC_CD, BC_NRML, BC_NRM2,
		    O_TYPE);
		break;
	default:
		return (MLIB_FAILURE);
	}

	return (MLIB_SUCCESS);
}

#undef  D_TYPE
#undef  O_TYPE
#undef  SATUR

/* *********************************************************** */

#define	D_TYPE	mlib_u16
#define	O_TYPE	mlib_s32

#define	SATUR	SAT_U16

mlib_status
mlib_ImageZoomOut2X_U16_Bicubic_Edge(
    EDGE_PARAMETERS)
{
	O_TYPE res;

	MASK_U16_INIT;
	EDGE_BICUBIC(D_TYPE);
	switch (edge) {
	case MLIB_EDGE_DST_NO_WRITE:
		break;
	case MLIB_EDGE_DST_FILL_ZERO:
		CASE_EDGE_FILL_ZERO(0) break;
	case MLIB_EDGE_OP_NEAREST:
		CASE_EDGE_NEAREST break;
	case MLIB_EDGE_SRC_EXTEND:
		CASE_EDGE_BICUBIC(SATUR, D_TYPE, BC_CF, BC_CD, BC_NRML, BC_NRM2,
		    O_TYPE);
		break;
	default:
		return (MLIB_FAILURE);
	}

	return (MLIB_SUCCESS);
}

#undef  D_TYPE
#undef  O_TYPE
#undef  SATUR

/* *********************************************************** */

#define	BC_FNRML	*0.0625
#define	BC_FNRM2	*0.00390625

#define	D_TYPE	mlib_s32

#define	O_TYPE	mlib_d64

#define	SATUR	SAT_S32

mlib_status
mlib_ImageZoomOut2X_S32_Bicubic_Edge(
    EDGE_PARAMETERS)
{
	O_TYPE res;

	EDGE_BICUBIC(D_TYPE);
	switch (edge) {
	case MLIB_EDGE_DST_NO_WRITE:
		break;
	case MLIB_EDGE_DST_FILL_ZERO:
		CASE_EDGE_FILL_ZERO(0) break;
	case MLIB_EDGE_OP_NEAREST:
		CASE_EDGE_NEAREST break;
	case MLIB_EDGE_SRC_EXTEND:
		CASE_EDGE_BICUBIC(SATUR, D_TYPE, BC_CF, BC_CD, BC_FNRML,
		    BC_FNRM2, O_TYPE);
		break;
	default:
		return (MLIB_FAILURE);
	}

	return (MLIB_SUCCESS);
}

#undef  D_TYPE
#undef  O_TYPE
#undef  SATUR
#undef  BC_FNRML
#undef  BC_FNRM2

/* *********************************************************** */

#undef  BC_CF
#undef  BC_CD
#undef  BC_NRML
#undef  BC_NRM2
#undef  BC_SHIFT
#undef  BC_SHIFT2

/* *********************************************************** */

#define	BC_FCF	9.0f
#define	BC_FCD	8.0f

#define	BC_FNRML	*0.0625f

#define	BC_FNRM2	*0.00390625f

#define	D_TYPE	mlib_f32

#define	O_TYPE	mlib_f32

#define	SATUR	SAT_F32

mlib_status
mlib_ImageZoomOut2X_F32_Bicubic_Edge(
    EDGE_PARAMETERS)
{
	O_TYPE res;

	EDGE_BICUBIC(D_TYPE);
	switch (edge) {
	case MLIB_EDGE_DST_FILL_ZERO:
		CASE_EDGE_FILL_ZERO(0.0f) break;
	case MLIB_EDGE_OP_NEAREST:
		CASE_EDGE_NEAREST break;
	case MLIB_EDGE_SRC_EXTEND:
		CASE_EDGE_BICUBIC(SATUR, D_TYPE, BC_FCF, BC_FCD, BC_FNRML,
		    BC_FNRM2, O_TYPE);
		break;
	default:
		return (MLIB_FAILURE);
	}

	return (MLIB_SUCCESS);
}

#undef  D_TYPE
#undef  O_TYPE
#undef  SATUR
#undef  BC_FCF
#undef  BC_FCD
#undef  BC_FNRML
#undef  BC_FNRM2

/* *********************************************************** */

#define	BC_FCF	9.0
#define	BC_FCD	8.0

#define	BC_FNRML	*0.0625

#define	BC_FNRM2	*0.00390625

#define	D_TYPE	mlib_d64

#define	O_TYPE	mlib_d64

#define	SATUR	SAT_D64

mlib_status
mlib_ImageZoomOut2X_D64_Bicubic_Edge(
    EDGE_PARAMETERS)
{
	O_TYPE res;

	EDGE_BICUBIC(D_TYPE);
	switch (edge) {
	case MLIB_EDGE_DST_FILL_ZERO:
		CASE_EDGE_FILL_ZERO(0.0) break;
	case MLIB_EDGE_OP_NEAREST:
		CASE_EDGE_NEAREST break;
	case MLIB_EDGE_SRC_EXTEND:
		CASE_EDGE_BICUBIC(SATUR, D_TYPE, BC_FCF, BC_FCD, BC_FNRML,
		    BC_FNRM2, O_TYPE);
		break;
	default:
		return (MLIB_FAILURE);
	}

	return (MLIB_SUCCESS);
}

#undef  D_TYPE
#undef  O_TYPE
#undef  SATUR
#undef  BC_FCF
#undef  BC_FCD
#undef  BC_FNRML
#undef  BC_FNRM2

/* *********************************************************** */

/*			bicubic2 edges				*/

/* *********************************************************** */

#define	BC_CF	5
#define	BC_CD	4

#define	BC_NRML	>>3

#define	BC_NRM2	>>6

/*  for s32 data  */
#define	BC_SHIFT	3

#define	BC_SHIFT2	2

/* *********************************************************** */

/*	data type	*/
#define	D_TYPE	mlib_u8
/*  operate type  */
#define	O_TYPE	mlib_s32

#define	SATUR	SAT_U8

mlib_status
mlib_ImageZoomOut2X_U8_Bicubic2_Edge(
    EDGE_PARAMETERS)
{
	O_TYPE res;

	MASK_U8_INIT EDGE_BICUBIC(D_TYPE);
	switch (edge) {
	case MLIB_EDGE_DST_NO_WRITE:
		break;
	case MLIB_EDGE_DST_FILL_ZERO:
		CASE_EDGE_FILL_ZERO(0) break;
	case MLIB_EDGE_OP_NEAREST:
		CASE_EDGE_NEAREST break;
	case MLIB_EDGE_SRC_EXTEND:
		CASE_EDGE_BICUBIC(SATUR, D_TYPE, BC_CF, BC_CD, BC_NRML, BC_NRM2,
		    O_TYPE);
		break;
	default:
		return (MLIB_FAILURE);
	}

	return (MLIB_SUCCESS);
}

#undef  D_TYPE
#undef  O_TYPE
#undef  SATUR

/* *********************************************************** */

#define	D_TYPE	mlib_s16
#define	O_TYPE	mlib_s32

#define	SATUR	SAT_S16

mlib_status
mlib_ImageZoomOut2X_S16_Bicubic2_Edge(
    EDGE_PARAMETERS)
{
	O_TYPE res;

	MASK_S16_INIT;
	EDGE_BICUBIC(D_TYPE);
	switch (edge) {
	case MLIB_EDGE_DST_NO_WRITE:
		break;
	case MLIB_EDGE_DST_FILL_ZERO:
		CASE_EDGE_FILL_ZERO(0) break;
	case MLIB_EDGE_OP_NEAREST:
		CASE_EDGE_NEAREST break;
	case MLIB_EDGE_SRC_EXTEND:
		CASE_EDGE_BICUBIC(SATUR, D_TYPE, BC_CF, BC_CD, BC_NRML, BC_NRM2,
		    O_TYPE);
		break;
	default:
		return (MLIB_FAILURE);
	}

	return (MLIB_SUCCESS);
}

#undef  D_TYPE
#undef  O_TYPE
#undef  SATUR

/* *********************************************************** */

#define	D_TYPE	mlib_u16
#define	O_TYPE	mlib_s32

#define	SATUR	SAT_U16

mlib_status
mlib_ImageZoomOut2X_U16_Bicubic2_Edge(
    EDGE_PARAMETERS)
{
	O_TYPE res;

	MASK_U16_INIT;
	EDGE_BICUBIC(D_TYPE);
	switch (edge) {
	case MLIB_EDGE_DST_NO_WRITE:
		break;
	case MLIB_EDGE_DST_FILL_ZERO:
		CASE_EDGE_FILL_ZERO(0) break;
	case MLIB_EDGE_OP_NEAREST:
		CASE_EDGE_NEAREST break;
	case MLIB_EDGE_SRC_EXTEND:
		CASE_EDGE_BICUBIC(SATUR, D_TYPE, BC_CF, BC_CD, BC_NRML, BC_NRM2,
		    O_TYPE);
		break;
	default:
		return (MLIB_FAILURE);
	}

	return (MLIB_SUCCESS);
}

#undef  D_TYPE
#undef  O_TYPE
#undef  SATUR

/* *********************************************************** */

#define	BC_FNRML	*0.125
#define	BC_FNRM2	*0.015625

#define	D_TYPE	mlib_s32

#define	O_TYPE	mlib_d64

#define	SATUR	SAT_S32

mlib_status
mlib_ImageZoomOut2X_S32_Bicubic2_Edge(
    EDGE_PARAMETERS)
{
	O_TYPE res;

	EDGE_BICUBIC(D_TYPE);
	switch (edge) {
	case MLIB_EDGE_DST_NO_WRITE:
		break;
	case MLIB_EDGE_DST_FILL_ZERO:
		CASE_EDGE_FILL_ZERO(0) break;
	case MLIB_EDGE_OP_NEAREST:
		CASE_EDGE_NEAREST break;
	case MLIB_EDGE_SRC_EXTEND:
		CASE_EDGE_BICUBIC(SATUR, D_TYPE, BC_CF, BC_CD, BC_FNRML,
		    BC_FNRM2, O_TYPE);
		break;
	default:
		return (MLIB_FAILURE);
	}

	return (MLIB_SUCCESS);
}

#undef  D_TYPE
#undef  O_TYPE
#undef  SATUR
#undef  BC_FNRML
#undef  BC_FNRM2

/* *********************************************************** */

#undef  BC_CF
#undef  BC_CD
#undef  BC_NRML
#undef  BC_NRM2
#undef  BC_SHIFT
#undef  BC_SHIFT2

/* *********************************************************** */

#define	BC_FCF	5.0f
#define	BC_FCD	4.0f

#define	BC_FNRML	*0.125f

#define	BC_FNRM2	*0.015625f

#define	D_TYPE	mlib_f32

#define	O_TYPE	mlib_f32

#define	SATUR	SAT_F32

mlib_status
mlib_ImageZoomOut2X_F32_Bicubic2_Edge(
    EDGE_PARAMETERS)
{
	O_TYPE res;

	EDGE_BICUBIC(D_TYPE);
	switch (edge) {
	case MLIB_EDGE_DST_FILL_ZERO:
		CASE_EDGE_FILL_ZERO(0.0f) break;
	case MLIB_EDGE_OP_NEAREST:
		CASE_EDGE_NEAREST break;
	case MLIB_EDGE_SRC_EXTEND:
		CASE_EDGE_BICUBIC(SATUR, D_TYPE, BC_FCF, BC_FCD, BC_FNRML,
		    BC_FNRM2, O_TYPE);
		break;
	default:
		return (MLIB_FAILURE);
	}

	return (MLIB_SUCCESS);
}

#undef  D_TYPE
#undef  O_TYPE
#undef  SATUR
#undef  BC_FCF
#undef  BC_FCD
#undef  BC_FNRML
#undef  BC_FNRM2

/* *********************************************************** */

#define	BC_FCF	5.0
#define	BC_FCD	4.0

#define	BC_FNRML	*0.125

#define	BC_FNRM2	*0.015625

#define	D_TYPE	mlib_d64

#define	O_TYPE	mlib_d64

#define	SATUR	SAT_D64

mlib_status
mlib_ImageZoomOut2X_D64_Bicubic2_Edge(
    EDGE_PARAMETERS)
{
	O_TYPE res;

	EDGE_BICUBIC(D_TYPE);
	switch (edge) {
	case MLIB_EDGE_DST_FILL_ZERO:
		CASE_EDGE_FILL_ZERO(0.0) break;
	case MLIB_EDGE_OP_NEAREST:
		CASE_EDGE_NEAREST break;
	case MLIB_EDGE_SRC_EXTEND:
		CASE_EDGE_BICUBIC(SATUR, D_TYPE, BC_FCF, BC_FCD, BC_FNRML,
		    BC_FNRM2, O_TYPE);
		break;
	default:
		return (MLIB_FAILURE);
	}

	return (MLIB_SUCCESS);
}

#undef  D_TYPE
#undef  O_TYPE
#undef  SATUR
#undef  BC_FCF
#undef  BC_FCD
#undef  BC_FNRML
#undef  BC_FNRM2

/* *********************************************************** */

#undef  EDGE_BILINEAR

/* *********************************************************** */

#define	EDGE_BILINEAR                                           \
	src_h_beg = src_height - 2 * height;                    \
	dh = src_h_beg & 1;                                     \
	if (src_h_beg <= 0) {                                   \
	    mlib_s32 dst_h_beg = (1 - src_h_beg) >> 2;          \
	                                                        \
	    dp += dst_h_beg * dlb;                              \
	    height -= 2 * dst_h_beg;                            \
	    src_h_beg &= 3;                                     \
	    height -= (top = (src_h_beg == 2));                 \
	    bottom = (src_h_beg == 3);                          \
	    src_h_beg = (src_h_beg < 2);                        \
	} else                                                  \
	    src_h_beg = (src_h_beg + 2) >> 1;                   \
	src_w_beg = src_width - 2 * width;                      \
	dw = src_w_beg & 1;                                     \
	if (src_w_beg <= 0) {                                   \
	    mlib_s32 dst_w_beg = (1 - src_w_beg) >> 2;          \
	                                                        \
	    dp += dst_w_beg * chan;                             \
	    width -= 2 * dst_w_beg;                             \
	    src_w_beg &= 3;                                     \
	    width -= (left = (src_w_beg == 2));                 \
	    right = (src_w_beg == 3);                           \
	    src_w_beg = (src_w_beg < 2);                        \
	} else                                                  \
	    src_w_beg = (src_w_beg + 2) >> 1;                   \
	sp += src_h_beg * slb + src_w_beg * chan

/* *********************************************************** */

#define	FUNC_BL_EDGE(STYPE, TT)                                            \
	mlib_status                                                        \
	mlib_ImageZoomOut2X_##TT##_Bilinear_Edge(const                     \
	    mlib_image *src, mlib_image *dst, mlib_edge edge)              \
	{                                                                  \
	    STYPE *sp = (STYPE *) mlib_ImageGetData(src);                  \
	    STYPE *dp = (STYPE *) mlib_ImageGetData(dst);                  \
	    mlib_s32 slb = mlib_ImageGetStride(src) / sizeof (STYPE);      \
	    mlib_s32 dlb = mlib_ImageGetStride(dst) / sizeof (STYPE);      \
	    mlib_s32 width = mlib_ImageGetWidth(dst);                      \
	    mlib_s32 height = mlib_ImageGetHeight(dst);                    \
	    mlib_s32 chan = mlib_ImageGetChannels(src);                    \
	    mlib_s32 src_width = mlib_ImageGetWidth(src);                  \
	    mlib_s32 src_height = mlib_ImageGetHeight(src);                \
	    mlib_s32 i, j, l, dh, dw, src_w_beg, src_h_beg;                \
	    mlib_s32 left = 0, right = 0, top = 0, bottom = 0;             \
	                                                                   \
	    if (edge == MLIB_EDGE_DST_NO_WRITE)                            \
		return (MLIB_SUCCESS);                                     \
	    EDGE_BILINEAR;                                                 \
	    switch (edge) {                                                \
	    case MLIB_EDGE_DST_FILL_ZERO:                                  \
		if (top) {                                                 \
		    for (j = 0; j < width * chan; j++)                     \
			dp[j] = 0;                                         \
		}                                                          \
		if (bottom) {                                              \
		    for (j = 0; j < width * chan; j++)                     \
			dp[(height - 1) * dlb + j] = 0;                    \
		}                                                          \
		for (l = 0; l < chan; l++) {                               \
		    if (left) {                                            \
			for (i = 0; i < height; i++)                       \
			    dp[i * dlb + l] = 0;                           \
		    }                                                      \
		    if (right) {                                           \
			for (i = 0; i < height; i++)                       \
			    dp[i * dlb + (width - 1) * chan + l] = 0;      \
		    }                                                      \
		}                                                          \
		break;                                                     \
	    case MLIB_EDGE_OP_NEAREST:                                     \
		for (l = 0; l < chan; l++) {                               \
		    if (top) {                                             \
			for (j = 0; j < width; j++) {                      \
			    dp[j * chan + l] = sp[2 * j * chan + l];       \
			}                                                  \
		    }                                                      \
		    if (bottom) {                                          \
			for (j = 0; j < width; j++) {                      \
			    dp[(height - 1) * dlb + j * chan + l] =        \
				sp[2 * (height - 1) * slb +                \
				2 * j * chan + l];                         \
			}                                                  \
		    }                                                      \
		    if (left) {                                            \
			for (i = 0; i < height; i++) {                     \
			    dp[i * dlb + l] = sp[2 * i * slb + l];         \
			}                                                  \
		    }                                                      \
		    if (right) {                                           \
			for (i = 0; i < height; i++) {                     \
			    dp[i * dlb + (width - 1) * chan + l] =         \
				sp[2 * i * slb + 2 * (width -              \
				1) * chan + l];                            \
			}                                                  \
		    }                                                      \
		}                                                          \
		break;                                                     \
	    case MLIB_EDGE_SRC_EXTEND:                                     \
		for (l = 0; l < chan; l++) {                               \
		    if (top) {                                             \
			if (dw) {                                          \
			    for (j = 0; j < width; j++)                    \
				dp[j * chan + l] = sp[2 * j * chan + l];   \
			} else {                                           \
			    if (left)                                      \
				dp[l] = sp[l];                             \
			    for (j = left; j < (width - right); j++) {     \
				dp[j * chan + l] =                         \
				    AVE2(sp[(2 * j - 1) * chan + l],       \
				    sp[2 * j * chan + l]);                 \
			    }                                              \
			}                                                  \
		    }                                                      \
		    if (bottom) {                                          \
			if (dw) {                                          \
			    for (j = 0; j < width; j++) {                  \
				dp[(height - 1) * dlb + j * chan + l] =    \
				    sp[2 * (height - 1) * slb +            \
				    2 * j * chan + l];                     \
			    }                                              \
			} else {                                           \
			    if (left)                                      \
				dp[(height - 1) * dlb + l] =               \
				    sp[2 * (height - 1) * slb + l];        \
			    for (j = left; j < (width - right); j++) {     \
				dp[(height - 1) * dlb + j * chan + l] =    \
				    AVE2(sp[2 * (height - 1) * slb +       \
				    (2 * j - 1) * chan + l],               \
				    sp[2 * (height - 1) * slb +            \
				    2 * j * chan + l]);                    \
			    }                                              \
			}                                                  \
		    }                                                      \
		    if (left) {                                            \
			if (dh) {                                          \
			    for (i = 0; i < height; i++)                   \
				dp[i * dlb + l] = sp[2 * i * slb + l];     \
			} else {                                           \
			    for (i = top; i < (height - bottom); i++) {    \
				dp[i * dlb + l] =                          \
				    AVE2(sp[(2 * i - 1) * slb + l],        \
				    sp[2 * i * slb + l]);                  \
			    }                                              \
			}                                                  \
		    }                                                      \
		    if (right) {                                           \
			if (dh) {                                          \
			    for (i = 0; i < height; i++) {                 \
				dp[i * dlb + (width - 1) * chan + l] =     \
				    sp[2 * i * slb + 2 * (width -          \
				    1) * chan + l];                        \
			    }                                              \
			} else {                                           \
			    for (i = top; i < (height - bottom); i++) {    \
				dp[i * dlb + (width - 1) * chan + l] =     \
				    AVE2(sp[(2 * i - 1) * slb +            \
				    2 * (width - 1) * chan + l],           \
				    sp[2 * i * slb + 2 * (width -          \
				    1) * chan + l]);                       \
			    }                                              \
			}                                                  \
		    }                                                      \
		}                                                          \
		break;                                                     \
	    default:                                                       \
		return (MLIB_FAILURE);                                     \
	    }                                                              \
	    return (MLIB_SUCCESS);                                         \
	}

/* *********************************************************** */

#define	AVE2(a, b)	(((a) + (b)) >> 1)

FUNC_BL_EDGE(mlib_u8, U8)
    FUNC_BL_EDGE(mlib_s16,
    S16)
FUNC_BL_EDGE(
    mlib_u16,
    U16)
#undef AVE2
/* *********************************************************** */
#ifdef _NO_LONGLONG
#define	AVE2(a, b)	(((a) >> 1) + ((b) >> 1))
#else		   /* _NO_LONGLONG */
#define	AVE2(a, b)	(((mlib_s64)(a) + (b)) >> 1)
#endif		   /* _NO_LONGLONG */
FUNC_BL_EDGE(
    mlib_s32,
    S32)

/* *********************************************************** */
