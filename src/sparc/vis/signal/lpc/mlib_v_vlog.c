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

#pragma ident	"@(#)mlib_v_vlog.c	9.2	07/11/05 SMI"

#include <mlib_types.h>
#include <mlib_SignalCepstral.h>
#include <vis_proto.h>

/* *********************************************************** */

static const mlib_d64 log_full[] = {
/* LOG (1 +   0/128) */
	0.0000000000000000000000000,
/* LOG (1 +   1/128) */
	0.0077821404420549489602821,
/* LOG (1 +   2/128) */
	0.0155041865359652544786861,
/* LOG (1 +   3/128) */
	0.0231670592815343794057537,
/* LOG (1 +   4/128) */
	0.0307716586667536873278550,
/* LOG (1 +   5/128) */
	0.0383188643021366015517515,
/* LOG (1 +   6/128) */
	0.0458095360312942012637194,
/* LOG (1 +   7/128) */
	0.0532445145188122845314460,
/* LOG (1 +   8/128) */
	0.0606246218164348399382035,
/* LOG (1 +   9/128) */
	0.0679506619085077506747794,
/* LOG (1 +  10/128) */
	0.0752234212375875316292095,
/* LOG (1 +  11/128) */
	0.0824436692110745855677223,
/* LOG (1 +  12/128) */
	0.0896121586896871380467644,
/* LOG (1 +  13/128) */
	0.0967296264585511128553108,
/* LOG (1 +  14/128) */
	0.1037967936816435593483376,
/* LOG (1 +  15/128) */
	0.1108143663402901130110578,
/* LOG (1 +  16/128) */
	0.1177830356563834557359627,
/* LOG (1 +  17/128) */
	0.1247034785009572405156675,
/* LOG (1 +  18/128) */
	0.1315763577887192614657152,
/* LOG (1 +  19/128) */
	0.1384023228591191312375486,
/* LOG (1 +  20/128) */
	0.1451820098444978890395163,
/* LOG (1 +  21/128) */
	0.1519160420258419685879403,
/* LOG (1 +  22/128) */
	0.1586050301766385728363673,
/* LOG (1 +  23/128) */
	0.1652495728953071729705471,
/* LOG (1 +  24/128) */
	0.1718502569266592283625528,
/* LOG (1 +  25/128) */
	0.1784076574728183095519540,
/* LOG (1 +  26/128) */
	0.1849223384940119896402422,
/* LOG (1 +  27/128) */
	0.1913948529996294667387957,
/* LOG (1 +  28/128) */
	0.1978257433299198675413777,
/* LOG (1 +  29/128) */
	0.2042155414286908887699923,
/* LOG (1 +  30/128) */
	0.2105647691073496419189581,
/* LOG (1 +  31/128) */
	0.2168739383006143550680633,
/* LOG (1 +  32/128) */
	0.2231435513142097648575657,
/* LOG (1 +  33/128) */
	0.2293741010648458200638089,
/* LOG (1 +  34/128) */
	0.2355660713127669114719254,
/* LOG (1 +  35/128) */
	0.2417199368871451592433175,
/* LOG (1 +  36/128) */
	0.2478361639045812692128123,
/* LOG (1 +  37/128) */
	0.2539152099809634521854207,
/* LOG (1 +  38/128) */
	0.2599575244369260462740101,
/* LOG (1 +  39/128) */
	0.2659635484971379359997457,
/* LOG (1 +  40/128) */
	0.2719337154836417580483499,
/* LOG (1 +  41/128) */
	0.2778684510034563071023683,
/* LOG (1 +  42/128) */
	0.2837681731306446186735570,
/* LOG (1 +  43/128) */
	0.2896332925830426563429398,
/* LOG (1 +  44/128) */
	0.2954642128938358980327905,
/* LOG (1 +  45/128) */
	0.3012613305781617900613867,
/* LOG (1 +  46/128) */
	0.3070250352949118743950407,
/* LOG (1 +  47/128) */
	0.3127557100038969029043301,
/* LOG (1 +  48/128) */
	0.3184537311185345886954678,
/* LOG (1 +  49/128) */
	0.3241194686542119840488851,
/* LOG (1 +  50/128) */
	0.3297532863724679796924022,
/* LOG (1 +  51/128) */
	0.3353555419211378119115352,
/* LOG (1 +  52/128) */
	0.3409265869705931928379528,
/* LOG (1 +  53/128) */
	0.3464667673462085706326263,
/* LOG (1 +  54/128) */
	0.3519764231571781976093405,
/* LOG (1 +  55/128) */
	0.3574558889218037993629196,
/* LOG (1 +  56/128) */
	0.3629054936893684746301858,
/* LOG (1 +  57/128) */
	0.3683255611587076261415064,
/* LOG (1 +  58/128) */
	0.3737164097935840589848056,
/* LOG (1 +  59/128) */
	0.3790783529349694425114592,
/* LOG (1 +  60/128) */
	0.3844116989103320558562871,
/* LOG (1 +  61/128) */
	0.3897167511400251860287369,
/* LOG (1 +  62/128) */
	0.3949938082408689932201185,
/* LOG (1 +  63/128) */
	0.4002431641270127182785643,
/* LOG (1 +  64/128) */
	0.4054651081081643848591511,
/* LOG (1 +  65/128) */
	0.4106599249852683763961636,
/* LOG (1 +  66/128) */
	0.4158278951437109904887279,
/* LOG (1 +  67/128) */
	0.4209692946441296323989434,
/* LOG (1 +  68/128) */
	0.4260843953109000881163126,
/* LOG (1 +  69/128) */
	0.4311734648183713214386614,
/* LOG (1 +  70/128) */
	0.4362367667749180721870061,
/* LOG (1 +  71/128) */
	0.4412745608048752044005880,
/* LOG (1 +  72/128) */
	0.4462871026284195297151314,
/* LOG (1 +  73/128) */
	0.4512746441394585583672949,
/* LOG (1 +  74/128) */
	0.4562374334815875731585777,
/* LOG (1 +  75/128) */
	0.4611757151221701489518523,
/* LOG (1 +  76/128) */
	0.4660897299245992386751425,
/* LOG (1 +  77/128) */
	0.4709797152187910063148024,
/* LOG (1 +  78/128) */
	0.4758459048699639204471623,
/* LOG (1 +  79/128) */
	0.4806885293457519026105729,
/* LOG (1 +  80/128) */
	0.4855078157817008244201418,
/* LOG (1 +  81/128) */
	0.4903039880451938170580206,
/* LOG (1 +  82/128) */
	0.4950772667978515229059155,
/* LOG (1 +  83/128) */
	0.4998278695564493112613036,
/* LOG (1 +  84/128) */
	0.5045560107523953119468274,
/* LOG (1 +  85/128) */
	0.5092619017898079025741254,
/* LOG (1 +  86/128) */
	0.5139457511022342828255205,
/* LOG (1 +  87/128) */
	0.5186077642080456628903562,
/* LOG (1 +  88/128) */
	0.5232481437645478683506894,
/* LOG (1 +  89/128) */
	0.5278670896208423890527683,
/* LOG (1 +  90/128) */
	0.5324647988694718447888476,
/* LOG (1 +  91/128) */
	0.5370414658968836185692908,
/* LOG (1 +  92/128) */
	0.5415972824327444090641848,
/* LOG (1 +  93/128) */
	0.5461324375981356782361331,
/* LOG (1 +  94/128) */
	0.5506471179526623016542430,
/* LOG (1 +  95/128) */
	0.5551415075405016219534104,
/* LOG (1 +  96/128) */
	0.5596157879354226594159627,
/* LOG (1 +  97/128) */
	0.5640701382848030132066697,
/* LOG (1 +  98/128) */
	0.5685047353526687663460848,
/* LOG (1 +  99/128) */
	0.5729197535617854830647389,
/* LOG (1 + 100/128) */
	0.5773153650348236132217039,
/* LOG (1 + 101/128) */
	0.5816917396346225066494640,
/* LOG (1 + 102/128) */
	0.5860490450035782394877515,
/* LOG (1 + 103/128) */
	0.5903874466021763467438177,
/* LOG (1 + 104/128) */
	0.5947071077466927757626536,
/* LOG (1 + 105/128) */
	0.5990081896460833776885124,
/* LOG (1 + 106/128) */
	0.6032908514380842524005288,
/* LOG (1 + 107/128) */
	0.6075552502245418207138528,
/* LOG (1 + 108/128) */
	0.6118015411059929409276492,
/* LOG (1 + 109/128) */
	0.6160298772155140545336849,
/* LOG (1 + 110/128) */
	0.6202404097518575687431053,
/* LOG (1 + 111/128) */
	0.6244332880118934614444015,
/* LOG (1 + 112/128) */
	0.6286086594223740942055656,
/* LOG (1 + 113/128) */
	0.6327666695710377764427790,
/* LOG (1 + 114/128) */
	0.6369074622370691773909357,
/* LOG (1 + 115/128) */
	0.6410311794209312408199253,
/* LOG (1 + 116/128) */
	0.6451379613735847007305324,
/* LOG (1 + 117/128) */
	0.6492279466251098529738783,
/* LOG (1 + 118/128) */
	0.6533012720127456818275391,
/* LOG (1 + 119/128) */
	0.6573580727083599972715433,
/* LOG (1 + 120/128) */
	0.6613984822453650158635696,
/* LOG (1 + 121/128) */
	0.6654226325450904866443125,
/* LOG (1 + 122/128) */
	0.6694306539426292390615458,
/* LOG (1 + 123/128) */
	0.6734226752121666992323412,
/* LOG (1 + 124/128) */
	0.6773988235918061429075010,
/* LOG (1 + 125/128) */
	0.6813592248079031188368049,
/* LOG (1 + 126/128) */
	0.6853040030989193676091986,
/* LOG (1 + 127/128) */
	0.6892332812388090035327082,
};

/* *********************************************************** */

/*
 * Define constant for LOG function
 */
#define	K2	4.00006488775094945494e-01
#define	K1	6.66666666629825839191e-01

#define	KLN2	0.69314718055994528623

#pragma align 8(mask_arr)
static const mlib_s32 mask_arr[] = {
	0, 0,
	-1, -1
};

/* *********************************************************** */

void
mlib_vlog(
    mlib_d64 *dst,
    const mlib_d64 *src,
    mlib_s32 n)
{
	mlib_s32 exp, i, tmp;
	mlib_d64 y0, ftmp0;
	mlib_d64 yd, ydv2, arg, res, dtmp, dtmp0, res0;
	mlib_d64 mask_7mant = vis_to_double(0x000fe000, 0x00000000);
	mlib_d64 mask_mant = vis_to_double(0x000fffff, 0xffffffff);
	mlib_d64 mask_one = vis_to_double(0x3ff00000, 0x00000000);

/* -Inf */
	mlib_d64 mInf = vis_to_double(0xfff00000, 0x00000000);
	mlib_s32 *isrc = (void *)src;
	mlib_d64 yd2;
	mlib_d64 *arr = (void *)mask_arr;

#pragma pipeloop(0)
	for (i = 0; i < n; i++) {

		dtmp = (*src++);
		tmp = *isrc;
		isrc += 2;
		exp = (tmp >> 20) - 1023;

		dtmp0 = arr[((mlib_u32)(-tmp)) >> 31];
		ftmp0 = log_full[(tmp >> 13) & 0x7f] + exp * KLN2;

		y0 = vis_for(vis_fand(dtmp, mask_7mant), mask_one);
		arg = vis_for(vis_fand(dtmp, mask_mant), mask_one);
		res0 = vis_fandnot(dtmp0, mInf);

		yd = (arg - y0) / (arg + y0);
		ydv2 = yd * yd;
		yd2 = yd + yd;
		res = (yd2 + K2 * yd * ydv2 * (ydv2 + K1 / K2) + ftmp0);
		(*dst++) = vis_for(res0, vis_fand(dtmp0, res));
	}
}

/* *********************************************************** */
