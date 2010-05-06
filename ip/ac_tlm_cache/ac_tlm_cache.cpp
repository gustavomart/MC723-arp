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
ac_tlm_cache::ac_tlm_cache( sc_module_name module_name, int s_block, int n_lines, int n_ways ) :
  sc_module( module_name ),
  target_export("iport"),
  R_port("R_port", 5242880U)
{
    int i, j;

    /// Binds target_export to the cache
    target_export( *this );

    /// Initialize cache vector
    // size of the block in bytes (s = 2^n)
    // receives in words and changes to number of bytes
    this.s_block = s_block+2;
    // number of lines in bits (s = 2^n)
    this.n_lines = n_lines;
    // number of ways in bits (s = 2^n)
    this.n_ways = n_ways;

    Cache.ways = new Way[n_ways];

    for (i=0; i < n_ways; i++)
    {
      Cache.ways[i].lines = new Line[n_lines];

      for (j=0; j < n_lines; j++)
      {
        Cache.ways[i].lines[j].blocks = new uint_8[4*s_block];
        Cache.ways[i].lines[j].valid = false;
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
       delete [] Cache.ways[i].lines[j].block;
     }

     delete [] Cache.ways[i].lines;
   }

   delete Cache.ways;
}

/** Cache Write
  * Note: Always write 32 bits
  * @param a is the address to write
  * @param d id the data being write
  * @returns A TLM response packet with SUCCESS
*/
ac_tlm_rsp_status ac_tlm_mem::write( const uint32_t &a , const uint32_t &d )
{
  ac_tlm_request req_aux;
  req_aux.addr = a;
  req_aux.data = d;

  uint32_t addr = a;

  int info_size = s_block+n_lines;
  int tag_size = 32 - info_size;

  // find tag maintain in left side
  int tag = addr & (~0x0 << info_size);

  // find line
  int line = (addr & (~0x0 >> tag_size)) >> s_block;

  // find position in block (indexed by bytes)
  int b_pos = addr & (~0x0 >> (32 - s_block);

  // USAR FOR E PULAR CARA Q VAI ESCREVER, AI NAO PRECISA DO IF

  // check if the block is one word
  if (s_block == 1)
  {
    // only modify cache and write to memory
    


  }
  else
  {
    // read the block, modify and write to memory


  }

  *((uint32_t *) &memory[a]) = *((uint32_t *) &d);
  return SUCCESS;
}

/** Cache Read
  * Note: Always read 32 bits
  * @param a is the address to read
  * @param d id the data that will be read
  * @returns A TLM response packet with SUCCESS and a modified d
*/
ac_tlm_rsp_status ac_tlm_mem::read( const uint32_t &a , uint32_t &d )
{
  ac_tlm_request req_aux;
  req_aux.addr = a;
  req_aux.data = d;


  *((uint32_t *) &d) = *((uint32_t *) &memory[a]);

  return SUCCESS;
}



