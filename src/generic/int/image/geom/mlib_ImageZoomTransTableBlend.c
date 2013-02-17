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

#pragma ident	"@(#)mlib_ImageZoomTransTableBlend.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageZoomTranslateTableBlend - image scaling using interpolation
 *                                          table, combined with alpha blending
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageZoomTranslateTableBlend(mlib_image       *dst,
 *                                                    const mlib_image *src,
 *                                                    mlib_d64         zoomx,
 *                                                    mlib_d64         zoomy,
 *                                                    mlib_d64         tx,
 *                                                    mlib_d64         ty,
 *                                                    const void       *table,
 *                                                    mlib_edge        edge,
 *                                                    mlib_blend       blend,
 *                                                    mlib_s32         cmask);
 *
 * ARGUMENTS
 *      dst       Pointer to the destination image.
 *      src       Pointer to the source image.
 *      zoomx     X zoom factor.
 *      zoomy     Y zoom factor.
 *      tx        X translation.
 *      ty        Y translation.
 *      table     Pointer to interpolation table structure.
 *      edge      Type of edge condition.
 *      blend     Type of alpha blending.
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
 *      It is assumed that the overall alpha for controlling the blending
 *      between the source image and the destination image has been pre-
 *      multiplied to the interpolation table for better performance.
 *
 *      The alpha blending is closely combined with the interpolation to
 *      achieve better performance. Part of alpha blending have to be
 *      performed before or together with the interpolation if the source
 *      image has an alpha channel. In that case, the color components of
 *      each neighboring source pixel which participates in the interpolation
 *      (src_r and etc.) have to be pre-multiplied by the alpha component
 *      of the same source pixel (src_a). After the interpolation, the
 *      interpolated alpha (interp_a, which has been multiplied by the
 *      overall alpha because of the pre-multiplied interpolation table)
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
 *          w = interp_a + (1 - interp_a)*dst_a;
 *          if (w != 0.0) {
 *              dst_r = (interp_r + (1 - interp_a)*dst_a*dst_r)/w;
 *              dst_g = (interp_g + (1 - interp_a)*dst_a*dst_g)/w;
 *              dst_b = (interp_b + (1 - interp_a)*dst_a*dst_b)/w;
 *              dst_a = w;
 *          } else if (MLIB_BLEND_GTK_SRC_OVER) {
 *              dst_r = 0;
 *              dst_g = 0;
 *              dst_b = 0;
 *              dst_a = 0;
 *          }
 *
 *      where, the src_a, interp_a and dst_a are assumed to be in the range
 *      of [0.0, 1.0].
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
 *      In the MLIB_BLEND_GTK_SRC case, the interpolated value from
 *      neighboring src pixels will be used directly to replace or set the
 *      value of a dst pixel. But if the src image has an alpha channel,
 *      then the src alpha pre-multiplication is still needed before the
 *      interpolation. Furthermore, if the dst image has an alpha channel,
 *      then the interpolated color components need to be divided by the
 *      interpolated alpha to keep the dst image color components non-alpha-
 *      premultiplied. This can be described by the following pseudo code:
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
 *      The MLIB_BLEND_GTK_SRC_OVER and MLIB_BLEND_GTK_SRC_OVER2 blending
 *      can be performed like the following pseudo code:
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
 *              } else {
 *                  interpolate(&interp_r, &interp_g, &interp_b, &interp_a,
 *                              src_r, src_g, src_b, 0xff);
 *              }
 *              a0 = interp_a;
 *
 *              if (dst_has_alpha) {
 *                  int a1 = (0xff - a0) * dst_a / 0xff;
 *                  int w = a0 + a1;
 *
 *                  if (w != 0) {
 *                      dst_r = (0xff * interp_r + a1 * dst_r) / w;
 *                      dst_g = (0xff * interp_g + a1 * dst_g) / w;
 *                      dst_b = (0xff * interp_b + a1 * dst_b) / w;
 *                      dst_a = w;
 *                  } else if (MLIB_BLEND_GTK_SRC_OVER) {
 *                      dst_r = 0;
 *                      dst_g = 0;
 *                      dst_b = 0;
 *                      dst_a = 0;
 *                  }
 *              } else {
 *                  dst_r = interp_r + (0xff - a0) * dst_r / 0xff;
 *                  dst_g = interp_g + (0xff - a0) * dst_g / 0xff;
 *                  dst_b = interp_b + (0xff - a0) * dst_b / 0xff;
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
#include <mlib_ImageAffine.h>
#include <mlib_ImageInterpTable.h>
#include <mlib_ImageZoomTranslateTable.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageZoomTranslateTableBlend = \
	__mlib_ImageZoomTranslateTableBlend

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageZoomTranslateTableBlend)
    mlib_ImageZoomTranslateTableBlend
    __attribute__((weak, alias("__mlib_ImageZoomTranslateTableBlend")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

#define	BUFF_SIZE	4096

/* *********************************************************** */

#define	INT_BITS	29

/* *********************************************************** */

#define	FUNCNAME_EXT	mlib_ImageZoomTranslateTableBlend_8ext

/* *********************************************************** */

#define	CREATE_X_IND()                                            \
	                                                          \
/*                                                                \
 * calc x positions                                               \
 */                                                               \
	p_x_ind = x_ind =                                         \
	__mlib_malloc((srcWidth + 8 * kw) * sizeof (mlib_s32));   \
	if (p_x_ind == NULL) {                                    \
	    if (param_e->buff_malloc != NULL)                     \
		__mlib_free(param_e->buff_malloc);                \
	    if (param->buff_malloc != NULL)                       \
		__mlib_free(param->buff_malloc);                  \
	    return (MLIB_FAILURE);                                \
	}                                                         \
	xpos = 0;                                                 \
	for (i = 0; i < 4 * kw; i++)                              \
	    x_ind[i] = xpos;                                      \
	x_ind += 4 * kw;                                          \
	for (i = 0; i < srcWidth - 1; i++) {                      \
	    x_ind[i] = xpos;                                      \
	    xpos += schan;                                        \
	}                                                         \
	                                                          \
	for (i = srcWidth - 1; i < srcWidth + 4 * kw; i++)        \
	    x_ind[i] = xpos

/* *********************************************************** */

#define	CREATE_X_EXT()                                                   \
	if (x_tab == NULL) {                                             \
	    x_tab = __mlib_malloc((dstWidth + 1) * sizeof (mlib_s32));   \
	    if (x_tab == NULL) {                                         \
		__mlib_free(p_x_ind);                                    \
		if (param_e->buff_malloc != NULL)                        \
		    __mlib_free(param_e->buff_malloc);                   \
		if (param->buff_malloc != NULL)                          \
		    __mlib_free(param->buff_malloc);                     \
		return (MLIB_FAILURE);                                   \
	    }                                                            \
	}                                                                \
	xpos = xStarts[yStart] - ws->x_move;                             \
	dx = ws->dx;                                                     \
	for (i = 0; i <= xRight_e - xLeft_e; i++) {                      \
	    x_tab[i] = xpos;                                             \
	    xpos += dx;                                                  \
	}
/* *********************************************************** */

#ifdef _NO_LONGLONG

#define	CREATE_X(xLeft, xRight)                                          \
	if (x_tab == NULL) {                                             \
	    x_tab = __mlib_malloc((dstWidth + 1) * sizeof (mlib_s32));   \
	    if (x_tab == NULL) {                                         \
		__mlib_free(p_x_ind);                                    \
		if (param_e->buff_malloc != NULL)                        \
		    __mlib_free(param_e->buff_malloc);                   \
		if (param->buff_malloc != NULL)                          \
		    __mlib_free(param->buff_malloc);                     \
		return (MLIB_FAILURE);                                   \
	    }                                                            \
	}                                                                \
	div = 1.0 / zoomx;                                               \
	dxs = ((xLeft) + 0.5 - tx) * div - (kw1 + 0.5);                  \
	for (i = 0; i <= (xRight) - (xLeft); i++) {                      \
	    tmp_dxs = dxs;                                               \
	    if (tmp_dxs < -kw)                                           \
		tmp_dxs = -kw;                                           \
	    else if (tmp_dxs > srcWidth - 1)                             \
		tmp_dxs = srcWidth - 1;                                  \
	    x_tab[i] = (mlib_s32)(tmp_dxs * (1 << x_shift));             \
	    dxs += div;                                                  \
	}

#else

/* 20 times magnify */
#define	AMP_DB 20
#define	DB2LL(d) (((d) << 1) == 0 ? 0 :					\
		((((d) & 0x000fffffffffffff | 0x0010000000000000)	\
		>> (52 - AMP_DB - ((((d) >> 52) & 0x7ff) - 1023)))	\
		* (((d) >> 63 << 1) + 1)))

#define	CREATE_X(xLeft, xRight)                                          \
	if (x_tab == NULL) {                                             \
	    x_tab = __mlib_malloc((dstWidth + 1) * sizeof (mlib_s32));   \
	    if (x_tab == NULL) {                                         \
		__mlib_free(p_x_ind);                                    \
		if (param_e->buff_malloc != NULL)                        \
		    __mlib_free(param_e->buff_malloc);                   \
		if (param->buff_malloc != NULL)                          \
		    __mlib_free(param->buff_malloc);                     \
		return (MLIB_FAILURE);                                   \
	    }                                                            \
	}                                                                \
	tmp_dxs = DB2LL(*(mlib_s64 *)(&tx));				\
	div = DB2LL(*(mlib_s64 *)(&zoomx));				\
	div = (1ll << (AMP_DB+AMP_DB)) / div;                           \
	dxs = ((xLeft) * div) + (div >> 1) - (tmp_dxs * div >> AMP_DB)	\
		-(kw1 << AMP_DB) - (1ll << AMP_DB >> 1);		\
	lower_bound = -kw << AMP_DB;					\
	upper_bound = (srcWidth - 1) << AMP_DB;				\
	for (i = 0; i <= (xRight) - (xLeft); i++) {                     \
	    tmp_dxs = dxs;	                                       \
	    if (tmp_dxs < lower_bound)                                 \
			tmp_dxs = lower_bound;                        \
	    else if (tmp_dxs > upper_bound)                           \
			tmp_dxs = upper_bound;                        \
	    x_tab[i] = (mlib_s32)(tmp_dxs << x_shift >> AMP_DB);	\
	    dxs += div;                                                \
	}

#endif
/* *********************************************************** */

mlib_status
__mlib_ImageZoomTranslateTableBlend(
    mlib_image *dst,
    const mlib_image *src,
    mlib_d64 zoomx,
    mlib_d64 zoomy,
    mlib_d64 tx,
    mlib_d64 ty,
    const void *table,
    mlib_edge edge,
    mlib_blend blend,
    mlib_s32 cmask)
{
	mlib_affine_param param[1];
	mlib_affine_param *cur_param;
	mlib_zoom_workspace ws[1];
	mlib_d64 buff_lcl[BUFF_SIZE / 8];
	mlib_type type;
	mlib_u8 *srcData, *dstData;
	mlib_s32 srcWidth, dstWidth, srcHeight, dstHeight;
	mlib_s32 srcStride, dstStride, schan, dchan;
	mlib_s32 *leftEdges, *rightEdges, *xStarts, *yStarts;
	mlib_s32 *p_x_ind = NULL, *x_ind, *x_tab = NULL, xpos;
	mlib_u8 **lineAddr = NULL;
	mlib_s32 kw, kh, kw1, kh1;
	mlib_status res = MLIB_SUCCESS;
	fun_type_nw fun_nw = NULL;
	mlib_interp_table *tbl = (mlib_interp_table *) table;
#ifdef	_NO_LONGLONG
	mlib_d64 mtx[6], dxs, tmp_dxs, div;
#else
	mlib_d64 mtx[6];
	mlib_s64 dxs, tmp_dxs, div, lower_bound, upper_bound;
#endif
	mlib_s32 i, x_shift, y_shift;
	mlib_s32 affine = 0, yStart;
	mlib_s32 xLeft_e, xRight_e, xLeft, xRight, dx;

#ifdef _NO_LONGLONG
	mtx[0] = zoomx;
	mtx[1] = 0;
	mtx[2] = tx;
	mtx[3] = 0;
	mtx[4] = zoomy;
	mtx[5] = ty;

	ws->zoomx = zoomx;
	ws->zoomy = zoomy;
#else
	*(mlib_s64 *)&mtx[0] = *(mlib_s64 *)&zoomx;
	*(mlib_s64 *)&mtx[1] = 0;
	*(mlib_s64 *)&mtx[2] = *(mlib_s64 *)&tx;
	*(mlib_s64 *)&mtx[3] = 0;
	*(mlib_s64 *)&mtx[4] = *(mlib_s64 *)&zoomy;
	*(mlib_s64 *)&mtx[5] = *(mlib_s64 *)&ty;

	*(mlib_s64 *)&ws->zoomx = *(mlib_s64 *)&zoomx;
	*(mlib_s64 *)&ws->zoomy = *(mlib_s64 *)&zoomy;
#endif

/* check for obvious errors */
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_TYPE_EQUAL(src, dst);
	MLIB_IMAGE_HAVE_TYPE(src, MLIB_BYTE);

#ifdef	_NO_LONGLONG
	if (zoomx <= 0 || zoomy <= 0)
		return (MLIB_OUTOFRANGE);
#else
	/* integer op instead of float point op */
	if (*(mlib_s64 *)(&zoomx) <= 0 || *(mlib_s64 *)(&zoomy) <= 0)
		return (MLIB_OUTOFRANGE);
#endif	/* defined _NO_LONGLONG */

	if (mlib_ImageGetWidth(src) >= (1 << 15) ||
	    mlib_ImageGetHeight(src) >= (1 << 15)) {
		return (MLIB_FAILURE);
	}

	MLIB_IMAGE_GET_ALL_PARAMS(src, type, schan, srcWidth, srcHeight,
	    srcStride, srcData);
	MLIB_IMAGE_GET_ALL_PARAMS(dst, type, dchan, dstWidth, dstHeight,
	    dstStride, dstData);

	if ((schan == 4 || dchan == 4) && cmask != 1 && cmask != 8)
	    return (MLIB_OUTOFRANGE);

	if (schan < 3 || schan > 4 || dchan < 3 || dchan > 4) {
		return (MLIB_FAILURE);
	}

	if ((blend == MLIB_BLEND_GTK_SRC) && (schan == 3) && (dchan == 3))
		return __mlib_ImageZoomTranslateTable(dst, src, zoomx, zoomy,
		    tx, ty, table, edge);

	kw = tbl->width;
	kh = tbl->height;
	kw1 = tbl->leftPadding;
	kh1 = tbl->topPadding;

	x_shift = INT_BITS - mlib_ilogb(srcWidth + kw);
	y_shift = INT_BITS - mlib_ilogb(srcHeight + kh);

	ws->type = type;
	ws->srcData = srcData;
	ws->dstData = dstData;
	ws->srcWidth = srcWidth;
	ws->srcHeight = srcHeight;
	ws->srcStride = srcStride;
	ws->dstStride = dstStride;
	ws->nchan = schan;
	ws->dchan = dchan;
	ws->blend = blend;
	ws->alpha_shift = 1;
	ws->edge = edge;
	ws->x_shift = x_shift;
	ws->y_shift = y_shift;
	ws->x_move = (kw1 << x_shift);
	ws->y_move = (kh1 << y_shift);

	if (cmask == 1)
		ws->alpha_shift = -3;

/* VIS version of non NULL */
	fun_nw = mlib_ImageZoomTranslate_GetFunc(ws, table);

	if (fun_nw == NULL) {
		fun_nw = mlib_ImageZoomTranslateTableBlend_8nw;
	}

/* NULL */
	STORE_PARAM(param, affine);
	STORE_PARAM(param, lineAddr);
	param->buff_malloc = NULL;

/* process internal pixels */

	res = mlib_AffineEdges(param, dst, src, buff_lcl, BUFF_SIZE,
	    kw, kh, kw1, kh1, edge, mtx, x_shift, y_shift);

	if (res != MLIB_SUCCESS)
		return (res);

	ws->yStart = param->yStart;
	ws->yFinish = param->yFinish;
	ws->max_xsize = param->max_xsize;
	ws->dx = param->dX;
	ws->dy = param->dY;

	LOAD_PARAM(param, lineAddr);
	LOAD_PARAM(param, leftEdges);
	LOAD_PARAM(param, rightEdges);
	LOAD_PARAM(param, xStarts);
	LOAD_PARAM(param, yStarts);

	if (edge == MLIB_EDGE_SRC_EXTEND)
		ws->y_move += (1 << (y_shift - 1));

	if ((ws->max_xsize) > 0) {
/* RTC */
		yStarts[(ws->yFinish) + 1] = 0;

		res = fun_nw(param->dstData, lineAddr,
		    leftEdges, rightEdges, xStarts, yStarts, ws, tbl);

		if (res != MLIB_SUCCESS) {
			if (param->buff_malloc != NULL)
				__mlib_free(param->buff_malloc);
			return (res);
		}
	}

/* process edge pixels */

	if (edge != MLIB_EDGE_DST_NO_WRITE && edge != MLIB_EDGE_SRC_PADDED) {
		mlib_affine_param param_e[1];

		param_e->buff_malloc = NULL;

		if (edge == MLIB_EDGE_DST_FILL_ZERO ||
		    edge == MLIB_EDGE_OP_NEAREST) {
			x_shift = 16;
			y_shift = 16;
		}

		STORE_PARAM(param_e, lineAddr);

		if (edge != MLIB_EDGE_SRC_EXTEND_INDEF) {
			res = mlib_AffineEdges(param_e, dst, src, NULL, 0,
			    kw, kh, kw1, kh1, -1, mtx, x_shift, y_shift);
		}

		if (res == MLIB_SUCCESS)
			switch (edge) {
			case MLIB_EDGE_DST_FILL_ZERO:
				mlib_ImageZoomTranslateTableBlendEdgeZero
				    (param, param_e, dchan, schan,
				    ws->alpha_shift, blend);
				break;

			case MLIB_EDGE_OP_NEAREST:
				mlib_ImageZoomTranslateTableBlendEdgeNearest
				    (param, param_e, dchan, schan,
				    ws->alpha_shift, blend);
				break;

			case MLIB_EDGE_SRC_EXTEND:
			case MLIB_EDGE_SRC_EXTEND_INDEF:

				ws->x_shift = x_shift;
				ws->x_move += (1 << (x_shift - 1));

				if (edge == MLIB_EDGE_SRC_EXTEND) {
					ws->yStart = param_e->yStart;
					ws->yFinish = param_e->yFinish;
					yStart = ws->yStart;
					ws->dx = param_e->dX;
					xLeft_e = param_e->leftEdges[yStart];
					xRight_e = param_e->rightEdges[yStart];
					cur_param = param_e;

				} else {
					cur_param = param;
					ws->yStart = param->yStart;
					yStart = ws->yStart;
					xLeft_e = 0;
					xRight_e = dstWidth - 1;
				}

				xLeft = param->leftEdges[param->yStart];
				xRight = param->rightEdges[param->yStart];

				if ((xLeft > xRight) ||
				    (param->yStart > param->yFinish)) {
					xLeft = xRight_e + 1;
					xRight = xRight_e;
				}

				if (((xRight_e - xLeft_e + 1) > 0) &&
				    (ws->yStart <= ws->yFinish)) {
					CREATE_X_IND();

					LOAD_PARAM(cur_param, lineAddr);
					LOAD_PARAM(cur_param, leftEdges);
					LOAD_PARAM(cur_param, xStarts);
					LOAD_PARAM(cur_param, yStarts);

/* RTC */
					yStarts[(ws->yFinish) + 1] = 0;

					if (edge == MLIB_EDGE_SRC_EXTEND) {
						CREATE_X_EXT()
					}

					ws->max_xsize = xLeft - xLeft_e;

					if ((ws->max_xsize) > 0) {
						if (edge !=
						    MLIB_EDGE_SRC_EXTEND) {
							CREATE_X(xLeft_e,
							    xLeft - 1);
						}

						leftEdges[0] = ws->max_xsize;
						leftEdges[1] = xLeft_e;

						res = FUNCNAME_EXT
						    (cur_param->dstData,
						    lineAddr, x_ind, leftEdges,
						    x_tab, xStarts, yStarts, ws,
						    tbl);
					}

					ws->max_xsize = xRight_e - xRight;

					if ((ws->max_xsize) > 0) {
						mlib_s32 shift = 0;

						if (edge !=
						    MLIB_EDGE_SRC_EXTEND) {
							CREATE_X(xRight + 1,
							    xRight_e);
						} else {
							shift =
							    xRight + 1 -
							    xLeft_e;
						}

						leftEdges[0] = ws->max_xsize;
						leftEdges[1] = xRight + 1;

						res = FUNCNAME_EXT
						    (cur_param->dstData,
						    lineAddr, x_ind, leftEdges,
						    x_tab + shift, xStarts,
						    yStarts, ws, tbl);
					}

					__mlib_free(p_x_ind);

					if (x_tab != NULL)
						__mlib_free(x_tab);
				}
				break;
			default:
				res = MLIB_FAILURE;
				break;
			}

		if (param_e->buff_malloc != NULL)
			__mlib_free(param_e->buff_malloc);
	}

	if (param->buff_malloc != NULL)
		__mlib_free(param->buff_malloc);

	return (res);
}

/* *********************************************************** */
