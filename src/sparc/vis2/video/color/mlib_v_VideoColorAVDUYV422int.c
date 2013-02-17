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

#pragma ident	"@(#)mlib_v_VideoColorAVDUYV422int.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorYUV444seq_to_UYVY422int - convert YUV sequential to
 *                                               interleaved with subsampling
 *
 * SYNOPSIS
 *
 *      void mlib_VideoColorYUV444seq_to_UYVY422int(mlib_u32      *uyvy,
 *                                                  const mlib_u8 *y,
 *                                                  const mlib_u8 *u,
 *                                                  const mlib_u8 *v,
 *                                                  mlib_s32      w,
 *                                                  mlib_s32      h,
 *                                                  mlib_s32      dlb,
 *                                                  mlib_s32      slb);
 * ARGUMENTS
 *      uyvy        pointer to output buffer
 *      y           pointer to Y input buffer
 *      u           pointer to U input buffer
 *      v           pointer to V input buffer
 *      width       image width in pixels
 *      height      image height in lines
 *      dst_stride  linebytes for UYVY buffer
 *      src_stride  linebytes for input buffers
 *
 * DESCRIPTION
 *      The Y, U, V pixel streams are combined into a UYVY pixel stream. All
 *      pixel components are 8-bit unsigned integers. The Y, U, and V buffers
 *      have dimensions width and height. Dimension width is assumed to be a
 *      multiple of 2.
 *      Adjacent U and V values are averaged to get the output U and V values.
 *
 *      For r = 0, 1, 2, ..., height-1 and c = 0, 2, 4, ..., width - 2:
 *        UYVY[r][c/2] =  | (((U[r][c] + U[r][c+1]) / 2) << 24) |
 *                          (Y[r][c] << 16)
 *                        | (((V[r][c] + V[r][c+1]) / 2) << 8)) |
 *                          (Y[r][c+1])
 */

/*
 * FUNCTION
 *      mlib_VideoColorYUV444int_to_UYVY422int - convert YUV interleaved with
 *                                               subsampling
 *
 * SYNOPSIS
 *
 *      void mlib_VideoColorYUV444int_to_UYVY422int(mlib_u32      *uyvy,
 *                                                  const mlib_u8 *yuv,
 *                                                  mlib_s32      w,
 *                                                  mlib_s32      h,
 *                                                  mlib_s32      dlb,
 *                                                  mlib_s32      slb);
 *
 * ARGUMENTS
 *      uyvy        pointer to output buffer
 *      yuv         pointer to input buffer
 *      width       image width in pixels
 *      height      image height in lines
 *      dst_stride  linebytes for UYVY buffer
 *      src_stride  linebytes for input buffers
 *
 * DESCRIPTION
 *      The YUV pixel stream is broken apart and recombined into a UYVY
 *      pixel stream. All pixel components are 8-bit unsigned integers.
 *      The YUV buffer has dimensions width and height. Dimension width is
 *      assumed to be a multiple of 2. Adjacent U and V values are averaged
 *      to get the output U and V values.
 *      The sequence of values in the input stream is Y[r][c], U[r][c],
 *      V[r][c], Y[r][c+1], U[r][c+1], V[r][c+1], ...
 *
 *      For r = 0, 1, 2, ..., height-1 and c = 0, 2, 4, ..., width - 2:
 *         UYVY[r][c/2] =  | (((U[r][c] + U[r][c+1]) / 2) << 24) |
 *                           (Y[r][c] << 16)
 *                         | (((V[r][c] + V[r][c+1]) / 2) << 8)) |
 *                           (Y[r][c+1])
 */

/*
 * FUNCTION
 *      mlib_VideoColorUYV444int_to_YUYV422int - convert UYV interleaved with
 *                                               subsampling
 *
 * SYNOPSIS
 *
 *      void mlib_VideoColorUYV444int_to_YUYV422int(mlib_u32      *yuyv,
 *                                                  const mlib_u8 *uyv,
 *                                                  mlib_s32      w,
 *                                                  mlib_s32      h,
 *                                                  mlib_s32      dlb,
 *                                                  mlib_s32      slb);
 *
 * ARGUMENTS
 *      yuyv        pointer to output buffer
 *      uyv         pointer to input buffer
 *      width       image width in pixels
 *      height      image height in lines
 *      dst_stride  linebytes for output buffer
 *      src_stride  linebytes for input buffer
 *
 * DESCRIPTION
 *      The UYV pixel stream is broken apart and recombined into a YUYV
 *      pixel stream. All pixel components are 8-bit unsigned integers.
 *      The UYV buffer has dimensions width and height. Dimension width is
 *      assumed to be a multiple of 2. Adjacent U and V values are averaged
 *      to get the output U and V values.
 *      The sequence of values in the input stream is U[r][c], Y[r][c],
 *      V[r][c], U[r][c+1], Y[r][c+1], V[r][c+1], ...
 *
 *      For r = 0, 1, 2, ..., height-1 and c = 0, 2, 4, ..., width - 2:
 *         YUYV[r][c/2] = (Y[r][c]  << 24) | (((U[r][c] + U[r][c+1]) / 2) << 16)
 *                      | (Y[r][c+1] << 8) | (((V[r][c] + V[r][c+1]) / 2))
 */

/*
 * FUNCTION
 *      mlib_VideoColorUYV444int_to_UYVY422int - convert UYV interleaved with
 *                                               subsampling
 *
 * SYNOPSIS
 *
 *      void mlib_VideoColorUYV444int_to_UYVY422int(mlib_u32      *uyvy,
 *                                                  const mlib_u8 *uyv,
 *                                                  mlib_s32      w,
 *                                                  mlib_s32      h,
 *                                                  mlib_s32      dlb,
 *                                                  mlib_s32      slb);
 *
 * ARGUMENTS
 *      uyvy        pointer to output buffer
 *      uyv         pointer to input buffer
 *      width       image width in pixels
 *      height      image height in lines
 *      dst_stride  linebytes for output buffer
 *      src_stride  linebytes for input buffer
 *
 * DESCRIPTION
 *      The UYV pixel stream is broken apart and recombined into a UYVY
 *      pixel stream. All pixel components are 8-bit unsigned integers.
 *      The UYV buffer has dimensions width and height. Dimension width is
 *      assumed to be a multiple of 2. Adjacent U and V values are averaged
 *      to get the output U and V values.
 *      The sequence of values in the input stream is U[r][c], Y[r][c],
 *      V[r][c], U[r][c+1], Y[r][c+1], V[r][c+1], ...
 *
 *      For r = 0, 1, 2, ..., height-1 and c = 0, 2, 4, ..., width - 2:
 *         UYVY[r][c/2] =  (((U[r][c] + U[r][c+1]) / 2) << 24) | (Y[r][c] << 16)
 *                       | (((V[r][c] + V[r][c+1]) / 2) << 8)) | (Y[r][c+1])
 */

#include <mlib_video.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorUYV444int_to_UYVY422int = \
	__mlib_VideoColorUYV444int_to_UYVY422int
#pragma weak mlib_VideoColorUYV444int_to_YUYV422int = \
	__mlib_VideoColorUYV444int_to_YUYV422int
#pragma weak mlib_VideoColorYUV444int_to_UYVY422int = \
	__mlib_VideoColorYUV444int_to_UYVY422int
#pragma weak mlib_VideoColorYUV444seq_to_UYVY422int = \
	__mlib_VideoColorYUV444seq_to_UYVY422int

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorUYV444int_to_UYVY422int)
	mlib_VideoColorUYV444int_to_UYVY422int
	__attribute__((weak,
	alias("__mlib_VideoColorUYV444int_to_UYVY422int")));
__typeof__(__mlib_VideoColorUYV444int_to_YUYV422int)
	mlib_VideoColorUYV444int_to_YUYV422int
	__attribute__((weak,
	alias("__mlib_VideoColorUYV444int_to_YUYV422int")));
__typeof__(__mlib_VideoColorYUV444int_to_UYVY422int)
	mlib_VideoColorYUV444int_to_UYVY422int
	__attribute__((weak,
	alias("__mlib_VideoColorYUV444int_to_UYVY422int")));
__typeof__(__mlib_VideoColorYUV444seq_to_UYVY422int)
	mlib_VideoColorYUV444seq_to_UYVY422int
	__attribute__((weak,
	alias("__mlib_VideoColorYUV444seq_to_UYVY422int")));

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

#define	UNALIGNLOAD_64(var, addr)                               \
	{                                                       \
	    mlib_d64 *al_addr = vis_alignaddr(addr, 0);         \
	                                                        \
	    var = vis_faligndata(*al_addr, *(al_addr + 1));     \
	}

/* *********************************************************** */

#define	UNALIGNLOAD_TWO_64(var1, var2, addr)                            \
	{                                                               \
	    mlib_d64 *al_addr = vis_alignaddr(addr, 0);                 \
	    mlib_d64 common;                                            \
	                                                                \
	    var1 = vis_faligndata(*al_addr, common = *(al_addr + 1));   \
	    var2 = vis_faligndata(common, *(al_addr + 2));              \
	}

/* *********************************************************** */

#define	UNALIGNLOAD_FOUR_64(var1, var2, var3, var4, addr)               \
	{                                                               \
	    mlib_d64 *al_addr = vis_alignaddr(addr, 0);                 \
	    mlib_d64 common, common1, common2;                          \
	                                                                \
	    var1 = vis_faligndata(*al_addr, common = *(al_addr + 1));   \
	    var2 = vis_faligndata(common, common1 = *(al_addr + 2));    \
	    var3 = vis_faligndata(common1, common2 = *(al_addr + 3));   \
	    var4 = vis_faligndata(common2, *(al_addr + 4));             \
	}

/* *********************************************************** */

#define	STORE_MIDDLE(addr, var1, var2)                          \
	* (mlib_d64 *)(addr) =                                  \
	vis_freg_pair(vis_read_lo(var1), vis_read_hi(var2))

/* *********************************************************** */

void
__mlib_VideoColorYUV444seq_to_UYVY422int(
	mlib_u32 *uyvy,
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

	for (i = 0; i < h; i++, y += slb, u += slb, v += slb, uyvy += dlb) {
		if ((((mlib_addr)u | (mlib_addr)v | (mlib_addr)y | (mlib_addr)
			uyvy) & 7) == 0) {
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

				((mlib_d64 *)uyvy)[2 * j] =
					VIS_FPMERGE_HI(w_uv, w_y);
				((mlib_d64 *)uyvy)[2 * j + 1] =
					VIS_FPMERGE_LO(w_uv, w_y);
			}

			if (w & 3) {
				mlib_d64 res_buf[2];

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

				res_buf[0] = VIS_FPMERGE_HI(w_uv, w_y);
				res_buf[1] = VIS_FPMERGE_LO(w_uv, w_y);

				for (k = 0; k < (w & 3); k++) {
					((mlib_f32 *)uyvy)[4 * j + k] =
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
			mlib_u8 *py = (void *)y, *pu = (void *)u, *pv =
				(void *)v;
			mlib_u32 *puyvy = uyvy;

			if ((mlib_addr)uyvy & 7) {
				val_y0 = py[0];
				val_y1 = py[1];
				val_u0 = (pu[0] + pu[1]) >> 1;
				val_v0 = (pv[0] + pv[1]) >> 1;
				(*puyvy++) =
					(val_u0 << 24) | (val_y0 << 16) |
					(val_v0 << 8) | val_y1;
				py += 2;
				pu += 2;
				pv += 2;
				w0--;
			}

			dpy = vis_alignaddr(py, 0);
			w_y0 = vis_ld_d64_nf(dpy); dpy++;
			dpu = vis_alignaddr(pu, 0);
			w_u0 = vis_ld_d64_nf(dpu); dpu++;
			dpv = vis_alignaddr(pv, 0);
			w_v0 = vis_ld_d64_nf(dpv); dpv++;

			vis_write_bmask(0x02461357, 0);

#pragma pipeloop(0)
			for (j = 0; j < w0 >> 2; j++) {
				w_y1 = vis_ld_d64_nf(dpy + j);
				vis_alignaddr(py, 0);
				w_y = vis_faligndata(w_y0, w_y1);
				w_y0 = w_y1;
				w_u1 = vis_ld_d64_nf(dpu + j);
				vis_alignaddr(pu, 0);
				w_u = vis_faligndata(w_u0, w_u1);
				w_u0 = w_u1;
				w_v1 = vis_ld_d64_nf(dpv + j);
				vis_alignaddr(pv, 0);
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

				((mlib_d64 *)puyvy)[2 * j] =
					VIS_FPMERGE_HI(w_uv, w_y);
				((mlib_d64 *)puyvy)[2 * j + 1] =
					VIS_FPMERGE_LO(w_uv, w_y);
			}

			if (w0 & 3) {
				mlib_d64 res_buf[2];

				vis_alignaddr(py, 0);
				w_y1 = vis_ld_d64_nf(dpy + j);
				w_y = vis_faligndata(w_y0, w_y1);
				vis_alignaddr(pu, 0);
				w_u1 = vis_ld_d64_nf(dpu + j);
				w_u = vis_faligndata(w_u0, w_u1);
				vis_alignaddr(pv, 0);
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

				res_buf[0] = VIS_FPMERGE_HI(w_uv, w_y);
				res_buf[1] = VIS_FPMERGE_LO(w_uv, w_y);

				for (k = 0; k < (w0 & 3); k++) {
					((mlib_f32 *)puyvy)[4 * j + k] =
						((mlib_f32 *)res_buf)[k];
				}
			}
		}
	}
}

/* *********************************************************** */

void
__mlib_VideoColorYUV444int_to_UYVY422int(
	mlib_u32 *uyvy,
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

	for (i = 0; i < h; i++, yuv += slb, uyvy += dlb) {
		if ((((mlib_addr)yuv | (mlib_addr)uyvy) & 7) == 0) {
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

				((mlib_d64 *)uyvy)[2 * j] =
					VIS_FPMERGE_HI(w_uv, w_y);
				((mlib_d64 *)uyvy)[2 * j + 1] =
					VIS_FPMERGE_LO(w_uv, w_y);
			}

			if (left) {
				mlib_d64 res_buf[2];

				w_ld0 = vis_ld_d64_nf((mlib_d64 *)yuv + 3 *
					count);
				w_ld1 = vis_ld_d64_nf((mlib_d64 *)yuv + 3 *
					count + 1);
				w_ld2 = vis_ld_d64_nf((mlib_d64 *)yuv + 3 *
					count + 2);

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

				res_buf[0] = VIS_FPMERGE_HI(w_uv, w_y);
				res_buf[1] = VIS_FPMERGE_LO(w_uv, w_y);

				for (j = 0; j < left; j++) {
					((mlib_f32 *)uyvy)[4 * count + j] =
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
			mlib_u32 *puyvy = uyvy;

			if ((mlib_addr)puyvy & 7) {
				val_y0 = yuv[0];
				val_y1 = yuv[3];
				val_u0 = (yuv[1] + yuv[4]) >> 1;
				val_v0 = (yuv[2] + yuv[5]) >> 1;
				puyvy[0] =
					(val_u0 << 24) | (val_y0 << 16) |
					(val_v0 << 8) | val_y1;
				pyuv += 6;
				puyvy++;
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

				((mlib_d64 *)puyvy)[2 * j] =
					VIS_FPMERGE_HI(w_uv, w_y);
				((mlib_d64 *)puyvy)[2 * j + 1] =
					VIS_FPMERGE_LO(w_uv, w_y);
			}

			if (left) {
				mlib_d64 res_buf[2];

				l1 = vis_ld_d64_nf(al_addr); al_addr++;
				l2 = vis_ld_d64_nf(al_addr); al_addr++;
				l3 = vis_ld_d64_nf(al_addr);
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

				res_buf[0] = VIS_FPMERGE_HI(w_uv, w_y);
				res_buf[1] = VIS_FPMERGE_LO(w_uv, w_y);

				for (j = 0; j < left; j++) {
					((mlib_f32 *)puyvy)[4 * count + j] =
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
__mlib_VideoColorUYV444int_to_YUYV422int(
	mlib_u32 *yuyv,
	const mlib_u8 *uyv,
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

	for (i = 0; i < h; i++, uyv += slb, yuyv += dlb) {
		if ((((mlib_addr)uyv | (mlib_addr)yuyv) & 7) == 0) {
			mlib_d64 w_y, w_u, w_v, w_uv, w_tmp0, w_acc0, w_acc1;
			mlib_d64 w_ld0, w_ld1, w_ld2;
			mlib_f32 v_one = vis_to_float(0x1000000);
			mlib_f32 v_u, v_v;
			mlib_s32 j;

#pragma pipeloop(0)
			for (j = 0; j < count; j++) {
				w_ld0 = ((mlib_d64 *)uyv)[3 * j];
				w_ld1 = ((mlib_d64 *)uyv)[3 * j + 1];
				w_ld2 = ((mlib_d64 *)uyv)[3 * j + 2];

				MLIB_SPLIT3_U8(w_u, w_y, w_v, w_ld0, w_ld1,
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

				w_ld0 = vis_ld_d64_nf((mlib_d64 *)uyv + 3 *
					count);
				w_ld1 = vis_ld_d64_nf((mlib_d64 *)uyv + 3 *
					count + 1);
				w_ld2 = vis_ld_d64_nf((mlib_d64 *)uyv + 3 *
					count + 2);

				MLIB_SPLIT3_U8(w_u, w_y, w_v, w_ld0, w_ld1,
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
			mlib_u8 *puyv = (void *)uyv;
			mlib_u32 *pyuyv = yuyv;

			if ((mlib_addr)pyuyv & 7) {
				val_y0 = uyv[1];
				val_y1 = uyv[4];
				val_u0 = (uyv[0] + uyv[3]) >> 1;
				val_v0 = (uyv[2] + uyv[5]) >> 1;
				pyuyv[0] =
					(val_y0 << 24) | (val_u0 << 16) |
					(val_y1 << 8) | val_v0;
				puyv += 6;
				pyuyv++;
				count = (w - 1) >> 2;
				left = (w - 1) - (count << 2);
			} else {
				count = w >> 2;
				left = w - (count << 2);
			}

			al_addr = vis_alignaddr(puyv, 0);
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
				MLIB_SPLIT3_U8(w_u, w_y, w_v, w_ld0, w_ld1,
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
				l3 = vis_ld_d64_nf(al_addr);
				w_ld0 = vis_faligndata(l0, l1);
				w_ld1 = vis_faligndata(l1, l2);
				w_ld2 = vis_faligndata(l2, l3);

				MLIB_SPLIT3_U8(w_u, w_y, w_v, w_ld0, w_ld1,
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
__mlib_VideoColorUYV444int_to_UYVY422int(
	mlib_u32 *uyvy,
	const mlib_u8 *uyv,
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

	for (i = 0; i < h; i++, uyv += slb, uyvy += dlb) {
		if ((((mlib_addr)uyv | (mlib_addr)uyvy) & 7) == 0) {
			mlib_d64 w_y, w_u, w_v, w_uv, w_tmp0, w_acc0, w_acc1;
			mlib_d64 w_ld0, w_ld1, w_ld2;
			mlib_f32 v_one = vis_to_float(0x1000000);
			mlib_f32 v_u, v_v;
			mlib_s32 j;

#pragma pipeloop(0)
			for (j = 0; j < count; j++) {
				w_ld0 = ((mlib_d64 *)uyv)[3 * j];
				w_ld1 = ((mlib_d64 *)uyv)[3 * j + 1];
				w_ld2 = ((mlib_d64 *)uyv)[3 * j + 2];

				MLIB_SPLIT3_U8(w_u, w_y, w_v, w_ld0, w_ld1,
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

				((mlib_d64 *)uyvy)[2 * j] =
					VIS_FPMERGE_HI(w_uv, w_y);
				((mlib_d64 *)uyvy)[2 * j + 1] =
					VIS_FPMERGE_LO(w_uv, w_y);
			}

			if (left) {
				mlib_d64 res_buf[2];

				w_ld0 = vis_ld_d64_nf((mlib_d64 *)uyv + 3 *
					count);
				w_ld1 = vis_ld_d64_nf((mlib_d64 *)uyv + 3 *
					count + 1);
				w_ld2 = vis_ld_d64_nf((mlib_d64 *)uyv + 3 *
					count + 2);

				MLIB_SPLIT3_U8(w_u, w_y, w_v, w_ld0, w_ld1,
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

				res_buf[0] = VIS_FPMERGE_HI(w_uv, w_y);
				res_buf[1] = VIS_FPMERGE_LO(w_uv, w_y);

				for (j = 0; j < left; j++) {
					((mlib_f32 *)uyvy)[4 * count + j] =
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
			mlib_u8 *puyv = (void *)uyv;
			mlib_u32 *puyvy = uyvy;

			if ((mlib_addr)puyvy & 7) {
				val_y0 = uyv[1];
				val_y1 = uyv[4];
				val_u0 = (uyv[0] + uyv[3]) >> 1;
				val_v0 = (uyv[2] + uyv[5]) >> 1;
				puyvy[0] =
					(val_u0 << 24) | (val_y0 << 16) |
					(val_v0 << 8) | val_y1;
				puyv += 6;
				puyvy++;
				count = (w - 1) >> 2;
				left = (w - 1) - (count << 2);
			} else {
				count = w >> 2;
				left = w - (count << 2);
			}

			al_addr = vis_alignaddr(puyv, 0);
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
				MLIB_SPLIT3_U8(w_u, w_y, w_v, w_ld0, w_ld1,
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

				((mlib_d64 *)puyvy)[2 * j] =
					VIS_FPMERGE_HI(w_uv, w_y);
				((mlib_d64 *)puyvy)[2 * j + 1] =
					VIS_FPMERGE_LO(w_uv, w_y);
			}

			if (left) {
				mlib_d64 res_buf[2];

				l1 = vis_ld_d64_nf(al_addr); al_addr++;
				l2 = vis_ld_d64_nf(al_addr); al_addr++;
				l3 = vis_ld_d64_nf(al_addr);
				w_ld0 = vis_faligndata(l0, l1);
				w_ld1 = vis_faligndata(l1, l2);
				w_ld2 = vis_faligndata(l2, l3);

				MLIB_SPLIT3_U8(w_u, w_y, w_v, w_ld0, w_ld1,
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

				res_buf[0] = VIS_FPMERGE_HI(w_uv, w_y);
				res_buf[1] = VIS_FPMERGE_LO(w_uv, w_y);

				for (j = 0; j < left; j++) {
					((mlib_f32 *)puyvy)[4 * count + j] =
						((mlib_f32 *)res_buf)[j];
				}
			}

			count = w >> 2;
			left = w - (count << 2);
		}
	}
}

/* *********************************************************** */
