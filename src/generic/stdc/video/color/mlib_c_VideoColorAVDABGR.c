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

#pragma ident	"@(#)mlib_c_VideoColorAVDABGR.c	9.4	07/10/09 SMI"

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
	mlib_s32 i, j;

	dlb >>= 2;
	slb >>= 2;

	for (i = 0; i < h; i++, argb += dlb, abgr += slb) {
		for (j = 0; j < w; j++) {
			mlib_u16 *src = (mlib_u16 *)(abgr + j);
			mlib_u16 *dst = (mlib_u16 *)(argb + j);
			mlib_u32 val0, val1, val2, val3;

			val0 = src[0];
			val1 = src[1];
#ifdef _LITTLE_ENDIAN
			val3 = (val0 & 0xFF00) | (val1 & 0xFF);
			val2 = (val0 & 0xFF) | (val1 & 0xFF00);
#else /* _LITTLE_ENDIAN */
			val2 = (val0 & 0xFF00) | (val1 & 0xFF);
			val3 = (val0 & 0xFF) | (val1 & 0xFF00);
#endif /* _LITTLE_ENDIAN */
			dst[0] = (mlib_u16)val2;
			dst[1] = (mlib_u16)val3;
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

	dlb >>= 2;
	slb >>= 2;

	for (i = 0; i < h; i++, abgr += dlb, bgra += slb) {
		for (j = 0; j < w; j++) {
			mlib_u16 *src = (mlib_u16 *)(bgra + j);
			mlib_u16 *dst = (mlib_u16 *)(abgr + j);
			mlib_u32 val0, val1, val2, val3;

			val0 = src[0];
			val1 = src[1];
#ifdef _LITTLE_ENDIAN
			val3 = (val0 >> 8) | (val1 << 8);
			val2 = (val0 << 8) | (val1 >> 8);
#else /* _LITTLE_ENDIAN */
			val2 = (val0 >> 8) | (val1 << 8);
			val3 = (val0 << 8) | (val1 >> 8);
#endif /* _LITTLE_ENDIAN */
			dst[0] = (mlib_u16)val2;
			dst[1] = (mlib_u16)val3;
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
	mlib_s32 i, j;

	dlb >>= 2;
	slb >>= 2;

	for (i = 0; i < h; i++, rgba += dlb, abgr += slb) {
		for (j = 0; j < w; j++) {
			mlib_u16 *src = (mlib_u16 *)(abgr + j);
			mlib_u16 *dst = (mlib_u16 *)(rgba + j);
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
	mlib_s32 i, j;
	mlib_u32 val;

	if (h == 0 || w == 0)
		return;

	dlb >>= 2;

	if (a_array) {
		for (i = 0; i < h;
			i++, a_array += slb, b += slb, g += slb, r += slb) {
#ifdef _LITTLE_ENDIAN
			val = (a_array[0]) | (b[0] << 8) | (g[0] << 16) | (r[0]
				<< 24);
#else /* _LITTLE_ENDIAN */
			val = (a_array[0] << 24) | (b[0] << 16) | (g[0] << 8) |
				r[0];
#endif /* _LITTLE_ENDIAN */

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 1; j < w; j++) {
#ifdef _LITTLE_ENDIAN
				mlib_s32 val1 = (a_array[j]) | (b[j] << 8);
				mlib_s32 val2 = (g[j] << 16) | (r[j] << 24);
#else /* _LITTLE_ENDIAN */
				mlib_s32 val1 =
					(a_array[j] << 24) | (b[j] << 16);
				mlib_s32 val2 = (g[j] << 8) | r[j];
#endif /* _LITTLE_ENDIAN */

				abgr[j - 1] = val;
				val = val1 | val2;
			}

			abgr[j - 1] = val;
			abgr += dlb;
		}
	} else {
#ifdef _LITTLE_ENDIAN
		mlib_u32 a_val = a_const;
#else /* _LITTLE_ENDIAN */
		mlib_u32 a_val = a_const << 24;
#endif /* _LITTLE_ENDIAN */

		for (i = 0; i < h;
			i++, b += slb, g += slb, r += slb, abgr += dlb) {
#ifdef _LITTLE_ENDIAN
			val = a_val | (b[0] << 8) | (g[0] << 16) | (r[0] << 24);
#else /* _LITTLE_ENDIAN */
			val = a_val | (b[0] << 16) | (g[0] << 8) | r[0];
#endif /* _LITTLE_ENDIAN */

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 1; j < w; j++) {
#ifdef _LITTLE_ENDIAN
				mlib_s32 val1 = (b[j] << 8) | (r[j] << 24);
				mlib_s32 val2 = (g[j] << 16) | a_val;
#else /* _LITTLE_ENDIAN */
				mlib_s32 val1 = (b[j] << 16) | r[j];
				mlib_s32 val2 = (g[j] << 8) | a_val;
#endif /* _LITTLE_ENDIAN */

				abgr[j - 1] = val;
				val = val2 | val1;
			}

			abgr[j - 1] = val;
		}
	}
}

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
	mlib_s32 i, j, red, green, blue;

#ifdef _LITTLE_ENDIAN
	mlib_u32 a_val = a_const;
#else /* _LITTLE_ENDIAN */
	mlib_u32 a_val = a_const << 24;
#endif /* _LITTLE_ENDIAN */

	dlb >>= 2;

	if (h == 0 || w == 0)
		return;

	if (a_array) {
		for (i = 0; i < h; i++, rgb += slb, a_array += alb,
		    abgr += dlb) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < w; j++) {
				mlib_s32 si = 3 * j;

#ifdef _LITTLE_ENDIAN
				a_val = a_array[j];
				blue = rgb[si + 2] << 8;
				green = rgb[si + 1] << 16;
				red = rgb[si] << 24;
#else /* _LITTLE_ENDIAN */
				a_val = a_array[j] << 24;
				blue = rgb[si + 2] << 16;
				green = rgb[si + 1] << 8;
				red = rgb[si];
#endif /* _LITTLE_ENDIAN */

				abgr[j] = a_val | blue | green | red;
			}
		}
	} else {

		for (i = 0; i < h; i++, rgb += slb, abgr += dlb) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < w; j++) {
				mlib_s32 si = 3 * j;

#ifdef _LITTLE_ENDIAN
				blue = rgb[si + 2] << 8;
				green = rgb[si + 1] << 16;
				red = rgb[si] << 24;
#else /* _LITTLE_ENDIAN */
				blue = rgb[si + 2] << 16;
				green = rgb[si + 1] << 8;
				red = rgb[si];
#endif /* _LITTLE_ENDIAN */

				abgr[j] = a_val | blue | green | red;
			}
		}
	}
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
	mlib_s32 i, j, red, green, blue;
	mlib_u32 a_val;

	if (h == 0 || w == 0)
		return;

	if (!(((mlib_addr)bgra | dlb) & 0x3)) {
		mlib_u32 *bgra32 = (mlib_u32 *)bgra;
		dlb >>= 2;

		if (a_array) {
			for (i = 0; i < h; i++, rgb += slb, a_array += alb,
			    bgra32 += dlb) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j < w; j++) {
					mlib_s32 si = 3 * j;

#ifdef _LITTLE_ENDIAN
					blue = rgb[si + 2];
					green = rgb[si + 1] << 8;
					red = rgb[si] << 16;
					a_val = a_array[j] << 24;
#else /* _LITTLE_ENDIAN */
					blue = rgb[si + 2] << 24;
					green = rgb[si + 1] << 16;
					red = rgb[si] << 8;
					a_val = a_array[j];
#endif /* _LITTLE_ENDIAN */

					bgra32[j] = a_val | blue | green | red;
				}
			}
		} else {
#ifdef _LITTLE_ENDIAN
			a_val = a_const << 24;
#else /* _LITTLE_ENDIAN */
			a_val = a_const;
#endif /* _LITTLE_ENDIAN */

			for (i = 0; i < h; i++, rgb += slb, bgra32 += dlb) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j < w; j++) {
					mlib_s32 si = 3 * j;

#ifdef _LITTLE_ENDIAN
					blue = rgb[si + 2];
					green = rgb[si + 1] << 8;
					red = rgb[si] << 16;
#else /* _LITTLE_ENDIAN */
					blue = rgb[si + 2] << 24;
					green = rgb[si + 1] << 16;
					red = rgb[si] << 8;
#endif /* _LITTLE_ENDIAN */

					bgra32[j] = a_val | blue | green | red;
				}
			}
		}
	} else {
		if (a_array) {
			for (i = 0; i < h; i++, rgb += slb, a_array += alb,
			    bgra += dlb) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j < w; j++) {
					mlib_s32 si = 3 * j;
					mlib_s32 di = 4 * j;

					bgra[di] = rgb[si + 2];
					bgra[di + 1] = rgb[si + 1];
					bgra[di + 2] = rgb[si];
					bgra[di + 3] = a_array[j];
				}
			}
		} else {
			a_val = a_const;

			for (i = 0; i < h; i++, rgb += slb, bgra += dlb) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
				for (j = 0; j < w; j++) {
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
	mlib_s32 i, j, red, green, blue;

#ifdef _LITTLE_ENDIAN
	mlib_u32 a_val = a_const;
#else /* _LITTLE_ENDIAN */
	mlib_u32 a_val = a_const << 24;
#endif /* _LITTLE_ENDIAN */

	dlb >>= 2;

	if (h == 0 || w == 0)
		return;

	if (a_array) {
		for (i = 0; i < h; i++, bgr += slb, a_array += alb,
		    abgr += dlb) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < w; j++) {
				mlib_s32 si = 3 * j;

#ifdef _LITTLE_ENDIAN
				a_val = a_array[j];
				blue = bgr[si] << 8;
				green = bgr[si + 1] << 16;
				red = bgr[si + 2] << 24;
#else /* _LITTLE_ENDIAN */
				a_val = a_array[j] << 24;
				blue = bgr[si] << 16;
				green = bgr[si + 1] << 8;
				red = bgr[si + 2];
#endif /* _LITTLE_ENDIAN */

				abgr[j] = a_val | blue | green | red;
			}
		}
	} else {

		for (i = 0; i < h; i++, bgr += slb, abgr += dlb) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < w; j++) {
				mlib_s32 si = 3 * j;

#ifdef _LITTLE_ENDIAN
				blue = bgr[si] << 8;
				green = bgr[si + 1] << 16;
				red = bgr[si + 2] << 24;
#else /* _LITTLE_ENDIAN */
				blue = bgr[si] << 16;
				green = bgr[si + 1] << 8;
				red = bgr[si + 2];
#endif /* _LITTLE_ENDIAN */

				abgr[j] = a_val | blue | green | red;
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
	mlib_s32 i, j, red, green, blue;
	mlib_u8 *psrc;

#ifdef _LITTLE_ENDIAN
	mlib_u32 a_val = a_const;
#else /* _LITTLE_ENDIAN */
	mlib_u32 a_val = a_const << 24;
#endif /* _LITTLE_ENDIAN */

	dlb >>= 2;
	slb >>= 2;

	if (h == 0 || w == 0)
		return;

	if (a_array) {
		for (i = 0; i < h;
			i++, rgbx += slb, a_array += alb, abgr += dlb) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < w; j++) {
				psrc = (mlib_u8 *)(rgbx + j);

#ifdef _LITTLE_ENDIAN
				a_val = a_array[j];
				blue = psrc[2] << 8;
				green = psrc[1] << 16;
				red = psrc[0] << 24;
#else /* _LITTLE_ENDIAN */
				a_val = a_array[j] << 24;
				blue = psrc[2] << 16;
				green = psrc[1] << 8;
				red = psrc[0];
#endif /* _LITTLE_ENDIAN */

				abgr[j] = a_val | blue | green | red;
			}
		}
	} else {

		for (i = 0; i < h; i++, rgbx += slb, abgr += dlb) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < w; j++) {
				psrc = (mlib_u8 *)(rgbx + j);

#ifdef _LITTLE_ENDIAN
				blue = psrc[2] << 8;
				green = psrc[1] << 16;
				red = psrc[0] << 24;
#else /* _LITTLE_ENDIAN */
				blue = psrc[2] << 16;
				green = psrc[1] << 8;
				red = psrc[0];
#endif /* _LITTLE_ENDIAN */

				abgr[j] = a_val | blue | green | red;
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
	mlib_u32 val0;

#ifdef _LITTLE_ENDIAN
	mlib_u32 a_val = a_const;
#else /* _LITTLE_ENDIAN */
	mlib_u32 a_val = a_const << 24;
#endif /* _LITTLE_ENDIAN */

	dlb >>= 2;
	slb >>= 2;

	if (h == 0 || w == 0)
		return;

	if (a_array) {
		for (i = 0; i < h;
			i++, rgbx += slb, a_array += alb, argb += dlb) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < w; j++) {
				val0 = rgbx[j];

#ifdef _LITTLE_ENDIAN
				a_val = a_array[j];
				val0 = val0 << 8;
#else /* _LITTLE_ENDIAN */
				a_val = a_array[j] << 24;
				val0 = val0 >> 8;
#endif /* _LITTLE_ENDIAN */

				argb[j] = a_val | val0;
			}
		}
	} else {

		for (i = 0; i < h; i++, rgbx += slb, argb += dlb) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < w; j++) {
				val0 = rgbx[j];

#ifdef _LITTLE_ENDIAN
				val0 = val0 << 8;
#else /* _LITTLE_ENDIAN */
				val0 = val0 >> 8;
#endif /* _LITTLE_ENDIAN */

				argb[j] = a_val | val0;
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
	mlib_s32 i, j, red, green, blue;
	mlib_u8 *psrc;

#ifdef _LITTLE_ENDIAN
	mlib_u32 a_val = a_const;
#else /* _LITTLE_ENDIAN */
	mlib_u32 a_val = a_const << 24;
#endif /* _LITTLE_ENDIAN */

	dlb >>= 2;
	slb >>= 2;

	if (h == 0 || w == 0)
		return;

	if (a_array) {
		for (i = 0; i < h;
			i++, xrgb += slb, a_array += alb, abgr += dlb) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < w; j++) {
				psrc = (mlib_u8 *)(xrgb + j);

#ifdef _LITTLE_ENDIAN
				a_val = a_array[j];
				blue = psrc[3] << 8;
				green = psrc[2] << 16;
				red = psrc[1] << 24;
#else /* _LITTLE_ENDIAN */
				a_val = a_array[j] << 24;
				blue = psrc[3] << 16;
				green = psrc[2] << 8;
				red = psrc[1];
#endif /* _LITTLE_ENDIAN */

				abgr[j] = a_val | blue | green | red;
			}
		}
	} else {

		for (i = 0; i < h; i++, xrgb += slb, abgr += dlb) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < w; j++) {
				psrc = (mlib_u8 *)(xrgb + j);

#ifdef _LITTLE_ENDIAN
				blue = psrc[3] << 8;
				green = psrc[2] << 16;
				red = psrc[1] << 24;
#else /* _LITTLE_ENDIAN */
				blue = psrc[3] << 16;
				green = psrc[2] << 8;
				red = psrc[1];
#endif /* _LITTLE_ENDIAN */

				abgr[j] = a_val | blue | green | red;
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
	mlib_u32 val0;

#ifdef _LITTLE_ENDIAN
	mlib_u32 a_val = a_const;
#else /* _LITTLE_ENDIAN */
	mlib_u32 a_val = a_const << 24;
#endif /* _LITTLE_ENDIAN */

	dlb >>= 2;
	slb >>= 2;

	if (h == 0 || w == 0)
		return;

	if (a_array) {
		for (i = 0; i < h;
			i++, xrgb += slb, a_array += alb, argb += dlb) {

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < w; j++) {
				val0 = xrgb[j];

#ifdef _LITTLE_ENDIAN
				a_val = a_array[j];
				val0 = val0 & ~0xFF;
#else /* _LITTLE_ENDIAN */
				a_val = a_array[j] << 24;
				val0 = val0 & 0xFFFFFF;
#endif /* _LITTLE_ENDIAN */

				argb[j] = a_val | val0;
			}
		}
	} else {

		for (i = 0; i < h; i++, xrgb += slb, argb += dlb) {
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (j = 0; j < w; j++) {
				val0 = xrgb[j];

#ifdef _LITTLE_ENDIAN
				val0 = val0 & ~0xFF;
#else /* _LITTLE_ENDIAN */
				val0 = val0 & 0xFFFFFF;
#endif /* _LITTLE_ENDIAN */

				argb[j] = a_val | val0;
			}
		}
	}
}

/* *********************************************************** */
