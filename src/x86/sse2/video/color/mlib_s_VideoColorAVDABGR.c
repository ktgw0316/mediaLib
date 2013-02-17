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

#pragma ident	"@(#)mlib_s_VideoColorAVDABGR.c	9.3	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_VideoColorRGBseq_to_ABGRint - convert RGB sequential to ABGR
 *                                         interleaved
 *
 * SYNOPSIS
 *     void mlib_VideoColorRGBseq_to_ABGRint(mlib_u32      *abgr,
 *                                           const mlib_u8 *r,
 *                                           const mlib_u8 *g,
 *                                           const mlib_u8 *b,
 *                                           const mlib_u8 *a,
 *                                           mlib_u8       alpha,
 *                                           mlib_s32      width,
 *                                           mlib_s32      height,
 *                                           mlib_s32      dst_stride,
 *                                           mlib_s32      src_stride)
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
 *     void mlib_VideoColorRGBint_to_ABGRint(mlib_u32      *abgr,
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
 *      mlib_VideoColorRGBint_to_ARGBint - convert RGB interleaved to ARGB
 *                                         interleaved
 *
 * SYNOPSIS
 *     void mlib_VideoColorRGBint_to_ARGBint(mlib_u32      *argb,
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
 *      argb        pointer to output buffer
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
 *      a A, R, G, B interleaved byte stream. Within each 24-bit input pixel,
 *      the component ordering is R (bits 23-16), G (bits 15-8), and
 *      B (bits 7-0).
 *      Within each 32-bit output word, the component ordering is
 *      A (bits 31-24), R (bits 23-16), G (bits 15-8), and B (bits 7-0).
 *      The alpha values for this function work in the following fashion:
 *      - if a pointer is not NULL the values are taken from there.
 *      It has to have the same dimensions as the R, G, and B buffers.
 *      - if a pointer is NULL, the alpha values for every pixel are set
 *      to alpha.
 */

/*
 * FUNCTION
 *      mlib_VideoColorBGRint_to_ABGRint - convert BGR interleaved to ABGR
 *                                         interleaved
 *
 * SYNOPSIS
 *     void mlib_VideoColorBGRint_to_ABGRint(mlib_u32      *abgr,
 *                                           const mlib_u8 *BGR,
 *                                           const mlib_u8 *a,
 *                                           mlib_u8       alpha,
 *                                           mlib_s32      width,
 *                                           mlib_s32      height,
 *                                           mlib_s32      dst_stride,
 *                                           mlib_s32      src_stride,
 *                                           mlib_s32      a_stride)
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
 *      mlib_VideoColorBGRint_to_ARGBint - convert BGR interleaved to ARGB
 *                                         interleaved
 *
 * SYNOPSIS
 *     void mlib_VideoColorBGRint_to_ARGBint(mlib_u32      *argb,
 *                                           const mlib_u8 *BGR,
 *                                           const mlib_u8 *a,
 *                                           mlib_u8       alpha,
 *                                           mlib_s32      width,
 *                                           mlib_s32      height,
 *                                           mlib_s32      dst_stride,
 *                                           mlib_s32      src_stride,
 *                                           mlib_s32      a_stride)
 *
 * ARGUMENTS
 *      argb        pointer to output buffer
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
 *      a A, R, G, B interleaved byte stream. Within each 24-bit input pixel,
 *      the component ordering is B (bits 23-16), G (bits 15-8), and
 *      R (bits 7-0).
 *      Within each 32-bit output word, the component ordering is
 *      A (bits 31-24), R (bits 23-16), G (bits 15-8), and B (bits 7-0).
 *      The alpha values for this function work in the following fashion:
 *      - if a pointer is not NULL the values are taken from there.
 *      It has to have the same dimensions as the R, G, and B buffers.
 *      - if a pointer is NULL, the alpha values for every pixel are
 *      set to alpha.
 */

/*
 * FUNCTION
 *      mlib_VideoColorBGRAint_to_ARGBint - convert BGRA interleaved to ARGB
 *
 * SYNOPSIS
 *     void mlib_VideoColorBGRAint_to_ARGBint(mlib_u32       *bgra,
 *                                            const mlib_u32 *argb,
 *                                            mlib_s32       width,
 *                                            mlib_s32       height,
 *                                            mlib_s32       dst_stride,
 *                                            mlib_s32       src_stride)
 *
 * ARGUMENTS
 *      bgra        pointer to output buffer
 *      argb        pointer to input buffer
 *      width       image width in pixels
 *      height      image height in lines
 *      dst_stride  linebytes for output buffer
 *      src_stride  linebytes for input buffer
 *
 * DESCRIPTION
 *      The ARGB pixel stream is broken apart and recombined into a BGRA
 *      pixel stream. All pixel components are 8-bit unsigned integers.
 *      The buffers has dimensions width and height.
 *      Within each 32-bit input word, the component ordering is
 *      A (bits 31-24), R (bits 23-16), G (bits 15-8), and B (bits 7-0).
 *      Within each 32-bit output word, the component ordering is
 *      B (bits 31-24), G (bits 23-16), R (bits 15-8), and A (bits 7-0).
 */

/*
 * FUNCTION
 *      mlib_VideoColorRGBAint_to_ARGBint - convert RGBA interleaved to ARGB
 *
 * SYNOPSIS
 *     void mlib_VideoColorRGBAint_to_ARGBint(mlib_u32       *argb,
 *                                            const mlib_u32 *rgba,
 *                                            mlib_s32       width,
 *                                            mlib_s32       height,
 *                                            mlib_s32       dst_stride,
 *                                            mlib_s32       src_stride)
 *
 * ARGUMENTS
 *      argb        pointer to output buffer
 *      rgba        pointer to input buffer
 *      width       image width in pixels
 *      height      image height in lines
 *      dst_stride  linebytes for output buffer
 *      src_stride  linebytes for input buffer
 *
 * DESCRIPTION
 *      The RGBA pixel stream is broken apart and recombined into a ARGB
 *      pixel stream. All pixel components are 8-bit unsigned integers.
 *      The buffers has dimensions width and height.
 *      Within each 32-bit input word, the component ordering is
 *      R (bits 31-24), G (bits 23-16), B (bits 15-8), and A (bits 7-0).
 *      Within each 32-bit output word, the component ordering is
 *      A (bits 31-24), R (bits 23-16), G (bits 15-8), and B (bits 7-0).
 */

/*
 * FUNCTION
 *      mlib_VideoColorABGRint_to_ARGBint - convert ABGR interleaved to ARGB
 *
 * SYNOPSIS
 *     void mlib_VideoColorABGRint_to_ARGBint(mlib_u32       *argb,
 *                                            const mlib_u32 *abgr,
 *                                            mlib_s32       width,
 *                                            mlib_s32       height,
 *                                            mlib_s32       dst_stride,
 *                                            mlib_s32       src_stride)
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
 *     void mlib_VideoColorRGBAint_to_ABGRint(mlib_u32       *abgr,
 *                                            const mlib_u32 *rgba,
 *                                            mlib_s32       width,
 *                                            mlib_s32       height,
 *                                            mlib_s32       dst_stride,
 *                                            mlib_s32       src_stride)
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
 *     void mlib_VideoColorBGRAint_to_ABGRint(mlib_u32       *abgr,
 *                                            const mlib_u32 *bgra,
 *                                            mlib_s32       width,
 *                                            mlib_s32       height,
 *                                            mlib_s32       dst_stride,
 *                                            mlib_s32       src_stride)
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
 *      void mlib_VideoColorRGBXint_to_ABGRint(mlib_u32       *abgr,
 *                                             const mlib_u32 *rgbx,
 *                                             const mlib_u8  *a,
 *                                             mlib_u8        alpha,
 *                                             mlib_s32       width,
 *                                             mlib_s32       height,
 *                                             mlib_s32       dst_stride,
 *                                             mlib_s32       src_stride,
 *                                             mlib_s32       a_stride)
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
 *      void mlib_VideoColorRGBXint_to_ARGBint(mlib_u32       *argb,
 *                                             const mlib_u32 *rgbx,
 *                                             const mlib_u8  *a,
 *                                             mlib_u8        alpha,
 *                                             mlib_s32       width,
 *                                             mlib_s32       height,
 *                                             mlib_s32       dst_stride,
 *                                             mlib_s32       src_stride,
 *                                             mlib_s32       a_stride)
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
 *      void mlib_VideoColorXRGBint_to_ABGRint(mlib_u32       *abgr,
 *                                             const mlib_u32 *xrgb,
 *                                             const mlib_u8  *a,
 *                                             mlib_u8        alpha,
 *                                             mlib_s32       width,
 *                                             mlib_s32       height,
 *                                             mlib_s32       dst_stride,
 *                                             mlib_s32       src_stride,
 *                                             mlib_s32       a_stride)
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
 *      void mlib_VideoColorXRGBint_to_ARGBint(mlib_u32       *argb,
 *                                             const mlib_u32 *xrgb,
 *                                             const mlib_u8  *a,
 *                                             mlib_u8        alpha,
 *                                             mlib_s32       width,
 *                                             mlib_s32       height,
 *                                             mlib_s32       dst_stride,
 *                                             mlib_s32       src_stride,
 *                                             mlib_s32       a_stride)
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
#ifdef __SUNPRO_C
#include <sunmedia_intrin.h>
#else
#include <emmintrin.h>
#endif

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

void
__mlib_VideoColorABGRint_to_ARGBint(
	mlib_u32 *argb,
	const mlib_u32 *abgr,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb)
{
	const __m128i x_mask_r = _mm_set1_epi32(0xff00);
	const __m128i x_mask_ag = _mm_set1_epi32(0xff00ff);
	const __m128i x_mask_b = _mm_set1_epi32(0xff000000);
	__m128i x_abgr, x_argb, x_r, x_ag, x_b, x_temp;
	__m128i *px_abgr, *px_argb;
	mlib_s32 i, j;

	dlb >>= 2;
	slb >>= 2;

	if ((0 == (0xf & ((mlib_addr)abgr | (mlib_addr)argb))) &&
			(0 == (0x3 & (dlb | slb)))) {
		for (i = 0; i < h; i++) {
			px_abgr = (__m128i *)abgr;
			px_argb = (__m128i *)argb;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j <= (w - 4); j += 4) {
				x_abgr = _mm_load_si128(px_abgr);
				px_abgr++;
				x_ag = _mm_and_si128(x_mask_ag, x_abgr);
				x_temp = _mm_slli_si128(x_abgr, 2);
				x_b = _mm_and_si128(x_mask_b, x_temp);
				x_temp = _mm_srli_si128(x_abgr, 2);
				x_r = _mm_and_si128(x_mask_r, x_temp);
				x_argb = _mm_or_si128(x_ag, x_b);
				x_argb = _mm_or_si128(x_argb, x_r);
				_mm_store_si128(px_argb, x_argb);
				px_argb++;
			}

			if (j <= (w - 2)) {
				x_abgr = _mm_loadl_epi64(px_abgr);
				x_ag = _mm_and_si128(x_mask_ag, x_abgr);
				x_temp = _mm_slli_si128(x_abgr, 2);
				x_b = _mm_and_si128(x_mask_b, x_temp);
				x_temp = _mm_srli_si128(x_abgr, 2);
				x_r = _mm_and_si128(x_mask_r, x_temp);
				x_argb = _mm_or_si128(x_ag, x_b);
				x_argb = _mm_or_si128(x_argb, x_r);
				_mm_storel_epi64(px_argb, x_argb);
				px_argb = (__m128i *)((__m64 *)px_argb + 1);

				j += 2;
			}

			if (j < w) {
				mlib_u16 *src = (mlib_u16 *)(abgr + j);
				mlib_u16 *dst = (mlib_u16 *)(argb + j);
				mlib_u32 val0, val1, val2, val3;

				val0 = src[0];
				val1 = src[1];
				val3 = (val0 & 0xFF00) | (val1 & 0xFF);
				val2 = (val0 & 0xFF) | (val1 & 0xFF00);
				dst[0] = (mlib_u16)val2;
				dst[1] = (mlib_u16)val3;
			}

			argb += dlb;
			abgr += slb;
		}
	} else {
		for (i = 0; i < h; i++) {
			px_abgr = (__m128i *)abgr;
			px_argb = (__m128i *)argb;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j <= (w - 4); j += 4) {
				x_abgr = _mm_loadu_si128(px_abgr);
				px_abgr++;
				x_ag = _mm_and_si128(x_mask_ag, x_abgr);
				x_temp = _mm_slli_si128(x_abgr, 2);
				x_b = _mm_and_si128(x_mask_b, x_temp);
				x_temp = _mm_srli_si128(x_abgr, 2);
				x_r = _mm_and_si128(x_mask_r, x_temp);
				x_argb = _mm_or_si128(x_ag, x_b);
				x_argb = _mm_or_si128(x_argb, x_r);
				_mm_storeu_si128(px_argb, x_argb);
				px_argb++;
			}

			if (j <= (w - 2)) {
				x_abgr = _mm_loadl_epi64(px_abgr);
				x_ag = _mm_and_si128(x_mask_ag, x_abgr);
				x_temp = _mm_slli_si128(x_abgr, 2);
				x_b = _mm_and_si128(x_mask_b, x_temp);
				x_temp = _mm_srli_si128(x_abgr, 2);
				x_r = _mm_and_si128(x_mask_r, x_temp);
				x_argb = _mm_or_si128(x_ag, x_b);
				x_argb = _mm_or_si128(x_argb, x_r);
				_mm_storel_epi64(px_argb, x_argb);
				px_argb = (__m128i *)((__m64 *)px_argb + 1);

				j += 2;
			}

			if (j < w) {
				mlib_u16 *src = (mlib_u16 *)(abgr + j);
				mlib_u16 *dst = (mlib_u16 *)(argb + j);
				mlib_u32 val0, val1, val2, val3;

				val0 = src[0];
				val1 = src[1];
				val3 = (val0 & 0xFF00) | (val1 & 0xFF);
				val2 = (val0 & 0xFF) | (val1 & 0xFF00);
				dst[0] = (mlib_u16)val2;
				dst[1] = (mlib_u16)val3;
			}

			argb += dlb;
			abgr += slb;
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
	mlib_s32 i, j;
	mlib_u32 *pbgra, *pabgr;
	__m128i x_bgra, x_abgr, x_a, x_bgr;
	const __m128i x_mask_a = _mm_set1_epi32(0xff);
	const __m128i x_mask_bgr = _mm_set1_epi32(0xffffff00);

	dlb >>= 2;
	slb >>= 2;

	if ((0 == (0xf & ((mlib_addr)bgra | (mlib_addr)abgr))) &&
			(0 == (0x3 & (dlb | slb)))) {
			for (i = 0; i < h; i++, abgr += dlb, bgra += slb) {
			pbgra = (mlib_u32 *)bgra;
			pabgr = abgr;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j <= (w - 4); j += 4) {
				x_bgra = _mm_load_si128((__m128i *)pbgra);
				pbgra += 4;
				x_bgr = _mm_slli_si128(x_bgra, 1);
				x_bgr = _mm_and_si128(x_bgr, x_mask_bgr);
				x_a = _mm_srli_si128(x_bgra, 3);
				x_a = _mm_and_si128(x_a, x_mask_a);
				x_abgr = _mm_or_si128(x_a, x_bgr);
				_mm_store_si128((__m128i *)pabgr, x_abgr);
				pabgr += 4;
			}

			if (j <= (w - 2)) {
				x_bgra = _mm_loadl_epi64((__m128i *)pbgra);
				pbgra += 2;
				x_bgr = _mm_slli_si128(x_bgra, 1);
				x_bgr = _mm_and_si128(x_bgr, x_mask_bgr);
				x_a = _mm_srli_si128(x_bgra, 3);
				x_a = _mm_and_si128(x_a, x_mask_a);
				x_abgr = _mm_or_si128(x_a, x_bgr);
				_mm_storel_epi64((__m128i *)pabgr, x_abgr);
				pabgr += 2;

				j += 2;
			}

			if (j < w) {
				mlib_u16 *src = (mlib_u16 *)(bgra + j);
				mlib_u16 *dst = (mlib_u16 *)(abgr + j);
				mlib_u32 val0, val1, val2, val3;

				val0 = src[0];
				val1 = src[1];
				val3 = (val0 >> 8) | (val1 << 8);
				val2 = (val0 << 8) | (val1 >> 8);
				dst[0] = (mlib_u16)val2;
				dst[1] = (mlib_u16)val3;
			}
		}
	} else {
		for (i = 0; i < h; i++, abgr += dlb, bgra += slb) {
			pbgra = (mlib_u32 *)bgra;
			pabgr = abgr;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j <= (w - 4); j += 4) {
				x_bgra = _mm_loadu_si128((__m128i *)pbgra);
				pbgra += 4;
				x_bgr = _mm_slli_si128(x_bgra, 1);
				x_bgr = _mm_and_si128(x_bgr, x_mask_bgr);
				x_a = _mm_srli_si128(x_bgra, 3);
				x_a = _mm_and_si128(x_a, x_mask_a);
				x_abgr = _mm_or_si128(x_a, x_bgr);
				_mm_storeu_si128((__m128i *)pabgr, x_abgr);
				pabgr += 4;
			}

			if (j <= (w - 2)) {
				x_bgra = _mm_loadl_epi64((__m128i *)pbgra);
				pbgra += 2;
				x_bgr = _mm_slli_si128(x_bgra, 1);
				x_bgr = _mm_and_si128(x_bgr, x_mask_bgr);
				x_a = _mm_srli_si128(x_bgra, 3);
				x_a = _mm_and_si128(x_a, x_mask_a);
				x_abgr = _mm_or_si128(x_a, x_bgr);
				_mm_storel_epi64((__m128i *)pabgr, x_abgr);
				pabgr += 2;

				j += 2;
			}

			if (j < w) {
				mlib_u16 *src = (mlib_u16 *)(bgra + j);
				mlib_u16 *dst = (mlib_u16 *)(abgr + j);
				mlib_u32 val0, val1, val2, val3;

				val0 = src[0];
				val1 = src[1];
				val3 = (val0 >> 8) | (val1 << 8);
				val2 = (val0 << 8) | (val1 >> 8);
				dst[0] = (mlib_u16)val2;
				dst[1] = (mlib_u16)val3;
			}
		}
	}
}

/* *********************************************************** */

void
__mlib_VideoColorRGBAint_to_ABGRint(
	mlib_u32 *abgr,
	const mlib_u32 *rgba,
	mlib_s32 w,
	mlib_s32 h,
	mlib_s32 dlb,
	mlib_s32 slb)
{
	mlib_s32 i, j;
	mlib_u32 *prgba, *pabgr;
	__m128i x_rgba, x_abgr, x_temp, x_ag, x_br;
	const __m128i x_mask_ag = _mm_set1_epi32(0xff00ff);
	const __m128i x_mask_br = _mm_set1_epi32(0xff00ff00);

	dlb >>= 2;
	slb >>= 2;
	if ((0 == (0xf & ((mlib_addr)rgba | (mlib_addr)abgr))) &&
			(0 == (0x3 & (dlb | slb)))) {
		for (i = 0; i < h; i++, abgr += dlb, rgba += slb) {
			prgba = (mlib_u32 *)rgba;
			pabgr = abgr;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j <= (w - 4); j += 4) {
				x_rgba = _mm_load_si128((__m128i *)prgba);
				prgba += 4;
				/* 10110001b = 0xb1 */
				x_temp = _mm_shufflehi_epi16(x_rgba, 0xb1);
				x_temp = _mm_shufflelo_epi16(x_temp, 0xb1);
				x_ag = _mm_srli_si128(x_temp, 1);
				x_ag = _mm_and_si128(x_mask_ag, x_ag);
				x_br = _mm_slli_si128(x_temp, 1);
				x_br = _mm_and_si128(x_mask_br, x_br);
				x_abgr = _mm_or_si128(x_ag, x_br);
				_mm_store_si128((__m128i *)pabgr, x_abgr);
				pabgr += 4;
			}

			for (; j < w; j++) {
				mlib_u16 *src = (mlib_u16 *)(rgba + j);
				mlib_u16 *dst = (mlib_u16 *)(abgr + j);
				mlib_u32 val0, val1;
				val0 = src[0];
				val1 = src[1];
				val0 = (val0 >> 8) | (val0 << 8);
				val1 = (val1 >> 8) | (val1 << 8);
				dst[0] = (mlib_u16)val1;
				dst[1] = (mlib_u16)val0;
			}
		}
	} else {
		for (i = 0; i < h; i++, abgr += dlb, rgba += slb) {
			prgba = (mlib_u32 *)rgba;
			pabgr = abgr;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j <= (w - 4); j += 4) {
				x_rgba = _mm_loadu_si128((__m128i *)prgba);
				prgba += 4;
				/* 10110001b = 0xb1 */
				x_temp = _mm_shufflehi_epi16(x_rgba, 0xb1);
				x_temp = _mm_shufflelo_epi16(x_temp, 0xb1);
				x_ag = _mm_srli_si128(x_temp, 1);
				x_ag = _mm_and_si128(x_mask_ag, x_ag);
				x_br = _mm_slli_si128(x_temp, 1);
				x_br = _mm_and_si128(x_mask_br, x_br);
				x_abgr = _mm_or_si128(x_ag, x_br);
				_mm_storeu_si128((__m128i *)pabgr, x_abgr);
				pabgr += 4;
			}

			for (; j < w; j++) {
				mlib_u16 *src = (mlib_u16 *)(rgba + j);
				mlib_u16 *dst = (mlib_u16 *)(abgr + j);
				mlib_u32 val0, val1;
				val0 = src[0];
				val1 = src[1];
				val0 = (val0 >> 8) | (val0 << 8);
				val1 = (val1 >> 8) | (val1 << 8);
				dst[0] = (mlib_u16)val1;
				dst[1] = (mlib_u16)val0;
			}
		}
	}
}

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
	__m128i x_r, x_g, x_b, x_a, x_abgr, x_const;
	__m128i x_ab_l, x_ab_h, x_gr_l, x_gr_h;
	mlib_u8 *pr, *pg, *pb, *pa;
	mlib_u32 *pabgr;
	mlib_s32 i, j;

	if (h == 0 || w == 0)
		return;

	dlb >>= 2;

	if (a_array) {
		if ((0 == (0xf & ((mlib_addr)a_array | (mlib_addr)b |
			(mlib_addr)g | (mlib_addr)r | (mlib_addr)abgr))) &&
			(0 == (0xf & slb)) && (0 == (0x3 & dlb))) {
			for (i = 0; i < h; i++,
			    a_array += slb, b += slb, g += slb,
			    r += slb, abgr += dlb) {
				pa = (mlib_u8 *)a_array;
				pb = (mlib_u8 *)b;
				pr = (mlib_u8 *)r;
				pg = (mlib_u8 *)g;
				pabgr = abgr;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j <= (w - 16); j += 16) {
					x_a = _mm_load_si128((__m128i *)pa);
					pa += 16;
					x_b = _mm_load_si128((__m128i *)pb);
					pb += 16;
					x_g = _mm_load_si128((__m128i *)pg);
					pg += 16;
					x_r = _mm_load_si128((__m128i *)pr);
					pr += 16;
					x_ab_l = _mm_unpacklo_epi8(x_a, x_b);
					x_ab_h = _mm_unpackhi_epi8(x_a, x_b);
					x_gr_l = _mm_unpacklo_epi8(x_g, x_r);
					x_gr_h = _mm_unpackhi_epi8(x_g, x_r);

					x_abgr = _mm_unpacklo_epi16(
						x_ab_l, x_gr_l);
					_mm_store_si128(
						(__m128i *)pabgr, x_abgr);
					pabgr += 4;
					x_abgr = _mm_unpackhi_epi16(
						x_ab_l, x_gr_l);
					_mm_store_si128(
						(__m128i *)pabgr, x_abgr);
					pabgr += 4;
					x_abgr = _mm_unpacklo_epi16(
						x_ab_h, x_gr_h);
					_mm_store_si128(
						(__m128i *)pabgr, x_abgr);
					pabgr += 4;
					x_abgr = _mm_unpackhi_epi16(
						x_ab_h, x_gr_h);
					_mm_store_si128(
						(__m128i *)pabgr, x_abgr);
					pabgr += 4;
				}

				if (j <= (w - 8)) {
					x_a = _mm_loadl_epi64((__m128i *)pa);
					pa += 8;
					x_b = _mm_loadl_epi64((__m128i *)pb);
					pb += 8;
					x_g = _mm_loadl_epi64((__m128i *)pg);
					pg += 8;
					x_r = _mm_loadl_epi64((__m128i *)pr);
					pr += 8;
					x_ab_l = _mm_unpacklo_epi8(x_a, x_b);
					x_gr_l = _mm_unpacklo_epi8(x_g, x_r);

					x_abgr = _mm_unpacklo_epi16(
						x_ab_l, x_gr_l);
					_mm_store_si128(
						(__m128i *)pabgr, x_abgr);
					pabgr += 4;
					x_abgr = _mm_unpackhi_epi16(
						x_ab_l, x_gr_l);
					_mm_store_si128(
						(__m128i *)pabgr, x_abgr);
					pabgr += 4;

					j += 8;
				}

				for (; j <= (w - 1); j++) {
					mlib_s32 val1 =
						(a_array[j]) | (b[j] << 8);
					mlib_s32 val2 =
						(g[j] << 16) | (r[j] << 24);
					mlib_s32 val = val1 | val2;
					abgr[j] = val;
				}
			}
		} else {
			for (i = 0; i < h; i++,	a_array += slb, b += slb,
				g += slb, r += slb, abgr += dlb) {
				pa = (mlib_u8 *)a_array;
				pb = (mlib_u8 *)b;
				pr = (mlib_u8 *)r;
				pg = (mlib_u8 *)g;
				pabgr = abgr;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j <= (w - 16); j += 16) {
					x_a = _mm_loadu_si128((__m128i *)pa);
					pa += 16;
					x_b = _mm_loadu_si128((__m128i *)pb);
					pb += 16;
					x_g = _mm_loadu_si128((__m128i *)pg);
					pg += 16;
					x_r = _mm_loadu_si128((__m128i *)pr);
					pr += 16;
					x_ab_l = _mm_unpacklo_epi8(x_a, x_b);
					x_ab_h = _mm_unpackhi_epi8(x_a, x_b);
					x_gr_l = _mm_unpacklo_epi8(x_g, x_r);
					x_gr_h = _mm_unpackhi_epi8(x_g, x_r);

					x_abgr = _mm_unpacklo_epi16(
						x_ab_l, x_gr_l);
					_mm_storeu_si128(
						(__m128i *)pabgr, x_abgr);
					pabgr += 4;
					x_abgr = _mm_unpackhi_epi16(
						x_ab_l, x_gr_l);
					_mm_storeu_si128(
						(__m128i *)pabgr, x_abgr);
					pabgr += 4;
					x_abgr = _mm_unpacklo_epi16(
						x_ab_h, x_gr_h);
					_mm_storeu_si128(
						(__m128i *)pabgr, x_abgr);
					pabgr += 4;
					x_abgr = _mm_unpackhi_epi16(
						x_ab_h, x_gr_h);
					_mm_storeu_si128(
						(__m128i *)pabgr, x_abgr);
					pabgr += 4;
				}

				if (j <= (w - 8)) {
					x_a = _mm_loadl_epi64((__m128i *)pa);
					pa += 8;
					x_b = _mm_loadl_epi64((__m128i *)pb);
					pb += 8;
					x_g = _mm_loadl_epi64((__m128i *)pg);
					pg += 8;
					x_r = _mm_loadl_epi64((__m128i *)pr);
					pr += 8;
					x_ab_l = _mm_unpacklo_epi8(x_a, x_b);
					x_gr_l = _mm_unpacklo_epi8(x_g, x_r);

					x_abgr = _mm_unpacklo_epi16(
						x_ab_l, x_gr_l);
					_mm_storeu_si128(
						(__m128i *)pabgr, x_abgr);
					pabgr += 4;
					x_abgr = _mm_unpackhi_epi16(
						x_ab_l, x_gr_l);
					_mm_storeu_si128(
						(__m128i *)pabgr, x_abgr);
					pabgr += 4;

					j += 8;
				}

				for (; j <= (w - 1); j++) {
					mlib_s32 val1 =
						(a_array[j]) | (b[j] << 8);
					mlib_s32 val2 =
						(g[j] << 16) | (r[j] << 24);
					mlib_s32 val = val1 | val2;
					abgr[j] = val;
				}
			}
		}
	} else {
		x_a = _mm_set1_epi8((char)a_const);
		if ((0 == (0xf & ((mlib_addr)b | (mlib_addr)g | (mlib_addr)r |
			(mlib_addr)abgr))) && (0 == (0xf & slb)) &&
			(0 == (0x3 & dlb))) {
			for (i = 0; i < h; i++,
				b += slb, g += slb, r += slb, abgr += dlb) {
				pb = (mlib_u8 *)b;
				pr = (mlib_u8 *)r;
				pg = (mlib_u8 *)g;
				pabgr = abgr;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j <= (w - 16); j += 16) {
					x_b = _mm_load_si128((__m128i *)pb);
					pb += 16;
					x_g = _mm_load_si128((__m128i *)pg);
					pg += 16;
					x_r = _mm_load_si128((__m128i *)pr);
					pr += 16;
					x_ab_l = _mm_unpacklo_epi8(x_a, x_b);
					x_ab_h = _mm_unpackhi_epi8(x_a, x_b);
					x_gr_l = _mm_unpacklo_epi8(x_g, x_r);
					x_gr_h = _mm_unpackhi_epi8(x_g, x_r);

					x_abgr = _mm_unpacklo_epi16(
						x_ab_l, x_gr_l);
					_mm_store_si128(
						(__m128i *)pabgr, x_abgr);
					pabgr += 4;
					x_abgr = _mm_unpackhi_epi16(
						x_ab_l, x_gr_l);
					_mm_store_si128(
						(__m128i *)pabgr, x_abgr);
					pabgr += 4;
					x_abgr = _mm_unpacklo_epi16(
						x_ab_h, x_gr_h);
					_mm_store_si128(
						(__m128i *)pabgr, x_abgr);
					pabgr += 4;
					x_abgr = _mm_unpackhi_epi16(
						x_ab_h, x_gr_h);
					_mm_store_si128(
						(__m128i *)pabgr, x_abgr);
					pabgr += 4;
				}

				if (j <= (w - 8)) {
					x_b = _mm_loadl_epi64((__m128i *)pb);
					pb += 8;
					x_g = _mm_loadl_epi64((__m128i *)pg);
					pg += 8;
					x_r = _mm_loadl_epi64((__m128i *)pr);
					pr += 8;
					x_ab_l = _mm_unpacklo_epi8(x_a, x_b);
					x_gr_l = _mm_unpacklo_epi8(x_g, x_r);

					x_abgr = _mm_unpacklo_epi16(
						x_ab_l, x_gr_l);
					_mm_store_si128(
						(__m128i *)pabgr, x_abgr);
					pabgr += 4;
					x_abgr = _mm_unpackhi_epi16(
						x_ab_l, x_gr_l);
					_mm_store_si128(
						(__m128i *)pabgr, x_abgr);
					pabgr += 4;

					j += 8;
				}

				for (; j <= (w - 1); j++) {
					mlib_s32 val1 =
						(a_const) | (b[j] << 8);
					mlib_s32 val2 =
						(g[j] << 16) | (r[j] << 24);
					mlib_s32 val = val1 | val2;
					abgr[j] = val;
				}
			}
		} else {
			for (i = 0; i < h; i++,
				b += slb, g += slb, r += slb, abgr += dlb) {
				pb = (mlib_u8 *)b;
				pr = (mlib_u8 *)r;
				pg = (mlib_u8 *)g;
				pabgr = abgr;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j <= (w - 16); j += 16) {
					x_b = _mm_loadu_si128((__m128i *)pb);
					pb += 16;
					x_g = _mm_loadu_si128((__m128i *)pg);
					pg += 16;
					x_r = _mm_loadu_si128((__m128i *)pr);
					pr += 16;
					x_ab_l = _mm_unpacklo_epi8(x_a, x_b);
					x_ab_h = _mm_unpackhi_epi8(x_a, x_b);
					x_gr_l = _mm_unpacklo_epi8(x_g, x_r);
					x_gr_h = _mm_unpackhi_epi8(x_g, x_r);

					x_abgr = _mm_unpacklo_epi16(
						x_ab_l, x_gr_l);
					_mm_storeu_si128(
						(__m128i *)pabgr, x_abgr);
					pabgr += 4;
					x_abgr = _mm_unpackhi_epi16(
						x_ab_l, x_gr_l);
					_mm_storeu_si128(
						(__m128i *)pabgr, x_abgr);
					pabgr += 4;
					x_abgr = _mm_unpacklo_epi16(
						x_ab_h, x_gr_h);
					_mm_storeu_si128(
						(__m128i *)pabgr, x_abgr);
					pabgr += 4;
					x_abgr = _mm_unpackhi_epi16(
						x_ab_h, x_gr_h);
					_mm_storeu_si128(
						(__m128i *)pabgr, x_abgr);
					pabgr += 4;
				}

				if (j <= (w - 8)) {
					x_b = _mm_loadl_epi64((__m128i *)pb);
					pb += 8;
					x_g = _mm_loadl_epi64((__m128i *)pg);
					pg += 8;
					x_r = _mm_loadl_epi64((__m128i *)pr);
					pr += 8;
					x_ab_l = _mm_unpacklo_epi8(x_a, x_b);
					x_gr_l = _mm_unpacklo_epi8(x_g, x_r);

					x_abgr = _mm_unpacklo_epi16(
						x_ab_l, x_gr_l);
					_mm_storeu_si128(
						(__m128i *)pabgr, x_abgr);
					pabgr += 4;
					x_abgr = _mm_unpackhi_epi16(
						x_ab_l, x_gr_l);
					_mm_storeu_si128(
						(__m128i *)pabgr, x_abgr);
					pabgr += 4;

					j += 8;
				}

				for (; j <= (w - 1); j++) {
					mlib_s32 val1 =
						(a_const) | (b[j] << 8);
					mlib_s32 val2 =
						(g[j] << 16) | (r[j] << 24);
					mlib_s32 val = val1 | val2;
					abgr[j] = val;
				}
			}
		}
	}
}

/* *********************************************************** */
/* rgb: 48 bytes */
#define	SeparateRGB48(x_rgb0, x_rgb1, x_rgb2, x_r, x_g,  x_b)	\
{	\
	x_t1 = _mm_unpacklo_epi8(x_rgb0, _mm_srli_si128(x_rgb1, 8));	\
	x_t2 = _mm_unpacklo_epi8(_mm_srli_si128(x_rgb0, 8), x_rgb2);	\
	x_t3 = _mm_unpacklo_epi8(x_rgb1, _mm_srli_si128(x_rgb2, 8));	\
	x_t4 = _mm_unpacklo_epi8(x_t1, _mm_srli_si128(x_t2, 8));	\
	x_t5 = _mm_unpacklo_epi8(_mm_srli_si128(x_t1, 8), x_t3);	\
	\
	x_t6 = _mm_unpacklo_epi8(x_t4, _mm_srli_si128(x_t5, 8));	\
	x_t7 = _mm_unpacklo_epi8(x_t2, _mm_srli_si128(x_t3, 8));	\
	x_t8 = _mm_unpacklo_epi8(_mm_srli_si128(x_t4, 8), x_t7);	\
	x_t9 = _mm_unpacklo_epi8(x_t5, _mm_srli_si128(x_t7, 8));	\
	\
	x_r = _mm_unpacklo_epi8(x_t6, _mm_srli_si128(x_t8, 8));	\
	x_g = _mm_unpacklo_epi8(_mm_srli_si128(x_t6, 8), x_t9);	\
	x_b = _mm_unpacklo_epi8(x_t8, _mm_srli_si128(x_t9, 8));	\
}

#define	SeparateRGBX64(x_rgb0, x_rgb1, x_rgb2, x_rgb3, x_r, x_g, x_b) \
	x_t1 = _mm_unpacklo_epi8(x_rgb0, x_rgb1);		\
	x_t2 = _mm_unpackhi_epi8(x_rgb0, x_rgb1);		\
	x_t3 = _mm_unpacklo_epi8(x_rgb2, x_rgb3);		\
	x_t4 = _mm_unpackhi_epi8(x_rgb2, x_rgb3);		\
	x_t5 = _mm_unpacklo_epi8(x_t1, x_t2);			\
	x_t6 = _mm_unpackhi_epi8(x_t1, x_t2);			\
	x_t7 = _mm_unpacklo_epi8(x_t3, x_t4);			\
	x_t8 = _mm_unpackhi_epi8(x_t3, x_t4);			\
	x_t1 = _mm_unpacklo_epi8(x_t5, x_t6);			\
	x_t2 = _mm_unpackhi_epi8(x_t5, x_t6);			\
	x_t3 = _mm_unpacklo_epi8(x_t7, x_t8);			\
	x_t4 = _mm_unpackhi_epi8(x_t7, x_t8);			\
	x_r = _mm_unpacklo_epi64(x_t1, x_t3);			\
	x_g = _mm_unpackhi_epi64(x_t1, x_t3);			\
	x_b = _mm_unpacklo_epi64(x_t2, x_t4);

#define	SeparateXRGB64(x_rgb0, x_rgb1, x_rgb2, x_rgb3, x_r, x_g, x_b) \
	x_t1 = _mm_unpacklo_epi8(x_rgb0, x_rgb1);		\
	x_t2 = _mm_unpackhi_epi8(x_rgb0, x_rgb1);		\
	x_t3 = _mm_unpacklo_epi8(x_rgb2, x_rgb3);		\
	x_t4 = _mm_unpackhi_epi8(x_rgb2, x_rgb3);		\
	x_t5 = _mm_unpacklo_epi8(x_t1, x_t2);			\
	x_t6 = _mm_unpackhi_epi8(x_t1, x_t2);			\
	x_t7 = _mm_unpacklo_epi8(x_t3, x_t4);			\
	x_t8 = _mm_unpackhi_epi8(x_t3, x_t4);			\
	x_t1 = _mm_unpacklo_epi8(x_t5, x_t6);			\
	x_t2 = _mm_unpackhi_epi8(x_t5, x_t6);			\
	x_t3 = _mm_unpacklo_epi8(x_t7, x_t8);			\
	x_t4 = _mm_unpackhi_epi8(x_t7, x_t8);			\
	x_r = _mm_unpackhi_epi64(x_t1, x_t3);			\
	x_g = _mm_unpacklo_epi64(x_t2, x_t4);			\
	x_b = _mm_unpackhi_epi64(x_t2, x_t4);

/* rgb: 24 bytes */
#define	SeparateRGB24(x_rgb0, x_rgb1, x_r, x_g,  x_b)	\
{	\
	x_rgb2 = x_zero;	\
	\
	x_t1 = _mm_unpacklo_epi8(x_rgb0, _mm_srli_si128(x_rgb1, 8));	\
	x_t2 = _mm_unpacklo_epi8(_mm_srli_si128(x_rgb0, 8), x_rgb2);	\
	x_t3 = _mm_unpacklo_epi8(x_rgb1, _mm_srli_si128(x_rgb2, 8));	\
	x_t4 = _mm_unpacklo_epi8(x_t1, _mm_srli_si128(x_t2, 8));	\
	x_t5 = _mm_unpacklo_epi8(_mm_srli_si128(x_t1, 8), x_t3);	\
	\
	x_t6 = _mm_unpacklo_epi8(x_t4, _mm_srli_si128(x_t5, 8));	\
	x_t7 = _mm_unpacklo_epi8(x_t2, _mm_srli_si128(x_t3, 8));	\
	x_t8 = _mm_unpacklo_epi8(_mm_srli_si128(x_t4, 8), x_t7);	\
	x_t9 = _mm_unpacklo_epi8(x_t5, _mm_srli_si128(x_t7, 8));	\
	\
	x_r = _mm_unpacklo_epi8(x_t6, _mm_srli_si128(x_t8, 8));	\
	x_g = _mm_unpacklo_epi8(_mm_srli_si128(x_t6, 8), x_t9);	\
	x_b = _mm_unpacklo_epi8(x_t8, _mm_srli_si128(x_t9, 8));	\
}

#define	RGB2ABGR_48(x_rgb0, x_rgb1, x_rgb2,	\
	x_abgr0, x_abgr1, x_abgr2, x_abgr3) 	\
{	\
	SeparateRGB48(x_rgb0, x_rgb1, x_rgb2, x_r, x_g, x_b);	\
	x_t1 = _mm_unpacklo_epi8(x_a, x_b);	\
	x_t2 = _mm_unpackhi_epi8(x_a, x_b);	\
	x_t3 = _mm_unpacklo_epi8(x_g, x_r);	\
	x_t4 = _mm_unpackhi_epi8(x_g, x_r);	\
	x_abgr0 = _mm_unpacklo_epi16(x_t1, x_t3);	\
	x_abgr1 = _mm_unpackhi_epi16(x_t1, x_t3);	\
	x_abgr2 = _mm_unpacklo_epi16(x_t2, x_t4);	\
	x_abgr3 = _mm_unpackhi_epi16(x_t2, x_t4);	\
}

#define	RGB2ABGR_24(x_rgb0, x_rgb1, x_abgr0, x_abgr1) \
{	\
	SeparateRGB24(x_rgb0, x_rgb1, x_r, x_g, x_b);	\
	x_t1 = _mm_unpacklo_epi8(x_a, x_b);	\
	x_t3 = _mm_unpacklo_epi8(x_g, x_r);	\
	x_abgr0 = _mm_unpacklo_epi16(x_t1, x_t3);	\
	x_abgr1 = _mm_unpackhi_epi16(x_t1, x_t3);	\
}

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
	const __m128i x_zero = _mm_setzero_si128();
	__m128i x_abgr, x_r, x_g, x_b, x_a, x_rgb0, x_rgb1, x_rgb2;
	__m128i x_abgr0, x_abgr1, x_abgr2, x_abgr3;
	__m128i x_t1, x_t2, x_t3, x_t4, x_t5, x_t6, x_t7, x_t8, x_t9;
	mlib_u8	*pa, *prgb;
	mlib_u32 *pabgr;
	mlib_s32 i, j;

	dlb >>= 2;

	if (h == 0 || w == 0)
		return;

	if (a_array) {
		if ((0 == (0xf & ((mlib_addr)rgb | (mlib_addr)a_array |
			(mlib_addr)abgr))) && (0 == (0xf & (slb | alb))) &&
			(0 == (0x3 & dlb))) {
		    for (i = 0; i < h; i++, rgb += slb, a_array += alb,
				abgr += dlb) {
			pa = (mlib_u8 *)a_array;
			prgb = (mlib_u8 *)rgb;
			pabgr = abgr;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j <= (w - 16); j += 16) {
				x_rgb0 = _mm_load_si128((__m128i *)prgb);
				prgb += 16;
				x_rgb1 = _mm_load_si128((__m128i *)prgb);
				prgb += 16;
				x_rgb2 = _mm_load_si128((__m128i *)prgb);
				prgb += 16;
				x_a = _mm_load_si128((__m128i *)pa);
				pa += 16;

				RGB2ABGR_48(x_rgb0, x_rgb1, x_rgb2, x_abgr0,
					x_abgr1, x_abgr2, x_abgr3);

				_mm_store_si128((__m128i *)pabgr, x_abgr0);
				pabgr += 4;
				_mm_store_si128((__m128i *)pabgr, x_abgr1);
				pabgr += 4;
				_mm_store_si128((__m128i *)pabgr, x_abgr2);
				pabgr += 4;
				_mm_store_si128((__m128i *)pabgr, x_abgr3);
				pabgr += 4;
			}

			if (j <= (w - 8)) {
				x_rgb0 = _mm_load_si128((__m128i *)prgb);
				prgb += 16;
				x_rgb1 = _mm_loadl_epi64((__m128i *)prgb);
				prgb += 8;
				x_a = _mm_loadl_epi64((__m128i *)pa);
				pa += 8;

				RGB2ABGR_24(x_rgb0, x_rgb1, x_abgr0, x_abgr1);

				_mm_store_si128((__m128i *)pabgr, x_abgr0);
				pabgr += 4;
				_mm_store_si128((__m128i *)pabgr, x_abgr1);
				pabgr += 4;

				j += 8;
			}

			for (; j <= (w - 1); j++) {
				mlib_s32 si = 3 * j;
				mlib_s32 red, green, blue, a_val;

				a_val = a_array[j];
				blue = rgb[si + 2] << 8;
				green = rgb[si + 1] << 16;
				red = rgb[si] << 24;
				abgr[j] = a_val | blue | green | red;
			}
		    }
		} else {
		    for (i = 0; i < h; i++, rgb += slb, a_array += alb,
				abgr += dlb) {
			pa = (mlib_u8 *)a_array;
			prgb = (mlib_u8 *)rgb;
			pabgr = abgr;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j <= (w - 16); j += 16) {
				x_rgb0 = _mm_loadu_si128((__m128i *)prgb);
				prgb += 16;
				x_rgb1 = _mm_loadu_si128((__m128i *)prgb);
				prgb += 16;
				x_rgb2 = _mm_loadu_si128((__m128i *)prgb);
				prgb += 16;
				x_a = _mm_loadu_si128((__m128i *)pa);
				pa += 16;

				RGB2ABGR_48(x_rgb0, x_rgb1, x_rgb2, x_abgr0,
					x_abgr1, x_abgr2, x_abgr3);

				_mm_storeu_si128((__m128i *)pabgr, x_abgr0);
				pabgr += 4;
				_mm_storeu_si128((__m128i *)pabgr, x_abgr1);
				pabgr += 4;
				_mm_storeu_si128((__m128i *)pabgr, x_abgr2);
				pabgr += 4;
				_mm_storeu_si128((__m128i *)pabgr, x_abgr3);
				pabgr += 4;
			}

			if (j <= (w - 8)) {
				x_rgb0 = _mm_loadu_si128((__m128i *)prgb);
				prgb += 16;
				x_rgb1 = _mm_loadl_epi64((__m128i *)prgb);
				prgb += 8;
				x_a = _mm_loadl_epi64((__m128i *)pa);
				pa += 8;

				RGB2ABGR_24(x_rgb0, x_rgb1, x_abgr0, x_abgr1);

				_mm_storeu_si128((__m128i *)pabgr, x_abgr0);
				pabgr += 4;
				_mm_storeu_si128((__m128i *)pabgr, x_abgr1);
				pabgr += 4;

				j += 8;
			}

			for (; j <= (w - 1); j++) {
				mlib_s32 si = 3 * j;
				mlib_s32 red, green, blue, a_val;

				a_val = a_array[j];
				blue = rgb[si + 2] << 8;
				green = rgb[si + 1] << 16;
				red = rgb[si] << 24;
				abgr[j] = a_val | blue | green | red;
			}
		    }
		}
	} else {
		x_a = _mm_set1_epi8((char)a_const);

		if ((0 == (0xf & ((mlib_addr)rgb | (mlib_addr)abgr))) &&
			(0 == (0xf & slb)) && (0 == (0x3 & dlb))) {
		    for (i = 0; i < h; i++, rgb += slb, abgr += dlb) {
			prgb = (mlib_u8 *)rgb;
			pabgr = abgr;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j <= (w - 16); j += 16) {
				x_rgb0 = _mm_load_si128((__m128i *)prgb);
				prgb += 16;
				x_rgb1 = _mm_load_si128((__m128i *)prgb);
				prgb += 16;
				x_rgb2 = _mm_load_si128((__m128i *)prgb);
				prgb += 16;

				RGB2ABGR_48(x_rgb0, x_rgb1, x_rgb2, x_abgr0,
					x_abgr1, x_abgr2, x_abgr3);

				_mm_store_si128((__m128i *)pabgr, x_abgr0);
				pabgr += 4;
				_mm_store_si128((__m128i *)pabgr, x_abgr1);
				pabgr += 4;
				_mm_store_si128((__m128i *)pabgr, x_abgr2);
				pabgr += 4;
				_mm_store_si128((__m128i *)pabgr, x_abgr3);
				pabgr += 4;
			}

			if (j <= (w - 8)) {
				x_rgb0 = _mm_load_si128((__m128i *)prgb);
				prgb += 16;
				x_rgb1 = _mm_loadl_epi64((__m128i *)prgb);
				prgb += 8;

				RGB2ABGR_24(x_rgb0, x_rgb1, x_abgr0, x_abgr1);

				_mm_store_si128((__m128i *)pabgr, x_abgr0);
				pabgr += 4;
				_mm_store_si128((__m128i *)pabgr, x_abgr1);
				pabgr += 4;

				j += 8;
			}

			for (; j <= (w - 1); j++) {
				mlib_s32 si = 3 * j;
				mlib_s32 red, green, blue, a_val;

				a_val = a_const;
				blue = rgb[si + 2] << 8;
				green = rgb[si + 1] << 16;
				red = rgb[si] << 24;
				abgr[j] = a_val | blue | green | red;
			}
		    }
		} else {
		    for (i = 0; i < h; i++, rgb += slb, abgr += dlb) {
			prgb = (mlib_u8 *)rgb;
			pabgr = abgr;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j <= (w - 16); j += 16) {
				x_rgb0 = _mm_loadu_si128((__m128i *)prgb);
				prgb += 16;
				x_rgb1 = _mm_loadu_si128((__m128i *)prgb);
				prgb += 16;
				x_rgb2 = _mm_loadu_si128((__m128i *)prgb);
				prgb += 16;

				RGB2ABGR_48(x_rgb0, x_rgb1, x_rgb2, x_abgr0,
					x_abgr1, x_abgr2, x_abgr3);

				_mm_storeu_si128((__m128i *)pabgr, x_abgr0);
				pabgr += 4;
				_mm_storeu_si128((__m128i *)pabgr, x_abgr1);
				pabgr += 4;
				_mm_storeu_si128((__m128i *)pabgr, x_abgr2);
				pabgr += 4;
				_mm_storeu_si128((__m128i *)pabgr, x_abgr3);
				pabgr += 4;
			}

			if (j <= (w - 8)) {
				x_rgb0 = _mm_loadu_si128((__m128i *)prgb);
				prgb += 16;
				x_rgb1 = _mm_loadl_epi64((__m128i *)prgb);
				prgb += 8;

				RGB2ABGR_24(x_rgb0, x_rgb1, x_abgr0, x_abgr1);

				_mm_storeu_si128((__m128i *)pabgr, x_abgr0);
				pabgr += 4;
				_mm_storeu_si128((__m128i *)pabgr, x_abgr1);
				pabgr += 4;

				j += 8;
			}

			for (; j <= (w - 1); j++) {
				mlib_s32 si = 3 * j;
				mlib_s32 red, green, blue, a_val;

				a_val = a_const;
				blue = rgb[si + 2] << 8;
				green = rgb[si + 1] << 16;
				red = rgb[si] << 24;
				abgr[j] = a_val | blue | green | red;
			}
		    }
		}
	}

}

#define	RGB2BGRA_48(x_rgb0, x_rgb1, x_rgb2,	\
	x_abgr0, x_abgr1, x_abgr2, x_abgr3) 	\
{	\
	SeparateRGB48(x_rgb0, x_rgb1, x_rgb2, x_r, x_g, x_b);	\
	x_t1 = _mm_unpacklo_epi8(x_b, x_g);	\
	x_t2 = _mm_unpackhi_epi8(x_b, x_g);	\
	x_t3 = _mm_unpacklo_epi8(x_r, x_a);	\
	x_t4 = _mm_unpackhi_epi8(x_r, x_a);	\
	x_abgr0 = _mm_unpacklo_epi16(x_t1, x_t3);	\
	x_abgr1 = _mm_unpackhi_epi16(x_t1, x_t3);	\
	x_abgr2 = _mm_unpacklo_epi16(x_t2, x_t4);	\
	x_abgr3 = _mm_unpackhi_epi16(x_t2, x_t4);	\
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
	const __m128i x_zero = _mm_setzero_si128();
	__m128i x_abgr, x_r, x_g, x_b, x_a, x_rgb0, x_rgb1, x_rgb2;
	__m128i x_abgr0, x_abgr1, x_abgr2, x_abgr3;
	__m128i x_t1, x_t2, x_t3, x_t4, x_t5, x_t6, x_t7, x_t8, x_t9;
	mlib_u8	*pa, *prgb;
	mlib_u32 *pbgra;
	mlib_s32 i, j, red, green, blue;
	mlib_u32 a_val;

	if (h == 0 || w == 0)
		return;

	if (a_array) {
		if ((0 == (0xf & ((mlib_addr)rgb | (mlib_addr)bgra))) &&
			(0 == (0xf & (slb | alb))) && (0 == (0x3 & dlb))) {
		mlib_u32 *bgra32 = (mlib_u32 *)bgra;
		dlb >>= 2;

			for (i = 0; i < h; i++, rgb += slb, a_array += alb,
			    bgra32 += dlb) {

			pa = (mlib_u8 *)a_array;
			prgb = (mlib_u8 *)rgb;
			pbgra = (mlib_u32 *)bgra32;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j <= (w - 16); j += 16) {
				x_rgb0 = _mm_load_si128((__m128i *)prgb);
				prgb += 16;
				x_rgb1 = _mm_load_si128((__m128i *)prgb);
				prgb += 16;
				x_rgb2 = _mm_load_si128((__m128i *)prgb);
				prgb += 16;
				x_a = _mm_load_si128((__m128i *)pa);
				pa += 16;

				RGB2BGRA_48(x_rgb0, x_rgb1, x_rgb2,
				x_abgr0, x_abgr1, x_abgr2, x_abgr3);

				_mm_store_si128((__m128i *)pbgra, x_abgr0);
				pbgra += 4;
				_mm_store_si128((__m128i *)pbgra, x_abgr1);
				pbgra += 4;
				_mm_store_si128((__m128i *)pbgra, x_abgr2);
				pbgra += 4;
				_mm_store_si128((__m128i *)pbgra, x_abgr3);
				pbgra += 4;
				}

				for (; j <= (w - 1); j++) {
					mlib_s32 si = 3 * j;
					blue = rgb[si + 2];
					green = rgb[si + 1] << 8;
					red = rgb[si] << 16;
					a_val = a_array[j] << 24;

					bgra32[j] = a_val | blue | green | red;
			}
			}
		} else {
		mlib_u32 *bgra32 = (mlib_u32 *)bgra;

			for (i = 0; i < h; i++, rgb += slb, a_array += alb,
			    bgra += dlb) {

			pa = (mlib_u8 *)a_array;
			prgb = (mlib_u8 *)rgb;
			pbgra = (mlib_u32 *)bgra;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j <= (w - 16); j += 16) {
				x_rgb0 = _mm_loadu_si128((__m128i *)prgb);
				prgb += 16;
				x_rgb1 = _mm_loadu_si128((__m128i *)prgb);
				prgb += 16;
				x_rgb2 = _mm_loadu_si128((__m128i *)prgb);
				prgb += 16;
				x_a = _mm_loadu_si128((__m128i *)pa);
				pa += 16;

				RGB2BGRA_48(x_rgb0, x_rgb1, x_rgb2, x_abgr0,
					x_abgr1, x_abgr2, x_abgr3);

				_mm_storeu_si128((__m128i *)pbgra, x_abgr0);
				pbgra += 4;
				_mm_storeu_si128((__m128i *)pbgra, x_abgr1);
				pbgra += 4;
				_mm_storeu_si128((__m128i *)pbgra, x_abgr2);
				pbgra += 4;
				_mm_storeu_si128((__m128i *)pbgra, x_abgr3);
				pbgra += 4;
				}

				for (; j <= (w - 1); j++) {
					mlib_s32 si = 3 * j;
					mlib_s32 di = 4 * j;
					bgra[di] = rgb[si + 2];
					bgra[di + 1] = rgb[si + 1];
					bgra[di + 2] = rgb[si];
					bgra[di + 3] = a_array[j];
			}
			}
		}
	} else {
			a_val = a_const;
			x_a = _mm_set1_epi8((char)a_const);

			mlib_u32 *bgra32;
		if ((0 == (0xf & ((mlib_addr)rgb | (mlib_addr)bgra))) &&
			(0 == (0xf & slb)) && (0 == (0x3 & dlb))) {
			for (i = 0; i < h; i++, rgb += slb, bgra += dlb) {
			prgb = (mlib_u8 *)rgb;
			pbgra = (mlib_u32 *)bgra;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j <= (w - 16); j += 16) {
				x_rgb0 = _mm_load_si128((__m128i *)prgb);
				prgb += 16;
				x_rgb1 = _mm_load_si128((__m128i *)prgb);
				prgb += 16;
				x_rgb2 = _mm_load_si128((__m128i *)prgb);
				prgb += 16;

				RGB2BGRA_48(x_rgb0, x_rgb1, x_rgb2,
				x_abgr0, x_abgr1, x_abgr2, x_abgr3);

				_mm_store_si128((__m128i *)pbgra, x_abgr0);
				pbgra += 4;
				_mm_store_si128((__m128i *)pbgra, x_abgr1);
				pbgra += 4;
				_mm_store_si128((__m128i *)pbgra, x_abgr2);
				pbgra += 4;
				_mm_store_si128((__m128i *)pbgra, x_abgr3);
				pbgra += 4;
				}

				for (; j <= (w - 1); j++) {
					mlib_s32 si = 3 * j;
					mlib_s32 di = 4 * j;
					bgra[di] = rgb[si + 2];
					bgra[di + 1] = rgb[si + 1];
					bgra[di + 2] = rgb[si];
					bgra[di + 3] = a_val;
			}
			}

	} else {
		for (i = 0; i < h; i++, rgb += slb, bgra += dlb) {
			prgb = (mlib_u8 *)rgb;
			pbgra = (mlib_u32 *)bgra;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (j = 0; j <= (w - 16); j += 16) {
				x_rgb0 = _mm_loadu_si128((__m128i *)prgb);
				prgb += 16;
				x_rgb1 = _mm_loadu_si128((__m128i *)prgb);
				prgb += 16;
				x_rgb2 = _mm_loadu_si128((__m128i *)prgb);
				prgb += 16;

				RGB2BGRA_48(x_rgb0, x_rgb1, x_rgb2,
			x_abgr0, x_abgr1, x_abgr2, x_abgr3);

				_mm_storeu_si128((__m128i *)pbgra, x_abgr0);
				pbgra += 4;
				_mm_storeu_si128((__m128i *)pbgra, x_abgr1);
				pbgra += 4;
				_mm_storeu_si128((__m128i *)pbgra, x_abgr2);
				pbgra += 4;
				_mm_storeu_si128((__m128i *)pbgra, x_abgr3);
				pbgra += 4;
				}

		for (; j <= (w - 1); j++) {
					mlib_s32 si = 3 * j;
					mlib_s32 di = 4 * j;
					bgra[di] = rgb[si + 2];
					bgra[di + 1] = rgb[si + 1];
					bgra[di + 2] = rgb[si];
					bgra[di + 3] = a_val;
			}
			}

	}
}
}

#define	BGR2ABGR_48(x_rgb0, x_rgb1, x_rgb2,	\
	x_abgr0, x_abgr1, x_abgr2, x_abgr3) 	\
{	\
	SeparateRGB48(x_rgb0, x_rgb1, x_rgb2, x_b, x_g, x_r);	\
	x_t1 = _mm_unpacklo_epi8(x_a, x_b);	\
	x_t2 = _mm_unpackhi_epi8(x_a, x_b);	\
	x_t3 = _mm_unpacklo_epi8(x_g, x_r);	\
	x_t4 = _mm_unpackhi_epi8(x_g, x_r);	\
	x_abgr0 = _mm_unpacklo_epi16(x_t1, x_t3);	\
	x_abgr1 = _mm_unpackhi_epi16(x_t1, x_t3);	\
	x_abgr2 = _mm_unpacklo_epi16(x_t2, x_t4);	\
	x_abgr3 = _mm_unpackhi_epi16(x_t2, x_t4);	\
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
	const __m128i x_zero = _mm_setzero_si128();
	__m128i x_abgr, x_r, x_g, x_b, x_a, x_rgb0, x_rgb1, x_rgb2;
	__m128i x_abgr0, x_abgr1, x_abgr2, x_abgr3;
	__m128i x_t1, x_t2, x_t3, x_t4, x_t5, x_t6, x_t7, x_t8, x_t9;
	mlib_u8	*pa, *prgb;
	mlib_u32 *pabgr;
	mlib_s32 i, j, red, green, blue;

	mlib_u32 a_val = a_const;

	dlb >>= 2;

	if (h == 0 || w == 0)
		return;

	if (a_array) {
		for (i = 0; i < h; i++, bgr += slb, a_array += alb,
		    abgr += dlb) {

			pa = (mlib_u8 *)a_array;
			prgb = (mlib_u8 *)bgr;
			pabgr = (mlib_u32 *)abgr;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j <= (w - 16); j += 16) {
				x_rgb0 = _mm_loadu_si128((__m128i *)prgb);
				prgb += 16;
				x_rgb1 = _mm_loadu_si128((__m128i *)prgb);
				prgb += 16;
				x_rgb2 = _mm_loadu_si128((__m128i *)prgb);
				prgb += 16;
				x_a = _mm_loadu_si128((__m128i *)pa);
				pa += 16;

				BGR2ABGR_48(x_rgb0, x_rgb1, x_rgb2, x_abgr0,
					x_abgr1, x_abgr2, x_abgr3);

				_mm_storeu_si128((__m128i *)pabgr, x_abgr0);
				pabgr += 4;
				_mm_storeu_si128((__m128i *)pabgr, x_abgr1);
				pabgr += 4;
				_mm_storeu_si128((__m128i *)pabgr, x_abgr2);
				pabgr += 4;
				_mm_storeu_si128((__m128i *)pabgr, x_abgr3);
				pabgr += 4;
				}

				for (; j <= (w - 1); j++) {
				mlib_s32 si = 3 * j;
				a_val = a_array[j];
				blue = bgr[si] << 8;
				green = bgr[si + 1] << 16;
				red = bgr[si + 2] << 24;
				abgr[j] = a_val | blue | green | red;
			}
		}
	} else {
			a_val = a_const;
			x_a = _mm_set1_epi8((char)a_const);

		for (i = 0; i < h; i++, bgr += slb, abgr += dlb) {
			pa = (mlib_u8 *)a_array;
			prgb = (mlib_u8 *)bgr;
			pabgr = (mlib_u32 *)abgr;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j <= (w - 16); j += 16) {
				x_rgb0 = _mm_loadu_si128((__m128i *)prgb);
				prgb += 16;
				x_rgb1 = _mm_loadu_si128((__m128i *)prgb);
				prgb += 16;
				x_rgb2 = _mm_loadu_si128((__m128i *)prgb);
				prgb += 16;

				BGR2ABGR_48(x_rgb0, x_rgb1, x_rgb2, x_abgr0,
					x_abgr1, x_abgr2, x_abgr3);

				_mm_storeu_si128((__m128i *)pabgr, x_abgr0);
				pabgr += 4;
				_mm_storeu_si128((__m128i *)pabgr, x_abgr1);
				pabgr += 4;
				_mm_storeu_si128((__m128i *)pabgr, x_abgr2);
				pabgr += 4;
				_mm_storeu_si128((__m128i *)pabgr, x_abgr3);
				pabgr += 4;
				}

				for (; j <= (w - 1); j++) {
				mlib_s32 si = 3 * j;
				blue = bgr[si] << 8;
				green = bgr[si + 1] << 16;
				red = bgr[si + 2] << 24;
				abgr[j] = a_val | blue | green | red;
			}
		}
	}
}

#define	RGBX2ABGR_48(x_rgb0, x_rgb1, x_rgb2, x_rgb3,\
	x_abgr0, x_abgr1, x_abgr2, x_abgr3) 	\
{	\
	SeparateRGBX64(x_rgb0, x_rgb1, x_rgb2, x_rgb3,\
		x_r, x_g, x_b);			\
	x_t1 = _mm_unpacklo_epi8(x_a, x_b);	\
	x_t2 = _mm_unpackhi_epi8(x_a, x_b);	\
	x_t3 = _mm_unpacklo_epi8(x_g, x_r);	\
	x_t4 = _mm_unpackhi_epi8(x_g, x_r);	\
	x_abgr0 = _mm_unpacklo_epi16(x_t1, x_t3);	\
	x_abgr1 = _mm_unpackhi_epi16(x_t1, x_t3);	\
	x_abgr2 = _mm_unpacklo_epi16(x_t2, x_t4);	\
	x_abgr3 = _mm_unpackhi_epi16(x_t2, x_t4);	\
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
	const __m128i x_zero = _mm_setzero_si128();
	__m128i x_abgr, x_r, x_g, x_b, x_a, x_rgb0, x_rgb1, x_rgb2, x_rgb3;
	__m128i x_abgr0, x_abgr1, x_abgr2, x_abgr3;
	__m128i x_t1, x_t2, x_t3, x_t4, x_t5, x_t6, x_t7, x_t8, x_t9;
	mlib_u8	*pa, *prgb;
	mlib_u32 *pabgr;
	mlib_s32 i, j, red, green, blue;
	mlib_u8 *psrc;

	mlib_u32 a_val = a_const;

	dlb >>= 2;
	slb >>= 2;

	if (h == 0 || w == 0)
		return;

	if (a_array) {
		for (i = 0; i < h;
			i++, rgbx += slb, a_array += alb, abgr += dlb) {

			pa = (mlib_u8 *)a_array;
			prgb = (mlib_u8 *)rgbx;
			pabgr = (mlib_u32 *)abgr;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j <= (w - 16); j += 16) {
				x_rgb0 = _mm_loadu_si128((__m128i *)prgb);
				prgb += 16;
				x_rgb1 = _mm_loadu_si128((__m128i *)prgb);
				prgb += 16;
				x_rgb2 = _mm_loadu_si128((__m128i *)prgb);
				prgb += 16;
				x_rgb3 = _mm_loadu_si128((__m128i *)prgb);
				prgb += 16;

				x_a = _mm_loadu_si128((__m128i *)pa);
				pa += 16;

				RGBX2ABGR_48(x_rgb0, x_rgb1, x_rgb2,
			x_rgb3, x_abgr0, x_abgr1, x_abgr2, x_abgr3);

				_mm_storeu_si128((__m128i *)pabgr, x_abgr0);
				pabgr += 4;
				_mm_storeu_si128((__m128i *)pabgr, x_abgr1);
				pabgr += 4;
				_mm_storeu_si128((__m128i *)pabgr, x_abgr2);
				pabgr += 4;
				_mm_storeu_si128((__m128i *)pabgr, x_abgr3);
				pabgr += 4;
				}

				for (; j <= (w - 1); j++) {
				psrc = (mlib_u8 *)(rgbx + j);
				a_val = a_array[j];
				blue = psrc[2] << 8;
				green = psrc[1] << 16;
				red = psrc[0] << 24;
				abgr[j] = a_val | blue | green | red;
			}
		}
	} else {
			a_val = a_const;
			x_a = _mm_set1_epi8((char)a_const);

		for (i = 0; i < h; i++, rgbx += slb, abgr += dlb) {
			pa = (mlib_u8 *)a_array;
			prgb = (mlib_u8 *)rgbx;
			pabgr = (mlib_u32 *)abgr;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j <= (w - 16); j += 16) {
				x_rgb0 = _mm_loadu_si128((__m128i *)prgb);
				prgb += 16;
				x_rgb1 = _mm_loadu_si128((__m128i *)prgb);
				prgb += 16;
				x_rgb2 = _mm_loadu_si128((__m128i *)prgb);
				prgb += 16;
				x_rgb3 = _mm_loadu_si128((__m128i *)prgb);
				prgb += 16;

				RGBX2ABGR_48(x_rgb0, x_rgb1, x_rgb2,
			x_rgb3, x_abgr0, x_abgr1, x_abgr2, x_abgr3);

				_mm_storeu_si128((__m128i *)pabgr, x_abgr0);
				pabgr += 4;
				_mm_storeu_si128((__m128i *)pabgr, x_abgr1);
				pabgr += 4;
				_mm_storeu_si128((__m128i *)pabgr, x_abgr2);
				pabgr += 4;
				_mm_storeu_si128((__m128i *)pabgr, x_abgr3);
				pabgr += 4;
				}

				for (; j <= (w - 1); j++) {
				psrc = (mlib_u8 *)(rgbx + j);
				blue = psrc[2] << 8;
				green = psrc[1] << 16;
				red = psrc[0] << 24;
				abgr[j] = a_val | blue | green | red;
			}
		}
	}
}

#define	RGBX2ARGB_48(x_rgb0, x_rgb1, x_rgb2, x_rgb3,\
	x_abgr0, x_abgr1, x_abgr2, x_abgr3) 	\
{	\
	__m128i x_a0, x_a1, x_a2, x_a3;		\
	x_rgb0 = _mm_slli_epi32(x_rgb0, 8);	\
	x_rgb1 = _mm_slli_epi32(x_rgb1, 8);	\
	x_rgb2 = _mm_slli_epi32(x_rgb2, 8);	\
	x_rgb3 = _mm_slli_epi32(x_rgb3, 8);	\
	x_a1 = _mm_unpacklo_epi8(x_a, x_zero);	\
	x_a3 = _mm_unpackhi_epi8(x_a, x_zero);	\
	x_a0 = _mm_unpacklo_epi16(x_a1, x_zero);\
	x_a1 = _mm_unpackhi_epi16(x_a1, x_zero);\
	x_a2 = _mm_unpacklo_epi16(x_a3, x_zero);\
	x_a3 = _mm_unpackhi_epi16(x_a3, x_zero);\
	x_abgr0 = _mm_or_si128(x_rgb0, x_a0);	\
	x_abgr1 = _mm_or_si128(x_rgb1, x_a1);	\
	x_abgr2 = _mm_or_si128(x_rgb2, x_a2);	\
	x_abgr3 = _mm_or_si128(x_rgb3, x_a3);	\
}

#define	RGBX2ARGB_48_UA(x_rgb0, x_rgb1, x_rgb2, x_rgb3,\
	x_abgr0, x_abgr1, x_abgr2, x_abgr3) 	\
{	\
	__m128i x_a0, x_a1, x_a2, x_a3;		\
	x_rgb0 = _mm_slli_epi32(x_rgb0, 8);	\
	x_rgb1 = _mm_slli_epi32(x_rgb1, 8);	\
	x_rgb2 = _mm_slli_epi32(x_rgb2, 8);	\
	x_rgb3 = _mm_slli_epi32(x_rgb3, 8);	\
	x_abgr0 = _mm_or_si128(x_rgb0, x_a);	\
	x_abgr1 = _mm_or_si128(x_rgb1, x_a);	\
	x_abgr2 = _mm_or_si128(x_rgb2, x_a);	\
	x_abgr3 = _mm_or_si128(x_rgb3, x_a);	\
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
	const __m128i x_zero = _mm_setzero_si128();
	__m128i x_abgr, x_r, x_g, x_b, x_a, x_rgb0, x_rgb1, x_rgb2, x_rgb3;
	__m128i x_abgr0, x_abgr1, x_abgr2, x_abgr3;
	__m128i x_t1, x_t2, x_t3, x_t4, x_t5, x_t6, x_t7, x_t8, x_t9;
	mlib_u8	*pa, *prgb;
	mlib_u32 *pargb;
	mlib_s32 i, j;
	mlib_u32 val0;

	mlib_u32 a_val = a_const;

	dlb >>= 2;
	slb >>= 2;

	if (h == 0 || w == 0)
		return;

	if (a_array) {
		for (i = 0; i < h;
			i++, rgbx += slb, a_array += alb, argb += dlb) {

			pa = (mlib_u8 *)a_array;
			prgb = (mlib_u8 *)rgbx;
			pargb = (mlib_u32 *)argb;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j <= (w - 16); j += 16) {
				x_rgb0 = _mm_loadu_si128((__m128i *)prgb);
				prgb += 16;
				x_rgb1 = _mm_loadu_si128((__m128i *)prgb);
				prgb += 16;
				x_rgb2 = _mm_loadu_si128((__m128i *)prgb);
				prgb += 16;
				x_rgb3 = _mm_loadu_si128((__m128i *)prgb);
				prgb += 16;
				x_a = _mm_loadu_si128((__m128i *)pa);
				pa += 16;

				RGBX2ARGB_48(x_rgb0, x_rgb1, x_rgb2,
			x_rgb3, x_abgr0, x_abgr1, x_abgr2, x_abgr3);

				_mm_storeu_si128((__m128i *)pargb, x_abgr0);
				pargb += 4;
				_mm_storeu_si128((__m128i *)pargb, x_abgr1);
				pargb += 4;
				_mm_storeu_si128((__m128i *)pargb, x_abgr2);
				pargb += 4;
				_mm_storeu_si128((__m128i *)pargb, x_abgr3);
				pargb += 4;
				}

				for (; j <= (w - 1); j++) {
				val0 = rgbx[j];
				a_val = a_array[j];
				val0 = val0 << 8;

				argb[j] = a_val | val0;
			}
		}
	} else {
			a_val = a_const;
			x_a = _mm_set1_epi32(a_const);

		for (i = 0; i < h; i++, rgbx += slb, argb += dlb) {
			pa = (mlib_u8 *)a_array;
			prgb = (mlib_u8 *)rgbx;
			pargb = (mlib_u32 *)argb;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j <= (w - 16); j += 16) {
				x_rgb0 = _mm_loadu_si128((__m128i *)prgb);
				prgb += 16;
				x_rgb1 = _mm_loadu_si128((__m128i *)prgb);
				prgb += 16;
				x_rgb2 = _mm_loadu_si128((__m128i *)prgb);
				prgb += 16;
				x_rgb3 = _mm_loadu_si128((__m128i *)prgb);
				prgb += 16;

				RGBX2ARGB_48_UA(x_rgb0, x_rgb1,
			x_rgb2, x_rgb3, x_abgr0, x_abgr1, x_abgr2, x_abgr3);

				_mm_storeu_si128((__m128i *)pargb, x_abgr0);
				pargb += 4;
				_mm_storeu_si128((__m128i *)pargb, x_abgr1);
				pargb += 4;
				_mm_storeu_si128((__m128i *)pargb, x_abgr2);
				pargb += 4;
				_mm_storeu_si128((__m128i *)pargb, x_abgr3);
				pargb += 4;
				}

				for (; j <= (w - 1); j++) {
				val0 = rgbx[j];
				val0 = val0 << 8;
				argb[j] = a_val | val0;
			}
		}
	}
}

#define	XRGB2ABGR_48(x_rgb0, x_rgb1, x_rgb2, x_rgb3,\
	x_abgr0, x_abgr1, x_abgr2, x_abgr3) 	\
{	\
	SeparateXRGB64(x_rgb0, x_rgb1, x_rgb2, x_rgb3,\
		x_r, x_g, x_b);			\
	x_t1 = _mm_unpacklo_epi8(x_a, x_b);	\
	x_t2 = _mm_unpackhi_epi8(x_a, x_b);	\
	x_t3 = _mm_unpacklo_epi8(x_g, x_r);	\
	x_t4 = _mm_unpackhi_epi8(x_g, x_r);	\
	x_abgr0 = _mm_unpacklo_epi16(x_t1, x_t3);	\
	x_abgr1 = _mm_unpackhi_epi16(x_t1, x_t3);	\
	x_abgr2 = _mm_unpacklo_epi16(x_t2, x_t4);	\
	x_abgr3 = _mm_unpackhi_epi16(x_t2, x_t4);	\
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
	const __m128i x_zero = _mm_setzero_si128();
	__m128i x_abgr, x_r, x_g, x_b, x_a, x_rgb0, x_rgb1, x_rgb2, x_rgb3;
	__m128i x_abgr0, x_abgr1, x_abgr2, x_abgr3;
	__m128i x_t1, x_t2, x_t3, x_t4, x_t5, x_t6, x_t7, x_t8, x_t9;
	mlib_u8	*pa, *prgb;
	mlib_u32 *pabgr;
	mlib_s32 i, j, red, green, blue;
	mlib_u8 *psrc;

	mlib_u32 a_val = a_const;

	dlb >>= 2;
	slb >>= 2;

	if (h == 0 || w == 0)
		return;

	if (a_array) {
		for (i = 0; i < h;
			i++, xrgb += slb, a_array += alb, abgr += dlb) {
			pa = (mlib_u8 *)a_array;
			prgb = (mlib_u8 *)xrgb;
			pabgr = (mlib_u32 *)abgr;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j <= (w - 16); j += 16) {
				x_rgb0 = _mm_loadu_si128((__m128i *)prgb);
				prgb += 16;
				x_rgb1 = _mm_loadu_si128((__m128i *)prgb);
				prgb += 16;
				x_rgb2 = _mm_loadu_si128((__m128i *)prgb);
				prgb += 16;
				x_rgb3 = _mm_loadu_si128((__m128i *)prgb);
				prgb += 16;
				x_a = _mm_loadu_si128((__m128i *)pa);
				pa += 16;

				XRGB2ABGR_48(x_rgb0, x_rgb1, x_rgb2,
			x_rgb3, x_abgr0, x_abgr1, x_abgr2, x_abgr3);

				_mm_storeu_si128((__m128i *)pabgr, x_abgr0);
				pabgr += 4;
				_mm_storeu_si128((__m128i *)pabgr, x_abgr1);
				pabgr += 4;
				_mm_storeu_si128((__m128i *)pabgr, x_abgr2);
				pabgr += 4;
				_mm_storeu_si128((__m128i *)pabgr, x_abgr3);
				pabgr += 4;
				}

				for (; j <= (w - 1); j++) {
				psrc = (mlib_u8 *)(xrgb + j);
				a_val = a_array[j];
				blue = psrc[3] << 8;
				green = psrc[2] << 16;
				red = psrc[1] << 24;
				abgr[j] = a_val | blue | green | red;
			}
		}
	} else {
			a_val = a_const;
			x_a = _mm_set1_epi8((char)a_const);

		for (i = 0; i < h; i++, xrgb += slb, abgr += dlb) {
			pa = (mlib_u8 *)a_array;
			prgb = (mlib_u8 *)xrgb;
			pabgr = (mlib_u32 *)abgr;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j <= (w - 16); j += 16) {
				x_rgb0 = _mm_loadu_si128((__m128i *)prgb);
				prgb += 16;
				x_rgb1 = _mm_loadu_si128((__m128i *)prgb);
				prgb += 16;
				x_rgb2 = _mm_loadu_si128((__m128i *)prgb);
				prgb += 16;
				x_rgb3 = _mm_loadu_si128((__m128i *)prgb);
				prgb += 16;

				XRGB2ABGR_48(x_rgb0, x_rgb1, x_rgb2,
			x_rgb3, x_abgr0, x_abgr1, x_abgr2, x_abgr3);

				_mm_storeu_si128((__m128i *)pabgr, x_abgr0);
				pabgr += 4;
				_mm_storeu_si128((__m128i *)pabgr, x_abgr1);
				pabgr += 4;
				_mm_storeu_si128((__m128i *)pabgr, x_abgr2);
				pabgr += 4;
				_mm_storeu_si128((__m128i *)pabgr, x_abgr3);
				pabgr += 4;
				}

				for (; j <= (w - 1); j++) {
				psrc = (mlib_u8 *)(xrgb + j);
				blue = psrc[3] << 8;
				green = psrc[2] << 16;
				red = psrc[1] << 24;
				abgr[j] = a_val | blue | green | red;
			}
		}
	}
}

#define	XRGB2ARGB_48(x_rgb0, x_rgb1, x_rgb2, x_rgb3,\
	x_abgr0, x_abgr1, x_abgr2, x_abgr3) 	\
{	\
	__m128i x_a0, x_a1, x_a2, x_a3;		\
	x_rgb0 = _mm_and_si128(x_rgb0, mask);	\
	x_rgb1 = _mm_and_si128(x_rgb1, mask);	\
	x_rgb2 = _mm_and_si128(x_rgb2, mask);	\
	x_rgb3 = _mm_and_si128(x_rgb3, mask);	\
	x_a1 = _mm_unpacklo_epi8(x_a, x_zero);	\
	x_a3 = _mm_unpackhi_epi8(x_a, x_zero);	\
	x_a0 = _mm_unpacklo_epi16(x_a1, x_zero);\
	x_a1 = _mm_unpackhi_epi16(x_a1, x_zero);\
	x_a2 = _mm_unpacklo_epi16(x_a3, x_zero);\
	x_a3 = _mm_unpackhi_epi16(x_a3, x_zero);\
	x_abgr0 = _mm_or_si128(x_rgb0, x_a0);	\
	x_abgr1 = _mm_or_si128(x_rgb1, x_a1);	\
	x_abgr2 = _mm_or_si128(x_rgb2, x_a2);	\
	x_abgr3 = _mm_or_si128(x_rgb3, x_a3);	\
}

#define	XRGB2ARGB_48_UA(x_rgb0, x_rgb1, x_rgb2, x_rgb3,\
	x_abgr0, x_abgr1, x_abgr2, x_abgr3) 	\
{	\
	__m128i x_a0, x_a1, x_a2, x_a3;		\
	x_rgb0 = _mm_and_si128(x_rgb0, mask);	\
	x_rgb1 = _mm_and_si128(x_rgb1, mask);	\
	x_rgb2 = _mm_and_si128(x_rgb2, mask);	\
	x_rgb3 = _mm_and_si128(x_rgb3, mask);	\
	x_abgr0 = _mm_or_si128(x_rgb0, x_a);	\
	x_abgr1 = _mm_or_si128(x_rgb1, x_a);	\
	x_abgr2 = _mm_or_si128(x_rgb2, x_a);	\
	x_abgr3 = _mm_or_si128(x_rgb3, x_a);	\
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
	const __m128i x_zero = _mm_setzero_si128();
	__m128i x_abgr, x_r, x_g, x_b, x_a, x_rgb0, x_rgb1, x_rgb2, x_rgb3;
	__m128i x_abgr0, x_abgr1, x_abgr2, x_abgr3;
	__m128i x_t1, x_t2, x_t3, x_t4, x_t5, x_t6, x_t7, x_t8, x_t9;
	__m128i mask = _mm_set1_epi32(0xffffff00);
	mlib_u8	*pa, *prgb;
	mlib_u32 *pargb;
	mlib_s32 i, j;
	mlib_u32 val0;

	mlib_u32 a_val = a_const;

	dlb >>= 2;
	slb >>= 2;

	if (h == 0 || w == 0)
		return;

	if (a_array) {
		for (i = 0; i < h;
			i++, xrgb += slb, a_array += alb, argb += dlb) {
			pa = (mlib_u8 *)a_array;
			prgb = (mlib_u8 *)xrgb;
			pargb = (mlib_u32 *)argb;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j <= (w - 16); j += 16) {
				x_rgb0 = _mm_loadu_si128((__m128i *)prgb);
				prgb += 16;
				x_rgb1 = _mm_loadu_si128((__m128i *)prgb);
				prgb += 16;
				x_rgb2 = _mm_loadu_si128((__m128i *)prgb);
				prgb += 16;
				x_rgb3 = _mm_loadu_si128((__m128i *)prgb);
				prgb += 16;
				x_a = _mm_loadu_si128((__m128i *)pa);
				pa += 16;

				XRGB2ARGB_48(x_rgb0, x_rgb1,
		x_rgb2, x_rgb3, x_abgr0, x_abgr1, x_abgr2, x_abgr3);

				_mm_storeu_si128((__m128i *)pargb, x_abgr0);
				pargb += 4;
				_mm_storeu_si128((__m128i *)pargb, x_abgr1);
				pargb += 4;
				_mm_storeu_si128((__m128i *)pargb, x_abgr2);
				pargb += 4;
				_mm_storeu_si128((__m128i *)pargb, x_abgr3);
				pargb += 4;
				}

				for (; j <= (w - 1); j++) {
				val0 = xrgb[j];
				a_val = a_array[j];
				val0 = val0 & ~0xFF;
				argb[j] = a_val | val0;
			}
		}
	} else {
			a_val = a_const;
			x_a = _mm_set1_epi32(a_const);
		for (i = 0; i < h; i++, xrgb += slb, argb += dlb) {
			pa = (mlib_u8 *)a_array;
			prgb = (mlib_u8 *)xrgb;
			pargb = (mlib_u32 *)argb;
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j <= (w - 16); j += 16) {
				x_rgb0 = _mm_loadu_si128((__m128i *)prgb);
				prgb += 16;
				x_rgb1 = _mm_loadu_si128((__m128i *)prgb);
				prgb += 16;
				x_rgb2 = _mm_loadu_si128((__m128i *)prgb);
				prgb += 16;
				x_rgb3 = _mm_loadu_si128((__m128i *)prgb);
				prgb += 16;

				XRGB2ARGB_48_UA(x_rgb0, x_rgb1,
		x_rgb2, x_rgb3, x_abgr0, x_abgr1, x_abgr2, x_abgr3);

				_mm_storeu_si128((__m128i *)pargb, x_abgr0);
				pargb += 4;
				_mm_storeu_si128((__m128i *)pargb, x_abgr1);
				pargb += 4;
				_mm_storeu_si128((__m128i *)pargb, x_abgr2);
				pargb += 4;
				_mm_storeu_si128((__m128i *)pargb, x_abgr3);
				pargb += 4;
				}

				for (; j <= (w - 1); j++) {
				val0 = xrgb[j];
				val0 = val0 & ~0xFF;
				argb[j] = a_val | val0;
			}
		}
	}
}

/* *********************************************************** */
