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

#pragma ident	"@(#)mlib_ImageConv3x3_Bit.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *    Internal functions for mlib_ImageConv3x3 on BIT type.
 *    (only 1-channel case implemented)
 *
 */

#include <mlib_image.h>
#include <mlib_ImageConv.h>

/* *********************************************************** */

static void mlib_conv3x3_bit_create_table(
    mlib_u32 *table,
    const mlib_s32 *kern,
    mlib_s32 scalef_expon);

/* *********************************************************** */

#define	BUFF_LINE	256

/* *********************************************************** */

#define	GET_SRC_DST_PARAMETERS()                                         \
	mlib_s32 hgt = mlib_ImageGetHeight(src);                         \
	mlib_s32 wid = mlib_ImageGetWidth(src);                          \
	mlib_s32 sll = mlib_ImageGetStride(src);                         \
	mlib_s32 dll = mlib_ImageGetStride(dst);                         \
	mlib_u8  *adr_src = mlib_ImageGetData(src);                      \
	mlib_u8  *adr_dst = mlib_ImageGetData(dst);                      \
	mlib_s32 bitoffs = mlib_ImageGetBitOffset(src);                  \
	mlib_s32 bitoffd = mlib_ImageGetBitOffset(dst);                  \
	mlib_s32 bitoff = bitoffs

/* *********************************************************** */

mlib_status
mlib_conv3x3nw_bit(
    mlib_image *dst,
    const mlib_image *src,
    const mlib_s32 *kern,
    mlib_s32 scalef_expon,
    mlib_s32 cmask)
{
	mlib_u8  *sl, *sp, *dp;
	mlib_u32 table[256];
	mlib_u16 buff[3 * BUFF_LINE + 1];
	mlib_u16 *pbuff, *buff0, *buff1, *buff2, *buffT;
	mlib_s32 amount, amount_s;
	mlib_u32 s0, s1, s2;
	mlib_u32 a0, a1, a2, a3;
	mlib_u32 shift0, shift1, shift2, shift3;
	mlib_s32 i, j, bitoffd_end;
	mlib_u8 mask_start, mask_end, tmp_start, tmp_end;

	GET_SRC_DST_PARAMETERS();

	mlib_conv3x3_bit_create_table(table, kern, scalef_expon);

	wid -= 2;
	hgt -= 2;

	adr_dst += dll;
	adr_dst += ((bitoffd + 1) >> 3);
	bitoffd = (bitoffd + 1) & 7;

	bitoffd_end = (bitoffd + wid) & 7;

	sl = adr_src;
	dp = adr_dst;

	amount   = (bitoffd + wid + 7) >> 3;
	amount_s = (bitoffs + wid + 2 + 7) >> 3;

	pbuff = buff;

	if (amount + 4 > BUFF_LINE)
		pbuff = __mlib_malloc((3*amount + 16)*sizeof (mlib_u16));

	buff0 = pbuff;
	buff1 = buff0 + amount + 4;
	buff2 = buff1 + amount + 4;

	bitoff -= bitoffd;

	if (bitoff < 0) {
		bitoff += 8;
		sl--;
		sp = sl;
		s0 = (sp[1] & 0xFF);
		amount_s++;
	} else {
		sp = sl;
		s0 = ((sp[0] & 0xFF) << 8) | (sp[1] & 0xFF);
	}

	s0 <<= (8 + bitoff);
	sp += 2;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < amount; i++) {
		s0 |= ((sp[0] & 0xFF) << bitoff);
		buff0[i] = (((s0 >> 8) & 0xF000) | ((s0 >> 10) & 0xF00) |
		    ((s0 >> 12) & 0xF0) | ((s0 >> 14) & 0xF));
		s0 <<= 8;
		sp++;
	}

	sl += sll;

	sp = sl;
	s0 = ((sp[0] & 0xFF) << 8) | (sp[1] & 0xFF);
	s0 <<= (8 + bitoff);
	sp += 2;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < amount; i++) {
		s0 |= ((sp[0] & 0xFF) << bitoff);
		buff1[i] = (((s0 >> 8) & 0xF000) | ((s0 >> 10) & 0xF00) |
		    ((s0 >> 12) & 0xF0) | ((s0 >> 14) & 0xF));
		s0 <<= 8;
		sp++;
	}

	sl += sll;

	mask_start = (0xFF >> bitoffd);
	mask_end = (0xFF << ((8 - bitoffd_end) & 7));

	for (j = 0; j < hgt; j++) {
		sp = sl;
		tmp_start = *dp;
		tmp_end = *(dp + amount - 1);

		s0 = (sp[0] & 0xFF) << 8;
		if (amount_s > 1) s0 |= (sp[1] & 0xFF);
		s0 <<= (8 + bitoff);
		sp += 2;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < amount_s - 2; i++) {
			s0 |= ((sp[0] & 0xFF) << bitoff);
			buff2[i] =
			    (((s0 >> 8) & 0xF000) | ((s0 >> 10) & 0x0F00) |
			    ((s0 >> 12) & 0x00F0) | ((s0 >> 14) & 0x000F));
			s0 <<= 8;
			sp++;
		}
		for (; i < amount; i++) {
			buff2[i] =
			    (((s0 >> 8) & 0xF000) | ((s0 >> 10) & 0x0F00) |
			    ((s0 >> 12) & 0x00F0) | ((s0 >> 14) & 0x000F));
			s0 <<= 8;
		}

		s0 = buff0[0];
		s1 = buff1[0];
		s2 = buff2[0];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < amount; i++) {
			a0 = *(mlib_u32 *)((mlib_u8 *)table +
			    ((s0 >> 6) & 0x3C0) + ((s1 >> 10) & 0x3C));
			a1 = *(mlib_u32 *)((mlib_u8 *)table +
			    ((s0 >> 2) & 0x3C0) + ((s1 >> 6) & 0x3C));
			a2 = *(mlib_u32 *)((mlib_u8 *)table +
			    ((s0 << 2) & 0x3C0) + ((s1 >> 2) & 0x3C));
			a3 = *(mlib_u32 *)((mlib_u8 *)table +
			    ((s0 << 6) & 0x3C0) + ((s1 << 2) & 0x3C));
			shift0 = ((s2 >> 11) & 0x1E);
			shift1 = ((s2 >> 7) & 0x1E);
			shift2 = ((s2 >> 3) & 0x1E);
			shift3 = ((s2 << 1) & 0x1E);

			dp[i] =
			    (mlib_u8)((((a0 >> shift0) & 0x3) << 6) | (((a1 >>
			    shift1) & 0x3) << 4) | (((a2 >> shift2) & 0x3) << 2)
			    | (((a3 >> shift3) & 0x3)));

			s0 = buff0[i + 1];
			s1 = buff1[i + 1];
			s2 = buff2[i + 1];
		}

		*dp = ((tmp_start & (~mask_start)) | ((*dp) & mask_start));
		*(dp + amount - 1) =
		    ((tmp_end & (~mask_end)) | ((*(dp + amount -
		    1)) & mask_end));

		sl += sll;
		dp += dll;

		buffT = buff0;
		buff0 = buff1;
		buff1 = buff2;
		buff2 = buffT;
	}

	if (pbuff != buff)
		__mlib_free(pbuff);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
mlib_conv3x3ext_bit(
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
	mlib_u8  *sl, *sp, *dp;
	mlib_u32 table[256];
	mlib_u16 buff[3 * BUFF_LINE + 1];
	mlib_u16 *pbuff, *buff0, *buff1, *buff2, *buffT;
	mlib_s32 amount, amount_s;
	mlib_u32 s0, s1, s2;
	mlib_u32 a0, a1, a2, a3;
	mlib_u32 shift0, shift1, shift2, shift3;
	mlib_s32 i, j, bitoffd_end;
	mlib_u8  mask_start, mask_end, tmp_start, tmp_end;
	mlib_u32 mask_ext_start, mask_ext_end;

	GET_SRC_DST_PARAMETERS();

	mlib_conv3x3_bit_create_table(table, kern, scalef_expon);

	bitoffd_end = (bitoffd + wid) & 7;

	mask_ext_start = (0x7FFFFF >> bitoffd);

	if (bitoffd_end != 0)
		mask_ext_end = (0xFFFF << (16 - bitoffd_end * 2 +
		    ((bitoffd_end - 1) & 1)));
	else
		mask_ext_end = 0xFFFE;

	sl = adr_src;
	dp = adr_dst;

	amount = (bitoffd + wid + 7) >> 3;
	amount_s = (bitoffs + wid + 7 + 2 - dx_l - dx_r) >> 3;

	pbuff = buff;

	if (amount + 4 > BUFF_LINE)
		pbuff = __mlib_malloc((3*amount + 16)*sizeof (mlib_u16));

	buff0 = pbuff;
	buff1 = buff0 + amount + 4;
	buff2 = buff1 + amount + 4;

	bitoff -= (bitoffd + dx_l);

	if (bitoff < 0) {
		bitoff += 8;
		sl--;
		sp = sl;
		s0 = (sp[1] & 0xFF);
		amount_s++;
	} else {
		sp = sl;
		s0 = ((sp[0] & 0xFF) << 8);
		if (amount_s > 1) s0 |= (sp[1] & 0xFF);
	}

	s0 <<= (8 + bitoff);

	if (dx_l != 0)
		s0 = ((s0 & mask_ext_start) | ((s0 << 1) & ~mask_ext_start));

	sp += 2;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < amount_s - 2; i++) {
		s0 |= ((sp[0] & 0xFF) << bitoff);
		buff0[i] = (((s0 >> 8) & 0xF000) | ((s0 >> 10) & 0xF00) |
		    ((s0 >> 12) & 0xF0) | ((s0 >> 14) & 0xF));
		s0 <<= 8;
		sp++;
	}
	for (; i < amount; i++) {
		buff0[i] = (((s0 >> 8) & 0xF000) | ((s0 >> 10) & 0xF00) |
		    ((s0 >> 12) & 0xF0) | ((s0 >> 14) & 0xF));
		s0 <<= 8;
	}

	if (dx_r != 0)
		buff0[amount - 1] = ((buff0[amount - 1] & mask_ext_end) |
		    ((buff0[amount - 1] >> 1) & ~mask_ext_end));

	if ((1 > dy_t) && (1 < hgt + 2 - dy_b))
		sl += sll;

	sp = sl;
	if (sp >= adr_src) s0 = (sp[0] & 0xFF) << 8; else s0 = 0;
	if (amount_s > 1) s0 |= (sp[1] & 0xFF);
	s0 <<= (8 + bitoff);

	if (dx_l != 0)
		s0 = ((s0 & mask_ext_start) | ((s0 << 1) & ~mask_ext_start));

	sp += 2;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	for (i = 0; i < amount_s - 2; i++) {
		s0 |= ((sp[0] & 0xFF) << bitoff);
		buff1[i] = (((s0 >> 8) & 0xF000) | ((s0 >> 10) & 0xF00) |
		    ((s0 >> 12) & 0xF0) | ((s0 >> 14) & 0xF));
		s0 <<= 8;
		sp++;
	}
	for (; i < amount; i++) {
		buff1[i] = (((s0 >> 8) & 0xF000) | ((s0 >> 10) & 0xF00) |
		    ((s0 >> 12) & 0xF0) | ((s0 >> 14) & 0xF));
		s0 <<= 8;
	}

	if (dx_r != 0)
		buff1[amount - 1] = ((buff1[amount - 1] & mask_ext_end) |
		    ((buff1[amount - 1] >> 1) & ~mask_ext_end));

	if ((hgt - dy_b) > 0)
		sl += sll;

	mask_start = (0xFF >> bitoffd);
	mask_end = (0xFF << ((8 - bitoffd_end) & 7));

	for (j = 0; j < hgt; j++) {
		sp = sl;
		tmp_start = *dp;
		tmp_end = *(dp + amount - 1);

		if (sp >= adr_src) s0 = (sp[0] & 0xFF) << 8; else s0 = 0;
		if (amount_s > 1) s0 |= (sp[1] & 0xFF);
		s0 <<= (8 + bitoff);

		if (dx_l != 0)
			s0 = ((s0 & mask_ext_start) | ((s0 << 1) &
			    ~mask_ext_start));

		sp += 2;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < amount_s - 2; i++) {
			s0 |= ((sp[0] & 0xFF) << bitoff);
			buff2[i] =
			    (((s0 >> 8) & 0xF000) | ((s0 >> 10) & 0xF00) | ((s0
			    >> 12) & 0xF0) | ((s0 >> 14) & 0xF));
			s0 <<= 8;
			sp++;
		}
		for (; i < amount; i++) {
			buff2[i] =
			    (((s0 >> 8) & 0xF000) | ((s0 >> 10) & 0xF00) | ((s0
			    >> 12) & 0xF0) | ((s0 >> 14) & 0xF));
			s0 <<= 8;
		}

		if (dx_r != 0)
			buff2[amount - 1] =
			    ((buff2[amount -
			    1] & mask_ext_end) | ((buff2[amount -
			    1] >> 1) & ~mask_ext_end));

		s0 = buff0[0];
		s1 = buff1[0];
		s2 = buff2[0];

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < amount; i++) {
			a0 = *(mlib_u32 *)((mlib_u8 *)table +
			    ((s0 >> 6) & 0x3C0) + ((s1 >> 10) & 0x3C));
			a1 = *(mlib_u32 *)((mlib_u8 *)table +
			    ((s0 >> 2) & 0x3C0) + ((s1 >> 6) & 0x3C));
			a2 = *(mlib_u32 *)((mlib_u8 *)table +
			    ((s0 << 2) & 0x3C0) + ((s1 >> 2) & 0x3C));
			a3 = *(mlib_u32 *)((mlib_u8 *)table +
			    ((s0 << 6) & 0x3C0) + ((s1 << 2) & 0x3C));
			shift0 = ((s2 >> 11) & 0x1E);
			shift1 = ((s2 >> 7) & 0x1E);
			shift2 = ((s2 >> 3) & 0x1E);
			shift3 = ((s2 << 1) & 0x1E);

			dp[i] =
			    (mlib_u8)((((a0 >> shift0) & 0x3) << 6) | (((a1 >>
			    shift1) & 0x3) << 4) | (((a2 >> shift2) & 0x3) << 2)
			    | (((a3 >> shift3) & 0x3)));

			s0 = buff0[i + 1];
			s1 = buff1[i + 1];
			s2 = buff2[i + 1];
		}

		*dp = ((tmp_start & (~mask_start)) | ((*dp) & mask_start));
		*(dp + amount - 1) =
		    ((tmp_end & (~mask_end)) | ((*(dp + amount -
		    1)) & mask_end));

		if (j < hgt - dy_b - 1)
			sl += sll;
		dp += dll;

		buffT = buff0;
		buff0 = buff1;
		buff1 = buff2;
		buff2 = buffT;
	}

	if (pbuff != buff)
		__mlib_free(pbuff);
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

void
mlib_conv3x3_bit_create_table(
    mlib_u32 *table,
    const mlib_s32 *kern,
    mlib_s32 scalef_expon)
{
	mlib_s32 ind00, ind01, ind0, ind1;
	mlib_u32 table1[64], *tmp_table;
	mlib_u32 tmp0, tmp1, tmp2;
	mlib_s32 i, j;

#ifdef _NO_LONGLONG
#ifdef _LITTLE_ENDIAN

/* *********************************************************** */

#define	SCALBN(res, shift)                                      \
	                                                        \
/*                                                              \
 * res = 2 ** (shift)                                           \
 */                                                             \
	res = 0.;                                               \
	((mlib_s32 *)&res)[1] = (1023 + shift) << 20

#else /* _LITTLE_ENDIAN */

/* *********************************************************** */

#define	SCALBN(res, shift)                                      \
	                                                        \
/*                                                              \
 * res = 2 ** (shift)                                           \
 */                                                             \
	res = 0.;                                               \
	((mlib_s32 *)&res)[0] = (1023 + shift) << 20

#endif /* _LITTLE_ENDIAN */

	mlib_s32 mask1[8] = { 0, -1, 0, -1, 0, -1, 0, -1 };
	mlib_s32 mask2[8] = { 0, 0, -1, -1, 0, 0, -1, -1 };
	mlib_s32 mask3[8] = { 0, 0, 0, 0, -1, -1, -1, -1 };
	mlib_s32 k0, k1, k2, k3, k4, k5, k6, k7, k8;
	mlib_d64 sum0, sum1, one_half, dscale, dtmp;

	k0 = kern[0];
	k1 = kern[1];
	k2 = kern[2];
	k3 = kern[3];
	k4 = kern[4];
	k5 = kern[5];
	k6 = kern[6];
	k7 = kern[7];
	k8 = kern[8];

	if (scalef_expon >= 36) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < 256; i++)
			table[i] = 0;
		return;
	}

	if (scalef_expon > 0) {
		SCALBN(one_half, scalef_expon - 1);
	} else
		one_half = 0;
	SCALBN(dscale, -scalef_expon);

	tmp_table = table1;

	for (i = 0; i < 8; i++) {
		sum0 = (mlib_d64)(k0 & mask3[i]) + (mlib_d64)(k1 & mask2[i]) +
		    (mlib_d64)(k2 & mask1[i]) + one_half;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < 8; j++) {
			sum1 =
			    (mlib_d64)(k3 & mask3[j]) +
			    (mlib_d64)(k4 & mask2[j]) +
			    (mlib_d64)(k5 & mask1[j]) + sum0;

			dtmp =
			    (sum1 + (mlib_d64)k6 + (mlib_d64)k7 +
			    (mlib_d64)k8) * dscale;
			tmp0 = (dtmp < 1.0) ? 1 : 0;
			tmp1 = (tmp0 << 29);
			tmp2 = (tmp0 << 14);

			dtmp = (sum1 + (mlib_d64)k6 + (mlib_d64)k7) * dscale;
			tmp0 = (dtmp < 1.0) ? 1 : 0;
			tmp1 |= (tmp0 << 25);
			tmp2 |= (tmp0 << 12);

			dtmp = (sum1 + (mlib_d64)k6 + (mlib_d64)k8) * dscale;
			tmp0 = (dtmp < 1.0) ? 1 : 0;
			tmp1 |= (tmp0 << 21);
			tmp2 |= (tmp0 << 10);

			dtmp = (sum1 + (mlib_d64)k6) * dscale;
			tmp0 = (dtmp < 1.0) ? 1 : 0;
			tmp1 |= (tmp0 << 17);
			tmp2 |= (tmp0 << 8);

			dtmp = (sum1 + (mlib_d64)k7 + (mlib_d64)k8) * dscale;
			tmp0 = (dtmp < 1.0) ? 1 : 0;
			tmp1 |= (tmp0 << 13);
			tmp2 |= (tmp0 << 6);

			dtmp = (sum1 + (mlib_d64)k7) * dscale;
			tmp0 = (dtmp < 1.0) ? 1 : 0;
			tmp1 |= (tmp0 << 9);
			tmp2 |= (tmp0 << 4);

			dtmp = (sum1 + (mlib_d64)k8) * dscale;
			tmp0 = (dtmp < 1.0) ? 1 : 0;
			tmp1 |= (tmp0 << 5);
			tmp2 |= (tmp0 << 2);

			dtmp = sum1 * dscale;
			tmp0 = (dtmp < 1.0) ? 1 : 0;
			tmp1 |= (tmp0 << 1);
			tmp2 |= tmp0;

			(*tmp_table++) =
			    ~(tmp1 | (tmp1 << 2) | tmp2 | (tmp2 << 16));
		}
	}

#else /* _NO_LONGLONG */

	mlib_s64 mask1[8] = { 0, -1, 0, -1, 0, -1, 0, -1 };
	mlib_s64 mask2[8] = { 0, 0, -1, -1, 0, 0, -1, -1 };
	mlib_s64 mask3[8] = { 0, 0, 0, 0, -1, -1, -1, -1 };
	mlib_s64 k0, k1, k2, k3, k4, k5, k6, k7, k8;
	mlib_s64 sum0, sum1, one_half;

	k0 = (mlib_s64)kern[0];
	k1 = (mlib_s64)kern[1];
	k2 = (mlib_s64)kern[2];
	k3 = (mlib_s64)kern[3];
	k4 = (mlib_s64)kern[4];
	k5 = (mlib_s64)kern[5];
	k6 = (mlib_s64)kern[6];
	k7 = (mlib_s64)kern[7];
	k8 = (mlib_s64)kern[8];

	if (scalef_expon >= 36) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (i = 0; i < 256; i++)
			table[i] = 0;
		return;
	}

	if (scalef_expon > 0)
		one_half = ((mlib_s64)1) << (scalef_expon - 1);
	else
		one_half = 0;

	tmp_table = table1;

	for (i = 0; i < 8; i++) {
		sum0 = (k0 & mask3[i]) + (k1 & mask2[i]) +
		    (k2 & mask1[i]) + one_half;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < 8; j++) {
			sum1 = (k3 & mask3[j]) + (k4 & mask2[j]) +
			    (k5 & mask1[j]) + sum0;

			tmp0 =
			    (mlib_u32)(((((sum1 + k6 + k7 +
			    k8) >> scalef_expon) - 1) >> 63) & 1);
			tmp1 = (tmp0 << 29);
			tmp2 = (tmp0 << 14);

			tmp0 =
			    (mlib_u32)(((((sum1 + k6 + k7) >> scalef_expon) -
			    1) >> 63) & 1);
			tmp1 |= (tmp0 << 25);
			tmp2 |= (tmp0 << 12);

			tmp0 =
			    (mlib_u32)(((((sum1 + k6 + k8) >> scalef_expon) -
			    1) >> 63) & 1);
			tmp1 |= (tmp0 << 21);
			tmp2 |= (tmp0 << 10);

			tmp0 =
			    (mlib_u32)(((((sum1 + k6) >> scalef_expon) -
			    1) >> 63) & 1);
			tmp1 |= (tmp0 << 17);
			tmp2 |= (tmp0 << 8);

			tmp0 =
			    (mlib_u32)(((((sum1 + k7 + k8) >> scalef_expon) -
			    1) >> 63) & 1);
			tmp1 |= (tmp0 << 13);
			tmp2 |= (tmp0 << 6);

			tmp0 =
			    (mlib_u32)(((((sum1 + k7) >> scalef_expon) -
			    1) >> 63) & 1);
			tmp1 |= (tmp0 << 9);
			tmp2 |= (tmp0 << 4);

			tmp0 =
			    (mlib_u32)(((((sum1 + k8) >> scalef_expon) -
			    1) >> 63) & 1);
			tmp1 |= (tmp0 << 5);
			tmp2 |= (tmp0 << 2);

			tmp0 =
			    (mlib_u32)(((((sum1) >> scalef_expon) -
			    1) >> 63) & 1);
			tmp1 |= (tmp0 << 1);
			tmp2 |= tmp0;

			(*tmp_table++) =
			    ~(tmp1 | (tmp1 << 2) | tmp2 | (tmp2 << 16));
		}
	}

#endif /* _NO_LONGLONG */

	for (i = 0; i < 16; i++) {
		ind00 = (i & 0xE) << 4;
		ind01 = (i & 0x7) << 5;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j < 16; j++) {
			ind0 = ind00 | ((j & 0xE) << 1);
			ind1 = ind01 | ((j & 0x7) << 2);

			(*table++) =
			    (((*(mlib_u32 *)((mlib_u8 *)table1 +
			    ind0)) & 0xAAAAAAAA) | ((*(mlib_u32 *)((mlib_u8 *)
			    table1 + ind1)) & 0x55555555));
		}
	}
}

/* *********************************************************** */
