/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/usr/hwpf/hwp/slave_sbe/proc_spless_sbe_startWA/proc_spless_sbe_startWA.H $ */
/*                                                                        */
/* OpenPOWER HostBoot Project                                             */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2013,2015                        */
/* [+] International Business Machines Corp.                              */
/*                                                                        */
/*                                                                        */
/* Licensed under the Apache License, Version 2.0 (the "License");        */
/* you may not use this file except in compliance with the License.       */
/* You may obtain a copy of the License at                                */
/*                                                                        */
/*     http://www.apache.org/licenses/LICENSE-2.0                         */
/*                                                                        */
/* Unless required by applicable law or agreed to in writing, software    */
/* distributed under the License is distributed on an "AS IS" BASIS,      */
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or        */
/* implied. See the License for the specific language governing           */
/* permissions and limitations under the License.                         */
/*                                                                        */
/* IBM_PROLOG_END_TAG                                                     */
// $Id: proc_spless_sbe_startWA.H,v 1.1 2015/08/04 22:04:52 baiocchi Exp $
// $Source: /afs/awd/projects/eclipz/KnowledgeBase/.cvsroot/eclipz/chips/p8/working/procedures/ipl/fapi/proc_spless_sbe_startWA.H,v $
//------------------------------------------------------------------------------
// *|
// *! (C) Copyright International Business Machines Corp. 2013
// *! All Rights Reserved -- Property of IBM
// *! ***  ***
// *|
// *! TITLE       : proc_spless_sbe_startWA.C
// *! DESCRIPTION : Issue workaround for CFAM Reset SBE start (FAPI)
// *!
// *! OWNER  NAME  : Benedikt Geukes         Email: benedikt.geukes@de.ibm.com
// *! BACKUP NAME  : Ralph Koester           Email: rkoester@de.ibm.com
// *!
//------------------------------------------------------------------------------

#ifndef _PROC_SPLESS_SBE_STARTWA_H_
#define _PROC_SPLESS_SBE_STARTWA_H_

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include <fapi.H>

//------------------------------------------------------------------------------
// Structure definitions
//------------------------------------------------------------------------------

// function pointer typedef definition for HWP call support
typedef fapi::ReturnCode (*proc_spless_sbe_startWA_FP_t)(const fapi::Target &,
                                                         const bool );


//------------------------------------------------------------------------------
// Constant definitions
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Function prototypes
//------------------------------------------------------------------------------


extern "C"
{

/**
 * @brief Issue workaround for SPless slave chips (ie started by CFAM Reset)
 *        Note this only applies to DD1.x parts (both murano/venice)
 *        The master chip is started by the FPGA, but the slaves need to be
 *        started by Hostboot
 *
 * @param[in] i_target            chip target
 * @param[in] i_sbeSeepromSelect  SBE Seeprom Select Bit:
 *                                if false then first image; otherwise 2nd image
 *
 * @return ReturnCode
 */
  fapi::ReturnCode proc_spless_sbe_startWA(const fapi::Target & i_target,
                                           const bool  i_sbeSeepromSelect);

} // extern "C"

#endif // _PROC_SPLESS_SBE_STARTWA_H_
