/**
 * @file      ac_tlm_router.cpp
 * @author    Gustavo Solaira
 *
 * @brief     Implements a ac_tlm router.
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

/// Namespace to isolate lock from ArchC
using user::ac_tlm_router;

/// Constructor
ac_tlm_router::ac_tlm_router( sc_module_name module_name ) :
  sc_module( module_name ),
  target_export("iport"),
  R_port_mem("R_port_mem", 5242880U),
  R_port_lock("R_port_lock", 32U),
  R_port_mdouble("R_port_mdouble", 1024U),
  R_port_adouble("R_port_adouble", 1024U)
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
ac_tlm_rsp ac_tlm_router::route( const ac_tlm_req &request )
{
  if (request.addr < LOCK_BASE)
  {
    // Route to mem
    return R_port_mem->transport( request );
  }
  else if (request.addr < MDOUBLE_BASE)
  {
    // Route to Read&Inc register
    ac_tlm_req req_aux = request;
    req_aux.addr -= LOCK_BASE;
    return R_port_lock->transport( request );
  }
  else if (request.addr < ADOUBLE_BASE)
  { 
    // Route to MDOUBLE
    ac_tlm_req req_aux = request;
    req_aux.addr -= MDOUBLE_BASE;
    return R_port_mdouble->transport( req_aux );
  } 
  else
  {
    ac_tlm_req req_aux = request;
    req_aux.addr -= ADOUBLE_BASE;
    return R_port_adouble->transport ( req_aux );
  }
  
}



