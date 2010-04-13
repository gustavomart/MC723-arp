/**
 * @file      ac_tlm_mem.cpp
 * @author    Bruno de Carvalho Albertini
 *
 * @author    The ArchC Team
 *            http://www.archc.org/
 *
 *            Computer Systems Laboratory (LSC)
 *            IC-UNICAMP
 *            http://www.lsc.ic.unicamp.br/
 *
 * @version   0.1
 * @date      Sun, 02 Apr 2006 08:07:46 -0200
 *
 * @brief     Implements a ac_tlm memory.
 *
 * @attention Copyright (C) 2002-2005 --- The ArchC Team
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License as published by the Free Software Foundation; either
 *   version 2.1 of the License, or (at your option) any later version.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Lesser General Public License for more details.
 *
 *
 */

//////////////////////////////////////////////////////////////////////////////
// Standard includes
// SystemC includes
// ArchC includes

#include "ac_tlm_router.h"

//////////////////////////////////////////////////////////////////////////////

/// Namespace to isolate memory from ArchC
using user::ac_tlm_router;

/// Constructor
ac_tlm_router::ac_tlm_router( sc_module_name module_name , int k ) :
  sc_module( module_name ),
  target_export("iport")
{
    /// Binds target_export to the router
    target_export( *this );

}

/// Destructor
ac_tlm_router::~ac_tlm_router() {
}

/** Route the request to one of the peripherals
  * @param request is the request received
  * @returns A TLM response packet with SUCCESS
*/
ac_tlm_rsp_status ac_tlm_router::route( ac_tlm_req &request )
{
  /*if (a & MEM_BASE)
  {*/
    // Route to mem
    return R_port_mem.transport( request.addr, request.data );
  /*}
  else if (a & LOCK_BASE)
  {
    // Route to Read&Inc register
    return R_port_lock.write( request.addr & MASK_BASE, request.data );
  }
  else if (a & FPU_BASE)
  { 
    // Route to FPU
    return R_port_fpu.write( request.addr & MASK_BASE, request.data );
  }
  else
  {
    return ERROR;
  }*/
}



