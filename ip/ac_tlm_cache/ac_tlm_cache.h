/**
 * @file      ac_tlm_cache.h
 * @author    Gustavo Solaira
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

#ifndef AC_TLM_CACHE_H_
#define AC_TLM_CACHE_H_

//////////////////////////////////////////////////////////////////////////////

// Standard includes
// SystemC includes
#include <systemc>
// ArchC includes
#include "ac_tlm_protocol.H"
#include "ac_tlm_port.H"

//////////////////////////////////////////////////////////////////////////////

// using statements
using tlm::tlm_transport_if;

// cache struct
typedef struct {

  uint32_t *blocks;
  // bit menos significativo como 1 e invalido, 0 eh valido
  uint32_t tag;

} Line;

typedef struct {
  
  Line *lines;
  
} Way;

typedef struct {
  
  Way *ways;

} Cache;
  

//////////////////////////////////////////////////////////////////////////////

/// Namespace to isolate memory from ArchC
namespace user
{

/// A TLM router
class ac_tlm_cache :
  public sc_module,
  public ac_tlm_transport_if
{
public:
  /// Exposed port with ArchC interface
  sc_export< ac_tlm_transport_if > target_export;

  ac_tlm_port R_port;

  int b_blocks, b_lines, b_ways, n_blocks, n_lines, n_ways;
  int info_size, tag_size;
  uint32_t mask_tag, mask_line, mask_block;

  unsigned long int hit, miss;

  char type;

  int round_robin;

  /// Internal write
  ac_tlm_rsp_status write( const ac_tlm_req & );
  /// Internal read
  ac_tlm_rsp read( const ac_tlm_req & );

  /**
   * Implementation of TLM transport method that
   * handle packets of the protocol doing apropriate actions.
   * This method must be implemented (required by SystemC TLM).
   * @param request is a received request packet
   * @return A response packet to be send
  */
  ac_tlm_rsp transport( const ac_tlm_req &request ) {

    ac_tlm_rsp response;

    switch( request.type ) {
    case READ :     // Packet is a READ one
      #ifdef DEBUG  // Turn it on to print transport level messages
    cout << "Transport READ at 0x" << hex << request.addr << " value ";
    cout << response.data << endl;
      #endif
      response = read( request );
      break;
    case WRITE:     // Packet is a WRITE
      #ifdef DEBUG
    cout << "Transport WRITE at 0x" << hex << request.addr << " value ";
    cout << request.data << endl;
      #endif
      response.status = write( request );
      break;
    default :
      response.status = ERROR;
      break;
    }

    return response;
  }


  /**
   * Default constructor.
   *
   */
  ac_tlm_cache( sc_module_name module_name, int s_block, int n_lines, int n_ways, char type );

  /**
   * Default destructor.
   */
  ~ac_tlm_cache();

private:
  Cache cache;

};

};

#endif //AC_TLM_ROUTER_H_
