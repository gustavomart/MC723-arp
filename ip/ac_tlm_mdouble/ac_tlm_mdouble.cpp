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

    fpu_reg = new uint8_t[24];
    for (int i=0; i < 24; i++) fpu_reg[i] = 0;
}

/// Destructor
ac_tlm_mdouble::~ac_tlm_mdouble() {
  delete [] fpu_reg;
}

/** Write the operands and perform calculation after wrote the second
  * Note: Always write 32 bits
  * @param a is the address to write
  * @param d id the data being write
  * @returns A TLM response packet with SUCCESS
*/
ac_tlm_rsp_status ac_tlm_mdouble::writed( const uint32_t &a , const uint32_t &d )
{
  uint32_t a_aux;

  switch (a) {
    case 0: a_aux = 4; break;
    case 4: a_aux = 0; break;
    case 8: a_aux = 12; break;
    case 12: a_aux = 8; break;
  }

  *((uint32_t *) &fpu_reg[a_aux]) = *((uint32_t *) &d);
}

/** Read Result
  * @param a is the address to read
  * @param d id the data that will be read
  * @returns A TLM response packet with SUCCESS and a modified d
*/
ac_tlm_rsp_status ac_tlm_mdouble::readd( const uint32_t &a , uint32_t &d )
{
  int a_aux;

  if (a == 16)
  {
    *((double*) &fpu_reg[16]) = *((double *) &fpu_reg[0]) * (*((double*) &fpu_reg[8]));
  }

  switch (a) {
    case 16: a_aux = 20; break;
    case 20: a_aux = 16; break;
  }

  *((uint32_t *) &d) = *((uint32_t *) &fpu_reg[a_aux]);

  return SUCCESS;
}



