/**
 * @file      ac_tlm_mdouble.cpp
 * @author    Gustavo Solaira
 *
 * @brief     Implements a ac_tlm double multiplier.
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

#include "ac_tlm_mdouble.h"

//////////////////////////////////////////////////////////////////////////////

/// Namespace to isolate lock from ArchC
using user::ac_tlm_mdouble;

/// Constructor
ac_tlm_mdouble::ac_tlm_mdouble( sc_module_name module_name ) :
  sc_module( module_name ),
  target_export("iport")
{
    /// Binds target_export to the double multiplier
    target_export( *this );

    for (int i=0; i < 6; i++)
      fpu_reg[i] = 0;
}

/// Destructor
ac_tlm_mdouble::~ac_tlm_mdouble() {
}

/** Write the operands and perform calculation after wrote the second
  * Note: Always write 32 bits
  * @param a is the address to write
  * @param d id the data being write
  * @returns A TLM response packet with SUCCESS
*/
ac_tlm_rsp_status ac_tlm_mdouble::writed( const uint32_t &a , const uint32_t &d )
{

  printf("addr=%d, data=%d\n", a/4, d);
  // write value
  *((uint32_t *) &fpu_reg[a/4]) = *((uint32_t *) &d);

  // perform calculation
  if (a == 12)
  {
    double reg1=0, reg2=0, result=0;
    // mount first reg
    *((uint32_t*)&reg1) = fpu_reg[1];
    *(((uint32_t*)&reg1)+1) = fpu_reg[0];
    // mount second reg
    *((uint32_t*)&reg2) = fpu_reg[3];
    *(((uint32_t*)&reg2)+1) = fpu_reg[2];

    // calculate result
    printf("%lf %lf\n", reg1, reg2);
    result = reg1 * reg2;
    fpu_reg[4] = *((uint32_t*)&result);
    fpu_reg[5] = *(((uint32_t*)&result)+1);
  }

  //printf("%d %d\n", fpu_reg[4], fpu_reg[5]);

  return SUCCESS;
}

/** Read Result
  * @param a is the address to read
  * @param d id the data that will be read
  * @returns A TLM response packet with SUCCESS and a modified d
*/
ac_tlm_rsp_status ac_tlm_mdouble::readd( const uint32_t &a , uint32_t &d )
{
  if (a != 16 || a != 20)
    return ERROR;  
  
  *((uint32_t *) &d) = fpu_reg[a];

  return SUCCESS;
}



