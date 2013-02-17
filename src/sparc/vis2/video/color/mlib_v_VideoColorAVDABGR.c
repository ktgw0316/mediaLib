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

#pragma ident	"@(#)mlib_v_VideoColorAVDABGR.c	9.4	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorRGBseq_to_ABGRint - convert RGB sequential to ABGR
 *                                         interleaved
 *
 * SYNOPSIS
 *
 *      void mlib_VideoColorRGBseq_to_ABGRint(mlib_u32      *abgr,
 *                                            const mlib_u8 *r,
 *                                            const mlib_u8 *g,
 *                                            const mlib_u8 *b,
 *                                            const mlib_u8 *a_array,
 *                                            mlib_u8       a_const,
 *                                            mlib_s32      w,
 *                                            mlib_s32      h,
 *                                            mlib_s32      dlb,
 *                                            mlib_s32      slb);
 * ARGUMENTS
 *      abgr        pointer to output buffer
 *      r           pointer to input R buffer
 *      g           pointer to input G buffer
 *      b           pointer to input B buffer
 *      a           pointer to array of alpha values
 *      alpha       constant alpha value
 *      width       image width in pixels
 *      height      image height in lines
 *      dst_stride  linebytes for output buffer
 *      src_stride  linebytes for input buffers
 *
 * DESCRIPTION
 *      The R, G, and B streams, and the A values are combined into
 *      a A, B, G, R interleaved byte stream. Within each 32-bit output word,
 *      the component ordering is A (bits 31-24), B (bits 23-16),
 *      G (bits 15-8), and R (bits 7-0).
 *      The alpha values for this function work in the following fashion:
 *      - if a pointer is not NULL the values are taken from there.  It
 *      has to have the same dimensions as the R, G, and B buffers.
 *      - if a pointer is NULL, the alpha values for every pixel are set
 *      to alpha.
 */

/*
 * FUNCTION
 *      mlib_VideoColorRGBint_to_ABGRint - convert RGB interleaved to ABGR
 *                                         interleaved
 *
 * SYNOPSIS
 *
 *      void mlib_VideoColorRGBint_to_ABGRint(mlib_u32      *abgr,
 *                                            const mlib_u8 *rgb,
 *                                            const mlib_u8 *a_array,
 *                                            mlib_u8       a_const,
 *                                            mlib_s32      w,
 *                                            mlib_s32      h,
 *                                            mlib_s32      dlb,
 *                                            mlib_s32      slb,
 *                                            mlib_s32      alb);
 *
 * ARGUMENTS
 *      abgr        pointer to output buffer
 *      rgb         pointer to input buffer
 *      a           pointer to array of alpha values
 *      alpha       constant alpha value
 *      width       image width in pixels
 *      height      image height in lines
 *      dst_stride  linebytes for output buffer
 *      src_stride  linebytes for input buffer
 *      a_stride    linebytes for alpha buffer
 *
 * DESCRIPTION
 *      The interleaved RGB stream, and the A values are combined into
 *      a A, B, G, R interleaved byte stream. Within each 24-bit input pixel,
 *      the component ordering is R (bits 23-16), G (bits 15-8), and
 *      B (bits 7-0).
 *      Within each 32-bit output word, the component ordering is
 *      A (bits 31-24), B (bits 23-16), G (bits 15-8), and R (bits 7-0).
 *      The alpha values for this function work in the following fashion:
 *      - if a pointer is not NULL the values are taken from there.  It
 *      has to have the same dimensions as the R, G, and B buffers.
 *      - if a pointer is NULL, the alpha values for every pixel are set
 *      to alpha.
 */

/*
 * FUNCTION
 *      mlib_VideoColorBGRint_to_ABGRint - convert BGR interleaved to ABGR
 *                                         interleaved
 *
 * SYNOPSIS
 *
 *      void mlib_VideoColorBGRint_to_ABGRint(mlib_u32      *abgr,
 *                                            const mlib_u8 *bgr,
 *                                            const mlib_u8 *a_array,
 *                                            mlib_u8       a_const,
 *                                            mlib_s32      w,
 *                                            mlib_s32      h,
 *                                            mlib_s32      dlb,
 *                                            mlib_s32      slb,
 *                                            mlib_s32      alb);
 *
 * ARGUMENTS
 *      abgr        pointer to output buffer
 *      bgr         pointer to input buffer
 *      a           pointer to array of alpha values
 *      alpha       constant alpha value
 *      width       image width in pixels
 *      height      image height in lines
 *      dst_stride  linebytes for output buffer
 *      src_stride  linebytes for input buffer
 *      a_stride    linebytes for alpha buffer
 *
 * DESCRIPTION
 *      The interleaved BGR stream, and the A values are combined into
 *      a A, B, G, R interleaved byte stream. Within each 24-bit input pixel,
 *      the component ordering is B (bits 23-16), G (bits 15-8), and
 *      R (bits 7-0).
 *      Within each 32-bit output word, the component ordering is A
 *      (bits 31-24), B (bits 23-16), G (bits 15-8), and R (bits 7-0).
 *      The alpha values for this function work in the following fashion:
 *      - if a pointer is not NULL the values are taken from there.
 *      It has to have the same dimensions as the R, G, and B buffers.
 *      - if a pointer is NULL, the alpha values for every pixel are set
 *      to alpha.
 */

/*
 * FUNCTION
 *      mlib_VideoColorRGBint_to_BGRAint - convert RGB interleaved to BGRA
 *                                         interleaved
 *
 * SYNOPSIS
 *     void mlib_VideoColorRGBint_to_BGRAint(mlib_u8       *bgra,
 *                                           const mlib_u8 *rgb,
 *                                           const mlib_u8 *a,
 *                                           mlib_u8       alpha,
 *                                           mlib_s32      width,
 *                                           mlib_s32      height,
 *                                           mlib_s32      dst_stride,
 *                                           mlib_s32      src_stride,
 *                                           mlib_s32      a_stride)
 *
 * ARGUMENTS
 *      bgra        pointer to output buffer
 *      rgb         pointer to input buffer
 *      a           pointer to array of alpha values
 *      alpha       constant alpha value
 *      width       image width in pixels
 *      height      image height in lines
 *      dst_stride  linebytes for output buffer
 *      src_stride  linebytes for input buffer
 *      a_stride    linebytes for alpha buffer
 *
 * DESCRIPTION
 *      The interleaved RGB stream, and the A values are combined into
 *      a BGRA interleaved byte stream.
 *
 *      The alpha values for this function work in the following fashion:
 *      - If the `a' pointer is not NULL the values are taken from there.
 *        It has to have at least 1/3 the dimension of the `rgb' buffer.
 *      - If the `a' pointer is NULL, the alpha values for every pixel are
 *        set to `alpha'.
 *
 *      In other words, this function's inner loop works like this:
 *
 *        bgra[0] = rgb[2];
 *        bgra[1] = rgb[1];
 *        bgra[2] = rgb[0];
 *        bgra[3] = (a == NULL) ? alpha : a[0];
 */

/*
 * FUNCTION
 *      mlib_VideoColorABGRint_to_ARGBint - convert ABGR interleaved to ARGB
 *
 * SYNOPSIS
 *
 *      void mlib_VideoColorABGRint_to_ARGBint(mlib_u32       *argb,
 *                                             const mlib_u32 *abgr,
 *                                             mlib_s32       w,
 *                                             mlib_s32       h,
 *                                             mlib_s32       dlb,
 *                                             mlib_s32       slb);
 *
 * ARGUMENTS
 *      argb        pointer to output buffer
 *      abgr        pointer to input buffer
 *      width       image width in pixels
 *      height      image height in lines
 *      dst_stride  linebytes for output buffer
 *      src_stride  linebytes for input buffer
 *
 * DESCRIPTION
 *      The ABGR pixel stream is broken apart and recombined into a ARGB
 *      pixel stream. All pixel components are 8-bit unsigned integers.
 *      The buffers has dimensions width and height.
 *      Within each 32-bit input word, the component ordering is
 *      A (bits 31-24), B (bits 23-16), G (bits 15-8), and R (bits 7-0).
 *      Within each 32-bit output word, the component ordering is
 *      A (bits 31-24), R (bits 23-16), G (bits 15-8), and B (bits 7-0).
 */

/*
 * FUNCTION
 *      mlib_VideoColorRGBAint_to_ABGRint - convert RGBA interleaved to ABGR
 *
 * SYNOPSIS
 *
 *      void mlib_VideoColorRGBAint_to_ABGRint(mlib_u32       *rgba,
 *                                             const mlib_u32 *abgr,
 *                                             mlib_s32       w,
 *                                             mlib_s32       h,
 *                                             mlib_s32       dlb,
 *                                             mlib_s32       slb);
 *
 * ARGUMENTS
 *      abgr        pointer to output buffer
 *      rgba        pointer to input buffer
 *      width       image width in pixels
 *      height      image height in lines
 *      dst_stride  linebytes for output buffer
 *      src_stride  linebytes for input buffer
 *
 * DESCRIPTION
 *      The RGBA pixel stream is broken apart and recombined into a ABGR
 *      pixel stream. All pixel components are 8-bit unsigned integers.
 *      The buffers has dimensions width and height.
 *      Within each 32-bit input word, the component ordering is
 *      R (bits 31-24), G (bits 23-16), B (bits 15-8), and A (bits 7-0).
 *      Within each 32-bit output word, the component ordering is
 *      A (bits 31-24), B (bits 23-16), G (bits 15-8), and R (bits 7-0).
 */

/*
 * FUNCTION
 *      mlib_VideoColorBGRAint_to_ABGRint - convert BGRA interleaved to ABGR
 *
 * SYNOPSIS
 *
 *      void mlib_VideoColorBGRAint_to_ABGRint(mlib_u32       *abgr,
 *                                             const mlib_u32 *bgra,
 *                                             mlib_s32       w,
 *                                             mlib_s32       h,
 *                                             mlib_s32       dlb,
 *                                             mlib_s32       slb);
 *
 * ARGUMENTS
 *      abgr        pointer to output buffer
 *      bgra        pointer to input buffer
 *      width       image width in pixels
 *      height      image height in lines
 *      dst_stride  linebytes for output buffer
 *      src_stride  linebytes for input buffer
 *
 * DESCRIPTION
 *      The BGRA pixel stream is broken apart and recombined into a ABGR
 *      pixel stream. All pixel components are 8-bit unsigned integers.
 *      The buffers has dimensions width and height.
 *      Within each 32-bit input word, the component ordering is
 *      B (bits 31-24), G (bits 23-16), R (bits 15-8), and A (bits 7-0).
 *      Within each 32-bit output word, the component ordering is
 *      A (bits 31-24), B (bits 23-16), G (bits 15-8), and R (bits 7-0).
 */

/*
 * FUNCTION
 *      mlib_VideoColorRGBXint_to_ABGRint - convert RGBX interleaved to ABGR
 *                                          interleaved
 *
 * SYNOPSIS
 *
 *      void mlib_VideoColorRGBXint_to_ABGRint(mlib_u32       *abgr,
 *                                             const mlib_u32 *rgbx,
 *                                             const mlib_u8  *a_array,
 *                                             mlib_u8        a_const,
 *                                             mlib_s32       w,
 *                                             mlib_s32       h,
 *                                             mlib_s32       dlb,
 *                                             mlib_s32       slb,
 *                                             mlib_s32       alb);
 *
 * ARGUMENTS
 *      abgr        pointer to output buffer
 *      rgbx        pointer to input buffer
 *      a           pointer to array of alpha values
 *      alpha       constant alpha value
 *      width       image width in pixels
 *      height      image height in lines
 *      dst_stride  linebytes for output buffer
 *      src_stride  linebytes for input buffer
 *      a_stride    linebytes for alpha buffer
 *
 * DESCRIPTION
 *      The interleaved RGBX stream and the alpha values are combined into
 *      an interleaved A, B, G, R output stream. Within each 32-bit input
 *      pixel, the component ordering is R (bits 31-24), G (bits 23-16),
 *      and B (bits 15-8). Within each 32-bit output pixel, the component
 *      ordering is A (bits 31-24), B (bits 23-16), G (bits 15-8), and
 *      R (bits 7-0). The alpha values for this function work in the following
 *      fashion: if 'a' pointer is not NULL the values are taken from the
 *      corresponding locations in the alpha array, otherwise a constant
 *      alpha value, specified by 'alpha', is store in each output pixel.
 *      Each element in the alpha array is an unsigned byte.
 *      width and height define the dimensions of the region of the buffers
 *      to be processed. The linebyte parameters are used to advance the data
 *      pointers for each of the buffers.
 */

/*
 * FUNCTION
 *      mlib_VideoColorRGBXint_to_ARGBint - convert RGBX interleaved to ARGB
 *                                          interleaved
 *
 * SYNOPSIS
 *
 *      void mlib_VideoColorRGBXint_to_ARGBint(mlib_u32       *argb,
 *                                             const mlib_u32 *rgbx,
 *                                             const mlib_u8  *a_array,
 *                                             mlib_u8        a_const,
 *                                             mlib_s32       w,
 *                                             mlib_s32       h,
 *                                             mlib_s32       dlb,
 *                                             mlib_s32       slb,
 *                                             mlib_s32       alb);
 *
 * ARGUMENTS
 *      argb        pointer to output buffer
 *      rgbx        pointer to input buffer
 *      a           pointer to array of alpha values
 *      alpha       constant alpha value
 *      width       image width in pixels
 *      height      image height in lines
 *      dst_stride  linebytes for output buffer
 *      src_stride  linebytes for input buffer
 *      a_stride    linebytes for alpha buffer
 *
 * DESCRIPTION
 *      The interleaved RGBX stream and the alpha values are combined into
 *      an interleaved A, R, G, B output stream. Within each 32-bit input
 *      pixel, the component ordering is R (bits 31-24), G (bits 23-16),
 *      and B (bits 15-8). Within each 32-bit output pixel, the component
 *      ordering is A (bits 31-24), R (bits 23-16), G (bits 15-8), and
 *      B (bits 7-0). The alpha values for this function work in the following
 *      fashion: if 'a' pointer is not NULL the values are taken from the
 *      corresponding locations in the alpha array, otherwise a constant
 *      alpha value, specified by 'alpha', is store in each output pixel.
 *      Each element in the alpha array is an unsigned byte.
 *      width and height define the dimensions of the region of the buffers
 *      to be processed. The linebyte parameters are used to advance the data
 *      pointers for each of the buffers.
 */

/*
 * FUNCTION
 *      mlib_VideoColorXRGBint_to_ABGRint - convert XRGB interleaved to ABGR
 *                                          interleaved
 *
 * SYNOPSIS
 *
 *      void mlib_VideoColorXRGBint_to_ABGRint(mlib_u32       *abgr,
 *                                             const mlib_u32 *xrgb,
 *                                             const mlib_u8  *a_array,
 *                                             mlib_u8        a_const,
 *                                             mlib_s32       w,
 *                                             mlib_s32       h,
 *                                             mlib_s32       dlb,
 *                                             mlib_s32       slb,
 *                                             mlib_s32       alb);
 *
 * ARGUMENTS
 *      abgr        pointer to output buffer
 *      xrgb        pointer to input buffer
 *      a           pointer to array of alpha values
 *      alpha       constant alpha value
 *      width       image width in pixels
 *      height      image height in lines
 *      dst_stride  linebytes for output buffer
 *      src_stride  linebytes for input buffer
 *      a_stride    linebytes for alpha buffer
 *
 * DESCRIPTION
 *      The interleaved XRGB stream and the alpha values are combined into
 *      an interleaved A, B, G, R output stream. Within each 32-bit input
 *      pixel, the component ordering is R (bits 23-16), G (bits 15-8),
 *      and B (bits 7-0). Within each 32-bit output pixel, the component
 *      ordering is A (bits 31-24), B (bits 23-16), G (bits 15-8), and
 *      R (bits 7-0). The alpha values for this function work in the following
 *      fashion: if 'a' pointer is not NULL the values are taken from the
 *      corresponding locations in the alpha array, otherwise a constant
 *      alpha value, specified by 'alpha', is store in each output pixel.
 *      Each element in the alpha array is an unsigned byte.
 *      width and height define the dimensions of the region of the buffers
 *      to be processed. The linebyte parameters are used to advance the data
 *      pointers for each of the buffers.
 */

/*
 * FUNCTION
 *      mlib_VideoColorXRGBint_to_ARGBint - convert XRGB interleaved to ARGB
 *                                          interleaved
 *
 * SYNOPSIS
 *
 *      void mlib_VideoColorXRGBint_to_ARGBint(mlib_u32       *argb,
 *                                             const mlib_u32 *xrgb,
 *                                             const mlib_u8  *a_array,
 *                                             mlib_u8        a_const,
 *                                             mlib_s32       w,
 *                                             mlib_s32       h,
 *                                             mlib_s32       dlb,
 *                                             mlib_s32       slb,
 *                                             mlib_s32       alb);
 *
 * ARGUMENTS
 *      argb        pointer to output buffer
 *      xrgb        pointer to input buffer
 *      a           pointer to array of alpha values
 *      alpha       constant alpha value
 *      width       image width in pixels
 *      height      image height in lines
 *      dst_stride  linebytes for output buffer
 *      src_stride  linebytes for input buffer
 *      a_stride    linebytes for alpha buffer
 *
 * DESCRIPTION
 *      The interleaved XRGB stream and the alpha values are combined into
 *      an interleaved A, R, G, B output stream. Within each 32-bit input
 *      pixel, the component ordering is R (bits 23-16), G (bits 15-8),
 *      and B (bits 7-0). Within each 32-bit output pixel, the component
 *      ordering is A (bits 31-24), R (bits 23-16), G (bits 15-8), and
 *      B (bits 7-0). The alpha values for this function work in the following
 *      fashion: if 'a' pointer is not NULL the values are taken from the
 *      corresponding locations in the alpha array, otherwise a constant
 *      alpha value, specified by 'alpha', is store in each output pixel.
 *      Each element in the alpha array is an unsigned byte.
 *      width and height define the dimensions of the region of the buffers
 *      to be processed. The linebyte parameters are used to advance the data
 *      pointers for each of the buffers.
 */

#include <mlib_video.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VideoColorABGRint_to_ARGBint = \
	__mlib_VideoColorABGRint_to_ARGBint
#pragma weak mlib_VideoColorBGRAint_to_ABGRint = \
	__mlib_VideoColorBGRAint_to_ABGRint
#pragma weak mlib_VideoColorBGRint_to_ABGRint = \
	__mlib_VideoColorBGRint_to_ABGRint
#pragma weak mlib_VideoColorRGBAint_to_ABGRint = \
	__mlib_VideoColorRGBAint_to_ABGRint
#pragma weak mlib_VideoColorRGBXint_to_ABGRint = \
	__mlib_VideoColorRGBXint_to_ABGRint
#pragma weak mlib_VideoColorRGBXint_to_ARGBint = \
	__mlib_VideoColorRGBXint_to_ARGBint
#pragma weak mlib_VideoColorRGBint_to_ABGRint = \
	__mlib_VideoColorRGBint_to_ABGRint
#pragma weak mlib_VideoColorRGBint_to_BGRAint = \
	__mlib_VideoColorRGBint_to_BGRAint
#pragma weak mlib_VideoColorRGBseq_to_ABGRint = \
	__mlib_VideoColorRGBseq_to_ABGRint
#pragma weak mlib_VideoColorXRGBint_to_ABGRint = \
	__mlib_VideoColorXRGBint_to_ABGRint
#pragma weak mlib_VideoColorXRGBint_to_ARGBint = \
	__mlib_VideoColorXRGBint_to_ARGBint

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_VideoColorABGRint_to_ARGBint)
	mlib_VideoColorABGRint_to_ARGBint
	__attribute__((weak, alias("__mlib_VideoColorABGRint_to_ARGBint")));
__typeof__(__mlib_VideoColorBGRAint_to_ABGRint)
	mlib_VideoColorBGRAint_to_ABGRint
	__attribute__((weak, alias("__mlib_VideoColorBGRAint_to_ABGRint")));
__typeof__(__mlib_VideoColorBGRint_to_ABGRint) mlib_VideoColorBGRint_to_ABGRint
	__attribute__((weak, alias("__mlib_VideoColorBGRint_to_ABGRint")));
__typeof__(__mlib_VideoColorRGBAint_to_ABGRint)
	mlib_VideoColorRGBAint_to_ABGRint
	__attribute__((weak, alias("__mlib_VideoColorRGBAint_to_ABGRint")));
__typeof__(__mlib_VideoColorRGBXint_to_ABGRint)
	mlib_VideoColorRGBXint_to_ABGRint
	__attribute__((weak, alias("__mlib_VideoColorRGBXint_to_ABGRint")));
__typeof__(__mlib_VideoColorRGBXint_to_ARGBint)
	mlib_VideoColorRGBXint_to_ARGBint
	__attribute__((weak, alias("__mlib_VideoColorRGBXint_to_ARGBint")));
__typeof__(__mlib_VideoColorRGBint_to_ABGRint) mlib_VideoColorRGBint_to_ABGRint
	__attribute__((weak, alias("__mlib_VideoColorRGBint_to_ABGRint")));
__typeof__(__mlib_VideoColorRGBint_to_BGRAint) mlib_VideoColorRGBint_to_BGRAint
	__attribute__((weak, alias("__mlib_VideoColorRGBint_to_BGRAint")));
__typeof__(__mlib_VideoColorRGBseq_to_ABGRint) mlib_VideoColorRGBseq_to_ABGRint
	__attribute__((weak, alias("__mlib_VideoColorRGBseq_to_ABGRint")));
__typeof__(__mlib_VideoColorXRGBint_to_ABGRint)
	mlib_VideoColorXRGBint_to_ABGRint
	__attribute__((weak, alias("__mlib_VideoColorXRGBint_to_ABGRint")));
__typeof__(__mlib_VideoColorXRGBint_to_ARGBint)
	mlib_VideoColorXRGBint_to_ARGBint
	__attribute__((weak, alias("__mlib_VideoColorXRGBint_to_ARGBint")));

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

void
__mlib_VideoColorABGRint_to_ARGBint(
	mlib_u32 *argb,
	const mlib_u32 *abgr,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb)
{
	mlib_s32 i, j, count = w >> 1, left = w & 1;
	mlib_d64 w_tmp0, w_tmp1, w_tmp2, w_tmp3, w_tmp4, *d_abgr, *d_argb;

	dlb >>= 2;
	slb >>= 2;

	for (i = 0; i < h; i++, argb += dlb, abgr += slb) {
		if ((((mlib_addr)argb | (mlib_addr)abgr) & 7) == 0) {
			d_abgr = (mlib_d64 *)abgr;
			d_argb = (mlib_d64 *)argb;

			vis_write_bmask(0x03214765, 0);
#pragma pipeloop(0)
			for (j = 0; j < count - 3; j += 4) {
				w_tmp1 = d_abgr[j];
				w_tmp2 = d_abgr[j + 1];
				w_tmp3 = d_abgr[j + 2];
				w_tmp4 = d_abgr[j + 3];
				d_argb[j] = vis_bshuffle(w_tmp1, 0);
				d_argb[j + 1] = vis_bshuffle(w_tmp2, 0);
				d_argb[j + 2] = vis_bshuffle(w_tmp3, 0);
				d_argb[j + 3] = vis_bshuffle(w_tmp4, 0);
			}

#pragma pipeloop(0)
			for (; j < count; j++) {
				d_argb[j] = vis_bshuffle(d_abgr[j], 0);
			}

			if (left) {
				((mlib_f32 *)argb)[2 * count] =
					vis_read_hi(vis_bshuffle(d_abgr[count],
					0));
			}
		} else {
			mlib_s32 off, count0, left0;
			mlib_u32 *pabgr = (void *)abgr, *pargb = argb;

			if ((mlib_addr)pargb & 7) {
				mlib_u32 ld0 = (*pabgr++);

				(*pargb++) =
					(ld0 & ~0xFF00FF) | ((ld0 & 0xFF) << 16)
					| ((ld0 & 0xFF0000) >> 16);
				count0 = (w - 1) >> 1;
				left0 = (w - 1) & 1;
			} else {
				count0 = count;
				left0 = left;
			}

			off = (mlib_addr)pabgr & 7;
			d_abgr = (mlib_d64 *)((mlib_u8 *)pabgr - off);
			d_argb = (mlib_d64 *)pargb;
			vis_write_bmask(0x11111111 * off, 0x03214765);
			w_tmp0 = vis_ld_d64_nf(d_abgr); d_abgr++;
#pragma pipeloop(0)
			for (j = 0; j < count0 - 3; j += 4) {
				w_tmp1 = d_abgr[j];
				w_tmp2 = d_abgr[j + 1];
				w_tmp3 = d_abgr[j + 2];
				w_tmp4 = vis_ld_d64_nf(d_abgr + j + 3);
				d_argb[j] = vis_bshuffle(w_tmp0, w_tmp1);
				d_argb[j + 1] = vis_bshuffle(w_tmp1, w_tmp2);
				d_argb[j + 2] = vis_bshuffle(w_tmp2, w_tmp3);
				d_argb[j + 3] = vis_bshuffle(w_tmp3, w_tmp4);
				w_tmp0 = w_tmp4;
			}

#pragma pipeloop(0)
			for (; j < count0; j++) {
				w_tmp1 = vis_ld_d64_nf(d_abgr +j);
				d_argb[j] = vis_bshuffle(w_tmp0, w_tmp1);
				w_tmp0 = w_tmp1;
			}

			if (left0) {
				w_tmp1 = vis_ld_d64_nf(d_abgr + j);
				((mlib_f32 *)pargb)[2 * count0] =
					vis_read_hi(vis_bshuffle(w_tmp0,
					w_tmp1));
			}
		}
	}
}

/* *********************************************************** */

void
__mlib_VideoColorBGRAint_to_ABGRint(
	mlib_u32 *abgr,
	const mlib_u32 *bgra,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb)
{
	mlib_s32 i, j, count = w >> 1, left = w & 1;
	mlib_d64 w_tmp0, w_tmp1, w_tmp2, w_tmp3, w_tmp4, *d_bgra, *d_abgr;

	dlb >>= 2;
	slb >>= 2;

	for (i = 0; i < h; i++, abgr += dlb, bgra += slb) {
		if ((((mlib_addr)abgr | (mlib_addr)bgra) & 7) == 0) {
			d_bgra = (mlib_d64 *)bgra;
			d_abgr = (mlib_d64 *)abgr;

			vis_write_bmask(0x30127456, 0);
#pragma pipeloop(0)
			for (j = 0; j < count - 3; j += 4) {
				w_tmp1 = d_bgra[j];
				w_tmp2 = d_bgra[j + 1];
				w_tmp3 = d_bgra[j + 2];
				w_tmp4 = d_bgra[j + 3];
				d_abgr[j] = vis_bshuffle(w_tmp1, 0);
				d_abgr[j + 1] = vis_bshuffle(w_tmp2, 0);
				d_abgr[j + 2] = vis_bshuffle(w_tmp3, 0);
				d_abgr[j + 3] = vis_bshuffle(w_tmp4, 0);
			}

#pragma pipeloop(0)
			for (; j < count; j++) {
				d_abgr[j] = vis_bshuffle(d_bgra[j], 0);
			}

			if (left) {
				((mlib_f32 *)abgr)[2 * count] =
					vis_read_hi(vis_bshuffle(d_bgra[count],
					0));
			}
		} else {
			mlib_s32 off, count0, left0;
			mlib_u32 *pbgra = (void *)bgra, *pabgr = (void *)abgr;

			if ((mlib_addr)pabgr & 7) {
				mlib_u32 ld0 = (*pbgra++);

				(*pabgr++) =
					((ld0 & ~0xFF) >> 8) | ((ld0 & 0xFF) <<
					24);
				count0 = (w - 1) >> 1;
				left0 = (w - 1) & 1;
			} else {
				count0 = count;
				left0 = left;
			}

			off = (mlib_addr)pbgra & 7;
			d_bgra = (mlib_d64 *)((mlib_u8 *)pbgra - off);
			d_abgr = (mlib_d64 *)pabgr;
			vis_write_bmask(0x11111111 * off, 0x30127456);
			w_tmp0 = vis_ld_d64_nf(d_bgra); d_bgra++;
#pragma pipeloop(0)
			for (j = 0; j < count0 - 3; j += 4) {
				w_tmp1 = d_bgra[j];
				w_tmp2 = d_bgra[j + 1];
				w_tmp3 = d_bgra[j + 2];
				w_tmp4 = vis_ld_d64_nf(d_bgra + j + 3);
				d_abgr[j] = vis_bshuffle(w_tmp0, w_tmp1);
				d_abgr[j + 1] = vis_bshuffle(w_tmp1, w_tmp2);
				d_abgr[j + 2] = vis_bshuffle(w_tmp2, w_tmp3);
				d_abgr[j + 3] = vis_bshuffle(w_tmp3, w_tmp4);
				w_tmp0 = w_tmp4;
			}

#pragma pipeloop(0)
			for (; j < count0; j++) {
				w_tmp1 = vis_ld_d64_nf(d_bgra + j);
				d_abgr[j] = vis_bshuffle(w_tmp0, w_tmp1);
				w_tmp0 = w_tmp1;
			}

			if (left0) {
				w_tmp1 = vis_ld_d64_nf(d_bgra + j);
				((mlib_f32 *)pabgr)[2 * count0] =
					vis_read_hi(vis_bshuffle(w_tmp0,
					w_tmp1));
			}
		}
	}
}

/* *********************************************************** */

void
__mlib_VideoColorRGBAint_to_ABGRint(
	mlib_u32 *rgba,
	const mlib_u32 *abgr,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb)
{
	mlib_s32 i, j, count = w >> 1, left = w & 1;
	mlib_d64 w_tmp0, w_tmp1, w_tmp2, w_tmp3, w_tmp4, *d_abgr, *d_rgba;

	dlb >>= 2;
	slb >>= 2;

	for (i = 0; i < h; i++, rgba += dlb, abgr += slb) {
		if ((((mlib_addr)rgba | (mlib_addr)abgr) & 7) == 0) {
			d_abgr = (mlib_d64 *)abgr;
			d_rgba = (mlib_d64 *)rgba;

			vis_write_bmask(0x32107654, 0);
#pragma pipeloop(0)
			for (j = 0; j < count - 3; j += 4) {
				w_tmp1 = d_abgr[j];
				w_tmp2 = d_abgr[j + 1];
				w_tmp3 = d_abgr[j + 2];
				w_tmp4 = d_abgr[j + 3];
				d_rgba[j] = vis_bshuffle(w_tmp1, 0);
				d_rgba[j + 1] = vis_bshuffle(w_tmp2, 0);
				d_rgba[j + 2] = vis_bshuffle(w_tmp3, 0);
				d_rgba[j + 3] = vis_bshuffle(w_tmp4, 0);
			}

#pragma pipeloop(0)
			for (; j < count; j++) {
				d_rgba[j] = vis_bshuffle(d_abgr[j], 0);
			}

			if (left) {
				((mlib_f32 *)rgba)[2 * count] =
					vis_read_hi(vis_bshuffle(d_abgr[count],
					0));
			}
		} else {
			mlib_s32 off, count0, left0;
			mlib_u32 *pabgr = (void *)abgr, *prgba = (void *)rgba;

			if ((mlib_addr)prgba & 7) {
				mlib_u32 ld0 = (*pabgr++);

				(*prgba++) =
					((ld0 & 0xFF) << 24) | ((ld0 & 0xFF00)
					<< 8) | ((ld0 & 0xFF0000) >> 8)
					| (ld0 >> 24);
				count0 = (w - 1) >> 1;
				left0 = (w - 1) & 1;
			} else {
				count0 = count;
				left0 = left;
			}

			off = (mlib_addr)pabgr & 7;
			d_abgr = (mlib_d64 *)((mlib_u8 *)pabgr - off);
			d_rgba = (mlib_d64 *)prgba;
			vis_write_bmask(0x11111111 * off, 0x32107654);
			w_tmp0 = vis_ld_d64_nf(d_abgr); d_abgr++;
#pragma pipeloop(0)
			for (j = 0; j < count0 - 3; j += 4) {
				w_tmp1 = d_abgr[j];
				w_tmp2 = d_abgr[j + 1];
				w_tmp3 = d_abgr[j + 2];
				w_tmp4 = vis_ld_d64_nf(d_abgr + j + 3);
				d_rgba[j] = vis_bshuffle(w_tmp0, w_tmp1);
				d_rgba[j + 1] = vis_bshuffle(w_tmp1, w_tmp2);
				d_rgba[j + 2] = vis_bshuffle(w_tmp2, w_tmp3);
				d_rgba[j + 3] = vis_bshuffle(w_tmp3, w_tmp4);
				w_tmp0 = w_tmp4;
			}

#pragma pipeloop(0)
			for (; j < count0; j++) {
				w_tmp1 = vis_ld_d64_nf(d_abgr + j);
				d_rgba[j] = vis_bshuffle(w_tmp0, w_tmp1);
				w_tmp0 = w_tmp1;
			}

			if (left0) {
				w_tmp1 = vis_ld_d64_nf(d_abgr + j);
				((mlib_f32 *)prgba)[2 * count0] =
					vis_read_hi(vis_bshuffle(w_tmp0,
					w_tmp1));
			}
		}
	}
}

/* *********************************************************** */

#define	UNALIGNLOAD_64(var, addr)                               \
	al_addr = vis_alignaddr(addr, 0);                       \
	var = vis_faligndata(*al_addr, *(al_addr + 1))

/* *********************************************************** */

void
__mlib_VideoColorRGBseq_to_ABGRint(
	mlib_u32 *abgr,
	const mlib_u8 *r,
	const mlib_u8 *g,
	const mlib_u8 *b,
	const mlib_u8 *a_array,
	mlib_u8 a_const,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb)
{
	mlib_s32 i, j, count = w >> 3, left = w - (count << 3);
	mlib_u32 val;
	mlib_d64 ld_a0, ld_r0, ld_b0, ld_g0, ld_a1, ld_r1, ld_b1, ld_g1;
	mlib_d64 *dpa, *dpr, *dpb, *dpg;
	mlib_s32 off;

	if (h == 0 || w == 0)
		return;

	dlb >>= 2;

	if (a_array) {
		for (i = 0; i < h;
			i++, a_array += slb, b += slb, g += slb, r +=
			slb, abgr += dlb) {

			if ((((mlib_addr)a_array | (mlib_addr)b | (mlib_addr)g |
				(mlib_addr)r | (mlib_addr)abgr) & 7) == 0) {
				mlib_d64 w_a, w_b, w_g, w_r, w_ag0, w_ag1,
					w_br0, w_br1;
				mlib_d64 w_res0, w_res1, w_res2, w_res3;

				count = w >> 3;
				left = w - (count << 3);

#pragma pipeloop(0)
				for (j = 0; j < count; j++) {
					w_a = ((mlib_d64 *)a_array)[j];
					w_b = ((mlib_d64 *)b)[j];
					w_g = ((mlib_d64 *)g)[j];
					w_r = ((mlib_d64 *)r)[j];
					w_ag0 = VIS_FPMERGE_HI(w_a, w_g);
					w_ag1 = VIS_FPMERGE_LO(w_a, w_g);
					w_br0 = VIS_FPMERGE_HI(w_b, w_r);
					w_br1 = VIS_FPMERGE_LO(w_b, w_r);
					w_res0 = VIS_FPMERGE_HI(w_ag0, w_br0);
					w_res1 = VIS_FPMERGE_LO(w_ag0, w_br0);
					w_res2 = VIS_FPMERGE_HI(w_ag1, w_br1);
					w_res3 = VIS_FPMERGE_LO(w_ag1, w_br1);

					((mlib_d64 *)abgr)[4 * j] = w_res0;
					((mlib_d64 *)abgr)[4 * j + 1] = w_res1;
					((mlib_d64 *)abgr)[4 * j + 2] = w_res2;
					((mlib_d64 *)abgr)[4 * j + 3] = w_res3;
				}

				if (left) {
					mlib_d64 res_buf[4];

					w_a = vis_ld_d64_nf(
						(mlib_d64 *)a_array +
						count);
					w_b = vis_ld_d64_nf((mlib_d64 *)b +
						count);
					w_r = vis_ld_d64_nf((mlib_d64 *)r +
						count);
					w_g = vis_ld_d64_nf((mlib_d64 *)g +
						count);
					w_ag0 = VIS_FPMERGE_HI(w_a, w_g);
					w_ag1 = VIS_FPMERGE_LO(w_a, w_g);
					w_br0 = VIS_FPMERGE_HI(w_b, w_r);
					w_br1 = VIS_FPMERGE_LO(w_b, w_r);
					res_buf[0] =
						VIS_FPMERGE_HI(w_ag0, w_br0);
					res_buf[1] =
						VIS_FPMERGE_LO(w_ag0, w_br0);
					res_buf[2] =
						VIS_FPMERGE_HI(w_ag1, w_br1);
					res_buf[3] =
						VIS_FPMERGE_LO(w_ag1, w_br1);
					for (j = 0; j < left; j++) {
						((mlib_f32 *)abgr)[8 * count +
							j] =
							((mlib_f32 *)
							res_buf)[j];
					}
				}
			} else {
				mlib_u32 *pabgr = abgr;
				mlib_u8 *pr = (void *)r;
				mlib_u8 *pg = (void *)g;
				mlib_u8 *pb = (void *)b;
				mlib_u8 *pa_array = (void *)a_array;

				mlib_d64 w_a, w_b, w_g, w_r, w_ag0, w_ag1,
					w_br0, w_br1;
				mlib_d64 w_res0, w_res1, w_res2, w_res3;

				if ((mlib_addr)pabgr & 7) {
					mlib_s32 val =
						(pa_array[0] << 24) | (pb[0] <<
						16) | (pg[0] << 8) | pr[0];

					pabgr[0] = val;
					pabgr++;
					pr++;
					pg++;
					pb++;
					pa_array++;
					count = (w - 1) >> 3;
					left = (w - 1) - (count << 3);
				} else {
					count = w >> 3;
					left = w - (count << 3);
				}

				dpa = vis_alignaddr(pa_array, 0);
				ld_a0 = vis_ld_d64_nf(dpa); dpa++;
				off = (mlib_addr)pb & 7;
				dpb = (mlib_d64 *)(pb - off);
				vis_write_bmask(0x11111111 * off, 0x01234567);
				ld_b0 = vis_ld_d64_nf(dpb); dpb++;
				dpr = vis_alignaddr(pr, 0);
				ld_r0 = vis_ld_d64_nf(dpr); dpr++;
				dpg = vis_alignaddr(pg, 0);
				ld_g0 = vis_ld_d64_nf(dpg); dpg++;
#pragma pipeloop(0)
#pragma unroll(3)
				for (j = 0; j < count; j++) {
					vis_alignaddr(pa_array, 0);
					ld_a1 = vis_ld_d64_nf(dpa + j);
					w_a = vis_faligndata(ld_a0, ld_a1);
					ld_a0 = ld_a1;
					ld_b1 = vis_ld_d64_nf(dpb + j);
					w_b = vis_bshuffle(ld_b0, ld_b1);
					ld_b0 = ld_b1;
					vis_alignaddr(pr, 0);
					ld_r1 = vis_ld_d64_nf(dpr + j);
					w_r = vis_faligndata(ld_r0, ld_r1);
					ld_r0 = ld_r1;
					vis_alignaddr(pg, 0);
					ld_g1 = vis_ld_d64_nf(dpg + j);
					w_g = vis_faligndata(ld_g0, ld_g1);
					ld_g0 = ld_g1;
					w_ag0 = VIS_FPMERGE_HI(w_a, w_g);
					w_ag1 = VIS_FPMERGE_LO(w_a, w_g);
					w_br0 = VIS_FPMERGE_HI(w_b, w_r);
					w_br1 = VIS_FPMERGE_LO(w_b, w_r);
					w_res0 = VIS_FPMERGE_HI(w_ag0, w_br0);
					w_res1 = VIS_FPMERGE_LO(w_ag0, w_br0);
					w_res2 = VIS_FPMERGE_HI(w_ag1, w_br1);
					w_res3 = VIS_FPMERGE_LO(w_ag1, w_br1);

					((mlib_d64 *)pabgr)[4 * j] = w_res0;
					((mlib_d64 *)pabgr)[4 * j + 1] = w_res1;
					((mlib_d64 *)pabgr)[4 * j + 2] = w_res2;
					((mlib_d64 *)pabgr)[4 * j + 3] = w_res3;
				}

				if (left) {
					pabgr += count * 8;
					pa_array += count * 8;
					pg += count * 8;
					pb += count * 8;
					pr += count * 8;
					val = (pa_array[0] << 24) | (pb[0] <<
						16) | (pg[0] << 8) | pr[0];

#pragma pipeloop(0)
					for (j = 1; j < left; j++) {
						mlib_s32 val1 =
							(pa_array[j] << 24) |
							(pb[j] << 16);
						mlib_s32 val2 =
							(pg[j] << 8) | pr[j];

						pabgr[j - 1] = val;
						val = val1 | val2;
					}

					pabgr[j - 1] = val;
				}
			}
		}
	} else {
		mlib_s32 a_val = a_const << 24;
		mlib_d64 w_alpha =
			vis_to_double_dup(a_const | (a_const << 8) | (a_const <<
			16) | (a_val));

		for (i = 0; i < h;
			i++, b += slb, g += slb, r += slb, abgr += dlb) {
			if ((((mlib_addr)b | (mlib_addr)g | (mlib_addr)r |
				(mlib_addr)abgr) & 7) == 0) {
				mlib_d64 w_a, w_b, w_g, w_r, w_ag0, w_ag1,
					w_br0, w_br1;

				count = w >> 3;
				left = w - (count << 3);

				w_a = w_alpha;
#pragma pipeloop(0)
				for (j = 0; j < count; j++) {
					w_r = ((mlib_d64 *)r)[j];
					w_g = ((mlib_d64 *)g)[j];
					w_b = ((mlib_d64 *)b)[j];
					w_ag0 = VIS_FPMERGE_HI(w_a, w_g);
					w_ag1 = VIS_FPMERGE_LO(w_a, w_g);
					w_br0 = VIS_FPMERGE_HI(w_b, w_r);
					w_br1 = VIS_FPMERGE_LO(w_b, w_r);
					((mlib_d64 *)abgr)[4 * j] =
						VIS_FPMERGE_HI(w_ag0, w_br0);
					((mlib_d64 *)abgr)[4 * j + 1] =
						VIS_FPMERGE_LO(w_ag0, w_br0);
					((mlib_d64 *)abgr)[4 * j + 2] =
						VIS_FPMERGE_HI(w_ag1, w_br1);
					((mlib_d64 *)abgr)[4 * j + 3] =
						VIS_FPMERGE_LO(w_ag1, w_br1);
				}

				if (left) {
					mlib_d64 res_buf[4];

					w_r = vis_ld_d64_nf((mlib_d64 *)r +
						count);
					w_g = vis_ld_d64_nf((mlib_d64 *)g +
						count);
					w_b = vis_ld_d64_nf((mlib_d64 *)b +
						count);
					w_ag0 = VIS_FPMERGE_HI(w_a, w_g);
					w_ag1 = VIS_FPMERGE_LO(w_a, w_g);
					w_br0 = VIS_FPMERGE_HI(w_b, w_r);
					w_br1 = VIS_FPMERGE_LO(w_b, w_r);
					res_buf[0] =
						VIS_FPMERGE_HI(w_ag0, w_br0);
					res_buf[1] =
						VIS_FPMERGE_LO(w_ag0, w_br0);
					res_buf[2] =
						VIS_FPMERGE_HI(w_ag1, w_br1);
					res_buf[3] =
						VIS_FPMERGE_LO(w_ag1, w_br1);
					for (j = 0; j < left; j++) {
						((mlib_f32 *)abgr)[8 * count +
							j] =
							((mlib_f32 *)
							res_buf)[j];
					}
				}
			} else {
				mlib_u32 *pabgr = abgr;
				mlib_u8 *pr = (void *)r, *pg = (void *)g, *pb =
					(void *)b;

				mlib_d64 w_a, w_b, w_g, w_r, w_ag0, w_ag1,
					w_br0, w_br1;
				mlib_d64 w_res0, w_res1, w_res2, w_res3;

				w_a = w_alpha;

				if ((mlib_addr)pabgr & 7) {
					mlib_s32 val =
						a_val | (pb[0] << 16) | (pg[0]
						<< 8) | pr[0];

					pabgr[0] = val;
					pabgr++;
					pr++;
					pg++;
					pb++;
					count = (w - 1) >> 3;
					left = (w - 1) - (count << 3);
				} else {
					count = w >> 3;
					left = w - (count << 3);
				}

				dpr = vis_alignaddr(pr, 0);
				ld_r0 = vis_ld_d64_nf(dpr); dpr++;
				off = (mlib_addr)pb & 7;
				dpb = (mlib_d64 *)(pb - off);
				vis_write_bmask(0x11111111 * off, 0x01234567);
				ld_b0 = vis_ld_d64_nf(dpb); dpb++;
				dpg = vis_alignaddr(pg, 0);
				ld_g0 = vis_ld_d64_nf(dpg); dpg++;
#pragma pipeloop(0)
#pragma unroll(3)
				for (j = 0; j < count; j++) {
					vis_alignaddr(pr, 0);
					ld_r1 = vis_ld_d64_nf(dpr + j);
					w_r = vis_faligndata(ld_r0, ld_r1);
					ld_r0 = ld_r1;
					ld_b1 = vis_ld_d64_nf(dpb + j);
					w_b = vis_bshuffle(ld_b0, ld_b1);
					ld_b0 = ld_b1;
					vis_alignaddr(pg, 0);
					ld_g1 = vis_ld_d64_nf(dpg + j);
					w_g = vis_faligndata(ld_g0, ld_g1);
					ld_g0 = ld_g1;

					w_ag0 = VIS_FPMERGE_HI(w_a, w_g);
					w_ag1 = VIS_FPMERGE_LO(w_a, w_g);
					w_br0 = VIS_FPMERGE_HI(w_b, w_r);
					w_br1 = VIS_FPMERGE_LO(w_b, w_r);
					w_res0 = VIS_FPMERGE_HI(w_ag0, w_br0);
					w_res1 = VIS_FPMERGE_LO(w_ag0, w_br0);
					w_res2 = VIS_FPMERGE_HI(w_ag1, w_br1);
					w_res3 = VIS_FPMERGE_LO(w_ag1, w_br1);

					((mlib_d64 *)pabgr)[4 * j] = w_res0;
					((mlib_d64 *)pabgr)[4 * j + 1] = w_res1;
					((mlib_d64 *)pabgr)[4 * j + 2] = w_res2;
					((mlib_d64 *)pabgr)[4 * j + 3] = w_res3;
				}

				if (left) {
					pabgr += count * 8;
					pg += count * 8;
					pb += count * 8;
					pr += count * 8;
					val = a_val | (pb[0] << 16) | (pg[0] <<
						8) | pr[0];

#pragma pipeloop(0)
					for (j = 1; j < left; j++) {
						mlib_s32 val1 =
							a_val | (pb[j] << 16);
						mlib_s32 val2 =
							(pg[j] << 8) | pr[j];

						pabgr[j - 1] = val;
						val = val1 | val2;
					}

					pabgr[j - 1] = val;
				}
			}
		}
	}
}

/* *********************************************************** */

#define	MERGE_S32_U8(x, a0, a1, a2, a3)                         \
	x = (a0 << 16) | (a1 << 8) | (a2) | (a3)

/* *********************************************************** */

void
__mlib_VideoColorRGBint_to_ABGRint(
	mlib_u32 *abgr,
	const mlib_u8 *rgb,
	const mlib_u8 *a_array,
	mlib_u8 a_const,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb,
	mlib_s32 alb)
{
	mlib_s32 i, j, count = w >> 3, left = w - (count << 3);
	mlib_u32 a_val = a_const << 24;
	mlib_d64 *al_addr;
	mlib_d64 w_a =
		vis_to_double_dup(a_val | a_const | (a_const << 8) | (a_const <<
		16));
	mlib_d64 l0, l1, l2, l3;
	mlib_d64 sd0, sd1, sd2, sd3, dd0, dd1, dd2, dd3, ld0, ld1;

	dlb >>= 2;

	if (h == 0 || w == 0)
		return;

	if (a_array) {
		for (i = 0; i < h; i++, rgb += slb, a_array += alb,
		    abgr += dlb) {
			if ((((mlib_addr)rgb | (mlib_addr)abgr | (mlib_addr)
				a_array) & 7) == 0) {
				count = w >> 3;
				left = w - (count << 3);

#pragma pipeloop(0)
#pragma unroll(3)
				for (j = 0; j < count; j++) {
					sd0 = ((mlib_d64 *)rgb)[3 * j];
					sd1 = ((mlib_d64 *)rgb)[3 * j + 1];
					sd2 = ((mlib_d64 *)rgb)[3 * j + 2];
					sd3 = ((mlib_d64 *)a_array)[j];
					vis_write_bmask(0x0a981dcb, 0);
					dd0 = vis_bshuffle(sd3, sd0);
					vis_write_bmask(0x2cba3fed, 0);
					ld0 = vis_freg_pair(vis_read_lo(sd0),
						vis_read_hi(sd1));
					dd1 = vis_bshuffle(sd3, ld0);
					vis_write_bmask(0x4a985dcb, 0);
					ld1 = vis_freg_pair(vis_read_lo(sd1),
						vis_read_hi(sd2));
					dd2 = vis_bshuffle(sd3, ld1);
					vis_write_bmask(0x6cba7fed, 0);
					dd3 = vis_bshuffle(sd3, sd2);
					((mlib_d64 *)abgr)[4 * j] = dd0;
					((mlib_d64 *)abgr)[4 * j + 1] = dd1;
					((mlib_d64 *)abgr)[4 * j + 2] = dd2;
					((mlib_d64 *)abgr)[4 * j + 3] = dd3;
				}

				for (j = 0; j < left; j++) {
					MERGE_S32_U8(((mlib_u32 *)abgr)[8 *
						count + j],
						rgb[24 * count + 3 * j + 2],
						rgb[24 * count + 3 * j + 1],
						rgb[24 * count + 3 * j],
						(a_array[8 * count + j] << 24));
				}
			} else {
				mlib_u32 *pabgr = abgr;
				mlib_u8 *prgb = (void *)rgb, *pa_array =
					(void *)a_array;
				mlib_d64 *pa, sa0, sa1;

				if ((mlib_addr)pabgr & 7) {
					MERGE_S32_U8(((mlib_u32 *)pabgr)[0],
						prgb[2], prgb[1], prgb[0],
						(pa_array[0] << 24));
					pabgr++;
					prgb += 3;
					pa_array++;
					count = (w - 1) >> 3;
					left = (w - 1) - (count << 3);
				} else {
					count = w >> 3;
					left = w - (count << 3);
				}

				al_addr = vis_alignaddr(prgb, 0);
				l0 = vis_ld_d64_nf(al_addr); al_addr++;
				pa = vis_alignaddr(pa_array, 0);
				sa0 = (*pa++);
#pragma pipeloop(0)
#pragma unroll(3)
				for (j = 0; j < count; j++) {
					l1 = al_addr[3 * j];
					l2 = al_addr[3 * j + 1];
					l3 = vis_ld_d64_nf(al_addr + 3 * j + 2);
					vis_alignaddr(prgb, 0);
					sd0 = vis_faligndata(l0, l1);
					sd1 = vis_faligndata(l1, l2);
					sd2 = vis_faligndata(l2, l3);
					l0 = l3;
					sa1 = vis_ld_d64_nf(pa + j);
					vis_alignaddr(pa_array, 0);
					w_a = vis_faligndata(sa0, sa1);
					sa0 = sa1;
					vis_write_bmask(0x0a981dcb, 0);
					dd0 = vis_bshuffle(w_a, sd0);
					vis_write_bmask(0x2cba3fed, 0);
					ld0 = vis_freg_pair(vis_read_lo(sd0),
						vis_read_hi(sd1));
					dd1 = vis_bshuffle(w_a, ld0);
					vis_write_bmask(0x4a985dcb, 0);
					ld1 = vis_freg_pair(vis_read_lo(sd1),
						vis_read_hi(sd2));
					dd2 = vis_bshuffle(w_a, ld1);
					vis_write_bmask(0x6cba7fed, 0);
					dd3 = vis_bshuffle(w_a, sd2);
					((mlib_d64 *)pabgr)[4 * j] = dd0;
					((mlib_d64 *)pabgr)[4 * j + 1] = dd1;
					((mlib_d64 *)pabgr)[4 * j + 2] = dd2;
					((mlib_d64 *)pabgr)[4 * j + 3] = dd3;
				}

				for (j = 0; j < left; j++) {
					MERGE_S32_U8(((mlib_u32 *)pabgr)[8 *
						count + j],
						prgb[24 * count + 3 * j + 2],
						prgb[24 * count + 3 * j + 1],
						prgb[24 * count + 3 * j],
						(pa_array[8 * count +
						j] << 24));
				}
			}
		}
	} else {
		mlib_d64 sd0, sd1, sd2, dd0, dd1, dd2, dd3, ld0, ld1, ld2, ld3;

		for (i = 0; i < h; i++, rgb += slb, abgr += dlb) {
			if ((((mlib_addr)rgb | (mlib_addr)abgr) & 7) == 0) {
				count = w >> 3;
				left = w - (count << 3);

				vis_alignaddr((void *)2, 0);
				vis_write_bmask(0x0a981dcb, 0);
#pragma pipeloop(0)
				for (j = 0; j < count; j++) {
					sd0 = ((mlib_d64 *)rgb)[3 * j];
					sd1 = ((mlib_d64 *)rgb)[3 * j + 1];
					sd2 = ((mlib_d64 *)rgb)[3 * j + 2];
					ld0 = vis_freg_pair(vis_read_lo(sd0),
						vis_read_hi(sd1));
					ld1 = vis_faligndata(ld0, ld0);
					ld2 = vis_freg_pair(vis_read_lo(sd1),
						vis_read_hi(sd2));
					ld3 = vis_faligndata(sd2, sd2);
					dd0 = vis_bshuffle(w_a, sd0);
					dd1 = vis_bshuffle(w_a, ld1);
					dd2 = vis_bshuffle(w_a, ld2);
					dd3 = vis_bshuffle(w_a, ld3);
					((mlib_d64 *)abgr)[4 * j] = dd0;
					((mlib_d64 *)abgr)[4 * j + 1] = dd1;
					((mlib_d64 *)abgr)[4 * j + 2] = dd2;
					((mlib_d64 *)abgr)[4 * j + 3] = dd3;
				}

				for (j = 0; j < left; j++) {
					MERGE_S32_U8(((mlib_u32 *)abgr)[8 *
						count + j],
						rgb[24 * count + 3 * j + 2],
						rgb[24 * count + 3 * j + 1],
						rgb[24 * count + 3 * j], a_val);
				}
			} else {
				mlib_u32 *pabgr = (void *)abgr;
				mlib_u8 *prgb = (void *)rgb, *pa_array =
					(void *)a_array;

				if ((mlib_addr)pabgr & 7) {
					MERGE_S32_U8(((mlib_u32 *)pabgr)[0],
						prgb[2], prgb[1], prgb[0],
						(a_const << 24));
					pabgr++;
					prgb += 3;
					pa_array++;
					count = (w - 1) >> 3;
					left = (w - 1) - (count << 3);
				} else {
					count = w >> 3;
					left = w - (count << 3);
				}

				al_addr = vis_alignaddr(prgb, 0);
				l0 = (*al_addr++);
#pragma pipeloop(0)
#pragma unroll(3)
				for (j = 0; j < count; j++) {
					l1 = al_addr[3 * j];
					l2 = al_addr[3 * j + 1];
					l3 = vis_ld_d64_nf(al_addr + 3 * j + 2);
					sd0 = vis_faligndata(l0, l1);
					sd1 = vis_faligndata(l1, l2);
					sd2 = vis_faligndata(l2, l3);
					l0 = l3;
					vis_write_bmask(0x0a981dcb, 0);
					dd0 = vis_bshuffle(w_a, sd0);
					vis_write_bmask(0x2cba3fed, 0);
					ld0 = vis_freg_pair(vis_read_lo(sd0),
						vis_read_hi(sd1));
					dd1 = vis_bshuffle(w_a, ld0);
					vis_write_bmask(0x4a985dcb, 0);
					ld1 = vis_freg_pair(vis_read_lo(sd1),
						vis_read_hi(sd2));
					dd2 = vis_bshuffle(w_a, ld1);
					vis_write_bmask(0x6cba7fed, 0);
					dd3 = vis_bshuffle(w_a, sd2);
					((mlib_d64 *)pabgr)[4 * j] = dd0;
					((mlib_d64 *)pabgr)[4 * j + 1] = dd1;
					((mlib_d64 *)pabgr)[4 * j + 2] = dd2;
					((mlib_d64 *)pabgr)[4 * j + 3] = dd3;
				}

				for (j = 0; j < left; j++) {
					MERGE_S32_U8(((mlib_u32 *)pabgr)[8 *
						count + j],
						prgb[24 * count + 3 * j + 2],
						prgb[24 * count + 3 * j + 1],
						prgb[24 * count + 3 * j],
						(a_const << 24));
				}
			}
		}
	}
}

/* *********************************************************** */

#define	MERGE_S32_U8_BGRA_ALIGN(x, a0, a1, a2, a3)               \
	x = (a0 << 24) | (a1 << 16) | (a2 << 8) | (a3)

/* *********************************************************** */

#define	MERGE_S32_U8_BGRA(x, a0, a1, a2, a3)                     \
	{                                                        \
		mlib_u8 *dst = x;                                \
		dst[0] = a0;                                     \
		dst[1] = a1;                                     \
		dst[2] = a2;                                     \
		dst[3] = a3;                                     \
	}

/* *********************************************************** */

void
__mlib_VideoColorRGBint_to_BGRAint(
	mlib_u8 *bgra,
	const mlib_u8 *rgb,
	const mlib_u8 *a_array,
	mlib_u8 a_const,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb,
	mlib_s32 alb)
{
	mlib_s32 i, j, count = w >> 3, left = w - (count << 3);
	mlib_u32 a_val = a_const;
	mlib_d64 *al_addr;
	mlib_d64 w_a =
		vis_to_double_dup(a_val | (a_const << 8) |
			(a_const << 16) | (a_const << 24));
	mlib_d64 l0, l1, l2, l3;
	mlib_d64 sd0, sd1, sd2, sd3, dd0, dd1, dd2, dd3, ld0, ld1;

	if (h == 0 || w == 0)
		return;

	if (a_array) {
		for (i = 0; i < h; i++, rgb += slb, a_array += alb,
		    bgra += dlb) {
			if ((((mlib_addr)rgb | (mlib_addr)bgra | (mlib_addr)
				a_array) & 7) == 0) {
				count = w >> 3;
				left = w - (count << 3);

#pragma pipeloop(0)
#pragma unroll(3)
				for (j = 0; j < count; j++) {
					sd0 = ((mlib_d64 *)rgb)[3 * j];
					sd1 = ((mlib_d64 *)rgb)[3 * j + 1];
					sd2 = ((mlib_d64 *)rgb)[3 * j + 2];
					sd3 = ((mlib_d64 *)a_array)[j];
					vis_write_bmask(0xa980dcb1, 0);
					dd0 = vis_bshuffle(sd3, sd0);
					vis_write_bmask(0xcba2fed3, 0);
					ld0 = vis_freg_pair(vis_read_lo(sd0),
						vis_read_hi(sd1));
					dd1 = vis_bshuffle(sd3, ld0);
					vis_write_bmask(0xa984dcb5, 0);
					ld1 = vis_freg_pair(vis_read_lo(sd1),
						vis_read_hi(sd2));
					dd2 = vis_bshuffle(sd3, ld1);
					vis_write_bmask(0xcba6fed7, 0);
					dd3 = vis_bshuffle(sd3, sd2);
					((mlib_d64 *)bgra)[4 * j] = dd0;
					((mlib_d64 *)bgra)[4 * j + 1] = dd1;
					((mlib_d64 *)bgra)[4 * j + 2] = dd2;
					((mlib_d64 *)bgra)[4 * j + 3] = dd3;
				}

				for (j = 0; j < left; j++) {
					MERGE_S32_U8_BGRA_ALIGN(
						((mlib_u32 *)bgra)[8 *
						count + j],
						rgb[24 * count + 3 * j + 2],
						rgb[24 * count + 3 * j + 1],
						rgb[24 * count + 3 * j],
						a_array[8 * count + j]);
				}
			} else if (((mlib_addr)bgra & 3) == 0) {
				mlib_u32 *pbgra = (mlib_u32 *)bgra;
				mlib_u8 *prgb = (void *)rgb, *pa_array =
					(void *)a_array;
				mlib_d64 *pa, sa0, sa1;

				if ((mlib_addr)pbgra & 7) {
					MERGE_S32_U8_BGRA_ALIGN(
						((mlib_u32 *)pbgra)[0],
						prgb[2], prgb[1], prgb[0],
						pa_array[0]);
					pbgra++;
					prgb += 3;
					pa_array++;
					count = (w - 1) >> 3;
					left = (w - 1) - (count << 3);
				} else {
					count = w >> 3;
					left = w - (count << 3);
				}

				al_addr = vis_alignaddr(prgb, 0);
				l0 = vis_ld_d64_nf(al_addr); al_addr++;
				pa = vis_alignaddr(pa_array, 0);
				sa0 = (*pa++);
#pragma pipeloop(0)
#pragma unroll(3)
				for (j = 0; j < count; j++) {
					l1 = al_addr[3 * j];
					l2 = al_addr[3 * j + 1];
					l3 = vis_ld_d64_nf(al_addr + 3 * j + 2);
					vis_alignaddr(prgb, 0);
					sd0 = vis_faligndata(l0, l1);
					sd1 = vis_faligndata(l1, l2);
					sd2 = vis_faligndata(l2, l3);
					l0 = l3;
					sa1 = vis_ld_d64_nf(pa + j);
					vis_alignaddr(pa_array, 0);
					w_a = vis_faligndata(sa0, sa1);
					sa0 = sa1;
					vis_write_bmask(0xa980dcb1, 0);
					dd0 = vis_bshuffle(w_a, sd0);
					vis_write_bmask(0xcba2fed3, 0);
					ld0 = vis_freg_pair(vis_read_lo(sd0),
						vis_read_hi(sd1));
					dd1 = vis_bshuffle(w_a, ld0);
					vis_write_bmask(0xa984dcb5, 0);
					ld1 = vis_freg_pair(vis_read_lo(sd1),
						vis_read_hi(sd2));
					dd2 = vis_bshuffle(w_a, ld1);
					vis_write_bmask(0xcba6fed7, 0);
					dd3 = vis_bshuffle(w_a, sd2);
					((mlib_d64 *)pbgra)[4 * j] = dd0;
					((mlib_d64 *)pbgra)[4 * j + 1] = dd1;
					((mlib_d64 *)pbgra)[4 * j + 2] = dd2;
					((mlib_d64 *)pbgra)[4 * j + 3] = dd3;
				}

				for (j = 0; j < left; j++) {
					MERGE_S32_U8_BGRA_ALIGN(
						((mlib_u32 *)pbgra)[8 *
						count + j],
						prgb[24 * count + 3 * j + 2],
						prgb[24 * count + 3 * j + 1],
						prgb[24 * count + 3 * j],
						pa_array[8 * count + j]);
				}
			} else {
#pragma pipeloop(0)
				for (j = 0; j < w; j++) {
					MERGE_S32_U8_BGRA(
						bgra + 4 * j,
						rgb[3 * j + 2],
						rgb[3 * j + 1],
						rgb[3 * j],
						a_array[j]);
				}
			}
		}
	} else {
		mlib_d64 sd0, sd1, sd2, dd0, dd1, dd2, dd3, ld0, ld1, ld2, ld3;

		for (i = 0; i < h; i++, rgb += slb, bgra += dlb) {
			if ((((mlib_addr)rgb | (mlib_addr)bgra) & 7) == 0) {
				count = w >> 3;
				left = w - (count << 3);

				vis_alignaddr((void *)2, 0);
				vis_write_bmask(0xa980dcb1, 0);
#pragma pipeloop(0)
				for (j = 0; j < count; j++) {
					sd0 = ((mlib_d64 *)rgb)[3 * j];
					sd1 = ((mlib_d64 *)rgb)[3 * j + 1];
					sd2 = ((mlib_d64 *)rgb)[3 * j + 2];
					ld0 = vis_freg_pair(vis_read_lo(sd0),
						vis_read_hi(sd1));
					ld1 = vis_faligndata(ld0, ld0);
					ld2 = vis_freg_pair(vis_read_lo(sd1),
						vis_read_hi(sd2));
					ld3 = vis_faligndata(sd2, sd2);
					dd0 = vis_bshuffle(w_a, sd0);
					dd1 = vis_bshuffle(w_a, ld1);
					dd2 = vis_bshuffle(w_a, ld2);
					dd3 = vis_bshuffle(w_a, ld3);
					((mlib_d64 *)bgra)[4 * j] = dd0;
					((mlib_d64 *)bgra)[4 * j + 1] = dd1;
					((mlib_d64 *)bgra)[4 * j + 2] = dd2;
					((mlib_d64 *)bgra)[4 * j + 3] = dd3;
				}

				for (j = 0; j < left; j++) {
					MERGE_S32_U8_BGRA_ALIGN(
						((mlib_u32 *)bgra)[8 *
						count + j],
						rgb[24 * count + 3 * j + 2],
						rgb[24 * count + 3 * j + 1],
						rgb[24 * count + 3 * j], a_val);
				}
			} else if (((mlib_addr)bgra & 3) == 0) {
				mlib_u32 *pbgra = (mlib_u32 *)bgra;
				mlib_u8 *prgb = (void *)rgb, *pa_array =
					(void *)a_array;

				if ((mlib_addr)pbgra & 7) {
					MERGE_S32_U8_BGRA_ALIGN(
						((mlib_u32 *)pbgra)[0],
						prgb[2], prgb[1], prgb[0],
						a_const);
					pbgra++;
					prgb += 3;
					pa_array++;
					count = (w - 1) >> 3;
					left = (w - 1) - (count << 3);
				} else {
					count = w >> 3;
					left = w - (count << 3);
				}

				al_addr = vis_alignaddr(prgb, 0);
				l0 = (*al_addr++);
#pragma pipeloop(0)
#pragma unroll(3)
				for (j = 0; j < count; j++) {
					l1 = al_addr[3 * j];
					l2 = al_addr[3 * j + 1];
					l3 = vis_ld_d64_nf(al_addr + 3 * j + 2);
					sd0 = vis_faligndata(l0, l1);
					sd1 = vis_faligndata(l1, l2);
					sd2 = vis_faligndata(l2, l3);
					l0 = l3;
					vis_write_bmask(0xa980dcb1, 0);
					dd0 = vis_bshuffle(w_a, sd0);
					vis_write_bmask(0xcba2fed3, 0);
					ld0 = vis_freg_pair(vis_read_lo(sd0),
						vis_read_hi(sd1));
					dd1 = vis_bshuffle(w_a, ld0);
					vis_write_bmask(0xa984dcb5, 0);
					ld1 = vis_freg_pair(vis_read_lo(sd1),
						vis_read_hi(sd2));
					dd2 = vis_bshuffle(w_a, ld1);
					vis_write_bmask(0xcba6fed7, 0);
					dd3 = vis_bshuffle(w_a, sd2);
					((mlib_d64 *)pbgra)[4 * j] = dd0;
					((mlib_d64 *)pbgra)[4 * j + 1] = dd1;
					((mlib_d64 *)pbgra)[4 * j + 2] = dd2;
					((mlib_d64 *)pbgra)[4 * j + 3] = dd3;
				}

				for (j = 0; j < left; j++) {
					MERGE_S32_U8_BGRA_ALIGN(
						((mlib_u32 *)pbgra)[8 *
						count + j],
						prgb[24 * count + 3 * j + 2],
						prgb[24 * count + 3 * j + 1],
						prgb[24 * count + 3 * j],
						a_const);
				}
			} else {
#pragma pipeloop(0)
				for (j = 0; j < w; j++) {
					MERGE_S32_U8_BGRA(
						bgra + 4 * j,
						rgb[3 * j + 2],
						rgb[3 * j + 1],
						rgb[3 * j],
						a_const);
				}
			}
		}
	}
}

/* *********************************************************** */

void
__mlib_VideoColorBGRint_to_ABGRint(
	mlib_u32 *abgr,
	const mlib_u8 *bgr,
	const mlib_u8 *a_array,
	mlib_u8 a_const,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb,
	mlib_s32 alb)
{
	mlib_s32 i, j, count = w >> 3, left = w - (count << 3);
	mlib_u32 a_val = a_const << 24;
	mlib_d64 *al_addr;
	mlib_d64 w_a =
		vis_to_double_dup(a_val | a_const | (a_const << 8) | (a_const <<
		16));
	mlib_d64 l0, l1, l2, l3;
	mlib_d64 sd0, sd1, sd2, sd3, dd0, dd1, dd2, dd3, ld0, ld1;

	dlb >>= 2;

	if (h == 0 || w == 0)
		return;

	if (a_array) {
		for (i = 0; i < h; i++, bgr += slb, a_array += alb,
		    abgr += dlb) {
			if ((((mlib_addr)bgr | (mlib_addr)abgr | (mlib_addr)
				a_array) & 7) == 0) {
				count = w >> 3;
				left = w - (count << 3);

#pragma pipeloop(0)
#pragma unroll(3)
				for (j = 0; j < count; j++) {
					sd0 = ((mlib_d64 *)bgr)[3 * j];
					sd1 = ((mlib_d64 *)bgr)[3 * j + 1];
					sd2 = ((mlib_d64 *)bgr)[3 * j + 2];
					sd3 = ((mlib_d64 *)a_array)[j];
					vis_write_bmask(0x089a1bcd, 0);
					dd0 = vis_bshuffle(sd3, sd0);
					vis_write_bmask(0x2abc3def, 0);
					ld0 = vis_freg_pair(vis_read_lo(sd0),
						vis_read_hi(sd1));
					dd1 = vis_bshuffle(sd3, ld0);
					vis_write_bmask(0x489a5bcd, 0);
					ld1 = vis_freg_pair(vis_read_lo(sd1),
						vis_read_hi(sd2));
					dd2 = vis_bshuffle(sd3, ld1);
					vis_write_bmask(0x6abc7def, 0);
					dd3 = vis_bshuffle(sd3, sd2);
					((mlib_d64 *)abgr)[4 * j] = dd0;
					((mlib_d64 *)abgr)[4 * j + 1] = dd1;
					((mlib_d64 *)abgr)[4 * j + 2] = dd2;
					((mlib_d64 *)abgr)[4 * j + 3] = dd3;
				}

				for (j = 0; j < left; j++) {
					MERGE_S32_U8(((mlib_u32 *)abgr)[8 *
						count + j],
						bgr[24 * count + 3 * j],
						bgr[24 * count + 3 * j + 1],
						bgr[24 * count + 3 * j + 2],
						(a_array[8 * count + j] << 24));
				}
			} else {
				mlib_u32 *pabgr = abgr;
				mlib_u8 *pbgr = (void *)bgr, *pa_array =
					(void *)a_array;
				mlib_d64 *pa, sa0, sa1;

				if ((mlib_addr)pabgr & 7) {
					MERGE_S32_U8(((mlib_u32 *)pabgr)[0],
						pbgr[0], pbgr[1], pbgr[2],
						(pa_array[0] << 24));
					pabgr++;
					pbgr += 3;
					pa_array++;
					count = (w - 1) >> 3;
					left = (w - 1) - (count << 3);
				} else {
					count = w >> 3;
					left = w - (count << 3);
				}

				al_addr = vis_alignaddr(pbgr, 0);
				l0 = vis_ld_d64_nf(al_addr); al_addr++;
				pa = vis_alignaddr(pa_array, 0);
				sa0 = (*pa++);
#pragma pipeloop(0)
#pragma unroll(3)
				for (j = 0; j < count; j++) {
					l1 = al_addr[3 * j];
					l2 = al_addr[3 * j + 1];
					l3 = vis_ld_d64_nf(al_addr + 3 * j + 2);
					vis_alignaddr(pbgr, 0);
					sd0 = vis_faligndata(l0, l1);
					sd1 = vis_faligndata(l1, l2);
					sd2 = vis_faligndata(l2, l3);
					l0 = l3;
					sa1 = vis_ld_d64_nf(pa + j);
					vis_alignaddr(pa_array, 0);
					w_a = vis_faligndata(sa0, sa1);
					sa0 = sa1;
					vis_write_bmask(0x089a1bcd, 0);
					dd0 = vis_bshuffle(w_a, sd0);
					vis_write_bmask(0x2abc3def, 0);
					ld0 = vis_freg_pair(vis_read_lo(sd0),
						vis_read_hi(sd1));
					dd1 = vis_bshuffle(w_a, ld0);
					vis_write_bmask(0x489a5bcd, 0);
					ld1 = vis_freg_pair(vis_read_lo(sd1),
						vis_read_hi(sd2));
					dd2 = vis_bshuffle(w_a, ld1);
					vis_write_bmask(0x6abc7def, 0);
					dd3 = vis_bshuffle(w_a, sd2);
					((mlib_d64 *)pabgr)[4 * j] = dd0;
					((mlib_d64 *)pabgr)[4 * j + 1] = dd1;
					((mlib_d64 *)pabgr)[4 * j + 2] = dd2;
					((mlib_d64 *)pabgr)[4 * j + 3] = dd3;
				}

				for (j = 0; j < left; j++) {
					MERGE_S32_U8(((mlib_u32 *)pabgr)[8 *
						count + j],
						pbgr[24 * count + 3 * j],
						pbgr[24 * count + 3 * j + 1],
						pbgr[24 * count + 3 * j + 2],
						(pa_array[8 * count +
						j] << 24));
				}
			}
		}
	} else {
		mlib_d64 sd0, sd1, sd2, dd0, dd1, dd2, dd3, ld0, ld1, ld2, ld3;

		for (i = 0; i < h; i++, bgr += slb, abgr += dlb) {
			if ((((mlib_addr)bgr | (mlib_addr)abgr) & 7) == 0) {
				count = w >> 3;
				left = w - (count << 3);

				vis_alignaddr((void *)2, 0);
				vis_write_bmask(0x089a1bcd, 0);
#pragma pipeloop(0)
				for (j = 0; j < count; j++) {
					sd0 = ((mlib_d64 *)bgr)[3 * j];
					sd1 = ((mlib_d64 *)bgr)[3 * j + 1];
					sd2 = ((mlib_d64 *)bgr)[3 * j + 2];
					ld0 = vis_freg_pair(vis_read_lo(sd0),
						vis_read_hi(sd1));
					ld1 = vis_faligndata(ld0, ld0);
					ld2 = vis_freg_pair(vis_read_lo(sd1),
						vis_read_hi(sd2));
					ld3 = vis_faligndata(sd2, sd2);
					dd0 = vis_bshuffle(w_a, sd0);
					dd1 = vis_bshuffle(w_a, ld1);
					dd2 = vis_bshuffle(w_a, ld2);
					dd3 = vis_bshuffle(w_a, ld3);
					((mlib_d64 *)abgr)[4 * j] = dd0;
					((mlib_d64 *)abgr)[4 * j + 1] = dd1;
					((mlib_d64 *)abgr)[4 * j + 2] = dd2;
					((mlib_d64 *)abgr)[4 * j + 3] = dd3;
				}

				for (j = 0; j < left; j++) {
					MERGE_S32_U8(((mlib_u32 *)abgr)[8 *
						count + j],
						bgr[24 * count + 3 * j],
						bgr[24 * count + 3 * j + 1],
						bgr[24 * count + 3 * j + 2],
						a_val);
				}
			} else {
				mlib_u32 *pabgr = abgr;
				mlib_u8 *pbgr = (void *)bgr, *pa_array =
					(void *)a_array;

				if ((mlib_addr)pabgr & 7) {
					MERGE_S32_U8(((mlib_u32 *)pabgr)[0],
						pbgr[0], pbgr[1], pbgr[2],
						(a_const << 24));
					pabgr++;
					pbgr += 3;
					pa_array++;
					count = (w - 1) >> 3;
					left = (w - 1) - (count << 3);
				} else {
					count = w >> 3;
					left = w - (count << 3);
				}

				al_addr = vis_alignaddr(pbgr, 0);
				l0 = (*al_addr++);
#pragma pipeloop(0)
#pragma unroll(3)
				for (j = 0; j < count; j++) {
					l1 = al_addr[3 * j];
					l2 = al_addr[3 * j + 1];
					l3 = vis_ld_d64_nf(al_addr + 3 * j + 2);
					sd0 = vis_faligndata(l0, l1);
					sd1 = vis_faligndata(l1, l2);
					sd2 = vis_faligndata(l2, l3);
					l0 = l3;
					vis_write_bmask(0x089a1bcd, 0);
					dd0 = vis_bshuffle(w_a, sd0);
					vis_write_bmask(0x2abc3def, 0);
					ld0 = vis_freg_pair(vis_read_lo(sd0),
						vis_read_hi(sd1));
					dd1 = vis_bshuffle(w_a, ld0);
					vis_write_bmask(0x489a5bcd, 0);
					ld1 = vis_freg_pair(vis_read_lo(sd1),
						vis_read_hi(sd2));
					dd2 = vis_bshuffle(w_a, ld1);
					vis_write_bmask(0x6abc7def, 0);
					dd3 = vis_bshuffle(w_a, sd2);
					((mlib_d64 *)pabgr)[4 * j] = dd0;
					((mlib_d64 *)pabgr)[4 * j + 1] = dd1;
					((mlib_d64 *)pabgr)[4 * j + 2] = dd2;
					((mlib_d64 *)pabgr)[4 * j + 3] = dd3;
				}

				for (j = 0; j < left; j++) {
					MERGE_S32_U8(((mlib_u32 *)pabgr)[8 *
						count + j],
						pbgr[24 * count + 3 * j],
						pbgr[24 * count + 3 * j + 1],
						pbgr[24 * count + 3 * j + 2],
						(a_const << 24));
				}
			}
		}
	}
}

void
__mlib_VideoColorRGBXint_to_ABGRint(
	mlib_u32 *abgr,
	const mlib_u32 *rgbx,
	const mlib_u8 *a_array,
	mlib_u8 a_const,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb,
	mlib_s32 alb)
{
	mlib_s32 i, j, k;
	mlib_d64 w_tmp0, w_tmp1, w_tmp2, w_tmp3, w_tmp4;
	mlib_s32 count = w >> 1, left = w & 1;
	mlib_d64 *d_rgbx, *d_abgr, w_a;

	dlb >>= 2;
	slb >>= 2;

	if (w < 1)
		return;

	if (a_array) {
		mlib_d64 *da, zero = vis_fzero();
		mlib_d64 a0, a1, a2, a3, a4, a5;

		for (i = 0; i < h;
			i++, abgr += dlb, rgbx += slb, a_array += alb) {
			if ((((mlib_addr)abgr | (mlib_addr)rgbx | (mlib_addr)
				a_array) & 7) == 0) {
				da = (mlib_d64 *)a_array;
				d_abgr = (mlib_d64 *)abgr;
				d_rgbx = (mlib_d64 *)rgbx;

				vis_write_bmask(0x0a984edc, 0);
#pragma pipeloop(0)
				for (j = 0; j < count - 3; j += 4) {
					w_a = (*da++);
					w_tmp0 = d_rgbx[j];
					w_tmp1 = d_rgbx[j + 1];
					w_tmp2 = d_rgbx[j + 2];
					w_tmp3 = d_rgbx[j + 3];
					a0 = vis_fpmerge(vis_read_hi(w_a),
						zero);
					a1 = vis_fpmerge(vis_read_lo(w_a),
						zero);
					a2 = vis_fpmerge(vis_read_hi(a0), zero);
					a3 = vis_fpmerge(vis_read_lo(a0), zero);
					a4 = vis_fpmerge(vis_read_hi(a1), zero);
					a5 = vis_fpmerge(vis_read_lo(a1), zero);
					d_abgr[j] = vis_bshuffle(a2, w_tmp0);
					d_abgr[j + 1] =
						vis_bshuffle(a3, w_tmp1);
					d_abgr[j + 2] =
						vis_bshuffle(a4, w_tmp2);
					d_abgr[j + 3] =
						vis_bshuffle(a5, w_tmp3);
				}

#pragma pipeloop(0)
				for (k = 0; j < count; j++, k++) {
					mlib_u32 ld = *((mlib_u16 *)da + k);

					w_a = vis_to_double(((ld & 0xFF00) <<
						16), ((ld & 0xFF) << 24));
					d_abgr[j] =
						vis_bshuffle(w_a, d_rgbx[j]);
				}

				if (left) {
					mlib_u32 ld = a_array[2 * count];

					w_a = vis_to_double_dup(ld << 24);
					((mlib_f32 *)abgr)[2 * count] =
						vis_read_hi(vis_bshuffle(w_a,
						d_rgbx[count]));
				}
			} else {
				mlib_s32 off, count0, left0;
				mlib_d64 s0, s1, s2, s3, w_a0, w_a1;
				mlib_u32 *prgbx = (void *)rgbx, *pabgr =
					(void *)abgr;
				mlib_u8 *pa = (void *)a_array;

				if ((mlib_addr)pabgr & 7) {
					mlib_u32 ld0 = (*prgbx++);

					(*pabgr++) =
						((ld0 & 0xFF00) << 8) | ((ld0 &
						0xFF0000) >> 8) | ((ld0 &
						~0xFFFFFF)
						>> 24) | ((*pa++) << 24);
					count0 = (w - 1) >> 1;
					left0 = (w - 1) & 1;
				} else {
					count0 = count;
					left0 = left;
				}

				off = (mlib_addr)pa & 7;
				da = (mlib_d64 *)(pa - off);
				w_a0 = vis_ld_d64_nf(da); da++;
				d_rgbx = vis_alignaddr(prgbx, 0);
				d_abgr = (mlib_d64 *)pabgr;
				w_tmp0 = vis_ld_d64_nf(d_rgbx); d_rgbx++;
#pragma pipeloop(0)
				for (j = 0; j < count0 - 3; j += 4) {
					w_a1 = vis_ld_d64_nf(da); da++;
					vis_alignaddr(pa, 0);
					w_a = vis_faligndata(w_a0, w_a1);
					w_a0 = w_a1;
					w_tmp1 = d_rgbx[j];
					w_tmp2 = d_rgbx[j + 1];
					w_tmp3 = d_rgbx[j + 2];
					w_tmp4 = vis_ld_d64_nf(d_rgbx + j + 3);
					vis_alignaddr(prgbx, 0);
					s0 = vis_faligndata(w_tmp0, w_tmp1);
					s1 = vis_faligndata(w_tmp1, w_tmp2);
					s2 = vis_faligndata(w_tmp2, w_tmp3);
					s3 = vis_faligndata(w_tmp3, w_tmp4);
					w_tmp0 = w_tmp4;
					vis_write_bmask(0x0a981edc, 0);
					d_abgr[j] = vis_bshuffle(w_a, s0);
					vis_write_bmask(0x2a983edc, 0);
					d_abgr[j + 1] = vis_bshuffle(w_a, s1);
					vis_write_bmask(0x4a985edc, 0);
					d_abgr[j + 2] = vis_bshuffle(w_a, s2);
					vis_write_bmask(0x6a987edc, 0);
					d_abgr[j + 3] = vis_bshuffle(w_a, s3);
				}

				pa = (mlib_u8 *)da + off - 8;
#pragma pipeloop(0)
				for (; j < count0; j++) {
					w_a = vis_to_double((pa[0] << 24),
						(pa[1] << 24));
					pa += 2;

					w_tmp1 = vis_ld_d64_nf(d_rgbx + j);
					vis_alignaddr(prgbx, 0);
					s0 = vis_faligndata(w_tmp0, w_tmp1);
					w_tmp0 = w_tmp1;
					vis_write_bmask(0x0a984edc, 0);
					d_abgr[j] = vis_bshuffle(w_a, s0);
				}

				if (left0) {
					w_a = vis_to_double((pa[0] << 24),
						(vis_ld_u8_nf(pa + 1) << 24));
					w_tmp1 = vis_ld_d64_nf(d_rgbx + j);
					vis_alignaddr(prgbx, 0);
					s0 = vis_faligndata(w_tmp0, w_tmp1);
					vis_write_bmask(0x0a984edc, 0);
					((mlib_f32 *)pabgr)[2 * count0] =
						vis_read_hi(vis_bshuffle(w_a,
						s0));
				}
			}
		}
	} else {
		w_a = vis_to_double_dup(a_const << 24);
		for (i = 0; i < h; i++, abgr += dlb, rgbx += slb) {
			if ((((mlib_addr)abgr | (mlib_addr)rgbx) & 7) == 0) {
				d_abgr = (mlib_d64 *)abgr;
				d_rgbx = (mlib_d64 *)rgbx;

				vis_write_bmask(0x0a984edc, 0);
#pragma pipeloop(0)
				for (j = 0; j < count - 3; j += 4) {
					w_tmp0 = d_rgbx[j];
					w_tmp1 = d_rgbx[j + 1];
					w_tmp2 = d_rgbx[j + 2];
					w_tmp3 = d_rgbx[j + 3];
					d_abgr[j] = vis_bshuffle(w_a, w_tmp0);
					d_abgr[j + 1] =
						vis_bshuffle(w_a, w_tmp1);
					d_abgr[j + 2] =
						vis_bshuffle(w_a, w_tmp2);
					d_abgr[j + 3] =
						vis_bshuffle(w_a, w_tmp3);
				}

#pragma pipeloop(0)
				for (; j < count; j++) {
					d_abgr[j] =
						vis_bshuffle(w_a, d_rgbx[j]);
				}

				if (left) {
					((mlib_f32 *)abgr)[2 * count] =
						vis_read_hi(vis_bshuffle(w_a,
						d_rgbx[count]));
				}
			} else {
				mlib_s32 count0, left0;
				mlib_d64 s0, s1, s2, s3;
				mlib_u32 *prgbx = (void *)rgbx, *pabgr =
					(void *)abgr;

				if ((mlib_addr)pabgr & 7) {
					mlib_u32 ld0 = (*prgbx++);

					(*pabgr++) =
						((ld0 & 0xFF00) << 8) | ((ld0 &
						0xFF0000) >> 8) | ((ld0 &
						~0xFFFFFF)
						>> 24) | (a_const << 24);
					count0 = (w - 1) >> 1;
					left0 = (w - 1) & 1;
				} else {
					count0 = count;
					left0 = left;
				}

				d_rgbx = vis_alignaddr(prgbx, 0);
				d_abgr = (mlib_d64 *)pabgr;
				w_tmp0 = vis_ld_d64_nf(d_rgbx); d_rgbx++;
				vis_write_bmask(0x0a984edc, 0);
#pragma pipeloop(0)
				for (j = 0; j < count0 - 3; j += 4) {
					w_tmp1 = d_rgbx[j];
					w_tmp2 = d_rgbx[j + 1];
					w_tmp3 = d_rgbx[j + 2];
					w_tmp4 = vis_ld_d64_nf(d_rgbx + j + 3);
					s0 = vis_faligndata(w_tmp0, w_tmp1);
					s1 = vis_faligndata(w_tmp1, w_tmp2);
					s2 = vis_faligndata(w_tmp2, w_tmp3);
					s3 = vis_faligndata(w_tmp3, w_tmp4);
					w_tmp0 = w_tmp4;
					d_abgr[j] = vis_bshuffle(w_a, s0);
					d_abgr[j + 1] = vis_bshuffle(w_a, s1);
					d_abgr[j + 2] = vis_bshuffle(w_a, s2);
					d_abgr[j + 3] = vis_bshuffle(w_a, s3);
				}

#pragma pipeloop(0)
				for (; j < count0; j++) {
					w_tmp1 = vis_ld_d64_nf(d_rgbx + j);
					s0 = vis_faligndata(w_tmp0, w_tmp1);
					w_tmp0 = w_tmp1;
					d_abgr[j] = vis_bshuffle(w_a, s0);
				}

				if (left0) {
					w_tmp1 = vis_ld_d64_nf(d_rgbx +j);
					s0 = vis_faligndata(w_tmp0, w_tmp1);
					((mlib_f32 *)pabgr)[2 * count0] =
						vis_read_hi(vis_bshuffle(w_a,
						s0));
				}
			}
		}
	}
}

/* *********************************************************** */

void
__mlib_VideoColorRGBXint_to_ARGBint(
	mlib_u32 *argb,
	const mlib_u32 *rgbx,
	const mlib_u8 *a_array,
	mlib_u8 a_const,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb,
	mlib_s32 alb)
{
	mlib_s32 i, j, k;
	mlib_d64 w_tmp0, w_tmp1, w_tmp2, w_tmp3, w_tmp4;
	mlib_s32 count = w >> 1, left = w & 1;
	mlib_d64 *d_rgbx, *d_argb, w_a;

	dlb >>= 2;
	slb >>= 2;

	if (w < 1)
		return;

	if (a_array) {
		mlib_d64 *da, zero = vis_fzero();
		mlib_d64 a0, a1, a2, a3, a4, a5;

		for (i = 0; i < h;
			i++, argb += dlb, rgbx += slb, a_array += alb) {
			if ((((mlib_addr)argb | (mlib_addr)rgbx | (mlib_addr)
				a_array) & 7) == 0) {
				da = (mlib_d64 *)a_array;
				d_argb = (mlib_d64 *)argb;
				d_rgbx = (mlib_d64 *)rgbx;

				vis_write_bmask(0x089a4cde, 0);
#pragma pipeloop(0)
				for (j = 0; j < count - 3; j += 4) {
					w_a = (*da++);
					w_tmp0 = d_rgbx[j];
					w_tmp1 = d_rgbx[j + 1];
					w_tmp2 = d_rgbx[j + 2];
					w_tmp3 = d_rgbx[j + 3];
					a0 = vis_fpmerge(vis_read_hi(w_a),
						zero);
					a1 = vis_fpmerge(vis_read_lo(w_a),
						zero);
					a2 = vis_fpmerge(vis_read_hi(a0), zero);
					a3 = vis_fpmerge(vis_read_lo(a0), zero);
					a4 = vis_fpmerge(vis_read_hi(a1), zero);
					a5 = vis_fpmerge(vis_read_lo(a1), zero);
					d_argb[j] = vis_bshuffle(a2, w_tmp0);
					d_argb[j + 1] =
						vis_bshuffle(a3, w_tmp1);
					d_argb[j + 2] =
						vis_bshuffle(a4, w_tmp2);
					d_argb[j + 3] =
						vis_bshuffle(a5, w_tmp3);
				}

#pragma pipeloop(0)
				for (k = 0; j < count; j++, k++) {
					mlib_u32 ld = *((mlib_u16 *)da + k);

					w_a = vis_to_double(((ld & 0xFF00) <<
						16), ((ld & 0xFF) << 24));
					d_argb[j] =
						vis_bshuffle(w_a, d_rgbx[j]);
				}

				if (left) {
					mlib_u32 ld = a_array[2 * count];

					w_a = vis_to_double_dup(ld << 24);
					((mlib_f32 *)argb)[2 * count] =
						vis_read_hi(vis_bshuffle(w_a,
						d_rgbx[count]));
				}
			} else {
				mlib_s32 off, count0, left0;
				mlib_d64 s0, s1, s2, s3, w_a0, w_a1;
				mlib_u32 *prgbx = (void *)rgbx, *pargb =
					(void *)argb;
				mlib_u8 *pa = (void *)a_array;

				if ((mlib_addr)pargb & 7) {
					mlib_u32 ld0 = (*prgbx++);

					(*pargb++) =
						((ld0 & ~0xFF) >> 8) | ((*pa++)
						<< 24);
					count0 = (w - 1) >> 1;
					left0 = (w - 1) & 1;
				} else {
					count0 = count;
					left0 = left;
				}

				off = (mlib_addr)pa & 7;
				da = (mlib_d64 *)(pa - off);
				w_a0 = vis_ld_d64_nf(da); da++;
				d_rgbx = vis_alignaddr(prgbx, 0);
				d_argb = (mlib_d64 *)pargb;
				w_tmp0 = vis_ld_d64_nf(d_rgbx); d_rgbx++;
#pragma pipeloop(0)
				for (j = 0; j < count0 - 3; j += 4) {
					w_a1 = vis_ld_d64_nf(da); da++;
					vis_alignaddr(pa, 0);
					w_a = vis_faligndata(w_a0, w_a1);
					w_a0 = w_a1;
					w_tmp1 = d_rgbx[j];
					w_tmp2 = d_rgbx[j + 1];
					w_tmp3 = d_rgbx[j + 2];
					w_tmp4 = vis_ld_d64_nf(d_rgbx + j + 3);
					vis_alignaddr(prgbx, 0);
					s0 = vis_faligndata(w_tmp0, w_tmp1);
					s1 = vis_faligndata(w_tmp1, w_tmp2);
					s2 = vis_faligndata(w_tmp2, w_tmp3);
					s3 = vis_faligndata(w_tmp3, w_tmp4);
					w_tmp0 = w_tmp4;
					vis_write_bmask(0x089a1cde, 0);
					d_argb[j] = vis_bshuffle(w_a, s0);
					vis_write_bmask(0x289a3cde, 0);
					d_argb[j + 1] = vis_bshuffle(w_a, s1);
					vis_write_bmask(0x489a5cde, 0);
					d_argb[j + 2] = vis_bshuffle(w_a, s2);
					vis_write_bmask(0x689a7cde, 0);
					d_argb[j + 3] = vis_bshuffle(w_a, s3);
				}

				pa = (mlib_u8 *)da + off - 8;
#pragma pipeloop(0)
				for (; j < count0; j++) {
					w_a = vis_to_double((pa[0] << 24),
						(pa[1] << 24));
					pa += 2;

					w_tmp1 = vis_ld_d64_nf(d_rgbx + j);
					vis_alignaddr(prgbx, 0);
					s0 = vis_faligndata(w_tmp0, w_tmp1);
					w_tmp0 = w_tmp1;
					vis_write_bmask(0x089a4cde, 0);
					d_argb[j] = vis_bshuffle(w_a, s0);
				}

				if (left0) {
					w_a = vis_to_double((pa[0] << 24),
						(vis_ld_u8_nf(pa + 1) << 24));
					w_tmp1 = vis_ld_d64_nf(d_rgbx + j);
					vis_alignaddr(prgbx, 0);
					s0 = vis_faligndata(w_tmp0, w_tmp1);
					vis_write_bmask(0x089a4cde, 0);
					((mlib_f32 *)pargb)[2 * count0] =
						vis_read_hi(vis_bshuffle(w_a,
						s0));
				}
			}
		}
	} else {
		w_a = vis_to_double_dup(a_const << 24);
		for (i = 0; i < h; i++, argb += dlb, rgbx += slb) {
			if ((((mlib_addr)argb | (mlib_addr)rgbx) & 7) == 0) {
				d_argb = (mlib_d64 *)argb;
				d_rgbx = (mlib_d64 *)rgbx;

				vis_write_bmask(0x089a4cde, 0);
#pragma pipeloop(0)
				for (j = 0; j < count - 3; j += 4) {
					w_tmp0 = d_rgbx[j];
					w_tmp1 = d_rgbx[j + 1];
					w_tmp2 = d_rgbx[j + 2];
					w_tmp3 = d_rgbx[j + 3];
					d_argb[j] = vis_bshuffle(w_a, w_tmp0);
					d_argb[j + 1] =
						vis_bshuffle(w_a, w_tmp1);
					d_argb[j + 2] =
						vis_bshuffle(w_a, w_tmp2);
					d_argb[j + 3] =
						vis_bshuffle(w_a, w_tmp3);
				}

#pragma pipeloop(0)
				for (; j < count; j++) {
					d_argb[j] =
						vis_bshuffle(w_a, d_rgbx[j]);
				}

				if (left) {
					((mlib_f32 *)argb)[2 * count] =
						vis_read_hi(vis_bshuffle(w_a,
						d_rgbx[count]));
				}
			} else {
				mlib_s32 count0, left0;
				mlib_d64 s0, s1, s2, s3;
				mlib_u32 *prgbx = (void *)rgbx, *pargb =
					(void *)argb;

				if ((mlib_addr)pargb & 7) {
					mlib_u32 ld0 = (*prgbx++);

					(*pargb++) =
						((ld0 & ~0xFF) >> 8) | (a_const
						<< 24);
					count0 = (w - 1) >> 1;
					left0 = (w - 1) & 1;
				} else {
					count0 = count;
					left0 = left;
				}

				d_rgbx = vis_alignaddr(prgbx, 0);
				d_argb = (mlib_d64 *)pargb;
				w_tmp0 = vis_ld_d64_nf(d_rgbx); d_rgbx++;
				vis_write_bmask(0x089a4cde, 0);
#pragma pipeloop(0)
				for (j = 0; j < count0 - 3; j += 4) {
					w_tmp1 = d_rgbx[j];
					w_tmp2 = d_rgbx[j + 1];
					w_tmp3 = d_rgbx[j + 2];
					w_tmp4 = vis_ld_d64_nf(d_rgbx + j + 3);
					s0 = vis_faligndata(w_tmp0, w_tmp1);
					s1 = vis_faligndata(w_tmp1, w_tmp2);
					s2 = vis_faligndata(w_tmp2, w_tmp3);
					s3 = vis_faligndata(w_tmp3, w_tmp4);
					w_tmp0 = w_tmp4;
					d_argb[j] = vis_bshuffle(w_a, s0);
					d_argb[j + 1] = vis_bshuffle(w_a, s1);
					d_argb[j + 2] = vis_bshuffle(w_a, s2);
					d_argb[j + 3] = vis_bshuffle(w_a, s3);
				}

#pragma pipeloop(0)
				for (; j < count0; j++) {
					w_tmp1 = vis_ld_d64_nf(d_rgbx + j);
					s0 = vis_faligndata(w_tmp0, w_tmp1);
					w_tmp0 = w_tmp1;
					d_argb[j] = vis_bshuffle(w_a, s0);
				}

				if (left0) {
					w_tmp1 = vis_ld_d64_nf(d_rgbx + j);
					s0 = vis_faligndata(w_tmp0, w_tmp1);
					((mlib_f32 *)pargb)[2 * count0] =
						vis_read_hi(vis_bshuffle(w_a,
						s0));
				}
			}
		}
	}
}

/* *********************************************************** */

void
__mlib_VideoColorXRGBint_to_ABGRint(
	mlib_u32 *abgr,
	const mlib_u32 *xrgb,
	const mlib_u8 *a_array,
	mlib_u8 a_const,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb,
	mlib_s32 alb)
{
	mlib_s32 i, j, k;
	mlib_d64 w_tmp0, w_tmp1, w_tmp2, w_tmp3, w_tmp4;
	mlib_s32 count = w >> 1, left = w & 1;
	mlib_d64 *d_xrgb, *d_abgr, w_a;

	dlb >>= 2;
	slb >>= 2;

	if (w < 1)
		return;

	if (a_array) {
		mlib_d64 *da, zero = vis_fzero();
		mlib_d64 a0, a1, a2, a3, a4, a5;

		for (i = 0; i < h;
			i++, abgr += dlb, xrgb += slb, a_array += alb) {
			if ((((mlib_addr)abgr | (mlib_addr)xrgb | (mlib_addr)
				a_array) & 7) == 0) {
				da = (mlib_d64 *)a_array;
				d_abgr = (mlib_d64 *)abgr;
				d_xrgb = (mlib_d64 *)xrgb;

				vis_write_bmask(0x0ba94fed, 0);
#pragma pipeloop(0)
				for (j = 0; j < count - 3; j += 4) {
					w_a = (*da++);
					w_tmp0 = d_xrgb[j];
					w_tmp1 = d_xrgb[j + 1];
					w_tmp2 = d_xrgb[j + 2];
					w_tmp3 = d_xrgb[j + 3];
					a0 = vis_fpmerge(vis_read_hi(w_a),
						zero);
					a1 = vis_fpmerge(vis_read_lo(w_a),
						zero);
					a2 = vis_fpmerge(vis_read_hi(a0), zero);
					a3 = vis_fpmerge(vis_read_lo(a0), zero);
					a4 = vis_fpmerge(vis_read_hi(a1), zero);
					a5 = vis_fpmerge(vis_read_lo(a1), zero);
					d_abgr[j] = vis_bshuffle(a2, w_tmp0);
					d_abgr[j + 1] =
						vis_bshuffle(a3, w_tmp1);
					d_abgr[j + 2] =
						vis_bshuffle(a4, w_tmp2);
					d_abgr[j + 3] =
						vis_bshuffle(a5, w_tmp3);
				}

#pragma pipeloop(0)
				for (k = 0; j < count; j++, k++) {
					mlib_u32 ld = *((mlib_u16 *)da + k);

					w_a = vis_to_double(((ld & 0xFF00) <<
						16), ((ld & 0xFF) << 24));
					d_abgr[j] =
						vis_bshuffle(w_a, d_xrgb[j]);
				}

				if (left) {
					mlib_u32 ld = a_array[2 * count];

					w_a = vis_to_double_dup(ld << 24);
					((mlib_f32 *)abgr)[2 * count] =
						vis_read_hi(vis_bshuffle(w_a,
						d_xrgb[count]));
				}
			} else {
				mlib_s32 off, count0, left0;
				mlib_d64 s0, s1, s2, s3, w_a0, w_a1;
				mlib_u32 *pxrgb = (void *)xrgb, *pabgr =
					(void *)abgr;
				mlib_u8 *pa = (void *)a_array;

				if ((mlib_addr)pabgr & 7) {
					mlib_u32 ld0 = (*pxrgb++);

					(*pabgr++) =
						(ld0 & 0xFF00) | ((ld0 &
						0xFF0000) >> 16) | ((ld0 & 0xFF)
						<< 16) | ((*pa++) << 24);
					count0 = (w - 1) >> 1;
					left0 = (w - 1) & 1;
				} else {
					count0 = count;
					left0 = left;
				}

				off = (mlib_addr)pa & 7;
				da = (mlib_d64 *)(pa - off);
				w_a0 = vis_ld_d64_nf(da); da++;
				d_xrgb = vis_alignaddr(pxrgb, 0);
				d_abgr = (mlib_d64 *)pabgr;
				w_tmp0 = vis_ld_d64_nf(d_xrgb); d_xrgb++;
#pragma pipeloop(0)
				for (j = 0; j < count0 - 3; j += 4) {
					w_a1 = vis_ld_d64_nf(da); da++;
					vis_alignaddr(pa, 0);
					w_a = vis_faligndata(w_a0, w_a1);
					w_a0 = w_a1;
					w_tmp1 = d_xrgb[j];
					w_tmp2 = d_xrgb[j + 1];
					w_tmp3 = d_xrgb[j + 2];
					w_tmp4 = vis_ld_d64_nf(d_xrgb + j + 3);
					vis_alignaddr(pxrgb, 0);
					s0 = vis_faligndata(w_tmp0, w_tmp1);
					s1 = vis_faligndata(w_tmp1, w_tmp2);
					s2 = vis_faligndata(w_tmp2, w_tmp3);
					s3 = vis_faligndata(w_tmp3, w_tmp4);
					w_tmp0 = w_tmp4;
					vis_write_bmask(0x0ba91fed, 0);
					d_abgr[j] = vis_bshuffle(w_a, s0);
					vis_write_bmask(0x2ba93fed, 0);
					d_abgr[j + 1] = vis_bshuffle(w_a, s1);
					vis_write_bmask(0x4ba95fed, 0);
					d_abgr[j + 2] = vis_bshuffle(w_a, s2);
					vis_write_bmask(0x6ba97fed, 0);
					d_abgr[j + 3] = vis_bshuffle(w_a, s3);
				}

				pa = (mlib_u8 *)da + off - 8;
#pragma pipeloop(0)
				for (; j < count0; j++) {
					w_a = vis_to_double((pa[0] << 24),
						(pa[1] << 24));
					pa += 2;

					w_tmp1 = vis_ld_d64_nf(d_xrgb + j);
					vis_alignaddr(pxrgb, 0);
					s0 = vis_faligndata(w_tmp0, w_tmp1);
					w_tmp0 = w_tmp1;
					vis_write_bmask(0x0ba94fed, 0);
					d_abgr[j] = vis_bshuffle(w_a, s0);
				}

				if (left0) {
					w_a = vis_to_double((pa[0] << 24),
						(vis_ld_u8_nf(pa + 1) << 24));
					w_tmp1 = vis_ld_d64_nf(d_xrgb + j);
					vis_alignaddr(pxrgb, 0);
					s0 = vis_faligndata(w_tmp0, w_tmp1);
					vis_write_bmask(0x0ba94fed, 0);
					((mlib_f32 *)pabgr)[2 * count0] =
						vis_read_hi(vis_bshuffle(w_a,
						s0));
				}
			}
		}
	} else {
		w_a = vis_to_double_dup(a_const << 24);
		for (i = 0; i < h; i++, abgr += dlb, xrgb += slb) {
			if ((((mlib_addr)abgr | (mlib_addr)xrgb) & 7) == 0) {
				d_abgr = (mlib_d64 *)abgr;
				d_xrgb = (mlib_d64 *)xrgb;

				vis_write_bmask(0x0ba94fed, 0);
#pragma pipeloop(0)
				for (j = 0; j < count - 3; j += 4) {
					w_tmp0 = d_xrgb[j];
					w_tmp1 = d_xrgb[j + 1];
					w_tmp2 = d_xrgb[j + 2];
					w_tmp3 = d_xrgb[j + 3];
					d_abgr[j] = vis_bshuffle(w_a, w_tmp0);
					d_abgr[j + 1] =
						vis_bshuffle(w_a, w_tmp1);
					d_abgr[j + 2] =
						vis_bshuffle(w_a, w_tmp2);
					d_abgr[j + 3] =
						vis_bshuffle(w_a, w_tmp3);
				}

#pragma pipeloop(0)
				for (; j < count; j++) {
					d_abgr[j] =
						vis_bshuffle(w_a, d_xrgb[j]);
				}

				if (left) {
					((mlib_f32 *)abgr)[2 * count] =
						vis_read_hi(vis_bshuffle(w_a,
						d_xrgb[count]));
				}
			} else {
				mlib_s32 count0, left0;
				mlib_d64 s0, s1, s2, s3;
				mlib_u32 *pxrgb = (void *)xrgb, *pabgr =
					(void *)abgr;

				if ((mlib_addr)pabgr & 7) {
					mlib_u32 ld0 = (*pxrgb++);

					(*pabgr++) =
						(ld0 & 0xFF00) | ((ld0 &
						0xFF0000) >> 16) | ((ld0 & 0xFF)
						<< 16) | (a_const << 24);
					count0 = (w - 1) >> 1;
					left0 = (w - 1) & 1;
				} else {
					count0 = count;
					left0 = left;
				}

				d_xrgb = vis_alignaddr(pxrgb, 0);
				d_abgr = (mlib_d64 *)pabgr;
				w_tmp0 = vis_ld_d64_nf(d_xrgb); d_xrgb++;
				vis_write_bmask(0x0ba94fed, 0);
#pragma pipeloop(0)
				for (j = 0; j < count0 - 3; j += 4) {
					w_tmp1 = d_xrgb[j];
					w_tmp2 = d_xrgb[j + 1];
					w_tmp3 = d_xrgb[j + 2];
					w_tmp4 = vis_ld_d64_nf(d_xrgb + j + 3);
					s0 = vis_faligndata(w_tmp0, w_tmp1);
					s1 = vis_faligndata(w_tmp1, w_tmp2);
					s2 = vis_faligndata(w_tmp2, w_tmp3);
					s3 = vis_faligndata(w_tmp3, w_tmp4);
					w_tmp0 = w_tmp4;
					d_abgr[j] = vis_bshuffle(w_a, s0);
					d_abgr[j + 1] = vis_bshuffle(w_a, s1);
					d_abgr[j + 2] = vis_bshuffle(w_a, s2);
					d_abgr[j + 3] = vis_bshuffle(w_a, s3);
				}

#pragma pipeloop(0)
				for (; j < count0; j++) {
					w_tmp1 = vis_ld_d64_nf(d_xrgb + j);
					s0 = vis_faligndata(w_tmp0, w_tmp1);
					w_tmp0 = w_tmp1;
					d_abgr[j] = vis_bshuffle(w_a, s0);
				}

				if (left0) {
					w_tmp1 = vis_ld_d64_nf(d_xrgb + j);
					s0 = vis_faligndata(w_tmp0, w_tmp1);
					((mlib_f32 *)pabgr)[2 * count0] =
						vis_read_hi(vis_bshuffle(w_a,
						s0));
				}
			}
		}
	}
}

/* *********************************************************** */

void
__mlib_VideoColorXRGBint_to_ARGBint(
	mlib_u32 *argb,
	const mlib_u32 *xrgb,
	const mlib_u8 *a_array,
	mlib_u8 a_const,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb,
	mlib_s32 alb)
{
	mlib_s32 i, j, k;
	mlib_d64 w_tmp0, w_tmp1, w_tmp2, w_tmp3, w_tmp4;
	mlib_s32 count = w >> 1, left = w & 1;
	mlib_d64 *d_xrgb, *d_argb, w_a;

	dlb >>= 2;
	slb >>= 2;

	if (w < 1)
		return;

	if (a_array) {
		mlib_d64 *da, zero = vis_fzero();
		mlib_d64 a0, a1, a2, a3, a4, a5;

		for (i = 0; i < h;
			i++, argb += dlb, xrgb += slb, a_array += alb) {
			if ((((mlib_addr)argb | (mlib_addr)xrgb | (mlib_addr)
				a_array) & 7) == 0) {
				da = (mlib_d64 *)a_array;
				d_argb = (mlib_d64 *)argb;
				d_xrgb = (mlib_d64 *)xrgb;

				vis_write_bmask(0x09ab4def, 0);
#pragma pipeloop(0)
				for (j = 0; j < count - 3; j += 4) {
					w_a = (*da++);
					w_tmp0 = d_xrgb[j];
					w_tmp1 = d_xrgb[j + 1];
					w_tmp2 = d_xrgb[j + 2];
					w_tmp3 = d_xrgb[j + 3];
					a0 = vis_fpmerge(vis_read_hi(w_a),
						zero);
					a1 = vis_fpmerge(vis_read_lo(w_a),
						zero);
					a2 = vis_fpmerge(vis_read_hi(a0), zero);
					a3 = vis_fpmerge(vis_read_lo(a0), zero);
					a4 = vis_fpmerge(vis_read_hi(a1), zero);
					a5 = vis_fpmerge(vis_read_lo(a1), zero);
					d_argb[j] = vis_bshuffle(a2, w_tmp0);
					d_argb[j + 1] =
						vis_bshuffle(a3, w_tmp1);
					d_argb[j + 2] =
						vis_bshuffle(a4, w_tmp2);
					d_argb[j + 3] =
						vis_bshuffle(a5, w_tmp3);
				}

#pragma pipeloop(0)
				for (k = 0; j < count; j++, k++) {
					mlib_u32 ld = *((mlib_u16 *)da + k);

					w_a = vis_to_double(((ld & 0xFF00) <<
						16), ((ld & 0xFF) << 24));
					d_argb[j] =
						vis_bshuffle(w_a, d_xrgb[j]);
				}

				if (left) {
					mlib_u32 ld = a_array[2 * count];

					w_a = vis_to_double_dup(ld << 24);
					((mlib_f32 *)argb)[2 * count] =
						vis_read_hi(vis_bshuffle(w_a,
						d_xrgb[count]));
				}
			} else {
				mlib_s32 off, count0, left0;
				mlib_d64 s0, s1, s2, s3, w_a0, w_a1;
				mlib_u32 *pxrgb = (void *)xrgb, *pargb =
					(void *)argb;
				mlib_u8 *pa = (void *)a_array;

				if ((mlib_addr)pargb & 7) {
					mlib_u32 ld0 = (*pxrgb++);

					(*pargb++) =
						(ld0 & 0xFFFFFF) | ((*pa++) <<
						24);
					count0 = (w - 1) >> 1;
					left0 = (w - 1) & 1;
				} else {
					count0 = count;
					left0 = left;
				}

				off = (mlib_addr)pa & 7;
				da = (mlib_d64 *)(pa - off);
				w_a0 = vis_ld_d64_nf(da); da++;
				d_xrgb = vis_alignaddr(pxrgb, 0);
				d_argb = (mlib_d64 *)pargb;
				w_tmp0 = vis_ld_d64_nf(d_xrgb); d_xrgb++;
#pragma pipeloop(0)
				for (j = 0; j < count0 - 3; j += 4) {
					w_a1 = vis_ld_d64_nf(da); da++;
					vis_alignaddr(pa, 0);
					w_a = vis_faligndata(w_a0, w_a1);
					w_a0 = w_a1;
					w_tmp1 = d_xrgb[j];
					w_tmp2 = d_xrgb[j + 1];
					w_tmp3 = d_xrgb[j + 2];
					w_tmp4 = vis_ld_d64_nf(d_xrgb + j + 3);
					vis_alignaddr(pxrgb, 0);
					s0 = vis_faligndata(w_tmp0, w_tmp1);
					s1 = vis_faligndata(w_tmp1, w_tmp2);
					s2 = vis_faligndata(w_tmp2, w_tmp3);
					s3 = vis_faligndata(w_tmp3, w_tmp4);
					w_tmp0 = w_tmp4;
					vis_write_bmask(0x09ab1def, 0);
					d_argb[j] = vis_bshuffle(w_a, s0);
					vis_write_bmask(0x29ab3def, 0);
					d_argb[j + 1] = vis_bshuffle(w_a, s1);
					vis_write_bmask(0x49ab5def, 0);
					d_argb[j + 2] = vis_bshuffle(w_a, s2);
					vis_write_bmask(0x69ab7def, 0);
					d_argb[j + 3] = vis_bshuffle(w_a, s3);
				}

				pa = (mlib_u8 *)da + off - 8;
#pragma pipeloop(0)
				for (; j < count0; j++) {
					w_a = vis_to_double((pa[0] << 24),
						(pa[1] << 24));
					pa += 2;

					w_tmp1 = vis_ld_d64_nf(d_xrgb + j);
					vis_alignaddr(pxrgb, 0);
					s0 = vis_faligndata(w_tmp0, w_tmp1);
					w_tmp0 = w_tmp1;
					vis_write_bmask(0x09ab4def, 0);
					d_argb[j] = vis_bshuffle(w_a, s0);
				}

				if (left0) {
					w_a = vis_to_double((pa[0] << 24),
						(vis_ld_u8_nf(pa + 1) << 24));
					w_tmp1 = vis_ld_d64_nf(d_xrgb + j);
					vis_alignaddr(pxrgb, 0);
					s0 = vis_faligndata(w_tmp0, w_tmp1);
					vis_write_bmask(0x09ab4def, 0);
					((mlib_f32 *)pargb)[2 * count0] =
						vis_read_hi(vis_bshuffle(w_a,
						s0));
				}
			}
		}
	} else {
		w_a = vis_to_double_dup(a_const << 24);
		for (i = 0; i < h; i++, argb += dlb, xrgb += slb) {
			if ((((mlib_addr)argb | (mlib_addr)xrgb) & 7) == 0) {
				d_argb = (mlib_d64 *)argb;
				d_xrgb = (mlib_d64 *)xrgb;

				vis_write_bmask(0x09ab4def, 0);
#pragma pipeloop(0)
				for (j = 0; j < count - 3; j += 4) {
					w_tmp0 = d_xrgb[j];
					w_tmp1 = d_xrgb[j + 1];
					w_tmp2 = d_xrgb[j + 2];
					w_tmp3 = d_xrgb[j + 3];
					d_argb[j] = vis_bshuffle(w_a, w_tmp0);
					d_argb[j + 1] =
						vis_bshuffle(w_a, w_tmp1);
					d_argb[j + 2] =
						vis_bshuffle(w_a, w_tmp2);
					d_argb[j + 3] =
						vis_bshuffle(w_a, w_tmp3);
				}

#pragma pipeloop(0)
				for (; j < count; j++) {
					d_argb[j] =
						vis_bshuffle(w_a, d_xrgb[j]);
				}

				if (left) {
					((mlib_f32 *)argb)[2 * count] =
						vis_read_hi(vis_bshuffle(w_a,
						d_xrgb[count]));
				}
			} else {
				mlib_s32 count0, left0;
				mlib_d64 s0, s1, s2, s3;
				mlib_u32 *pxrgb = (void *)xrgb, *pargb = argb;

				if ((mlib_addr)pargb & 7) {
					mlib_u32 ld0 = (*pxrgb++);

					(*pargb++) =
						(ld0 & 0xFFFFFF) | (a_const <<
						24);
					count0 = (w - 1) >> 1;
					left0 = (w - 1) & 1;
				} else {
					count0 = count;
					left0 = left;
				}

				d_xrgb = vis_alignaddr(pxrgb, 0);
				d_argb = (mlib_d64 *)pargb;
				w_tmp0 = vis_ld_d64_nf(d_xrgb); d_xrgb++;
				vis_write_bmask(0x09ab4def, 0);
#pragma pipeloop(0)
				for (j = 0; j < count0 - 3; j += 4) {
					w_tmp1 = d_xrgb[j];
					w_tmp2 = d_xrgb[j + 1];
					w_tmp3 = d_xrgb[j + 2];
					w_tmp4 = vis_ld_d64_nf(d_xrgb + j + 3);
					s0 = vis_faligndata(w_tmp0, w_tmp1);
					s1 = vis_faligndata(w_tmp1, w_tmp2);
					s2 = vis_faligndata(w_tmp2, w_tmp3);
					s3 = vis_faligndata(w_tmp3, w_tmp4);
					w_tmp0 = w_tmp4;
					d_argb[j] = vis_bshuffle(w_a, s0);
					d_argb[j + 1] = vis_bshuffle(w_a, s1);
					d_argb[j + 2] = vis_bshuffle(w_a, s2);
					d_argb[j + 3] = vis_bshuffle(w_a, s3);
				}

#pragma pipeloop(0)
				for (; j < count0; j++) {
					w_tmp1 = vis_ld_d64_nf(d_xrgb + j);
					s0 = vis_faligndata(w_tmp0, w_tmp1);
					w_tmp0 = w_tmp1;
					d_argb[j] = vis_bshuffle(w_a, s0);
				}

				if (left0) {
					w_tmp1 = vis_ld_d64_nf(d_xrgb + j);
					s0 = vis_faligndata(w_tmp0, w_tmp1);
					((mlib_f32 *)pargb)[2 * count0] =
						vis_read_hi(vis_bshuffle(w_a,
						s0));
				}
			}
		}
	}
}

/* *********************************************************** */
