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

#pragma ident	"@(#)mlib_c_ImageClrOrdDitherMxN_Bit.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *   Internal functions for mlib_ImageColorOrderedDitherMxN()
 */

#include <stdlib.h>
#include <mlib_image.h>
#include <mlib_SysMath.h>
#include <mlib_ImageCheck.h>
#include <mlib_ImageColormap.h>
#include <mlib_ImageColorOrderedDitherMxN.h>

/* *********************************************************** */

#define	FUNC(SUFF)	mlib_ImageColorOrderedDitherBit_##SUFF

/* *********************************************************** */

#define	FUNC_M_ARG                                                   \
	mlib_u8 *dp, mlib_u8 *sp, mlib_s32 *pkern, mlib_s32 mstep,   \
	    mlib_s32 sw, mlib_s32 tail

/* *********************************************************** */

#define	NCHAN	1

/* *********************************************************** */

#define	DEF_VAR                                                 \
	mlib_s32 mask1 = 0x000000FF;                            \
	mlib_s32 mask2 = 0x0000FF00;                            \
	mlib_s32 i

/* *********************************************************** */

#define	SAT_U8(out, src)                                         \
	{                                                        \
	    mlib_s32 sum = src;                                  \
	    mlib_s32 imask = (sum >> 5) & 0x18;                  \
	                                                         \
	    out = (sum & (mask1 >> imask)) | (mask2 >> imask);   \
	}

/* *********************************************************** */

void
FUNC(m1) (FUNC_M_ARG)
{
	DEF_VAR;
	mlib_s32 k0 = pkern[0];

	for (i = 0; i < sw; i++) {
		SAT_U8(dp[0], sp[0] + k0);
		sp += mstep;
		dp += mstep;
	}
}

/* *********************************************************** */

void
FUNC(m2) (FUNC_M_ARG)
{
	DEF_VAR;
	mlib_s32 k0 = pkern[0];
	mlib_s32 k1 = pkern[1];

	for (i = 0; i < sw; i++) {
		SAT_U8(dp[0], sp[0] + k0);
		SAT_U8(dp[1], sp[1] + k1);
		sp += mstep;
		dp += mstep;
	}

	if (tail > 0) {
		SAT_U8(dp[0], sp[0] + k0);
	}
}

/* *********************************************************** */

void
FUNC(m3) (FUNC_M_ARG)
{
	DEF_VAR;
	mlib_s32 k0 = pkern[0];
	mlib_s32 k1 = pkern[1];
	mlib_s32 k2 = pkern[2];

	for (i = 0; i < sw; i++) {
		SAT_U8(dp[0], sp[0] + k0);
		SAT_U8(dp[1], sp[1] + k1);
		SAT_U8(dp[2], sp[2] + k2);
		sp += mstep;
		dp += mstep;
	}

	if (tail > 0) {
		SAT_U8(dp[0], sp[0] + k0);

		if (tail > 1) {
		    SAT_U8(dp[1], sp[1] + k1);
		}
	}
}

/* *********************************************************** */

#define	KW_MAX	3

typedef void (*func_m_type) (FUNC_M_ARG);

static func_m_type func_m_arr[] = {
	NULL,
	FUNC(m1),
	FUNC(m2),
	FUNC(m3)
};

/* *********************************************************** */

#define	MAX_KERN	64

/* *********************************************************** */

mlib_status
FUNC(MxN) (
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 **dmask,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 scale,
    const void *colormap)
{
	mlib_type stype, dtype;
	const mlib_s32 *dmask0;
	mlib_s32 method = mlib_ImageGetMethod(colormap);
	mlib_u8 *sl, *dl;
	mlib_s32 schan, dchan, sll, dll, sw, sh, dw, dh, num_blk, tail;
	mlib_s32 off, off1, kw, mstep, kern_size, i, j;
	mlib_u8 *pbuff, *p_lut;
	mlib_s32 kern_lcl[MAX_KERN], *kern = kern_lcl, *pkern;
	mlib_d64 dscale, dscale0;
	mlib_s32 step0, half_step0;
	mlib_s32 bit_offset = mlib_ImageGetBitOffset(dst);

	MLIB_IMAGE_GET_ALL_PARAMS(dst, dtype, dchan, dw, dh, dll, dl);
	MLIB_IMAGE_GET_ALL_PARAMS(src, stype, schan, sw, sh, sll, sl);

	p_lut = (mlib_u8 *)mlib_ImageGetLutInversTable(colormap);
	step0 = abs(p_lut[1] - p_lut[0]);

	kern_size = n * m * NCHAN;

	if (kern_size > MAX_KERN) {
		kern = __mlib_malloc(kern_size * sizeof (mlib_s32));

		if (kern == NULL)
			return (MLIB_FAILURE);
	}

	dscale = 1.0;
	while (scale > 30) {
		dscale *= 1.0 / (1 << 30);
		scale -= 30;
	}

	dscale /= (1 << scale);

	dscale0 = dscale * step0;
	half_step0 = (step0 - 1) >> 1;

	dmask0 = dmask[0];
	for (j = 0; j < n; j++) {
		for (i = 0; i < m; i++) {
			kern[j * m + i] =
			    half_step0 - (mlib_s32)(dmask0[j * m +
			    i] * dscale0);
		}
	}

	num_blk = sw / m;
	tail = sw % m;
	pbuff = __mlib_malloc(sw * NCHAN * sizeof (mlib_u8));

	if (pbuff == NULL) {
		if (kern != kern_lcl)
			__mlib_free(kern);
		return (MLIB_FAILURE);
	}

	mstep = m * NCHAN;
	pkern = kern;

	for (j = 0; j < sh; j++) {
		for (off = 0; off < m; off += kw) {
			kw = m - off;

			if (kw > KW_MAX)
				kw = KW_MAX;
			off1 = off * NCHAN;
			if (tail - off >= kw) {
				func_m_arr[kw] (pbuff + off1, sl + off1,
				pkern + off1, mstep, num_blk + 1, 0);
			} else {
				func_m_arr[kw] (pbuff + off1, sl + off1,
				pkern + off1, mstep, num_blk, tail - off);
			}
		}

		pkern += mstep;

		if (pkern >= kern + kern_size)
			pkern = kern;

		mlib_ImageColorTrue2IndexLine_U8_BIT_1(pbuff, dl, bit_offset,
		    sw, colormap);

		sl += sll;
		dl += dll;
	}

	__mlib_free(pbuff);

	if (kern != kern_lcl)
		__mlib_free(kern);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
