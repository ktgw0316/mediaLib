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

#pragma ident	"@(#)mlib_v_VectorSub8.c	9.5	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorSub_U8 - subtraction of unsigned 8-bit format vectors
 *      mlib_VectorSub_S8 - subtraction of signed 8-bit format vectors
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorSub_S8_[Sat|Mod](mlib_s8       *xz,
 *                                              const mlib_s8 *y,
 *                                              mlib_s32      n);
 *      mlib_status mlib_VectorSub_S8_S8_[Sat|Mod](mlib_s8       *z,
 *                                                 const mlib_s8 *x,
 *                                                 const mlib_s8 *y,
 *                                                 mlib_s32      n);
 *      mlib_status mlib_VectorSub_U8_[Sat|Mod](mlib_u8       *xz,
 *                                              const mlib_u8 *y,
 *                                              mlib_s32      n);
 *      mlib_status mlib_VectorSub_U8_U8_[Sat|Mod](mlib_u8       *z,
 *                                                 const mlib_u8 *x,
 *                                                 const mlib_u8 *y,
 *                                                 mlib_s32      n);
 *
 *      mlib_status mlib_VectorSub_S16_S8_Mod(mlib_s16      *z,
 *                                            const mlib_s8 *x,
 *                                            const mlib_s8 *y,
 *                                            mlib_s32      n);
 *      mlib_status mlib_VectorSub_S16_U8_Mod(mlib_s16      *z,
 *                                            const mlib_u8 *x,
 *                                            const mlib_u8 *y,
 *                                            mlib_s32      n);
 *
 * ARGUMENTS
 *      z    pointer to the first element of the result vector
 *      x    pointer to the first element of the first vector
 *      y    pointer to the first element of the second vector
 *      n    number of elements in the vectors
 *
 * DESCRIPTION
 *      z = x - y
 */

/*
 * FUNCTIONS
 *      mlib_VectorSub_U8C - subtraction of unsigned 8-bit
 *                           format complex vectors
 *      mlib_VectorSub_S8C - subtraction of signed 8-bit format complex vectors
 *
 * SYNOPSIS
 *      mlib_status mlib_VectorSub_S8C_[Sat|Mod](mlib_s8       *xz,
 *                                               const mlib_s8 *y,
 *                                               mlib_s32      n);
 *      mlib_status mlib_VectorSub_S8C_S8C_[Sat|Mod](mlib_s8       *z,
 *                                                   const mlib_s8 *x,
 *                                                   const mlib_s8 *y,
 *                                                   mlib_s32      n);
 *      mlib_status mlib_VectorSub_U8C_[Sat|Mod](mlib_u8       *xz,
 *                                               const mlib_u8 *y,
 *                                               mlib_s32      n);
 *      mlib_status mlib_VectorSub_U8C_U8C_[Sat|Mod](mlib_u8       *z,
 *                                                   const mlib_u8 *x,
 *                                                   const mlib_u8 *y,
 *                                                   mlib_s32      n);
 *
 *      mlib_status mlib_VectorSub_S16C_S8C_Mod(mlib_s16      *z,
 *                                              const mlib_s8 *x,
 *                                              const mlib_s8 *y,
 *                                              mlib_s32      n);
 *      mlib_status mlib_VectorSub_S16C_U8C_Mod(mlib_s16      *z,
 *                                              const mlib_u8 *x,
 *                                              const mlib_u8 *y,
 *                                              mlib_s32      n);
 *
 * ARGUMENTS
 *      z    pointer to the first complex element of the result vector.
 *           z[2*i] hold the real parts, and z[2*i + 1] hold the imaginary
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
 *      z = x - y
 */

#include <mlib_algebra.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorSub_S8C_S8C_Sat = __mlib_VectorSub_S8C_S8C_Sat
#pragma weak mlib_VectorSub_S16_S8_Mod = __mlib_VectorSub_S16_S8_Mod
#pragma weak mlib_VectorSub_S16_U8_Mod = __mlib_VectorSub_S16_U8_Mod
#pragma weak mlib_VectorSub_U8C_U8C_Sat = __mlib_VectorSub_U8C_U8C_Sat
#pragma weak mlib_VectorSub_S16C_S8C_Mod = __mlib_VectorSub_S16C_S8C_Mod
#pragma weak mlib_VectorSub_S8_Mod = __mlib_VectorSub_S8_Mod
#pragma weak mlib_VectorSub_S16C_U8C_Mod = __mlib_VectorSub_S16C_U8C_Mod
#pragma weak mlib_VectorSub_U8_Mod = __mlib_VectorSub_U8_Mod
#pragma weak mlib_VectorSub_S8C_Mod = __mlib_VectorSub_S8C_Mod
#pragma weak mlib_VectorSub_U8C_Mod = __mlib_VectorSub_U8C_Mod
#pragma weak mlib_VectorSub_S8_Sat = __mlib_VectorSub_S8_Sat
#pragma weak mlib_VectorSub_U8_Sat = __mlib_VectorSub_U8_Sat
#pragma weak mlib_VectorSub_S8_S8_Mod = __mlib_VectorSub_S8_S8_Mod
#pragma weak mlib_VectorSub_S8C_Sat = __mlib_VectorSub_S8C_Sat
#pragma weak mlib_VectorSub_U8C_Sat = __mlib_VectorSub_U8C_Sat
#pragma weak mlib_VectorSub_U8_U8_Mod = __mlib_VectorSub_U8_U8_Mod
#pragma weak mlib_VectorSub_S8C_S8C_Mod = __mlib_VectorSub_S8C_S8C_Mod
#pragma weak mlib_VectorSub_S8_S8_Sat = __mlib_VectorSub_S8_S8_Sat
#pragma weak mlib_VectorSub_U8C_U8C_Mod = __mlib_VectorSub_U8C_U8C_Mod
#pragma weak mlib_VectorSub_U8_U8_Sat = __mlib_VectorSub_U8_U8_Sat

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_VectorSub_S8C_S8C_Sat) mlib_VectorSub_S8C_S8C_Sat
	__attribute__((weak, alias("__mlib_VectorSub_S8C_S8C_Sat")));
__typeof__(__mlib_VectorSub_S16_S8_Mod) mlib_VectorSub_S16_S8_Mod
	__attribute__((weak, alias("__mlib_VectorSub_S16_S8_Mod")));
__typeof__(__mlib_VectorSub_S16_U8_Mod) mlib_VectorSub_S16_U8_Mod
	__attribute__((weak, alias("__mlib_VectorSub_S16_U8_Mod")));
__typeof__(__mlib_VectorSub_U8C_U8C_Sat) mlib_VectorSub_U8C_U8C_Sat
	__attribute__((weak, alias("__mlib_VectorSub_U8C_U8C_Sat")));
__typeof__(__mlib_VectorSub_S16C_S8C_Mod) mlib_VectorSub_S16C_S8C_Mod
	__attribute__((weak, alias("__mlib_VectorSub_S16C_S8C_Mod")));
__typeof__(__mlib_VectorSub_S8_Mod) mlib_VectorSub_S8_Mod
	__attribute__((weak, alias("__mlib_VectorSub_S8_Mod")));
__typeof__(__mlib_VectorSub_S16C_U8C_Mod) mlib_VectorSub_S16C_U8C_Mod
	__attribute__((weak, alias("__mlib_VectorSub_S16C_U8C_Mod")));
__typeof__(__mlib_VectorSub_U8_Mod) mlib_VectorSub_U8_Mod
	__attribute__((weak, alias("__mlib_VectorSub_U8_Mod")));
__typeof__(__mlib_VectorSub_S8C_Mod) mlib_VectorSub_S8C_Mod
	__attribute__((weak, alias("__mlib_VectorSub_S8C_Mod")));
__typeof__(__mlib_VectorSub_U8C_Mod) mlib_VectorSub_U8C_Mod
	__attribute__((weak, alias("__mlib_VectorSub_U8C_Mod")));
__typeof__(__mlib_VectorSub_S8_Sat) mlib_VectorSub_S8_Sat
	__attribute__((weak, alias("__mlib_VectorSub_S8_Sat")));
__typeof__(__mlib_VectorSub_U8_Sat) mlib_VectorSub_U8_Sat
	__attribute__((weak, alias("__mlib_VectorSub_U8_Sat")));
__typeof__(__mlib_VectorSub_S8_S8_Mod) mlib_VectorSub_S8_S8_Mod
	__attribute__((weak, alias("__mlib_VectorSub_S8_S8_Mod")));
__typeof__(__mlib_VectorSub_S8C_Sat) mlib_VectorSub_S8C_Sat
	__attribute__((weak, alias("__mlib_VectorSub_S8C_Sat")));
__typeof__(__mlib_VectorSub_U8C_Sat) mlib_VectorSub_U8C_Sat
	__attribute__((weak, alias("__mlib_VectorSub_U8C_Sat")));
__typeof__(__mlib_VectorSub_U8_U8_Mod) mlib_VectorSub_U8_U8_Mod
	__attribute__((weak, alias("__mlib_VectorSub_U8_U8_Mod")));
__typeof__(__mlib_VectorSub_S8C_S8C_Mod) mlib_VectorSub_S8C_S8C_Mod
	__attribute__((weak, alias("__mlib_VectorSub_S8C_S8C_Mod")));
__typeof__(__mlib_VectorSub_S8_S8_Sat) mlib_VectorSub_S8_S8_Sat
	__attribute__((weak, alias("__mlib_VectorSub_S8_S8_Sat")));
__typeof__(__mlib_VectorSub_U8C_U8C_Mod) mlib_VectorSub_U8C_U8C_Mod
	__attribute__((weak, alias("__mlib_VectorSub_U8C_U8C_Mod")));
__typeof__(__mlib_VectorSub_U8_U8_Sat) mlib_VectorSub_U8_U8_Sat
	__attribute__((weak, alias("__mlib_VectorSub_U8_U8_Sat")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

/*
 *
 *   1. Expand from 8 -> 16 bits representation.
 *   2. Arithmetic minus in 16-bit format.
 *   3. Pack to 8 bits again.
 *   4. 64-bit Store.
 *
 */

#define	SUB_U8_SAT                                              \
	dxh = vis_fmul8x16al(vis_read_hi(dx), fmul);            \
	dxl = vis_fmul8x16al(vis_read_lo(dx), fmul);            \
	dyh = vis_fexpand_hi(dy);                               \
	dyl = vis_fexpand_lo(dy);                               \
	dzh = vis_fpsub16(dxh, dyh);                            \
	dzl = vis_fpsub16(dxl, dyl);                            \
	dz = vis_fpack16_pair(dzh, dzl)

/* *********************************************************** */

/*
 *
 *   1. Expand from 8 -> 16 bits representation via "vis_fmul8x16al"
 *      (multiplication by 256).
 *   2. Arithmetic minus in 16-bit format.
 *   3. 2*64-bit Stores.
 *
 */

#define	SUB_U8_S16                                              \
	dxh = vis_fmul8x16al(vis_read_hi(dx), fone);            \
	dyh = vis_fmul8x16al(vis_read_hi(dy), fone);            \
	dxl = vis_fmul8x16al(vis_read_lo(dx), fone);            \
	dyl = vis_fmul8x16al(vis_read_lo(dy), fone);            \
	dzh = vis_fpsub16(dxh, dyh);                            \
	dzl = vis_fpsub16(dxl, dyl)

/* *********************************************************** */

/*
 *
 *   1. Logical XOR dx ^ 0x8080808080808080. This is equivalent to
 *      addition to mlib_s32 byte 128. Hence 'dx' becomes positive.
 *   2. Logical XOR dy ^ 0x8080808080808080. This is equivalent to
 *      addition to mlib_s32 byte 128. 'dy' also becomes positive.
 *   3. Expand from 8 -> 16 bits representation via "vis_fmul8x16al"
 *      (multiplication by 256). (dx + 128)-(dy + 128)==dx - dy.
 *   4. 16-bit arithmetic minus.
 *   5. 2*64-bit Stores.
 *
 */

#define	SUB_S8_S16                                              \
	dx = vis_fxor(dx, restore);                             \
	dy = vis_fxor(dy, restore);                             \
	dxh = vis_fmul8x16al(vis_read_hi(dx), fone);            \
	dyh = vis_fmul8x16al(vis_read_hi(dy), fone);            \
	dxl = vis_fmul8x16al(vis_read_lo(dx), fone);            \
	dyl = vis_fmul8x16al(vis_read_lo(dy), fone);            \
	dzh = vis_fpsub16(dxh, dyh);                            \
	dzl = vis_fpsub16(dxl, dyl)

/* *********************************************************** */

/*
 *
 *   zi = displacement - ((xi ^ 0x7f) - (yi ^ 0x7f)),
 *   where  displacement = 128 (we use 128 to reduce the result to the
 *                              range which is necessary for correct
 *                              execution of fpack16 operation)
 *    restore = 0x80808080808080
 *
 *    After fpack16 operation is done all results are in the range
 *    [0 , 255] and xor operation with constant "restore" reduces
 *    the result to the range [-128 , 127].
 */

#define	SUB_S8_SAT                                              \
	dx = vis_fxnor(dx, restore);                            \
	dxh = vis_fmul8x16al(vis_read_hi(dx), fmul);            \
	dxl = vis_fmul8x16al(vis_read_lo(dx), fmul);            \
	dy = vis_fxnor(dy, restore);                            \
	dyh = vis_fmul8x16al(vis_read_hi(dy), fmul);            \
	dyl = vis_fmul8x16al(vis_read_lo(dy), fmul);            \
	dzh = vis_fpsub16(dxh, dyh);                            \
	dzl = vis_fpsub16(dxl, dyl);                            \
	dh = vis_fpsub16(displacement, dzh);                    \
	dl = vis_fpsub16(displacement, dzl);                    \
	dz = vis_fpack16_pair(dh, dl);                          \
	dz = vis_fxor(dz, restore)

/* *********************************************************** */

#define	SUB_S8_MOD                                              \
	dxl = vis_fand(dx, mask_control);                       \
	dh = vis_fpsub16(dx, dy);                               \
	dyl = vis_fand(dy, mask_control);                       \
	dh = vis_fandnot(mask_control, dh);                     \
	dl = vis_fpsub16(dxl, dyl);                             \
	dz = vis_for(dh, dl)

/* *********************************************************** */

#define	SUB_S16_U8_IN_C                                         \
	{                                                       \
	    while (n--) {                                       \
		(*pz++) = ((mlib_s16)(*px)) - (*py);            \
		px++;                                           \
		py++;                                           \
	    }                                                   \
	    return (MLIB_SUCCESS);                              \
	}

/* *********************************************************** */

#define	SUB_S16_S8_IN_C                                         \
	{                                                       \
	    while (n--) {                                       \
		(*pz++) = ((mlib_s16)(*px)) - (*py);            \
		px++;                                           \
		py++;                                           \
	    }                                                   \
	    return (MLIB_SUCCESS);                              \
	}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_U8_U8_Sat(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
	mlib_d64 *dpz, *dpx, *dpy;
	mlib_d64 dx, dy, dz, dx0, dx1, dy0, dy1;
	mlib_d64 dxh, dxl, dyh, dyl, dzh, dzl;
	mlib_u8 *pz = z, *px, *py, *pzend;

/* offset of address alignment in destination */
	mlib_s32 off;
	mlib_s32 len = n, i;

/* rest and leng in terms of 8 bytes. */
	mlib_s32 rest_8, even_8;

/* edge masks */
	mlib_s32 emask;
	mlib_f32 fmul = vis_to_float(0x1000);
	mlib_s32 align;

	if (n <= 0)
		return (MLIB_FAILURE);

	px = (mlib_u8 *)x;
	py = (mlib_u8 *)y;

/* initialize GSR scale factor */
	vis_write_gsr(3 << 3);

	dpz = (mlib_d64 *)((mlib_addr)z & (~7));
	off = (mlib_s64)dpz - (mlib_s64)z;
	pzend = pz + n - 1;

/*
 * generate edge mask for the start point
 */
	emask = vis_edge8(pz, pzend);

/*
 * prepare the source address
 */

	if (off) {
		dpy = (mlib_d64 *)vis_alignaddr(py, off);
		dy0 = vis_ld_d64_nf(dpy);
		dy1 = vis_ld_d64_nf(dpy + 1);
		dy = vis_faligndata(dy0, dy1);
		dpx = (mlib_d64 *)vis_alignaddr(px, off);
		dx0 = vis_ld_d64_nf(dpx);
		dx1 = vis_ld_d64_nf(dpx + 1);
		dx = vis_faligndata(dx0, dx1);
		SUB_U8_SAT;

/*
 * store first bytes of result
 */

		vis_pst_8(dz, dpz, emask);

		px += (8 + off);
		py += (8 + off);
		len -= (8 + off);
		dpz++;

		if (len <= 0)
			return (MLIB_SUCCESS);
	}

	even_8 = len >> 3;
	rest_8 = len & 0x7;

	if (!(((mlib_addr)px | (mlib_addr)py) & 7)) {
		dpx = (mlib_d64 *)px;
		dpy = (mlib_d64 *)py;

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dx = dpx[0];
			dy = dpy[0];
			SUB_U8_SAT;
			dpz[0] = dz;
			dpx++;
			dpy++;
			dpz++;
		}

		dx1 = vis_ld_d64_nf(dpx);
		dy1 = vis_ld_d64_nf(dpy);

	} else
	if (!((mlib_addr)px & 7)) {
		dpx = (mlib_d64 *)px;
		align = (mlib_addr)py & 7;
		dpy = (mlib_d64 *)(py - align);
		vis_write_bmask(align * 0x11111111, 0x01234567);
		dy0 = vis_ld_d64_nf(dpy); dpy++;

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dx = dpx[0];
			dy1 = vis_ld_d64_nf(dpy);
			dy = vis_bshuffle(dy0, dy1);
			SUB_U8_SAT;
			dpz[0] = dz;
			dy0 = dy1;
			dpx++;
			dpy++;
			dpz++;
		}

		dx1 = vis_ld_d64_nf(dpx);
		dy1 = dy0;

	} else
	if (!((mlib_addr)py & 7)) {
		dpy = (mlib_d64 *)py;
		align = (mlib_addr)px & 7;
		dpx = (mlib_d64 *)(px - align);
		vis_write_bmask(align * 0x11111111, 0x01234567);
		dx0 = vis_ld_d64_nf(dpx); dpx++;

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dx1 = vis_ld_d64_nf(dpx);
			dx = vis_bshuffle(dx0, dx1);
			dy = dpy[0];
			SUB_U8_SAT;
			dpz[0] = dz;
			dx0 = dx1;
			dpx++;
			dpy++;
			dpz++;
		}

		dy1 = vis_ld_d64_nf(dpy);
		dx1 = dx0;

	} else
	if (((((mlib_addr)px) ^ ((mlib_addr)py)) & 7) == 0) {
		align = (mlib_addr)px & 7;
		dpx = (mlib_d64 *)(px - align);
		dpy = (mlib_d64 *)(py - align);
		vis_write_bmask(align * 0x11111111, 0x01234567);
		dx0 = vis_ld_d64_nf(dpx); dpx++;
		dy0 = vis_ld_d64_nf(dpy); dpy++;

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dx1 = vis_ld_d64_nf(dpx);
			dx = vis_bshuffle(dx0, dx1);
			dy1 = vis_ld_d64_nf(dpy);
			dy = vis_bshuffle(dy0, dy1);

			dxh = vis_fmul8x16al(vis_read_hi(dx), fmul);
			dyh = vis_fmul8x16al(vis_read_hi(dy), fmul);
			dxl = vis_fmul8x16al(vis_read_lo(dx), fmul);
			dyl = vis_fexpand_lo(dy);
			dzh = vis_fpsub16(dxh, dyh);
			dzl = vis_fpsub16(dxl, dyl);
			dz = vis_fpack16_pair(dzh, dzl);
			dpz[0] = dz;
			dx0 = dx1;
			dy0 = dy1;
			dpx++;
			dpy++;
			dpz++;
		}

		dx1 = dx0;
		dy1 = dy0;

	} else {
		align = (mlib_addr)px & 7;
		dpx = (mlib_d64 *)(px - align);
		vis_write_bmask(align * 0x11111111, 0x01234567);
		dx0 = vis_ld_d64_nf(dpx); dpx++;
		dpy = vis_alignaddr(py, 0);
		dy0 = vis_ld_d64_nf(dpy); dpy++;

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dx1 = vis_ld_d64_nf(dpx);
			dx = vis_bshuffle(dx0, dx1);
			dy1 = vis_ld_d64_nf(dpy);
			dy = vis_faligndata(dy0, dy1);

			dxh = vis_fmul8x16al(vis_read_hi(dx), fmul);
			dyh = vis_fmul8x16al(vis_read_hi(dy), fmul);
			dxl = vis_fmul8x16al(vis_read_lo(dx), fmul);
			dyl = vis_fexpand_lo(dy);
			dzh = vis_fpsub16(dxh, dyh);
			dzl = vis_fpsub16(dxl, dyl);
			dz = vis_fpack16_pair(dzh, dzl);

/*
 * store 8 bytes of result
 */
			(*dpz++) = dz;
			dx0 = dx1;
			dy0 = dy1;
			dpy++;
			dpx++;
		}

		dx1 = dx0;
		dy1 = dy0;
	}

	if (!rest_8)
		return (MLIB_SUCCESS);

	align = (mlib_addr)px & 7;
	vis_write_bmask(align * 0x11111111, 0x01234567);
	dx0 = dx1;
	dx1 = vis_ld_d64_nf(dpx);
	vis_alignaddr(py, 0);
	dy0 = dy1;
	dy1 = vis_ld_d64_nf(dpy);
	dx = vis_bshuffle(dx0, dx1);
	dy = vis_faligndata(dy0, dy1);
	SUB_U8_SAT;

/*
 * prepare edge mask for the last bytes
 */

	emask = vis_edge8((void *)(rest_8), pzend);

/* store last bytes of result */
	vis_pst_8(dz, dpz, ~emask);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_U8_Sat(
	mlib_u8 *xz,
	const mlib_u8 *y,
	mlib_s32 n)
{
	return (__mlib_VectorSub_U8_U8_Sat(xz, xz, y, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S8_S8_Sat(
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

/* offset of address alignment in destination */
	mlib_s32 off;
	mlib_s32 len = n, i;

/* rest and leng in terms of 8 bytes. */
	mlib_s32 rest_8, even_8;

/* edge masks */
	mlib_s32 emask;
	mlib_d64 displacement = vis_to_double_dup(0x8000800);
	mlib_d64 restore = vis_to_double_dup(0x80808080);
	mlib_f32 fmul = vis_to_float(0x1000);
	mlib_s32 align;

	if (n <= 0)
		return (MLIB_FAILURE);

	px = (mlib_s8 *)x;
	py = (mlib_s8 *)y;

/* initialize GSR scale factor */
	vis_write_gsr(3 << 3);

	dpz = (mlib_d64 *)((mlib_addr)z & (~7));
	off = (mlib_s64)dpz - (mlib_s64)z;
	pzend = pz + n - 1;
/*
 * generate edge mask for the start point
 */
	emask = vis_edge8(pz, pzend);

/*
 * prepare the source address
 */

	if (off) {
		dpy = (mlib_d64 *)vis_alignaddr(py, off);
		dy0 = vis_ld_d64_nf(dpy);
		dy1 = vis_ld_d64_nf(dpy + 1);
		dy = vis_faligndata(dy0, dy1);
		dpx = (mlib_d64 *)vis_alignaddr(px, off);
		dx0 = vis_ld_d64_nf(dpx);
		dx1 = vis_ld_d64_nf(dpx + 1);
		dx = vis_faligndata(dx0, dx1);
		SUB_S8_SAT;

/*
 * store first bytes of result
 */

		vis_pst_8(dz, dpz, emask);

		px += (8 + off);
		py += (8 + off);
		len -= (8 + off);
		dpz++;

		if (len <= 0)
			return (MLIB_SUCCESS);
	}

	even_8 = len >> 3;
	rest_8 = len & 0x7;

/*
 * Now try to analyze source "x" and "y" addresses.
 */

	if (!(((mlib_addr)px | (mlib_addr)py) & 7)) {
		dpx = (mlib_d64 *)px;
		dpy = (mlib_d64 *)py;

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dx = (*dpx++);
			dy = (*dpy++);
			SUB_S8_SAT;
			(*dpz++) = dz;
		}

		dx1 = vis_ld_d64_nf(dpx); dpx++;
		dy1 = vis_ld_d64_nf(dpy); dpy++;

	} else if (!((mlib_addr)px & 7)) {
		dpx = (mlib_d64 *)px;
		dpy = vis_alignaddr(py, 0);
		dy0 = vis_ld_d64_nf(dpy); dpy++;

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dx = (*dpx++);
			dy1 = vis_ld_d64_nf(dpy); dpy++;
			dy = vis_faligndata(dy0, dy1);
			SUB_S8_SAT;
			dy0 = dy1;
			(*dpz++) = dz;
		}

		dx1 = vis_ld_d64_nf(dpx); dpx++;
		dy1 = dy0;
	} else if (!((mlib_addr)py & 7)) {
		dpy = (mlib_d64 *)py;
		dpx = vis_alignaddr(px, 0);
		dx0 = vis_ld_d64_nf(dpx); dpx++;

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dx1 = vis_ld_d64_nf(dpx); dpx++;
			dx = vis_faligndata(dx0, dx1);
			dy = (*dpy++);
			SUB_S8_SAT;
			dx0 = dx1;
			(*dpz++) = dz;
		}

		dx1 = dx0;
		dy1 = vis_ld_d64_nf(dpy); dpy++;

	} else if (((((mlib_addr)px) ^ ((mlib_addr)py)) & 7) == 0) {
		dpx = vis_alignaddr(px, 0);
		dx0 = vis_ld_d64_nf(dpx); dpx++;
		dpy = vis_alignaddr(py, 0);
		dy0 = vis_ld_d64_nf(dpy); dpy++;

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dx1 = vis_ld_d64_nf(dpx); dpx++;
			dx = vis_faligndata(dx0, dx1);
			dy1 = vis_ld_d64_nf(dpy); dpy++;
			dy = vis_faligndata(dy0, dy1);
			SUB_S8_SAT;
			dx0 = dx1;
			dy0 = dy1;
			(*dpz++) = dz;
		}

		dx1 = dx0;
		dy1 = dy0;

	} else {
		align = (mlib_addr)px & 7;
		dpx = (mlib_d64 *)(px - align);
		vis_write_bmask(align * 0x11111111, 0x01234567);
		dx0 = vis_ld_d64_nf(dpx); dpx++;
		dpy = vis_alignaddr(py, 0);
		dy0 = vis_ld_d64_nf(dpy); dpy++;

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dx1 = vis_ld_d64_nf(dpx); dpx++;
			dx = vis_bshuffle(dx0, dx1);
			dy1 = vis_ld_d64_nf(dpy); dpy++;
			dy = vis_faligndata(dy0, dy1);
			SUB_S8_SAT;
			dx0 = dx1;
			dy0 = dy1;
			(*dpz++) = dz;
		}

		dx1 = dx0;
		dy1 = dy0;
	}

	if (!rest_8)
		return (MLIB_SUCCESS);

	align = (mlib_addr)px & 7;
	vis_write_bmask(align * 0x11111111, 0x01234567);
	dx0 = dx1;
	dx1 = vis_ld_d64_nf(dpx);
	dx = vis_bshuffle(dx0, dx1);
	vis_alignaddr(py, 0);
	dy0 = dy1;
	dy1 = vis_ld_d64_nf(dpy);
	dy = vis_faligndata(dy0, dy1);
	SUB_S8_SAT;

/*
 * prepare edge mask for the last bytes
 */

	emask = vis_edge8((void *)(rest_8), pzend);

/* store last bytes of result */
	vis_pst_8(dz, dpz, ~emask);

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S8_Sat(
	mlib_s8 *xz,
	const mlib_s8 *y,
	mlib_s32 n)
{
	return (__mlib_VectorSub_S8_S8_Sat(xz, xz, y, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_U8_U8_Mod(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
/* 8-byte aligned start point in destination */
	mlib_d64 *dpz;

/* 8-byte aligned start point in source */
	mlib_d64 *dpx, *dpy;

/* source  data */
	mlib_d64 dx, dy, dx0;
	mlib_d64 dx1, dy0, dy1;

/* destination data */
	mlib_d64 dz;

/* intermediate result */
	mlib_d64 dh, dl;
	mlib_d64 dxl, dyl;

/* end point of a line in destination */
	mlib_u8 *pzend;

/* start point of a line in source */
	mlib_u8 *px, *py;

/* offset of address alignment in destination */
	mlib_s32 off;

/* edge mask */
	mlib_s32 emask;
	mlib_s32 sr1, sr2, sr3, sr3_;
	mlib_s32 x8, x12, y8, y12;
	mlib_s32 mask = 0x7f7f7f7f, align;
	mlib_u8 *pz;
	mlib_s32 nrest;
	mlib_s32 len = n, i;

/* rest and leng in terms of 8 bytes. */
	mlib_s32 rest_8, even_8;
	mlib_d64 mask_control = vis_to_double_dup(0xff00ff00);

	if (n <= 0)
		return (MLIB_FAILURE);

	px = (mlib_u8 *)x;
	py = (mlib_u8 *)y;
	pz = (mlib_u8 *)z;

/*
 * prepare the destination address
 */
	pzend = pz + n - 1;

/*
 * check for 64-bit aligned special case
 */

	if ((((mlib_addr)x | (mlib_addr)y | (mlib_addr)z) & 7) == 0) {

/*  We can process source and destination vectors by 16 bytes.  */
		dpx = (mlib_d64 *)x;
		dpy = (mlib_d64 *)y;
		dpz = (mlib_d64 *)z;
#pragma pipeloop(0)
		for (i = 0; i < n >> 4; i++) {
			mlib_u64 ld0, ld1;

			dx = dpx[0];
			dy = dpy[0];
			SUB_S8_MOD;
/*  store 8 bytes of result  */
			(*dpz++) = dz;

			ld0 = *((mlib_u64 *)dpx + 1);
			ld1 = *((mlib_u64 *)dpy + 1);
			x8 = ld0 >> 32;
			y8 = ld1 >> 32;
			sr1 = x8 ^ ~y8;
			sr2 = (x8 | ~mask) - (y8 & mask);
			sr3 = (sr1 & ~mask) ^ sr2;
			x12 = ld0 & 0xFFFFFFFF;
			y12 = ld1 & 0xFFFFFFFF;
			sr1 = x12 ^ ~y12;
			sr2 = (x12 | ~mask) - (y12 & mask);
			sr3_ = (sr1 & ~mask) ^ sr2;
/*  store 8 bytes of result  */
			(*dpz++) = vis_to_double(sr3, sr3_);
			dpx += 2;
			dpy += 2;
		}

		nrest = n & 0xf;

		if (nrest >= 8) {
			dx = (*dpx++);
			dy = (*dpy++);
			SUB_S8_MOD;
			(*dpz++) = dz;
			nrest -= 8;
		}

		if (nrest > 0) {
			dx = (*dpx++);
			dy = (*dpy++);
			SUB_S8_MOD;
			emask = vis_edge8(dpz, pzend);
			vis_pst_8(dz, dpz, emask);
		}

	} else {

/*
 * General case.
 */

/* initialize GSR scale factor */
		vis_write_gsr(3 << 3);

		dpz = (mlib_d64 *)((mlib_addr)z & (~7));
		off = (mlib_addr)dpz - (mlib_addr)z;

/*
 * generate edge mask for the start point
 */
		emask = vis_edge8(pz, pzend);

/*
 * prepare the source address
 */

		if (off) {
			dpy = (mlib_d64 *)vis_alignaddr(py, off);
			dy0 = vis_ld_d64_nf(dpy);
			dy1 = vis_ld_d64_nf(dpy + 1);
			dy = vis_faligndata(dy0, dy1);
			dpx = (mlib_d64 *)vis_alignaddr(px, off);
			dx0 = vis_ld_d64_nf(dpx);
			dx1 = vis_ld_d64_nf(dpx + 1);
			dx = vis_faligndata(dx0, dx1);
			SUB_S8_MOD;

/*
 * store first bytes of result
 */

			vis_pst_8(dz, dpz, emask);

			px += (8 + off);
			py += (8 + off);
			len -= (8 + off);
			dpz++;

			if (len <= 0)
				return (MLIB_SUCCESS);
		}

		even_8 = len >> 3;
		rest_8 = len & 0x7;

/*
 * Now try to analyze source "x" and "y" addresses.
 */

		if (!(((mlib_addr)px | (mlib_addr)py) & 7)) {
			dpx = (mlib_d64 *)px;
			dpy = (mlib_d64 *)py;

#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx = (*dpx++);
				dy = (*dpy++);
				SUB_S8_MOD;
				(*dpz++) = dz;
			}

			dx1 = vis_ld_d64_nf(dpx); dpx++;
			dy1 = vis_ld_d64_nf(dpy); dpy++;

		} else
		if (!((mlib_addr)px & 7)) {
			dpx = (mlib_d64 *)px;
			dpy = vis_alignaddr(py, 0);
			dy0 = vis_ld_d64_nf(dpy); dpy++;

#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx = (*dpx++);
				dy1 = vis_ld_d64_nf(dpy); dpy++;
				dy = vis_faligndata(dy0, dy1);
				SUB_S8_MOD;
				dy0 = dy1;
				(*dpz++) = dz;
			}

			dx1 = vis_ld_d64_nf(dpx); dpx++;
			dy1 = dy0;

		} else
		if (!((mlib_addr)py & 7)) {
			dpy = (mlib_d64 *)py;
			dpx = vis_alignaddr(px, 0);
			dx0 = vis_ld_d64_nf(dpx); dpx++;

#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx1 = vis_ld_d64_nf(dpx); dpx++;
				dx = vis_faligndata(dx0, dx1);
				dy = (*dpy++);
				SUB_S8_MOD;
				(*dpz++) = dz;
				dx0 = dx1;
			}

			dx1 = dx0;
			dy1 = vis_ld_d64_nf(dpy); dpy++;

		} else
		if (((((mlib_addr)px) ^ ((mlib_addr)py)) & 7) == 0) {
			align = (mlib_addr)px & 7;
			dpx = (mlib_d64 *)(px - align);
			dpy = (mlib_d64 *)(py - align);
			vis_write_bmask(align * 0x11111111, 0x01234567);
			dx0 = vis_ld_d64_nf(dpx); dpx++;
			dy0 = vis_ld_d64_nf(dpy); dpy++;

#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx1 = vis_ld_d64_nf(dpx); dpx++;
				dx = vis_bshuffle(dx0, dx1);
				dy1 = vis_ld_d64_nf(dpy); dpy++;
				dy = vis_bshuffle(dy0, dy1);
				SUB_S8_MOD;
				(*dpz++) = dz;
				dx0 = dx1;
				dy0 = dy1;
			}

			dx1 = dx0;
			dy1 = dy0;
		} else {
			dpx = vis_alignaddr(px, 0);
			dx0 = vis_ld_d64_nf(dpx); dpx++;
			align = (mlib_addr)py & 7;
			vis_write_bmask(align * 0x11111111, 0x01234567);
			dpy = (mlib_d64 *)(py - align);
			dy0 = vis_ld_d64_nf(dpy); dpy++;

#pragma pipeloop(0)
			for (i = 0; i < even_8; i++) {
				dx1 = vis_ld_d64_nf(dpx);
				dx = vis_faligndata(dx0, dx1);
				dy1 = vis_ld_d64_nf(dpy);
				dy = vis_bshuffle(dy0, dy1);
				SUB_S8_MOD;

/*
 * store 8 bytes of result
 */
				(*dpz++) = dz;
				dpx++;
				dpy++;
				dx0 = dx1;
				dy0 = dy1;
			}

			dx1 = dx0;
			dy1 = dy0;
		}

		if (!rest_8)
			return (MLIB_SUCCESS);

		align = (mlib_addr)px & 7;
		vis_write_bmask(align * 0x11111111, 0x01234567);
		dx0 = dx1;
		dx1 = vis_ld_d64_nf(dpx);
		dx = vis_bshuffle(dx0, dx1);
		vis_alignaddr(py, 0);
		dy0 = dy1;
		dy1 = vis_ld_d64_nf(dpy);
		dy = vis_faligndata(dy0, dy1);
		SUB_S8_MOD;

/*
 * prepare edge mask for the last bytes
 */

		emask = vis_edge8((void *)(rest_8), pzend);

/* store last bytes of result */
		vis_pst_8(dz, dpz, ~emask);
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S16_U8_Mod(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
	mlib_d64 *dpz, *dpx, *dpy;
	mlib_d64 dx, dy, dx0, dx1, dy0, dy1;
	mlib_d64 dxh, dxl, dyh, dyl, dzh, dzl;
	mlib_f32 fone = vis_to_float(0x100);
	mlib_u8 *px, *py;
	mlib_s16 *pz;
	mlib_s32 len = n, i, boff;

/* rest and leng in terms of 8 bytes. */
	mlib_s32 rest_8, even_8;

	if (n <= 0)
		return (MLIB_FAILURE);

	px = (mlib_u8 *)x;
	py = (mlib_u8 *)y;
	pz = (mlib_s16 *)z;

	if (n <= 8)
		SUB_S16_U8_IN_C;

/*
 * prepare the source address
 */

	while ((mlib_addr)pz & 7) {
		(*pz++) = ((mlib_s16)(*px)) - (*py);
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

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dx = (*dpx++);
			dy = (*dpy++);
			SUB_U8_S16;
			(*dpz++) = dzh;
			(*dpz++) = dzl;
		}

	} else
	if (!((mlib_addr)px & 7)) {
		dpx = (mlib_d64 *)px;
		dpy = vis_alignaddr(py, 0);
		dy0 = vis_ld_d64_nf(dpy); dpy++;

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dx = (*dpx++);
			dy1 = vis_ld_d64_nf(dpy); dpy++;
			dy = vis_faligndata(dy0, dy1);
			SUB_U8_S16;
			dpz[0] = dzh;
			dpz[1] = dzl;
			dy0 = dy1;
			dpz += 2;
		}

	} else
	if (!((mlib_addr)py & 7)) {
		dpy = (mlib_d64 *)py;
		dpx = vis_alignaddr(px, 0);
		dx0 = vis_ld_d64_nf(dpx); dpx++;

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dx1 = vis_ld_d64_nf(dpx); dpx++;
			dx = vis_faligndata(dx0, dx1);
			dy = (*dpy++);
			SUB_U8_S16;
			dpz[0] = dzh;
			dpz[1] = dzl;
			dx0 = dx1;
			dpz += 2;
		}

	} else
	if (((((mlib_addr)px) ^ ((mlib_addr)py)) & 7) == 0) {
		dpx = vis_alignaddr(px, 0);
		dx0 = vis_ld_d64_nf(dpx); dpx++;
		dpy = vis_alignaddr(py, 0);
		dy0 = vis_ld_d64_nf(dpy); dpy++;

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dy1 = vis_ld_d64_nf(dpy); dpy++;
			dy = vis_faligndata(dy0, dy1);
			dx1 = vis_ld_d64_nf(dpx); dpx++;
			dx = vis_faligndata(dx0, dx1);
			SUB_U8_S16;
			dpz[0] = dzh;
			dpz[1] = dzl;
			dx0 = dx1;
			dy0 = dy1;
			dpz += 2;
		}

	} else {
		boff = (mlib_addr)px & 7;
		dpx = (mlib_d64 *)(px - boff);
		vis_write_bmask(boff * 0x11111111, 0x01234567);
		dx0 = vis_ld_d64_nf(dpx); dpx++;
		dpy = vis_alignaddr(py, 0);
		dy0 = vis_ld_d64_nf(dpy); dpy++;

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dx1 = vis_ld_d64_nf(dpx); dpx++;
			dx = vis_bshuffle(dx0, dx1);
			dy1 = vis_ld_d64_nf(dpy); dpy++;
			dy = vis_faligndata(dy0, dy1);
			SUB_U8_S16;
			dx0 = dx1;
			dy0 = dy1;
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
	py += (even_8 << 3);
	pz += (even_8 << 3);

	while (rest_8--) {
		(*pz++) = ((mlib_s16)(*px)) - (*py);
		px++;
		py++;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_U8_Mod(
	mlib_u8 *xz,
	const mlib_u8 *y,
	mlib_s32 n)
{
	return (__mlib_VectorSub_U8_U8_Mod(xz, xz, y, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S8_S8_Mod(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n)
{
	return __mlib_VectorSub_U8_U8_Mod((mlib_u8 *)z, (mlib_u8 *)x,
		(mlib_u8 *)y, n);
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S16_S8_Mod(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n)
{
	mlib_d64 *dpz, *dpx, *dpy;
	mlib_d64 dx, dy, dx0, dx1, dy0, dy1;
	mlib_d64 dxh, dxl, dyh, dyl, dzh, dzl;
	mlib_f32 fone = vis_to_float(0x100);
	mlib_s8 *px, *py;
	mlib_s16 *pz;
	mlib_s32 len = n, i;

/* rest and leng in terms of 8 bytes. */
	mlib_s32 rest_8, even_8;
	mlib_d64 restore = vis_to_double_dup(0x80808080);
	mlib_s32 align;

	if (n <= 0)
		return (MLIB_FAILURE);

	px = (mlib_s8 *)x;
	py = (mlib_s8 *)y;
	pz = (mlib_s16 *)z;

	if (n <= 8)
		SUB_S16_S8_IN_C;

/*
 * prepare the source address
 */

	while ((mlib_addr)pz & 7) {
		(*pz++) = ((mlib_s16)(*px)) - (*py);
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

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dx = (*dpx++);
			dy = (*dpy++);
			SUB_S8_S16;
			dpz[0] = dzh;
			dpz[1] = dzl;
			dpz += 2;
		}

	} else
	if (!((mlib_addr)px & 7)) {
		dpx = (mlib_d64 *)px;
		align = (mlib_addr)py & 7;
		dpy = (mlib_d64 *)(py - align);
		vis_write_bmask(align * 0x11111111, 0x01234567);
		dy0 = vis_ld_d64_nf(dpy); dpy++;

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dx = (*dpx++);
			dy1 = vis_ld_d64_nf(dpy); dpy++;
			dy = vis_bshuffle(dy0, dy1);
			SUB_S8_S16;
			dy0 = dy1;
			dpz[0] = dzh;
			dpz[1] = dzl;
			dpz += 2;
		}

	} else
	if (!((mlib_addr)py & 7)) {
		align = (mlib_addr)px & 7;
		dpx = (mlib_d64 *)(px - align);
		vis_write_bmask(align * 0x11111111, 0x01234567);
		dx0 = vis_ld_d64_nf(dpx); dpx++;
		dpy = (mlib_d64 *)py;

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dx1 = vis_ld_d64_nf(dpx); dpx++;
			dx = vis_bshuffle(dx0, dx1);
			dy = (*dpy++);
			SUB_S8_S16;
			dx0 = dx1;
			dpz[0] = dzh;
			dpz[1] = dzl;
			dpz += 2;
		}

	} else
	if (((((mlib_addr)px) ^ ((mlib_addr)py)) & 7) == 0) {
		mlib_s32 align;

		align = (mlib_addr)px & 7;
		dpx = (mlib_d64 *)(px - align);
		vis_write_bmask(align * 0x11111111, 0x01234567);
		dx0 = vis_ld_d64_nf(dpx); dpx++;
		dpy = (mlib_d64 *)(py - align);
		dy0 = vis_ld_d64_nf(dpy); dpy++;

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dx1 = vis_ld_d64_nf(dpx); dpx++;
			dx = vis_bshuffle(dx0, dx1);
			dy1 = vis_ld_d64_nf(dpy); dpy++;
			dy = vis_bshuffle(dy0, dy1);
			SUB_S8_S16;
			dx0 = dx1;
			dy0 = dy1;
			dpz[0] = dzh;
			dpz[1] = dzl;
			dpz += 2;
		}

	} else {
		mlib_s32 align;

		align = (mlib_addr)px & 7;
		dpx = (mlib_d64 *)(px - align);
		vis_write_bmask(align * 0x11111111, 0x01234567);
		dx0 = vis_ld_d64_nf(dpx); dpx++;
		dpy = vis_alignaddr(py, 0);
		dy0 = vis_ld_d64_nf(dpy); dpy++;

#pragma pipeloop(0)
		for (i = 0; i < even_8; i++) {
			dx1 = vis_ld_d64_nf(dpx); dpx++;
			dx = vis_bshuffle(dx0, dx1);
			dy1 = vis_ld_d64_nf(dpy); dpy++;
			dy = vis_faligndata(dy0, dy1);
			SUB_S8_S16;
			dx0 = dx1;
			dy0 = dy1;
			dpz[0] = dzh;
			dpz[1] = dzl;
			dpz += 2;
		}
	}

	if (!rest_8)
		return (MLIB_SUCCESS);

	px += (even_8 << 3);
	py += (even_8 << 3);
	pz += (even_8 << 3);

	while (rest_8--) {
		(*pz++) = ((mlib_s16)(*px)) - (*py);
		px++;
		py++;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S8_Mod(
	mlib_s8 *xz,
	const mlib_s8 *y,
	mlib_s32 n)
{
	return (__mlib_VectorSub_S8_S8_Mod(xz, xz, y, n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_U8C_U8C_Sat(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
	return (__mlib_VectorSub_U8_U8_Sat(z, x, y, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_U8C_Sat(
	mlib_u8 *xz,
	const mlib_u8 *y,
	mlib_s32 n)
{
	return (__mlib_VectorSub_U8_U8_Sat(xz, xz, y, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_U8C_U8C_Mod(
	mlib_u8 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
	return (__mlib_VectorSub_U8_U8_Mod(z, x, y, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S16C_U8C_Mod(
	mlib_s16 *z,
	const mlib_u8 *x,
	const mlib_u8 *y,
	mlib_s32 n)
{
	return (__mlib_VectorSub_S16_U8_Mod(z, x, y, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_U8C_Mod(
	mlib_u8 *xz,
	const mlib_u8 *y,
	mlib_s32 n)
{
	return (__mlib_VectorSub_U8_U8_Mod(xz, xz, y, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S8C_S8C_Sat(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n)
{
	return (__mlib_VectorSub_S8_S8_Sat(z, x, y, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S16C_S8C_Mod(
	mlib_s16 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n)
{
	return (__mlib_VectorSub_S16_S8_Mod(z, x, y, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S8C_Sat(
	mlib_s8 *xz,
	const mlib_s8 *y,
	mlib_s32 n)
{
	return (__mlib_VectorSub_S8_S8_Sat(xz, xz, y, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S8C_S8C_Mod(
	mlib_s8 *z,
	const mlib_s8 *x,
	const mlib_s8 *y,
	mlib_s32 n)
{
	return (__mlib_VectorSub_S8_S8_Mod(z, x, y, n + n));
}

/* *********************************************************** */

mlib_status
__mlib_VectorSub_S8C_Mod(
	mlib_s8 *xz,
	const mlib_s8 *y,
	mlib_s32 n)
{
	return (__mlib_VectorSub_S8_S8_Mod(xz, xz, y, n + n));
}

/* *********************************************************** */
