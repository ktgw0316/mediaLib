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

#pragma ident	"@(#)mlib_v_ImageClrOrdDitherMxN_Bit.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *   Internal functions for mlib_ImageColorOrderedDitherMxN()
 */

#include <stdlib.h>
#include <vis_proto.h>
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
	    mlib_s32 sw

/* *********************************************************** */

#define	NCHAN	1

/* *********************************************************** */

#define	LOAD_NA(ss, sp)                                         \
	{                                                       \
	    mlib_d64 *pp = vis_alignaddr(sp, 0);                \
	                                                        \
	    ss = vis_faligndata(pp[0], pp[1]);                  \
	}

/* *********************************************************** */

#define	LOAD_NA_NF(ss, sp)                                      \
	{                                                       \
	    mlib_d64 *pp = vis_alignaddr(sp, 0);                \
	                                                        \
	    ss = vis_faligndata(vis_ld_d64_nf(pp),              \
		vis_ld_d64_nf(pp + 1));                         \
	}

/* *********************************************************** */

#define	MAX_KERN	64

/* *********************************************************** */

mlib_status FUNC(
    MxN) (
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 **dmask,
    mlib_s32 m,
    mlib_s32 n,
    mlib_s32 scale,
    const void *colormap)
{
	mlib_type stype, dtype;
	const mlib_s32 *dmask0 = dmask[0], *dmask1 = dmask[1], *dmask2 =
	    dmask[2];
	mlib_s32 method = mlib_ImageGetMethod(colormap);
	mlib_u8 *sl, *dl;
	mlib_s32 schan, dchan, sll, dll, sw, sh, dw, dh, num_blk;
	mlib_s32 off, off1, kw, mstep, line_size, kern_size, xsize8, i, j, k;
	mlib_d64 *pbuff;
	mlib_u8 *p_dim;
	mlib_s16 *kern, *pkern;
	mlib_d64 *dkern;
	mlib_d64 dscale, dscale0, dscale1, dscale2;
	mlib_d64 ss, d0, d1;
	mlib_f32 fzeros = vis_fzeros();
	mlib_s32 step0, half_step0, v0;
	mlib_s32 bit_offset = mlib_ImageGetBitOffset(dst);
	mlib_u8 *p_lut;

	MLIB_IMAGE_GET_ALL_PARAMS(dst, dtype, dchan, dw, dh, dll, dl);
	MLIB_IMAGE_GET_ALL_PARAMS(src, stype, schan, sw, sh, sll, sl);

	p_lut = (mlib_u8 *)mlib_ImageGetLutInversTable(colormap);
	step0 = abs(p_lut[1] - p_lut[0]);

	num_blk = (sw + (m - 1)) / m;
	mstep = m * NCHAN;
	line_size = (mstep * num_blk + 7) & ~7;
	xsize8 = (NCHAN * sw + 7) / 8;

	dscale = 1.0;
	while (scale > 30) {
		dscale *= 1.0 / (1 << 30);
		scale -= 30;
	}

	dscale /= (1 << scale);

	dscale0 = dscale * step0;
	half_step0 = (step0 - 1) >> 1;

	kern_size = n * line_size;
	kern = __mlib_malloc(kern_size * sizeof (mlib_s16));

	if (kern == NULL)
		return (MLIB_FAILURE);

	for (j = 0; j < n; j++) {
		for (i = 0; i < m; i++) {
			pkern = kern + j * line_size + i;
			v0 = half_step0 - (mlib_s32)(dmask0[j * m +
			    i] * dscale0);
			for (k = 0; k < num_blk; k++) {
				pkern[k * mstep] = v0;
			}
		}
	}

	pbuff = __mlib_malloc(xsize8 * sizeof (mlib_d64) + 16);

	if (pbuff == NULL) {
		__mlib_free(kern);
		return (MLIB_FAILURE);
	}

	pkern = kern;

	vis_write_gsr(7 << 3);

	for (j = 0; j < sh; j++) {
		dkern = (mlib_d64 *)pkern;

		if ((mlib_s32)sl & 7) {
			mlib_u8 *sp = sl;

#pragma pipeloop(0)
			for (i = 0; i < xsize8; i++) {
				LOAD_NA_NF(ss, sp);
				d0 = vis_fpadd16(vis_fpmerge(vis_fzeros(),
				    vis_read_hi(ss)), dkern[2 * i]);
				d1 = vis_fpadd16(vis_fpmerge(vis_fzeros(),
				    vis_read_lo(ss)), dkern[2 * i + 1]);
				pbuff[i] = vis_fpack16_pair(d0, d1);
				sp += 8;
			}

		} else {
			mlib_d64 *sp = (mlib_d64 *)sl;

#pragma pipeloop(0)
			for (i = 0; i < xsize8; i++) {
				ss = sp[i];
				d0 = vis_fpadd16(vis_fpmerge(vis_fzeros(),
				    vis_read_hi(ss)), dkern[2 * i]);
				d1 = vis_fpadd16(vis_fpmerge(vis_fzeros(),
				    vis_read_lo(ss)), dkern[2 * i + 1]);
				pbuff[i] = vis_fpack16_pair(d0, d1);
			}
		}

		pkern += line_size;

		if (pkern >= kern + kern_size)
			pkern = kern;

		mlib_ImageColorTrue2IndexLine_U8_BIT_1((mlib_u8 *)pbuff, dl,
		    bit_offset, sw, colormap);

		sl += sll;
		dl += dll;
	}

	__mlib_free(pbuff);
	__mlib_free(kern);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
