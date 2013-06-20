/*****************************************************************************/
/*!
 *\MODULE              Message types
 *
 *\COMPONENT           $HeadURL: https://www.collabnet.nxp.com/svn/lprf_apps/Application_Notes/JN-AN-1134-ZigBee-PRO-Serial-Cable-Replacement/Tags/Release_1v3-Public/Coordinator/Source/os_msg_types.h $
 *
 *\VERSION			   $Revision: 5797 $
 *
 *\REVISION            $Id: os_msg_types.h 5797 2010-04-20 12:26:04Z mlook $
 *
 *\DATED               $Date: 2010-04-20 13:26:04 +0100 (Tue, 20 Apr 2010) $
 *
 *\AUTHOR              $Author: mlook $
 *
 *\DESCRIPTION         Message types
 */
/*****************************************************************************
 *
 * This software is owned by Jennic and/or its supplier and is protected
 * under applicable copyright laws. All rights are reserved. We grant You,
 * and any third parties, a license to use this software solely and
 * exclusively on Jennic products. You, and any third parties must reproduce
 * the copyright and warranty notice and any other legend of ownership on each
 * copy or partial copy of the software.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS". JENNIC MAKES NO WARRANTIES, WHETHER
 * EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE,
 * ACCURACY OR LACK OF NEGLIGENCE. JENNIC SHALL NOT, IN ANY CIRCUMSTANCES,
 * BE LIABLE FOR ANY DAMAGES, INCLUDING, BUT NOT LIMITED TO, SPECIAL,
 * INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON WHATSOEVER.
 *
 * Copyright Jennic Ltd 2010. All rights reserved
 *
 ****************************************************************************/
#ifndef MSG_TYPES_H_
#define MSG_TYPES_H_

#include "jendefs.h"
#include "pdum_apl.h"

#define ZPS_NWK_OPT_ALL

#include "zps_apl_af.h"
#include "zps_apl.h"

#ifdef SPLIT_STACK
#include "zps_apl_nwk_spidl.h"
#endif

#endif /*MSG_TYPES_H_*/
