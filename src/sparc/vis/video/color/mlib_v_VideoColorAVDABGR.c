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
 *
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

	dlb >>= 2;
	slb >>= 2;

	for (i = 0; i < h; i++, argb += dlb, abgr += slb) {
		if ((((mlib_addr)argb | (mlib_addr)abgr) & 7) == 0) {
			mlib_d64 w_abgr, w_tmp0, w_tmp1, w_argb;
			mlib_d64 *d_abgr = (mlib_d64 *)abgr;
			mlib_d64 *d_argb = (mlib_d64 *)argb;

#pragma pipeloop(0)
			for (j = 0; j < count; j++) {
				w_abgr = d_abgr[j];
				w_tmp0 = vis_fpmerge(vis_read_hi(w_abgr),
					vis_read_lo(w_abgr));
				w_tmp1 = vis_fpmerge(vis_read_lo(w_tmp0),
					vis_read_hi(w_tmp0));
				w_tmp0 = vis_fpmerge(vis_read_hi(w_tmp0),
					vis_read_lo(w_tmp0));
				w_argb = vis_fpmerge(vis_read_hi(w_tmp0),
					vis_read_lo(w_tmp1));
				d_argb[j] = w_argb;
			}

			if (left) {
				w_abgr = d_abgr[count];
				w_tmp0 = vis_fpmerge(vis_read_hi(w_abgr),
					vis_read_lo(w_abgr));
				w_tmp1 = vis_fpmerge(vis_read_lo(w_tmp0),
					vis_read_hi(w_tmp0));
				w_tmp0 = vis_fpmerge(vis_read_hi(w_tmp0),
					vis_read_lo(w_tmp0));
				w_argb = vis_fpmerge(vis_read_hi(w_tmp0),
					vis_read_lo(w_tmp1));
				((mlib_f32 *)argb)[2 * count] =
					vis_read_hi(w_argb);
			}
		} else {
			mlib_d64 w_tmp0, w_tmp1, w_argb;
			mlib_f32 v_abgr0, v_abgr1;

#pragma pipeloop(0)
			for (j = 0; j < count; j++) {
				v_abgr0 = ((mlib_f32 *)abgr)[2 * j];
				v_abgr1 = ((mlib_f32 *)abgr)[2 * j + 1];
				w_tmp0 = vis_fpmerge(v_abgr0, v_abgr1);
				w_tmp1 = vis_fpmerge(vis_read_lo(w_tmp0),
					vis_read_hi(w_tmp0));
				w_tmp0 = vis_fpmerge(vis_read_hi(w_tmp0),
					vis_read_lo(w_tmp0));
				w_argb = vis_fpmerge(vis_read_hi(w_tmp0),
					vis_read_lo(w_tmp1));
				((mlib_f32 *)argb)[2 * j] = vis_read_hi(w_argb);
				((mlib_f32 *)argb)[2 * j + 1] =
					vis_read_lo(w_argb);
			}

			if (left) {
				v_abgr0 = ((mlib_f32 *)abgr)[2 * count];
				w_tmp0 = vis_fpmerge(v_abgr0, v_abgr0);
				w_tmp1 = vis_fpmerge(vis_read_lo(w_tmp0),
					vis_read_hi(w_tmp0));
				w_tmp0 = vis_fpmerge(vis_read_hi(w_tmp0),
					vis_read_lo(w_tmp0));
				w_argb = vis_fpmerge(vis_read_hi(w_tmp0),
					vis_read_lo(w_tmp1));
				((mlib_f32 *)argb)[2 * count] =
					vis_read_hi(w_argb);
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
	mlib_s32 i, j, count = (w >> 1), left = w & 1;

	dlb >>= 2;
	slb >>= 2;

	for (i = 0; i < h; i++, abgr += dlb, bgra += slb) {
		if ((((mlib_addr)abgr | (mlib_addr)bgra) & 7) == 0) {
			mlib_d64 w_bgra, w_tmp0, w_tmp1, w_abgr;

#pragma pipeloop(0)
			for (j = 0; j < count; j++) {
				w_bgra = ((mlib_d64 *)bgra)[j];
				w_tmp0 = vis_fpmerge(vis_read_hi(w_bgra),
					vis_read_lo(w_bgra));
				w_tmp1 = vis_fpmerge(vis_read_lo(w_tmp0),
					vis_read_hi(w_tmp0));
				w_tmp0 = vis_fpmerge(vis_read_hi(w_tmp0),
					vis_read_lo(w_tmp0));
				w_abgr = vis_fpmerge(vis_read_lo(w_tmp1),
					vis_read_hi(w_tmp0));
				((mlib_d64 *)abgr)[j] = w_abgr;
			}

			if (left) {
				w_bgra = ((mlib_d64 *)bgra)[count];
				w_tmp0 = vis_fpmerge(vis_read_hi(w_bgra),
					vis_read_lo(w_bgra));
				w_tmp1 = vis_fpmerge(vis_read_lo(w_tmp0),
					vis_read_hi(w_tmp0));
				w_tmp0 = vis_fpmerge(vis_read_hi(w_tmp0),
					vis_read_lo(w_tmp0));
				w_abgr = vis_fpmerge(vis_read_lo(w_tmp1),
					vis_read_hi(w_tmp0));
				((mlib_f32 *)abgr)[2 * count] =
					vis_read_hi(w_abgr);
			}
		} else {
			mlib_d64 w_tmp0, w_tmp1, w_abgr;
			mlib_f32 v_bgra0, v_bgra1;

#pragma pipeloop(0)
			for (j = 0; j < count; j++) {
				v_bgra0 = ((mlib_f32 *)bgra)[2 * j];
				v_bgra1 = ((mlib_f32 *)bgra)[2 * j + 1];
				w_tmp0 = vis_fpmerge(v_bgra0, v_bgra1);
				w_tmp1 = vis_fpmerge(vis_read_lo(w_tmp0),
					vis_read_hi(w_tmp0));
				w_tmp0 = vis_fpmerge(vis_read_hi(w_tmp0),
					vis_read_lo(w_tmp0));
				w_abgr = vis_fpmerge(vis_read_lo(w_tmp1),
					vis_read_hi(w_tmp0));
				((mlib_f32 *)abgr)[2 * j] = vis_read_hi(w_abgr);
				((mlib_f32 *)abgr)[2 * j + 1] =
					vis_read_lo(w_abgr);
			}

			if (left) {
				v_bgra0 = ((mlib_f32 *)bgra)[2 * count];
				w_tmp0 = vis_fpmerge(v_bgra0, v_bgra0);
				w_tmp1 = vis_fpmerge(vis_read_lo(w_tmp0),
					vis_read_hi(w_tmp0));
				w_tmp0 = vis_fpmerge(vis_read_hi(w_tmp0),
					vis_read_lo(w_tmp0));
				w_abgr = vis_fpmerge(vis_read_lo(w_tmp1),
					vis_read_hi(w_tmp0));
				((mlib_f32 *)abgr)[2 * count] =
					vis_read_hi(w_abgr);
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
	mlib_s32 i, j, count = (w >> 1), left = w & 1;

	dlb >>= 2;
	slb >>= 2;

	if (w == 0 || h == 0)
		return;

	for (i = 0; i < h; i++, rgba += dlb, abgr += slb) {
		if ((((mlib_addr)rgba | (mlib_addr)abgr) & 7) == 0) {
			mlib_d64 w_rgba_0, w_rgba_1;
			mlib_d64 *d_rgba = (mlib_d64 *)rgba;
			mlib_d64 *d_abgr = (mlib_d64 *)abgr;

#pragma pipeloop(0)
			for (j = 0; j < (count - 1); j += 2) {
				w_rgba_0 = vis_ld_d64_le(d_abgr);
				w_rgba_1 = vis_ld_d64_le(d_abgr + 1);
				d_rgba[0] =
					vis_freg_pair(vis_read_lo(w_rgba_0),
					vis_read_hi(w_rgba_0));
				d_rgba[1] =
					vis_freg_pair(vis_read_lo(w_rgba_1),
					vis_read_hi(w_rgba_1));
				d_abgr += 2;
				d_rgba += 2;
			}

			if (j == (count - 1)) {
				w_rgba_0 = vis_ld_d64_le(d_abgr);
				d_rgba[0] =
					vis_freg_pair(vis_read_lo(w_rgba_0),
					vis_read_hi(w_rgba_0));
			}

			if (left) {
				w_rgba_0 =
					vis_ld_d64_le((mlib_d64 *)abgr + count);
				((mlib_f32 *)rgba)[2 * count] =
					vis_read_lo(w_rgba_0);
			}
		} else {
			mlib_d64 w_tmp, w_rgba;
			mlib_f32 v_abgr0, f_last;
			mlib_s32 w2 = w, count2, left2;
			mlib_f32 *f_rgba = (mlib_f32 *)rgba;
			mlib_f32 *f_abgr = (mlib_f32 *)abgr;

			if ((mlib_addr)f_abgr & 7) {
				v_abgr0 = f_abgr[0];
				w_tmp = vis_fpmerge(v_abgr0, v_abgr0);
				w_tmp = vis_fpmerge(vis_read_lo(w_tmp),
					vis_read_hi(w_tmp));
				w_rgba = vis_fpmerge(vis_read_lo(w_tmp),
					vis_read_hi(w_tmp));
				f_rgba[0] = vis_read_hi(w_rgba);
				f_rgba++;
				f_abgr++;
				w2--;
			}

			count2 = w2 >> 1;
			left2 = w2 & 1;

			if ((mlib_addr)f_rgba & 7) {
				if (count2 > 0) {
					mlib_d64 *d_rgba =
						(mlib_d64 *)(f_rgba - 1);

					w_rgba = vis_ld_d64_le((mlib_d64 *)
						f_abgr);
					f_rgba[0] = vis_read_lo(w_rgba);
					f_last = vis_read_hi(w_rgba);
#pragma pipeloop(0)
					for (j = 1; j < count2; j++) {
						w_rgba = vis_ld_d64_le((mlib_d64
							*)f_abgr + j);
						d_rgba[j] =
							vis_freg_pair(f_last,
							vis_read_lo(w_rgba));
						f_last = vis_read_hi(w_rgba);
					}

					f_rgba[2 * count2 - 1] = f_last;
				}
			} else {
				mlib_d64 *d_rgba = (mlib_d64 *)f_rgba;

#pragma pipeloop(0)
				for (j = 0; j < count2; j++) {
					w_rgba = vis_ld_d64_le((mlib_d64 *)
						f_abgr + j);
					d_rgba[j] =
						vis_freg_pair(vis_read_lo
						(w_rgba), vis_read_hi(w_rgba));
				}
			}

			if (left2) {
				w_rgba = vis_ld_d64_le((mlib_d64 *)f_abgr +
					count2);
				((mlib_f32 *)f_rgba)[2 * count2] =
					vis_read_lo(w_rgba);
			}
		}
	}
}

/* *********************************************************** */

#define	UNALIGNLOAD_64(var, addr)                               \
	al_addr = vis_alignaddr(addr, 0);                       \
	var = vis_faligndata(*al_addr, *(al_addr + 1))

/* *********************************************************** */

#define	UNALIGNLOAD_64_NF(var, addr)                            \
	al_addr = vis_alignaddr(addr, 0);                       \
	var = vis_faligndata(vis_ld_d64_nf(al_addr),            \
		vis_ld_d64_nf(al_addr + 1))

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
	mlib_d64 *al_addr;

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

				w_a = vis_ld_d64_nf((mlib_d64 *)a_array);
				w_g = vis_ld_d64_nf((mlib_d64 *)g);

#pragma pipeloop(0)
				for (j = 0; j < count; j++) {
					w_r = ((mlib_d64 *)r)[j];
					w_b = ((mlib_d64 *)b)[j];
					w_ag0 = VIS_FPMERGE_HI(w_a, w_g);
					w_ag1 = VIS_FPMERGE_LO(w_a, w_g);
					w_br0 = VIS_FPMERGE_HI(w_b, w_r);
					w_br1 = VIS_FPMERGE_LO(w_b, w_r);
					w_res0 = VIS_FPMERGE_HI(w_ag0, w_br0);
					w_res1 = VIS_FPMERGE_LO(w_ag0, w_br0);
					w_res2 = VIS_FPMERGE_HI(w_ag1, w_br1);
					w_res3 = VIS_FPMERGE_LO(w_ag1, w_br1);

					w_a = vis_ld_d64_nf((mlib_d64 *)a_array
						+ j + 1);
					w_g = vis_ld_d64_nf((mlib_d64 *)g
						+ j + 1);
					((mlib_d64 *)abgr)[4 * j] = w_res0;
					((mlib_d64 *)abgr)[4 * j + 1] = w_res1;
					((mlib_d64 *)abgr)[4 * j + 2] = w_res2;
					((mlib_d64 *)abgr)[4 * j + 3] = w_res3;
				}

				if (left) {
					mlib_d64 res_buf[4];

					w_r = vis_ld_d64_nf((mlib_d64 *)r +
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
				const mlib_u8 *pr = r, *pg = g, *pb = b,
					*pa_array = a_array;

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

#pragma pipeloop(0)
				for (j = 0; j < count; j++) {
					UNALIGNLOAD_64_NF(w_r, (mlib_d64 *)pr +
						j);
					UNALIGNLOAD_64_NF(w_b, (mlib_d64 *)pb +
						j);
					UNALIGNLOAD_64_NF(w_a,
						(mlib_d64 *)pa_array + j);
					UNALIGNLOAD_64_NF(w_g, (mlib_d64 *)pg +
						j);
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
				(mlib_addr)abgr) & 7)
				== 0) {
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
				const mlib_u8 *pr = r, *pg = g, *pb = b;

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

#pragma pipeloop(0)
				for (j = 0; j < count; j++) {
					UNALIGNLOAD_64_NF(w_r, (mlib_d64 *)pr +
						j);
					UNALIGNLOAD_64_NF(w_b, (mlib_d64 *)pb +
						j);
					UNALIGNLOAD_64_NF(w_g, (mlib_d64 *)pg +
						j);
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

#define	INSERT_U8_34R_I(dd0, dd1, dd2, dd3, sd0, sd1, sd2, sd3)          \
	{                                                                \
	    mlib_d64 sda, sdb, sdc, sdd, sde, sdf, sdg, sdh, sdi, sdj,   \
		    sdk, sdl, sdm;                                       \
	    sda = vis_fpmerge(vis_read_hi(sd0), vis_read_lo(sd1));       \
	    sdb = vis_fpmerge(vis_read_lo(sd0), vis_read_hi(sd2));       \
	    sdc = vis_fpmerge(vis_read_hi(sd1), vis_read_lo(sd2));       \
	    sdd = vis_fpmerge(vis_read_hi(sda), vis_read_lo(sdb));       \
	    sde = vis_fpmerge(vis_read_lo(sda), vis_read_hi(sdc));       \
	    sdf = vis_fpmerge(vis_read_hi(sdb), vis_read_lo(sdc));       \
	    sdg = vis_fpmerge(vis_read_hi(sdd), vis_read_lo(sde));       \
	    sdh = vis_fpmerge(vis_read_lo(sdd), vis_read_hi(sdf));       \
	    sdi = vis_fpmerge(vis_read_hi(sde), vis_read_lo(sdf));       \
	    sdj = vis_fpmerge(vis_read_hi(sdi), vis_read_hi(sdg));       \
	    sdk = vis_fpmerge(vis_read_lo(sdi), vis_read_lo(sdg));       \
	    sdl = vis_fpmerge(vis_read_hi(sd3), vis_read_hi(sdh));       \
	    sdm = vis_fpmerge(vis_read_lo(sd3), vis_read_lo(sdh));       \
	    dd0 = vis_fpmerge(vis_read_hi(sdl), vis_read_hi(sdj));       \
	    dd1 = vis_fpmerge(vis_read_lo(sdl), vis_read_lo(sdj));       \
	    dd2 = vis_fpmerge(vis_read_hi(sdm), vis_read_hi(sdk));       \
	    dd3 = vis_fpmerge(vis_read_lo(sdm), vis_read_lo(sdk));       \
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

	dlb >>= 2;

	if (h == 0 || w == 0)
		return;

	if (a_array) {
		for (i = 0; i < h; i++, rgb += slb, a_array += alb,
		    abgr += dlb) {
			if ((((mlib_addr)rgb | (mlib_addr)abgr | (mlib_addr)
				a_array) & 7) == 0) {
				mlib_d64 sd0, sd1, sd2, sd3, dd0, dd1, dd2, dd3;

				count = w >> 3;
				left = w - (count << 3);

#pragma pipeloop(0)
				for (j = 0; j < count; j++) {
					sd0 = ((mlib_d64 *)rgb)[3 * j];
					sd1 = ((mlib_d64 *)rgb)[3 * j + 1];
					sd2 = ((mlib_d64 *)rgb)[3 * j + 2];
					sd3 = ((mlib_d64 *)a_array)[j];
					INSERT_U8_34R_I(dd0, dd1, dd2, dd3, sd0,
						sd1, sd2, sd3);
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
				const mlib_u8 *prgb = rgb, *pa_array = a_array;
				mlib_d64 sd0, sd1, sd2, sd3, dd0, dd1, dd2, dd3;

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

#pragma pipeloop(0)
				for (j = 0; j < count; j++) {
					mlib_d64 l1, l2, l3, l4;

					al_addr =
						vis_alignaddr((mlib_d64 *)prgb +
						3 * j, 0);
					l1 = (*al_addr++);
					l2 = (*al_addr++);
					l3 = (*al_addr++);
					l4 = vis_ld_d64_nf(al_addr);
					sd0 = vis_faligndata(l1, l2);
					sd1 = vis_faligndata(l2, l3);
					sd2 = vis_faligndata(l3, l4);
					UNALIGNLOAD_64_NF(sd3,
						(mlib_d64 *)pa_array + j);
					INSERT_U8_34R_I(dd0, dd1, dd2, dd3, sd0,
						sd1, sd2, sd3);
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

		for (i = 0; i < h; i++, rgb += slb, abgr += dlb) {
			if ((((mlib_addr)rgb | (mlib_addr)abgr) & 7) == 0) {
				mlib_d64 sd0, sd1, sd2, dd0, dd1, dd2, dd3;

				count = w >> 3;
				left = w - (count << 3);

#pragma pipeloop(0)
				for (j = 0; j < count; j++) {
					sd0 = ((mlib_d64 *)rgb)[3 * j];
					sd1 = ((mlib_d64 *)rgb)[3 * j + 1];
					sd2 = ((mlib_d64 *)rgb)[3 * j + 2];
					INSERT_U8_34R_I(dd0, dd1, dd2, dd3, sd0,
						sd1, sd2, w_a);
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
				mlib_u32 *pabgr = abgr;
				const mlib_u8 *prgb = rgb, *pa_array = a_array;
				mlib_d64 sd0, sd1, sd2, dd0, dd1, dd2, dd3;

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

				al_addr = vis_alignaddr((void *)prgb, 0);
#pragma pipeloop(0)
				for (j = 0; j < count; j++) {
					mlib_d64 l1, l2, l3, l4;

					l1 = (*al_addr++);
					l2 = (*al_addr++);
					l3 = (*al_addr++);
					l4 = vis_ld_d64_nf(al_addr);
					sd0 = vis_faligndata(l1, l2);
					sd1 = vis_faligndata(l2, l3);
					sd2 = vis_faligndata(l3, l4);
					INSERT_U8_34R_I(dd0, dd1, dd2, dd3, sd0,
						sd1, sd2, w_a);
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

/*
 * sdg = (r0, r1, r2, r3, r4, r5, r6, r7)
 * sdh = (g0, g1, g2, g3, g4, g5, g6, g7)
 * sdi = (b0, b1, b2, b3, b4, b5, b6, b7)
 */

#define	INSERT_U8_34R_I_BGRA(dd0, dd1, dd2, dd3, sd0, sd1, sd2, sd3)     \
	{                                                                \
	    mlib_d64 sda, sdb, sdc, sdd, sde, sdf, sdg, sdh, sdi, sdj,   \
		    sdk, sdl, sdm;                                       \
	    sda = vis_fpmerge(vis_read_hi(sd0), vis_read_lo(sd1));       \
	    sdb = vis_fpmerge(vis_read_lo(sd0), vis_read_hi(sd2));       \
	    sdc = vis_fpmerge(vis_read_hi(sd1), vis_read_lo(sd2));       \
	    sdd = vis_fpmerge(vis_read_hi(sda), vis_read_lo(sdb));       \
	    sde = vis_fpmerge(vis_read_lo(sda), vis_read_hi(sdc));       \
	    sdf = vis_fpmerge(vis_read_hi(sdb), vis_read_lo(sdc));       \
	    sdg = vis_fpmerge(vis_read_hi(sdd), vis_read_lo(sde));       \
	    sdh = vis_fpmerge(vis_read_lo(sdd), vis_read_hi(sdf));       \
	    sdi = vis_fpmerge(vis_read_hi(sde), vis_read_lo(sdf));       \
	    sdj = vis_fpmerge(vis_read_hi(sdi), vis_read_hi(sdg));       \
	    sdk = vis_fpmerge(vis_read_hi(sdh), vis_read_hi(sd3));       \
	    sdl = vis_fpmerge(vis_read_lo(sdi), vis_read_lo(sdg));       \
	    sdm = vis_fpmerge(vis_read_lo(sdh), vis_read_lo(sd3));       \
	    dd0 = vis_fpmerge(vis_read_hi(sdj), vis_read_hi(sdk));       \
	    dd1 = vis_fpmerge(vis_read_lo(sdj), vis_read_lo(sdk));       \
	    dd2 = vis_fpmerge(vis_read_hi(sdl), vis_read_hi(sdm));       \
	    dd3 = vis_fpmerge(vis_read_lo(sdl), vis_read_lo(sdm));       \
	}

/* *********************************************************** */

#define	MERGE_S32_U8_BGRA_ALIGN(x, a0, a1, a2, a3)               \
	*(mlib_u32 *)(x) = (a0 << 24) | (a1 << 16) | (a2 << 8) | (a3)

/* *********************************************************** */

#define	MERGE_S32_U8_BGRA(x, a0, a1, a2, a3)                     \
	{                                                        \
		mlib_u8 *dst = x;                                \
		dst[0] = a0; /* Blue  */                         \
		dst[1] = a1; /* Green */                         \
		dst[2] = a2; /* Red   */                         \
		dst[3] = a3; /* Alpha */                         \
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
	mlib_u8 a_val = a_const;
	mlib_d64 *al_addr;
	mlib_d64 w_a =
		vis_to_double_dup(a_val | (a_const << 8) |
			(a_const << 16) | (a_const << 24));

	if (h == 0 || w == 0)
		return;

	if (a_array) {
		for (i = 0; i < h; i++, rgb += slb, a_array += alb,
		    bgra += dlb) {
			if ((((mlib_addr)rgb | (mlib_addr)bgra | (mlib_addr)
				a_array) & 7) == 0) {
				mlib_d64 sd0, sd1, sd2, sd3, dd0, dd1, dd2, dd3;

				count = w >> 3;
				left = w - (count << 3);

#pragma pipeloop(0)
				for (j = 0; j < count; j++) {
					sd0 = ((mlib_d64 *)rgb)[3 * j];
					sd1 = ((mlib_d64 *)rgb)[3 * j + 1];
					sd2 = ((mlib_d64 *)rgb)[3 * j + 2];
					sd3 = ((mlib_d64 *)a_array)[j];
					INSERT_U8_34R_I_BGRA(
						dd0, dd1, dd2, dd3,
						sd0, sd1, sd2, sd3);
					((mlib_d64 *)bgra)[4 * j] = dd0;
					((mlib_d64 *)bgra)[4 * j + 1] = dd1;
					((mlib_d64 *)bgra)[4 * j + 2] = dd2;
					((mlib_d64 *)bgra)[4 * j + 3] = dd3;
				}

				for (j = 0; j < left; j++) {
					MERGE_S32_U8_BGRA_ALIGN(
						bgra + 32 * count + 4 * j,
						rgb[24 * count + 3 * j + 2],
						rgb[24 * count + 3 * j + 1],
						rgb[24 * count + 3 * j],
						a_array[8 * count + j]);
				}
			} else if (((mlib_addr)bgra & 3) == 0) {
				mlib_u8 *pbgra = bgra;
				const mlib_u8 *prgb = rgb, *pa_array = a_array;
				mlib_d64 sd0, sd1, sd2, sd3, dd0, dd1, dd2, dd3;

				if ((mlib_addr)pbgra & 7) {
					MERGE_S32_U8_BGRA_ALIGN(pbgra,
						prgb[2], prgb[1], prgb[0],
						pa_array[0]);
					pbgra += 4;
					prgb += 3;
					pa_array++;
					count = (w - 1) >> 3;
					left = (w - 1) - (count << 3);
				} else {
					count = w >> 3;
					left = w - (count << 3);
				}

#pragma pipeloop(0)
				for (j = 0; j < count; j++) {
					mlib_d64 l1, l2, l3, l4;

					al_addr =
						vis_alignaddr((mlib_d64 *)prgb +
						3 * j, 0);
					l1 = (*al_addr++);
					l2 = (*al_addr++);
					l3 = (*al_addr++);
					l4 = vis_ld_d64_nf(al_addr);
					sd0 = vis_faligndata(l1, l2);
					sd1 = vis_faligndata(l2, l3);
					sd2 = vis_faligndata(l3, l4);
					UNALIGNLOAD_64_NF(sd3,
						(mlib_d64 *)pa_array + j);
					INSERT_U8_34R_I_BGRA(
						dd0, dd1, dd2, dd3,
						sd0, sd1, sd2, sd3);
					((mlib_d64 *)pbgra)[4 * j] = dd0;
					((mlib_d64 *)pbgra)[4 * j + 1] = dd1;
					((mlib_d64 *)pbgra)[4 * j + 2] = dd2;
					((mlib_d64 *)pbgra)[4 * j + 3] = dd3;
				}

				for (j = 0; j < left; j++) {
					MERGE_S32_U8_BGRA_ALIGN(
						pbgra + 32 * count + 4 * j,
						prgb[24 * count + 3 * j + 2],
						prgb[24 * count + 3 * j + 1],
						prgb[24 * count + 3 * j],
						pa_array[8 * count + j]);
				}
			} else {
				mlib_u8 *pbgra = bgra;
				const mlib_u8 *prgb = rgb, *pa_array = a_array;

#pragma pipeloop(0)
				for (j = 0; j < w; j++) {
					MERGE_S32_U8_BGRA(pbgra + 4 * j,
						prgb[3 * j + 2],
						prgb[3 * j + 1],
						prgb[3 * j],
						pa_array[j]);
				}
			}
		}
	} else {

		for (i = 0; i < h; i++, rgb += slb, bgra += dlb) {
			if ((((mlib_addr)rgb | (mlib_addr)bgra) & 7) == 0) {
				mlib_d64 sd0, sd1, sd2, dd0, dd1, dd2, dd3;

				count = w >> 3;
				left = w - (count << 3);

#pragma pipeloop(0)
				for (j = 0; j < count; j++) {
					sd0 = ((mlib_d64 *)rgb)[3 * j];
					sd1 = ((mlib_d64 *)rgb)[3 * j + 1];
					sd2 = ((mlib_d64 *)rgb)[3 * j + 2];
					INSERT_U8_34R_I_BGRA(
						dd0, dd1, dd2, dd3,
						sd0, sd1, sd2, w_a);
					((mlib_d64 *)bgra)[4 * j] = dd0;
					((mlib_d64 *)bgra)[4 * j + 1] = dd1;
					((mlib_d64 *)bgra)[4 * j + 2] = dd2;
					((mlib_d64 *)bgra)[4 * j + 3] = dd3;
				}

				for (j = 0; j < left; j++) {
					MERGE_S32_U8_BGRA_ALIGN(
						bgra + 32 * count + 4 * j,
						rgb[24 * count + 3 * j + 2],
						rgb[24 * count + 3 * j + 1],
						rgb[24 * count + 3 * j], a_val);
				}
			} else if (((mlib_addr)bgra & 3) == 0) {
				mlib_u8 *pbgra = bgra;
				const mlib_u8 *prgb = rgb, *pa_array = a_array;
				mlib_d64 sd0, sd1, sd2, dd0, dd1, dd2, dd3;

				if ((mlib_addr)pbgra & 7) {
					MERGE_S32_U8_BGRA_ALIGN(pbgra,
						prgb[2], prgb[1], prgb[0],
						a_const);
					pbgra += 4;
					prgb += 3;
					pa_array++;
					count = (w - 1) >> 3;
					left = (w - 1) - (count << 3);
				} else {
					count = w >> 3;
					left = w - (count << 3);
				}

				al_addr = vis_alignaddr((void *)prgb, 0);
#pragma pipeloop(0)
				for (j = 0; j < count; j++) {
					mlib_d64 l1, l2, l3, l4;

					l1 = (*al_addr++);
					l2 = (*al_addr++);
					l3 = (*al_addr++);
					l4 = vis_ld_d64_nf(al_addr);
					sd0 = vis_faligndata(l1, l2);
					sd1 = vis_faligndata(l2, l3);
					sd2 = vis_faligndata(l3, l4);
					INSERT_U8_34R_I_BGRA(
						dd0, dd1, dd2, dd3,
						sd0, sd1, sd2, w_a);
					((mlib_d64 *)pbgra)[4 * j] = dd0;
					((mlib_d64 *)pbgra)[4 * j + 1] = dd1;
					((mlib_d64 *)pbgra)[4 * j + 2] = dd2;
					((mlib_d64 *)pbgra)[4 * j + 3] = dd3;
				}

				for (j = 0; j < left; j++) {
					MERGE_S32_U8_BGRA_ALIGN(
						pbgra + 32 * count + 4 * j,
						prgb[24 * count + 3 * j + 2],
						prgb[24 * count + 3 * j + 1],
						prgb[24 * count + 3 * j],
						a_const);
				}
			} else {
				mlib_u8 *pbgra = bgra;
				const mlib_u8 *prgb = rgb;

#pragma pipeloop(0)
				for (j = 0; j < w; j++) {
					MERGE_S32_U8_BGRA(pbgra,
						prgb[2], prgb[1], prgb[0],
						a_const);
					pbgra += 4;
					prgb += 3;
				}
			}
		}
	}
}

/* *********************************************************** */

#define	INSERT_U8_34R(dd0, dd1, dd2, dd3, sd0, sd1, sd2, sd3)            \
	{                                                                \
	    mlib_d64 sda, sdb, sdc, sdd, sde, sdf, sdg, sdh, sdi, sdj,   \
		    sdk, sdl, sdm;                                       \
	    sda = vis_fpmerge(vis_read_hi(sd0), vis_read_lo(sd1));       \
	    sdb = vis_fpmerge(vis_read_lo(sd0), vis_read_hi(sd2));       \
	    sdc = vis_fpmerge(vis_read_hi(sd1), vis_read_lo(sd2));       \
	    sdd = vis_fpmerge(vis_read_hi(sda), vis_read_lo(sdb));       \
	    sde = vis_fpmerge(vis_read_lo(sda), vis_read_hi(sdc));       \
	    sdf = vis_fpmerge(vis_read_hi(sdb), vis_read_lo(sdc));       \
	    sdg = vis_fpmerge(vis_read_hi(sdd), vis_read_lo(sde));       \
	    sdh = vis_fpmerge(vis_read_lo(sdd), vis_read_hi(sdf));       \
	    sdi = vis_fpmerge(vis_read_hi(sde), vis_read_lo(sdf));       \
	    sdj = vis_fpmerge(vis_read_hi(sdg), vis_read_hi(sdi));       \
	    sdk = vis_fpmerge(vis_read_lo(sdg), vis_read_lo(sdi));       \
	    sdl = vis_fpmerge(vis_read_hi(sd3), vis_read_hi(sdh));       \
	    sdm = vis_fpmerge(vis_read_lo(sd3), vis_read_lo(sdh));       \
	    dd0 = vis_fpmerge(vis_read_hi(sdl), vis_read_hi(sdj));       \
	    dd1 = vis_fpmerge(vis_read_lo(sdl), vis_read_lo(sdj));       \
	    dd2 = vis_fpmerge(vis_read_hi(sdm), vis_read_hi(sdk));       \
	    dd3 = vis_fpmerge(vis_read_lo(sdm), vis_read_lo(sdk));       \
	}

/* *********************************************************** */

#define	MERGE_S32(argb0, argb1, argb2, argb3, rgbr, gbrg,           \
	brgb, a_val0, a_val1, a_val2, a_val3)                       \
	argb0 = a_val0 | (rgbr >> 8);                               \
	argb1 = a_val1 | (((rgbr & 0xFF) << 16) | (gbrg >> 16));    \
	argb2 = a_val2 | (((gbrg & 0xFFFF) << 8) | (brgb >> 24));   \
	argb3 = a_val3 | (brgb & 0xFFFFFF)

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
	mlib_s32 i, j, count = (w >> 3), left = w - (count << 3);
	mlib_u32 a_val = a_const << 24;
	mlib_d64 *al_addr;
	mlib_d64 w_a =
		vis_to_double_dup(a_val | a_const | (a_const << 8) | (a_const <<
		16));

	dlb >>= 2;

	if (h == 0 || w == 0)
		return;

	if (a_array) {
		for (i = 0; i < h; i++, bgr += slb, a_array += alb,
		    abgr += dlb) {
			if ((((mlib_addr)bgr | (mlib_addr)abgr | (mlib_addr)
				a_array) & 7) == 0) {
				mlib_d64 sd0, sd1, sd2, sd3, dd0, dd1, dd2, dd3;

				count = w >> 3;
				left = w - (count << 3);

#pragma pipeloop(0)
				for (j = 0; j < count; j++) {
					sd0 = ((mlib_d64 *)bgr)[3 * j];
					sd1 = ((mlib_d64 *)bgr)[3 * j + 1];
					sd2 = ((mlib_d64 *)bgr)[3 * j + 2];
					sd3 = ((mlib_d64 *)a_array)[j];
					INSERT_U8_34R(dd0, dd1, dd2, dd3, sd0,
						sd1, sd2, sd3);
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
				const mlib_u8 *prgb = bgr, *pa_array = a_array;
				mlib_d64 sd0, sd1, sd2, sd3, dd0, dd1, dd2, dd3;

				if ((mlib_addr)pabgr & 7) {
					MERGE_S32_U8(((mlib_u32 *)pabgr)[0],
						prgb[0], prgb[1], prgb[2],
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

#pragma pipeloop(0)
				for (j = 0; j < count; j++) {
					mlib_d64 l1, l2, l3, l4;

					al_addr =
						vis_alignaddr((mlib_d64 *)prgb +
						3 * j, 0);
					l1 = (*al_addr++);
					l2 = (*al_addr++);
					l3 = (*al_addr++);
					l4 = vis_ld_d64_nf(al_addr);
					sd0 = vis_faligndata(l1, l2);
					sd1 = vis_faligndata(l2, l3);
					sd2 = vis_faligndata(l3, l4);
					UNALIGNLOAD_64_NF(sd3,
						(mlib_d64 *)pa_array + j);
					INSERT_U8_34R(dd0, dd1, dd2, dd3, sd0,
						sd1, sd2, sd3);
					((mlib_d64 *)pabgr)[4 * j] = dd0;
					((mlib_d64 *)pabgr)[4 * j + 1] = dd1;
					((mlib_d64 *)pabgr)[4 * j + 2] = dd2;
					((mlib_d64 *)pabgr)[4 * j + 3] = dd3;
				}

				for (j = 0; j < left; j++) {
					MERGE_S32_U8(((mlib_u32 *)pabgr)[8 *
						count + j],
						prgb[24 * count + 3 * j],
						prgb[24 * count + 3 * j + 1],
						prgb[24 * count + 3 * j + 2],
						(pa_array[8 * count +
						j] << 24));
				}
			}
		}
	} else {

		for (i = 0; i < h; i++, bgr += slb, abgr += dlb) {
			if ((((mlib_addr)bgr | (mlib_addr)abgr) & 7) == 0) {
				mlib_d64 sd0, sd1, sd2, dd0, dd1, dd2, dd3;

				count = w >> 3;
				left = w - (count << 3);

#pragma pipeloop(0)
				for (j = 0; j < count; j++) {
					sd0 = ((mlib_d64 *)bgr)[3 * j];
					sd1 = ((mlib_d64 *)bgr)[3 * j + 1];
					sd2 = ((mlib_d64 *)bgr)[3 * j + 2];
					INSERT_U8_34R(dd0, dd1, dd2, dd3, sd0,
						sd1, sd2, w_a);
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
				const mlib_u8 *prgb = bgr, *pa_array = a_array;
				mlib_d64 sd0, sd1, sd2, dd0, dd1, dd2, dd3;

				if ((mlib_addr)pabgr & 7) {
					MERGE_S32_U8(((mlib_u32 *)pabgr)[0],
						prgb[0], prgb[1], prgb[2],
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

				al_addr = vis_alignaddr((void *)prgb, 0);
#pragma pipeloop(0)
				for (j = 0; j < count; j++) {
					mlib_d64 l1, l2, l3, l4;

					l1 = (*al_addr++);
					l2 = (*al_addr++);
					l3 = (*al_addr++);
					l4 = vis_ld_d64_nf(al_addr);
					sd0 = vis_faligndata(l1, l2);
					sd1 = vis_faligndata(l2, l3);
					sd2 = vis_faligndata(l3, l4);
					INSERT_U8_34R(dd0, dd1, dd2, dd3, sd0,
						sd1, sd2, w_a);
					((mlib_d64 *)pabgr)[4 * j] = dd0;
					((mlib_d64 *)pabgr)[4 * j + 1] = dd1;
					((mlib_d64 *)pabgr)[4 * j + 2] = dd2;
					((mlib_d64 *)pabgr)[4 * j + 3] = dd3;
				}

				for (j = 0; j < left; j++) {
					MERGE_S32_U8(((mlib_u32 *)pabgr)[8 *
						count + j],
						prgb[24 * count + 3 * j],
						prgb[24 * count + 3 * j + 1],
						prgb[24 * count + 3 * j + 2],
						(a_const << 24));
				}
			}
		}
	}
}

/* *********************************************************** */

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
	mlib_s32 i, j;
	mlib_d64 zero = vis_fzero();
	mlib_f32 f_zero = vis_to_float(0x0);
	mlib_f32 fmask = vis_to_float(~0xFF);
	mlib_f32 f_revert_mask = vis_to_float(0xFFFFFF);
	mlib_d64 d_revert_mask = vis_freg_pair(f_revert_mask, f_revert_mask);

	dlb >>= 2;
	slb >>= 2;

	if (w < 1)
		return;

	if (a_array) {
		for (i = 0; i < h;
			i++, abgr += dlb, rgbx += slb, a_array += alb) {
			if (((((mlib_addr)abgr | (mlib_addr)rgbx) & 7) == 0) &&
				(((mlib_addr)a_array & 3) == 0)) {
				mlib_d64 w_xbgr_0, w_xbgr_1;
				mlib_d64 w_a, w_a0, w_a1;
				mlib_f32 f_a, f_rgbx0;
				mlib_f32 *f_abgr;
				mlib_d64 *d_rgbx;

				j = 0;
				f_abgr = (mlib_f32 *)abgr - 4;
				d_rgbx = (mlib_d64 *)rgbx;

#pragma pipeloop(0)
				for (; j <= w - 4; j += 4) {
					f_abgr += 4;
					f_a = *((mlib_f32 *)(a_array + j));
					w_xbgr_0 = vis_ld_d64_le(d_rgbx);
					w_xbgr_1 = vis_ld_d64_le(d_rgbx + 1);

					w_a = vis_fpmerge(f_a, f_zero);
					w_a0 = vis_fpmerge(vis_read_hi(w_a),
						f_zero);
					w_a1 = vis_fpmerge(vis_read_lo(w_a),
						f_zero);
					w_xbgr_0 =
						vis_fand(w_xbgr_0,
						d_revert_mask);
					w_xbgr_1 =
						vis_fand(w_xbgr_1,
						d_revert_mask);

					f_abgr[0] =
						vis_fors(vis_read_lo(w_xbgr_0),
						vis_read_hi(w_a0));
					f_abgr[1] =
						vis_fors(vis_read_hi(w_xbgr_0),
						vis_read_lo(w_a0));
					f_abgr[2] =
						vis_fors(vis_read_lo(w_xbgr_1),
						vis_read_hi(w_a1));
					f_abgr[3] =
						vis_fors(vis_read_hi(w_xbgr_1),
						vis_read_lo(w_a1));
					d_rgbx += 2;
				}

				f_abgr += 3;
				for (; j < w; j++) {
					mlib_d64 w_a =
						vis_ld_u8((void *)(a_array +
						j));

					f_abgr++;
					f_rgbx0 = *((mlib_f32 *)(rgbx + j));
					f_rgbx0 = vis_fands(f_rgbx0, fmask);
					f_rgbx0 =
						vis_fors(f_rgbx0,
						vis_read_lo(w_a));
					w_xbgr_0 = vis_fpmerge(zero, f_rgbx0);
					w_xbgr_0 =
						vis_fpmerge(vis_read_lo
						(w_xbgr_0),
						vis_read_hi(w_xbgr_0));
					w_xbgr_0 =
						vis_fpmerge(vis_read_lo
						(w_xbgr_0),
						vis_read_hi(w_xbgr_0));
					*f_abgr = vis_read_lo(w_xbgr_0);
				}
			} else {
				mlib_d64 w_tmp0, w_tmp1, w_tmp2, w_abgr0,
					w_abgr1;
				mlib_d64 w_a0, w_a1, w_a2, w_a3;
				mlib_f32 f_rgbx0, f_rgbx1, f_rgbx2, f_rgbx3;

				j = 0;

				if ((mlib_addr)abgr & 7) {
					w_a0 = vis_ld_u8((void *)(a_array));
					f_rgbx0 = *((mlib_f32 *)rgbx);
					f_rgbx0 = vis_fands(f_rgbx0, fmask);
					f_rgbx0 =
						vis_fors(f_rgbx0,
						vis_read_lo(w_a0));
					w_tmp0 = vis_fpmerge(zero, f_rgbx0);
					w_tmp1 = vis_fpmerge(vis_read_lo
						(w_tmp0), vis_read_hi(w_tmp0));
					w_abgr0 =
						vis_fpmerge(vis_read_lo(w_tmp1),
						vis_read_hi(w_tmp1));
					*((mlib_f32 *)(abgr)) =
						vis_read_lo(w_abgr0);
					j++;
				}
#pragma pipeloop(0)
				for (; j <= w - 4; j += 4) {
					w_a0 = vis_ld_u8((void *)(a_array + j));
					w_a1 = vis_ld_u8((void *)(a_array + j +
						1));
					w_a2 = vis_ld_u8((void *)(a_array + j +
						2));
					w_a3 = vis_ld_u8((void *)(a_array + j +
						3));
					f_rgbx0 = *((mlib_f32 *)(rgbx + j));
					f_rgbx1 = *((mlib_f32 *)(rgbx + j) + 1);
					f_rgbx2 = *((mlib_f32 *)(rgbx + j) + 2);
					f_rgbx3 = *((mlib_f32 *)(rgbx + j) + 3);
					f_rgbx0 = vis_fands(f_rgbx0, fmask);
					f_rgbx1 = vis_fands(f_rgbx1, fmask);
					f_rgbx2 = vis_fands(f_rgbx2, fmask);
					f_rgbx3 = vis_fands(f_rgbx3, fmask);
					f_rgbx0 =
						vis_fors(f_rgbx0,
						vis_read_lo(w_a0));
					f_rgbx1 =
						vis_fors(f_rgbx1,
						vis_read_lo(w_a1));
					f_rgbx2 =
						vis_fors(f_rgbx2,
						vis_read_lo(w_a2));
					f_rgbx3 =
						vis_fors(f_rgbx3,
						vis_read_lo(w_a3));
					w_tmp0 = vis_freg_pair(f_rgbx0,
						f_rgbx1);
					w_tmp1 = vis_fpmerge(vis_read_hi
						(w_tmp0), vis_read_lo(w_tmp0));
					w_tmp2 = vis_fpmerge(vis_read_lo
						(w_tmp1), vis_read_hi(w_tmp1));
					w_abgr0 =
						vis_fpmerge(vis_read_lo(w_tmp2),
						vis_read_hi(w_tmp2));
					*((mlib_d64 *)(abgr + j)) = w_abgr0;
					w_tmp0 = vis_freg_pair(f_rgbx2,
						f_rgbx3);
					w_tmp1 = vis_fpmerge(vis_read_hi
						(w_tmp0), vis_read_lo(w_tmp0));
					w_tmp2 = vis_fpmerge(vis_read_lo
						(w_tmp1), vis_read_hi(w_tmp1));
					w_abgr1 =
						vis_fpmerge(vis_read_lo(w_tmp2),
						vis_read_hi(w_tmp2));
					*((mlib_d64 *)(abgr + j) + 1) = w_abgr1;
				}

				for (; j < w; j++) {
					w_a0 = vis_ld_u8((void *)(a_array + j));
					f_rgbx0 = *((mlib_f32 *)(rgbx + j));
					f_rgbx0 = vis_fands(f_rgbx0, fmask);
					f_rgbx0 =
						vis_fors(f_rgbx0,
						vis_read_lo(w_a0));
					w_tmp0 = vis_fpmerge(zero, f_rgbx0);
					w_tmp1 = vis_fpmerge(vis_read_lo
						(w_tmp0), vis_read_hi(w_tmp0));
					w_abgr0 =
						vis_fpmerge(vis_read_lo(w_tmp1),
						vis_read_hi(w_tmp1));
					*((mlib_f32 *)(abgr + j)) =
						vis_read_lo(w_abgr0);
				}
			}
		}
	} else {
		mlib_f32 f_a_const = vis_to_float(a_const);
		mlib_f32 f_tmp;
		mlib_d64 d_tmp;
		mlib_d64 w_a = vis_to_double_dup(a_const << 24);
		mlib_d64 w_xbgr_0, w_xbgr_1;
		mlib_d64 *d_rgbx;
		mlib_f32 *f_abgr;

		for (i = 0; i < h; i++, abgr += dlb, rgbx += slb) {

			j = 0;
			f_abgr = (mlib_f32 *)abgr - 4;
			d_rgbx = (mlib_d64 *)rgbx;

			if ((mlib_addr)rgbx & 7) {
				f_tmp = *((mlib_f32 *)rgbx);
				f_tmp = vis_fands(f_tmp, fmask);
				f_tmp = vis_fors(f_tmp, f_a_const);
				d_tmp = vis_fpmerge(zero, f_tmp);
				d_tmp = vis_fpmerge(vis_read_lo(d_tmp),
					vis_read_hi(d_tmp));
				d_tmp = vis_fpmerge(vis_read_lo(d_tmp),
					vis_read_hi(d_tmp));
				*((mlib_f32 *)(abgr)) = vis_read_lo(d_tmp);
				d_rgbx = (mlib_d64 *)(rgbx + 1);
				f_abgr++;
				j++;
			}
#pragma pipeloop(0)
			for (; j <= w - 4; j += 4) {
				f_abgr += 4;
				w_xbgr_0 = vis_ld_d64_le(d_rgbx);
				w_xbgr_1 = vis_ld_d64_le(d_rgbx + 1);
				w_xbgr_0 = vis_fand(w_xbgr_0, d_revert_mask);
				w_xbgr_1 = vis_fand(w_xbgr_1, d_revert_mask);
				w_xbgr_0 = vis_for(w_xbgr_0, w_a);
				w_xbgr_1 = vis_for(w_xbgr_1, w_a);
				f_abgr[0] = vis_read_lo(w_xbgr_0);
				f_abgr[1] = vis_read_hi(w_xbgr_0);
				f_abgr[2] = vis_read_lo(w_xbgr_1);
				f_abgr[3] = vis_read_hi(w_xbgr_1);
				d_rgbx += 2;
			}

			if (j < (w - 1)) {
				f_abgr += 2;
				w_xbgr_0 = vis_ld_d64_le(d_rgbx);
				w_xbgr_0 = vis_fand(w_xbgr_0, d_revert_mask);
				w_xbgr_0 = vis_for(w_xbgr_0, w_a);
				f_abgr[2] = vis_read_lo(w_xbgr_0);
				f_abgr[3] = vis_read_hi(w_xbgr_0);
				j += 2;
				d_rgbx += 1;
			}

			if (j < w) {
				w_xbgr_0 = vis_ld_d64_le(d_rgbx);
				w_xbgr_0 = vis_fand(w_xbgr_0, d_revert_mask);
				w_xbgr_0 = vis_for(w_xbgr_0, w_a);
				f_abgr[4] = vis_read_lo(w_xbgr_0);
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
	mlib_s32 i, j;
	mlib_d64 zero = vis_fzero();
	mlib_f32 fmask = vis_to_float(0xFFFFFF);
	mlib_d64 dmask = vis_freg_pair(fmask, fmask);

	dlb >>= 2;
	slb >>= 2;

	if (a_array) {
		for (i = 0; i < h;
			i++, argb += dlb, rgbx += slb, a_array += alb) {
			if (((((mlib_addr)argb | (mlib_addr)rgbx) & 7) == 0) &&
				(((mlib_addr)a_array & 3) == 0)) {
				mlib_d64 w_rgbx0, w_rgbx1, w_tmp0, w_tmp1,
					w_argb0, w_argb1;
				mlib_d64 w_a0, w_a1, w_a2;
				mlib_f32 w_a;

				vis_alignaddr((void *)0, 7);
#pragma pipeloop(0)
				for (j = 0; j <= w - 4; j += 4) {
					w_a = *((mlib_f32 *)(a_array + j));
					w_rgbx0 = *((mlib_d64 *)(rgbx + j));
					w_rgbx1 = *((mlib_d64 *)(rgbx + j) + 1);
					w_a0 = vis_fpmerge(w_a, zero);
					w_a1 = vis_fpmerge(vis_read_hi(w_a0),
						zero);
					w_a2 = vis_fpmerge(vis_read_lo(w_a0),
						zero);
					w_tmp0 = vis_faligndata(w_rgbx0,
						w_rgbx0);
					w_tmp1 = vis_faligndata(w_rgbx1,
						w_rgbx1);
					w_tmp0 = vis_fand(w_tmp0, dmask);
					w_tmp1 = vis_fand(w_tmp1, dmask);
					w_argb0 = vis_for(w_tmp0, w_a1);
					w_argb1 = vis_for(w_tmp1, w_a2);
					*((mlib_d64 *)(argb + j)) = w_argb0;
					*((mlib_d64 *)(argb + j) + 1) = w_argb1;
				}

				for (; j < w; j++) {
					mlib_f32 f_tmp0, f_rgbx;

					w_a0 = vis_ld_u8((void *)(a_array + j));
					w_a1 = vis_fpmerge(vis_read_lo(w_a0),
						zero);
					w_a2 = vis_fpmerge(vis_read_lo(w_a1),
						zero);
					f_rgbx = *((mlib_f32 *)(rgbx + j));
					w_rgbx0 = vis_freg_pair(f_rgbx, f_rgbx);
					w_tmp0 = vis_faligndata(w_rgbx0,
						w_rgbx0);
					f_tmp0 = vis_fands(vis_read_hi(w_tmp0),
						fmask);
					*((mlib_f32 *)argb + j) =
						vis_fors(vis_read_lo(w_a2),
						f_tmp0);
				}
			} else {
				mlib_d64 dld0, dld1, d_tmp0, d_tmp2, d_tmp3;
				mlib_d64 w_rgbx0, w_rgbx1, w_tmp0;
				mlib_d64 w_a0, w_a1, w_a2;
				mlib_d64 *prgbx;

				vis_alignaddr((void *)0, 7);
				j = 0;

				if ((mlib_addr)argb & 7) {
					mlib_f32 f_tmp0, f_rgbx;

					w_a0 = vis_ld_u8((void *)(a_array));
					w_a1 = vis_fpmerge(vis_read_lo(w_a0),
						zero);
					w_a2 = vis_fpmerge(vis_read_lo(w_a1),
						zero);
					f_rgbx = *((mlib_f32 *)(rgbx));
					w_rgbx0 = vis_freg_pair(f_rgbx, f_rgbx);
					w_tmp0 = vis_faligndata(w_rgbx0,
						w_rgbx0);
					f_tmp0 = vis_fands(vis_read_hi(w_tmp0),
						fmask);
					*((mlib_f32 *)argb) =
						vis_fors(vis_read_lo(w_a2),
						f_tmp0);
					j++;
				}

				prgbx = (mlib_d64 *)vis_alignaddr((void *)(rgbx
					+ j), -1);
				w_rgbx0 = vis_ld_d64_nf(prgbx); prgbx++;
#pragma pipeloop(0)
				for (; j <= w - 2; j += 2) {
					dld0 = vis_ld_u8((void *)(a_array + j));
					dld1 = vis_ld_u8((void *)(a_array + j +
						1));
					d_tmp0 = vis_fpmerge(vis_read_lo(dld0),
						vis_read_lo(dld1));
					d_tmp2 = vis_fpmerge(vis_read_lo
						(d_tmp0), zero);
					d_tmp3 = vis_fpmerge(vis_read_lo
						(d_tmp2), zero);
					w_rgbx1 = vis_ld_d64_nf(prgbx); prgbx++;
					w_tmp0 = vis_faligndata(w_rgbx0,
						w_rgbx1);
					w_tmp0 = vis_fand(w_tmp0, dmask);
					*((mlib_d64 *)(argb + j)) =
						vis_for(d_tmp3, w_tmp0);
					w_rgbx0 = w_rgbx1;
				}

				vis_alignaddr((void *)0, 7);
				for (; j < w; j++) {
					mlib_f32 f_tmp0, f_rgbx;

					w_a0 = vis_ld_u8((void *)(a_array + j));
					w_a1 = vis_fpmerge(vis_read_lo(w_a0),
						zero);
					w_a2 = vis_fpmerge(vis_read_lo(w_a1),
						zero);
					f_rgbx = *((mlib_f32 *)(rgbx + j));
					w_rgbx0 = vis_freg_pair(f_rgbx, f_rgbx);
					w_tmp0 = vis_faligndata(w_rgbx0,
						w_rgbx0);
					f_tmp0 = vis_fands(vis_read_hi(w_tmp0),
						fmask);
					*((mlib_f32 *)(argb + j)) =
						vis_fors(vis_read_lo(w_a2),
						f_tmp0);
				}
			}
		}
	} else {
		mlib_d64 w_a = vis_to_double_dup(a_const << 24);

		for (i = 0; i < h; i++, argb += dlb, rgbx += slb) {
			if ((((mlib_addr)argb | (mlib_addr)rgbx) & 7) == 0) {
				mlib_d64 w_rgbx0, w_rgbx1, w_tmp0, w_tmp1,
					w_argb0, w_argb1;

				vis_alignaddr((void *)0, 7);
#pragma pipeloop(0)
				for (j = 0; j <= w - 4; j += 4) {
					w_rgbx0 = *((mlib_d64 *)(rgbx + j));
					w_rgbx1 = *((mlib_d64 *)(rgbx + j) + 1);
					w_tmp0 = vis_faligndata(w_rgbx0,
						w_rgbx0);
					w_tmp1 = vis_faligndata(w_rgbx1,
						w_rgbx1);
					w_tmp0 = vis_fand(w_tmp0, dmask);
					w_tmp1 = vis_fand(w_tmp1, dmask);
					w_argb0 = vis_for(w_tmp0, w_a);
					w_argb1 = vis_for(w_tmp1, w_a);
					*((mlib_d64 *)(argb + j)) = w_argb0;
					*((mlib_d64 *)(argb + j) + 1) = w_argb1;
				}

				for (; j < w; j++) {
					mlib_f32 f_tmp0, f_rgbx;

					f_rgbx = *((mlib_f32 *)(rgbx + j));
					w_rgbx0 = vis_freg_pair(f_rgbx, f_rgbx);
					w_tmp0 = vis_faligndata(w_rgbx0,
						w_rgbx0);
					f_tmp0 = vis_fands(vis_read_hi(w_tmp0),
						fmask);
					*((mlib_f32 *)argb + j) =
						vis_fors(vis_read_lo(w_a),
						f_tmp0);
				}
			} else {
				mlib_d64 w_rgbx0, w_rgbx1, w_tmp0;
				mlib_d64 *prgbx;

				vis_alignaddr((void *)0, 7);
				j = 0;

				if ((mlib_addr)argb & 7) {
					mlib_f32 f_tmp0, f_rgbx;

					f_rgbx = *((mlib_f32 *)(rgbx));
					w_rgbx0 = vis_freg_pair(f_rgbx, f_rgbx);
					w_tmp0 = vis_faligndata(w_rgbx0,
						w_rgbx0);
					f_tmp0 = vis_fands(vis_read_hi(w_tmp0),
						fmask);
					*((mlib_f32 *)argb) =
						vis_fors(vis_read_lo(w_a),
						f_tmp0);
					j++;
				}

				prgbx = (mlib_d64 *)vis_alignaddr((void *)(rgbx
					+ j), -1);
				w_rgbx0 = vis_ld_d64_nf(prgbx); prgbx++;
#pragma pipeloop(0)
				for (; j <= w - 2; j += 2) {
					w_rgbx1 = vis_ld_d64_nf(prgbx); prgbx++;
					w_tmp0 = vis_faligndata(w_rgbx0,
						w_rgbx1);
					w_tmp0 = vis_fand(w_tmp0, dmask);
					*((mlib_d64 *)(argb + j)) =
						vis_for(w_a, w_tmp0);
					w_rgbx0 = w_rgbx1;
				}

				vis_alignaddr((void *)0, 7);
				for (; j < w; j++) {
					mlib_f32 f_tmp0, f_rgbx;

					f_rgbx = *((mlib_f32 *)(rgbx + j));
					w_rgbx0 = vis_freg_pair(f_rgbx, f_rgbx);
					w_tmp0 = vis_faligndata(w_rgbx0,
						w_rgbx0);
					f_tmp0 = vis_fands(vis_read_hi(w_tmp0),
						fmask);
					*((mlib_f32 *)(argb + j)) =
						vis_fors(vis_read_lo(w_a),
						f_tmp0);
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
	mlib_s32 i, j;
	mlib_d64 zero = vis_fzero();
	mlib_f32 fone = vis_to_float(0x10001);
	mlib_f32 fmask = vis_to_float(0xFFFFFF);
	mlib_d64 dmask = vis_freg_pair(fmask, fmask);

	if (w < 1)
		return;

	dlb >>= 2;
	slb >>= 2;

	if (a_array) {
		for (i = 0; i < h;
			i++, abgr += dlb, xrgb += slb, a_array += alb) {
			if (((((mlib_addr)abgr | (mlib_addr)xrgb) & 7) == 0) &&
				(((mlib_addr)a_array & 3) == 0)) {
				mlib_d64 w_xrgb0, w_xrgb1, w_tmp0, w_tmp1,
					w_abgr0, w_abgr1;
				mlib_d64 w_a0, w_a1, w_a2;
				mlib_f32 w_a;

#pragma pipeloop(0)
				for (j = 0; j <= w - 4; j += 4) {
					w_a = *((mlib_f32 *)(a_array + j));
					w_xrgb0 = *((mlib_d64 *)(xrgb + j));
					w_xrgb1 = *((mlib_d64 *)(xrgb + j) + 1);
					w_a0 = vis_fpmerge(w_a, w_a);
					w_a1 = vis_fmuld8ulx16(vis_read_hi
						(w_a0), fone);
					w_a2 = vis_fmuld8ulx16(vis_read_lo
						(w_a0), fone);
					w_xrgb0 = vis_fpack32(w_xrgb0, zero);
					w_xrgb1 = vis_fpack32(w_xrgb1, zero);
					w_xrgb0 = vis_for(w_xrgb0, w_a1);
					w_xrgb1 = vis_for(w_xrgb1, w_a2);
					w_tmp0 = vis_fpmerge(vis_read_hi
						(w_xrgb0),
						vis_read_lo(w_xrgb0));
					w_tmp1 = vis_fpmerge(vis_read_lo
						(w_tmp0), vis_read_hi(w_tmp0));
					w_abgr0 =
						vis_fpmerge(vis_read_lo(w_tmp1),
						vis_read_hi(w_tmp1));
					*((mlib_d64 *)(abgr + j)) = w_abgr0;
					w_tmp0 = vis_fpmerge(vis_read_hi
						(w_xrgb1),
						vis_read_lo(w_xrgb1));
					w_tmp1 = vis_fpmerge(vis_read_lo
						(w_tmp0), vis_read_hi(w_tmp0));
					w_abgr1 =
						vis_fpmerge(vis_read_lo(w_tmp1),
						vis_read_hi(w_tmp1));
					*((mlib_d64 *)(abgr + j) + 1) = w_abgr1;
				}

				for (; j < w; j++) {
					mlib_f32 f_xrgb =
						*((mlib_f32 *)(xrgb + j));

					w_a0 = vis_ld_u8((void *)(a_array + j));
					w_xrgb0 = vis_freg_pair(f_xrgb, f_xrgb);
					w_xrgb0 = vis_fpack32(w_xrgb0, zero);
					w_xrgb0 = vis_for(w_xrgb0, w_a0);
					w_tmp0 = vis_fpmerge(vis_read_hi
						(w_xrgb0),
						vis_read_lo(w_xrgb0));
					w_tmp1 = vis_fpmerge(vis_read_lo
						(w_tmp0), vis_read_hi(w_tmp0));
					w_abgr0 =
						vis_fpmerge(vis_read_lo(w_tmp1),
						vis_read_hi(w_tmp1));
					*((mlib_f32 *)(abgr + j)) =
						vis_read_lo(w_abgr0);
				}
			} else {
				mlib_d64 w_tmp0, w_tmp1, w_tmp2, w_abgr0,
					w_abgr1;
				mlib_d64 w_a0, w_a1, w_a2, w_a3;
				mlib_f32 f_xrgb0, f_xrgb1, f_xrgb2, f_xrgb3;
				mlib_d64 w_xrgb0, w_xrgb1;

				j = 0;

				if ((mlib_addr)abgr & 7) {
					w_a0 = vis_ld_u8((void *)(a_array));
					w_a1 = vis_fpmerge(vis_read_lo(w_a0),
						zero);
					w_a2 = vis_fpmerge(vis_read_lo(w_a1),
						zero);
					f_xrgb0 = *((mlib_f32 *)xrgb);
					f_xrgb0 = vis_fands(f_xrgb0, fmask);
					f_xrgb0 =
						vis_fors(f_xrgb0,
						vis_read_lo(w_a2));
					w_tmp0 = vis_fpmerge(f_xrgb0, f_xrgb0);
					w_tmp1 = vis_fpmerge(vis_read_hi
						(w_tmp0), vis_read_lo(w_tmp0));
					w_tmp2 = vis_fpmerge(vis_read_lo
						(w_tmp0), vis_read_hi(w_tmp0));
					w_abgr0 =
						vis_fpmerge(vis_read_hi(w_tmp1),
						vis_read_lo(w_tmp2));
					*((mlib_f32 *)(abgr)) =
						vis_read_hi(w_abgr0);
					j++;
				}
#pragma pipeloop(0)
				for (; j <= w - 4; j += 4) {
					w_a0 = vis_ld_u8((void *)(a_array + j));
					w_a1 = vis_ld_u8((void *)(a_array + j +
						1));
					w_a2 = vis_ld_u8((void *)(a_array + j +
						2));
					w_a3 = vis_ld_u8((void *)(a_array + j +
						3));
					f_xrgb0 = *((mlib_f32 *)(xrgb + j));
					f_xrgb1 = *((mlib_f32 *)(xrgb + j) + 1);
					f_xrgb2 = *((mlib_f32 *)(xrgb + j) + 2);
					f_xrgb3 = *((mlib_f32 *)(xrgb + j) + 3);
					w_xrgb0 =
						vis_freg_pair(f_xrgb0, f_xrgb1);
					w_xrgb1 =
						vis_freg_pair(f_xrgb2, f_xrgb3);
					w_xrgb0 = vis_fpack32(w_xrgb0, zero);
					w_xrgb1 = vis_fpack32(w_xrgb1, zero);
					f_xrgb0 =
						vis_fors(vis_read_hi(w_xrgb0),
						vis_read_lo(w_a0));
					f_xrgb1 =
						vis_fors(vis_read_lo(w_xrgb0),
						vis_read_lo(w_a1));
					f_xrgb2 =
						vis_fors(vis_read_hi(w_xrgb1),
						vis_read_lo(w_a2));
					f_xrgb3 =
						vis_fors(vis_read_lo(w_xrgb1),
						vis_read_lo(w_a3));
					w_tmp0 = vis_freg_pair(f_xrgb0,
						f_xrgb1);
					w_tmp1 = vis_fpmerge(vis_read_hi
						(w_tmp0), vis_read_lo(w_tmp0));
					w_tmp2 = vis_fpmerge(vis_read_lo
						(w_tmp1), vis_read_hi(w_tmp1));
					w_abgr0 =
						vis_fpmerge(vis_read_lo(w_tmp2),
						vis_read_hi(w_tmp2));
					*((mlib_d64 *)(abgr + j)) = w_abgr0;
					w_tmp0 = vis_freg_pair(f_xrgb2,
						f_xrgb3);
					w_tmp1 = vis_fpmerge(vis_read_hi
						(w_tmp0), vis_read_lo(w_tmp0));
					w_tmp2 = vis_fpmerge(vis_read_lo
						(w_tmp1), vis_read_hi(w_tmp1));
					w_abgr1 =
						vis_fpmerge(vis_read_lo(w_tmp2),
						vis_read_hi(w_tmp2));
					*((mlib_d64 *)(abgr + j) + 1) = w_abgr1;
				}

				for (; j < w; j++) {
					w_a0 = vis_ld_u8((void *)(a_array + j));
					w_a1 = vis_fpmerge(vis_read_lo(w_a0),
						zero);
					w_a2 = vis_fpmerge(vis_read_lo(w_a1),
						zero);
					f_xrgb0 = *((mlib_f32 *)(xrgb + j));
					f_xrgb0 = vis_fands(f_xrgb0, fmask);
					f_xrgb0 =
						vis_fors(f_xrgb0,
						vis_read_lo(w_a2));
					w_tmp0 = vis_fpmerge(f_xrgb0, f_xrgb0);
					w_tmp1 = vis_fpmerge(vis_read_hi
						(w_tmp0), vis_read_lo(w_tmp0));
					w_tmp2 = vis_fpmerge(vis_read_lo
						(w_tmp0), vis_read_hi(w_tmp0));
					w_abgr0 =
						vis_fpmerge(vis_read_hi(w_tmp1),
						vis_read_lo(w_tmp2));
					*((mlib_f32 *)(abgr + j)) =
						vis_read_hi(w_abgr0);
				}
			}
		}
	} else {
		mlib_d64 w_a = vis_to_double_dup(a_const << 24);
		mlib_d64 w_a23 = vis_to_double_dup(a_const << 23);
		mlib_d64 w_xrgb0, w_xrgb1, w_tmp0, w_tmp1, w_abgr0, w_abgr1;
		mlib_f32 f_src;

		vis_write_gsr((0 << 3) | 3);

		for (i = 0; i < h; i++, abgr += dlb, xrgb += slb) {
			mlib_f32 *f_xrgb = (mlib_f32 *)xrgb;
			mlib_f32 *f_abgr = (mlib_f32 *)abgr;

			j = 0;

			if ((mlib_addr)xrgb & 7) {
				f_src = f_xrgb[0];
				w_xrgb0 = vis_freg_pair(f_src, f_src);
				w_xrgb0 = vis_fpack32(w_xrgb0, w_a23);
				w_tmp0 = vis_fpmerge(vis_read_hi(w_xrgb0),
					vis_read_lo(w_xrgb0));
				w_tmp1 = vis_fpmerge(vis_read_lo(w_tmp0),
					vis_read_hi(w_tmp0));
				w_abgr0 =
					vis_fpmerge(vis_read_lo(w_tmp1),
					vis_read_hi(w_tmp1));
				f_abgr[0] = vis_read_lo(w_abgr0);
				j++;
			}

			if ((mlib_addr)(f_abgr + j) & 7) {
#pragma pipeloop(0)
				for (; j <= w - 4; j += 4) {
					w_xrgb0 =
						vis_ld_d64_le((mlib_d64 *)(xrgb
						+ j));
					w_xrgb1 =
						vis_ld_d64_le((mlib_d64 *)(xrgb
						+ j) + 1);
					w_tmp0 = vis_faligndata(w_xrgb0,
						w_xrgb0);
					w_tmp1 = vis_faligndata(w_xrgb1,
						w_xrgb1);
					w_tmp0 = vis_fand(w_tmp0, dmask);
					w_tmp1 = vis_fand(w_tmp1, dmask);

					w_abgr0 = vis_for(w_tmp0, w_a);
					w_abgr1 = vis_for(w_tmp1, w_a);

					f_abgr[j] = vis_read_hi(w_abgr0);
					f_abgr[j + 1] = vis_read_lo(w_abgr0);
					f_abgr[j + 2] = vis_read_hi(w_abgr1);
					f_abgr[j + 3] = vis_read_lo(w_abgr1);
				}
			} else {
#pragma pipeloop(0)
				for (; j <= w - 4; j += 4) {
					w_xrgb0 =
						vis_ld_d64_le((mlib_d64 *)(xrgb
						+ j));
					w_xrgb1 =
						vis_ld_d64_le((mlib_d64 *)(xrgb
						+ j) + 1);

					w_tmp0 = vis_faligndata(w_xrgb0,
						w_xrgb0);
					w_tmp1 = vis_faligndata(w_xrgb1,
						w_xrgb1);
					w_tmp0 = vis_fand(w_tmp0, dmask);
					w_tmp1 = vis_fand(w_tmp1, dmask);
					w_abgr0 = vis_for(w_tmp0, w_a);
					w_abgr1 = vis_for(w_tmp1, w_a);
					((mlib_d64 *)(f_abgr + j))[0] = w_abgr0;
					((mlib_d64 *)(f_abgr + j))[1] = w_abgr1;
				}
			}

			for (; j < w; j++) {
				f_src = f_xrgb[j];
				w_xrgb0 = vis_freg_pair(f_src, f_src);
				w_xrgb0 = vis_fpack32(w_xrgb0, w_a23);
				w_tmp0 = vis_fpmerge(vis_read_hi(w_xrgb0),
					vis_read_lo(w_xrgb0));
				w_tmp1 = vis_fpmerge(vis_read_lo(w_tmp0),
					vis_read_hi(w_tmp0));
				w_abgr0 =
					vis_fpmerge(vis_read_lo(w_tmp1),
					vis_read_hi(w_tmp1));
				f_abgr[j] = vis_read_lo(w_abgr0);
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
	mlib_s32 i, j;
	mlib_f32 zero = vis_fzeros(), fmask = vis_to_float(0xFFFFFF);
	mlib_d64 dmask = vis_freg_pair(fmask, fmask);

	dlb >>= 2;
	slb >>= 2;

	if (h == 0 || w == 0)
		return;

	if (a_array) {
		for (i = 0; i < h;
			i++, xrgb += slb, a_array += alb, argb += dlb) {
			if (((((mlib_addr)argb | (mlib_addr)xrgb) & 7) == 0) &&
				(((mlib_addr)a_array & 3) == 0)) {
				mlib_d64 w_xrgb0, w_xrgb1, w_argb0, w_argb1,
					w_tmp0, w_tmp1, w_tmp2;
				mlib_f32 w_a;

#pragma pipeloop(0)
				for (j = 0; j <= w - 4; j += 4) {
					w_a = *((mlib_f32 *)(a_array + j));
					w_xrgb0 = *((mlib_d64 *)(xrgb + j));
					w_xrgb1 = *((mlib_d64 *)(xrgb + j) + 1);
					w_tmp0 = vis_fpmerge(w_a, zero);
					w_tmp1 = vis_fpmerge(vis_read_hi
						(w_tmp0), zero);
					w_tmp2 = vis_fpmerge(vis_read_lo
						(w_tmp0), zero);
					w_argb0 = vis_fand(w_xrgb0, dmask);
					w_argb1 = vis_fand(w_xrgb1, dmask);
					*((mlib_d64 *)(argb + j)) =
						vis_for(w_argb0, w_tmp1);
					*((mlib_d64 *)(argb + j) + 1) =
						vis_for(w_argb1, w_tmp2);
				}

				for (; j < w; j++) {
					mlib_f32 f_tmp0 =
						vis_fands(*((mlib_f32 *)xrgb +
						j), fmask);
					mlib_f32 f_tmp1 =
						vis_to_float(a_array[j] << 24);

					*((mlib_f32 *)argb + j) =
						vis_fors(f_tmp0, f_tmp1);
				}
			} else {
				j = 0;

				if ((mlib_addr)argb & 7) {
					mlib_f32 f_tmp0 =
						vis_fands(*((mlib_f32 *)xrgb),
						fmask);
					mlib_d64 dld =
						vis_ld_u8((void *)(a_array)),
						d_tmp0, d_tmp1;

					d_tmp0 = vis_fpmerge(vis_read_lo(dld),
						zero);
					d_tmp1 = vis_fpmerge(vis_read_lo
						(d_tmp0), zero);
					*((mlib_f32 *)argb) =
						vis_fors(vis_read_lo(d_tmp1),
						f_tmp0);
					j++;
				}
#pragma pipeloop(0)
				for (; j <= w - 4; j += 4) {
					mlib_d64 dld0, dld1, dld2, dld3;
					mlib_d64 d_tmp0, d_tmp1, d_tmp2, d_tmp3,
						d_tmp4, d_tmp5;
					mlib_f32 f_tmp0, f_tmp1, f_tmp2, f_tmp3;

					f_tmp0 = vis_fands(*((mlib_f32 *)xrgb +
						j), fmask);
					f_tmp1 = vis_fands(*((mlib_f32 *)xrgb +
						j + 1), fmask);
					f_tmp2 = vis_fands(*((mlib_f32 *)xrgb +
						j + 2), fmask);
					f_tmp3 = vis_fands(*((mlib_f32 *)xrgb +
						j + 3), fmask);
					dld0 = vis_ld_u8((void *)(a_array + j));
					dld1 = vis_ld_u8((void *)(a_array + j +
						1));
					dld2 = vis_ld_u8((void *)(a_array + j +
						2));
					dld3 = vis_ld_u8((void *)(a_array + j +
						3));
					d_tmp0 = vis_fpmerge(vis_read_lo(dld0),
						vis_read_lo(dld2));
					d_tmp1 = vis_fpmerge(vis_read_lo(dld1),
						vis_read_lo(dld3));
					d_tmp2 = vis_fpmerge(vis_read_lo
						(d_tmp0), vis_read_lo(d_tmp1));
					d_tmp3 = vis_fpmerge(vis_read_lo
						(d_tmp2), zero);
					d_tmp4 = vis_fpmerge(vis_read_hi
						(d_tmp3), zero);
					d_tmp5 = vis_fpmerge(vis_read_lo
						(d_tmp3), zero);
					*((mlib_d64 *)(argb + j)) =
						vis_for(d_tmp4,
						vis_freg_pair(f_tmp0, f_tmp1));
					*((mlib_d64 *)(argb + j) + 1) =
						vis_for(d_tmp5,
						vis_freg_pair(f_tmp2, f_tmp3));
				}

				for (; j < w; j++) {
					mlib_f32 f_tmp0 =
						vis_fands(*((mlib_f32 *)xrgb +
						j), fmask);
					mlib_d64 dld =
						vis_ld_u8((void *)(a_array +
						j)), d_tmp0, d_tmp1;

					d_tmp0 = vis_fpmerge(vis_read_lo(dld),
						zero);
					d_tmp1 = vis_fpmerge(vis_read_lo
						(d_tmp0), zero);
					*((mlib_f32 *)argb + j) =
						vis_fors(vis_read_lo(d_tmp1),
						f_tmp0);
				}
			}
		}
	} else {
		mlib_f32 f_a = vis_to_float(a_const << 24);
		mlib_d64 w_a = vis_freg_pair(f_a, f_a);

		for (i = 0; i < h; i++, xrgb += slb, argb += dlb) {
			if ((((mlib_addr)argb | (mlib_addr)xrgb) & 7) == 0) {
				mlib_d64 w_xrgb0, w_xrgb1, w_argb0, w_argb1;

#pragma pipeloop(0)
				for (j = 0; j <= w - 4; j += 4) {
					w_xrgb0 = *((mlib_d64 *)(xrgb + j));
					w_xrgb1 = *((mlib_d64 *)(xrgb + j) + 1);
					w_argb0 = vis_fand(w_xrgb0, dmask);
					w_argb1 = vis_fand(w_xrgb1, dmask);
					*((mlib_d64 *)(argb + j)) =
						vis_for(w_argb0, w_a);
					*((mlib_d64 *)(argb + j) + 1) =
						vis_for(w_argb1, w_a);
				}

				for (; j < w; j++) {
					mlib_f32 f_tmp0 =
						vis_fands(*((mlib_f32 *)xrgb +
						j), fmask);

					*((mlib_f32 *)argb + j) =
						vis_fors(f_tmp0, f_a);
				}
			} else {
				j = 0;

				if ((mlib_addr)argb & 7) {
					mlib_f32 f_tmp0 =
						vis_fands(*((mlib_f32 *)xrgb),
						fmask);

					*((mlib_f32 *)argb) =
						vis_fors(f_a, f_tmp0);
					j++;
				}
#pragma pipeloop(0)
				for (; j <= w - 4; j += 4) {
					mlib_f32 f_tmp0, f_tmp1, f_tmp2, f_tmp3;

					f_tmp0 = vis_fands(*((mlib_f32 *)xrgb +
						j), fmask);
					f_tmp1 = vis_fands(*((mlib_f32 *)xrgb +
						j + 1), fmask);
					f_tmp2 = vis_fands(*((mlib_f32 *)xrgb +
						j + 2), fmask);
					f_tmp3 = vis_fands(*((mlib_f32 *)xrgb +
						j + 3), fmask);
					*((mlib_d64 *)(argb + j)) =
						vis_for(w_a,
						vis_freg_pair(f_tmp0, f_tmp1));
					*((mlib_d64 *)(argb + j) + 1) =
						vis_for(w_a,
						vis_freg_pair(f_tmp2, f_tmp3));
				}

				for (; j < w; j++) {
					mlib_f32 f_tmp0 =
						vis_fands(*((mlib_f32 *)xrgb +
						j), fmask);

					*((mlib_f32 *)argb + j) =
						vis_fors(f_a, f_tmp0);
				}
			}
		}
	}
}

/* *********************************************************** */
