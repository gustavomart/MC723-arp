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
    round_robin = 0;

    int i, j;

    /// Binds target_export to the cache
    target_export( *this );

    /// Initialize cache vector
    // number of words in bits (s = 2^n)
    b_blocks = bb;
    n_blocks = 0x1 << bb;
    // number of lines in bits (s = 2^n)
    b_lines = bl;
    n_lines = 0x1 << bl;
    // number of ways in bits (s = 2^n)
    b_ways = bw;
    n_ways = 0x1 << bw;


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
  * @param a is the address to write
  * @param d id the data being write
  * @returns A TLM response packet with SUCCESS
*/
ac_tlm_rsp_status ac_tlm_cache::write( const uint32_t &a , const uint32_t &d )
{
  int i;
  bool found = false;
  ac_tlm_req req_aux, req_read;
  ac_tlm_rsp rsp_read;
  req_aux.addr = a;
  req_aux.data = d;
  req_aux.type = WRITE;
  uint32_t addr = a;

  int info_size = b_blocks+b_lines+2;
  int tag_size = 32-info_size;

  // find tag maintain in left side
  int tag = addr & ((uint32_t)~0x0 << info_size);
  // find line
  int l = (addr & ((uint32_t)~0x0 >> tag_size)) >> (b_blocks+2);
  // find position in block (indexed by words)
  int b = (addr & ((uint32_t)~0x0 >> (32 - b_blocks - 2))) >> 2;

  //printf("addr=0x%x tag=0x%x l=0x%x b=0x%x\n", addr, tag, l, b);

  // only modify cache and write to memory
  for (i=0; i < n_ways; i++)
  {
    if (cache.ways[i].lines[l].tag == tag)
    {
      cache.ways[i].lines[l].blocks[b] = *((uint32_t *) &d);
      // redirect to router
      R_port->transport( req_aux );
      found = true;
    }
  }

  if (! found)
  {
    //printf("addr=0x%x tag=0x%x l=0x%x b=0x%x\n", addr, tag, l, b);

    // put in the cache
    cache.ways[round_robin].lines[l].tag = tag;
    // read whole block to cache     
    req_read.type = READ;      
    
    for (i=0; i < n_blocks; i++)
    {
      req_read.addr = tag | (l << info_size-1) | (b << (b_blocks+2-1));
      rsp_read = R_port->transport( req_read );
//printf("addr=0x%x tag=0x%x l=0x%x b=0x%x\n", addr, tag, l, b);
      cache.ways[i].lines[l].blocks[b] = rsp_read.data;
    }
    // modify value
    cache.ways[round_robin].lines[l].blocks[b] = *((uint32_t *) &d);
    // redirect to router
    R_port->transport( req_aux );
    round_robin = (round_robin+1) % n_ways;
  }

  return SUCCESS;
}

/** Cache Read
  * Note: Always read 32 bits
  * @param a is the address to read
  * @param d id the data that will be read
  * @returns A TLM response packet with SUCCESS and a modified d
*/
ac_tlm_rsp_status ac_tlm_cache::read( const uint32_t &a , uint32_t &d )
{
  ac_tlm_req req_aux;
  req_aux.addr = a;
  req_aux.data = d;

  return SUCCESS;
}



