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

#pragma ident	"@(#)mlib_v_VolumeRayCast_BPT.c	9.2	07/11/05 SMI"

/*
 *  FUNCTION
 *    mlib_VolumeRayCast_Blocked_Parallel_Trilinear_[U8_U8|S16_S16]
 *      Cast parallel rays through a three dimensional data set
 *      and return the interpolated samples.
 *
 *  SYNOPSIS
 *    mlib_status mlib_VolumeRayCast_Blocked_Parallel_Trilinear_[U8_U8|S16_S16]
 *                                           (const mlib_rays      *rays,
 *                                            const mlib_blkvolume *blk,
 *                                            void                 *buffer)
 *
 *  ARGUMENTS
 *    rays      Casting rays.
 *    blk       Volume data in blocked data format.
 *    buffer    Working buffer.
 *
 *  DESCRIPTION
 *    In trilinear interpolation operation, the sample value at point P
 *    is replaced with the interpolated value of the 8 neighbor voxels.
 *
 */

#include <mlib_volume.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VolumeRayCast_Blocked_Parallel_Trilinear_U8_U8 = \
__mlib_VolumeRayCast_Blocked_Parallel_Trilinear_U8_U8
#pragma weak mlib_VolumeRayCast_Blocked_Parallel_Trilinear_S16_S16 = \
	__mlib_VolumeRayCast_Blocked_Parallel_Trilinear_S16_S16
#elif defined(__GNUC__)

__typeof__(__mlib_VolumeRayCast_Blocked_Parallel_Trilinear_U8_U8)
	mlib_VolumeRayCast_Blocked_Parallel_Trilinear_U8_U8
	__attribute__((weak,
	alias("__mlib_VolumeRayCast_Blocked_Parallel_Trilinear_U8_U8")));
__typeof__(__mlib_VolumeRayCast_Blocked_Parallel_Trilinear_S16_S16)
	mlib_VolumeRayCast_Blocked_Parallel_Trilinear_S16_S16
	__attribute__((weak,
	alias("__mlib_VolumeRayCast_Blocked_Parallel_Trilinear_S16_S16")));

#else

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	PREPOINT(ind)                                                    \
	ipack##ind## =                                                   \
		((mlib_u64)(((qz15 << 8) & 0xff800000) | (qy15 >> 14))   \
		<< 32) | (qx15 >> 4)

/* *********************************************************** */

#define	MUL16(b, a)                                               \
	vis_fpadd16(vis_fmul8sux16(a, b), vis_fmul8ulx16(a, b))

/* *********************************************************** */

#define	MUL8HI16(a, b)	vis_fmul8x16(vis_read_hi(a), b)

/* *********************************************************** */

#define	LOADPOINT_U8_8X                                                    \
	{                                                                  \
	    p000 = vis_faligndata(vis_ld_u8_i(voxels, vis_array8(ipack0,   \
		    n)), p000);                                            \
	    p001 = vis_faligndata(vis_ld_u8_i(voxels,                      \
		    vis_array8(ipack0 + 0x800, n)), p001);                 \
	    p010 = vis_faligndata(vis_ld_u8_i(voxels,                      \
		    vis_array8(ipack0 + 0x200000000, n)), p010);           \
	    p011 = vis_faligndata(vis_ld_u8_i(voxels,                      \
		    vis_array8(ipack0 + 0x200000800, n)), p011);           \
	    ipack0 += 0x80000000000000;                                    \
	    p100 = vis_faligndata(vis_ld_u8_i(voxels, vis_array8(ipack0,   \
		    n)), p100);                                            \
	    p101 = vis_faligndata(vis_ld_u8_i(voxels,                      \
		    vis_array8(ipack0 + 0x800, n)), p101);                 \
	    p110 = vis_faligndata(vis_ld_u8_i(voxels,                      \
		    vis_array8(ipack0 + 0x200000000, n)), p110);           \
	    p111 = vis_faligndata(vis_ld_u8_i(voxels,                      \
		    vis_array8(ipack0 + 0x200000800, n)), p111);           \
	    p000 = vis_faligndata(vis_ld_u8_i(voxels, vis_array8(ipack1,   \
		    n)), p000);                                            \
	    p001 = vis_faligndata(vis_ld_u8_i(voxels,                      \
		    vis_array8(ipack1 + 0x800, n)), p001);                 \
	    p010 = vis_faligndata(vis_ld_u8_i(voxels,                      \
		    vis_array8(ipack1 + 0x200000000, n)), p010);           \
	    p011 = vis_faligndata(vis_ld_u8_i(voxels,                      \
		    vis_array8(ipack1 + 0x200000800, n)), p011);           \
	    ipack1 += 0x80000000000000;                                    \
	    p100 = vis_faligndata(vis_ld_u8_i(voxels, vis_array8(ipack1,   \
		    n)), p100);                                            \
	    p101 = vis_faligndata(vis_ld_u8_i(voxels,                      \
		    vis_array8(ipack1 + 0x800, n)), p101);                 \
	    p110 = vis_faligndata(vis_ld_u8_i(voxels,                      \
		    vis_array8(ipack1 + 0x200000000, n)), p110);           \
	    p111 = vis_faligndata(vis_ld_u8_i(voxels,                      \
		    vis_array8(ipack1 + 0x200000800, n)), p111);           \
	    p000 = vis_faligndata(vis_ld_u8_i(voxels, vis_array8(ipack2,   \
		    n)), p000);                                            \
	    p001 = vis_faligndata(vis_ld_u8_i(voxels,                      \
		    vis_array8(ipack2 + 0x800, n)), p001);                 \
	    p010 = vis_faligndata(vis_ld_u8_i(voxels,                      \
		    vis_array8(ipack2 + 0x200000000, n)), p010);           \
	    p011 = vis_faligndata(vis_ld_u8_i(voxels,                      \
		    vis_array8(ipack2 + 0x200000800, n)), p011);           \
	    ipack2 += 0x80000000000000;                                    \
	    p100 = vis_faligndata(vis_ld_u8_i(voxels, vis_array8(ipack2,   \
		    n)), p100);                                            \
	    p101 = vis_faligndata(vis_ld_u8_i(voxels,                      \
		    vis_array8(ipack2 + 0x800, n)), p101);                 \
	    p110 = vis_faligndata(vis_ld_u8_i(voxels,                      \
		    vis_array8(ipack2 + 0x200000000, n)), p110);           \
	    p111 = vis_faligndata(vis_ld_u8_i(voxels,                      \
		    vis_array8(ipack2 + 0x200000800, n)), p111);           \
	    p000 = vis_faligndata(vis_ld_u8_i(voxels, vis_array8(ipack3,   \
		    n)), p000);                                            \
	    p001 = vis_faligndata(vis_ld_u8_i(voxels,                      \
		    vis_array8(ipack3 + 0x800, n)), p001);                 \
	    p010 = vis_faligndata(vis_ld_u8_i(voxels,                      \
		    vis_array8(ipack3 + 0x200000000, n)), p010);           \
	    p011 = vis_faligndata(vis_ld_u8_i(voxels,                      \
		    vis_array8(ipack3 + 0x200000800, n)), p011);           \
	    ipack3 += 0x80000000000000;                                    \
	    p100 = vis_faligndata(vis_ld_u8_i(voxels, vis_array8(ipack3,   \
		    n)), p100);                                            \
	    p101 = vis_faligndata(vis_ld_u8_i(voxels,                      \
		    vis_array8(ipack3 + 0x800, n)), p101);                 \
	    p110 = vis_faligndata(vis_ld_u8_i(voxels,                      \
		    vis_array8(ipack3 + 0x200000000, n)), p110);           \
	    p111 = vis_faligndata(vis_ld_u8_i(voxels,                      \
		    vis_array8(ipack3 + 0x200000800, n)), p111);           \
	}

/* *********************************************************** */

#define	LOADPOINT_U8_8X_CHECK                                          \
	{                                                              \
	    mlib_s32 index;                                            \
	                                                               \
	    index = vis_array8(ipack0, n);                             \
	    p000 = vis_faligndata(vis_ld_u8_i(voxels, index), p000);   \
	    index = vis_array8(ipack0 + 0x800, n);                     \
	    p001 = vis_faligndata(vis_ld_u8_i(voxels, index), p001);   \
	    index = vis_array8(ipack0 + 0x200000000, n);               \
	    p010 = vis_faligndata(vis_ld_u8_i(voxels, index), p010);   \
	    index = vis_array8(ipack0 + 0x200000800, n);               \
	    p011 = vis_faligndata(vis_ld_u8_i(voxels, index), p011);   \
	    ipack0 += 0x80000000000000;                                \
	    index = vis_array8(ipack0, n);                             \
	    p100 = vis_faligndata(vis_ld_u8_i(voxels, index), p100);   \
	    index = vis_array8(ipack0 + 0x800, n);                     \
	    p101 = vis_faligndata(vis_ld_u8_i(voxels, index), p101);   \
	    index = vis_array8(ipack0 + 0x200000000, n);               \
	    p110 = vis_faligndata(vis_ld_u8_i(voxels, index), p110);   \
	    index = vis_array8(ipack0 + 0x200000800, n);               \
	    p111 = vis_faligndata(vis_ld_u8_i(voxels, index), p111);   \
	    index = vis_array8(ipack1, n);                             \
	    index = index & ((index - maxindex) >> 31);                \
	    p000 = vis_faligndata(vis_ld_u8_i(voxels, index), p000);   \
	    index = vis_array8(ipack1 + 0x800, n);                     \
	    index = index & ((index - maxindex) >> 31);                \
	    p001 = vis_faligndata(vis_ld_u8_i(voxels, index), p001);   \
	    index = vis_array8(ipack1 + 0x200000000, n);               \
	    index = index & ((index - maxindex) >> 31);                \
	    p010 = vis_faligndata(vis_ld_u8_i(voxels, index), p010);   \
	    index = vis_array8(ipack1 + 0x200000800, n);               \
	    index = index & ((index - maxindex) >> 31);                \
	    p011 = vis_faligndata(vis_ld_u8_i(voxels, index), p011);   \
	    ipack1 += 0x80000000000000;                                \
	    index = vis_array8(ipack1, n);                             \
	    index = index & ((index - maxindex) >> 31);                \
	    p100 = vis_faligndata(vis_ld_u8_i(voxels, index), p100);   \
	    index = vis_array8(ipack1 + 0x800, n);                     \
	    index = index & ((index - maxindex) >> 31);                \
	    p101 = vis_faligndata(vis_ld_u8_i(voxels, index), p101);   \
	    index = vis_array8(ipack1 + 0x200000000, n);               \
	    index = index & ((index - maxindex) >> 31);                \
	    p110 = vis_faligndata(vis_ld_u8_i(voxels, index), p110);   \
	    index = vis_array8(ipack1 + 0x200000800, n);               \
	    index = index & ((index - maxindex) >> 31);                \
	    p111 = vis_faligndata(vis_ld_u8_i(voxels, index), p111);   \
	    index = vis_array8(ipack2, n);                             \
	    index = index & ((index - maxindex) >> 31);                \
	    p000 = vis_faligndata(vis_ld_u8_i(voxels, index), p000);   \
	    index = vis_array8(ipack2 + 0x800, n);                     \
	    index = index & ((index - maxindex) >> 31);                \
	    p001 = vis_faligndata(vis_ld_u8_i(voxels, index), p001);   \
	    index = vis_array8(ipack2 + 0x200000000, n);               \
	    index = index & ((index - maxindex) >> 31);                \
	    p010 = vis_faligndata(vis_ld_u8_i(voxels, index), p010);   \
	    index = vis_array8(ipack2 + 0x200000800, n);               \
	    index = index & ((index - maxindex) >> 31);                \
	    p011 = vis_faligndata(vis_ld_u8_i(voxels, index), p011);   \
	    ipack2 += 0x80000000000000;                                \
	    index = vis_array8(ipack2, n);                             \
	    index = index & ((index - maxindex) >> 31);                \
	    p100 = vis_faligndata(vis_ld_u8_i(voxels, index), p100);   \
	    index = vis_array8(ipack2 + 0x800, n);                     \
	    index = index & ((index - maxindex) >> 31);                \
	    p101 = vis_faligndata(vis_ld_u8_i(voxels, index), p101);   \
	    index = vis_array8(ipack2 + 0x200000000, n);               \
	    index = index & ((index - maxindex) >> 31);                \
	    p110 = vis_faligndata(vis_ld_u8_i(voxels, index), p110);   \
	    index = vis_array8(ipack2 + 0x200000800, n);               \
	    index = index & ((index - maxindex) >> 31);                \
	    p111 = vis_faligndata(vis_ld_u8_i(voxels, index), p111);   \
	    index = vis_array8(ipack3, n);                             \
	    index = index & ((index - maxindex) >> 31);                \
	    p000 = vis_faligndata(vis_ld_u8_i(voxels, index), p000);   \
	    index = vis_array8(ipack3 + 0x800, n);                     \
	    index = index & ((index - maxindex) >> 31);                \
	    p001 = vis_faligndata(vis_ld_u8_i(voxels, index), p001);   \
	    index = vis_array8(ipack3 + 0x200000000, n);               \
	    index = index & ((index - maxindex) >> 31);                \
	    p010 = vis_faligndata(vis_ld_u8_i(voxels, index), p010);   \
	    index = vis_array8(ipack3 + 0x200000800, n);               \
	    index = index & ((index - maxindex) >> 31);                \
	    p011 = vis_faligndata(vis_ld_u8_i(voxels, index), p011);   \
	    ipack3 += 0x80000000000000;                                \
	    index = vis_array8(ipack3, n);                             \
	    index = index & ((index - maxindex) >> 31);                \
	    p100 = vis_faligndata(vis_ld_u8_i(voxels, index), p100);   \
	    index = vis_array8(ipack3 + 0x800, n);                     \
	    index = index & ((index - maxindex) >> 31);                \
	    p101 = vis_faligndata(vis_ld_u8_i(voxels, index), p101);   \
	    index = vis_array8(ipack3 + 0x200000000, n);               \
	    index = index & ((index - maxindex) >> 31);                \
	    p110 = vis_faligndata(vis_ld_u8_i(voxels, index), p110);   \
	    index = vis_array8(ipack3 + 0x200000800, n);               \
	    index = index & ((index - maxindex) >> 31);                \
	    p111 = vis_faligndata(vis_ld_u8_i(voxels, index), p111);   \
	}

/* *********************************************************** */

mlib_status
__mlib_VolumeRayCast_Blocked_Parallel_Trilinear_U8_U8(
	mlib_rays *rays,
	const mlib_blkvolume *blk,
	void *buffer)
{
	const mlib_d64 done = vis_to_double_dup(0x7fff7fff);
	mlib_u8 *voxels;
	mlib_s32 xysize, zsize, nrays, nsteps;
	mlib_u32 n;
	mlib_s32 qdx, qdy, qdz, qdx15, qdy15, qdz15;
	mlib_u32 qx, qy, qz, qx15, qy15, qz15;
	mlib_d64 dxplus, dyplus, dzplus;
	mlib_s32 i, j, k;

	if (!rays || !blk)
		return (MLIB_NULLPOINTER);

	voxels = (mlib_u8 *)blk->voxels;
	xysize = blk->xysize, zsize = blk->zsize;
	nrays = rays->nrays;

	vis_write_gsr((2 << 3) + 7);

	i = xysize >> 6;
	n = 0;
	while (i >>= 1)
		n++;

	qdx = (mlib_s32)(rays->incs[0][0] * 4194304.);
	qdy = (mlib_s32)(rays->incs[0][1] * 4194304.);
	qdz = (mlib_s32)(rays->incs[0][2] * 4194304.);
	qdx15 = qdx >> 7;
	qdy15 = qdy >> 7;
	qdz15 = qdz >> 7;

	dxplus = vis_to_double(((qdx15 + qdx15 + qdx15) << 16) | ((qdx15 +
		qdx15) & 0x7fff), qdx15 << 16);
	dyplus = vis_to_double(((qdy15 + qdy15 + qdy15) << 16) | ((qdy15 +
		qdy15) & 0x7fff), qdy15 << 16);
	dzplus = vis_to_double(((qdz15 + qdz15 + qdz15) << 16) | ((qdz15 +
		qdz15) & 0x7fff), qdz15 << 16);

	for (i = 0; i < nrays; i++) {
		mlib_d64 ddx, ddy, ddz, dmx, dmy, dmz;
		mlib_d64 p000, p001, p010, p011, p100, p101, p110, p111;
		mlib_u64 ipack0, ipack1;
		mlib_u64 ipack2, ipack3;

		if ((nsteps = rays->nsteps[i]) <= 0)
			continue;

		qx = (mlib_u32)(mlib_s32)(rays->starts[i][0] * 1048576.);
		qy = (mlib_u32)(mlib_s32)(rays->starts[i][1] * 1048576.);
		qz = (mlib_u32)(mlib_s32)(rays->starts[i][2] * 1048576.);

		qx15 = qx >> 5;
		qy15 = qy >> 5;
		qz15 = qz >> 5;

		ddx = vis_to_double_dup((qx15 << 16) | (qx15 & 0x7fff));
		ddy = vis_to_double_dup((qy15 << 16) | (qy15 & 0x7fff));
		ddz = vis_to_double_dup((qz15 << 16) | (qz15 & 0x7fff));
		ddx = vis_fpadd16(ddx, dxplus);
		ddy = vis_fpadd16(ddy, dyplus);
		ddz = vis_fpadd16(ddz, dzplus);
		ddx = vis_fand(done, ddx);
		dmx = vis_fpsub16(done, ddx);
		ddy = vis_fand(done, ddy);
		dmy = vis_fpsub16(done, ddy);
		ddz = vis_fand(done, ddz);
		dmz = vis_fpsub16(done, ddz);
		qx += qdx;
		qy += qdy;
		qz += qdz;
		PREPOINT(0);
		qx15 += qdx15;
		qy15 += qdy15;
		qz15 += qdz15;
		PREPOINT(1);
		qx15 += qdx15;
		qy15 += qdy15;
		qz15 += qdz15;
		PREPOINT(2);
		qx15 += qdx15;
		qy15 += qdy15;
		qz15 += qdz15;
		PREPOINT(3);

#pragma pipeloop(0)
		for (j = 0; j < (nsteps - 1) >> 2; j++) {

			LOADPOINT_U8_8X;

			p000 = vis_fpadd16(MUL8HI16(p000, dmx), MUL8HI16(p001,
				ddx));
			p010 = vis_fpadd16(MUL8HI16(p010, dmx), MUL8HI16(p011,
				ddx));
			p100 = vis_fpadd16(MUL8HI16(p100, dmx), MUL8HI16(p101,
				ddx));
			p110 = vis_fpadd16(MUL8HI16(p110, dmx), MUL8HI16(p111,
				ddx));

			p000 = vis_fpadd16(MUL16(p000, dmy), MUL16(p010, ddy));
			p100 = vis_fpadd16(MUL16(p100, dmy), MUL16(p110, ddy));

			p000 = vis_fpadd16(MUL16(p000, dmz), MUL16(p100, ddz));

			qx15 = qx >> 5;
			qy15 = qy >> 5;
			qz15 = qz >> 5;
			qx += qdx;
			qy += qdy;
			qz += qdz;
			ddx = vis_to_double_dup((qx15 << 16) | (qx15 & 0x7fff));
			ddy = vis_to_double_dup((qy15 << 16) | (qy15 & 0x7fff));
			ddz = vis_to_double_dup((qz15 << 16) | (qz15 & 0x7fff));
			ddx = vis_fpadd16(ddx, dxplus);
			ddy = vis_fpadd16(ddy, dyplus);
			ddz = vis_fpadd16(ddz, dzplus);
			ddx = vis_fand(done, ddx);
			dmx = vis_fpsub16(done, ddx);
			ddy = vis_fand(done, ddy);
			dmy = vis_fpsub16(done, ddy);
			ddz = vis_fand(done, ddz);
			dmz = vis_fpsub16(done, ddz);
			PREPOINT(0);
			qx15 += qdx15;
			qy15 += qdy15;
			qz15 += qdz15;
			PREPOINT(1);
			qx15 += qdx15;
			qy15 += qdy15;
			qz15 += qdz15;
			PREPOINT(2);
			qx15 += qdx15;
			qy15 += qdy15;
			qz15 += qdz15;
			PREPOINT(3);

			p000 = vis_write_lo(p000, vis_fpack16(p000));

			vis_st_u8_i(p000, rays->results[4 * j + 0], i);
			p000 = vis_faligndata(p000, p000);
			vis_st_u8_i(p000, rays->results[4 * j + 1], i);
			p000 = vis_faligndata(p000, p000);
			vis_st_u8_i(p000, rays->results[4 * j + 2], i);
			p000 = vis_faligndata(p000, p000);
			vis_st_u8_i(p000, rays->results[4 * j + 3], i);
		}

		{
			mlib_s32 tail = nsteps - j * 4;
			mlib_s32 maxindex = xysize * xysize * zsize;

			LOADPOINT_U8_8X_CHECK;

			p000 = vis_fpadd16(MUL8HI16(p000, dmx), MUL8HI16(p001,
				ddx));
			p010 = vis_fpadd16(MUL8HI16(p010, dmx), MUL8HI16(p011,
				ddx));
			p100 = vis_fpadd16(MUL8HI16(p100, dmx), MUL8HI16(p101,
				ddx));
			p110 = vis_fpadd16(MUL8HI16(p110, dmx), MUL8HI16(p111,
				ddx));

			p000 = vis_fpadd16(MUL16(p000, dmy), MUL16(p010, ddy));
			p100 = vis_fpadd16(MUL16(p100, dmy), MUL16(p110, ddy));

			p000 = vis_fpadd16(MUL16(p000, dmz), MUL16(p100, ddz));

			p000 = vis_write_lo(p000, vis_fpack16(p000));

			vis_st_u8_i(p000, rays->results[4 * j], i);
			for (k = 1; k < tail; ++k) {
				p000 = vis_faligndata(p000, p000);
				vis_st_u8_i(p000, rays->results[4 * j + k], i);
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

#define	LOADPOINT_S16_8X                                            \
	{                                                           \
	    p000 = vis_faligndata(vis_ld_u16_i(voxels,              \
		    vis_array16(ipack0, n)), p000);                 \
	    p001 = vis_faligndata(vis_ld_u16_i(voxels,              \
		    vis_array16(ipack0 + 0x800, n)), p001);         \
	    p010 = vis_faligndata(vis_ld_u16_i(voxels,              \
		    vis_array16(ipack0 + 0x200000000, n)), p010);   \
	    p011 = vis_faligndata(vis_ld_u16_i(voxels,              \
		    vis_array16(ipack0 + 0x200000800, n)), p011);   \
	    ipack0 += 0x80000000000000;                             \
	    p100 = vis_faligndata(vis_ld_u16_i(voxels,              \
		    vis_array16(ipack0, n)), p100);                 \
	    p101 = vis_faligndata(vis_ld_u16_i(voxels,              \
		    vis_array16(ipack0 + 0x800, n)), p101);         \
	    p110 = vis_faligndata(vis_ld_u16_i(voxels,              \
		    vis_array16(ipack0 + 0x200000000, n)), p110);   \
	    p111 = vis_faligndata(vis_ld_u16_i(voxels,              \
		    vis_array16(ipack0 + 0x200000800, n)), p111);   \
	    p000 = vis_faligndata(vis_ld_u16_i(voxels,              \
		    vis_array16(ipack1, n)), p000);                 \
	    p001 = vis_faligndata(vis_ld_u16_i(voxels,              \
		    vis_array16(ipack1 + 0x800, n)), p001);         \
	    p010 = vis_faligndata(vis_ld_u16_i(voxels,              \
		    vis_array16(ipack1 + 0x200000000, n)), p010);   \
	    p011 = vis_faligndata(vis_ld_u16_i(voxels,              \
		    vis_array16(ipack1 + 0x200000800, n)), p011);   \
	    ipack1 += 0x80000000000000;                             \
	    p100 = vis_faligndata(vis_ld_u16_i(voxels,              \
		    vis_array16(ipack1, n)), p100);                 \
	    p101 = vis_faligndata(vis_ld_u16_i(voxels,              \
		    vis_array16(ipack1 + 0x800, n)), p101);         \
	    p110 = vis_faligndata(vis_ld_u16_i(voxels,              \
		    vis_array16(ipack1 + 0x200000000, n)), p110);   \
	    p111 = vis_faligndata(vis_ld_u16_i(voxels,              \
		    vis_array16(ipack1 + 0x200000800, n)), p111);   \
	    p000 = vis_faligndata(vis_ld_u16_i(voxels,              \
		    vis_array16(ipack2, n)), p000);                 \
	    p001 = vis_faligndata(vis_ld_u16_i(voxels,              \
		    vis_array16(ipack2 + 0x800, n)), p001);         \
	    p010 = vis_faligndata(vis_ld_u16_i(voxels,              \
		    vis_array16(ipack2 + 0x200000000, n)), p010);   \
	    p011 = vis_faligndata(vis_ld_u16_i(voxels,              \
		    vis_array16(ipack2 + 0x200000800, n)), p011);   \
	    ipack2 += 0x80000000000000;                             \
	    p100 = vis_faligndata(vis_ld_u16_i(voxels,              \
		    vis_array16(ipack2, n)), p100);                 \
	    p101 = vis_faligndata(vis_ld_u16_i(voxels,              \
		    vis_array16(ipack2 + 0x800, n)), p101);         \
	    p110 = vis_faligndata(vis_ld_u16_i(voxels,              \
		    vis_array16(ipack2 + 0x200000000, n)), p110);   \
	    p111 = vis_faligndata(vis_ld_u16_i(voxels,              \
		    vis_array16(ipack2 + 0x200000800, n)), p111);   \
	    p000 = vis_faligndata(vis_ld_u16_i(voxels,              \
		    vis_array16(ipack3, n)), p000);                 \
	    p001 = vis_faligndata(vis_ld_u16_i(voxels,              \
		    vis_array16(ipack3 + 0x800, n)), p001);         \
	    p010 = vis_faligndata(vis_ld_u16_i(voxels,              \
		    vis_array16(ipack3 + 0x200000000, n)), p010);   \
	    p011 = vis_faligndata(vis_ld_u16_i(voxels,              \
		    vis_array16(ipack3 + 0x200000800, n)), p011);   \
	    ipack3 += 0x80000000000000;                             \
	    p100 = vis_faligndata(vis_ld_u16_i(voxels,              \
		    vis_array16(ipack3, n)), p100);                 \
	    p101 = vis_faligndata(vis_ld_u16_i(voxels,              \
		    vis_array16(ipack3 + 0x800, n)), p101);         \
	    p110 = vis_faligndata(vis_ld_u16_i(voxels,              \
		    vis_array16(ipack3 + 0x200000000, n)), p110);   \
	    p111 = vis_faligndata(vis_ld_u16_i(voxels,              \
		    vis_array16(ipack3 + 0x200000800, n)), p111);   \
	}

/* *********************************************************** */

#define	LOADPOINT_S16_8X_CHECK                                          \
	{                                                               \
	    mlib_s32 index;                                             \
	                                                                \
	    index = vis_array16(ipack0, n);                             \
	    p000 = vis_faligndata(vis_ld_u16_i(voxels, index), p000);   \
	    index = vis_array16(ipack0 + 0x800, n);                     \
	    p001 = vis_faligndata(vis_ld_u16_i(voxels, index), p001);   \
	    index = vis_array16(ipack0 + 0x200000000, n);               \
	    p010 = vis_faligndata(vis_ld_u16_i(voxels, index), p010);   \
	    index = vis_array16(ipack0 + 0x200000800, n);               \
	    p011 = vis_faligndata(vis_ld_u16_i(voxels, index), p011);   \
	    ipack0 += 0x80000000000000;                                 \
	    index = vis_array16(ipack0, n);                             \
	    p100 = vis_faligndata(vis_ld_u16_i(voxels, index), p100);   \
	    index = vis_array16(ipack0 + 0x800, n);                     \
	    p101 = vis_faligndata(vis_ld_u16_i(voxels, index), p101);   \
	    index = vis_array16(ipack0 + 0x200000000, n);               \
	    p110 = vis_faligndata(vis_ld_u16_i(voxels, index), p110);   \
	    index = vis_array16(ipack0 + 0x200000800, n);               \
	    p111 = vis_faligndata(vis_ld_u16_i(voxels, index), p111);   \
	    index = vis_array16(ipack1, n);                             \
	    index = index & ((index - maxindex) >> 31);                 \
	    p000 = vis_faligndata(vis_ld_u16_i(voxels, index), p000);   \
	    index = vis_array16(ipack1 + 0x800, n);                     \
	    index = index & ((index - maxindex) >> 31);                 \
	    p001 = vis_faligndata(vis_ld_u16_i(voxels, index), p001);   \
	    index = vis_array16(ipack1 + 0x200000000, n);               \
	    index = index & ((index - maxindex) >> 31);                 \
	    p010 = vis_faligndata(vis_ld_u16_i(voxels, index), p010);   \
	    index = vis_array16(ipack1 + 0x200000800, n);               \
	    index = index & ((index - maxindex) >> 31);                 \
	    p011 = vis_faligndata(vis_ld_u16_i(voxels, index), p011);   \
	    ipack1 += 0x80000000000000;                                 \
	    index = vis_array16(ipack1, n);                             \
	    index = index & ((index - maxindex) >> 31);                 \
	    p100 = vis_faligndata(vis_ld_u16_i(voxels, index), p100);   \
	    index = vis_array16(ipack1 + 0x800, n);                     \
	    index = index & ((index - maxindex) >> 31);                 \
	    p101 = vis_faligndata(vis_ld_u16_i(voxels, index), p101);   \
	    index = vis_array16(ipack1 + 0x200000000, n);               \
	    index = index & ((index - maxindex) >> 31);                 \
	    p110 = vis_faligndata(vis_ld_u16_i(voxels, index), p110);   \
	    index = vis_array16(ipack1 + 0x200000800, n);               \
	    index = index & ((index - maxindex) >> 31);                 \
	    p111 = vis_faligndata(vis_ld_u16_i(voxels, index), p111);   \
	    index = vis_array16(ipack2, n);                             \
	    index = index & ((index - maxindex) >> 31);                 \
	    p000 = vis_faligndata(vis_ld_u16_i(voxels, index), p000);   \
	    index = vis_array16(ipack2 + 0x800, n);                     \
	    index = index & ((index - maxindex) >> 31);                 \
	    p001 = vis_faligndata(vis_ld_u16_i(voxels, index), p001);   \
	    index = vis_array16(ipack2 + 0x200000000, n);               \
	    index = index & ((index - maxindex) >> 31);                 \
	    p010 = vis_faligndata(vis_ld_u16_i(voxels, index), p010);   \
	    index = vis_array16(ipack2 + 0x200000800, n);               \
	    index = index & ((index - maxindex) >> 31);                 \
	    p011 = vis_faligndata(vis_ld_u16_i(voxels, index), p011);   \
	    ipack2 += 0x80000000000000;                                 \
	    index = vis_array16(ipack2, n);                             \
	    index = index & ((index - maxindex) >> 31);                 \
	    p100 = vis_faligndata(vis_ld_u16_i(voxels, index), p100);   \
	    index = vis_array16(ipack2 + 0x800, n);                     \
	    index = index & ((index - maxindex) >> 31);                 \
	    p101 = vis_faligndata(vis_ld_u16_i(voxels, index), p101);   \
	    index = vis_array16(ipack2 + 0x200000000, n);               \
	    index = index & ((index - maxindex) >> 31);                 \
	    p110 = vis_faligndata(vis_ld_u16_i(voxels, index), p110);   \
	    index = vis_array16(ipack2 + 0x200000800, n);               \
	    index = index & ((index - maxindex) >> 31);                 \
	    p111 = vis_faligndata(vis_ld_u16_i(voxels, index), p111);   \
	    index = vis_array16(ipack3, n);                             \
	    index = index & ((index - maxindex) >> 31);                 \
	    p000 = vis_faligndata(vis_ld_u16_i(voxels, index), p000);   \
	    index = vis_array16(ipack3 + 0x800, n);                     \
	    index = index & ((index - maxindex) >> 31);                 \
	    p001 = vis_faligndata(vis_ld_u16_i(voxels, index), p001);   \
	    index = vis_array16(ipack3 + 0x200000000, n);               \
	    index = index & ((index - maxindex) >> 31);                 \
	    p010 = vis_faligndata(vis_ld_u16_i(voxels, index), p010);   \
	    index = vis_array16(ipack3 + 0x200000800, n);               \
	    index = index & ((index - maxindex) >> 31);                 \
	    p011 = vis_faligndata(vis_ld_u16_i(voxels, index), p011);   \
	    ipack3 += 0x80000000000000;                                 \
	    index = vis_array16(ipack3, n);                             \
	    index = index & ((index - maxindex) >> 31);                 \
	    p100 = vis_faligndata(vis_ld_u16_i(voxels, index), p100);   \
	    index = vis_array16(ipack3 + 0x800, n);                     \
	    index = index & ((index - maxindex) >> 31);                 \
	    p101 = vis_faligndata(vis_ld_u16_i(voxels, index), p101);   \
	    index = vis_array16(ipack3 + 0x200000000, n);               \
	    index = index & ((index - maxindex) >> 31);                 \
	    p110 = vis_faligndata(vis_ld_u16_i(voxels, index), p110);   \
	    index = vis_array16(ipack3 + 0x200000800, n);               \
	    index = index & ((index - maxindex) >> 31);                 \
	    p111 = vis_faligndata(vis_ld_u16_i(voxels, index), p111);   \
	}

/* *********************************************************** */

mlib_status
__mlib_VolumeRayCast_Blocked_Parallel_Trilinear_S16_S16(
	mlib_rays *rays,
	const mlib_blkvolume *blk,
	void *buffer)
{
	const mlib_d64 done = vis_to_double_dup(0x7fff7fff);
	mlib_s16 *voxels;
	mlib_s32 xysize, zsize, nrays, nsteps;
	mlib_u32 n;
	mlib_s32 qdx, qdy, qdz, qdx15, qdy15, qdz15;
	mlib_u32 qx, qy, qz, qx15, qy15, qz15;
	mlib_d64 dxplus, dyplus, dzplus;
	mlib_s32 i, j, k;

	if (!rays || !blk)
		return (MLIB_NULLPOINTER);

	voxels = (mlib_s16 *)blk->voxels;
	xysize = blk->xysize, zsize = blk->zsize;
	nrays = rays->nrays;

	vis_write_gsr((2 << 3) + 6);

	i = xysize >> 6;
	n = 0;
	while (i >>= 1)
		n++;

	qdx = (mlib_s32)(rays->incs[0][0] * 4194304.);
	qdy = (mlib_s32)(rays->incs[0][1] * 4194304.);
	qdz = (mlib_s32)(rays->incs[0][2] * 4194304.);
	qdx15 = qdx >> 7;
	qdy15 = qdy >> 7;
	qdz15 = qdz >> 7;

	dxplus = vis_to_double(((qdx15 + qdx15 + qdx15) << 16) | ((qdx15 +
		qdx15) & 0x7fff), qdx15 << 16);
	dyplus = vis_to_double(((qdy15 + qdy15 + qdy15) << 16) | ((qdy15 +
		qdy15) & 0x7fff), qdy15 << 16);
	dzplus = vis_to_double(((qdz15 + qdz15 + qdz15) << 16) | ((qdz15 +
		qdz15) & 0x7fff), qdz15 << 16);

	for (i = 0; i < nrays; i++) {
		mlib_d64 ddx, ddy, ddz, dmx, dmy, dmz;
		mlib_d64 p000, p001, p010, p011, p100, p101, p110, p111;
		mlib_u64 ipack0, ipack1;
		mlib_u64 ipack2, ipack3;

		if ((nsteps = rays->nsteps[i]) <= 0)
			continue;

		qx = (mlib_u32)(mlib_s32)(rays->starts[i][0] * 1048576.) + 15;
		qy = (mlib_u32)(mlib_s32)(rays->starts[i][1] * 1048576.) + 15;
		qz = (mlib_u32)(mlib_s32)(rays->starts[i][2] * 1048576.) + 15;

		qx15 = qx >> 5;
		qy15 = qy >> 5;
		qz15 = qz >> 5;

		ddx = vis_to_double_dup((qx15 << 16) | (qx15 & 0x7fff));
		ddy = vis_to_double_dup((qy15 << 16) | (qy15 & 0x7fff));
		ddz = vis_to_double_dup((qz15 << 16) | (qz15 & 0x7fff));
		ddx = vis_fpadd16(ddx, dxplus);
		ddy = vis_fpadd16(ddy, dyplus);
		ddz = vis_fpadd16(ddz, dzplus);
		ddx = vis_fand(done, ddx);
		dmx = vis_fpsub16(done, ddx);
		ddy = vis_fand(done, ddy);
		dmy = vis_fpsub16(done, ddy);
		ddz = vis_fand(done, ddz);
		dmz = vis_fpsub16(done, ddz);
		qx += qdx;
		qy += qdy;
		qz += qdz;
		PREPOINT(0);
		qx15 += qdx15;
		qy15 += qdy15;
		qz15 += qdz15;
		PREPOINT(1);
		qx15 += qdx15;
		qy15 += qdy15;
		qz15 += qdz15;
		PREPOINT(2);
		qx15 += qdx15;
		qy15 += qdy15;
		qz15 += qdz15;
		PREPOINT(3);

#pragma pipeloop(0)
		for (j = 0; j < (nsteps - 1) >> 2; j++) {

			LOADPOINT_S16_8X;

			p000 = vis_fpadd16(MUL16(p000, dmx), MUL16(p001, ddx));
			p010 = vis_fpadd16(MUL16(p010, dmx), MUL16(p011, ddx));
			p100 = vis_fpadd16(MUL16(p100, dmx), MUL16(p101, ddx));
			p110 = vis_fpadd16(MUL16(p110, dmx), MUL16(p111, ddx));
			p000 = vis_fpadd16(p000, p000);
			p010 = vis_fpadd16(p010, p010);
			p100 = vis_fpadd16(p100, p100);
			p110 = vis_fpadd16(p110, p110);

			p000 = vis_fpadd16(MUL16(p000, dmy), MUL16(p010, ddy));
			p100 = vis_fpadd16(MUL16(p100, dmy), MUL16(p110, ddy));
			p000 = vis_fpadd16(p000, p000);
			p100 = vis_fpadd16(p100, p100);

			p000 = vis_fpadd16(MUL16(p000, dmz), MUL16(p100, ddz));
			p000 = vis_fpadd16(p000, p000);

			qx15 = qx >> 5;
			qy15 = qy >> 5;
			qz15 = qz >> 5;
			qx += qdx;
			qy += qdy;
			qz += qdz;
			ddx = vis_to_double_dup((qx15 << 16) | (qx15 & 0x7fff));
			ddy = vis_to_double_dup((qy15 << 16) | (qy15 & 0x7fff));
			ddz = vis_to_double_dup((qz15 << 16) | (qz15 & 0x7fff));
			ddx = vis_fpadd16(ddx, dxplus);
			ddy = vis_fpadd16(ddy, dyplus);
			ddz = vis_fpadd16(ddz, dzplus);
			ddx = vis_fand(done, ddx);
			dmx = vis_fpsub16(done, ddx);
			ddy = vis_fand(done, ddy);
			dmy = vis_fpsub16(done, ddy);
			ddz = vis_fand(done, ddz);
			dmz = vis_fpsub16(done, ddz);
			PREPOINT(0);
			qx15 += qdx15;
			qy15 += qdy15;
			qz15 += qdz15;
			PREPOINT(1);
			qx15 += qdx15;
			qy15 += qdy15;
			qz15 += qdz15;
			PREPOINT(2);
			qx15 += qdx15;
			qy15 += qdy15;
			qz15 += qdz15;
			PREPOINT(3);

			vis_st_u16_i(p000, rays->results[4 * j + 0], i << 1);
			p000 = vis_faligndata(p000, p000);
			vis_st_u16_i(p000, rays->results[4 * j + 1], i << 1);
			p000 = vis_faligndata(p000, p000);
			vis_st_u16_i(p000, rays->results[4 * j + 2], i << 1);
			p000 = vis_faligndata(p000, p000);
			vis_st_u16_i(p000, rays->results[4 * j + 3], i << 1);
		}

		{
			mlib_s32 tail = nsteps - j * 4;
			mlib_s32 maxindex = xysize * xysize * zsize * 2;

			LOADPOINT_S16_8X_CHECK;

			p000 = vis_fpadd16(MUL16(p000, dmx), MUL16(p001, ddx));
			p010 = vis_fpadd16(MUL16(p010, dmx), MUL16(p011, ddx));
			p100 = vis_fpadd16(MUL16(p100, dmx), MUL16(p101, ddx));
			p110 = vis_fpadd16(MUL16(p110, dmx), MUL16(p111, ddx));
			p000 = vis_fpadd16(p000, p000);
			p010 = vis_fpadd16(p010, p010);
			p100 = vis_fpadd16(p100, p100);
			p110 = vis_fpadd16(p110, p110);

			p000 = vis_fpadd16(MUL16(p000, dmy), MUL16(p010, ddy));
			p100 = vis_fpadd16(MUL16(p100, dmy), MUL16(p110, ddy));
			p000 = vis_fpadd16(p000, p000);
			p100 = vis_fpadd16(p100, p100);

			p000 = vis_fpadd16(MUL16(p000, dmz), MUL16(p100, ddz));
			p000 = vis_fpadd16(p000, p000);

			vis_st_u16_i(p000, rays->results[4 * j], i << 1);
			for (k = 1; k < tail; ++k) {
				p000 = vis_faligndata(p000, p000);
				vis_st_u16_i(p000, rays->results[4 * j + k],
					i << 1);
			}
		}
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
