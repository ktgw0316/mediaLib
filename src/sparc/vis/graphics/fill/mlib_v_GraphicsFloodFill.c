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

#pragma ident	"@(#)mlib_v_GraphicsFloodFill.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_GraphicsFloodFill_[8 | 32] - fills region of image
 *
 * SYNOPSIS
 *      mlib_status mlib_GraphicsFloodFill_8 (mlib_image *buffer,
 *                                            mlib_s16   x,
 *                                            mlib_s16   y,
 *                                            mlib_s32   c,
 *                                            mlib_s32   c2)
 *
 *      mlib_status mlib_GraphicsFloodFill_32(mlib_image *buffer,
 *                                            mlib_s16   x,
 *                                            mlib_s16   y,
 *                                            mlib_s32   c,
 *                                            mlib_s32   c2)
 *
 * ARGUMENTS
 *      buffer  pointer to an dst image
 *      x, y     start point coords
 *      c       fill color
 *      c2      interior color
 *
 * DESCRIPTION
 *      Function fills region of color c2 in image pointed by buffer
 *      with color c starting from point x, y.
 *
 */

#include <vis_proto.h>
#include <mlib_graphics.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_GraphicsFloodFill_32 = __mlib_GraphicsFloodFill_32
#pragma weak mlib_GraphicsFloodFill_8 = __mlib_GraphicsFloodFill_8

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_GraphicsFloodFill_32) mlib_GraphicsFloodFill_32
    __attribute__((weak, alias("__mlib_GraphicsFloodFill_32")));
__typeof__(__mlib_GraphicsFloodFill_8) mlib_GraphicsFloodFill_8
    __attribute__((weak, alias("__mlib_GraphicsFloodFill_8")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

typedef struct tag_span
{
	mlib_u16 line;
	mlib_u16 left;
	mlib_s32 right;
/*  parent span  */
	mlib_u16 pline;
	mlib_u16 pleft;
	mlib_s32 pright;
} span;

/* *********************************************************** */

typedef struct tag_stub
{
	mlib_u16 line;
	mlib_u16 left;
	mlib_s32 right;
} stub;

/* *********************************************************** */

#define	SPANSHIFT	10
#define	SPANSTACKSIZE	(1 << (SPANSHIFT))
#define	SPANMASK	(SPANSTACKSIZE - 1)

/* *********************************************************** */

#define	ADDSPAN(sy, sl, sr, py, pl, pr)                                   \
	{                                                                 \
	    mlib_u32 cmpval = ((sy) << 16) | (sl);                        \
	    mlib_u32 parval = ((py) << 16) | (pl);                        \
	                                                                  \
	    if (stacktop >= SPANSTACKSIZE * (1 + nexspan)) {              \
		spanp =                                                   \
		    (span *) __mlib_malloc(SPANSTACKSIZE *                \
		    sizeof (span));                                       \
		if (!spanp)                                               \
		    return (MLIB_FAILURE);                                \
		pexspan[++nexspan] = spanp;                               \
	    }                                                             \
	    spanp =                                                       \
		pexspan[stacktop >> SPANSHIFT] + (stacktop & SPANMASK);   \
	    stacktop++;                                                   \
	    *(mlib_u32 *)spanp = cmpval;                                  \
	    spanp->right = sr;                                            \
	    *(mlib_u32 *)&spanp->pline = parval;                          \
	    spanp->pright = pr;                                           \
	}

/* *********************************************************** */

#define	GETSPAN                                                           \
	-- stacktop;                                                      \
	if (stacktop < 0)                                                 \
	    break;                                                        \
	spanp = pexspan[stacktop >> SPANSHIFT] + (stacktop & SPANMASK);   \
	cline = spanp->line;                                              \
	curl = spanp->left;                                               \
	curr = spanp->right;                                              \
	parline = spanp->pline;                                           \
	parl = spanp->pleft;                                              \
	parr = spanp->pright

/* *********************************************************** */

#define	CHECKBACK                                                     \
	if (parl - 1 > curl) {                                        \
	    for (spr = parl - 1; spr >= curl; ) {                     \
		for (; spr >= curl && sline[spr] != c2; )             \
		    spr--;                                            \
		if (spr < curl)                                       \
		    break;                                            \
/*                                                                    \
 * no more spans to left                                              \
 */                                                                   \
		spl = spr;                                            \
		for (; spl >= 0 && sline[spl] == c2; )                \
		    sline[spl--] = c;                                 \
		ADDSPAN(parline, (spl + 1), (spr + 1), cline, curl,   \
		    curr);                                            \
		spr = spl - 1;                                        \
	    }                                                         \
	}                                                             \
	if (parr + 1 < curr) {                                        \
	    for (spl = parr + 1; spl < curr; ) {                      \
		for (; spl < curr && sline[spl] != c2; )              \
		    spl++;                                            \
		if (spl >= curr)                                      \
		    break;                                            \
/*                                                                    \
 * no more spans to right                                             \
 */                                                                   \
		spr = spl;                                            \
		for (; spr < width && sline[spr] == c2; )             \
		    sline[spr++] = c;                                 \
		ADDSPAN(parline, spl, spr, cline, curl, curr);        \
		spl = spr + 1;                                        \
	    }                                                         \
	}

/* *********************************************************** */

#define	COND1	((mlib_addr)(sline + spr) & 7) && sline[spr] == c2

#define	CHECKFORW8                                                     \
	if (cline + cline - parline >= 0 &&                            \
	    cline + cline - parline < height) {                        \
	    spl = curl;                                                \
/*                                                                     \
 * lookup for the left border of span                                  \
 */                                                                    \
	    if (sline[spl] == c2) {                                    \
		spr = spl;                                             \
		for (; spl > 0 && sline[spl - 1] == c2; )              \
		    sline[--spl] = c;                                  \
	    } else {                                                   \
		for (; spl < curr && sline[spl] != c2; )               \
		    spl++;                                             \
		spr = spl;                                             \
	    }                                                          \
	                                                               \
/*                                                                     \
 * lookup for the spans                                                \
 */                                                                    \
	    for (; spr < curr; ) {                                     \
		if (width - spr < 8)                                   \
		    for (; spr < width && sline[spr] == c2; )          \
			sline[spr++] = c;                              \
		else {                                                 \
		    for (; COND1; )                                    \
			sline[spr++] = c;                              \
		    if (0 == ((mlib_addr)(sline + spr) & 7)) {         \
			mlib_d64 dtmp = *(mlib_d64 *)(sline + spr);    \
	                                                               \
			for (;                                         \
			    ((vis_fcmpeq32(dtmp,                       \
			    d8_c2) - 3) | (width - 9 - spr)) >= 0;     \
			    spr += 8) {                                \
			    dtmp = *(mlib_d64 *)(sline + (spr + 8));   \
			    *(mlib_d64 *)(sline + spr) = d8_c;         \
			}                                              \
			for (; spr < width && sline[spr] == c2; )      \
			    sline[spr++] = c;                          \
		    }                                                  \
		}                                                      \
		ADDSPAN((cline + cline - parline), spl, spr, cline,    \
		    curl, curr);                                       \
		for (; spr < curr && sline[spr] != c2; )               \
		    spr++;                                             \
		spl = spr;                                             \
	    }                                                          \
	}

/* *********************************************************** */

#define	CHECKFORW32                                                    \
	if (cline + cline - parline >= 0 &&                            \
	    cline + cline - parline < height) {                        \
	    spl = curl;                                                \
/*                                                                     \
 * lookup for the left border of span                                  \
 */                                                                    \
	    if (sline[spl] == c2) {                                    \
		spr = spl;                                             \
		for (; spl > 0 && sline[spl - 1] == c2; )              \
		    sline[--spl] = c;                                  \
	    } else {                                                   \
		for (; spl < curr && sline[spl] != c2; )               \
		    spl++;                                             \
		spr = spl;                                             \
	    }                                                          \
	                                                               \
/*                                                                     \
 * lookup for the spans                                                \
 */                                                                    \
	    for (; spr < curr; ) {                                     \
		if (width - spr < 3) {                                 \
		    for (; spr < width && sline[spr] == c2; )          \
			sline[spr++] = c;                              \
		} else {                                               \
		    if (((mlib_addr)(sline + spr) & 7) &&              \
			sline[spr] == c2)                              \
			sline[spr++] = c;                              \
		    if (0 == ((mlib_addr)(sline + spr) & 7)) {         \
			mlib_d64 dtmp = *(mlib_d64 *)(sline + spr);    \
	                                                               \
			for (;                                         \
			    ((vis_fcmpeq32(dtmp,                       \
			    dc2) - 3) | (width - 3 - spr)) >= 0;       \
			    spr += 2) {                                \
			    dtmp = *(mlib_d64 *)(sline + (spr + 2));   \
			    *(mlib_d64 *)(sline + spr) = dc;           \
			}                                              \
			if (spr < width && sline[spr] == c2) {         \
			    sline[spr++] = c;                          \
			    if (spr < width && sline[spr] == c2)       \
				sline[spr++] = c;                      \
			}                                              \
		    }                                                  \
		}                                                      \
		ADDSPAN((cline + cline - parline), spl, spr, cline,    \
		    curl, curr);                                       \
		for (; spr < curr && sline[spr] != c2; )               \
		    spr++;                                             \
		spl = spr;                                             \
	    }                                                          \
	}

/* *********************************************************** */

#define	imtype	mlib_u8
#define	CHECKFORW	CHECKFORW8

/* *********************************************************** */

mlib_status
__mlib_GraphicsFloodFill_8(
    mlib_image *buffer,
    mlib_s16 x,
    mlib_s16 y,
    mlib_s32 c,
    mlib_s32 c2)
{
	mlib_s32 stride = mlib_ImageGetStride(buffer) / sizeof (imtype);
	mlib_s32 width = mlib_ImageGetWidth(buffer);
	mlib_s32 height = mlib_ImageGetHeight(buffer);
	imtype *data = mlib_ImageGetData(buffer);
	imtype *line = data + y * stride, *sline;
	mlib_s32 cline, curl, curr, spl, spr;
	mlib_s32 parline, parl, parr;

/* stack counters */
	mlib_s32 stacktop = 0;

/* number of additional mem blocks */
	mlib_s32 nexspan = 0;
	span *spanp;

	mlib_d64 d8_c =
	    vis_to_double_dup((c | (c << 8)) | ((c | (c << 8)) << 16));
	mlib_d64 d8_c2 =
	    vis_to_double_dup((c2 | (c2 << 8)) | ((c2 | (c2 << 8)) << 16));

/* ptrs to span stack blocks */
	span *pexspan[256];

/* 1st block of span stack */
	span spanstack[SPANSTACKSIZE];

	if (!data)
		return (MLIB_NULLPOINTER);

	if (x < 0 || x >= width || y < 0 || y >= height || stride < width)
		return (MLIB_FAILURE);

	if (line[x] != c2)
		return (MLIB_SUCCESS);

	pexspan[0] = spanstack;
	curl = curr = x;

/*  fill first with finding borders  */
	while (curl > 0 && line[curl - 1] == c2) {
		--curl;
		line[curl] = c;
	}

	while (curr < width && line[curr] == c2) {
		line[curr] = c;
		++curr;
	}

	if (height == 1)
		return (MLIB_SUCCESS);

	cline = y;
	parline = y + 1;
	sline = line - stride;
	CHECKFORW;
	parline = y - 1;
	sline = line + stride;
	CHECKFORW;

#pragma pipeloop(0)
	for (; ; ) {
		GETSPAN;
		line = data + cline * stride;
		sline = line - stride * (parline - cline);
		CHECKFORW;
		sline = line + stride * (parline - cline);
		CHECKBACK;

	}	/* end of main loop */

/*  free stacks  */
	for (; nexspan; nexspan--)
		__mlib_free(pexspan[nexspan]);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef  imtype
#undef  CHECKFORW

/* *********************************************************** */

#define	imtype	mlib_s32
#define	CHECKFORW	CHECKFORW32

/* *********************************************************** */

mlib_status
__mlib_GraphicsFloodFill_32(
    mlib_image *buffer,
    mlib_s16 x,
    mlib_s16 y,
    mlib_s32 c,
    mlib_s32 c2)
{
	mlib_s32 stride = mlib_ImageGetStride(buffer) / sizeof (imtype);
	mlib_s32 width = mlib_ImageGetWidth(buffer);
	mlib_s32 height = mlib_ImageGetHeight(buffer);
	imtype *data = mlib_ImageGetData(buffer);
	imtype *line = data + y * stride, *sline;
	mlib_s32 cline, curl, curr, spl, spr;
	mlib_s32 parline, parl, parr;

/* stack counters */
	mlib_s32 stacktop = 0;

/* number of additional mem blocks */
	mlib_s32 nexspan = 0;
	span *spanp;

	mlib_d64 dc = vis_to_double_dup(c);
	mlib_d64 dc2 = vis_to_double_dup(c2);

/* ptrs to span stack blocks */
	span *pexspan[256];

/* 1st block of span stack */
	span spanstack[SPANSTACKSIZE];

	if (!data)
		return (MLIB_NULLPOINTER);

	if (x < 0 || x >= width || y < 0 || y >= height || stride < width)
		return (MLIB_FAILURE);

	if (line[x] != c2)
		return (MLIB_SUCCESS);

	pexspan[0] = spanstack;
	curl = curr = x;

/*  fill first with finding borders  */
	while (curl > 0 && line[curl - 1] == c2) {
		--curl;
		line[curl] = c;
	}

	while (curr < width && line[curr] == c2) {
		line[curr] = c;
		++curr;
	}

	if (height == 1)
		return (MLIB_SUCCESS);

	cline = y;
	parline = y + 1;
	sline = line - stride;
	CHECKFORW;
	parline = y - 1;
	sline = line + stride;
	CHECKFORW;

	for (; ; ) {
		GETSPAN;
		line = data + cline * stride;
		sline = line + stride * (parline - cline);
		CHECKBACK;
		sline = line - stride * (parline - cline);
		CHECKFORW;

	}	/* end of main loop */

/*  free stubs  */
	for (; nexspan; nexspan--)
		__mlib_free(pexspan[nexspan]);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
