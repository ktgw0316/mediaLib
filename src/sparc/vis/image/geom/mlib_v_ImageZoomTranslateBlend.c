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

#pragma ident	"@(#)mlib_v_ImageZoomTranslateBlend.c	9.2	07/11/05 SMI"

/*
 * FUNCTION
 *      mlib_ImageZoomTranslateBlend(), internal pixels.
 *
 */

#include <mlib_image.h>
#include <mlib_SysMath.h>
#include <mlib_ImageZoom.h>
#include <mlib_ImageCheck.h>
#include <mlib_v_ImageFilters.h>

/* *********************************************************** */

const mlib_u64 mlib_v_tbl_255DivAlpha[] = {
	0x0000000000000000, 0x7fff7fff7fff7fff,
	0x7f807f807f807f80, 0x5500550055005500,
	0x3fc03fc03fc03fc0, 0x3300330033003300,
	0x2a802a802a802a80, 0x246e246e246e246e,
	0x1fe01fe01fe01fe0, 0x1c551c551c551c55,
	0x1980198019801980, 0x172f172f172f172f,
	0x1540154015401540, 0x139e139e139e139e,
	0x1237123712371237, 0x1100110011001100,
	0x0ff00ff00ff00ff0, 0x0f000f000f000f00,
	0x0e2b0e2b0e2b0e2b, 0x0d6c0d6c0d6c0d6c,
	0x0cc00cc00cc00cc0, 0x0c250c250c250c25,
	0x0b970b970b970b97, 0x0b160b160b160b16,
	0x0aa00aa00aa00aa0, 0x0a330a330a330a33,
	0x09cf09cf09cf09cf, 0x0972097209720972,
	0x091b091b091b091b, 0x08cb08cb08cb08cb,
	0x0880088008800880, 0x083a083a083a083a,
	0x07f807f807f807f8, 0x07ba07ba07ba07ba,
	0x0780078007800780, 0x0749074907490749,
	0x0715071507150715, 0x06e406e406e406e4,
	0x06b606b606b606b6, 0x068a068a068a068a,
	0x0660066006600660, 0x0638063806380638,
	0x0612061206120612, 0x05ee05ee05ee05ee,
	0x05cc05cc05cc05cc, 0x05ab05ab05ab05ab,
	0x058b058b058b058b, 0x056d056d056d056d,
	0x0550055005500550, 0x0534053405340534,
	0x051a051a051a051a, 0x0500050005000500,
	0x04e704e704e704e7, 0x04d004d004d004d0,
	0x04b904b904b904b9, 0x04a304a304a304a3,
	0x048e048e048e048e, 0x0479047904790479,
	0x0466046604660466, 0x0452045204520452,
	0x0440044004400440, 0x042e042e042e042e,
	0x041d041d041d041d, 0x040c040c040c040c,
	0x03fc03fc03fc03fc, 0x03ec03ec03ec03ec,
	0x03dd03dd03dd03dd, 0x03ce03ce03ce03ce,
	0x03c003c003c003c0, 0x03b203b203b203b2,
	0x03a503a503a503a5, 0x0397039703970397,
	0x038b038b038b038b, 0x037e037e037e037e,
	0x0372037203720372, 0x0366036603660366,
	0x035b035b035b035b, 0x0350035003500350,
	0x0345034503450345, 0x033a033a033a033a,
	0x0330033003300330, 0x0326032603260326,
	0x031c031c031c031c, 0x0313031303130313,
	0x0309030903090309, 0x0300030003000300,
	0x02f702f702f702f7, 0x02ee02ee02ee02ee,
	0x02e602e602e602e6, 0x02dd02dd02dd02dd,
	0x02d502d502d502d5, 0x02cd02cd02cd02cd,
	0x02c602c602c602c6, 0x02be02be02be02be,
	0x02b602b602b602b6, 0x02af02af02af02af,
	0x02a802a802a802a8, 0x02a102a102a102a1,
	0x029a029a029a029a, 0x0293029302930293,
	0x028d028d028d028d, 0x0286028602860286,
	0x0280028002800280, 0x027a027a027a027a,
	0x0274027402740274, 0x026e026e026e026e,
	0x0268026802680268, 0x0262026202620262,
	0x025c025c025c025c, 0x0257025702570257,
	0x0251025102510251, 0x024c024c024c024c,
	0x0247024702470247, 0x0242024202420242,
	0x023d023d023d023d, 0x0238023802380238,
	0x0233023302330233, 0x022e022e022e022e,
	0x0229022902290229, 0x0225022502250225,
	0x0220022002200220, 0x021c021c021c021c,
	0x0217021702170217, 0x0213021302130213,
	0x020e020e020e020e, 0x020a020a020a020a,
	0x0206020602060206, 0x0202020202020202,
	0x01fe01fe01fe01fe, 0x01fa01fa01fa01fa,
	0x01f601f601f601f6, 0x01f201f201f201f2,
	0x01ef01ef01ef01ef, 0x01eb01eb01eb01eb,
	0x01e701e701e701e7, 0x01e401e401e401e4,
	0x01e001e001e001e0, 0x01dc01dc01dc01dc,
	0x01d901d901d901d9, 0x01d601d601d601d6,
	0x01d201d201d201d2, 0x01cf01cf01cf01cf,
	0x01cc01cc01cc01cc, 0x01c901c901c901c9,
	0x01c501c501c501c5, 0x01c201c201c201c2,
	0x01bf01bf01bf01bf, 0x01bc01bc01bc01bc,
	0x01b901b901b901b9, 0x01b601b601b601b6,
	0x01b301b301b301b3, 0x01b001b001b001b0,
	0x01ad01ad01ad01ad, 0x01ab01ab01ab01ab,
	0x01a801a801a801a8, 0x01a501a501a501a5,
	0x01a201a201a201a2, 0x01a001a001a001a0,
	0x019d019d019d019d, 0x019b019b019b019b,
	0x0198019801980198, 0x0195019501950195,
	0x0193019301930193, 0x0190019001900190,
	0x018e018e018e018e, 0x018c018c018c018c,
	0x0189018901890189, 0x0187018701870187,
	0x0185018501850185, 0x0182018201820182,
	0x0180018001800180, 0x017e017e017e017e,
	0x017c017c017c017c, 0x0179017901790179,
	0x0177017701770177, 0x0175017501750175,
	0x0173017301730173, 0x0171017101710171,
	0x016f016f016f016f, 0x016d016d016d016d,
	0x016b016b016b016b, 0x0169016901690169,
	0x0167016701670167, 0x0165016501650165,
	0x0163016301630163, 0x0161016101610161,
	0x015f015f015f015f, 0x015d015d015d015d,
	0x015b015b015b015b, 0x0159015901590159,
	0x0158015801580158, 0x0156015601560156,
	0x0154015401540154, 0x0152015201520152,
	0x0150015001500150, 0x014f014f014f014f,
	0x014d014d014d014d, 0x014b014b014b014b,
	0x014a014a014a014a, 0x0148014801480148,
	0x0146014601460146, 0x0145014501450145,
	0x0143014301430143, 0x0142014201420142,
	0x0140014001400140, 0x013e013e013e013e,
	0x013d013d013d013d, 0x013b013b013b013b,
	0x013a013a013a013a, 0x0138013801380138,
	0x0137013701370137, 0x0135013501350135,
	0x0134013401340134, 0x0132013201320132,
	0x0131013101310131, 0x0130013001300130,
	0x012e012e012e012e, 0x012d012d012d012d,
	0x012b012b012b012b, 0x012a012a012a012a,
	0x0129012901290129, 0x0127012701270127,
	0x0126012601260126, 0x0125012501250125,
	0x0123012301230123, 0x0122012201220122,
	0x0121012101210121, 0x0120012001200120,
	0x011e011e011e011e, 0x011d011d011d011d,
	0x011c011c011c011c, 0x011b011b011b011b,
	0x0119011901190119, 0x0118011801180118,
	0x0117011701170117, 0x0116011601160116,
	0x0115011501150115, 0x0113011301130113,
	0x0112011201120112, 0x0111011101110111,
	0x0110011001100110, 0x010f010f010f010f,
	0x010e010e010e010e, 0x010d010d010d010d,
	0x010c010c010c010c, 0x010a010a010a010a,
	0x0109010901090109, 0x0108010801080108,
	0x0107010701070107, 0x0106010601060106,
	0x0105010501050105, 0x0104010401040104,
	0x0103010301030103, 0x0102010201020102,
	0x0101010101010101, 0x0100010001000100
};

/* *********************************************************** */

static const mlib_zoomblend_fun_type mlib_zoom_nn_funs[] = {
	mlib_ImageZoom_U8_33_Nearest,
	mlib_ImageZoom_U8_43_Nearest,
	mlib_ImageZoom_U8_34_Nearest,
	mlib_ImageZoom_U8_44_Nearest,
};

/* *********************************************************** */

static const mlib_zoom_fun_type mlib_zoom_bl_funs[] = {
	mlib_v_ImageZoomBilinear_U8_3,
	mlib_v_ImageZoomBilinear_U8_4,
	mlib_v_ImageZoomBilinear_U8_3,
	mlib_v_ImageZoomBilinear_U8_4,
};

/* *********************************************************** */

static const mlib_zoom_fun_type mlib_zoom_bc_funs[] = {
	mlib_v_ImageZoomBicubic_U8_3,
	mlib_v_ImageZoomBicubic_U8_4,
	mlib_v_ImageZoomBicubic_U8_3,
	mlib_v_ImageZoomBicubic_U8_4,
};

/* *********************************************************** */

mlib_status
mlib_ImageZoomTranslateBlend_intern(
    mlib_work_image *param,
    mlib_filter filter,
    mlib_d64 zoomy,
    mlib_s32 t_ind)
{
	switch (filter) {
	case MLIB_NEAREST:
		return mlib_zoom_nn_funs[t_ind] (param, param->current->sp,
		    param->current->dp);

	case MLIB_BILINEAR:
		return (mlib_zoom_bl_funs[t_ind] (param));

	case MLIB_BICUBIC:
	case MLIB_BICUBIC2:
		if (filter == MLIB_BICUBIC) {
			param->filter1 = (void *)mlib_filters_u8_bc;
			param->filter3 = (void *)mlib_filters_u8_bc_3;
			param->filter4 = (void *)mlib_filters_u8_bc_4;
		} else {
			param->filter1 = (void *)mlib_filters_u8_bc2;
			param->filter3 = (void *)mlib_filters_u8_bc2_3;
			param->filter4 = (void *)mlib_filters_u8_bc2_4;
		}
		return (mlib_zoom_bc_funs[t_ind] (param));

	default:
		return (MLIB_FAILURE);
	}
}

/* *********************************************************** */
