# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/build/beam/beam_parms.tcl $
#
# IBM CONFIDENTIAL
#
# COPYRIGHT International Business Machines Corp. 2013
#
# p1
#
# Object Code Only (OCO) source materials
# Licensed Internal Code Source Materials
# IBM HostBoot Licensed Internal Code
#
# The source code for this program is not published or otherwise
# divested of its trade secrets, irrespective of what has been
# deposited with the U.S. Copyright Office.
#
# Origin: 30
#
# IBM_PROLOG_END_TAG
##  Set maximum time to spend analyzing to 1 minute per 1000 lines of code.
##  The default is 5 minutes; there are a few files in HB that take
##  more than an hour with the default setting.
set beam::max_time_per_kloc_in_sec "60"