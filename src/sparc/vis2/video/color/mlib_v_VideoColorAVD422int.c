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

#pragma ident	"@(#)mlib_v_VideoColorAVD422int.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorYUV420seq_to_YUYV422int - convert YUV sequential to
 *                                               interleaved
 *
 * SYNOPSIS
 *
 *      void mlib_VideoColorYUV420seq_to_YUYV422int(mlib_u32      *yuyv,
 *                                                  const mlib_u8 *y,
 *                                                  const mlib_u8 *u,
 *                                                  const mlib_u8 *v,
 *                                                  mlib_s32      w,
 *                                                  mlib_s32      h,
 *                                                  mlib_s32      dlb,
 *                                                  mlib_s32      ylb,
 *                                                  mlib_s32      uvlb);
 * ARGUMENTS
 *      yuyv        pointer to output buffer
 *      y           pointer to Y input buffer
 *      u           pointer to U input buffer
 *      v           pointer to V input buffer
 *      width       image width in pixels
 *      height      image height in lines
 *      dst_stride  linebytes for YUYV buffer
 *      y_stride    linebytes for Y buffer
 *      uv_stride   linebytes for U and V buffers
 *
 * DESCRIPTION
 *      The Y, U, V pixel streams are combined into a YUYV pixel stream. All
 *      pixel components are 8-bit unsigned integers. The Y buffer has
 *      dimensions width and height. The U and V buffers have dimensions
 *      width/2 and height/2.
 *      Dimensions width and height are assumed to be even. Successive row
 *      of the output buffer YUYV use successive rows of Y and the same rows
 *      of U and V.
 *
 *      For r = 0, 2, 4, ..., height - 2 and c = 0, 2, 4, ..., width - 2:
 *         YUYV[r][c/2] = (Y[r][c]  << 24) | (U[r/2][c/2] << 16) |
 *                        (Y[r][c+1] << 8) | (V[r/2][c/2])
 *
 *         YUYV[r+1][c/2] = (Y[r+1][c]  << 24) | (U[r/2][c/2] << 16) |
 *                          (Y[r+1][c+1] << 8) | (V[r/2][c/2])
 */

/*
 * FUNCTION
 *      mlib_VideoColorYUV411seq_to_YUYV422int - convert YUV sequential to
 *                                               interleaved
 *
 * SYNOPSIS
 *
 *      void mlib_VideoColorYUV411seq_to_YUYV422int(mlib_u32      *yuyv,
 *                                                  const mlib_u8 *y,
 *                                                  const mlib_u8 *u,
 *                                                  const mlib_u8 *v,
 *                                                  mlib_s32      w,
 *                                                  mlib_s32      h,
 *                                                  mlib_s32      dlb,
 *                                                  mlib_s32      ylb,
 *                                                  mlib_s32      uvlb);
 *
 * ARGUMENTS
 *      yuyv        pointer to output buffer
 *      y           pointer to Y input buffer
 *      u           pointer to U input buffer
 *      v           pointer to V input buffer
 *      width       image width in pixels
 *      height      image height in lines
 *      dst_stride  linebytes for YUYV buffer
 *      y_stride    linebytes for Y buffer
 *      uv_stride   linebytes for U and V buffers
 *
 * DESCRIPTION
 *      The Y, U, V pixel streams are combined into a YUYV pixel stream. All
 *      pixel components are 8-bit unsigned integers. The Y buffer has
 *      dimensions width and height. The U and V buffers have dimensions
 *      width/4 and height.
 *      Dimension width is assumed to be a multiple of 4. In each row, every
 *      4 Y values use the same U and V values.
 *
 *      For r = 0, 1, 2, ..., height - 1 and c = 0, 4, 8, ..., width - 4:
 *         YUYV[r][c/2] = (Y[r][c]  << 24) | (U[r][c/4] << 16) |
 *                        (Y[r][c+1] << 8) | (V[r][c/4])
 *
 *         YUYV[r][c/2+1] = (Y[r][c+2] << 24) | (U[r][c/4] << 16) |
 *                          (Y[r][c+3] <<  8) | (V[r][c/4])
 */

/*
 * FUNCTION
 *      mlib_VideoColorYUV422seq_to_YUYV422int - convert YUV sequential to
 *                                               interleaved
 *
 * SYNOPSIS
 *
 *      void mlib_VideoColorYUV422seq_to_YUYV422int(mlib_u32      *yuyv,
 *                                                  const mlib_u8 *y,
 *                                                  const mlib_u8 *u,
 *                                                  const mlib_u8 *v,
 *                                                  mlib_s32      w,
 *                                                  mlib_s32      h,
 *                                                  mlib_s32      dlb,
 *                                                  mlib_s32      ylb,
 *                                                  mlib_s32      uvlb);
 *
 * ARGUMENTS
 *      yuyv        pointer to output buffer
 *      y           pointer to Y input buffer
 *      u           pointer to U input buffer
 *      v           pointer to V input buffer
 *      width       image width in pixels
 *      height      image height in lines
 *      dst_stride  linebytes for YUYV buffer
 *      y_stride    linebytes for Y buffer
 *      uv_stride   linebytes for U and V buffers
 *
 * DESCRIPTION
 *      The Y, U, V pixel streams are combined into a YUYV pixel stream. All
 *      pixel components are 8-bit unsigned integers. The Y buffer has
 *      dimensions width and height. The U and V buffers have dimensions
 *      width / 2 and height.
 *      Dimension width is assumed to be even. Similar to
 *      mlib_VideoColorYUV420seq_to_YUYV422int except U and V are not sampled
 *      in the height direction.
 *
 *      For r = 0, 1, 2, ..., height - 1 and c = 0, 2, 4, ..., width - 2:
 *         YUYV[r][c/2] = (Y[r][c]  << 24) | (U[r][c/2] << 16) |
 *                        (Y[r][c+1] << 8) | (V[r][c/2])
 *
 */

/*
 * FUNCTION
 *      mlib_VideoColorYUV444seq_to_YUYV422int - convert YUV sequential to
 *                                               interleaved with subsampling
 *
 * SYNOPSIS
 *
 *      void mlib_VideoColorYUV444seq_to_YUYV422int(mlib_u32      *yuyv,
 *                                                  const mlib_u8 *y,
 *                                                  const mlib_u8 *u,
 *                                                  const mlib_u8 *v,
 *                                                  mlib_s32      w,
 *                                                  mlib_s32      h,
 *                                                  mlib_s32      dlb,
 *                                                  mlib_s32      slb);
 *
 * ARGUMENTS
 *      yuyv        pointer to output buffer
 *      y           pointer to Y input buffer
 *      u           pointer to U input buffer
 *      v           pointer to V input buffer
 *      width       image width in pixels
 *      height      image height in lines
 *      dst_stride  linebytes for output buffer
 *      src_stride  linebytes for input buffers
 *
 * DESCRIPTION
 *      The Y, U, V pixel streams are combined into a YUYV pixel stream. All
 *      pixel components are 8-bit unsigned integers. The Y, U, and V buffers
 *      have dimensions width and height. Dimension width is assumed to be a
 *      multiple of 2.
 *      Adjacent U and V values are averaged to get the output U and V values.
 *
 *      For r = 0, 1, 2, ..., height - 1 and c = 0, 2, 4, ..., width - 2:
 *         YUYV[r][c/2] = (Y[r][c]  << 24) | (((U[r][c] + U[r][c+1]) / 2) << 16)
 *                      | (Y[r][c+1] << 8) | (((V[r][c] + V[r][c+1]) / 2))
 */

/*
 * FUNCTION
 *      mlib_VideoColorYUV444int_to_YUYV422int - convert YUV interleaved with
 *                                               subsampling
 *
 * SYNOPSIS
 *
 *      void mlib_VideoColorYUV444int_to_YUYV422int(mlib_u32      *yuyv,
 *                                                  const mlib_u8 *yuv,
 *                                                  mlib_s32      w,
 *                                                  mlib_s32      h,
 *                                                  mlib_s32      dlb,
 *                                                  mlib_s32      slb);
 *
 * ARGUMENTS
 *      yuyv        pointer to output buffer
 *      yuv         pointer to input buffer
 *      width       image width in pixels
 *      height      image height in lines
 *      dst_stride  linebytes for output buffer
 *      src_stride  linebytes for input buffer
 *
 * DESCRIPTION
 *      The YUV pixel stream is broken apart and recombined into a YUYV
 *      pixel stream. All pixel components are 8-bit unsigned integers.
 *      The YUV buffer has dimensions width and height. Dimension width is
 *      assumed to be a multiple of 2. Adjacent U and V values are averaged
 *      to get the output U and V values.
 *      The sequence of values in the input stream is Y[r][c], U[r][c],
 *      V[r][c], Y[r][c+1], U[r][c+1], V[r][c+1], ...
 *
 *      For r = 0, 1, 2, ..., height - 1 and c = 0, 2, 4, ..., width - 2:
 *         YUYV[r][c/2] = (Y[r][c]  << 24) | (((U[r][c] + U[r][c+1]) / 2) << 16)
 *                      | (Y[r][c+1] << 8) | (((V[r][c] + V[r][c+1]) / 2))
 */

/*
 * FUNCTION
 *      mlib_VideoColorYUV420seq_to_UYVY422int - convert YUV sequential to
 *                                               interleaved
 *
 * SYNOPSIS
 *
 *      void mlib_VideoColorYUV420seq_to_UYVY422int(mlib_u32      *uyvy,
 *                                                  const mlib_u8 *y,
 *                                                  const mlib_u8 *u,
 *                                                  const mlib_u8 *v,
 *                                                  mlib_s32      w,
 *                                                  mlib_s32      h,
 *                                                  mlib_s32      dlb,
 *                                                  mlib_s32      ylb,
 *                                                  mlib_s32      uvlb);
 *
 * ARGUMENTS
 *      uyvy        pointer to output buffer
 *      y           pointer to Y input buffer
 *      u           pointer to U input buffer
 *      v           pointer to V input buffer
 *      width       image width in pixels
 *      height      image height in lines
 *      dst_stride  linebytes for UYVY buffer
 *      y_stride    linebytes for Y buffer
 *      uv_stride   linebytes for U and V buffers
 *
 * DESCRIPTION
 *      The Y, U, V pixel streams are combined into a UYVY pixel stream.
 *      All pixel components are 8-bit unsigned integers. The Y buffer has
 *      dimensions width and height. The U and V buffers have dimensions
 *      width/2 and height/2.
 *      Dimensions width and height are assumed to be even. Successive rows
 *      of the output buffer UYVY use successive rows of Y and the same
 *      rows of U and V.
 *
 *      For r = 0, 2, 4, ..., height - 2 and c = 0, 2, 4, ..., width - 2:
 *         UYVY[r][c/2] = (U[r/2][c/2] << 24) | (Y[r][c] << 16) |
 *                        (V[r/2][c/2] <<  8) | (Y[r][c+1])
 *
 *         UYVY[r+1][c/2] = (U[r/2][c/2] << 24) | (Y[r+1][c] << 16) |
 *                          (V[r/2][c/2] <<  8) | (Y[r+1][c+1])
 */

/*
 * FUNCTION
 *      mlib_VideoColorYUV411seq_to_UYVY422int - convert YUV sequential to
 *                                               interleaved
 *
 * SYNOPSIS
 *
 *      void mlib_VideoColorYUV411seq_to_UYVY422int(mlib_u32      *uyvy,
 *                                                  const mlib_u8 *y,
 *                                                  const mlib_u8 *u,
 *                                                  const mlib_u8 *v,
 *                                                  mlib_s32      w,
 *                                                  mlib_s32      h,
 *                                                  mlib_s32      dlb,
 *                                                  mlib_s32      ylb,
 *                                                  mlib_s32      uvlb);
 *
 * ARGUMENTS
 *      uyvy        pointer to output buffer
 *      y           pointer to Y input buffer
 *      u           pointer to U input buffer
 *      v           pointer to V input buffer
 *      width       image width in pixels
 *      height      image height in lines
 *      dst_stride  linebytes for UYVY buffer
 *      y_stride    linebytes for Y buffer
 *      uv_stride   linebytes for U and V buffers
 *
 * DESCRIPTION
 *      The Y, U, V pixel streams are combined into a UYVY pixel stream.
 *      All pixel components are 8-bit unsigned integers. The Y buffer has
 *      dimensions width and height. The U and V buffers have dimensions
 *      width/4 and height.
 *      Dimension width is assumed to be a multiple of 4. In each row,
 *      every 4 Y values use the same U and V values.
 *
 *      For r = 0, 1, 2, ..., height-1 and c = 0, 4, 8, ..., width - 4:
 *         UYVY[r][c/2] = (U[r][c/4] << 24) | (Y[r][c] << 16) |
 *                        (V[r][c/4] <<  8) | (Y[r][c+1])
 *
 *         UYVY[r][c/2+1] = (U[r][c/4] << 24) | (Y[r][c+2] << 16) |
 *                          (V[r][c/4] <<  8) | (Y[r][c+3])
 */

/*
 * FUNCTION
 *      mlib_VideoColorYUV422seq_to_UYVY422int - convert YUV sequential to
 *                                               interleaved
 *
 * SYNOPSIS
 *
 *      void mlib_VideoColorYUV422seq_to_UYVY422int(mlib_u32      *uyvy,
 *                                                  const mlib_u8 *y,
 *                                                  const mlib_u8 *u,
 *                                                  const mlib_u8 *v,
 *                                                  mlib_s32      w,
 *                                                  mlib_s32      h,
 *                                                  mlib_s32      dlb,
 *                                                  mlib_s32      ylb,
 *                                                  mlib_s32      uvlb);
 *
 * ARGUMENTS
 *      uyvy        pointer to output buffer
 *      y           pointer to Y input buffer
 *      u           pointer to U input buffer
 *      v           pointer to V input buffer
 *      width       image width in pixels
 *      height      image height in lines
 *      dst_stride  linebytes for UYVY buffer
 *      y_stride    linebytes for Y buffer
 *      uv_stride   linebytes for U and V buffers
 *
 * DESCRIPTION
 *      The Y, U, V pixel streams are combined into a UYVY pixel stream.
 *      All pixel components are 8-bit unsigned integers. The Y buffer has
 *      dimensions width and height. The U and V buffers have dimensions
 *      width/2 and height.
 *      Dimensions width is assumed to be even. Similar to
 *      mlib_VideoColorYUV420seq_to_UYVY422int except U and V are not sampled
 *      in the height direction.
 *
 *      For r = 0, 1, 2, ..., height - 1 and c = 0, 2, 4, ..., width - 2:
 *         UYVY[r][c/2] = (U[r][c/2] << 24) | (Y[r][c] << 16) |
 *                        (V[r][c/2] << 8)  | (Y[r][c+1])
 */

#include <mlib_video.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorYUV411seq_to_UYVY422int = \
	__mlib_VideoColorYUV411seq_to_UYVY422int
#pragma weak mlib_VideoColorYUV411seq_to_YUYV422int = \
	__mlib_VideoColorYUV411seq_to_YUYV422int
#pragma weak mlib_VideoColorYUV420seq_to_UYVY422int = \
	__mlib_VideoColorYUV420seq_to_UYVY422int
#pragma weak mlib_VideoColorYUV420seq_to_YUYV422int = \
	__mlib_VideoColorYUV420seq_to_YUYV422int
#pragma weak mlib_VideoColorYUV422seq_to_UYVY422int = \
	__mlib_VideoColorYUV422seq_to_UYVY422int
#pragma weak mlib_VideoColorYUV422seq_to_YUYV422int = \
	__mlib_VideoColorYUV422seq_to_YUYV422int
#pragma weak mlib_VideoColorYUV444int_to_YUYV422int = \
	__mlib_VideoColorYUV444int_to_YUYV422int
#pragma weak mlib_VideoColorYUV444seq_to_YUYV422int = \
	__mlib_VideoColorYUV444seq_to_YUYV422int

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorYUV411seq_to_UYVY422int)
	mlib_VideoColorYUV411seq_to_UYVY422int
	__attribute__((weak,
	alias("__mlib_VideoColorYUV411seq_to_UYVY422int")));
__typeof__(__mlib_VideoColorYUV411seq_to_YUYV422int)
	mlib_VideoColorYUV411seq_to_YUYV422int
	__attribute__((weak,
	alias("__mlib_VideoColorYUV411seq_to_YUYV422int")));
__typeof__(__mlib_VideoColorYUV420seq_to_UYVY422int)
	mlib_VideoColorYUV420seq_to_UYVY422int
	__attribute__((weak,
	alias("__mlib_VideoColorYUV420seq_to_UYVY422int")));
__typeof__(__mlib_VideoColorYUV420seq_to_YUYV422int)
	mlib_VideoColorYUV420seq_to_YUYV422int
	__attribute__((weak,
	alias("__mlib_VideoColorYUV420seq_to_YUYV422int")));
__typeof__(__mlib_VideoColorYUV422seq_to_UYVY422int)
	mlib_VideoColorYUV422seq_to_UYVY422int
	__attribute__((weak,
	alias("__mlib_VideoColorYUV422seq_to_UYVY422int")));
__typeof__(__mlib_VideoColorYUV422seq_to_YUYV422int)
	mlib_VideoColorYUV422seq_to_YUYV422int
	__attribute__((weak,
	alias("__mlib_VideoColorYUV422seq_to_YUYV422int")));
__typeof__(__mlib_VideoColorYUV444int_to_YUYV422int)
	mlib_VideoColorYUV444int_to_YUYV422int
	__attribute__((weak,
	alias("__mlib_VideoColorYUV444int_to_YUYV422int")));
__typeof__(__mlib_VideoColorYUV444seq_to_YUYV422int)
	mlib_VideoColorYUV444seq_to_YUYV422int
	__attribute__((weak,
	alias("__mlib_VideoColorYUV444seq_to_YUYV422int")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	VIS_FPMERGE_HI(VAL0, VAL1)                              \
	vis_fpmerge(vis_read_hi(VAL0), vis_read_hi(VAL1))

/* *********************************************************** */

#define	VIS_FPMERGE_LO(VAL0, VAL1)                              \
	vis_fpmerge(vis_read_lo(VAL0), vis_read_lo(VAL1))

/* *********************************************************** */

#define	MLIB_SPLIT3_U8(dd0, dd1, dd2, sd0, sd1, sd2)                 \
	{                                                            \
	    mlib_d64 sda, sdb, sdc, sdd, sde, sdf;                   \
	                                                             \
	    sda = vis_fpmerge(vis_read_hi(sd0), vis_read_lo(sd1));   \
	    sdb = vis_fpmerge(vis_read_lo(sd0), vis_read_hi(sd2));   \
	    sdc = vis_fpmerge(vis_read_hi(sd1), vis_read_lo(sd2));   \
	    sdd = vis_fpmerge(vis_read_hi(sda), vis_read_lo(sdb));   \
	    sdf = vis_fpmerge(vis_read_lo(sda), vis_read_hi(sdc));   \
	    sde = vis_fpmerge(vis_read_hi(sdb), vis_read_lo(sdc));   \
	    dd0 = vis_fpmerge(vis_read_hi(sdd), vis_read_lo(sdf));   \
	    dd1 = vis_fpmerge(vis_read_lo(sdd), vis_read_hi(sde));   \
	    dd2 = vis_fpmerge(vis_read_hi(sdf), vis_read_lo(sde));   \
	}

/* *********************************************************** */

#define	UNALIGNLOAD_64_NF(var, addr)                            \
	{                                                       \
	    mlib_d64 *al_addr = vis_alignaddr(addr, 0);         \
	                                                        \
	    var = vis_faligndata(*al_addr,                      \
		vis_ld_d64_nf(al_addr + 1));                    \
	}

/* *********************************************************** */

#define	UNALIGNLOAD_TWO_64_NF(var1, var2, addr)                         \
	{                                                               \
	    mlib_d64 *al_addr = vis_alignaddr(addr, 0);                 \
	    mlib_d64 common;                                            \
	                                                                \
	    var1 = vis_faligndata(*al_addr,                             \
		common = vis_ld_d64_nf(al_addr + 1));                   \
	    var2 = vis_faligndata(common, vis_ld_d64_nf(al_addr + 2));  \
	}

/* *********************************************************** */

#define	UNALIGNLOAD_FOUR_64_NF(var1, var2, var3, var4, addr)            \
	{                                                               \
	    mlib_d64 *al_addr = vis_alignaddr(addr, 0);                 \
	    mlib_d64 common, common1, common2;                          \
	                                                                \
	    var1 = vis_faligndata(*al_addr,                             \
		common = vis_ld_d64_nf(al_addr + 1));                   \
	    var2 = vis_faligndata(common,                               \
		common1 = vis_ld_d64_nf(al_addr + 2));                  \
	    var3 = vis_faligndata(common1,                              \
		common2 = vis_ld_d64_nf(al_addr + 3));                  \
	    var4 = vis_faligndata(common2, vis_ld_d64_nf(al_addr + 4)); \
	}

/* *********************************************************** */

#define	STORE_MIDDLE(addr, var1, var2)                          \
	* (mlib_d64 *)(addr) =                                  \
	vis_freg_pair(vis_read_lo(var1), vis_read_hi(var2))

/* *********************************************************** */

void
__mlib_VideoColorYUV444seq_to_YUYV422int(
	mlib_u32 *yuyv,
	const mlib_u8 *y,
	const mlib_u8 *u,
	const mlib_u8 *v,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb)
{
	mlib_s32 i, j, k, val_y0, val_y1, val_u0, val_v0;
	mlib_f32 v_one = vis_to_float(0x1000000);

	dlb >>= 2;
	w >>= 1;

	if (w == 0 || h == 0)
		return;

	vis_write_gsr(6 << 3);

	for (i = 0; i < h; i++, y += slb, u += slb, v += slb, yuyv += dlb) {
		if ((((mlib_addr)u | (mlib_addr)v | (mlib_addr)y | (mlib_addr)
			yuyv) & 7)
			== 0) {
			mlib_d64 w_y, w_u, w_v, w_uv, w_tmp0, w_acc0, w_acc1;
			mlib_f32 v_u, v_v;

			vis_write_bmask(0x02461357, 0);
#pragma pipeloop(0)
			for (j = 0; j < w >> 2; j++) {
				w_y = ((mlib_d64 *)y)[j];
				w_u = ((mlib_d64 *)u)[j];
				w_v = ((mlib_d64 *)v)[j];

				w_tmp0 = vis_bshuffle(w_u, w_u);
				w_acc0 = vis_fmul8x16au(vis_read_hi(w_tmp0),
					v_one);
				w_acc1 = vis_fmul8x16au(vis_read_lo(w_tmp0),
					v_one);
				v_u = vis_fpack16(vis_fpadd16(w_acc0, w_acc1));

				w_tmp0 = vis_bshuffle(w_v, w_v);
				w_acc0 = vis_fmul8x16au(vis_read_hi(w_tmp0),
					v_one);
				w_acc1 = vis_fmul8x16au(vis_read_lo(w_tmp0),
					v_one);
				v_v = vis_fpack16(vis_fpadd16(w_acc0, w_acc1));

				w_uv = vis_fpmerge(v_u, v_v);

				((mlib_d64 *)yuyv)[2 * j] =
					VIS_FPMERGE_HI(w_y, w_uv);
				((mlib_d64 *)yuyv)[2 * j + 1] =
					VIS_FPMERGE_LO(w_y, w_uv);
			}

			if (w & 3) {
				mlib_d64 res_buf[2];

				w_y = vis_ld_d64_nf(((mlib_d64 *)y) + j);
				w_u = vis_ld_d64_nf(((mlib_d64 *)u) + j);
				w_v = vis_ld_d64_nf(((mlib_d64 *)v) + j);

				w_tmp0 = vis_bshuffle(w_u, w_u);
				w_acc0 = vis_fmul8x16au(vis_read_hi(w_tmp0),
					v_one);
				w_acc1 = vis_fmul8x16au(vis_read_lo(w_tmp0),
					v_one);
				v_u = vis_fpack16(vis_fpadd16(w_acc0, w_acc1));

				w_tmp0 = vis_bshuffle(w_v, w_v);
				w_acc0 = vis_fmul8x16au(vis_read_hi(w_tmp0),
					v_one);
				w_acc1 = vis_fmul8x16au(vis_read_lo(w_tmp0),
					v_one);
				v_v = vis_fpack16(vis_fpadd16(w_acc0, w_acc1));

				w_uv = vis_fpmerge(v_u, v_v);

				res_buf[0] = VIS_FPMERGE_HI(w_y, w_uv);
				res_buf[1] = VIS_FPMERGE_LO(w_y, w_uv);

				for (k = 0; k < (w & 3); k++) {
					((mlib_f32 *)yuyv)[4 * j + k] =
						((mlib_f32 *)res_buf)[k];
				}
			}
		} else {
			mlib_d64 w_y, w_u, w_v, w_uv, w_y0, w_u0, w_v0, w_y1,
				w_u1, w_v1;
			mlib_d64 w_tmp0, w_acc0, w_acc1;
			mlib_f32 v_u, v_v;
			mlib_s32 w0 = w;
			mlib_d64 *dpy, *dpu, *dpv;
			const mlib_u8 *py = y, *pu = u, *pv = v;
			mlib_u32 *pyuyv = yuyv;

			if ((mlib_addr)yuyv & 7) {
				val_y0 = py[0];
				val_y1 = py[1];
				val_u0 = (pu[0] + pu[1]) >> 1;
				val_v0 = (pv[0] + pv[1]) >> 1;
				(*pyuyv++) =
					(val_y0 << 24) | (val_u0 << 16) |
					(val_y1 << 8) | val_v0;
				py += 2;
				pu += 2;
				pv += 2;
				w0--;
			}

			dpy = vis_alignaddr((void *)py, 0);
			w_y0 = vis_ld_d64_nf(dpy); dpy++;
			dpu = vis_alignaddr((void *)pu, 0);
			w_u0 = vis_ld_d64_nf(dpu); dpu++;
			dpv = vis_alignaddr((void *)pv, 0);
			w_v0 = vis_ld_d64_nf(dpv); dpv++;

			vis_write_bmask(0x02461357, 0);

#pragma pipeloop(0)
			for (j = 0; j < w0 >> 2; j++) {
				w_y1 = vis_ld_d64_nf(dpy + j);
				vis_alignaddr((void *)py, 0);
				w_y = vis_faligndata(w_y0, w_y1);
				w_y0 = w_y1;
				w_u1 = vis_ld_d64_nf(dpu + j);
				vis_alignaddr((void *)pu, 0);
				w_u = vis_faligndata(w_u0, w_u1);
				w_u0 = w_u1;
				w_v1 = vis_ld_d64_nf(dpv + j);
				vis_alignaddr((void *)pv, 0);
				w_v = vis_faligndata(w_v0, w_v1);
				w_v0 = w_v1;

				w_tmp0 = vis_bshuffle(w_u, w_u);
				w_acc0 = vis_fmul8x16au(vis_read_hi(w_tmp0),
					v_one);
				w_acc1 = vis_fmul8x16au(vis_read_lo(w_tmp0),
					v_one);
				v_u = vis_fpack16(vis_fpadd16(w_acc0, w_acc1));

				w_tmp0 = vis_bshuffle(w_v, w_v);
				w_acc0 = vis_fmul8x16au(vis_read_hi(w_tmp0),
					v_one);
				w_acc1 = vis_fmul8x16au(vis_read_lo(w_tmp0),
					v_one);
				v_v = vis_fpack16(vis_fpadd16(w_acc0, w_acc1));

				w_uv = vis_fpmerge(v_u, v_v);

				((mlib_d64 *)pyuyv)[2 * j] =
					VIS_FPMERGE_HI(w_y, w_uv);
				((mlib_d64 *)pyuyv)[2 * j + 1] =
					VIS_FPMERGE_LO(w_y, w_uv);
			}

			if (w0 & 3) {
				mlib_d64 res_buf[2];

				vis_alignaddr((void *)py, 0);
				w_y1 = vis_ld_d64_nf(dpy + j);
				w_y = vis_faligndata(w_y0, w_y1);
				vis_alignaddr((void *)pu, 0);
				w_u1 = vis_ld_d64_nf(dpu + j);
				w_u = vis_faligndata(w_u0, w_u1);
				vis_alignaddr((void *)pv, 0);
				w_v1 = vis_ld_d64_nf(dpv + j);
				w_v = vis_faligndata(w_v0, w_v1);
				w_tmp0 = vis_bshuffle(w_u, w_u);
				w_acc0 = vis_fmul8x16au(vis_read_hi(w_tmp0),
					v_one);
				w_acc1 = vis_fmul8x16au(vis_read_lo(w_tmp0),
					v_one);
				v_u = vis_fpack16(vis_fpadd16(w_acc0, w_acc1));

				w_tmp0 = vis_bshuffle(w_v, w_v);
				w_acc0 = vis_fmul8x16au(vis_read_hi(w_tmp0),
					v_one);
				w_acc1 = vis_fmul8x16au(vis_read_lo(w_tmp0),
					v_one);
				v_v = vis_fpack16(vis_fpadd16(w_acc0, w_acc1));

				w_uv = vis_fpmerge(v_u, v_v);

				res_buf[0] = VIS_FPMERGE_HI(w_y, w_uv);
				res_buf[1] = VIS_FPMERGE_LO(w_y, w_uv);

				for (k = 0; k < (w0 & 3); k++) {
					((mlib_f32 *)pyuyv)[4 * j + k] =
						((mlib_f32 *)res_buf)[k];
				}
			}
		}
	}
}

/* *********************************************************** */

void
__mlib_VideoColorYUV444int_to_YUYV422int(
	mlib_u32 *yuyv,
	const mlib_u8 *yuv,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb)
{
	mlib_s32 i, val_y0, val_y1, val_u0, val_v0, count, left;

	dlb >>= 2;
	w >>= 1;
	count = w >> 2;
	left = w - (count << 2);

	if (w == 0 || h == 0)
		return;

	vis_write_gsr(6 << 3);
	vis_write_bmask(0x02461357, 0);

	for (i = 0; i < h; i++, yuv += slb, yuyv += dlb) {
		if ((((mlib_addr)yuv | (mlib_addr)yuyv) & 7) == 0) {
			mlib_d64 w_y, w_u, w_v, w_uv, w_tmp0, w_acc0, w_acc1;
			mlib_d64 w_ld0, w_ld1, w_ld2;
			mlib_f32 v_one = vis_to_float(0x1000000);
			mlib_f32 v_u, v_v;
			mlib_s32 j;

#pragma pipeloop(0)
			for (j = 0; j < count; j++) {
				w_ld0 = ((mlib_d64 *)yuv)[3 * j];
				w_ld1 = ((mlib_d64 *)yuv)[3 * j + 1];
				w_ld2 = ((mlib_d64 *)yuv)[3 * j + 2];

				MLIB_SPLIT3_U8(w_y, w_u, w_v, w_ld0, w_ld1,
					w_ld2);

				w_tmp0 = vis_bshuffle(w_u, w_u);
				w_acc0 = vis_fmul8x16au(vis_read_hi(w_tmp0),
					v_one);
				w_acc1 = vis_fmul8x16au(vis_read_lo(w_tmp0),
					v_one);
				v_u = vis_fpack16(vis_fpadd16(w_acc0, w_acc1));

				w_tmp0 = vis_bshuffle(w_v, w_v);
				w_acc0 = vis_fmul8x16au(vis_read_hi(w_tmp0),
					v_one);
				w_acc1 = vis_fmul8x16au(vis_read_lo(w_tmp0),
					v_one);
				v_v = vis_fpack16(vis_fpadd16(w_acc0, w_acc1));
				w_uv = vis_fpmerge(v_u, v_v);

				((mlib_d64 *)yuyv)[2 * j] =
					VIS_FPMERGE_HI(w_y, w_uv);
				((mlib_d64 *)yuyv)[2 * j + 1] =
					VIS_FPMERGE_LO(w_y, w_uv);
			}

			if (left) {
				mlib_d64 res_buf[2];

				w_ld0 = vis_ld_d64_nf((mlib_d64 *)yuv +
					3 * count);
				w_ld1 = vis_ld_d64_nf((mlib_d64 *)yuv +
					3 * count + 1);
				w_ld2 = vis_ld_d64_nf((mlib_d64 *)yuv +
					3 * count + 2);

				MLIB_SPLIT3_U8(w_y, w_u, w_v, w_ld0, w_ld1,
					w_ld2);

				w_tmp0 = vis_bshuffle(w_u, w_u);
				w_acc0 = vis_fmul8x16au(vis_read_hi(w_tmp0),
					v_one);
				w_acc1 = vis_fmul8x16au(vis_read_lo(w_tmp0),
					v_one);
				v_u = vis_fpack16(vis_fpadd16(w_acc0, w_acc1));

				w_tmp0 = vis_bshuffle(w_v, w_v);
				w_acc0 = vis_fmul8x16au(vis_read_hi(w_tmp0),
					v_one);
				w_acc1 = vis_fmul8x16au(vis_read_lo(w_tmp0),
					v_one);
				v_v = vis_fpack16(vis_fpadd16(w_acc0, w_acc1));
				w_uv = vis_fpmerge(v_u, v_v);

				res_buf[0] = VIS_FPMERGE_HI(w_y, w_uv);
				res_buf[1] = VIS_FPMERGE_LO(w_y, w_uv);

				for (j = 0; j < left; j++) {
					((mlib_f32 *)yuyv)[4 * count + j] =
						((mlib_f32 *)res_buf)[j];
				}
			}
		} else {
			mlib_d64 w_y, w_u, w_v, w_uv, w_tmp0, w_acc0, w_acc1;
			mlib_d64 w_ld0, w_ld1, w_ld2;
			mlib_f32 v_one = vis_to_float(0x1000000);
			mlib_f32 v_u, v_v;
			mlib_s32 j;
			mlib_d64 *al_addr;
			mlib_d64 l0, l1, l2, l3;
			mlib_u8 *pyuv = (void *)yuv;
			mlib_u32 *pyuyv = (void *)yuyv;

			if ((mlib_addr)pyuyv & 7) {
				val_y0 = yuv[0];
				val_y1 = yuv[3];
				val_u0 = (yuv[1] + yuv[4]) >> 1;
				val_v0 = (yuv[2] + yuv[5]) >> 1;
				pyuyv[0] =
					(val_y0 << 24) | (val_u0 << 16) |
					(val_y1 << 8) | val_v0;
				pyuv += 6;
				pyuyv++;
				count = (w - 1) >> 2;
				left = (w - 1) - (count << 2);
			} else {
				count = w >> 2;
				left = w - (count << 2);
			}

			al_addr = vis_alignaddr(pyuv, 0);
			l0 = vis_ld_d64_nf(al_addr); al_addr++;

#pragma pipeloop(0)
			for (j = 0; j < count; j++) {
				l1 = (*al_addr++);
				l2 = (*al_addr++);
				l3 = vis_ld_d64_nf(al_addr); al_addr++;
				w_ld0 = vis_faligndata(l0, l1);
				w_ld1 = vis_faligndata(l1, l2);
				w_ld2 = vis_faligndata(l2, l3);
				l0 = l3;
				MLIB_SPLIT3_U8(w_y, w_u, w_v, w_ld0, w_ld1,
					w_ld2);

				w_tmp0 = vis_bshuffle(w_u, w_u);
				w_acc0 = vis_fmul8x16au(vis_read_hi(w_tmp0),
					v_one);
				w_acc1 = vis_fmul8x16au(vis_read_lo(w_tmp0),
					v_one);
				v_u = vis_fpack16(vis_fpadd16(w_acc0, w_acc1));

				w_tmp0 = vis_bshuffle(w_v, w_v);
				w_acc0 = vis_fmul8x16au(vis_read_hi(w_tmp0),
					v_one);
				w_acc1 = vis_fmul8x16au(vis_read_lo(w_tmp0),
					v_one);
				v_v = vis_fpack16(vis_fpadd16(w_acc0, w_acc1));
				w_uv = vis_fpmerge(v_u, v_v);

				((mlib_d64 *)pyuyv)[2 * j] =
					VIS_FPMERGE_HI(w_y, w_uv);
				((mlib_d64 *)pyuyv)[2 * j + 1] =
					VIS_FPMERGE_LO(w_y, w_uv);
			}

			if (left) {
				mlib_d64 res_buf[2];

				l1 = vis_ld_d64_nf(al_addr); al_addr++;
				l2 = vis_ld_d64_nf(al_addr); al_addr++;
				l3 = vis_ld_d64_nf(al_addr); al_addr++;
				w_ld0 = vis_faligndata(l0, l1);
				w_ld1 = vis_faligndata(l1, l2);
				w_ld2 = vis_faligndata(l2, l3);

				MLIB_SPLIT3_U8(w_y, w_u, w_v, w_ld0, w_ld1,
					w_ld2);

				w_tmp0 = vis_bshuffle(w_u, w_u);
				w_acc0 = vis_fmul8x16au(vis_read_hi(w_tmp0),
					v_one);
				w_acc1 = vis_fmul8x16au(vis_read_lo(w_tmp0),
					v_one);
				v_u = vis_fpack16(vis_fpadd16(w_acc0, w_acc1));

				w_tmp0 = vis_bshuffle(w_v, w_v);
				w_acc0 = vis_fmul8x16au(vis_read_hi(w_tmp0),
					v_one);
				w_acc1 = vis_fmul8x16au(vis_read_lo(w_tmp0),
					v_one);
				v_v = vis_fpack16(vis_fpadd16(w_acc0, w_acc1));
				w_uv = vis_fpmerge(v_u, v_v);

				res_buf[0] = VIS_FPMERGE_HI(w_y, w_uv);
				res_buf[1] = VIS_FPMERGE_LO(w_y, w_uv);

				for (j = 0; j < left; j++) {
					((mlib_f32 *)pyuyv)[4 * count + j] =
						((mlib_f32 *)res_buf)[j];
				}
			}

			count = w >> 2;
			left = w - (count << 2);
		}
	}
}

/* *********************************************************** */

void
__mlib_VideoColorYUV422seq_to_YUYV422int(
	mlib_u32 *yuyv,
	const mlib_u8 *y,
	const mlib_u8 *u,
	const mlib_u8 *v,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 ylb,
	mlib_s32 uvlb)
{
	mlib_s32 i, count, left;

	dlb >>= 2;
	w >>= 1;
	count = w >> 3;
	left = w - (count << 3);

	if (w == 0 || h == 0)
		return;

	for (i = 0; i < h; i++, y += ylb, u += uvlb, v += uvlb, yuyv += dlb) {
		if ((((mlib_addr)u | (mlib_addr)v | (mlib_addr)y | (mlib_addr)
			yuyv) & 7)
			== 0) {
			mlib_d64 w_uv0, w_uv1, w_y0, w_y1, w_u, w_v;
			mlib_s32 j;

#pragma pipeloop(0)
			for (j = 0; j < count; j++) {
				w_u = ((mlib_d64 *)u)[j];
				w_v = ((mlib_d64 *)v)[j];
				w_y0 = ((mlib_d64 *)y)[2 * j];
				w_y1 = ((mlib_d64 *)y)[2 * j + 1];
				w_uv0 = VIS_FPMERGE_HI(w_u, w_v);
				w_uv1 = VIS_FPMERGE_LO(w_u, w_v);
				((mlib_d64 *)yuyv)[4 * j] =
					VIS_FPMERGE_HI(w_y0, w_uv0);
				((mlib_d64 *)yuyv)[4 * j + 1] =
					VIS_FPMERGE_LO(w_y0, w_uv0);
				((mlib_d64 *)yuyv)[4 * j + 2] =
					VIS_FPMERGE_HI(w_y1, w_uv1);
				((mlib_d64 *)yuyv)[4 * j + 3] =
					VIS_FPMERGE_LO(w_y1, w_uv1);
			}

			if (left) {
				mlib_d64 res_buf[4];

				w_u = vis_ld_d64_nf((mlib_d64 *)u + j);
				w_v = vis_ld_d64_nf((mlib_d64 *)v + j);
				w_y0 = vis_ld_d64_nf((mlib_d64 *)y + 2 * j);
				w_y1 = vis_ld_d64_nf((mlib_d64 *)y + 2 * j + 1);
				w_uv0 = VIS_FPMERGE_HI(w_u, w_v);
				w_uv1 = VIS_FPMERGE_LO(w_u, w_v);
				res_buf[0] = VIS_FPMERGE_HI(w_y0, w_uv0);
				res_buf[1] = VIS_FPMERGE_LO(w_y0, w_uv0);
				res_buf[2] = VIS_FPMERGE_HI(w_y1, w_uv1);
				res_buf[3] = VIS_FPMERGE_LO(w_y1, w_uv1);

				for (j = 0; j < left; j++) {
					((mlib_f32 *)yuyv)[8 * count + j] =
						((mlib_f32 *)res_buf)[j];
				}
			}
		} else {
			mlib_d64 w_uv0, w_uv1, w_y0, w_y1, w_y2, w_y3, w_u0,
				w_v0, w_u1, w_v1;
			mlib_s32 j, off;

			const mlib_u8 *pu = u, *py = y, *pv = v;
			mlib_u32 *pyuyv = yuyv;
			mlib_d64 ld0, ld1, ld2, *dpv;

			if ((mlib_addr)pyuyv & 7) {
				pyuyv[0] =
					(pu[0] << 16) | (pv[0]) | (py[0] << 24)
					| (py[1] << 8);
				pyuyv++;
				pu++;
				pv++;
				py += 2;
				count = (w - 1) >> 4;
				left = (w - 1) - (count << 4);
			} else {
				count = w >> 4;
				left = w - (count << 4);
			}

			off = (mlib_addr)pv & 7;
			dpv = (mlib_d64 *)(pv - off);
			vis_write_bmask(off * 0x11111111, 0x01234567);
			ld0 = vis_ld_d64_nf(dpv);
#pragma pipeloop(0)
			for (j = 0; j < count; j++) {
				UNALIGNLOAD_TWO_64_NF(w_u0, w_u1,
					(mlib_d64 *)pu + j * 2);

				ld1 = dpv[1];
				ld2 = vis_ld_d64_nf(dpv + 2);
				w_v0 = vis_bshuffle(ld0, ld1);
				w_v1 = vis_bshuffle(ld1, ld2);
				ld0 = ld2;
				dpv += 2;

				UNALIGNLOAD_FOUR_64_NF(w_y0, w_y1, w_y2, w_y3,
					(mlib_d64 *)py + j * 4);
				w_uv0 = VIS_FPMERGE_HI(w_u0, w_v0);
				w_uv1 = VIS_FPMERGE_LO(w_u0, w_v0);
				((mlib_d64 *)pyuyv)[8 * j] =
					VIS_FPMERGE_HI(w_y0, w_uv0);
				((mlib_d64 *)pyuyv)[8 * j + 1] =
					VIS_FPMERGE_LO(w_y0, w_uv0);
				((mlib_d64 *)pyuyv)[8 * j + 2] =
					VIS_FPMERGE_HI(w_y1, w_uv1);
				((mlib_d64 *)pyuyv)[8 * j + 3] =
					VIS_FPMERGE_LO(w_y1, w_uv1);
				w_uv0 = VIS_FPMERGE_HI(w_u1, w_v1);
				w_uv1 = VIS_FPMERGE_LO(w_u1, w_v1);
				((mlib_d64 *)pyuyv)[8 * j + 4] =
					VIS_FPMERGE_HI(w_y2, w_uv0);
				((mlib_d64 *)pyuyv)[8 * j + 5] =
					VIS_FPMERGE_LO(w_y2, w_uv0);
				((mlib_d64 *)pyuyv)[8 * j + 6] =
					VIS_FPMERGE_HI(w_y3, w_uv1);
				((mlib_d64 *)pyuyv)[8 * j + 7] =
					VIS_FPMERGE_LO(w_y3, w_uv1);
			}

			if (left) {
				mlib_d64 res_buf[8];

				UNALIGNLOAD_TWO_64_NF(w_u0, w_u1,
					(mlib_d64 *)pu + j * 2);

				ld1 = vis_ld_d64_nf(dpv + 1);
				ld2 = vis_ld_d64_nf(dpv + 2);
				w_v0 = vis_bshuffle(ld0, ld1);
				w_v1 = vis_bshuffle(ld1, ld2);

				UNALIGNLOAD_FOUR_64_NF(w_y0, w_y1, w_y2, w_y3,
					(mlib_d64 *)py + j * 4);

				w_uv0 = VIS_FPMERGE_HI(w_u0, w_v0);
				w_uv1 = VIS_FPMERGE_LO(w_u0, w_v0);
				res_buf[0] = VIS_FPMERGE_HI(w_y0, w_uv0);
				res_buf[1] = VIS_FPMERGE_LO(w_y0, w_uv0);
				res_buf[2] = VIS_FPMERGE_HI(w_y1, w_uv1);
				res_buf[3] = VIS_FPMERGE_LO(w_y1, w_uv1);
				w_uv0 = VIS_FPMERGE_HI(w_u1, w_v1);
				w_uv1 = VIS_FPMERGE_LO(w_u1, w_v1);
				res_buf[4] = VIS_FPMERGE_HI(w_y2, w_uv0);
				res_buf[5] = VIS_FPMERGE_LO(w_y2, w_uv0);
				res_buf[6] = VIS_FPMERGE_HI(w_y3, w_uv1);
				res_buf[7] = VIS_FPMERGE_LO(w_y3, w_uv1);
				for (j = 0; j < left; j++) {
					((mlib_f32 *)pyuyv)[16 * count + j] =
						((mlib_f32 *)res_buf)[j];
				}
			}

			count = w >> 3;
			left = w - (count << 3);
		}
	}
}

/* *********************************************************** */

void
__mlib_VideoColorYUV411seq_to_YUYV422int(
	mlib_u32 *yuyv,
	const mlib_u8 *y,
	const mlib_u8 *u,
	const mlib_u8 *v,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 ylb,
	mlib_s32 uvlb)
{
	mlib_s32 i, count, left;

	dlb >>= 2;
	w >>= 2;
	count = w >> 2;
	left = w - (count << 2);

	if (w == 0 || h == 0)
		return;

	for (i = 0; i < h; i++, y += ylb, u += uvlb, v += uvlb, yuyv += dlb) {
		if ((((((mlib_addr)u | (mlib_addr)v) & 7) | (mlib_addr)y |
			(mlib_addr)
			yuyv) & 7) == 0) {
			mlib_d64 w_y0, w_y1, w_uv0, w_uv1, w_u, w_v;
			mlib_f32 v_u, v_v;
			mlib_s32 j;

#pragma pipeloop(0)
			for (j = 0; j < count; j++) {
				v_u = ((mlib_f32 *)u)[j];
				v_v = ((mlib_f32 *)v)[j];
				w_y0 = ((mlib_d64 *)y)[2 * j];
				w_y1 = ((mlib_d64 *)y)[2 * j + 1];
				w_u = vis_fpmerge(v_u, v_u);
				w_v = vis_fpmerge(v_v, v_v);
				w_uv0 = VIS_FPMERGE_HI(w_u, w_v);
				w_uv1 = VIS_FPMERGE_LO(w_u, w_v);
				((mlib_d64 *)yuyv)[4 * j] =
					VIS_FPMERGE_HI(w_y0, w_uv0);
				((mlib_d64 *)yuyv)[4 * j + 1] =
					VIS_FPMERGE_LO(w_y0, w_uv0);
				((mlib_d64 *)yuyv)[4 * j + 2] =
					VIS_FPMERGE_HI(w_y1, w_uv1);
				((mlib_d64 *)yuyv)[4 * j + 3] =
					VIS_FPMERGE_LO(w_y1, w_uv1);
			}

			if (left) {
				mlib_d64 res_buf[4];

				v_u = vis_ld_f32_nf((mlib_f32 *)u + count);
				v_v = vis_ld_f32_nf((mlib_f32 *)v + count);
				w_y0 = vis_ld_d64_nf((mlib_d64 *)y + 2 * count);
				w_y1 = vis_ld_d64_nf((mlib_d64 *)y + 2 * count +
					1);
				w_u = vis_fpmerge(v_u, v_u);
				w_v = vis_fpmerge(v_v, v_v);
				w_uv0 = VIS_FPMERGE_HI(w_u, w_v);
				w_uv1 = VIS_FPMERGE_LO(w_u, w_v);
				res_buf[0] = VIS_FPMERGE_HI(w_y0, w_uv0);
				res_buf[1] = VIS_FPMERGE_LO(w_y0, w_uv0);
				res_buf[2] = VIS_FPMERGE_HI(w_y1, w_uv1);
				res_buf[3] = VIS_FPMERGE_LO(w_y1, w_uv1);
				for (j = 0; j < left; j++)
					((mlib_d64 *)yuyv)[4 * count + j] =
						res_buf[j];
			}
		} else {
			mlib_d64 w_y0, w_y1, w_y2, w_y3, w_uv0, w_uv1, w_u, w_v;
			mlib_d64 v_u, v_v;
			mlib_s32 j;

			count = w >> 3;
			left = w - (count << 3);

			if ((mlib_addr)yuyv & 7) {
#pragma pipeloop(0)
				for (j = 0; j < count; j++) {
					mlib_d64 res0, res1, res2, res3, res4,
						res5, res6, res7;

					UNALIGNLOAD_64_NF(v_u, (mlib_d64 *)u +
					    j);
					UNALIGNLOAD_64_NF(v_v, (mlib_d64 *)v +
					    j);
					UNALIGNLOAD_FOUR_64_NF(w_y0, w_y1, w_y2,
						w_y3, (mlib_d64 *)y + 4 * j);
					w_uv0 = vis_fpmerge(vis_read_hi(v_u),
						vis_read_hi(v_v));
					w_uv1 = vis_fpmerge(vis_read_lo(v_u),
						vis_read_lo(v_v));
					vis_write_bmask(0x08192839, 0);
					res0 = vis_bshuffle(w_y0, w_uv0);
					res4 = vis_bshuffle(w_y2, w_uv1);
					vis_write_bmask(0x4a5b6a7b, 0);
					res1 = vis_bshuffle(w_y0, w_uv0);
					res5 = vis_bshuffle(w_y2, w_uv1);
					vis_write_bmask(0x0c1d2c3d, 0);
					res2 = vis_bshuffle(w_y1, w_uv0);
					res6 = vis_bshuffle(w_y3, w_uv1);
					vis_write_bmask(0x4e5f6e7f, 0);
					res3 = vis_bshuffle(w_y1, w_uv0);
					res7 = vis_bshuffle(w_y3, w_uv1);
					((mlib_f32 *)yuyv)[16 * j] =
						vis_read_hi(res0);
					STORE_MIDDLE((mlib_f32 *)yuyv + 16 * j +
						1, res0, res1);
					STORE_MIDDLE((mlib_f32 *)yuyv + 16 * j +
						3, res1, res2);
					STORE_MIDDLE((mlib_f32 *)yuyv + 16 * j +
						5, res2, res3);
					STORE_MIDDLE((mlib_f32 *)yuyv + 16 * j +
						7, res3, res4);
					STORE_MIDDLE((mlib_f32 *)yuyv + 16 * j +
						9, res4, res5);
					STORE_MIDDLE((mlib_f32 *)yuyv + 16 * j +
						11, res5, res6);
					STORE_MIDDLE((mlib_f32 *)yuyv + 16 * j +
						13, res6, res7);
					((mlib_f32 *)yuyv)[16 * j + 15] =
						vis_read_lo(res7);
				}

				if (left) {
					mlib_d64 res_buf[8];

					UNALIGNLOAD_64_NF(v_u, (mlib_d64 *)u +
					    j);
					UNALIGNLOAD_64_NF(v_v, (mlib_d64 *)v +
					    j);
					UNALIGNLOAD_FOUR_64_NF(w_y0, w_y1, w_y2,
						w_y3, (mlib_d64 *)y + 4 * j);

					w_uv0 = vis_fpmerge(vis_read_hi(v_u),
						vis_read_hi(v_v));
					w_uv1 = vis_fpmerge(vis_read_lo(v_u),
						vis_read_lo(v_v));
					vis_write_bmask(0x08192839, 0);
					res_buf[0] = vis_bshuffle(w_y0, w_uv0);
					res_buf[4] = vis_bshuffle(w_y2, w_uv1);
					vis_write_bmask(0x4a5b6a7b, 0);
					res_buf[1] = vis_bshuffle(w_y0, w_uv0);
					res_buf[5] = vis_bshuffle(w_y2, w_uv1);
					vis_write_bmask(0x0c1d2c3d, 0);
					res_buf[2] = vis_bshuffle(w_y1, w_uv0);
					res_buf[6] = vis_bshuffle(w_y3, w_uv1);
					vis_write_bmask(0x4e5f6e7f, 0);
					res_buf[3] = vis_bshuffle(w_y1, w_uv0);
					res_buf[7] = vis_bshuffle(w_y3, w_uv1);

#pragma pipeloop(0)
					for (j = 0; j < left * 2; j++)
						((mlib_f32 *)yuyv)[16 * count +
							j] =
							((mlib_f32 *)
							res_buf)[j];
				}
			} else {
#pragma pipeloop(0)
				for (j = 0; j < count; j++) {
					UNALIGNLOAD_64_NF(v_u, (mlib_d64 *)u +
					    j);
					UNALIGNLOAD_64_NF(v_v, (mlib_d64 *)v +
					    j);
					UNALIGNLOAD_FOUR_64_NF(w_y0, w_y1, w_y2,
						w_y3, (mlib_d64 *)y + 4 * j);
					w_u = vis_fpmerge(vis_read_hi(v_u),
						vis_read_hi(v_u));
					w_v = vis_fpmerge(vis_read_hi(v_v),
						vis_read_hi(v_v));
					w_uv0 = VIS_FPMERGE_HI(w_u, w_v);
					w_uv1 = VIS_FPMERGE_LO(w_u, w_v);
					((mlib_d64 *)yuyv)[8 * j] =
						VIS_FPMERGE_HI(w_y0, w_uv0);
					((mlib_d64 *)yuyv)[8 * j + 1] =
						VIS_FPMERGE_LO(w_y0, w_uv0);
					((mlib_d64 *)yuyv)[8 * j + 2] =
						VIS_FPMERGE_HI(w_y1, w_uv1);
					((mlib_d64 *)yuyv)[8 * j + 3] =
						VIS_FPMERGE_LO(w_y1, w_uv1);

					w_u = vis_fpmerge(vis_read_lo(v_u),
						vis_read_lo(v_u));
					w_v = vis_fpmerge(vis_read_lo(v_v),
						vis_read_lo(v_v));
					w_uv0 = VIS_FPMERGE_HI(w_u, w_v);
					w_uv1 = VIS_FPMERGE_LO(w_u, w_v);
					((mlib_d64 *)yuyv)[8 * j + 4] =
						VIS_FPMERGE_HI(w_y2, w_uv0);
					((mlib_d64 *)yuyv)[8 * j + 5] =
						VIS_FPMERGE_LO(w_y2, w_uv0);
					((mlib_d64 *)yuyv)[8 * j + 6] =
						VIS_FPMERGE_HI(w_y3, w_uv1);
					((mlib_d64 *)yuyv)[8 * j + 7] =
						VIS_FPMERGE_LO(w_y3, w_uv1);
				}

				if (left) {
					mlib_d64 res_buf[8];

					UNALIGNLOAD_64_NF(v_u, (mlib_d64 *)u +
					    j);
					UNALIGNLOAD_64_NF(v_v, (mlib_d64 *)v +
					    j);
					UNALIGNLOAD_FOUR_64_NF(w_y0, w_y1, w_y2,
						w_y3, (mlib_d64 *)y + 4 * j);

					w_u = vis_fpmerge(vis_read_hi(v_u),
						vis_read_hi(v_u));
					w_v = vis_fpmerge(vis_read_hi(v_v),
						vis_read_hi(v_v));
					w_uv0 = VIS_FPMERGE_HI(w_u, w_v);
					w_uv1 = VIS_FPMERGE_LO(w_u, w_v);
					res_buf[0] =
						VIS_FPMERGE_HI(w_y0, w_uv0);
					res_buf[1] =
						VIS_FPMERGE_LO(w_y0, w_uv0);
					res_buf[2] =
						VIS_FPMERGE_HI(w_y1, w_uv1);
					res_buf[3] =
						VIS_FPMERGE_LO(w_y1, w_uv1);

					w_u = vis_fpmerge(vis_read_lo(v_u),
						vis_read_lo(v_u));
					w_v = vis_fpmerge(vis_read_lo(v_v),
						vis_read_lo(v_v));
					w_uv0 = VIS_FPMERGE_HI(w_u, w_v);
					w_uv1 = VIS_FPMERGE_LO(w_u, w_v);
					res_buf[4] =
						VIS_FPMERGE_HI(w_y2, w_uv0);
					res_buf[5] =
						VIS_FPMERGE_LO(w_y2, w_uv0);
					res_buf[6] =
						VIS_FPMERGE_HI(w_y3, w_uv1);
					res_buf[7] =
						VIS_FPMERGE_LO(w_y3, w_uv1);

#pragma pipeloop(0)
					for (j = 0; j < left * 2; j++)
						((mlib_f32 *)yuyv)[16 * count +
							j] =
							((mlib_f32 *)
							res_buf)[j];
				}
			}

			count = w >> 2;
			left = w - (count << 2);
		}
	}
}

/* *********************************************************** */

void
__mlib_VideoColorYUV420seq_to_YUYV422int(
	mlib_u32 *yuyv,
	const mlib_u8 *y,
	const mlib_u8 *u,
	const mlib_u8 *v,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 ylb,
	mlib_s32 uvlb)
{
	mlib_s32 i2, count = w >> 4, left;

	w >>= 1;
	dlb >>= 2;
	left = w - (count << 3);

	if (w == 0 || h == 0)
		return;

	for (i2 = 0; i2 < h; ) {
		if ((((mlib_addr)u | (mlib_addr)v | (mlib_addr)y | (mlib_addr)
			yuyv) & 7)
			== 0) {
			mlib_d64 w_uv0, w_uv1, w_y0, w_y1;
			mlib_d64 v_u, v_v;
			mlib_s32 j;

/*
 * this loop is not the most optimal as in VIS1 version, but it
 * works faster because of the store problems on UIII
 */
#pragma pipeloop(0)
			for (j = 0; j < count; j++) {
				v_u = ((mlib_d64 *)u)[j];
				v_v = ((mlib_d64 *)v)[j];
				w_y0 = ((mlib_d64 *)y)[2 * j];
				w_y1 = ((mlib_d64 *)y)[2 * j + 1];
				w_uv0 = vis_fpmerge(vis_read_hi(v_u),
					vis_read_hi(v_v));
				w_uv1 = vis_fpmerge(vis_read_lo(v_u),
					vis_read_lo(v_v));
				((mlib_d64 *)yuyv)[4 * j] =
					VIS_FPMERGE_HI(w_y0, w_uv0);
				((mlib_d64 *)yuyv)[4 * j + 1] =
					VIS_FPMERGE_LO(w_y0, w_uv0);
				((mlib_d64 *)yuyv)[4 * j + 2] =
					VIS_FPMERGE_HI(w_y1, w_uv1);
				((mlib_d64 *)yuyv)[4 * j + 3] =
					VIS_FPMERGE_LO(w_y1, w_uv1);
			}

			if (left) {
				mlib_d64 res_buf0[4];

				v_u = vis_ld_d64_nf(((mlib_d64 *)u) + j);
				v_v = vis_ld_d64_nf(((mlib_d64 *)v) + j);
				w_y0 = vis_ld_d64_nf(((mlib_d64 *)y) + 2 * j);
				w_y1 = vis_ld_d64_nf(((mlib_d64 *)y) +
				    2 * j + 1);
				w_uv0 = vis_fpmerge(vis_read_hi(v_u),
					vis_read_hi(v_v));
				res_buf0[0] = VIS_FPMERGE_HI(w_y0, w_uv0);
				res_buf0[1] = VIS_FPMERGE_LO(w_y0, w_uv0);
				w_uv1 = vis_fpmerge(vis_read_lo(v_u),
					vis_read_lo(v_v));
				res_buf0[2] = VIS_FPMERGE_HI(w_y1, w_uv1);
				res_buf0[3] = VIS_FPMERGE_LO(w_y1, w_uv1);
				for (j = 0; j < left; j++) {
					((mlib_f32 *)yuyv)[8 * count + j] =
						((mlib_f32 *)res_buf0)[j];
				}
			}

			i2++;
			yuyv += dlb;
			y += ylb;
			u += (i2 & 1) ? 0 : uvlb;
			v += (i2 & 1) ? 0 : uvlb;
		} else {
			mlib_d64 w_uv0, w_uv1;
			mlib_d64 w_y0, w_y1, w_y2, w_y3;

			mlib_d64 v_u0, v_v0, v_u1, v_v1;
			mlib_s32 j;
			mlib_u8 *py0 = (void *)y, *pv = (void *)v, *pu =
				(void *)u;
			mlib_u32 *pyuyv0 = yuyv;

			if ((mlib_addr)pyuyv0 & 7) {
				pyuyv0[0] =
					(u[0] << 16) | v[0] | (py0[0] << 24) |
					(py0[1] << 8);
				pyuyv0++;
				py0 += 2;
				pu++;
				pv++;
				count = (w - 1) >> 4;
				left = (w - 1) - (count << 4);
			} else {
				count = w >> 4;
				left = w - (count << 4);
			}

#pragma pipeloop(0)
			for (j = 0; j < count; j++) {
				UNALIGNLOAD_TWO_64_NF(v_u0, v_u1,
					(mlib_d64 *)pu + 2 * j);
				UNALIGNLOAD_TWO_64_NF(v_v0, v_v1,
					(mlib_d64 *)pv + 2 * j);
				UNALIGNLOAD_FOUR_64_NF(w_y0, w_y1, w_y2, w_y3,
					(mlib_d64 *)py0 + 4 * j);

				w_uv0 = vis_fpmerge(vis_read_hi(v_u0),
					vis_read_hi(v_v0));
				((mlib_d64 *)pyuyv0)[8 * j] =
					VIS_FPMERGE_HI(w_y0, w_uv0);
				((mlib_d64 *)pyuyv0)[8 * j + 1] =
					VIS_FPMERGE_LO(w_y0, w_uv0);
				w_uv1 = vis_fpmerge(vis_read_lo(v_u0),
					vis_read_lo(v_v0));
				((mlib_d64 *)pyuyv0)[8 * j + 2] =
					VIS_FPMERGE_HI(w_y1, w_uv1);
				((mlib_d64 *)pyuyv0)[8 * j + 3] =
					VIS_FPMERGE_LO(w_y1, w_uv1);
				w_uv0 = vis_fpmerge(vis_read_hi(v_u1),
					vis_read_hi(v_v1));
				((mlib_d64 *)pyuyv0)[8 * j + 4] =
					VIS_FPMERGE_HI(w_y2, w_uv0);
				((mlib_d64 *)pyuyv0)[8 * j + 5] =
					VIS_FPMERGE_LO(w_y2, w_uv0);
				w_uv1 = vis_fpmerge(vis_read_lo(v_u1),
					vis_read_lo(v_v1));
				((mlib_d64 *)pyuyv0)[8 * j + 6] =
					VIS_FPMERGE_HI(w_y3, w_uv1);
				((mlib_d64 *)pyuyv0)[8 * j + 7] =
					VIS_FPMERGE_LO(w_y3, w_uv1);
			}

			if (left) {
				mlib_d64 res_buf[8];

				UNALIGNLOAD_TWO_64_NF(v_u0, v_u1,
					(mlib_d64 *)pu + 2 * j);
				UNALIGNLOAD_TWO_64_NF(v_v0, v_v1,
					(mlib_d64 *)pv + 2 * j);
				UNALIGNLOAD_FOUR_64_NF(w_y0, w_y1, w_y2, w_y3,
					(mlib_d64 *)py0 + 4 * j);

				w_uv0 = vis_fpmerge(vis_read_hi(v_u0),
					vis_read_hi(v_v0));
				res_buf[0] = VIS_FPMERGE_HI(w_y0, w_uv0);
				res_buf[1] = VIS_FPMERGE_LO(w_y0, w_uv0);
				w_uv1 = vis_fpmerge(vis_read_lo(v_u0),
					vis_read_lo(v_v0));
				res_buf[2] = VIS_FPMERGE_HI(w_y1, w_uv1);
				res_buf[3] = VIS_FPMERGE_LO(w_y1, w_uv1);
				w_uv0 = vis_fpmerge(vis_read_hi(v_u1),
					vis_read_hi(v_v1));
				res_buf[4] = VIS_FPMERGE_HI(w_y2, w_uv0);
				res_buf[5] = VIS_FPMERGE_LO(w_y2, w_uv0);
				w_uv1 = vis_fpmerge(vis_read_lo(v_u1),
					vis_read_lo(v_v1));
				res_buf[6] = VIS_FPMERGE_HI(w_y3, w_uv1);
				res_buf[7] = VIS_FPMERGE_LO(w_y3, w_uv1);

				for (j = 0; j < left; j++) {
					((mlib_f32 *)pyuyv0)[16 * count + j] =
						((mlib_f32 *)res_buf)[j];
				}
			}

			count = w >> 3;
			left = w - (count << 3);
			i2++;
			yuyv += dlb;
			y += ylb;
			u += (i2 & 1) ? 0 : uvlb;
			v += (i2 & 1) ? 0 : uvlb;
		}
	}
}

/* *********************************************************** */

void
__mlib_VideoColorYUV420seq_to_UYVY422int(
	mlib_u32 *uyvy,
	const mlib_u8 *y,
	const mlib_u8 *u,
	const mlib_u8 *v,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 ylb,
	mlib_s32 uvlb)
{
	mlib_s32 i2, count = w >> 4, left;

	w >>= 1;
	dlb >>= 2;
	left = w - (count << 3);

	if (w == 0 || h == 0)
		return;

	for (i2 = 0; i2 < h; ) {
		if ((((mlib_addr)u | (mlib_addr)v | (mlib_addr)y | (mlib_addr)
			uyvy) & 7)
			== 0) {
			mlib_d64 w_uv0, w_uv1, w_y0, w_y1;
			mlib_d64 v_u, v_v;
			mlib_s32 j;

/*
 * this loop is not the most optimal as in VIS1 version, but it
 * works faster because of the store problems on UIII
 */
#pragma pipeloop(0)
			for (j = 0; j < count; j++) {
				v_u = ((mlib_d64 *)u)[j];
				v_v = ((mlib_d64 *)v)[j];
				w_y0 = ((mlib_d64 *)y)[2 * j];
				w_y1 = ((mlib_d64 *)y)[2 * j + 1];
				w_uv0 = vis_fpmerge(vis_read_hi(v_u),
					vis_read_hi(v_v));
				w_uv1 = vis_fpmerge(vis_read_lo(v_u),
					vis_read_lo(v_v));
				((mlib_d64 *)uyvy)[4 * j] =
					VIS_FPMERGE_HI(w_uv0, w_y0);
				((mlib_d64 *)uyvy)[4 * j + 1] =
					VIS_FPMERGE_LO(w_uv0, w_y0);
				((mlib_d64 *)uyvy)[4 * j + 2] =
					VIS_FPMERGE_HI(w_uv1, w_y1);
				((mlib_d64 *)uyvy)[4 * j + 3] =
					VIS_FPMERGE_LO(w_uv1, w_y1);
			}

			if (left) {
				mlib_d64 res_buf0[4];

				v_u = vis_ld_d64_nf(((mlib_d64 *)u) + j);
				v_v = vis_ld_d64_nf(((mlib_d64 *)v) + j);
				w_y0 = vis_ld_d64_nf(((mlib_d64 *)y) + 2 * j);
				w_y1 = vis_ld_d64_nf(((mlib_d64 *)y) +
				    2 * j + 1);
				w_uv0 = vis_fpmerge(vis_read_hi(v_u),
					vis_read_hi(v_v));
				res_buf0[0] = VIS_FPMERGE_HI(w_uv0, w_y0);
				res_buf0[1] = VIS_FPMERGE_LO(w_uv0, w_y0);
				w_uv1 = vis_fpmerge(vis_read_lo(v_u),
					vis_read_lo(v_v));
				res_buf0[2] = VIS_FPMERGE_HI(w_uv1, w_y1);
				res_buf0[3] = VIS_FPMERGE_LO(w_uv1, w_y1);
				for (j = 0; j < left; j++) {
					((mlib_f32 *)uyvy)[8 * count + j] =
						((mlib_f32 *)res_buf0)[j];
				}
			}

			i2++;
			uyvy += dlb;
			y += ylb;
			u += (i2 & 1) ? 0 : uvlb;
			v += (i2 & 1) ? 0 : uvlb;
		} else {
			mlib_d64 w_uv0, w_uv1;
			mlib_d64 w_y0, w_y1, w_y2, w_y3;

			mlib_d64 v_u0, v_v0, v_u1, v_v1;
			mlib_s32 j;
			mlib_u8 *py0 = (void *)y, *pv = (void *)v, *pu =
				(void *)u;
			mlib_u32 *puyvy0 = uyvy;

			if ((mlib_addr)puyvy0 & 7) {
				puyvy0[0] =
					(u[0] << 24) | (v[0] << 8) | (py0[0] <<
					16) | py0[1];
				puyvy0++;
				py0 += 2;
				pu++;
				pv++;
				count = (w - 1) >> 4;
				left = (w - 1) - (count << 4);
			} else {
				count = w >> 4;
				left = w - (count << 4);
			}

#pragma pipeloop(0)
			for (j = 0; j < count; j++) {
				UNALIGNLOAD_TWO_64_NF(v_u0, v_u1,
					(mlib_d64 *)pu + 2 * j);
				UNALIGNLOAD_TWO_64_NF(v_v0, v_v1,
					(mlib_d64 *)pv + 2 * j);
				UNALIGNLOAD_FOUR_64_NF(w_y0, w_y1, w_y2, w_y3,
					(mlib_d64 *)py0 + 4 * j);

				w_uv0 = vis_fpmerge(vis_read_hi(v_u0),
					vis_read_hi(v_v0));
				((mlib_d64 *)puyvy0)[8 * j] =
					VIS_FPMERGE_HI(w_uv0, w_y0);
				((mlib_d64 *)puyvy0)[8 * j + 1] =
					VIS_FPMERGE_LO(w_uv0, w_y0);
				w_uv1 = vis_fpmerge(vis_read_lo(v_u0),
					vis_read_lo(v_v0));
				((mlib_d64 *)puyvy0)[8 * j + 2] =
					VIS_FPMERGE_HI(w_uv1, w_y1);
				((mlib_d64 *)puyvy0)[8 * j + 3] =
					VIS_FPMERGE_LO(w_uv1, w_y1);
				w_uv0 = vis_fpmerge(vis_read_hi(v_u1),
					vis_read_hi(v_v1));
				((mlib_d64 *)puyvy0)[8 * j + 4] =
					VIS_FPMERGE_HI(w_uv0, w_y2);
				((mlib_d64 *)puyvy0)[8 * j + 5] =
					VIS_FPMERGE_LO(w_uv0, w_y2);
				w_uv1 = vis_fpmerge(vis_read_lo(v_u1),
					vis_read_lo(v_v1));
				((mlib_d64 *)puyvy0)[8 * j + 6] =
					VIS_FPMERGE_HI(w_uv1, w_y3);
				((mlib_d64 *)puyvy0)[8 * j + 7] =
					VIS_FPMERGE_LO(w_uv1, w_y3);
			}

			if (left) {
				mlib_d64 res_buf[8];

				UNALIGNLOAD_TWO_64_NF(v_u0, v_u1,
					(mlib_d64 *)pu + 2 * j);
				UNALIGNLOAD_TWO_64_NF(v_v0, v_v1,
					(mlib_d64 *)pv + 2 * j);
				UNALIGNLOAD_FOUR_64_NF(w_y0, w_y1, w_y2, w_y3,
					(mlib_d64 *)py0 + 4 * j);

				w_uv0 = vis_fpmerge(vis_read_hi(v_u0),
					vis_read_hi(v_v0));
				res_buf[0] = VIS_FPMERGE_HI(w_uv0, w_y0);
				res_buf[1] = VIS_FPMERGE_LO(w_uv0, w_y0);
				w_uv1 = vis_fpmerge(vis_read_lo(v_u0),
					vis_read_lo(v_v0));
				res_buf[2] = VIS_FPMERGE_HI(w_uv1, w_y1);
				res_buf[3] = VIS_FPMERGE_LO(w_uv1, w_y1);
				w_uv0 = vis_fpmerge(vis_read_hi(v_u1),
					vis_read_hi(v_v1));
				res_buf[4] = VIS_FPMERGE_HI(w_uv0, w_y2);
				res_buf[5] = VIS_FPMERGE_LO(w_uv0, w_y2);
				w_uv1 = vis_fpmerge(vis_read_lo(v_u1),
					vis_read_lo(v_v1));
				res_buf[6] = VIS_FPMERGE_HI(w_uv1, w_y3);
				res_buf[7] = VIS_FPMERGE_LO(w_uv1, w_y3);

				for (j = 0; j < left; j++) {
					((mlib_f32 *)puyvy0)[16 * count + j] =
						((mlib_f32 *)res_buf)[j];
				}
			}

			count = w >> 3;
			left = w - (count << 3);
			i2++;
			uyvy += dlb;
			y += ylb;
			u += (i2 & 1) ? 0 : uvlb;
			v += (i2 & 1) ? 0 : uvlb;
		}
	}
}

/* *********************************************************** */

void
__mlib_VideoColorYUV411seq_to_UYVY422int(
	mlib_u32 *uyvy,
	const mlib_u8 *y,
	const mlib_u8 *u,
	const mlib_u8 *v,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 ylb,
	mlib_s32 uvlb)
{
	mlib_s32 i, count, left;

	dlb >>= 2;
	w >>= 2;
	count = w >> 2;
	left = w - (count << 2);

	if (w == 0 || h == 0)
		return;

	for (i = 0; i < h; i++, y += ylb, u += uvlb, v += uvlb, uyvy += dlb) {
		if ((((((mlib_addr)u | (mlib_addr)v) & 7) | (mlib_addr)y |
			(mlib_addr)
			uyvy) & 7) == 0) {
			mlib_d64 w_y0, w_y1, w_uv0, w_uv1, w_u, w_v;
			mlib_f32 v_u, v_v;
			mlib_s32 j;

#pragma pipeloop(0)
			for (j = 0; j < count; j++) {
				v_u = ((mlib_f32 *)u)[j];
				v_v = ((mlib_f32 *)v)[j];
				w_y0 = ((mlib_d64 *)y)[2 * j];
				w_y1 = ((mlib_d64 *)y)[2 * j + 1];
				w_u = vis_fpmerge(v_u, v_u);
				w_v = vis_fpmerge(v_v, v_v);
				w_uv0 = VIS_FPMERGE_HI(w_u, w_v);
				w_uv1 = VIS_FPMERGE_LO(w_u, w_v);
				((mlib_d64 *)uyvy)[4 * j] =
					VIS_FPMERGE_HI(w_uv0, w_y0);
				((mlib_d64 *)uyvy)[4 * j + 1] =
					VIS_FPMERGE_LO(w_uv0, w_y0);
				((mlib_d64 *)uyvy)[4 * j + 2] =
					VIS_FPMERGE_HI(w_uv1, w_y1);
				((mlib_d64 *)uyvy)[4 * j + 3] =
					VIS_FPMERGE_LO(w_uv1, w_y1);
			}

			if (left) {
				mlib_d64 res_buf[4];

				v_u = vis_ld_f32_nf((mlib_f32 *)u + count);
				v_v = vis_ld_f32_nf((mlib_f32 *)v + count);
				w_y0 = vis_ld_d64_nf((mlib_d64 *)y + 2 * count);
				w_y1 = vis_ld_d64_nf((mlib_d64 *)y + 2 * count +
					1);
				w_u = vis_fpmerge(v_u, v_u);
				w_v = vis_fpmerge(v_v, v_v);
				w_uv0 = VIS_FPMERGE_HI(w_u, w_v);
				w_uv1 = VIS_FPMERGE_LO(w_u, w_v);
				res_buf[0] = VIS_FPMERGE_HI(w_uv0, w_y0);
				res_buf[1] = VIS_FPMERGE_LO(w_uv0, w_y0);
				res_buf[2] = VIS_FPMERGE_HI(w_uv1, w_y1);
				res_buf[3] = VIS_FPMERGE_LO(w_uv1, w_y1);
				for (j = 0; j < left; j++)
					((mlib_d64 *)uyvy)[4 * count + j] =
						res_buf[j];
			}
		} else {
			mlib_d64 w_y0, w_y1, w_y2, w_y3, w_uv0, w_uv1, w_u, w_v;
			mlib_d64 v_u, v_v;
			mlib_s32 j;

			count = w >> 3;
			left = w - (count << 3);

			if ((mlib_addr)uyvy & 7) {
#pragma pipeloop(0)
				for (j = 0; j < count; j++) {
					mlib_d64 res0, res1, res2, res3, res4,
						res5, res6, res7;

					UNALIGNLOAD_64_NF(v_u, (mlib_d64 *)u +
					    j);
					UNALIGNLOAD_64_NF(v_v, (mlib_d64 *)v +
					    j);
					UNALIGNLOAD_FOUR_64_NF(w_y0, w_y1, w_y2,
						w_y3, (mlib_d64 *)y + 4 * j);
					w_uv0 = vis_fpmerge(vis_read_hi(v_u),
						vis_read_hi(v_v));
					w_uv1 = vis_fpmerge(vis_read_lo(v_u),
						vis_read_lo(v_v));
					vis_write_bmask(0x80918293, 0);
					res0 = vis_bshuffle(w_y0, w_uv0);
					res4 = vis_bshuffle(w_y2, w_uv1);
					vis_write_bmask(0xa4b5a6b7, 0);
					res1 = vis_bshuffle(w_y0, w_uv0);
					res5 = vis_bshuffle(w_y2, w_uv1);
					vis_write_bmask(0xc0d1c2d3, 0);
					res2 = vis_bshuffle(w_y1, w_uv0);
					res6 = vis_bshuffle(w_y3, w_uv1);
					vis_write_bmask(0xe4f5e6f7, 0);
					res3 = vis_bshuffle(w_y1, w_uv0);
					res7 = vis_bshuffle(w_y3, w_uv1);

					((mlib_f32 *)uyvy)[16 * j] =
						vis_read_hi(res0);
					STORE_MIDDLE((mlib_f32 *)uyvy + 16 * j +
						1, res0, res1);
					STORE_MIDDLE((mlib_f32 *)uyvy + 16 * j +
						3, res1, res2);
					STORE_MIDDLE((mlib_f32 *)uyvy + 16 * j +
						5, res2, res3);
					STORE_MIDDLE((mlib_f32 *)uyvy + 16 * j +
						7, res3, res4);
					STORE_MIDDLE((mlib_f32 *)uyvy + 16 * j +
						9, res4, res5);
					STORE_MIDDLE((mlib_f32 *)uyvy + 16 * j +
						11, res5, res6);
					STORE_MIDDLE((mlib_f32 *)uyvy + 16 * j +
						13, res6, res7);
					((mlib_f32 *)uyvy)[16 * j + 15] =
						vis_read_lo(res7);
				}

				if (left) {
					mlib_d64 res_buf[8];

					UNALIGNLOAD_64_NF(v_u, (mlib_d64 *)u +
					    j);
					UNALIGNLOAD_64_NF(v_v, (mlib_d64 *)v +
					    j);
					UNALIGNLOAD_FOUR_64_NF(w_y0, w_y1, w_y2,
						w_y3, (mlib_d64 *)y + 4 * j);
					w_uv0 = vis_fpmerge(vis_read_hi(v_u),
						vis_read_hi(v_v));
					w_uv1 = vis_fpmerge(vis_read_lo(v_u),
						vis_read_lo(v_v));
					vis_write_bmask(0x80918293, 0);
					res_buf[0] = vis_bshuffle(w_y0, w_uv0);
					res_buf[4] = vis_bshuffle(w_y2, w_uv1);
					vis_write_bmask(0xa4b5a6b7, 0);
					res_buf[1] = vis_bshuffle(w_y0, w_uv0);
					res_buf[5] = vis_bshuffle(w_y2, w_uv1);
					vis_write_bmask(0xc0d1c2d3, 0);
					res_buf[2] = vis_bshuffle(w_y1, w_uv0);
					res_buf[6] = vis_bshuffle(w_y3, w_uv1);
					vis_write_bmask(0xe4f5e6f7, 0);
					res_buf[3] = vis_bshuffle(w_y1, w_uv0);
					res_buf[7] = vis_bshuffle(w_y3, w_uv1);

#pragma pipeloop(0)
					for (j = 0; j < left * 2; j++)
						((mlib_f32 *)uyvy)[16 * count +
							j] =
							((mlib_f32 *)
							res_buf)[j];
				}
			} else {
#pragma pipeloop(0)
				for (j = 0; j < count; j++) {
					UNALIGNLOAD_64_NF(v_u, (mlib_d64 *)u +
					    j);
					UNALIGNLOAD_64_NF(v_v, (mlib_d64 *)v +
					    j);
					UNALIGNLOAD_FOUR_64_NF(w_y0, w_y1, w_y2,
						w_y3, (mlib_d64 *)y + 4 * j);
					w_u = vis_fpmerge(vis_read_hi(v_u),
						vis_read_hi(v_u));
					w_v = vis_fpmerge(vis_read_hi(v_v),
						vis_read_hi(v_v));
					w_uv0 = VIS_FPMERGE_HI(w_u, w_v);
					w_uv1 = VIS_FPMERGE_LO(w_u, w_v);
					((mlib_d64 *)uyvy)[8 * j] =
						VIS_FPMERGE_HI(w_uv0, w_y0);
					((mlib_d64 *)uyvy)[8 * j + 1] =
						VIS_FPMERGE_LO(w_uv0, w_y0);
					((mlib_d64 *)uyvy)[8 * j + 2] =
						VIS_FPMERGE_HI(w_uv1, w_y1);
					((mlib_d64 *)uyvy)[8 * j + 3] =
						VIS_FPMERGE_LO(w_uv1, w_y1);

					w_u = vis_fpmerge(vis_read_lo(v_u),
						vis_read_lo(v_u));
					w_v = vis_fpmerge(vis_read_lo(v_v),
						vis_read_lo(v_v));
					w_uv0 = VIS_FPMERGE_HI(w_u, w_v);
					w_uv1 = VIS_FPMERGE_LO(w_u, w_v);
					((mlib_d64 *)uyvy)[8 * j + 4] =
						VIS_FPMERGE_HI(w_uv0, w_y2);
					((mlib_d64 *)uyvy)[8 * j + 5] =
						VIS_FPMERGE_LO(w_uv0, w_y2);
					((mlib_d64 *)uyvy)[8 * j + 6] =
						VIS_FPMERGE_HI(w_uv1, w_y3);
					((mlib_d64 *)uyvy)[8 * j + 7] =
						VIS_FPMERGE_LO(w_uv1, w_y3);
				}

				if (left) {
					mlib_d64 res_buf[8];

					UNALIGNLOAD_64_NF(v_u, (mlib_d64 *)u +
					    j);
					UNALIGNLOAD_64_NF(v_v, (mlib_d64 *)v +
					    j);
					UNALIGNLOAD_FOUR_64_NF(w_y0, w_y1, w_y2,
						w_y3, (mlib_d64 *)y + 4 * j);

					w_u = vis_fpmerge(vis_read_hi(v_u),
						vis_read_hi(v_u));
					w_v = vis_fpmerge(vis_read_hi(v_v),
						vis_read_hi(v_v));
					w_uv0 = VIS_FPMERGE_HI(w_u, w_v);
					w_uv1 = VIS_FPMERGE_LO(w_u, w_v);
					res_buf[0] =
						VIS_FPMERGE_HI(w_uv0, w_y0);
					res_buf[1] =
						VIS_FPMERGE_LO(w_uv0, w_y0);
					res_buf[2] =
						VIS_FPMERGE_HI(w_uv1, w_y1);
					res_buf[3] =
						VIS_FPMERGE_LO(w_uv1, w_y1);

					w_u = vis_fpmerge(vis_read_lo(v_u),
						vis_read_lo(v_u));
					w_v = vis_fpmerge(vis_read_lo(v_v),
						vis_read_lo(v_v));
					w_uv0 = VIS_FPMERGE_HI(w_u, w_v);
					w_uv1 = VIS_FPMERGE_LO(w_u, w_v);
					res_buf[4] =
						VIS_FPMERGE_HI(w_uv0, w_y2);
					res_buf[5] =
						VIS_FPMERGE_LO(w_uv0, w_y2);
					res_buf[6] =
						VIS_FPMERGE_HI(w_uv1, w_y3);
					res_buf[7] =
						VIS_FPMERGE_LO(w_uv1, w_y3);

#pragma pipeloop(0)
					for (j = 0; j < left * 2; j++)
						((mlib_f32 *)uyvy)[16 * count +
							j] =
							((mlib_f32 *)
							res_buf)[j];
				}
			}

			count = w >> 2;
			left = w - (count << 2);
		}
	}
}

/* *********************************************************** */

void
__mlib_VideoColorYUV422seq_to_UYVY422int(
	mlib_u32 *uyvy,
	const mlib_u8 *y,
	const mlib_u8 *u,
	const mlib_u8 *v,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 ylb,
	mlib_s32 uvlb)
{
	mlib_s32 i, count, left;

	dlb >>= 2;
	w >>= 1;
	count = w >> 3;
	left = w - (count << 3);

	if (w == 0 || h == 0)
		return;

#pragma pipeloop(0)
	for (i = 0; i < h; i++, y += ylb, u += uvlb, v += uvlb, uyvy += dlb) {
		if ((((mlib_addr)u | (mlib_addr)v | (mlib_addr)y | (mlib_addr)
			uyvy) & 7)
			== 0) {
			mlib_d64 w_uv0, w_uv1, w_y0, w_y1, w_u, w_v;
			mlib_s32 j;

#pragma pipeloop(0)
			for (j = 0; j < count; j++) {
				w_u = ((mlib_d64 *)u)[j];
				w_v = ((mlib_d64 *)v)[j];
				w_y0 = ((mlib_d64 *)y)[2 * j];
				w_y1 = ((mlib_d64 *)y)[2 * j + 1];
				w_uv0 = VIS_FPMERGE_HI(w_u, w_v);
				w_uv1 = VIS_FPMERGE_LO(w_u, w_v);
				((mlib_d64 *)uyvy)[4 * j] =
					VIS_FPMERGE_HI(w_uv0, w_y0);
				((mlib_d64 *)uyvy)[4 * j + 1] =
					VIS_FPMERGE_LO(w_uv0, w_y0);
				((mlib_d64 *)uyvy)[4 * j + 2] =
					VIS_FPMERGE_HI(w_uv1, w_y1);
				((mlib_d64 *)uyvy)[4 * j + 3] =
					VIS_FPMERGE_LO(w_uv1, w_y1);
			}

			if (left) {
				mlib_d64 res_buf[4];

				w_u = vis_ld_d64_nf((mlib_d64 *)u + j);
				w_v = vis_ld_d64_nf((mlib_d64 *)v + j);
				w_y0 = vis_ld_d64_nf((mlib_d64 *)y + 2 * j);
				w_y1 = vis_ld_d64_nf((mlib_d64 *)y + 2 * j + 1);
				w_uv0 = VIS_FPMERGE_HI(w_u, w_v);
				w_uv1 = VIS_FPMERGE_LO(w_u, w_v);
				res_buf[0] = VIS_FPMERGE_HI(w_uv0, w_y0);
				res_buf[1] = VIS_FPMERGE_LO(w_uv0, w_y0);
				res_buf[2] = VIS_FPMERGE_HI(w_uv1, w_y1);
				res_buf[3] = VIS_FPMERGE_LO(w_uv1, w_y1);

				for (j = 0; j < left; j++) {
					((mlib_f32 *)uyvy)[8 * count + j] =
						((mlib_f32 *)res_buf)[j];
				}
			}
		} else {
			mlib_d64 w_uv0, w_uv1, w_y0, w_y1, w_y2, w_y3, w_u0,
				w_v0, w_u1, w_v1;
			mlib_s32 j, off;

			const mlib_u8 *pu = u, *py = y, *pv = v;
			mlib_u32 *puyvy = uyvy;
			mlib_d64 ld0, ld1, ld2, *dpv;

			if ((mlib_addr)puyvy & 7) {
				puyvy[0] =
					(pu[0] << 24) | (pv[0] << 8) | (py[0] <<
					16) | (py[1]);
				puyvy++;
				pu++;
				pv++;
				py += 2;
				count = (w - 1) >> 4;
				left = (w - 1) - (count << 4);
			} else {
				count = w >> 4;
				left = w - (count << 4);
			}

			off = (mlib_addr)pv & 7;
			dpv = (mlib_d64 *)(pv - off);
			vis_write_bmask(off * 0x11111111, 0x01234567);
			ld0 = vis_ld_d64_nf(dpv);
#pragma pipeloop(0)
			for (j = 0; j < count; j++) {
				UNALIGNLOAD_TWO_64_NF(w_u0, w_u1,
					(mlib_d64 *)pu + j * 2);

				ld1 = dpv[1];
				ld2 = vis_ld_d64_nf(dpv + 2);
				w_v0 = vis_bshuffle(ld0, ld1);
				w_v1 = vis_bshuffle(ld1, ld2);
				ld0 = ld2;
				dpv += 2;

				UNALIGNLOAD_FOUR_64_NF(w_y0, w_y1, w_y2, w_y3,
					(mlib_d64 *)py + j * 4);
				w_uv0 = VIS_FPMERGE_HI(w_u0, w_v0);
				w_uv1 = VIS_FPMERGE_LO(w_u0, w_v0);
				((mlib_d64 *)puyvy)[8 * j] =
					VIS_FPMERGE_HI(w_uv0, w_y0);
				((mlib_d64 *)puyvy)[8 * j + 1] =
					VIS_FPMERGE_LO(w_uv0, w_y0);
				((mlib_d64 *)puyvy)[8 * j + 2] =
					VIS_FPMERGE_HI(w_uv1, w_y1);
				((mlib_d64 *)puyvy)[8 * j + 3] =
					VIS_FPMERGE_LO(w_uv1, w_y1);
				w_uv0 = VIS_FPMERGE_HI(w_u1, w_v1);
				w_uv1 = VIS_FPMERGE_LO(w_u1, w_v1);
				((mlib_d64 *)puyvy)[8 * j + 4] =
					VIS_FPMERGE_HI(w_uv0, w_y2);
				((mlib_d64 *)puyvy)[8 * j + 5] =
					VIS_FPMERGE_LO(w_uv0, w_y2);
				((mlib_d64 *)puyvy)[8 * j + 6] =
					VIS_FPMERGE_HI(w_uv1, w_y3);
				((mlib_d64 *)puyvy)[8 * j + 7] =
					VIS_FPMERGE_LO(w_uv1, w_y3);
			}

			if (left) {
				mlib_d64 res_buf[8];

				UNALIGNLOAD_TWO_64_NF(w_u0, w_u1,
					(mlib_d64 *)pu + j * 2);

				ld1 = vis_ld_d64_nf(dpv + 1);
				ld2 = vis_ld_d64_nf(dpv + 2);
				w_v0 = vis_bshuffle(ld0, ld1);
				w_v1 = vis_bshuffle(ld1, ld2);

				UNALIGNLOAD_FOUR_64_NF(w_y0, w_y1, w_y2, w_y3,
					(mlib_d64 *)py + j * 4);
				w_uv0 = VIS_FPMERGE_HI(w_u0, w_v0);
				w_uv1 = VIS_FPMERGE_LO(w_u0, w_v0);
				res_buf[0] = VIS_FPMERGE_HI(w_uv0, w_y0);
				res_buf[1] = VIS_FPMERGE_LO(w_uv0, w_y0);
				res_buf[2] = VIS_FPMERGE_HI(w_uv1, w_y1);
				res_buf[3] = VIS_FPMERGE_LO(w_uv1, w_y1);
				w_uv0 = VIS_FPMERGE_HI(w_u1, w_v1);
				w_uv1 = VIS_FPMERGE_LO(w_u1, w_v1);
				res_buf[4] = VIS_FPMERGE_HI(w_uv0, w_y2);
				res_buf[5] = VIS_FPMERGE_LO(w_uv0, w_y2);
				res_buf[6] = VIS_FPMERGE_HI(w_uv1, w_y3);
				res_buf[7] = VIS_FPMERGE_LO(w_uv1, w_y3);
				for (j = 0; j < left; j++) {
					((mlib_f32 *)puyvy)[16 * count + j] =
						((mlib_f32 *)res_buf)[j];
				}
			}

			count = w >> 3;
			left = w - (count << 3);
		}
	}
}

/* *********************************************************** */
