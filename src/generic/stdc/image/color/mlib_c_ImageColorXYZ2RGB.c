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

#pragma ident	"@(#)mlib_c_ImageColorXYZ2RGB.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageColorXYZ2RGB - color conversion from XYZ to RGB space
 *
 * SYNOPSIS
 *      mlib_status mlib_ImageColorXYZ2RGB(mlib_image       *dst,
 *                                         const mlib_image *src);
 *
 * ARGUMENT
 *      dst     pointer to output image
 *      src     pointer to input image
 *
 * RESTRICTION
 *      src and dst must be the same type, the same size and the same
 *      number of channels.
 *      They must be 3-channel images.
 *      They can be in MLIB_BYTE, MLIB_SHORT, MLIB_USHORT or MLIB_INT data type.
 *
 * DESCRIPTION
 *
 *        |R|   |fmat[0] fmat[1] fmat[2]|   |X|
 *        |G| = |fmat[3] fmat[4] fmat[5]| * |Y|
 *        |B|   |fmat[6] fmat[7] fmat[8]|   |Z|
 *
 *   where
 *
 *      fmat[] = { 3.240479, -1.537150, -0.498535,
 *                -0.969256,  1.875992,  0.041566,
 *                 0.055648, -0.204043,  1.057311};
 */

#include <mlib_image.h>
#include <mlib_ImageCheck.h>
#include <mlib_c_ImageColorTables.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_ImageColorXYZ2RGB = __mlib_ImageColorXYZ2RGB

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_ImageColorXYZ2RGB) mlib_ImageColorXYZ2RGB
    __attribute__((weak, alias("__mlib_ImageColorXYZ2RGB")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

static const mlib_s32 t_x2rgb[256] = {
	262144, 3406848, 6551552, 10744832, 13889536, 17034240, 20178944,
	    24372224,
	27516928, 30661633, 33806337, 37999617, 41144321, 44289025, 47433729,
	    51627009,
	54771713, 57917441, 61062145, 65255425, 68400129, 71544833, 74689537,
	    78882817,
	82027521, 85172225, 88316929, 91461634, 95654914, 98799618, 101944322,
	    105089026,
	109282306, 112427010, 115571714, 118716418, 122909698, 126054402,
	    129199106, 132343810,
	136537090, 139681794, 142826498, 145971202, 150164482, 153309187,
	    156453891, 159598595,
	163791875, 166937603, 170082307, 173227011, 177420291, 180564995,
	    183709699, 186854403,
	189999107, 194192387, 197337091, 200481795, 203626499, 207819779,
	    210964483, 214109188,
	217253892, 221447172, 224591876, 227736580, 230881284, 235074564,
	    238219268, 241363972,
	244508676, 248701956, 251846660, 254991364, 258136068, 262329348,
	    265474052, 268618756,
	271763460, 274908165, 279102469, 282247173, 285391877, 288536581,
	    292729861, 295874565,
	299019269, 302163973, 306357253, 309501957, 312646661, 315791365,
	    319984645, 323129349,
	326274053, 329418757, 333612037, 336756742, 339901446, 343046150,
	    347239430, 350384134,
	353528838, 356673542, 359818246, 364011526, 367156230, 370300934,
	    373445638, 377638918,
	380783622, 383928326, 387074054, 391267334, 394412038, 397556743,
	    400701447, 404894727,
	408039431, 411184135, 414328839, 418522119, 421666823, 424811527,
	    427956231, 432149511,
	435294215, 438438919, 441583623, 445776903, 448921607, 452066311,
	    455211015, 458355720,
	462549000, 465693704, 468838408, 471983112, 476176392, 479321096,
	    482465800, 485610504,
	489803784, 492948488, 496093192, 499238920, 503432200, 506576904,
	    509721608, 512866312,
	517059592, 520204297, 523349001, 526493705, 530686985, 533831689,
	    536976393, 540121097,
	543265801, 547459081, 550603785, 553748489, 556893193, 561086473,
	    564231177, 567375881,
	570520585, 574713865, 577858569, 581003274, 584147978, 588341258,
	    591485962, 594630666,
	597775370, 601968650, 605113354, 608259082, 611403786, 615597066,
	    618741770, 621886474,
	625031178, 628175882, 632369162, 635513866, 638658570, 641803275,
	    645996555, 649141259,
	652285963, 655430667, 659623947, 662768651, 665913355, 669058059,
	    673251339, 676396043,
	679540747, 682685451, 686878731, 690023435, 693168139, 696312843,
	    700506123, 703650828,
	706795532, 709940236, 714133516, 717278220, 720423948, 723568652,
	    726713356, 730906636,
	734051340, 737196044, 740340748, 744534028, 747678732, 750823436,
	    753968140, 758161420,
	761306124, 764450829, 767595533, 771788813, 774933517, 778078221,
	    781222925, 785416205,
	788560909, 791705613, 794850317, 799043597, 802188301, 805333005,
	    808477709, 811622413,
	815815693, 818960397, 822105101, 825249806, 829444110, 832588814,
	    835733518, 838878222,
	843071502, 846216206, 849360910, 852505614, 856698894, 859843598,
	    862988302, 866133006
};

/* *********************************************************** */

static const mlib_s32 t_y2rgb[256] = {
	256, -2094848, -3141376, -5236481, -6283009, -8379137, -9425665,
	    -11520769,
	-12567298, -14662402, -15708930, -17804034, -18850562, -20946691,
	    -23041795, -24088323,
	-26183427, -27229955, -29325060, -30371588, -32466692, -33514244,
	    -35609348, -36655877,
	-38750981, -39797509, -41892613, -43987718, -45034246, -47130374,
	    -48176902, -50272006,
	-51318535, -53413639, -54460167, -56555271, -57601799, -59697928,
	    -60744456, -62839560,
	-63886088, -65981192, -68076297, -69122825, -71217929, -72265481,
	    -74360585, -75407114,
	-77502218, -78548746, -80643850, -81690378, -83785483, -84833035,
	    -86928139, -89023243,
	-90069771, -92164876, -93211404, -95306508, -96353036, -98449164,
	    -99495693, -101590797,
	-102637325, -104732429, -105778957, -107874062, -109969166, -111016718,
	    -113111822, -114158350,
	-116253455, -117299983, -119395087, -120441615, -122536720, -123584272,
	    -125679376, -126725904,
	-128821008, -130916113, -131962641, -134057745, -135104273, -137200401,
	    -138246930, -140342034,
	-141388562, -143483666, -144530194, -146625299, -147671827, -149767955,
	    -150814483, -152909587,
	-155004692, -156051220, -158146324, -159192852, -161287956, -162335509,
	    -164430613, -165477141,
	-167572245, -168618773, -170713878, -171760406, -173855510, -175951638,
	    -176998166, -179093271,
	-180139799, -182234903, -183281431, -185376535, -186423064, -188519192,
	    -189565720, -191660824,
	-192707352, -194802457, -196897561, -197944089, -200039193, -201086746,
	    -203181850, -204228378,
	-206323482, -207370010, -209465115, -210511643, -212606747, -213653275,
	    -215749403, -217844508,
	-218891036, -220986140, -222032668, -224127772, -225174301, -227269405,
	    -228316957, -230412061,
	-231458589, -233553694, -234600222, -236695326, -237741854, -239836958,
	    -241933087, -242979615,
	-245074719, -246121247, -248216351, -249262880, -251357984, -252404512,
	    -254500640, -255547168,
	-257642273, -258688801, -260783905, -262879009, -263925537, -266020642,
	    -267068194, -269163298,
	-270209826, -272304930, -273351459, -275446563, -276493091, -278588195,
	    -279635748, -281730852,
	-283825956, -284872484, -286967588, -288014117, -290109221, -291155749,
	    -293251877, -294298405,
	-296393510, -297440038, -299535142, -300581670, -302676774, -304771879,
	    -305819431, -307914535,
	-308961063, -311056167, -312102696, -314197800, -315244328, -317339432,
	    -318386984, -320482089,
	-321528617, -323623721, -325718825, -326765353, -328860458, -329906986,
	    -332003114, -333049642,
	-335144746, -336191275, -338286379, -339332907, -341428011, -342474539,
	    -344570668, -345617196,
	-347712300, -349807404, -350853932, -352949037, -353995565, -356090669,
	    -357138221, -359233326,
	-360279854, -362374958, -363421486, -365516590, -366563119, -368658223,
	    -370754351, -371800879,
	-373895983, -374942512, -377037616, -378084144, -380179248, -381225776,
	    -383321905, -384368433,
	-386463537, -387510065, -389605169, -391700274, -392746802, -394841906,
	    -395889458, -397984562,
	-399031091, -401126195, -402172723, -404267827, -405314355, -407409460,
	    -408455988, -410552116
};

/* *********************************************************** */

static const mlib_s32 t_z2rgb[256] = {
	0, 1, -1048574, -1048573, -2097148, -2097147, -3145722, -3145721,
	-4194296, -4194294, -5242869, -5242868, -6291443, -6290418, -7338993,
	    -7338992,
	-8387567, -8387566, -9436141, -9436140, -10484715, -10484714, -11533289,
	    -11533288,
	-12581863, -12581862, -13630437, -13630435, -14679010, -14679009,
	    -15727584, -15727583,
	-16776158, -16776157, -17824732, -17824731, -18873306, -18872281,
	    -19920856, -19920855,
	-20969430, -20969429, -22018004, -22018003, -23066577, -23066576,
	    -24115151, -24115150,
	-25163725, -25163724, -26212299, -26212298, -27260873, -27260872,
	    -28309447, -28309446,
	-29358021, -29358020, -30406595, -30406594, -31455169, -31454144,
	    -32502718, -32502717,
	-33551292, -33551291, -34599866, -34599865, -35648440, -35648439,
	    -36697014, -36697013,
	-37745588, -37745587, -38794162, -38794161, -39842736, -39842735,
	    -40891310, -40891308,
	-41939883, -41939882, -42988457, -42988456, -44037031, -44036006,
	    -45084581, -45084580,
	-46133155, -46133154, -47181729, -47181728, -48230303, -48230302,
	    -49278877, -49278876,
	-50327450, -50327449, -51376024, -51376023, -52424598, -52424597,
	    -53473172, -53473171,
	-54521746, -54521745, -55570320, -55570319, -56618894, -56617869,
	    -57666444, -57666443,
	-58715018, -58715017, -59763591, -59763590, -60812165, -60812164,
	    -61860739, -61860738,
	-62909313, -62909312, -63957887, -63957886, -65006461, -65006460,
	    -66055035, -66055034,
	-67103609, -67103608, -68152183, -68152181, -69200756, -69199731,
	    -70248306, -70248305,
	-71296880, -71296879, -72345454, -72345453, -73394028, -73394027,
	    -74442602, -74442601,
	-75491176, -75491175, -76539750, -76539749, -77588324, -77588322,
	    -78636897, -78636896,
	-79685471, -79685470, -80734045, -80734044, -81782619, -81781594,
	    -82830169, -82830168,
	-83878743, -83878742, -84927317, -84927316, -85975891, -85975890,
	    -87024464, -87024463,
	-88073038, -88073037, -89121612, -89121611, -90170186, -90170185,
	    -91218760, -91218759,
	-92267334, -92267333, -93315908, -93315907, -94364482, -94363457,
	    -95412032, -95412031,
	-96460605, -96460604, -97509179, -97509178, -98557753, -98557752,
	    -99606327, -99606326,
	-100654901, -100654900, -101703475, -101703474, -102752049, -102752048,
	    -103800623, -103800622,
	-104849197, -104849195, -105897770, -105897769, -106946344, -106945319,
	    -107993894, -107993893,
	-109042468, -109042467, -110091042, -110091041, -111139616, -111139615,
	    -112188190, -112188189,
	-113236764, -113236763, -114285338, -114285336, -115333911, -115333910,
	    -116382485, -116382484,
	-117431059, -117431058, -118479633, -118479632, -119528207, -119527182,
	    -120575757, -120575756,
	-121624331, -121624330, -122672905, -122672904, -123721478, -123721477,
	    -124770052, -124770051,
	-125818626, -125818625, -126867200, -126867199, -127915774, -127915773,
	    -128964348, -128964347,
	-130012922, -130012921, -131061496, -131061495, -132110070, -132109045,
	    -133157619, -133157618
};

/* *********************************************************** */

#define	TCLAMP_U8(dst, s)	dst = mlib_pClipOff256[s]

/* *********************************************************** */

#define	SCLAMP_U8(dst, s)                                       \
	{                                                       \
	    mlib_s32 v = s, mask = (v - 0xff00000) >> 31;       \
	                                                        \
	    dst = (((mlib_u32)v >> 20) | ~mask) & ~(v >> 31);   \
	}

/* *********************************************************** */

#ifdef _LITTLE_ENDIAN

#define	SPLIT_S32_U8(x, a0, a1, a2, a3)                         \
	a3 = (x >> 24);                                         \
	a2 = (x & 0xff0000) >> 16;                              \
	a1 = (x & 0x00ff00) >> 8;                               \
	a0 = (x & 0x0000ff)

#else /* _LITTLE_ENDIAN */

#define	SPLIT_S32_U8(x, a0, a1, a2, a3)                         \
	a0 = (x >> 24);                                         \
	a1 = (x & 0xff0000) >> 16;                              \
	a2 = (x & 0x00ff00) >> 8;                               \
	a3 = (x & 0x0000ff)

#endif /* _LITTLE_ENDIAN */

/* *********************************************************** */

#define	MLIB_CONVERT_U8_1(pd, ps)                               \
	{                                                       \
	    mlib_u32 x0, y0, z0;                                \
	    mlib_u32 s0;                                        \
	                                                        \
	    x0 = ps[0];                                         \
	    y0 = ps[1];                                         \
	    z0 = ps[2];                                         \
	    s0 = t_x2rgb[x0] + t_y2rgb[y0] + t_z2rgb[z0];       \
	    SCLAMP_U8(pd[0], s0);                               \
	    TCLAMP_U8(pd[1], (s0 >> 10) & 0x3ff);               \
	    TCLAMP_U8(pd[2], s0 & 0x3ff);                       \
	}

/* *********************************************************** */

mlib_status
__mlib_ImageColorXYZ2RGB(
    mlib_image *dst,
    const mlib_image *src)
{
/* CIE XYZ to Rec709 RGB with D64 White Point */
	mlib_d64 fmat[9] = { 3.240479, -1.537150, -0.498535,
		-0.969256, 1.875992, 0.041566,
		0.055648, -0.204043, 1.057311
	};
	mlib_s32 slb, dlb, xsize, ysize;
	mlib_type dtype;
	mlib_u8 *psrc, *pdst;
	mlib_s32 j;

	MLIB_IMAGE_CHECK(dst);
	MLIB_IMAGE_CHECK(src);
	MLIB_IMAGE_FULL_EQUAL(dst, src);

	dtype = mlib_ImageGetType(dst);
	xsize = mlib_ImageGetWidth(dst);
	ysize = mlib_ImageGetHeight(dst);
	dlb = mlib_ImageGetStride(dst);
	pdst = (void *)mlib_ImageGetData(dst);

	slb = mlib_ImageGetStride(src);
	psrc = mlib_ImageGetData(src);

	if (dtype == MLIB_BYTE) {

		for (j = 0; j < ysize; j++) {
			mlib_u8 *ps = psrc,
			    *pd = pdst, *pend = pdst + 3 * xsize;

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
			for (; pd < pend; pd += 3) {
				MLIB_CONVERT_U8_1(pd, ps);
				ps += 3;
			}

			psrc += slb;
			pdst += dlb;
		}

		return (MLIB_SUCCESS);

	} else {

		return (__mlib_ImageColorConvert1(dst, src, fmat));
	}
}

/* *********************************************************** */
