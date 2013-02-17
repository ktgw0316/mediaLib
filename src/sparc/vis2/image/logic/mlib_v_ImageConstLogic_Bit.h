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

#ifndef _MLIB_V_IMAGECONSTLOGIC_BIT_H
#define	_MLIB_V_IMAGECONSTLOGIC_BIT_H

#pragma ident	"@(#)mlib_v_ImageConstLogic_Bit.h	9.2	07/11/05 SMI"

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(MLIB_VIS) || MLIB_VIS < 0x200
#error this include file can be used with MLIB_VIS >= 2.0
#endif /* !defined(MLIB_VIS) || MLIB_VIS < 0x200 */

#include <mlib_image.h>
#include <vis_proto.h>
#include <mlib_ImageCheck.h>
#include <mlib_v_ImageLogic_proto.h>

/* *********************************************************** */
    CONST_LOGIC_BITS
{
	mlib_s32 i, j;
	mlib_s32 offdst, offsrc, emask, dend_offset;
	mlib_d64 *dpp, *spp;
	mlib_d64 sa1, sa2, da, sa;
	mlib_d64 ssa, ssa1, ssa2, sa3, sa4;
	mlib_s32 amount;
	mlib_u8 *dend;
	mlib_u8 tmp, mask;
	mlib_d64 c1, c2, c3;
/* pointers for pixel and line of source */
	mlib_u8 *sp, *sl;
/* pointers for pixel and line of dst */
	mlib_u8 *dp, *dl;
	mlib_s32 width, height, nchannels;
/* for src */
	mlib_s32 stride;
/* for dst */
	mlib_s32 strided;
/* for dst */
	mlib_s32 bitoffsetd;
	mlib_u32 c01, c02, c03;
	mlib_d64 dc01, dc02, dc03;
	mlib_d64 dd1, dd2, dd3;

	    MLIB_IMAGE_SIZE_EQUAL(
	    dst,
	    src);
	    MLIB_IMAGE_TYPE_EQUAL(
	    dst,
	    src);
	    MLIB_IMAGE_CHAN_EQUAL(
	    dst,
	    src);

	    dp = (mlib_u8 *)mlib_ImageGetData(dst);
	    sp = (mlib_u8 *)mlib_ImageGetData(src);
	    height = mlib_ImageGetHeight(dst);
	    width = mlib_ImageGetWidth(dst);
	    stride = mlib_ImageGetStride(src);
	    strided = mlib_ImageGetStride(dst);
	    nchannels = mlib_ImageGetChannels(dst);
	    bitoffsetd = mlib_ImageGetBitOffset(dst);

	    width = width * nchannels;

	if (nchannels == 1) {
		c01 = c[0] & 1;
		c01 = (((mlib_s32)c01) << 31) >> 31;
		if (
		    bitoffsetd != 0)
		{
			c01 = (c01 >> bitoffsetd) | (c01 << (32 - bitoffsetd));
		}

		dc01 = vis_to_double_dup(c01);
	} else if (nchannels == 2) {
		c01 = ((c[0] & 1) << 1) | (c[1] & 1);
		c01 |= (c01 << 2);
		c01 |= (c01 << 4);
		c01 |= (c01 << 8);
		c01 |= (c01 << 16);
		if (bitoffsetd != 0) {
			c01 = (c01 >> bitoffsetd) | (c01 << (32 - bitoffsetd));
		}

		dc01 = vis_to_double_dup(c01);
	} else if (nchannels == 3) {
		c01 = ((c[0] & 1) << 2) | ((c[1] & 1) << 1) | (c[2] & 1);
		c01 |= (c01 << 3);
		c02 = (c01 << 6) | c01;
		c02 |= (c02 << 12);
		c01 = (c02 << 6) | c01;
		if (bitoffsetd != 0) {
			c01 =
			    (((c01 << (8 - bitoffsetd)) | (c01 >> (22 +
			    bitoffsetd))) & 0x3FFFFFFF);
		}

		c02 = (c01 << 4) | (c01 >> 26);
		c03 = (c01 << 3) | (c01 >> 27);
		c01 = (c01 << 2) | (c01 >> 28);
		dc01 = vis_to_double(c01, c02);
		dc02 = vis_to_double(c03, c01);
		dc03 = vis_to_double(c02, c03);
	} else {
		c01 =
		    ((c[0] & 1) << 3) | ((c[1] & 1) << 2) | ((c[2] & 1) << 1) |
		    (c[3] & 1);
		c01 |= (c01 << 4);
		c01 |= (c01 << 8);
		c01 |= (c01 << 16);
		if (bitoffsetd != 0) {
			c01 = (c01 >> bitoffsetd) | (c01 << (32 - bitoffsetd));
		}

		dc01 = vis_to_double_dup(c01);
	}

	if ((width > stride * 8) || (width > strided * 8))
		return (MLIB_FAILURE);

	if (nchannels == 3) {
		if ((width == stride * 8) && (width == strided * 8)) {

			amount = height * width;
			dend_offset = (bitoffsetd + amount) & 7;

			if (bitoffsetd != 0) {
				mask = (0xFF >> bitoffsetd);
				if (amount < 8 - bitoffsetd) {
					mask =
					    mask & (0xFF << (8 - bitoffsetd -
					    amount));
					tmp = C_CONSTLOGIC((c01 >> 8), *sp);
					*dp =
					    (((*dp) & (~mask)) | (tmp & mask));
					return (MLIB_SUCCESS);
				}

				tmp = C_CONSTLOGIC((c01 >> 8), *sp);
				*dp = (((*dp) & (~mask)) | (tmp & mask));
				sp++;
				dp++;
				amount -= (8 - bitoffsetd);
			}

			if (amount >= 8) {

				amount >>= 3;
				dend = dp + amount - 1;

				offdst = ((mlib_addr)dp) & 7;
				offsrc = ((mlib_addr)sp) & 7;

				if (offsrc == offdst) {

/* prepare the destination addresses */
					dpp = (mlib_d64 *)vis_alignaddr(dp, 0);
					i = (mlib_u8 *)dpp - dp;

					if (i != 0) {
						vis_alignaddrl((void *)offdst,
						    0);
						c3 = vis_faligndata(dc03, dc01);
						c1 = vis_faligndata(dc01, dc02);
						c2 = vis_faligndata(dc02, dc03);
					} else {
						c1 = dc01;
						c2 = dc02;
						c3 = dc03;
					}

/* prepare the destination addresses */
					spp = (mlib_d64 *)vis_alignaddr(sp, 0);

					if (i != 0) {
/* generate edge mask for the start point */
						emask = vis_edge8(dp, dend);
						sa1 = vis_ld_d64_nf(spp);
						da = VIS_CONSTLOGIC(c3, sa1);
						vis_pst_8(da, dpp++, emask);
						i += 8;
						spp++;
					}

					dd1 =
					    VIS_CONSTLOGIC(c1,
					    vis_ld_d64_nf(spp));
					dd2 =
					    VIS_CONSTLOGIC(c2,
					    vis_ld_d64_nf(spp + 1));
					dd3 =
					    VIS_CONSTLOGIC(c3,
					    vis_ld_d64_nf(spp + 2));

#pragma pipeloop(0)
					for (; i < amount - 24; i += 24) {
						dpp[0] = dd1;
						dd1 =
						    VIS_CONSTLOGIC(c1,
						    vis_ld_d64_nf(spp + 3));
						dpp[1] = dd2;
						dd2 =
						    VIS_CONSTLOGIC(c2,
						    vis_ld_d64_nf(spp + 4));
						dpp[2] = dd3;
						dd3 =
						    VIS_CONSTLOGIC(c3,
						    vis_ld_d64_nf(spp + 5));
						dpp += 3;
						spp += 3;
					}

					if (i < amount) {
						emask = vis_edge8(dpp, dend);
						sa1 = vis_ld_d64_nf(spp);
						da = VIS_CONSTLOGIC(c1, sa1);
						vis_pst_8(da, dpp++, emask);
						i += 8;
						spp++;

						if (i < amount) {
						    emask =
							vis_edge8(dpp,
							    dend);
						    sa1 =
							vis_ld_d64_nf(spp);
						    da = VIS_CONSTLOGIC(c2,
							    sa1);
						    vis_pst_8(da, dpp++,
							    emask);
						    i += 8;
						    spp++;

						    if (i < amount) {
							emask =
								vis_edge8
								    (dpp, dend);
							sa1 =
								vis_ld_d64_nf
								    (spp);
							da = VIS_CONSTLOGIC
								    (c3, sa1);
							vis_pst_8(da,
								    dpp, emask);
						    }
						}
					}
				} else {

/* prepare the destination addresses */
					dpp = (mlib_d64 *)vis_alignaddr(dp, 0);
					i = (mlib_u8 *)dpp - dp;

					if (i != 0) {
						vis_alignaddrl((void *)offdst,
						    0);
						c3 = vis_faligndata(dc03, dc01);
						c1 = vis_faligndata(dc01, dc02);
						c2 = vis_faligndata(dc02, dc03);
					} else {
						c1 = dc01;
						c2 = dc02;
						c3 = dc03;
					}

/* prepare the destination addresses */
					spp = (mlib_d64 *)vis_alignaddr(sp, i);

					sa1 = vis_ld_d64_nf(spp);
					if (i != 0) {
/* generate edge mask for the start point */
						emask = vis_edge8(dp, dend);
						sa2 = vis_ld_d64_nf(spp + 1);
						sa = vis_faligndata(sa1, sa2);
						da = VIS_CONSTLOGIC(c3, sa);
						vis_pst_8(da, dpp++, emask);
						sa1 = sa2;
						i += 8;
						spp++;
					}

					sa2 = vis_ld_d64_nf(spp + 1);
					ssa = vis_faligndata(sa1, sa2);
					dd1 = VIS_CONSTLOGIC(c1, ssa);

					sa3 = vis_ld_d64_nf(spp + 2);
					ssa1 = vis_faligndata(sa2, sa3);
					dd2 = VIS_CONSTLOGIC(c2, ssa1);

					sa4 = vis_ld_d64_nf(spp + 3);
					ssa2 = vis_faligndata(sa3, sa4);
					dd3 = VIS_CONSTLOGIC(c3, ssa2);

					sa1 = sa4;
					sa2 = vis_ld_d64_nf(spp + 4);
					sa3 = vis_ld_d64_nf(spp + 5);
					sa4 = vis_ld_d64_nf(spp + 6);

#pragma pipeloop(0)
					for (; i < amount - 24; i += 24) {
						dpp[0] = dd1;
						ssa = vis_faligndata(sa1, sa2);
						dd1 = VIS_CONSTLOGIC(c1, ssa);
						dpp[1] = dd2;
						ssa1 = vis_faligndata(sa2, sa3);
						dd2 = VIS_CONSTLOGIC(c2, ssa1);
						dpp[2] = dd3;
						ssa2 = vis_faligndata(sa3, sa4);
						dd3 = VIS_CONSTLOGIC(c3, ssa2);
						sa1 = sa4;
						sa2 = vis_ld_d64_nf(spp + 7);
						sa3 = vis_ld_d64_nf(spp + 8);
						sa4 = vis_ld_d64_nf(spp + 9);
						dpp += 3;
						spp += 3;
					}

					if (i < amount) {
						emask = vis_edge8(dpp, dend);
						sa2 = vis_ld_d64_nf(spp + 1);
						sa = vis_faligndata
						    (vis_ld_d64_nf(spp), sa2);
						da = VIS_CONSTLOGIC(c1, sa);
						vis_pst_8(da, dpp++, emask);
						sa1 = sa2;
						i += 8;
						spp++;

						if (i < amount) {
						    emask =
							vis_edge8(dpp,
							    dend);
						    sa2 =
							vis_ld_d64_nf(spp +
							    1);
						    sa = vis_faligndata(sa1,
							    sa2);
						    da = VIS_CONSTLOGIC(c2,
							    sa);
						    vis_pst_8(da, dpp++,
							    emask);
						    sa1 = sa2;
						    i += 8;
						    spp++;

						    if (i < amount) {
							emask =
								vis_edge8
								    (dpp, dend);
							sa2 =
								vis_ld_d64_nf
								    (spp + 1);
							sa = vis_faligndata
								    (sa1, sa2);
							da = VIS_CONSTLOGIC
								    (c3, sa);
							vis_pst_8(da,
								    dpp, emask);
						    }
						}
					}
				}

				sp += amount;
				dp = dend + 1;
			}

			if (dend_offset != 0) {
				if (bitoffsetd == 0) {
					tmp =
					    C_CONSTLOGIC((c01 >> dend_offset),
					    *sp);
				} else {
					tmp =
					    C_CONSTLOGIC((c01 >> (8 -
					    bitoffsetd + dend_offset)), *sp);
				}
				mask = (0xFF << (8 - dend_offset));
				*dp = (((*dp) & (~mask)) | (tmp & mask));
			}
		} else {

			sl = sp;
			dl = dp;

			dend_offset = (bitoffsetd + width) & 7;

			for (j = 0; j < height; j++) {

				amount = width;

				if (bitoffsetd != 0) {
					mask = (0xFF >> bitoffsetd);
					if (amount < 8 - bitoffsetd) {
						mask =
						    mask & (0xFF << (8 -
						    bitoffsetd - amount));
						tmp =
						    C_CONSTLOGIC((c01 >> 8),
						    *sp);
						*dp =
						    (((*dp) & (~mask)) | (tmp &
						    mask));
						sp = sl += stride;
						dp = dl += strided;
						continue;
					}

					tmp = C_CONSTLOGIC((c01 >> 8), *sp);
					*dp =
					    (((*dp) & (~mask)) | (tmp & mask));
					sp++;
					dp++;
					amount -= (8 - bitoffsetd);
				}

				if (amount >= 8) {

					amount >>= 3;
					dend = dp + amount - 1;

					offdst = ((mlib_addr)dp) & 7;
					offsrc = ((mlib_addr)sp) & 7;

					if (offsrc == offdst) {

/* prepare the destination addresses */
						dpp =
						    (mlib_d64 *)
						    vis_alignaddr(dp, 0);
						i = (mlib_u8 *)dpp - dp;

						if (i != 0) {
							vis_alignaddrl((void *)
							    offdst, 0);
							c3 = vis_faligndata
							    (dc03, dc01);
							c1 = vis_faligndata
							    (dc01, dc02);
							c2 = vis_faligndata
							    (dc02, dc03);
						} else {
							c1 = dc01;
							c2 = dc02;
							c3 = dc03;
						}

/* prepare the destination addresses */
						spp =
						    (mlib_d64 *)
						    vis_alignaddr(sp, 0);

						if (i != 0) {
/* generate edge mask for the start point */
							emask =
							    vis_edge8(dp, dend);
							sa1 =
							    vis_ld_d64_nf(spp);
							da = VIS_CONSTLOGIC(c3,
							    sa1);
							vis_pst_8(da, dpp++,
							    emask);
							i += 8;
							spp++;
						}

						dd1 =
						    VIS_CONSTLOGIC(c1,
						    vis_ld_d64_nf(spp));
						dd2 =
						    VIS_CONSTLOGIC(c2,
						    vis_ld_d64_nf(spp + 1));
						dd3 =
						    VIS_CONSTLOGIC(c3,
						    vis_ld_d64_nf(spp + 2));

#pragma pipeloop(0)
						for (; i < amount - 24;
						    i += 24) {
							dpp[0] = dd1;
							dd1 =
							    VIS_CONSTLOGIC(c1,
							    vis_ld_d64_nf(spp +
							    3));
							dpp[1] = dd2;
							dd2 =
							    VIS_CONSTLOGIC(c2,
							    vis_ld_d64_nf(spp +
							    4));
							dpp[2] = dd3;
							dd3 =
							    VIS_CONSTLOGIC(c3,
							    vis_ld_d64_nf(spp +
							    5));
							dpp += 3;
							spp += 3;
						}

						if (i < amount) {
						    emask =
							vis_edge8(dpp,
							    dend);
						    sa1 =
							vis_ld_d64_nf(spp);
						    spp++;
						    da = VIS_CONSTLOGIC(c1,
							    sa1);
						    vis_pst_8(da, dpp++,
							    emask);
						    i += 8;

						    if (i < amount) {
							emask =
								vis_edge8
								    (dpp, dend);
							sa1 =
								vis_ld_d64_nf
								    (spp);
							spp++;
							da = VIS_CONSTLOGIC
								    (c2, sa1);
							vis_pst_8(da,
								    dpp++,
								    emask);
							i += 8;

							if (i < amount) {
							    emask =
								vis_edge8
									(dpp,
									dend);
							    sa1 =
								vis_ld_d64_nf
									(spp);
							    da = VIS_CONSTLOGIC
									(c3,
									sa1);
							    vis_pst_8
									(da,
									dpp,
									emask);
							}
						    }
						}
					} else {

/* prepare the destination addresses */
						dpp =
						    (mlib_d64 *)
						    vis_alignaddr(dp, 0);
						i = (mlib_u8 *)dpp - dp;

						if (i != 0) {
							vis_alignaddrl((void *)
							    offdst, 0);
							c3 = vis_faligndata
							    (dc03, dc01);
							c1 = vis_faligndata
							    (dc01, dc02);
							c2 = vis_faligndata
							    (dc02, dc03);
						} else {
							c1 = dc01;
							c2 = dc02;
							c3 = dc03;
						}

/* prepare the destination addresses */
						spp =
						    (mlib_d64 *)
						    vis_alignaddr(sp, i);

						sa1 = vis_ld_d64_nf(spp);
						if (i != 0) {
/* generate edge mask for the start point */
							emask =
							    vis_edge8(dp, dend);
							sa2 =
							    vis_ld_d64_nf(spp +
							    1);
							sa = vis_faligndata(sa1,
							    sa2);
							da = VIS_CONSTLOGIC(c3,
							    sa);
							vis_pst_8(da, dpp++,
							    emask);
							sa1 = sa2;
							i += 8;
							spp++;
						}

						sa2 = vis_ld_d64_nf(spp + 1);
						ssa = vis_faligndata(sa1, sa2);
						dd1 = VIS_CONSTLOGIC(c1, ssa);

						sa3 = vis_ld_d64_nf(spp + 2);
						ssa1 = vis_faligndata(sa2, sa3);
						dd2 = VIS_CONSTLOGIC(c2, ssa1);

						sa4 = vis_ld_d64_nf(spp + 3);
						ssa2 = vis_faligndata(sa3, sa4);
						dd3 = VIS_CONSTLOGIC(c3, ssa2);

						sa1 = sa4;
						sa2 = vis_ld_d64_nf(spp + 4);
						sa3 = vis_ld_d64_nf(spp + 5);
						sa4 = vis_ld_d64_nf(spp + 6);

#pragma pipeloop(0)
						for (; i < amount - 24;
						    i += 24) {
							dpp[0] = dd1;
							ssa =
							    vis_faligndata(sa1,
							    sa2);
							dd1 =
							    VIS_CONSTLOGIC(c1,
							    ssa);
							dpp[1] = dd2;
							ssa1 =
							    vis_faligndata(sa2,
							    sa3);
							dd2 =
							    VIS_CONSTLOGIC(c2,
							    ssa1);
							dpp[2] = dd3;
							ssa2 =
							    vis_faligndata(sa3,
							    sa4);
							dd3 =
							    VIS_CONSTLOGIC(c3,
							    ssa2);
							sa1 = sa4;
							sa2 =
							    vis_ld_d64_nf(spp +
							    7);
							sa3 =
							    vis_ld_d64_nf(spp +
							    8);
							sa4 =
							    vis_ld_d64_nf(spp +
							    9);
							dpp += 3;
							spp += 3;
						}

						if (i < amount) {
						    emask =
							vis_edge8(dpp,
							    dend);
						    sa2 =
							vis_ld_d64_nf(spp +
							    1);
						    sa = vis_faligndata
							(vis_ld_d64_nf(spp),
							    sa2);
						    da = VIS_CONSTLOGIC(c1,
							    sa);
						    vis_pst_8(da, dpp++,
							    emask);
						    sa1 = sa2;
						    i += 8;
						    spp++;

						    if (i < amount) {
							emask =
								vis_edge8
								    (dpp, dend);
							sa2 =
								vis_ld_d64_nf
								    (spp + 1);
							sa = vis_faligndata
								    (sa1, sa2);
							da = VIS_CONSTLOGIC
								    (c2, sa);
							vis_pst_8(da,
								    dpp++,
								    emask);
							sa1 = sa2;
							i += 8;
							spp++;

							if (i < amount) {
							    emask =
								vis_edge8
								    (dpp,
								    dend);
							    sa2 =
								vis_ld_d64_nf
								    (spp
								    +
								    1);
							    sa = vis_faligndata
								    (sa1, sa2);
							    da = VIS_CONSTLOGIC
								    (c3, sa);
							    vis_pst_8
								    (da,
								    dpp,
								    emask);
							}
						    }
						}
					}

					sp += amount;
					dp = dend + 1;
				}

				if (dend_offset != 0) {
					if (bitoffsetd == 0) {
						tmp =
						    C_CONSTLOGIC((c01 >>
						    dend_offset), *sp);
					} else {
						tmp =
						    C_CONSTLOGIC((c01 >> (8 -
						    bitoffsetd + dend_offset)),
						    *sp);
					}
					mask = (0xFF << (8 - dend_offset));
					*dp =
					    (((*dp) & (~mask)) | (tmp & mask));
				}

				sp = sl += stride;
				dp = dl += strided;
			}
		}
	} else {
		if ((width == stride * 8) && (width == strided * 8)) {

			amount = height * width;
			dend_offset = (bitoffsetd + amount) & 7;

			if (bitoffsetd != 0) {
				mask = (0xFF >> bitoffsetd);
				if (amount < 8 - bitoffsetd) {
					mask =
					    mask & (0xFF << (8 - bitoffsetd -
					    amount));
					tmp = C_CONSTLOGIC(c01, *sp);
					*dp =
					    (((*dp) & (~mask)) | (tmp & mask));
					return (MLIB_SUCCESS);
				}

				tmp = C_CONSTLOGIC(c01, *sp);
				*dp = (((*dp) & (~mask)) | (tmp & mask));
				sp++;
				dp++;
				amount -= (8 - bitoffsetd);
			}

			if (amount >= 8) {

				amount >>= 3;
				dend = dp + amount - 1;

				offdst = ((mlib_addr)dp) & 7;
				offsrc = ((mlib_addr)sp) & 7;

				if (offsrc == offdst) {

/* prepare the destination addresses */
					dpp = (mlib_d64 *)vis_alignaddr(dp, 0);
					i = (mlib_u8 *)dpp - dp;

/* prepare the destination addresses */
					spp = (mlib_d64 *)vis_alignaddr(sp, 0);

					if (i != 0) {
/* generate edge mask for the start point */
						emask = vis_edge8(dp, dend);
						sa1 = vis_ld_d64_nf(spp);
						da = VIS_CONSTLOGIC(dc01, sa1);
						vis_pst_8(da, dpp++, emask);
						i += 8;
						spp++;
					}
#pragma pipeloop(0)
					for (; i < amount - 8; i += 8) {
						(*dpp++) =
						    VIS_CONSTLOGIC(dc01, *spp);
						spp++;
					}

					if (i < amount) {
						emask = vis_edge8(dpp, dend);
						sa1 = vis_ld_d64_nf(spp);
						da = VIS_CONSTLOGIC(dc01, sa1);
						vis_pst_8(da, dpp, emask);
					}
				} else {

/* prepare the destination addresses */
					dpp = (mlib_d64 *)vis_alignaddr(dp, 0);
					i = (mlib_u8 *)dpp - dp;

/* prepare the destination addresses */
					spp = (mlib_d64 *)vis_alignaddr(sp, i);

					sa1 = vis_ld_d64_nf(spp);
					if (i != 0) {
/* generate edge mask for the start point */
						emask = vis_edge8(dp, dend);
						sa2 = vis_ld_d64_nf(spp + 1);
						sa = vis_faligndata(sa1, sa2);
						da = VIS_CONSTLOGIC(dc01, sa);
						vis_pst_8(da, dpp++, emask);
						sa1 = sa2;
						i += 8;
						spp++;
					}
#pragma pipeloop(0)
					for (; i < amount - 8; i += 8) {
						sa2 = spp[1];
						sa = vis_faligndata(sa1, sa2);
						(*dpp++) =
						    VIS_CONSTLOGIC(dc01, sa);
						sa1 = sa2;
						spp++;
					}

					if (i < amount) {
						emask = vis_edge8(dpp, dend);
						sa2 = vis_ld_d64_nf(spp + 1);
						sa = vis_faligndata(sa1, sa2);
						da = VIS_CONSTLOGIC(dc01, sa);
						vis_pst_8(da, dpp, emask);
					}
				}

				sp += amount;
				dp = dend + 1;
			}

			if (dend_offset != 0) {
				tmp = C_CONSTLOGIC(c01, *sp);
				mask = (0xFF << (8 - dend_offset));
				*dp = (((*dp) & (~mask)) | (tmp & mask));
			}
		} else {

			sl = sp;
			dl = dp;

			dend_offset = (bitoffsetd + width) & 7;

			for (j = 0; j < height; j++) {

				amount = width;

				if (bitoffsetd != 0) {
					mask = (0xFF >> bitoffsetd);
					if (amount < 8 - bitoffsetd) {
						mask =
						    mask & (0xFF << (8 -
						    bitoffsetd - amount));
						tmp = C_CONSTLOGIC(c01, *sp);
						*dp =
						    (((*dp) & (~mask)) | (tmp &
						    mask));
						sp = sl += stride;
						dp = dl += strided;
						continue;
					}

					tmp = C_CONSTLOGIC(c01, *sp);
					*dp =
					    (((*dp) & (~mask)) | (tmp & mask));
					sp++;
					dp++;
					amount -= (8 - bitoffsetd);
				}

				if (amount >= 8) {

					amount >>= 3;
					dend = dp + amount - 1;

					offdst = ((mlib_addr)dp) & 7;
					offsrc = ((mlib_addr)sp) & 7;

					if (offsrc == offdst) {

/* prepare the destination addresses */
						dpp =
						    (mlib_d64 *)
						    vis_alignaddr(dp, 0);
						i = (mlib_u8 *)dpp - dp;

/* prepare the destination addresses */
						spp =
						    (mlib_d64 *)
						    vis_alignaddr(sp, 0);

						if (i != 0) {
/* generate edge mask for the start point */
							emask =
							    vis_edge8(dp, dend);
							sa1 =
							    vis_ld_d64_nf(spp);
							da = VIS_CONSTLOGIC
							    (dc01, sa1);
							vis_pst_8(da, dpp++,
							    emask);
							i += 8;
							spp++;
						}
#pragma pipeloop(0)
						for (; i < amount - 8; i += 8) {
							(*dpp++) =
							    VIS_CONSTLOGIC(dc01,
							    *spp);
							spp++;
						}

						if (i < amount) {
							emask =
							    vis_edge8(dpp,
							    dend);
							sa1 =
							    vis_ld_d64_nf(spp);
							da = VIS_CONSTLOGIC
							    (dc01, sa1);
							vis_pst_8(da, dpp,
							    emask);
						}
					} else {

/* prepare the destination addresses */
						dpp =
						    (mlib_d64 *)
						    vis_alignaddr(dp, 0);
						i = (mlib_u8 *)dpp - dp;

/* prepare the destination addresses */
						spp =
						    (mlib_d64 *)
						    vis_alignaddr(sp, i);

						sa1 = vis_ld_d64_nf(spp);
						if (i != 0) {
/* generate edge mask for the start point */
							emask =
							    vis_edge8(dp, dend);
							sa2 =
							    vis_ld_d64_nf(spp +
							    1);
							sa = vis_faligndata(sa1,
							    sa2);
							da = VIS_CONSTLOGIC
							    (dc01, sa);
							vis_pst_8(da, dpp++,
							    emask);
							sa1 = sa2;
							i += 8;
							spp++;
						}
#pragma pipeloop(0)
						for (; i < amount - 8; i += 8) {
							sa2 = spp[1];
							sa = vis_faligndata(sa1,
							    sa2);
							(*dpp++) =
							    VIS_CONSTLOGIC(dc01,
							    sa);
							sa1 = sa2;
							spp++;
						}

						if (i < amount) {
							emask =
							    vis_edge8(dpp,
							    dend);
							sa2 =
							    vis_ld_d64_nf(spp +
							    1);
							sa = vis_faligndata(sa1,
							    sa2);
							da = VIS_CONSTLOGIC
							    (dc01, sa);
							vis_pst_8(da, dpp,
							    emask);
						}
					}

					sp += amount;
					dp = dend + 1;
				}

				if (dend_offset != 0) {
					tmp = C_CONSTLOGIC(c01, *sp);
					mask = (0xFF << (8 - dend_offset));
					*dp =
					    (((*dp) & (~mask)) | (tmp & mask));
				}

				sp = sl += stride;
				dp = dl += strided;
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#ifdef __cplusplus
}
#endif

#endif /* _MLIB_V_IMAGECONSTLOGIC_BIT_H */
