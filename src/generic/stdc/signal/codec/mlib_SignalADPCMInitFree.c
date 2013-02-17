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

#pragma ident	"@(#)mlib_SignalADPCMInitFree.c	9.2	07/10/09 SMI"

/*
 * FUNCTIONS
 *
 *   Function mlib_SignalADPCMInit() -
 *   allocate the memory for internal encoder(decoder) structure and
 *   initialized this structure.
 *
 *   Function mlib_SignalADPCMFree() -
 *   release the memory allocated for the internal structure of the
 *   encoder(decoder)
 *
 * SYNOPSIS
 *
 *   mlib_status mlib_SignalADPCMInit(void **state)
 *
 *   void mlib_SignalADPCMFree(void *state)
 *
 * ARGUMENTS
 *
 *   state    Internal encoder(decoder) structure
 *
 */

#include <mlib_signal.h>
#include <mlib_SignalCodec.h>

/* *********************************************************** */

#if ! defined(__MEDIALIB_OLD_NAMES)
#if defined(__SUNPRO_C)

#pragma weak mlib_SignalADPCMFree = __mlib_SignalADPCMFree
#pragma weak mlib_SignalADPCMInit = __mlib_SignalADPCMInit

#elif defined(__GNUC__)	/* defined(__SUNPRO_C) */
__typeof__(__mlib_SignalADPCMFree) mlib_SignalADPCMFree
    __attribute__((weak, alias("__mlib_SignalADPCMFree")));
__typeof__(__mlib_SignalADPCMInit) mlib_SignalADPCMInit
    __attribute__((weak, alias("__mlib_SignalADPCMInit")));

#else /* defined(__SUNPRO_C) */

#error  "unknown platform"

#endif /* defined(__SUNPRO_C) */
#endif /* ! defined(__MEDIALIB_OLD_NAMES) */

/* *********************************************************** */

mlib_status
__mlib_SignalADPCMInit(
    void **state)
{
	mlib_G72x_state *pstate;

	*state = (void *)__mlib_malloc(sizeof (mlib_G72x_state));

	if (*state == NULL)
		return (MLIB_FAILURE);

	pstate = (mlib_G72x_state *) (*state);

	pstate->yl = 34816;
	pstate->yu = 544;

	pstate->dms =
	    pstate->dml =
	    pstate->ap =
	    pstate->a[0] = pstate->a[1] =
	    pstate->pk[0] = pstate->pk[1] =
	    pstate->b[0] = pstate->b[1] =
	    pstate->b[2] = pstate->b[3] =
	    pstate->b[4] = pstate->b[5] = pstate->td = 0;

	pstate->sr[0] = pstate->sr[1] =
	    pstate->dq[0] = pstate->dq[1] =
	    pstate->dq[2] = pstate->dq[3] = pstate->dq[4] = pstate->dq[5] = 0;

	return (MLIB_SUCCESS);
}

/* *********************************************************** */

void
__mlib_SignalADPCMFree(
    void *state)
{
	if (state != NULL)
		__mlib_free(state);
}

/* *********************************************************** */
