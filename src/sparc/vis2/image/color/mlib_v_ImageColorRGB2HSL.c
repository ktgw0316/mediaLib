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

#pragma ident	"@(#)mlib_v_ImageColorRGB2HSL.c	9.2	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_ImageColorRGB2HSL - converts image from RGB to HSL
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageColorRGB2HSL(mlib_image       *dst,
 *                                         const mlib_image *src)
 *
 * ARGUMENTS
 *      dst  pointer to an dst image
 *      src  pointer to an src image
 *
 * MLIB_BYTE case notes:
 *      ranges for all values supposed to be [MLIB_U8_MIN; MLIB_U8_MAX]
 *      if R==G==B then H = S=MLIB_U8_MIN;
 *
 * MLIB_SHORT case notes:
 *      ranges for all values supposed to be [MLIB_S16_MIN; MLIB_S16_MAX]
 *      if R==G==B then H = S=MLIB_S16_MIN;
 *
 * MLIB_USHORT case notes:
 *      ranges for all values supposed to be [MLIB_U16_MIN; MLIB_U16_MAX]
 *      if R==G==B then H = S=MLIB_U16_MIN;
 *
 * MLIB_INT case notes:
 *      ranges for all values supposed to be [MLIB_S32_MIN; MLIB_S32_MAX]
 *      if R==G==B then H = S=MLIB_S32_MIN;
 *
 * RESTRICTION
 *      src and dst must be the same type and the same number
 *      of channels.
 *      They must be 3-channel images.
 *      They can be in MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 *
 * DESCRIPTION
 *      The center of the source image is mapped to the center of the
 *      destination image.
 *
 *      data values are mapped to next ranges
 *      0 <= H < 1
 *      0 <= S, L,V, R,G, B,P <= 1
 *
 *      P = min(R, G,B)
 *      V = max(R, G,B)
 *
 *      L = (V + P) / 2
 *
 *      S = | (V - P) / (V + P)      if L <= 0.5
 *          | (V - P) / (2 - V - P)  if L <= 0.5
 *
 *          | (5 + (V-B)/(V-P))/6  if R==V and G==P
 *          | (1 + (V-G)/(V-P))/6  if R==V and B==P
 *      H = | (1 + (V-R)/(V-P))/6  if G==V and B==P
 *          | (3 + (V-B)/(V-P))/6  if G==V and R==P
 *          | (3 + (V-G)/(V-P))/6  if B==V and R==P
 *          | (5 + (V-R)/(V-P))/6  if B==V and G==P
 *
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_ImageCopy.h>
#include <mlib_ImageCheck.h>
#include <mlib_SysMath.h>
#include <mlib_ImageDivTables.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageColorRGB2HSL = __mlib_ImageColorRGB2HSL

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageColorRGB2HSL) mlib_ImageColorRGB2HSL
    __attribute__((weak, alias("__mlib_ImageColorRGB2HSL")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	BUFF_SIZE	256

/* *********************************************************** */

#define	VIS_LD_U16_I(X, Y)	vis_ld_u16_i((void *)(X), (Y))

/* *********************************************************** */

static void mlib_v_ImageColorRGB2HSL_U8(
    mlib_image *dst,
    const mlib_image *src);

static void mlib_v_ImageColorRGB2HSL_S16(
    mlib_image *dst,
    const mlib_image *src);

static void mlib_v_ImageColorRGB2HSL_U16(
    mlib_image *dst,
    const mlib_image *src);

static void mlib_v_ImageColorRGB2HSL_S32(
    mlib_image *dst,
    const mlib_image *src);

/* *********************************************************** */

mlib_status
__mlib_ImageColorRGB2HSL(
    mlib_image *dst,
    const mlib_image *src)
{
/*  check for obvious errors  */
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_TYPE_EQUAL(dst, src);
	MLIB_IMAGE_CHAN_EQUAL(dst, src);
	MLIB_IMAGE_HAVE_CHAN(dst, 3);

	switch (mlib_ImageGetType(dst)) {
/*  handle MLIB_BYTE data type of image  */
	case MLIB_BYTE:
		mlib_v_ImageColorRGB2HSL_U8(dst, src);
		break;
/*  handle MLIB_SHORT data type of image  */
	case MLIB_SHORT:
		mlib_v_ImageColorRGB2HSL_S16(dst, src);
		break;
/*  handle MLIB_USHORT data type of image  */
	case MLIB_USHORT:
		mlib_v_ImageColorRGB2HSL_U16(dst, src);
		break;
/*  handle MLIB_INT data type of image  */
	case MLIB_INT:
		mlib_v_ImageColorRGB2HSL_S32(dst, src);
		break;
/*  discard any other data types  */
	default:
		return (MLIB_FAILURE);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	dw	sw
#define	dh	sh

#define	PREPAREVARS(data_type)                                     \
	mlib_s32 sstride =                                         \
	mlib_ImageGetStride(src) / sizeof (data_type);             \
	    mlib_s32 dstride =                                     \
		mlib_ImageGetStride(dst) / sizeof (data_type);     \
	    mlib_s32 ws_ = mlib_ImageGetWidth(src);                \
	    mlib_s32 hs_ = mlib_ImageGetHeight(src);               \
	    mlib_s32 wd_ = mlib_ImageGetWidth(dst);                \
	    mlib_s32 hd_ = mlib_ImageGetHeight(dst);               \
	    mlib_s32 sw = (ws_ < wd_) ? ws_ : wd_;                 \
	    mlib_s32 sh = (hs_ < hd_) ? hs_ : hd_;                 \
	    mlib_s32 dx = ((mlib_s32)ws_ - (mlib_s32)wd_) >> 1;    \
	    mlib_s32 dy = ((mlib_s32)hs_ - (mlib_s32)hd_) >> 1;    \
	    mlib_s32 dxs = ((dx > 0) ? dx : 0);                    \
	    mlib_s32 dxd = ((dx > 0) ? 0 : -dx);                   \
	    mlib_s32 dys = ((dy > 0) ? dy : 0);                    \
	    mlib_s32 dyd = ((dy > 0) ? 0 : -dy);                   \
	    data_type *sdata =                                     \
		(data_type *) mlib_ImageGetData(src) + dxs * 3 +   \
	    sstride * dys;                                         \
	    data_type *ddata =                                     \
		(data_type *) mlib_ImageGetData(dst) + dxd * 3 +   \
	    dstride * dyd;                                         \
	    data_type *ps = sdata;                                 \
	    data_type *pd = ddata;                                 \
	    mlib_s32 i, j

/* *********************************************************** */

#define	MASK_SEPARATE_S16	0x016789EF
#define	MASK_ALIGN_S16	0x6789ABCD

/* *********************************************************** */

#define	CHANNELSEPARATE_S16(sd0, sd1, sd2, rdd, gdd, bdd)        \
	sda = vis_faligndata(sd1, sd2);                          \
	rdd = vis_bshuffle(sd0, sda);                            \
	sdb = vis_faligndata(sd0, sd1);                          \
	bdd = vis_bshuffle(sdb, sd2);                            \
	sda = vis_fpmerge(vis_read_hi(sd0), vis_read_lo(sd1));   \
	sdb = vis_fpmerge(vis_read_hi(sd1), vis_read_lo(sd2));   \
	sdc = vis_fpmerge(vis_read_lo(sda), vis_read_hi(sdb));   \
	gdd = vis_fpmerge(vis_read_hi(sdc), vis_read_lo(sdc))

/* *********************************************************** */

#define	CHANNELSEPARATE_U8(sd0, sd1, sd2, rdd, gdd, bdd)         \
	sda = vis_fpmerge(vis_read_hi(sd0), vis_read_lo(sd1));   \
	sdb = vis_fpmerge(vis_read_lo(sd0), vis_read_hi(sd2));   \
	sdc = vis_fpmerge(vis_read_hi(sd1), vis_read_lo(sd2));   \
	sdd = vis_fpmerge(vis_read_hi(sda), vis_read_lo(sdb));   \
	sde = vis_fpmerge(vis_read_lo(sda), vis_read_hi(sdc));   \
	rdd = vis_fpmerge(vis_read_hi(sdd), vis_read_lo(sde));   \
	sde = vis_fpmerge(vis_read_hi(sdb), vis_read_lo(sdc));   \
	gdd = vis_fpmerge(vis_read_lo(sdd), vis_read_hi(sde));   \
	sdd = vis_fpmerge(vis_read_lo(sda), vis_read_hi(sdc));   \
	bdd = vis_fpmerge(vis_read_hi(sdd), vis_read_lo(sde))

/* *********************************************************** */

void
mlib_v_ImageColorRGB2HSL_U8(
    mlib_image *dst,
    const mlib_image *src)
{
	mlib_d64 buffColorConvert1[BUFF_SIZE + 4];
	mlib_d64 *buff1, *pbuff_arr1 = buffColorConvert1;
	mlib_d64 one_third = vis_to_double_dup(0x00550055);
	mlib_d64 two_third = vis_to_double_dup(0x00ab00ab);
	mlib_d64 rounder = vis_to_double_dup(0x7f007f);
	mlib_d64 halfer = vis_to_double_dup(0x80008000);
	mlib_d64 comparer = vis_to_double_dup(0x00ff00ff);
	mlib_d64 reverser = vis_to_double_dup(0x03fc03fc);
	mlib_u8 *ps0;
	mlib_u8 *pd0;
	mlib_s32 dsize, n, k, dp_aligned;

	PREPAREVARS(mlib_u8);

	ps0 = ps;
	pd0 = pd;
	vis_write_gsr((8 << 3) | 4);

	for (j = 0; j < dh; ++j) {
		for (i = 0; i < dw && ((mlib_addr)ps & 7); i++) {
			mlib_u8 r = ps[i * 3 + 0];
			mlib_u8 g = ps[i * 3 + 1];
			mlib_u8 b = ps[i * 3 + 2];
			mlib_u8 v, minim;
			mlib_u32 fh, delta, sum, sumind, sub, s, div2;
			mlib_u32 shft = 43690;
			mlib_s32 dmin = r - g;

			v = minim = r;

			if (r > g)
				minim = g;
			else
				v = g;

			if (minim > b)
				minim = b;

			if (b > v)
				v = b;
			sum = v + minim;
			sumind =
			    ((sum ^ (((mlib_s32)sum << 23) >> 31)) -
			    (sum >> 8)) & 0xff;
			sub = v - minim;
			delta = mlib_div6_tab[sub];

			if (r == v) {
				dmin = g - b;
				shft = 0;
			} else if (g == v) {
				dmin = b - r;
				shft = 21845;
			}

			div2 = mlib_div1_tab[sumind];
			fh = (shft + dmin * (mlib_s32)delta) >> 8;
			s = ((mlib_s32)sub * (mlib_s32)div2 + 0x80) >> 8;

			pd[i * 3 + 2] = sum >> 1;
			pd[i * 3 + 0] = fh;
			pd[i * 3 + 1] = s;
		}

		dsize = (dw - i) >> 3;
		dp_aligned = (((mlib_addr)pd & 7) == 0);

/* loop on buffer size */
		while (dsize > 0) {

			if (dsize > BUFF_SIZE / 3) {
				n = BUFF_SIZE / 3;
			} else {
				n = dsize;
			}

			dsize -= n;
			i += (n << 3);

			if (dp_aligned)
				buff1 = (mlib_d64 *)pd;
			else
				buff1 = pbuff_arr1;

#pragma pipeloop(0)
			for (k = 0; k < n; k++) {
				mlib_d64 r, rl, g, gl, b, bl, r1, g1, b1;
				mlib_d64 v, minim, sum, vsubminim, ind, revind;
				mlib_d64 sh, sl, hh, hl, h0, l0;
				mlib_d64 delta, vdiv, mida, midb;
				mlib_d64 sda, sdb, sdc, sdd, sde;
				mlib_d64 shft = vis_fzero();
				mlib_s32 mask1, mask2, mask3, mask4, mask5;
				mlib_u16 c0, c1, c2, c3;
				mlib_u8 a0, a1, a2, a3;
				mlib_d64 mult = vis_to_double_dup(0x01000100);
				mlib_u64 ll0, ll1;
				mlib_d64 pdd0, pdd1, pdd2, pdd3, pdd4;

				rl = ((mlib_d64 *)ps)[0];
				gl = ((mlib_d64 *)ps)[1];
				bl = ((mlib_d64 *)ps)[2];
				CHANNELSEPARATE_U8(rl, gl, bl, r1, g1, b1);

				r = vis_fmul8x16(vis_read_hi(r1), mult);
				g = vis_fmul8x16(vis_read_hi(g1), mult);
				b = vis_fmul8x16(vis_read_hi(b1), mult);

				v = r;
				mida = g;
				minim = g;
				midb = b;

				mask1 = vis_fcmpgt16(g, r);
				mask2 = vis_fcmpgt16(b, g);
				mask3 = vis_fcmpgt16(r, b);
				mask4 = mask2 & ~mask3;

				vis_pst_16(g, (void *)&v, mask1);
				vis_pst_16(b, (void *)&v, mask4);
				vis_pst_16(r, (void *)&minim, mask1);
				vis_pst_16(b, (void *)&minim, ~mask2 & mask3);
				vis_pst_16(b, (void *)&mida, mask1);
				vis_pst_16(r, (void *)&mida, mask4);
				vis_pst_16(r, (void *)&midb, mask1);
				vis_pst_16(g, (void *)&midb, mask4);

				sum = vis_fpadd16(v, minim);
				vsubminim = vis_fpsub16(v, minim);

				vis_pst_16(one_third, (void *)&shft, mask1);
				vis_pst_16(two_third, (void *)&shft, mask4);

				ll1 = ((mlib_u64 *)&vsubminim)[0];

				ind = vis_fpadd16(sum, sum);
				mask5 = vis_fcmpgt16(sum, comparer);
				l0 = vis_fpadd16(vis_fmul8sux16(halfer, sum),
				    sum);
				revind = vis_fpsub16(reverser, ind);
				vis_pst_16(revind, (void *)&ind, mask5);

				a3 = (mlib_u8)ll1;
				a2 = (mlib_u8)(ll1 >> 16);
				a1 = (mlib_u8)(ll1 >> 32);
				a0 = (mlib_u8)(ll1 >> 48);

				ll0 = ((mlib_u64 *)&ind)[0];
				c3 = (mlib_u16)(ll0 & 0xffff);
				c2 = (mlib_u16)((ll0 >> 16) & 0xffff);
				c1 = (mlib_u16)((ll0 >> 32) & 0xffff);
				c0 = (mlib_u16)((ll0 >> 48) & 0xffff);

				vis_write_bmask(0x4657cedf, 0);

				delta =
				    vis_bshuffle(vis_fpmerge(vis_read_lo
				    (VIS_LD_U16_I(mlib_div6_tab, a0 << 1)),
				    vis_read_lo(VIS_LD_U16_I(mlib_div6_tab,
				    a1 << 1))),
				    vis_fpmerge(vis_read_lo(VIS_LD_U16_I
				    (mlib_div6_tab, a2 << 1)),
				    vis_read_lo(VIS_LD_U16_I(mlib_div6_tab,
				    a3 << 1))));

				vdiv =
				    vis_bshuffle(vis_fpmerge(vis_read_lo
				    (VIS_LD_U16_I(mlib_div1_tab, c0)),
				    vis_read_lo(VIS_LD_U16_I(mlib_div1_tab,
				    c1))),
				    vis_fpmerge(vis_read_lo(VIS_LD_U16_I
				    (mlib_div1_tab, c2)),
				    vis_read_lo(VIS_LD_U16_I(mlib_div1_tab,
				    c3))));

				sl = vis_fmuld8ulx16(vis_read_lo(vsubminim),
				    vis_read_lo(vdiv));
				sh = vis_fmuld8ulx16(vis_read_hi(vsubminim),
				    vis_read_hi(vdiv));

				sl = vis_fpadd32(sl, rounder);
				sh = vis_fpadd32(sh, rounder);

				hl = vis_fpsub32(vis_fmuld8ulx16(vis_read_lo
				    (mida), vis_read_lo(delta)),
				    vis_fmuld8ulx16(vis_read_lo(midb),
				    vis_read_lo(delta)));
				hh = vis_fpsub32(vis_fmuld8ulx16(vis_read_hi
				    (mida), vis_read_hi(delta)),
				    vis_fmuld8ulx16(vis_read_hi(midb),
				    vis_read_hi(delta)));

				h0 = vis_fpackfix_pair(hh, hl);
				h0 = vis_fpadd16(h0, shft);
				pdd0 = vis_fpackfix_pair(sh, sl);
				vis_write_bmask(0x13579bdf, 0);
				pdd0 = vis_bshuffle(h0, pdd0);
				vis_write_bmask(0x04915b26, 0);
				pdd1 = vis_bshuffle(pdd0, l0);
				vis_write_bmask(0xd37fd37f, 0);
				pdd2 = vis_bshuffle(pdd0, l0);
				buff1[0] = pdd1;

				shft = vis_fzero();

				r = vis_fmul8x16(vis_read_lo(r1), mult);
				g = vis_fmul8x16(vis_read_lo(g1), mult);
				b = vis_fmul8x16(vis_read_lo(b1), mult);

				v = r;
				mida = g;
				minim = g;
				midb = b;

				mask1 = vis_fcmpgt16(g, r);
				mask2 = vis_fcmpgt16(b, g);
				mask3 = vis_fcmpgt16(r, b);
				mask4 = mask2 & ~mask3;

				vis_pst_16(g, (void *)&v, mask1);
				vis_pst_16(b, (void *)&v, mask4);
				vis_pst_16(r, (void *)&minim, mask1);
				vis_pst_16(b, (void *)&minim, ~mask2 & mask3);
				vis_pst_16(b, (void *)&mida, mask1);
				vis_pst_16(r, (void *)&mida, mask4);
				vis_pst_16(r, (void *)&midb, mask1);
				vis_pst_16(g, (void *)&midb, mask4);

				sum = vis_fpadd16(v, minim);
				vsubminim = vis_fpsub16(v, minim);

				vis_pst_16(one_third, (void *)&shft, mask1);
				vis_pst_16(two_third, (void *)&shft, mask4);

				ind = vis_fpadd16(sum, sum);
				mask5 = vis_fcmpgt16(sum, comparer);
				l0 = vis_fpadd16(vis_fmul8sux16(halfer, sum),
				    sum);
				revind = vis_fpsub16(reverser, ind);
				vis_pst_16(revind, (void *)&ind, mask5);

				ll0 = ((mlib_u64 *)&ind)[0];
				c3 = (mlib_u16)(ll0 & 0xffff);
				c2 = (mlib_u16)((ll0 >> 16) & 0xffff);
				c1 = (mlib_u16)((ll0 >> 32) & 0xffff);
				c0 = (mlib_u16)((ll0 >> 48) & 0xffff);

				ll1 = ((mlib_u64 *)&vsubminim)[0];
				a3 = (mlib_u8)ll1;
				a2 = (mlib_u8)(ll1 >> 16);
				a1 = (mlib_u8)(ll1 >> 32);
				a0 = (mlib_u8)(ll1 >> 48);

				vis_write_bmask(0x4657cedf, 0);

				delta =
				    vis_bshuffle(vis_fpmerge(vis_read_lo
				    (VIS_LD_U16_I(mlib_div6_tab, a0 << 1)),
				    vis_read_lo(VIS_LD_U16_I(mlib_div6_tab,
				    a1 << 1))),
				    vis_fpmerge(vis_read_lo(VIS_LD_U16_I
				    (mlib_div6_tab, a2 << 1)),
				    vis_read_lo(VIS_LD_U16_I(mlib_div6_tab,
				    a3 << 1))));

				vdiv =
				    vis_bshuffle(vis_fpmerge(vis_read_lo
				    (VIS_LD_U16_I(mlib_div1_tab, c0)),
				    vis_read_lo(VIS_LD_U16_I(mlib_div1_tab,
				    c1))),
				    vis_fpmerge(vis_read_lo(VIS_LD_U16_I
				    (mlib_div1_tab, c2)),
				    vis_read_lo(VIS_LD_U16_I(mlib_div1_tab,
				    c3))));

				sl = vis_fmuld8ulx16(vis_read_lo(vsubminim),
				    vis_read_lo(vdiv));
				sh = vis_fmuld8ulx16(vis_read_hi(vsubminim),
				    vis_read_hi(vdiv));

				sl = vis_fpadd32(sl, rounder);
				sh = vis_fpadd32(sh, rounder);

				hl = vis_fpsub32(vis_fmuld8ulx16(vis_read_lo
				    (mida), vis_read_lo(delta)),
				    vis_fmuld8ulx16(vis_read_lo(midb),
				    vis_read_lo(delta)));
				hh = vis_fpsub32(vis_fmuld8ulx16(vis_read_hi
				    (mida), vis_read_hi(delta)),
				    vis_fmuld8ulx16(vis_read_hi(midb),
				    vis_read_hi(delta)));

				h0 = vis_fpackfix_pair(hh, hl);
				h0 = vis_fpadd16(h0, shft);
				pdd0 = vis_fpackfix_pair(sh, sl);
				vis_write_bmask(0x13579bdf, 0);
				pdd0 = vis_bshuffle(h0, pdd0);
				vis_write_bmask(0x04915b26, 0);
				pdd3 = vis_bshuffle(pdd0, l0);
				vis_write_bmask(0xd37fd37f, 0);
				pdd4 = vis_bshuffle(pdd0, l0);
				buff1[1] = vis_faligndata(pdd2, pdd3);
				buff1[2] = vis_faligndata(pdd3, pdd4);

				ps += 24;
				buff1 += 3;
			}

			if (!dp_aligned) {
				buff1 = pbuff_arr1;
				mlib_ImageCopy_na((mlib_u8 *)buff1,
				    (mlib_u8 *)pd, (n << 4) + (n << 3));
				vis_alignaddr((void *)0, 4);
			}

			pd += (n << 4) + (n << 3);
		}

#pragma pipeloop(0)
		for (; i < dw; i++) {
			mlib_u8 r = ps[0];
			mlib_u8 g = ps[1];
			mlib_u8 b = ps[2];
			mlib_u8 v, minim;
			mlib_u32 fh, delta, sum, sumind, sub, s, div2;
			mlib_u32 shft = 43690;
			mlib_s32 dmin = r - g;

			v = minim = r;

			if (r > g)
				minim = g;
			else
				v = g;

			if (minim > b)
				minim = b;

			if (b > v)
				v = b;
			sum = v + minim;
			sumind =
			    ((sum ^ (((mlib_s32)sum << 23) >> 31)) -
			    (sum >> 8)) & 0xff;
			sub = v - minim;
			delta = mlib_div6_tab[sub];

			div2 = mlib_div1_tab[sumind];
			s = ((mlib_s32)sub * (mlib_s32)div2 + 0x80) >> 8;

			if (r == v) {
				dmin = g - b;
				shft = 0;
			} else if (g == v) {
				dmin = b - r;
				shft = 21845;
			}

			fh = (shft + dmin * (mlib_s32)delta) >> 8;

			pd[0] = fh;
			pd[1] = s;
			pd[2] = sum >> 1;
			ps += 3;
			pd += 3;
		}

		ps0 += sstride;
		pd0 += dstride;
		ps = ps0;
		pd = pd0;
	}
}

/* *********************************************************** */

void
mlib_v_ImageColorRGB2HSL_S16(
    mlib_image *dst,
    const mlib_image *src)
{
	mlib_d64 one_third = vis_to_double_dup(0xd556d556);
	mlib_d64 two_third = vis_to_double_dup(0x2aaa2aaa);
	mlib_d64 non_third = vis_to_double_dup(0x80008000);
	mlib_d64 non_thirdp = vis_fnot(non_third);
	mlib_d64 undefiner = vis_fpsub16(non_third, vis_fone());
	mlib_d64 reverser = vis_to_double_dup(0x0000fffe);
	mlib_d64 unsigner32 = vis_to_double_dup(0x00008000);
	mlib_d64 unsigner32x2 = vis_fpadd32(unsigner32, unsigner32);
	mlib_f32 f_one = vis_to_float(0x00010001);
	mlib_s16 *ps0;
	mlib_s16 *pd0;
	mlib_d64 *psd;

	PREPAREVARS(mlib_s16);

	ps0 = ps;
	pd0 = pd;
	vis_write_gsr((15 << 3) | 4);

	for (j = 0; j < dh; ++j) {

#pragma unroll(1)
		for (i = 0; (i < dw) && ((mlib_addr)ps & 7); i++) {
			mlib_s32 r = ps[0];
			mlib_s32 g = ps[1];
			mlib_s32 b = ps[2];
			mlib_s32 v, minim, h, s;
			mlib_s32 delta, vdiv, sum;
			mlib_f32 fh, rdelta;
			mlib_f32 shft = 0.666666667f;
			mlib_s32 dmin = (r - g);

			v = minim = r;

			if (r > g)
				minim = g;
			else
				v = g;

			if (b > v)
				v = b;

			if (minim > b)
				minim = b;

			if (v == minim) {
				pd[2] = v;
				pd[1] = MLIB_S16_MIN;
				pd[0] = MLIB_S16_MIN;
				ps += 3;
				pd += 3;
				continue;
			}

			if (r == v) {
				shft = 1.0f;
				dmin = (g - b);
			} else if (g == v) {
				shft = 0.333333333f;
				dmin = (b - r);
			}

			delta = (v - minim);
			sum = v + minim + 1;
			vdiv = ((sum >> 31) ^ sum) - (sum >> 31);
			vdiv = 65535 - vdiv;
			rdelta = (1.0f / 6.0f) / delta;
			s = (mlib_s32)((((mlib_u32)delta << 16) -
			    delta) / (mlib_f32)vdiv) - 32768;
			fh = shft + (mlib_f32)dmin *rdelta;

			h = ((mlib_s32)(fh * 65536.0f)) - 32768;

			pd[2] = sum >> 1;
			pd[1] = s;
			pd[0] = h;

			ps += 3;
			pd += 3;
		}

		psd = (mlib_d64 *)ps;
		vis_write_bmask(MASK_SEPARATE_S16, 0);

#pragma pipeloop(0)
		for (; i <= (dw - 4); i += 4) {
			mlib_d64 r, rl, g, gl, b, bl, sh, sl, s1, s2, hl, hh,
			    h1, h2;
			mlib_d64 v, v0, minim, vl, vh, miniml, minimh, v1, v2,
			    v3;
			mlib_d64 vsubminimh, vsubminiml, shft, shfth, shftl;
			mlib_d64 suml, sumh, rsuml, rsumh;
			mlib_d64 midh, midl, mida, midah, midal, midb, midbh,
			    midbl;
			mlib_s32 mask1, mask2, mask3, mask4, mask5;
			mlib_f32 fa0, fa1, fa2, fa3, fb0, fb1, fb2, fb3;
			mlib_d64 sda, sdb, sdc;
			mlib_d64 m_v, m_minim, m_mida, m_midb, m_shft;

/*  for R== max  */
			m_shft = non_third;

			rl = psd[0];
			gl = psd[1];
			bl = psd[2];

			CHANNELSEPARATE_S16(rl, gl, bl, r, g, b);

			m_v = r;
			m_mida = g;
			m_minim = g;
			m_midb = b;

			mask1 = vis_fcmpgt16(g, r);
			mask2 = vis_fcmpgt16(b, g);
			mask3 = vis_fcmpgt16(r, b);
			mask4 = mask2 & ~mask3;

			vis_write_bmask(MASK_ALIGN_S16, 0);

			vis_pst_16(r, (void *)&m_minim, mask1);
			vis_pst_16(b, (void *)&m_minim, ~mask2 & mask3);
			vis_pst_16(g, (void *)&m_v, mask1);
			vis_pst_16(b, (void *)&m_v, mask4);
			vis_pst_16(b, (void *)&m_mida, mask1);
			vis_pst_16(r, (void *)&m_mida, mask4);
			vis_pst_16(r, (void *)&m_midb, mask1);
			vis_pst_16(g, (void *)&m_midb, mask4);
			mask5 = ~(mask1 | mask2 | mask3);

			vis_pst_16(one_third, (void *)&m_shft, mask1);
			vis_pst_16(non_thirdp, (void *)&m_shft, mask2 & mask3);
			vis_pst_16(two_third, (void *)&m_shft, mask4);
			vis_pst_16(undefiner, (void *)&m_shft, mask5);
			v = m_v;
			minim = m_minim;

			vl = vis_fmuld8ulx16(f_one, vis_read_lo(v));
			miniml = vis_fmuld8ulx16(f_one, vis_read_lo(minim));
			vh = vis_fmuld8ulx16(f_one, vis_read_hi(v));
			minimh = vis_fmuld8ulx16(f_one, vis_read_hi(minim));
			suml = vis_fpadd32(vl, miniml);
			sumh = vis_fpadd32(vh, minimh);
			mask1 = vis_fcmpgt32(suml, vis_fone());
			mask2 = vis_fcmpgt32(sumh, vis_fone());
			v0 = vis_fpackfix_pair(sumh, suml);
			rsuml = vis_fpsub32(reverser, suml);
			rsumh = vis_fpsub32(reverser, sumh);
			suml = vis_fpadd32(unsigner32x2, suml);
			sumh = vis_fpadd32(unsigner32x2, sumh);
			vis_pst_32(rsuml, (void *)&suml, mask1);
			vis_pst_32(undefiner, (void *)&suml, mask5);
			vis_pst_32(rsumh, (void *)&sumh, mask2);
			vis_pst_32(undefiner, (void *)&sumh, mask5 >> 2);

			fb3 = 65535.5f / (mlib_f32)((mlib_s32 *)&suml)[1];
			fb2 = 65535.5f / (mlib_f32)((mlib_s32 *)&suml)[0];
			fb1 = 65535.5f / (mlib_f32)((mlib_s32 *)&sumh)[1];
			fb0 = 65535.5f / (mlib_f32)((mlib_s32 *)&sumh)[0];

			vsubminiml = vis_fpsub32(vl, miniml);
			vsubminimh = vis_fpsub32(vh, minimh);
			fa3 =
			    10922.6667f /
			    (mlib_f32)((mlib_s32 *)&vsubminiml)[1];
			fa2 =
			    10922.6667f /
			    (mlib_f32)((mlib_s32 *)&vsubminiml)[0];
			fa1 =
			    10922.6667f /
			    (mlib_f32)((mlib_s32 *)&vsubminimh)[1];
			fa0 =
			    10922.6667f /
			    (mlib_f32)((mlib_s32 *)&vsubminimh)[0];

			sl = vis_to_double((mlib_s32)((mlib_f32)((mlib_s32 *)
			    &vsubminiml)[0] * fb2),
			    (mlib_s32)((mlib_f32)((mlib_s32 *)&vsubminiml)[1] *
			    fb3));
			sh = vis_to_double((mlib_s32)((mlib_f32)((mlib_s32 *)
			    &vsubminimh)[0] * fb0),
			    (mlib_s32)((mlib_f32)((mlib_s32 *)&vsubminimh)[1] *
			    fb1));

			v1 = vis_bshuffle(v0, v0);
			v2 = vis_bshuffle(v1, v1);
			v3 = vis_bshuffle(v2, v2);

			vis_st_u16_i(v0, pd, 22);
			vis_st_u16_i(v1, pd, 16);
			vis_st_u16_i(v2, pd, 10);
			vis_st_u16_i(v3, pd, 4);

			sl = vis_fpsub32(sl, unsigner32);
			sh = vis_fpsub32(sh, unsigner32);
			s1 = vis_write_lo(s1, vis_read_hi(sl));
			s2 = vis_write_lo(s2, vis_read_hi(sh));
			vis_st_u16_i(sl, pd, 20);
			vis_st_u16_i(s1, pd, 14);
			vis_st_u16_i(sh, pd, 8);
			vis_st_u16_i(s2, pd, 2);

			mida = m_mida;
			midb = m_midb;
			shft = m_shft;
			midal = vis_fmuld8ulx16(f_one, vis_read_lo(mida));
			midbl = vis_fmuld8ulx16(f_one, vis_read_lo(midb));
			midah = vis_fmuld8ulx16(f_one, vis_read_hi(mida));
			midbh = vis_fmuld8ulx16(f_one, vis_read_hi(midb));
			midl = vis_fpsub32(midal, midbl);
			midh = vis_fpsub32(midah, midbh);
			shftl = vis_fmuld8ulx16(f_one, vis_read_lo(shft));
			shfth = vis_fmuld8ulx16(f_one, vis_read_hi(shft));

			vis_write_bmask(MASK_SEPARATE_S16, 0);

			hl = vis_to_double((mlib_s32)(fa2 *
			    (mlib_f32)((mlib_s32 *)&midl)[0]),
			    (mlib_s32)(fa3 * (mlib_f32)((mlib_s32 *)&midl)[1]));
			hh = vis_to_double((mlib_s32)(fa0 *
			    (mlib_f32)((mlib_s32 *)&midh)[0]),
			    (mlib_s32)(fa1 * (mlib_f32)((mlib_s32 *)&midh)[1]));
			hl = vis_fpadd32(hl, shftl);
			hh = vis_fpadd32(hh, shfth);
			h1 = vis_write_lo(h1, vis_read_hi(hl));
			h2 = vis_write_lo(h2, vis_read_hi(hh));
			vis_st_u16(h2, pd);
			vis_st_u16_i(h1, pd, 12);
			vis_st_u16_i(hh, pd, 6);
			vis_st_u16_i(hl, pd, 18);

			psd += 3;
			pd += 12;
		}

		ps = (mlib_s16 *)(psd);
#pragma unroll(1)
		for (; i < dw; i++) {
			mlib_s32 r = ps[0];
			mlib_s32 g = ps[1];
			mlib_s32 b = ps[2];
			mlib_s32 v, minim, h, s;
			mlib_s32 delta, vdiv, sum;
			mlib_f32 fh, rdelta;
			mlib_f32 shft = 0.666666667f;
			mlib_s32 dmin = (r - g);

			v = minim = r;

			if (r > g)
				minim = g;
			else
				v = g;

			if (b > v)
				v = b;

			if (minim > b)
				minim = b;

			if (v == minim) {
				pd[2] = v;
				pd[1] = MLIB_S16_MIN;
				pd[0] = MLIB_S16_MIN;
				ps += 3;
				pd += 3;
				continue;
			}

			if (r == v) {
				shft = 1.0f;
				dmin = (g - b);
			} else if (g == v) {
				shft = 0.333333333f;
				dmin = (b - r);
			}

			delta = (v - minim);
			sum = v + minim + 1;
			vdiv = ((sum >> 31) ^ sum) - (sum >> 31);
			vdiv = 65535 - vdiv;
			rdelta = (1.0f / 6.0f) / delta;
			s = (mlib_s32)((((mlib_u32)delta << 16) -
			    delta) / (mlib_f32)vdiv) - 32768;
			fh = shft + (mlib_f32)dmin *rdelta;

			h = ((mlib_s32)(fh * 65536.0f)) - 32768;

			pd[2] = sum >> 1;
			pd[1] = s;
			pd[0] = h;

			ps += 3;
			pd += 3;
		}

		ps0 += sstride;
		pd0 += dstride;
		ps = ps0;
		pd = pd0;
	}
}

/* *********************************************************** */

void
mlib_v_ImageColorRGB2HSL_U16(
    mlib_image *dst,
    const mlib_image *src)
{
	mlib_d64 one_third = vis_to_double_dup(0xd556d556);
	mlib_d64 two_third = vis_to_double_dup(0x2aaa2aaa);
	mlib_d64 non_third = vis_to_double_dup(0x80008000);
	mlib_d64 non_thirdp = vis_fnot(non_third);
	mlib_d64 undefiner = vis_fpsub16(non_third, vis_fone());
	mlib_d64 reverser = vis_to_double_dup(0x0000fffe);
	mlib_d64 unsigner32 = vis_to_double_dup(0x00008000);
	mlib_d64 unsigner32x2 = vis_fpadd32(unsigner32, unsigner32);
	mlib_f32 f_one = vis_to_float(0x00010001);
	mlib_u16 *ps0;
	mlib_u16 *pd0;
	mlib_d64 *psd;

	PREPAREVARS(mlib_u16);

	ps0 = ps;
	pd0 = pd;
	vis_write_gsr((15 << 3) | 4);

	for (j = 0; j < dh; ++j) {

#pragma unroll(1)
		for (i = 0; (i < dw) && ((mlib_addr)ps & 7); i++) {
			mlib_s32 r = ps[0] - 32768;
			mlib_s32 g = ps[1] - 32768;
			mlib_s32 b = ps[2] - 32768;
			mlib_s32 v, minim, h, s;
			mlib_s32 delta, vdiv, sum;
			mlib_f32 fh, rdelta;
			mlib_f32 shft = 0.666666667f;
			mlib_s32 dmin = (r - g);

			v = minim = r;

			if (r > g)
				minim = g;
			else
				v = g;

			if (b > v)
				v = b;

			if (minim > b)
				minim = b;

			if (v == minim) {
				pd[2] = v + 32768;
				pd[1] = MLIB_U16_MIN;
				pd[0] = MLIB_U16_MIN;
				ps += 3;
				pd += 3;
				continue;
			}

			if (r == v) {
				shft = 1.0f;
				dmin = (g - b);
			} else if (g == v) {
				shft = 0.333333333f;
				dmin = (b - r);
			}

			delta = (v - minim);
			sum = v + minim + 1;
			vdiv = ((sum >> 31) ^ sum) - (sum >> 31);
			vdiv = 65535 - vdiv;
			rdelta = (1.0f / 6.0f) / delta;
			s = (mlib_s32)((((mlib_u32)delta << 16) -
			    delta) / (mlib_f32)vdiv);
			fh = shft + (mlib_f32)dmin *rdelta;

			h = ((mlib_s32)(fh * 65536.0f));

			pd[2] = (sum >> 1) + 32768;
			pd[1] = s;
			pd[0] = h;

			ps += 3;
			pd += 3;
		}

		psd = (mlib_d64 *)ps;
		vis_write_bmask(MASK_SEPARATE_S16, 0);

#pragma pipeloop(0)
		for (; i <= (dw - 4); i += 4) {
			mlib_d64 r, rl, g, gl, b, bl, sh, sl, s1, s2, hl, hh,
			    h1, h2;
			mlib_d64 v, v0, minim, vl, vh, miniml, minimh, v1, v2,
			    v3;
			mlib_d64 vsubminimh, vsubminiml, shft, shfth, shftl;
			mlib_d64 suml, sumh, rsuml, rsumh;
			mlib_d64 midh, midl, mida, midah, midal, midb, midbh,
			    midbl;
			mlib_s32 mask1, mask2, mask3, mask4, mask5;
			mlib_f32 fa0, fa1, fa2, fa3, fb0, fb1, fb2, fb3;
			mlib_d64 sda, sdb, sdc;
			mlib_d64 m_v, m_minim, m_mida, m_midb, m_shft;
			mlib_d64 xor_mask = vis_to_double_dup(0x80008000);

/*  for R== max  */
			m_shft = non_third;
			rl = psd[0];
			gl = psd[1];
			bl = psd[2];
			CHANNELSEPARATE_S16(rl, gl, bl, r, g, b);

			r = vis_fxor(r, xor_mask);
			g = vis_fxor(g, xor_mask);
			b = vis_fxor(b, xor_mask);

			vis_write_bmask(MASK_ALIGN_S16, 0);

			m_v = r;
			m_mida = g;
			m_minim = g;
			m_midb = b;

			mask1 = vis_fcmpgt16(g, r);
			mask2 = vis_fcmpgt16(b, g);
			mask3 = vis_fcmpgt16(r, b);
			mask4 = mask2 & ~mask3;

			vis_pst_16(r, (void *)&m_minim, mask1);
			vis_pst_16(b, (void *)&m_minim, ~mask2 & mask3);
			vis_pst_16(g, (void *)&m_v, mask1);
			vis_pst_16(b, (void *)&m_v, mask4);
			vis_pst_16(b, (void *)&m_mida, mask1);
			vis_pst_16(r, (void *)&m_mida, mask4);
			vis_pst_16(r, (void *)&m_midb, mask1);
			vis_pst_16(g, (void *)&m_midb, mask4);
			vis_pst_16(one_third, (void *)&m_shft, mask1);
			mask5 = ~(mask1 | mask2 | mask3);

			vis_pst_16(non_thirdp, (void *)&m_shft, mask2 & mask3);
			v = m_v;
			minim = m_minim;
			vis_pst_16(two_third, (void *)&m_shft, mask4);

			vis_pst_16(undefiner, (void *)&m_shft, mask5);

			vl = vis_fmuld8ulx16(f_one, vis_read_lo(v));
			miniml = vis_fmuld8ulx16(f_one, vis_read_lo(minim));
			vh = vis_fmuld8ulx16(f_one, vis_read_hi(v));
			minimh = vis_fmuld8ulx16(f_one, vis_read_hi(minim));
			suml = vis_fpadd32(vl, miniml);
			sumh = vis_fpadd32(vh, minimh);
			mask1 = vis_fcmpgt32(suml, vis_fone());
			mask2 = vis_fcmpgt32(sumh, vis_fone());
			v0 = vis_fpackfix_pair(sumh, suml);
			rsuml = vis_fpsub32(reverser, suml);
			rsumh = vis_fpsub32(reverser, sumh);
			suml = vis_fpadd32(unsigner32x2, suml);
			sumh = vis_fpadd32(unsigner32x2, sumh);
			vis_pst_32(rsuml, (void *)&suml, mask1);
			vis_pst_32(undefiner, (void *)&suml, mask5);
			vis_pst_32(rsumh, (void *)&sumh, mask2);
			vis_pst_32(undefiner, (void *)&sumh, mask5 >> 2);

			fb3 = 65535.5f / (mlib_f32)((mlib_s32 *)&suml)[1];
			fb2 = 65535.5f / (mlib_f32)((mlib_s32 *)&suml)[0];
			fb1 = 65535.5f / (mlib_f32)((mlib_s32 *)&sumh)[1];
			fb0 = 65535.5f / (mlib_f32)((mlib_s32 *)&sumh)[0];

			vsubminiml = vis_fpsub32(vl, miniml);
			vsubminimh = vis_fpsub32(vh, minimh);
			fa3 =
			    10922.6667f /
			    (mlib_f32)((mlib_s32 *)&vsubminiml)[1];
			fa2 =
			    10922.6667f /
			    (mlib_f32)((mlib_s32 *)&vsubminiml)[0];
			fa1 =
			    10922.6667f /
			    (mlib_f32)((mlib_s32 *)&vsubminimh)[1];
			fa0 =
			    10922.6667f /
			    (mlib_f32)((mlib_s32 *)&vsubminimh)[0];

			sl = vis_to_double((mlib_s32)((mlib_f32)((mlib_s32 *)
			    &vsubminiml)[0] * fb2),
			    (mlib_s32)((mlib_f32)((mlib_s32 *)&vsubminiml)[1] *
			    fb3));
			sh = vis_to_double((mlib_s32)((mlib_f32)((mlib_s32 *)
			    &vsubminimh)[0] * fb0),
			    (mlib_s32)((mlib_f32)((mlib_s32 *)&vsubminimh)[1] *
			    fb1));

			v1 = vis_bshuffle(v0, v0);
			v2 = vis_bshuffle(v1, v1);
			v3 = vis_bshuffle(v2, v2);

			v0 = vis_fxor(v0, xor_mask);
			v1 = vis_fxor(v1, xor_mask);
			v2 = vis_fxor(v2, xor_mask);
			v3 = vis_fxor(v3, xor_mask);

			vis_st_u16_i(v0, pd, 22);
			vis_st_u16_i(v1, pd, 16);
			vis_st_u16_i(v2, pd, 10);
			vis_st_u16_i(v3, pd, 4);

			s1 = vis_write_lo(s1, vis_read_hi(sl));
			s2 = vis_write_lo(s2, vis_read_hi(sh));
			vis_st_u16_i(sl, pd, 20);
			vis_st_u16_i(s1, pd, 14);
			vis_st_u16_i(sh, pd, 8);
			vis_st_u16_i(s2, pd, 2);

			mida = m_mida;
			midb = m_midb;
			shft = m_shft;
			midal = vis_fmuld8ulx16(f_one, vis_read_lo(mida));
			midbl = vis_fmuld8ulx16(f_one, vis_read_lo(midb));
			midah = vis_fmuld8ulx16(f_one, vis_read_hi(mida));
			midbh = vis_fmuld8ulx16(f_one, vis_read_hi(midb));
			midl = vis_fpsub32(midal, midbl);
			midh = vis_fpsub32(midah, midbh);
			shftl = vis_fmuld8ulx16(f_one, vis_read_lo(shft));
			shfth = vis_fmuld8ulx16(f_one, vis_read_hi(shft));

			hl = vis_to_double((mlib_s32)(fa2 *
			    (mlib_f32)((mlib_s32 *)&midl)[0]),
			    (mlib_s32)(fa3 * (mlib_f32)((mlib_s32 *)&midl)[1]));
			hh = vis_to_double((mlib_s32)(fa0 *
			    (mlib_f32)((mlib_s32 *)&midh)[0]),
			    (mlib_s32)(fa1 * (mlib_f32)((mlib_s32 *)&midh)[1]));

			vis_write_bmask(MASK_SEPARATE_S16, 0);

			hl = vis_fpadd32(hl, shftl);
			hh = vis_fpadd32(hh, shfth);
			hl = vis_fxor(hl, xor_mask);
			hh = vis_fxor(hh, xor_mask);
			h1 = vis_write_lo(h1, vis_read_hi(hl));
			h2 = vis_write_lo(h2, vis_read_hi(hh));
			vis_st_u16(h2, pd);
			vis_st_u16_i(h1, pd, 12);
			vis_st_u16_i(hh, pd, 6);
			vis_st_u16_i(hl, pd, 18);

			psd += 3;
			pd += 12;
		}

		ps = (mlib_u16 *)(psd);
#pragma unroll(1)
		for (; i < dw; i++) {
			mlib_s32 r = ps[0] - 32768;
			mlib_s32 g = ps[1] - 32768;
			mlib_s32 b = ps[2] - 32768;
			mlib_s32 v, minim, h, s;
			mlib_s32 delta, vdiv, sum;
			mlib_f32 fh, rdelta;
			mlib_f32 shft = 0.666666667f;
			mlib_s32 dmin = (r - g);

			v = minim = r;

			if (r > g)
				minim = g;
			else
				v = g;

			if (b > v)
				v = b;

			if (minim > b)
				minim = b;

			if (v == minim) {
				pd[2] = v + 32768;
				pd[1] = MLIB_U16_MIN;
				pd[0] = MLIB_U16_MIN;
				ps += 3;
				pd += 3;
				continue;
			}

			if (r == v) {
				shft = 1.0f;
				dmin = (g - b);
			} else if (g == v) {
				shft = 0.333333333f;
				dmin = (b - r);
			}

			delta = (v - minim);
			sum = v + minim + 1;
			vdiv = ((sum >> 31) ^ sum) - (sum >> 31);
			vdiv = 65535 - vdiv;
			rdelta = (1.0f / 6.0f) / delta;
			s = (mlib_s32)((((mlib_u32)delta << 16) -
			    delta) / (mlib_f32)vdiv);
			fh = shft + (mlib_f32)dmin *rdelta;

			h = ((mlib_s32)(fh * 65536.0f));

			pd[2] = (sum >> 1) + 32768;
			pd[1] = s;
			pd[0] = h;

			ps += 3;
			pd += 3;
		}

		ps0 += sstride;
		pd0 += dstride;
		ps = ps0;
		pd = pd0;
	}
}

/* *********************************************************** */

void
mlib_v_ImageColorRGB2HSL_S32(
    mlib_image *dst,
    const mlib_image *src)
{
	PREPAREVARS(mlib_s32);

	for (j = 0; j < dh; ++j) {

#pragma pipeloop(0)
		for (i = 0; i < dw; ++i) {
			mlib_d64 r = ps[i * 3 + 0];
			mlib_d64 g = ps[i * 3 + 1];
			mlib_d64 b = ps[i * 3 + 2];
			mlib_d64 v, minim, h, s;
			mlib_d64 delta, vdiv, sum;
			mlib_d64 rdelta;
			mlib_d64 shft = 2.0 / 3.0;
			mlib_d64 dmin = r - g;

			v = minim = r;

			if (r > g)
				minim = g;

			if (r <= g)
				v = g;

			if (minim > b)
				minim = b;

			if (b > v)
				v = b;

			if (v == minim) {
				pd[i * 3 + 2] = ps[i * 3 + 0];
				pd[i * 3 + 1] = MLIB_S32_MIN;
				pd[i * 3 + 0] = MLIB_S32_MIN;
				continue;
			}

			if (r == v) {
				shft = 1.0;
				dmin = g - b;
			} else if (g == v) {
				shft = 1.0 / 3.0;
				dmin = b - r;
			}

			delta = (v - minim);
			rdelta = 1.0 / delta;
			rdelta *= (1.0 / 6.0);

			sum = v + minim;

			vdiv = (mlib_d64)(MLIB_U32_MAX) - mlib_fabs(sum + 1.);
			s = delta / vdiv;

			s = s * (mlib_d64)MLIB_U32_MAX + (mlib_d64)MLIB_S32_MIN;
			h = shft + dmin * rdelta;
			h = h - (mlib_s32)h;
			h = h * (mlib_d64)MLIB_U32_MAX + (mlib_d64)MLIB_S32_MIN;

			pd[i * 3 + 2] = sum * 0.5;
			pd[i * 3 + 1] = s;
			pd[i * 3 + 0] = h;
		}

		ps += sstride;
		pd += dstride;
	}
}

/* *********************************************************** */
