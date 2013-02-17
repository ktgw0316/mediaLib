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

#pragma ident	"@(#)mlib_c_ImageChannelSplit_U8_3.c	9.2	07/10/09 SMI"

/*
 * FUNCTION
 *      mlib_ImageChannelSplit3_U8
 *
 * DESCRIPTION
 *      Internal function.
 *
 */

#include <mlib_image.h>
#include <mlib_ImageChannelSplit.h>

/* *********************************************************** */

mlib_status
mlib_ImageChannelSplit3_U8(
    mlib_u8 *dst_u8_0,
    mlib_u8 *dst_u8_1,
    mlib_u8 *dst_u8_2,
    const mlib_u8 *src_u8_0,
    mlib_s32 height,
    mlib_s32 width,
    mlib_s32 dst0_str,
    mlib_s32 dst1_str,
    mlib_s32 dst2_str,
    mlib_s32 src0_str)
{
	mlib_s32 j, i, n = width * 3;
	mlib_u8 *dst0, *dst1, *dst2, *src;
	mlib_u32 uint0, uint1, uint2;

	for (j = 0; j < height; j++) {

		i = 0;

		dst0 = dst_u8_0;
		dst1 = dst_u8_1;
		dst2 = dst_u8_2;
		src = (void *)src_u8_0;

		if ((mlib_addr)(src_u8_0 + i) & 3) {
			dst_u8_0[0] = (*src++);
			i++;

			if ((mlib_addr)(src_u8_0 + i) & 3) {
				dst_u8_1[0] = (*src++);
				i++;

				if ((mlib_addr)(src_u8_0 + i) & 3) {
					dst_u8_2[0] = (*src++);
					i++;
				}
			}
		}

		if (i == 3) {
			dst0 = dst_u8_0 + 1;
			dst1 = dst_u8_1 + 1;
			dst2 = dst_u8_2 + 1;
		} else if (i == 2) {
			dst1 = dst_u8_0 + 1;
			dst2 = dst_u8_1 + 1;
			dst0 = dst_u8_2 + 0;
		} else if (i == 1) {
			dst2 = dst_u8_0 + 1;
			dst0 = dst_u8_1 + 0;
			dst1 = dst_u8_2 + 0;
		}
#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; i < (n - 11); i += 12) {
			uint0 = ((mlib_u32 *)src)[0];
			uint1 = ((mlib_u32 *)src)[1];
			uint2 = ((mlib_u32 *)src)[2];
#ifdef _LITTLE_ENDIAN
			dst0[0] = uint0;
			dst0[1] = uint0 >> 24;
			dst0[2] = uint1 >> 16;
			dst0[3] = uint2 >> 8;

			dst1[0] = uint0 >> 8;
			dst1[1] = uint1;
			dst1[2] = uint1 >> 24;
			dst1[3] = uint2 >> 16;

			dst2[0] = uint0 >> 16;
			dst2[1] = uint1 >> 8;
			dst2[2] = uint2;
			dst2[3] = uint2 >> 24;
#else /* _LITTLE_ENDIAN */
			dst0[1] = uint0;
			dst2[0] = uint0 >> 8;
			dst1[0] = uint0 >> 16;
			dst0[0] = uint0 >> 24;

			dst1[2] = uint1;
			dst0[2] = uint1 >> 8;
			dst2[1] = uint1 >> 16;
			dst1[1] = uint1 >> 24;

			dst2[3] = uint2;
			dst1[3] = uint2 >> 8;
			dst0[3] = uint2 >> 16;
			dst2[2] = uint2 >> 24;
#endif /* _LITTLE_ENDIAN */
			dst0 += 4;
			dst1 += 4;
			dst2 += 4;
			src += 12;
		}

#ifdef __SUNPRO_C
#pragma pipeloop(0)
#endif /* __SUNPRO_C */
		for (; i < (n - 2); i += 3) {
			(*dst0++) = (*src++);
			(*dst1++) = (*src++);
			(*dst2++) = (*src++);
		}

		if (i < (n - 1)) {
			dst_u8_1[width - 1] = (*src++);
			dst_u8_2[width - 1] = (*src++);
		} else if (i < n) {
			dst_u8_2[width - 1] = (*src++);
		}

		dst_u8_0 += dst0_str;
		dst_u8_1 += dst1_str;
		dst_u8_2 += dst2_str;
		src_u8_0 += src0_str;
	}

	return (MLIB_SUCCESS);
}

/* *********************************************************** */
