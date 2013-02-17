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

#pragma ident	"@(#)mlib_version.c	9.2	07/10/09 SMI"

#ifndef MLIB_VERSION
#define	MLIB_VERSION	"mediaLib"
#endif

#include <mlib_types.h>
#include <mlib_sys_proto.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_version = __mlib_version

#elif defined(__GNUC__)

__typeof__(__mlib_version) mlib_version
    __attribute__((weak, alias("__mlib_version")));

#else

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

const char mlibid[] = "@(#)" MLIB_VERSION;

/*
 * This function returns a string to identify the version of the mediaLib.
 * The token MLIB_VERSION should be defined in building script. It should
 * contains information of version number, building time, and architecture.
 */
char *
__mlib_version()
{
	return ((char *)(MLIB_VERSION));
}
