/**
 * @file      ac_tlm_cache.cpp
 * @author    Gustavo Solaira
 *
 * @brief     Implements a ac_tlm cache.
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

#include "ac_tlm_cache.h"

#include <math.h>

//////////////////////////////////////////////////////////////////////////////

/// Namespace to isolate cache from ArchC
using user::ac_tlm_cache;

/// Constructor
ac_tlm_cache::ac_tlm_cache( sc_module_name module_name, int bb, int bl, int bw ) :
  sc_module( module_name ),
  target_export("iport"),
  R_port("R_port", 5242880U)
{
    // variable that says which way will be filled
    round_robin = 0;

    int i, j;

    /// Binds target_export to the cache
    target_export( *this );

    // number of words in bits (s = 2^n)
    b_blocks = bb;
    n_blocks = 0x1 << bb;
    // number of lines in bits (s = 2^n)
    b_lines = bl;
    n_lines = 0x1 << bl;
    // number of ways in bits (s = 2^n)
    b_ways = bw;
    n_ways = 0x1 << bw;

    // initialize fields size info    
    info_size = b_blocks+b_lines+2;
    tag_size = 32-info_size;

    // initialize masks
    mask_tag = (uint32_t)~0x0 << info_size;
    mask_line = (uint32_t)~0x0 >> tag_size;
    mask_block = (uint32_t)~0x0 >> (32 - b_blocks - 2);

    // Initialize cache structures
    cache.ways = new Way[n_ways];
    for (i=0; i < n_ways; i++)
    {
      cache.ways[i].lines = new Line[n_lines];

      for (j=0; j < n_lines; j++)
      {
        cache.ways[i].lines[j].blocks = new uint32_t[n_blocks];
        cache.ways[i].lines[j].tag = 0x1;
      }
    }

}

/// Destructor
ac_tlm_cache::~ac_tlm_cache() {

  int i, j;

   for (i=0; i < n_ways; i++)
   {
     for (j=0; j < n_lines; j++)
     {
       delete [] cache.ways[i].lines[j].blocks;
     }

     delete [] cache.ways[i].lines;
   }

   delete [] cache.ways;
}

/** Cache Write
  * It is a write-through cache
  * Note: Always write 32 bits
  * @param request
  * @returns A TLM response packet with SUCCESS
*/
ac_tlm_rsp_status ac_tlm_cache::write( const ac_tlm_req &request )
{
  int i;
  bool found = false;
  ac_tlm_req req_read;
  ac_tlm_rsp rsp_read, response;
  uint32_t addr = request.addr;

  // find tag maintain in left side
  int tag = addr & mask_tag;
  // find line
  uint32_t l = (addr & mask_line) >> (b_blocks+2);
  // find position in block (indexed by words)
  uint32_t b = (addr & mask_block) >> 2;

  // only modify cache and write to memory
  for (i=0; i < n_ways; i++)
  {
    if (cache.ways[i].lines[l].tag == tag)
    {
      cache.ways[i].lines[l].blocks[b] = *((uint32_t *) &request.data);
      // redirect to router
      response = R_port->transport( request );
      found = true;
      //printf("FOUND addr=0x%x tag=0x%x l=0x%x b=0x%x\n", addr, tag, l, b);
    }
  }

  if (! found)
  {
    // put in the cache
    cache.ways[round_robin].lines[l].tag = tag;
    req_read.type = READ;      
    //printf("NOT FOUND addr=0x%x tag=0x%x l=0x%x b=0x%x\n", addr, tag, l, b);
    
    // read whole block to cache     
    for (i=0; i < n_blocks; i++)
    {
      req_read.addr = tag | (l << (info_size-1)) | (b << (b_blocks+1));
      rsp_read = R_port->transport( req_read );
      cache.ways[round_robin].lines[l].blocks[b] = rsp_read.data;
    }

    // modify value
    cache.ways[round_robin].lines[l].blocks[b] = *((uint32_t *) &request.data);
    // redirect to router
    response = R_port->transport( request );
    round_robin = (round_robin+1) % n_ways;
  }

  return response.status;
}

/** Cache Read
  * Note: Always read 32 bits
  * @param request
  * @returns A TLM response packet with SUCCESS and a modified data
*/
ac_tlm_rsp ac_tlm_cache::read( const ac_tlm_req &request )
{
  int i;
  ac_tlm_rsp rsp_read, response;
  uint32_t addr = request.addr;
  ac_tlm_req req_read;
  bool found = false;

  // find tag maintain in left side
  int tag = addr & mask_tag;
  // find line
  uint32_t l = (addr & mask_line) >> (b_blocks+2);
  // find position in block (indexed by words)
  uint32_t b = (addr & mask_block) >> 2;

  // tries to find the data in the cache
  for (i=0; i < n_ways; i++)
  {
    if (cache.ways[i].lines[l].tag == tag)
    {
      *((uint32_t *) &response.data) = *((uint32_t *) &cache.ways[i].lines[l].blocks[b]);
      found = true;
      printf("FOUND addr=0x%x tag=0x%x l=0x%x b=0x%x\n", addr, tag, l, b);
    }
  }

  if (! found)
  {
    // put in the cache
    cache.ways[round_robin].lines[l].tag = tag;
    req_read.type = READ;      
    printf("NOT FOUND addr=0x%x tag=0x%x l=0x%x b=0x%x\n", addr, tag, l, b);
    
    // read whole block to cache     
    for (i=0; i < n_blocks; i++)
    {
      req_read.addr = tag | (l << (info_size-1)) | (b << (b_blocks+1));
      rsp_read = R_port->transport( req_read );
      cache.ways[round_robin].lines[l].blocks[b] = rsp_read.data;
    }

    // redirect to router
    *((uint32_t *) &response.data) = *((uint32_t *) &cache.ways[round_robin].lines[l].blocks[b]);
    round_robin = (round_robin+1) % n_ways;
  }
  
  return response;
}



