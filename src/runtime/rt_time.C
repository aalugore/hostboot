/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/runtime/rt_time.C $                                       */
/*                                                                        */
/* OpenPOWER HostBoot Project                                             */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2014                             */
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
#include <sys/time.h>
#include <runtime/interface.h>

void nanosleep(uint64_t sec, uint64_t nsec)
{
    if (g_hostInterfaces && g_hostInterfaces->nanosleep)
    {
        g_hostInterfaces->nanosleep(sec, nsec);
    }
}

int clock_gettime(clockid_t i_clkId, timespec_t* o_tp)
{
    int l_rc = -1;
    if (g_hostInterfaces && g_hostInterfaces->clock_gettime)
    {
        l_rc = g_hostInterfaces->clock_gettime(i_clkId, o_tp);
    }
    return l_rc;
}
