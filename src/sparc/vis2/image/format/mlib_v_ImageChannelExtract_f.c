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
 * FUNCTIONS
 *      mlib_v_ImageChannelExtract_32_21
 *      mlib_v_ImageChannelExtract_32_31
 *      mlib_v_ImageChannelExtract_32_41
 *      mlib_v_ImageChannelExtract_32_32
 *      mlib_v_ImageChannelExtract_32_43
 *      mlib_v_ImageChannelExtract_D64_21
 *      mlib_v_ImageChannelExtract_D64_31
 *      mlib_v_ImageChannelExtract_D64_41
 *      mlib_v_ImageChannelExtract_D64_32
 *      mlib_v_ImageChannelExtract_D64_42
 *      mlib_v_ImageChannelExtract_D64_43
 *
 * ARGUMENT
 *      src    pointer to source image data
 *      dst    pointer to destination image data
 *      slb    source image line stride in bytes
 *      dlb   destination image line stride in bytes
 *      dsize  image data size in pixels
 *      xsize  image width in pixels
 *      ysize  image height in lines
 *      cmask channel mask
 *
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_v_ImageChannelExtract_2.h>

/* *********************************************************** */

void
mlib_v_ImageChannelExtract_32_21(
    const mlib_f32 *src,
    mlib_s32 slb,
    mlib_f32 *dst,
    mlib_s32 dlb,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 cmask)
{
	mlib_d64 *sp, *dp;
	mlib_f32 *sa, *sl, *da, *dl;
	mlib_d64 sd0, sd1, sd2, sd3;
	mlib_s32 soff, bmask, xsize;
	mlib_s32 i, j;

	if (width <= 0)
		return;

	if ((4 * width == dlb) && (slb == 2 * dlb)) {
		width *= height;
		height = 1;
	}

	if (cmask == 1)
		src += 1;

	sa = sl = (void *)src;
	da = dl = dst;

	for (j = 0; j < height; j++) {
		xsize = width;

		if (((mlib_addr)da & 7) != 0) {
			(*da++) = *sa;
			sa += 2;
			xsize -= 1;
		}

		dp = (mlib_d64 *)da;
		sp = (mlib_d64 *)((mlib_addr)sa & (~7));
		soff = (sa - (mlib_f32 *)sp) & 1;
		bmask = 0x012389AB;

		if (soff != 0)
			bmask = 0x4567CDEF;

		vis_write_bmask(bmask, 0);

#pragma pipeloop(0)
		for (i = 0; i < xsize - 3; i += 4) {
			sd0 = (*sp++);
			sd1 = (*sp++);
			sd2 = (*sp++);
			sd3 = (*sp++);
			(*dp++) = vis_bshuffle(sd0, sd1);
			(*dp++) = vis_bshuffle(sd2, sd3);
		}

#pragma pipeloop(0)
		for (; i < xsize - 1; i += 2) {
			sd0 = (*sp++);
			sd1 = (*sp++);
			(*dp++) = vis_bshuffle(sd0, sd1);
		}

		if (xsize & 1)
			*(mlib_f32 *)dp = *((mlib_f32 *)sp + soff);

		sa = sl = (mlib_f32 *)((mlib_u8 *)sl + slb);
		da = dl = (mlib_f32 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelExtract_32_31(
    const mlib_f32 *src,
    mlib_s32 slb,
    mlib_f32 *dst,
    mlib_s32 dlb,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 cmask)
{
	mlib_d64 *sp, *dp;
	mlib_f32 *sa, *sl, *da, *dl;
	mlib_d64 sd0, sd1, sd2, sd3;
	mlib_s32 soff, xsize;
	mlib_s32 i, j;

	if (width <= 0)
		return;

	if ((4 * width == dlb) && (slb == 3 * dlb)) {
		width *= height;
		height = 1;
	}

	if (cmask == 1)
		src += 2;
	else if (cmask == 2)
		src += 1;

	sa = sl = (void *)src;
	da = dl = dst;

	for (j = 0; j < height; j++) {
		xsize = width;

		if (((mlib_addr)da & 7) != 0) {
			(*da++) = *sa;
			sa += 3;
			xsize -= 1;
		}

		dp = (mlib_d64 *)da;
		sp = (mlib_d64 *)((mlib_addr)sa & (~7));
		soff = (sa - (mlib_f32 *)sp) & 1;

		if (soff == 0) {
			vis_write_bmask(0x0123CDEF, 0);

#pragma pipeloop(0)
			for (i = 0; i < xsize - 3; i += 4) {
				sd0 = (*sp++);
				sd1 = *sp;
				sp += 2;
				sd2 = (*sp++);
				sd3 = *sp;
				sp += 2;
				(*dp++) = vis_bshuffle(sd0, sd1);
				(*dp++) = vis_bshuffle(sd2, sd3);
			}

#pragma pipeloop(0)
			for (; i < xsize - 1; i += 2) {
				sd0 = (*sp++);
				sd1 = *sp;
				sp += 2;
				(*dp++) = vis_bshuffle(sd0, sd1);
			}

			if (xsize & 1)
				*(mlib_f32 *)dp = *(mlib_f32 *)sp;
		} else {
			vis_write_bmask(0x456789AB, 0);

#pragma pipeloop(0)
			for (i = 0; i < xsize - 3; i += 4) {
				sd0 = *sp;
				sp += 2;
				sd1 = (*sp++);
				sd2 = *sp;
				sp += 2;
				sd3 = (*sp++);
				(*dp++) = vis_bshuffle(sd0, sd1);
				(*dp++) = vis_bshuffle(sd2, sd3);
			}

#pragma pipeloop(0)
			for (; i < xsize - 1; i += 2) {
				sd0 = *sp;
				sp += 2;
				sd1 = (*sp++);
				(*dp++) = vis_bshuffle(sd0, sd1);
			}

			if (xsize & 1)
				*(mlib_f32 *)dp = *((mlib_f32 *)sp + 1);
		}

		sa = sl = (mlib_f32 *)((mlib_u8 *)sl + slb);
		da = dl = (mlib_f32 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelExtract_32_41(
    const mlib_f32 *src,
    mlib_s32 slb,
    mlib_f32 *dst,
    mlib_s32 dlb,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 cmask)
{
	mlib_d64 *sp, *dp;
	mlib_f32 *sa, *sl, *da, *dl;
	mlib_d64 sd0, sd1, sd2, sd3;
	mlib_s32 soff, bmask, xsize;
	mlib_s32 i, j;

	if (width <= 0)
		return;

	if ((4 * width == dlb) && (slb == 4 * dlb)) {
		width *= height;
		height = 1;
	}

	if (cmask == 1)
		src += 3;
	else if (cmask == 2)
		src += 2;
	else if (cmask == 4)
		src += 1;

	sa = sl = (void *)src;
	da = dl = dst;

	for (j = 0; j < height; j++) {
		xsize = width;

		if (((mlib_addr)da & 7) != 0) {
			(*da++) = *sa;
			sa += 4;
			xsize -= 1;
		}

		dp = (mlib_d64 *)da;
		sp = (mlib_d64 *)((mlib_addr)sa & (~7));
		soff = (sa - (mlib_f32 *)sp) & 1;
		bmask = 0x012389AB;

		if (soff != 0)
			bmask = 0x4567CDEF;

		vis_write_bmask(bmask, 0);

#pragma pipeloop(0)
		for (i = 0; i < xsize - 3; i += 4) {
			sd0 = *sp;
			sp += 2;
			sd1 = *sp;
			sp += 2;
			sd2 = *sp;
			sp += 2;
			sd3 = *sp;
			sp += 2;
			(*dp++) = vis_bshuffle(sd0, sd1);
			(*dp++) = vis_bshuffle(sd2, sd3);
		}

#pragma pipeloop(0)
		for (; i < xsize - 1; i += 2) {
			sd0 = *sp;
			sp += 2;
			sd1 = *sp;
			sp += 2;
			(*dp++) = vis_bshuffle(sd0, sd1);
		}

		if (xsize & 1)
			*(mlib_f32 *)dp = *((mlib_f32 *)sp + soff);

		sa = sl = (mlib_f32 *)((mlib_u8 *)sl + slb);
		da = dl = (mlib_f32 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelExtract_32_32(
    const mlib_f32 *src,
    mlib_s32 slb,
    mlib_f32 *dst,
    mlib_s32 dlb,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 cmask)
{
	mlib_d64 *sp, *dp;
	mlib_f32 *sa, *sl, *da, *dl, *dend;
	mlib_d64 sd0, sd1, sd2, sd3, dd0;
	mlib_s32 soff, xsize, cmask1, emask;
	mlib_s32 i, j;

	if (width <= 0)
		return;

	if ((8 * width == dlb) && (2 * slb == 3 * dlb)) {
		width *= height;
		height = 1;
	}

	width *= 2;

	if (cmask == 3) {
		src += 1;
		cmask = 6;
	}

	sa = sl = (void *)src;
	da = dl = dst;

	for (j = 0; j < height; j++) {
		cmask1 = cmask;
		xsize = width;
		dend = da + width - 1;

		if (((mlib_addr)da & 7) != 0) {
			(*da++) = *sa;
			sa++;
			xsize -= 1;
			cmask1 = ((cmask1 << 1) + 1) & 7;
		}

		dp = (mlib_d64 *)da;
		sp = (mlib_d64 *)((mlib_addr)sa & (~7));
		soff = (sa - (mlib_f32 *)sp) & 1;

		if (cmask1 != 5) {
			if (cmask1 == 3) {
				cmask1 <<= soff;
				sp += soff;
			} else
				cmask1 >>= soff;

			vis_write_bmask(0x456789AB, 0);

			if (cmask1 == 3) {

#pragma pipeloop(0)
				for (i = 0; i < xsize - 3; i += 4) {
					sd0 = (*sp++);
					sd1 = (*sp++);
					(*dp++) = vis_bshuffle(sd0, sd1);
					(*dp++) = (*sp++);
				}

/* end point handling */

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					emask = vis_edge32(dp, dend);
					sd0 = vis_ld_d64_nf(sp);
					sd1 = vis_ld_d64_nf(sp + 1);
					dd0 = vis_bshuffle(sd0, sd1);
					vis_pst_32(dd0, dp++, emask);

					if ((mlib_addr)dp <= (mlib_addr)dend) {
						emask = vis_edge32(dp, dend);
						dd0 = vis_ld_d64_nf(sp + 2);
						vis_pst_32(dd0, dp, emask);
					}
				}
			} else {

#pragma pipeloop(0)
				for (i = 0; i < xsize - 3; i += 4) {
					(*dp++) = (*sp++);
					sd0 = (*sp++);
					sd1 = (*sp++);
					(*dp++) = vis_bshuffle(sd0, sd1);
				}

/* end point handling */

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					emask = vis_edge32(dp, dend);
					dd0 = vis_ld_d64_nf(sp);
					vis_pst_32(dd0, dp++, emask);

					if ((mlib_addr)dp <= (mlib_addr)dend) {
						emask = vis_edge32(dp, dend);
						sd0 = vis_ld_d64_nf(sp + 1);
						sd1 = vis_ld_d64_nf(sp + 2);
						dd0 = vis_bshuffle(sd0, sd1);
						vis_pst_32(dd0, dp, emask);
					}
				}
			}
		} else {
			if (soff == 0) {
				vis_write_bmask(0x012389AB, 0);

#pragma pipeloop(0)
				for (i = 0; i < xsize - 3; i += 4) {
					sd0 = (*sp++);
					sd1 = (*sp++);
					sd2 = (*sp++);
					(*dp++) = vis_bshuffle(sd0, sd1);
					(*dp++) =
					    vis_freg_pair(vis_read_lo(sd1),
					    vis_read_lo(sd2));
				}

/* end point handling */

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					emask = vis_edge32(dp, dend);
					sd0 = vis_ld_d64_nf(sp);
					sd1 = vis_ld_d64_nf(sp + 1);
					dd0 = vis_bshuffle(sd0, sd1);
					vis_pst_32(dd0, dp++, emask);

					if ((mlib_addr)dp <= (mlib_addr)dend) {
						emask = vis_edge32(dp, dend);
						sd2 = vis_ld_d64_nf(sp + 2);
						dd0 =
						    vis_freg_pair(vis_read_lo
						    (sd1), vis_read_lo(sd2));
						vis_pst_32(dd0, dp, emask);
					}
				}
			} else {
				vis_write_bmask(0x4567CDEF, 0);
				sd0 = vis_ld_d64_nf(sp);
				sp++;

#pragma pipeloop(0)
				for (i = 0; i < xsize - 3; i += 4) {
					sd1 = (*sp++);
					sd2 = (*sp++);
					sd3 = (*sp++);
					(*dp++) = vis_bshuffle(sd0, sd1);
					(*dp++) =
					    vis_freg_pair(vis_read_hi(sd2),
					    vis_read_hi(sd3));
					sd0 = sd3;
				}

/* end point handling */

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					emask = vis_edge32(dp, dend);
					sd1 = vis_ld_d64_nf(sp);
					dd0 = vis_bshuffle(sd0, sd1);
					vis_pst_32(dd0, dp++, emask);

					if ((mlib_addr)dp <= (mlib_addr)dend) {
						emask = vis_edge32(dp, dend);
						sd2 = vis_ld_d64_nf(sp + 1);
						sd3 = vis_ld_d64_nf(sp + 2);
						dd0 =
						    vis_freg_pair(vis_read_hi
						    (sd2), vis_read_hi(sd3));
						vis_pst_32(dd0, dp, emask);
					}
				}
			}
		}

		sa = sl = (mlib_f32 *)((mlib_u8 *)sl + slb);
		da = dl = (mlib_f32 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelExtract_32_42(
    const mlib_f32 *src,
    mlib_s32 slb,
    mlib_f32 *dst,
    mlib_s32 dlb,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 cmask)
{
	mlib_d64 *sp, *dp;
	mlib_f32 *sa, *sl, *da, *dl, *dend;
	mlib_d64 sd0, sd1, sd2, sd3, dd0;
	mlib_s32 soff, xsize, cmask1, emask, bmask;
	mlib_s32 i, j;

	if (width <= 0)
		return;

	if ((8 * width == dlb) && (slb == 2 * dlb)) {
		width *= height;
		height = 1;
	}

	width *= 2;

	if (cmask == 3) {
		src += 2;
		cmask = 12;
	} else if ((cmask & 8) == 0) {
		src += 1;
		cmask *= 2;
	}

	sa = sl = (void *)src;
	da = dl = dst;

	for (j = 0; j < height; j++) {
		cmask1 = cmask;
		xsize = width;
		dend = da + width - 1;

		if (((mlib_addr)da & 7) != 0) {
			(*da++) = *sa;
			sa++;
			xsize -= 1;
			cmask1 = ((cmask1 << 1) + 1) & 0xF;

			if (cmask1 == 3) {
				sa += 2;
				cmask1 = 12;
			} else if ((cmask1 & 8) == 0) {
				sa += 1;
				cmask1 *= 2;
			}
		}

		dp = (mlib_d64 *)da;
		sp = (mlib_d64 *)((mlib_addr)sa & (~7));
		soff = (sa - (mlib_f32 *)sp) & 1;

		if ((cmask1 == 12) && (soff == 0)) {

#pragma pipeloop(0)
			for (i = 0; i < xsize - 3; i += 4) {
				(*dp++) = *sp;
				sp += 2;
				(*dp++) = *sp;
				sp += 2;
			}

/* end point handling */

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge32(dp, dend);
				dd0 = vis_ld_d64_nf(sp);
				vis_pst_32(dd0, dp++, emask);

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					emask = vis_edge32(dp, dend);
					dd0 = vis_ld_d64_nf(sp + 2);
					vis_pst_32(dd0, dp, emask);
				}
			}
		} else if ((cmask1 == 9) && (soff != 0)) {
			vis_write_bmask(0x456789AB, 0);
			sd0 = vis_ld_d64_nf(sp);
			sp += 2;

#pragma pipeloop(0)
			for (i = 0; i < xsize - 3; i += 4) {
				sd1 = *sp;
				sp += 2;
				sd2 = *sp;
				sp += 2;
				(*dp++) = vis_bshuffle(sd0, sd1);
				(*dp++) = vis_bshuffle(sd1, sd2);
				sd0 = sd2;
			}

/* end point handling */

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge32(dp, dend);
				sd1 = vis_ld_d64_nf(sp);
				dd0 = vis_bshuffle(sd0, sd1);
				vis_pst_32(dd0, dp++, emask);

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					emask = vis_edge32(dp, dend);
					sd2 = vis_ld_d64_nf(sp + 2);
					dd0 = vis_bshuffle(sd1, sd2);
					vis_pst_32(dd0, dp, emask);
				}
			}
		} else {
			if (soff == 0) {
				bmask = 0x012389AB;

				if (cmask1 == 9) {
					bmask = 0x0123CDEF;
				}
			} else {
				bmask = 0x4567CDEF;

				if (cmask1 == 12) {
					bmask = 0x456789AB;
				}
			}

			vis_write_bmask(bmask, 0);

#pragma pipeloop(0)
			for (i = 0; i < xsize - 3; i += 4) {
				sd0 = (*sp++);
				sd1 = (*sp++);
				sd2 = (*sp++);
				sd3 = (*sp++);
				(*dp++) = vis_bshuffle(sd0, sd1);
				(*dp++) = vis_bshuffle(sd2, sd3);
			}

/* end point handling */

			if ((mlib_addr)dp <= (mlib_addr)dend) {
				emask = vis_edge32(dp, dend);
				sd0 = vis_ld_d64_nf(sp);
				sd1 = vis_ld_d64_nf(sp + 1);
				dd0 = vis_bshuffle(sd0, sd1);
				vis_pst_32(dd0, dp++, emask);

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					emask = vis_edge32(dp, dend);
					sd2 = vis_ld_d64_nf(sp + 2);
					sd3 = vis_ld_d64_nf(sp + 3);
					dd0 = vis_bshuffle(sd2, sd3);
					vis_pst_32(dd0, dp, emask);
				}
			}
		}

		sa = sl = (mlib_f32 *)((mlib_u8 *)sl + slb);
		da = dl = (mlib_f32 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelExtract_32_43(
    const mlib_f32 *src,
    mlib_s32 slb,
    mlib_f32 *dst,
    mlib_s32 dlb,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 cmask)
{
	mlib_d64 *sp, *dp;
	mlib_f32 *sa, *sl, *da, *dl, *dend;
	mlib_d64 sd0, sd1, sd2, sd3, sd4, dd0;
	mlib_s32 soff, xsize, cmask1, emask;
	mlib_s32 i, j;

	if (width <= 0)
		return;

	if ((12 * width == dlb) && (3 * slb == 4 * dlb)) {
		width *= height;
		height = 1;
	}

	width *= 3;

	if (cmask == 7) {
		src += 1;
		cmask = 14;
	}

	vis_alignaddr((void *)0, 4);

	sa = sl = (void *)src;
	da = dl = dst;

	for (j = 0; j < height; j++) {
		cmask1 = cmask;
		xsize = width;
		dend = da + width - 1;

		if (((mlib_addr)da & 7) != 0) {
			(*da++) = *sa;
			sa++;
			xsize -= 1;
			cmask1 = ((cmask1 << 1) + 1) & 0xF;

			if (cmask1 == 7) {
				sa += 1;
				cmask1 = 14;
			}
		}

		if (xsize <= 0)
			continue;

		dp = (mlib_d64 *)da;

		if (cmask1 == 13) {
			emask = vis_edge32(dp, dend);
			dd0 =
			    vis_freg_pair(vis_ld_f32_nf(sa),
			    vis_ld_f32_nf(sa + 1));
			vis_pst_32(dd0, dp++, emask);
			xsize -= 2;
			sa += 3;
			cmask1 = 14;
		}

		sp = (mlib_d64 *)((mlib_addr)sa & (~7));
		soff = (sa - (mlib_f32 *)sp) & 1;

		if (cmask1 == 14) {

			if (soff == 0) {
				vis_write_bmask(0x012389AB, 0);

#pragma pipeloop(0)
				for (i = 0; i < xsize - 5; i += 6) {
					sd0 = (*sp++);
					sd1 = (*sp++);
					sd2 = (*sp++);
					sd3 = (*sp++);
					(*dp++) = sd0;
					(*dp++) = vis_bshuffle(sd1, sd2);
					(*dp++) = vis_faligndata(sd2, sd3);
				}

/* end point handling */

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					emask = vis_edge32(dp, dend);
					dd0 = vis_ld_d64_nf(sp);
					vis_pst_32(dd0, dp++, emask);

					if ((mlib_addr)dp <= (mlib_addr)dend) {
						emask = vis_edge32(dp, dend);
						sd1 = vis_ld_d64_nf(sp + 1);
						sd2 = vis_ld_d64_nf(sp + 2);
						dd0 = vis_bshuffle(sd1, sd2);
						vis_pst_32(dd0, dp++, emask);

						if ((mlib_addr)dp <=
						    (mlib_addr)dend) {
							emask =
							    vis_edge32(dp,
							    dend);
							sd3 =
							    vis_ld_d64_nf(sp +
							    3);
							dd0 =
							    vis_faligndata(sd2,
							    sd3);
							vis_pst_32(dd0, dp,
							    emask);
						}
					}
				}
			} else {
				vis_write_bmask(0x4567CDEF, 0);

#pragma pipeloop(0)
				for (i = 0; i < xsize - 5; i += 6) {
					sd0 = (*sp++);
					sd1 = (*sp++);
					sd2 = (*sp++);
					sd3 = (*sp++);
					(*dp++) = vis_faligndata(sd0, sd1);
					(*dp++) = vis_bshuffle(sd1, sd2);
					(*dp++) = sd3;
				}

/* end point handling */

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					emask = vis_edge32(dp, dend);
					sd0 = vis_ld_d64_nf(sp);
					sd1 = vis_ld_d64_nf(sp + 1);
					dd0 = vis_faligndata(sd0, sd1);
					vis_pst_32(dd0, dp++, emask);

					if ((mlib_addr)dp <= (mlib_addr)dend) {
						emask = vis_edge32(dp, dend);
						sd2 = vis_ld_d64_nf(sp + 2);
						dd0 = vis_bshuffle(sd1, sd2);
						vis_pst_32(dd0, dp++, emask);

						if ((mlib_addr)dp <=
						    (mlib_addr)dend) {
							emask =
							    vis_edge32(dp,
							    dend);
							dd0 =
							    vis_ld_d64_nf(sp +
							    3);
							vis_pst_32(dd0, dp,
							    emask);
						}
					}
				}
			}
		} else {
			if (soff == 0) {
				vis_write_bmask(0x012389AB, 0);

#pragma pipeloop(0)
				for (i = 0; i < xsize - 5; i += 6) {
					sd0 = (*sp++);
					sd1 = (*sp++);
					sd2 = (*sp++);
					sd3 = (*sp++);
					(*dp++) = vis_bshuffle(sd0, sd1);
					(*dp++) = vis_faligndata(sd1, sd2);
					(*dp++) = sd3;
				}

/* end point handling */

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					emask = vis_edge32(dp, dend);
					sd0 = vis_ld_d64_nf(sp);
					sd1 = vis_ld_d64_nf(sp + 1);
					dd0 = vis_bshuffle(sd0, sd1);
					vis_pst_32(dd0, dp++, emask);

					if ((mlib_addr)dp <= (mlib_addr)dend) {
						emask = vis_edge32(dp, dend);
						sd2 = vis_ld_d64_nf(sp + 2);
						dd0 = vis_faligndata(sd1, sd2);
						vis_pst_32(dd0, dp++, emask);

						if ((mlib_addr)dp <=
						    (mlib_addr)dend) {
							emask =
							    vis_edge32(dp,
							    dend);
							dd0 =
							    vis_ld_d64_nf(sp +
							    3);
							vis_pst_32(dd0, dp,
							    emask);
						}
					}
				}
			} else {
				vis_write_bmask(0x4567CDEF, 0);
				sd0 = vis_ld_d64_nf(sp);
				sp++;

#pragma pipeloop(0)
				for (i = 0; i < xsize - 5; i += 6) {
					sd1 = (*sp++);
					sd2 = (*sp++);
					sd3 = (*sp++);
					sd4 = (*sp++);
					(*dp++) = vis_bshuffle(sd0, sd1);
					(*dp++) = sd2;
					(*dp++) = vis_faligndata(sd3, sd4);
					sd0 = sd4;
				}

/* end point handling */

				if ((mlib_addr)dp <= (mlib_addr)dend) {
					emask = vis_edge32(dp, dend);
					sd1 = vis_ld_d64_nf(sp);
					dd0 = vis_bshuffle(sd0, sd1);
					vis_pst_32(dd0, dp++, emask);

					if ((mlib_addr)dp <= (mlib_addr)dend) {
						emask = vis_edge32(dp, dend);
						dd0 = vis_ld_d64_nf(sp + 1);
						vis_pst_32(dd0, dp++, emask);

						if ((mlib_addr)dp <=
						    (mlib_addr)dend) {
							emask =
							    vis_edge32(dp,
							    dend);
							sd3 =
							    vis_ld_d64_nf(sp +
							    2);
							sd4 =
							    vis_ld_d64_nf(sp +
							    3);
							dd0 =
							    vis_faligndata(sd3,
							    sd4);
							vis_pst_32(dd0, dp,
							    emask);
						}
					}
				}
			}
		}

		sa = sl = (mlib_f32 *)((mlib_u8 *)sl + slb);
		da = dl = (mlib_f32 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelExtract_D64_21(
    const mlib_d64 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 cmask)
{
	mlib_d64 *sp, *dp;
	mlib_d64 *sl, *dl;
	mlib_s32 i, j;

	if (cmask == 1)
		src++;

	sp = sl = (void *)src;
	dp = dl = dst;

	if ((8 * width == dlb) && (slb == 2 * dlb)) {
		width *= height;
		height = 1;
	}

	for (j = 0; j < height; j++) {

#pragma pipeloop(0)
		for (i = 0; i < width; i++) {
			*dp = *sp;
			dp++;
			sp += 2;
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelExtract_D64_31(
    const mlib_d64 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 cmask)
{
	mlib_d64 *sp, *dp;
	mlib_d64 *sl, *dl;
	mlib_s32 i, j;

	if (cmask == 1)
		src += 2;
	else if (cmask == 2)
		src += 1;

	sp = sl = (void *)src;
	dp = dl = dst;

	if ((8 * width == dlb) && (slb == 3 * dlb)) {
		width *= height;
		height = 1;
	}

	for (j = 0; j < height; j++) {

#pragma pipeloop(0)
		for (i = 0; i < width; i++) {
			*dp = *sp;
			dp++;
			sp += 3;
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelExtract_D64_41(
    const mlib_d64 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 cmask)
{
	mlib_d64 *sp, *dp;
	mlib_d64 *sl, *dl;
	mlib_s32 i, j;

	if (cmask == 1)
		src += 3;
	else if (cmask == 2)
		src += 2;
	else if (cmask == 4)
		src += 1;

	sp = sl = (void *)src;
	dp = dl = dst;

	if ((8 * width == dlb) && (slb == 3 * dlb)) {
		width *= height;
		height = 1;
	}

	for (j = 0; j < height; j++) {

#pragma pipeloop(0)
		for (i = 0; i < width; i++) {
			*dp = *sp;
			dp++;
			sp += 4;
		}

		sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
		dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelExtract_D64_32(
    const mlib_d64 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 cmask)
{
	mlib_d64 *sp, *dp;
	mlib_d64 *sl, *dl;
	mlib_s32 i, j;

	if (cmask == 3) {
		src += 1;
		cmask = 6;
	}

	sp = sl = (void *)src;
	dp = dl = dst;

	if ((16 * width == dlb) && (2 * slb == 3 * dlb)) {
		width *= height;
		height = 1;
	}

	if (cmask == 5) {

		for (j = 0; j < height; j++) {

#pragma pipeloop(0)
			for (i = 0; i < width; i++) {
				(*dp++) = *sp;
				sp += 2;
				(*dp++) = (*sp++);
			}

			sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
			dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
		}
	} else {
		for (j = 0; j < height; j++) {

#pragma pipeloop(0)
			for (i = 0; i < width; i++) {
				(*dp++) = (*sp++);
				(*dp++) = *sp;
				sp += 2;
			}

			sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
			dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelExtract_D64_42(
    const mlib_d64 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 cmask)
{
	mlib_d64 *sp, *dp;
	mlib_d64 *sl, *dl;
	mlib_s32 i, j;

	if (cmask == 3) {
		src += 2;
		cmask = 12;
	} else if ((cmask & 8) == 0) {
		src += 1;
		cmask *= 2;
	}

	sp = sl = (void *)src;
	dp = dl = dst;

	if ((16 * width == dlb) && (slb == 2 * dlb)) {
		width *= height;
		height = 1;
	}

	if (cmask == 9) {

		for (j = 0; j < height; j++) {

#pragma pipeloop(0)
			for (i = 0; i < width; i++) {
				(*dp++) = *sp;
				sp += 3;
				(*dp++) = (*sp++);
			}

			sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
			dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
		}
	} else if (cmask == 10) {
		for (j = 0; j < height; j++) {

#pragma pipeloop(0)
			for (i = 0; i < width; i++) {
				(*dp++) = *sp;
				sp += 2;
				(*dp++) = *sp;
				sp += 2;
			}

			sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
			dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
		}
	} else {
		for (j = 0; j < height; j++) {

#pragma pipeloop(0)
			for (i = 0; i < width; i++) {
				(*dp++) = (*sp++);
				(*dp++) = *sp;
				sp += 3;
			}

			sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
			dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
		}
	}
}

/* *********************************************************** */

void
mlib_v_ImageChannelExtract_D64_43(
    const mlib_d64 *src,
    mlib_s32 slb,
    mlib_d64 *dst,
    mlib_s32 dlb,
    mlib_s32 width,
    mlib_s32 height,
    mlib_s32 cmask)
{
	mlib_d64 *sp, *dp;
	mlib_d64 *sl, *dl;
	mlib_s32 i, j;

	if (cmask == 7) {
		src += 1;
		cmask = 14;
	}

	sp = sl = (void *)src;
	dp = dl = dst;

	if ((24 * width == dlb) && (3 * slb == 4 * dlb)) {
		width *= height;
		height = 1;
	}

	if (cmask == 11) {

		for (j = 0; j < height; j++) {

#pragma pipeloop(0)
			for (i = 0; i < width; i++) {
				(*dp++) = *sp;
				sp += 2;
				(*dp++) = (*sp++);
				(*dp++) = (*sp++);
			}

			sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
			dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
		}
	} else if (cmask == 13) {
		for (j = 0; j < height; j++) {

#pragma pipeloop(0)
			for (i = 0; i < width; i++) {
				(*dp++) = (*sp++);
				(*dp++) = *sp;
				sp += 2;
				(*dp++) = (*sp++);
			}

			sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
			dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
		}
	} else {
		for (j = 0; j < height; j++) {

#pragma pipeloop(0)
			for (i = 0; i < width; i++) {
				(*dp++) = (*sp++);
				(*dp++) = (*sp++);
				(*dp++) = *sp;
				sp += 2;
			}

			sp = sl = (mlib_d64 *)((mlib_u8 *)sl + slb);
			dp = dl = (mlib_d64 *)((mlib_u8 *)dl + dlb);
		}
	}
}

/* *********************************************************** */
