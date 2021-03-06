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

#pragma ident   "@(#)mlib_s_SignalWinTab.c	9.3	07/11/05 SMI"

/*
 *
 *  DESCRIPTION
 *    Hanning     :   window[i] = 0.5*(1 - cos(2 * i * pi/(N -1)))
 *
 *    Hamming     :   window[i] = 0.54 - 0.46*cos(2 * i * pi/(N - 1))
 *
 *    Blackman    :   window[i] = (alpha + 1)/2 - 0.5*cos(2*pi*i/(N - 1)) -
 *			(alpha/2)*cos(4*pi*i/(N - 1))
 *			alpha = -0.16 (standard Blackman Window)
 *
 *    Kaiser      :   window[i] = I0(beta * sqrt(sqr((N - 1)/2) -
 *                        sqr(i - (N - 1)/2)))/I0(beta*((N - 1)/2))
 *                      beta = 0.5 (general situation)
 *
 *  COMMENTS
 *   This table is used by integer versions of the mlib_SignalMul...
 *   functions. All table's elements are in Q15 data type.
 */

#include <mlib_signal.h>
#include <mlib_s_SignalWinTab.h>

/* *********************************************************** */

const mlib_s16 mlib_s_Hanning_S16[] = {
/* n = 2 */
	0x0,
/* n = 4 */
	0x0, 0x5fff,
/* n = 8 */
	0x0, 0x1818, 0x4e3d, 0x79a9,
/* n = 16 */
	0x0, 0x588, 0x152c, 0x2c39,
	0x46b0, 0x6000, 0x73c6, 0x7e99,
/* n = 32 */
	0x0, 0x14f, 0x52f, 0xb78,
	0x13e7, 0x1e25, 0x29c5, 0x364f,
	0x433d, 0x500a, 0x5c2f, 0x672c,
	0x708f, 0x77f5, 0x7d10, 0x7fab,
/* n = 64 */
	0x0, 0x51, 0x144, 0x2d7,
	0x506, 0x7cb, 0xb1e, 0xef9,
	0x1350, 0x1818, 0x1d46, 0x22cd,
	0x289e, 0x2eaa, 0x34e2, 0x3b37,
	0x4198, 0x47f5, 0x4e3d, 0x5462,
	0x5a52, 0x6000, 0x655c, 0x6a59,
	0x6eea, 0x7304, 0x769c, 0x79a9,
	0x7c23, 0x7e05, 0x7f49, 0x7feb,
/* n = 128 */
	0x0, 0x14, 0x50, 0xb4,
	0x13f, 0x1f2, 0x2cc, 0x3cc,
	0x4f2, 0x63d, 0x7ac, 0x93e,
	0xaf3, 0xcc9, 0xebf, 0x10d4,
	0x1306, 0x1555, 0x17be, 0x1a40,
	0x1cdb, 0x1f8b, 0x224f, 0x2526,
	0x280e, 0x2b05, 0x2e09, 0x3119,
	0x3431, 0x3751, 0x3a77, 0x3da0,
	0x40ca, 0x43f4, 0x471c, 0x4a3f,
	0x4d5b, 0x506f, 0x5379, 0x5677,
	0x5967, 0x5c46, 0x5f14, 0x61cf,
	0x6474, 0x6703, 0x6979, 0x6bd5,
	0x6e16, 0x7039, 0x723f, 0x7425,
	0x75ea, 0x778e, 0x790f, 0x7a6c,
	0x7ba4, 0x7cb8, 0x7da5, 0x7e6b,
	0x7f0a, 0x7f82, 0x7fd2, 0x7ffa,
/* n = 256 */
	0x0, 0x4, 0x13, 0x2c,
	0x4f, 0x7c, 0xb2, 0xf3,
	0x13d, 0x191, 0x1ee, 0x256,
	0x2c6, 0x341, 0x3c5, 0x452,
	0x4e8, 0x588, 0x631, 0x6e2,
	0x79d, 0x860, 0x92c, 0xa01,
	0xade, 0xbc3, 0xcb0, 0xda5,
	0xea3, 0xfa7, 0x10b4, 0x11c7,
	0x12e2, 0x1404, 0x152c, 0x165c,
	0x1792, 0x18ce, 0x1a10, 0x1b58,
	0x1ca6, 0x1df9, 0x1f51, 0x20af,
	0x2211, 0x2379, 0x24e4, 0x2654,
	0x27c8, 0x293f, 0x2aba, 0x2c39,
	0x2dba, 0x2f3e, 0x30c5, 0x324e,
	0x33da, 0x3567, 0x36f6, 0x3886,
	0x3a18, 0x3baa, 0x3d3d, 0x3ed1,
	0x4064, 0x41f8, 0x438b, 0x451e,
	0x46b0, 0x4841, 0x49d1, 0x4b5f,
	0x4ceb, 0x4e75, 0x4ffe, 0x5183,
	0x5306, 0x5486, 0x5603, 0x577c,
	0x58f2, 0x5a64, 0x5bd1, 0x5d3b,
	0x5e9f, 0x6000, 0x615b, 0x62b1,
	0x6401, 0x654c, 0x6691, 0x67d0,
	0x6909, 0x6a3c, 0x6b68, 0x6c8d,
	0x6dab, 0x6ec3, 0x6fd2, 0x70db,
	0x71dc, 0x72d5, 0x73c6, 0x74b0,
	0x7591, 0x7669, 0x773a, 0x7801,
	0x78c0, 0x7977, 0x7a24, 0x7ac8,
	0x7b63, 0x7bf5, 0x7c7d, 0x7cfd,
	0x7d72, 0x7dde, 0x7e41, 0x7e99,
	0x7ee9, 0x7f2e, 0x7f69, 0x7f9b,
	0x7fc3, 0x7fe0, 0x7ff4, 0x7ffe,
/* n = 512 */
	0x0, 0x1, 0x4, 0xb,
	0x13, 0x1e, 0x2c, 0x3c,
	0x4f, 0x64, 0x7b, 0x95,
	0xb2, 0xd0, 0xf2, 0x115,
	0x13c, 0x164, 0x18f, 0x1bd,
	0x1ec, 0x21f, 0x253, 0x28a,
	0x2c4, 0x300, 0x33e, 0x37e,
	0x3c1, 0x406, 0x44e, 0x497,
	0x4e3, 0x532, 0x583, 0x5d5,
	0x62b, 0x682, 0x6dc, 0x737,
	0x796, 0x7f6, 0x858, 0x8bd,
	0x923, 0x98c, 0x9f7, 0xa64,
	0xad3, 0xb44, 0xbb8, 0xc2d,
	0xca4, 0xd1d, 0xd98, 0xe15,
	0xe94, 0xf15, 0xf98, 0x101d,
	0x10a4, 0x112c, 0x11b6, 0x1242,
	0x12d0, 0x1360, 0x13f1, 0x1484,
	0x1519, 0x15af, 0x1647, 0x16e0,
	0x177c, 0x1818, 0x18b7, 0x1956,
	0x19f8, 0x1a9a, 0x1b3f, 0x1be4,
	0x1c8b, 0x1d34, 0x1ddd, 0x1e88,
	0x1f35, 0x1fe2, 0x2091, 0x2141,
	0x21f3, 0x22a5, 0x2359, 0x240d,
	0x24c3, 0x257a, 0x2632, 0x26eb,
	0x27a5, 0x285f, 0x291b, 0x29d8,
	0x2a95, 0x2b53, 0x2c12, 0x2cd2,
	0x2d93, 0x2e54, 0x2f16, 0x2fd9,
	0x309c, 0x3160, 0x3224, 0x32e9,
	0x33ae, 0x3474, 0x353b, 0x3601,
	0x36c9, 0x3790, 0x3858, 0x3920,
	0x39e9, 0x3ab1, 0x3b7a, 0x3c43,
	0x3d0c, 0x3dd6, 0x3e9f, 0x3f68,
	0x4032, 0x40fb, 0x41c5, 0x428e,
	0x4357, 0x4420, 0x44e9, 0x45b2,
	0x467b, 0x4743, 0x480b, 0x48d3,
	0x499a, 0x4a61, 0x4b28, 0x4bee,
	0x4cb3, 0x4d79, 0x4e3d, 0x4f01,
	0x4fc5, 0x5088, 0x514a, 0x520c,
	0x52cd, 0x538d, 0x544c, 0x550b,
	0x55c9, 0x5686, 0x5742, 0x57fd,
	0x58b8, 0x5971, 0x5a29, 0x5ae1,
	0x5b97, 0x5c4c, 0x5d00, 0x5db3,
	0x5e65, 0x5f16, 0x5fc5, 0x6074,
	0x6121, 0x61cc, 0x6277, 0x6320,
	0x63c7, 0x646e, 0x6513, 0x65b6,
	0x6658, 0x66f9, 0x6798, 0x6835,
	0x68d1, 0x696c, 0x6a04, 0x6a9c,
	0x6b31, 0x6bc5, 0x6c57, 0x6ce7,
	0x6d76, 0x6e03, 0x6e8e, 0x6f17,
	0x6f9f, 0x7025, 0x70a8, 0x712a,
	0x71aa, 0x7228, 0x72a5, 0x731f,
	0x7397, 0x740d, 0x7481, 0x74f3,
	0x7564, 0x75d2, 0x763e, 0x76a7,
	0x770f, 0x7775, 0x77d8, 0x783a,
	0x7899, 0x78f6, 0x7950, 0x79a9,
	0x79ff, 0x7a53, 0x7aa5, 0x7af5,
	0x7b42, 0x7b8d, 0x7bd5, 0x7c1c,
	0x7c60, 0x7ca1, 0x7ce1, 0x7d1e,
	0x7d58, 0x7d90, 0x7dc6, 0x7dfa,
	0x7e2b, 0x7e59, 0x7e86, 0x7eaf,
	0x7ed7, 0x7efc, 0x7f1e, 0x7f3e,
	0x7f5c, 0x7f77, 0x7f90, 0x7fa6,
	0x7fba, 0x7fcb, 0x7fda, 0x7fe6,
	0x7ff0, 0x7ff8, 0x7ffd, 0x7fff,
/* n = 1024 */
	0x0, 0x0, 0x1, 0x2,
	0x4, 0x7, 0xb, 0xf,
	0x13, 0x19, 0x1e, 0x25,
	0x2c, 0x34, 0x3c, 0x45,
	0x4f, 0x59, 0x64, 0x6f,
	0x7b, 0x88, 0x95, 0xa3,
	0xb1, 0xc0, 0xd0, 0xe0,
	0xf1, 0x103, 0x115, 0x128,
	0x13b, 0x14f, 0x163, 0x179,
	0x18e, 0x1a5, 0x1bc, 0x1d3,
	0x1eb, 0x204, 0x21e, 0x238,
	0x252, 0x26d, 0x289, 0x2a5,
	0x2c2, 0x2e0, 0x2fe, 0x31d,
	0x33c, 0x35c, 0x37c, 0x39d,
	0x3bf, 0x3e1, 0x404, 0x428,
	0x44b, 0x470, 0x495, 0x4bb,
	0x4e1, 0x508, 0x52f, 0x557,
	0x580, 0x5a9, 0x5d3, 0x5fd,
	0x628, 0x653, 0x67f, 0x6ab,
	0x6d8, 0x706, 0x734, 0x763,
	0x792, 0x7c2, 0x7f2, 0x823,
	0x854, 0x886, 0x8b8, 0x8eb,
	0x91f, 0x953, 0x988, 0x9bd,
	0x9f2, 0xa28, 0xa5f, 0xa96,
	0xace, 0xb06, 0xb3f, 0xb78,
	0xbb2, 0xbec, 0xc27, 0xc62,
	0xc9e, 0xcda, 0xd17, 0xd54,
	0xd92, 0xdd0, 0xe0f, 0xe4e,
	0xe8d, 0xece, 0xf0e, 0xf4f,
	0xf91, 0xfd3, 0x1015, 0x1058,
	0x109c, 0x10e0, 0x1124, 0x1169,
	0x11ae, 0x11f4, 0x123a, 0x1280,
	0x12c7, 0x130f, 0x1356, 0x139f,
	0x13e7, 0x1431, 0x147a, 0x14c4,
	0x150f, 0x1559, 0x15a5, 0x15f0,
	0x163c, 0x1689, 0x16d6, 0x1723,
	0x1771, 0x17bf, 0x180d, 0x185c,
	0x18ab, 0x18fb, 0x194b, 0x199b,
	0x19ec, 0x1a3d, 0x1a8e, 0x1ae0,
	0x1b32, 0x1b85, 0x1bd7, 0x1c2b,
	0x1c7e, 0x1cd2, 0x1d26, 0x1d7b,
	0x1dd0, 0x1e25, 0x1e7b, 0x1ed0,
	0x1f27, 0x1f7d, 0x1fd4, 0x202b,
	0x2083, 0x20da, 0x2132, 0x218b,
	0x21e3, 0x223c, 0x2296, 0x22ef,
	0x2349, 0x23a3, 0x23fd, 0x2458,
	0x24b3, 0x250e, 0x2569, 0x25c5,
	0x2621, 0x267d, 0x26d9, 0x2736,
	0x2793, 0x27f0, 0x284e, 0x28ab,
	0x2909, 0x2967, 0x29c5, 0x2a24,
	0x2a82, 0x2ae1, 0x2b40, 0x2ba0,
	0x2bff, 0x2c5f, 0x2cbf, 0x2d1f,
	0x2d7f, 0x2ddf, 0x2e40, 0x2ea1,
	0x2f02, 0x2f63, 0x2fc4, 0x3026,
	0x3087, 0x30e9, 0x314b, 0x31ad,
	0x320f, 0x3271, 0x32d4, 0x3336,
	0x3399, 0x33fc, 0x345e, 0x34c1,
	0x3525, 0x3588, 0x35eb, 0x364f,
	0x36b2, 0x3716, 0x3779, 0x37dd,
	0x3841, 0x38a5, 0x3909, 0x396d,
	0x39d1, 0x3a35, 0x3a99, 0x3afe,
	0x3b62, 0x3bc7, 0x3c2b, 0x3c8f,
	0x3cf4, 0x3d58, 0x3dbd, 0x3e22,
	0x3e86, 0x3eeb, 0x3f4f, 0x3fb4,
	0x4019, 0x407d, 0x40e2, 0x4147,
	0x41ab, 0x4210, 0x4274, 0x42d9,
	0x433d, 0x43a2, 0x4406, 0x446b,
	0x44cf, 0x4533, 0x4598, 0x45fc,
	0x4660, 0x46c4, 0x4728, 0x478c,
	0x47f0, 0x4854, 0x48b8, 0x491b,
	0x497f, 0x49e2, 0x4a46, 0x4aa9,
	0x4b0c, 0x4b6f, 0x4bd2, 0x4c35,
	0x4c98, 0x4cfa, 0x4d5d, 0x4dbf,
	0x4e21, 0x4e83, 0x4ee5, 0x4f47,
	0x4fa9, 0x500a, 0x506c, 0x50cd,
	0x512e, 0x518f, 0x51ef, 0x5250,
	0x52b0, 0x5310, 0x5370, 0x53d0,
	0x5430, 0x548f, 0x54ee, 0x554d,
	0x55ac, 0x560b, 0x5669, 0x56c7,
	0x5725, 0x5783, 0x57e0, 0x583d,
	0x589a, 0x58f7, 0x5954, 0x59b0,
	0x5a0c, 0x5a68, 0x5ac3, 0x5b1f,
	0x5b7a, 0x5bd4, 0x5c2f, 0x5c89,
	0x5ce3, 0x5d3d, 0x5d96, 0x5def,
	0x5e48, 0x5ea0, 0x5ef9, 0x5f51,
	0x5fa8, 0x6000, 0x6056, 0x60ad,
	0x6104, 0x615a, 0x61af, 0x6205,
	0x625a, 0x62af, 0x6303, 0x6357,
	0x63ab, 0x63fe, 0x6451, 0x64a4,
	0x64f6, 0x6548, 0x659a, 0x65eb,
	0x663c, 0x668c, 0x66dc, 0x672c,
	0x677c, 0x67cb, 0x6819, 0x6867,
	0x68b5, 0x6903, 0x6950, 0x699c,
	0x69e9, 0x6a35, 0x6a80, 0x6acb,
	0x6b16, 0x6b60, 0x6baa, 0x6bf3,
	0x6c3c, 0x6c84, 0x6ccd, 0x6d14,
	0x6d5b, 0x6da2, 0x6de8, 0x6e2e,
	0x6e74, 0x6eb9, 0x6efd, 0x6f41,
	0x6f85, 0x6fc8, 0x700b, 0x704d,
	0x708f, 0x70d0, 0x7111, 0x7152,
	0x7191, 0x71d1, 0x7210, 0x724e,
	0x728c, 0x72ca, 0x7307, 0x7343,
	0x737f, 0x73bb, 0x73f6, 0x7430,
	0x746a, 0x74a4, 0x74dd, 0x7515,
	0x754d, 0x7584, 0x75bb, 0x75f2,
	0x7628, 0x765d, 0x7692, 0x76c6,
	0x76fa, 0x772d, 0x7760, 0x7792,
	0x77c4, 0x77f5, 0x7825, 0x7855,
	0x7885, 0x78b4, 0x78e2, 0x7910,
	0x793d, 0x796a, 0x7996, 0x79c2,
	0x79ed, 0x7a17, 0x7a41, 0x7a6b,
	0x7a94, 0x7abc, 0x7ae3, 0x7b0b,
	0x7b31, 0x7b57, 0x7b7d, 0x7ba1,
	0x7bc6, 0x7be9, 0x7c0c, 0x7c2f,
	0x7c51, 0x7c72, 0x7c93, 0x7cb3,
	0x7cd3, 0x7cf2, 0x7d10, 0x7d2e,
	0x7d4b, 0x7d68, 0x7d84, 0x7d9f,
	0x7dba, 0x7dd4, 0x7dee, 0x7e07,
	0x7e20, 0x7e38, 0x7e4f, 0x7e66,
	0x7e7c, 0x7e91, 0x7ea6, 0x7eba,
	0x7ece, 0x7ee1, 0x7ef3, 0x7f05,
	0x7f16, 0x7f27, 0x7f37, 0x7f46,
	0x7f55, 0x7f63, 0x7f71, 0x7f7e,
	0x7f8a, 0x7f96, 0x7fa1, 0x7fab,
	0x7fb5, 0x7fbf, 0x7fc7, 0x7fcf,
	0x7fd7, 0x7fdd, 0x7fe4, 0x7fe9,
	0x7fee, 0x7ff2, 0x7ff6, 0x7ff9,
	0x7ffc, 0x7ffe, 0x7fff, 0x7fff
};

/* *********************************************************** */

const mlib_s16 mlib_s_Hamming_S16[] = {
/* n = 2 */
	0xa3d,
/* n = 4 */
	0xa3d, 0x628f,
/* n = 8 */
	0xa3d, 0x2068, 0x5238, 0x7a2b,
/* n = 16 */
	0xa3d, 0xf54, 0x1db8, 0x32ec,
	0x4b46, 0x628f, 0x74c1, 0x7eb6,
/* n = 32 */
	0xa3d, 0xb71, 0xf03, 0x14cb,
	0x1c8d, 0x25f9, 0x30ab, 0x3c34,
	0x481a, 0x53e0, 0x5f0c, 0x6929,
	0x71cb, 0x7899, 0x7d4c, 0x7fb2,
/* n = 64 */
	0xa3d, 0xa88, 0xb68, 0xcdb,
	0xedd, 0x1169, 0x1478, 0x1803,
	0x1c02, 0x2068, 0x252c, 0x2a42,
	0x2f9b, 0x352c, 0x3ae5, 0x40b8,
	0x4696, 0x4c70, 0x5238, 0x57df,
	0x5d56, 0x628f, 0x677d, 0x6c14,
	0x7048, 0x740e, 0x775c, 0x7a2b,
	0x7c72, 0x7e2d, 0x7f57, 0x7fed,
/* n = 128 */
	0xa3d, 0xa4f, 0xa87, 0xae3,
	0xb63, 0xc08, 0xcd0, 0xdbc,
	0xeca, 0xffb, 0x114c, 0x12bf,
	0x1450, 0x1601, 0x17ce, 0x19b9,
	0x1bbe, 0x1ddd, 0x2015, 0x2264,
	0x24c9, 0x2742, 0x29ce, 0x2c6b,
	0x2f17, 0x31d2, 0x3498, 0x3768,
	0x3a42, 0x3d22, 0x4007, 0x42ef,
	0x45d9, 0x48c2, 0x4ba9, 0x4e8b,
	0x5168, 0x543d, 0x5709, 0x59ca,
	0x5c7d, 0x5f22, 0x61b7, 0x6439,
	0x66a8, 0x6903, 0x6b46, 0x6d72,
	0x6f85, 0x717c, 0x7359, 0x7518,
	0x76b9, 0x783b, 0x799d, 0x7ade,
	0x7bfe, 0x7cfb, 0x7dd5, 0x7e8b,
	0x7f1e, 0x7f8c, 0x7fd6, 0x7ffb,
/* n = 256 */
	0xa3d, 0xa42, 0xa4f, 0xa66,
	0xa86, 0xaaf, 0xae1, 0xb1d,
	0xb61, 0xbae, 0xc04, 0xc63,
	0xccb, 0xd3c, 0xdb5, 0xe37,
	0xec1, 0xf54, 0xfef, 0x1093,
	0x113e, 0x11f2, 0x12ae, 0x1371,
	0x143d, 0x150f, 0x15ea, 0x16cb,
	0x17b4, 0x18a4, 0x199b, 0x1a99,
	0x1b9d, 0x1ca7, 0x1db8, 0x1ecf,
	0x1fec, 0x210f, 0x2237, 0x2365,
	0x2498, 0x25d0, 0x270d, 0x284f,
	0x2995, 0x2adf, 0x2c2e, 0x2d80,
	0x2ed6, 0x3030, 0x318d, 0x32ec,
	0x344f, 0x35b4, 0x371c, 0x3886,
	0x39f1, 0x3b5f, 0x3cce, 0x3e3e,
	0x3faf, 0x4122, 0x4294, 0x4408,
	0x457b, 0x46ee, 0x4861, 0x49d4,
	0x4b46, 0x4cb7, 0x4e26, 0x4f95,
	0x5101, 0x526c, 0x53d5, 0x553b,
	0x569f, 0x5800, 0x595f, 0x5aba,
	0x5c12, 0x5d66, 0x5eb6, 0x6003,
	0x614b, 0x628f, 0x63ce, 0x6509,
	0x663e, 0x676f, 0x689a, 0x69c0,
	0x6adf, 0x6bf9, 0x6d0d, 0x6e1b,
	0x6f23, 0x7024, 0x711e, 0x7211,
	0x72fe, 0x73e3, 0x74c1, 0x7597,
	0x7666, 0x772e, 0x77ed, 0x78a5,
	0x7955, 0x79fc, 0x7a9c, 0x7b33,
	0x7bc1, 0x7c48, 0x7cc5, 0x7d3a,
	0x7da6, 0x7e0a, 0x7e64, 0x7eb6,
	0x7eff, 0x7f3f, 0x7f75, 0x7fa3,
	0x7fc7, 0x7fe3, 0x7ff5, 0x7ffe,
/* n = 512 */
	0xa3d, 0xa3e, 0xa41, 0xa47,
	0xa4f, 0xa59, 0xa66, 0xa75,
	0xa86, 0xa99, 0xaaf, 0xac7,
	0xae1, 0xafd, 0xb1c, 0xb3d,
	0xb60, 0xb85, 0xbad, 0xbd6,
	0xc02, 0xc31, 0xc61, 0xc94,
	0xcc9, 0xd00, 0xd39, 0xd74,
	0xdb1, 0xdf1, 0xe33, 0xe77,
	0xebd, 0xf05, 0xf4f, 0xf9b,
	0xfea, 0x103a, 0x108d, 0x10e1,
	0x1138, 0x1190, 0x11eb, 0x1247,
	0x12a6, 0x1306, 0x1368, 0x13cd,
	0x1433, 0x149b, 0x1505, 0x1571,
	0x15de, 0x164e, 0x16bf, 0x1732,
	0x17a7, 0x181e, 0x1896, 0x1910,
	0x198c, 0x1a0a, 0x1a89, 0x1b0a,
	0x1b8c, 0x1c10, 0x1c96, 0x1d1d,
	0x1da6, 0x1e30, 0x1ebc, 0x1f49,
	0x1fd8, 0x2068, 0x20fa, 0x218d,
	0x2221, 0x22b7, 0x234e, 0x23e6,
	0x2480, 0x251b, 0x25b7, 0x2655,
	0x26f3, 0x2793, 0x2834, 0x28d6,
	0x2979, 0x2a1d, 0x2ac2, 0x2b68,
	0x2c10, 0x2cb8, 0x2d61, 0x2e0b,
	0x2eb6, 0x2f62, 0x300f, 0x30bc,
	0x316a, 0x3219, 0x32c9, 0x337a,
	0x342b, 0x34dd, 0x358f, 0x3642,
	0x36f6, 0x37aa, 0x385f, 0x3914,
	0x39c9, 0x3a7f, 0x3b36, 0x3bed,
	0x3ca4, 0x3d5c, 0x3e13, 0x3ecc,
	0x3f84, 0x403d, 0x40f5, 0x41ae,
	0x4267, 0x4321, 0x43da, 0x4493,
	0x454d, 0x4606, 0x46bf, 0x4778,
	0x4832, 0x48eb, 0x49a3, 0x4a5c,
	0x4b15, 0x4bcd, 0x4c85, 0x4d3d,
	0x4df4, 0x4eab, 0x4f62, 0x5018,
	0x50ce, 0x5183, 0x5238, 0x52ed,
	0x53a1, 0x5454, 0x5507, 0x55b9,
	0x566a, 0x571b, 0x57cb, 0x587b,
	0x5929, 0x59d7, 0x5a84, 0x5b31,
	0x5bdc, 0x5c86, 0x5d30, 0x5dd9,
	0x5e80, 0x5f27, 0x5fcd, 0x6072,
	0x6115, 0x61b8, 0x6259, 0x62fa,
	0x6399, 0x6437, 0x64d4, 0x656f,
	0x6609, 0x66a2, 0x673a, 0x67d1,
	0x6866, 0x68f9, 0x698c, 0x6a1d,
	0x6aac, 0x6b3a, 0x6bc7, 0x6c52,
	0x6cdb, 0x6d63, 0x6dea, 0x6e6e,
	0x6ef2, 0x6f73, 0x6ff3, 0x7072,
	0x70ee, 0x7169, 0x71e3, 0x725a,
	0x72d0, 0x7344, 0x73b6, 0x7426,
	0x7495, 0x7502, 0x756d, 0x75d6,
	0x763d, 0x76a2, 0x7705, 0x7767,
	0x77c6, 0x7824, 0x787f, 0x78d9,
	0x7930, 0x7986, 0x79d9, 0x7a2b,
	0x7a7a, 0x7ac7, 0x7b13, 0x7b5c,
	0x7ba3, 0x7be8, 0x7c2b, 0x7c6b,
	0x7caa, 0x7ce6, 0x7d21, 0x7d59,
	0x7d8f, 0x7dc2, 0x7df4, 0x7e23,
	0x7e50, 0x7e7b, 0x7ea4, 0x7eca,
	0x7eef, 0x7f11, 0x7f30, 0x7f4e,
	0x7f69, 0x7f82, 0x7f99, 0x7fad,
	0x7fbf, 0x7fcf, 0x7fdd, 0x7fe8,
	0x7ff2, 0x7ff8, 0x7ffd, 0x7fff,
/* n = 1024 */
	0xa3d, 0xa3d, 0xa3e, 0xa3f,
	0xa41, 0xa44, 0xa47, 0xa4b,
	0xa4f, 0xa54, 0xa59, 0xa5f,
	0xa66, 0xa6d, 0xa75, 0xa7d,
	0xa86, 0xa8f, 0xa99, 0xaa3,
	0xaaf, 0xaba, 0xac6, 0xad3,
	0xae0, 0xaee, 0xafd, 0xb0c,
	0xb1b, 0xb2b, 0xb3c, 0xb4d,
	0xb5f, 0xb71, 0xb84, 0xb98,
	0xbac, 0xbc0, 0xbd6, 0xbeb,
	0xc02, 0xc18, 0xc30, 0xc48,
	0xc60, 0xc79, 0xc93, 0xcad,
	0xcc7, 0xce2, 0xcfe, 0xd1a,
	0xd37, 0xd55, 0xd72, 0xd91,
	0xdb0, 0xdcf, 0xdef, 0xe10,
	0xe31, 0xe53, 0xe75, 0xe97,
	0xebb, 0xede, 0xf03, 0xf27,
	0xf4d, 0xf72, 0xf99, 0xfc0,
	0xfe7, 0x100f, 0x1037, 0x1060,
	0x1089, 0x10b3, 0x10de, 0x1109,
	0x1134, 0x1160, 0x118d, 0x11b9,
	0x11e7, 0x1215, 0x1243, 0x1272,
	0x12a2, 0x12d1, 0x1302, 0x1333,
	0x1364, 0x1396, 0x13c8, 0x13fb,
	0x142e, 0x1462, 0x1496, 0x14cb,
	0x1500, 0x1535, 0x156b, 0x15a2,
	0x15d9, 0x1610, 0x1648, 0x1680,
	0x16b9, 0x16f2, 0x172c, 0x1766,
	0x17a1, 0x17dc, 0x1817, 0x1853,
	0x188f, 0x18cc, 0x1909, 0x1947,
	0x1985, 0x19c3, 0x1a02, 0x1a41,
	0x1a81, 0x1ac1, 0x1b02, 0x1b43,
	0x1b84, 0x1bc6, 0x1c08, 0x1c4a,
	0x1c8d, 0x1cd1, 0x1d14, 0x1d58,
	0x1d9d, 0x1de2, 0x1e27, 0x1e6c,
	0x1eb2, 0x1ef9, 0x1f3f, 0x1f86,
	0x1fce, 0x2016, 0x205e, 0x20a6,
	0x20ef, 0x2138, 0x2182, 0x21cc,
	0x2216, 0x2261, 0x22ac, 0x22f7,
	0x2342, 0x238e, 0x23db, 0x2427,
	0x2474, 0x24c1, 0x250f, 0x255c,
	0x25ab, 0x25f9, 0x2648, 0x2697,
	0x26e6, 0x2736, 0x2786, 0x27d6,
	0x2826, 0x2877, 0x28c8, 0x2919,
	0x296b, 0x29bd, 0x2a0f, 0x2a61,
	0x2ab4, 0x2b07, 0x2b5a, 0x2bad,
	0x2c01, 0x2c54, 0x2ca9, 0x2cfd,
	0x2d51, 0x2da6, 0x2dfb, 0x2e51,
	0x2ea6, 0x2efc, 0x2f52, 0x2fa8,
	0x2ffe, 0x3054, 0x30ab, 0x3102,
	0x3159, 0x31b1, 0x3208, 0x3260,
	0x32b8, 0x3310, 0x3368, 0x33c0,
	0x3419, 0x3471, 0x34ca, 0x3523,
	0x357c, 0x35d6, 0x362f, 0x3689,
	0x36e3, 0x373d, 0x3797, 0x37f1,
	0x384b, 0x38a5, 0x3900, 0x395b,
	0x39b5, 0x3a10, 0x3a6b, 0x3ac6,
	0x3b22, 0x3b7d, 0x3bd8, 0x3c34,
	0x3c8f, 0x3ceb, 0x3d47, 0x3da2,
	0x3dfe, 0x3e5a, 0x3eb6, 0x3f12,
	0x3f6e, 0x3fcb, 0x4027, 0x4083,
	0x40df, 0x413c, 0x4198, 0x41f5,
	0x4251, 0x42ad, 0x430a, 0x4367,
	0x43c3, 0x4420, 0x447c, 0x44d9,
	0x4535, 0x4592, 0x45ef, 0x464b,
	0x46a8, 0x4704, 0x4761, 0x47bd,
	0x481a, 0x4876, 0x48d3, 0x492f,
	0x498b, 0x49e8, 0x4a44, 0x4aa0,
	0x4afc, 0x4b58, 0x4bb4, 0x4c10,
	0x4c6c, 0x4cc8, 0x4d24, 0x4d7f,
	0x4ddb, 0x4e36, 0x4e92, 0x4eed,
	0x4f48, 0x4fa4, 0x4fff, 0x505a,
	0x50b4, 0x510f, 0x516a, 0x51c4,
	0x521f, 0x5279, 0x52d3, 0x532d,
	0x5387, 0x53e0, 0x543a, 0x5493,
	0x54ed, 0x5546, 0x559f, 0x55f7,
	0x5650, 0x56a9, 0x5701, 0x5759,
	0x57b1, 0x5809, 0x5860, 0x58b8,
	0x590f, 0x5966, 0x59bd, 0x5a13,
	0x5a6a, 0x5ac0, 0x5b16, 0x5b6c,
	0x5bc1, 0x5c17, 0x5c6c, 0x5cc1,
	0x5d15, 0x5d6a, 0x5dbe, 0x5e12,
	0x5e66, 0x5eb9, 0x5f0c, 0x5f5f,
	0x5fb2, 0x6005, 0x6057, 0x60a9,
	0x60fa, 0x614c, 0x619d, 0x61ee,
	0x623f, 0x628f, 0x62df, 0x632f,
	0x637e, 0x63cd, 0x641c, 0x646b,
	0x64b9, 0x6507, 0x6555, 0x65a2,
	0x65ef, 0x663c, 0x6688, 0x66d4,
	0x6720, 0x676b, 0x67b6, 0x6801,
	0x684b, 0x6896, 0x68df, 0x6929,
	0x6972, 0x69ba, 0x6a03, 0x6a4b,
	0x6a92, 0x6ada, 0x6b20, 0x6b67,
	0x6bad, 0x6bf3, 0x6c38, 0x6c7d,
	0x6cc2, 0x6d06, 0x6d4a, 0x6d8e,
	0x6dd1, 0x6e13, 0x6e56, 0x6e98,
	0x6ed9, 0x6f1a, 0x6f5b, 0x6f9b,
	0x6fdb, 0x701b, 0x705a, 0x7098,
	0x70d7, 0x7114, 0x7152, 0x718f,
	0x71cb, 0x7207, 0x7243, 0x727e,
	0x72b9, 0x72f3, 0x732d, 0x7367,
	0x73a0, 0x73d8, 0x7410, 0x7448,
	0x747f, 0x74b6, 0x74ec, 0x7522,
	0x7557, 0x758c, 0x75c1, 0x75f5,
	0x7628, 0x765b, 0x768e, 0x76c0,
	0x76f1, 0x7722, 0x7753, 0x7783,
	0x77b3, 0x77e2, 0x7810, 0x783f,
	0x786c, 0x7899, 0x78c6, 0x78f2,
	0x791e, 0x7949, 0x7974, 0x799e,
	0x79c8, 0x79f1, 0x7a1a, 0x7a42,
	0x7a69, 0x7a90, 0x7ab7, 0x7add,
	0x7b03, 0x7b28, 0x7b4c, 0x7b70,
	0x7b94, 0x7bb6, 0x7bd9, 0x7bfb,
	0x7c1c, 0x7c3d, 0x7c5d, 0x7c7d,
	0x7c9c, 0x7cbb, 0x7cd9, 0x7cf7,
	0x7d14, 0x7d30, 0x7d4c, 0x7d68,
	0x7d83, 0x7d9d, 0x7db7, 0x7dd0,
	0x7de9, 0x7e01, 0x7e18, 0x7e30,
	0x7e46, 0x7e5c, 0x7e71, 0x7e86,
	0x7e9b, 0x7eae, 0x7ec2, 0x7ed4,
	0x7ee6, 0x7ef8, 0x7f09, 0x7f19,
	0x7f29, 0x7f38, 0x7f47, 0x7f55,
	0x7f63, 0x7f70, 0x7f7c, 0x7f88,
	0x7f94, 0x7f9e, 0x7fa9, 0x7fb2,
	0x7fbb, 0x7fc4, 0x7fcc, 0x7fd3,
	0x7fda, 0x7fe0, 0x7fe6, 0x7feb,
	0x7ff0, 0x7ff3, 0x7ff7, 0x7ffa,
	0x7ffc, 0x7ffe, 0x7fff, 0x7fff
};

/* *********************************************************** */

const mlib_s16 mlib_s_Blackman_S16[] = {
/* n = 2 */
	0x0,
/* n = 4 */
	0x0, 0x50a3,
/* n = 8 */
	0x0, 0xb93, 0x3ac6, 0x75ce,
/* n = 16 */
	0x0, 0x225, 0x9dd, 0x19b2,
	0x326e, 0x50a3, 0x6cb3, 0x7db7,
/* n = 32 */
	0x0, 0x7a, 0x200, 0x4c9,
	0x925, 0xf65, 0x17c3, 0x224c,
	0x2ed0, 0x3cd9, 0x4bad, 0x5a5e,
	0x67df, 0x7322, 0x7b3a, 0x7f76,
/* n = 64 */
	0x0, 0x1d, 0x77, 0x110,
	0x1ef, 0x31c, 0x49f, 0x682,
	0x8d1, 0xb93, 0xed3, 0x1295,
	0x16df, 0x1bb0, 0x2106, 0x26da,
	0x2d20, 0x33cb, 0x3ac6, 0x41fb,
	0x494e, 0x50a3, 0x57db, 0x5ed5,
	0x6570, 0x6b8c, 0x710a, 0x75ce,
	0x79be, 0x7cc6, 0x7ed4, 0x7fde,
/* n = 128 */
	0x0, 0x7, 0x1c, 0x41,
	0x75, 0xb8, 0x10b, 0x170,
	0x1e7, 0x270, 0x30e, 0x3c1,
	0x48a, 0x56b, 0x665, 0x779,
	0x8a8, 0x9f4, 0xb5d, 0xce5,
	0xe8d, 0x1054, 0x123d, 0x1446,
	0x1671, 0x18be, 0x1b2b, 0x1dba,
	0x2069, 0x2337, 0x2623, 0x292c,
	0x2c50, 0x2f8d, 0x32e1, 0x364a,
	0x39c5, 0x3d4e, 0x40e4, 0x4482,
	0x4826, 0x4bcb, 0x4f6e, 0x530b,
	0x569f, 0x5a24, 0x5d98, 0x60f6,
	0x6439, 0x6760, 0x6a64, 0x6d43,
	0x6ff9, 0x7282, 0x74dc, 0x7702,
	0x78f3, 0x7aac, 0x7c2a, 0x7d6c,
	0x7e6f, 0x7f33, 0x7fb6, 0x7ff7,
/* n = 256 */
	0x0, 0x1, 0x7, 0x10,
	0x1c, 0x2d, 0x40, 0x58,
	0x74, 0x93, 0xb6, 0xde,
	0x109, 0x139, 0x16d, 0x1a6,
	0x1e3, 0x225, 0x26b, 0x2b7,
	0x308, 0x35d, 0x3b9, 0x41a,
	0x480, 0x4ed, 0x55f, 0x5d8,
	0x657, 0x6dc, 0x768, 0x7fb,
	0x894, 0x935, 0x9dd, 0xa8c,
	0xb43, 0xc01, 0xcc7, 0xd95,
	0xe6a, 0xf48, 0x102d, 0x111b,
	0x1211, 0x130f, 0x1416, 0x1524,
	0x163b, 0x175b, 0x1882, 0x19b2,
	0x1aea, 0x1c2b, 0x1d73, 0x1ec3,
	0x201c, 0x217c, 0x22e4, 0x2453,
	0x25ca, 0x2747, 0x28cc, 0x2a58,
	0x2bea, 0x2d82, 0x2f21, 0x30c5,
	0x326e, 0x341d, 0x35d1, 0x3789,
	0x3946, 0x3b06, 0x3cca, 0x3e91,
	0x405b, 0x4226, 0x43f4, 0x45c3,
	0x4793, 0x4964, 0x4b35, 0x4d05,
	0x4ed5, 0x50a3, 0x5270, 0x543a,
	0x5602, 0x57c6, 0x5986, 0x5b42,
	0x5cfa, 0x5eac, 0x6059, 0x61ff,
	0x639e, 0x6537, 0x66c7, 0x6850,
	0x69cf, 0x6b46, 0x6cb3, 0x6e16,
	0x6f6f, 0x70bc, 0x71ff, 0x7336,
	0x7460, 0x757f, 0x7690, 0x7794,
	0x788b, 0x7974, 0x7a4e, 0x7b1b,
	0x7bd8, 0x7c87, 0x7d27, 0x7db7,
	0x7e38, 0x7ea8, 0x7f0a, 0x7f5b,
	0x7f9c, 0x7fcd, 0x7fed, 0x7ffd,
/* n = 512 */
	0x0, 0x0, 0x1, 0x4,
	0x7, 0xb, 0x10, 0x15,
	0x1c, 0x24, 0x2c, 0x36,
	0x40, 0x4c, 0x58, 0x65,
	0x73, 0x82, 0x92, 0xa4,
	0xb6, 0xc9, 0xdd, 0xf2,
	0x108, 0x120, 0x138, 0x151,
	0x16c, 0x187, 0x1a4, 0x1c2,
	0x1e1, 0x201, 0x222, 0x245,
	0x269, 0x28e, 0x2b4, 0x2db,
	0x304, 0x32e, 0x35a, 0x387,
	0x3b5, 0x3e4, 0x415, 0x448,
	0x47b, 0x4b1, 0x4e7, 0x51f,
	0x559, 0x594, 0x5d1, 0x610,
	0x650, 0x691, 0x6d4, 0x719,
	0x760, 0x7a8, 0x7f2, 0x83d,
	0x88b, 0x8da, 0x92b, 0x97d,
	0x9d2, 0xa28, 0xa80, 0xada,
	0xb36, 0xb93, 0xbf3, 0xc55,
	0xcb8, 0xd1d, 0xd85, 0xdee,
	0xe59, 0xec7, 0xf36, 0xfa7,
	0x101a, 0x1090, 0x1107, 0x1180,
	0x11fc, 0x1279, 0x12f9, 0x137a,
	0x13fe, 0x1483, 0x150b, 0x1595,
	0x1621, 0x16af, 0x173f, 0x17d1,
	0x1865, 0x18fb, 0x1994, 0x1a2e,
	0x1aca, 0x1b69, 0x1c09, 0x1cac,
	0x1d50, 0x1df6, 0x1e9f, 0x1f49,
	0x1ff6, 0x20a4, 0x2154, 0x2206,
	0x22ba, 0x2370, 0x2428, 0x24e2,
	0x259d, 0x265a, 0x2719, 0x27da,
	0x289d, 0x2961, 0x2a27, 0x2aee,
	0x2bb7, 0x2c82, 0x2d4e, 0x2e1c,
	0x2eeb, 0x2fbb, 0x308d, 0x3161,
	0x3236, 0x330c, 0x33e3, 0x34bb,
	0x3595, 0x3670, 0x374c, 0x3829,
	0x3907, 0x39e6, 0x3ac6, 0x3ba7,
	0x3c88, 0x3d6b, 0x3e4e, 0x3f32,
	0x4016, 0x40fb, 0x41e1, 0x42c7,
	0x43ae, 0x4494, 0x457c, 0x4663,
	0x474b, 0x4833, 0x491a, 0x4a02,
	0x4aea, 0x4bd2, 0x4cba, 0x4da2,
	0x4e89, 0x4f70, 0x5057, 0x513d,
	0x5222, 0x5308, 0x53ec, 0x54d0,
	0x55b3, 0x5696, 0x5777, 0x5858,
	0x5938, 0x5a16, 0x5af4, 0x5bd0,
	0x5cab, 0x5d85, 0x5e5d, 0x5f35,
	0x600a, 0x60de, 0x61b1, 0x6282,
	0x6351, 0x641e, 0x64ea, 0x65b3,
	0x667b, 0x6741, 0x6804, 0x68c6,
	0x6985, 0x6a42, 0x6afd, 0x6bb5,
	0x6c6b, 0x6d1e, 0x6dcf, 0x6e7e,
	0x6f29, 0x6fd3, 0x7079, 0x711c,
	0x71bd, 0x725b, 0x72f6, 0x738d,
	0x7422, 0x74b4, 0x7543, 0x75ce,
	0x7656, 0x76db, 0x775d, 0x77db,
	0x7856, 0x78cd, 0x7941, 0x79b2,
	0x7a1f, 0x7a88, 0x7aee, 0x7b50,
	0x7baf, 0x7c09, 0x7c60, 0x7cb4,
	0x7d03, 0x7d4f, 0x7d97, 0x7ddb,
	0x7e1b, 0x7e57, 0x7e8f, 0x7ec3,
	0x7ef4, 0x7f20, 0x7f49, 0x7f6d,
	0x7f8d, 0x7faa, 0x7fc2, 0x7fd6,
	0x7fe7, 0x7ff3, 0x7ffb, 0x7fff,
/* n = 1024 */
	0x0, 0x0, 0x0, 0x1,
	0x1, 0x2, 0x4, 0x5,
	0x7, 0x9, 0xb, 0xd,
	0x10, 0x12, 0x15, 0x19,
	0x1c, 0x20, 0x24, 0x28,
	0x2c, 0x31, 0x36, 0x3b,
	0x40, 0x46, 0x4b, 0x51,
	0x58, 0x5e, 0x65, 0x6c,
	0x73, 0x7a, 0x82, 0x8a,
	0x92, 0x9b, 0xa3, 0xac,
	0xb5, 0xbf, 0xc8, 0xd2,
	0xdc, 0xe7, 0xf2, 0xfd,
	0x108, 0x113, 0x11f, 0x12b,
	0x137, 0x144, 0x151, 0x15e,
	0x16b, 0x179, 0x186, 0x195,
	0x1a3, 0x1b2, 0x1c1, 0x1d0,
	0x1e0, 0x1f0, 0x200, 0x210,
	0x221, 0x232, 0x244, 0x255,
	0x267, 0x27a, 0x28c, 0x29f,
	0x2b3, 0x2c6, 0x2da, 0x2ee,
	0x303, 0x318, 0x32d, 0x342,
	0x358, 0x36e, 0x385, 0x39c,
	0x3b3, 0x3ca, 0x3e2, 0x3fa,
	0x413, 0x42c, 0x445, 0x45f,
	0x479, 0x493, 0x4ae, 0x4c9,
	0x4e5, 0x500, 0x51d, 0x539,
	0x556, 0x574, 0x591, 0x5af,
	0x5ce, 0x5ed, 0x60c, 0x62c,
	0x64c, 0x66d, 0x68d, 0x6af,
	0x6d0, 0x6f3, 0x715, 0x738,
	0x75b, 0x77f, 0x7a3, 0x7c8,
	0x7ed, 0x813, 0x839, 0x85f,
	0x886, 0x8ad, 0x8d5, 0x8fd,
	0x925, 0x94e, 0x978, 0x9a2,
	0x9cc, 0x9f7, 0xa22, 0xa4e,
	0xa7a, 0xaa7, 0xad4, 0xb01,
	0xb2f, 0xb5e, 0xb8d, 0xbbc,
	0xbec, 0xc1d, 0xc4d, 0xc7f,
	0xcb1, 0xce3, 0xd16, 0xd49,
	0xd7d, 0xdb1, 0xde6, 0xe1b,
	0xe51, 0xe87, 0xebe, 0xef5,
	0xf2d, 0xf65, 0xf9e, 0xfd7,
	0x1011, 0x104b, 0x1086, 0x10c1,
	0x10fd, 0x1139, 0x1176, 0x11b3,
	0x11f1, 0x122f, 0x126e, 0x12ad,
	0x12ed, 0x132e, 0x136f, 0x13b0,
	0x13f2, 0x1434, 0x1477, 0x14bb,
	0x14ff, 0x1543, 0x1588, 0x15ce,
	0x1614, 0x165a, 0x16a1, 0x16e9,
	0x1731, 0x177a, 0x17c3, 0x180c,
	0x1857, 0x18a1, 0x18ec, 0x1938,
	0x1984, 0x19d1, 0x1a1e, 0x1a6c,
	0x1aba, 0x1b09, 0x1b58, 0x1ba8,
	0x1bf8, 0x1c49, 0x1c9a, 0x1cec,
	0x1d3f, 0x1d91, 0x1de5, 0x1e38,
	0x1e8d, 0x1ee1, 0x1f37, 0x1f8c,
	0x1fe3, 0x2039, 0x2091, 0x20e8,
	0x2140, 0x2199, 0x21f2, 0x224c,
	0x22a6, 0x2300, 0x235b, 0x23b7,
	0x2413, 0x246f, 0x24cc, 0x2529,
	0x2587, 0x25e5, 0x2644, 0x26a3,
	0x2703, 0x2762, 0x27c3, 0x2824,
	0x2885, 0x28e7, 0x2949, 0x29ab,
	0x2a0e, 0x2a71, 0x2ad5, 0x2b39,
	0x2b9e, 0x2c03, 0x2c68, 0x2cce,
	0x2d34, 0x2d9a, 0x2e01, 0x2e68,
	0x2ed0, 0x2f38, 0x2fa0, 0x3009,
	0x3072, 0x30db, 0x3145, 0x31af,
	0x3219, 0x3284, 0x32ef, 0x335a,
	0x33c6, 0x3432, 0x349e, 0x350a,
	0x3577, 0x35e4, 0x3652, 0x36bf,
	0x372d, 0x379c, 0x380a, 0x3879,
	0x38e8, 0x3957, 0x39c6, 0x3a36,
	0x3aa6, 0x3b16, 0x3b86, 0x3bf7,
	0x3c68, 0x3cd9, 0x3d4a, 0x3dbb,
	0x3e2d, 0x3e9e, 0x3f10, 0x3f82,
	0x3ff4, 0x4067, 0x40d9, 0x414c,
	0x41be, 0x4231, 0x42a4, 0x4317,
	0x438a, 0x43fe, 0x4471, 0x44e4,
	0x4558, 0x45cb, 0x463f, 0x46b3,
	0x4726, 0x479a, 0x480e, 0x4882,
	0x48f6, 0x496a, 0x49dd, 0x4a51,
	0x4ac5, 0x4b39, 0x4bad, 0x4c21,
	0x4c94, 0x4d08, 0x4d7c, 0x4def,
	0x4e63, 0x4ed6, 0x4f4a, 0x4fbd,
	0x5030, 0x50a3, 0x5116, 0x5189,
	0x51fc, 0x526f, 0x52e1, 0x5353,
	0x53c5, 0x5437, 0x54a9, 0x551b,
	0x558c, 0x55fe, 0x566f, 0x56df,
	0x5750, 0x57c0, 0x5831, 0x58a1,
	0x5910, 0x5980, 0x59ef, 0x5a5e,
	0x5acc, 0x5b3b, 0x5ba9, 0x5c16,
	0x5c84, 0x5cf1, 0x5d5e, 0x5dca,
	0x5e36, 0x5ea2, 0x5f0d, 0x5f78,
	0x5fe3, 0x604d, 0x60b7, 0x6121,
	0x618a, 0x61f3, 0x625b, 0x62c3,
	0x632a, 0x6391, 0x63f8, 0x645e,
	0x64c3, 0x6528, 0x658d, 0x65f1,
	0x6655, 0x66b8, 0x671b, 0x677d,
	0x67df, 0x6840, 0x68a0, 0x6900,
	0x6960, 0x69bf, 0x6a1d, 0x6a7b,
	0x6ad8, 0x6b35, 0x6b91, 0x6bec,
	0x6c47, 0x6ca1, 0x6cfb, 0x6d54,
	0x6dac, 0x6e04, 0x6e5b, 0x6eb1,
	0x6f07, 0x6f5c, 0x6fb0, 0x7004,
	0x7057, 0x70a9, 0x70fb, 0x714c,
	0x719c, 0x71eb, 0x723a, 0x7288,
	0x72d5, 0x7322, 0x736e, 0x73b9,
	0x7403, 0x744d, 0x7495, 0x74dd,
	0x7524, 0x756b, 0x75b0, 0x75f5,
	0x7639, 0x767c, 0x76bf, 0x7700,
	0x7741, 0x7781, 0x77c0, 0x77fe,
	0x783b, 0x7878, 0x78b3, 0x78ee,
	0x7928, 0x7961, 0x7999, 0x79d0,
	0x7a07, 0x7a3c, 0x7a71, 0x7aa5,
	0x7ad7, 0x7b09, 0x7b3a, 0x7b6a,
	0x7b99, 0x7bc8, 0x7bf5, 0x7c21,
	0x7c4d, 0x7c77, 0x7ca1, 0x7cc9,
	0x7cf1, 0x7d18, 0x7d3e, 0x7d62,
	0x7d86, 0x7da9, 0x7dcb, 0x7dec,
	0x7e0c, 0x7e2b, 0x7e49, 0x7e66,
	0x7e82, 0x7e9d, 0x7eb7, 0x7ed1,
	0x7ee9, 0x7f00, 0x7f16, 0x7f2b,
	0x7f3f, 0x7f52, 0x7f65, 0x7f76,
	0x7f86, 0x7f95, 0x7fa3, 0x7fb0,
	0x7fbd, 0x7fc8, 0x7fd2, 0x7fdb,
	0x7fe3, 0x7fea, 0x7ff0, 0x7ff5,
	0x7ff9, 0x7ffc, 0x7ffe, 0x7fff
};

/* *********************************************************** */

const mlib_s16 mlib_s_Kaiser_S16[] = {
/* n = 2 */
	0x7e05,
/* n = 4 */
	0x6fba, 0x7e22,
/* n = 8 */
	0x427c, 0x5dda, 0x72ff, 0x7e83,
/* n = 16 */
	0xe09, 0x1d48, 0x2ff7, 0x4482,
	0x58d8, 0x6ab6, 0x7800, 0x7f17,
/* n = 32 */
	0x60, 0x17f, 0x39f, 0x71b,
	0xc47, 0x135d, 0x1c76, 0x277c,
	0x3421, 0x41e2, 0x500d, 0x5dcb,
	0x6a36, 0x7471, 0x7bba, 0x7f84,
/* n = 64 */
	0x0, 0x0, 0x2, 0x6,
	0xe, 0x20, 0x40, 0x77,
	0xd1, 0x15b, 0x22a, 0x352,
	0x4ed, 0x714, 0x9e4, 0xd77,
	0x11e2, 0x1736, 0x1d7c, 0x24b0,
	0x2cc2, 0x3595, 0x3efc, 0x48bf,
	0x5297, 0x5c37, 0x6549, 0x6d7a,
	0x7479, 0x79fd, 0x7dce, 0x7fc1,
/* n = 128 */
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x1, 0x2, 0x4,
	0x6, 0xa, 0x10, 0x19,
	0x25, 0x37, 0x4f, 0x71,
	0x9e, 0xdb, 0x12b, 0x193,
	0x219, 0x2c3, 0x397, 0x49f,
	0x5e2, 0x769, 0x93e, 0xb68,
	0xdf1, 0x10e0, 0x143c, 0x180a,
	0x1c4c, 0x2104, 0x262f, 0x2bc6,
	0x31c1, 0x3813, 0x3eab, 0x4575,
	0x4c5b, 0x5342, 0x5a0d, 0x60a0,
	0x66db, 0x6ca0, 0x71d1, 0x7654,
	0x7a0f, 0x7cef, 0x7ee3, 0x7fe0,
/* n = 256 */
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x1, 0x1,
	0x2, 0x3, 0x4, 0x5,
	0x7, 0x9, 0xc, 0x10,
	0x16, 0x1c, 0x24, 0x2f,
	0x3b, 0x4b, 0x5f, 0x78,
	0x95, 0xba, 0xe5, 0x11a,
	0x159, 0x1a4, 0x1fd, 0x266,
	0x2e1, 0x370, 0x417, 0x4d7,
	0x5b3, 0x6af, 0x7ce, 0x912,
	0xa7f, 0xc18, 0xde0, 0xfd9,
	0x1205, 0x1468, 0x1702, 0x19d6,
	0x1ce3, 0x202a, 0x23ab, 0x2763,
	0x2b50, 0x2f70, 0x33bf, 0x3837,
	0x3cd2, 0x418b, 0x4659, 0x4b33,
	0x5012, 0x54eb, 0x59b3, 0x5e61,
	0x62e9, 0x6741, 0x6b5d, 0x6f33,
	0x72b8, 0x75e4, 0x78ae, 0x7b0d,
	0x7cfc, 0x7e73, 0x7f70, 0x7ff0,
/* n = 512 */
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x1,
	0x1, 0x1, 0x2, 0x2,
	0x3, 0x3, 0x4, 0x5,
	0x7, 0x8, 0xa, 0xc,
	0xe, 0x11, 0x15, 0x19,
	0x1e, 0x23, 0x2a, 0x32,
	0x3b, 0x45, 0x51, 0x5f,
	0x6f, 0x82, 0x97, 0xaf,
	0xcb, 0xea, 0x10e, 0x136,
	0x164, 0x198, 0x1d2, 0x213,
	0x25c, 0x2ae, 0x309, 0x36e,
	0x3df, 0x45b, 0x4e4, 0x57c,
	0x622, 0x6d8, 0x79f, 0x878,
	0x965, 0xa66, 0xb7c, 0xca9,
	0xded, 0xf49, 0x10be, 0x124e,
	0x13f8, 0x15be, 0x179f, 0x199d,
	0x1bb8, 0x1df0, 0x2044, 0x22b4,
	0x2541, 0x27e9, 0x2aab, 0x2d87,
	0x307c, 0x3387, 0x36a7, 0x39da,
	0x3d1e, 0x4070, 0x43cf, 0x4736,
	0x4aa3, 0x4e13, 0x5182, 0x54ed,
	0x5850, 0x5ba7, 0x5eef, 0x6224,
	0x6541, 0x6844, 0x6b27, 0x6de8,
	0x7083, 0x72f4, 0x7537, 0x774b,
	0x792b, 0x7ad5, 0x7c47, 0x7d7f,
	0x7e7b, 0x7f38, 0x7fb8, 0x7ff8,
/* n = 1024 */
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x1,
	0x1, 0x1, 0x1, 0x1,
	0x2, 0x2, 0x2, 0x3,
	0x3, 0x4, 0x4, 0x5,
	0x6, 0x7, 0x8, 0x9,
	0xa, 0xb, 0xd, 0xf,
	0x11, 0x13, 0x16, 0x18,
	0x1c, 0x1f, 0x23, 0x27,
	0x2c, 0x32, 0x38, 0x3f,
	0x46, 0x4f, 0x58, 0x62,
	0x6d, 0x79, 0x87, 0x96,
	0xa6, 0xb8, 0xcc, 0xe2,
	0xf9, 0x113, 0x130, 0x14e,
	0x170, 0x194, 0x1bc, 0x1e7,
	0x215, 0x248, 0x27e, 0x2b9,
	0x2f9, 0x33d, 0x387, 0x3d6,
	0x42b, 0x486, 0x4e8, 0x551,
	0x5c1, 0x638, 0x6b8, 0x740,
	0x7d0, 0x86a, 0x90d, 0x9ba,
	0xa71, 0xb33, 0xbff, 0xcd7,
	0xdbb, 0xeab, 0xfa6, 0x10af,
	0x11c4, 0x12e7, 0x1417, 0x1555,
	0x16a0, 0x17fa, 0x1962, 0x1ad8,
	0x1c5c, 0x1def, 0x1f90, 0x213f,
	0x22fc, 0x24c7, 0x26a0, 0x2887,
	0x2a7b, 0x2c7b, 0x2e88, 0x30a1,
	0x32c5, 0x34f4, 0x372d, 0x396f,
	0x3bba, 0x3e0d, 0x4066, 0x42c6,
	0x452a, 0x4793, 0x49fe, 0x4c6c,
	0x4ed9, 0x5146, 0x53b1, 0x5619,
	0x587c, 0x5ada, 0x5d30, 0x5f7e,
	0x61c2, 0x63fa, 0x6626, 0x6844,
	0x6a52, 0x6c50, 0x6e3b, 0x7013,
	0x71d7, 0x7385, 0x751c, 0x769a,
	0x7800, 0x794c, 0x7a7c, 0x7b91,
	0x7c89, 0x7d63, 0x7e20, 0x7ebd,
	0x7f3c, 0x7f9c, 0x7fdc, 0x7ffc
};
