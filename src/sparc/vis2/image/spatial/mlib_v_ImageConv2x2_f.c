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

#pragma ident	"@(#)mlib_v_ImageConv2x2_f.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *      Internal functions for mlib_ImageConv2x2 on U8/S16/U16 types
 */

#include <vis_proto.h>
#include <mlib_image.h>
#include <mlib_c_ImageConv.h>

/* *********************************************************** */

#define	TYPE_64BIT	mlib_d64

/* *********************************************************** */

#define	LOAD_KERNEL_INTO_DOUBLE()                               \
	while (scalef_expon > 30) {                             \
	    scalef /= (1 << 30);                                \
	    scalef_expon -= 30;                                 \
	}                                                       \
	                                                        \
	scalef /= (1 << scalef_expon);                          \
/*                                                              \
 * keep kernel in regs                                          \
 */                                                             \
	k0 = scalef * kern[0];                                  \
	k1 = scalef * kern[1];                                  \
	k2 = scalef * kern[2];                                  \
	k3 = scalef * kern[3]

/* *********************************************************** */

#define	GET_SRC_DST_PARAMETERS(type)                            \
	hgt = mlib_ImageGetHeight(src);                         \
	wid = mlib_ImageGetWidth(src);                          \
	nchannel = mlib_ImageGetChannels(src);                  \
	sll = mlib_ImageGetStride(src) / sizeof (type);         \
	dll = mlib_ImageGetStride(dst) / sizeof (type);         \
	adr_src = (type *) mlib_ImageGetData(src);              \
	adr_dst = (type *) mlib_ImageGetData(dst)

/* *********************************************************** */

#define	CLAMP_S32(x)	((mlib_s32)(x))

/* *********************************************************** */

#define	STORE2(res0, res1)	dp[0 ] = res0; dp[chan1] = res1

/* *********************************************************** */

#define	LOAD_BUFF(buff)                                                \
	*(mlib_s64 *)(buff + i) =                                      \
	    (((mlib_s64)sp[0]) << 32) | S64TOS32((mlib_s64)sp[chan1])

/* *********************************************************** */

typedef union
{
	mlib_d64 d64;
	struct
	{
		mlib_s32 i0;
		mlib_s32 i1;
	} i32s;
	struct
	{
		mlib_f32 f0;
		mlib_f32 f1;
	} f32s;
} d64_2x32;

/* *********************************************************** */

#define	D_KER	1

#define	BUFF_LINE	256

/* *********************************************************** */

#define	DTYPE	mlib_s16
#define	S64TOS32(x)	((x) & 0xffffffff)

mlib_status
mlib_c_conv2x2nw_s16(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kern,
    mlib_s32 scalef_expon,
    mlib_s32 cmask)
{
	mlib_d64 buff_arr[2 * BUFF_LINE];
	mlib_s32 *pbuff = (mlib_s32 *)buff_arr, *buff0, *buff1, *buff2, *buffT;
	DTYPE *adr_src, *sl, *sp, *sl1;
	DTYPE *adr_dst, *dl, *dp;
	mlib_d64 k0, k1, k2, k3, scalef = 1.0;
	mlib_d64 p00, p01, p02, p10, p11, p12;
	mlib_s32 wid, hgt, sll, dll, wid1;
	mlib_s32 nchannel, chan1, chan2;
	mlib_s32 i, j, c;

	LOAD_KERNEL_INTO_DOUBLE();
	GET_SRC_DST_PARAMETERS(DTYPE);

	vis_write_gsr(16 << 3);

	wid1 = (wid + 1) & ~1;

	if (wid1 > BUFF_LINE) {
		pbuff = __mlib_malloc(3 * sizeof (mlib_s32) * wid1);

		if (pbuff == NULL)
			return (MLIB_FAILURE);
	}

	buff0 = pbuff;
	buff1 = buff0 + wid1;
	buff2 = buff1 + wid1;

	chan1 = nchannel;
	chan2 = chan1 + chan1;

	wid -= D_KER;
	hgt -= D_KER;

	for (c = 0; c < nchannel; c++) {
		if (!(cmask & (1 << (nchannel - 1 - c))))
			continue;

		sl = adr_src + c;
		dl = adr_dst + c;

		sl1 = sl + sll;

#pragma pipeloop(0)
		for (i = 0; i < wid + D_KER; i++) {
			buff0[i - 1] = (mlib_s32)sl[i * chan1];
			buff1[i - 1] = (mlib_s32)sl1[i * chan1];
		}

		sl += (D_KER + 1) * sll;

		for (j = 0; j < hgt; j++) {
			sp = sl;
			dp = dl;

			buff2[-1] = (mlib_s32)sp[0];
			sp += chan1;

			p02 = buff0[-1];
			p12 = buff1[-1];

#pragma pipeloop(0)
			for (i = 0; i <= (wid - 2); i += 2) {
				d64_2x32 sd0, sd1;
				d64_2x32 dd0, dd1;

				p00 = p02;
				p10 = p12;

				sd0.d64 = *(TYPE_64BIT *) (buff0 + i);
				sd1.d64 = *(TYPE_64BIT *) (buff1 + i);
				p01 = (mlib_d64)sd0.i32s.i0;
				p02 = (mlib_d64)sd0.i32s.i1;
				p11 = (mlib_d64)sd1.i32s.i0;
				p12 = (mlib_d64)sd1.i32s.i1;

				LOAD_BUFF(buff2);

				dd0.i32s.i0 =
				    CLAMP_S32(p00 * k0 + p01 * k1 + p10 * k2 +
				    p11 * k3);
				dd0.i32s.i1 =
				    CLAMP_S32(p01 * k0 + p02 * k1 + p11 * k2 +
				    p12 * k3);

				dd1.f32s.f0 = vis_fpackfix(dd0.d64);

				STORE2(dd1.i32s.i0 >> 16, dd1.i32s.i0);

				sp += chan2;
				dp += chan2;
			}

			for (; i < wid; i++) {
				d64_2x32 dd0, dd1;

				p00 = buff0[i - 1];
				p10 = buff1[i - 1];
				p01 = buff0[i];
				p11 = buff1[i];

				buff2[i] = (mlib_s32)sp[0];

				dd0.i32s.i1 =
				    CLAMP_S32(p00 * k0 + p01 * k1 + p10 * k2 +
				    p11 * k3);

				dd1.f32s.f0 = vis_fpackfix(dd0.d64);
				dp[0] = dd1.i32s.i0;

				sp += chan1;
				dp += chan1;
			}

			sl += sll;
			dl += dll;

			buffT = buff0;
			buff0 = buff1;
			buff1 = buff2;
			buff2 = buffT;
		}
	}

	if (pbuff != (mlib_s32 *)buff_arr)
		__mlib_free(pbuff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_c_conv2x2ext_s16(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_s32 *kern,
    mlib_s32 scalef_expon,
    mlib_s32 cmask)
{
	mlib_d64 buff_arr[2 * BUFF_LINE];
	mlib_s32 *pbuff = (mlib_s32 *)buff_arr, *buff0, *buff1, *buff2, *buffT;
	DTYPE *adr_src, *sl, *sp, *sl1;
	DTYPE *adr_dst, *dl, *dp;
	mlib_d64 k0, k1, k2, k3, scalef = 1.0;
	mlib_d64 p00, p01, p02, p10, p11, p12;
	mlib_s32 wid, hgt, sll, dll, wid1;
	mlib_s32 nchannel, chan1, chan2;
	mlib_s32 i, j, c, swid;

	LOAD_KERNEL_INTO_DOUBLE();
	GET_SRC_DST_PARAMETERS(DTYPE);

	vis_write_gsr(16 << 3);

	swid = wid + D_KER;

	wid1 = (swid + 1) & ~1;

	if (wid1 > BUFF_LINE) {
		pbuff = __mlib_malloc(3 * sizeof (mlib_s32) * wid1);

		if (pbuff == NULL)
			return (MLIB_FAILURE);
	}

	buff0 = pbuff;
	buff1 = buff0 + wid1;
	buff2 = buff1 + wid1;

	swid -= dx_r;

	chan1 = nchannel;
	chan2 = chan1 + chan1;

	for (c = 0; c < nchannel; c++) {
		if (!(cmask & (1 << (nchannel - 1 - c))))
			continue;

		sl = adr_src + c;
		dl = adr_dst + c;

		if ((hgt - dy_b) > 0)
			sl1 = sl + sll;
		else
			sl1 = sl;

#pragma pipeloop(0)
		for (i = 0; i < swid; i++) {
			buff0[i - 1] = (mlib_s32)sl[i * chan1];
			buff1[i - 1] = (mlib_s32)sl1[i * chan1];
		}

		if (dx_r != 0) {
			buff0[swid - 1] = buff0[swid - 2];
			buff1[swid - 1] = buff1[swid - 2];
		}

		if ((hgt - dy_b) > 1)
			sl = sl1 + sll;
		else
			sl = sl1;

		for (j = 0; j < hgt; j++) {
			sp = sl;
			dp = dl;

			buff2[-1] = (mlib_s32)sp[0];
			sp += chan1;

			p02 = buff0[-1];
			p12 = buff1[-1];

#pragma pipeloop(0)
			for (i = 0; i <= (wid - 2); i += 2) {
				d64_2x32 sd0, sd1;
				d64_2x32 dd0, dd1;

				p00 = p02;
				p10 = p12;

				sd0.d64 = *(TYPE_64BIT *) (buff0 + i);
				sd1.d64 = *(TYPE_64BIT *) (buff1 + i);
				p01 = (mlib_d64)sd0.i32s.i0;
				p02 = (mlib_d64)sd0.i32s.i1;
				p11 = (mlib_d64)sd1.i32s.i0;
				p12 = (mlib_d64)sd1.i32s.i1;

				LOAD_BUFF(buff2);

				dd0.i32s.i0 =
				    CLAMP_S32(p00 * k0 + p01 * k1 + p10 * k2 +
				    p11 * k3);
				dd0.i32s.i1 =
				    CLAMP_S32(p01 * k0 + p02 * k1 + p11 * k2 +
				    p12 * k3);

				dd1.f32s.f0 = vis_fpackfix(dd0.d64);

				STORE2(dd1.i32s.i0 >> 16, dd1.i32s.i0);

				sp += chan2;
				dp += chan2;
			}

			for (; i < wid; i++) {
				d64_2x32 dd0, dd1;

				p00 = buff0[i - 1];
				p10 = buff1[i - 1];
				p01 = buff0[i];
				p11 = buff1[i];

				buff2[i] = (mlib_s32)sp[0];

				dd0.i32s.i1 =
				    CLAMP_S32(p00 * k0 + p01 * k1 + p10 * k2 +
				    p11 * k3);

				dd1.f32s.f0 = vis_fpackfix(dd0.d64);
				dp[0] = dd1.i32s.i0;

				sp += chan1;
				dp += chan1;
			}

			if (dx_r != 0)
				buff2[swid - 1] = buff2[swid - 2];

			if (j < hgt - dy_b - 2)
				sl += sll;
			dl += dll;

			buffT = buff0;
			buff0 = buff1;
			buff1 = buff2;
			buff2 = buffT;
		}
	}

	if (pbuff != (mlib_s32 *)buff_arr)
		__mlib_free(pbuff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef  DTYPE
#define	DTYPE	mlib_u16

/* *********************************************************** */

#undef  S64TOS32
#define	S64TOS32(x)	(x)

mlib_status
mlib_c_conv2x2nw_u16(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kern,
    mlib_s32 scalef_expon,
    mlib_s32 cmask)
{
	mlib_d64 buff_arr[2 * BUFF_LINE];
	mlib_s32 *pbuff = (mlib_s32 *)buff_arr, *buff0, *buff1, *buff2, *buffT;
	DTYPE *adr_src, *sl, *sp, *sl1;
	DTYPE *adr_dst, *dl, *dp;
	mlib_d64 k0, k1, k2, k3, scalef = 1.0;
	mlib_d64 p00, p01, p02, p10, p11, p12;
	mlib_s32 wid, hgt, sll, dll, wid1;
	mlib_s32 nchannel, chan1, chan2;
	mlib_s32 i, j, c;
	mlib_d64 doff = (1 << 15);

	LOAD_KERNEL_INTO_DOUBLE();
	GET_SRC_DST_PARAMETERS(DTYPE);

	vis_write_gsr(16 << 3);

	wid1 = (wid + 1) & ~1;

	if (wid1 > BUFF_LINE) {
		pbuff = __mlib_malloc(3 * sizeof (mlib_s32) * wid1);

		if (pbuff == NULL)
			return (MLIB_FAILURE);
	}

	buff0 = pbuff;
	buff1 = buff0 + wid1;
	buff2 = buff1 + wid1;

	chan1 = nchannel;
	chan2 = chan1 + chan1;

	wid -= D_KER;
	hgt -= D_KER;

	for (c = 0; c < nchannel; c++) {
		if (!(cmask & (1 << (nchannel - 1 - c))))
			continue;

		sl = adr_src + c;
		dl = adr_dst + c;

		sl1 = sl + sll;

#pragma pipeloop(0)
		for (i = 0; i < wid + D_KER; i++) {
			buff0[i - 1] = (mlib_s32)sl[i * chan1];
			buff1[i - 1] = (mlib_s32)sl1[i * chan1];
		}

		sl += (D_KER + 1) * sll;

		for (j = 0; j < hgt; j++) {
			sp = sl;
			dp = dl;

			buff2[-1] = (mlib_s32)sp[0];
			sp += chan1;

			p02 = buff0[-1];
			p12 = buff1[-1];

#pragma pipeloop(0)
			for (i = 0; i <= (wid - 2); i += 2) {
				d64_2x32 sd0, sd1;
				d64_2x32 dd0, dd1;

				p00 = p02;
				p10 = p12;

				sd0.d64 = *(TYPE_64BIT *) (buff0 + i);
				sd1.d64 = *(TYPE_64BIT *) (buff1 + i);
				p01 = (mlib_d64)sd0.i32s.i0;
				p02 = (mlib_d64)sd0.i32s.i1;
				p11 = (mlib_d64)sd1.i32s.i0;
				p12 = (mlib_d64)sd1.i32s.i1;

				LOAD_BUFF(buff2);

				dd0.i32s.i0 =
				    CLAMP_S32(p00 * k0 + p01 * k1 + p10 * k2 +
				    p11 * k3 - doff);
				dd0.i32s.i1 =
				    CLAMP_S32(p01 * k0 + p02 * k1 + p11 * k2 +
				    p12 * k3 - doff);

				dd1.f32s.f0 = vis_fpackfix(dd0.d64);

				STORE2((dd1.i32s.i0 >> 16) ^ 0x8000,
				    dd1.i32s.i0 ^ 0x8000);

				sp += chan2;
				dp += chan2;
			}

			for (; i < wid; i++) {
				d64_2x32 dd0, dd1;

				p00 = buff0[i - 1];
				p10 = buff1[i - 1];
				p01 = buff0[i];
				p11 = buff1[i];

				buff2[i] = (mlib_s32)sp[0];

				dd0.i32s.i1 =
				    CLAMP_S32(p00 * k0 + p01 * k1 + p10 * k2 +
				    p11 * k3 - doff);

				dd1.f32s.f0 = vis_fpackfix(dd0.d64);
				dp[0] = dd1.i32s.i0 ^ 0x8000;

				sp += chan1;
				dp += chan1;
			}

			sl += sll;
			dl += dll;

			buffT = buff0;
			buff0 = buff1;
			buff1 = buff2;
			buff2 = buffT;
		}
	}

	if (pbuff != (mlib_s32 *)buff_arr)
		__mlib_free(pbuff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_c_conv2x2ext_u16(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_s32 *kern,
    mlib_s32 scalef_expon,
    mlib_s32 cmask)
{
	mlib_d64 buff_arr[2 * BUFF_LINE];
	mlib_s32 *pbuff = (mlib_s32 *)buff_arr, *buff0, *buff1, *buff2, *buffT;
	DTYPE *adr_src, *sl, *sp, *sl1;
	DTYPE *adr_dst, *dl, *dp;
	mlib_d64 k0, k1, k2, k3, scalef = 1.0;
	mlib_d64 p00, p01, p02, p10, p11, p12;
	mlib_s32 wid, hgt, sll, dll, wid1;
	mlib_s32 nchannel, chan1, chan2;
	mlib_s32 i, j, c, swid;
	mlib_d64 doff = (1 << 15);

	LOAD_KERNEL_INTO_DOUBLE();
	GET_SRC_DST_PARAMETERS(DTYPE);

	vis_write_gsr(16 << 3);

	swid = wid + D_KER;

	wid1 = (swid + 1) & ~1;

	if (wid1 > BUFF_LINE) {
		pbuff = __mlib_malloc(3 * sizeof (mlib_s32) * wid1);

		if (pbuff == NULL)
			return (MLIB_FAILURE);
	}

	buff0 = pbuff;
	buff1 = buff0 + wid1;
	buff2 = buff1 + wid1;

	swid -= dx_r;

	chan1 = nchannel;
	chan2 = chan1 + chan1;

	for (c = 0; c < nchannel; c++) {
		if (!(cmask & (1 << (nchannel - 1 - c))))
			continue;

		sl = adr_src + c;
		dl = adr_dst + c;

		if ((hgt - dy_b) > 0)
			sl1 = sl + sll;
		else
			sl1 = sl;

#pragma pipeloop(0)
		for (i = 0; i < swid; i++) {
			buff0[i - 1] = (mlib_s32)sl[i * chan1];
			buff1[i - 1] = (mlib_s32)sl1[i * chan1];
		}

		if (dx_r != 0) {
			buff0[swid - 1] = buff0[swid - 2];
			buff1[swid - 1] = buff1[swid - 2];
		}

		if ((hgt - dy_b) > 1)
			sl = sl1 + sll;
		else
			sl = sl1;

		for (j = 0; j < hgt; j++) {
			sp = sl;
			dp = dl;

			buff2[-1] = (mlib_s32)sp[0];
			sp += chan1;

			p02 = buff0[-1];
			p12 = buff1[-1];

#pragma pipeloop(0)
			for (i = 0; i <= (wid - 2); i += 2) {
				d64_2x32 sd0, sd1;
				d64_2x32 dd0, dd1;

				p00 = p02;
				p10 = p12;

				sd0.d64 = *(TYPE_64BIT *) (buff0 + i);
				sd1.d64 = *(TYPE_64BIT *) (buff1 + i);
				p01 = (mlib_d64)sd0.i32s.i0;
				p02 = (mlib_d64)sd0.i32s.i1;
				p11 = (mlib_d64)sd1.i32s.i0;
				p12 = (mlib_d64)sd1.i32s.i1;

				LOAD_BUFF(buff2);

				dd0.i32s.i0 =
				    CLAMP_S32(p00 * k0 + p01 * k1 + p10 * k2 +
				    p11 * k3 - doff);
				dd0.i32s.i1 =
				    CLAMP_S32(p01 * k0 + p02 * k1 + p11 * k2 +
				    p12 * k3 - doff);

				dd1.f32s.f0 = vis_fpackfix(dd0.d64);

				STORE2((dd1.i32s.i0 >> 16) ^ 0x8000,
				    dd1.i32s.i0 ^ 0x8000);

				sp += chan2;
				dp += chan2;
			}

			for (; i < wid; i++) {
				d64_2x32 dd0, dd1;

				p00 = buff0[i - 1];
				p10 = buff1[i - 1];
				p01 = buff0[i];
				p11 = buff1[i];

				buff2[i] = (mlib_s32)sp[0];

				dd0.i32s.i1 =
				    CLAMP_S32(p00 * k0 + p01 * k1 + p10 * k2 +
				    p11 * k3 - doff);
				dd1.f32s.f0 = vis_fpackfix(dd0.d64);
				dp[0] = dd1.i32s.i0 ^ 0x8000;

				sp += chan1;
				dp += chan1;
			}

			if (dx_r != 0)
				buff2[swid - 1] = buff2[swid - 2];

			if (j < hgt - dy_b - 2)
				sl += sll;
			dl += dll;

			buffT = buff0;
			buff0 = buff1;
			buff1 = buff2;
			buff2 = buffT;
		}
	}

	if (pbuff != (mlib_s32 *)buff_arr)
		__mlib_free(pbuff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#undef  DTYPE
#define	DTYPE	mlib_u8

mlib_status
mlib_c_conv2x2nw_u8(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kern,
    mlib_s32 scalef_expon,
    mlib_s32 cmask)
{
	mlib_d64 buff_arr[2 * BUFF_LINE];
	mlib_s32 *pbuff = (mlib_s32 *)buff_arr, *buff0, *buff1, *buff2, *buffT;
	DTYPE *adr_src, *sl, *sp, *sl1;
	DTYPE *adr_dst, *dl, *dp;
	mlib_d64 k0, k1, k2, k3, scalef = 1.0;
	mlib_d64 p00, p01, p02, p10, p11, p12;
	mlib_s32 wid, hgt, sll, dll, wid1;
	mlib_s32 nchannel, chan1, chan2;
	mlib_s32 i, j, c;

	LOAD_KERNEL_INTO_DOUBLE();
	GET_SRC_DST_PARAMETERS(DTYPE);

	vis_write_gsr(23 << 3);

	wid1 = (wid + 1) & ~1;

	if (wid1 > BUFF_LINE) {
		pbuff = __mlib_malloc(3 * sizeof (mlib_s32) * wid1);

		if (pbuff == NULL)
			return (MLIB_FAILURE);
	}

	buff0 = pbuff;
	buff1 = buff0 + wid1;
	buff2 = buff1 + wid1;

	chan1 = nchannel;
	chan2 = chan1 + chan1;

	wid -= D_KER;
	hgt -= D_KER;

	for (c = 0; c < nchannel; c++) {
		if (!(cmask & (1 << (nchannel - 1 - c))))
			continue;

		sl = adr_src + c;
		dl = adr_dst + c;

		sl1 = sl + sll;

#pragma pipeloop(0)
		for (i = 0; i < wid + D_KER; i++) {
			buff0[i - 1] = (mlib_s32)sl[i * chan1];
			buff1[i - 1] = (mlib_s32)sl1[i * chan1];
		}

		sl += (D_KER + 1) * sll;

		for (j = 0; j < hgt; j++) {
			sp = sl;
			dp = dl;

			buff2[-1] = (mlib_s32)sp[0];
			sp += chan1;

			p02 = buff0[-1];
			p12 = buff1[-1];

#pragma pipeloop(0)
			for (i = 0; i <= (wid - 2); i += 2) {
				d64_2x32 sd0, sd1;
				d64_2x32 dd0, dd1;

				p00 = p02;
				p10 = p12;

				sd0.d64 = *(TYPE_64BIT *) (buff0 + i);
				sd1.d64 = *(TYPE_64BIT *) (buff1 + i);
				p01 = (mlib_d64)sd0.i32s.i0;
				p02 = (mlib_d64)sd0.i32s.i1;
				p11 = (mlib_d64)sd1.i32s.i0;
				p12 = (mlib_d64)sd1.i32s.i1;

				LOAD_BUFF(buff2);

				dd0.i32s.i0 =
				    CLAMP_S32(p00 * k0 + p01 * k1 + p10 * k2 +
				    p11 * k3);
				dd0.i32s.i1 =
				    CLAMP_S32(p01 * k0 + p02 * k1 + p11 * k2 +
				    p12 * k3);

				dd1.d64 = vis_fpack32(dd1.d64, dd0.d64);

				STORE2(dd1.i32s.i0, dd1.i32s.i1);

				sp += chan2;
				dp += chan2;
			}

			for (; i < wid; i++) {
				d64_2x32 dd0, dd1;

				p00 = buff0[i - 1];
				p10 = buff1[i - 1];
				p01 = buff0[i];
				p11 = buff1[i];

				buff2[i] = (mlib_s32)sp[0];

				dd0.i32s.i1 =
				    CLAMP_S32(p00 * k0 + p01 * k1 + p10 * k2 +
				    p11 * k3);
				dd1.d64 = vis_fpack32(dd1.d64, dd0.d64);
				dp[0] = dd1.i32s.i1;

				sp += chan1;
				dp += chan1;
			}

			sl += sll;
			dl += dll;

			buffT = buff0;
			buff0 = buff1;
			buff1 = buff2;
			buff2 = buffT;
		}
	}

	if (pbuff != (mlib_s32 *)buff_arr)
		__mlib_free(pbuff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_c_conv2x2ext_u8(
    mlib_image *dst,
    const mlib_image *src,
    mlib_s32 dx_l,
    mlib_s32 dx_r,
    mlib_s32 dy_t,
    mlib_s32 dy_b,
    const mlib_s32 *kern,
    mlib_s32 scalef_expon,
    mlib_s32 cmask)
{
	mlib_d64 buff_arr[4 * BUFF_LINE];
	mlib_s32 *pbuff = (mlib_s32 *)buff_arr, *buff0, *buff1, *buff2, *buffT;
	DTYPE *adr_src, *sl, *sp, *sl1;
	DTYPE *adr_dst, *dl, *dp;
	mlib_d64 k0, k1, k2, k3, scalef = 1.0;
	mlib_d64 p00, p01, p02, p10, p11, p12;
	mlib_s32 wid, hgt, sll, dll, wid1;
	mlib_s32 nchannel, chan1, chan2;
	mlib_s32 i, j, c, swid;

	LOAD_KERNEL_INTO_DOUBLE();
	GET_SRC_DST_PARAMETERS(DTYPE);

	vis_write_gsr(23 << 3);

	swid = wid + D_KER;

	wid1 = (swid + 1) & ~1;

	if (wid1 > BUFF_LINE) {
		pbuff = __mlib_malloc(4 * sizeof (mlib_s32) * wid1);

		if (pbuff == NULL)
			return (MLIB_FAILURE);
	}

	buff0 = pbuff;
	buff1 = buff0 + wid1;
	buff2 = buff1 + wid1;

	chan1 = nchannel;
	chan2 = chan1 + chan1;

	swid -= dx_r;

	for (c = 0; c < nchannel; c++) {
		if (!(cmask & (1 << (nchannel - 1 - c))))
			continue;

		sl = adr_src + c;
		dl = adr_dst + c;

		if ((hgt - dy_b) > 0)
			sl1 = sl + sll;
		else
			sl1 = sl;

#pragma pipeloop(0)
		for (i = 0; i < swid; i++) {
			buff0[i - 1] = (mlib_s32)sl[i * chan1];
			buff1[i - 1] = (mlib_s32)sl1[i * chan1];
		}

		if (dx_r != 0) {
			buff0[swid - 1] = buff0[swid - 2];
			buff1[swid - 1] = buff1[swid - 2];
		}

		if ((hgt - dy_b) > 1)
			sl = sl1 + sll;
		else
			sl = sl1;

		for (j = 0; j < hgt; j++) {
			sp = sl;
			dp = dl;

			buff2[-1] = (mlib_s32)sp[0];
			sp += chan1;

			p02 = buff0[-1];
			p12 = buff1[-1];

#pragma pipeloop(0)
			for (i = 0; i <= (wid - 2); i += 2) {
				d64_2x32 sd0, sd1;
				d64_2x32 dd0, dd1;

				p00 = p02;
				p10 = p12;

				sd0.d64 = *(TYPE_64BIT *) (buff0 + i);
				sd1.d64 = *(TYPE_64BIT *) (buff1 + i);
				p01 = (mlib_d64)sd0.i32s.i0;
				p02 = (mlib_d64)sd0.i32s.i1;
				p11 = (mlib_d64)sd1.i32s.i0;
				p12 = (mlib_d64)sd1.i32s.i1;

				LOAD_BUFF(buff2);

				dd0.i32s.i0 =
				    CLAMP_S32(p00 * k0 + p01 * k1 + p10 * k2 +
				    p11 * k3);
				dd0.i32s.i1 =
				    CLAMP_S32(p01 * k0 + p02 * k1 + p11 * k2 +
				    p12 * k3);

				dd1.d64 = vis_fpack32(dd1.d64, dd0.d64);

				STORE2(dd1.i32s.i0, dd1.i32s.i1);

				sp += chan2;
				dp += chan2;
			}

			for (; i < wid; i++) {
				d64_2x32 dd0, dd1;

				p00 = buff0[i - 1];
				p10 = buff1[i - 1];
				p01 = buff0[i];
				p11 = buff1[i];

				buff2[i] = (mlib_s32)sp[0];

				dd0.i32s.i1 =
				    CLAMP_S32(p00 * k0 + p01 * k1 + p10 * k2 +
				    p11 * k3);
				dd1.d64 = vis_fpack32(dd1.d64, dd0.d64);
				dp[0] = dd1.i32s.i1;

				sp += chan1;
				dp += chan1;
			}

			if (dx_r != 0)
				buff2[swid - 1] = buff2[swid - 2];

			if (j < hgt - dy_b - 2)
				sl += sll;
			dl += dll;

			buffT = buff0;
			buff0 = buff1;
			buff1 = buff2;
			buff2 = buffT;
		}
	}

	if (pbuff != (mlib_s32 *)buff_arr)
		__mlib_free(pbuff);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
