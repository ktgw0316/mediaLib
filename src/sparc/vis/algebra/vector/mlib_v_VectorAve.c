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

#pragma ident	"@(#)mlib_v_VectorAve.c	9.6	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorAve_[U8_U8|S8_S8],
 *      mlib_VectorAve_S16_[U8|S8],
 *      mlib_VectorAve_[U8|S8] - average of signed/unsigned
 *                               8-bit format vectors
 *
 *      mlib_VectorAve_[S16|S32]_S16,
 *      mlib_VectorAve_S16 - average of signed 16-bit format vectors
 *
 *      mlib_VectorAve_S32_S32,
 *      mlib_VectorAve_S32 - average of signed 32-bit format vectors
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorAve_U8_U8(mlib_u8       *z,
 *                                       const mlib_u8 *x,
 *                                       const mlib_u8 *y,
 *                                       mlib_s32      n)
 *      mlib_status mlib_VectorAve_S8_S8(mlib_s8       *z,
 *                                       const mlib_s8 *x,
 *                                       const mlib_s8 *y,
 *                                       mlib_s32      n)
 *      mlib_status mlib_VectorAve_S16_U8(mlib_s16      *z,
 *                                        const mlib_u8 *x,
 *                                        const mlib_u8 *y,
 *                                        mlib_s32      n)
 *      mlib_status mlib_VectorAve_S16_S8(mlib_s16      *z,
 *                                        const mlib_s8 *x,
 *                                        const mlib_s8 *y,
 *                                        mlib_s32      n)
 *      mlib_status mlib_VectorAve_S16_S16(mlib_s16       *z,
 *                                         const mlib_s16 *x,
 *                                         const mlib_s16 *y,
 *                                         mlib_s32       n)
 *      mlib_status mlib_VectorAve_S32_S16(mlib_s32       *z,
 *                                         const mlib_s16 *x,
 *                                         const mlib_s16 *y,
 *                                         mlib_s32       n)
 *      mlib_status mlib_VectorAve_S32_S32(mlib_s32       *z,
 *                                         const mlib_s32 *x,
 *                                         const mlib_s32 *y,
 *                                         mlib_s32       n)
 *
 *      mlib_status mlib_VectorAve_U8(mlib_u8       *xz,
 *                                    const mlib_u8 *y,
 *                                    mlib_s32      n)
 *      mlib_status mlib_VectorAve_S8(mlib_s8       *xz,
 *                                    const mlib_s8 *y,
 *                                    mlib_s32      n)
 *      mlib_status mlib_VectorAve_S16(mlib_s16       *xz,
 *                                     const mlib_s16 *y,
 *                                     mlib_s32       n)
 *      mlib_status mlib_VectorAve_S32(mlib_s32       *xz,
 *                                     const mlib_s32 *y,
 *                                     mlib_s32       n)
 *
 * ARGUMENTS
 *      z    pointer to the first element of the result vector
 *      xz   pointer to the first element of the first input and
 *           the output vector
 *      x    pointer to the first element of the first vector
 *      y    pointer to the first element of the second vector
 *      n    number of elements in the vectors
 *
 * DESCRIPTION
 *      z = (x + y + 1) / 2
 */

/*
 * FUNCTIONS
 *      mlib_VectorAve_[U8C_U8C|S8C_S8C],
 *      mlib_VectorAve_S16C_[U8C|S8C],
 *      mlib_VectorAve_[U8C|S8C] - average of signed/unsigned
 *                                 8-bit format complex vectors
 *
 *      mlib_VectorAve_[S16C|S32C]_S16C,
 *      mlib_VectorAve_S16C - average of signed 16-bit format complex vectors
 *
 *      mlib_VectorAve_S32C_S32C,
 *      mlib_VectorAve_S32C - average of signed 32-bit format complex vectors
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorAve_U8C_U8C(mlib_u8       *z,
 *                                         const mlib_u8 *x,
 *                                         const mlib_u8 *y,
 *                                         mlib_s32      n)
 *      mlib_status mlib_VectorAve_S8C_S8C(mlib_s8       *z,
 *                                         const mlib_s8 *x,
 *                                         const mlib_s8 *y,
 *                                         mlib_s32      n)
 *      mlib_status mlib_VectorAve_S16C_U8C(mlib_s16      *z,
 *                                          const mlib_u8 *x,
 *                                          const mlib_u8 *y,
 *                                          mlib_s32      n)
 *      mlib_status mlib_VectorAve_S16C_S8C(mlib_s16      *z,
 *                                          const mlib_s8 *x,
 *                                          const mlib_s8 *y,
 *                                          mlib_s32      n)
 *      mlib_status mlib_VectorAve_S16C_S16C(mlib_s16       *z,
 *                                           const mlib_s16 *x,
 *                                           const mlib_s16 *y,
 *                                           mlib_s32       n)
 *      mlib_status mlib_VectorAve_S32C_S16C(mlib_s32       *z,
 *                                           const mlib_s16 *x,
 *                                           const mlib_s16 *y,
 *                                           mlib_s32       n)
 *      mlib_status mlib_VectorAve_S32C_S32C(mlib_s32       *z,
 *                                           const mlib_s32 *x,
 *                                           const mlib_s32 *y,
 *                                           mlib_s32       n)
 *
 *      mlib_status mlib_VectorAve_U8C(mlib_u8       *xz,
 *                                     const mlib_u8 *y,
 *                                     mlib_s32      n)
 *      mlib_status mlib_VectorAve_S8C(mlib_s8       *xz,
 *                                     const mlib_s8 *y,
 *                                     mlib_s32      n)
 *      mlib_status mlib_VectorAve_S16C(mlib_s16       *xz,
 *                                      const mlib_s16 *y,
 *                                      mlib_s32       n)
 *      mlib_status mlib_VectorAve_S32C(mlib_s32       *xz,
 *                                      const mlib_s32 *y,
 *                                      mlib_s32       n)
 *
 * ARGUMENTS
 *      z    pointer to the first complex element of the result vector.
 *           z[2*i] hold the real parts, and z[2*i + 1] hold the imaginary
 *           parts
 *      xz   pointer to the first element of the first input and
 *           the output vector.
 *           xz[2*i] hold the real parts, and xz[2*i + 1] hold the imaginary
 *           parts
 *      x    pointer to the first complex element of the first vector.
 *           x[2*i] hold the real parts, and x[2*i + 1] hold the imaginary
 *           parts
 *      y    pointer to the first complex element of the second vector.
 *           y[2*i] hold the real parts, and y[2*i + 1] hold the imaginary
 *           parts
 *      n    number of complex elements in the vectors
 *
 * DESCRIPTION
 *      z = (x + y + 1) / 2
 */

/* *********************************************************** */

#include <stdio.h>
#include <mlib_algebra.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)
#pragma weak mlib_VectorAve_S8 = __mlib_VectorAve_S8
#pragma weak mlib_VectorAve_U8 = __mlib_VectorAve_U8
#pragma weak mlib_VectorAve_S16 = __mlib_VectorAve_S16
#pragma weak mlib_VectorAve_S32 = __mlib_VectorAve_S32
#pragma weak mlib_VectorAve_S8C = __mlib_VectorAve_S8C
#pragma weak mlib_VectorAve_U8C = __mlib_VectorAve_U8C
#pragma weak mlib_VectorAve_S16C = __mlib_VectorAve_S16C
#pragma weak mlib_VectorAve_S32C = __mlib_VectorAve_S32C
#pragma	weak mlib_VectorAve_S8_S8 = __mlib_VectorAve_S8_S8
#pragma weak mlib_VectorAve_U8_U8 = __mlib_VectorAve_U8_U8
#pragma weak mlib_VectorAve_S16_U8 = __mlib_VectorAve_S16_U8
#pragma weak mlib_VectorAve_S16_S8 = __mlib_VectorAve_S16_S8
#pragma weak mlib_VectorAve_S16_S16 = __mlib_VectorAve_S16_S16
#pragma weak mlib_VectorAve_S32_S16 = __mlib_VectorAve_S32_S16
#pragma weak mlib_VectorAve_S32_S32 = __mlib_VectorAve_S32_S32
#pragma weak mlib_VectorAve_S8C_S8C = __mlib_VectorAve_S8C_S8C
#pragma weak mlib_VectorAve_U8C_U8C = __mlib_VectorAve_U8C_U8C
#pragma weak mlib_VectorAve_S16C_U8C = __mlib_VectorAve_S16C_U8C
#pragma weak mlib_VectorAve_S16C_S8C = __mlib_VectorAve_S16C_S8C
#pragma weak mlib_VectorAve_S16C_S16C = __mlib_VectorAve_S16C_S16C
#pragma weak mlib_VectorAve_S32C_S16C = __mlib_VectorAve_S32C_S16C
#pragma weak mlib_VectorAve_S32C_S32C = __mlib_VectorAve_S32C_S32C

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_VectorAve_S8) mlib_VectorAve_S8(dx, dy, dz)
    __attribute__((weak, alias("__mlib_VectorAve_S8")));
__typeof__(__mlib_VectorAve_U8) mlib_VectorAve_U8
    __attribute__((weak, alias("__mlib_VectorAve_U8")));
__typeof__(__mlib_VectorAve_S16) mlib_VectorAve_S16
    __attribute__((weak, alias("__mlib_VectorAve_S16")));
__typeof__(__mlib_VectorAve_S32) mlib_VectorAve_S32
    __attribute__((weak, alias("__mlib_VectorAve_S32")));
__typeof__(__mlib_VectorAve_S8C) mlib_VectorAve_S8C
    __attribute__((weak, alias("__mlib_VectorAve_S8C")));
__typeof__(__mlib_VectorAve_U8C) mlib_VectorAve_U8C
    __attribute__((weak, alias("__mlib_VectorAve_U8C")));
__typeof__(__mlib_VectorAve_S16C) mlib_VectorAve_S16C
    __attribute__((weak, alias("__mlib_VectorAve_S16C")));
__typeof__(__mlib_VectorAve_S32C) mlib_VectorAve_S32C
    __attribute__((weak, alias("__mlib_VectorAve_S32C")));
__typeof__(__mlib_VectorAve_S8_S8) mlib_VectorAve_S8_S8
    __attribute__((weak, alias("__mlib_VectorAve_S8_S8")));
__typeof__(__mlib_VectorAve_U8_U8) mlib_VectorAve_U8_U8
    __attribute__((weak, alias("__mlib_VectorAve_U8_U8")));
__typeof__(__mlib_VectorAve_S16_U8) mlib_VectorAve_S16_U8
    __attribute__((weak, alias("__mlib_VectorAve_S16_U8")));
__typeof__(__mlib_VectorAve_S16_S8) mlib_VectorAve_S16_S8
    __attribute__((weak, alias("__mlib_VectorAve_S16_S8")));
__typeof__(__mlib_VectorAve_S16_S16) mlib_VectorAve_S16_S16
    __attribute__((weak, alias("__mlib_VectorAve_S16_S16")));
__typeof__(__mlib_VectorAve_S32_S16) mlib_VectorAve_S32_S16
    __attribute__((weak, alias("__mlib_VectorAve_S32_S16")));
__typeof__(__mlib_VectorAve_S32_S32) mlib_VectorAve_S32_S32
    __attribute__((weak, alias("__mlib_VectorAve_S32_S32")));
__typeof__(__mlib_VectorAve_S8C_S8C) mlib_VectorAve_S8C_S8C
    __attribute__((weak, alias("__mlib_VectorAve_S8C_S8C")));
__typeof__(__mlib_VectorAve_U8C_U8C) mlib_VectorAve_U8C_U8C
    __attribute__((weak, alias("__mlib_VectorAve_U8C_U8C")));
__typeof__(__mlib_VectorAve_S16C_U8C) mlib_VectorAve_S16C_U8C
    __attribute__((weak, alias("__mlib_VectorAve_S16C_U8C")));
__typeof__(__mlib_VectorAve_S16C_S8C) mlib_VectorAve_S16C_S8C
    __attribute__((weak, alias("__mlib_VectorAve_S16C_S8C")));
__typeof__(__mlib_VectorAve_S16C_S16C) mlib_VectorAve_S16C_S16C
    __attribute__((weak, alias("__mlib_VectorAve_S16C_S16C")));
__typeof__(__mlib_VectorAve_S32C_S16C) mlib_VectorAve_S32C_S16C
    __attribute__((weak, alias("__mlib_VectorAve_S32C_S16C")));
__typeof__(__mlib_VectorAve_S32C_S32C) mlib_VectorAve_S32C_S32C
    __attribute__((weak, alias("__mlib_VectorAve_S32C_S32C")));
#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	MLIB_V_AVE_U8(dx, dy, dz)                     	\
	sd1h = vis_fpmerge(nul, vis_read_hi(dx));     	\
	sd2h = vis_fpmerge(nul, vis_read_hi(dy));     	\
	sd1l = vis_fmul8x16al(vis_read_lo(dx), fone); 	\
	sd2l = vis_fmul8x16al(vis_read_lo(dy), fone); 	\
	rdh = vis_fpadd16(sd1h, sd2h);                	\
	rdl = vis_fpadd16(sd1l, sd2l);                	\
	rzh = vis_fpadd16(rdh, inc);			\
	rzl = vis_fpadd16(rdl, inc);			\
	dz = vis_fpack16_pair(rzh, rzl);

/* *********************************************************** */

#define	AVE_WORK_LOOP_S32_IN_D64(z, x, y, n)      	\
	for (i = 0; i < n; i++) {                  	\
		z[i] = (mlib_s32)(((mlib_d64)(x[i])	\
			+ (y[i]))*0.5 + 0.5); 		\
	}

/* *********************************************************** */

#define	AVE_WORK_LOOP_S32_IN_S64(z, x, y, n)      	\
	for (i = 0; i < n; i++) {                  	\
		z[i] = (mlib_s32)(((mlib_s64)(x[i]) 	\
			+ (mlib_s64)(y[i]) + 1) >> 1); 	\
	}

/* *********************************************************** */

mlib_status
__mlib_VectorAve_U8_U8(
    mlib_u8 *z,
    const mlib_u8 *x,
    const mlib_u8 *y,
    mlib_s32 n)
{
	mlib_s32 i, j, k;
	mlib_s32 offdst, offsrc1, offsrc2, emask;
	mlib_s32 amount;
	mlib_d64 *dpp, *spp2, *spp1, *tmp_ptr;
	mlib_d64 dd, dd0, dd1, sd10, sd11, sd20, sd21;
	mlib_d64 sd1h, sd2h, sd1l, sd2l, rdh, rdl, rzh, rzl;
	mlib_f32 nul = vis_to_float(0), fone = vis_to_float(0x100);
	mlib_d64 inc = vis_to_double_dup(0x00010001);
	mlib_u8 *dend;
	mlib_u8 *sp1;
	mlib_u8 *sp2;
	mlib_u8 *dp;

	dp = z;
	sp1 = (mlib_u8 *)x;
	sp2 = (mlib_u8 *)y;

	if (n < 1) {
		return (MLIB_FAILURE);
	}

	vis_write_gsr(6 << 3);

	amount = n;

	offdst = ((mlib_addr)dp) & 7;
	offsrc1 = ((mlib_addr)sp1) & 7;
	offsrc2 = ((mlib_addr)sp2) & 7;

	if ((offdst == offsrc1) && (offdst == offsrc2)) {
		dpp = (mlib_d64 *)vis_alignaddr(dp, 0);
		i = (mlib_u8 *)dpp - dp;

		spp1 = (mlib_d64 *)vis_alignaddr(sp1, 0);
		spp2 = (mlib_d64 *)vis_alignaddr(sp2, 0);

		dend = dp + amount - 1;
		emask = vis_edge8(dp, dend);

		if (emask != 0xFF) {
			sd10 = (*spp1++);
			sd20 = (*spp2++);
			MLIB_V_AVE_U8(sd10, sd20, dd)
			vis_pst_8(dd, dpp++, emask);
			i += 8;
		}
#pragma pipeloop(0)
		for (; i <= amount - 8; i += 8) {
			sd10 = (*spp1++);
			sd20 = (*spp2++);
			MLIB_V_AVE_U8(sd10, sd20, dd)
			(*dpp++) = dd;
		}

		if (i < amount) {
			emask = vis_edge8(dpp, dend);
			sd10 = (*spp1++);
			sd20 = (*spp2++);
			MLIB_V_AVE_U8(sd10, sd20, dd)
			vis_pst_8(dd, dpp, emask);
		}
	} else if (offdst == offsrc1) {
		dpp = (mlib_d64 *)vis_alignaddr(dp, 0);
		i = (mlib_u8 *)dpp - dp;

		spp1 = (mlib_d64 *)vis_alignaddr(sp1, 0);
		spp2 = (mlib_d64 *)vis_alignaddr(sp2, i);

		dend = dp + amount - 1;
		emask = vis_edge8(dp, dend);

		sd20 = spp2[0];

		if (emask != 0xff) {
			sd10 = (*spp1++);
			sd21 = spp2[1];
			sd20 = vis_faligndata(sd20, sd21);
			MLIB_V_AVE_U8(sd10, sd20, dd)
			vis_pst_8(dd, dpp++, emask);
			sd20 = sd21;
			spp2++;
			i += 8;
		}
#pragma pipeloop(0)
		for (; i <= amount - 8; i += 8) {
			sd10 = (*spp1++);
			sd21 = spp2[1];
			sd20 = vis_faligndata(sd20, sd21);
			MLIB_V_AVE_U8(sd10, sd20, dd)
			(*dpp++) = dd;
			sd20 = sd21;
			spp2++;
		}

		if (i < amount) {
			emask = vis_edge8(dpp, dend);
			sd10 = (*spp1++);
			sd20 = vis_faligndata(sd20, spp2[1]);
			MLIB_V_AVE_U8(sd10, sd20, dd)
			vis_pst_8(dd, dpp, emask);
		}
	} else if (offdst == offsrc2) {
		dpp = (mlib_d64 *)vis_alignaddr(dp, 0);
		i = (mlib_u8 *)dpp - dp;

		spp2 = (mlib_d64 *)vis_alignaddr(sp2, 0);
		spp1 = (mlib_d64 *)vis_alignaddr(sp1, i);

		dend = dp + amount - 1;
		emask = vis_edge8(dp, dend);

		sd10 = spp1[0];
		if (emask != 0xff) {
			sd20 = (*spp2++);
			sd11 = spp1[1];
			sd10 = vis_faligndata(sd10, sd11);
			MLIB_V_AVE_U8(sd10, sd20, dd)
			vis_pst_8(dd, dpp++, emask);
			sd10 = sd11;
			spp1++;
			i += 8;
		}
#pragma pipeloop(0)
		for (; i <= amount - 8; i += 8) {
			sd20 = (*spp2++);
			sd11 = spp1[1];
			sd10 = vis_faligndata(sd10, sd11);
			MLIB_V_AVE_U8(sd10, sd20, dd)
			(*dpp++) = dd;
			sd10 = sd11;
			spp1++;
		}

		if (i < amount) {
			emask = vis_edge8(dpp, dend);
			sd20 = (*spp2++);
			sd10 = vis_faligndata(sd10, spp1[1]);
			MLIB_V_AVE_U8(sd10, sd20, dd)
			vis_pst_8(dd, dpp, emask);
		}
	} else if (offsrc1 == offsrc2) {

		dpp = (mlib_d64 *)vis_alignaddr(dp, 0);
		i = (mlib_u8 *)dpp - dp;

		spp1 = (mlib_d64 *)vis_alignaddr(sp1, i);
		spp2 = (mlib_d64 *)vis_alignaddr(sp2, i);

		dend = dp + amount - 1;
		emask = vis_edge8(dp, dend);

		sd10 = (*spp1++);
		sd20 = (*spp2++);
		MLIB_V_AVE_U8(sd10, sd20, dd0)
		if (emask != 0xff) {
			sd10 = (*spp1++);
			sd20 = (*spp2++);
			MLIB_V_AVE_U8(sd10, sd20, dd1)
			dd = vis_faligndata(dd0, dd1);
			vis_pst_8(dd, dpp++, emask);
			dd0 = dd1;
			i += 8;
		}
#pragma pipeloop(0)
		for (; i <= amount - 8; i += 8) {
			sd10 = (*spp1++);
			sd20 = (*spp2++);
			MLIB_V_AVE_U8(sd10, sd20, dd1)
			(*dpp++) = vis_faligndata(dd0, dd1);
			dd0 = dd1;
		}

		if (i < amount) {
			emask = vis_edge8(dpp, dend);
			sd10 = (*spp1++);
			sd20 = (*spp2++);
			MLIB_V_AVE_U8(sd10, sd20, dd1)
			dd = vis_faligndata(dd0, dd1);
			vis_pst_8(dd, dpp, emask);
		}
	} else {
		dpp = (mlib_d64 *)vis_alignaddr(dp, 0);
		i = (mlib_u8 *)dpp - dp;

		dend = dp + amount - 1;
		emask = vis_edge8(dp, dend);

		if (emask != 0xff) {
			spp1 = (mlib_d64 *)vis_alignaddr(sp1, i);
			sd10 = vis_faligndata(spp1[0], spp1[1]);
			spp2 = (mlib_d64 *)vis_alignaddr(sp2, i);
			sd20 = vis_faligndata(spp2[0], spp2[1]);
			MLIB_V_AVE_U8(sd10, sd20, dd)
			vis_pst_8(dd, dpp++, emask);
			i += 8;
		}

		spp1 = (mlib_d64 *)vis_alignaddr(sp1, i);
		sd11 = spp1[0];
		tmp_ptr = dpp;

#pragma pipeloop(0)
		for (k = i; k <= (amount - 8); k += 8) {
			sd10 = sd11;
			sd11 = spp1[1];
			(*tmp_ptr++) = vis_faligndata(sd10, sd11);
			spp1++;
		}
		sd11 = vis_faligndata(sd11, spp1[1]);

		spp2 = (mlib_d64 *)vis_alignaddr(sp2, i);
		sd20 = spp2[0];
		tmp_ptr = dpp;

#pragma pipeloop(0)
		for (; i <= amount - 8; i += 8) {
			sd10 = (*tmp_ptr++);
			sd21 = spp2[1];
			sd20 = vis_faligndata(sd20, sd21);
			MLIB_V_AVE_U8(sd10, sd20, dd)
			(*dpp++) = dd;
			sd20 = sd21;
			spp2++;
		}

		if (i < amount) {
			emask = vis_edge8(dpp, dend);
			sd20 = vis_faligndata(sd20, spp2[1]);
			MLIB_V_AVE_U8(sd11, sd20, dd)
			vis_pst_8(dd, dpp, emask);
		}
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MLIB_V_AVE_S8(dx, dy, dz)                    	\
	dx = vis_fxnor(dx, restore);                 	\
	dxh = vis_fmul8x16al(vis_read_hi(dx), fmul); 	\
	dxl = vis_fmul8x16al(vis_read_lo(dx), fmul); 	\
	dy = vis_fxnor(dy, restore);                 	\
	dyh = vis_fmul8x16al(vis_read_hi(dy), fmul); 	\
	dyl = vis_fmul8x16al(vis_read_lo(dy), fmul); 	\
	dzh = vis_fpadd16(dxh, dyh);                 	\
	dzl = vis_fpadd16(dxl, dyl);                 	\
	dh = vis_fpsub16(displacement, dzh);         	\
	dl = vis_fpsub16(displacement, dzl);         	\
	dz = vis_fpack16_pair(dh, dl);               	\
	dz = vis_fxor(dz, restore);

/* *********************************************************** */

mlib_status
__mlib_VectorAve_S8_S8(
    mlib_s8 *z,
    const mlib_s8 *x,
    const mlib_s8 *y,
    mlib_s32 n)
{
	mlib_d64 *dpz, *dpx, *dpy;
	mlib_d64 dx, dy, dz, dx0, dx1, dy0, dy1;
	mlib_d64 dxh, dxl, dyh, dyl, dzh, dzl;
	mlib_d64 dh, dl;
	mlib_s8 *pz = z, *px, *py, *pzend;
	mlib_s32 off;
	mlib_s32 len = n, i;
	mlib_s32 rest_8, even_8;
	mlib_s32 emask;
	mlib_d64 displacement = vis_to_double_dup(0x0ff80ff8);
	mlib_d64 restore = vis_to_double_dup(0x80808080);
	mlib_f32 fmul = vis_to_float(0x800);

	if (n <= 0) {
		return (MLIB_FAILURE);
	}

	px = (mlib_s8 *)x;
	py = (mlib_s8 *)y;

	vis_write_gsr(3 << 3);

	dpz = (mlib_d64 *)((mlib_addr)z & (~7));
	off = (mlib_u8 *)dpz - (mlib_u8 *)z;
	pzend = pz + n - 1;
	emask = vis_edge8(pz, pzend);

	if (off) {
		dpy = (mlib_d64 *)vis_alignaddr(py, off);
		dy0 = vis_ld_d64_nf(dpy);
		dy1 = vis_ld_d64_nf(dpy + 1);
		dy = vis_faligndata(dy0, dy1);
		dpx = (mlib_d64 *)vis_alignaddr(px, off);
		dx0 = vis_ld_d64_nf(dpx);
		dx1 = vis_ld_d64_nf(dpx + 1);
		dx = vis_faligndata(dx0, dx1);
		MLIB_V_AVE_S8(dx, dy, dz)

		vis_pst_8(dz, dpz, emask);

		px += (8 + off);
		py += (8 + off);
		len -= (8 + off);
		dpz++;

		if (len <= 0) {
			return (MLIB_SUCCESS);
		}
	}

	even_8 = len >> 3;
	rest_8 = len & 0x7;

	if (!(((mlib_addr)px | (mlib_addr)py) & 7)) {

		dpx = (mlib_d64 *)px;
		dpy = (mlib_d64 *)py;

		dx = vis_ld_d64_nf(dpx);
		dpx++;
		dy = vis_ld_d64_nf(dpy);
		dpy++;

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dx1 = vis_ld_d64_nf(dpx);
			dy1 = vis_ld_d64_nf(dpy);
			MLIB_V_AVE_S8(dx, dy, dz)
			dx = dx1;
			dy = dy1;

			dpz[0] = dz;
			dpx++;
			dpy++;
			dpz++;
		}

		dx1 = dx;
		dy1 = dy;
	} else if ((!((mlib_addr)px & 7))) {

/*
 * First ("x") address is 8-byte aligned.
 * vis_alignaddr and vis_faligndata only for "y".
 */

		dpx = (mlib_d64 *)px;
		dpy = vis_alignaddr(py, 0);
		dy0 = vis_ld_d64_nf(dpy);
		dpy++;
		dy1 = vis_ld_d64_nf(dpy);
		dy = vis_faligndata(dy0, dy1);
		dx = vis_ld_d64_nf(dpx);
		dpx++;

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			MLIB_V_AVE_S8(dx, dy, dz)
			dx = vis_ld_d64_nf(dpx);
			dy0 = dy1;
			dy1 = vis_ld_d64_nf(dpy + 1);
			dy = vis_faligndata(dy0, dy1);

			((*dpz++)) = dz;
			dpx++;
			dpy++;
		}

		dx1 = dx;
		dy1 = dy0;

	} else if ((!((mlib_addr)py & 7))) {

		dpy = (mlib_d64 *)py;
		dpx = vis_alignaddr(px, 0);
		dx1 = vis_ld_d64_nf(dpx);
		dpx++;

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dy = ((*dpy++));
			dx0 = dx1;
			dx1 = vis_ld_d64_nf(dpx);
			dpx++;
			dx = vis_faligndata(dx0, dx1);
			MLIB_V_AVE_S8(dx, dy, dz)

			((*dpz++)) = dz;
		}

		dy1 = vis_ld_d64_nf(dpy);
		dpy++;

	} else if ((((mlib_addr)px ^ (mlib_addr)py) & 7) == 0) {

		dpx = vis_alignaddr(px, 0);
		dx1 = vis_ld_d64_nf(dpx);
		dpx++;
		dpy = vis_alignaddr(py, 0);
		dy1 = vis_ld_d64_nf(dpy);
		dpy++;

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dy0 = dy1;
			dy1 = vis_ld_d64_nf(dpy);
			dpy++;
			dy = vis_faligndata(dy0, dy1);
			dx0 = dx1;
			dx1 = vis_ld_d64_nf(dpx);
			dpx++;
			dx = vis_faligndata(dx0, dx1);
			MLIB_V_AVE_S8(dx, dy, dz)

			((*dpz++)) = dz;
		}

	} else {

		dpx = vis_alignaddr(px, 0);
		dx0 = vis_ld_d64_nf(dpx);
		dpx++;
		dx1 = vis_ld_d64_nf(dpx);
		dx = vis_faligndata(dx0, dx1);
		dpy = vis_alignaddr(py, 0);
		dy0 = vis_ld_d64_nf(dpy);
		dpy++;
		dy1 = vis_ld_d64_nf(dpy);
		dy = vis_faligndata(dy0, dy1);

		for (i = 0; i < even_8; i++) {
			MLIB_V_AVE_S8(dx, dy, dz)
			vis_alignaddr(py, 0);
			dy0 = dy1;
			dy1 = vis_ld_d64_nf(dpy + 1);
			dy = vis_faligndata(dy0, dy1);
			vis_alignaddr(px, 0);
			dx0 = dx1;
			dx1 = vis_ld_d64_nf(dpx + 1);
			dx = vis_faligndata(dx0, dx1);

			((*dpz++)) = dz;
			dpy++;
			dpx++;
		}

		dx1 = dx0;
		dy1 = dy0;
	}

	if (!rest_8) {
		return (MLIB_SUCCESS);
	}

	vis_alignaddr(px, 0);
	dx0 = dx1;
	dx1 = vis_ld_d64_nf(dpx);
	dx = vis_faligndata(dx0, dx1);
	vis_alignaddr(py, 0);
	dy0 = dy1;
	dy1 = vis_ld_d64_nf(dpy);
	dy = vis_faligndata(dy0, dy1);
	MLIB_V_AVE_S8(dx, dy, dz)

	emask = vis_edge8((void *)(rest_8), pzend);

	vis_pst_8(dz, dpz, ~emask);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MLIB_V_AVE_S16_S8_ADD                        \
	dx = vis_fxnor(dx, restore);                 \
	dy = vis_fxnor(dy, restore);                 \
	dxh = vis_fmul8x16al(vis_read_hi(dx), fone); \
	dyh = vis_fmul8x16al(vis_read_hi(dy), fone); \
	dxl = vis_fmul8x16al(vis_read_lo(dx), fone); \
	dyl = vis_fmul8x16al(vis_read_lo(dy), fone);

/* *********************************************************** */

#define	MLIB_V_AVE_S16_S8_MUL                 \
	dzh = vis_fpadd16(dxh, dyh);          \
	dzl = vis_fpadd16(dxl, dyl);          \
	dzh = vis_fpsub16(displacement, dzh); \
	dzl = vis_fpsub16(displacement, dzl); \
	dzh = vis_fmul8x16(div2mask, dzh);    \
	dzl = vis_fmul8x16(div2mask, dzl);

/* *********************************************************** */

mlib_status
__mlib_VectorAve_S16_S8(
    mlib_s16 *z,
    const mlib_s8 *x,
    const mlib_s8 *y,
    mlib_s32 n)
{
	mlib_d64 *dpz, *dpx, *dpy;
	mlib_d64 dx, dy, dx0, dx1, dy0, dy1;
	mlib_d64 dxh, dxl, dyh, dyl, dzh, dzl;
	mlib_s8 *px, *py;
	mlib_s16 *pz;
	mlib_f32 fone = vis_to_float(0x1000);
	mlib_d64 restore = vis_to_double_dup(0x80808080);
	mlib_d64 displacement = vis_to_double_dup(0x0fe00fe0);
	mlib_f32 div2mask = vis_to_float(0x08080808);
	mlib_s32 len = n, i;

/* rest and leng in terms of 8 bytes. */
	mlib_s32 rest_8, even_8;

	if (n <= 0) {
		return (MLIB_FAILURE);
	}

	px = (mlib_s8 *)x;
	py = (mlib_s8 *)y;
	pz = (mlib_s16 *)z;

	if (n <= 8) {
		while (n--) {
			((*pz++)) = (((mlib_s16)(*px)) + (*py) + 1) >> 1;
			px++;
			py++;
		}
		return (MLIB_SUCCESS);
	}

	while ((mlib_addr)pz & 7) {
		((*pz++)) = (((mlib_s16)(*px)) + (*py) + 1) >> 1;
		px++;
		py++;
		len--;
	}

	dpz = (mlib_d64 *)pz;

	even_8 = len >> 3;
	rest_8 = len & 0x7;

	if (!(((mlib_addr)px | (mlib_addr)py) & 7)) {

		dpx = (mlib_d64 *)px;
		dpy = (mlib_d64 *)py;

		dx = vis_ld_d64_nf(dpx);
		dy = vis_ld_d64_nf(dpy);
		dpx++;
		dpy++;
		dx1 = vis_ld_d64_nf(dpx);
		dy1 = vis_ld_d64_nf(dpy);

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			MLIB_V_AVE_S16_S8_ADD
			MLIB_V_AVE_S16_S8_MUL
			dx = dx1;
			dy = dy1;

			dpz[0] = dzh;
			dpz[1] = dzl;
			dpx++;
			dpy++;
			dpz += 2;
			dx1 = vis_ld_d64_nf(dpx);
			dy1 = vis_ld_d64_nf(dpy);
		}
	} else if ((!((mlib_addr)px & 7))) {

/*
 * First ("x") address is 8-byte aligned.
 * vis_alignaddr and vis_faligndata only for "y".
 */

		dpx = (mlib_d64 *)px;
		dx = vis_ld_d64_nf(dpx);

		dpy = vis_alignaddr(py, 0);
		dy0 = vis_ld_d64_nf(dpy);
		dpy++;
		dy1 = vis_ld_d64_nf(dpy);
		dy = vis_faligndata(dy0, dy1);
		MLIB_V_AVE_S16_S8_ADD

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			MLIB_V_AVE_S16_S8_MUL
			dpz[0] = dzh;
			dpz[1] = dzl;
			dpz += 2;
			dpx++;
			dpy++;
			dx = vis_ld_d64_nf(dpx);
			dy0 = dy1;
			dy1 = vis_ld_d64_nf(dpy);
			dy = vis_faligndata(dy0, dy1);
			MLIB_V_AVE_S16_S8_ADD
		}

	} else if ((!((mlib_addr)py & 7))) {

		dpy = (mlib_d64 *)py;
		dy = vis_ld_d64_nf(dpy);

		dpx = vis_alignaddr(px, 0);
		dx0 = vis_ld_d64_nf(dpx);
		dpx++;
		dx1 = vis_ld_d64_nf(dpx);
		dx = vis_faligndata(dx0, dx1);
		MLIB_V_AVE_S16_S8_ADD

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			MLIB_V_AVE_S16_S8_MUL
			dpz[0] = dzh;
			dpz[1] = dzl;
			dpz += 2;
			dpx++;
			dpy++;
			dy = vis_ld_d64_nf(dpy);
			dx0 = dx1;
			dx1 = vis_ld_d64_nf(dpx);
			dx = vis_faligndata(dx0, dx1);
			MLIB_V_AVE_S16_S8_ADD
		}

	} else if ((((mlib_addr)px ^ (mlib_addr)py) & 7) == 0) {

		dpx = vis_alignaddr(px, 0);
		dx0 = vis_ld_d64_nf(dpx);
		dpx++;
		dx1 = vis_ld_d64_nf(dpx);
		dx = vis_faligndata(dx0, dx1);

		dpy = vis_alignaddr(py, 0);
		dy0 = vis_ld_d64_nf(dpy);
		dpy++;
		dy1 = vis_ld_d64_nf(dpy);
		dy = vis_faligndata(dy0, dy1);
		MLIB_V_AVE_S16_S8_ADD
#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			MLIB_V_AVE_S16_S8_MUL
			dpz[0] = dzh;
			dpz[1] = dzl;
			dpz += 2;
			dpx++;
			dpy++;
			dy0 = dy1;
			dy1 = vis_ld_d64_nf(dpy);
			dy = vis_faligndata(dy0, dy1);

			dx0 = dx1;
			dx1 = vis_ld_d64_nf(dpx);
			dx = vis_faligndata(dx0, dx1);
			MLIB_V_AVE_S16_S8_ADD
		}

	} else {

		dpx = vis_alignaddr(px, 0);
		dx0 = vis_ld_d64_nf(dpx);
		dpx++;
		dx1 = vis_ld_d64_nf(dpx);
		dx = vis_faligndata(dx0, dx1);

		dpy = vis_alignaddr(py, 0);
		dy0 = vis_ld_d64_nf(dpy);
		dpy++;
		dy1 = vis_ld_d64_nf(dpy);
		dy = vis_faligndata(dy0, dy1);
		MLIB_V_AVE_S16_S8_ADD
#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			MLIB_V_AVE_S16_S8_MUL
			vis_alignaddr(py, (mlib_addr)dpy);
			dy0 = dy1;
			dy1 = vis_ld_d64_nf(dpy + 1);
			dy = vis_faligndata(dy0, dy1);
			vis_alignaddr(px, (mlib_addr)dpx);
			dx0 = dx1;
			dx1 = vis_ld_d64_nf(dpx + 1);
			dx = vis_faligndata(dx0, dx1);
			dpz[0] = dzh;
			dpz[1] = dzl;
			dpz += 2;
			dpy++;
			dpx++;
			MLIB_V_AVE_S16_S8_ADD
		}
	}

	if (!rest_8) {
		return (MLIB_SUCCESS);
	}

	px += (even_8 << 3);
	py += (even_8 << 3);
	pz += (even_8 << 3);

	while (rest_8--) {
		((*pz++)) = (((mlib_s16)(*px)) + (*py) + 1) >> 1;
		px++;
		py++;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MLIB_V_AVE_S16_U8                      		\
	dxh = vis_fpmerge(fzero, vis_read_hi(dx)); 	\
	dyh = vis_fpmerge(fzero, vis_read_hi(dy)); 	\
	dxl = vis_fpmerge(fzero, vis_read_lo(dx)); 	\
	dyl = vis_fpmerge(fzero, vis_read_lo(dy)); 	\
	dzh = vis_fpadd16(dxh, dyh);                 	\
	dzl = vis_fpadd16(dxl, dyl);                 	\
	dzh = vis_fmul8x16(div2mask, dzh);           	\
	dzl = vis_fmul8x16(div2mask, dzl);

/* *********************************************************** */

mlib_status
__mlib_VectorAve_S16_U8(
    mlib_s16 *z,
    const mlib_u8 *x,
    const mlib_u8 *y,
    mlib_s32 n)
{
	mlib_d64 *dpz, *dpx, *dpy;
	mlib_d64 dx, dy, dx0, dx1, dy0, dy1;
	mlib_d64 dxh, dxl, dyh, dyl, dzh, dzl;
	mlib_f32 fzero = vis_fzeros();
	mlib_f32 div2mask = vis_to_float(0x80808080);
	mlib_u8 *px, *py;
	mlib_s16 *pz;
	mlib_s32 len = n, i;
	mlib_s32 rest_8, even_8;

	if (n <= 0) {
		return (MLIB_FAILURE);
	}

	px = (mlib_u8 *)x;
	py = (mlib_u8 *)y;
	pz = (mlib_s16 *)z;

	if (n <= 8) {
		while (n--) {
			((*pz++)) = (((mlib_s16)(*px)) + (*py) + 1) >> 1;
			px++;
			py++;
		}
		return (MLIB_SUCCESS);
	}

	while ((mlib_addr)pz & 7) {
		((*pz++)) = (((mlib_s16)(*px)) + (*py) + 1) >> 1;
		px++;
		py++;
		len--;
	}

	dpz = (mlib_d64 *)pz;

	even_8 = len >> 3;
	rest_8 = len & 0x7;

	if (!(((mlib_addr)px | (mlib_addr)py) & 7)) {
		dpx = (mlib_d64 *)px;
		dpy = (mlib_d64 *)py;

		dx = vis_ld_d64_nf(dpx);
		dy = vis_ld_d64_nf(dpy);
		dpx++;
		dpy++;

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dx1 = vis_ld_d64_nf(dpx);
			dy1 = vis_ld_d64_nf(dpy);
			MLIB_V_AVE_S16_U8 dx = dx1;
			dy = dy1;

/*
 * store 16 bytes of result
 */
			((*dpz++)) = dzh;
			((*dpz++)) = dzl;
			dpx++;
			dpy++;
		}

	} else if ((!((mlib_addr)px & 7))) {

		dpx = (mlib_d64 *)px;
		dpy = vis_alignaddr(py, 0);
		dy1 = vis_ld_d64_nf(dpy);
		dpy++;

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dx = ((*dpx++));
			dy0 = dy1;
			dy1 = vis_ld_d64_nf(dpy);
			dpy++;
			dy = vis_faligndata(dy0, dy1);
			MLIB_V_AVE_S16_U8
			dpz[0] = dzh;
			dpz[1] = dzl;
			dpz += 2;
		}

	} else if ((!((mlib_addr)py & 7))) {

		dpy = (mlib_d64 *)py;
		dpx = vis_alignaddr(px, 0);
		dx1 = vis_ld_d64_nf(dpx);
		dpx++;

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dy = ((*dpy++));
			dx0 = dx1;
			dx1 = vis_ld_d64_nf(dpx);
			dpx++;
			dx = vis_faligndata(dx0, dx1);
			MLIB_V_AVE_S16_U8
			dpz[0] = dzh;
			dpz[1] = dzl;
			dpz += 2;
		}

	} else if ((((mlib_addr)px ^ (mlib_addr)py) & 7) == 0) {

		dpx = vis_alignaddr(px, 0);
		dx1 = vis_ld_d64_nf(dpx);
		dpx++;
		dpy = vis_alignaddr(py, 0);
		dy1 = vis_ld_d64_nf(dpy);
		dpy++;

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dy0 = dy1;
			dy1 = vis_ld_d64_nf(dpy);
			dpy++;
			dy = vis_faligndata(dy0, dy1);
			dx0 = dx1;
			dx1 = vis_ld_d64_nf(dpx);
			dpx++;
			dx = vis_faligndata(dx0, dx1);
			MLIB_V_AVE_S16_U8
			dpz[0] = dzh;
			dpz[1] = dzl;
			dpz += 2;
		}

	} else {

		dpx = vis_alignaddr(px, 0);
		dx0 = vis_ld_d64_nf(dpx);
		dpx++;
		dx1 = vis_ld_d64_nf(dpx);
		dx = vis_faligndata(dx0, dx1);
		dpy = vis_alignaddr(py, 0);
		dy0 = vis_ld_d64_nf(dpy);
		dpy++;
		dy1 = vis_ld_d64_nf(dpy);
		dy = vis_faligndata(dy0, dy1);

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			MLIB_V_AVE_S16_U8 vis_alignaddr(py, 0);
			dy0 = dy1;
			dy1 = vis_ld_d64_nf(dpy + 1);
			dy = vis_faligndata(dy0, dy1);
			vis_alignaddr(px, 0);
			dx0 = dx1;
			dx1 = vis_ld_d64_nf(dpx + 1);
			dx = vis_faligndata(dx0, dx1);
			dpz[0] = dzh;
			dpz[1] = dzl;
			dpz += 2;
			dpy++;
			dpx++;
		}
	}

	if (!rest_8) {
		return (MLIB_SUCCESS);
	}

	px += (even_8 << 3);
	py += (even_8 << 3);
	pz += (even_8 << 3);

	while (rest_8--) {
		((*pz++)) = (((mlib_s16)(*px)) + (*py) + 1) >> 1;
		px++;
		py++;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MLIB_V_AVE_S16_S16         		\
	inc = vis_fand(dx, dy);			\
	inc = vis_fand(inc, roundmask);		\
	dx = vis_fmul8x16(div2mask, dx); 	\
	dy = vis_fmul8x16(div2mask, dy); 	\
	dz = vis_fpadd16(dx, dy);        	\
	dz = vis_fpsub16(dz, inc);		\
	vis_pst_16(dz, dpz, emask);

/* *********************************************************** */

mlib_status
__mlib_VectorAve_S16_S16(
    mlib_s16 *z,
    const mlib_s16 *x,
    const mlib_s16 *y,
    mlib_s32 n)
{
	mlib_d64 roundmask = vis_to_double_dup(0x00010001);
	mlib_f32 div2mask = vis_to_float(0x80808080);
	mlib_d64 *dpz, *dpx, *dpy;
	mlib_d64 dx, dy, dz, dx0, dx1, dy0, dy1, inc;
	mlib_s16 *pz, *px, *py, *pzend;
	mlib_s32 off;
	mlib_s32 emask;
	mlib_s32 len = n, i;
	mlib_s32 rest_8, even_8;

	if (n <= 0) {
		return (MLIB_FAILURE);
	}

	px = (mlib_s16 *)x;
	py = (mlib_s16 *)y;
	pz = (mlib_s16 *)z;

	dpz = (mlib_d64 *)((mlib_addr)z & (~7));
	off = (long)dpz - (long)z;
	pzend = pz + n - 1;
	emask = vis_edge16(pz, pzend);

	if (off) {
		dpy = (mlib_d64 *)vis_alignaddr(py, off);
		dy0 = vis_ld_d64_nf(dpy);
		dy1 = vis_ld_d64_nf(dpy + 1);
		dy = vis_faligndata(dy0, dy1);
		dpx = (mlib_d64 *)vis_alignaddr(px, off);
		dx0 = vis_ld_d64_nf(dpx);
		dx1 = vis_ld_d64_nf(dpx + 1);
		dx = vis_faligndata(dx0, dx1);
		MLIB_V_AVE_S16_S16 px += (8 + off) >> 1;
		py += (8 + off) >> 1;
		len -= (8 + off) >> 1;
		dpz++;
	}

	if (len <= 0) {
		return (MLIB_SUCCESS);
	}

	even_8 = len >> 2;
	rest_8 = len & 0x3;
	emask = 0xf;

	if ((!((mlib_addr)px & 7)) && (!((mlib_addr)py & 7))) {
		dpx = (mlib_d64 *)px;
		dpy = (mlib_d64 *)py;

		dx = vis_ld_d64_nf(dpx);
		dpx++;
		dy = vis_ld_d64_nf(dpy);
		dpy++;

		for (i = 0; i < even_8; i++) {
			dx1 = vis_ld_d64_nf(dpx);
			dy1 = vis_ld_d64_nf(dpy);
			MLIB_V_AVE_S16_S16 dx = dx1;
			dy = dy1;
			dpx++;
			dpy++;
			dpz++;
		}

		dx1 = dx;
		dy1 = dy;
	} else if ((!((mlib_addr)px & 7))) {
		dpx = (mlib_d64 *)px;
		dpy = vis_alignaddr(py, 0);
		dy0 = vis_ld_d64_nf(dpy);
		dpy++;
		dy1 = vis_ld_d64_nf(dpy);
		dy = vis_faligndata(dy0, dy1);
		dx = vis_ld_d64_nf(dpx);
		dpx++;

		for (i = 0; i < even_8; i++) {
			MLIB_V_AVE_S16_S16 dx = vis_ld_d64_nf(dpx);
			dy0 = dy1;
			dy1 = vis_ld_d64_nf(dpy + 1);
			dy = vis_faligndata(dy0, dy1);
			dpz++;
			dpx++;
			dpy++;
		}

		dx1 = dx;
		dy1 = dy0;

	} else if ((!((mlib_addr)py & 7))) {

		dpy = (mlib_d64 *)py;
		dpx = vis_alignaddr(px, 0);
		dx1 = vis_ld_d64_nf(dpx);
		dpx++;

		for (i = 0; i < even_8; i++) {
			dy = ((*dpy++));
			dx0 = dx1;
			dx1 = vis_ld_d64_nf(dpx);
			dpx++;
			dx = vis_faligndata(dx0, dx1);
			MLIB_V_AVE_S16_S16 dpz++;
		}

		dy1 = vis_ld_d64_nf(dpy);
		dpy++;

	} else if (((mlib_addr)px & 7) == ((mlib_addr)py & 7)) {

		dpx = vis_alignaddr(px, 0);
		dx1 = vis_ld_d64_nf(dpx);
		dpx++;
		dpy = vis_alignaddr(py, 0);
		dy1 = vis_ld_d64_nf(dpy);
		dpy++;

		for (i = 0; i < even_8; i++) {
			dy0 = dy1;
			dy1 = vis_ld_d64_nf(dpy);
			dpy++;
			dy = vis_faligndata(dy0, dy1);
			dx0 = dx1;
			dx1 = vis_ld_d64_nf(dpx);
			dpx++;
			dx = vis_faligndata(dx0, dx1);
			MLIB_V_AVE_S16_S16 dpz++;
		}

	} else {

		dpy = vis_alignaddr(py, 0);
		dy0 = vis_ld_d64_nf(dpy);
		dpy++;
		dy1 = vis_ld_d64_nf(dpy);
		dy = vis_faligndata(dy0, dy1);
		dpx = vis_alignaddr(px, 0);
		dx0 = vis_ld_d64_nf(dpx);
		dpx++;
		dx1 = vis_ld_d64_nf(dpx);

		for (i = 0; i < even_8; i++) {
			dx = vis_faligndata(dx0, dx1);
			MLIB_V_AVE_S16_S16 vis_alignaddr(py, 0);
			dy0 = dy1;
			dy1 = vis_ld_d64_nf(dpy + 1);
			dy = vis_faligndata(dy0, dy1);
			vis_alignaddr(px, 0);
			dx0 = dx1;
			dx1 = vis_ld_d64_nf(dpx + 1);
			dpz++;
			dpy++;
			dpx++;
		}

		dx1 = dx0;
		dy1 = dy0;
	}

	if (!rest_8) {
		return (MLIB_SUCCESS);
	}

	emask = ~(vis_edge16((void *)(rest_8 << 1), pzend));

	vis_alignaddr(px, 0);
	dx0 = dx1;
	dx1 = vis_ld_d64_nf(dpx);
	dx = vis_faligndata(dx0, dx1);
	vis_alignaddr(py, 0);
	dy0 = dy1;
	dy1 = vis_ld_d64_nf(dpy);
	dy = vis_faligndata(dy0, dy1);

	MLIB_V_AVE_S16_S16 return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	MLIB_V_AVE_S32_S16                                      \
	inc = vis_fand(dx, dy);					\
	inc = vis_fand(inc, roundmask);				\
	dx = vis_fmul8x16(div2mask, dx);                        \
	dy = vis_fmul8x16(div2mask, dy);                        \
	dx = vis_fpadd16(dx, dy);                               \
	dx = vis_fpsub16(dx, inc);				\
	dzh = vis_fmuld8ulx16(two_16_ones, vis_read_hi(dx));    \
	dzl = vis_fmuld8ulx16(two_16_ones, vis_read_lo(dx));

/* *********************************************************** */

mlib_status
__mlib_VectorAve_S32_S16(
    mlib_s32 *z,
    const mlib_s16 *x,
    const mlib_s16 *y,
    mlib_s32 n)
{
	mlib_d64 *dpz, *dpx, *dpy;
	mlib_d64 dx, dy, dx0, dx1, dy0, dy1;
	mlib_d64 dxh, dxl, dyh, dyl, dzh, dzl;
	mlib_d64 dbit, dbith, dbitl;
	mlib_s16 *px, *py;
	mlib_s32 *pz;
	mlib_s32 len = n, i;
	mlib_s32 rest_8, even_8;
	mlib_d64 inc, roundmask = vis_to_double_dup(0x00010001);
	mlib_f32 div2mask = vis_to_float(0x80808080);
	mlib_f32 two_16_ones = vis_to_float(0x00010001);

	if (n <= 0) {
		return (MLIB_FAILURE);
	}

	px = (mlib_s16 *)x;
	py = (mlib_s16 *)y;
	pz = (mlib_s32 *)z;

	if (n <= 4) {
		while (n--) {
			((*pz++)) = (((mlib_s32)(*px)) + *py + 1) >> 1;
			px++;
			py++;
		}
		return (MLIB_SUCCESS);
	}

	if ((mlib_addr)pz & 7) {
		((*pz++)) = (((mlib_s32)(*px)) + (*py) + 1) >> 1;
		px++;
		py++;
		len--;
	}

	dpz = (mlib_d64 *)pz;

	even_8 = len >> 2;
	rest_8 = len & 0x3;

	if ((!((mlib_addr)px & 7)) && (!((mlib_addr)py & 7))) {

		dpx = (mlib_d64 *)px;
		dpy = (mlib_d64 *)py;

		dx = vis_ld_d64_nf(dpx);
		dy = vis_ld_d64_nf(dpy);
		dpx++;
		dpy++;

		for (i = 0; i < even_8; i++) {
			dx1 = vis_ld_d64_nf(dpx);
			dy1 = vis_ld_d64_nf(dpy);
			MLIB_V_AVE_S32_S16
			dx = dx1;
			dy = dy1;
			((*dpz++)) = dzh;
			((*dpz++)) = dzl;
			dpx++;
			dpy++;
		}

	} else if ((!((mlib_addr)px & 7))) {

		dpx = (mlib_d64 *)px;
		dpy = vis_alignaddr(py, 0);
		dy1 = vis_ld_d64_nf(dpy);
		dpy++;

		for (i = 0; i < even_8; i++) {
			dx = ((*dpx++));
			dy0 = dy1;
			dy1 = vis_ld_d64_nf(dpy);
			dpy++;
			dy = vis_faligndata(dy0, dy1);
			MLIB_V_AVE_S32_S16
			dpz[0] = dzh;
			dpz[1] = dzl;
			dpz += 2;
		}

	} else if ((!((mlib_addr)py & 7))) {

		dpy = (mlib_d64 *)py;
		dpx = vis_alignaddr(px, 0);
		dx1 = vis_ld_d64_nf(dpx);
		dpx++;

		for (i = 0; i < even_8; i++) {
			dy = ((*dpy++));
			dx0 = dx1;
			dx1 = vis_ld_d64_nf(dpx);
			dpx++;
			dx = vis_faligndata(dx0, dx1);
			MLIB_V_AVE_S32_S16
			dpz[0] = dzh;
			dpz[1] = dzl;
			dpz += 2;
		}

	} else if (((mlib_addr)px & 7) == ((mlib_addr)py & 7)) {

		dpx = vis_alignaddr(px, 0);
		dx1 = vis_ld_d64_nf(dpx);
		dpx++;
		dpy = vis_alignaddr(py, 0);
		dy1 = vis_ld_d64_nf(dpy);
		dpy++;

		for (i = 0; i < even_8; i++) {
			dy0 = dy1;
			dy1 = vis_ld_d64_nf(dpy);
			dpy++;
			dy = vis_faligndata(dy0, dy1);
			dx0 = dx1;
			dx1 = vis_ld_d64_nf(dpx);
			dpx++;
			dx = vis_faligndata(dx0, dx1);
			MLIB_V_AVE_S32_S16
			dpz[0] = dzh;
			dpz[1] = dzl;
			dpz += 2;
		}

	} else {

		dpx = vis_alignaddr(px, 0);
		dx0 = vis_ld_d64_nf(dpx);
		dpx++;
		dx1 = vis_ld_d64_nf(dpx);
		dx = vis_faligndata(dx0, dx1);
		dpy = vis_alignaddr(py, 0);
		dy0 = vis_ld_d64_nf(dpy);
		dpy++;
		dy1 = vis_ld_d64_nf(dpy);
		dy = vis_faligndata(dy0, dy1);

		for (i = 0; i < even_8; i++) {
			MLIB_V_AVE_S32_S16
			vis_alignaddr(py, 0);
			dy0 = dy1;
			dy1 = vis_ld_d64_nf(dpy + 1);
			dy = vis_faligndata(dy0, dy1);
			vis_alignaddr(px, 0);
			dx0 = dx1;
			dx1 = vis_ld_d64_nf(dpx + 1);
			dx = vis_faligndata(dx0, dx1);
			dpz[0] = dzh;
			dpz[1] = dzl;
			dpz += 2;
			dpy++;
			dpx++;
		}
	}

	if (!rest_8) {
		return (MLIB_SUCCESS);
	}

	px += (even_8 << 2);
	py += (even_8 << 2);
	pz += (even_8 << 2);

	while (rest_8--) {
		((*pz++)) = (((mlib_s32)(*px)) + (*py) + 1) >> 1;
		px++;
		py++;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorAve_S32_S32(
	mlib_s32			 *z,
	const mlib_s32 *x,
	const mlib_s32 *y,
	mlib_s32			 n)
{
	mlib_s32 i;

	if (n < 1) {
		return (MLIB_FAILURE);
	}

#ifdef _NO_LONGLONG

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	AVE_WORK_LOOP_S32_IN_D64(z, x, y, n)

#else /* _NO_LONGLONG */

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	AVE_WORK_LOOP_S32_IN_S64(z, x, y, n)

#endif  /* _NO_LONGLONG */

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorAve_U8(
    mlib_u8 *xz,
    const mlib_u8 *y,
    mlib_s32 n)
{
	mlib_s32 i, j, k;
	mlib_s32 offdst, offsrc2, emask;
	mlib_s32 amount;
	mlib_d64 *dpp, *spp2;
	mlib_d64 dd, dd0, dd1, sd10, sd11, sd20, sd21;
	mlib_d64 sd1h, sd2h, sd1l, sd2l, rdh, rdl, rzh, rzl;
	mlib_f32 nul = vis_to_float(0), fone = vis_to_float(0x100);
	mlib_d64 inc = vis_to_double_dup(0x00010001);
	mlib_u8 *dend;
	mlib_u8 *sp2;
	mlib_u8 *dp;

	dp = xz;
	sp2 = (mlib_u8 *)y;

	if (n < 1) {
		return (MLIB_FAILURE);
	}

	vis_write_gsr(6 << 3);

	amount = n;

	offdst = ((mlib_addr)dp) & 7;
	offsrc2 = ((mlib_addr)sp2) & 7;

	if (offdst == offsrc2) {
		dpp = (mlib_d64 *)vis_alignaddr(dp, 0);
		i = (mlib_u8 *)dpp - dp;

		spp2 = (mlib_d64 *)vis_alignaddr(sp2, 0);

		dend = dp + amount - 1;
		emask = vis_edge8(dp, dend);

		if (emask != 0xFF) {
			sd10 = *dpp;
			sd20 = (*spp2++);
			MLIB_V_AVE_U8(sd10, sd20, dd)
			vis_pst_8(dd, dpp++, emask);
			i += 8;
		}
#pragma pipeloop(0)
		for (; i <= amount - 8; i += 8) {
			sd10 = *dpp;
			sd20 = (*spp2++);
			MLIB_V_AVE_U8(sd10, sd20, dd)
			(*dpp++) = dd;
		}

		if (i < amount) {
			emask = vis_edge8(dpp, dend);
			sd10 = *dpp;
			sd20 = (*spp2++);
			MLIB_V_AVE_U8(sd10, sd20, dd)
			vis_pst_8(dd, dpp, emask);
		}

	} else {

		dpp = (mlib_d64 *)vis_alignaddr(dp, 0);
		i = (mlib_u8 *)dpp - dp;

		spp2 = (mlib_d64 *)vis_alignaddr(sp2, i);

		dend = dp + amount - 1;
		emask = vis_edge8(dp, dend);

		sd20 = spp2[0];

		if (emask != 0xff) {
			sd10 = *dpp;
			sd21 = spp2[1];
			sd20 = vis_faligndata(sd20, sd21);
			MLIB_V_AVE_U8(sd10, sd20, dd)
			vis_pst_8(dd, dpp++, emask);
			sd20 = sd21;
			spp2++;
			i += 8;
		}

#pragma pipeloop(0)
		for (; i <= amount - 8; i += 8) {
			sd10 = *dpp;
			sd21 = spp2[1];
			sd20 = vis_faligndata(sd20, sd21);
			MLIB_V_AVE_U8(sd10, sd20, dd)
			(*dpp++) = dd;
			sd20 = sd21;
			spp2++;
		}

		if (i < amount) {
			emask = vis_edge8(dpp, dend);
			sd10 = *dpp;
			sd20 = vis_faligndata(sd20, spp2[1]);
			MLIB_V_AVE_U8(sd10, sd20, dd)
			vis_pst_8(dd, dpp, emask);
		}
	}
	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorAve_S8(
    mlib_s8 *xz,
    const mlib_s8 *y,
    mlib_s32 n)
{
	mlib_d64 *dpz, *dpx, *dpy;
	mlib_d64 dx, dy, dz, dx0, dx1, dy0, dy1;
	mlib_d64 dxh, dxl, dyh, dyl, dzh, dzl;
	mlib_d64 dh, dl;
	mlib_s8 *pz = xz, *px, *py, *pzend;
	mlib_s32 off;
	mlib_s32 len = n, i;
	mlib_s32 rest_8, even_8;
	mlib_s32 emask;
	mlib_d64 displacement = vis_to_double_dup(0x0ff80ff8);
	mlib_d64 restore = vis_to_double_dup(0x80808080);
	mlib_f32 fmul = vis_to_float(0x800);

	if (n <= 0) {
		return (MLIB_FAILURE);
	}

	px = pz;
	py = (mlib_s8 *)y;

	vis_write_gsr(3 << 3);

	dpz = (mlib_d64 *)((mlib_addr)xz & (~7));
	off = (mlib_u8 *)dpz - (mlib_u8 *)xz;
	pzend = pz + n - 1;
	emask = vis_edge8(pz, pzend);

	if (off) {
		dpy = (mlib_d64 *)vis_alignaddr(py, off);
		dy0 = vis_ld_d64_nf(dpy);
		dy1 = vis_ld_d64_nf(dpy + 1);
		dy = vis_faligndata(dy0, dy1);
		dpx = (mlib_d64 *)vis_alignaddr(px, off);
		dx0 = vis_ld_d64_nf(dpx);
		dx1 = vis_ld_d64_nf(dpx + 1);
		dx = vis_faligndata(dx0, dx1);
		MLIB_V_AVE_S8(dx, dy, dz)

		vis_pst_8(dz, dpz, emask);

		px += (8 + off);
		py += (8 + off);
		len -= (8 + off);
		dpz++;

		if (len <= 0) {
			return (MLIB_SUCCESS);
		}
	}

	even_8 = len >> 3;
	rest_8 = len & 0x7;

	if (!(((mlib_addr)py) & 7)) {

		dpx = (mlib_d64 *)px;
		dpy = (mlib_d64 *)py;

		dx = vis_ld_d64_nf(dpx);
		dpx++;
		dy = vis_ld_d64_nf(dpy);
		dpy++;

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dx1 = vis_ld_d64_nf(dpx);
			dy1 = vis_ld_d64_nf(dpy);
			MLIB_V_AVE_S8(dx, dy, dz)
			    dx = dx1;
			dy = dy1;

			dpz[0] = dz;
			dpx++;
			dpy++;
			dpz++;
		}

		dx1 = dx;
		dy1 = dy;
	} else {

		dpx = (mlib_d64 *)px;
		dpy = vis_alignaddr(py, 0);
		dy0 = vis_ld_d64_nf(dpy);
		dpy++;
		dy1 = vis_ld_d64_nf(dpy);
		dy = vis_faligndata(dy0, dy1);
		dx = vis_ld_d64_nf(dpx);
		dpx++;

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			MLIB_V_AVE_S8(dx, dy, dz)
			    dx = vis_ld_d64_nf(dpx);
			dy0 = dy1;
			dy1 = vis_ld_d64_nf(dpy + 1);
			dy = vis_faligndata(dy0, dy1);

			((*dpz++)) = dz;
			dpx++;
			dpy++;
		}

		dx1 = dx;
		dy1 = dy0;
	}

	if (!rest_8) {
		return (MLIB_SUCCESS);
	}

	vis_alignaddr(px, 0);
	dx0 = dx1;
	dx1 = vis_ld_d64_nf(dpx);
	dx = vis_faligndata(dx0, dx1);
	vis_alignaddr(py, 0);
	dy0 = dy1;
	dy1 = vis_ld_d64_nf(dpy);
	dy = vis_faligndata(dy0, dy1);
	MLIB_V_AVE_S8(dx, dy, dz)

	emask = vis_edge8((void *)(rest_8), pzend);

	vis_pst_8(dz, dpz, ~emask);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorAve_S16(
    mlib_s16 *xz,
    const mlib_s16 *y,
    mlib_s32 n)
{
	mlib_d64 roundmask = vis_to_double_dup(0x00010001);
	mlib_f32 div2mask = vis_to_float(0x80808080);
	mlib_d64 *dpz, *dpx, *dpy;
	mlib_d64 dx, dy, dz, dx0, dx1, dy0, dy1, inc;
	mlib_s16 *pz, *px, *py, *pzend;
	mlib_s32 off;
	mlib_s32 emask;
	mlib_s32 len = n, i;
	mlib_s32 rest_8, even_8;

	if (n <= 0) {
		return (MLIB_FAILURE);
	}

	px = (mlib_s16 *)xz;
	py = (mlib_s16 *)y;
	pz = (mlib_s16 *)xz;

	dpz = (mlib_d64 *)((mlib_addr)xz & (~7));
	off = (long)dpz - (long)xz;
	pzend = pz + n - 1;

	emask = vis_edge16(pz, pzend);

	if (off) {
		dpy = (mlib_d64 *)vis_alignaddr(py, off);
		dy0 = vis_ld_d64_nf(dpy);
		dy1 = vis_ld_d64_nf(dpy + 1);
		dy = vis_faligndata(dy0, dy1);
		dpx = (mlib_d64 *)vis_alignaddr(px, off);
		dx0 = vis_ld_d64_nf(dpx);
		dx1 = vis_ld_d64_nf(dpx + 1);
		dx = vis_faligndata(dx0, dx1);
		MLIB_V_AVE_S16_S16 px += (8 + off) >> 1;
		py += (8 + off) >> 1;
		len -= (8 + off) >> 1;
		dpz++;
	}

	if (len <= 0) {
		return (MLIB_SUCCESS);
	}

	even_8 = len >> 2;
	rest_8 = len & 0x3;
	emask = 0xf;

	if (!((mlib_addr)py & 7)) {
		dpx = (mlib_d64 *)px;
		dpy = (mlib_d64 *)py;

		dx = vis_ld_d64_nf(dpx);
		dpx++;
		dy = vis_ld_d64_nf(dpy);
		dpy++;

		for (i = 0; i < even_8; i++) {
			dx1 = vis_ld_d64_nf(dpx);
			dy1 = vis_ld_d64_nf(dpy);
			MLIB_V_AVE_S16_S16 dx = dx1;
			dy = dy1;
			dpx++;
			dpy++;
			dpz++;
		}

		dx1 = dx;
		dy1 = dy;
	} else {
		dpx = (mlib_d64 *)px;
		dpy = vis_alignaddr(py, 0);
		dy0 = vis_ld_d64_nf(dpy);
		dpy++;
		dy1 = vis_ld_d64_nf(dpy);
		dy = vis_faligndata(dy0, dy1);
		dx = vis_ld_d64_nf(dpx);
		dpx++;

		for (i = 0; i < even_8; i++) {
			MLIB_V_AVE_S16_S16 dx = vis_ld_d64_nf(dpx);
			dy0 = dy1;
			dy1 = vis_ld_d64_nf(dpy + 1);
			dy = vis_faligndata(dy0, dy1);
			dpz++;
			dpx++;
			dpy++;
		}

		dx1 = dx;
		dy1 = dy0;
	}

	if (!rest_8) {
		return (MLIB_SUCCESS);
	}

	emask = ~(vis_edge16((void *)(rest_8 << 1), pzend));

	vis_alignaddr(px, 0);
	dx0 = dx1;
	dx1 = vis_ld_d64_nf(dpx);
	dx = vis_faligndata(dx0, dx1);
	vis_alignaddr(py, 0);
	dy0 = dy1;
	dy1 = vis_ld_d64_nf(dpy);
	dy = vis_faligndata(dy0, dy1);

	MLIB_V_AVE_S16_S16 return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorAve_S32(
	mlib_s32 *xz,
	const mlib_s32 *y,
	mlib_s32 n)
{
	mlib_s32 i;

	if (n < 1) {
		return (MLIB_FAILURE);
	}

#ifdef _NO_LONGLONG

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	AVE_WORK_LOOP_S32_IN_D64(xz, xz, y, n)

#else /* _NO_LONGLONG */

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
	AVE_WORK_LOOP_S32_IN_S64(xz, xz, y, n)

#endif  /* _NO_LONGLONG */

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorAve_U8C_U8C(
    mlib_u8 *z,
    const mlib_u8 *x,
    const mlib_u8 *y,
    mlib_s32 n)
{
	return (__mlib_VectorAve_U8_U8(z, x, y, (n << 1)));
}

/* *********************************************************** */

mlib_status
__mlib_VectorAve_S8C_S8C(
    mlib_s8 *z,
    const mlib_s8 *x,
    const mlib_s8 *y,
    mlib_s32 n)
{
	return (__mlib_VectorAve_S8_S8(z, x, y, (n << 1)));
}

/* *********************************************************** */

mlib_status
__mlib_VectorAve_S16C_U8C(
    mlib_s16 *z,
    const mlib_u8 *x,
    const mlib_u8 *y,
    mlib_s32 n)
{
	return (__mlib_VectorAve_S16_U8(z, x, y, (n << 1)));
}

/* *********************************************************** */

mlib_status
__mlib_VectorAve_S16C_S8C(
    mlib_s16 *z,
    const mlib_s8 *x,
    const mlib_s8 *y,
    mlib_s32 n)
{
	return (__mlib_VectorAve_S16_S8(z, x, y, (n << 1)));
}

/* *********************************************************** */

mlib_status
__mlib_VectorAve_S16C_S16C(
    mlib_s16 *z,
    const mlib_s16 *x,
    const mlib_s16 *y,
    mlib_s32 n)
{
	return (__mlib_VectorAve_S16_S16(z, x, y, (n << 1)));
}

/* *********************************************************** */

mlib_status
__mlib_VectorAve_S32C_S16C(
    mlib_s32 *z,
    const mlib_s16 *x,
    const mlib_s16 *y,
    mlib_s32 n)
{
	return (__mlib_VectorAve_S32_S16(z, x, y, (n << 1)));
}

/* *********************************************************** */

mlib_status
__mlib_VectorAve_S32C_S32C(
    mlib_s32 *z,
    const mlib_s32 *x,
    const mlib_s32 *y,
    mlib_s32 n)
{
	return (__mlib_VectorAve_S32_S32(z, x, y, (n << 1)));
}

/* *********************************************************** */

mlib_status
__mlib_VectorAve_U8C(
    mlib_u8 *xz,
    const mlib_u8 *y,
    mlib_s32 n)
{
	return (__mlib_VectorAve_U8(xz, y, (n << 1)));
}

/* *********************************************************** */

mlib_status
__mlib_VectorAve_S8C(
    mlib_s8 *xz,
    const mlib_s8 *y,
    mlib_s32 n)
{
	return (__mlib_VectorAve_S8(xz, y, (n << 1)));
}

/* *********************************************************** */

mlib_status
__mlib_VectorAve_S16C(
    mlib_s16 *xz,
    const mlib_s16 *y,
    mlib_s32 n)
{
	return (__mlib_VectorAve_S16(xz, y, (n << 1)));
}

/* *********************************************************** */

mlib_status
__mlib_VectorAve_S32C(
    mlib_s32 *xz,
    const mlib_s32 *y,
    mlib_s32 n)
{
	return (__mlib_VectorAve_S32(xz, y, (n << 1)));
}

/* *********************************************************** */
