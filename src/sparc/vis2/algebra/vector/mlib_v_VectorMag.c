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

#pragma ident	"@(#)mlib_v_VectorMag.c	9.3	07/11/05 SMI"

/*
 * FUNCTIONS
 *      mlib_VectorMag_S8C - magnitude of mlib_u32 8-bit format
 *                          complex vector
 *      mlib_VectorMag_S8C - magnitude of mlib_s32 8-bit format
 *                           complex vector
 *      mlib_VectorMag_S16C - magnitude of mlib_s32 16-bit format
 *                            complex vector
 *      mlib_VectorMag_S32C - magnitude of mlib_s32 32-bit format
 *                            complex vector
 *
 * SYNOPSIS
 *
 *      mlib_status mlib_VectorMag_U8C(mlib_d64      *m,
 *                                     const mlib_u8 *x,
 *                                     mlib_s32      n);
 *      mlib_status mlib_VectorMag_S8C(mlib_d64      *m,
 *                                     const mlib_s8 *x,
 *                                     mlib_s32      n);
 *      mlib_status mlib_VectorMag_S16C(mlib_d64       * m,
 *                                      const mlib_s16 * x,
 *                                      mlib_s32       n);
 *      mlib_status mlib_VectorMag_S32C(mlib_d64       *m,
 *                                      const mlib_s32 *x,
 *                                      mlib_s32       n);
 *
 * ARGUMENTS
 *      m    pointer to the output magnitude vector
 *      x    pointer to the first complex element of the input vector.
 *           x[2*i] hold the real parts, and x[2*i + 1] hold the imaginary
 *           parts
 *      n    number of complex elements in the x vector
 *
 * DESCRIPTION
 *      m = sqrt(x_imag * x_imag + x_real * x_real)
 */

#include <mlib_algebra.h>
#include <vis_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_VectorMag_S8C = __mlib_VectorMag_S8C
#pragma weak mlib_VectorMag_U8C = __mlib_VectorMag_U8C
#pragma weak mlib_VectorMag_S32C = __mlib_VectorMag_S32C
#pragma weak mlib_VectorMag_S16C = __mlib_VectorMag_S16C

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */

__typeof__(__mlib_VectorMag_S8C) mlib_VectorMag_S8C
	__attribute__((weak, alias("__mlib_VectorMag_S8C")));
__typeof__(__mlib_VectorMag_U8C) mlib_VectorMag_U8C
	__attribute__((weak, alias("__mlib_VectorMag_U8C")));
__typeof__(__mlib_VectorMag_S32C) mlib_VectorMag_S32C
	__attribute__((weak, alias("__mlib_VectorMag_S32C")));
__typeof__(__mlib_VectorMag_S16C) mlib_VectorMag_S16C
	__attribute__((weak, alias("__mlib_VectorMag_S16C")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

/*  Table for vsqrtf (soft version of sqrtf)  */

static const mlib_s64 arr[] = {
	0x0ULL, 0x0ULL,
	0x3ff0000000000000ULL, 0x3fe6a09e667f3bccULL,
	0x3fefe01fe01fe020ULL, 0x3fe6abebe307d6d9ULL,
	0x3fefc07f01fc07f0ULL, 0x3fe6b733bfd8c647ULL,
	0x3fefa11caa01fa12ULL, 0x3fe6c276054ecf79ULL,
	0x3fef81f81f81f820ULL, 0x3fe6cdb2bbb212eaULL,
	0x3fef6310aca0dbb5ULL, 0x3fe6d8e9eb365353ULL,
	0x3fef44659e4a4271ULL, 0x3fe6e41b9bfb3b75ULL,
	0x3fef25f644230ab5ULL, 0x3fe6ef47d60ca2c6ULL,
	0x3fef07c1f07c1f08ULL, 0x3fe6fa6ea162d0f0ULL,
	0x3feee9c7f8458e02ULL, 0x3fe7059005e2c01dULL,
	0x3feecc07b301ecc0ULL, 0x3fe710ac0b5e5e31ULL,
	0x3feeae807aba01ebULL, 0x3fe71bc2b994cce2ULL,
	0x3fee9131abf0b767ULL, 0x3fe726d41832a0beULL,
	0x3fee741aa59750e4ULL, 0x3fe731e02ed21f21ULL,
	0x3fee573ac901e574ULL, 0x3fe73ce704fb7b23ULL,
	0x3fee3a9179dc1a73ULL, 0x3fe747e8a2251188ULL,
	0x3fee1e1e1e1e1e1eULL, 0x3fe752e50db3a3a1ULL,
	0x3fee01e01e01e01eULL, 0x3fe75ddc4efa914aULL,
	0x3fede5d6e3f8868aULL, 0x3fe768ce6d3c11dfULL,
	0x3fedca01dca01dcaULL, 0x3fe773bb6fa96c51ULL,
	0x3fedae6076b981dbULL, 0x3fe77ea35d632e43ULL,
	0x3fed92f2231e7f8aULL, 0x3fe789863d796253ULL,
	0x3fed77b654b82c34ULL, 0x3fe7946416ebc56bULL,
	0x3fed5cac807572b2ULL, 0x3fe79f3cf0a9fb4eULL,
	0x3fed41d41d41d41dULL, 0x3fe7aa10d193c22dULL,
	0x3fed272ca3fc5b1aULL, 0x3fe7b4dfc079258dULL,
	0x3fed0cb58f6ec074ULL, 0x3fe7bfa9c41ab03fULL,
	0x3fecf26e5c44bfc6ULL, 0x3fe7ca6ee3299d9bULL,
	0x3fecd85689039b0bULL, 0x3fe7d52f244809e9ULL,
	0x3fecbe6d9601cbe7ULL, 0x3fe7dfea8e092212ULL,
	0x3feca4b3055ee191ULL, 0x3fe7eaa126f15283ULL,
	0x3fec8b265afb8a42ULL, 0x3fe7f552f5767564ULL,
	0x3fec71c71c71c71cULL, 0x3fe7ffffffffffffULL,
	0x3fec5894d10d4986ULL, 0x3fe80aa84ce72f89ULL,
	0x3fec3f8f01c3f8f0ULL, 0x3fe8154be2773526ULL,
	0x3fec26b5392ea01cULL, 0x3fe81feac6ed6149ULL,
	0x3fec0e070381c0e0ULL, 0x3fe82a8500794e6bULL,
	0x3febf583ee868d8bULL, 0x3fe8351a953d0b0aULL,
	0x3febdd2b899406f7ULL, 0x3fe83fab8b4d4315ULL,
	0x3febc4fd65883e7bULL, 0x3fe84a37e8b168a8ULL,
	0x3febacf914c1bad0ULL, 0x3fe854bfb363dc39ULL,
	0x3feb951e2b18ff23ULL, 0x3fe85f42f1521411ULL,
	0x3feb7d6c3dda338bULL, 0x3fe869c1a85cc346ULL,
	0x3feb65e2e3beee05ULL, 0x3fe8743bde58000cULL,
	0x3feb4e81b4e81b4fULL, 0x3fe87eb1990b697aULL,
	0x3feb37484ad806ceULL, 0x3fe88922de324cbeULL,
	0x3feb2036406c80d9ULL, 0x3fe8938fb37bc9c0ULL,
	0x3feb094b31d922a4ULL, 0x3fe89df81e8af73fULL,
	0x3feaf286bca1af28ULL, 0x3fe8a85c24f7065aULL,
	0x3feadbe87f94905eULL, 0x3fe8b2bbcc4b659cULL,
	0x3feac5701ac5701bULL, 0x3fe8bd171a07e38aULL,
	0x3feaaf1d2f87ebfdULL, 0x3fe8c76e13a0d095ULL,
	0x3fea98ef606a63beULL, 0x3fe8d1c0be7f20acULL,
	0x3fea82e65130e159ULL, 0x3fe8dc0f20008c3aULL,
	0x3fea6d01a6d01a6dULL, 0x3fe8e6593d77b0b7ULL,
	0x3fea574107688a4aULL, 0x3fe8f09f1c2c30b8ULL,
	0x3fea41a41a41a41aULL, 0x3fe8fae0c15ad38aULL,
	0x3fea2c2a87c51ca0ULL, 0x3fe9051e3235a457ULL,
	0x3fea16d3f97a4b02ULL, 0x3fe90f5773e410e3ULL,
	0x3fea01a01a01a01aULL, 0x3fe9198c8b8307c8ULL,
	0x3fe9ec8e951033d9ULL, 0x3fe923bd7e25164cULL,
	0x3fe9d79f176b682dULL, 0x3fe92dea50d285d0ULL,
	0x3fe9c2d14ee4a102ULL, 0x3fe93813088978c4ULL,
	0x3fe9ae24ea5510daULL, 0x3fe94237aa3e0749ULL,
	0x3fe999999999999aULL, 0x3fe94c583ada5b52ULL,
	0x3fe9852f0d8ec0ffULL, 0x3fe95674bf3ecc74ULL,
	0x3fe970e4f80cb872ULL, 0x3fe9608d3c41fb4bULL,
	0x3fe95cbb0be377aeULL, 0x3fe96aa1b6b0ec77ULL,
	0x3fe948b0fcd6e9e0ULL, 0x3fe974b2334f2346ULL,
	0x3fe934c67f9b2ce6ULL, 0x3fe97ebeb6d6bbf3ULL,
	0x3fe920fb49d0e229ULL, 0x3fe988c745f88592ULL,
	0x3fe90d4f120190d5ULL, 0x3fe992cbe55c1b94ULL,
	0x3fe8f9c18f9c18faULL, 0x3fe99ccc999fff00ULL,
	0x3fe8e6527af1373fULL, 0x3fe9a6c96759af48ULL,
	0x3fe8d3018d3018d3ULL, 0x3fe9b0c25315c2cdULL,
	0x3fe8bfce8062ff3aULL, 0x3fe9bab76157ff0dULL,
	0x3fe8acb90f6bf3aaULL, 0x3fe9c4a8969b7076ULL,
	0x3fe899c0f601899cULL, 0x3fe9ce95f75281f5ULL,
	0x3fe886e5f0abb04aULL, 0x3fe9d87f87e71422ULL,
	0x3fe87427bcc092b9ULL, 0x3fe9e2654cba9429ULL,
	0x3fe8618618618618ULL, 0x3fe9ec474a261263ULL,
	0x3fe84f00c2780614ULL, 0x3fe9f625847a5898ULL,
	0x3fe83c977ab2beddULL, 0x3fe9ffffffffffffULL,
	0x3fe82a4a0182a4a0ULL, 0x3fea09d6c0f786f1ULL,
	0x3fe8181818181818ULL, 0x3fea13a9cb996650ULL,
	0x3fe8060180601806ULL, 0x3fea1d79241626a3ULL,
	0x3fe7f405fd017f40ULL, 0x3fea2744ce9674f4ULL,
	0x3fe7e225515a4f1dULL, 0x3fea310ccf3b3760ULL,
	0x3fe7d05f417d05f4ULL, 0x3fea3ad12a1da160ULL,
	0x3fe7beb3922e017cULL, 0x3fea4491e34f47d9ULL,
	0x3fe7ad2208e0ecc3ULL, 0x3fea4e4efeda34deULL,
	0x3fe79baa6bb6398bULL, 0x3fea580880c0fb33ULL,
	0x3fe78a4c8178a4c8ULL, 0x3fea61be6cfec997ULL,
	0x3fe77908119ac60dULL, 0x3fea6b70c7877dbfULL,
	0x3fe767dce434a9b1ULL, 0x3fea751f9447b724ULL,
	0x3fe756cac201756dULL, 0x3fea7ecad724e984ULL,
	0x3fe745d1745d1746ULL, 0x3fea887293fd6f34ULL,
	0x3fe734f0c541fe8dULL, 0x3fea9216cea89b23ULL,
	0x3fe724287f46debcULL, 0x3fea9bb78af6cabbULL,
	0x3fe713786d9c7c09ULL, 0x3feaa554ccb17776ULL,
	0x3fe702e05c0b8170ULL, 0x3feaaeee979b4837ULL,
	0x3fe6f26016f26017ULL, 0x3feab884ef702284ULL,
	0x3fe6e1f76b4337c7ULL, 0x3feac217d7e53b66ULL,
	0x3fe6d1a62681c861ULL, 0x3feacba754a9282eULL,
	0x3fe6c16c16c16c17ULL, 0x3fead5336963eefbULL,
	0x3fe6b1490aa31a3dULL, 0x3feadebc19b71702ULL,
	0x3fe6a13cd1537290ULL, 0x3feae841693db8b3ULL,
	0x3fe691473a88d0c0ULL, 0x3feaf1c35b8c8d9fULL,
	0x3fe6816816816817ULL, 0x3feafb41f432002dULL,
	0x3fe6719f3601671aULL, 0x3feb04bd36b63b22ULL,
	0x3fe661ec6a5122f9ULL, 0x3feb0e35269b38f5ULL,
	0x3fe6524f853b4aa3ULL, 0x3feb17a9c75cd2eeULL,
	0x3fe642c8590b2164ULL, 0x3feb211b1c70d022ULL,
	0x3fe63356b88ac0deULL, 0x3feb2a892946f435ULL,
	0x3fe623fa77016240ULL, 0x3feb33f3f1490deeULL,
	0x3fe614b36831ae94ULL, 0x3feb3d5b77db05a8ULL,
	0x3fe6058160581606ULL, 0x3feb46bfc05aeb89ULL,
	0x3fe5f66434292dfcULL, 0x3feb5020ce210599ULL,
	0x3fe5e75bb8d015e7ULL, 0x3feb597ea47fdda2ULL,
	0x3fe5d867c3ece2a5ULL, 0x3feb62d946c44ef2ULL,
	0x3fe5c9882b931057ULL, 0x3feb6c30b83593e6ULL,
	0x3fe5babcc647fa91ULL, 0x3feb7584fc155353ULL,
	0x3fe5ac056b015ac0ULL, 0x3feb7ed6159fadc7ULL,
	0x3fe59d61f123ccaaULL, 0x3feb8824080b4a9dULL,
	0x3fe58ed2308158edULL, 0x3feb916ed68964ebULL,
	0x3fe5805601580560ULL, 0x3feb9ab68445d84cULL,
	0x3fe571ed3c506b3aULL, 0x3feba3fb14672d7cULL,
	0x3fe56397ba7c52e2ULL, 0x3febad3c8a0ea6d5ULL,
	0x3fe5555555555555ULL, 0x3febb67ae8584ca9ULL,
	0x3fe54725e6bb82feULL, 0x3febbfb6325af96aULL,
	0x3fe5390948f40febULL, 0x3febc8ee6b2865b8ULL,
	0x3fe52aff56a8054bULL, 0x3febd22395cd344dULL,
	0x3fe51d07eae2f815ULL, 0x3febdb55b550fdbbULL,
	0x3fe50f22e111c4c5ULL, 0x3febe484ccb65c14ULL,
	0x3fe5015015015015ULL, 0x3febedb0defaf660ULL,
	0x3fe4f38f62dd4c9bULL, 0x3febf6d9ef178c08ULL,
	0x3fe4e5e0a72f0539ULL, 0x3febffffffffffffULL,
	0x3fe4d843bedc2c4cULL, 0x3fec092314a363f0ULL,
	0x3fe4cab88725af6eULL, 0x3fec12432fec0329ULL,
	0x3fe4bd3edda68fe1ULL, 0x3fec1b6054bf6d7cULL,
	0x3fe4afd6a052bf5bULL, 0x3fec247a85fe81faULL,
	0x3fe4a27fad76014aULL, 0x3fec2d91c6857989ULL,
	0x3fe49539e3b2d067ULL, 0x3fec36a6192bf167ULL,
	0x3fe4880522014880ULL, 0x3fec3fb780c4f583ULL,
	0x3fe47ae147ae147bULL, 0x3fec48c6001f0abfULL,
	0x3fe46dce34596066ULL, 0x3fec51d19a043914ULL,
	0x3fe460cbc7f5cf9aULL, 0x3fec5ada513a1593ULL,
	0x3fe453d9e2c776caULL, 0x3fec63e02881cc54ULL,
	0x3fe446f86562d9fbULL, 0x3fec6ce322982a3eULL,
	0x3fe43a2730abee4dULL, 0x3fec75e34235a6bdULL,
	0x3fe42d6625d51f87ULL, 0x3fec7ee08a0e6d4dULL,
	0x3fe420b5265e5951ULL, 0x3fec87dafcd266fdULL,
	0x3fe4141414141414ULL, 0x3fec90d29d2d43cdULL,
	0x3fe40782d10e6566ULL, 0x3fec99c76dc683f0ULL,
	0x3fe3fb013fb013fbULL, 0x3feca2b9714180f6ULL,
	0x3fe3ee8f42a5af07ULL, 0x3fecaba8aa3d76e1ULL,
	0x3fe3e22cbce4a902ULL, 0x3fecb4951b558d16ULL,
	0x3fe3d5d991aa75c6ULL, 0x3fecbd7ec720df44ULL,
	0x3fe3c995a47babe7ULL, 0x3fecc665b0328621ULL,
	0x3fe3bd60d9232955ULL, 0x3feccf49d919a01cULL,
	0x3fe3b13b13b13b14ULL, 0x3fecd82b446159f2ULL,
	0x3fe3a524387ac822ULL, 0x3fece109f490f72cULL,
	0x3fe3991c2c187f63ULL, 0x3fece9e5ec2bda7fULL,
	0x3fe38d22d366088eULL, 0x3fecf2bf2db18e25ULL,
	0x3fe3813813813814ULL, 0x3fecfb95bb9dcc0bULL,
	0x3fe3755bd1c945eeULL, 0x3fed0469986885fdULL,
	0x3fe3698df3de0748ULL, 0x3fed0d3ac685eda4ULL,
	0x3fe35dce5f9f2af8ULL, 0x3fed160948667c88ULL,
	0x3fe3521cfb2b78c1ULL, 0x3fed1ed52076fbe9ULL,
	0x3fe34679ace01346ULL, 0x3fed279e51208c8aULL,
	0x3fe33ae45b57bcb2ULL, 0x3fed3064dcc8ae67ULL,
	0x3fe32f5ced6a1dfaULL, 0x3fed3928c5d14856ULL,
	0x3fe323e34a2b10bfULL, 0x3fed41ea0e98af91ULL,
	0x3fe3187758e9ebb6ULL, 0x3fed4aa8b979af2fULL,
	0x3fe30d190130d190ULL, 0x3fed5364c8cb8f85ULL,
	0x3fe301c82ac40260ULL, 0x3fed5c1e3ee21d7dULL,
	0x3fe2f684bda12f68ULL, 0x3fed64d51e0db1c5ULL,
	0x3fe2eb4ea1fed14bULL, 0x3fed6d89689b3806ULL,
	0x3fe2e025c04b8097ULL, 0x3fed763b20d435efULL,
	0x3fe2d50a012d50a0ULL, 0x3fed7eea48fed23fULL,
	0x3fe2c9fb4d812ca0ULL, 0x3fed8796e35ddbb2ULL,
	0x3fe2bef98e5a3711ULL, 0x3fed9040f230cfdfULL,
	0x3fe2b404ad012b40ULL, 0x3fed98e877b3e206ULL,
	0x3fe2a91c92f3c105ULL, 0x3feda18d762001cbULL,
	0x3fe29e4129e4129eULL, 0x3fedaa2fefaae1d7ULL,
	0x3fe293725bb804a5ULL, 0x3fedb2cfe686fe7cULL,
	0x3fe288b01288b013ULL, 0x3fedbb6d5ce3a42eULL,
	0x3fe27dfa38a1ce4dULL, 0x3fedc40854ecf600ULL,
	0x3fe27350b8812735ULL, 0x3fedcca0d0cbf407ULL,
	0x3fe268b37cd60127ULL, 0x3fedd536d2a681abULL,
	0x3fe25e22708092f1ULL, 0x3fedddca5c9f6be7ULL,
	0x3fe2539d7e9177b2ULL, 0x3fede65b70d66f8aULL,
	0x3fe2492492492492ULL, 0x3fedeeea11683f49ULL,
	0x3fe23eb79717605bULL, 0x3fedf776406e89dcULL,
	0x3fe23456789abcdfULL, 0x3fee000000000000ULL,
	0x3fe22a0122a0122aULL, 0x3fee088752305a6aULL,
	0x3fe21fb78121fb78ULL, 0x3fee110c39105fafULL,
	0x3fe21579804855e6ULL, 0x3fee198eb6adea15ULL,
	0x3fe20b470c67c0d9ULL, 0x3fee220ecd13ed5fULL,
	0x3fe2012012012012ULL, 0x3fee2a8c7e4a7c85ULL,
	0x3fe1f7047dc11f70ULL, 0x3fee3307cc56cf5bULL,
	0x3fe1ecf43c7fb84cULL, 0x3fee3b80b93b4830ULL,
	0x3fe1e2ef3b3fb874ULL, 0x3fee43f746f7795aULL,
	0x3fe1d8f5672e4abdULL, 0x3fee4c6b77882ab7ULL,
	0x3fe1cf06ada2811dULL, 0x3fee54dd4ce75f1dULL,
	0x3fe1c522fc1ce059ULL, 0x3fee5d4cc90c59c2ULL,
	0x3fe1bb4a4046ed29ULL, 0x3fee65b9edeba38dULL,
	0x3fe1b17c67f2bae3ULL, 0x3fee6e24bd771066ULL,
	0x3fe1a7b9611a7b96ULL, 0x3fee768d399dc46fULL,
	0x3fe19e0119e0119eULL, 0x3fee7ef3644c3931ULL,
	0x3fe19453808ca29cULL, 0x3fee87573f6c42c4ULL,
	0x3fe18ab083902bdbULL, 0x3fee8fb8cce514e3ULL,
	0x3fe1811811811812ULL, 0x3fee98180e9b47f2ULL,
	0x3fe1778a191bd684ULL, 0x3feea0750670ddfbULL,
	0x3fe16e0689427379ULL, 0x3feea8cfb64547abULL,
	0x3fe1648d50fc3201ULL, 0x3feeb1281ff56922ULL,
	0x3fe15b1e5f75270dULL, 0x3feeb97e455b9edbULL,
	0x3fe151b9a3fdd5c9ULL, 0x3feec1d2284fc276ULL,
	0x3fe1485f0e0acd3bULL, 0x3feeca23caa72f72ULL,
	0x3fe13f0e8d344724ULL, 0x3feed2732e34c7f0ULL,
	0x3fe135c81135c811ULL, 0x3feedac054c8f94bULL,
	0x3fe12c8b89edc0acULL, 0x3feee30b4031c0cbULL,
	0x3fe12358e75d3033ULL, 0x3feeeb53f23ab028ULL,
	0x3fe11a3019a74826ULL, 0x3feef39a6cacf21aULL,
	0x3fe1111111111111ULL, 0x3feefbdeb14f4ed9ULL,
	0x3fe107fbbe011080ULL, 0x3fef0420c1e6308cULL,
	0x3fe0fef010fef011ULL, 0x3fef0c60a033a7b2ULL,
	0x3fe0f5edfab325a2ULL, 0x3fef149e4df76f81ULL,
	0x3fe0ecf56be69c90ULL, 0x3fef1cd9cceef239ULL,
	0x3fe0e40655826011ULL, 0x3fef25131ed54d6bULL,
	0x3fe0db20a88f4696ULL, 0x3fef2d4a4563563fULL,
	0x3fe0d24456359e3aULL, 0x3fef357f424f9d9eULL,
	0x3fe0c9714fbcda3bULL, 0x3fef3db2174e7468ULL,
	0x3fe0c0a7868b4171ULL, 0x3fef45e2c611ef8cULL,
	0x3fe0b7e6ec259dc8ULL, 0x3fef4e115049ec25ULL,
	0x3fe0af2f722eecb5ULL, 0x3fef563db7a41388ULL,
	0x3fe0a6810a6810a7ULL, 0x3fef5e67fdcbdf43ULL,
	0x3fe09ddba6af8360ULL, 0x3fef6690246a9d1fULL,
	0x3fe0953f39010954ULL, 0x3fef6eb62d27730cULL,
	0x3fe08cabb37565e2ULL, 0x3fef76da19a7630cULL,
	0x3fe0842108421084ULL, 0x3fef7efbeb8d4f11ULL,
	0x3fe07b9f29b8eae2ULL, 0x3fef871ba479fcd6ULL,
	0x3fe073260a47f7c6ULL, 0x3fef8f39460c19a8ULL,
	0x3fe06ab59c7912fbULL, 0x3fef9754d1e03e2eULL,
	0x3fe0624dd2f1a9fcULL, 0x3fef9f6e4990f227ULL,
	0x3fe059eea0727586ULL, 0x3fefa785aeb6b015ULL,
	0x3fe05197f7d73404ULL, 0x3fefaf9b02e7e8f1ULL,
	0x3fe04949cc1664c5ULL, 0x3fefb7ae47b907cbULL,
	0x3fe0410410410410ULL, 0x3fefbfbf7ebc755eULL,
	0x3fe038c6b78247fcULL, 0x3fefc7cea9829babULL,
	0x3fe03091b51f5e1aULL, 0x3fefcfdbc999e97dULL,
	0x3fe02864fc7729e9ULL, 0x3fefd7e6e08ed5eaULL,
	0x3fe0204081020408ULL, 0x3fefdfefefebe3d4ULL,
	0x3fe0182436517a37ULL, 0x3fefe7f6f939a553ULL,
	0x3fe0101010101010ULL, 0x3fefeffbfdfebf1eULL,
	0x3fe0080402010080ULL, 0x3feff7feffbfebf8ULL,
	0x3ff0000000000000ULL, 0x3ff0000000000000ULL,
	0x3fefe01fe01fe020ULL, 0x3ff007fe00ff6070ULL,
	0x3fefc07f01fc07f0ULL, 0x3ff00ff807f60debULL,
	0x3fefa11caa01fa12ULL, 0x3ff017ee1acdc963ULL,
	0x3fef81f81f81f820ULL, 0x3ff01fe03f61bad0ULL,
	0x3fef6310aca0dbb5ULL, 0x3ff027ce7b7ea376ULL,
	0x3fef44659e4a4271ULL, 0x3ff02fb8d4e30f48ULL,
	0x3fef25f644230ab5ULL, 0x3ff0379f513f8570ULL,
	0x3fef07c1f07c1f08ULL, 0x3ff03f81f636b80cULL,
	0x3feee9c7f8458e02ULL, 0x3ff04760c95db310ULL,
	0x3feecc07b301ecc0ULL, 0x3ff04f3bd03c0a64ULL,
	0x3feeae807aba01ebULL, 0x3ff05713104c0736ULL,
	0x3fee9131abf0b767ULL, 0x3ff05ee68efad48bULL,
	0x3fee741aa59750e4ULL, 0x3ff066b651a8ab0fULL,
	0x3fee573ac901e574ULL, 0x3ff06e825da8fc2bULL,
	0x3fee3a9179dc1a73ULL, 0x3ff0764ab8429c66ULL,
	0x3fee1e1e1e1e1e1eULL, 0x3ff07e0f66afed07ULL,
	0x3fee01e01e01e01eULL, 0x3ff085d06e1f0517ULL,
	0x3fede5d6e3f8868aULL, 0x3ff08d8dd3b1d9aaULL,
	0x3fedca01dca01dcaULL, 0x3ff095479c7e6581ULL,
	0x3fedae6076b981dbULL, 0x3ff09cfdcd8ed009ULL,
	0x3fed92f2231e7f8aULL, 0x3ff0a4b06be193b9ULL,
	0x3fed77b654b82c34ULL, 0x3ff0ac5f7c69a3c8ULL,
	0x3fed5cac807572b2ULL, 0x3ff0b40b040e9153ULL,
	0x3fed41d41d41d41dULL, 0x3ff0bbb307acafdbULL,
	0x3fed272ca3fc5b1aULL, 0x3ff0c3578c15393eULL,
	0x3fed0cb58f6ec074ULL, 0x3ff0caf8960e710dULL,
	0x3fecf26e5c44bfc6ULL, 0x3ff0d2962a53c75eULL,
	0x3fecd85689039b0bULL, 0x3ff0da304d95fb06ULL,
	0x3fecbe6d9601cbe7ULL, 0x3ff0e1c7047b3b51ULL,
	0x3feca4b3055ee191ULL, 0x3ff0e95a539f492cULL,
	0x3fec8b265afb8a42ULL, 0x3ff0f0ea3f9397ceULL,
	0x3fec71c71c71c71cULL, 0x3ff0f876ccdf6cd9ULL,
	0x3fec5894d10d4986ULL, 0x3ff1000000000000ULL,
	0x3fec3f8f01c3f8f0ULL, 0x3ff10785dd689a29ULL,
	0x3fec26b5392ea01cULL, 0x3ff10f086982b418ULL,
	0x3fec0e070381c0e0ULL, 0x3ff11687a8ae14a3ULL,
	0x3febf583ee868d8bULL, 0x3ff11e039f40ee66ULL,
	0x3febdd2b899406f7ULL, 0x3ff1257c5187fd09ULL,
	0x3febc4fd65883e7bULL, 0x3ff12cf1c3c6a213ULL,
	0x3febacf914c1bad0ULL, 0x3ff13463fa37014eULL,
	0x3feb951e2b18ff23ULL, 0x3ff13bd2f90a1cb4ULL,
	0x3feb7d6c3dda338bULL, 0x3ff1433ec467effbULL,
	0x3feb65e2e3beee05ULL, 0x3ff14aa7606f8bb0ULL,
	0x3feb4e81b4e81b4fULL, 0x3ff1520cd1372febULL,
	0x3feb37484ad806ceULL, 0x3ff1596f1acc669bULL,
	0x3feb2036406c80d9ULL, 0x3ff160ce41341d74ULL,
	0x3feb094b31d922a4ULL, 0x3ff1682a486abf71ULL,
	0x3feaf286bca1af28ULL, 0x3ff16f8334644df9ULL,
	0x3feadbe87f94905eULL, 0x3ff176d9090c79a8ULL,
	0x3feac5701ac5701bULL, 0x3ff17e2bca46bab9ULL,
	0x3feaaf1d2f87ebfdULL, 0x3ff1857b7bee690dULL,
	0x3fea98ef606a63beULL, 0x3ff18cc821d6d3e3ULL,
	0x3fea82e65130e159ULL, 0x3ff19411bfcb592fULL,
	0x3fea6d01a6d01a6dULL, 0x3ff19b58598f7c9fULL,
	0x3fea574107688a4aULL, 0x3ff1a29bf2defe49ULL,
	0x3fea41a41a41a41aULL, 0x3ff1a9dc8f6df104ULL,
	0x3fea2c2a87c51ca0ULL, 0x3ff1b11a32e8d06cULL,
	0x3fea16d3f97a4b02ULL, 0x3ff1b854e0f496a0ULL,
	0x3fea01a01a01a01aULL, 0x3ff1bf8c9d2ed1a2ULL,
	0x3fe9ec8e951033d9ULL, 0x3ff1c6c16b2db870ULL,
	0x3fe9d79f176b682dULL, 0x3ff1cdf34e803fd5ULL,
	0x3fe9c2d14ee4a102ULL, 0x3ff1d5224aae2ee1ULL,
	0x3fe9ae24ea5510daULL, 0x3ff1dc4e63383328ULL,
	0x3fe999999999999aULL, 0x3ff1e3779b97f4a8ULL,
	0x3fe9852f0d8ec0ffULL, 0x3ff1ea9df740296fULL,
	0x3fe970e4f80cb872ULL, 0x3ff1f1c1799ca8ffULL,
	0x3fe95cbb0be377aeULL, 0x3ff1f8e226127f61ULL,
	0x3fe948b0fcd6e9e0ULL, 0x3ff2000000000000ULL,
	0x3fe934c67f9b2ce6ULL, 0x3ff2071b0abcd838ULL,
	0x3fe920fb49d0e229ULL, 0x3ff20e33499a21a9ULL,
	0x3fe90d4f120190d5ULL, 0x3ff21548bfe27445ULL,
	0x3fe8f9c18f9c18faULL, 0x3ff21c5b70d9f824ULL,
	0x3fe8e6527af1373fULL, 0x3ff2236b5fbe7711ULL,
	0x3fe8d3018d3018d3ULL, 0x3ff22a788fc76de5ULL,
	0x3fe8bfce8062ff3aULL, 0x3ff2318304261d9aULL,
	0x3fe8acb90f6bf3aaULL, 0x3ff2388ac0059c28ULL,
	0x3fe899c0f601899cULL, 0x3ff23f8fc68ae52bULL,
	0x3fe886e5f0abb04aULL, 0x3ff246921ad4ea49ULL,
	0x3fe87427bcc092b9ULL, 0x3ff24d91bffca360ULL,
	0x3fe8618618618618ULL, 0x3ff2548eb9151e85ULL,
	0x3fe84f00c2780614ULL, 0x3ff25b89092b8fbfULL,
	0x3fe83c977ab2beddULL, 0x3ff26280b3476096ULL,
	0x3fe82a4a0182a4a0ULL, 0x3ff26975ba6a3f6bULL,
	0x3fe8181818181818ULL, 0x3ff2706821902e9aULL,
	0x3fe8060180601806ULL, 0x3ff27757ebaf9368ULL,
	0x3fe7f405fd017f40ULL, 0x3ff27e451bb944c3ULL,
	0x3fe7e225515a4f1dULL, 0x3ff2852fb49899cdULL,
	0x3fe7d05f417d05f4ULL, 0x3ff28c17b9337834ULL,
	0x3fe7beb3922e017cULL, 0x3ff292fd2c6a6262ULL,
	0x3fe7ad2208e0ecc3ULL, 0x3ff299e011188575ULL,
	0x3fe79baa6bb6398bULL, 0x3ff2a0c06a13c70bULL,
	0x3fe78a4c8178a4c8ULL, 0x3ff2a79e3a2cd2e6ULL,
	0x3fe77908119ac60dULL, 0x3ff2ae79842f2858ULL,
	0x3fe767dce434a9b1ULL, 0x3ff2b5524ae1278eULL,
	0x3fe756cac201756dULL, 0x3ff2bc2891041ea7ULL,
	0x3fe745d1745d1746ULL, 0x3ff2c2fc595456a7ULL,
	0x3fe734f0c541fe8dULL, 0x3ff2c9cda6892035ULL,
	0x3fe724287f46debcULL, 0x3ff2d09c7b54e03eULL,
	0x3fe713786d9c7c09ULL, 0x3ff2d768da651c63ULL,
	0x3fe702e05c0b8170ULL, 0x3ff2de32c6628741ULL,
	0x3fe6f26016f26017ULL, 0x3ff2e4fa41f10c9bULL,
	0x3fe6e1f76b4337c7ULL, 0x3ff2ebbf4fafdd4bULL,
	0x3fe6d1a62681c861ULL, 0x3ff2f281f2397b1dULL,
	0x3fe6c16c16c16c17ULL, 0x3ff2f9422c23c47eULL,
	0x3fe6b1490aa31a3dULL, 0x3ff3000000000000ULL,
	0x3fe6a13cd1537290ULL, 0x3ff306bb705ae7c3ULL,
	0x3fe691473a88d0c0ULL, 0x3ff30d747fbcb4b5ULL,
	0x3fe6816816816817ULL, 0x3ff3142b30a929abULL,
	0x3fe6719f3601671aULL, 0x3ff31adf859f9e5eULL,
	0x3fe661ec6a5122f9ULL, 0x3ff32191811b0a41ULL,
	0x3fe6524f853b4aa3ULL, 0x3ff3284125920f33ULL,
	0x3fe642c8590b2164ULL, 0x3ff32eee75770416ULL,
	0x3fe63356b88ac0deULL, 0x3ff335997337ff40ULL,
	0x3fe623fa77016240ULL, 0x3ff33c42213ee0c9ULL,
	0x3fe614b36831ae94ULL, 0x3ff342e881f15cc2ULL,
	0x3fe6058160581606ULL, 0x3ff3498c97b10540ULL,
	0x3fe5f66434292dfcULL, 0x3ff3502e64db5456ULL,
	0x3fe5e75bb8d015e7ULL, 0x3ff356cdebc9b5e2ULL,
	0x3fe5d867c3ece2a5ULL, 0x3ff35d6b2ed19148ULL,
	0x3fe5c9882b931057ULL, 0x3ff3640630445306ULL,
	0x3fe5babcc647fa91ULL, 0x3ff36a9ef26f762fULL,
	0x3fe5ac056b015ac0ULL, 0x3ff37135779c8dcbULL,
	0x3fe59d61f123ccaaULL, 0x3ff377c9c2114e15ULL,
	0x3fe58ed2308158edULL, 0x3ff37e5bd40f95a1ULL,
	0x3fe5805601580560ULL, 0x3ff384ebafd57667ULL,
	0x3fe571ed3c506b3aULL, 0x3ff38b79579d3eabULL,
	0x3fe56397ba7c52e2ULL, 0x3ff39204cd9d81d6ULL,
	0x3fe5555555555555ULL, 0x3ff3988e1409212eULL,
	0x3fe54725e6bb82feULL, 0x3ff39f152d0f5470ULL,
	0x3fe5390948f40febULL, 0x3ff3a59a1adbb257ULL,
	0x3fe52aff56a8054bULL, 0x3ff3ac1cdf963908ULL,
	0x3fe51d07eae2f815ULL, 0x3ff3b29d7d635662ULL,
	0x3fe50f22e111c4c5ULL, 0x3ff3b91bf663f03aULL,
	0x3fe5015015015015ULL, 0x3ff3bf984cb56c77ULL,
	0x3fe4f38f62dd4c9bULL, 0x3ff3c6128271b923ULL,
	0x3fe4e5e0a72f0539ULL, 0x3ff3cc8a99af5453ULL,
	0x3fe4d843bedc2c4cULL, 0x3ff3d30094815409ULL,
	0x3fe4cab88725af6eULL, 0x3ff3d97474f76df2ULL,
	0x3fe4bd3edda68fe1ULL, 0x3ff3dfe63d1dff15ULL,
	0x3fe4afd6a052bf5bULL, 0x3ff3e655eefe1367ULL,
	0x3fe4a27fad76014aULL, 0x3ff3ecc38c9d6d4dULL,
	0x3fe49539e3b2d067ULL, 0x3ff3f32f17fe8d04ULL,
	0x3fe4880522014880ULL, 0x3ff3f9989320b7f7ULL,
	0x3fe47ae147ae147bULL, 0x3ff4000000000000ULL,
	0x3fe46dce34596066ULL, 0x3ff4066560954a8fULL,
	0x3fe460cbc7f5cf9aULL, 0x3ff40cc8b6d657c2ULL,
	0x3fe453d9e2c776caULL, 0x3ff4132a04b5c969ULL,
	0x3fe446f86562d9fbULL, 0x3ff419894c2329f0ULL,
	0x3fe43a2730abee4dULL, 0x3ff41fe68f0af33dULL,
	0x3fe42d6625d51f87ULL, 0x3ff42641cf569572ULL,
	0x3fe420b5265e5951ULL, 0x3ff42c9b0eec7da4ULL,
	0x3fe4141414141414ULL, 0x3ff432f24fb01c7aULL,
	0x3fe40782d10e6566ULL, 0x3ff439479381ecbdULL,
	0x3fe3fb013fb013fbULL, 0x3ff43f9adc3f79ceULL,
	0x3fe3ee8f42a5af07ULL, 0x3ff445ec2bc36615ULL,
	0x3fe3e22cbce4a902ULL, 0x3ff44c3b83e57153ULL,
	0x3fe3d5d991aa75c6ULL, 0x3ff45288e67a7eedULL,
	0x3fe3c995a47babe7ULL, 0x3ff458d455549c1aULL,
	0x3fe3bd60d9232955ULL, 0x3ff45f1dd243060aULL,
	0x3fe3b13b13b13b14ULL, 0x3ff465655f122ff6ULL,
	0x3fe3a524387ac822ULL, 0x3ff46baafd8bc921ULL,
	0x3fe3991c2c187f63ULL, 0x3ff471eeaf76c2c6ULL,
	0x3fe38d22d366088eULL, 0x3ff47830769755feULL,
	0x3fe3813813813814ULL, 0x3ff47e7054af0989ULL,
	0x3fe3755bd1c945eeULL, 0x3ff484ae4b7cb793ULL,
	0x3fe3698df3de0748ULL, 0x3ff48aea5cbc935fULL,
	0x3fe35dce5f9f2af8ULL, 0x3ff491248a282eedULL,
	0x3fe3521cfb2b78c1ULL, 0x3ff4975cd5768088ULL,
	0x3fe34679ace01346ULL, 0x3ff49d93405be849ULL,
	0x3fe33ae45b57bcb2ULL, 0x3ff4a3c7cc8a358aULL,
	0x3fe32f5ced6a1dfaULL, 0x3ff4a9fa7bb0ac4bULL,
	0x3fe323e34a2b10bfULL, 0x3ff4b02b4f7c0a88ULL,
	0x3fe3187758e9ebb6ULL, 0x3ff4b65a49968d7fULL,
	0x3fe30d190130d190ULL, 0x3ff4bc876ba7f6ecULL,
	0x3fe301c82ac40260ULL, 0x3ff4c2b2b7559234ULL,
	0x3fe2f684bda12f68ULL, 0x3ff4c8dc2e423980ULL,
	0x3fe2eb4ea1fed14bULL, 0x3ff4cf03d20e5ad0ULL,
	0x3fe2e025c04b8097ULL, 0x3ff4d529a457fcfcULL,
	0x3fe2d50a012d50a0ULL, 0x3ff4db4da6bac4aaULL,
	0x3fe2c9fb4d812ca0ULL, 0x3ff4e16fdacff937ULL,
	0x3fe2bef98e5a3711ULL, 0x3ff4e790422e898fULL,
	0x3fe2b404ad012b40ULL, 0x3ff4edaede6b10feULL,
	0x3fe2a91c92f3c105ULL, 0x3ff4f3cbb117dbf4ULL,
	0x3fe29e4129e4129eULL, 0x3ff4f9e6bbc4ecb3ULL,
	0x3fe293725bb804a5ULL, 0x3ff5000000000000ULL,
	0x3fe288b01288b013ULL, 0x3ff506177f5491bbULL,
	0x3fe27dfa38a1ce4dULL, 0x3ff50c2d3b4be170ULL,
	0x3fe27350b8812735ULL, 0x3ff51241356cf6e0ULL,
	0x3fe268b37cd60127ULL, 0x3ff518536f3ca675ULL,
	0x3fe25e22708092f1ULL, 0x3ff51e63ea3d95b0ULL,
	0x3fe2539d7e9177b2ULL, 0x3ff52472a7f03f92ULL,
	0x3fe2492492492492ULL, 0x3ff52a7fa9d2f8eaULL,
	0x3fe23eb79717605bULL, 0x3ff5308af161f4a5ULL,
	0x3fe23456789abcdfULL, 0x3ff5369480174810ULL,
	0x3fe22a0122a0122aULL, 0x3ff53c9c576aef0bULL,
	0x3fe21fb78121fb78ULL, 0x3ff542a278d2d036ULL,
	0x3fe21579804855e6ULL, 0x3ff548a6e5c2c110ULL,
	0x3fe20b470c67c0d9ULL, 0x3ff54ea99fac8a0fULL,
	0x3fe2012012012012ULL, 0x3ff554aaa7ffeaaaULL,
	0x3fe1f7047dc11f70ULL, 0x3ff55aaa002a9d5aULL,
	0x3fe1ecf43c7fb84cULL, 0x3ff560a7a9985b93ULL,
	0x3fe1e2ef3b3fb874ULL, 0x3ff566a3a5b2e1b1ULL,
	0x3fe1d8f5672e4abdULL, 0x3ff56c9df5e1f2daULL,
	0x3fe1cf06ada2811dULL, 0x3ff572969b8b5cd8ULL,
	0x3fe1c522fc1ce059ULL, 0x3ff5788d9812fbebULL,
	0x3fe1bb4a4046ed29ULL, 0x3ff57e82ecdabe8dULL,
	0x3fe1b17c67f2bae3ULL, 0x3ff584769b42a930ULL,
	0x3fe1a7b9611a7b96ULL, 0x3ff58a68a4a8d9f3ULL,
	0x3fe19e0119e0119eULL, 0x3ff590590a698c4bULL,
	0x3fe19453808ca29cULL, 0x3ff59647cddf1ca5ULL,
	0x3fe18ab083902bdbULL, 0x3ff59c34f0620bffULL,
	0x3fe1811811811812ULL, 0x3ff5a22073490377ULL,
	0x3fe1778a191bd684ULL, 0x3ff5a80a57e8d7d1ULL,
	0x3fe16e0689427379ULL, 0x3ff5adf29f948cfbULL,
	0x3fe1648d50fc3201ULL, 0x3ff5b3d94b9d5979ULL,
	0x3fe15b1e5f75270dULL, 0x3ff5b9be5d52a9daULL,
	0x3fe151b9a3fdd5c9ULL, 0x3ff5bfa1d602241cULL,
	0x3fe1485f0e0acd3bULL, 0x3ff5c583b6f7ab03ULL,
	0x3fe13f0e8d344724ULL, 0x3ff5cb64017d6177ULL,
	0x3fe135c81135c811ULL, 0x3ff5d142b6dbadc5ULL,
	0x3fe12c8b89edc0acULL, 0x3ff5d71fd8593cefULL,
	0x3fe12358e75d3033ULL, 0x3ff5dcfb673b05dfULL,
	0x3fe11a3019a74826ULL, 0x3ff5e2d564c44ca1ULL,
	0x3fe1111111111111ULL, 0x3ff5e8add236a58fULL,
	0x3fe107fbbe011080ULL, 0x3ff5ee84b0d1f876ULL,
	0x3fe0fef010fef011ULL, 0x3ff5f45a01d483b4ULL,
	0x3fe0f5edfab325a2ULL, 0x3ff5fa2dc67adf4eULL,
	0x3fe0ecf56be69c90ULL, 0x3ff6000000000000ULL,
	0x3fe0e40655826011ULL, 0x3ff605d0af9d3a44ULL,
	0x3fe0db20a88f4696ULL, 0x3ff60b9fd68a4554ULL,
	0x3fe0d24456359e3aULL, 0x3ff6116d75fd3e21ULL,
	0x3fe0c9714fbcda3bULL, 0x3ff617398f2aaa48ULL,
	0x3fe0c0a7868b4171ULL, 0x3ff61d0423457afbULL,
	0x3fe0b7e6ec259dc8ULL, 0x3ff622cd337f0fe8ULL,
	0x3fe0af2f722eecb5ULL, 0x3ff62894c1073a17ULL,
	0x3fe0a6810a6810a7ULL, 0x3ff62e5acd0c3ebeULL,
	0x3fe09ddba6af8360ULL, 0x3ff6341f58bada14ULL,
	0x3fe0953f39010954ULL, 0x3ff639e2653e421bULL,
	0x3fe08cabb37565e2ULL, 0x3ff63fa3f3c02962ULL,
	0x3fe0842108421084ULL, 0x3ff645640568c1c3ULL,
	0x3fe07b9f29b8eae2ULL, 0x3ff64b229b5ebf1bULL,
	0x3fe073260a47f7c6ULL, 0x3ff650dfb6c759f4ULL,
	0x3fe06ab59c7912fbULL, 0x3ff6569b58c65239ULL,
	0x3fe0624dd2f1a9fcULL, 0x3ff65c55827df1d2ULL,
	0x3fe059eea0727586ULL, 0x3ff6620e350f0f44ULL,
	0x3fe05197f7d73404ULL, 0x3ff667c57199104bULL,
	0x3fe04949cc1664c5ULL, 0x3ff66d7b3939ec6aULL,
	0x3fe0410410410410ULL, 0x3ff6732f8d0e2f77ULL,
	0x3fe038c6b78247fcULL, 0x3ff678e26e30fc21ULL,
	0x3fe03091b51f5e1aULL, 0x3ff67e93ddbc0e73ULL,
	0x3fe02864fc7729e9ULL, 0x3ff68443dcc7be4aULL,
	0x3fe0204081020408ULL, 0x3ff689f26c6b01d0ULL,
	0x3fe0182436517a37ULL, 0x3ff68f9f8dbb6fe7ULL,
	0x3fe0101010101010ULL, 0x3ff6954b41cd4293ULL,
	0x3fe0080402010080ULL, 0x3ff69af589b35963ULL,
};

/* *********************************************************** */

/*  Table for S16 upper half : x[i] = (mlib_f32)(i * 256)   -128 <= i <= 127  */

static const mlib_d64 S16uh[] = {
	-32768, -32512, -32256, -32000, -31744, -31488, -31232, -30976, -30720,
		-30464,
	-30208, -29952, -29696, -29440, -29184, -28928, -28672, -28416, -28160,
		-27904,
	-27648, -27392, -27136, -26880, -26624, -26368, -26112, -25856, -25600,
		-25344,
	-25088, -24832, -24576, -24320, -24064, -23808, -23552, -23296, -23040,
		-22784,
	-22528, -22272, -22016, -21760, -21504, -21248, -20992, -20736, -20480,
		-20224,
	-19968, -19712, -19456, -19200, -18944, -18688, -18432, -18176, -17920,
		-17664,
	-17408, -17152, -16896, -16640, -16384, -16128, -15872, -15616, -15360,
		-15104,
	-14848, -14592, -14336, -14080, -13824, -13568, -13312, -13056, -12800,
		-12544,
	-12288, -12032, -11776, -11520, -11264, -11008, -10752, -10496, -10240,
		-9984,
	-9728, -9472, -9216, -8960, -8704, -8448, -8192, -7936, -7680, -7424,
	-7168, -6912, -6656, -6400, -6144, -5888, -5632, -5376, -5120, -4864,
	-4608, -4352, -4096, -3840, -3584, -3328, -3072, -2816, -2560, -2304,
	-2048, -1792, -1536, -1280, -1024, -768, -512, -256, 0, 256,
	512, 768, 1024, 1280, 1536, 1792, 2048, 2304, 2560, 2816,
	3072, 3328, 3584, 3840, 4096, 4352, 4608, 4864, 5120, 5376,
	5632, 5888, 6144, 6400, 6656, 6912, 7168, 7424, 7680, 7936,
	8192, 8448, 8704, 8960, 9216, 9472, 9728, 9984, 10240, 10496,
	10752, 11008, 11264, 11520, 11776, 12032, 12288, 12544, 12800, 13056,
	13312, 13568, 13824, 14080, 14336, 14592, 14848, 15104, 15360, 15616,
	15872, 16128, 16384, 16640, 16896, 17152, 17408, 17664, 17920, 18176,
	18432, 18688, 18944, 19200, 19456, 19712, 19968, 20224, 20480, 20736,
	20992, 21248, 21504, 21760, 22016, 22272, 22528, 22784, 23040, 23296,
	23552, 23808, 24064, 24320, 24576, 24832, 25088, 25344, 25600, 25856,
	26112, 26368, 26624, 26880, 27136, 27392, 27648, 27904, 28160, 28416,
	28672, 28928, 29184, 29440, 29696, 29952, 30208, 30464, 30720, 30976,
	31232, 31488, 31744, 32000, 32256, 32512
};

/*  Table for S16 upper half : x[i] = (mlib_f32)(i * 256)   -128 <= i <= 127  */

static const mlib_d64 S16lh[] = {
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
	10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
	20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
	30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
	40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
	50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
	60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
	70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
	80, 81, 82, 83, 84, 85, 86, 87, 88, 89,
	90, 91, 92, 93, 94, 95, 96, 97, 98, 99,
	100, 101, 102, 103, 104, 105, 106, 107, 108, 109,
	110, 111, 112, 113, 114, 115, 116, 117, 118, 119,
	120, 121, 122, 123, 124, 125, 126, 127, 128, 129,
	130, 131, 132, 133, 134, 135, 136, 137, 138, 139,
	140, 141, 142, 143, 144, 145, 146, 147, 148, 149,
	150, 151, 152, 153, 154, 155, 156, 157, 158, 159,
	160, 161, 162, 163, 164, 165, 166, 167, 168, 169,
	170, 171, 172, 173, 174, 175, 176, 177, 178, 179,
	180, 181, 182, 183, 184, 185, 186, 187, 188, 189,
	190, 191, 192, 193, 194, 195, 196, 197, 198, 199,
	200, 201, 202, 203, 204, 205, 206, 207, 208, 209,
	210, 211, 212, 213, 214, 215, 216, 217, 218, 219,
	220, 221, 222, 223, 224, 225, 226, 227, 228, 229,
	230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
	240, 241, 242, 243, 244, 245, 246, 247, 248, 249,
	250, 251, 252, 253, 254, 255
};

/* *********************************************************** */

/*  Table for S8/U8 : x[i] = (mlib_f32)(i * i)   -128 <= i <= 255  */

static const mlib_f32 sqw_S8[] = {
	16384, 16129, 15876, 15625, 15376, 15129, 14884, 14641, 14400, 14161,
	13924, 13689, 13456, 13225, 12996, 12769, 12544, 12321, 12100, 11881,
	11664, 11449, 11236, 11025, 10816, 10609, 10404, 10201, 10000, 9801,
	9604, 9409, 9216, 9025, 8836, 8649, 8464, 8281, 8100, 7921,
	7744, 7569, 7396, 7225, 7056, 6889, 6724, 6561, 6400, 6241,
	6084, 5929, 5776, 5625, 5476, 5329, 5184, 5041, 4900, 4761,
	4624, 4489, 4356, 4225, 4096, 3969, 3844, 3721, 3600, 3481,
	3364, 3249, 3136, 3025, 2916, 2809, 2704, 2601, 2500, 2401,
	2304, 2209, 2116, 2025, 1936, 1849, 1764, 1681, 1600, 1521,
	1444, 1369, 1296, 1225, 1156, 1089, 1024, 961, 900, 841,
	784, 729, 676, 625, 576, 529, 484, 441, 400, 361,
	324, 289, 256, 225, 196, 169, 144, 121, 100, 81,
	64, 49, 36, 25, 16, 9, 4, 1, 0, 1,
	4, 9, 16, 25, 36, 49, 64, 81, 100, 121,
	144, 169, 196, 225, 256, 289, 324, 361, 400, 441,
	484, 529, 576, 625, 676, 729, 784, 841, 900, 961,
	1024, 1089, 1156, 1225, 1296, 1369, 1444, 1521, 1600, 1681,
	1764, 1849, 1936, 2025, 2116, 2209, 2304, 2401, 2500, 2601,
	2704, 2809, 2916, 3025, 3136, 3249, 3364, 3481, 3600, 3721,
	3844, 3969, 4096, 4225, 4356, 4489, 4624, 4761, 4900, 5041,
	5184, 5329, 5476, 5625, 5776, 5929, 6084, 6241, 6400, 6561,
	6724, 6889, 7056, 7225, 7396, 7569, 7744, 7921, 8100, 8281,
	8464, 8649, 8836, 9025, 9216, 9409, 9604, 9801, 10000, 10201,
	10404, 10609, 10816, 11025, 11236, 11449, 11664, 11881, 12100, 12321,
	12544, 12769, 12996, 13225, 13456, 13689, 13924, 14161, 14400, 14641,
	14884, 15129, 15376, 15625, 15876, 16129, 16384, 16641,
	16900, 17161, 17424, 17689, 17956, 18225, 18496, 18769, 19044, 19321,
	19600, 19881, 20164, 20449, 20736, 21025, 21316, 21609, 21904, 22201,
	22500, 22801, 23104, 23409, 23716, 24025, 24336, 24649, 24964, 25281,
	25600, 25921, 26244, 26569, 26896, 27225, 27556, 27889, 28224, 28561,
	28900, 29241, 29584, 29929, 30276, 30625, 30976, 31329, 31684, 32041,
	32400, 32761, 33124, 33489, 33856, 34225, 34596, 34969, 35344, 35721,
	36100, 36481, 36864, 37249, 37636, 38025, 38416, 38809, 39204, 39601,
	40000, 40401, 40804, 41209, 41616, 42025, 42436, 42849, 43264, 43681,
	44100, 44521, 44944, 45369, 45796, 46225, 46656, 47089, 47524, 47961,
	48400, 48841, 49284, 49729, 50176, 50625, 51076, 51529, 51984, 52441,
	52900, 53361, 53824, 54289, 54756, 55225, 55696, 56169, 56644, 57121,
	57600, 58081, 58564, 59049, 59536, 60025, 60516, 61009, 61504, 62001,
	62500, 63001, 63504, 64009, 64516, 65025
};

/* *********************************************************** */

mlib_status
__mlib_VectorMag_U8C(
	mlib_d64 *m,
	const mlib_u8 *x,
	mlib_s32 n)
{
	mlib_s32 i, ax;
	mlib_u8 *px = (mlib_u8 *)x;
	const mlib_f32 *psqw = sqw_S8 + 128;

	mlib_d64 K1 = 5.00000715259318464227e-01;
	mlib_d64 K2 = -1.25000447037521686593e-01;
	mlib_d64 h_hi0, res0, xx0, db0, hyp0;
	mlib_d64 DC0 = vis_to_double(0x000fffff, 0xffffffff);
	mlib_d64 DC1 = vis_to_double(0x3ff00000, 0);
	mlib_d64 DC2 = vis_to_double(0x7ffff000, 0);
	mlib_d64 DA0 = vis_to_double(0x7fe00000, 0);
	mlib_d64 DA1 = vis_to_double(0x20000000, 0);
	mlib_f32 SCALE = vis_to_float(0x80000000);
	mlib_s32 si0;

#pragma pipeloop(0)
	for (i = 0; i < n; i++) {
		hyp0 = (psqw[px[2 * i]] + psqw[px[2 * i + 1]]);

		ax = ((mlib_s32 *)&hyp0)[0];

		db0 = vis_fand(hyp0, DA0);
		db0 = vis_fmul8x16(SCALE, db0);
		db0 = vis_fpadd32(db0, DA1);

		hyp0 = vis_fand(hyp0, DC0);
		hyp0 = vis_for(hyp0, DC1);
		h_hi0 = vis_fand(hyp0, DC2);

		ax >>= 8;
		si0 = ax & 0x1ff0;
		si0 -= ((-ax) >> 31) & ~0xf;
		xx0 = (hyp0 - h_hi0) * ((mlib_d64 *)((mlib_s8 *)arr + si0))[0];
		res0 = ((mlib_d64 *)((mlib_s8 *)arr + si0))[1] * ((K2 * xx0 +
			K1) * xx0 + DC1);
		res0 *= db0;

		m[i] = res0;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMag_S8C(
	mlib_d64 *m,
	const mlib_s8 *x,
	mlib_s32 n)
{
	mlib_s32 i, ax;
	mlib_s8 *px = (mlib_s8 *)x;
	const mlib_f32 *psqw = sqw_S8 + 128;

	mlib_d64 K1 = 5.00000715259318464227e-01;
	mlib_d64 K2 = -1.25000447037521686593e-01;
	mlib_d64 h_hi0, res0, xx0, db0, hyp0;
	mlib_d64 DC0 = vis_to_double(0x000fffff, 0xffffffff);
	mlib_d64 DC1 = vis_to_double(0x3ff00000, 0);
	mlib_d64 DC2 = vis_to_double(0x7ffff000, 0);
	mlib_d64 DA0 = vis_to_double(0x7fe00000, 0);
	mlib_d64 DA1 = vis_to_double(0x20000000, 0);
	mlib_f32 SCALE = vis_to_float(0x80000000);
	mlib_s32 si0;

#pragma pipeloop(0)
	for (i = 0; i < n; i++) {
		hyp0 = psqw[px[2 * i]] + psqw[px[2 * i + 1]];

		ax = ((mlib_s32 *)&hyp0)[0];

		db0 = vis_fand(hyp0, DA0);
		db0 = vis_fmul8x16(SCALE, db0);
		db0 = vis_fpadd32(db0, DA1);

		hyp0 = vis_fand(hyp0, DC0);
		hyp0 = vis_for(hyp0, DC1);
		h_hi0 = vis_fand(hyp0, DC2);

		ax >>= 8;
		si0 = ax & 0x1ff0;
/*  to perform sqrtf from zero properly  */
		si0 -= ((-ax) >> 31) & ~0xf;
		xx0 = (hyp0 - h_hi0) * ((mlib_d64 *)((mlib_s8 *)arr + si0))[0];
		res0 = ((mlib_d64 *)((mlib_s8 *)arr + si0))[1] * ((K2 * xx0 +
			K1) * xx0 + DC1);
		res0 *= db0;

		m[i] = res0;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

mlib_status
__mlib_VectorMag_S16C(
	mlib_d64 *m,
	const mlib_s16 *x,
	mlib_s32 n)
{
	mlib_s32 i, ax;
	mlib_s16 *px = (mlib_s16 *)x;
	const mlib_d64 *psqw = S16uh + 128;

	mlib_d64 K1 = 5.00000715259318464227e-01;
	mlib_d64 K2 = -1.25000447037521686593e-01;
	mlib_d64 h_hi0, res0, xx0, db0, hyp0;
	mlib_d64 DC0 = vis_to_double(0x000fffff, 0xffffffff);
	mlib_d64 DC1 = vis_to_double(0x3ff00000, 0);
	mlib_d64 DC2 = vis_to_double(0x7ffff000, 0);
	mlib_d64 DA0 = vis_to_double(0x7fe00000, 0);
	mlib_d64 DA1 = vis_to_double(0x20000000, 0);
	mlib_f32 SCALE = vis_to_float(0x80000000);
	mlib_s32 si0;
	mlib_d64 ld00, ld01, ld10, ld11;

#pragma pipeloop(0)
	for (i = 0; i < n; i++) {
		ld00 = psqw[px[2 * i] >> 8];
		ld01 = S16lh[px[2 * i] & 0xFF];
		ld10 = psqw[px[2 * i + 1] >> 8];
		ld11 = S16lh[px[2 * i + 1] & 0xFF];

		hyp0 = (ld00 + ld01) * (ld00 + ld01) +
			(ld10 + ld11) * (ld10 + ld11);

		ax = ((mlib_s32 *)&hyp0)[0];

		db0 = vis_fand(hyp0, DA0);
		db0 = vis_fmul8x16(SCALE, db0);
		db0 = vis_fpadd32(db0, DA1);

		hyp0 = vis_fand(hyp0, DC0);
		hyp0 = vis_for(hyp0, DC1);
		h_hi0 = vis_fand(hyp0, DC2);

		ax >>= 8;
		si0 = ax & 0x1ff0;
/*  to perform sqrtf from zero properly  */
		si0 -= ((-ax) >> 31) & ~0xf;
		xx0 = (hyp0 - h_hi0) * ((mlib_d64 *)((mlib_s8 *)arr + si0))[0];
		res0 = ((mlib_d64 *)((mlib_s8 *)arr + si0))[1] * ((K2 * xx0 +
			K1) * xx0 + DC1);
		res0 *= db0;

		m[i] = res0;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */

/*  Table for vsqrt (soft version of sqrt)  */

static const mlib_u64 TBL[] = {
	0x0ULL, 0x0ULL, 0x0ULL, 0x0ULL,
	0x4000000000000000ULL, 0x3ff6a09e667f3bcdULL,
	0xbc9bdd3413b26456ULL, 0x0000000000000000ULL,
	0x4000000000000000ULL, 0x4000000000000000ULL,
	0xb8f0000000000000ULL, 0x0000000000000000ULL,
	0x3fffc07f01fc07f0ULL, 0x3ff6b733bfd8c648ULL,
	0x3c53b62905629048ULL, 0x0000000000000000ULL,
	0x3fffc07f01fc07f0ULL, 0x40000ff807f60debULL,
	0x3c90655c648a53f1ULL, 0x0000000000000000ULL,
	0x3fff81f81f81f820ULL, 0x3ff6cdb2bbb212ebULL,
	0x3c960332cdbaba2dULL, 0x0000000000000000ULL,
	0x3fff81f81f81f820ULL, 0x40001fe03f61bad0ULL,
	0x3ca2c41a15cbfaf2ULL, 0x0000000000000000ULL,
	0x3fff44659e4a4271ULL, 0x3ff6e41b9bfb3b75ULL,
	0xbc925d8cfd6d5c87ULL, 0x0000000000000000ULL,
	0x3fff44659e4a4271ULL, 0x40002fb8d4e30f48ULL,
	0xbca64203ab1ba910ULL, 0x0000000000000000ULL,
	0x3fff07c1f07c1f08ULL, 0x3ff6fa6ea162d0f0ULL,
	0x3c691a243d6297e9ULL, 0x0000000000000000ULL,
	0x3fff07c1f07c1f08ULL, 0x40003f81f636b80cULL,
	0xbca0efc8ba812a8cULL, 0x0000000000000000ULL,
	0x3ffecc07b301ecc0ULL, 0x3ff710ac0b5e5e32ULL,
	0xbc991218b8d2850dULL, 0x0000000000000000ULL,
	0x3ffecc07b301ecc0ULL, 0x40004f3bd03c0a64ULL,
	0x3c9ee2cf2d8ae22bULL, 0x0000000000000000ULL,
	0x3ffe9131abf0b767ULL, 0x3ff726d41832a0beULL,
	0xbc2d9b1aa8ecb058ULL, 0x0000000000000000ULL,
	0x3ffe9131abf0b767ULL, 0x40005ee68efad48bULL,
	0xbc9c35f48f4b89f7ULL, 0x0000000000000000ULL,
	0x3ffe573ac901e574ULL, 0x3ff73ce704fb7b23ULL,
	0x3c91470b816b17a6ULL, 0x0000000000000000ULL,
	0x3ffe573ac901e574ULL, 0x40006e825da8fc2bULL,
	0x3c9a315a8bd8a03bULL, 0x0000000000000000ULL,
	0x3ffe1e1e1e1e1e1eULL, 0x3ff752e50db3a3a2ULL,
	0xbc9393313eea4381ULL, 0x0000000000000000ULL,
	0x3ffe1e1e1e1e1e1eULL, 0x40007e0f66afed07ULL,
	0xbc74a6e1dcd59eafULL, 0x0000000000000000ULL,
	0x3ffde5d6e3f8868aULL, 0x3ff768ce6d3c11e0ULL,
	0xbc9478b8ab33074dULL, 0x0000000000000000ULL,
	0x3ffde5d6e3f8868aULL, 0x40008d8dd3b1d9aaULL,
	0x3c81d53385fe2b96ULL, 0x0000000000000000ULL,
	0x3ffdae6076b981dbULL, 0x3ff77ea35d632e43ULL,
	0x3c92f7149a22fa4fULL, 0x0000000000000000ULL,
	0x3ffdae6076b981dbULL, 0x40009cfdcd8ed009ULL,
	0xbc4862a9bcf7f372ULL, 0x0000000000000000ULL,
	0x3ffd77b654b82c34ULL, 0x3ff7946416ebc56cULL,
	0x3c9a7cd5224c7375ULL, 0x0000000000000000ULL,
	0x3ffd77b654b82c34ULL, 0x4000ac5f7c69a3c8ULL,
	0x3ca94dff7bfa2757ULL, 0x0000000000000000ULL,
	0x3ffd41d41d41d41dULL, 0x3ff7aa10d193c22dULL,
	0xbc790ed9403afe85ULL, 0x0000000000000000ULL,
	0x3ffd41d41d41d41dULL, 0x4000bbb307acafdbULL,
	0xbc852a97686f9d2eULL, 0x0000000000000000ULL,
	0x3ffd0cb58f6ec074ULL, 0x3ff7bfa9c41ab040ULL,
	0x3c8d6bc302ae758fULL, 0x0000000000000000ULL,
	0x3ffd0cb58f6ec074ULL, 0x4000caf8960e710dULL,
	0x3c9caa6be2366171ULL, 0x0000000000000000ULL,
	0x3ffcd85689039b0bULL, 0x3ff7d52f244809e9ULL,
	0x3c9081f6f3b99d5fULL, 0x0000000000000000ULL,
	0x3ffcd85689039b0bULL, 0x4000da304d95fb06ULL,
	0xbc9e126976855586ULL, 0x0000000000000000ULL,
	0x3ffca4b3055ee191ULL, 0x3ff7eaa126f15284ULL,
	0xbc846ce468c1882bULL, 0x0000000000000000ULL,
	0x3ffca4b3055ee191ULL, 0x4000e95a539f492cULL,
	0xbc80c73fc38a2184ULL, 0x0000000000000000ULL,
	0x3ffc71c71c71c71cULL, 0x3ff8000000000000ULL,
	0x0000000000000000ULL, 0x0000000000000000ULL,
	0x3ffc71c71c71c71cULL, 0x4000f876ccdf6cd9ULL,
	0x3cab1a18f13a34c0ULL, 0x0000000000000000ULL,
	0x3ffc3f8f01c3f8f0ULL, 0x3ff8154be2773526ULL,
	0xbc857147e067d0eeULL, 0x0000000000000000ULL,
	0x3ffc3f8f01c3f8f0ULL, 0x40010785dd689a29ULL,
	0xbcaaabbe9e4d810aULL, 0x0000000000000000ULL,
	0x3ffc0e070381c0e0ULL, 0x3ff82a8500794e6cULL,
	0xbc82edaa75e6ac5fULL, 0x0000000000000000ULL,
	0x3ffc0e070381c0e0ULL, 0x40011687a8ae14a3ULL,
	0x3cac9b43bcf06106ULL, 0x0000000000000000ULL,
	0x3ffbdd2b899406f7ULL, 0x3ff83fab8b4d4315ULL,
	0x3c829e062d3e134dULL, 0x0000000000000000ULL,
	0x3ffbdd2b899406f7ULL, 0x4001257c5187fd09ULL,
	0xbca4a750a83950a4ULL, 0x0000000000000000ULL,
	0x3ffbacf914c1bad0ULL, 0x3ff854bfb363dc39ULL,
	0x3c99399fca38787eULL, 0x0000000000000000ULL,
	0x3ffbacf914c1bad0ULL, 0x40013463fa37014eULL,
	0x3c7b295baa698cd3ULL, 0x0000000000000000ULL,
	0x3ffb7d6c3dda338bULL, 0x3ff869c1a85cc346ULL,
	0x3c9fcc99de11b1d1ULL, 0x0000000000000000ULL,
	0x3ffb7d6c3dda338bULL, 0x4001433ec467effbULL,
	0x3c92c0313b7278c8ULL, 0x0000000000000000ULL,
	0x3ffb4e81b4e81b4fULL, 0x3ff87eb1990b697aULL,
	0x3c7c43e9f593ea0fULL, 0x0000000000000000ULL,
	0x3ffb4e81b4e81b4fULL, 0x4001520cd1372febULL,
	0xbcadec225d8e66d2ULL, 0x0000000000000000ULL,
	0x3ffb2036406c80d9ULL, 0x3ff8938fb37bc9c1ULL,
	0xbc7c115f9f5c8d6fULL, 0x0000000000000000ULL,
	0x3ffb2036406c80d9ULL, 0x400160ce41341d74ULL,
	0x3c967036863a1bb2ULL, 0x0000000000000000ULL,
	0x3ffaf286bca1af28ULL, 0x3ff8a85c24f70659ULL,
	0x3c9f6e076b588a50ULL, 0x0000000000000000ULL,
	0x3ffaf286bca1af28ULL, 0x40016f8334644df9ULL,
	0xbcae867980a1c48eULL, 0x0000000000000000ULL,
	0x3ffac5701ac5701bULL, 0x3ff8bd171a07e38aULL,
	0x3c9c20b5a697f23fULL, 0x0000000000000000ULL,
	0x3ffac5701ac5701bULL, 0x40017e2bca46bab9ULL,
	0x3ca1519b10d04d5fULL, 0x0000000000000000ULL,
	0x3ffa98ef606a63beULL, 0x3ff8d1c0be7f20acULL,
	0xbc8bdb8a6df021f3ULL, 0x0000000000000000ULL,
	0x3ffa98ef606a63beULL, 0x40018cc821d6d3e3ULL,
	0xbca30af1d725cc5bULL, 0x0000000000000000ULL,
	0x3ffa6d01a6d01a6dULL, 0x3ff8e6593d77b0b8ULL,
	0xbc7d99d764769954ULL, 0x0000000000000000ULL,
	0x3ffa6d01a6d01a6dULL, 0x40019b58598f7c9fULL,
	0xbc72e0d851c0e011ULL, 0x0000000000000000ULL,
	0x3ffa41a41a41a41aULL, 0x3ff8fae0c15ad38aULL,
	0xbc7db7adb6817f6dULL, 0x0000000000000000ULL,
	0x3ffa41a41a41a41aULL, 0x4001a9dc8f6df104ULL,
	0xbcafc519c18dc1d5ULL, 0x0000000000000000ULL,
	0x3ffa16d3f97a4b02ULL, 0x3ff90f5773e410e4ULL,
	0x3c6fb605cee75482ULL, 0x0000000000000000ULL,
	0x3ffa16d3f97a4b02ULL, 0x4001b854e0f496a0ULL,
	0x3ca27006899b7c3aULL, 0x0000000000000000ULL,
	0x3ff9ec8e951033d9ULL, 0x3ff923bd7e25164dULL,
	0xbc9278d1901d3b40ULL, 0x0000000000000000ULL,
	0x3ff9ec8e951033d9ULL, 0x4001c6c16b2db870ULL,
	0x3c887e1d8335fb28ULL, 0x0000000000000000ULL,
	0x3ff9c2d14ee4a102ULL, 0x3ff93813088978c5ULL,
	0xbc54312c627e5c52ULL, 0x0000000000000000ULL,
	0x3ff9c2d14ee4a102ULL, 0x4001d5224aae2ee1ULL,
	0x3ca91222f6aebdc9ULL, 0x0000000000000000ULL,
	0x3ff999999999999aULL, 0x3ff94c583ada5b53ULL,
	0xbc9b7ed750df3ccaULL, 0x0000000000000000ULL,
	0x3ff999999999999aULL, 0x4001e3779b97f4a8ULL,
	0xbc9f506319fcfd19ULL, 0x0000000000000000ULL,
	0x3ff970e4f80cb872ULL, 0x3ff9608d3c41fb4bULL,
	0x3c73df32eaa86b83ULL, 0x0000000000000000ULL,
	0x3ff970e4f80cb872ULL, 0x4001f1c1799ca8ffULL,
	0xbca28b52eb725e0aULL, 0x0000000000000000ULL,
	0x3ff948b0fcd6e9e0ULL, 0x3ff974b2334f2346ULL,
	0x3c814e4ad3ae9e3fULL, 0x0000000000000000ULL,
	0x3ff948b0fcd6e9e0ULL, 0x4002000000000000ULL,
	0xb900000000000000ULL, 0x0000000000000000ULL,
	0x3ff920fb49d0e229ULL, 0x3ff988c745f88592ULL,
	0x3c95af701a56047bULL, 0x0000000000000000ULL,
	0x3ff920fb49d0e229ULL, 0x40020e33499a21a9ULL,
	0xbc924ba274fea9a1ULL, 0x0000000000000000ULL,
	0x3ff8f9c18f9c18faULL, 0x3ff99ccc999fff00ULL,
	0x3c866234063b88eeULL, 0x0000000000000000ULL,
	0x3ff8f9c18f9c18faULL, 0x40021c5b70d9f824ULL,
	0xbca844f99eee6fc3ULL, 0x0000000000000000ULL,
	0x3ff8d3018d3018d3ULL, 0x3ff9b0c25315c2ceULL,
	0xbc87f64a65cc6887ULL, 0x0000000000000000ULL,
	0x3ff8d3018d3018d3ULL, 0x40022a788fc76de5ULL,
	0x3c931e32d4e07a48ULL, 0x0000000000000000ULL,
	0x3ff8acb90f6bf3aaULL, 0x3ff9c4a8969b7077ULL,
	0xbc96ca9e5cd4517aULL, 0x0000000000000000ULL,
	0x3ff8acb90f6bf3aaULL, 0x4002388ac0059c28ULL,
	0xbc96072fbe0e5da3ULL, 0x0000000000000000ULL,
	0x3ff886e5f0abb04aULL, 0x3ff9d87f87e71422ULL,
	0xbc85fdd8b11b7b1dULL, 0x0000000000000000ULL,
	0x3ff886e5f0abb04aULL, 0x400246921ad4ea49ULL,
	0xbcaa6d9b268ef62dULL, 0x0000000000000000ULL,
	0x3ff8618618618618ULL, 0x3ff9ec474a261264ULL,
	0xbc8540c489ba5074ULL, 0x0000000000000000ULL,
	0x3ff8618618618618ULL, 0x4002548eb9151e85ULL,
	0x3c9998200a774879ULL, 0x0000000000000000ULL,
	0x3ff83c977ab2beddULL, 0x3ffa000000000000ULL,
	0x0000000000000000ULL, 0x0000000000000000ULL,
	0x3ff83c977ab2beddULL, 0x40026280b3476096ULL,
	0x3c9ab88b5ffe1cf5ULL, 0x0000000000000000ULL,
	0x3ff8181818181818ULL, 0x3ffa13a9cb996651ULL,
	0xbc9f9ab90e4e85c3ULL, 0x0000000000000000ULL,
	0x3ff8181818181818ULL, 0x4002706821902e9aULL,
	0x3c90ff4c20f541f6ULL, 0x0000000000000000ULL,
	0x3ff7f405fd017f40ULL, 0x3ffa2744ce9674f5ULL,
	0xbc8b936c81e54daaULL, 0x0000000000000000ULL,
	0x3ff7f405fd017f40ULL, 0x40027e451bb944c3ULL,
	0x3c8e4a1642099ef0ULL, 0x0000000000000000ULL,
	0x3ff7d05f417d05f4ULL, 0x3ffa3ad12a1da160ULL,
	0x3c951168f4be5984ULL, 0x0000000000000000ULL,
	0x3ff7d05f417d05f4ULL, 0x40028c17b9337834ULL,
	0xbc8af150a0e88972ULL, 0x0000000000000000ULL,
	0x3ff7ad2208e0ecc3ULL, 0x3ffa4e4efeda34deULL,
	0x3c6afbb4dbdadd0dULL, 0x0000000000000000ULL,
	0x3ff7ad2208e0ecc3ULL, 0x400299e011188575ULL,
	0xbc9a61693fb250e5ULL, 0x0000000000000000ULL,
	0x3ff78a4c8178a4c8ULL, 0x3ffa61be6cfec997ULL,
	0xbc8c37eab2bb5ca0ULL, 0x0000000000000000ULL,
	0x3ff78a4c8178a4c8ULL, 0x4002a79e3a2cd2e6ULL,
	0xbca5ddd49cc9ad59ULL, 0x0000000000000000ULL,
	0x3ff767dce434a9b1ULL, 0x3ffa751f9447b724ULL,
	0x3c82b909477e9ed1ULL, 0x0000000000000000ULL,
	0x3ff767dce434a9b1ULL, 0x4002b5524ae1278eULL,
	0xbca2f2a98841b934ULL, 0x0000000000000000ULL,
	0x3ff745d1745d1746ULL, 0x3ffa887293fd6f34ULL,
	0x3c768ef24f198721ULL, 0x0000000000000000ULL,
	0x3ff745d1745d1746ULL, 0x4002c2fc595456a7ULL,
	0xbc996f60b0fc7e96ULL, 0x0000000000000000ULL,
	0x3ff724287f46debcULL, 0x3ffa9bb78af6cabcULL,
	0x3c8ba60dc999aba7ULL, 0x0000000000000000ULL,
	0x3ff724287f46debcULL, 0x4002d09c7b54e03eULL,
	0x3c98c747fdeda6deULL, 0x0000000000000000ULL,
	0x3ff702e05c0b8170ULL, 0x3ffaaeee979b4838ULL,
	0xbc91f08aef9ef6c0ULL, 0x0000000000000000ULL,
	0x3ff702e05c0b8170ULL, 0x4002de32c6628741ULL,
	0x3ca78746c499a4f7ULL, 0x0000000000000000ULL,
	0x3ff6e1f76b4337c7ULL, 0x3ffac217d7e53b66ULL,
	0xbc64282aaa967e4fULL, 0x0000000000000000ULL,
	0x3ff6e1f76b4337c7ULL, 0x4002ebbf4fafdd4bULL,
	0xbca78a73b72d5c41ULL, 0x0000000000000000ULL,
	0x3ff6c16c16c16c17ULL, 0x3ffad5336963eefcULL,
	0xbc977c4a537dbdd2ULL, 0x0000000000000000ULL,
	0x3ff6c16c16c16c17ULL, 0x4002f9422c23c47eULL,
	0xbc827c85f29db65dULL, 0x0000000000000000ULL,
	0x3ff6a13cd1537290ULL, 0x3ffae841693db8b4ULL,
	0x3c90f773cd7a0713ULL, 0x0000000000000000ULL,
	0x3ff6a13cd1537290ULL, 0x400306bb705ae7c3ULL,
	0x3caf4933907af47aULL, 0x0000000000000000ULL,
	0x3ff6816816816817ULL, 0x3ffafb41f432002eULL,
	0xbc7ac94afdfe8c5bULL, 0x0000000000000000ULL,
	0x3ff6816816816817ULL, 0x4003142b30a929abULL,
	0x3c98dc01081a6c5cULL, 0x0000000000000000ULL,
	0x3ff661ec6a5122f9ULL, 0x3ffb0e35269b38f5ULL,
	0xbc4f69a805c3271aULL, 0x0000000000000000ULL,
	0x3ff661ec6a5122f9ULL, 0x40032191811b0a41ULL,
	0xbc9ce3f0b38c0bf7ULL, 0x0000000000000000ULL,
	0x3ff642c8590b2164ULL, 0x3ffb211b1c70d023ULL,
	0x3c2e4c5e66eae2f0ULL, 0x0000000000000000ULL,
	0x3ff642c8590b2164ULL, 0x40032eee75770416ULL,
	0x3caed8e7730eaff2ULL, 0x0000000000000000ULL,
	0x3ff623fa77016240ULL, 0x3ffb33f3f1490defULL,
	0xbc95894bcb02373bULL, 0x0000000000000000ULL,
	0x3ff623fa77016240ULL, 0x40033c42213ee0c9ULL,
	0x3ca84c244ba98124ULL, 0x0000000000000000ULL,
	0x3ff6058160581606ULL, 0x3ffb46bfc05aeb89ULL,
	0x3c9b1c7cc39adc9fULL, 0x0000000000000000ULL,
	0x3ff6058160581606ULL, 0x4003498c97b10540ULL,
	0x3c734193bc8543b4ULL, 0x0000000000000000ULL,
	0x3ff5e75bb8d015e7ULL, 0x3ffb597ea47fdda3ULL,
	0xbc923cc89d1e4635ULL, 0x0000000000000000ULL,
	0x3ff5e75bb8d015e7ULL, 0x400356cdebc9b5e2ULL,
	0x3c96dee146bb1571ULL, 0x0000000000000000ULL,
	0x3ff5c9882b931057ULL, 0x3ffb6c30b83593e6ULL,
	0x3c8f4e3fd28d84bcULL, 0x0000000000000000ULL,
	0x3ff5c9882b931057ULL, 0x4003640630445306ULL,
	0xbca78d862327430aULL, 0x0000000000000000ULL,
	0x3ff5ac056b015ac0ULL, 0x3ffb7ed6159fadc8ULL,
	0xbc899bcff04d134bULL, 0x0000000000000000ULL,
	0x3ff5ac056b015ac0ULL, 0x40037135779c8dcbULL,
	0xbc8fe126ce9778aeULL, 0x0000000000000000ULL,
	0x3ff58ed2308158edULL, 0x3ffb916ed68964ecULL,
	0x3c826a5d5dbaae29ULL, 0x0000000000000000ULL,
	0x3ff58ed2308158edULL, 0x40037e5bd40f95a1ULL,
	0x3cac6ff5eca5d122ULL, 0x0000000000000000ULL,
	0x3ff571ed3c506b3aULL, 0x3ffba3fb14672d7cULL,
	0xbc8117d397dcefc9ULL, 0x0000000000000000ULL,
	0x3ff571ed3c506b3aULL, 0x40038b79579d3eabULL,
	0xbcac254fc0db598eULL, 0x0000000000000000ULL,
	0x3ff5555555555555ULL, 0x3ffbb67ae8584caaULL,
	0x3c9cec95d0b5c1e3ULL, 0x0000000000000000ULL,
	0x3ff5555555555555ULL, 0x4003988e1409212eULL,
	0x3caf40c86450c869ULL, 0x0000000000000000ULL,
	0x3ff5390948f40febULL, 0x3ffbc8ee6b2865b9ULL,
	0x3c9394eb90f645c8ULL, 0x0000000000000000ULL,
	0x3ff5390948f40febULL, 0x4003a59a1adbb257ULL,
	0x3ca6adce020a308dULL, 0x0000000000000000ULL,
	0x3ff51d07eae2f815ULL, 0x3ffbdb55b550fdbcULL,
	0x3c7365e96aa5fae3ULL, 0x0000000000000000ULL,
	0x3ff51d07eae2f815ULL, 0x4003b29d7d635662ULL,
	0x3cac99b05e282129ULL, 0x0000000000000000ULL,
	0x3ff5015015015015ULL, 0x3ffbedb0defaf661ULL,
	0x3c91a627b279170dULL, 0x0000000000000000ULL,
	0x3ff5015015015015ULL, 0x4003bf984cb56c77ULL,
	0x3ca8f653bcc0c4a1ULL, 0x0000000000000000ULL,
	0x3ff4e5e0a72f0539ULL, 0x3ffc000000000000ULL,
	0x0000000000000000ULL, 0x0000000000000000ULL,
	0x3ff4e5e0a72f0539ULL, 0x4003cc8a99af5453ULL,
	0xbc4863644f05f2beULL, 0x0000000000000000ULL,
	0x3ff4cab88725af6eULL, 0x3ffc12432fec0329ULL,
	0x3c96e0d78dd23a7dULL, 0x0000000000000000ULL,
	0x3ff4cab88725af6eULL, 0x4003d97474f76df2ULL,
	0x3c82e3c9fdbbbdc2ULL, 0x0000000000000000ULL,
	0x3ff4afd6a052bf5bULL, 0x3ffc247a85fe81faULL,
	0x3c89d8eef6854220ULL, 0x0000000000000000ULL,
	0x3ff4afd6a052bf5bULL, 0x4003e655eefe1367ULL,
	0x3c80eb35bb532559ULL, 0x0000000000000000ULL,
	0x3ff49539e3b2d067ULL, 0x3ffc36a6192bf168ULL,
	0xbc9083d81a423b11ULL, 0x0000000000000000ULL,
	0x3ff49539e3b2d067ULL, 0x4003f32f17fe8d04ULL,
	0xbc905d6c1c437de0ULL, 0x0000000000000000ULL,
	0x3ff47ae147ae147bULL, 0x3ffc48c6001f0ac0ULL,
	0xbc92d481189efd6bULL, 0x0000000000000000ULL,
	0x3ff47ae147ae147bULL, 0x4004000000000000ULL,
	0x0000000000000000ULL, 0x0000000000000000ULL,
	0x3ff460cbc7f5cf9aULL, 0x3ffc5ada513a1593ULL,
	0xbc7aaedd014f5f03ULL, 0x0000000000000000ULL,
	0x3ff460cbc7f5cf9aULL, 0x40040cc8b6d657c2ULL,
	0xbc9c05abf480ce19ULL, 0x0000000000000000ULL,
	0x3ff446f86562d9fbULL, 0x3ffc6ce322982a3fULL,
	0x3c891b2df3e15f29ULL, 0x0000000000000000ULL,
	0x3ff446f86562d9fbULL, 0x400419894c2329f0ULL,
	0x3c97603746da0ea6ULL, 0x0000000000000000ULL,
	0x3ff42d6625d51f87ULL, 0x3ffc7ee08a0e6d4cULL,
	0x3c991c54c53e75c8ULL, 0x0000000000000000ULL,
	0x3ff42d6625d51f87ULL, 0x40042641cf569572ULL,
	0xbcadf80b1442c029ULL, 0x0000000000000000ULL,
	0x3ff4141414141414ULL, 0x3ffc90d29d2d43ceULL,
	0xbc9edadb07f1137aULL, 0x0000000000000000ULL,
	0x3ff4141414141414ULL, 0x400432f24fb01c7aULL,
	0x3ca38bfe0e012c1cULL, 0x0000000000000000ULL,
	0x3ff3fb013fb013fbULL, 0x3ffca2b9714180f7ULL,
	0xbc81a63d6750c57cULL, 0x0000000000000000ULL,
	0x3ff3fb013fb013fbULL, 0x40043f9adc3f79ceULL,
	0x3c66d2b1767ae30aULL, 0x0000000000000000ULL,
	0x3ff3e22cbce4a902ULL, 0x3ffcb4951b558d17ULL,
	0x3c8fcbcb357f2308ULL, 0x0000000000000000ULL,
	0x3ff3e22cbce4a902ULL, 0x40044c3b83e57153ULL,
	0x3c98c853c6be5ee1ULL, 0x0000000000000000ULL,
	0x3ff3c995a47babe7ULL, 0x3ffcc665b0328622ULL,
	0xbc91baa4d369f814ULL, 0x0000000000000000ULL,
	0x3ff3c995a47babe7ULL, 0x400458d455549c1aULL,
	0x3ca02d728d9a6054ULL, 0x0000000000000000ULL,
	0x3ff3b13b13b13b14ULL, 0x3ffcd82b446159f3ULL,
	0x3c983fb7b33cdfe8ULL, 0x0000000000000000ULL,
	0x3ff3b13b13b13b14ULL, 0x400465655f122ff6ULL,
	0x3ca862c5d2f0ca4cULL, 0x0000000000000000ULL,
	0x3ff3991c2c187f63ULL, 0x3ffce9e5ec2bda80ULL,
	0xbc94ccf3d8e249abULL, 0x0000000000000000ULL,
	0x3ff3991c2c187f63ULL, 0x400471eeaf76c2c6ULL,
	0x3c975c62eff26e8eULL, 0x0000000000000000ULL,
	0x3ff3813813813814ULL, 0x3ffcfb95bb9dcc0cULL,
	0x3c92cea20857ae03ULL, 0x0000000000000000ULL,
	0x3ff3813813813814ULL, 0x40047e7054af0989ULL,
	0x3c9d8c33c0054830ULL, 0x0000000000000000ULL,
	0x3ff3698df3de0748ULL, 0x3ffd0d3ac685eda4ULL,
	0x3c94115a0ff4cf9eULL, 0x0000000000000000ULL,
	0x3ff3698df3de0748ULL, 0x40048aea5cbc935fULL,
	0xbca8cb0012d14ff5ULL, 0x0000000000000000ULL,
	0x3ff3521cfb2b78c1ULL, 0x3ffd1ed52076fbe9ULL,
	0x3c8f48a86b72875fULL, 0x0000000000000000ULL,
	0x3ff3521cfb2b78c1ULL, 0x4004975cd5768088ULL,
	0xbca1731ebc02f748ULL, 0x0000000000000000ULL,
	0x3ff33ae45b57bcb2ULL, 0x3ffd3064dcc8ae67ULL,
	0x3c93480e805158baULL, 0x0000000000000000ULL,
	0x3ff33ae45b57bcb2ULL, 0x4004a3c7cc8a358aULL,
	0xbc9d8f7fd2726ffaULL, 0x0000000000000000ULL,
	0x3ff323e34a2b10bfULL, 0x3ffd41ea0e98af91ULL,
	0x3c8246400309962fULL, 0x0000000000000000ULL,
	0x3ff323e34a2b10bfULL, 0x4004b02b4f7c0a88ULL,
	0xbcaf71e1f6cafde2ULL, 0x0000000000000000ULL,
	0x3ff30d190130d190ULL, 0x3ffd5364c8cb8f86ULL,
	0x3c8ad003c00630e1ULL, 0x0000000000000000ULL,
	0x3ff30d190130d190ULL, 0x4004bc876ba7f6ecULL,
	0x3c9c1edb2be943b8ULL, 0x0000000000000000ULL,
	0x3ff2f684bda12f68ULL, 0x3ffd64d51e0db1c6ULL,
	0xbc911ed36986d362ULL, 0x0000000000000000ULL,
	0x3ff2f684bda12f68ULL, 0x4004c8dc2e423980ULL,
	0xbc949d1f46ef5d2cULL, 0x0000000000000000ULL,
	0x3ff2e025c04b8097ULL, 0x3ffd763b20d435efULL,
	0x3c9d6780f76cb258ULL, 0x0000000000000000ULL,
	0x3ff2e025c04b8097ULL, 0x4004d529a457fcfcULL,
	0xbca1404a46484e3dULL, 0x0000000000000000ULL,
	0x3ff2c9fb4d812ca0ULL, 0x3ffd8796e35ddbb2ULL,
	0x3c83fdd91aeb637aULL, 0x0000000000000000ULL,
	0x3ff2c9fb4d812ca0ULL, 0x4004e16fdacff937ULL,
	0xbca1deb9d3815ad2ULL, 0x0000000000000000ULL,
	0x3ff2b404ad012b40ULL, 0x3ffd98e877b3e207ULL,
	0xbc48c301ee02dee8ULL, 0x0000000000000000ULL,
	0x3ff2b404ad012b40ULL, 0x4004edaede6b10feULL,
	0x3ca997094a91a780ULL, 0x0000000000000000ULL,
	0x3ff29e4129e4129eULL, 0x3ffdaa2fefaae1d8ULL,
	0xbc63fe0e03f44594ULL, 0x0000000000000000ULL,
	0x3ff29e4129e4129eULL, 0x4004f9e6bbc4ecb3ULL,
	0x3c6ce5a6018493f1ULL, 0x0000000000000000ULL,
	0x3ff288b01288b013ULL, 0x3ffdbb6d5ce3a42fULL,
	0xbc922c27f71c8337ULL, 0x0000000000000000ULL,
	0x3ff288b01288b013ULL, 0x400506177f5491bbULL,
	0xbc9e591e7b2a6d1aULL, 0x0000000000000000ULL,
	0x3ff27350b8812735ULL, 0x3ffdcca0d0cbf408ULL,
	0x3c7a6d162310db57ULL, 0x0000000000000000ULL,
	0x3ff27350b8812735ULL, 0x40051241356cf6e0ULL,
	0x3ca37dc260e8bc2dULL, 0x0000000000000000ULL,
	0x3ff25e22708092f1ULL, 0x3ffdddca5c9f6be8ULL,
	0x3c818520f0a3f809ULL, 0x0000000000000000ULL,
	0x3ff25e22708092f1ULL, 0x40051e63ea3d95b0ULL,
	0x3caecf782e88d5ceULL, 0x0000000000000000ULL,
	0x3ff2492492492492ULL, 0x3ffdeeea11683f49ULL,
	0x3c802aae4bfa7c27ULL, 0x0000000000000000ULL,
	0x3ff2492492492492ULL, 0x40052a7fa9d2f8eaULL,
	0xbca21c62b033c079ULL, 0x0000000000000000ULL,
	0x3ff23456789abcdfULL, 0x3ffe000000000000ULL,
	0x0000000000000000ULL, 0x0000000000000000ULL,
	0x3ff23456789abcdfULL, 0x4005369480174810ULL,
	0xbc9c3ec1a4ee7c21ULL, 0x0000000000000000ULL,
	0x3ff21fb78121fb78ULL, 0x3ffe110c39105fafULL,
	0x3c7761614c513964ULL, 0x0000000000000000ULL,
	0x3ff21fb78121fb78ULL, 0x400542a278d2d036ULL,
	0xbca495c245254df4ULL, 0x0000000000000000ULL,
	0x3ff20b470c67c0d9ULL, 0x3ffe220ecd13ed60ULL,
	0xbc729f01f18c9dc9ULL, 0x0000000000000000ULL,
	0x3ff20b470c67c0d9ULL, 0x40054ea99fac8a0fULL,
	0x3c80cfbb19353b3dULL, 0x0000000000000000ULL,
	0x3ff1f7047dc11f70ULL, 0x3ffe3307cc56cf5cULL,
	0xbc81f04ec3189131ULL, 0x0000000000000000ULL,
	0x3ff1f7047dc11f70ULL, 0x40055aaa002a9d5aULL,
	0xbc4bf50476241f94ULL, 0x0000000000000000ULL,
	0x3ff1e2ef3b3fb874ULL, 0x3ffe43f746f7795bULL,
	0xbc931e7f8af68f8cULL, 0x0000000000000000ULL,
	0x3ff1e2ef3b3fb874ULL, 0x400566a3a5b2e1b1ULL,
	0x3caa1fd28cc92e33ULL, 0x0000000000000000ULL,
	0x3ff1cf06ada2811dULL, 0x3ffe54dd4ce75f1eULL,
	0xbc811b195dfc62e5ULL, 0x0000000000000000ULL,
	0x3ff1cf06ada2811dULL, 0x400572969b8b5cd8ULL,
	0x3ca30cbf1c53312eULL, 0x0000000000000000ULL,
	0x3ff1bb4a4046ed29ULL, 0x3ffe65b9edeba38eULL,
	0xbc7bb73251e8c364ULL, 0x0000000000000000ULL,
	0x3ff1bb4a4046ed29ULL, 0x40057e82ecdabe8dULL,
	0xbc7c2aedf3c4c4bdULL, 0x0000000000000000ULL,
	0x3ff1a7b9611a7b96ULL, 0x3ffe768d399dc470ULL,
	0xbc9a8c813405c01cULL, 0x0000000000000000ULL,
	0x3ff1a7b9611a7b96ULL, 0x40058a68a4a8d9f3ULL,
	0x3ca50798e67012d9ULL, 0x0000000000000000ULL,
	0x3ff19453808ca29cULL, 0x3ffe87573f6c42c5ULL,
	0x3c9dbf9cf7bbcda3ULL, 0x0000000000000000ULL,
	0x3ff19453808ca29cULL, 0x40059647cddf1ca5ULL,
	0x3ca14a95f35dea0bULL, 0x0000000000000000ULL,
	0x3ff1811811811812ULL, 0x3ffe98180e9b47f2ULL,
	0xbc9b6bd74396d08eULL, 0x0000000000000000ULL,
	0x3ff1811811811812ULL, 0x4005a22073490377ULL,
	0xbcadd03639925812ULL, 0x0000000000000000ULL,
	0x3ff16e0689427379ULL, 0x3ffea8cfb64547abULL,
	0x3c8721b26374e19fULL, 0x0000000000000000ULL,
	0x3ff16e0689427379ULL, 0x4005adf29f948cfbULL,
	0xbca42520f7716fa6ULL, 0x0000000000000000ULL,
	0x3ff15b1e5f75270dULL, 0x3ffeb97e455b9edbULL,
	0x3c999b4540857883ULL, 0x0000000000000000ULL,
	0x3ff15b1e5f75270dULL, 0x4005b9be5d52a9daULL,
	0x3c9098cd1b3af777ULL, 0x0000000000000000ULL,
	0x3ff1485f0e0acd3bULL, 0x3ffeca23caa72f73ULL,
	0x3c7e3ed529679959ULL, 0x0000000000000000ULL,
	0x3ff1485f0e0acd3bULL, 0x4005c583b6f7ab03ULL,
	0x3ca963bc9d795b51ULL, 0x0000000000000000ULL,
	0x3ff135c81135c811ULL, 0x3ffedac054c8f94cULL,
	0x3c90b5c115a56207ULL, 0x0000000000000000ULL,
	0x3ff135c81135c811ULL, 0x4005d142b6dbadc5ULL,
	0x3ca6f1f55323d116ULL, 0x0000000000000000ULL,
	0x3ff12358e75d3033ULL, 0x3ffeeb53f23ab028ULL,
	0xbc8617e4b5384f5dULL, 0x0000000000000000ULL,
	0x3ff12358e75d3033ULL, 0x4005dcfb673b05dfULL,
	0xbca099dfc321634fULL, 0x0000000000000000ULL,
	0x3ff1111111111111ULL, 0x3ffefbdeb14f4edaULL,
	0xbc93a145fe1be078ULL, 0x0000000000000000ULL,
	0x3ff1111111111111ULL, 0x4005e8add236a58fULL,
	0xbc7ef8c7c0d1fec6ULL, 0x0000000000000000ULL,
	0x3ff0fef010fef011ULL, 0x3fff0c60a033a7b3ULL,
	0xbc91b0fc15cd89c6ULL, 0x0000000000000000ULL,
	0x3ff0fef010fef011ULL, 0x4005f45a01d483b4ULL,
	0xbc94a237dc0fa105ULL, 0x0000000000000000ULL,
	0x3ff0ecf56be69c90ULL, 0x3fff1cd9cceef239ULL,
	0x3c91afd864eab60aULL, 0x0000000000000000ULL,
	0x3ff0ecf56be69c90ULL, 0x4006000000000000ULL,
	0x0000000000000000ULL, 0x0000000000000000ULL,
	0x3ff0db20a88f4696ULL, 0x3fff2d4a45635640ULL,
	0xbc8eebaeea670bc2ULL, 0x0000000000000000ULL,
	0x3ff0db20a88f4696ULL, 0x40060b9fd68a4554ULL,
	0x3ca328e170dae176ULL, 0x0000000000000000ULL,
	0x3ff0c9714fbcda3bULL, 0x3fff3db2174e7468ULL,
	0x3c9e15132d6ac52aULL, 0x0000000000000000ULL,
	0x3ff0c9714fbcda3bULL, 0x400617398f2aaa48ULL,
	0xbc9b672bba260735ULL, 0x0000000000000000ULL,
	0x3ff0b7e6ec259dc8ULL, 0x3fff4e115049ec26ULL,
	0xbc9b6656b6bd5d76ULL, 0x0000000000000000ULL,
	0x3ff0b7e6ec259dc8ULL, 0x400622cd337f0fe8ULL,
	0x3c9fe2073279559fULL, 0x0000000000000000ULL,
	0x3ff0a6810a6810a7ULL, 0x3fff5e67fdcbdf44ULL,
	0xbc98af061849d6fcULL, 0x0000000000000000ULL,
	0x3ff0a6810a6810a7ULL, 0x40062e5acd0c3ebeULL,
	0xbca2c50e2092203aULL, 0x0000000000000000ULL,
	0x3ff0953f39010954ULL, 0x3fff6eb62d27730dULL,
	0xbc9401d95ca1ce34ULL, 0x0000000000000000ULL,
	0x3ff0953f39010954ULL, 0x400639e2653e421bULL,
	0xbc9f75e05835e4b9ULL, 0x0000000000000000ULL,
	0x3ff0842108421084ULL, 0x3fff7efbeb8d4f12ULL,
	0xbc7e84e8a6ff3256ULL, 0x0000000000000000ULL,
	0x3ff0842108421084ULL, 0x400645640568c1c3ULL,
	0x3cad17787e4c8970ULL, 0x0000000000000000ULL,
	0x3ff073260a47f7c6ULL, 0x3fff8f39460c19a8ULL,
	0x3c989b4e16ee9aafULL, 0x0000000000000000ULL,
	0x3ff073260a47f7c6ULL, 0x400650dfb6c759f4ULL,
	0x3c99063c91db4c77ULL, 0x0000000000000000ULL,
	0x3ff0624dd2f1a9fcULL, 0x3fff9f6e4990f227ULL,
	0x3c8b42e5b5d1e808ULL, 0x0000000000000000ULL,
	0x3ff0624dd2f1a9fcULL, 0x40065c55827df1d2ULL,
	0xbca3923df03e1e2fULL, 0x0000000000000000ULL,
	0x3ff05197f7d73404ULL, 0x3fffaf9b02e7e8f2ULL,
	0x3c897a768f34e1c2ULL, 0x0000000000000000ULL,
	0x3ff05197f7d73404ULL, 0x400667c57199104bULL,
	0x3c875b896f332e70ULL, 0x0000000000000000ULL,
	0x3ff0410410410410ULL, 0x3fffbfbf7ebc755fULL,
	0xbc9b2a94084da0b6ULL, 0x0000000000000000ULL,
	0x3ff0410410410410ULL, 0x4006732f8d0e2f77ULL,
	0xbc93dffd470422e3ULL, 0x0000000000000000ULL,
	0x3ff03091b51f5e1aULL, 0x3fffcfdbc999e97dULL,
	0x3c82be17ecdd3bbcULL, 0x0000000000000000ULL,
	0x3ff03091b51f5e1aULL, 0x40067e93ddbc0e73ULL,
	0xbc86eb9f32ac1a5cULL, 0x0000000000000000ULL,
	0x3ff0204081020408ULL, 0x3fffdfefefebe3d6ULL,
	0xbc909afcfc7c1f3bULL, 0x0000000000000000ULL,
	0x3ff0204081020408ULL, 0x400689f26c6b01d0ULL,
	0x3cae816f9d2a1032ULL, 0x0000000000000000ULL,
	0x3ff0101010101010ULL, 0x3fffeffbfdfebf1fULL,
	0x3c95dee51994f18bULL, 0x0000000000000000ULL,
	0x3ff0101010101010ULL, 0x4006954b41cd4293ULL,
	0x3ca3d5bccc443076ULL, 0x0000000000000000ULL,
	0x3ff0000000000000ULL, 0x4000000000000000ULL,
	0x0000000000000000ULL, 0x0000000000000000ULL,
	0x3ff0000000000000ULL, 0x4006a09e667f3bcdULL,
	0xbcabdd3413b26456ULL, 0x0000000000000000ULL,
};

/* *********************************************************** */

mlib_status
__mlib_VectorMag_S32C(
	mlib_d64 *m,
	const mlib_s32 *x,
	mlib_s32 n)
{
	mlib_s32 i;
	mlib_s32 *px = (mlib_s32 *)x;

	mlib_d64 xx, dexp_hi, dexp_lo, dsqrt_exp;
	mlib_d64 res, res_c, ld;
	mlib_s64 sqrt_exp;
	long ind0;
	void *pind;
	mlib_s32 hx;

	mlib_d64 A1 = 5.00000000000000001789e-01;
	mlib_d64 A2 = -1.24999999997314110667e-01;
	mlib_d64 A3 = 6.24999999978896565817e-02;
	mlib_d64 A4 = -3.90629693917215481458e-02;
	mlib_d64 A5 = 2.73441188080261677282e-02;
	mlib_d64 DC0 = vis_to_double(0x000fffff, 0xffffffff);
	mlib_d64 DC2 = vis_to_double(0x00001000, 0);
	mlib_d64 DC3 = vis_to_double(0x7fffe000, 0);

	ld = vis_ld_s32_nf(px) * (mlib_d64)vis_ld_s32_nf(px) +
		vis_ld_s32_nf(px + 1) * (mlib_d64)vis_ld_s32_nf(px + 1);
	res = ld;
#pragma pipeloop(0)
	for (i = 0; i < n; i++) {
		hx = ((mlib_s32 *)&ld)[0];
		sqrt_exp = ((mlib_s64)(hx >> 21) + 511) << 52;
		dsqrt_exp = *(mlib_d64 *)&sqrt_exp;
		ind0 = (((long)(hx >> 7) & 0x1ff8) + 32) & -64;
		hx >>= 15;
		ind0 += ((long)hx & 32) - (((-hx) >> 31) & ~0x1f);

		res = vis_fand(res, DC0);
		res = vis_for(res, A1);
		res_c = vis_fpadd32(res, DC2);
		res_c = vis_fand(res_c, DC3);

		pind = (mlib_s8 *)TBL + ind0;
		dexp_hi = ((mlib_d64 *)pind)[1];
		dexp_lo = ((mlib_d64 *)pind)[2];

		xx = (res - res_c) * ((mlib_d64 *)pind)[0];
		res = ((((A5 * xx + A4) * xx + A3) * xx + A2) * xx + A1) * xx;

		res = dexp_hi * res;
		res += dexp_lo;
		res += dexp_hi;

		m[i] = (mlib_d64)dsqrt_exp *res;

		ld = vis_ld_s32_nf(px + 2 * i + 2) *
			(mlib_d64)vis_ld_s32_nf(px + 2 * i + 2) +
			vis_ld_s32_nf(px + 2 * i + 3) *
			(mlib_d64)vis_ld_s32_nf(px + 2 * i + 3);
		res = ld;
	}

	return ((n > 0) ? MLIB_SUCCESS : MLIB_FAILURE);
}

/* *********************************************************** */
