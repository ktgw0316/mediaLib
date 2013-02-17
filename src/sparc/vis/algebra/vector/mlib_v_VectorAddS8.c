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

#pragma ident	"@(#)mlib_v_VectorAddS8.c	9.4	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorAddS_U8  - addition of unsigned 8-bit
 *                            format vector to a scalar
 *      mlib_VectorAddS_S8  - addition of  signed 8-bit
 *                            format vector to a scalar
 *      mlib_VectorAddS_S16 - addition of  signed 8-bit
 *                            format vector to a scalar
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorAddS_S8_[Sat|Mod](mlib_s8       *xz,
 *                                               const mlib_s8 *c,
 *                                               mlib_s32      n);
 *      mlib_status mlib_VectorAddS_S8_S8_[Sat|Mod](mlib_s8       *z,
 *                                                  const mlib_s8 *x,
 *                                                  const mlib_s8 *c,
 *                                                  mlib_s32      n);
 *      mlib_status mlib_VectorAddS_U8_[Sat|Mod](mlib_u8       *xz,
 *                                               const mlib_u8 *c,
 *                                               mlib_s32      n);
 *      mlib_status mlib_VectorAddS_U8_U8_[Sat|Mod](mlib_u8       *z,
 *                                                  const mlib_u8 *x,
 *                                                  const mlib_u8 *c,
 *                                                  mlib_s32      n);
 *      mlib_status mlib_VectorAddS_S16_S8_Mod(mlib_s16      *z,
 *                                             const mlib_s8 *x,
 *                                             const mlib_s8 *c,
 *                                             mlib_s32      n);
 *      mlib_status mlib_VectorAddS_S16_U8_Mod(mlib_s16      *z,
 *                                             const mlib_u8 *x,
 *                                             const mlib_u8 *c,
 *                                             mlib_s32      n);
 *
 * ARGUMENTS
 *      z    pointer to the first element of the result vector
 *      x    pointer to the first element of the first vector
 *      c    pointer to a scalar
 *      n    number of elements in the vectors
 *
 * DESCRIPTION
 *      z = x + c
 */

/*
 * FUNCTIONS
 *      mlib_VectorAddS_U8C  - addition of unsigned 8-bit format complex vector
 *                            to a scalar
 *      mlib_VectorAddS_S8C  - addition of  signed 8-bit format complex vector
 *                            to a scalar
 *      mlib_VectorAddS_S16C - addition of  signed 8-bit format complex vector
 *                            to a scalar
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorAddS_S8C_[Sat|Mod](mlib_s8       *xz,
 *                                                const mlib_s8 *c,
 *                                                mlib_s32      n);
 *      mlib_status mlib_VectorAddS_S8C_S8C_[Sat|Mod](mlib_s8       *z,
 *                                                    const mlib_s8 *x,
 *                                                    const mlib_s8 *c,
 *                                                    mlib_s32      n);
 *      mlib_status mlib_VectorAddS_U8C_[Sat|Mod](mlib_u8       *xz,
 *                                                const mlib_u8 *c,
 *                                                mlib_s32      n);
 *      mlib_status mlib_VectorAddS_U8C_U8C_[Sat|Mod](mlib_u8       *z,
 *                                                    const mlib_u8 *x,
 *                                                    const mlib_u8 *c,
 *                                                    mlib_s32      n);
 *
 *      mlib_status mlib_VectorAddS_S16C_S8C_Mod(mlib_s16      *z,
 *                                               const mlib_s8 *x,
 *                                               const mlib_s8 *c,
 *                                               mlib_s32      n);
 *      mlib_status mlib_VectorAddS_S16C_U8C_Mod(mlib_s16      *z,
 *                                               const mlib_u8 *x,
 *                                               const mlib_u8 *c,
 *                                               mlib_s32      n);
 *
 * ARGUMENTS
 *      z    pointer to the first complex element of the result vector.
 *           z[2*i] hold the real parts, and z[2*i + 1] hold the imaginary
 *           parts
 *      x    pointer to the first complex element of the first vector.
 *           x[2*i] hold the real parts, and x[2*i + 1] hold the imaginary
 *           parts
 *      c    pointer to a complex scalar. c[0] hold the real part,
 *           and c[1] hold the imaginary part
 *      n    number of complex elements in the vectors
 *
 * DESCRIPTION
 *      z = x + c
 */

#include <mlib_algebra.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorAddS_U8C_Mod = __mlib_VectorAddS_U8C_Mod
#pragma weak mlib_VectorAddS_S8_Sat = __mlib_VectorAddS_S8_Sat
#pragma weak mlib_VectorAddS_S8_S8_Mod = __mlib_VectorAddS_S8_S8_Mod
#pragma weak mlib_VectorAddS_U8_Sat = __mlib_VectorAddS_U8_Sat
#pragma weak mlib_VectorAddS_S8C_Sat = __mlib_VectorAddS_S8C_Sat
#pragma weak mlib_VectorAddS_U8C_Sat = __mlib_VectorAddS_U8C_Sat
#pragma weak mlib_VectorAddS_U8_U8_Mod = __mlib_VectorAddS_U8_U8_Mod
#pragma weak mlib_VectorAddS_S8C_S8C_Mod = __mlib_VectorAddS_S8C_S8C_Mod
#pragma weak mlib_VectorAddS_S8_S8_Sat = __mlib_VectorAddS_S8_S8_Sat
#pragma weak mlib_VectorAddS_U8C_U8C_Mod = __mlib_VectorAddS_U8C_U8C_Mod
#pragma weak mlib_VectorAddS_U8_U8_Sat = __mlib_VectorAddS_U8_U8_Sat
#pragma weak mlib_VectorAddS_S8C_S8C_Sat = __mlib_VectorAddS_S8C_S8C_Sat
#pragma weak mlib_VectorAddS_S16_S8_Mod = __mlib_VectorAddS_S16_S8_Mod
#pragma weak mlib_VectorAddS_S16_U8_Mod = __mlib_VectorAddS_S16_U8_Mod
#pragma weak mlib_VectorAddS_U8C_U8C_Sat = __mlib_VectorAddS_U8C_U8C_Sat
#pragma weak mlib_VectorAddS_S16C_S8C_Mod = __mlib_VectorAddS_S16C_S8C_Mod
#pragma weak mlib_VectorAddS_S8_Mod = __mlib_VectorAddS_S8_Mod
#pragma weak mlib_VectorAddS_S16C_U8C_Mod = __mlib_VectorAddS_S16C_U8C_Mod
#pragma weak mlib_VectorAddS_U8_Mod = __mlib_VectorAddS_U8_Mod
#pragma weak mlib_VectorAddS_S8C_Mod = __mlib_VectorAddS_S8C_Mod

#elif defined(__GNUC__)

__typeof__(__mlib_VectorAddS_U8C_Mod) mlib_VectorAddS_U8C_Mod
	__attribute__((weak, alias("__mlib_VectorAddS_U8C_Mod")));
__typeof__(__mlib_VectorAddS_S8_Sat) mlib_VectorAddS_S8_Sat
	__attribute__((weak, alias("__mlib_VectorAddS_S8_Sat")));
__typeof__(__mlib_VectorAddS_S8_S8_Mod) mlib_VectorAddS_S8_S8_Mod
	__attribute__((weak, alias("__mlib_VectorAddS_S8_S8_Mod")));
__typeof__(__mlib_VectorAddS_U8_Sat) mlib_VectorAddS_U8_Sat
	__attribute__((weak, alias("__mlib_VectorAddS_U8_Sat")));
__typeof__(__mlib_VectorAddS_S8C_Sat) mlib_VectorAddS_S8C_Sat
	__attribute__((weak, alias("__mlib_VectorAddS_S8C_Sat")));
__typeof__(__mlib_VectorAddS_U8C_Sat) mlib_VectorAddS_U8C_Sat
	__attribute__((weak, alias("__mlib_VectorAddS_U8C_Sat")));
__typeof__(__mlib_VectorAddS_U8_U8_Mod) mlib_VectorAddS_U8_U8_Mod
	__attribute__((weak, alias("__mlib_VectorAddS_U8_U8_Mod")));
__typeof__(__mlib_VectorAddS_S8C_S8C_Mod) mlib_VectorAddS_S8C_S8C_Mod
	__attribute__((weak, alias("__mlib_VectorAddS_S8C_S8C_Mod")));
__typeof__(__mlib_VectorAddS_S8_S8_Sat) mlib_VectorAddS_S8_S8_Sat
	__attribute__((weak, alias("__mlib_VectorAddS_S8_S8_Sat")));
__typeof__(__mlib_VectorAddS_U8C_U8C_Mod) mlib_VectorAddS_U8C_U8C_Mod
	__attribute__((weak, alias("__mlib_VectorAddS_U8C_U8C_Mod")));
__typeof__(__mlib_VectorAddS_U8_U8_Sat) mlib_VectorAddS_U8_U8_Sat
	__attribute__((weak, alias("__mlib_VectorAddS_U8_U8_Sat")));
__typeof__(__mlib_VectorAddS_S8C_S8C_Sat) mlib_VectorAddS_S8C_S8C_Sat
	__attribute__((weak, alias("__mlib_VectorAddS_S8C_S8C_Sat")));
__typeof__(__mlib_VectorAddS_S16_S8_Mod) mlib_VectorAddS_S16_S8_Mod
	__attribute__((weak, alias("__mlib_VectorAddS_S16_S8_Mod")));
__typeof__(__mlib_VectorAddS_S16_U8_Mod) mlib_VectorAddS_S16_U8_Mod
	__attribute__((weak, alias("__mlib_VectorAddS_S16_U8_Mod")));
__typeof__(__mlib_VectorAddS_U8C_U8C_Sat) mlib_VectorAddS_U8C_U8C_Sat
	__attribute__((weak, alias("__mlib_VectorAddS_U8C_U8C_Sat")));
__typeof__(__mlib_VectorAddS_S16C_S8C_Mod) mlib_VectorAddS_S16C_S8C_Mod
	__attribute__((weak, alias("__mlib_VectorAddS_S16C_S8C_Mod")));
__typeof__(__mlib_VectorAddS_S8_Mod) mlib_VectorAddS_S8_Mod
	__attribute__((weak, alias("__mlib_VectorAddS_S8_Mod")));
__typeof__(__mlib_VectorAddS_S16C_U8C_Mod) mlib_VectorAddS_S16C_U8C_Mod
	__attribute__((weak, alias("__mlib_VectorAddS_S16C_U8C_Mod")));
__typeof__(__mlib_VectorAddS_U8_Mod) mlib_VectorAddS_U8_Mod
	__attribute__((weak, alias("__mlib_VectorAddS_U8_Mod")));
__typeof__(__mlib_VectorAddS_S8C_Mod) mlib_VectorAddS_S8C_Mod
	__attribute__((weak, alias("__mlib_VectorAddS_S8C_Mod")));

#else

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	ADDS_U8_SAT                                             \
	dr0 = vis_fexpand_hi(dx);                               \
	dr1 = vis_fexpand_lo(dx);                               \
	dr0 = vis_fpadd16(dr0, dc);                             \
	dr1 = vis_fpadd16(dr1, dc);                             \
	dr = vis_fpack16_pair(dr0, dr1);

/*
 *
 *   1. Expand 'dx' from 8 -> 16 bits representation via "vis_fmul8x16al"
 *      (multiplication by 256).
 *   2. Arithmetic plus in 16-bit format.
 *   3. 2*64-bit Stores.
 *
 */

#define	ADDS_U8_S16                                             \
	dxh = vis_fmul8x16al(vis_read_hi(dx), fone);            \
	dxl = vis_fmul8x16al(vis_read_lo(dx), fone);            \
	dzh = vis_fpadd16(dxh, dc);                             \
	dzl = vis_fpadd16(dxl, dc);

/* *********************************************************** */

#define	ADDS_U8C_S16C                                           \
	dxh = vis_fmul8x16al(vis_read_hi(dx), fone);            \
	dxl = vis_fmul8x16al(vis_read_lo(dx), fone);            \
	dzh = vis_fpadd16(dxh, dc);                             \
	dzl = vis_fpadd16(dxl, dc);

/* *********************************************************** */

#define	ADDS_S16C_U8C_WITH_ODD                                  \
	dxh = vis_fmul8x16al(vis_read_hi(dx), fone);            \
	dxl = vis_fmul8x16al(vis_read_lo(dx), fone);            \
	dzh = vis_fpadd16(dxh, dc_odd);                         \
	dzl = vis_fpadd16(dxl, dc_odd);

/*
 *
 *   1. Logical XOR dx ^ 0x8080808080808080. This is equivalent to
 *      addition to mlib_s32 byte 128. Hence 'dx' becomes positive.
 *   3. Expand from 8 -> 16 bits representation via "vis_fmul8x16al"
 *      (multiplication by 256).
 *   4. 16-bit arithmetic plus with 4_16_d64 constant ('dc').
 *   5. Arithmetic subtraction_16 (dz.16 - 128) to return each 16-bit value
 *      to initial range.
 *   6. 2*64-bit Stores.
 *
 */

#define	ADDS_S8_MOD                                             \
	dr0 = vis_fand(dx, uncontrol_mask);                     \
	dr0 = vis_fpadd16(dr0, dch);                            \
	dr1 = vis_fpadd16(dx, dcl);                             \
	dr1 = vis_fandnot(uncontrol_mask, dr1);                 \
	dr = vis_for(dr0, dr1);

/* *********************************************************** */

#define	ADDS_S8_S16                                             \
	dx = vis_fxor(dx, restore);                             \
	dxh = vis_fmul8x16al(vis_read_hi(dx), fone);            \
	dxl = vis_fmul8x16al(vis_read_lo(dx), fone);            \
	dzh = vis_fpadd16(dxh, dc);                             \
	dzl = vis_fpadd16(dxl, dc);                             \
	dzh = vis_fpsub16(dzh, displacement);                   \
	dzl = vis_fpsub16(dzl, displacement);

#define	ADDS_S8C_S16C                                           \
	dx = vis_fxor(dx, restore);                             \
	dxh = vis_fmul8x16al(vis_read_hi(dx), fone);            \
	dxl = vis_fmul8x16al(vis_read_lo(dx), fone);            \
	dzh = vis_fpadd16(dxh, dc);                             \
	dzl = vis_fpadd16(dxl, dc);                             \
	dzh = vis_fpsub16(dzh, displacement);                   \
	dzl = vis_fpsub16(dzl, displacement);

#define	ADDS_S16C_S8C_WITH_ODD                                            \
	dx = vis_fxor(dx, restore);                                       \
	dxh = vis_fmul8x16al(vis_read_hi(dx), fone);                      \
	dxl = vis_fmul8x16al(vis_read_lo(dx), fone);                      \
	dzh = vis_fpadd16(dxh, dc_odd), dzl = vis_fpadd16(dxl, dc_odd);   \
	dzh = vis_fpsub16(dzh, displacement);                             \
	dzl = vis_fpsub16(dzl, displacement);

/* *********************************************************** */

#define	ADDS_US8_S16_IN_C                                       \
	{                                                       \
	    while (n--) {                                       \
		(*pz++) = ((mlib_s16)(*px)) + uc;               \
		px++;                                           \
	    }                                                   \
	    return (MLIB_SUCCESS);                              \
	}

/* *********************************************************** */

#define	ADDS_US8C_S16C_IN_C                                     \
	{                                                       \
	    while (n--) {                                       \
		(*pz++) = ((mlib_s16)(*px)) + uc;               \
		px++;                                           \
		(*pz++) = ((mlib_s16)(*px)) + uc1;              \
		px++;                                           \
	    }                                                   \
	    return (MLIB_SUCCESS);                              \
	}

/* *********************************************************** */

mlib_status
__mlib_VectorAddS_U8_U8_Sat(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *c,
	mlib_s32 n)
{
/* edge masks */
	mlib_s32 emask;

/* offset of address alignment in destination */
	mlib_s32 off;

/* end point of a line in destination */
	mlib_u8 *pzend;

/* 8-byte aligned start point in source */
	mlib_d64 *dpx;

/* 8-byte aligned start point in destination */
	mlib_d64 *dpz;

/* 8-byte aligned end point in destination */
	mlib_d64 *dpzend;

/* source data */
	mlib_d64 dx, dx0, dx1;

/* destination data */
	mlib_d64 dr0, dr1, dr;
	mlib_u16 cc = *((mlib_u8 *)c);

/* prepare the scaling factors */
	mlib_d64 dc = vis_to_double_dup((cc << 4) | (cc << 20));

	if (n <= 0)
		return (MLIB_FAILURE);

/* initialize GSR scale factor */
	vis_write_gsr(3 << 3);

/*
 * prepare the destination address
 */
	pzend = (mlib_u8 *)z + n - 1;
	dpzend = (mlib_d64 *)((mlib_addr)pzend & (~7));
	dpz = (mlib_d64 *)((mlib_addr)z & (~7));
	off = (mlib_addr)dpz - (mlib_addr)z;

/*
 * prepare the source address
 */
	dpx = (mlib_d64 *)vis_alignaddr((void *)x, off);
	dx0 = vis_ld_d64_nf(dpx);
	dpx++;
	dx1 = vis_ld_d64_nf(dpx);
	dpx++;

/*
 * generate edge mask for the start bytes
 */
	emask = vis_edge8(z, pzend);
	dx = vis_faligndata(dx0, dx1);
	ADDS_U8_SAT;
/* store first bytes of result */
	vis_pst_8(dr, dpz, emask);
	dpz++;
	dx0 = dx1;

#pragma pipeloop(0)
	for (; (mlib_addr)dpz < (mlib_addr)dpzend; ) {
		dx1 = vis_ld_d64_nf(dpx);
		dpx++;
		dx = vis_faligndata(dx0, dx1);
		ADDS_U8_SAT;
		(*dpz++) = dr;
		dx0 = dx1;
	}

	if ((mlib_addr)dpz <= (mlib_addr)pzend) {
		dx1 = vis_ld_d64_nf(dpx);
		dpx++;
		dx = vis_faligndata(dx0, dx1);
		ADDS_U8_SAT;
/* prepare edge mask for the last bytes */
		emask = vis_edge8(dpz, pzend);
/* store last bytes of result */
		vis_pst_8(dr, dpz, emask);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorAddS_S16_U8_Mod(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *c,
	mlib_s32 n)
{
	mlib_d64 *dpz, *dpx;
	mlib_d64 dx, dx0, dx1;
	mlib_d64 dxh, dxl, dzh, dzl;
	mlib_f32 fone = vis_to_float(0x100);
	mlib_u8 uc = *((mlib_u8 *)c);
	mlib_u8 *px;
	mlib_s16 *pz;
	mlib_s32 len = n, i;

/* rest and leng in terms of 8 bytes. */
	mlib_s32 rest_8, even_8;
	mlib_d64 dc = vis_to_double_dup(uc | (uc << 16));

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!uc)
		return (__mlib_VectorConvert_S16_U8_Mod(z, x, n));

	px = (mlib_u8 *)x;
	pz = (mlib_s16 *)z;

	if (n <= 8)
		ADDS_US8_S16_IN_C;

/*
 * prepare the destination address
 */

	while ((mlib_addr)pz & 7) {
		(*pz++) = ((mlib_s16)(*px)) + uc;
		px++;
		len--;
	}

	dpz = (mlib_d64 *)pz;

	even_8 = len >> 3;
	rest_8 = len & 0x7;

	if (!((mlib_addr)px & 7)) {

/*
 * 'x' address is 8-byte aligned.
 * No  vis_alignaddr and  vis_faligndata at all.
 */

		dpx = (mlib_d64 *)px;

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dx = (*dpx++);
			ADDS_U8_S16;

/*
 * store 16 bytes of result
 */
			(*dpz++) = dzh;
			(*dpz++) = dzl;
		}

	} else {

/*
 * "x"  address is arbitrary aligned.
 * 1 vis_alignaddr and 1 vis_faligndata in the loop.
 */

		dpx = vis_alignaddr(px, 0);
		dx0 = vis_ld_d64_nf(dpx);
		dpx++;

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dx1 = vis_ld_d64_nf(dpx);
			dpx++;
			dx = vis_faligndata(dx0, dx1);
			ADDS_U8_S16;
			dx0 = dx1;
/*
 * store 16 bytes of result
 */
			dpz[0] = dzh;
			dpz[1] = dzl;
			dpz += 2;
		}
	}

	if (!rest_8)
		return (MLIB_SUCCESS);

	px += (even_8 << 3);
	pz += (even_8 << 3);

	while (rest_8--) {
		(*pz++) = ((mlib_s16)(*px)) + uc;
		px++;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorAddS_U8_Sat(
	mlib_u8 *xz,
	const mlib_u8 *c,
	mlib_s32 n)
{
	return (__mlib_VectorAddS_U8_U8_Sat(xz, xz, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorAddS_U8_Mod(
	mlib_u8 *xz,
	const mlib_u8 *c,
	mlib_s32 n)
{
	return (__mlib_VectorAddS_U8_U8_Mod(xz, xz, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorAddS_U8_U8_Mod(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *c,
	mlib_s32 n)
{
/* edge masks */
	mlib_s32 emask;

/* offset of address alignment in destination */
	mlib_s32 off;

/* end point of a line in destination */
	mlib_u8 *pzend;

/* 8-byte aligned start point in source */
	mlib_d64 *dpx;

/* 8-byte aligned start point in destination */
	mlib_d64 *dpz;

/* 8-byte aligned end point in destination */
	mlib_d64 *dpzend;

/* source data */
	mlib_d64 dx, dx0, dx1;

/* destination data */
	mlib_d64 dr0, dr1, dr;
	mlib_u8 uc = *((mlib_u8 *)c);
	mlib_d64 uncontrol_mask = vis_to_double_dup(0xff00ff00);

/* prepare the scaling factors */
	mlib_d64 dcl = vis_to_double_dup(uc | (uc << 16));
	mlib_d64 dch = vis_to_double_dup((uc << 8) | (uc << 24));
	mlib_s32 scal = uc << 24 | uc << 16 | uc << 8 | uc;
	mlib_s32 sr1, sr2, sr3, sr3_;
	mlib_s32 mask = 0x7f7f7f7f;
	mlib_s32 x8, x12;
	mlib_s32 nrest, i;

	if (n <= 0)
		return (MLIB_FAILURE);

/*
 * prepare the destination address
 */
	pzend = (mlib_u8 *)z + n - 1;
	dpzend = (mlib_d64 *)((mlib_addr)pzend & (~7));

/*
 * check for 64-bit aligned special case
 */

	if ((((mlib_addr)x | (mlib_addr)z) & 7) == 0) {

/*
 * We can process source and destination vectors by 16 bytes.
 */

		dpx = (mlib_d64 *)x;
		dpz = (mlib_d64 *)z;
#pragma pipeloop(0)
		for (i = 0; i < n >> 4; i++) {
			mlib_u64 ld0;

			dx = dpx[0];
			ADDS_S8_MOD;
			(*dpz++) = dr;

			ld0 = *((mlib_u64 *)dpx + 1);
			x8 = ld0 >> 32;
			sr1 = x8 ^ scal;
			sr2 = (x8 & mask) + (scal & mask);
			sr3 = (sr1 & ~mask) ^ sr2;
			x12 = ld0 & 0xFFFFFFFF;
			sr1 = x12 ^ scal;
			sr2 = (x12 & mask) + (scal & mask);
			sr3_ = (sr1 & ~mask) ^ sr2;
			(*dpz++) = vis_to_double(sr3, sr3_);
			dpx += 2;
		}

		nrest = n & 0xf;

		if (nrest >= 8) {
			dx = (*dpx++);
			ADDS_S8_MOD;
			(*dpz++) = dr;
			nrest -= 8;
		}

		if (nrest > 0) {
			dx = (*dpx++);
			ADDS_S8_MOD;
			emask = vis_edge8(dpz, pzend);
			vis_pst_8(dr, dpz, emask);
		}

	} else {

		dpz = (mlib_d64 *)((mlib_addr)z & (~7));
		off = (mlib_addr)dpz - (mlib_addr)z;

/*
 * prepare the source address
 */
		dpx = (mlib_d64 *)vis_alignaddr((void *)x, off);
		dx0 = vis_ld_d64_nf(dpx);
		dpx++;
		dx1 = vis_ld_d64_nf(dpx);
		dpx++;

/*
 * generate edge mask for the start bytes
 */
		emask = vis_edge8(z, pzend);
		dx = vis_faligndata(dx0, dx1);
		ADDS_S8_MOD;
/* store first bytes of result */
		vis_pst_8(dr, dpz, emask);
		dpz++;
		dx0 = dx1;

#pragma pipeloop(0)
		for (; (mlib_addr)dpz < (mlib_addr)dpzend; ) {
			dx1 = vis_ld_d64_nf(dpx);
			dpx++;
			dx = vis_faligndata(dx0, dx1);
			ADDS_S8_MOD;
			(*dpz++) = dr;
			dx0 = dx1;
		}

		if ((mlib_addr)dpz <= (mlib_addr)pzend) {
			dx1 = vis_ld_d64_nf(dpx);
			dpx++;
			dx = vis_faligndata(dx0, dx1);
			ADDS_S8_MOD;
/* prepare edge mask for the last bytes */
			emask = vis_edge8(dpz, pzend);
/* store last bytes of result */
			vis_pst_8(dr, dpz, emask);
		}
	}

	return (MLIB_SUCCESS);
}

/*
 *    zi = (c + 255) - (xi ^ 0x7f)
 *
 *    restore = 0x80808080808080
 *
 *    After fpack16 operation is done all results are in the range
 *    [0 , 255] and xor operation with constant "restore" reduce
 *    the result to the range [-128 , 127].
 */

#define	ADDS_S8_SAT                                             \
	dx = vis_fxnor(dx, restore);                            \
	dr0 = vis_fexpand_hi(dx);                               \
	dr1 = vis_fexpand_lo(dx);                               \
	dr0 = vis_fpsub16(dc, dr0);                             \
	dr1 = vis_fpsub16(dc, dr1);                             \
	dr = vis_fpack16_pair(dr0, dr1);                        \
	dr = vis_fxor(dr, restore);

/* *********************************************************** */

mlib_status
__mlib_VectorAddS_S8_S8_Sat(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *c,
	mlib_s32 n)
{
/* edge masks */
	mlib_s32 emask;

/* offset of address alignment in destination */
	mlib_s32 off;

/* end point of a line in destination */
	mlib_s8 *pzend;

/* 8-byte aligned start point in source */
	mlib_d64 *dpx;

/* 8-byte aligned start point in destination */
	mlib_d64 *dpz;

/* 8-byte aligned end point in destination */
	mlib_d64 *dpzend;

/* source data */
	mlib_d64 dx, dx0, dx1;

/* destination data */
	mlib_d64 dr0, dr1, dr;
	mlib_u16 cc = *((mlib_s8 *)c) + 255;
	mlib_d64 restore = vis_to_double_dup(0x80808080);

/* prepare the scaling factors */
	mlib_d64 dc = vis_to_double_dup((cc << 4) | (cc << 20));

	if (n <= 0)
		return (MLIB_FAILURE);

/* initialize GSR scale factor */
	vis_write_gsr(3 << 3);

/*
 * prepare the destination address
 */
	pzend = (mlib_s8 *)z + n - 1;
	dpzend = (mlib_d64 *)((mlib_addr)pzend & (~7));
	dpz = (mlib_d64 *)((mlib_addr)z & (~7));
	off = (mlib_addr)dpz - (mlib_addr)z;

/*
 * prepare the source address
 */
	dpx = (mlib_d64 *)vis_alignaddr((void *)x, off);
	dx0 = vis_ld_d64_nf(dpx);
	dpx++;
	dx1 = vis_ld_d64_nf(dpx);
	dpx++;

/*
 * generate edge mask for the start bytes
 */
	emask = vis_edge8(z, pzend);
	dx = vis_faligndata(dx0, dx1);
	ADDS_S8_SAT;
/* store first bytes of result */
	vis_pst_8(dr, dpz, emask);
	dpz++;
	dx0 = dx1;

#pragma pipeloop(0)
	for (; (mlib_addr)dpz < (mlib_addr)dpzend; ) {
		dx1 = vis_ld_d64_nf(dpx);
		dpx++;
		dx = vis_faligndata(dx0, dx1);
		ADDS_S8_SAT;
		(*dpz++) = dr;
		dx0 = dx1;
	}

	if ((mlib_addr)dpz <= (mlib_addr)pzend) {
		dx1 = vis_ld_d64_nf(dpx);
		dpx++;
		dx = vis_faligndata(dx0, dx1);
		ADDS_S8_SAT;
/* prepare edge mask for the last bytes */
		emask = vis_edge8(dpz, pzend);
/* store last bytes of result */
		vis_pst_8(dr, dpz, emask);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorAddS_S8_S8_Mod(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *c,
	mlib_s32 n)
{
/* edge masks */
	mlib_s32 emask;

/* offset of address alignment in destination */
	mlib_s32 off;

/* end point of a line in destination */
	mlib_s8 *pzend;

/* 8-byte aligned start point in source */
	mlib_d64 *dpx;

/* 8-byte aligned start point in destination */
	mlib_d64 *dpz;

/* 8-byte aligned end point in destination */
	mlib_d64 *dpzend;

/* source data */
	mlib_d64 dx, dx0, dx1;

/* destination data */
	mlib_d64 dr0, dr1, dr;
	mlib_u8 uc = *((mlib_s8 *)c);
	mlib_d64 uncontrol_mask = vis_to_double_dup(0xff00ff00);

/* prepare the scaling factors */
	mlib_d64 dcl = vis_to_double_dup(uc | (uc << 16)),
		dch = vis_to_double_dup((uc << 8) | (uc << 24));

	if (n <= 0)
		return (MLIB_FAILURE);

/*
 * prepare the destination address
 */
	pzend = (mlib_s8 *)z + n - 1;
	dpzend = (mlib_d64 *)((mlib_addr)pzend & (~7));
	dpz = (mlib_d64 *)((mlib_addr)z & (~7));
	off = (mlib_addr)dpz - (mlib_addr)z;

/*
 * prepare the source address
 */
	dpx = (mlib_d64 *)vis_alignaddr((void *)x, off);
	dx0 = vis_ld_d64_nf(dpx);
	dpx++;
	dx1 = vis_ld_d64_nf(dpx);
	dpx++;

/*
 * generate edge mask for the start bytes
 */
	emask = vis_edge8(z, pzend);
	dx = vis_faligndata(dx0, dx1);
	ADDS_S8_MOD;
/* store first bytes of result */
	vis_pst_8(dr, dpz, emask);
	dpz++;
	dx0 = dx1;

#pragma pipeloop(0)
	for (; (mlib_addr)dpz < (mlib_addr)dpzend; ) {
		dx1 = vis_ld_d64_nf(dpx);
		dpx++;
		dx = vis_faligndata(dx0, dx1);
		ADDS_S8_MOD;
		(*dpz++) = dr;
		dx0 = dx1;
	}

	if ((mlib_addr)dpz <= (mlib_addr)pzend) {
		dx1 = vis_ld_d64_nf(dpx);
		dpx++;
		dx = vis_faligndata(dx0, dx1);
		ADDS_S8_MOD;
/* prepare edge mask for the last bytes */
		emask = vis_edge8(dpz, pzend);
/* store last bytes of result */
		vis_pst_8(dr, dpz, emask);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorAddS_S16_S8_Mod(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *c,
	mlib_s32 n)
{
	mlib_d64 *dpz, *dpx;
	mlib_d64 dx, dx0, dx1;
	mlib_d64 dxh, dxl, dzh, dzl;
	mlib_f32 fone = vis_to_float(0x100);
	mlib_s8 uc = *((mlib_s8 *)c);
	mlib_s8 *px;
	mlib_s16 *pz;
	mlib_s32 len = n, i;

/* rest and leng in terms of 8 bytes. */
	mlib_s32 rest_8, even_8;
	mlib_d64 dc = vis_to_double_dup((uc & 0xffff) | (uc << 16));
	mlib_d64 restore = vis_to_double_dup(0x80808080);
	mlib_d64 displacement = vis_to_double_dup(0x00800080);

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!uc)
		return (__mlib_VectorConvert_S16_S8_Mod(z, x, n));

	px = (mlib_s8 *)x;
	pz = (mlib_s16 *)z;

	if (n <= 8)
		ADDS_US8_S16_IN_C;

/*
 * prepare the destination address
 */

	while ((mlib_addr)pz & 7) {
		(*pz++) = ((mlib_s16)(*px)) + uc;
		px++;
		len--;
	}

	dpz = (mlib_d64 *)pz;

	even_8 = len >> 3;
	rest_8 = len & 0x7;

	if (!((mlib_addr)px & 7)) {

/*
 * 'x' address is 8-byte aligned.
 * No  vis_alignaddr and  vis_faligndata at all.
 */

		dpx = (mlib_d64 *)px;

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dx = (*dpx++);
			ADDS_S8_S16;
			dx = dx1;

/*
 * store 16 bytes of result
 */
			(*dpz++) = dzh;
			(*dpz++) = dzl;
		}

	} else {

/*
 * "x"  address is arbitrary aligned.
 * 1 vis_alignaddr and 1 vis_faligndata in the loop.
 */

		dpx = vis_alignaddr(px, 0);
		dx0 = vis_ld_d64_nf(dpx);
		dpx++;

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dx1 = vis_ld_d64_nf(dpx);
			dpx++;
			dx = vis_faligndata(dx0, dx1);
			ADDS_S8_S16;
			dx0 = dx1;
/*
 * store 16 bytes of result
 */
			dpz[0] = dzh;
			dpz[1] = dzl;
			dpz += 2;
		}
	}

	if (!rest_8)
		return (MLIB_SUCCESS);

	px += (even_8 << 3);
	pz += (even_8 << 3);

	while (rest_8--) {
		(*pz++) = ((mlib_s16)(*px)) + uc;
		px++;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorAddS_S8_Sat(
	mlib_s8 *xz,
	const mlib_s8 *c,
	mlib_s32 n)
{
	return (__mlib_VectorAddS_S8_S8_Sat(xz, xz, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorAddS_S8_Mod(
	mlib_s8 *xz,
	const mlib_s8 *c,
	mlib_s32 n)
{
	return (__mlib_VectorAddS_S8_S8_Mod(xz, xz, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorAddS_U8C_U8C_Sat(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *c,
	mlib_s32 n)
{
/* edge masks */
	mlib_s32 emask;

/* offset of address alignment in destination */
	mlib_s32 off;

/* end point of a line in destination */
	mlib_u8 *pzend;

/* 8-byte aligned start point in source */
	mlib_d64 *dpx;

/* 8-byte aligned start point in destination */
	mlib_d64 *dpz;

/* 8-byte aligned end point in destination */
	mlib_d64 *dpzend;

/* source data */
	mlib_d64 dx, dx0, dx1;

/* destination data */
	mlib_d64 dr0, dr1, dr;
	mlib_u16 cc0 = *((mlib_u8 *)c);
	mlib_u16 cc1 = *((mlib_u8 *)c + 1);
	mlib_d64 dc =
		((mlib_addr)z & 1) ? vis_to_double_dup((cc0 << 4) | (cc1 << 20))
		: vis_to_double_dup((cc1 << 4) | (cc0 << 20));

	if (n <= 0)
		return (MLIB_FAILURE);

/* initialize GSR scale factor */
	vis_write_gsr(3 << 3);

/*
 * prepare the destination address
 */
	pzend = (mlib_u8 *)z + 2 * n - 1;
	dpzend = (mlib_d64 *)((mlib_addr)pzend & (~7));
	dpz = (mlib_d64 *)((mlib_addr)z & (~7));
	off = (mlib_addr)dpz - (mlib_addr)z;

/*
 * prepare the source address
 */
	dpx = (mlib_d64 *)vis_alignaddr((void *)x, off);
	dx0 = vis_ld_d64_nf(dpx);
	dpx++;
	dx1 = vis_ld_d64_nf(dpx);
	dpx++;

/*
 * generate edge mask for the start bytes
 */
	emask = vis_edge8(z, pzend);
	dx = vis_faligndata(dx0, dx1);
	ADDS_U8_SAT;
/* store first bytes of result */
	vis_pst_8(dr, dpz, emask);
	dpz++;
	dx0 = dx1;

#pragma pipeloop(0)
	for (; (mlib_addr)dpz < (mlib_addr)dpzend; ) {
		dx1 = vis_ld_d64_nf(dpx);
		dpx++;
		dx = vis_faligndata(dx0, dx1);
		ADDS_U8_SAT;
		(*dpz++) = dr;
		dx0 = dx1;
	}

	if ((mlib_addr)dpz <= (mlib_addr)pzend) {
		dx1 = vis_ld_d64_nf(dpx);
		dpx++;
		dx = vis_faligndata(dx0, dx1);
		ADDS_U8_SAT;
/* prepare edge mask for the last bytes */
		emask = vis_edge8(dpz, pzend);
/* store last bytes of result */
		vis_pst_8(dr, dpz, emask);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorAddS_U8C_Sat(
	mlib_u8 *xz,
	const mlib_u8 *c,
	mlib_s32 n)
{
	return (__mlib_VectorAddS_U8C_U8C_Sat(xz, xz, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorAddS_U8C_Mod(
	mlib_u8 *xz,
	const mlib_u8 *c,
	mlib_s32 n)
{
	return (__mlib_VectorAddS_U8C_U8C_Mod(xz, xz, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorAddS_U8C_U8C_Mod(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *c,
	mlib_s32 n)
{
/* edge masks */
	mlib_s32 emask;

/* offset of address alignment in destination */
	mlib_s32 off;

/* end point of a line in destination */
	mlib_u8 *pzend;

/* 8-byte aligned start point in source */
	mlib_d64 *dpx;

/* 8-byte aligned start point in destination */
	mlib_d64 *dpz;

/* 8-byte aligned end point in destination */
	mlib_d64 *dpzend;

/* source data */
	mlib_d64 dx, dx0, dx1;

/* destination data */
	mlib_d64 dr0, dr1, dr;

	mlib_u8 uc0 = *((mlib_u8 *)c);
	mlib_u8 uc1 = *((mlib_u8 *)c + 1);
	mlib_d64 uncontrol_mask = vis_to_double_dup(0xff00ff00);
	mlib_d64 dcl = ((mlib_addr)z & 1) ? vis_to_double_dup(uc0 | (uc0 << 16))
		: vis_to_double_dup(uc1 | (uc1 << 16));
	mlib_d64 dch =
		((mlib_addr)z & 1) ? vis_to_double_dup((uc1 << 8) | (uc1 << 24))
		: vis_to_double_dup((uc0 << 8) | (uc0 << 24));
	mlib_s32 scal = uc0 << 24 | uc1 << 16 | uc0 << 8 | uc1;
	mlib_s32 sr1, sr2, sr3, sr3_;
	mlib_s32 mask = 0x7f7f7f7f;
	mlib_s32 x8, x12;
	mlib_s32 nn = n * 2;
	mlib_s32 nrest, i;

	if (n <= 0)
		return (MLIB_FAILURE);

/*
 * prepare the destination address
 */
	pzend = (mlib_u8 *)z + nn - 1;
	dpzend = (mlib_d64 *)((mlib_addr)pzend & (~7));

/*
 * check for 64-bit aligned special case
 */

	if ((((mlib_addr)x | (mlib_addr)z) & 7) == 0) {

/*
 * We can process source and destination vectors by 16 bytes.
 */

		dpx = (mlib_d64 *)x;
		dpz = (mlib_d64 *)z;
#pragma pipeloop(0)
		for (i = 0; i < nn >> 4; i++) {
			mlib_u64 ld0;

			dx = dpx[0];
			ADDS_S8_MOD;
			(*dpz++) = dr;

			ld0 = *((mlib_u64 *)dpx + 1);
			x8 = ld0 >> 32;
			sr1 = x8 ^ scal;
			sr2 = (x8 & mask) + (scal & mask);
			sr3 = (sr1 & ~mask) ^ sr2;
			x12 = ld0 & 0xFFFFFFFF;
			sr1 = x12 ^ scal;
			sr2 = (x12 & mask) + (scal & mask);
			sr3_ = (sr1 & ~mask) ^ sr2;
			(*dpz++) = vis_to_double(sr3, sr3_);
			dpx += 2;
		}

		nrest = nn & 0xf;

		if (nrest >= 8) {
			dx = (*dpx++);
			ADDS_S8_MOD;
			(*dpz++) = dr;
			nrest -= 8;
		}

		if (nrest > 0) {
			dx = (*dpx++);
			ADDS_S8_MOD;
			emask = vis_edge8(dpz, pzend);
			vis_pst_8(dr, dpz, emask);
		}

	} else {

		dpz = (mlib_d64 *)((mlib_addr)z & (~7));
		off = (mlib_addr)dpz - (mlib_addr)z;

/*
 * prepare the source address
 */
		dpx = (mlib_d64 *)vis_alignaddr((void *)x, off);
		dx0 = vis_ld_d64_nf(dpx);
		dpx++;
		dx1 = vis_ld_d64_nf(dpx);
		dpx++;

/*
 * generate edge mask for the start bytes
 */
		emask = vis_edge8(z, pzend);
		dx = vis_faligndata(dx0, dx1);
		ADDS_S8_MOD;
/* store first bytes of result */
		vis_pst_8(dr, dpz, emask);
		dpz++;
		dx0 = dx1;

#pragma pipeloop(0)
		for (; (mlib_addr)dpz < (mlib_addr)dpzend; ) {
			dx1 = vis_ld_d64_nf(dpx);
			dpx++;
			dx = vis_faligndata(dx0, dx1);
			ADDS_S8_MOD;
			(*dpz++) = dr;
			dx0 = dx1;
		}

		if ((mlib_addr)dpz <= (mlib_addr)pzend) {
			dx1 = vis_ld_d64_nf(dpx);
			dpx++;
			dx = vis_faligndata(dx0, dx1);
			ADDS_S8_MOD;
/* prepare edge mask for the last bytes */
			emask = vis_edge8(dpz, pzend);
/* store last bytes of result */
			vis_pst_8(dr, dpz, emask);
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorAddS_S16C_U8C_Mod(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *c,
	mlib_s32 n)
{
	mlib_d64 *dpz, *dpx;
	mlib_d64 dx, dx0, dx1;
	mlib_d64 dxh, dxl, dzh, dzl;
	mlib_f32 fone = vis_to_float(0x100);
	mlib_u8 uc = *((mlib_u8 *)c);
	mlib_u8 uc1 = *((mlib_u8 *)c + 1);
	mlib_u8 *px;
	mlib_s16 *pz;
	mlib_s32 len = n << 1, i;

/* rest and leng in terms of 8 bytes. */
	mlib_s32 rest_8, even_8;

/*
 * Prepare scale == mlib_d64 (Re_Im_Re_Im_Re_Im_Re_Im)
 */
	mlib_d64 dc = vis_to_double_dup(uc1 | (uc << 16));

/*
 * Prepare scale == mlib_d64 (Im_Re_Im_Re_Im_Re_Im_Re)
 */
	mlib_d64 dc_odd = vis_to_double_dup(uc | (uc1 << 16));
	mlib_s32 odd = 0;

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!(uc | uc1))
		return (__mlib_VectorConvert_S16C_U8C_Mod(z, x, n));

	px = (mlib_u8 *)x;
	pz = (mlib_s16 *)z;

	if (len <= 8)
		ADDS_US8C_S16C_IN_C;

/*
 * prepare the destination address
 */

	while ((mlib_addr)pz & 7) {
		(*pz++) = ((mlib_s16)(*px)) + uc;
		px++;
		len -= 2;

		if (!((mlib_addr)pz & 7)) {
			odd = 1;
			break;
		}

		(*pz++) = ((mlib_s16)(*px)) + uc1;
		px++;
	}

	dpz = (mlib_d64 *)pz;

	even_8 = len >> 3;
	rest_8 = len & 0x7;

	if (!odd) {
/*
 * Start with real part.
 */

		if (!((mlib_addr)px & 7)) {

/*
 * 'x' address is 8-byte aligned.
 * No  vis_alignaddr and  vis_faligndata at all.
 */

			dpx = (mlib_d64 *)px;

#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx = (*dpx++);
				ADDS_U8C_S16C;

/*
 * store 16 bytes of result
 */
				(*dpz++) = dzh;
				(*dpz++) = dzl;
			}

		} else {

/*
 * "x"  address is arbitrary aligned.
 * 1 vis_alignaddr and 1 vis_faligndata in the loop.
 */

			dpx = vis_alignaddr(px, 0);
			dx0 = vis_ld_d64_nf(dpx);
			dpx++;

#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx1 = vis_ld_d64_nf(dpx);
				dpx++;
				dx = vis_faligndata(dx0, dx1);
				ADDS_U8C_S16C;
				dx0 = dx1;
/*
 * store 16 bytes of result
 */
				dpz[0] = dzh;
				dpz[1] = dzl;
				dpz += 2;
			}
		}

		px += (even_8 << 3);
		pz = (mlib_s16 *)dpz;

	} else {

/*
 * odd==1. Start with imaginary part.
 */

		if (!((mlib_addr)px & 7)) {

/*
 * 'x' address is 8-byte aligned.
 * No  vis_alignaddr and  vis_faligndata at all.
 */

			dpx = (mlib_d64 *)px;

#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx = (*dpx++);
				ADDS_S16C_U8C_WITH_ODD;

/*
 * store 16 bytes of result
 */
				(*dpz++) = dzh;
				(*dpz++) = dzl;
			}

		} else {

/*
 * "x"  address is arbitrary aligned.
 * 1 vis_alignaddr and 1 vis_faligndata in the loop.
 */

			dpx = vis_alignaddr(px, 0);
			dx0 = vis_ld_d64_nf(dpx);
			dpx++;

#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx1 = vis_ld_d64_nf(dpx);
				dpx++;
				dx = vis_faligndata(dx0, dx1);
				ADDS_S16C_U8C_WITH_ODD;
				dx0 = dx1;
/*
 * store 16 bytes of result
 */
				dpz[0] = dzh;
				dpz[1] = dzl;
				dpz += 2;
			}
		}

		px += (even_8 << 3);
		pz = (mlib_s16 *)dpz;
		(*pz++) = *px + uc1;
		px++;
	}

	if (!rest_8)
		return (MLIB_SUCCESS);

	while (rest_8) {
		(*pz++) = ((mlib_s16)(*px)) + uc;
		px++;
		(*pz++) = ((mlib_s16)(*px)) + uc1;
		px++;
		rest_8 -= 2;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorAddS_S8C_S8C_Sat(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *c,
	mlib_s32 n)
{
/* edge masks */
	mlib_s32 emask;

/* offset of address alignment in destination */
	mlib_s32 off;

/* end point of a line in destination */
	mlib_s8 *pzend;

/* 8-byte aligned start point in source */
	mlib_d64 *dpx;

/* 8-byte aligned start point in destination */
	mlib_d64 *dpz;

/* 8-byte aligned end point in destination */
	mlib_d64 *dpzend;

/* source data */
	mlib_d64 dx, dx0, dx1;

/* destination data */
	mlib_d64 dr0, dr1, dr;
	mlib_u16 cc0 = *((mlib_s8 *)c) + 255;
	mlib_u16 cc1 = *((mlib_s8 *)c + 1) + 255;
	mlib_d64 restore = vis_to_double_dup(0x80808080);
	mlib_d64 dc =
		((mlib_addr)z & 1) ? vis_to_double_dup((cc0 << 4) | (cc1 << 20))
		: vis_to_double_dup((cc1 << 4) | (cc0 << 20));

	if (n <= 0)
		return (MLIB_FAILURE);

/* initialize GSR scale factor */
	vis_write_gsr(3 << 3);

/*
 * prepare the destination address
 */
	pzend = (mlib_s8 *)z + 2 * n - 1;
	dpzend = (mlib_d64 *)((mlib_addr)pzend & (~7));
	dpz = (mlib_d64 *)((mlib_addr)z & (~7));
	off = (mlib_addr)dpz - (mlib_addr)z;

/*
 * prepare the source address
 */
	dpx = (mlib_d64 *)vis_alignaddr((void *)x, off);
	dx0 = vis_ld_d64_nf(dpx);
	dpx++;
	dx1 = vis_ld_d64_nf(dpx);
	dpx++;

/*
 * generate edge mask for the start bytes
 */
	emask = vis_edge8(z, pzend);
	dx = vis_faligndata(dx0, dx1);
	ADDS_S8_SAT;
/* store first bytes of result */
	vis_pst_8(dr, dpz, emask);
	dpz++;
	dx0 = dx1;

#pragma pipeloop(0)
	for (; (mlib_addr)dpz < (mlib_addr)dpzend; ) {
		dx1 = vis_ld_d64_nf(dpx);
		dpx++;
		dx = vis_faligndata(dx0, dx1);
		ADDS_S8_SAT;
		(*dpz++) = dr;
		dx0 = dx1;
	}

	if ((mlib_addr)dpz <= (mlib_addr)pzend) {
		dx1 = vis_ld_d64_nf(dpx);
		dpx++;
		dx = vis_faligndata(dx0, dx1);
		ADDS_S8_SAT;
/* prepare edge mask for the last bytes */
		emask = vis_edge8(dpz, pzend);
/* store last bytes of result */
		vis_pst_8(dr, dpz, emask);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorAddS_S8C_S8C_Mod(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *c,
	mlib_s32 n)
{
/* edge masks */
	mlib_s32 emask;

/* offset of address alignment in destination */
	mlib_s32 off;

/* end point of a line in destination */
	mlib_s8 *pzend;

/* 8-byte aligned start point in source */
	mlib_d64 *dpx;

/* 8-byte aligned start point in destination */
	mlib_d64 *dpz;

/* 8-byte aligned end point in destination */
	mlib_d64 *dpzend;

/* source data */
	mlib_d64 dx, dx0, dx1;

/* destination data */
	mlib_d64 dr0, dr1, dr;

	mlib_u8 uc0 = *((mlib_s8 *)c);
	mlib_u8 uc1 = *((mlib_s8 *)c + 1);
	mlib_d64 uncontrol_mask = vis_to_double_dup(0xff00ff00);
	mlib_d64 dcl = ((mlib_addr)z & 1) ? vis_to_double_dup(uc0 | (uc0 << 16))
		: vis_to_double_dup(uc1 | (uc1 << 16));
	mlib_d64 dch =
		((mlib_addr)z & 1) ? vis_to_double_dup((uc1 << 8) | (uc1 << 24))
		: vis_to_double_dup((uc0 << 8) | (uc0 << 24));

	if (n <= 0)
		return (MLIB_FAILURE);

/*
 * prepare the destination address
 */
	pzend = (mlib_s8 *)z + 2 * n - 1;
	dpzend = (mlib_d64 *)((mlib_addr)pzend & (~7));
	dpz = (mlib_d64 *)((mlib_addr)z & (~7));
	off = (mlib_addr)dpz - (mlib_addr)z;

/*
 * prepare the source address
 */
	dpx = (mlib_d64 *)vis_alignaddr((void *)x, off);
	dx0 =  vis_ld_d64_nf(dpx);
	dpx++;
	dx1 =  vis_ld_d64_nf(dpx);
	dpx++;

/*
 * generate edge mask for the start bytes
 */
	emask = vis_edge8(z, pzend);
	dx = vis_faligndata(dx0, dx1);
	ADDS_S8_MOD;
/* store first bytes of result */
	vis_pst_8(dr, dpz, emask);
	dpz++;
	dx0 = dx1;

#pragma pipeloop(0)
	for (; (mlib_addr)dpz < (mlib_addr)dpzend; ) {
		dx1 = vis_ld_d64_nf(dpx);
		dpx++;
		dx = vis_faligndata(dx0, dx1);
		ADDS_S8_MOD;
		(*dpz++) = dr;
		dx0 = dx1;
	}

	if ((mlib_addr)dpz <= (mlib_addr)pzend) {
		dx1 = vis_ld_d64_nf(dpx);
		dpx++;
		dx = vis_faligndata(dx0, dx1);
		ADDS_S8_MOD;
/* prepare edge mask for the last bytes */
		emask = vis_edge8(dpz, pzend);
/* store last bytes of result */
		vis_pst_8(dr, dpz, emask);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorAddS_S16C_S8C_Mod(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *c,
	mlib_s32 n)
{
	mlib_d64 *dpz, *dpx;
	mlib_d64 dx, dx0, dx1;
	mlib_d64 dxh, dxl, dzh, dzl;
	mlib_f32 fone = vis_to_float(0x100);
	mlib_s8 uc = *((mlib_u8 *)c);
	mlib_s8 uc1 = *((mlib_u8 *)c + 1);
	mlib_s8 *px;
	mlib_s16 *pz;
	mlib_s32 len = n << 1, i;

/* rest and leng in terms of 8 bytes. */
	mlib_s32 rest_8, even_8;

/*
 * Prepare scale == mlib_d64 (Re_Im_Re_Im)
 */
	mlib_d64 dc = vis_to_double_dup((uc1 & 0xffff) | (uc << 16));

/*
 * Prepare scale == mlib_d64 (Im_Re_Im_Re)
 */
	mlib_d64 dc_odd = vis_to_double_dup(uc & 0xffff | (uc1 << 16));
	mlib_s32 odd = 0;
	mlib_d64 restore = vis_to_double_dup(0x80808080);
	mlib_d64 displacement = vis_to_double_dup(0x00800080);

	if (n <= 0)
		return (MLIB_FAILURE);

	if (!(uc | uc1))
		return (__mlib_VectorConvert_S16C_S8C_Mod(z, x, n));

	px = (mlib_s8 *)x;
	pz = (mlib_s16 *)z;

	if (len <= 8)
		ADDS_US8C_S16C_IN_C;

/*
 * prepare the destination address
 */

	while ((mlib_addr)pz & 7) {
		(*pz++) = ((mlib_s16)(*px)) + uc;
		px++;
		len -= 2;

		if (!((mlib_addr)pz & 7)) {
			odd = 1;
			break;
		}

		(*pz++) = ((mlib_s16)(*px)) + uc1;
		px++;
	}

	dpz = (mlib_d64 *)pz;

	even_8 = len >> 3;
	rest_8 = len & 0x7;

	if (!odd) {
/*
 * Start with real part.
 */

		if (!((mlib_addr)px & 7)) {

/*
 * 'x' address is 8-byte aligned.
 * No  vis_alignaddr and  vis_faligndata at all.
 */

			dpx = (mlib_d64 *)px;

#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx = (*dpx++);
				ADDS_S8C_S16C;

/*
 * store 16 bytes of result
 */
				(*dpz++) = dzh;
				(*dpz++) = dzl;
			}

		} else {

/*
 * "x"  address is arbitrary aligned.
 * 1 vis_alignaddr and 1 vis_faligndata in the loop.
 */

			dpx = vis_alignaddr(px, 0);
			dx0 = vis_ld_d64_nf(dpx);
			dpx++;

#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx1 = vis_ld_d64_nf(dpx);
				dpx++;
				dx = vis_faligndata(dx0, dx1);
				ADDS_S8C_S16C;
				dx0 = dx1;
/*
 * store 16 bytes of result
 */
				dpz[0] = dzh;
				dpz[1] = dzl;
				dpz += 2;
			}
		}

		px += (even_8 << 3);
		pz = (mlib_s16 *)dpz;

	} else {

/*
 * odd==1. Start with imaginary part.
 */

		if (!((mlib_addr)px & 7)) {

/*
 * 'x' address is 8-byte aligned.
 * No  vis_alignaddr and  vis_faligndata at all.
 */

			dpx = (mlib_d64 *)px;

#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx = (*dpx++);
				ADDS_S16C_S8C_WITH_ODD;

/*
 * store 16 bytes of result
 */
				(*dpz++) = dzh;
				(*dpz++) = dzl;
			}

		} else {

/*
 * "x"  address is arbitrary aligned.
 * 1 vis_alignaddr and 1 vis_faligndata in the loop.
 */

			dpx = vis_alignaddr(px, 0);
			dx0 = vis_ld_d64_nf(dpx);
			dpx++;

#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx1 = vis_ld_d64_nf(dpx);
				dpx++;
				dx = vis_faligndata(dx0, dx1);
				ADDS_S16C_S8C_WITH_ODD;
				dx0 = dx1;
/*
 * store 16 bytes of result
 */
				dpz[0] = dzh;
				dpz[1] = dzl;
				dpz += 2;
			}
		}

		px += (even_8 << 3);
		pz = (mlib_s16 *)dpz;
		(*pz++) = *px + uc1;
		px++;
	}

	if (!rest_8)
		return (MLIB_SUCCESS);

	while (rest_8) {
		(*pz++) = ((mlib_s16)(*px)) + uc;
		px++;
		(*pz++) = ((mlib_s16)(*px)) + uc1;
		px++;
		rest_8 -= 2;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorAddS_S8C_Sat(
	mlib_s8 *xz,
	const mlib_s8 *c,
	mlib_s32 n)
{
	return (__mlib_VectorAddS_S8C_S8C_Sat(xz, xz, c, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorAddS_S8C_Mod(
	mlib_s8 *xz,
	const mlib_s8 *c,
	mlib_s32 n)
{
	return (__mlib_VectorAddS_S8C_S8C_Mod(xz, xz, c, n));
}

/* *********************************************************** */
