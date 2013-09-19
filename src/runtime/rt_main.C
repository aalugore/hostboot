/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/runtime/rt_main.C $                                       */
/*                                                                        */
/* IBM CONFIDENTIAL                                                       */
/*                                                                        */
/* COPYRIGHT International Business Machines Corp. 2013                   */
/*                                                                        */
/* p1                                                                     */
/*                                                                        */
/* Object Code Only (OCO) source materials                                */
/* Licensed Internal Code Source Materials                                */
/* IBM HostBoot Licensed Internal Code                                    */
/*                                                                        */
/* The source code for this program is not published or otherwise         */
/* divested of its trade secrets, irrespective of what has been           */
/* deposited with the U.S. Copyright Office.                              */
/*                                                                        */
/* Origin: 30                                                             */
/*                                                                        */
/* IBM_PROLOG_END_TAG                                                     */
#include <builtins.h>
#include <limits.h>
#include <runtime/interface.h>
#include <util/singleton.H>
#include <stdio.h>
#include <util/align.H>

// Address of the first writable page, initialized by the linker.
extern void* data_load_address;
// Forward declaration to vfs code.
void vfs_module_init();


/** @fn _main
 *
 *  @brief Entry point called from Sapphire to initialize RT image.
 *
 *  @param[in] intf - Runtime interfaces structure from Sapphire.
 *  @param[in] base - Memory address of the start of the RT image.
 *
 *  @return - runtimeInterfaces_t* - Pointer to RT image's interfaces.
 *
 *  This function is required to be in the .text.intvects section so that
 *  it is placed into the first page (4k) of the image.  When HB IPL
 *  loads the RT image for CxxTest execution it will only mark the first
 *  page executable.  The remainder is marked execute via callbacks from
 *  within _main.
 */
extern "C"
{
    runtimeInterfaces_t* _main(hostInterfaces_t*, uint64_t base)
        SYMB_SECTION(".text.intvects");
}

/** @fn rt_start
 *
 *  @brief Remainder of RT image initializion.
 *
 *  After _main marks pages executable we can leave the first page of the
 *  image.  This function performs the bulk of the initialization:
 *
 *      1) Call C++ constructors for this image.
 *      2) Load all modules in the image.
 *
 *  @param[in] intf - Runtime interfaces structure from Sapphire.
 */
runtimeInterfaces_t* rt_start(hostInterfaces_t*) NEVER_INLINE;

/** Call C++ constructors present in this image. */
void rt_cppBootstrap();

runtimeInterfaces_t* _main(hostInterfaces_t* intf, uint64_t base)
{
    // Ensure remainder of image has text pages marked execute.
    for (uint64_t i = base;
         i < ALIGN_PAGE_DOWN((uint64_t)&data_load_address);
         i += PAGESIZE)
    {
        (intf->set_page_execute)(reinterpret_cast<void*>(i));
    }

    // Tail-recurse to real entry point.
    return rt_start(intf);
}

runtimeInterfaces_t* rt_start(hostInterfaces_t* intf)
{
    (intf->puts)("Starting Runtime Services....\n");

    // Save a pointer to interfaces from Sapphire.
    g_hostInterfaces = intf;

    // Call C++ constructors.
    rt_cppBootstrap();

    // Initialize all modules.
    vfs_module_init();

    // Return our interface pointer structure.
    return getRuntimeInterfaces();
}

void rt_cppBootstrap()
{
    // Call default constructors for any static objects.
    extern void (*ctor_start_address)();
    extern void (*ctor_end_address)();
    void(**ctors)() = &ctor_start_address;
    while(ctors != &ctor_end_address)
    {
        (*ctors)();
        ctors++;
    }
}

hostInterfaces_t* g_hostInterfaces = NULL;

runtimeInterfaces_t* getRuntimeInterfaces()
{
    return &Singleton<runtimeInterfaces_t>::instance();
}