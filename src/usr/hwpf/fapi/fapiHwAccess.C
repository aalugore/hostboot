/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/usr/hwpf/fapi/fapiHwAccess.C $                            */
/*                                                                        */
/* OpenPOWER HostBoot Project                                             */
/*                                                                        */
/* COPYRIGHT International Business Machines Corp. 2011,2014              */
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
// $Id: fapiHwAccess.C,v 1.14 2013/10/15 13:13:29 dcrowell Exp $
// $Source: /afs/awd/projects/eclipz/KnowledgeBase/.cvsroot/eclipz/hwpf/working/fapi/fapiHwAccess.C,v $

/**
 *  @file fapiHwAccess.C
 *
 *  @brief Implements the fapiHwAccess.H functions at a high level,
 *  allowing for scand common tracing to occur before and after
 *  the call to the platform-specific worker. 
 *
 *  Note that platform code must provide the implementation.
 */

/*
 * Change Log ******************************************************************
 * Flag     Defect/Feature  User        Date        Description
 * ------   --------------  ----------  ----------- ----------------------------
 *                          copelanm    09/13/2011  new for common scan traces
 *                          mjjones     09/14/2011  Prepended fapi to functions
 *                                                  and enabled all functions
 *                          mjjones     10/13/2011  util namespace change
 *                          mjjones     02/21/2012  Use high performance Target
 *                                                  toEcmdString
 *          836579          thi         May 18,2012 Spy/ring supports
 *                          mjjones     07/12/2012  Add Pulse mode option to
 *                                                  Ring funcs
 *                          rjknight    09/20/2012  Update fapiGetSpy to take
 *                                                  a string as input
 *          F876964         jknight     Apr, 02,2013  fapi get/setSpyImage
 *                                                  support
 *          F873646         srimeesa    Mar 10,2013 64Bit SPYID and 
 *                                                  ClockDomain ID support  
 *          F883863         atendolk    05/06/2013  Update to support MultiScom
 */
#include <fapi.H>
#include <fapiPlatHwAccess.H>

extern "C"
{

//******************************************************************************
// fapiGetScom function
//******************************************************************************
fapi::ReturnCode fapiGetScom(const fapi::Target& i_target,
                             const uint64_t i_address,
                             ecmdDataBufferBase & o_data)
{
    fapi::ReturnCode l_rc;
    bool l_traceit = platIsScanTraceEnabled();

    // call the platform implementation
    l_rc = platGetScom( i_target, i_address, o_data );

    if (l_rc)
    {
        FAPI_ERR("fapiGetScom failed - Target %s, Addr %.16llX",
                  i_target.toEcmdString(), i_address);
    }

    if( l_traceit )
    {
        FAPI_SCAN( "TRACE : GETSCOM     : %s : %.16llX %.16llX",
                   i_target.toEcmdString(),
                   i_address,
                   o_data.getDoubleWord( 0 ) );
    }

    return l_rc;
}


//******************************************************************************
// fapiPutScom function
//******************************************************************************
fapi::ReturnCode fapiPutScom(const fapi::Target& i_target,
                             const uint64_t i_address,
                             ecmdDataBufferBase & i_data)
{
    fapi::ReturnCode l_rc;
    bool l_traceit = platIsScanTraceEnabled();

    // call the platform implementation
    l_rc = platPutScom( i_target, i_address, i_data );

    if (l_rc)
    {
        FAPI_ERR("fapiPutScom failed - Target %s, Addr %.16llX",
                  i_target.toEcmdString(), i_address);
    }

    if( l_traceit )
    {
        FAPI_SCAN( "TRACE : PUTSCOM     : %s : %.16llX %.16llX",
                   i_target.toEcmdString(),
                   i_address,
                   i_data.getDoubleWord( 0 )  );
    }

    return l_rc;
}

//******************************************************************************
// fapiPutScomUnderMask function
//******************************************************************************
fapi::ReturnCode fapiPutScomUnderMask(const fapi::Target& i_target,
                                      const uint64_t i_address,
                                      ecmdDataBufferBase & i_data,
                                      ecmdDataBufferBase & i_mask)
{
    fapi::ReturnCode l_rc;
    bool l_traceit = platIsScanTraceEnabled();

    // call the platform implementation
    l_rc = platPutScomUnderMask( i_target, i_address, i_data, i_mask );

    if (l_rc)
    {
        FAPI_ERR("fapiPutScomUnderMask failed - Target %s, Addr %.16llX",
                  i_target.toEcmdString(), i_address);
    }

    if( l_traceit )
    {
        FAPI_SCAN( "TRACE : PUTSCOMMASK : %s : %.16llX %.16llX %.16llX",
               i_target.toEcmdString(),
               i_address,
               i_data.getDoubleWord(0),
               i_mask.getDoubleWord(0));
    }

    return l_rc;
}

//******************************************************************************
// fapiMultiScom function
//******************************************************************************
#ifdef FAPI_SUPPORT_MULTI_SCOM
fapi::ReturnCode fapiMultiScom (
                    const   fapi::Target&       i_target,
                            fapi::MultiScom&    io_multiScomObj)
{
    FAPI_DBG (ENTER_MRK "fapiMultiScom - i_target: %s, # input ops: %d",
              i_target.toEcmdString (), io_multiScomObj.iv_ScomList.size ());

    fapi::ReturnCode l_rc;

    // Call the platform specific implemetation
    l_rc = platMultiScom (i_target, io_multiScomObj);

    if (!l_rc.ok ())
    {
        uint32_t l_retCode = l_rc;

        FAPI_ERR ("fapiMultiScom Failed with RC: 0x%.8X! i_target: %s, "
                  "# input ops: %d, # ops complete: %d", l_retCode,
                  i_target.toEcmdString (),
                  io_multiScomObj.iv_ScomList.size (),
                  io_multiScomObj.iv_NumOfCompletes);
    }

    FAPI_DBG (EXIT_MRK "fapiMultiScom - i_target: %s, # input ops: %d, "
              "#ops complete: %d", i_target.toEcmdString (),
              io_multiScomObj.iv_ScomList.size (),
              io_multiScomObj.iv_NumOfCompletes);

    return l_rc;
}
#endif // FAPI_SUPPORT_MULTI_SCOM


//******************************************************************************
// fapiGetCfamRegister function
//******************************************************************************
fapi::ReturnCode fapiGetCfamRegister(const fapi::Target& i_target,
                                     const uint32_t i_address,
                                     ecmdDataBufferBase & o_data)
{
    fapi::ReturnCode l_rc;
    bool l_traceit = platIsScanTraceEnabled();

    // call the platform implementation
    l_rc = platGetCfamRegister( i_target, i_address, o_data );

    if (l_rc)
    {
        FAPI_ERR("fapiGetCfamRegister failed - Target %s, Addr %.8X",
                  i_target.toEcmdString(), i_address);
    }

    if( l_traceit )
    {
        FAPI_SCAN( "TRACE : GETCFAMREG  : %s : %.8X %.8X",
               i_target.toEcmdString(),
               i_address,
               o_data.getWord(0) );
    }

    return l_rc;
}

//******************************************************************************
// fapiPutCfamRegister function
//******************************************************************************
fapi::ReturnCode fapiPutCfamRegister(const fapi::Target& i_target,
                                     const uint32_t i_address,
                                     ecmdDataBufferBase & i_data)
{
    fapi::ReturnCode l_rc;
    bool l_traceit = platIsScanTraceEnabled();

    // call the platform implementation
    l_rc = platPutCfamRegister( i_target, i_address, i_data );

    if (l_rc)
    {
        FAPI_ERR("platPutCfamRegister failed - Target %s, Addr %.8X",
                  i_target.toEcmdString(), i_address);
    }

    if( l_traceit )
    {
        FAPI_SCAN( "TRACE : PUTCFAMREG  : %s : %.8X %.8X",
               i_target.toEcmdString(),
               i_address,
               i_data.getWord(0) );
    }

    return l_rc;
}

//******************************************************************************
// fapiModifyCfamRegister function
//******************************************************************************
fapi::ReturnCode fapiModifyCfamRegister(const fapi::Target& i_target,
                                      const uint32_t i_address,
                                      ecmdDataBufferBase & i_data,
                                      const fapi::ChipOpModifyMode i_modifyMode)
{
    fapi::ReturnCode l_rc;
    bool l_traceit = platIsScanTraceEnabled();

    // call the platform implementation
    l_rc = platModifyCfamRegister( i_target, i_address, i_data, i_modifyMode );

    if (l_rc)
    {
        FAPI_ERR("platModifyCfamRegister failed - Target %s, Addr %.8X",
                  i_target.toEcmdString(), i_address);
    }

    if( l_traceit )
    {
        // get string representation of the modify mode
        const char * l_pMode = NULL;

        if (i_modifyMode == fapi::CHIP_OP_MODIFY_MODE_OR)
        {
            l_pMode = "OR";
        }
        else if (i_modifyMode == fapi::CHIP_OP_MODIFY_MODE_AND)
        {
            l_pMode = "AND";
        }
        else if (i_modifyMode == fapi::CHIP_OP_MODIFY_MODE_XOR)
        {
            l_pMode = "XOR";
        }
        else
        {
            l_pMode = "?";
        }

        FAPI_SCAN( "TRACE : MODCFAMREG  : %s : %.8X %.8X %s",
               i_target.toEcmdString(),
               i_address,
               i_data.getWord(0),
               l_pMode );
    }

    return l_rc;
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
fapi::ReturnCode fapiGetRing(const fapi::Target& i_target,
                             const scanRingId_t i_address,
                             ecmdDataBufferBase & o_data,
                             const uint32_t i_ringMode)
{
    fapi::ReturnCode l_rc;
    bool l_traceit = platIsScanTraceEnabled();

    // call the platform implementation
    l_rc = platGetRing( i_target, i_address, o_data, i_ringMode );

    if (l_rc)
    {
        FAPI_ERR("fapiGetRing failed - Target %s, Addr 0x%.16llX", 
                  i_target.toEcmdString(), i_address);
    }

    if( l_traceit )
    {
        FAPI_SCAN( "TRACE : GETRING     : %s : %.16llX %.16llX", 
                   i_target.toEcmdString(),
                   i_address,
                   o_data.getDoubleWord( 0 ) );
    }

    return l_rc;
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
fapi::ReturnCode fapiPutRing(const fapi::Target& i_target,
                             const scanRingId_t i_address,
                             ecmdDataBufferBase & i_data,
                             const uint32_t i_ringMode)
{
    fapi::ReturnCode l_rc;
    bool l_traceit = platIsScanTraceEnabled();

    // call the platform implementation
    l_rc = platPutRing( i_target, i_address, i_data, i_ringMode );

    if (l_rc)
    {
        FAPI_ERR("fapiPutRing failed - Target %s, Addr 0x%.16llX",
                  i_target.toEcmdString(), i_address);
    }

    if( l_traceit )
    {
        FAPI_SCAN( "TRACE : PUTRING     : %s : %.16llX %.16llX",
                   i_target.toEcmdString(),
                   i_address,
                   i_data.getDoubleWord(0));
    }

    return l_rc;
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
fapi::ReturnCode fapiModifyRing(const fapi::Target& i_target,
                                const scanRingId_t i_address,
                                ecmdDataBufferBase & i_data,
                                const fapi::ChipOpModifyMode i_modifyMode,
                                const uint32_t i_ringMode)
{
    fapi::ReturnCode l_rc;
    bool l_traceit = platIsScanTraceEnabled();

    // call the platform implementation
    l_rc = platModifyRing( i_target, i_address, i_data, i_modifyMode, i_ringMode );

    if (l_rc)
    {
        FAPI_ERR("platModifyRing failed - Target %s, Addr 0x%.16llX,"
                 "ModifyMode 0x%.8X", i_target.toEcmdString(),
                 i_address, i_modifyMode);
    }

    if( l_traceit )
    {
        // get string representation of the modify mode
        const char * l_pMode = NULL;

        if (i_modifyMode == fapi::CHIP_OP_MODIFY_MODE_OR)
        {
            l_pMode = "OR";
        }
        else if (i_modifyMode == fapi::CHIP_OP_MODIFY_MODE_AND)
        {
            l_pMode = "AND";
        }
        else if (i_modifyMode == fapi::CHIP_OP_MODIFY_MODE_XOR)
        {
            l_pMode = "XOR";
        }
        else
        {
            l_pMode = "?";
        }

        FAPI_SCAN( "TRACE : MODRING     : %s : %.16llX %.16llX %s",
               i_target.toEcmdString(),
               i_address,
               i_data.getDoubleWord(0),
               l_pMode);
    }

    return l_rc;
}

// --------------------------------------------------------------------------
// NOTE:
// These spy access interfaces are only used in FSP and cronus.
// HB does not allow spy access

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
#ifdef FAPI_SUPPORT_SPY_AS_ENUM
fapi::ReturnCode _fapiGetSpy(const fapi::Target& i_target,
                            const spyId_t i_spyId,
                            ecmdDataBufferBase & o_data)
{
    fapi::ReturnCode l_rc;
    bool l_traceit = platIsScanTraceEnabled();

    // call the platform implementation
    l_rc = platGetSpy( i_target, i_spyId, o_data );

    if (l_rc)
    {
        FAPI_ERR("fapiGetSpy failed - Target %s, SpyId 0x%.16llX",
                  i_target.toEcmdString(), i_spyId);
    }

    if( l_traceit )
    {
        FAPI_SCAN( "TRACE : GETSPY      : %s : %.16llX %.16llX", 
                   i_target.toEcmdString(),
                   i_spyId,
                   o_data.getDoubleWord(0));
    }

    return l_rc;
}
#endif

#ifdef FAPI_SUPPORT_SPY_AS_STRING
fapi::ReturnCode _fapiGetSpy(const fapi::Target& i_target,
                            const char * i_spyId,
                            ecmdDataBufferBase & o_data)
{
    fapi::ReturnCode l_rc;
    bool l_traceit = platIsScanTraceEnabled();

    // call the platform implementation
    l_rc = platGetSpy( i_target, i_spyId, o_data );

    if (l_rc)
    {
        FAPI_ERR("fapiGetSpy failed - Target %s, SpyId %s",
                  i_target.toEcmdString(), i_spyId);
    }

    if( l_traceit )
    {
        FAPI_SCAN( "TRACE : GETSPY      : %s : %s %.16llX",
                   i_target.toEcmdString(),
                   i_spyId,
                   o_data.getDoubleWord(0));
    }

    return l_rc;
}
#endif

#ifdef FAPI_SUPPORT_SPY_AS_ENUM
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
fapi::ReturnCode _fapiPutSpy(const fapi::Target& i_target,
                            const spyId_t i_spyId,
                            ecmdDataBufferBase & i_data)
{
    fapi::ReturnCode l_rc;
    bool l_traceit = platIsScanTraceEnabled();

    // call the platform implementation
    l_rc = platPutSpy( i_target, i_spyId, i_data );

    if (l_rc)
    {
        FAPI_ERR("fapiPutSpy failed - Target %s, SpyId 0x%.16llX",
                  i_target.toEcmdString(), i_spyId);
    }

    if( l_traceit )
    {
        FAPI_SCAN( "TRACE : PUTSPY      : %s : %.16llX %.16llX",
                   i_target.toEcmdString(),
                   i_spyId,
                   i_data.getDoubleWord(0));
    }

    return l_rc;
}
#endif

#ifdef FAPI_SUPPORT_SPY_AS_STRING
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
fapi::ReturnCode _fapiPutSpy(const fapi::Target& i_target,
                            const char * const i_spyId,
                            ecmdDataBufferBase & i_data)
{
    fapi::ReturnCode l_rc;
    bool l_traceit = platIsScanTraceEnabled();

    // call the platform implementation
    l_rc = platPutSpy( i_target, i_spyId, i_data );

    if (l_rc)
    {
        FAPI_ERR("fapiPutSpy failed - Target %s, SpyId %s.8X",
                  i_target.toEcmdString(), i_spyId);
    }

    if( l_traceit )
    {
        FAPI_SCAN( "TRACE : PUTSPY      : %s : %s %.16llX",
                   i_target.toEcmdString(),
                   i_spyId,
                   i_data.getDoubleWord(0));
    }

    return l_rc;
}
#endif

#ifdef FAPI_SUPPORT_SPY_AS_STRING
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
fapi::ReturnCode _fapiGetSpyImage(const fapi::Target& i_target,
                            const char * const i_spyId,
                            ecmdDataBufferBase & o_data,
                            const ecmdDataBufferBase & i_imageData)
{
    fapi::ReturnCode l_rc;
    bool l_traceit = platIsScanTraceEnabled();

    // call the platform implementation
    l_rc = platGetSpyImage( i_target, i_spyId, o_data, i_imageData );

    if (l_rc)
    {
      FAPI_ERR("fapiGetSpyImage failed - Target %s, SpyId %s",
                i_target.toEcmdString(), i_spyId);
    }

    if( l_traceit )
    {
      FAPI_SCAN( "TRACE : GETSPYIMAGE : %s : %s %.16llX",
                   i_target.toEcmdString(),
                   i_spyId,
                   o_data.getDoubleWord(0));

    }

    return l_rc;
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
fapi::ReturnCode _fapiPutSpyImage(const fapi::Target& i_target,
                                const char * const i_spyId,
                                const ecmdDataBufferBase & i_data,
                                ecmdDataBufferBase & io_imageData)
{
    fapi::ReturnCode l_rc;
    bool l_traceit = platIsScanTraceEnabled();

    // call the platform implementation
   l_rc = platPutSpyImage( i_target, i_spyId, i_data, io_imageData );

    if (l_rc)
    {
      FAPI_ERR("fapiPutSpyImage failed - Target %s, SpyId %s",
               i_target.toEcmdString(), i_spyId);
    }

    if( l_traceit )
    {
        FAPI_SCAN("TRACE : PUTSPYIMG : %s : %s %.16llX",
                   i_target.toEcmdString(),
                   i_spyId,
                   i_data.getDoubleWord(0));

    }

    return l_rc;
}
#endif
#ifdef FAPI_SUPPORT_SPY_AS_ENUM
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
fapi::ReturnCode _fapiGetSpyImage(const fapi::Target& i_target,
                            const spyId_t i_spyId,
                            ecmdDataBufferBase & o_data,
                            const ecmdDataBufferBase & i_imageData)
{

    fapi::ReturnCode l_rc;
    bool l_traceit = platIsScanTraceEnabled();

    // call the platform implementation
    l_rc = platGetSpyImage( i_target, i_spyId, o_data, i_imageData );

    if (l_rc)
    {
        FAPI_ERR("fapiGetSpyImage failed - Target %s, SpyId 0x%.16llX",
                  i_target.toEcmdString(), i_spyId);
    }

    if( l_traceit )
    {
        FAPI_SCAN( "TRACE : GETSPYIMG  : %s : %.16llX %.16llX",
                   i_target.toEcmdString(),
                   i_spyId,
                   o_data.getDoubleWord(0));

    }

    return l_rc;
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
fapi::ReturnCode _fapiPutSpyImage(const fapi::Target& i_target,
                                const spyId_t i_spyId,
                                const ecmdDataBufferBase & i_data,
                                ecmdDataBufferBase & io_imageData)
{
    fapi::ReturnCode l_rc;
    bool l_traceit = platIsScanTraceEnabled();

    // call the platform implementation
    l_rc = platPutSpyImage( i_target, i_spyId, i_data, io_imageData );

    if (l_rc)
    {
      FAPI_ERR("fapiPutSpyImage failed - Target %s, SpyId 0x%.16llX",
                i_target.toEcmdString(), i_spyId);
    }

    if( l_traceit )
    {
        FAPI_SCAN( "TRACE : PUTSPYIMG  : %s : %.16llX %.16llX",
                   i_target.toEcmdString(),
                   i_spyId,
                   i_data.getDoubleWord(0));

    }

    return l_rc;
}
#endif
} // extern "C"
