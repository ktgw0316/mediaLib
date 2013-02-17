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

#pragma ident	"@(#)mlib_ImageZoomTranslateBlend.c	9.3	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageZoomTranslateBlend - image scaling with alpha blending
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageZoomTranslateBlend(mlib_image       *dst,
 *                                               const mlib_image *src,
 *                                               mlib_d64         zoomx,
 *                                               mlib_d64         zoomy,
 *                                               mlib_d64         tx,
 *                                               mlib_d64         ty,
 *                                               mlib_filter      filter,
 *                                               mlib_edge        edge,
 *                                               mlib_blend       blend,
 *                                               mlib_s32         alpha,
 *                                               mlib_s32         cmask);
 *
 * ARGUMENTS
 *      dst       Pointer to the destination image.
 *      src       Pointer to the source image.
 *      zoomx     X zoom factor.
 *      zoomy     Y zoom factor.
 *      tx        X translation.
 *      ty        Y translation.
 *      filter    Type of resampling filter.
 *      edge      Type of edge condition.
 *      blend     Type of alpha blending.
 *      alpha     The overall alpha for blending.
 *      cmask     The channel mask to indicate the alpha channel.
 *
 * DESCRIPTION
 *      Zoom an image in or out, translate it, and blend it with another
 *      image.
 *
 *      The zoom and translation are governed by the following formulae on
 *      coordinate mapping:
 *
 *          xd = zoomx*xs + tx
 *          yd = zoomy*ys + ty
 *
 *      where (xs, ys) and (xd, yd) are the points in the source and
 *      destination images respectively.
 *
 *      The center of the upper-left corner pixel of an image is considered
 *      to be located at (0.5, 0.5).
 *
 *      The resampling filter can be one of the following:
 *          MLIB_NEAREST
 *          MLIB_BILINEAR
 *          MLIB_BICUBIC
 *          MLIB_BICUBIC2
 *
 *      The edge condition can be one of the following:
 *          MLIB_EDGE_DST_NO_WRITE  (default)
 *          MLIB_EDGE_DST_FILL_ZERO
 *          MLIB_EDGE_OP_NEAREST
 *          MLIB_EDGE_SRC_EXTEND
 *          MLIB_EDGE_SRC_EXTEND_INDEF
 *          MLIB_EDGE_SRC_PADDED
 *
 *      The type of alpha blending can be one of the following:
 *          MLIB_BLEND_GTK_SRC
 *          MLIB_BLEND_GTK_SRC_OVER
 *          MLIB_BLEND_GTK_SRC_OVER2
 *
 *      The alpha blending is closely combined with the interpolation to
 *      achieve better performance. Part of alpha blending have to be
 *      performed before or together with the interpolation if the source
 *      image has an alpha channel. In that case, the color components of
 *      each neighboring source pixel which participates in the interpolation
 *      (src_r and etc.) have to be pre-multiplied by the alpha component
 *      of the same source pixel (src_a). After the interpolation, the
 *      overall alpha (alpha), the interpolated source alpha (interp_a)
 *      and the destination pixel's original alpha (dst_a, if any) are used
 *      to blend the interpolated source pixel (with components interp_r and
 *      etc.) with the destination pixel (with components dst_r and etc.).
 *
 *      The MLIB_BLEND_GTK_SRC blending is similar to the SRC rule
 *      of the Porter-Duff rules for image compositing. It is defined by
 *
 *          Cd = Cs
 *          Ad = As
 *
 *      in general, and by the following formula for this function:
 *
 *          if (interp_a != 0.0) {
 *              if (dst_has_alpha) {
 *                  dst_r = interp_r/interp_a;
 *                  dst_g = interp_g/interp_a;
 *                  dst_b = interp_b/interp_a;
 *                  dst_a = interp_a;
 *              } else {
 *                  dst_r = interp_r;
 *                  dst_g = interp_g;
 *                  dst_b = interp_b;
 *                  dst_a = 1.0; // implied
 *              }
 *          } else {
 *              dst_r = 0;
 *              dst_g = 0;
 *              dst_b = 0;
 *              dst_a = 0;
 *          }
 *
 *      The MLIB_BLEND_GTK_SRC_OVER or MLIB_BLEND_GTK_SRC_OVER2 blending is
 *      similar to the SRC_OVER rule of the Porter-Duff rules for image
 *      compositing. It is defined by
 *
 *          Cd = Cs + Cd*(1-As)
 *          Ad = As + Ad*(1-As)
 *
 *      in general, and by the following formula for this function:
 *
 *          w = alpha*interp_a + (1 - alpha*interp_a)*dst_a;
 *          if (w != 0.0) {
 *              dst_r = (alpha*interp_r + (1 - alpha*interp_a)*dst_a*dst_r)/w;
 *              dst_g = (alpha*interp_g + (1 - alpha*interp_a)*dst_a*dst_g)/w;
 *              dst_b = (alpha*interp_b + (1 - alpha*interp_a)*dst_a*dst_b)/w;
 *              dst_a = w;
 *          } else if (MLIB_BLEND_GTK_SRC_OVER) {
 *              dst_r = 0;
 *              dst_g = 0;
 *              dst_b = 0;
 *              dst_a = 0;
 *          }
 *
 *      where, the alpha, src_a, interp_a and dst_a are assumed to be in
 *      the range of [0.0, 1.0].
 *
 *      For an image with 4 channels, the first or the fourth channel is
 *      considered the alpha channel if cmask equals to 8 or 1, respectively.
 *      An image with 3 channels is considered to have no alpha channel,
 *      which is equivalent to having an alpha channel filled with all 1.0,
 *      or 0xff in case of MLIB_BYTE, if you want to use the general formulae
 *      shown above for blending.
 *
 *      Below are some hints on how this function might be implemented in the
 *      case of MLIB_BYTE images.
 *
 *      In the MLIB_BLEND_GTK_SRC case, the overall alpha is ignored. The
 *      interpolated value from neighboring src pixels will be used directly
 *      to replace or set the value of a dst pixel. But if the src image
 *      has an alpha channel, then the src alpha pre-multiplication is still
 *      needed before the interpolation. Furthermore, if the dst image has
 *      an alpha channel, then the interpolated color components need to be
 *      divided by the interpolated alpha to keep the dst image color
 *      components non-alpha-premultiplied. This can be described by the
 *      following pseudo code:
 *
 *          foreach pixel in dst
 *              if (src_has_alpha) {
 *                  foreach neighboring_pixel in src
 *                     pm_src_r = src_r * src_a / 0xff;
 *                     pm_src_g = src_g * src_a / 0xff;
 *                     pm_src_b = src_b * src_a / 0xff;
 *                  }
 *                  interpolate(&interp_r, &interp_g, &interp_b, &interp_a,
 *                              pm_src_r, pm_src_g, pm_src_b, src_a);
 *              } else {
 *                  interpolate(&interp_r, &interp_g, &interp_b,
 *                              src_r, src_g, src_b);
 *              }
 *
 *              if (dst_has_alpha) {
 *                  if (src_has_alpha) {
 *                      if (interp_a != 0) {
 *                          interp_r = interp_r * 0xff / interp_a;
 *                          interp_g = interp_g * 0xff / interp_a;
 *                          interp_b = interp_b * 0xff / interp_a;
 *                      } else {
 *                          interp_r = 0;
 *                          interp_g = 0;
 *                          interp_b = 0;
 *                      }
 *                      dst_a = interp_a;
 *                  } else {
 *                      dst_a = 0xff;
 *                  }
 *              }
 *              dst_r = interp_r;
 *              dst_g = interp_g;
 *              dst_b = interp_b;
 *          end
 *
 *      In the MLIB_BLEND_GTK_SRC_OVER and MLIB_BLEND_GTK_SRC_OVER2 cases,
 *      the overall alpha is important.
 *
 *      If (alpha == 0 && MLIB_BLEND_GTK_SRC_OVER2), this function should do
 *      nothing more than returning MLIB_SUCCESS.
 *
 *      If (alpha == 255 && !src_has_alpha), then no alpha blending is needed.
 *      This is similar to mlib_ImageZoomTranslate, except for that the dst
 *      alpha channel should be filled with 0xff if it exists.
 *
 *      If (alpha < 0 || alpha > 255), this function should return
 *      MLIB_OUTOFRANGE.
 *
 *      For any other cases, the MLIB_BLEND_GTK_SRC_OVER or
 *      MLIB_BLEND_GTK_SRC_OVER2 blending can be performed like the following
 *      pseudo code:
 *
 *          foreach pixel in dst
 *              int a0;
 *
 *              if (src_has_alpha) {
 *                  foreach neighboring_pixel in src
 *                     pm_src_r = src_r * src_a / 0xff;
 *                     pm_src_g = src_g * src_a / 0xff;
 *                     pm_src_b = src_b * src_a / 0xff;
 *                  }
 *                  interpolate(&interp_r, &interp_g, &interp_b, &interp_a,
 *                              pm_src_r, pm_src_g, pm_src_b, src_a);
 *                  a0 = (alpha * interp_a) / 0xff;
 *              } else {
 *                  interpolate(&interp_r, &interp_g, &interp_b,
 *                              src_r, src_g, src_b);
 *                  a0 = alpha;
 *              }
 *
 *              switch (a0) {
 *              case 0:
 *                  if (MLIB_BLEND_GTK_SRC_OVER && dst_has_alpha) {
 *                      if (dst_a == 0) {
 *                          dst_r = 0;
 *                          dst_g = 0;
 *                          dst_b = 0;
 *                      }
 *                  }
 *                  break;
 *              case 255:
 *                  dst_r = interp_r;
 *                  dst_g = interp_g;
 *                  dst_b = interp_b;
 *                  if (dst_has_alpha)
 *                     dst_a = 0xff;
 *                  break;
 *              default:
 *                  if (dst_has_alpha) {
 *                      int w0 = 0xff * a0;
 *                      int w1 = (0xff - a0) * dst_a
 *                      int w = w0 + w1;
 *
 *                      dst_r = (w0 * interp_r + w1 * dst_r) / w;
 *                      dst_g = (w0 * interp_g + w1 * dst_g) / w;
 *                      dst_b = (w0 * interp_b + w1 * dst_b) / w;
 *                      dst_a = w / 0xff;
 *                  } else {
 *                      int a1 = 0xff - a0;
 *                      int tmp;
 *
 *                      tmp = alpha * interp_r + a1 * dst_r + 0x80;
 *                      dst_r = (tmp + (tmp >> 8)) >> 8;
 *                      tmp = alpha * interp_g + a1 * dst_g + 0x80;
 *                      dst_g = (tmp + (tmp >> 8)) >> 8;
 *                      tmp = alpha * interp_b + a1 * dst_b + 0x80;
 *                      dst_b = (tmp + (tmp >> 8)) >> 8;
 *                  }
 *                  break;
 *              }
 *          end
 *
 *      It's up to the implementor to determine whether it has performance
 *      advantage of doing alpha pre-multiplication for the entire source
 *      image up front.
 *
 * RESTRICTION
 *      Both src and dst must be of type MLIB_BYTE.
 *      They can have either 3 or 4 channels.
 *      The cmask has to contain only 1 bit corresponding to 0 or 3
 *      channel number for a four-channels image, it is not used
 *      for three-channels image.
 *
 *      The src image can not have width or height larger than 32767.
 */

#include <mlib_image.h>
#include <mlib_SysMath.h>
#include <mlib_ImageCheck.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageZoomTranslateBlend = __mlib_ImageZoomTranslateBlend

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageZoomTranslateBlend) mlib_ImageZoomTranslateBlend
	__attribute__((weak, alias("__mlib_ImageZoomTranslateBlend")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	_MLIB_C_VER_

#include <mlib_ImageZoom.h>

/* *********************************************************** */

const mlib_u8 mlib_sat_u8[256 + 2 * 256] = {
	0, 1, 2, 3, 4, 5, 6, 7,
	8, 9, 10, 11, 12, 13, 14, 15,
	16, 17, 18, 19, 20, 21, 22, 23,
	24, 25, 26, 27, 28, 29, 30, 31,
	32, 33, 34, 35, 36, 37, 38, 39,
	40, 41, 42, 43, 44, 45, 46, 47,
	48, 49, 50, 51, 52, 53, 54, 55,
	56, 57, 58, 59, 60, 61, 62, 63,
	64, 65, 66, 67, 68, 69, 70, 71,
	72, 73, 74, 75, 76, 77, 78, 79,
	80, 81, 82, 83, 84, 85, 86, 87,
	88, 89, 90, 91, 92, 93, 94, 95,
	96, 97, 98, 99, 100, 101, 102, 103,
	104, 105, 106, 107, 108, 109, 110, 111,
	112, 113, 114, 115, 116, 117, 118, 119,
	120, 121, 122, 123, 124, 125, 126, 127,
	128, 129, 130, 131, 132, 133, 134, 135,
	136, 137, 138, 139, 140, 141, 142, 143,
	144, 145, 146, 147, 148, 149, 150, 151,
	152, 153, 154, 155, 156, 157, 158, 159,
	160, 161, 162, 163, 164, 165, 166, 167,
	168, 169, 170, 171, 172, 173, 174, 175,
	176, 177, 178, 179, 180, 181, 182, 183,
	184, 185, 186, 187, 188, 189, 190, 191,
	192, 193, 194, 195, 196, 197, 198, 199,
	200, 201, 202, 203, 204, 205, 206, 207,
	208, 209, 210, 211, 212, 213, 214, 215,
	216, 217, 218, 219, 220, 221, 222, 223,
	224, 225, 226, 227, 228, 229, 230, 231,
	232, 233, 234, 235, 236, 237, 238, 239,
	240, 241, 242, 243, 244, 245, 246, 247,
	248, 249, 250, 251, 252, 253, 254, 255,

	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,

	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255,
};

/* *********************************************************** */

mlib_status
__mlib_ImageZoomTranslateBlend(
	mlib_image *dst,
	const mlib_image *src,
	mlib_d64 zoomx,
	mlib_d64 zoomy,
	mlib_d64 tx,
	mlib_d64 ty,
	mlib_filter filter,
	mlib_edge edge,
	mlib_blend blend,
	mlib_s32 alpha,
	mlib_s32 cmask)
{
	mlib_s32 schan, dchan, t_ind, alp_ind;
	mlib_clipping nearest, current;
	mlib_work_image border, *param = &border;

	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_TYPE_EQUAL(src, dst);
	MLIB_IMAGE_HAVE_TYPE(src, MLIB_BYTE);

	schan = mlib_ImageGetChannels(src);
	dchan = mlib_ImageGetChannels(dst);

	if ((schan == 4 || dchan == 4) && cmask != 1 && cmask != 8)
	    return (MLIB_OUTOFRANGE);

	if (schan < 3 || schan > 4 || dchan < 3 || dchan > 4) {
		return (MLIB_FAILURE);
	}

	if (zoomx <= 0 || zoomy <= 0) {
		return (MLIB_OUTOFRANGE);
	}

	if ((alpha < 0 || alpha > 255) && (blend != MLIB_BLEND_GTK_SRC)) {
		return (MLIB_OUTOFRANGE);
	}

	if (mlib_ImageGetWidth(src) >= (1 << 15) ||
		mlib_ImageGetHeight(src) >= (1 << 15)) {
		return (MLIB_FAILURE);
	}

	t_ind = (schan - 3) + 2 * (dchan - 3);

	if (blend == MLIB_BLEND_GTK_SRC) {
		if (schan == 3) {
			alpha = 255;
		} else {
			if (dchan == 4 && filter == MLIB_NEAREST) {
				return __mlib_ImageZoomTranslate(dst, src,
					zoomx, zoomy, tx, ty, filter, edge);
			}
			alpha = -1;
		}
	}

	if (alpha == 0 && (blend == MLIB_BLEND_GTK_SRC_OVER2 || dchan == 3)) {
		return (MLIB_SUCCESS);
	}

	if (alpha == 255 && t_ind == 0) {
		return __mlib_ImageZoomTranslate(dst, src, zoomx, zoomy, tx, ty,
			filter, edge);
	}

	border.nearest = &nearest;
	border.current = &current;

	border.blend = blend;
	border.alpha = alpha;

	mlib_ImageZoomClipping(dst, src, zoomx, zoomy, tx, ty, filter, edge,
		param);

	if (cmask == 1) {
		alp_ind = 3;
	} else {
		alp_ind = -1;

		if (schan == 4)
			border.current->sp = (mlib_u8 *)border.current->sp + 1;
		if (dchan == 4)
			border.current->dp = (mlib_u8 *)border.current->dp + 1;
	}

	param->alp_ind = alp_ind;

	if (current.width > 0) {
		mlib_status res;

		res = mlib_ImageZoomTranslateBlend_intern(param, filter, zoomy,
			t_ind);
		if (res != MLIB_SUCCESS)
			return (res);
	}

	if (edge == MLIB_EDGE_DST_FILL_ZERO || edge == MLIB_EDGE_OP_NEAREST ||
		edge == MLIB_EDGE_SRC_EXTEND ||
		edge == MLIB_EDGE_SRC_EXTEND_INDEF) {
		return mlib_ZoomBlendEdge(dst, src, param, filter, edge,
			alp_ind);
	} else {
		return (MLIB_SUCCESS);
	}
}

/* *********************************************************** */
